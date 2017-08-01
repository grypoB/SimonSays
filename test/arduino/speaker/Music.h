#ifndef __MUSIC_H__
#define __MUSIC_H__

#include "Arduino.h"
#define PAUSE_INTER_NOTES_RATIO 0.3

enum SongName {SONG_1UP, SONG_COIN, SONG_GAME_OVER};

class Song {
    public:
        Song();
        Song(int *pMelody, int* pNoteDurations, int pMelodyLength, int pBeatPeriod);

        int *melody;
        int *noteDurations;
        int melodyLength;
        int beatPeriod;
};

class Music {
    public:
        Music(int pin);

        void updatePlay(uint32_t now);

        void playSong(SongName name, uint32_t now);

        bool isPlaying();

    private:
        int pin;

        Song song;
        bool playing;
        bool startedTone;
        int currNote;
        int startNoteTic; 
};


#endif
