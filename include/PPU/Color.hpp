#ifndef __COLOR_H__
#define __COLOR_H__

#pragma once
#include <cstdint>

class Color
{
  public:
    uint8_t red;
    uint8_t green;
    uint8_t blue;

    Color();
    Color(uint8_t red, uint8_t green, uint8_t blue);
    Color(uint8_t *colorAddr);

    Color getNormalizedColor();

    // When writing color, the intensities should not be normalized
    void writeColorAsBytes(uint8_t *dest);

    // Creates a Color from a value that is between 0-3
    static Color getDmgColor(uint8_t colorValue);

    friend bool operator== (const Color &color1, const Color &color2);
    friend bool operator!= (const Color &color1, const Color &color2);
};

#endif // __COLOR_H__
