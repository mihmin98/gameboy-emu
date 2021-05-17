#include "FifoPixel.hpp"

FifoPixel::FifoPixel() : color(0), palette(0), spritePriority(255), bgPriority(0), isSprite(false) {}

FifoPixel::FifoPixel(uint8_t color, uint8_t palette, uint8_t spritePriority, uint8_t bgPriority,
                     bool isSprite)
    : color(color), palette(palette), spritePriority(spritePriority), bgPriority(bgPriority),
      isSprite(isSprite)
{
}
