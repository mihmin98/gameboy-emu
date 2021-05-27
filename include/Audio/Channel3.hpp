#ifndef __CHANNEL_3_H__
#define __CHANNEL_3_H__

#pragma once
#include <cstdint>

class Audio;

class Channel3
{
  public:
    Channel3();
    
    Audio *audio;

    uint8_t internalVolume;
    uint8_t outputVolume;

    uint8_t samplePosition;

    uint8_t soundLengthData;
    uint64_t remainingSoundLengthCycles;
    
    uint64_t currentCycles;
    uint64_t cyclesUntilNextStep;

    void initCh();  
    void cycle(uint8_t numCycles);

    void updateSoundLengthCycles(uint8_t soundLength);
};

#endif // __CHANNEL_3_H__
