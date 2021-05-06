#include "Color.hpp"

Color::Color() {}

Color::Color(uint8_t red, uint8_t green, uint8_t blue) : red(red), green(green), blue(blue) {}

Color::Color(uint8_t *colorAddr)
{
    red = (colorAddr[0] & 0xF8) >> 3;
    green = ((colorAddr[0] & 0x07) << 2) | ((colorAddr[1] & 0xC0) >> 6);
    blue = (colorAddr[1] & 0x3E) >> 1;
}

Color Color::getNormalizedColor()
{
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

Color Color::getDmgColor(uint8_t colorValue)
{
    // 0 is white
    // 3 is black

    // 0 = 255
    // 1 = 170
    // 2 = 85
    // 3 = 0

    if (colorValue > 3) {
        return Color(0, 0, 0);
    }

    uint8_t val = 255 - (colorValue * (255 / 3));
    return Color(val, val, val);
}
