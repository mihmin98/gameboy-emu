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

    uint16_t lfsr;

    uint8_t soundLengthData;
    uint64_t remainingSoundLengthCycles;

    uint8_t divisor[8] = {8, 16, 32, 48, 64, 80, 96, 112};

    // envelope
    uint8_t defaultEnvelopeValue;
    uint8_t envelopeDirection;
    uint8_t envelopeStepLength;
    uint8_t remainingEnvelopeCycles;

    uint64_t currentCycles;
    uint64_t cyclesUntilNextStep;

    void initCh();
    void cycleLfsr(uint8_t numCycles);
    void cycleLength();
    void cycleEnvelope();
    uint8_t getVolume();

    void updateSoundLengthCycles(uint8_t soundLength);
    uint64_t calcStepCycles(uint8_t shiftClockFrequency, uint8_t dividingRatio);
};

#endif // __CHANNEL_4_H__
