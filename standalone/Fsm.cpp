#include "Fsm.h"

Fsm::Fsm(Button *pButt, Output *pOutput) {
    _state = IDLE;
    _pButt = pButt;
    _pOutput = pOutput;
    _rankCombi = 0;
}

void Fsm::update(uint32_t nowTic) {
    int16_t buttonState = _pButt->update(nowTic);

    if (!_pOutput->busy()) {
        switch(_state) {
            case IDLE:
                _pOutput->idle();
                if (buttonState == BUTTON_PRESS) {
                    _pOutput->button_press(-1, false);
                    generateCombi();
                    _rankCombi = 0;
                    _state = TRUE_COMBI;
                }
                break;
            case TRUE_COMBI:
                if (_rankCombi < FSM_COMBI_LENGTH) {
                    _pOutput->button_press(_combi[_rankCombi], true);
                    _rankCombi++;
                } else if (_rankCombi == FSM_COMBI_LENGTH) {
                    _pOutput->end_of_press();

                    _buttSelect = -1;
                    _rankCombi = 0;
                    _lastSelectTic = nowTic;
                    _state = USER_COMBI;
                }
                break;
            case USER_COMBI:
                updateUserCombi(buttonState, nowTic);
                break;
            case WIN:
                _pOutput->win();
                _state = IDLE;
                break;
            case GAME_OVER:
                _pOutput->game_over();
                _state = IDLE;
        }
    }
}


void Fsm::generateCombi() {
    for (int i=0 ; i<FSM_COMBI_LENGTH ; i++) {
        _combi[i] = random(0,FSM_MAX_SELECT);
    }
}

void Fsm::updateUserCombi(int16_t buttonState, uint32_t nowTic) {
    if (buttonState == BUTTON_PRESS) {
        _buttSelect = (_buttSelect+1) % FSM_MAX_SELECT;

        _pOutput->button_press(_buttSelect, false);

        _lastSelectTic = nowTic;
    }

    if (nowTic - _lastSelectTic > FSM_SELECT_TIMEOUT) {

        if (_buttSelect == _combi[_rankCombi]) {
            _pOutput->next_combi_rank(true);

            _rankCombi++;
            _buttSelect = -1;
            _lastSelectTic = nowTic;

            if (_rankCombi == FSM_COMBI_LENGTH) {
                _state = WIN;
            }
        } else {
            _pOutput->next_combi_rank(false);
            _state = GAME_OVER;
        }

    }
}
