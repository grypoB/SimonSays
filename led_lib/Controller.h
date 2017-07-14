#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

#include "Arduino.h"
#include "RGBConverter.h"
#include "Color.h"

#define NUM_PIXEL 65
#define CONTROLLER_MAX_BRIGHTNESS 255

enum Mode {MODE_AUTO, MODE_BLINK, MODE_BREATH, MODE_MANUAL,
           MODE_AUTO_HSV, MODE_AUTO_HSV_SINGLE,
           MODE_FILL_HSV, MODE_FILL_HSV_REVERSE,
           MODE_EMPTY, MODE_EMPTY_REVERSE};
enum Effect {EFFECT_NONE, EFFECT_BEAM, EFFECT_BEAM_REVERSE, EFFECT_FLASH};

class Controller {
  public:
    // Public variable
    Color mainColor;


    // Public function

    Controller(Color nStrip[], int16_t nSize, Color nMainColor = Color());

    /* Set the strip to a constant manual color
    */
    void setManual(Color color);

    /* Blink between color 1 and color 2 in succession
     * (same as breath, but no transition anc actually stays at the color)
     * dealy1: time it stays at color 1
     * delay2: time it stays at color 2
     */
    void setBlink(Color nColor1, Color nColor2, uint32_t nDelay1, uint32_t nDelay2);

    /* Goes from color 1 to color 2 in succession
     * upTime : time to fade from 1 to 2
     * downTime : time to fade from 2 to 1
     * never actually stays at color 1 or 2
     */
    void setBreath(Color nColor1, Color nColor2, uint16_t upTime, uint16_t downTime);

    /* Transition from one random color to the next
     * transition: time it takes to go from one color to the next
     * stable: time it stays at one color
     * brightness: max brightness for the random color
     */
    void setAuto(uint16_t nTransition, uint16_t nStable, uint8_t nBrightness);

    /* Transition from one random color to the next
     * transition: time it takes to go from one color to the next
     * stable: time it stays at one color
     * brightness: max brightness for the random color
     */
    void setAutoHSV(uint16_t nTransition, uint16_t nStable);

    void setAutoHSV_single(uint16_t nTransition, uint16_t nStable);


    void setEffectFlash(Color color, uint32_t duration);
    void setEffectBeam(Color color, uint16_t radius, uint16_t speed, bool reverse=false);
    void setFillHSV(uint16_t nTransition, uint16_t nStable,
            uint16_t speed, bool reverse=false);
    void setEmpty(Color color, uint16_t speed, bool reverse=false);


    void setColor(Color color);
    void setColorSingle(Color color[]);

    void update(uint32_t now);

  private:
    enum Mode mode = MODE_AUTO;
    enum Effect effect = EFFECT_NONE;
    
    int16_t size;

    uint32_t lastTic = 0;
    uint32_t nowTic = 0;
    uint32_t effectTic = 0;

    int16_t beamRadius; // linearly from 100 to 0 %
    int16_t beamSpeed; // ms per LED
    uint32_t effectDuration;

    uint32_t delay1 = 0;
    uint32_t delay2 = 0;


    Color color1;
    Color color2;
    Color effectColor;

    Color color1t[NUM_PIXEL];
    Color color2t[NUM_PIXEL];
    Color *colort;

    uint8_t brightness = CONTROLLER_MAX_BRIGHTNESS; // brightness of the color (set a top boundary (max 255))
    uint16_t transition = 900; // time to do a transition (in millisecond)
    uint16_t stable = 100; // set how much time a color should stay before changing (in millesecond)

    // Private function

    void updateBlink();
    void updateBreath();
    void updateAuto();

    void updateAutoSingle();

    void updateFlash();
    void updateBeam();
    void updateFillHSV();
    void updateEmpty();

};

#endif
