#include <SoftwareWire.h>
#include "VCNL4010Software.h"

VCNL4010Software outSensor;

void setup() {
  Serial.begin(115200);
  pinMode(7, INPUT_PULLUP);
  while(!outSensor.begin()) {
    Serial.println("no sensor");
    asm volatile ("  jmp 0");
  }
  outSensor.setProximityContinuous(true);
  outSensor.setProximityHz(251);
  outSensor.setLEDmA(200);
  outSensor.setInterrupt(1,false,false,true,false,0,5000);
  Serial.println("sensor began");
}

bool interrupt = false;
bool lastInt = false;

void loop() {
  interrupt = digitalRead(7) == LOW;
  if (interrupt)
    outSensor.clearInterrupt();
  if (!interrupt && lastInt) {
    Serial.println("a");
  }
  lastInt = interrupt;
}
