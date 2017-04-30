/*
  Transmitting Motor Code
  Takes numbers from joysticks and sends them through the Serial1 Port
  It's alright I guess

  Written for the Sea Sweepers ROV Club
  Michael Georgariou 2017
*/

int data [12];    // to send bytes
int start [2];

int Pin1A = A2;           
int Joystick1A;   // crab

int Pin1B = A1;
int Joystick1B;   // up down numbers are backwards

int Pin1C = A0;
int Joystick1C;   // pitch

int Pin2A = A3;          
int Joystick2A;   // up down broken

int Pin2B = A4;   // yaw r-l
int Joystick2B;

int Pin2C = A5;
int Joystick2C;   // forward-back   fdsafdsaf

int BlueSwitch = 22;
int BlueVal;
int BlueState;
int WhiteSwitch = 24;
int WhiteVal;
int WhiteState;

unsigned char checksum0;
unsigned char checksum1;
unsigned char checksum2;
unsigned char checksum3;
unsigned char checksum4;
unsigned char checksum5;
unsigned char checksum6;
unsigned char checksum7;
unsigned char handshake;
unsigned char handshake2;
unsigned char handshake3;
unsigned char handshake4;
unsigned char handshake5;
unsigned char handshake6;

void setup() {
  Serial.begin(19200);
  Serial1.begin(19200);

  pinMode(BlueSwitch, INPUT);
  pinMode(WhiteSwitch, INPUT);
  delay(100);

}

void loop() {
  ReadandMap();
  Checksums();
  Switches();
  Communication();
  delay(40);
}

void ReadandMap () {
  Joystick1A = analogRead(Pin1A);   //reads joysticks and splits them into bytes
  data[0] = Joystick1A & 0xFF;
  data[1] = (Joystick1A >> 8);
  
  Joystick1B = analogRead(Pin1B);
  data[2] = Joystick1B & 0xFF;
  data[3] = (Joystick1B >> 8);
  
  Joystick1C = analogRead(Pin1C);
  data[4] = Joystick1C & 0xFF;
  data[5] = (Joystick1C >> 8);
  
  Joystick2A = analogRead(Pin2A);
  data[6] = Joystick2A & 0xFF;
  data[7] = (Joystick2A >> 8);
  
  Joystick2B = analogRead(Pin2B);
  data[8] = Joystick2B & 0xFF;
  data[9] = (Joystick2B >> 8);

  Joystick2C = analogRead(Pin2C);
  data[10] = Joystick2C & 0xFF;      //least significant 8 bit byte
  data[11] = (Joystick2C >> 8);
}

void Checksums() {
  checksum0 = ~(data[0] + data[1]) + 1;   //creates checksums for the RX code to check
  checksum1 = ~(data[2] + data[3]) + 1;
  checksum2 = ~(data[4] + data[5]) + 1;
  checksum3 = ~(data[6] + data[7]) + 1;
  checksum4 = ~(data[8] + data[9]) + 1;
  checksum5 = ~(data[10] + data[11]) + 1;
}

void Switches() {
  WhiteState = digitalRead(WhiteSwitch);
  BlueState = digitalRead(BlueSwitch);

  if (BlueState == HIGH) {
    BlueVal = 1;
  }
  if (BlueState == LOW) {
    BlueVal = 0;
  }

  if (WhiteState == HIGH) {
    WhiteVal = 1;
  }
  if (WhiteState == LOW) {
    WhiteVal = 0;
  }
}

void Communication() {
  while (Serial1.available() < 3) {   //wait to get a handshake
    Serial.println("No Communication"); //wait for request from receiver
  }
  
  handshake = Serial1.read();
  handshake2 = Serial1.read();
  handshake3 = Serial1.read();

  if ((handshake == 1) && (handshake2 == 2) && (handshake3 == 3)) {  //check that the handshakes are equal
    Serial.println("Handshakes Equal");
    Serial1.write(1);
    Serial1.write(data[0]);
    Serial1.write(data[1]);
    Serial1.write(checksum0);

    Serial1.write(data[2]);
    Serial1.write(data[3]); //bytes sent
    Serial1.write(checksum1);

    Serial1.write(data[4]);
    Serial1.write(data[5]);
    Serial1.write(checksum2);

    Serial1.write(data[6]);
    Serial1.write(data[7]);
    Serial1.write(checksum3);

    Serial1.write(data[8]);
    Serial1.write(data[9]); //bytes sent
    Serial1.write(checksum4);

    Serial1.write(data[10]);
    Serial1.write(data[11]);
    Serial1.write(checksum5);

    Serial1.write(WhiteVal);
    Serial1.write(BlueVal);

    Serial1.write(2);

  }
  else {
    Serial.println("Handshakes Not Equal");
  }
}

