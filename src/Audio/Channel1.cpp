#include "Channel1.hpp"
#include "Audio.hpp"
#include <iostream>

Channel1::Channel1()
{
    internalVolume = 0;
    outputVolume = 0;

    currentCycles = 0;

    sweepTimeCycles[0] = 0;
    for (uint8_t i = 1; i < 8; ++i) {
        sweepTimeCycles[i] = (4194304 * i) / 128;
    }
}

void Channel1::initCh()
{
    currentDutyStep = 0;

    soundLengthData = audio->getChannel1SoundLengthData();
    updateSoundLengthCycles(soundLengthData);

    cyclesUntilNextStep = 8 * (2048 - audio->getChannel1Frequency());
    currentCycles = 0;

    // currentSweepFrequency = 4194304 / (8 * (2048 - audio->getChannel1Frequency()));
    currentSweepFrequency = audio->getChannel1Frequency();
    currentSweepCycles = 0;
    sweepShiftNumber = audio->getChannel1SweepShift();
    sweepDirection = audio->getChannel1SweepIncreaseDecrease();
    sweepTime = audio->getChannel1SweepTime();
    sweepOverflow = false;

    // envelope stuff (this stuff remains the same, until reinit)
    defaultEnvelopeValue = audio->getChannel1InitialVolumeEnvelope();
    internalVolume = defaultEnvelopeValue;

    envelopeDirection = audio->getChannel1EnvelopeDirection();
    envelopeStepLength = audio->getChannel1EnvelopeSweep();

    if (envelopeStepLength != 0) {
        cyclesUntilNextEnvelopeStep = (4194304 * envelopeStepLength) / 64;
        currentEnvelopeCycles = 0;
    }
}

void Channel1::cycle(uint8_t numCycles)
{
    // cycle duty step
    currentCycles += numCycles;
    if (currentCycles >= cyclesUntilNextStep) {
        currentCycles -= cyclesUntilNextStep;

        cyclesUntilNextStep = 8 * (2048 - currentSweepFrequency);
        // cyclesUntilNextStep = 4194304 / currentSweepFrequency;
        currentDutyStep = (currentDutyStep + 1) % 8;
    }

    // cycle sweep
    sweepShiftNumber = audio->getChannel1SweepShift();
    sweepDirection = audio->getChannel1SweepIncreaseDecrease();
    sweepTime = audio->getChannel1SweepTime();

    if (sweepTime != 0) {
        ++currentSweepCycles;
        if (currentSweepCycles == sweepTimeCycles[sweepTime]) {
            currentSweepCycles = 0;

            uint8_t sign = sweepDirection == 0 ? 1 : -1;
            uint16_t newSweepFrequency =
                currentSweepFrequency + sign * (currentSweepFrequency >> sweepShiftNumber);

            if ((newSweepFrequency & 0x7FF) != newSweepFrequency) {
                sweepOverflow = true;
            } else {
                currentSweepFrequency = newSweepFrequency;
            }
        }
    }

    // cycle envelope
    if (envelopeStepLength != 0) {
        currentEnvelopeCycles += numCycles;
        if (currentCycles >= cyclesUntilNextEnvelopeStep) {
            currentEnvelopeCycles -= cyclesUntilNextEnvelopeStep;

            if (internalVolume > 0 && internalVolume < 15) {
                if (envelopeDirection == 0)
                    --internalVolume;
                else
                    ++internalVolume;
            }
        }
    }

    // set channel ON flag
    // update remiainig sound length cycles
    for (uint i = 0; i < numCycles && remainingSoundLengthCycles > 0; ++i) {
        --remainingSoundLengthCycles;
    }

    if ((remainingSoundLengthCycles == 0 && audio->getChannel1CounterSelection() == 1) ||
        sweepOverflow) {
        audio->setChannel1SoundOn(0);
    } else {
        audio->setChannel1SoundOn(1);
    }

    if (audio->getChannel1SoundOn() == 0) {
        outputVolume = 0;
    }

    else {
        outputVolume = internalVolume *
                       audio->dutyPatterns[audio->getChannel1WavePatternDuty()][currentDutyStep];
    }
}

void Channel1::updateSoundLengthCycles(uint8_t soundLength)
{
    remainingSoundLengthCycles = (4194304 * (64 - soundLength)) / 256;
}
