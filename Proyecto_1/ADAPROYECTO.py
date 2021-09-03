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


ser = serial.Serial('COM2', baudrate = 9600)

ADAFRUIT_IO_KEY = "aio_YAsE55Qo3Ewin96ARqeYnmza0wXa"
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
    lock_compare = 1
    door = flags[2]
    door_compare = 1
    light = flags[3]
    light_compare = 1
    light_state_compare = 1

    light_state = 0
    light_int = 1
    print(lock)
    print(door)
    print(light)

#VALORES ENVIAR

    #DOOR
    door_feed = aio.feeds('door')
    if(door != door_compare):
        door_compare = door
        aio.send_data(door_feed.key, door)

    #LOCK
    lock_feed = aio.feeds('lock')
    if(lock != lock_compare):
        lock_compare = lock
        aio.send_data(lock_feed.key, lock)

    #LIGHT
    light_feed = aio.feeds('light')
    if(light != light_compare):
        light_compare = light
        aio.send_data(light_feed.key, light)

    light_ctrl_feed = aio.feeds('light-ctrl')
#VALOR A RECIBIR

    #LIGHT
    if (light_state != light_state_compare):
        light_state_compare = light_state
        light_state = aio.receive(light_ctrl_feed.key)
        print(f'LIGHT: {light_state.value}')
        light_int = int(light_state.value)
        ser.write(bytes([light_int]))
    #if light_int == 2:
    #    ser.write('2'.encode('utf-8'))
    #if light_int == 1:
    #    ser.write('1'.encode('utf-8'))
    #if light_int == 0:
    #    ser.write('0'.encode('utf-8'))

    time.sleep(3)
