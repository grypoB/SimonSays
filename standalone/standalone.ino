#include <Adafruit_NeoPixel.h>

#include "Controller.h"
#include "Button.h"
#include "Fsm.h"
#include "Output.h"

#define STRIP_LENGTH  20
#define STRIP_LENGTH2 15
#define BUTTON_PIN 5 // pin number
#define PIN_STRIP 6 // pin number
#define PIN_STRIP2 8 // pin number

#define DEBOUNCE 40 // in ms

#define BAUD 9600 // serial baud rate
#define BUFFER_SIZE 255 // size of print buffer

char buffer[BUFFER_SIZE] = {0};

Color color_strip[STRIP_LENGTH];
Color color_strip2[STRIP_LENGTH];
Adafruit_NeoPixel strip = Adafruit_NeoPixel(STRIP_LENGTH, PIN_STRIP, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel back_strip = Adafruit_NeoPixel(STRIP_LENGTH2, PIN_STRIP2, NEO_GRB + NEO_KHZ800);
Controller controller = Controller(color_strip, STRIP_LENGTH);
Controller back_ctrl = Controller(color_strip2, STRIP_LENGTH);

Button butt   = Button(BUTTON_PIN, DEBOUNCE);
Output output = Output(&controller, buffer);
Fsm fsm       = Fsm(&butt, &output);

void setup() {
    Serial.begin(BAUD);

    strip.begin();
    back_strip.begin();
    butt.init();
    //fsm.init();

    delay(500);

    Serial.println("Startup");
    back_ctrl.setAutoHSV_single(700,300);
}


void loop() {
    uint32_t now = millis();

    buffer[0] = '\0';

    fsm.update(now);

    output.update(now);
    back_ctrl.update(now);

    update_strip(&strip, color_strip);
    update_strip(&back_strip, color_strip2);

    Serial.print(buffer);

    delayMicroseconds(500); // Delay to insure enough time in between strips update
}

void update_strip(Adafruit_NeoPixel *pStrip, Color color_strip[]) {
    for (uint16_t i=0 ; i<pStrip->numPixels() ; i++) {
        pStrip->setPixelColor(i, pStrip->Color(color_strip[i].getR(),
                                               color_strip[i].getG(),
                                               color_strip[i].getB()));
    }
    pStrip->show();
}
