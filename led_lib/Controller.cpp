#include "Controller.h"

Controller::Controller(Color nStrip[], int16_t nSize, Color nMainColor)
    : mainColor(nMainColor), size(nSize), colort(nStrip) {};

void Controller::update(uint32_t now) {
    nowTic = now;

    switch (mode) {
        case MODE_AUTO_HSV:
        case MODE_AUTO: updateAuto();
            break;
        case MODE_BLINK: updateBlink();
            break;
        case MODE_BREATH: updateBreath();
            break;
        case MODE_AUTO_HSV_SINGLE: updateAutoSingle();
            break;
        case MODE_FILL_HSV:
        case MODE_FILL_HSV_REVERSE: updateFillHSV();
            break;
        case MODE_EMPTY:
        case MODE_EMPTY_REVERSE: updateEmpty();
            break;
        case MODE_MANUAL: setColor(color1);
            break;
    }


    if (effect != EFFECT_NONE) {
        if (effectTic == 0) {
            // happens in case of lag
            // some effects timer are set to 0 to insure they play in full
            effectTic = nowTic;
        }

        switch (effect) {
            case EFFECT_BEAM:
            case EFFECT_BEAM_REVERSE: updateBeam();
                break;
            case EFFECT_FLASH: updateFlash();
            case EFFECT_NONE: //don't do anything
                break;
        }
    }
}


// ============================================================================
// Set mode
// ============================================================================

// ------------------------- Full mode ----------------------------------------

void Controller::setBreath(Color nColor1, Color nColor2,
                           uint16_t upTime, uint16_t downTime) {
    mode = MODE_BREATH;
    color1 = nColor1;
    color2 = nColor2;

    delay1 = upTime;
    delay2 = downTime;
}


void Controller::setAuto(uint16_t nTransition, uint16_t nStable, uint8_t nBrightness) {
    mode = MODE_AUTO;

    brightness = nBrightness;
    transition = nTransition;
    stable     = nStable;
}


void Controller::setAutoHSV(uint16_t nTransition, uint16_t nStable) {
    mode = MODE_AUTO_HSV;

    transition = nTransition;
    stable     = nStable;
}


void Controller::setManual(Color color) {
    mode = MODE_MANUAL;
    color1 = color;
}


void Controller::setBlink(Color nColor1, Color nColor2, uint32_t nDelay1, uint32_t nDelay2) {
    mode = MODE_BLINK;

    color1 = nColor1;
    color2 = nColor2;

    delay1 = nDelay1;
    delay2 = nDelay2;
}


// ------------------------- Set effect ---------------------------------------

void Controller::setEffectFlash(Color color, uint32_t duration) {
    effect = EFFECT_FLASH;

    effectColor = color;
    effectDuration = duration;

    effectTic = 0;
}


void Controller::setEffectBeam(Color color, uint16_t radius, uint16_t speed, bool reverse) {
    if (reverse) {
        effect = EFFECT_BEAM_REVERSE;
    } else {
        effect = EFFECT_BEAM;
    }
    effectColor = color;
    beamRadius  = radius;
    beamSpeed   = speed;
    effectTic   = 0;
}


void Controller::setFillHSV(uint16_t nTransition, uint16_t nStable, uint16_t speed, bool reverse) {
    transition = nTransition;
    stable     = nStable;

    if (reverse) {
        mode = MODE_FILL_HSV_REVERSE;
    } else {
        mode = MODE_FILL_HSV;
    }

    beamSpeed = speed;
    effectTic = 0;

    for (int16_t i=0 ; i<size ; i++) {
        color1t[i].randHSV();
        colort[i].set(0,0,0); // set black background
    }
}


void Controller::setEmpty(Color color, uint16_t speed, bool reverse) {
    if (reverse) {
        mode = MODE_EMPTY_REVERSE;
    } else {
        mode = MODE_EMPTY;
    }

    beamSpeed = speed;
    effectTic = 0;

    color1 = color;
}


// ------------------------- Singles mode -------------------------------------

void Controller::setAutoHSV_single(uint16_t nTransition, uint16_t nStable) {
    mode = MODE_AUTO_HSV_SINGLE;

    transition = nTransition;
    stable     = nStable;
}



// ============================================================================
// Set colors
// ============================================================================

