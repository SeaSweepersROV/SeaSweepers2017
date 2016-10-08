#include <Wire.h>
#include "MS5837.h"

MS5837 sensor;

void setup() {
  
  //pinMode(##buttonPin##, INPUT);
  Serial.begin(9600);
  
  Serial.println("Starting");
  
  Wire.begin();

  sensor.init();
  
  sensor.setFluidDensity(1001); // kg/m^3 (997 freshwater, 1029 for seawater)
}

void loop() {

  //float depthSavedState = 0;
  //bool lockedZ = false;
  sensor.read();

  Serial.print("Pressure: "); 
  Serial.print(sensor.pressure()); 
  Serial.println(" mbar");
  
  Serial.print("Temperature: "); 
  Serial.print(sensor.temperature()); 
  Serial.println(" deg C");
  
  Serial.print("Depth: "); 
  Serial.print(sensor.depth()); 
  Serial.println(" m deep");
  
  Serial.print("Altitude: "); 
  Serial.print(sensor.altitude()); 
  Serial.println(" m above mean sea level");

  /*
  buttonState = digitalRead(##buttonPin##);
  // if putton pressed, ACTIVATE Z Stabalization
  if(buttonState == HIGH && !lockedZ){
    depthSavedState = sensor.depth();
    lockedZ = true;
  }
  // if putton pressed, DEACTIVATE Z Stabalization
  if(buttonState == HIGH && lockedZ){
    depthSavedState = sensor.depth();
    lockedZ = false;
  }
    // if Z STABLAIZATION = ACTIVE
    while(lockedZ){
    // if ROV is above saved state
      if(sensor.depth() > depthSavedState){
          // Move ROV DOWN
          // Insert MOTOR CODE here
          // michael note: use map to max it from max to zero. you'll remember
          // move from up to down (closer to depth saved, closer to zero
          // map(x, max sensor, depthSavedState, 255, 0)
      }
    // if ROV is below saved state
      if(sensor.depth() < depthSavedState){
          // Move ROV UP 
          // Insert MOTOR CODE here
          // michael note: use map to max it from max to zero. you'll remember
          // move from up to down (closer to depth saved, closer to zero
          // map(x, max sensor, depthSavedState, 0, 255)
      }
      
    }
    

*/
  
  delay(1000);
}
