# Código de ejemplo AdafruitIO
# Universidad del Valle de Guatemala
# IE3027 - Electrónica Digital 2
# Diego Morales

# Adafruit IO
# https://io.adafruit.com/

# if Module not Found. Open Terminal/CMD and execute:
# pip3 install Adafruit_IO

from Adafruit_IO import Client, RequestError, Feed

ADAFRUIT_IO_KEY = "aio_dKyH51MpFNbdUI5HQZM5ItDhIsTG"
ADAFRUIT_IO_USERNAME = "Stefano_Papadopolo"
aio = Client(ADAFRUIT_IO_USERNAME, ADAFRUIT_IO_KEY)

# #Digital Feed
temperature = aio.feeds('temperatura')
aio.send_data(temperature.key, 1)

distance = aio.feeds("distancia")
aio.send_data(distance.key, 6)

