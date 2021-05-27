#ifndef __CHANNEL_4_H__
#define __CHANNEL_4_H__

#pragma once
#include <cstdint>

class Audio;

class Channel4
{
  public:
    Channel4();
    
    Audio *audio;

    uint8_t internalVolume;
    uint8_t outputVolume;

    uint16_t lfsr;

    uint8_t soundLengthData;
    uint64_t remainingSoundLengthCycles;

    uint64_t shiftClockFrequencies[16];

    // envelope
    uint8_t defaultEnvelopeValue;
    uint8_t envelopeDirection;
    uint8_t envelopeStepLength;
    
    uint64_t currentCycles;
    uint64_t cyclesUntilNextStep;

    uint64_t currentEnvelopeCycles;
    uint64_t cyclesUntilNextEnvelopeStep;

    void initCh();  
    void cycle();

    void updateSoundLengthCycles(uint8_t soundLength);
    uint64_t calcStepCycles(uint8_t shiftClockFrequency, uint8_t dividingRatio);
};

#endif // __CHANNEL_4_H__
