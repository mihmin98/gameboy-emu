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
    Color(uint8_t *colorAddr);

    Color getNormalizedColor();

    // When writing color, the intensities should not be normalized
    void writeColorAsBytes(uint8_t *dest);
};

#endif // __COLOR_H__
