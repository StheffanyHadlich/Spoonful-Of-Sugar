#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN            2
#define NUMPIXELS      4ff
int brilho = 10;
#define BRIGHTNESS brilho
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
//int delayval = 500; // delay for half a second

int LED;

void setup(){
  Serial.begin(9600);
  pixels.setBrightness(BRIGHTNESS);
  pixels.begin();
  pixels.show();
  
}

void teclasindividuais(){
      LED = 0;
      LED = 1;            
      LED = 2;
      LED = 3;

  
    pixels.setPixelColor(LED, pixels.Color(255,0,0));
    pixels.show();
 
    pixels.setPixelColor(LED, pixels.Color(0,255,0));
    pixels.show();        
  
    pixels.setPixelColor(LED, pixels.Color(0,0,255));
    pixels.show();        
  
    pixels.setPixelColor(LED, pixels.Color(0,0,0));
    pixels.show();        

}

void pisca(){


  pixels.setPixelColor(0, pixels.Color(0,0,0));
  pixels.setPixelColor(1, pixels.Color(0,0,0));
  pixels.setPixelColor(2, pixels.Color(0,0,0));
  pixels.setPixelColor(3, pixels.Color(0,0,0));  
  pixels.show();
  delay(1000);

for (int i = 0; i < 6; i++) {
    pixels.setPixelColor(0, pixels.Color(255,0,0));
    pixels.setPixelColor(1, pixels.Color(0,0,0));
    pixels.setPixelColor(2, pixels.Color(0,0,0));
    pixels.setPixelColor(3, pixels.Color(0,0,0));     
    pixels.show();
    delay(100);    
    pixels.setPixelColor(0, pixels.Color(0,255,0));
    pixels.setPixelColor(1, pixels.Color(255,0,0));
    pixels.setPixelColor(2, pixels.Color(0,0,0));
    pixels.setPixelColor(3, pixels.Color(0,0,0));     
    pixels.show(); 
    delay(100);    
    pixels.setPixelColor(0, pixels.Color(0,0,255));
    pixels.setPixelColor(1, pixels.Color(0,255,0));
    pixels.setPixelColor(2, pixels.Color(255,0,0));
    pixels.setPixelColor(3, pixels.Color(0,0,0));     
    pixels.show();
    delay(100);
    pixels.setPixelColor(0, pixels.Color(0,0,0));
    pixels.setPixelColor(1, pixels.Color(0,0,255));
    pixels.setPixelColor(2, pixels.Color(0,255,0));
    pixels.setPixelColor(3, pixels.Color(255,0,0));    
    pixels.show();
    delay(100);
    pixels.setPixelColor(0, pixels.Color(0,0,0));
    pixels.setPixelColor(1, pixels.Color(0,0,0));
    pixels.setPixelColor(2, pixels.Color(0,0,255));
    pixels.setPixelColor(3, pixels.Color(0,255,0));    
    pixels.show();
    delay(100);
    pixels.setPixelColor(0, pixels.Color(0,0,0));
    pixels.setPixelColor(1, pixels.Color(0,0,0));
    pixels.setPixelColor(2, pixels.Color(0,0,0));
    pixels.setPixelColor(3, pixels.Color(0,0,255));    
    pixels.show();
    delay(100);
}
    pixels.setPixelColor(0, pixels.Color(0,0,0));
    pixels.setPixelColor(1, pixels.Color(0,0,0));
    pixels.setPixelColor(2, pixels.Color(0,0,0));
    pixels.setPixelColor(3, pixels.Color(0,0,0));  
    pixels.show();

}

void aumBrilho(){
  if(brilho < 10){
    brilho = brilho + 1;
  }else if(brilho < 250){
    brilho = brilho + 10;
  }else if(brilho < 255){
    brilho = brilho + 1;
  }
  pixels.show();
}

void baiBrilho(){
  if(brilho > 250){
    brilho = brilho - 1;
  }else if(brilho > 10){
    brilho = brilho - 10;    
  }else if(brilho > 1){
    brilho = brilho - 1;    
  }
  pixels.show();
}

void loop() {

pixels.setBrightness(brilho);

pisca();
  
}
