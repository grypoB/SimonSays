/*
  Melody
 */

#include "Music.h"
#define PIN 10

Music music = Music(PIN);
int counter = 0;

void setup() {
    Serial.begin(9600);
    pinMode(PIN, OUTPUT);
    delay(1000);

    Serial.println("star");
}

void loop() {
    uint32_t now = millis();

    music.updatePlay(now);

    if (!music.isPlaying()) {
        delay(1000);
        Serial.print("New music");
        Serial.println(counter++);
        if (counter%3 == 0) {
            music.playSong(SONG_LOST_WOODS, now);
        } else if (counter%3 == 1) {
            //music.playSong(SONG_1UP, now);
        } else if (counter%3 == 2) {
            //music.playSong(SONG_COIN, now);
        }
    }
}
