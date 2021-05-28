#include "Channel4.hpp"
#include "Audio.hpp"
#include <iostream>

Channel4::Channel4()
{
    internalVolume = 0;

    currentCycles = 0;

    // Start lfsr with all bits set
    lfsr = 0x7FFF;
}

void Channel4::initCh()
{
    soundLengthData = audio->getChannel4SoundLength();
    updateSoundLengthCycles(soundLengthData);

    cyclesUntilNextStep =
        calcStepCycles(audio->getChannel4ShiftClockFrequency(), audio->getChannel4DividingRatio());
    currentCycles = 0;

    defaultEnvelopeValue = audio->getChannel4InitialVolumeEnvelope();
    internalVolume = defaultEnvelopeValue;

    envelopeDirection = audio->getChannel4EnvelopeDirection();
    envelopeStepLength = audio->getChannel4EnvelopeSweep();

    if (envelopeStepLength != 0) {
        remainingEnvelopeCycles = envelopeStepLength;
    }

    lfsr = 0x7FFF;
    audio->setChannel4SoundOn(1);
}

void Channel4::cycleLfsr(uint8_t numCycles)
{
    currentCycles += numCycles;
    if (currentCycles >= cyclesUntilNextStep) {
        currentCycles -= cyclesUntilNextStep;

        cyclesUntilNextStep = calcStepCycles(audio->getChannel4ShiftClockFrequency(),
                                             audio->getChannel4DividingRatio());

        uint8_t xorResult = (lfsr & 1) ^ ((lfsr & 2) >> 1);

        lfsr = (lfsr & 0x3FFF) | (xorResult << 14);

        lfsr = lfsr >> 1;

        if (audio->getChannel4CounterStep() == 1) {
            lfsr = (lfsr & 0x7FBF) | (xorResult << 6);
        }
    }
}

void Channel4::cycleLength()
{
    if (remainingSoundLengthCycles > 0) {
        --remainingSoundLengthCycles;

        if (remainingSoundLengthCycles == 0 && audio->getChannel4CounterSelection() == 1) {
            audio->setChannel4SoundOn(0);
        }
    }
}

void Channel4::cycleEnvelope()
{
    if (envelopeStepLength != 0) {
        --remainingEnvelopeCycles;

        if (remainingEnvelopeCycles == 0) {
            remainingEnvelopeCycles = envelopeStepLength;

            if (internalVolume > 0 && internalVolume < 15) {
                if (envelopeDirection == 0)
                    --internalVolume;
                else
                    ++internalVolume;
            }
        }
    }
}

void Channel4::updateSoundLengthCycles(uint8_t soundLength)
{
    remainingSoundLengthCycles = 64 - soundLength;
}

uint8_t Channel4::getVolume()
{
    if (audio->getChannel4SoundOn() == 0) {
        return 0;
    }

    else {
        uint8_t aux = ((lfsr & 1) == 0) ? 1 : 0;
        return internalVolume * aux;
    }
}

uint64_t Channel4::calcStepCycles(uint8_t shiftClockFrequency, uint8_t dividingRatio)
{
    return divisor[dividingRatio & 7] << shiftClockFrequency;
}
