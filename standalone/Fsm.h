#ifndef __FSM_H_
#define __FSM_H_

#include "Arduino.h"
#include "Button.h"
#include "Output.h"

#define FSM_MAX_SELECT 4
#define FSM_SELECT_TIMEOUT 1000
#define FSM_INITIAL_SELECT_TIMEOUT 5000
#define FSM_COMBI_LENGTH 4

enum Fsm_state {IDLE, TRUE_COMBI, USER_COMBI, WIN, GAME_OVER};

class Fsm {
    public:
        Fsm(Button *pButt, Output *pOutput);

        void update(uint32_t nowTic);

    private:
        Fsm_state _state;
        Button *_pButt;
        Output *_pOutput;
        uint8_t _combi[FSM_COMBI_LENGTH];
        
        int16_t _rankCombi;
        int16_t _buttSelect;
        uint32_t _lastSelectTic;

        
        void generateCombi();
        void updateUserCombi(int16_t buttonState, uint32_t nowTic);
};


#endif
