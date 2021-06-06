#include "Channel2.hpp"
#include "Audio.hpp"
#include <iostream>

Channel2::Channel2()
{
    internalVolume = 0;

    currentCycles = 0;
}

void Channel2::initCh()
{
    currentDutyStep = 0;

    soundLengthData = audio->getChannel2SoundLengthData();
    updateSoundLengthCycles(soundLengthData);

    cyclesUntilNextStep = 4 * (2048 - audio->getChannel2Frequency());
    currentCycles = 0;

    // Envelope parameters (these params remain the same until reinit)
    defaultEnvelopeValue = audio->getChannel2InitialVolumeEnvelope();
    internalVolume = defaultEnvelopeValue;

    envelopeDirection = audio->getChannel2EnvelopeDirection();
    envelopeStepLength = audio->getChannel2EnvelopeSweep();

    if (envelopeStepLength != 0) {
        remainingEnvelopeCycles = envelopeStepLength;
    }

    audio->setChannel2SoundOn(1);
}

void Channel2::cycleDuty(uint8_t numCycles)
{
    currentCycles += numCycles;
    if (currentCycles >= cyclesUntilNextStep) {
        currentCycles -= cyclesUntilNextStep;

        cyclesUntilNextStep = 4 * (2048 - audio->getChannel2Frequency());
        currentDutyStep = (currentDutyStep + 1) % 8;
    }
}

void Channel2::cycleLength()
{
    if (remainingSoundLengthCycles > 0) {
        --remainingSoundLengthCycles;

        if (remainingSoundLengthCycles == 0 && audio->getChannel2CounterSelection() == 1) {
            audio->setChannel2SoundOn(0);
        }
    }
}

void Channel2::cycleEnvelope()
{
    if (envelopeStepLength != 0) {
        --remainingEnvelopeCycles;

        if (remainingEnvelopeCycles == 0) {
            remainingEnvelopeCycles = envelopeStepLength;

            if (envelopeDirection == 0 && internalVolume > 0)
                --internalVolume;
            else if (envelopeDirection == 1 && internalVolume < 15)
                ++internalVolume;
        }
    }
}

uint8_t Channel2::getVolume()
{
    if (audio->getChannel2SoundOn() == 0) {
        return 0;
    } else {
        return internalVolume *
               audio->dutyPatterns[audio->getChannel2WavePatternDuty()][currentDutyStep];
    }
}

void Channel2::updateSoundLengthCycles(uint8_t soundLength)
{
    remainingSoundLengthCycles = 64 - soundLength;
}
