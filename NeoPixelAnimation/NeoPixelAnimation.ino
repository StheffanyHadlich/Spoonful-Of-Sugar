#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN            2
#define NUMPIXELS      2
int brilho = 10;
#define BRIGHTNESS brilho
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
//int delayval = 500; // delay for half a second

int LED;

void setup(){
  Serial.begin(9600);
  pixels.setBrightness(BRIGHTNESS);
  pixels.setPixelColor(0, pixels.Color(0,0,0));
  pixels.setPixelColor(0, pixels.Color(0,0,0));
  pixels.begin();
  pixels.show();
  
}

void loop() {

pixels.setPixelColor(0, pixels.Color(0,0,255));
pixels.setPixelColor(1, pixels.Color(255,0,0));
pixels.show();
delay(500);

pixels.setPixelColor(0, pixels.Color(0,0,0));
pixels.setPixelColor(1, pixels.Color(0,0,0));
pixels.show();
delay(1000);

}
