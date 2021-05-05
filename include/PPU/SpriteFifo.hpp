#ifndef __SPRITE_FIFO_H__
#define __SPRITE_FIFO_H__

#pragma once
#include "FifoPixel.hpp"
#include "BgFifo.hpp"
#include "OAMSprite.hpp"
#include <queue>
#include <set>
#include <vector>

class PPU;

class SpriteFifo
{
  public:
    PPU *ppu;

    bool abortFetch;
    bool fetchingSprite;
    bool fetchedSprite;
    bool appliedPenaltyAtXPos0;
    bool checkForAbort;
    uint8_t fetcherStep;
    uint8_t fetcherStage;
    
    uint8_t currentSpriteIndex;
    uint8_t spriteIndexInFoundSprites;

    uint8_t oamPenalty;

    // this should only hold 8 pixels?
    std::queue<FifoPixel> pixelQueue;

    std::set<uint8_t> processedSprites;

    uint16_t fetcherXPos;
    uint16_t fetcherYPos; // nu cred ca am nevoie de el

    SpriteFifo();
    SpriteFifo(PPU *ppu);

    FifoPixel *cycle(BgFifo &bgFifo);
    // do i need the reference? or just the number of elements in the bgfifo queue?
    // but why do i need it???

    void prepareForLine();
};

#endif // __SPRITE_FIFO_H__
