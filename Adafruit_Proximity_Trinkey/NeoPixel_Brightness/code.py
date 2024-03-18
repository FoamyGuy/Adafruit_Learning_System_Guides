# SPDX-FileCopyrightText: 2021 Kattni Rembor for Adafruit Industries
#
# SPDX-License-Identifier: MIT

"""
NeoPixel brightness proximity example. Increases the brightness of the NeoPixels as you move closer
to the proximity sensor.
"""
import time
import board
import neopixel
from adafruit_apds9960.apds9960 import APDS9960

i2c = board.I2C()  # uses board.SCL and board.SDA
# i2c = board.STEMMA_I2C()  # For using the built-in STEMMA QT connector on a microcontroller
apds = APDS9960(i2c)
pixels = neopixel.NeoPixel(board.NEOPIXEL, 2)

apds.enable_proximity = True


def proximity_to_brightness(value):
    """Maps the proximity values (0 - 255) to the brightness values (0.0 - 1.0)"""
    return value / 255 * 1.0


pixels.fill((255, 0, 0))

while True:
    print(apds.proximity)
    pixels.brightness = proximity_to_brightness(apds.proximity)
    time.sleep(0.2)
