#include "OAMSprite.hpp"

OAMSprite::OAMSprite()
    : yPos(0), xPos(0), tileNumber(0), objToBgPriority(false), yFlip(false), xFlip(false),
      dmgPaletteNumber(0), tileVramBank(0), cgbPaletteNumber(0)
{
}

OAMSprite::OAMSprite(uint8_t *spriteAddr)
{
    yPos = spriteAddr[0];
    xPos = spriteAddr[1];
    tileNumber = spriteAddr[2];

    objToBgPriority = spriteAddr[3] & 0x80;
    yFlip = spriteAddr[3] & 0x40;
    xFlip = spriteAddr[3] & 0x20;
    dmgPaletteNumber = (spriteAddr[3] & 0x10) >> 4;
    tileVramBank = (spriteAddr[3] & 0x8) >> 3;
    cgbPaletteNumber = spriteAddr[3] & 0x7;
}

void OAMSprite::writeAsBytes(uint8_t *dest)
{
    dest[0] = yPos;
    dest[1] = xPos;
    dest[2] = tileNumber;

    uint8_t objToBgPriorityBit = objToBgPriority ? 1 : 0;
    uint8_t yFlipBit = yFlip ? 1 : 0;
    uint8_t xFlipBit = xFlip ? 1 : 0;
    uint8_t flagByte = (objToBgPriorityBit << 7) | (yFlipBit << 6) | (xFlipBit << 5) |
                       (dmgPaletteNumber << 4) | (tileVramBank << 3) | (cgbPaletteNumber);
    dest[3] = flagByte;
}
