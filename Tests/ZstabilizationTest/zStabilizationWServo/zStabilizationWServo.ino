

#include <Servo.h>
#include <Wire.h>
#include "MS5837.h"

MS5837 sensor;
Servo myservo;  // create servo object to control a servo
int val;    // variable to read the value from the analog pin

void setup() {
  
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
    Serial.begin(9600);

  Serial.println("Starting");

  Wire.begin();

  sensor.init();

  sensor.setFluidDensity(997); // kg/m^3 (997 freshwater, 1029 for seawater) (Use 1001 for chlorine pool)
}


void loop() {

  sensor.read();
   
  Serial.print(sensor.depth());
  Serial.println(" m deep");
  
  val = sensor.depth() * 100; 
  Serial.print("Val after assigning: " );
  Serial.print(val);
  Serial.println(" cm" );
  
  val = map(val,-20,0, 0, 180);     // scale it to use it with the servo (value between 0 and 180 for )
  myservo.write(val);                  // sets the servo position according to the scaled value
  Serial.println(val);
  
  delay(1000);

}

