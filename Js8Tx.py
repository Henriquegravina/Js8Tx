import socket
import json
import time
import serial

ser = serial.Serial('/dev/ttyUSB2', 19200)

UDP_IP = "127.0.0.1"
UDP_PORT = 2242

sock = socket.socket(socket.AF_INET, 
                     socket.SOCK_DGRAM)
sock.bind((UDP_IP, UDP_PORT))

while True:
    data, addr = sock.recvfrom(1024) 
    message = json.loads(data)
    if(message['type'] == 'TX.FRAME'):
        print(message['params']['TONES'])
        symbols = message['params']['TONES']
        ser.write("8")
        for symbol in symbols:
            time.sleep(12.64 / 79) # 12.64 = Normal Mode Speed 25.28 = Slow
            print symbol
            ser.write(str(symbol))
        ser.write("9")

