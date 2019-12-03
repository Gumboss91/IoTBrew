import socket
import logging
import json
import datetime
from coapthon.client.helperclient import HelperClient as CoapClient
import paho.mqtt.client as mqtt
from influxdb import InfluxDBClient

logging.disable(logging.DEBUG)

# 6LoPaWAN
IP_6LBR = "bbbb::100"
PORT_6LBR = 3000

# Sensor Settings
COAP_PORT = 5683
COAP_TIMEOUT=5
PORT_DISCOVERY = 3001
SENSOR_ONTIME=30
SENSOR_OFFTIME=60*4

# Influxdb
INFLUXDB_ADDRESS = 'localhost'
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
    resp = coapclient.get(".well-known/core", timeout=COAP_TIMEOUT)
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
    coapclient.stop()
    coapclient.close()
    del(coapclient)
    return ressources

def configureSleep(devaddr):
    print("Configure sleep")
    coapclient = CoapClient(server=(devaddr, COAP_PORT))
    resp = coapclient.get("very_sleepy_config", timeout=COAP_TIMEOUT)
    resp = coapclient.post("very_sleepy_config", "mode=1&interval="+str(SENSOR_OFFTIME)+"&duration="+str(SENSOR_ONTIME), timeout=COAP_TIMEOUT)
    coapclient.stop()
    coapclient.close()
    del(coapclient)

# Influxdb
def init_influxdb_database(influxdb_client):
    databases = influxdb_client.get_list_database()
    if len(list(filter(lambda x: x['name'] == INFLUXDB_DATABASE, databases))) == 0:
        influxdb_client.create_database(INFLUXDB_DATABASE)
    influxdb_client.switch_database(INFLUXDB_DATABASE)

def influxdb_sendSensorData(influxdb_client, sensor, data):
    jsondata = json.loads(data)
    for value_name in jsondata:
        json_body = [
            {
                'measurement': value_name,
                'tags': {
                    'location': sensor
                },
                'fields': {
                    'value': jsondata[value_name]["v"],
                    'unit': jsondata[value_name]["u"],
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

client = None
influxdb_client = None

if USE_MQTT:
    client = mqtt.Client()
    client.on_connect = mqtt_on_connect
    client.on_disconnect = mqtt_on_disconnect
    client.loop_start()

while True:
    if not mqtt_connected and USE_MQTT:
        print("Reconnecting MQTT")
        client.connect(MQTT_BROCKER)

    if not influxdb_connected and USE_INFLUXDB:
        print("Connect to Influxdb")
        influxdb_client = InfluxDBClient(INFLUXDB_ADDRESS, 8086, INFLUXDB_USER, INFLUXDB_PASSWORD, None)
        init_influxdb_database(influxdb_client)
        influxdb_connected = True

    print(datetime.datetime.now())
    print("Waiting for data")
    message, address = server_socket.recvfrom(1024)
    print("UDP received, starting coap", address[0])
    caophost = address[0]

    if(caophost not in sensor_res_cache or len(sensor_res_cache[caophost]) < 1):
        sensor_res_cache[caophost] = getRessources(caophost)
        configureSleep(caophost)
        
    print("Cached", sensor_res_cache[caophost])
    for url in sensor_res_cache[caophost]:
        coapclient = CoapClient(server=(caophost, COAP_PORT))
        response = coapclient.get(url, timeout=COAP_TIMEOUT)
        if(response):
            if influxdb_connected:
                influxdb_sendSensorData(influxdb_client, caophost, response.payload)
                print("Store:", response.payload)
            if mqtt_connected:
                client.publish("6lopawan/sensor/" + caophost + "/" + url, response.payload)
                print("Publish:", response.payload)
        else:
            print("Coap Timeout")
        coapclient.stop()
        coapclient.close()
        del(coapclient)