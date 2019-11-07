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
IP_MULTICAST = "ff02::1"
PORT_6LBR = 3000

COAP_PORT = 5683
PORT_DISCOVERY = 3001

MQTT_BROCKER = "127.0.0.1"

mqtt_connected = False

deviselist = []

def sensor_callback(response):
    print("Sensor callback", response.payload)

# MQTT functions
def mqtt_on_connect(client, userdata, flags, rc):
    global mqtt_connected
    mqtt_connected = True
    print("MQTT Connected")

def mqtt_on_disconnect(client, userdata, flags, rc):
    mqtt_connected = False
    print("MQTT Disconnected")

def observeSensor(devaddr, sensors):
    tokens = list()
    for url in sensors:
        coapclient = CoapClient(server=(devaddr, COAP_PORT))
        resp = coapclient.get(".well-known/core")
        tokens.append(coapclient.observe(url, sensor_callback))
    return tokens

def connectCoap(devaddr):
    coapclient = CoapClient(server=(devaddr, COAP_PORT))
    resp = coapclient.get(".well-known/core")
    data = resp.payload

    elems = data.split(",")

    sensors = []
    for elem in elems:
        props = elem.split(";")
        observable = False
        for prop in props:
            if prop == "obs":
                observable = True
                break

        if observable:
            url = props[0][1:-1]
            sensors.append(url)
    print(devaddr, sensors)
    coapclient.close()

    observed = observeSensor(devaddr, sensors)
    return {"dev": devaddr, "response": resp, "observed": observed}

def parseDevice(devaddr, newdevices):
    global deviselist
    newdevices.append(devaddr)

    found = False
    for key in deviselist:
        dev = deviselist[key]
        if(dev["dev"] == devaddr):
            found = True
            break

    if(not found):
        print("New sensor device", devaddr)   
        elem = connectCoap(devaddr)
        deviselist.append(elem)

    return newdevices

# HTTP parse devices
def parseDeviceWebsite():
    global deviselist
    fp = urllib.request.urlopen("http://[" + IP_6LBR + "]/network.html")
    mybytes = fp.read()

    mystr = mybytes.decode("utf8")
    fp.close()

    bs = BeautifulSoup(mystr, 'html.parser')

    newdevices = []
    # Find new Sensors
    print("Parse 6lbr")
    pres = bs.findAll('pre', recursive=True)
    for row in pres:
        cells = row.findAll('a')
        if len(cells) > 0:
            prev = row.previous_sibling
            if str(prev) == "<h2>Routes</h2>":
                print("Found", cells[0])
                matches = re.match("<a href=\"route-rm\?(.*)\"", str(cells[0]))
                if(matches):     
                    devaddr = matches.group(1)
                    newdevices = parseDevice(devaddr, newdevices)

    # Delete old Sensors
    for key, dnu in enumerate(deviselist[:]):
        found = False
        for devaddr in newdevices:
            if(deviselist[key]["dev"] == devaddr):
                found = True
                break
        
        if(not found):
            print("Sensor device lost", deviselist[key]["dev"])
            deviselist[key]["coap"].cancel_observing(deviselist[key]["coap_response"], True)
            deviselist.remove(key)

def closeConnections():
    global deviselist
    for dev in deviselist:
        dev["coap"].close()


def thread_function(name):
    while True:
        parseDeviceWebsite()
        time.sleep(20)

parseDeviceWebsite()
closeConnections()
exit()

# Start Sensor Discovery
#thread = threading.Thread(target=thread_function, args=(1,), daemon=True)

#client = mqtt.Client()
#client.on_connect = mqtt_on_connect
#client.on_disconnect = mqtt_on_disconnect

#client.loop_start()

ttl = struct.pack('b', 100)
server_socket = socket.socket(socket.AF_INET6, socket.SOCK_DGRAM)
server_socket.setsockopt(socket.IPPROTO_IP, socket.IP_MULTICAST_TTL, ttl)
server_socket.sendto(bytes("ffff\n\n", "utf-8"), (IP_MULTICAST, PORT_DISCOVERY))

exit()
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