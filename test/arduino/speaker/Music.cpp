#include "Music.h"
#include "pitches.h"


namespace {
    namespace OneUp { // Mario 1up, 6 notes, 500 ms
        int melody[] = {
            E4,G4,E5,C5,
            D5,G5
        };

        int noteDurations[] = {
            4,4,4,4,
            4,4
        };
        int melodyLength = 6;
        int beatPeriod = 500;
    }

    namespace Coin { // Mario coin, 2 notes, 500 ms
        int melody[] = {
            B4,E5
        };

        int noteDurations[] = {
            8,2
        };
        int melodyLength = 2;
        int beatPeriod = 500;
    }
    
    namespace GameOver { // Mario game over, 19 notes, 500 ms
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

        int melodyLength = 19;
        int beatPeriod = 500;
    }

    namespace LostWoods { // Lost woods, first 16 measures, 42 notes
        int melody[] = {
            F5,A6,B6,F5,A6,B6,
            F5,A6,B6,E6,D6,C6,D6,
            B6,G5,E5,0,D5,
            E5,G5,D5,0,

            F5,A6,B6,F5,A6,B6,
            F5,A6,B6,E2,D6,C6,D6,
            B6,G5,E6,0,B6,
            G5,D5,E6,0
        };

        int noteDurations[] = {
            8,8,4,8,8,4,
            8,8,8,8,4,8,8,
            8,8,2,8,8,
            8,8,2,4,

            8,8,4,8,8,4,
            8,8,8,8,4,8,8,
            8,8,2,8,8,
            8,8,2,4
        };

        int melodyLength = 44;
        int beatPeriod = 1100;
    }

    Song song1Up      = Song(OneUp::melody, OneUp::noteDurations,
                             OneUp::melodyLength, OneUp::beatPeriod);
    Song songCoin     = Song(Coin::melody, Coin::noteDurations,
                             Coin::melodyLength, Coin::beatPeriod);
    Song songGameOver = Song(GameOver::melody, GameOver::noteDurations,
                             GameOver::melodyLength, GameOver::beatPeriod);
    Song songLostWoods = Song(LostWoods::melody, LostWoods::noteDurations,
                             LostWoods::melodyLength, LostWoods::beatPeriod);
}


Song::Song() {}

Song::Song(int *pMelody, int* pNoteDurations, int pMelodyLength, int pBeatPeriod)
    : melody(pMelody), noteDurations(pNoteDurations),
      melodyLength(pMelodyLength), beatPeriod(pBeatPeriod) {};


Music::Music(int pin) : pin(pin) {
    playing = false;
}


void Music::updatePlay(uint32_t now) {
    uint16_t noteDuration = 0;

    if (playing) {
        // to calculate the note duration, take one second
        // divided by the note type.
        //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
        noteDuration = song.beatPeriod / song.noteDurations[currNote];

        if (!startedTone) {
            tone(pin, song.melody[currNote], noteDuration); // not blocking
            startedTone = true;
            startNoteTic = now;
        } else if (now-startNoteTic > noteDuration*(1+PAUSE_INTER_NOTES_RATIO)) {
            currNote++;
            startNoteTic = now;
            startedTone = false;
            if (currNote >= song.melodyLength) {
                playing = false;
            }
        }
    }
}


void Music::playSong(SongName name, uint32_t now) {
    playing = true;
    currNote = 0;
    startNoteTic = now;
    startedTone = false;

    switch (name) {
        case SONG_1UP:
            song = song1Up;
        break;
        case SONG_COIN:
            song = songCoin;
        break;
        case SONG_GAME_OVER:
            song = songGameOver;
        break;
        case SONG_LOST_WOODS:
            song = songLostWoods;
        break;
    }
}

bool Music::isPlaying() {
    return playing;
}

