#include "Fsm.h"

Fsm::Fsm(Button *pButt) {
    _state = IDLE;
    _pButt = pButt;
    _rankCombi = 0;
}

void Fsm::update(uint32_t nowTic) {
    int16_t buttonState = _pButt->update(nowTic);

    switch(_state) {
        case IDLE:
            if (buttonState == BUTTON_PRESS) {
                Serial.println("was Idle");
                Serial.println("pressed!");
                generateCombi();
                _state = TRUE_COMBI;
            }
            break;
        case TRUE_COMBI:
            Serial.print("true combi :");
            for (int i=0 ; i<FSM_COMBI_LENGTH ; i++) {
                Serial.print(_combi[i]);
            }
            Serial.print("\n");
            _buttSelect = -1;
            _rankCombi = 0;
            _lastSelectTic = nowTic;
            _state = USER_COMBI;
            break;
        case USER_COMBI:
            updateUserCombi(buttonState, nowTic);
            break;
        case WIN:
            Serial.println("WIN!");
            _state = IDLE;
            break;
        case GAME_OVER:
            Serial.println("GAME OVER!");
            _state = IDLE;
        case IN_OUTPUT:
            Serial.println("doing some output");
            break;
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
        Serial.print("Select = ");
        Serial.println(_buttSelect);
        _lastSelectTic = nowTic;
    }

    if (nowTic - _lastSelectTic > FSM_SELECT_TIMEOUT) {
        Serial.print("End Select = ");
        Serial.println(_buttSelect);

        if (_buttSelect == _combi[_rankCombi]) {
            Serial.println("correct");

            _rankCombi++;
            _buttSelect = -1;
            _lastSelectTic = nowTic;

            if (_rankCombi == FSM_COMBI_LENGTH) {
                _state = WIN;
            }
        } else {
            _state = GAME_OVER;
        }

    }
}
