import socket
import logging
import json
from coapthon.client.helperclient import HelperClient as CoapClient
import paho.mqtt.client as mqtt

logging.disable(logging.DEBUG)

urls = [
    "sen/readings/temp",
    "sen/readings/batt",
    "sen/readings/press",
    "sen/readings/hum",
    "sen/readings/light"
]

IP_6LBR = "bbbb::100"
MQTT_BROCKER = "127.0.0.1"
PORT_6LBR = 3000
COAP_PORT = 5683
PORT_DISCOVERY = 3001

SENSOR_ONTIME=11
SENSOR_OFFTIME=60

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

while True:
    if not mqtt_connected:
        print("Reconnecting MQTT")
        client.connect(MQTT_BROCKER)

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
            if mqtt_connected:
                elem = response.payload
                client.publish("6lopawan/sensor/" + caophost + "/" + url, elem)
                print("Publish:", elem)
            else:
                print("No MQTT connection", mqtt_connected)
        else:
            print("Coap Timeout")
        coapclient.stop()