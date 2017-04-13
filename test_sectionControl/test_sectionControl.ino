#include "control.h"
#include "RGBConverter.h"
#include <Adafruit_NeoPixel.h>

#define NUMPIXELS      150
#define CT1            50
#define PIN            6

#define TRANSITION 500
#define STABLE     0

#define MIN_H 140
#define MAX_H 280

#define MIN_S 50
#define MAX_S 100

#define MIN_V 10
#define MAX_V 20

#define TIME 2000
#define BEAM_SPEED 7
#define BEAM_SIZE 4

Color color_strip[NUMPIXELS];

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
Controller controller1 = Controller(color_strip, CT1);
Controller controller2 = Controller(color_strip+CT1, NUMPIXELS-CT1);
Color color(255,255,255);
Color color2(255,0,0);


uint32_t now  = millis();
uint32_t last = millis();


void update_strips() {
    for (int i=0 ; i<NUMPIXELS ; i++) {
        strip.setPixelColor(i, strip.Color(color_strip[i].red,
                                             color_strip[i].green,
                                             color_strip[i].blue));
    }
    strip.show();
}

void setup() {
  Serial.begin(9600);
  strip.begin();
  controller1.setAutoHSV_single(TRANSITION, STABLE);
  controller2.setAuto(TRANSITION,STABLE,MAX_V);
  color.setHSV(MIN_H, MAX_H, MIN_S, MAX_S, MIN_V, MAX_V);
}

void loop() {
  now = millis();
  controller1.update(now);
  controller2.update(now);
  //delay(10);

  if (now-last > TIME) {
    last = now;
    controller1.setEffectBeam(color, BEAM_SIZE, BEAM_SPEED, true);
    controller2.setEffectBeam(color2, BEAM_SIZE, BEAM_SPEED);
  }

  update_strips();
}
