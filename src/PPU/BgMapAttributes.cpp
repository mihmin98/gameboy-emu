#include "BgMapAttributes.hpp"

BgMapAttributes::BgMapAttributes() {}

BgMapAttributes::BgMapAttributes(uint8_t bgToOamPriority, uint8_t verticalFlip,
                                 uint8_t horizontalFlip, uint8_t tileVramBankNumber,
                                 uint8_t bgPaletteNumber)
    : bgToOamPriority(bgToOamPriority), verticalFlip(verticalFlip), horizontalFlip(horizontalFlip),
      tileVramBankNumber(tileVramBankNumber), bgPaletteNumber(bgPaletteNumber)
{
}

BgMapAttributes::BgMapAttributes(uint8_t byte)
{
    bgToOamPriority = (byte & 0x80) >> 7;
    verticalFlip = (byte & 0x40) >> 6;
    horizontalFlip = (byte & 0x20) >> 5;
    tileVramBankNumber = (byte & 0x8) >> 3;
    bgPaletteNumber = byte & 0x7;
}

uint8_t BgMapAttributes::getAsByte()
{
    uint8_t byte = (bgToOamPriority << 7) | (verticalFlip << 6) | (horizontalFlip << 5) |
                   (tileVramBankNumber << 3) | bgPaletteNumber;

    return byte;
}
