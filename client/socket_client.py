import socket
import struct
import netifaces



class SocketClient:

    def __init__(self, server_ip=None, server_port=1234):
        if server_ip is None:
            gtws = netifaces.gateways()
            server_ip = gtws['default'][2][0]
        self.server_ip = server_ip
        self.server_port = server_port


    def setup_socket(self, port=1234):
        client_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        client_socket.bind(('0.0.0.0', port))
        client_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 0);
        self.client_socket = client_socket
        return self.client_socket


    def send_signal(self, signal):
        signal_byte = struct.pack('b', signal)
        return self.client_socket.sendto(signal_byte, (self.server_ip, self.server_port))


    def close_socket(self):
        self.client_socket.close()

    
    def __del__(self):
        self.close_socket()