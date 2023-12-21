""" Simple TCP listening server for the ESP32 or 
    ESP32 Simulator.
"""

import socket
import time
import config

# Create a TCP/IP socket
server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_address = (config.DEFAULT_IP, config.DEFAULT_PORT)
server.bind(server_address)
server.listen(5)
server.settimeout(0.5)

print(f"Server Starting on {server_address[0]}:{server_address[1]}")

try:
    while True:
        connection = None
        time.sleep(0.5)
        try:
            connection,addr = server.accept()
            from_client = ''
            received_message = connection.recv(300)
            if not received_message:
                break
            else:
                print(received_message.decode('utf8'))
        except TimeoutError:
            pass
        except KeyboardInterrupt:
            if connection:
                connection.close()
            break

except KeyboardInterrupt:
    print("Program Closed with Keyboard Interrupt")
    server.close()