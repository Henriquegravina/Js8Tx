import socket
import json
import time
import serial
import time, threading

    
ser = serial.Serial('/dev/ttyUSB1', 19200)

UDP_IP = "127.0.0.1"
UDP_PORT = 2242

sock = socket.socket(socket.AF_INET, 
                     socket.SOCK_DGRAM)
sock.bind((UDP_IP, UDP_PORT))



while True:
    data, addr = sock.recvfrom(1024) 
    message = json.loads(data)
    
    print message
    if(message['type'] == 'TX.FRAME'):
        print(message['params']['TONES'])
        symbols = message['params']['TONES']
        #ser.write("SYMBOLS 811111111112222222222333333333344444444445555555555666666666677777777771111111119\r".encode())
        ser.write("SYMBOLS ")
        ser.write("8")
        for symbol in symbols:
            print symbol
            ser.write(str(symbol))
        ser.write("9\r".encode())
    
    if(message['type'] == 'PING'): # PING MESSAGE 
        print("Ping received")
        # When we receive a ping from JS8Call we can ask for this:
        sock.sendto(json.dumps({'type': 'RIG.GET_FREQ'}), addr)    #QRG and Offset
        sock.sendto(json.dumps({'type': 'MODE.GET_SPEED'}), addr)  #Offset
        
    if(message['type'] == 'RIG.FREQ'): # RIG.FREQ MESSAGE
        print("QRG:" + str(message['params']['DIAL']))
        ser.write("QRG " + str(message['params']['DIAL']) + "\r".encode() ) #Send QRG to Arduino

        print("OFFSET: " + str(message['params']['OFFSET']))
        ser.write("OFFSET " + str(message['params']['OFFSET']) + "\r".encode() ) # Send offset to Arduino

    if(message['type'] == 'MODE.SPEED'): # MODE.SPEED message
        #SPEED NORMAL = 0, SLOW = 4, FAST = 1, TURBO = 2
        print("SPEED: " + str(message['params']['SPEED']))
        ser.write("SPEED " + str(message['params']['SPEED']) + "\r".encode() )
    
        
