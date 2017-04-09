// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif


#define PIN1           6
#define PIN2           8
#define PIN3           7
#define MAX            50

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      150

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixel = Adafruit_NeoPixel(NUMPIXELS, PIN1, NEO_GRB + NEO_KHZ800);


int delayval = 500; // delay for half a second

void setup() {
  pixel.begin(); // This initializes the NeoPixel library.
}

void loop() {

  // For a set of NeoPixels the first NeoPixel is 0, second is 1, all the way up to the count of pixels minus one.
 
  pixel.setPin(PIN3);
  light_strip(&pixel, MAX,0,0);
  delay(1000);
  light_strip(&pixel, 0,MAX,0);
  delay(1000);
  light_strip(&pixel, 0,0,MAX);
  delay(1000);
  light_strip(&pixel, MAX,MAX,MAX);
  delay(1000);
  /*light_swap(&pixel, MAX,0,0,0,0,MAX);
  delay(1000);
  light_swap(&pixel, MAX/2,MAX/3,0,MAX/2,0,MAX/2);
  delay(1000);*/
  
 /*
  pixel.setPin(PIN3);
  light_strip(&pixel, MAX,0,0);
  delay(1000);
  light_strip(&pixel, 0,MAX,0);
  delay(1000);
      
  pixel.setPin(PIN2);
  light_strip(&pixel, MAX,0,0);
  delay(1000);
  light_strip(&pixel, 0,MAX,0);
  delay(1000); */

}

void light_strip(Adafruit_NeoPixel *pixels, int r, int g, int b) {
  for(int i=0;i<NUMPIXELS;i++){
    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pixels->setPixelColor(i, pixels->Color(r,g,b)); // Moderately bright green color.
    //delay(10);
    //pixels->show(); // This sends the updated pixel color to the hardware.
  }
  pixels->show(); // This sends the updated pixel color to the hardware.

}

void light_swap(Adafruit_NeoPixel *pixels, int r1, int g1, int b1, int r2, int g2, int b2) {
  for(int i=0;i<NUMPIXELS;i++){
    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    if (i%2==0) {
      pixels->setPixelColor(i, pixels->Color(r1,g1,b1)); // Moderately bright green color.
    } else {
      pixels->setPixelColor(i, pixels->Color(r2,g2,b2)); // Moderately bright green color.
    }
  }
  pixels->show(); // This sends the updated pixel color to the hardware.

}
