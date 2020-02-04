
#include <arduino.h>
#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#include "i2c_slave.h"
#include "defines.h"

I2CSlave i2c(SDA_PIN, SCL_PIN, I2C_ADDR);

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

void receiveEvent(size_t length);

void setup() {
  // put your setup code here, to run once:
  i2c.begin();
  i2c.onReceive(receiveEvent);

  strip.begin();
  strip.show();
}

void receiveEvent() {
  // Set the neopixel ring to the right color and number of pixels
  I2CSlave::Registers reg = i2c.getRegisters();
  strip.clear();
  for (int i = 0; i < LED_COUNT; i++) {
    if (i <= reg.bargraph.count) {
      strip.setPixelColor(i, reg.bargraph.red, reg.bargraph.green,
                          reg.bargraph.blue);
    }
  }
  strip.show();
}

void loop() {}
