/*
  Receiving Motor Code
  Adds mutliaxis support in order to make JP happy

  Written for the Sea Sweepers ROV Club
  Michael Georgariou 2017
*/

//valve 12
//okay 13
//accent light on 9

#include <Servo.h>
byte H1Pin = 6; //connects all motors
Servo H1;
byte H2Pin = 3;
Servo H2;
byte H3Pin = 2;
Servo H3;
byte H4Pin = 7;
Servo H4;
byte V1Pin = 8;
Servo V1;
byte V2Pin = 10;
Servo V2;
byte V3Pin = 4;
Servo V3;
byte V4Pin = 5;
Servo V4;

#include <Adafruit_NeoPixel.h>
Adafruit_NeoPixel Lights = Adafruit_NeoPixel(14, 11, NEO_GRBW + NEO_KHZ800); //connects LED


unsigned char val[18]; //creates variable to send over serial
unsigned char BlueVal;
unsigned char WhiteVal;
unsigned char Valve1Val;
unsigned char Valve2Val;
unsigned char MagnetVal;

int Joystick1A; //vertical
int Map1A;      //map for servo
int Map1ACR;    //map for counter-rotating props
int Joystick1B;
int Map1B;
int Map1BCR;
int Joystick1C;
int Map1C;
int Map1CCR;
int Joystick2A; //horizontal
int Map2A;
int Map2ACR;
int Joystick2B;
int Map2B;
int Map2BCR;
int Joystick2C;
int Map2C;
int Map2CCR;

unsigned char checksum0; //checkcums and failsafes
unsigned char checksum1;
unsigned char checksum2;
unsigned char checksum3;
unsigned char checksum4;
unsigned char checksum5;
unsigned char checksum6;
unsigned char checksum7;
unsigned char FirstByte;
unsigned char LastByte;

unsigned char checkit0;   //variable to check checksums
unsigned char checkit1;
unsigned char checkit2;
unsigned char checkit3;
unsigned char checkit4;
unsigned char checkit5;
unsigned char checkit6;
unsigned char checkit7;


int VertVar; //to store number for stabilization

int Crab;   // to add stabilization possibly in future
int Roll;

void setup() {

  Serial.begin(19200); //initialize Serial
  Serial1.begin(19200);

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

  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT); //extra motors
  pinMode(52, OUTPUT);
  pinMode(53, OUTPUT);

  Lights.begin(); //starts light

  delay(2000);
}

void loop() {

  Communication();
  SerialPrint();
  MotorWriteBasic(); //replace with MA later
  DrivingLight();
  ValveTurner();
  Magnet();
  FunLEDs();
  delay(10);

  //  if () { //ADD SWTICH
  //    VertStabilization(); //stays completely flat (always on)
  //  }
}

void Communication() {
  Serial1.write(1);   //send handshake
  Serial1.write(2);
  Serial1.write(3);

  if (Serial1.available()>22) {   //check if serial communication is available
    FirstByte = Serial1.read();

    if (FirstByte == 1) {
      Serial.println("Receiving Good Data");

      val[0] = Serial1.read();       // least significant 8 bits
      val[1] = Serial1.read();       // most significant 2 bits
      checksum0 = Serial1.read();   // checksum

      val[2] = Serial1.read();
      val[3] = Serial1.read();
      checksum1 = Serial1.read();

      val[4] = Serial1.read();
      val[5] = Serial1.read();
      checksum2 = Serial1.read();

      val[6] = Serial1.read();       // least significant 8 bits
      val[7] = Serial1.read();       // most significant 2 bits
      checksum3 = Serial1.read();   // checksum

      val[8] = Serial1.read();
      val[9] = Serial1.read();
      checksum4 = Serial1.read();

      val[10] = Serial1.read();
      val[11] = Serial1.read();
      checksum5 = Serial1.read();
      
      WhiteVal = Serial1.read();
      BlueVal = Serial1.read();

      Valve1Val = Serial1.read();
      Valve2Val = Serial1.read();
      
      MagnetVal = Serial1.read();

      LastByte = Serial1.read();

    }
    //See if Checksums equal zero
    CombineValues();
    MapJoysticks();

  }
}
void MotorWriteBasic() {

  H2.writeMicroseconds(Map2B);    //writes values to motors
  H3.writeMicroseconds(Map2B);
  H1.writeMicroseconds(Map2ACR);
  H4.writeMicroseconds(Map2ACR);

  V1.writeMicroseconds(Map1ACR);
  V2.writeMicroseconds(Map1A);
  V3.writeMicroseconds(Map1BCR);
  V4.writeMicroseconds(Map1B);
}


