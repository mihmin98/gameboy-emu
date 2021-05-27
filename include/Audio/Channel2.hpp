#ifndef __CHANNEL_2_H__
#define __CHANNEL_2_H__

#pragma once
#include <cstdint>

class Audio;

class Channel2
{
  public:
    Channel2();
    
    Audio *audio;

    uint8_t internalVolume;
    uint8_t outputVolume;

    uint8_t soundLengthData;
    uint64_t remainingSoundLengthCycles;
    uint8_t currentDutyStep;

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
};

#endif // __CHANNEL_2_H__
