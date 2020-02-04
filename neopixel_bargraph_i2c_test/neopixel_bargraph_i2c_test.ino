
#include <Wire.h>

#define SDA_PIN 4
#define SCL_PIN 5
const int16_t I2C_MASTER = 0x40;
const int16_t I2C_SLAVE = 0x10;

void setup() {
  Wire.begin(SDA_PIN, SCL_PIN,
             I2C_MASTER); // join i2c bus (address optional for master)
}

void loop() {

  static uint8_t count = 0;

  Wire.beginTransmission(I2C_SLAVE);
  Wire.write((uint8_t)0);   // offset
  Wire.write((uint8_t)4);   // length
  Wire.write((uint8_t)0);   // red
  Wire.write((uint8_t)255); // green
  Wire.write((uint8_t)0);   // blue
  Wire.write(count);        // count
  Wire.endTransmission();

  count++;
  count %= 16;

  delay(1000 / 16);
}