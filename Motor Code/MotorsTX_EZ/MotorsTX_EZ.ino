/*
  Transmitting Motor Code
  Takes numbers from joysticks and sends them through the Serial1 Port
  It's alright I guess

  Written for the Sea Sweepers ROV Club
  Michael Georgariou 2017
*/

#include <EasyTransfer.h>
EasyTransfer ET;

struct SEND_DATA_STRUCTURE {
  int Joystick1A;
  int Joystick1B;
  int Joystick1C;
  int Joystick2A;
  int Joystick2B;
  int Joystick2C;
  int WhiteVal;
  int BlueVal;
  int Valve1Val;
  int Valve2Val;
  int MagnetVal;
}

SEND_DATA_STRUCTURE data;

int Pin1A = A2;           
int Pin1B = A1;
int Pin1C = A0;
int Pin2A = A3;          
int Pin2B = A4;   // yaw r-l
int Pin2C = A5;

int BlueSwitch = 22;
int BlueState;
int WhiteSwitch = 24;
int WhiteState;
int Valve1Switch = 26;
int Valve1State;
int Valve2Switch = 28;
int Valve2State;
int MagnetSwitch = 30;
int MagnetState;


void setup() {
  Serial.begin(19200);
  Serial1.begin(19200);
  ET.begin(details(data), &Serial1);

  pinMode(BlueSwitch, INPUT);
  pinMode(WhiteSwitch, INPUT);
  pinMode(Valve1Switch, INPUT);
  pinMode(Valve2Switch, INPUT);
  pinMode(MagnetSwitch, INPUT);
  
  delay(100);
}

void loop() {
  ReadandMap();
  Switches();
  ET.sendData();
  delay(40);
}

void ReadandMap () {
  data.Joystick1A = analogRead(Pin1A);   //reads joysticks and splits them into bytes
  data.Joystick1B = analogRead(Pin1B);
  data.Joystick1C = analogRead(Pin1C);
  data.Joystick2A = analogRead(Pin2A);
  data.Joystick2B = analogRead(Pin2B);
  data.Joystick2C = analogRead(Pin2C);
}


void Switches() {
  WhiteState = digitalRead(WhiteSwitch);
  BlueState = digitalRead(BlueSwitch);

  if (BlueState == HIGH) {
    data.BlueVal = 1;
  }
  if (BlueState == LOW) {
    data.BlueVal = 0;
  }

  if (WhiteState == HIGH) {
    data.WhiteVal = 1;
  }
  if (WhiteState == LOW) {
    data.WhiteVal = 0;
  }

  Valve1State = digitalRead(Valve1);
  Valve2State = digitalRead(Valve2);

  if (Valve1State == HIGH) {
    data.Valve1Val = 1;
  }
  if (Valve1State == LOW) {
    data.Valve1Val = 0;
  }
  if (Valve2State == HIGH) {
    data.Valve2Val = 1;
  }
  if (Valve2State == LOW) {
    data.Valve2Val = 0;
  }

  MagnetState = digitalRead(MagnetSwitch);
  if (MagnetSwitch == HIGH) {
    data.MagnetVal = 1;
  }
  if (MagnetSwitch == LOW) {
    data.MagnetVal = 0;
  }
}
