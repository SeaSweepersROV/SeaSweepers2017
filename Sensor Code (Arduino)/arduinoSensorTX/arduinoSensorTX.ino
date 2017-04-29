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

//receive data 
//print data

void setup()
{
  Serial.begin(9600);
  BTserial.begin(38400); 
  
  pinMode(SOSPIN, INPUT);    // sets the digital pin 3 as input
    dht.begin();
  
  // Initialize the sensors.
  accel.begin();
  mag.begin();
  sensor.init();
  // Set liquid density
  sensor.setFluidDensity(997); // kg/m^3 (997 freshwater, 1029 for seawater)
}

void loop()
{
    recieveData();
    printData();
    delay(300);
}

void recieveData(){
  
      sensors_vec_t   orientation;

      sensor.read();

      float pressure = sensor.pressure();

      float outTemp = sensor.temperature();

      float depth = sensor.depth();

      depth = max(sensor.depth(), 0);

  
  if (ahrs.getOrientation(&orientation))
  {

    float roll = orientation.roll;

    float pitch = orientation.pitch;

    float heading = orientation.heading;

  }


   int leakState = digitalRead(SOSPIN);   // read the input pin
      
  if (leakState == HIGH) {              //  1 if input pin is high
    float leak = 1;
  }
  
  else if (leakState == LOW) {       //  0 if input pin is low
    float leak = 0;
  }

      
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t) ) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
     float inTemp = t;

     float humidity h;

     // Keep reading from HC-05 and send to Arduino Serial Monitor
    if (BTserial.available())
    {  
        c = BTserial.read();
        Serial.println("bluetooth" + String(c));
    }
    else{
      Serial.println("bluetooth--");
    }

    //ADD VOLTAGE HERE
    int val = 0;
    val = analogRead(A0);

    val = map(val, 0, 822, 0, 48);
    if(isnan(val))
      Serial.println("voltagebaddata");
    else
      Serial.println("voltage" + String(val));

}

void printData(){

 
}

