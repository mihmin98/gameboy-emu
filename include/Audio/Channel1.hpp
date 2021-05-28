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

    uint8_t soundLengthData;
    uint64_t remainingSoundLengthCycles;
    uint8_t currentDutyStep;

    // Sweep
    uint8_t sweepShiftNumber;
    uint8_t sweepDirection;
    uint8_t sweepTime;
    uint8_t remainingSweepCycles;

    bool sweepOverflow = false;

    uint16_t currentSweepFrequency;

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
    void cycleSweep();
    uint8_t getVolume();

    uint16_t calcNewSweepFreq(bool &overflow);

    void updateSoundLengthCycles(uint8_t soundLength);
};

#endif // __CHANNEL_1_H__
