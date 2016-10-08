#include <OneWire.h> 


#include <Wire.h>
#include "MS5837.h"
//temp probe
int DS18S20_Pin = 2;
OneWire ds(DS18S20_Pin);
//pressure sensor
MS5837 sensor;

//code for inside temp sensor
#include "DHT.h" //library
#define DHTPIN 7  // what digital pin we're connected to for temperature sensor
#define DHTTYPE DHT11   // DHT 11 (AM2301)
DHT dht(DHTPIN, DHTTYPE); //set varaibles

//voltage setup
int val11; 
int val2; 
// bytes to send
int data [46];    
//starting bytes
int start [2];    
//analog pins
int totalVoltPin = A0;
int totalAmpPin = A1;
int waterSenseOnePin = A4;
int waterSenseTwoPin = A5;

// variable to store the value coming from the sensor
int totalVolt = 0;
int totalAmp = 0;

//water sensors
int waterOne = 0;
int waterTwo = 0;

unsigned char insideTemperatureCheck;
unsigned char checksum1;
unsigned char checksum2;
unsigned char depthCheck;
unsigned char checksum4;
unsigned char pressureCheck;
unsigned char probeTemperatureCheck;
unsigned char totalVoltCheck;
unsigned char totalAmpCheck;
unsigned char xAccelCheck;
unsigned char yAccelCheck;
unsigned char zAccelCheck;
unsigned char angleCheck;
unsigned char waterSenseOneCheck;
unsigned char waterSenseTwoCheck;
unsigned char handshake;
unsigned char handshake2;
unsigned char handshake3;


void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);
  Wire.begin();  //pressure sensor
  sensor.init();
  sensor.setFluidDensity(997);
   dht.begin();   //inside temp sensor

} 

