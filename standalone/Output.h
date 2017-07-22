#ifndef __OUTPUT_H_
#define __OUTPUT_H_

#include "Arduino.h"

enum Output_state {OUTPUT_NONE, OUTPUT_IDLE, OUTPUT_BUTTON};

class Output {
    public:
        Output();

        void update(uint32_t nowTic);
        bool busy();
        
        void idle(); 
        void button_press(int16_t butt, bool fake); 
        void end_of_press();
        void next_combi_rank(bool correct);
        void win(); 
        void game_over(); 

    private:
        Output_state _state;
        uint32_t _lastTic;
        uint32_t _delay;
};


#endif
