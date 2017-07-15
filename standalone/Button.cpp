#include "Button.h"

Button::Button(uint16_t pin, uint32_t debounceTime)
    : _debounceTime(debounceTime), _pin(pin) {};

void Button::init() {
    pinMode(_pin, INPUT);
    _state = digitalRead(_pin);
}

int16_t Button::update(uint32_t nowTic) {
    bool cur_state = digitalRead(_pin);
    int16_t button_pressed = 0;

    if        (cur_state==LOW  && _old_state==HIGH) { // falling edge
        _up_trigged    = false;
        _down_trigged  = true;
        _trigTic = nowTic;
    } else if (cur_state==HIGH && _old_state==LOW ) { // rising edge
        _up_trigged    = true;
        _down_trigged  = false;
        _trigTic = nowTic;
    } else if ((_up_trigged||_down_trigged) && cur_state==_state) { // changed too soon
        _up_trigged = false;
        _down_trigged = false;
    } else if ((_up_trigged||_down_trigged) && nowTic-_trigTic>_debounceTime) {
        if (_up_trigged) {
            button_pressed = 1; 
            _state = true;
        } else {
            button_pressed = -1; 
            _state = false;
        }

        _up_trigged   = false;
        _down_trigged = false;
    }

    _old_state = cur_state;

    return button_pressed;
}
