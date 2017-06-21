  
  int sensorPin = A0;    // select the input pin for the potentiometer
  int sensorValue = 0;  // variable to store the value coming from the sensor
  int threshold = 1023;
  int buttonValue = 0;
  
  #include <SoftwareSerial.h> 
  SoftwareSerial BTserial(10, 11); // RX | TX
  
void setup() {
  // put your setup code here, to run once:
pinMode(2, OUTPUT);
pinMode(3, INPUT);
Serial.begin(9600);
BTserial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

      sensorValue = analogRead(sensorPin);
      Serial.println(sensorValue);

      buttonValue = digitalRead(3);
      Serial.println("Button" + String(buttonValue));
      
      if(buttonValue == HIGH)
      {
      threshold = (sensorValue + 50);
      }
      
      
      
      if(sensorValue >= threshold)
      {
       Serial.println("thresholdReached");
       digitalWrite(2, HIGH);       
       BTserial.write("AB12345");
       Serial.println("AB12345");
      }
      
      if(sensorValue < threshold)
      {
      Serial.println("thresholdNOTReached");
      digitalWrite(2, LOW);
      BTserial.write("MATE017");
      Serial.println("MATE017");
      }
      
      
}

