#ifndef __BUTTON_H_
#define __BUTTON_H_

#include "Arduino.h"

#define BUTTON_PRESS   -1 // -1 for pull-up, +1 pull-down
#define BUTTON_RELEASE +1 //  1 for pull-up, -1 pull-down

class Button {
    public:
        Button(uint16_t pin, uint32_t debounceTime);

        void init();

        // return -1 if falling edge, +1 if rising, 0 else
        int16_t update(uint32_t nowTic);

    private:
        uint32_t _trigTic;
        uint32_t _debounceTime;
        uint16_t _pin; 

        bool _old_state = false;
        bool _up_trigged = false;
        bool _down_trigged = false;
        bool _state = true;
};


#endif
