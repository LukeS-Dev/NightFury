from esp_interface import ESP32_Serial_Interface, ESP32_Protocol, ESP32_Socket_Interface
import config

# Singular Node for ESP32
class ESP32_Node():

    def __init__(self,ip_address,port):
        self.socket = ESP32_Socket_Interface(ip_address,port)    

if __name__ == "__main__":
    ESP_Node = ESP32_Node(config.DEFAULT_IP,config.DEFAULT_PORT)

    if (ESP_Node.socket.check_connection_status() == ESP32_Socket_Interface.CONNECTED): 
        print("Connection Success")
        ESP_Node.socket.socket_send("Hello Server!")
    else:
        print("Connection Failed")