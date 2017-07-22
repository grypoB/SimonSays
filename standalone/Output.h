#ifndef __OUTPUT_H_
#define __OUTPUT_H_

#include "Arduino.h"
#include "Controller.h"

enum Output_state {OUTPUT_NONE, OUTPUT_IDLE, OUTPUT_DELAY};

class Output {
    public:
        Output(Controller *pCont, char *buffer);

        void update(uint32_t nowTic);
        bool busy();
        
        void idle(); 
        void game_start();
        void button_press(int16_t butt, bool fake); 
        void end_of_combi();
        void flash();
        void next_combi_rank(bool correct);
        void win(); 
        void game_over(); 

    private:
        Controller *_pCont;
        Output_state _state;
        uint32_t _lastTic;
        uint32_t _delay;

        char *_buffer;

        Color convertButton(int16_t butt);
        void wait(uint32_t delay);
};


#endif
