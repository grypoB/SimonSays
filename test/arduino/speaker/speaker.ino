/*
  Melody

 Plays a melody

 circuit:
 * 8-ohm speaker on digital pin 8

 created 21 Jan 2010
 modified 30 Aug 2011
 by Tom Igoe

This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/Tone

 */
#include "pitches.h"
#define PIN 10
#define BEAT_PERIOD 500 // in ms
#define MELODY_LENGTH 19

#define PAUSE_INTER_NOTES_RATIO 0.3

bool musicPlaying = false;
bool startedTone = false;
int currNote = 0;
int startNoteTic = 0;

int melody[] = {
    C4,CS4,D4,0,0,
    G3,D4,0,F4,
    F4,E4,D4,
    G3,E3,0,E3,
    C3,0,0
};

int noteDurations[] = {
    16,16,8,4,2,
    4,4,4,4,
    3,3,3,
    4,4,4,4,
    4,4,2
};
void setup() {
    Serial.begin(9600);
    pinMode(PIN, OUTPUT);
    initMusic(millis());
}

void loop() {
    uint32_t now = millis();
    play(now);

    if (!musicPlaying) {
        delay(3000);
        Serial.println("New music");
        initMusic(millis());
    }
}

void play(uint32_t now) {
    uint16_t noteDuration = 0;

    if (musicPlaying) {
        // to calculate the note duration, take one second
        // divided by the note type.
        //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
        noteDuration = BEAT_PERIOD / noteDurations[currNote];

        if (!startedTone) {
            tone(PIN, melody[currNote],noteDuration); // not blocking
            startedTone = true;
            startNoteTic = now;
        } else if (now-startNoteTic > noteDuration*(1+PAUSE_INTER_NOTES_RATIO)) {
            currNote++;
            startNoteTic = now;
            startedTone = false;
            if (currNote == MELODY_LENGTH) {
                musicPlaying = false;
            }
        }
    }
}


void initMusic(uint32_t now) {
    musicPlaying = true;
    currNote = 0;
    startNoteTic = now;
    startedTone = false;
}
