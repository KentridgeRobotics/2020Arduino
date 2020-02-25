#include "Adafruit_VCNL4010Software.h"

Adafruit_VCNL4010Software outSensor;

void setup() {
  Serial.begin(115200);
  while(!outSensor.begin(6, 7)) {
    Serial.println("no sensor");
    asm volatile ("  jmp 0");
  }
  outSensor.setLEDcurrent(20);
  outSensor.setFrequency(VCNL4010_250);
  Serial.println("sensor began");
}

bool outStatus, outLast;

void loop() {
  outStatus = outSensor.readProximity() >= 5000;
  if (!outStatus && outLast) {
    Serial.println("dec");
  }
  outLast = outStatus;
}
