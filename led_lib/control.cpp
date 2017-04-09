#include "control.h"

void Controller::update(uint32_t now) {
    nowTic = now;
    
    switch (mode) {
        case MODE_AUTO: updateAuto();
        break;
        case MODE_BLINK: updateBlink();
        break;
        case MODE_BREATH: updateBreath();
        break;
        case MODE_MANUAL: //don't do anything :(
        break;
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
    
    if (nowTic - lastTic > (unsigned long) (stable+transition)) // check if it's time to change color
    {
        color1 = color2;
        lastTic = nowTic;
        
        color2.rand(brightness); // new color to go to
    }
    else if (nowTic - lastTic < (unsigned long) transition)// check if we are still in a transition (change color accordingly)
    {
        //color[i] = ((long)(toColor[i]-initialColor[i])*(long)(nowTic - lastTic))/transition + initialColor[i];
        // change color according to the linear function
        color.fade(color1,color2, nowTic, lastTic, lastTic+transition);
    }
    
    // apply color to led strip
    setColor(color);
}

void Controller::updateBlink() {
    uint32_t spent = (nowTic - lastTic);
    if (spent < delay1) {
        setColor(color1);
    } else if (spent < delay1+delay2) {
        setColor(color2);
    } else {
        lastTic = nowTic;
    }
}
