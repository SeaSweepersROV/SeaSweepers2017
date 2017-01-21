#include <Adafruit_NeoPixel.h>

#define PIN            6
#define NUMPIXELS      7
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRBW + NEO_KHZ800);

int delayval = 500; // delay for half a second

void setup() {
  pixels.begin(); // This initializes the NeoPixel library.
  pinMode(3, INPUT);
  pinMode(4,OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // For a set of NeoPixels the first NeoPixel is 0, second is 1, all the way up to the count of pixels minus one.
  digitalWrite(4, HIGH);
  if (digitalRead(3) == HIGH) {
      for(int i=0;i<NUMPIXELS;i++){
        pixels.setPixelColor(i, pixels.Color(0,0,0,255));
        pixels.show();
      }
        Serial.println("ONE");
  }
  if (digitalRead(3) == LOW) {
      for(int i=0;i<NUMPIXELS;i++){
        pixels.setPixelColor(i, pixels.Color(255,0,0,0));
        pixels.show();
      }
        Serial.println("TWO");
  }
  else {
    Serial.println("GAY");
      for(int i=0;i<NUMPIXELS;i++){
        pixels.setPixelColor(i, pixels.Color(0,0,255,0));
        pixels.show();
      }
    
  }
  delay(10);
}
