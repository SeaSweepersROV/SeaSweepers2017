
/* Sensor ROV TX Code for the 2017 Season
-----------------------------------------------------
Transmitting sensor info using EasyTransfer library.

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
   long longBT1;
   long longBT2;
   long longBT3;
  float voltage;
};
    String cc;
//give a name to the group of data
SEND_DATA_STRUCTURE txdata;

    String str;
    
void setup()
{

  Serial.begin(9600);
  Serial2.begin(19200);
  BTserial.begin(9600); 
  
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
    delay(50);
}

void receiveData(){
          
  //Serial.print("Hello");
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


  //    Keep reading from HC-05 and send to Arduino Serial Monitor
  /**
   * START BLUETOOTH
   */
    String BT1;
    String BT2;
    String BT3;
    int BTcount = 0;
    String newStr = "";
    bool found = false;
    
    
    BTserial.begin(9600);
    if (BTserial.available())
    {
       cc = "";
        while(true){
         c = BTserial.read();
         if(cc.length() >= 7|| int(c) <= -1){
            break;
         }
         else
              cc += String(c);

            Serial.println(c);
            Serial.println(cc);
            
        }
        
        BTserial.end();
        //delay(79);
        Serial.println("exit");


//        c = BTserial.read();
//        
//        if( c == '\n'){
//        String f = BTserial.readStringUntil('\n');
//        Serial.println(f);
//        }
//        BTserial.end();

    //    cc = String(c);
        Serial.print("Cc: ");
        Serial.println(cc);
        
        for (int i = 0; i <= 10; i++)
        {
            newStr = newStr + int(cc[i]);
            Serial.print("ASCII: ");
            Serial.print(int(cc[i]));
            if(BTcount <= 2)
                BT1 = BT1 + int(cc[i]);
                
            if(BTcount >= 3 && BTcount <= 5)
                BT2 = BT2 + int(cc[i]);
    
            if(BTcount >= 6 && BTcount <= 8)
                BT3 = BT3 + int(cc[i]);
            BTcount += 1;
         }
         
            Serial.print(" String BT1: ");
            Serial.print(BT1);
            char charBuf1[50];
            BT1.toCharArray(charBuf1, 50);
            
            Serial.print("Char buff1: ");
            Serial.print(charBuf1);
            txdata.longBT1 = atol(charBuf1);
            
            Serial.print(" longBT1: ");
            Serial.print(txdata.longBT1);
        
        
        //
        
             Serial.print(" String BT2: ");
            Serial.print(BT2);
            char charBuf2[50];
            BT2.toCharArray(charBuf2, 50) ;
            
            Serial.print("Char buff2: ");
            Serial.print(charBuf2);
            txdata.longBT2 = atol(charBuf2);
            
            Serial.print(" longBT2: ");  
            Serial.print(txdata.longBT2);
        
        
        //
        
            Serial.print(" String BT3: ");
            Serial.print(BT3);
            char charBuf3[50];
            BT3.toCharArray(charBuf3, 50) ;
            
            Serial.print("Char buff3: ");
            Serial.print(charBuf3);
            txdata.longBT3 = atol(charBuf3);
            
            Serial.print(" longBT3: ");
            Serial.print(txdata.longBT3); 
            

    }
    
 /**
  * END BLUETOOTH
  */

Serial.print("Getting out: ");

    int val = 0;
    val = analogRead(A0);
    val = map(val, 0, 822, 0, 48);
    if(isnan(val))
      txdata.voltage = 000;
    else
      txdata.voltage = val;

        ET.sendData();
}