void Controller::setColor(Color color) {
    for (int16_t i=0 ; i<size ; i++) {
        colort[i] = color;
    }
}


void Controller::setColorSingle(Color color[]) {
    for (int16_t i=0 ; i<size ; i++) {
        colort[i] = color[i];
    }
}



// ============================================================================
// Update mode
// ============================================================================

// ------------------------- Full mode ----------------------------------------

void Controller::updateBlink() {
    uint32_t spent = nowTic - lastTic;

    if (spent < delay1) {
        setColor(color1);
    } else if (spent < delay1+delay2) {
        setColor(color2);
    } else {
        lastTic = nowTic;
    }
}


void Controller::updateBreath() {
    Color color = color1;
    uint32_t spent = nowTic - lastTic;

    if (spent < delay1) {
        color.fade(color1, color2, spent, 0, delay1);
    } else if (spent < delay1+delay2) {
        color.fade(color2, color1, spent, delay1, delay1+delay2);
    } else {
        lastTic = nowTic;
    }

    setColor(color);
}


void Controller::updateAuto() {
    Color color = color2;

    if (nowTic-lastTic > stable+transition) { // check if it's time to change color
        color1 = color2;
        lastTic = nowTic;

        if (mode == MODE_AUTO_HSV) {
            color2.randHSV();
        } else {
            color2.rand(brightness); // new color to go to
        }

    }
    else if (nowTic-lastTic < transition) {
        // check if we are still in a transition
        // change color according to the linear function
        color.fade(color1,color2, nowTic, lastTic, lastTic+transition);
    }

    setColor(color); // apply color to led strip
}

// ------------------------- Single mode --------------------------------------

void Controller::updateAutoSingle() {
    for (int16_t i=0 ; i<size ; i++) {
        colort[i] = color2t[i];
    }

    if (nowTic-lastTic > stable+transition) { // check if it's time to change color
        lastTic = nowTic;
        for (int16_t i=0 ; i<size ; i++) {
            color1t[i] = color2t[i];
            color2t[i].randHSV();
        }
    } else if (nowTic-lastTic < transition) { // check if we are still in a transition (change color accordingly)
        for (int16_t i=0 ; i<size ; i++) {
            colort[i].fade(color1t[i], color2t[i], nowTic, lastTic, lastTic+transition);
        }
    }
}


// ------------------------- Effects ------------------------------------------

void Controller::updateFlash() {
    if (nowTic-effectTic < effectDuration) {
        setColor(effectColor);
    } else {
        effect = EFFECT_NONE;
    }
}


void Controller::updateBeam() {
    int16_t centerPixel = (nowTic-effectTic)/beamSpeed;

    if (effect == EFFECT_BEAM_REVERSE) {
        centerPixel = size - centerPixel - 1;
    }

    for (int16_t i=centerPixel-beamRadius ; i<=centerPixel+beamRadius ; i++) {
        if (i>=0 && i<size) {
            colort[i] = effectColor;
        }
    }

    if (centerPixel-beamRadius >= size || centerPixel+beamRadius<0) {
        effect = EFFECT_NONE;
    }
}


void Controller::updateFillHSV() {
    int16_t centerPixel = (nowTic-effectTic)/beamSpeed;
    int16_t dir = -1;

    if (mode == MODE_FILL_HSV_REVERSE) {
        dir = +1;
        centerPixel = size - centerPixel - 1;
    }

    for (int16_t i=centerPixel; i>=0 && i<size; i+= dir) {
        colort[i] = color1t[i];
    }

    if (centerPixel>=size || centerPixel<0) {
        mode = MODE_AUTO_HSV_SINGLE;
        lastTic = nowTic;
    }
}


void Controller::updateEmpty() {
    int16_t centerPixel = (nowTic-effectTic)/beamSpeed;
    int16_t dir = +1;

    if (mode == MODE_EMPTY_REVERSE) {
        dir = -1;
    } else {
        centerPixel = size - centerPixel - 1;
    }

    for (int16_t i=centerPixel; i>=0 && i<size ; i+= dir) {
        colort[i] = color1;
    }

    if (centerPixel>=size || centerPixel<0) {
        mode = MODE_MANUAL;
        lastTic = nowTic;
    }
}
