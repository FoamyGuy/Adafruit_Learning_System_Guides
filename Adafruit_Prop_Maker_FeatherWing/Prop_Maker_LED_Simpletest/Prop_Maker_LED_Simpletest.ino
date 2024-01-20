// SPDX-FileCopyrightText: 2019 Brent Rubell for Adafruit Industries
//
// SPDX-License-Identifier: MIT

/*
* Adafruit Prop-Maker Featherwing
* LED Example
*
* Rainbow swirl example for 3W LED.
*/

#ifdef USE_TINYUSB  // For Serial when selecting TinyUSB
#include <Adafruit_TinyUSB.h>
#endif

#if defined(__SAMD21G18A__) || defined(__AVR_ATmega32U4__) || defined(NRF52840_XXAA)
  // No green PWM on 32u4
  #define POWER_PIN    10
  #define RED_LED      11
  #define GREEN_LED    12  // no PWM on atmega32u4
  #define BLUE_LED     13
#elif defined(__AVR_ATmega328P__)
  // No Red or Blue PWM
  #define POWER_PIN    10
  #define RED_LED      2
  #define GREEN_LED    3  // the only PWM pin!
  #define BLUE_LED     4
#elif defined(NRF52)
  #define POWER_PIN    11
  #define RED_LED      7
  #define GREEN_LED    15
  #define BLUE_LED     16
#elif defined(ESP8266)
  #define POWER_PIN    15
  #define RED_LED      13
  #define GREEN_LED    12
  #define BLUE_LED     14
#elif defined(TEENSYDUINO)
  #define POWER_PIN    10
  #define RED_LED      9
  #define GREEN_LED    6
  #define BLUE_LED     5
#elif defined(ARDUINO_ADAFRUIT_FEATHER_ESP32S2)
  #define POWER_PIN    10
  #define RED_LED       0
  #define GREEN_LED     1
  #define BLUE_LED      2
  #define RED_PIN      11
  #define GREEN_PIN    12
  #define BLUE_PIN     13
  #define analogWrite ledcWrite
#elif defined(ESP32)
  #define POWER_PIN    33
  #define RED_LED       0
  #define GREEN_LED     1
  #define BLUE_LED      2
  #define RED_PIN      27
  #define GREEN_PIN    12
  #define BLUE_PIN     13
  #define analogWrite ledcWrite
#endif

uint8_t i=0;

uint8_t red_out = RED_LED;
uint8_t green_out = GREEN_LED;
uint8_t blue_out = BLUE_LED;

void setup() {
  Serial.begin(115200);
  Serial.println("\nProp-Maker Wing: LED Example");

  // set up the power pin
  pinMode(POWER_PIN, OUTPUT);
  // disable the power pin, we're not writing to the LEDs
  digitalWrite(POWER_PIN, LOW);

  // Set up the LED Pins
  #if defined(ESP32) // and ESP32-S2!
    #if ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(5, 1, 1)
      // newer LEDC API, use pins instead of channel
      red_out = RED_PIN;
      green_out = GREEN_PIN;
      blue_out = BLUE_PIN;
      ledcAttach(RED_PIN, 5000, 8);
      ledcAttach(GREEN_PIN, 5000, 8);
      ledcAttach(BLUE_PIN, 5000, 8);
    #else
      // older LEDC API, use channel, attach pin to channel
      ledcSetup(RED_LED, 5000, 8);
      ledcAttachPin(RED_PIN, RED_LED);
      ledcSetup(GREEN_LED, 5000, 8);
      ledcAttachPin(GREEN_PIN, GREEN_LED);
      ledcSetup(BLUE_LED, 5000, 8);
      ledcAttachPin(BLUE_PIN, BLUE_LED);
    #endif
  #else
    pinMode(red_out, OUTPUT);
    pinMode(green_out, OUTPUT);
    pinMode(blue_out, OUTPUT);
  #endif

  analogWrite(red_out, 0);
  analogWrite(green_out, 0);
  analogWrite(blue_out, 0);
}

uint32_t Color(uint8_t r, uint8_t g, uint8_t b) {
  return ((uint32_t)r << 16) | ((uint32_t)g << 8) | b;
}

uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

void loop()
{
  // set RGB Colors
  uint32_t color = Wheel(i++);
  uint8_t red = color >> 16;
  uint8_t green = color >> 8;
  uint8_t blue = color;

  // turn on the power pin
  digitalWrite(POWER_PIN, HIGH);

  // write colors to the 3W LED
  analogWrite(red_out, red);
  analogWrite(green_out, green);
  analogWrite(blue_out, blue);
  delay(2);
}