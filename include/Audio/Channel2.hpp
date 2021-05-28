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

    uint8_t soundLengthData;
    uint64_t remainingSoundLengthCycles;
    uint8_t currentDutyStep;

    // Envelope
    uint8_t defaultEnvelopeValue;
    uint8_t envelopeDirection;
    uint8_t envelopeStepLength;
    uint8_t remainingEnvelopeCycles;

    uint64_t currentCycles;
    uint64_t cyclesUntilNextStep;

    void initCh();  
    void cycleDuty(uint8_t numCycles);
    void cycleLength();
    void cycleEnvelope();
    uint8_t getVolume();

    void updateSoundLengthCycles(uint8_t soundLength);
};

#endif // __CHANNEL_2_H__
