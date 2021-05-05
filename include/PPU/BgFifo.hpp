#ifndef __BG_FIFO_H__
#define __BG_FIFO_H__

#pragma once
#include "BgMapAttributes.hpp"
#include "FifoPixel.hpp"
#include "Tile.hpp"
#include <queue>

class PPU;

enum PixelFetcherStage { GET_TILE, GET_TILE_DATA_LOW, GET_TILE_DATA_HIGH, SLEEP, PUSH };

class BgFifo
{
  public:
    PPU *ppu;

    bool isDrawingWindow;
    std::queue<FifoPixel> pixelQueue;
    uint8_t scxPixelsToDiscard; // at the beginning of the line, SCX % 8 pixels must be discarded
    uint16_t pushedPixels;

    PixelFetcherStage fetcherStage;
    int fetcherStageCycles;

    uint8_t fetcherXPos; // fetcher x pos (0..159)
    uint8_t fetcherYPos; // current scanline (0..143)

    uint8_t tileXPos; // tile x pos in tilemap
    uint8_t tileYPos; // tile y pos in tilemap

    uint16_t tilemapBaseAddr;
    Tile tile;

    BgFifo();
    BgFifo(PPU *ppu);

    // Returns the pushed pixel, otherwise nullptr. The returned pixel is allocated dynamically and
    // thus should be deleted after being used
    FifoPixel *cycle();

    // Cycles the fetcher
    void cycleFetcher();

    // Empties the pixelQueue
    void clearQueue();

    // Checks if the given coordinates are inside the window
    bool coordsInsideWindow(uint16_t xPos, uint16_t yPos);
    
    // Resets the fetcher
    void resetFetcher(bool resetYPos = false);

    // Prepares the Fifo and fetcher for a new line
    void prepareForLine(uint8_t line = 0);
};

#endif // __BG_FIFO_H__
