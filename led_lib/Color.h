#ifndef __COLOR_H__
#define __COLOR_H__

#include "Arduino.h"
#include "RGBConverter.h"

#define COLOR_MAX_H 360
#define COLOR_MAX_S 100
#define COLOR_MAX_V 100

class Color {
    public :

        // Constructor
        Color();
        Color(uint8_t r, uint8_t g, uint8_t b);

        void set(uint8_t r, uint8_t g, uint8_t b);

        void mix(Color color1, Color color2, double opacity);

        /* Fade the color from color1 to color2 from t=from to t=to
         * Caculated at t=at
         */
        void fade(Color color1, Color color2,
                         uint32_t at, uint32_t from, uint32_t to);


        void setHSV(int16_t min_h, int16_t max_h,
                           int16_t min_s, int16_t max_s,
                           int16_t min_v, int16_t max_v);

        // Foncions
        /* Random color in [0, max] for RGB
        */
        void rand(uint8_t max);
        void randHSV();
    private:
        static int16_t _min_h, _max_h; // from 0 to COLOR_MAX_H
        static int16_t _min_s, _max_s; // from 0 to COLOR_MAX_S
        static int16_t _min_v, _max_v; // from 0 to COLOR_MAX_V
        static RGBConverter _converter; 

        uint8_t _r, _g, _b;
};

#endif
