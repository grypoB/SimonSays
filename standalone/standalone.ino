#include <Adafruit_NeoPixel.h>

#include "Controller.h"
#include "Button.h"
#include "Fsm.h"
#include "Output.h"

#define STRIP_LENGTH 20
#define BUTTON_PIN 9 // pin number
#define LED_PIN 6 // pin number

#define DEBOUNCE 40 // in ms

#define BAUD 9600 // serial baud rate
#define BUFFER_SIZE 255 // size of print buffer

char buffer[BUFFER_SIZE] = {0};

Color color_strip[STRIP_LENGTH];
Adafruit_NeoPixel strip = Adafruit_NeoPixel(STRIP_LENGTH, LED_PIN, NEO_GRB + NEO_KHZ800);
Controller controller = Controller(color_strip, STRIP_LENGTH);

Button butt   = Button(BUTTON_PIN, DEBOUNCE);
Output output = Output(&controller, buffer);
Fsm fsm       = Fsm(&butt, &output);

void setup() {
    Serial.begin(BAUD);

    strip.begin();
    butt.init();
    //fsm.init();

    delay(500);

    Serial.println("Startup");

}


void loop() {
    uint32_t now = millis();

    buffer[0] = '\0';

    fsm.update(now);

    output.update(now);
    //controller.update(now);

    update_strip(&strip, color_strip);

    Serial.print(buffer);
}

void update_strip(Adafruit_NeoPixel *pStrip, Color color_strip[]) {
    for (uint16_t i=0 ; i<pStrip->numPixels() ; i++) {
        pStrip->setPixelColor(i, pStrip->Color(color_strip[i].getR(),
                                               color_strip[i].getG(),
                                               color_strip[i].getB()));
    }
    pStrip->show();
}
