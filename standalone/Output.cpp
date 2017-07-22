#include "Output.h"

#define BUTTON_FAKE_DURATION 1000
#define BUTTON_DURATION 10

Output::Output() {
    _state = OUTPUT_NONE;
}

bool Output::busy() {
    switch(_state) {
        case OUTPUT_NONE:
        case OUTPUT_IDLE:
            return false;
            break;
        case OUTPUT_BUTTON:
            return true;
            break;
    }

    return true;
}

void Output::update(uint32_t nowTic) {
    if (_lastTic == 0) {
        _lastTic = nowTic; 
    }

    switch(_state) {
        case OUTPUT_NONE:
        case OUTPUT_IDLE:
            break;
        case OUTPUT_BUTTON:
            if (nowTic-_lastTic > _delay) {
                _state = OUTPUT_NONE;
            }
            break;
    }
}

void Output::idle() {
    if (_state != OUTPUT_IDLE) {
        Serial.println("IDLE");
    }

    _state = OUTPUT_IDLE;
}

void Output::button_press(int16_t butt, bool fake) {
    if (fake) {
        Serial.print("Fake ");
        _delay = BUTTON_FAKE_DURATION;
    } else {
        Serial.print("Pressed ");
        _delay = BUTTON_DURATION;
    }
    Serial.println(butt);

    _lastTic = 0;
    _state = OUTPUT_BUTTON;
}

void Output::end_of_press() {
    Serial.println("End of fake sequence");

    _state = OUTPUT_NONE;
}

void Output::next_combi_rank(bool correct) {
    Serial.print("Selected ");
    if (correct) {
        Serial.println("correct");
    } else {
        Serial.println("WRONG");
    }

    _state = OUTPUT_NONE;
}

void Output::win() {
    Serial.println("WIN!");

    _state = OUTPUT_NONE;
}


void Output::game_over() {
    Serial.println("Game over!");

    _state = OUTPUT_NONE;
}
