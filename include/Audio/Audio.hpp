#ifndef __AUDIO_H__
#define __AUDIO_H__

#pragma once
#include <cstdint>

class Memory;

class Audio
{
  public:
    Memory *memory;

    Audio();

    void cycle();
    void cycleCh1();
    void cycleCh2();
    void cycleCh3();
    void cycleCh4();

    // Channel 1 Sweep - NR10 - 0xFF10
    uint8_t getChannel1Sweep();
    uint8_t getChannel1SweepTime();
    uint8_t getChannel1SweepIncreaseDecrease();
    uint8_t getChannel1SweepShift();
    void setChannel1Sweep(uint8_t val);
    void setChannel1SweepTime(uint8_t val);
    void setChannel1SweepIncreaseDecrease(uint8_t val);
    void setChannel1SweepShift(uint8_t val);

    // Channel 1 Length / Wave pattern duty - NR11 - 0xFF11
    uint8_t getChannel1WavePatternDuty();
    uint8_t getChannel1SoundLengthData();
    void setChannel1WavePatternDuty(uint8_t val);
    void setChannel1SoundLengthData(uint8_t val);

    // Channel 1 Volume Envelope - NR 12 - 0xFF12
    uint8_t getChannel1InitialVolumeEnvelope();
    uint8_t getChannel1EnvelopeDirection();
    uint8_t getChannel1EnvelopeSweep();
    void setChannel1InitialVolumeEnvelope(uint8_t val);
    void setChannel1EnvelopeDirection(uint8_t val);
    void setChannel1EnvelopeSweep(uint8_t val);

    // Channel 1 Frequency Low - NR13 - 0xFF13
    uint8_t getChannel1FrequencyLow();
    void setChannel1FrequencyLow(uint8_t val);

    // Channel 1 Frequency High - NR14 - 0xFF14
    uint8_t getChannel1FrequencyHigh();
    uint8_t getChannel1Initial();
    uint8_t getChannel1CounterSelection();
    void setChannel1FrequencyHigh(uint8_t val);
    void setChannel1Initial(uint8_t val);
    void setChannel1CounterSelection(uint8_t val);

    uint16_t getChannel1Frequency();
    void setChannel1Frequency(uint16_t val);

    // Channel 2 Length / Wave Pattern Duty - NR21 - 0xFF16
    uint8_t getChannel2WavePatternDuty();
    uint8_t getChannel2SoundLengthData();
    void setChannel2WavePatternDuty(uint8_t val);
    void setChannel2SoundLengthData(uint8_t val);

    // Channel 2 Volume Envelope - NR22 - 0xFF17
    uint8_t getChannel2InitialVolumeEnvelope();
    uint8_t getChannel2EnvelopeDirection();
    uint8_t getChannel2EnvelopeSweep();
    void setChannel2InitialVolumeEnvelope(uint8_t val);
    void setChannel2EnvelopeDirection(uint8_t val);
    void setChannel2EnvelopeSweep(uint8_t val);

    // Channel 2 Frequency Low - NR23 - 0xFF18
    uint8_t getChannel2FrequencyLow();
    void setChannel2FrequencyLow(uint8_t val);

    // Channel 2 Frequency High - NR24 - 0xFF19
    uint8_t getChannel2FrequencyHigh();
    uint8_t getChannel2Initial();
    uint8_t getChannel2CounterSelection();
    void setChannel2FrequencyHigh(uint8_t val);
    void setChannel2Initial(uint8_t val);
    void setChannel2CounterSelection(uint8_t val);

    uint16_t getChannel2Frequency();
    void setChannel2Frequency(uint16_t val);

    // Channel 3 Sound On / Off - NR30 - 0xFF1A
    uint8_t getChannel3Enable();
    void setChannel3Enable(uint8_t val);

    // Channel 3 Sound Length - NR31 - 0xFF1B
    uint8_t getChannel3SoundLength();
    void setChannel3SoundLength(uint8_t val);

    // Channel 3 Output Level - NR32 - 0xFF1C
    uint8_t getChannel3OutputLevel();
    void setChannel3OutputLevel(uint8_t val);

    // Channel 3 Frequency Low - NR33 - 0xFF1D
    uint8_t getChannel3FrequencyLow();
    void setChannel3FrequencyLow(uint8_t val);

    // Channel 3 Frequency High - NR34 - 0xFF1E
    uint8_t getChannel3FrequencyHigh();
    uint8_t getChannel3Initial();
    uint8_t getChannel3CounterSelection();
    void setChannel3FrequencyHigh(uint8_t val);
    void setChannel3Initial(uint8_t val);
    void setChannel3CounterSelection(uint8_t val);

    uint16_t getChannel3Frequency();
    void setChannel3Frequency(uint16_t val);

    // Wave ram??
    // are 16 bytes: de la 0xFF30 la 0xFF3F
    // se citesc bytesii in ordine, nu tine cont de little endian i think
    // uint32_t getChannel3WavePattern();
    // void setChannel3WavePattern(uint32_t val);

    // Channel 4 Sound Length - NR41 - 0xFF20
    uint8_t getChannel4SoundLength();
    void setChannel4SoundLength(uint8_t val);

    // Channel 4 Volume Envelope - NR42 - 0xFF21
    uint8_t getChannel4InitialVolumeEnvelope();
    uint8_t getChannel4EnvelopeDirection();
    uint8_t getChannel4EnvelopeSweep();
    void setChannel4InitialVolumeEnvelope(uint8_t val);
    void setChannel4EnvelopeDirection(uint8_t val);
    void setChannel4EnvelopeSweep(uint8_t val);

    // Channel 4 Polynomial Counter - NR43 - 0xFF22
    uint8_t getChannel4ShiftClockFrequency();
    uint8_t getChannel4CounterStep();
    uint8_t getChannel4DividingRatio();
    void setChannel4ShiftClockFrequency(uint8_t val);
    void setChannel4CounterStep(uint8_t val);
    void setChannel4DividingRatio(uint8_t val);

    // Channel 4 Counter - NR44 - 0xFF23
    uint8_t getChannel4Initial();
    uint8_t getChannel4CounterSelection();
    void setChannel4Initial(uint8_t val);
    void setChannel4CounterSelection(uint8_t val);

    // Channel Control - NR50 - 0xFF24
    uint8_t getChannelControl();
    void setChannelControl(uint8_t val);

    // Sound Output Selection - NR51 - 0xFF25
    uint8_t getSoundOutputSelection();
    void setSoundOutputSelection(uint8_t val);

    // Sound Enable - NR52 - 0xFF26
    uint8_t getSoundOn();
    void setSoundOn(uint8_t val);
};

#endif // __AUDIO_H__
