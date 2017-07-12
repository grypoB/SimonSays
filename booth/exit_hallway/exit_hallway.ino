#include "control.h"
#include "RGBConverter.h"
#include <Adafruit_NeoPixel.h>

#define TOTAL_LENGTH  200

#define PIN1          6
#define PIN2          7
#define PIN3          8

#define MAX           15

#define TRANSITION 800
#define STABLE     0

#define MIN_H 150
#define MAX_H 260

#define MIN_S 70
#define MAX_S 100

#define MIN_V 90
#define MAX_V 100

#define BREATH 100
#define SPEED 14

#define TIME 9000
#define BEAM 5

Color color_strip1[TOTAL_LENGTH];
//Color color_strip2[TOTAL_LENGTH];
//Color color_strip3[TOTAL_LENGTH];
Color black;
Color white(255,255,255);
Color color1(120,120,120);
Color color_ct1 = Color(MAX,0,0);

Adafruit_NeoPixel strip1 = Adafruit_NeoPixel(TOTAL_LENGTH, PIN1, NEO_GRB + NEO_KHZ800);
//Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(TOTAL_LENGTH, PIN2, NEO_GRB + NEO_KHZ800);
//Adafruit_NeoPixel strip3 = Adafruit_NeoPixel(TOTAL_LENGTH, PIN3, NEO_GRB + NEO_KHZ800);


Controller ct1 = Controller(color_strip1, TOTAL_LENGTH, color_ct1);
//Controller ct2 = Controller(color_strip2, TOTAL_LENGTH, color_ct1);
//Controller ct3 = Controller(color_strip3, TOTAL_LENGTH, color_ct1);

uint32_t now  = millis();
uint32_t last = millis();

void update_strip(Adafruit_NeoPixel *pStrip, Color color_strip[]) {
    for (int i=0 ; i<pStrip->numPixels() ; i++) {
        pStrip->setPixelColor(i, pStrip->Color(color_strip[i].red,
                                               color_strip[i].green,
                                               color_strip[i].blue));
    }
    pStrip->show();
}

void update_strips() {
    update_strip(&strip1, color_strip1);
    //update_strip(&strip2, color_strip1);
    //update_strip(&strip3, color_strip1);
}

void update_controllers() {
  ct1.update(now);
  //ct2.update(now);
  //ct3.update(now);
}

void setup() {
  Serial.begin(9600);
  strip1.begin();
  //strip2.begin();
  //strip3.begin();
  
  black.setHSV(MIN_H, MAX_H, MIN_S, MAX_S, MIN_V, MAX_V);

  //ct1.setBlink(color_ct1,black,BREATH,BREATH);
  
  ct1.setAutoHSV_single(TRANSITION,STABLE);
  //ct2.setFillHSV(TRANSITION,STABLE,SPEED);
  //ct3.setFillHSV(TRANSITION,STABLE,SPEED);

}

void loop() {
  Serial.println(millis()-now);
  now = millis();
  update_controllers();
  update_strips();

  if (now-last>TIME) {
    last = now;
    ct1.setEffectBeam(white, BEAM, SPEED);
  }
}
