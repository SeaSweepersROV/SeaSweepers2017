

/* Sensor ROV RX Code for the 2017 Season
-----------------------------------------------------
Receiving sensor info using EasyTransfer library.

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


#include <EasyTransfer.h>

//create object
EasyTransfer ET;



struct RECEIVE_DATA_STRUCTURE{
  //put your variable definitions here for the data you want to receive
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

//give a name to the group of data
RECEIVE_DATA_STRUCTURE rxdata;

String fullBT = "";
String fullBTString = "";
String stringPartToConvert;

void setup()
{
  Serial.begin(9600);
  Serial2.begin(19200);
  ET.begin(details(rxdata), &Serial2);
}

void loop()
{
  receiveData();
  delay(10);
}

void receiveData(){

  if(ET.receiveData()){
        printData();
    }
}

void printData(){


  Serial.println("----");  

  Serial.println("pressure" + String(rxdata.pressure));

  Serial.println("outTemp" + String(rxdata.outTemp)); 

  Serial.println("depth" + String(rxdata.depth));

   Serial.println( "roll" + String(rxdata.roll));

  Serial.println( "pitch" + String(rxdata.pitch));

    Serial.println( "heading" + String(rxdata.heading));

 Serial.println( "leak" + String(rxdata.leak));

  Serial.println( "inTemp" + String(rxdata.inTemp));

   Serial.println( "humidity" + String(rxdata.humidity));



   Serial.print("bluetoothOne"); 
   Serial.println(String(rxdata.longBT1));
   Serial.print("bluetoothTwo");
   Serial.println(String(rxdata.longBT2));
   Serial.print("bluetoothThree");
   Serial.println(String(rxdata.longBT3));

//    fullBT = "77456789345678945678"
//   for(int i = 0; i <= 18; i++){
//          stringPartToConvert += fullBT[i];
//          if (i % 2 == 0){
//            Serial.write(stringPartToConvert.toInt());
//            stringPartToConvert = "";
//          }
//   }
    
      fullBT = "";
    Serial.println("");

    Serial.println( "voltage" + String(rxdata.voltage));
}

