import socket
import logging
from coapthon.client.helperclient import HelperClient as CoapClient

logging.disable(logging.DEBUG)

path ="sens/temp"
COAP_PORT = 5683

LOCALPORT = 3001

server_socket = socket.socket(socket.AF_INET6, socket.SOCK_DGRAM)
server_socket.bind(('', LOCALPORT))

while True:
    message, address = server_socket.recvfrom(1024)
    print("UDP received, starting coap", address)
    caophost = address[0]
    coapclient = CoapClient(server=(caophost, COAP_PORT))
    response = coapclient.get(path, timeout=20)
    coapclient.stop()
    print("Received", response.pretty_print())