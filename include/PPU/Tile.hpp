#ifndef __TILE_H__
#define __TILE_H__

#pragma once
#include <algorithm>
#include <cstdint>

class Tile
{
  public:
    uint8_t tileData[64];

    Tile();
    Tile(uint8_t *tileAddr);

    // Returns the tile row colors in the provided dest array; The array must contain 8 uint8_t
    // elements
    void getTileRow(uint8_t row, uint8_t *dest);
    
    // Flips the tile horizontally
    void flipHor();

    // Flips the tile vertically
    void flipVert();
};

#endif // __TILE_H__
