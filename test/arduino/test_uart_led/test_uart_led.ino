// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif



#define PIN1           6
#define PIN2           8
#define MAX            50

#define R 0
#define G 1
#define B 2
#define UPDATE_STRIP1 1
#define UPDATE_STRIP2 2

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      150

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel strip1 = Adafruit_NeoPixel(NUMPIXELS, PIN1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(NUMPIXELS, PIN2, NEO_GRB + NEO_KHZ800);

char cmd;
uint8_t led[NUMPIXELS][3];
char buf[256];

void setup() {
  strip1.begin(); // This initializes the NeoPixel library.
  strip2.begin(); // This initializes the NeoPixel library.
  Serial.begin(9600);
}

void loop() {

  if (Serial.available()>0) {
    cmd = Serial.read();
    sprintf(buf,"cmd = %d\n", cmd);
    Serial.print(buf);
    if (cmd == UPDATE_STRIP1 || UPDATE_STRIP2 ) {
      //Serial.println("got update strip");
      while (Serial.available() == 0);

      int length = Serial.read();
      for (int i=0 ; i<length ; i++) {
        while (Serial.available() < 3);

        led[i][R] = Serial.read();
        led[i][G] = Serial.read();
        led[i][B] = Serial.read();
        //sprintf(buf,"%d %d %d\n",led[i][R],led[i][G],led[i][B]);
        //Serial.print(buf);

      }
      Serial.println("updated strip");
      sprintf(buf,"l = %d\n", length);
      Serial.print(buf);

      if (cmd == UPDATE_STRIP1) {
        update_strip(&strip1, length);
      } else {
        update_strip(&strip2, length);
      }
    }
  }
}

void update_strip(Adafruit_NeoPixel *pixels, int l) {
  for (int i=0 ; i<l ; i++) {
    pixels->setPixelColor(i, pixels->Color(led[i][R],led[i][G],led[i][B]));
  }
  pixels->show(); // This sends the updated pixel color to the hardware.

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
