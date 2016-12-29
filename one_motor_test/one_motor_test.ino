/*
Used to test one motor.
Attach the motor's ESC to pin 2 and a potentiometer to pin 0.
Potentiometer halfway will be a motor at neutral, clockwise will go forward and counter-clockwise will go backward.
 */

#include <Servo.h>
Servo Motor;

int Pot;
int PotMap;

void setup() {
  Motor.attach(2);
  Serial.begin(9600);

}

void loop() {
Pot = analogRead(A0);
PotMap = map(Pot, 0, 1023, 1100, 1900);
Serial.println(Pot);

Motor.write(PotMap);

}
