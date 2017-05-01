/*
  Receiving Motor Code
  Adds mutliaxis support in order to make JP happy

  Written for the Sea Sweepers ROV Club
  Michael Georgariou 2017
*/

//valve 12
//okay 13
//accent light on 9

#include <EasyTransfer.h>
EasyTransfer ET;

struct RECEIVE_DATA_STRUCTURE {
  int data.Joystick1A;
  int data.Joystick1B;
  int data.Joystick1C;
  int data.Joystick2A;
  int data.Joystick2B;
  int data.Joystick2C;
  int WhiteVal;
  int BlueVal;
  int Valve1Val;
  int Valve2Val;
  int MagnetVal;
}

RECEIVE_DATA_STRUCTURE data;

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


int Map1A;      //map for servo
int Map1ACR;    //map for counter-rotating props
int Map1B;
int Map1BCR;
int Map1C;
int Map1CCR;
int Map2A;
int Map2ACR;
int Map2B;
int Map2BCR;
int Map2C;
int Map2CCR;


int VertVar; //to store number for stabilization

int Crab;   // to add stabilization possibly in future
int Roll;

void setup() {

  Serial.begin(19200); //initialize Serial
  Serial1.begin(19200);
  ET.begin(details(data), &Serial1);

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

  if (ET.receiveData()) {
    MapJoysticks();
    SerialPrint();
    MotorWriteBasic(); //replace with MA later
    DrivingLight();
    ValveTurner();
    Magner();
    FunLEDs();
  }

  delay(10);

  //  if () { //ADD SWTICH
  //    VertStabilization(); //stays completely flat (always on)
  //  }
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


void MotorWriteMA() { // Writes data.Joystick values to ESCs (multiaxis, not working correctly)

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
  Serial.print(data.BlueVal);
//  Serial.print(data.WhiteVal);
  Serial.println("");
}


void MapJoysticks() { // Maps data.Joysticks for use with ESC
  if (data.Joystick1A > 600) {                                   //creates deadzones of a range of 88
    Map1A = map(data.Joystick1A, 600, 1023, 1500, 1900);
    Map1ACR = map(data.Joystick1A, 600, 1023, 1500, 1100);
  }
  if (data.Joystick1A < 424) {
    Map1A = map(data.Joystick1A, 0, 424, 1100, 1500);
    Map1ACR = map(data.Joystick1A, 0, 424, 1900, 1500);
  }
  if ((data.Joystick1A > 424) && (data.Joystick1A < 600)) {
    Map1A = 1500;
    Map1ACR = 1500;
  }

  if (data.Joystick1B > 600) {
    Map1B = map(data.Joystick1B, 600, 1023, 1500, 1900);
    Map1BCR = map(data.Joystick1B, 600, 1023, 1500, 1100);
  }
  if (data.Joystick1B < 424) {
    Map1B = map(data.Joystick1B, 0, 424, 1100, 1500);
    Map1BCR = map(data.Joystick1B, 0, 424, 1900, 1500);
  }
  if ((data.Joystick1B > 424) && (data.Joystick1B < 600)) {
    Map1B = 1500;
    Map1BCR = 1500;
  }

  if (data.Joystick1C > 600) {
    Map1C = map(data.Joystick1C, 600, 1023, 1500, 1900);
    Map1CCR = map(data.Joystick1C, 600, 1023, 1500, 1100);
  }
  if (data.Joystick1C < 424) {
    Map1C = map(data.Joystick1C, 0, 424, 1100, 1500);
    Map1CCR = map(data.Joystick1C, 0, 424, 1900, 1500);
  }
  if ((data.Joystick1C > 424) && (data.Joystick1C < 600)) {
    Map1C = 1500;
    Map1CCR = 1500;
  }  
  
  if (data.Joystick2A > 600) {
    Map2A = map(data.Joystick2A, 600, 1023, 1500, 1900);
    Map2ACR = map(data.Joystick2A, 600, 1023, 1500, 1100);
  }
  if (data.Joystick2A < 424) {
    Map2A = map(data.Joystick2A, 0, 424, 1100, 1500);
    Map2ACR = map(data.Joystick2A, 0, 424, 1900, 1500);
  }
  if ((data.Joystick2A > 424) && (data.Joystick2A < 600)) {
    Map2A = 1500;
    Map2ACR = 1500;
  }

  if (data.Joystick2B > 600) {
    Map2B = map(data.Joystick2B, 600, 1023, 1500, 1900);
    Map2BCR = map(data.Joystick2B, 600, 1023, 1500, 1100);
  }
  if (data.Joystick2B < 424) {
    Map2B = map(data.Joystick2B, 0, 424, 1100, 1500);
    Map2BCR = map(data.Joystick2B, 0, 424, 1900, 1500);
  }
  if ((data.Joystick2B > 424) && (data.Joystick2B < 600)) {
    Map2B = 1500;
    Map2BCR = 1500;
  }

  if (data.Joystick2C > 600) {
    Map2C = map(data.Joystick2C, 600, 1023, 1500, 1900);
    Map2CCR = map(data.Joystick2C, 600, 1023, 1500, 1100);
  }
  if (data.Joystick2C < 424) {
    Map2C = map(data.Joystick2C, 0, 424, 1100, 1500);
    Map2CCR = map(data.Joystick2C, 0, 424, 1900, 1500);
  }
  if ((data.Joystick2C > 424) && (data.Joystick2C < 600)) {
    Map2C = 1500;
    Map2CCR = 1500;
  }
}

void DrivingLight() {
  if ((data.BlueVal == 1) && (data.WhiteVal == 0)) {              //blue switch on, make light blue
    for (int i=0;i<14;i++) {
      Lights.setPixelColor(i, Lights.Color(0,0,255,0));
    }
  }
  else if ((data.WhiteVal == 1) && (data.BlueVal == 0)) {         //white switch on, make light white
    for (int i=0;i<14;i++) {
      Lights.setPixelColor(i, Lights.Color(0,0,0,255));
    }
  }
  else if ((data.WhiteVal == 1) && (data.BlueVal == 1)) {         //both switches on? make it red baby
    for (int i=0;i<14;i++) {
      Lights.setPixelColor(i, Lights.Color(255,0,0,0));
    }
  }
  else if ((data.WhiteVal == 0) && (data.BlueVal == 0)) {         //both switches off? kill that mo-fo
    for (int i=0;i<14;i++) {
      Lights.setPixelColor(i, Lights.Color(0,0,0,0));
    }
  }
  Lights.show();
}

void ValveTurner() {
  if (data.Valve1Val == 1) {
    digitalWrite (52, HIGH); // 13 is other
    delayMicroseconds(100);
    analogWrite (12, 255);
  }
  if (data.Valve2Val == 1) {
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
  if (data.MagnetVal == 1) {
    digitalWrite (53, LOW);
    delayMicroseconds(100);
    analogWrite (13, 255);
  }
  if (data.MagnetVal == 0) {
    digitalWrite (53, LOW);
    delayMicroseconds(100);
    analogWrite(13, 0);
  }
}

void FunLEDs() {      //accent lights to add later

}

void VertStabilization() {  //if we ever get around to stabilization it'll go here



}
