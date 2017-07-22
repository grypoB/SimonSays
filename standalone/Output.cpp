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
#define DELAY_GAME_START 1000

namespace {
    // Base colors
    Color white = Color(255, 255, 255);
    Color red   = Color(255,0,0);
    Color green = Color(0,255,0);
    Color purple = Color(161,0,255);
    Color blue  = Color(0,0,255);
    Color black = Color(0,0,0);
    Color flashColor    = Color(255,255,255);
    Color nextRankColor = Color(255,208,0);
    Color gameStartColor = Color(255,208,0);
}

Output::Output(Controller *pCont, char *buffer) {
    _pCont = pCont;
    _state = OUTPUT_NONE;
    _buffer = buffer;

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
    //Serial.println("u");
    //sprintf(_buffer + strlen(_buffer), "u\n");
    return;
}

void Output::idle() {
    if (_state != OUTPUT_IDLE) {
        _pCont->setAutoHSV(TRANSITION,STABLE);
        sprintf(_buffer + strlen(_buffer), "IDLE\n");
    }

    _state = OUTPUT_IDLE;
}


void Output::game_start() {
    sprintf(_buffer + strlen(_buffer), "Game start\n");

    _pCont->setManual(black); 
    _pCont->setEffectFlash(gameStartColor, DELAY_GAME_START);

    wait(DELAY_GAME_START);
}

void Output::button_press(int16_t butt, bool fake) {
    uint32_t delay = 0;

    if (fake) {
        sprintf(_buffer + strlen(_buffer), "Fake ");
        delay = BUTTON_FAKE_DURATION;
    } else {
        sprintf(_buffer + strlen(_buffer), "Pressed ");
        delay = BUTTON_DURATION;
    }

    sprintf(_buffer + strlen(_buffer), "%d \n", butt);
    //_pCont->setManual(convertButton(butt));

    wait(delay);
}

void Output::flash() {
    sprintf(_buffer + strlen(_buffer), "Flash !\n");
    _pCont->setEffectFlash(flashColor, DELAY_FLASH);

    wait(DELAY_FLASH);
}

void Output::end_of_combi() {
    sprintf(_buffer + strlen(_buffer), "End of fake sequence\n");

    _state = OUTPUT_NONE;
}

void Output::next_combi_rank(bool correct) {
    uint32_t delay = 0;

    //_pCont->setManual(black);

    sprintf(_buffer + strlen(_buffer), "Selected ");
    if (correct) {
        sprintf(_buffer + strlen(_buffer), "correct\n");

        delay = DELAY_NEXT_RANK_COMBI;
        _pCont->setEffectFlash(nextRankColor, DELAY_NEXT_RANK_COMBI);
    } else {
        sprintf(_buffer + strlen(_buffer), "WRONG\n");
    }

    wait(delay);
}

void Output::win() {
    sprintf(_buffer + strlen(_buffer), "WIN!\n");

    _state = OUTPUT_NONE;
}


void Output::game_over() {
    sprintf(_buffer + strlen(_buffer), "Game Over!\n");

    _state = OUTPUT_NONE;
}

Color Output::convertButton(int16_t butt) {
    if (butt == -1) {
        return white;
    } else if (butt == 0) {
        return red;
    } else if (butt == 1) {
        return green;
    } else if (butt == 2) {
        return purple;
    } else if (butt == 3) {
        return blue;
    } else {
        return black;
    }
}

void Output::wait(uint32_t delay) {
    _lastTic = 0;
    _delay = delay;
    _state = OUTPUT_DELAY;
}
