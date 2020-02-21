#include <Wire.h>
#include "VCNL4010.h"
#include <CSSerialProtocol.h>

VCNL4010 sensor;
boolean currentTrigger;
boolean prevTrigger;

int ballCount = 5;

void setup() {
  initSerial(0xB02D);
  while(!sensor.begin()) {
    asm volatile ("  jmp 0"); 
  }
  sensor.setProximityContinuous(true);
  sensor.setProximityHz(250);
  sensor.setLEDmA(200);
}

bool ball = false;

void loop() {
  // put your main code here, to run repeatedly:
  sensor.setInterrupt(1,false,false,true,false,0,15000);
  sensor.getProximity();
  if(sensor.getInterrupt() == bit(0)) {
    currentTrigger = true;
    sensor.clearInterrupt(bit(0));
  }
  else{
    currentTrigger = false;
    if(currentTrigger == false && prevTrigger == true) {
      ballCount--;
      inc = true;
    }
  }
  prevTrigger = currentTrigger;
}
