import socket
import logging
import json
import struct
import re
from coapthon.client.helperclient import HelperClient as CoapClient
import paho.mqtt.client as mqtt
import urllib.request
from bs4 import BeautifulSoup
import threading
import time

logging.disable(logging.DEBUG)

urls = [
    "sen/readings/temp",
    "sen/readings/batt",
    "sen/readings/press",
    "sen/readings/hum",
    "sen/readings/light"
]

IP_6LBR = "bbbb::100"
IP_LAN = "bbbb::200"
IP_MULTICAST = "ff01::1"
PORT_6LBR = 3000

COAP_PORT = 5683
PORT_DISCOVERY = 3001

MQTT_BROCKER = "127.0.0.1"

mqtt_connected = False

deviselist = []

# MQTT functions
def mqtt_on_connect(client, userdata, flags, rc):
    global mqtt_connected
    mqtt_connected = True
    print("MQTT Connected")

def mqtt_on_disconnect(client, userdata, flags, rc):
    mqtt_connected = False
    print("MQTT Disconnected")

#client = mqtt.Client()
#client.on_connect = mqtt_on_connect
#client.on_disconnect = mqtt_on_disconnect

#client.loop_start()

ttl = struct.pack('b', 100)
server_socket = socket.socket(socket.AF_INET6, socket.SOCK_DGRAM)
server_socket.setsockopt(socket.IPPROTO_IP, socket.IP_MULTICAST_TTL, ttl)
server_socket.sendto(bytes("ffff\n\n", "utf-8"), (IP_MULTICAST, PORT_DISCOVERY))
while True:
    #if not mqtt_connected:
    #    client.connect(MQTT_BROCKER)


    print("Waiting for data")
    message, address = server_socket.recvfrom(1024)
    print("UDP received, starting coap", address[0])
    caophost = address[0]

    #for url in urls:
    #    coapclient = CoapClient(server=(caophost, COAP_PORT))
    #    response = coapclient.get(url, timeout=20)
    #    if(response):
    #        print("Received", response.pretty_print())
    #        if mqtt_connected:
    #            elem = response.payload
    #            client.publish("6lopawan/sensor/" + caophost + "/" + url, elem)
    #            print("Publish:", elem)
    #        else:
    #            print("No MQTT connection", mqtt_connected)
    #    else:
    #        print("Coap Timeout")
    #    coapclient.stop()