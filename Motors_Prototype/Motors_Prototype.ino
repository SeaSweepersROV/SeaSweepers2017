/*
  Receiving Motor Code
  Attempt 1 of mutliaxis support in order to make JP happy

  Written for the Sea Sweepers ROV Club
  Michael Georgariou 2016
*/

#include <Servo.h>
Servo H1;
Servo H2;
Servo H3;
Servo H4;
Servo V1;
Servo V2;
Servo V3;
Servo V4;

int Joystick1A; //vertical
  int 1AMap;
int Joystick1B;
  int 1BMap;
int Joystick1C;
  int 1CMap;
int Joystick2A; //horizontal
  int 2AMap;
int Joystick2B;
  int 2BMap;
int Joystick2C;
  int 2CMap;

int Crab;
int Pitch;

void setup() {

  H1.attach(2); // left front
  H2.attach(3); // right front
  H3.attach(4); // right back
  H4.attach(5); // left back
  V1.attach(6);
  V2.attach(7);
  V3.attach(8);
  V4.attach(9);
}

void loop() {
  1AMap = map(Joystick1A, 0, 1023, 1100, 1900);
  1BMap = map(Joystick1B, 0, 1023, 1100, 1900);
  1CMap = map(Joystick1C, 0, 1023, 1100, 1900); 
  2AMap = map(Joystick2A, 0, 1023, 1100, 1900);
  2BMap = map(Joystick2B, 0, 1023, 1100, 1900);
  2CMap = map(Joystick2C, 0, 1023, 1100, 1900);


  if ((2CMap < 1600) && (2CMap > 1400)) { //if 2CMap is neutral
    H2.write(2AMap);
    H3.write(2AMap);
    H1.write(2BMap);
    H4.write(2BMap);
  }
  
  if (2CMap > 1600) {
    Crab = map(2CMap, 1600, 1900, 0, 400);
    if ((2AMap - Crab) > 1100){
      H2.write(2AMap + Crab);
      H3.write(2AMap + Crab);
    }
    else {
      H2.write(2AMap);
      H3.write(2AMap);
    }
    if ((2BMap + Crab) < 1900) {
      H1.write(2BMap - Crab);
      H4.write(2BMap - Crab);
    }
    else {
      H1.write(2BMap);
      H4.write(2BMap);
    }
  }
    
  if (2CMap < 1400) {
    Crab = map(2CMap, 1400, 1100, 0, 400);
    if ((2AMap - Crab) > 1100){
      H2.write(2AMap - Crab);
      H3.write(2AMap - Crab);
    }
    else {
      H2.write(2AMap);
      H3.write(2AMap);
    }
    if ((2BMap + Crab) < 1900) {
      H1.write(2BMap + Crab);
      H4.write(2BMap + Crab);
    }
    else {
      H1.write(2BMap);
      H4.write(2BMap);
    }
  }
  
  
  if ((1CMap < 1600) && (1CMap > 1400)) { //if 2CMap is neutral
    V2.write(1AMap);
    V3.write(1AMap);
    V1.write(1BMap);
    V4.write(1BMap);
  }
  if (1CMap > 1600) {
    Pitch = map(1CMap, 1600, 1900, 0, 400);
    if ((2AMap + Pitch) < 1900){
      H1.write(2AMap + Pitch);
      H2.write(2AMap + Pitch);
    }
    else {
      H1.write(2AMap);
      H2.write(2AMap);
    }
    if ((2BMap - Pitch) > 1100) {
      H3.write(2BMap - Pitch);
      H4.write(2BMap - Pitch);
    }
    else {
      H3.write(2BMap);
      H4.write(2BMap);
    }
  }
    
  if (2CMap < 1400) {
    Pitch = map(2CMap, 1400, 1100, 0, 400);
    if ((2AMap - Pitch) > 1100){
      H1.write(2AMap - Pitch);
      H2.write(2AMap - Pitch);
    }
    else {
      H1.write(2AMap);
      H2.write(2AMap);
    }
    if ((2BMap + Pitch) < 1900) {
      H3.write(2BMap + Pitch);
      H4.write(2BMap + Pitch);
    }
    else {
      H3.write(2BMap);
      H4.write(2BMap);
    }
  }
}
 
