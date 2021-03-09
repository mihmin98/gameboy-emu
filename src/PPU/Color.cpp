#include "Color.hpp"

Color::Color() {}

Color::Color(uint8_t *colorAddr) {
    red = (colorAddr[0] & 0xF8) >> 3;
    green = ((colorAddr[0] & 0x07) << 3) | ((colorAddr[1] & 0xC0) >> 6);
    blue = (colorAddr[1] & 0x3E) >> 1;
}

Color Color::getNormalizedColor() {
    Color normalizedColor;
    
    // Color intensity in gameboy memory is in [0, 32)
    normalizedColor.red = (red / 31.0) * 255;
    normalizedColor.green = (green / 31.0) * 255;
    normalizedColor.blue = (blue / 31.0) * 255;

    return normalizedColor;
}

void Color::writeColorAsBytes(uint8_t *dest)
{
    dest[0] = ((red & 0x1F) << 3) | ((green & 0x1C) >> 2);
    dest[1] = ((green * 0x03) << 6) | ((blue & 0x1F) << 1);
}