void MotorWriteMA() { // Writes joystick values to ESCs (multiaxis, not working correctly)

  if ((Map2C < 1600) && (Map2C > 1400)) { //if Map2C is neutral
    H2.write(Map2B);
    H3.write(Map2B);
    H1.write(Map2A);
    H4.write(Map2A);
  }

  if (Map2C > 1600) {
    Crab = map(Map2C, 1600, 1900, 0, 400);
    if ((Map2B - Crab) > 1100) {
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
    if ((Map2B - Crab) > 1100) {
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
    if ((Map1A - Roll) > 1100) {
      V1.write(Map1A - Roll);
      V2.write(Map1A - Roll);
    }
    if ((Map1A - Roll) < 1100) {
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
  checkit0 = val[0] + val[1] + checksum0;
  checkit1 = val[2] + val[3] + checksum1;
  checkit2 = val[4] + val[5] + checksum2;
  checkit3 = val[6] + val[7] + checksum3;
  checkit4 = val[8] + val[9] + checksum4;
  checkit5 = val[10] + val[11] + checksum5;
  //check if they equal zero
  if (LastByte == 2) { 
    if (checkit0 == 0)  {
      Joystick1A = val[1] << 8 | val[0];
    }
    if (checkit1 == 0)  {
      Joystick1B = val[3] << 8 | val[2];
    }
    if (checkit2 == 0)  {
      Joystick1C = val[5] << 8 | val[4];
    }
    if (checkit3 == 0)  {
      Joystick2A = val[7] << 8 | val[6];
    }
    if (checkit4 == 0)  {
      Joystick2B = val[9] << 8 | val[8];
    }
    if (checkit5 == 0)  {
      Joystick2C = val[11] << 8 | val[10];
    }
  }
}

void SerialPrint() {
//  Serial.print(Map1A);
//  Serial.print(" ");
//  Serial.print(Map1B);
//  Serial.print(" ");
//  Serial.print(Map1C);
//  Serial.print(" ");
  Serial.print(Map2A);
  Serial.print(" ");
  Serial.print(Map2B);
  Serial.print(" ");
//  Serial.print(Map2C);
  Serial.print(BlueVal);
//  Serial.print(WhiteVal);
  Serial.println("");
}


void MapJoysticks() { // Maps joysticks for use with ESC
  if (Joystick1A > 600) {                                   //creates deadzones of a range of 88
    Map1A = map(Joystick1A, 600, 1023, 1500, 1900);
    Map1ACR = map(Joystick1A, 600, 1023, 1500, 1100);
  }
  if (Joystick1A < 424) {
    Map1A = map(Joystick1A, 0, 424, 1100, 1500);
    Map1ACR = map(Joystick1A, 0, 424, 1900, 1500);
  }
  if ((Joystick1A > 424) && (Joystick1A < 600)) {
    Map1A = 1500;
    Map1ACR = 1500;
  }

  if (Joystick1B > 600) {
    Map1B = map(Joystick1B, 600, 1023, 1500, 1900);
    Map1BCR = map(Joystick1B, 600, 1023, 1500, 1100);
  }
  if (Joystick1B < 424) {
    Map1B = map(Joystick1B, 0, 424, 1100, 1500);
    Map1BCR = map(Joystick1B, 0, 424, 1900, 1500);
  }
  if ((Joystick1B > 424) && (Joystick1B < 600)) {
    Map1B = 1500;
    Map1BCR = 1500;
  }

  if (Joystick1C > 600) {
    Map1C = map(Joystick1C, 600, 1023, 1500, 1900);
    Map1CCR = map(Joystick1C, 600, 1023, 1500, 1100);
  }
  if (Joystick1C < 424) {
    Map1C = map(Joystick1C, 0, 424, 1100, 1500);
    Map1CCR = map(Joystick1C, 0, 424, 1900, 1500);
  }
  if ((Joystick1C > 424) && (Joystick1C < 600)) {
    Map1C = 1500;
    Map1CCR = 1500;
  }  
  
  if (Joystick2A > 600) {
    Map2A = map(Joystick2A, 600, 1023, 1500, 1900);
    Map2ACR = map(Joystick2A, 600, 1023, 1500, 1100);
  }
  if (Joystick2A < 424) {
    Map2A = map(Joystick2A, 0, 424, 1100, 1500);
    Map2ACR = map(Joystick2A, 0, 424, 1900, 1500);
  }
  if ((Joystick2A > 424) && (Joystick2A < 600)) {
    Map2A = 1500;
    Map2ACR = 1500;
  }

  if (Joystick2B > 600) {
    Map2B = map(Joystick2B, 600, 1023, 1500, 1900);
    Map2BCR = map(Joystick2B, 600, 1023, 1500, 1100);
  }
  if (Joystick2B < 424) {
    Map2B = map(Joystick2B, 0, 424, 1100, 1500);
    Map2BCR = map(Joystick2B, 0, 424, 1900, 1500);
  }
  if ((Joystick2B > 424) && (Joystick2B < 600)) {
    Map2B = 1500;
    Map2BCR = 1500;
  }

  if (Joystick2C > 600) {
    Map2C = map(Joystick2C, 600, 1023, 1500, 1900);
    Map2CCR = map(Joystick2C, 600, 1023, 1500, 1100);
  }
  if (Joystick2C < 424) {
    Map2C = map(Joystick2C, 0, 424, 1100, 1500);
    Map2CCR = map(Joystick2C, 0, 424, 1900, 1500);
  }
  if ((Joystick2C > 424) && (Joystick2C < 600)) {
    Map2C = 1500;
    Map2CCR = 1500;
  }
}

void DrivingLight() {
  if ((BlueVal == 1) && (WhiteVal == 0)) {              //blue switch on, make light blue
    for (int i=0;i<14;i++) {
      Lights.setPixelColor(i, Lights.Color(0,0,255,0));
    }
  }
  else if ((WhiteVal == 1) && (BlueVal == 0)) {         //white switch on, make light white
    for (int i=0;i<14;i++) {
      Lights.setPixelColor(i, Lights.Color(0,0,0,255));
    }
  }
  else if ((WhiteVal == 1) && (BlueVal == 1)) {         //both switches on? make it red baby
    for (int i=0;i<14;i++) {
      Lights.setPixelColor(i, Lights.Color(255,0,0,0));
    }
  }
  else if ((WhiteVal == 0) && (BlueVal == 0)) {         //both switches off? kill that mo-fo
    for (int i=0;i<14;i++) {
      Lights.setPixelColor(i, Lights.Color(0,0,0,0));
    }
  }
  Lights.show();
}

void ValveTurner() {
  if (Valve1Val == 1) {
    digitalWrite (52, HIGH); // 13 is other
    delayMicroseconds(100);
    analogWrite (12, 255);
  }
  if (Valve2Val == 1) {
    digitalWrite (52, LOW); //53 is other
    delayMicroseconds(100);
    analogWrite(12, 255);
  }
  if ((Valve1Val == 0) && (Valve2Val == 0)) {
    digitalWrite (52, LOW);
    delayMicroseconds(100);
    analogWrite (12, 0);
  }
}

void Magnet() {
  if (MagnetVal == 1) {
    digitalWrite (53, LOW);
    delayMicroseconds(100);
    analogWrite (13, 255);
  }
  if (MagnetVal == 0) {
    digitalWrite (53, LOW);
    delayMicroseconds(100);
    analogWrite(13, 0);
  }
}

void FunLEDs() {      //accent lights to add later

}

void VertStabilization() {  //if we ever get around to stabilization it'll go here



}

