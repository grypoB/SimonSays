#define BUTTON 9 // pin number

#define DEBOUNCE 40 // in ms

#define BAUD 9600 // serial baud rate


// pull-up
class Button {
    public:
        bool _old_state = false;
        bool _up_trigged = false;
        bool _down_trigged = false;
        bool _state = true;
        uint16_t _pin;

        Button(uint16_t pin) : _pin(pin) {
            _state = digitalRead(_pin);
        };

        int16_t update(uint32_t nowTic) {
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
            } else if ((_up_trigged||_down_trigged) && nowTic-_trigTic>DEBOUNCE) {
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
        };

    private:
        uint32_t _trigTic;
};


Button butt = Button(BUTTON);
static int16_t press = 0;

void setup() {
    Serial.begin(BAUD);
    pinMode(BUTTON, INPUT);

    delay(1000);

    Serial.println("Startup");
}


void loop() {
    uint32_t now = millis();

    int16_t result = butt.update(now);

    if(result == 1) {
        press++;
        Serial.print("relea ");
        Serial.println(press);
    } else if (result == -1) {
        press--;
        Serial.print("press ");
        Serial.println(press);
    }
}
