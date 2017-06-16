/* Sensor ROV TX Code for the 2017 Season
-----------------------------------------------------

The MIT License (MIT)

Copyright (c) Sea Sweepers ROV

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
-------------------------------*/
#include <Wire.h>
#include "MS5837.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
#include <Adafruit_Simple_AHRS.h>
#include "DHT.h"
#include <SoftwareSerial.h>

SoftwareSerial BTserial(10, 11); // RX | TX
char c = ' ';

MS5837 sensor;

// Create sensor instances.
Adafruit_LSM303_Accel_Unified accel(30301);
Adafruit_LSM303_Mag_Unified   mag(30302);

// Create simple AHRS algorithm using the above sensors.
Adafruit_Simple_AHRS          ahrs(&accel, &mag);

// Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, public domain

#define DHTPIN 23   // what digital pin we're connected to
#define DHTTYPE DHT11   // DHT 11
#define SOSPIN 3   // SOS connected to digital pin 3

DHT dht(DHTPIN, DHTTYPE);

//Declare Variables
  String leak;
  String pressure;
  String outTemp;
  String depth;
  String roll;
  String pitch;
  String heading;
  String inTemp;
  String humidity;
  String bluetooth;
  String voltage;
  
void setup()
{
  Serial.begin(9600);
  Serial2.begin(19200);
  BTserial.begin(38400); 
  
  pinMode(SOSPIN, INPUT);    // sets the digital pin 3 as input
    dht.begin();
  
  // Initialize the sensors.
  accel.begin();
  mag.begin();
  sensor.init();
  // Set liquid density
  sensor.setFluidDensity(997); // kg/m^3 (997 freshwater, 1029 for seawater)
  delay(1000);
  
}

void loop()
{
    receiveData();
    printData();
}

void receiveData(){
  
 sensors_vec_t   orientation;

  // Use the simple AHRS function to get the current orientation.

     sensor.read();


  pressure = "pressure" + String(sensor.pressure());

  outTemp = "outTemp" + String(sensor.temperature()); 

  depth = "depth" + String(sensor.depth());

  
  if (ahrs.getOrientation(&orientation))
  {

    roll = "roll" + String(orientation.roll);

    pitch = "pitch" + String(orientation.pitch);

    heading = "heading" + String(orientation.heading);

  }


   int leakState = digitalRead(SOSPIN);   // read the input pin
      
  if (leakState == HIGH) {              // prints "LEAK!" if input pin is high
    leak = "leak1";

  }
  else if (leakState == LOW) {       // prints "Dry" if input pin is low
    leak = "leak0";

  }

      
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) ) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  
     inTemp = "inTemp" + String(t);


     humidity = "humidity" + String(h);


     // Keep reading from HC-05 and send to Arduino Serial Monitor
    if (BTserial.available())
    {  
        c = BTserial.read();
        bluetooth = "bluetooth" + String(c);
    }
    else{
      bluetooth = "bluetooth--";
    }


    float val = 0;
    val = analogRead(A0);
    val = map(val, 0, 822, 0, 48);
    if(isnan(val))
      voltage = "voltagebaddata";
    else
      voltage = "voltage" + String(val);
}
void printData(){
  delay(300);
  Serial2.println(leak + '\n');
  delay(300);
  Serial2.println(pressure + '\n');
  delay(300);
  Serial2.println(outTemp + '\n');
  delay(300);
  Serial2.println(depth + '\n');
  delay(300);
  Serial2.println(roll  + '\n');
  delay(300);
  Serial2.println(pitch  + '\n');
 delay(300);
  Serial2.println(heading  + '\n');
  delay(300);
  Serial2.println(inTemp  + '\n');
  delay(300);
  Serial2.println(humidity  + '\n');
  delay(300);
  Serial2.println(bluetooth + '\n');
  delay(200);
  Serial2.println(voltage  + '\n');

  
}


