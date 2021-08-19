import serial
import Adafruit_IO
from Adafruit_IO import Client, RequestError, Feed

ADAFRUIT_IO_KEY = "aio_dKyH51MpFNbdUI5HQZM5ItDhIsTG"
ADAFRUIT_IO_USERNAME = "Stefano_Papadopolo"
aio = Client(ADAFRUIT_IO_USERNAME, ADAFRUIT_IO_KEY)
ser = serial.Serial("COM6", 9600, timeout=(1))
counter_temp=255
counter_IO = aio.feeds('pb-counter')  
slider_IO = aio.feeds("slider-counter")
while 1:
    counter = ser.read(1)
    int_counter=int.from_bytes(counter, "big")
    slider_data = int(aio.receive(slider_IO.key).value)
    slider_data_byte=bytes([slider_data])
    ser.write(slider_data_byte)
    print(slider_data)
    if int_counter == 0:
        continue
    else:
        while counter_temp!=int_counter:
            counter = ser.read(3)
            counter_temp =  int_counter
            aio.send_data(counter_IO.key, int_counter)
            print(int_counter)
    
