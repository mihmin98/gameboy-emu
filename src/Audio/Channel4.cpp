#include "Channel4.hpp"
#include "Audio.hpp"
#include <iostream>

Channel4::Channel4()
{
    internalVolume = 0;
    outputVolume = 0;

    currentCycles = 0;

    for (uint8_t i = 0; i < 16; ++i) {
        shiftClockFrequencies[i] = 1 << (i + 1);
    }

    // start lfsr with all bits set
    lfsr = 0x7FFF;
}

void Channel4::initCh()
{
    // std::cout << "init ch4\n";
    soundLengthData = audio->getChannel4SoundLength();
    updateSoundLengthCycles(soundLengthData);

    // TODO: cycles until next step
    cyclesUntilNextStep =
        calcStepCycles(audio->getChannel4ShiftClockFrequency(), audio->getChannel4DividingRatio());

    defaultEnvelopeValue = audio->getChannel4InitialVolumeEnvelope();
    internalVolume = defaultEnvelopeValue;

    envelopeDirection = audio->getChannel4EnvelopeDirection();
    envelopeStepLength = audio->getChannel4EnvelopeSweep();

    if (envelopeStepLength != 0) {
        cyclesUntilNextEnvelopeStep = (4194304 * envelopeStepLength) / 64;
        currentEnvelopeCycles = 0;
    }
}

void Channel4::cycle()
{
    // cycle step
    ++currentCycles;
    if (currentCycles >= cyclesUntilNextStep) {
        currentCycles = 0;
        cyclesUntilNextStep = calcStepCycles(audio->getChannel4ShiftClockFrequency(),
                                             audio->getChannel4DividingRatio());

        uint8_t xorResult = (lfsr & 1) ^ ((lfsr & 2) >> 1);

        lfsr = (lfsr & 0x3FFF) | (xorResult << 14);

        lfsr = lfsr >> 1;

        if (audio->getChannel4CounterStep() == 1) {
            lfsr = (lfsr & 0x7FBF) | (xorResult << 6);
        }
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

    if (remainingSoundLengthCycles == 0 && audio->getChannel4CounterSelection() == 1) {
        audio->setChannel4SoundOn(0);
    } else {
        audio->setChannel4SoundOn(1);
    }

    // output
    if (audio->getChannel4SoundOn() == 0) {
        outputVolume = 0;
    }

    else {
        uint8_t aux = ((lfsr & 1) == 0) ? 1: 0;
        outputVolume = internalVolume * (lfsr & 1);
    }
}

void Channel4::updateSoundLengthCycles(uint8_t soundLength)
{
    remainingSoundLengthCycles = (4194304 * (64 - soundLength)) / 256;
}

uint64_t Channel4::calcStepCycles(uint8_t shiftClockFrequency, uint8_t dividingRatio)
{
    uint64_t freq;
    if (dividingRatio == 0) {
        return 4194304 / (524228 * 2 / shiftClockFrequencies[shiftClockFrequency]);
    } else {
        return 4194304 / (524228 / dividingRatio / shiftClockFrequencies[shiftClockFrequency]);
    }
}
