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
  int Map1A;
int Joystick1B;
  int Map1B;
int Joystick1C;
  int Map1C;
int Joystick2A; //horizontal
  int Map2A;
int Joystick2B;
  int Map2B;
int Joystick2C;
  int Map2C;

int VertVar; //to store number for stabilization

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

  MotorWrite();
  DrivingLight();
  FunLEDs();

  if () { //ADD SWTICH
    VertStabilization(); //stays completely flat (always on)
  }
  
  if () { //ADD SWITCH
    
  }
}



void MotorWrite() { // Writes joystick values to ESCs
  
  MapJoysticks();
  
  
    if ((Map2C < 1600) && (Map2C > 1400)) { //if Map2C is neutral
    H2.write(Map2A);
    H3.write(Map2A);
    H1.write(Map2B);
    H4.write(Map2B);
  }
  
  if (Map2C > 1600) {
    Crab = map(Map2C, 1600, 1900, 0, 400);
    if ((Map2A - Crab) > 1100){
      H2.write(Map2A - Crab);
      H3.write(Map2A - Crab);
    }
    else {
      H2.write(Map2A);
      H3.write(Map2A);
    }
    if ((Map2B + Crab) < 1900) {
      H1.write(Map2B + Crab);
      H4.write(Map2B + Crab);
    }
    else {
      H1.write(Map2B);
      H4.write(Map2B);
    }
  }
    
  if (Map2C < 1400) {
    Crab = map(Map2C, 1400, 1100, 0, 400);
    if ((Map2A - Crab) > 1100){
      H2.write(Map2A - Crab);
      H3.write(Map2A - Crab);
    }
    else {
      H2.write(Map2A);
      H3.write(Map2A);
    }
    if ((Map2B + Crab) < 1900) {
      H1.write(Map2B + Crab);
      H4.write(Map2B + Crab);
    }
    else {
      H1.write(Map2B);
      H4.write(Map2B);
    }
  }
  
  
  if ((Map1C < 1600) && (Map1C > 1400)) { //if Map2C is neutral
    V2.write(Map1A);
    V3.write(Map1A);
    V1.write(Map1B);
    V4.write(Map1B);
  }
  if (Map1C > 1600) {
    Pitch = map(Map1C, 1600, 1900, 0, 400);
    if ((Map2A + Pitch) < 1900){
      H1.write(Map2A + Pitch);
      H2.write(Map2A + Pitch);
    }
    else {
      H1.write(Map2A);
      H2.write(Map2A);
    }
    if ((Map2B - Pitch) > 1100) {
      H3.write(Map2B - Pitch);
      H4.write(Map2B - Pitch);
    }
    else {
      H3.write(Map2B);
      H4.write(Map2B);
    }
  }
    
  if (Map2C < 1400) {
    Pitch = map(Map2C, 1400, 1100, 0, 400);
    if ((Map2A - Pitch) > 1100){
      H1.write(Map2A - Pitch);
      H2.write(Map2A - Pitch);
    }
    else {
      H1.write(Map2A);
      H2.write(Map2A);
    }
    if ((Map2B + Pitch) < 1900) {
      H3.write(Map2B + Pitch);
      H4.write(Map2B + Pitch);
    }
    else {
      H3.write(Map2B);
      H4.write(Map2B);
    }
  }
}


void MapJoysticks() { // Maps joysticks for use with ESC
  Map1A = map(Joystick1A, 0, 1023, 1100, 1900);
  Map1B = map(Joystick1B, 0, 1023, 1100, 1900);
  Map1C = map(Joystick1C, 0, 1023, 1100, 1900); 
  Map2A = map(Joystick2A, 0, 1023, 1100, 1900);
  Map2B = map(Joystick2B, 0, 1023, 1100, 1900);
  Map2C = map(Joystick2C, 0, 1023, 1100, 1900);
}

void DrivingLight () {
  
}

void FunLEDs () {
  
}

void VertStabilization () {
  

  
}

