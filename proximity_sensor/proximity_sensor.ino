#include <Wire.h>
#include <SoftwareWire.h>
#include "VCNL4010.h"
#include "VCNL4010Software.h"
#include <CSSerial.h>

#define PROXIMITY_THRESHOLD 15000

VCNL4010 inSensor;
VCNL4010Software outSensor;

void setup() {
  serialBegin(0xB02D);
  while(!inSensor.begin()) {
    asm volatile ("  jmp 0");
  }
  inSensor.setProximityContinuous(true);
  inSensor.setProximityHz(250);
  inSensor.setLEDmA(200);
  while(!outSensor.begin()) {
    asm volatile ("  jmp 0");
  }
  outSensor.setProximityContinuous(true);
  outSensor.setProximityHz(250);
  outSensor.setLEDmA(200);
}

bool lastIn, lastOut, currentIn, currentOut;
uint8_t ballCount = 5;

void loop() {
  currentIn = inSensor.getProximity() > PROXIMITY_THRESHOLD;
  currentOut = outSensor.getProximity() > PROXIMITY_THRESHOLD;
  if (currentIn == false && lastIn == true) {
    ballCount++;
  }
  if (currentOut == false && lastOut == true) {
    ballCount--;
  }
  lastIn = currentIn;
  lastOut = currentOut;
  if (serialAvailable() >= 0) {
    addData(ballCount);
    sendData();
  }
}
