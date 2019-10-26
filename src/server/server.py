import socket

LOCALPORT = 3001

server_socket = socket.socket(socket.AF_INET6, socket.SOCK_DGRAM)
server_socket.bind(('', LOCALPORT))

while True:
    message, address = server_socket.recvfrom(1024)
    print("Received", message, address)