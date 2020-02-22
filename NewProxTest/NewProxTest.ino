#include <Wire.h>
#include "Adafruit_VCNL4010.h"

Adafruit_VCNL4010 vcnl;
 boolean currTrig = false;
 boolean lastTrig = false;

void setup() {
  Serial.begin(9600);
  Serial.println("VCNL4010 test");

  if (! vcnl.begin()){
    Serial.println("Sensor not found :(");
    while (1);
  }
  Serial.println("Found VCNL4010");
}


void loop() {
   //Serial.print("Ambient: "); Serial.println(vcnl.readAmbient());
   if(vcnl.readProximity() > 15000) {
    currTrig = true;
   }
   else {
    currTrig = false;
    if(!currTrig && lastTrig) {
      Serial.println("dec");
    }
   }
   //Serial.print("Proximity: "); Serial.println(vcnl.readProximity());
   lastTrig = currTrig;
   //delay(100);
}
