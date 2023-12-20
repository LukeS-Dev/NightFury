import socket

class ESP32_Socket_Interface():

    CONNECTED       = "connected"
    DISCONNECTED    = "disconnected"

    def __init__(self, ip_address:str, port:int):
        self.ip_address             = ip_address
        self.port                   = port
        self.socket                 = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self._connection_status     = ESP32_Socket_Interface.DISCONNECTED
        
        self.connect_socket()

    def get_socket_interface(self):
        return self
    
    def connect_socket(self):
        if (self.socket.connect_ex((self.ip_address,self.port)) == 0):
            self._connection_status = ESP32_Socket_Interface.CONNECTED
        else:
            self._connection_status = ESP32_Socket_Interface.DISCONNECTED

    def check_connection_status(self):
        return self._connection_status
    
    def socket_send(self,message:str):
        self.socket.send(message.encode())

# TODO: Protocol has not been defined yet as of 20/12/2023
class ESP32_Protocol(): 
    def __init__(self):
        pass

# TODO: Serial interface has not been defined yet as of 20/12/2023
class ESP32_Serial_Interface():
    def __init__(self):
        pass

