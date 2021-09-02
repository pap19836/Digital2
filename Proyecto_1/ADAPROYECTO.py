# Código de ejemplo AdafruitIO
# Universidad del Valle de Guatemala
# IE3027 - Electrónica Digital 2
# Editado por Gerardo Fuentes

# Adafruit IO
# https://io.adafruit.com/

# if Module not Found. Open Terminal/CMD and execute:
# pip3 install Adafruit_IO

from Adafruit_IO import Client, RequestError, Feed
import serial
import time


ser = serial.Serial('COM6', baudrate = 9600)

ADAFRUIT_IO_KEY = "aio_GZgj60uQsE4Q6pRqYdN9hVhehlH8"
ADAFRUIT_IO_USERNAME = "YRAR"
aio = Client(ADAFRUIT_IO_USERNAME, ADAFRUIT_IO_KEY)

while(1):
    ser.close()
    ser.open()
    #Readval pyserial
    data = ser.read(7)
    data0 = str(data)
    print(data0)
    flags = data0.split(',')
    lock = flags[1]
    door = flags[2]
    light = flags[3]

    print(lock)
    print(door)
    print(light)

#VALORES ENVIAR

    #DOOR
    door_feed = aio.feeds('door')
    aio.send_data(door_feed.key, door)

    #LOCK
    lock_feed = aio.feeds('lock')
    aio.send_data(lock_feed.key, lock)

    #LIGHT
    light_feed = aio.feeds('light')
    aio.send_data(lock_feed.key, light)

#VALOR A RECIBIR

    #LIGHT
    light_state = aio.receive(light_feed.key)
    print(f'LIGHT: {light_state.value}')
    light_int = int(light_state.value)

    if light_int == 1:
        ser.write('1'.encode('utf-8'))
    if light_int == 0:
        ser.write('0'.encode('utf-8'))
    #intlight = int(light_state.value)
    #bytelight = light_state.value.to_bytes(3, 'big')
    #ser.write(bytelight)
    time.sleep(3)
