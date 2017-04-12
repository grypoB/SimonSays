#include "control.h"

static RGBConverter conv; 
static int min_h, max_h; // from 360
static int min_s, max_s; // from 100
static int min_v, max_v; // from 100
static double h,s,v;
static unsigned char rgb[3];


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
        case MODE_MANUAL: //don't do anything :(
        break;
    }

    if (mode == MODE_AUTO_HSV_SINGLE) {
      if (effect == EFFECT_BEAM) {
        updateBeam();
      }
      
      setColorSingle(colort);
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
        
        if (mode==MODE_AUTO_HSV) {
            color2.randHSV();
        } else {
            color2.rand(brightness); // new color to go to
        }

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

void Controller::updateAutoSingle() {
    int i;

    for (i=0 ; i<NUM_PIXEL ; i++) {
      colort[i] = color2t[i];
    }
    
    if (nowTic - lastTic > (unsigned long) (stable+transition)) // check if it's time to change color
    {
        lastTic = nowTic;
        for (i=0 ; i<NUM_PIXEL ; i++) {
          color1t[i] = color2t[i];
          color2t[i].randHSV();
        }
    }
    else if (nowTic - lastTic < (unsigned long) transition)// check if we are still in a transition (change color accordingly)
    {
        for (i=0 ; i<NUM_PIXEL ;i++) {
          colort[i].fade(color1t[i],color2t[i], nowTic, lastTic, lastTic+transition);
        }
    }
    
    // apply color to led strip
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

void Controller::updateBeam() {
  
    int32_t centerPixel = (nowTic-effectTic)/beamSpeed;
    
    for (int i=centerPixel-beamRadius ; i<=centerPixel+beamRadius ; i++) {
        if (i>=0 && i<strip->numPixels()) {
            colort[i] = beamColor;
            //colort[i].mix(colort[i], beamColor,
                          //((double) (beamRadius-abs(i-centerPixel)))/beamRadius);
        }
    }
    
    if (centerPixel-beamRadius >= NUM_PIXEL) {
        effect = EFFECT_NONE;
    }
    
    


}

void Color::randHSV() {
        // create a random color (in function of the max)
        h = random(min_h, max_h+1);  
        s = random(min_s, max_s+1);  
        v = random(min_v, max_v+1);  

        conv.hsvToRgb(h/360,s/100,v/100, rgb);

        red   = rgb[0];
        green = rgb[1];
        blue  = rgb[2];
}

void Color::setHSV(int nMin_h, int nMax_h, int nMin_s, int nMax_s, int nMin_v, int nMax_v) {
      min_h = nMin_h;
      max_h = nMax_h;
      min_s = nMin_s;
      max_s = nMax_s;
      min_v = nMin_v;
      max_v = nMax_v;
}


