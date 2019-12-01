import socket
import logging
import json
from coapthon.client.helperclient import HelperClient as CoapClient
import paho.mqtt.client as mqtt
from influxdb import InfluxDBClient

logging.disable(logging.DEBUG)

# 6LoPaWAN
IP_6LBR = "bbbb::100"
PORT_6LBR = 3000

# Sensor Settings
COAP_PORT = 5683
PORT_DISCOVERY = 3001
SENSOR_ONTIME=11
SENSOR_OFFTIME=60*4

# Influxdb
INFLUXDB_ADDRESS = '127.0.0.1'
INFLUXDB_USER = 'root'
INFLUXDB_PASSWORD = 'root'
INFLUXDB_DATABASE = 'home_db'

# MQTT
MQTT_BROCKER = "127.0.0.1"

# Usage of data
USE_INFLUXDB = True
USE_MQTT=False

# Normal stuff
influxdb_connected = False
mqtt_connected = False
sensor_res_cache = {}

# MQTT functions
def mqtt_on_connect(client, userdata, flags, rc):
    global mqtt_connected
    mqtt_connected = True
    print("MQTT Connected")

def mqtt_on_disconnect(client, userdata, flags, rc):
    mqtt_connected = False
    print("MQTT Disconnected")

# Sensor Functions
def getRessources(devaddr):
    coapclient = CoapClient(server=(devaddr, COAP_PORT))
    print("Discover Coap", devaddr)
    resp = coapclient.get(".well-known/core", timeout=20)
    ressources = []
    if(resp):
        data = resp.payload

        elems = data.split(",")    
        for elem in elems:
            props = elem.split(";")
            observable = False
            for prop in props:
                if prop == "obs":
                    observable = True
                    break

            if observable:
                url = props[0][1:-1]
                ressources.append(url)
        coapclient.close()
    return ressources

def configureSleep(devaddr):
    coapclient = CoapClient(server=(devaddr, COAP_PORT))
    resp = coapclient.get("very_sleepy_config", timeout=20)
    print("Received", resp.pretty_print(), resp, dir(resp))
    resp = coapclient.post("very_sleepy_config", "mode=1&interval="+str(SENSOR_OFFTIME)+"&duration="+str(SENSOR_ONTIME))
    print("Received", resp.pretty_print(), resp, dir(resp))
    coapclient.close()

# Influxdb
def init_influxdb_database():
    databases = influxdb_client.get_list_database()
    if len(list(filter(lambda x: x['name'] == INFLUXDB_DATABASE, databases))) == 0:
        influxdb_client.create_database(INFLUXDB_DATABASE)
    influxdb_client.switch_database(INFLUXDB_DATABASE)

def influxdb_sendSensorData(influxdb_client, sensor, data):
    for value_name in data:
        json_body = [
            {
                'measurement': value_name,
                'tags': {
                    'location': sensor
                },
                'fields': {
                    'value': data[value_name]["v"],
                    'unit': data[value_name]["u"],
                }
            }
        ]
        influxdb_client.write_points(json_body)

# Announce our route to 6lbr
client_socket = socket.socket(socket.AF_INET6, socket.SOCK_DGRAM) # UDP
client_socket.sendto(bytes("1\n\n", "utf-8"), (IP_6LBR, PORT_6LBR))

# UDP
server_socket = socket.socket(socket.AF_INET6, socket.SOCK_DGRAM)
server_socket.bind(('', PORT_DISCOVERY))

client = mqtt.Client()
client.on_connect = mqtt_on_connect
client.on_disconnect = mqtt_on_disconnect
client.loop_start()

influxdb_client = None

while True:
    if not mqtt_connected and USE_MQTT:
        print("Reconnecting MQTT")
        client.connect(MQTT_BROCKER)

    if not influxdb_connected and USE_INFLUXDB:
        print("Connect to Influxdb")
        influxdb_client = InfluxDBClient(INFLUXDB_ADDRESS, 8086, INFLUXDB_USER, INFLUXDB_PASSWORD, None)
        init_influxdb_database()
        influxdb_connected = True

    print("Waiting for data")
    message, address = server_socket.recvfrom(1024)
    print("UDP received, starting coap", address[0])
    caophost = address[0]

    if(caophost not in sensor_res_cache):
        sensor_res_cache[caophost] = getRessources(caophost)
        configureSleep(caophost)
        
    print("Cached", sensor_res_cache[caophost])
    for url in sensor_res_cache[caophost]:
        coapclient = CoapClient(server=(caophost, COAP_PORT))
        response = coapclient.get(url, timeout=20)
        if(response):
            #print("Received", response.pretty_print())
            if influxdb_connected:
                influxdb_sendSensorData(influxdb_client, caophost, response.payload)
                print("Store:", response.payload)
            if mqtt_connected:
                client.publish("6lopawan/sensor/" + caophost + "/" + url, response.payload)
                print("Publish:", response.payload)
            else:
                print("No MQTT connection", mqtt_connected)
        else:
            print("Coap Timeout")
        coapclient.stop()