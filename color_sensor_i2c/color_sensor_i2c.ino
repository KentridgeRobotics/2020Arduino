#include <Wire.h>
#include "Adafruit_TCS34725.h"
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_16X);

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
  Serial.begin(9600);

  if(tcs.begin()){
    //
  }else{
    Serial.println("No TCS34725");
    while(1);
  }
}

void loop() {
  float r,g,b;
  tcs.setInterrupt(false);
  delay(60);
  tcs.getRGB(&r,&g,&b);
  tcs.setInterrupt(true);
  
  Serial.print("R:");
  Serial.print(r);
  Serial.print(" G:");
  Serial.print(g);
  Serial.print(" B:");
  Serial.println(b);
 

  int color = 0;

  double prev_dist = -1.0;
  for ( byte i = 0; i < 4; i++ ) {
   double distance = colorDistance(r,g,b,colors[i][0],colors[i][1],colors[i][2]);
   if (i == 0 || distance < prev_dist)
   {
      color = i;
      prev_dist = distance;
      newDistance = distance;
   }

  }

  if (newDistance > 100) {
      color = -1;
  }
  Serial.print("COLOR ");
  if (color == 0) Serial.print("R ");
  else if (color == 1) Serial.print("G ");
  else if (color == 2) Serial.print("B ");
  else if (color == 3) Serial.print("Y ");
  else Serial.print("N ");
}
