# Código de ejemplo AdafruitIO
# Universidad del Valle de Guatemala
# IE3027 - Electrónica Digital 2
# Diego Morales

# Adafruit IO
# https://io.adafruit.com/

# if Module not Found. Open Terminal/CMD and execute:
# pip3 install Adafruit_IO

from adafruit_io import Client, RequestError, Feed

ADAFRUIT_IO_KEY = "aio_QVdY56SbQT39GcmjKrLpI58ub9GN"
ADAFRUIT_IO_USERNAME = "Stefano_Papadopolo"
aio = Client(ADAFRUIT_IO_USERNAME, ADAFRUIT_IO_KEY)

# #Digital Feed
# digital_feed = aio.feeds('Temperature')
# aio.send_data(digital_feed.key, 1)
# digital_data = aio.receive(digital_feed.key)
# print(f'digital signal: {digital_data.value}')
# 
# #Analog Feed
# analog_feed = aio.feeds('hello-analog')
# aio.send_data(analog_feed.key, 100)
# analog_data = aio.receive(analog_feed.key)
# print(f'analog signal: {analog_data.value}')
