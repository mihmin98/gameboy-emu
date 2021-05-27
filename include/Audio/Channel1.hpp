#ifndef __CHANNEL_1_H__
#define __CHANNEL_1_H__

#pragma once
#include <cstdint>

class Audio;

class Channel1
{
  public:
    Channel1();
    
    Audio *audio;

    uint8_t internalVolume;
    uint8_t outputVolume;

    uint8_t soundLengthData;
    uint64_t remainingSoundLengthCycles;
    uint8_t currentDutyStep;

    // sweep
    uint64_t sweepTimeCycles[8];
    uint64_t currentSweepCycles;

    uint8_t sweepShiftNumber;
    uint8_t sweepDirection;
    uint8_t sweepTime;

    bool sweepOverflow = false;

    uint16_t currentSweepFrequency;

    // envelope
    uint8_t defaultEnvelopeValue;
    uint8_t envelopeDirection;
    uint8_t envelopeStepLength;

    uint64_t currentCycles;
    uint64_t cyclesUntilNextStep;

    uint64_t currentEnvelopeCycles;
    uint64_t cyclesUntilNextEnvelopeStep;

    void initCh();  
    void cycle(uint8_t numCycles);

    void updateSoundLengthCycles(uint8_t soundLength);
};

#endif // __CHANNEL_1_H__
