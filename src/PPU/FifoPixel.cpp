#include "FifoPixel.hpp"

FifoPixel::FifoPixel() {}

FifoPixel::FifoPixel(uint8_t color, uint8_t palette, uint8_t spritePriority, uint8_t bgPriority,
                     bool isSprite)
    : color(color), palette(palette), spritePriority(spritePriority), bgPriority(bgPriority),
      isSprite(isSprite)
{
}
