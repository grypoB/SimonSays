#include "control.h"
#include "RGBConverter.h"
#include <Adafruit_NeoPixel.h>

#define LENGTH_B      20
#define LENGTH_C      20
#define TOTAL_LENGTH  150

#define LENGTH_W      (TOTAL_LENGTH/2-LENGTH_C/2-LENGTH_B)
#define PIN1          6
#define PIN2          7
#define MAX           15

#define TRANSITION 250
#define STABLE     0

#define MIN_H 140
#define MAX_H 280

#define MIN_S 50
#define MAX_S 100

#define MIN_V 10
#define MAX_V 20

#define TIME 2000
#define BEAM_SPEED 7
#define BEAM_SIZE 4

#define FLASH 130
#define BREATH 500

#define BREATH_UP  100
#define BREATH_DOWN 250

Color color_strip1[TOTAL_LENGTH];
Color color_strip2[TOTAL_LENGTH];

Color black;

Color cb1 = Color(MAX,0,0);
Color cb2 = Color(0,MAX,0);
Color wa1 = cb1;
Color wa2 = cb2;
Color cry = Color(MAX/2,MAX/2,MAX);

Color cb3 = Color(0,0,MAX);
Color cb4 = Color(MAX,MAX*0.8,0);
Color wa3 = cb3;
Color wa4 = cb4;

Adafruit_NeoPixel strip1 = Adafruit_NeoPixel(TOTAL_LENGTH, PIN1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(TOTAL_LENGTH, PIN2, NEO_GRB + NEO_KHZ800);

Controller butt1 = Controller(color_strip1                             , LENGTH_B, cb1);
Controller wall1 = Controller(color_strip1+LENGTH_B                    , LENGTH_W, wa2);
Controller crysA = Controller(color_strip1+LENGTH_B+LENGTH_W           , LENGTH_C, cry);
Controller wall2 = Controller(color_strip1+LENGTH_B+LENGTH_W+LENGTH_C  , LENGTH_W, wa2);
Controller butt2 = Controller(color_strip1+LENGTH_B+2*LENGTH_W+LENGTH_C, LENGTH_B, cb2);

Controller butt3 = Controller(color_strip2                             , LENGTH_B, cb3);
Controller wall3 = Controller(color_strip2+LENGTH_B                    , LENGTH_W, wa3);
Controller crysB = Controller(color_strip2+LENGTH_B+LENGTH_W           , LENGTH_C, cry);
Controller wall4 = Controller(color_strip2+LENGTH_B+LENGTH_W+LENGTH_C  , LENGTH_W, wa4);
Controller butt4 = Controller(color_strip2+LENGTH_B+2*LENGTH_W+LENGTH_C, LENGTH_B, cb4);

Controller *ctrl = NULL;

uint32_t now  = millis();
uint32_t last = millis();

int cmd  = 0;
int area = 0;

bool direction = false;

void update_strip(Adafruit_NeoPixel *pStrip, Color color_strip[]) {
    for (int i=0 ; i<pStrip->numPixels() ; i++) {
        pStrip->setPixelColor(i, pStrip->Color(color_strip[i].red,
                                               color_strip[i].green,
                                               color_strip[i].blue));
    }
    pStrip->show();
}

void update_strips() {
    update_strip(&strip1, color_strip1);
    update_strip(&strip2, color_strip2);
}

void update_controllers() {
  butt1.update(now);
  butt2.update(now);
  wall1.update(now);
  wall2.update(now);
  crysA.update(now);

  butt3.update(now);
  butt4.update(now);
  wall3.update(now);
  wall4.update(now);
  crysB.update(now);
}

void setup() {
  Serial.begin(9600);
  //Serial.println(LENGTH_W);
  //Serial.println(LENGTH_B+2*LENGTH_W+LENGTH_C);
  strip1.begin();
  strip2.begin();
  
  black.setHSV(MIN_H, MAX_H, MIN_S, MAX_S, MIN_V, MAX_V);

  butt1.setBreath(cb1,black,BREATH,BREATH);
  butt2.setBreath(cb2,black,BREATH,BREATH);
  wall1.setBreath(wa1,black,BREATH,BREATH);
  wall2.setBreath(wa2,black,BREATH,BREATH);
  crysA.setBreath(black,cry,BREATH_UP,BREATH_DOWN);

  butt3.setBreath(cb3,black,BREATH,BREATH);
  butt4.setBreath(cb4,black,BREATH,BREATH);
  wall3.setBreath(wa3,black,BREATH,BREATH);
  wall4.setBreath(wa4,black,BREATH,BREATH);
  crysB.setBreath(black,cry,BREATH_UP,BREATH_DOWN);
}

void loop() {
  now = millis();

  while (Serial.available()>=2) {
    //Serial.println("copy");
    cmd  = Serial.read();
    area = Serial.read();

    switch(area) {
      case 0: ctrl = &crysA; break; // actually need crysB also
      case 1: ctrl = &butt1; break;
      case 2: ctrl = &butt2; break;
      case 3: ctrl = &butt3; break;
      case 4: ctrl = &butt4; break;
      case 5: ctrl= &wall1; break;
      case 6: ctrl= &wall2; break;
      case 7: ctrl= &wall3; break;
      case 8: ctrl= &wall4; break;
      case 9: ctrl = &crysB; break;
      default: Serial.println("Unknown area"); break;
    }

    if (area%2==1) {
      direction = false;
    } else {
      direction = true;
    }
    
    switch (cmd) {
      case 0: ctrl->setManual     (black);                                           break;
      case 1: ctrl->setEffectBeam (ctrl->mainColor,BEAM_SIZE, BEAM_SPEED,direction); break;
      case 2: ctrl->setBlink      (ctrl->mainColor, black,BREATH, BREATH);           break;
      case 3: ctrl->setFillHSV    (TRANSITION, STABLE, BEAM_SPEED, direction);       break;
      case 4: ctrl->setEffectFlash(ctrl->mainColor, FLASH);                          break;
      case 5:
        while(Serial.available()<2); // wait for next bytes
        ctrl->setBreath(ctrl->mainColor, black, Serial.read()*10, Serial.read()*10);
    }

    Serial.print(cmd);
    Serial.print(" ");
    Serial.println(area);
  }

  update_controllers();
  update_strips();

  //delay(10);
}
