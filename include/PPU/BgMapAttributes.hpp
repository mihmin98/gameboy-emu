#ifndef __BG_MAP_ATTRIBUTES_H__
#define __BG_MAP_ATTRIBUTES_H__

#pragma once
#include <cstdint>

class BgMapAttributes
{
  public:
    uint8_t bgToOamPriority;
    uint8_t verticalFlip;
    uint8_t horizontalFlip;
    uint8_t tileVramBankNumber;
    uint8_t bgPaletteNumber;

    BgMapAttributes();
    BgMapAttributes(uint8_t bgToOamPriority, uint8_t verticalFlip, uint8_t horizontalFlip,
          uint8_t tileVramBankNumber, uint8_t bgPaletteNumber);
    BgMapAttributes(uint8_t byte);
};

#endif // __BG_MAP_ATTRIBUTES_H__
