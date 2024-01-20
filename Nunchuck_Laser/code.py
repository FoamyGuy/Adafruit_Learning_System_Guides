# SPDX-FileCopyrightText: 2021 ladyada for Adafruit Industries
# SPDX-License-Identifier: MIT

import time
import board
import digitalio
import simpleio
import adafruit_nunchuk
import adafruit_pca9685
import adafruit_motor.servo

PITCH_OFFSET = 45  # The offset for the pitch
PITCH_RANGE = 90  # The range the servo can rotate up and down in degrees
YAW_RANGE = 90  # The range the servo can rotate side to side in degrees
INVERT_PITCH = False

# STEMMA QT 3V needs to be activated
i2c_power = digitalio.DigitalInOut(board.I2C_POWER)
i2c_power.switch_to_output(value=False)
i2c = board.I2C()  # uses board.SCL and board.SDA
# i2c = board.STEMMA_I2C()  # For using the built-in STEMMA QT connector on a microcontroller

wing = adafruit_pca9685.PCA9685(i2c)
wing.frequency = 50
servo_yaw = adafruit_motor.servo.Servo(wing.channels[0])
servo_pitch = adafruit_motor.servo.Servo(wing.channels[1])
laser = wing.channels[2]

nc = adafruit_nunchuk.Nunchuk(i2c)

# Pre-calculate the angles
min_yaw_angle = YAW_RANGE / 2
max_yaw_angle = 180 - (YAW_RANGE / 2)
min_pitch_angle = PITCH_OFFSET + (PITCH_RANGE / 2)
max_pitch_angle = PITCH_OFFSET + 180 - (PITCH_RANGE / 2)

pitch_inputs = [0, 255]
if INVERT_PITCH:  # Swap the Min and Max Values
    pitch_inputs[0], pitch_inputs[1] = pitch_inputs[1], pitch_inputs[0]

brightness = 0xFFFF  # Initial brightness value

while True:
    x, y = nc.joystick
    servo_yaw.angle = simpleio.map_range(255 - x, 0, 255, min_yaw_angle, max_yaw_angle)
    servo_pitch.angle = simpleio.map_range(
        y, pitch_inputs[0], pitch_inputs[1], min_pitch_angle, max_pitch_angle
    )
    ax = nc.acceleration[0]

    if nc.buttons.Z:  # Z-Button sets laser PWM to current brightness
        laser.duty_cycle = brightness
    elif nc.buttons.C:  # C-Button sets laser brightness to value of the nunchuck roll position
        brightness = int(simpleio.map_range(ax, 250, 750, 0, 0xFFFF))
        laser.duty_cycle = brightness
    else:  # No button pressed sets laser to off
        laser.duty_cycle = 0
    time.sleep(0.01)
