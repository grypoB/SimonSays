#ifndef CONTROL_H
#define CONTROL_H

#include "Arduino.h"
#include "RGBConverter.h"
#include <Adafruit_NeoPixel.h>

#define NUM_PIXEL 60
enum Mode {MODE_AUTO, MODE_BLINK, MODE_BREATH, MODE_MANUAL,
           MODE_AUTO_HSV, MODE_AUTO_HSV_SINGLE, MODE_FILL_HSV, MODE_FILL_HSV_REVERSE,
           MODE_EMPTY, MODE_EMPTY_REVERSE};
enum Effect {EFFECT_NONE, EFFECT_BEAM, EFFECT_BEAM_REVERSE, EFFECT_FLASH};

class Color {
  public :
    unsigned char red, green, blue;
    
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
        red   = random(0,max+1);
        green = random(0,max+1);
        blue  = random(0,max+1);
    }
    
    // Foncions
    /* Random color in [0, max] for RGB
     */
    void randHSV();
    void setHSV(int nMin_h, int nMax_h,
                int nMin_s, int nMax_s,
                int nMin_v, int nMax_v);
    /* Fade the color from color1 to color2 from t=from to t=to
     * Caculated at t=at
     */
    void fade(Color color1, Color color2, unsigned long at, unsigned long from, unsigned long to) {
        red   = map(at, from, to, color1.red,   color2.red);
        green = map(at, from, to, color1.green, color2.green);
        blue  = map(at, from, to, color1.blue,  color2.blue);
    }

    void mix(Color color1, Color color2, double opacity) {
        red   = color1.red  *opacity + color2.red  *(1-opacity); 
        green = color1.green*opacity + color2.green*(1-opacity); 
        blue  = color1.blue *opacity + color2.blue *(1-opacity); 
    }

    void set(unsigned char r, unsigned char g, unsigned char b) {
      red = r;
      green = g;
      blue = b;
    }
};


class Controller {
  public:
    Controller(Color nStrip[], int nSize, Color nMainColor = Color()) {
      //Serial.begin(9600);
      colort = nStrip;
      size = nSize;
      mainColor = nMainColor;
      /*
      if (nSize>NUM_PIXEL) {
        Serial.print("strip too long ");
        Serial.println(nSize);
      } else {
        Serial.print("initialized at ");
        Serial.println(nSize);
      }
      */
    };


    enum Mode mode;
    enum Effect effect = EFFECT_NONE;
    
    int size;

    uint32_t lastTic = 0;
    uint32_t nowTic = 0;
    uint32_t effectTic = 0;

    Color effectColor;
    int32_t beamRadius; // linearly from 100 to 0 %
    int32_t beamSpeed; // ms per LED
    int32_t effectDuration;
    
    uint32_t delay1 = 0;
    uint32_t delay2 = 0;

    Color mainColor;

    Color color1;
    Color color2;

    Color color1t[NUM_PIXEL];
    Color color2t[NUM_PIXEL];
    Color *colort;

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

    /* Transition from one random color to the next
     * transition: time it takes to go from one color to the next
     * stable: time it stays at one color
     * brightness: max brightness for the random color
     */
    void setAutoHSV(unsigned int nTransition, unsigned int nStable) {
        mode = MODE_AUTO_HSV;

        transition = nTransition;
        stable     = nStable;
    };

    void setAutoHSV_single(unsigned int nTransition, unsigned int nStable) {
        mode = MODE_AUTO_HSV_SINGLE;

        transition = nTransition;
        stable     = nStable;
    };

    /* Set the strip to a constant manual color
     */
    void setManual(Color color) {
        mode = MODE_MANUAL;
        color1 = color;
    }

    void setEffectBeam(Color color, uint32_t radius, uint32_t speed, bool reverse=false) {
        if (reverse) {
          effect = EFFECT_BEAM_REVERSE;
        } else {
          effect = EFFECT_BEAM;
        }
        effectColor = color;
        beamRadius = radius;
        beamSpeed = speed;
        effectTic = 0;
    }

    void setFillHSV(unsigned int nTransition, unsigned int nStable,
                    uint32_t speed, bool reverse=false) {
        transition = nTransition;
        stable     = nStable;
        
        if (reverse) {
          mode = MODE_FILL_HSV_REVERSE;
        } else {
          mode = MODE_FILL_HSV;
        }
        
        beamSpeed = speed;
        effectTic = 0;

        for (int i=0 ; i<size ; i++) {
          color1t[i].randHSV();
          colort[i].set(0,0,0); // set black background
        }
    }

    void setEmpty(Color color, uint32_t speed, bool reverse=false) {
        if (reverse) {
          mode = MODE_EMPTY_REVERSE;
        } else {
          mode = MODE_EMPTY;
        }
        
        beamSpeed = speed;
        effectTic = 0;

        color1 = color;
    }

    void setEffectFlash(Color color, int32_t duration) {
        effect = EFFECT_FLASH;
        
        effectColor = color;
        effectDuration = duration;
        
        effectTic = nowTic;
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

    void setColor(Color color) {
        for (int i=0 ; i<size ; i++) {
            colort[i] = color;
        }
    };

    void setColorSingle(Color color[]) {
        for (int i=0 ; i<size ; i++) {
            colort[i] = color[i];
        }
    };

    void update(uint32_t now);

    private:
      void updateBreath();
      void updateAuto();
      void updateBlink();
      void updateAutoSingle();
      void updateBeam();
      void updateFlash();
      void updateFillHSV();
      void updateEmpty();

};


#endif
