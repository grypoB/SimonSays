#include <Adafruit_NeoPixel.h>

#include "Controller.h"

#define STRIP_LENGTH 8
#define PIN_STRIP 6 // pin number

#define BAUD 57600 // serial baud rate
#define BUFFER_SIZE 255 // size of print buffer

char buffer[BUFFER_SIZE] = {0};

Color white = Color(200,240,200);

Color color_strip[STRIP_LENGTH];
Adafruit_NeoPixel strip = Adafruit_NeoPixel(STRIP_LENGTH, PIN_STRIP, NEO_GRB + NEO_KHZ800);
Controller controller = Controller(color_strip, STRIP_LENGTH);

void setup() {
    Serial.begin(BAUD);

    strip.begin();

    Serial.println("Startup");
}


void loop() {
    int r,g,b,i;
    uint32_t now = millis();

    controller.setManual(white);
    controller.update(now);

    update_strip(&strip, color_strip);

    if (Serial.available()) {
        delay(200);
        for (i=0 ; Serial.available() ; i++) {
            buffer[i] = Serial.read();
        }
        buffer[i] = '\0';

        sscanf(buffer, "%d %d %d",&r, &g, &b);

        white.set(r,g,b);

        sprintf(buffer, "%d\t%d\t%d\n",r,g,b);
        Serial.print(buffer);
    }
}

void update_strip(Adafruit_NeoPixel *pStrip, Color color_strip[]) {
    for (uint16_t i=0 ; i<pStrip->numPixels() ; i++) {
        pStrip->setPixelColor(i, pStrip->Color(color_strip[i].getR(),
                                               color_strip[i].getG(),
                                               color_strip[i].getB()));
        
        //pStrip->setPixelColor(i, pStrip->Color(200, 240, 200));
    }
    pStrip->show();
}
