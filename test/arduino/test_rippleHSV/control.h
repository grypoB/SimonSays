#ifndef CONTROL_H
#define CONTROL_H

#include "Arduino.h"
#include <Adafruit_NeoPixel.h>
#include "RGBConverter.h"

enum Mode {MODE_AUTO, MODE_BLINK, MODE_BREATH, MODE_MANUAL};


class Color {
  public :
    unsigned char red, green, blue;
    double h,s,v;
    RGBConverter conv = RGBConverter();


    // Constructor
    Color() {
        red = 0;
        green = 0;
        blue = 0;
    }

    Color(unsigned char r, unsigned char g, unsigned char b) {
        red = r;
        green = g;
        blue = b;
    }
    

    // Foncions
    /* Random color in [0, max] for RGB
     */
    void rand(unsigned int max) {
        // create a random color (in function of the max)
        byte rgb[3];
        #define RMAX_H 360
#define RMAX_S 100
#define RMAX_V 100

#define MIN_H 130
#define MAX_H 250

#define MIN_S 60
#define MAX_S 100

#define MIN_V 5
#define MAX_V 15

#define NORM_H 1
#define NORM_S 1
#define NORM_V 1
        h = random(MIN_H,MAX_H+1);
        h = h*NORM_H/RMAX_H;

        s = random(MIN_S,MAX_S+1);
        s = s*NORM_S/RMAX_S;

        v = random(MIN_V,MAX_V+1);
        v = v*NORM_V/RMAX_V;
        conv.hsvToRgb(h, s, v, rgb);
        red = rgb[0];
        red = rgb[1];
        red = rgb[2];
    }
    

    /* Fade the color from color1 to color2 from t=from to t=to
     * Caculated at t=at
     */
    void fade(Color color1, Color color2, unsigned long at, unsigned long from, unsigned long to) {
        red   = map(at, from, to, color1.red,   color2.red);
        green = map(at, from, to, color1.green, color2.green);
        blue  = map(at, from, to, color1.blue,  color2.blue);
    }

};


class Controller {
  public:
    Controller(Adafruit_NeoPixel* nStrip) {
      strip = nStrip;
    };

    Adafruit_NeoPixel* strip;
    enum Mode mode;

    uint32_t lastTic = 0;
    uint32_t nowTic = 0;
    
    uint32_t delay1 = 0;
    uint32_t delay2 = 0;

    Color color1;
    Color color2;

    uint32_t brightness = 255; // brightness of the color (set a top boundary (max 255))
    uint32_t transition = 900; // time to do a transition (in millisecond)
    uint32_t stable = 100; // set how much time a color should stay before changing (in millesecond)

    /* Goes from color 1 to color 2 in succession
     * upTime : time to fade from 1 to 2
     * downTime : time to fade from 2 to 1
     * never actually stays at color 1 or 2
     */

    void setBreath(Color nColor1, Color nColor2, unsigned int upTime, unsigned int downTime) {
        mode = MODE_BREATH;
        color1 = nColor1;
        color2 = nColor2;

        delay1 = upTime;
        delay2 = downTime;
    };

    /* Transition from one random color to the next
     * transition: time it takes to go from one color to the next
     * stable: time it stays at one color
     * brightness: max brightness for the random color
     */
    void setAuto(unsigned int nTransition, unsigned int nStable, unsigned int nBrightness) {
        mode = MODE_AUTO;

        if (nBrightness > 255) {
            nBrightness = 255;
        }

        brightness = nBrightness;
        transition = nTransition;
        stable     = nStable;
    };

    /* Set the strip to a constant manual color
     */
    void setManual(Color color) {
        mode = MODE_MANUAL;
        setColor(color);
    }

    /* Blink between color 1 and color 2 in succession
     * (same as breath, but no transition anc actually stays at the color)
     * dealy1: time it stays at color 1
     * delay2: time it stays at color 2
     */
    void setBlink(Color nColor1, Color nColor2, uint32_t nDelay1, uint32_t nDelay2) {
        mode = MODE_BLINK;

        color1 = nColor1;
        color2 = nColor2;

        delay1 = nDelay1;
        delay2 = nDelay2;
    }

    void setColor(uint32_t color) {
        for (int i=0 ; i<strip->numPixels() ; i++) {
            strip->setPixelColor(i, color);
        }
        strip->show();
    };

    void setColor(Color color) {
        for (int i=0 ; i<strip->numPixels() ; i++) {
            strip->setPixelColor(i, strip->Color(color.red,color.green,color.blue));
        }
        strip->show();
    };

    void update(uint32_t now);

    private:
      void updateBreath();
      void updateAuto();
      void updateBlink();
};


#endif

