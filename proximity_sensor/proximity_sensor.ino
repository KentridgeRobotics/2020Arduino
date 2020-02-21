#include <Wire.h>
#include <SoftwareWire.h>
#include "VCNL4010.h"
#include "VCNL4010Software.h"
#include <CSSerial.h>

#define PROXIMITY_THRESHOLD 15000

VCNL4010 inSensor;
VCNL4010Software outSensor;

void setup() {
  //serialBegin(0xB02D);
  Serial.begin(115200);
  pinMode(6, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
  while(!inSensor.begin()) {
    asm volatile ("  jmp 0");
  }
  inSensor.setProximityContinuous(true);
  inSensor.setProximityHz(251);
  inSensor.setLEDmA(200);
  inSensor.setInterrupt(1,false,false,true,false,0,5000);
  while(!outSensor.begin()) {
    asm volatile ("  jmp 0");
  }
  outSensor.setProximityContinuous(true);
  outSensor.setProximityHz(251);
  outSensor.setLEDmA(200);
  outSensor.setInterrupt(1,false,false,true,false,0,5000);
}

bool lastIn, lastOut, currentIn, currentOut;
uint8_t ballCount = 5;

void loop() {
  currentIn = digitalRead(6) == LOW;
  currentOut = digitalRead(7) == LOW;
  if (currentIn)
    inSensor.clearInterrupt();
  if (currentOut)
    outSensor.clearInterrupt();
  if (lastIn && !currentIn) {
    ballCount++;
    Serial.println("inc");
  }
  if (lastOut && !currentOut) {
    ballCount--;
    Serial.println("dec");
  }
  lastIn = currentIn;
  lastOut = currentOut;
  //if (serialAvailable() >= 0) {
    //addData(ballCount);
    //sendData();
  //}
}
