/* Sensor ROV TX Code for the 2017 Season
-----------------------------------------------------
NEWW AND UPDATEDD

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
#include <EasyTransfer.h>

//create object
EasyTransfer ET;

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

struct SEND_DATA_STRUCTURE{
  //put your variable definitions here for the data you want to send
  //THIS MUST BE EXACTLY THE SAME ON THE OTHER ARDUINO
//Declare Variables
  float leak;
  float pressure;
  float outTemp;
  float depth;
  float roll;
  float pitch;
  float heading;
  float inTemp;
  float humidity;
  float bluetooth;
  float voltage;
};

//give a name to the group of data
SEND_DATA_STRUCTURE txdata;

    String str;
    
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
    sensor.setFluidDensity(997); // kg/m^3 (997 freshwater, 1029 for seawater)
    ET.begin(details(txdata), &Serial2);
}

void loop()
{
    receiveData();
    delay(100);
}

void receiveData(){
  
 sensors_vec_t   orientation;


     sensor.read();


  txdata.pressure = sensor.pressure();

  txdata.outTemp = sensor.temperature(); 

  txdata.depth = sensor.depth();

  
  if (ahrs.getOrientation(&orientation))
  {

    txdata.roll = orientation.roll;

    txdata.pitch = orientation.pitch;

    txdata.heading = orientation.heading;

  }


   int leakState = digitalRead(SOSPIN);   // read the input pin
      
  if (leakState == HIGH) {              // prints "LEAK!" if input pin is high
    txdata.leak = 1;

  }
  else if (leakState == LOW) {       // prints "Dry" if input pin is low
    txdata.leak = 0;
  }

      
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) ) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  
     txdata.inTemp = t;


     txdata.humidity = h;


     // Keep reading from HC-05 and send to Arduino Serial Monitor
    if (BTserial.available())
    {  
        c = BTserial.read();
        txdata.bluetooth = c;
    }
    else{
      txdata.bluetooth = 0000000;
    }


    int val = 0;
    val = analogRead(A0);
    val = map(val, 0, 822, 0, 48);
    if(isnan(val))
      txdata.voltage = 000;
    else
      txdata.voltage = val;

        ET.sendData();
}



