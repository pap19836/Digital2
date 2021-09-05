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


ser = serial.Serial('COM2', baudrate = 9600)  #Inicialización de serial

ADAFRUIT_IO_KEY = "aaio_YAsE55Qo3Ewin96ARqeYnmza0wXa"  #Definición de Adafruit
ADAFRUIT_IO_USERNAME = "YRAR"
aio = Client(ADAFRUIT_IO_USERNAME, ADAFRUIT_IO_KEY)

while(1):
    # Reseteo de comunicación serial
    ser.close()
    ser.open()

    #Readval pyserial
    data = ser.read(7) # Lectura de cada uno de los caracteres enviados
    data0 = str(data) # Conversión a string

    # Creación de array de caracteres
    flags = data0.split(',')
    lock = flags[1] # Guardado de caracteres
    lock_compare = 1
    door = flags[2]
    door_compare = 1
    light = flags[3]

    #Variables
    light_compare = 1
    light_state_compare = 1
    light_state = 0
    light_int = 1

    #Print de visualización
    print(lock)
    print(door)
    print(light)

#VALORES ENVIAR

    #DOOR feed set and send
    door_feed = aio.feeds('door')
    if(door != door_compare):
        door_compare = door
        aio.send_data(door_feed.key, door)

    #LOCK feed set and send
    lock_feed = aio.feeds('lock')
    if(lock != lock_compare):
        lock_compare = lock
        aio.send_data(lock_feed.key, lock)

    #LIGHT feed set and send
    light_feed = aio.feeds('light')
    if(light != light_compare):
        light_compare = light
        aio.send_data(light_feed.key, light)

    #LIGHT control set
    light_ctrl_feed = aio.feeds('light-ctrl')

    
#VALOR A RECIBIR

    #LIGHT control receive
    if (light_state != light_state_compare):
        light_state_compare = light_state
        light_state = aio.receive(light_ctrl_feed.key)
        print(f'LIGHT: {light_state.value}')
        light_int = int(light_state.value)
        ser.write(bytes([light_int]))


    time.sleep(4)
