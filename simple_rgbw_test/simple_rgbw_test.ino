#include <Adafruit_NeoPixel.h>

#define PIN            6
#define NUMPIXELS      7

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRBW + NEO_KHZ800);

int delayval = 500; // delay for half a second

void setup() {
  pixels.begin(); // This initializes the NeoPixel library.
}

void loop() {
  // For a set of NeoPixels the first NeoPixel is 0, second is 1, all the way up to the count of pixels minus one.
  for(int i=0;i<NUMPIXELS;i++){
    pixels.setPixelColor(i, pixels.Color(0,0,0,255));
    pixels.show();
  }
    delay(delayval);
  for(int i=0;i<NUMPIXELS;i++){
    pixels.setPixelColor(i, pixels.Color(255,255,255,0));
    pixels.show();
  }
    delay(delayval);
}
