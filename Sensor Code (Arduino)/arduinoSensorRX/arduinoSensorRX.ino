/* Sensor ROV RX Code for the 2017 Season
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
char c = ' ';

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
  String str;

void setup()
{
  Serial.begin(9600);
  Serial2.begin(19200);
  
}

void loop()
{
  receiveData();
}

void receiveData(){
   
    if (Serial2.available() > 0)
    {
          str = Serial2.readStringUntil('\n');
          //Serial.println(str);
    }
    
    if(str.startsWith("leak")){
      leak = str;
    }
    else if(str.startsWith("pressure")){
      pressure = str;
    }
    else if(str.startsWith("outTemp")){
      outTemp = str;
    }
    else if(str.startsWith("depth")){
      depth = str;
    }
    else if(str.startsWith("roll")){
      roll = str;
    }
    else if(str.startsWith("pitch")){
      pitch = str;
    }
    else if(str.startsWith("heading")){
      heading = str;
    }
    else if(str.startsWith("inTemp")){
      inTemp = str;
    }
    else if(str.startsWith("humidity")){
      humidity = str;
    }
    else if(str.startsWith("bluetooth")){
      bluetooth = str;
    }
    else if(str.startsWith("voltage")){
      voltage = str;
    }
    Serial.println(str);
    if(leak.length() != 0 && pressure.length() != 0 && outTemp.length() != 0 && depth.length() != 0 && roll.length() != 0 && pitch.length() != 0 && heading.length() != 0 && inTemp.length() != 0 && humidity.length() != 0 && bluetooth.length() != 0 && voltage.length() != 0){
        printData();
    }
    
    str = "";
    
}

void printData(){

  Serial.println("----");

  Serial.println(leak); 

  Serial.println(pressure);

  Serial.println(outTemp);

  Serial.println(depth);

  Serial.println(roll);

  Serial.println(pitch);

  Serial.println(heading);

  Serial.println(inTemp);

  Serial.println(humidity);

  Serial.println(bluetooth);

  Serial.println(voltage);   
  
}

