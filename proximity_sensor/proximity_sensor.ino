#include "Adafruit_VCNL4010.h"
#include "Adafruit_VCNL4010Software.h"
#include <CSSerial.h>

#define PROXIMITY_THRESHOLD 5000

Adafruit_VCNL4010 inSensor;
Adafruit_VCNL4010Software outSensor;

void setup() {
  serialBegin(0xB02D);
  registerHeader(0xB0FF);
  pinMode(LED_BUILTIN, OUTPUT);
  while(!inSensor.begin()) {
    asm volatile ("  jmp 0");
  }
  inSensor.setLEDcurrent(20);
  inSensor.setFrequency(VCNL4010_250);
  while(!outSensor.begin(6, 7)) {
    asm volatile ("  jmp 0");
  }
  outSensor.setLEDcurrent(20);
  outSensor.setFrequency(VCNL4010SOFTWARE_250);
}

bool inCurrent, outCurrent, inLast, outLast;
uint8_t ballCount = 3;

long lastTime = 0;

void loop() {
  inCurrent = inSensor.readProximity() >= PROXIMITY_THRESHOLD;
  outCurrent = outSensor.readProximity() >= PROXIMITY_THRESHOLD;
  if (inLast && !inCurrent && ballCount < 127) {
    ballCount++;
    digitalWrite(LED_BUILTIN, HIGH);
  }
  if (outLast && !outCurrent && ballCount > 0) {
    ballCount--;
    digitalWrite(LED_BUILTIN, LOW);
  }
  inLast = inCurrent;
  outLast = outCurrent;
  if (serialAvailable() >= 0) {
    if (getHeader() == 0xB0FF) {
      ballCount = 3;
    } else {
      addData(ballCount);
      sendData();
    }
  }
}
