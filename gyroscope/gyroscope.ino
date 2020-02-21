#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <CSSerialProtocol.h>
  
Adafruit_BNO055 bno = Adafruit_BNO055(55);
 
void setup(void) 
{
  initSerial(0xA02D);
  /* Initialise the sensor */
  if(!bno.begin()){
    asm volatile ("  jmp 0"); 
  }
  delay(1000);
  bno.setExtCrystalUse(true);
}

unsigned long startTime = 0;
float x,y,z;

void loop(void) 
{
  if (millis() - startTime >= 100) {
    sensors_event_t event; 
    bno.getEvent(&event);
    x = event.orientation.x;
    y = event.orientation.y;
    z = event.orientation.z;
    startTime = millis();
  }
  if (runSerial() >= 0) {
    addData(x);
    addData(y);
    addData(z);
    sendData();
  }
}
