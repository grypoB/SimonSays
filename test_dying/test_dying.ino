#include "control.h"
#include "RGBConverter.h"
#include <Adafruit_NeoPixel.h>

#define NUMPIXELS      150
#define PIN            6

#define TRANSITION 500
#define STABLE     0

#define MIN_H 260
#define MAX_H 360

#define MIN_S 50
#define MAX_S 100

#define MIN_V 10
#define MAX_V 20

#define TIME 2000
#define BEAM_SPEED 4
#define BEAM_SIZE 4

#define UP 500
#define DOWN 100
Color color1(25,0,0);
Color color2(0,10,25);

Color color_strip[NUMPIXELS];

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
Controller controller = Controller(color_strip, NUMPIXELS);
Color color(255,255,255);


uint32_t now=millis();
uint32_t last=millis();

void setup() {
  Serial.begin(9600);
  strip.begin();
  controller.setBreath(color1,color2,UP,DOWN);
  color.setHSV(MIN_H, MAX_H, MIN_S, MAX_S, MIN_V, MAX_V);
}

void loop() {
  now = millis();
  controller.update(now);
  //delay(10);

  if (now-last > TIME) {
    last = now;
    //controller.setEffectBeam(color, BEAM_SIZE, BEAM_SPEED);
      
  }

  for (int i=0 ; i<NUMPIXELS ; i++) {
        strip.setPixelColor(i, strip.Color(color_strip[i].red,
                                             color_strip[i].green,
                                             color_strip[i].blue));
    }
    strip.show();
}
