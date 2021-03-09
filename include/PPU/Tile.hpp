#ifndef __TILE_H__
#define __TILE_H__

#pragma once
#include <cstdint>

class Tile
{
  public:
    uint8_t tileData[64];

    Tile();
    Tile(uint8_t *tileAddr);

    void getTileRow(uint8_t row, uint8_t *dest);
};

#endif // __TILE_H__
