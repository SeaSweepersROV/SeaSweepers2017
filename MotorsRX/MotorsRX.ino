/*
  Receiving Motor Code
  Adds mutliaxis support in order to make JP happy

  Written for the Sea Sweepers ROV Club
  Michael Georgariou 2017
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

unsigned char val[16];
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

unsigned char checksum0;
unsigned char checksum1;
unsigned char checksum2;
unsigned char checksum3;
unsigned char checksum4;
unsigned char checksum5;
unsigned char checksum6;
unsigned char checksum7;
unsigned char FirstByte;
unsigned char LastByte;

unsigned char checkit0;
unsigned char checkit1;
unsigned char checkit2;
unsigned char checkit3;
unsigned char checkit4;
unsigned char checkit5;
unsigned char checkit6;
unsigned char checkit7;
  

int VertVar; //to store number for stabilization

int Crab;
int Roll;

void setup() {

  Serial1.begin(115200);

  H1.attach(1); // left front
  H2.attach(2); // right front
  H3.attach(3); // right back
  H4.attach(4); // left back
  V1.attach(5);
  V2.attach(6);
  V3.attach(7);
  V4.attach(8);
}

void loop() {

  Communication();
  MotorWrite();
  DrivingLight();
  FunLEDs();

//  if () { //ADD SWTICH
//    VertStabilization(); //stays completely flat (always on)
//  }
}

void Communication() {
  Serial1.write(1);
  Serial1.write(2);
  Serial1.write(3);

  if (Serial1.available()>19) { 
    FirstByte=Serial1.read();

    if (FirstByte == 1) {
      val[0]=Serial1.read();         // least significant 8 bits
      val[1]=Serial1.read();         // most significant 2 bits   
       checksum0=Serial1.read();     // checksum
  
      val[2]=Serial1.read();
      val[3]=Serial1.read();
       checksum1=Serial1.read();
  
      val[4]=Serial1.read();
      val[5]=Serial1.read(); 
       checksum2=Serial1.read();
  
      val[6]=Serial1.read();         // least significant 8 bits
      val[7]=Serial1.read();         // most significant 2 bits   
       checksum3=Serial1.read();     // checksum
  
      val[8]=Serial1.read();
      val[9]=Serial1.read();
       checksum4=Serial1.read();
  
      val[10]=Serial1.read();
      val[11]=Serial1.read(); 
       checksum5=Serial1.read();

      LastByte = Serial1.read();
      
    }
  //See if Checksums equal zero
  CombineValues();
  }
}

void MotorWrite() { // Writes joystick values to ESCs
  
  MapJoysticks();
  
  
    if ((Map2C < 1600) && (Map2C > 1400)) { //if Map2C is neutral
    H2.write(Map2B);
    H3.write(Map2B);
    H1.write(Map2A);
    H4.write(Map2A);
  }
  
  if (Map2C > 1600) {
    Crab = map(Map2C, 1600, 1900, 0, 400);
    if ((Map2B - Crab) > 1100){
      H2.write(Map2B - Crab);
      H3.write(Map2B - Crab);
    }
    else {
      H2.write(Map2B);
      H3.write(Map2B);
    }
    if ((Map2A + Crab) < 1900) {
      H1.write(Map2A + Crab);
      H4.write(Map2A + Crab);
    }
    else {
      H1.write(Map2A);
      H4.write(Map2A);
    }
  }
    
  if (Map2C < 1400) {
    Crab = map(Map2C, 1400, 1100, 0, 400);
    if ((Map2B - Crab) > 1100){
      H2.write(Map2B - Crab);
      H3.write(Map2B - Crab);
    }
    else {
      H2.write(Map2B);
      H3.write(Map2B);
    }
    if ((Map2A + Crab) < 1900) {
      H1.write(Map2A + Crab);
      H4.write(Map2A + Crab);
    }
    else {
      H1.write(Map2A);
      H4.write(Map2A);
    }
  }
  
  
  if ((Map1C < 1600) && (Map1C > 1400)) { //if Map1C is neutral
    V1.write(Map1A);
    V2.write(Map1A);
    V3.write(Map1B);
    V4.write(Map1B);
  }
  if (Map1C > 1600) {
    Roll = map(Map1C, 1600, 1900, 0, 400);
    if ((Map2A + Roll) < 1900){
      V1.write(Map1A + Roll);
      V2.write(Map1A + Roll);
    }
    else {
      V1.write(Map1A);
      V2.write(Map1A);
    }
    if ((Map1B - Roll) > 1100) {
      V3.write(Map1B - Roll);
      V4.write(Map1B - Roll);
    }
    else {
      V3.write(Map1B);
      V4.write(Map1B);
    }
  }
    
  if (Map1C < 1400) {
    Roll = map(Map1C, 1400, 1100, 0, 400);
    if ((Map1A - Roll) > 1100){
      V1.write(Map1A - Roll);
      V2.write(Map1A - Roll);
    }
    else {
      V1.write(Map1A);
      V2.write(Map1A);
    }
    if ((Map1B + Roll) < 1900) {
      V3.write(Map1B + Roll);
      V4.write(Map1B + Roll);
    }
    else {
      V3.write(Map1B);
      V4.write(Map1B);
    }
  }
}

void CombineValues() {
  // Add up checksums
  checkit0 = val[0]+val[1]+checksum0;
  checkit1 = val[2]+val[3]+checksum1;
  checkit2 = val[4]+val[5]+checksum2; 
  checkit3 = val[6]+val[7]+checksum3;
  checkit4 = val[8]+val[9]+checksum4;
  checkit5 = val[10]+val[11]+checksum5; 
  //check if they equal zero
  if (checkit0==0)  {
    Joystick1A = val[1]<<8 | val[0];
  }
  if (checkit1==0)  {
    Joystick1B = val[3]<<8 | val[2];
  }
  if (checkit2==0)  {
    Joystick1C = val[5]<<8 | val[4]; 
  }
  if (checkit3==0)  {
    Joystick2A = val[7]<<8 | val[6]; 
  }
  if (checkit4==0)  {
    Joystick2B = val[9]<<8 | val[8]; 
  }
  if (checkit5==0)  {
    Joystick2C = val[11]<<8 | val[10]; 
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

void DrivingLight() {
  
}

void FunLEDs() {
  
}

void VertStabilization() {
  

  
}

