/** Test file for the Library */
#include "Controller.h"
#include <Adafruit_NeoPixel.h>

#define TOTAL_LENGTH 8

#define PIN_STRIP 6

#define MAX 15

#define TRANSITION 1000
#define STABLE     500

#define MIN_H 0
#define MAX_H 360

#define MIN_S 70
#define MAX_S 100

#define MIN_V (MAX-10)
#define MAX_V MAX

#define SPEED 50
#define RADIUS 2


Color color_strip[TOTAL_LENGTH];

Color white  = Color(MAX, MAX, MAX);
Color red    = Color(MAX,0,0);
Color yellow = Color(MAX,MAX,0);
Color green  = Color(0,MAX,0);
Color purple = Color(0.6*MAX,0,MAX);
Color blue   = Color(0,0,MAX);
Color black  = Color(0,0,0);

Adafruit_NeoPixel strip = Adafruit_NeoPixel(TOTAL_LENGTH, PIN_STRIP, NEO_GRB + NEO_KHZ800);

Controller ct = Controller(color_strip, TOTAL_LENGTH);



void update_strip(Adafruit_NeoPixel *pStrip, Color color_strip[]) {
    for (uint16_t i=0 ; i<pStrip->numPixels() ; i++) {
        pStrip->setPixelColor(i, pStrip->Color(color_strip[i].getR(),
                                               color_strip[i].getG(),
                                               color_strip[i].getB()));
    }
    pStrip->show();
}


void runControllerFor(uint32_t delta) {
    uint32_t start = millis();
    uint32_t now   = start;

    while (now-start < delta) {
        ct.update(now);
        update_strip(&strip, color_strip);
        now = millis();
    }
}

void setup() {
    Serial.begin(9600);

    strip.begin();

    black.setHSV(MIN_H, MAX_H, MIN_S, MAX_S, MIN_V, MAX_V);
}


void loop() {
    // Set manual
    Serial.println("Manual");
    ct.setManual(purple);
    runControllerFor(1000);
    ct.setManual(red);
    runControllerFor(1000);
    ct.setManual(green);
    runControllerFor(1000);
    ct.setManual(blue);
    runControllerFor(1000);

    // Set blink
    Serial.println("Blink");
    ct.setBlink(black,yellow,300,300);
    runControllerFor(3000);
    ct.setBlink(red,yellow,100,500);
    runControllerFor(3000);
    ct.setBlink(red,blue,10,1000);
    runControllerFor(3000);

    // Set breath
    Serial.println("Breath");
    ct.setBreath(black,yellow,300,300);
    runControllerFor(3000);
    ct.setBreath(red,yellow,100,500);
    runControllerFor(3000);
    ct.setBreath(red,blue,10,1000);
    runControllerFor(3000);

    // Set Auto
    Serial.println("Auto");
    ct.setAuto(TRANSITION, STABLE, MAX);
    runControllerFor(10000);

    // Set Auto HSV
    Serial.println("Auto HSV");
    ct.setAutoHSV(TRANSITION, STABLE);
    runControllerFor(10000);
    
    // Set Auto HSV single
    Serial.println("Auto HSV single");
    ct.setAutoHSV_single(TRANSITION, STABLE);
    runControllerFor(10000);

    // -------- Effects ----------------
    // Flash
    Serial.println("Effect flash");
    ct.setEffectFlash(green, 300);
    runControllerFor(1000);
    ct.setEffectFlash(purple, 300);
    runControllerFor(1000);
     
    // Beam
    Serial.println("Effect beam");
    ct.setEffectBeam(green, RADIUS, SPEED, true);
    runControllerFor(1000);
    ct.setEffectBeam(green, RADIUS, SPEED, false);
    runControllerFor(1000);

    // Fill HSV
    Serial.println("Effect Fill HSV");
    ct.setManual(purple);
    runControllerFor(1000);
    ct.setFillHSV(TRANSITION, STABLE, SPEED*4, true);
    runControllerFor(3000);
    ct.setManual(purple);
    runControllerFor(3000);
    ct.setFillHSV(TRANSITION, STABLE, SPEED*4, false);
    runControllerFor(5000);

    // Empty
    Serial.println("Effect Empty");
    ct.setManual(purple);
    runControllerFor(3000);
    ct.setEmpty(red, SPEED*4, true);
    runControllerFor(3000);
    ct.setManual(purple);
    runControllerFor(3000);
    ct.setEmpty(red, SPEED*4, false);
    runControllerFor(5000);

    // Set Black
    Serial.print("\n\n--------------\n\n");
    ct.setManual(black);
    runControllerFor(5000);

}


