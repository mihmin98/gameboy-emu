#ifndef __OAM_SPRITE_H__
#define __OAM_SPRITE_H__

#pragma once
#include <cstdint>

class OAMSprite
{
  public:
    uint8_t yPos;
    uint8_t xPos;
    uint8_t tileNumber;

    // Flags
    bool objToBgPriority;
    bool yFlip;
    bool xFlip;
    uint8_t dmgPaletteNumber;
    uint8_t tileVramBank;
    uint8_t cgbPaletteNumber;

    OAMSprite();
    OAMSprite(uint8_t *spriteAddr);

    void writeAsBytes(uint8_t *dest);
};

#endif // __OAM_SPRITE_H__
