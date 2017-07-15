#ifndef __FSM_H_
#define __FSM_H_

#include "Arduino.h"
#include "Button.h"

#define FSM_MAX_SELECT 4
#define FSM_SELECT_TIMEOUT 500
#define FSM_COMBI_LENGTH 4

enum Fsm_state {IDLE, TRUE_COMBI, USER_COMBI, WIN, GAME_OVER, IN_OUTPUT};

class Fsm {
    public:
        Fsm(Button *pButt);

        void update(uint32_t nowTic);

    private:
        Fsm_state _state;
        Button *_pButt;
        uint8_t _combi[FSM_COMBI_LENGTH];
        
        int16_t _rankCombi;
        int16_t _buttSelect;
        uint32_t _lastSelectTic;

        
        void generateCombi();
        void updateUserCombi(int16_t buttonState, uint32_t nowTic);
};


#endif
