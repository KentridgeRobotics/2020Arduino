#include <CSSerialProtocol.h>
#include <Wire.h>
#include "Adafruit_TCS34725.h"
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_24MS, TCS34725_GAIN_16X); //Can be increased to 50 MS for greater accuracy if needed

double newDistance = 0;

float colors[][4] = {
  {161.3868, 56.9116, 56.2178},
  {48.6522, 125.8932, 83.3956},
  {28.623, 99.7964, 127.0984},
  {96.3246, 107.721, 51.8917}
};


float colorDistance(float red1, float green1, float blue1, float red2, float green2, float blue2) {
  uint64_t sum_of_squares; 
  sum_of_squares = sqrt(pow(red2-red1,2)+pow(green2-green1,2)+pow(blue2-blue1,2));
  return ( sum_of_squares  );
};

void setup() {
  initSerial(0x802D);
  if(!tcs.begin()){
    asm volatile ("  jmp 0"); 
  }
}

int color = 0;
unsigned long startTime = 0;
boolean start = false;

float r,g,b;

void loop() {
  if (!start) {
    r = 0;
    g = 0;
    b = 0;
    tcs.setInterrupt(false);
    start = true;
    startTime = millis();
  } else if (millis() - startTime >= 60) {
    start = false;
    tcs.getRGB(&r,&g,&b);
    tcs.setInterrupt(true); 

    color = 0;

    double prev_dist = -1.0;
    for ( byte i = 0; i < 4; i++ ) {
      double distance = colorDistance(r,g,b,colors[i][0],colors[i][1],colors[i][2]);
      if (i == 0 || distance < prev_dist) {
        color = i;
        prev_dist = distance;
        newDistance = distance;
      }
    }

    if (newDistance > 50) {
      color = -1;
    }
  }
  if (runSerial() >= 0) {
    if (color == 0)
      addData('R');
    else if (color == 1)
      addData('G');
    else if (color == 2)
      addData('B');
    else if (color == 3)
      addData('Y');
    else
      addData('N');
    sendData();
  }
}
