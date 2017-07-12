// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library

#include <Adafruit_NeoPixel.h>
#include "RGBConverter.h"
#ifdef __AVR__
  #include <avr/power.h>
#endif


#define PIN           6
#define MAX            255
#define RMAX           255


#define RMAX_H 360
#define RMAX_S 100
#define RMAX_V 100

#define MIN_H 120
#define MAX_H 260

#define MIN_S 80
#define MAX_S 100

#define MIN_V 3
#define MAX_V 10

#define NORM_H 1
#define NORM_S 1
#define NORM_V 1

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      150

byte r,g,b;
byte rgb[3];
long rgbval; 

double h,s,v;

char buf[256];

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixel = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
RGBConverter conv = RGBConverter();

void setup() {
  Serial.begin(9600);
  pixel.begin(); // This initializes the NeoPixel library.
}

void loop() {
  h = random(MIN_H,MAX_H+1);
  h = h*NORM_H/RMAX_H;

  s = random(MIN_S,MAX_S+1);
  s = s*NORM_S/RMAX_S;

  v = random(MIN_V,MAX_V+1);
  v = v*NORM_V/RMAX_V;

  conv.hsvToRgb(h, s, v, rgb);

  r = rgb[0];
  g = rgb[1];
  b = rgb[2];

  sprintf(buf,"h=%d s=%d v=%d ; r=%d g=%d b=%d\n", (int)(h*RMAX_H/NORM_H),(int)(s*RMAX_S),(int)(v*RMAX_V),r,g,b);
  Serial.print(buf);
  
  //light_strip(&pixel, r, g, b);
  light_hsv_single(&pixel);
  delay(50);
}

void light_strip(Adafruit_NeoPixel *pixels, int r, int g, int b) {
  for(int i=0;i<NUMPIXELS;i++){
    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pixels->setPixelColor(i, pixels->Color(r,g,b)); // Moderately bright green color.
  }
  pixels->show(); // This sends the updated pixel color to the hardware.

}

void light_hsv_single(Adafruit_NeoPixel *pixels) {
  byte rgb[3];

  for(int i=0;i<NUMPIXELS;i++){
    h = random(MIN_H,MAX_H+1);
    h = h*NORM_H/RMAX_H;

    s = random(MIN_S,MAX_S+1);
    s = s*NORM_S/RMAX_S;

    v = random(MIN_V,MAX_V+1);
    v = v*NORM_V/RMAX_V;
    conv.hsvToRgb(h, s, v, rgb);
    pixels->setPixelColor(i, pixels->Color(rgb[0],rgb[1],rgb[2]));
  }
  pixels->show(); // This sends the updated pixel color to the hardware.
}

long HSV_to_RGB( float h, float s, float v ) {
  /* modified from Alvy Ray Smith's site: http://www.alvyray.com/Papers/hsv2rgb.htm */
  // H is given on [0, 6]. S and V are given on [0, 1].
  // RGB is returned as a 24-bit long #rrggbb
  int i;
  float m, n, f;
 
  // not very elegant way of dealing with out of range: return black
  if ((s<0.0) || (s>1.0) || (v<0.0) || (v>1.0)) {
    return 0L;
  }
 
  if ((h < 0.0) || (h > 6.0)) {
    return long( v * 255 ) + long( v * 255 ) * 256 + long( v * 255 ) * 65536;
  }
  i = floor(h);
  f = h - i;
  if ( !(i&1) ) {
    f = 1 - f; // if i is even
  }
  m = v * (1 - s);
  n = v * (1 - s * f);
  switch (i) {
  case 6:
  case 0:
    return long(v * 255 ) * 65536 + long( n * 255 ) * 256 + long( m * 255);
  case 1:
    return long(n * 255 ) * 65536 + long( v * 255 ) * 256 + long( m * 255);
  case 2:
    return long(m * 255 ) * 65536 + long( v * 255 ) * 256 + long( n * 255);
  case 3:
    return long(m * 255 ) * 65536 + long( n * 255 ) * 256 + long( v * 255);
  case 4:
    return long(n * 255 ) * 65536 + long( m * 255 ) * 256 + long( v * 255);
  case 5:
    return long(v * 255 ) * 65536 + long( m * 255 ) * 256 + long( n * 255);
  }
}
