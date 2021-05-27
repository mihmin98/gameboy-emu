#include "Channel3.hpp"
#include "Audio.hpp"
#include "Memory.hpp"
#include <iostream>

Channel3::Channel3()
{
    internalVolume = 0;
    outputVolume = 0;

    currentCycles = 0;
}

void Channel3::initCh()
{
    samplePosition = 0;

    cyclesUntilNextStep = 8 * (2048 - audio->getChannel3Frequency());
    currentCycles = 0;
}

void Channel3::cycle()
{
    ++currentCycles;
    if (currentCycles == cyclesUntilNextStep) {
        currentCycles = 0;
        cyclesUntilNextStep = 8 * (2048 - audio->getChannel3Frequency());

        if (audio->getChannel3Enable()) {
            uint16_t byteAddr = 0xFF30 + (samplePosition / 2);
            uint8_t byte = audio->memory->readmem(byteAddr, true);

            uint8_t sample;
            if ((samplePosition & 1) == 0) {
                // Upper half
                sample = (byte & 0xF0) >> 4;
            } else {
                // Lower half
                sample = byte & 0x0F;
            }

            switch (audio->getChannel3OutputLevel()) {
            case 0:
                internalVolume = 0;
                break;
            case 1:
                internalVolume = sample;
                break;
            case 2:
                internalVolume = sample >> 1;
                break;
            case 3:
                internalVolume = sample >> 2;
                break;
            default:
                internalVolume = 0;
            }

            samplePosition = (samplePosition + 1) & 0x1F;
        }
    }

    // update remiainig sound length cycles
    if (remainingSoundLengthCycles > 0)
        --remainingSoundLengthCycles;

    if ((remainingSoundLengthCycles == 0 && audio->getChannel3CounterSelection() == 1) ||
        audio->getChannel3Enable() == 0) {
        audio->setChannel3SoundOn(0);
    } else {
        audio->setChannel3SoundOn(1);
    }

    if (audio->getChannel3SoundOn() == 0) {
        outputVolume = 0;
    }

    else {
        outputVolume = internalVolume;
    }
}

void Channel3::updateSoundLengthCycles(uint8_t soundLength)
{
    remainingSoundLengthCycles = (4194304 * (64 - soundLength)) / 256;
}
