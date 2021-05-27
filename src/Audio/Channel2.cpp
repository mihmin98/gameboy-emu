#include "Channel2.hpp"
#include "Audio.hpp"
#include <iostream>

Channel2::Channel2()
{
    internalVolume = 0;
    outputVolume = 0;

    currentCycles = 0;
}

void Channel2::initCh()
{
    currentDutyStep = 0;

    soundLengthData = audio->getChannel2SoundLengthData();
    updateSoundLengthCycles(soundLengthData);

    cyclesUntilNextStep = 8 * (2048 - audio->getChannel2Frequency());
    currentCycles = 0;

    // envelope stuff (this stuff remains the same, until reinit)
    defaultEnvelopeValue = audio->getChannel2InitialVolumeEnvelope();
    internalVolume = defaultEnvelopeValue;

    envelopeDirection = audio->getChannel2EnvelopeDirection();
    envelopeStepLength = audio->getChannel2EnvelopeSweep();

    if (envelopeStepLength != 0) {
        cyclesUntilNextEnvelopeStep = (4194304 * envelopeStepLength) / 64;
        currentEnvelopeCycles = 0;
    }
}

void Channel2::cycle()
{
    // cycle dutystep
    ++currentCycles;
    if (currentCycles == cyclesUntilNextStep) {
        currentCycles = 0;

        cyclesUntilNextStep = 8 * (2048 - audio->getChannel2Frequency());
        currentDutyStep = (currentDutyStep + 1) % 8;
    }

    // cycle envelope
    if (envelopeStepLength != 0) {
        ++currentEnvelopeCycles;
        if (currentCycles == cyclesUntilNextEnvelopeStep) {
            currentEnvelopeCycles = 0;

            if (internalVolume > 0 && internalVolume < 15) {
                if (envelopeDirection == 0)
                    --internalVolume;
                else
                    ++internalVolume;
            }
        }
    }

    // update remiainig sound length cycles
    if (remainingSoundLengthCycles > 0)
        --remainingSoundLengthCycles;

    if (remainingSoundLengthCycles == 0 && audio->getChannel2CounterSelection() == 1) {
        audio->setChannel2SoundOn(0);
    } else {
        audio->setChannel2SoundOn(1);
    }

    // output
    if (audio->getChannel2SoundOn() == 0) {
        outputVolume = 0;
    }

    else {
        outputVolume = internalVolume *
                       audio->dutyPatterns[audio->getChannel2WavePatternDuty()][currentDutyStep];
    }
}

void Channel2::updateSoundLengthCycles(uint8_t soundLength)
{
    remainingSoundLengthCycles = (4194304 * (64 - soundLength)) / 256;
}