void loop() {
  //DHT21 read inside temp sensor
  float hum = dht.readHumidity();
  float inTemp = dht.readTemperature();
  int checkNumber;
  //inside temp sensor check
  checkNumber = checkSensorDHT21(hum,inTemp);
  if (checkNumber == 1) {
    hum = 999;
    inTemp = 999;
  }
  sensor.read();

  float alt = sensor.altitude();
  float dep = sensor.depth();
  int dataDepth = dep* 100;

  float outTemp = sensor.temperature();
  float pre = sensor.pressure();

  // READ THE SENSORS:

  //DHT21 read inside temp sensor
  int insideTemperature = inTemp;
  data[0] = insideTemperature & 0xFF;      //least significant 8 bit byte
  data[1] = (insideTemperature >> 8);      //most significant 2 bits
  
  int insideHumidity = hum;
  data[2] = insideHumidity & 0xFF;
  data[3] = (insideHumidity >> 8);
 
  int altitude = -1* alt;
  data[4] = altitude & 0xFF;
  data[5] = (altitude >> 8);
  
  int depth = dep * 100;   // reads the value of the potentiometer (Joystick2B between 0 and 1023)
  data[6] = depth & 0xFF;      
  data[7] = (depth >> 8);      
 
  int outsideTemperature = -1*outTemp;
  data[8] = outsideTemperature & 0xFF;
  data[9] = (outsideTemperature >> 8);

  int pressure = pre;
  data[10] = pressure & 0xFF;
  data[11] = (pressure >> 8);

  int probeTemperature = getTemp() * 100;
  data[12] = probeTemperature & 0xFF;
  data[13] = (probeTemperature >> 8);

//volts and amps
  float t; 
   val11=analogRead(1); 
  t=val11/4.092; 
  val11=(int)t;// 
  val2=((val11%100)/10);
  totalVolt = val2;
  data[30] = totalVolt & 0xFF;
  data[31] = (totalVolt >> 8);

  totalAmp = 12;
  data[32] = totalAmp & 0xFF;
  data[33] = (totalAmp >> 8);

  int XAcceleration = 2;
  data[34] = XAcceleration & 0xFF;
  data[35] = (XAcceleration >> 8);

  int YAcceleration = 2;
  data[36] = YAcceleration & 0xFF;
  data[37] = (YAcceleration >> 8);

  int ZAcceleration = 2;
  data[38] = ZAcceleration & 0xFF;
  data[39] = (ZAcceleration >> 8);

  float Pi = 3.14159;
  int angle = 55; //variable for compass
    if (angle < 0)
  {
    angle = 360 + angle;
  }
  data[40] = angle & 0xFF;
  data[41] = (angle >> 8);

int waterOne = 0;
int waterTwo = 0;
  waterOne = analogRead(waterSenseOnePin);
  data[42] = waterOne & 0xFF;
  data[43] = (waterOne >> 8);

  waterTwo = analogRead(waterSenseTwoPin);
  data[44] = waterTwo & 0xFF;
  data[45] = (waterTwo >> 8);
  
  // CREATE CHECKSUMS:

  insideTemperatureCheck = ~(data[0] + data[1]) + 1;

  checksum1 = ~(data[2] + data[3]) + 1;

  checksum2 = ~(data[4] + data[5]) + 1;

  depthCheck = ~(data[6] + data[7]) + 1;

  //checksum4 = ~(data[8] + data[9]) + 1;

  pressureCheck = ~(data[10] + data[11]) + 1;
  
  probeTemperatureCheck = ~(data[12] + data[13]) + 1;

  totalVoltCheck = ~(data[30] + data[31]) + 1;

  totalAmpCheck = ~(data[32] + data[33]) + 1;

  xAccelCheck = ~(data[34] + data[35]) + 1;

  yAccelCheck = ~(data[36] + data[37]) + 1;

  zAccelCheck = ~(data[38] + data[39]) + 1;
  
  angleCheck = ~(data[40] + data[41]) + 1;

  waterSenseOneCheck = ~(data[42] + data[43]) + 1;

  waterSenseTwoCheck = ~(data[44] + data[45]) + 1;

  // WRITE VALUES AND CHECKSUMS TO SERIAL:
  Serial.print(insideTemperature);
  Serial.print(" ");
  //Serial.print(insideHumidity);
  //Serial.print(" ");
  //Serial.print(altitude);
  //Serial.print(" ");
  Serial.print(depth);
  Serial.print(" ");
  //Serial.print(outsideTemperature);
  //Serial.print(" ");
  Serial.print(pressure);
  Serial.print(" ");
  Serial.print(probeTemperature);
  Serial.print(" ");
  Serial.print(totalVolt);
  Serial.print(" ");
  Serial.print(totalAmp);
  Serial.print(" ");
  Serial.print(XAcceleration);
  Serial.print(" ");
  Serial.print(YAcceleration);
  Serial.print(" ");
  Serial.print(ZAcceleration);
  Serial.print(" ");
  Serial.print(angle);
  Serial.print(" ");
  Serial.print(waterOne);
  Serial.print(" ");
  Serial.println(waterTwo);
  
    while (Serial1.available() < 3) {
    ; //wait for request from receiver
  }
  handshake = Serial1.read();
  handshake2 = Serial1.read();
  handshake3 = Serial1.read();

  if ((handshake == 1) && (handshake2 == 2) && (handshake3 == 3)) {    
    Serial1.write(255);
    Serial1.write(255);
    Serial1.write(255);

    Serial1.write(data[0]);    
    Serial1.write(data[1]);
    Serial1.write(insideTemperatureCheck);
    
    Serial1.write(data[6]);
    Serial1.write(data[7]);
    Serial1.write(depthCheck);
    
    Serial1.write(data[10]);
    Serial1.write(data[11]);
    Serial1.write(pressureCheck);
    
    Serial1.write(data[12]);
    Serial1.write(data[13]);
    Serial1.write(probeTemperatureCheck);

    Serial1.write(data[30]);
    Serial1.write(data[31]);
    Serial1.write(totalVoltCheck);

    Serial1.write(data[32]);
    Serial1.write(data[33]);
    Serial1.write(totalAmpCheck);

    Serial1.write(data[34]);
    Serial1.write(data[35]);
    Serial1.write(xAccelCheck);

    Serial1.write(data[36]);    
    Serial1.write(data[37]); 
    Serial1.write(yAccelCheck);
    
    Serial1.write(data[38]);
    Serial1.write(data[39]);
    Serial1.write(zAccelCheck);

    Serial1.write(data[40]);
    Serial1.write(data[41]);
    Serial1.write(angleCheck);
    
    Serial1.write(data[42]);
    Serial1.write(data[43]);
    Serial1.write(waterSenseOneCheck);
    
    Serial1.write(data[44]);
    Serial1.write(data[45]);
    Serial1.write(waterSenseTwoCheck);
    
    delay(50);     // delay in between reads for stability
  }

}
//function for getting probe temp
int getTemp(){
 //returns the temperature from one DS18S20 in DEG Celsius

 byte data[12];
 byte addr[8];

 if ( !ds.search(addr)) {
   //no more sensors on chain, reset search
   ds.reset_search();
   return 999;
 }

 if ( OneWire::crc8( addr, 7) != addr[7]) {
   //Serial1.writeln("CRC is not valid!");
   return 999;
 }

 if ( addr[0] != 0x10 && addr[0] != 0x28) {
   Serial.print("Device is not recognized");
   return 999;
 }

 ds.reset();
 ds.select(addr);
 ds.write(0x44,1); // start conversion, with parasite power on at the end

 byte present = ds.reset();
 ds.select(addr);  
 ds.write(0xBE); // Read Scratchpad

 
 for (int i = 0; i < 9; i++) { // we need 9 bytes
  data[i] = ds.read();
 }
 
 ds.reset_search();
 
 byte MSB = data[1];
 byte LSB = data[0];

 float tempRead = ((MSB << 8) | LSB); //using two's compliment
 float TemperatureSum = tempRead / 16;
 
 return TemperatureSum;
 
}
int checkSensorDHT21(float h,float t) {
  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return 1;
  }
  return 0;
}    
