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
};

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
int MagnetSwitch = 32;
int MagnetState;
int SensSwitch = 30; // to adjust sensitivity to half
int SensState;


void setup() {
  Serial.begin(19200);
  Serial1.begin(19200);
  ET.begin(details(data), &Serial1);

  pinMode(BlueSwitch, INPUT);
  pinMode(WhiteSwitch, INPUT);
  pinMode(Valve1Switch, INPUT);
  pinMode(Valve2Switch, INPUT);
  pinMode(MagnetSwitch, INPUT);
  pinMode(SensSwitch, INPUT);
  
  delay(100);
}

void loop() {
  ReadandMap();
  Switches();
  ET.sendData();
  delay(10);
}

void ReadandMap () {
  SensState = digitalRead(SensSwitch);
  if (SensState == LOW) {
    data.Joystick1A = (((2*(analogRead(Pin1A))) / 5) + 308);  //if switch is on, half joystick values that are sent
    data.Joystick1B = (((2*(analogRead(Pin1B))) / 5) + 308);  
    data.Joystick1C = (((2*(analogRead(Pin1C))) / 5) + 308);
    data.Joystick2A = (((2*(analogRead(Pin2A))) / 5) + 308);
    data.Joystick2B = (((2*(analogRead(Pin2B))) / 5) + 308);
    data.Joystick2C = (((2*(analogRead(Pin2C))) / 4) + 256);  // SPECIAL CASE: Crab was too weak, so its joystick values will be higher
  }
  if (SensState == HIGH) {
    data.Joystick1A = (((3*(analogRead(Pin1A))) / 4) + 128);
    data.Joystick1B = (((3*(analogRead(Pin1B))) / 4) + 128);
    data.Joystick1C = (((3*(analogRead(Pin1C))) / 4) + 128);
    data.Joystick2A = (((3*(analogRead(Pin2A))) / 4) + 128);
    data.Joystick2B = (((3*(analogRead(Pin2B))) / 4) + 128);
    data.Joystick2C = (((3*(analogRead(Pin2C))) / 4) + 128);
  }
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

  Valve1State = digitalRead(Valve1Switch);
  Valve2State = digitalRead(Valve2Switch);

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
  if (MagnetState == HIGH) {
    data.MagnetVal = 1;
  }
  if (MagnetState == LOW) {
    data.MagnetVal = 0;
  }
}
