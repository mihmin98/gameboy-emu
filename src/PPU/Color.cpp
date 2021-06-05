#include "Color.hpp"
#include <iostream>

Color::Color() {}

Color::Color(uint8_t red, uint8_t green, uint8_t blue) : red(red), green(green), blue(blue) {}

Color::Color(uint8_t *colorAddr)
{
    uint16_t colorWord = (colorAddr[1] << 8) | colorAddr[0];
    red = colorWord & 0x1F;
    green = (colorWord & 0x3E0) >> 5;
    blue = (colorWord & 0x7C00) >> 10;
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

Color Color::getColorFromHexString(std::string colorString)
{
    // replace # with 0x
    colorString[0] = '0';
    colorString.insert(1, "x");

    std::cout << "replaced color string: " << colorString << "\n";

    ulong colorValue;
    colorValue = std::stoul(colorString, NULL, 16);
    std::cout << std::hex << colorValue << "\n";

    return Color((colorValue & 0xFF0000) >> 16, (colorValue & 0x00FF00) >> 8, colorValue & 0x0000FF); 
}

bool operator==(const Color &color1, const Color &color2)
{
    return color1.red == color2.red && color1.green == color2.green && color1.blue == color2.blue;
}

bool operator!=(const Color &color1, const Color &color2) { return !(color1 == color2); }
