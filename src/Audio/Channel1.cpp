#include "Channel1.hpp"
#include "Audio.hpp"
#include <iostream>

Channel1::Channel1()
{
    internalVolume = 0;

    currentCycles = 0;
}

void Channel1::initCh()
{
    currentDutyStep = 0;

    soundLengthData = audio->getChannel1SoundLengthData();
    updateSoundLengthCycles(soundLengthData);

    cyclesUntilNextStep = 4 * (2048 - audio->getChannel1Frequency());
    currentCycles = 0;

    // Sweep parameters
    currentSweepFrequency = audio->getChannel1Frequency();
    sweepShiftNumber = audio->getChannel1SweepShift();
    sweepDirection = audio->getChannel1SweepIncreaseDecrease();
    sweepTime = audio->getChannel1SweepTime();

    remainingSweepCycles = sweepTime == 0 ? 8 : sweepTime;
    sweepOverflow = false;

    // Envelope parameters (these params remain the same until reinit)
    defaultEnvelopeValue = audio->getChannel1InitialVolumeEnvelope();
    internalVolume = defaultEnvelopeValue;

    envelopeDirection = audio->getChannel1EnvelopeDirection();
    envelopeStepLength = audio->getChannel1EnvelopeSweep();

    if (envelopeStepLength != 0) {
        remainingEnvelopeCycles = envelopeStepLength;
    }

    audio->setChannel1SoundOn(1);
}

void Channel1::cycleDuty(uint8_t numCycles)
{
    currentCycles += numCycles;
    if (currentCycles >= cyclesUntilNextStep) {
        currentCycles -= cyclesUntilNextStep;

        cyclesUntilNextStep = 4 * (2048 - currentSweepFrequency);
        currentDutyStep = (currentDutyStep + 1) % 8;
    }
}

void Channel1::cycleLength()
{
    if (remainingSoundLengthCycles > 0) {
        --remainingSoundLengthCycles;

        if (remainingSoundLengthCycles == 0 && audio->getChannel1CounterSelection() == 1) {
            audio->setChannel1SoundOn(0);
        }
    }
}

void Channel1::cycleEnvelope()
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

void Channel1::cycleSweep()
{
    if (remainingSweepCycles > 0)
        --remainingSweepCycles;

    if (remainingSweepCycles == 0) {
        remainingSweepCycles = sweepTime == 0 ? 8 : sweepTime;

        if (!sweepOverflow && sweepTime > 0) {
            uint16_t newSweepFreq = calcNewSweepFreq(sweepOverflow);

            if (!sweepOverflow && sweepShiftNumber != 0) {
                currentSweepFrequency = newSweepFreq;

                calcNewSweepFreq(sweepOverflow);
            }

            if (sweepOverflow) {
                audio->setChannel1SoundOn(0);
            }
        }
    }
}

uint16_t Channel1::calcNewSweepFreq(bool &overflow)
{
    uint8_t sign = sweepDirection == 0 ? 1 : -1;
    uint16_t newSweepFreq =
        currentSweepFrequency + sign * (currentSweepFrequency >> sweepShiftNumber);

    if ((newSweepFreq & 0x7FF) != newSweepFreq) {
        overflow = true;
    }

    return newSweepFreq;
}

uint8_t Channel1::getVolume()
{
    if (audio->getChannel1SoundOn() == 0) {
        return 0;
    } else {
        return internalVolume *
               audio->dutyPatterns[audio->getChannel1WavePatternDuty()][currentDutyStep];
    }
}

void Channel1::updateSoundLengthCycles(uint8_t soundLength)
{
    remainingSoundLengthCycles = 64 - soundLength;
}
