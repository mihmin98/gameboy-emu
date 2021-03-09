#include "OAMSprite.hpp"

OAMSprite::OAMSprite() {}

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

    uint8_t flagByte = (objToBgPriority << 7) | (yFlip << 6) | (xFlip << 5) |
                       (dmgPaletteNumber << 4) | (tileVramBank << 3) | (cgbPaletteNumber);
    dest[3] = flagByte;
}
