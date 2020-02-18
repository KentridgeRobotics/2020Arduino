#include <Wire.h>
#include "VCNL4010.h"

VCNL4010 Sensor;
boolean currentTrigger;
boolean prevTrigger;

int ballCount = 5;

void setup() {
  Serial.begin(9600);
  while(!Sensor.begin()) {
    Serial.println("Proxy Sensor not connected");
    delay(3000);
  }
  Sensor.setProximityContinuous(true);
  Sensor.setProximityHz(250);
  Sensor.setLEDmA(200);
}

void loop() {
  // put your main code here, to run repeatedly:
  Sensor.setInterrupt(1,false,false,true,false,0,15000);
  Sensor.getProximity();
  if(Sensor.getInterrupt() == bit(0)) {
    currentTrigger = true;
    Sensor.clearInterrupt(bit(0));
  }
  else{
    currentTrigger = false;
    if(currentTrigger == false && prevTrigger == true) {
      ballCount--;
      Serial.println("PRXYI increment");
    }
  }
  prevTrigger = currentTrigger;
}
