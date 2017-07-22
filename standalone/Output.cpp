#include "Output.h"

#define BUTTON_FAKE_DURATION 1000
#define BUTTON_DURATION 10

#define TRANSITION 1000
#define STABLE     400

#define MIN_H -100
#define MAX_H 30
#define MIN_S 90
#define MAX_S 100
#define MIN_V 60
#define MAX_V 70

#define DELAY_FLASH 100
#define DELAY_NEXT_RANK_COMBI 100

namespace {
    // Base colors
    Color white = Color(255, 255, 255);
    Color red   = Color(255,0,0);
    Color green = Color(0,255,0);
    Color purple = Color(161,0,255);
    Color blue  = Color(0,0,255);
    Color black = Color(0,255,0);
    Color flashColor    = Color(255,255,255);
    Color nextRankColor = Color(255,208,0);
}

Output::Output(Controller *pCont) {
    _pCont = pCont;
    _state = OUTPUT_NONE;

    black.setHSV(MIN_H, MAX_H, MIN_S, MAX_S, MIN_V, MAX_V);
}

bool Output::busy() {
    switch(_state) {
        case OUTPUT_NONE:
        case OUTPUT_IDLE:
            return false;
            break;
        case OUTPUT_DELAY:
            return true;
            break;
    }

    return true;
}

void Output::update(uint32_t nowTic) {
    switch(_state) {
        case OUTPUT_NONE:
        case OUTPUT_IDLE:
            break;
        case OUTPUT_DELAY:
            if (_lastTic == 0) {
                _lastTic = nowTic; 
            } else if (nowTic-_lastTic > _delay) {
                _state = OUTPUT_NONE;
            }
            break;
    }

    _pCont->update(nowTic);
}

void Output::idle() {
    if (_state != OUTPUT_IDLE) {
        _pCont->setAutoHSV(TRANSITION,STABLE);
        Serial.println("IDLE");
    }

    _state = OUTPUT_IDLE;
}


void Output::game_start() {
    Serial.println("Game start");

    _pCont->setManual(black); 
    _pCont->setEffectFlash(flashColor, DELAY_NEXT_RANK_COMBI);

    wait(DELAY_NEXT_RANK_COMBI);
}

void Output::button_press(int16_t butt, bool fake) {
    uint32_t delay = 0;
    if (fake) {
        Serial.print("Fake ");
        _pCont->setManual(convertButton(butt));
        delay = BUTTON_FAKE_DURATION;
    } else {
        Serial.print("Pressed ");
        delay = BUTTON_DURATION;
    }

    Serial.println(butt);
    _pCont->setManual(convertButton(butt));

    wait(delay);
}

void Output::flash() {
    _pCont->setEffectFlash(flashColor, DELAY_FLASH);

    wait(DELAY_FLASH);
}

void Output::end_of_combi() {
    Serial.println("End of fake sequence");

    _state = OUTPUT_NONE;
}

void Output::next_combi_rank(bool correct) {
    uint32_t delay = 0;

    _pCont->setManual(black);

    Serial.print("Selected ");
    if (correct) {
        Serial.println("correct");

        delay = DELAY_NEXT_RANK_COMBI;
        _pCont->setEffectFlash(nextRankColor, DELAY_NEXT_RANK_COMBI);
    } else {
        Serial.println("WRONG");
    }

    wait(delay);
}

void Output::win() {
    Serial.println("WIN!");

    _state = OUTPUT_NONE;
}


void Output::game_over() {
    Serial.println("Game over!");

    _state = OUTPUT_NONE;
}
