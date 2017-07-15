#include <Adafruit_NeoPixel.h>

#include "Controller.h"
#include "Button.h"

#define LENGTH 8
#define BUTTON_PIN 9 // pin number
#define LED_PIN 6 // pin number

#define DEBOUNCE 40 // in ms

#define BAUD 9600 // serial baud rate

#define TRANSITION 1000
#define STABLE     100

#define MIN_H -100
#define MAX_H 30
#define MIN_S 90
#define MAX_S 100
#define MIN_V 20
#define MAX_V 30

Color color_strip[LENGTH];
Adafruit_NeoPixel strip = Adafruit_NeoPixel(LENGTH, LED_PIN, NEO_GRB + NEO_KHZ800);
Controller controller = Controller(color_strip, LENGTH);

// Base colors
Color red   = Color(255,0,0);
Color green = Color(0,255,0);
Color blue  = Color(0,0,255);
Color black = Color(0,255,0);


Button butt = Button(BUTTON_PIN, DEBOUNCE);
static int16_t press = 0;

void setup() {
    Serial.begin(BAUD);

    strip.begin();
    butt.init();

    delay(1000);

    Serial.println("Startup");

    black.setHSV(MIN_H, MAX_H, MIN_S, MAX_S, MIN_V, MAX_V);
    controller.setAutoHSV(TRANSITION,STABLE);
    //controller.setBlink(red,green,TRANSITION,TRANSITION);
}


void loop() {
    uint32_t now = millis();

    int16_t result = butt.update(now);

    if(result == BUTTON_RELEASE) {
        press++;
        Serial.print("relea ");
        Serial.println(press);
    } else if (result == BUTTON_PRESS) {
        press--;
        Serial.print("press ");
        Serial.println(press);
    }

    controller.update(now);

    update_strip(&strip, color_strip);
}

void update_strip(Adafruit_NeoPixel *pStrip, Color color_strip[]) {
    for (int i=0 ; i<pStrip->numPixels() ; i++) {
        pStrip->setPixelColor(i, pStrip->Color(color_strip[i].getR(),
                                               color_strip[i].getG(),
                                               color_strip[i].getB()));
    }
    pStrip->show();
}
