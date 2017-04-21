/*
  Receiving Motor Code
  Adds mutliaxis support in order to make JP happy

  Written for the Sea Sweepers ROV Club
  Michael Georgariou 2017
*/

#include <Servo.h>
byte H1Pin = 2;
Servo H1;
byte H2Pin = 7;
Servo H2;
byte H3Pin = 6;
Servo H3;
byte H4Pin = 3;
Servo H4;
byte V1Pin = 4;
Servo V1;
byte V2Pin = 5;
Servo V2;
byte V3Pin = 8;
Servo V3;
byte V4Pin = 1;
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

  Serial.begin(115200);
  Serial1.begin(115200);

  H1.attach(H1Pin); // left front
    H1.writeMicroseconds(1500);
  H2.attach(H2Pin); // right front
    H2.writeMicroseconds(1500);
  H3.attach(H3Pin); // right back
    H3.writeMicroseconds(1500);
  H4.attach(H4Pin); // left back
    H4.writeMicroseconds(1500);
  V1.attach(V1Pin);
    V1.writeMicroseconds(1500);
  V2.attach(V2Pin);
    V2.writeMicroseconds(1500);
  V3.attach(V3Pin);
    V3.writeMicroseconds(1500);
  V4.attach(V4Pin);
    V4.writeMicroseconds(1500);
  delay(2000);
}

void loop() {

  Communication();
  delay(10);
  MotorWriteBasic(); //replace with MA later
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

//  if (Serial1.available()<19) { 

    FirstByte=Serial1.read();

    if (FirstByte == 1) {
      Serial.println("Receiving Good Data");
          
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
      
//    }
  //See if Checksums equal zero
  CombineValues();
  MapJoysticks();
  }
}

void MotorWriteBasic() {
  H2.writeMicroseconds(Map2B);
  H3.writeMicroseconds(Map2B);
  H1.writeMicroseconds(Map2A);
  H4.writeMicroseconds(Map2A);

  V1.writeMicroseconds(Map1A);
  V2.writeMicroseconds(Map1A);
  V3.writeMicroseconds(Map1B);
  V4.writeMicroseconds(Map1B);
}


void MotorWriteMA() { // Writes joystick values to ESCs (multiaxis)
  
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
    if ((Map2B - Crab) < 1100) {
      H2.write(Map2B);
      H3.write(Map2B);
    }
    if ((Map2A + Crab) < 1900) {
      H1.write(Map2A + Crab);
      H4.write(Map2A + Crab);
    }
    if ((Map2A + Crab) > 1900) {
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
    if ((Map2B - Crab) < 1100) {
      H2.write(Map2B);
      H3.write(Map2B);
    }
    if ((Map2A + Crab) < 1900) {
      H1.write(Map2A + Crab);
      H4.write(Map2A + Crab);
    }
    if ((Map2A + Crab) > 1900) {
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
    if ((Map2A + Roll) < 1900) {
      V1.write(Map1A + Roll);
      V2.write(Map1A + Roll);
    }
    if ((Map2A + Roll) > 1900) {
      V1.write(Map1A);
      V2.write(Map1A);
    }
    if ((Map1B - Roll) > 1100) {
      V3.write(Map1B - Roll);
      V4.write(Map1B - Roll);
    }
    if ((Map1B - Roll) < 1100) {
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
    if ((Map1A - Roll) < 1100){ 
      V1.write(Map1A);
      V2.write(Map1A);
    }
    if ((Map1B + Roll) < 1900) {
      V3.write(Map1B + Roll);
      V4.write(Map1B + Roll);
    }
    if ((Map1B + Roll) > 1900) {
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
  else {
    Joystick1A = 512;
  }
  if (checkit1==0)  {
    Joystick1B = val[3]<<8 | val[2];
  }
  else {
    Joystick1B = 512;
  }
  if (checkit2==0)  {
    Joystick1C = val[5]<<8 | val[4]; 
  }
  else {
    Joystick1C = 512;
  }
  if (checkit3==0)  {
    Joystick2A = val[7]<<8 | val[6]; 
  }
  else {
    Joystick2A = 512;
  }
  if (checkit4==0)  {
    Joystick2B = val[9]<<8 | val[8]; 
  }
  else {
    Joystick2B = 512;
  }
  if (checkit5==0)  {
    Joystick2C = val[11]<<8 | val[10]; 
  }
  else {
    Joystick2C = 512;
  }
}

void MapJoysticks() { // Maps joysticks for use with ESC
  Map1A = map(Joystick1A, 0, 1023, 1100, 1900);
  Map1B = map(Joystick1B, 0, 1023, 1100, 1900);
  Map1C = map(Joystick1C, 0, 1023, 1100, 1900); 
  Map2A = map(Joystick2A, 0, 1023, 1100, 1900);
  Map2B = map(Joystick2B, 0, 1023, 1100, 1900);
  Map2C = map(Joystick2C, 0, 1023, 1100, 1900);
Serial.println(Map2B);

}

void DrivingLight() {
  
}

void FunLEDs() {
  
}

void VertStabilization() {
  

  
}

