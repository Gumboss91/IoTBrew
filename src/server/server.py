import socket
from coapthon.client.helperclient import HelperClient as CoapClient

path ="sens/temp"
COAP_PORT = 5683

LOCALPORT = 3001

server_socket = socket.socket(socket.AF_INET6, socket.SOCK_DGRAM)
server_socket.bind(('', LOCALPORT))

while True:
    message, address = server_socket.recvfrom(1024)
    caophost = address[0]
    client = CoapClient(server=(caophost, COAP_PORT))
    response = client.get(path)
    client.stop()
    print("Received", response, address)