# SPDX-FileCopyrightText: 2022 Kattni Rembor for Adafruit Industries
# SPDX-License-Identifier: MIT
"""
CircuitPython analog voltage value example
"""
import time
import board
import analogio

analog_pin = analogio.AnalogIn(board.A0)


def get_voltage(pin):
    return (pin.value * 3.1) / 61000


while True:
    print(get_voltage(analog_pin))
    time.sleep(0.1)
