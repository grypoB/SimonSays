#include "Color.h"


// Static varaible declaration
int16_t Color::_min_h = 0;
int16_t Color::_max_h = COLOR_MAX_H;
int16_t Color::_min_s = 0;
int16_t Color::_max_s = COLOR_MAX_S;
int16_t Color::_min_v = 0;
int16_t Color::_max_v = COLOR_MAX_V;
RGBConverter Color::_converter;


// Constructor
Color::Color() : _r(0), _g(0), _b(0) {}
Color::Color(uint8_t r, uint8_t g, uint8_t b) : _r(r), _g(g), _b(b)  {}


void Color::set(uint8_t r, uint8_t g, uint8_t b) {
    _r = r;
    _g = g;
    _b = b;
}


void Color::mix(Color color1, Color color2, double opacity) {
    _r = color1._r * opacity + color2._r * (1-opacity); 
    _g = color1._g * opacity + color2._g * (1-opacity); 
    _b = color1._b * opacity + color2._b * (1-opacity); 
}


void Color::fade(Color color1, Color color2,
                 uint32_t at, uint32_t from, uint32_t to) {
    _r = map(at, from, to, color1._r, color2._r);
    _g = map(at, from, to, color1._g, color2._g);
    _b = map(at, from, to, color1._b, color2._b);
}


void Color::setHSV(int16_t min_h, int16_t max_h,
                   int16_t min_s, int16_t max_s,
                   int16_t min_v, int16_t max_v) {
    _min_h = min_h;
    _max_h = max_h;
    _min_s = min_s;
    _max_s = max_s;
    _min_v = min_v;
    _max_v = max_v;
}


void Color::rand(uint8_t max) {
    // create a random color (in function of the max)
    _r = random(0,max+1);
    _g = random(0,max+1);
    _b = random(0,max+1);
}


void Color::randHSV() {
    uint8_t rgb[3];

    _converter.hsvToRgb((double) random(_min_h, _max_h+1)/COLOR_MAX_H,
                        (double) random(_min_s, _max_s+1)/COLOR_MAX_S,
                        (double) random(_min_v, _max_v+1)/COLOR_MAX_V,
                        rgb);

    _r = rgb[0];
    _g = rgb[1];
    _b = rgb[2];
}


uint8_t Color::getR() {
    return _r;
}

uint8_t Color::getG() {
    return _g;
}

uint8_t Color::getB() {
    return _b;
}
