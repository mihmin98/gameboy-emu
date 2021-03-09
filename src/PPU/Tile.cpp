#include "Tile.hpp"

Tile::Tile() {}

Tile::Tile(uint8_t *tileAddr)
{
    int currPixel = 0;
    for (int i = 0; i < 16; i += 2) {
        for (int j = 0; j < 8; ++j) {
            uint8_t lowBit = (tileAddr[i] & (0x1 << (7 - j))) >> (7 - j);
            uint8_t highBit = (tileAddr[i + 1] & (0x1 << (7 - j))) >> (7 - j);

            tileData[currPixel++] = (highBit << 1) | lowBit;
        }
    }
}

void Tile::getTileRow(uint8_t row, uint8_t *dest)
{
    for (int i = 0; i < 8; ++i)
        dest[i] = tileData[row * 8 + i];
}
