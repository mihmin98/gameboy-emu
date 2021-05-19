#ifndef __FIFO_PIXEL_H__
#define __FIFO_PIXEL_H__

#pragma once
#include <cstdint>

class FifoPixel
{
  public:
    uint8_t color;
    uint8_t palette;
    uint8_t spritePriority;
    uint8_t spriteBgAndWindowOverObjPriority;
    uint8_t bgPriority;
    uint8_t tileIndex;
    uint8_t tileXpos, tileYpos;

    bool isSprite;

    FifoPixel();
    FifoPixel(uint8_t color, uint8_t palette, uint8_t spritePriority,
              uint8_t spriteBgAndWindowOverObjPriority, uint8_t bgPriority, bool isSprite);
};

#endif // __FIFO_PIXEL_H__
