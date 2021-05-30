#include "Audio.hpp"
#include "Memory.hpp"
#include "Config.hpp"

Audio::Audio()
{
    currentAudioSamples = 0;
    currentCyclesUntilSampleCollection = 0;
    memset(audioBuffer, 0, AUDIO_NUM_SAMPLES * sizeof(float));

    channel1.audio = this;
    channel2.audio = this;
    channel3.audio = this;
    channel4.audio = this;

    currentWaitCycles = 0;
    frameSequencer = 0;

    // duty pattern 0
    dutyPatterns[0][0] = 0;
    for (uint i = 1; i < 8; ++i) {
        dutyPatterns[0][i] = 1;
    }

    // duty pattern 1
    for (uint i = 0; i < 2; ++i) {
        dutyPatterns[1][i] = 0;
    }

    for (uint i = 2; i < 8; ++i) {
        dutyPatterns[1][i] = 1;
    }

    // duty pattern 2
    for (uint i = 0; i < 4; ++i) {
        dutyPatterns[2][i] = 0;
    }

    for (uint i = 4; i < 8; ++i) {
        dutyPatterns[2][i] = 1;
    }

    // duty pattern 3
    for (uint i = 0; i < 6; ++i) {
        dutyPatterns[3][i] = 0;
    }

    for (uint i = 6; i < 8; ++i) {
        dutyPatterns[3][i] = 1;
    }
}

Audio::~Audio()
{
}

void Audio::initSDL()
{
    SDL_AudioSpec desiredSpec;

    desiredSpec.freq = AUDIO_FREQUENCY;
    desiredSpec.format = sdlAudioFormat;
    desiredSpec.channels = 2;
    desiredSpec.samples = AUDIO_NUM_SAMPLES;
    desiredSpec.callback = NULL;
    desiredSpec.userdata = this;

    SDL_AudioSpec obtainedSpec;

    SDL_OpenAudio(&desiredSpec, &obtainedSpec);

    SDL_PauseAudio(0);
}

/* CHANNEL 1 */
/* NR10 - 0xFF10 */

uint8_t Audio::getChannel1Sweep() { return memory->ioRegisters[0xFF10 - MEM_IO_START]; }

uint8_t Audio::getChannel1SweepTime()
{
    return (memory->ioRegisters[0xFF10 - MEM_IO_START] & 0x70) >> 4;
}

uint8_t Audio::getChannel1SweepIncreaseDecrease()
{
    return (memory->ioRegisters[0xFF10 - MEM_IO_START] & 0x8) >> 3;
}

uint8_t Audio::getChannel1SweepShift() { return memory->ioRegisters[0xFF10 - MEM_IO_START] & 0x7; }

void Audio::setChannel1Sweep(uint8_t val) { memory->ioRegisters[0xFF10 - MEM_IO_START] = val; }

void Audio::setChannel1SweepTime(uint8_t val)
{
    uint8_t nr10 = getChannel1Sweep();
    nr10 = nr10 & 0x88;
    val = (val & 7) << 4;
    // memory->writemem(nr10 | val, 0xFF10, true);
    memory->ioRegisters[0xFF10 - MEM_IO_START] = nr10 | val;
}

void Audio::setChannel1SweepIncreaseDecrease(uint8_t val)
{
    memory->writebit(val, 3, 0xFF10, true);
}

void Audio::setChannel1SweepShift(uint8_t val)
{
    uint8_t nr10 = getChannel1Sweep();
    nr10 = nr10 & 0xF8;
    val = val & 7;
    // memory->writemem(nr10 | val, 0xFF10, true);
    memory->ioRegisters[0xFF10 - MEM_IO_START] = nr10 | val;
}

/* NR11 - 0xFF11 */

uint8_t Audio::getChannel1WavePatternDuty()
{
    return (memory->ioRegisters[0xFF11 - MEM_IO_START] & 0xC0) >> 6;
}

uint8_t Audio::getChannel1SoundLengthData()
{
    return memory->ioRegisters[0xFF11 - MEM_IO_START] & 0x3F;
}

void Audio::setChannel1WavePatternDuty(uint8_t val)
{
    uint8_t nr11 = memory->readmem(0xFF11, true);
    nr11 = nr11 & 0x3F;
    val = (val & 3) << 6;
    // memory->writemem(nr11 | val, 0xFF11, true);
    memory->ioRegisters[0xFF11 - MEM_IO_START] = nr11 | val;
}

void Audio::setChannel1SoundLengthData(uint8_t val)
{
    uint8_t nr11 = memory->readmem(0xFF11, true);
    nr11 = nr11 & 0x3F;
    val = (val & 3) << 6;
    // memory->writemem(nr11 | val, 0xFF11, true);
    memory->ioRegisters[0xFF11 - MEM_IO_START] = nr11 | val;
}

/* NR12 - 0xFF12 */

uint8_t Audio::getChannel1InitialVolumeEnvelope()
{
    return (memory->ioRegisters[0xFF12 - MEM_IO_START] & 0xF0) >> 4;
}

uint8_t Audio::getChannel1EnvelopeDirection()
{
    return (memory->ioRegisters[0xFF12 - MEM_IO_START] & 0x8) >> 3;
}

uint8_t Audio::getChannel1EnvelopeSweep()
{
    return memory->ioRegisters[0xFF12 - MEM_IO_START] & 0x7;
}

void Audio::setChannel1InitialVolumeEnvelope(uint8_t val)
{
    uint8_t nr12 = memory->readmem(0xFF12, true);
    nr12 = nr12 & 0x0F;
    val = (val & 0xF) << 4;
    // memory->writemem(nr12 | val, 0xFF12, true);
    memory->ioRegisters[0xFF12 - MEM_IO_START] = nr12 | val;
}

void Audio::setChannel1EnvelopeDirection(uint8_t val)
{
    memory->ioRegisters[0xFF12 - MEM_IO_START] = val;
}

void Audio::setChannel1EnvelopeSweep(uint8_t val)
{
    uint8_t nr12 = memory->readmem(0xFF12, true);
    nr12 = nr12 & 0xF0;
    val = val & 0x7;
    // memory->writemem(nr12 | val, 0xFF12, true);
    memory->ioRegisters[0xFF12 - MEM_IO_START] = nr12 | val;
}

/* NR13 - 0xFF13 */

uint8_t Audio::getChannel1FrequencyLow() { return memory->ioRegisters[0xFF13 - MEM_IO_START]; }

void Audio::setChannel1FrequencyLow(uint8_t val)
{
    memory->ioRegisters[0xFF13 - MEM_IO_START] = val;
}

/* NR14 - 0xFF14 */

uint8_t Audio::getChannel1FrequencyHigh()
{
    return memory->ioRegisters[0xFF14 - MEM_IO_START] & 0x7;
}

uint8_t Audio::getChannel1Initial()
{
    return (memory->ioRegisters[0xFF14 - MEM_IO_START] & 0x80) >> 7;
}

uint8_t Audio::getChannel1CounterSelection()
{
    return (memory->ioRegisters[0xFF14 - MEM_IO_START] & 0x40) >> 6;
}

void Audio::setChannel1FrequencyHigh(uint8_t val)
{
    uint8_t nr14 = memory->readmem(0xFF14, true);
    nr14 = nr14 & 0xF8;
    val = val & 0x7;
    // memory->writemem(nr14 | val, 0xFF14, true);
    memory->ioRegisters[0xFF14 - MEM_IO_START] = nr14 | val;
}

void Audio::setChannel1Initial(uint8_t val) { memory->writebit(val, 7, 0xFF14, true); }

void Audio::setChannel1CounterSelection(uint8_t val) { memory->writebit(val, 6, 0xFF14, true); }

uint16_t Audio::getChannel1Frequency()
{
    return (getChannel1FrequencyHigh() << 8) | getChannel1FrequencyLow();
}

void Audio::setChannel1Frequency(uint16_t val)
{
    uint8_t low = val & 0xFF;
    uint8_t high = val & 0x700;

    setChannel1FrequencyLow(low);
    setChannel1FrequencyHigh(high);
}

/* CHANNEL 2 */
/* NR21 - 0xFF16 */

uint8_t Audio::getChannel2WavePatternDuty()
{
    return (memory->ioRegisters[0xFF16 - MEM_IO_START] & 0xC0) >> 6;
}

uint8_t Audio::getChannel2SoundLengthData()
{
    return memory->ioRegisters[0xFF16 - MEM_IO_START] & 0x3F;
}

void Audio::setChannel2WavePatternDuty(uint8_t val)
{
    uint8_t nr21 = memory->ioRegisters[0xFF16 - MEM_IO_START];
    nr21 = nr21 & 0x3F;
    val = (val & 0x3) << 6;
    // memory->writemem(nr21 | val, 0xFF16, true);
    memory->ioRegisters[0xFF16 - MEM_IO_START] = nr21 | val;
}

void Audio::setChannel2SoundLengthData(uint8_t val)
{
    uint8_t nr21 = memory->ioRegisters[0xFF16 - MEM_IO_START];
    nr21 = nr21 & 0xC0;
    val = val & 0x3F;
    // memory->writemem(nr21 | val, 0xFF16, true);
    memory->ioRegisters[0xFF16 - MEM_IO_START] = nr21 | val;
}

/* NR22 - 0xFF17 */

uint8_t Audio::getChannel2InitialVolumeEnvelope()
{
    return (memory->ioRegisters[0xFF17 - MEM_IO_START] & 0xF0) >> 4;
}

uint8_t Audio::getChannel2EnvelopeDirection()
{
    return (memory->ioRegisters[0xFF17 - MEM_IO_START] & 0x8) >> 3;
}

uint8_t Audio::getChannel2EnvelopeSweep()
{
    return memory->ioRegisters[0xFF17 - MEM_IO_START] & 0x7;
}

void Audio::setChannel2InitialVolumeEnvelope(uint8_t val)
{
    uint8_t nr22 = memory->ioRegisters[0xFF17 - MEM_IO_START];
    nr22 = nr22 & 0x0F;
    val = (val & 0xF) << 4;
    memory->writemem(nr22 | val, 0xFF17, true);
}

void Audio::setChannel2EnvelopeDirection(uint8_t val) { memory->writebit(val, 3, 0xFF17, true); }

void Audio::setChannel2EnvelopeSweep(uint8_t val)
{
    uint8_t nr22 = memory->ioRegisters[0xFF17 - MEM_IO_START];
    nr22 = nr22 & 0xF8;
    val = val & 0x7;
    // memory->writemem(nr22 | val, 0xFF17, true);
    memory->ioRegisters[0xFF17 - MEM_IO_START] = nr22 | val;
}

/* NR23 - 0xFF18 */

uint8_t Audio::getChannel2FrequencyLow() { return memory->ioRegisters[0xFF18 - MEM_IO_START]; }

void Audio::setChannel2FrequencyLow(uint8_t val) { memory->writemem(val, 0xFF18, true); }

/* NR24 - 0xFF19 */

uint8_t Audio::getChannel2FrequencyHigh()
{
    return memory->ioRegisters[0xFF19 - MEM_IO_START] & 0x7;
}

uint8_t Audio::getChannel2Initial()
{
    return (memory->ioRegisters[0xFF19 - MEM_IO_START] & 0x80) >> 7;
}

uint8_t Audio::getChannel2CounterSelection()
{
    return (memory->ioRegisters[0xFF19 - MEM_IO_START] & 0x40) >> 6;
}

void Audio::setChannel2FrequencyHigh(uint8_t val)
{
    uint8_t nr24 = memory->ioRegisters[0xFF19 - MEM_IO_START];
    nr24 = nr24 & 0xF8;
    val = val & 0x7;
    // memory->writemem(nr24 | val, 0xFF19, true);
    memory->ioRegisters[0xFF19 - MEM_IO_START] = nr24 | val;
}

void Audio::setChannel2Initial(uint8_t val) { memory->writebit(val, 7, 0xFF19, true); }

void Audio::setChannel2CounterSelection(uint8_t val) { memory->writebit(val, 6, 0xFF19, true); }

uint16_t Audio::getChannel2Frequency()
{
    return (getChannel2FrequencyHigh() << 8) | getChannel2FrequencyLow();
}

void Audio::setChannel2Frequency(uint16_t val)
{
    uint8_t low = val & 0xFF;
    uint8_t high = val & 0x700;

    setChannel2FrequencyLow(low);
    setChannel2FrequencyHigh(high);
}

/* CHANNEL 3 */
/* NR30 - 0xFF1A */

uint8_t Audio::getChannel3Enable()
{
    return (memory->ioRegisters[0xFF1A - MEM_IO_START] & 0x80) >> 7;
}

void Audio::setChannel3Enable(uint8_t val) { memory->writebit(val, 7, 0xFF1A, true); }

/* NR31 - 0xFF1B */

uint8_t Audio::getChannel3SoundLength() { return memory->ioRegisters[0xFF1B - MEM_IO_START]; }

void Audio::setChannel3SoundLength(uint8_t val)
{
    memory->ioRegisters[0xFF1B - MEM_IO_START] = val;
}

/* NR32 - 0xFF1C */

uint8_t Audio::getChannel3OutputLevel()
{
    return (memory->ioRegisters[0xFF1C - MEM_IO_START] & 0x60) >> 5;
}

void Audio::setChannel3OutputLevel(uint8_t val)
{
    uint8_t nr32 = memory->ioRegisters[0xFF1C - MEM_IO_START];
    nr32 = nr32 & 0x9F;
    val = (val & 0x3) << 5;
    memory->ioRegisters[0xFF1C - MEM_IO_START] = nr32 | val;
    ;
}

/* NR33 - 0xFF1D */

uint8_t Audio::getChannel3FrequencyLow() { return memory->ioRegisters[0xFF1D - MEM_IO_START]; }

void Audio::setChannel3FrequencyLow(uint8_t val)
{
    memory->ioRegisters[0xFF1D - MEM_IO_START] = val;
}

/* NR34 - 0xFF1E */

uint8_t Audio::getChannel3FrequencyHigh()
{
    return memory->ioRegisters[0xFF1E - MEM_IO_START] & 0x7;
}

uint8_t Audio::getChannel3Initial()
{
    return (memory->ioRegisters[0xFF1D - MEM_IO_START] & 0x80) >> 7;
}

uint8_t Audio::getChannel3CounterSelection()
{
    return (memory->ioRegisters[0xFF1D - MEM_IO_START] & 0x40) >> 6;
}

void Audio::setChannel3FrequencyHigh(uint8_t val)
{
    uint8_t nr34 = memory->ioRegisters[0xFF1D - MEM_IO_START];
    nr34 = nr34 & 0xF8;
    val = val & 0x7;
    // memory->writemem(nr34 | val, 0xFF1E, true);
    memory->ioRegisters[0xFF1E - MEM_IO_START] = nr34 | val;
}

void Audio::setChannel3Initial(uint8_t val) { memory->writebit(val, 7, 0xFF1E, true); }

void Audio::setChannel3CounterSelection(uint8_t val) { memory->writebit(val, 6, 0xFF1E, true); }

uint16_t Audio::getChannel3Frequency()
{
    return (getChannel3FrequencyHigh() << 8) | getChannel3FrequencyLow();
}

void Audio::setChannel3Frequency(uint16_t val)
{
    uint8_t low = val & 0xFF;
    uint8_t high = val & 0x700;

    setChannel3FrequencyLow(low);
    setChannel3FrequencyHigh(high);
}

/* Wave RAM */
// TODO

/* CHANNEL 4 */
/* NR41 - 0xFF20 */

uint8_t Audio::getChannel4SoundLength()
{
    return memory->ioRegisters[0xFF20 - MEM_IO_START] & 0x3F;
}

void Audio::setChannel4SoundLength(uint8_t val)
{
    uint8_t nr41 = memory->ioRegisters[0xFF20 - MEM_IO_START];
    nr41 = nr41 & 0xC0;
    val = val & 0x3F;
    // memory->writemem(nr41 | val, 0xFF20, true);
    memory->ioRegisters[0xFF20 - MEM_IO_START] = nr41 | val;
}

/* NR42 - 0xFF21 */

uint8_t Audio::getChannel4InitialVolumeEnvelope()
{
    return (memory->ioRegisters[0xFF21 - MEM_IO_START] & 0xF0) >> 4;
}

uint8_t Audio::getChannel4EnvelopeDirection()
{
    return (memory->ioRegisters[0xFF21 - MEM_IO_START] & 0x8) >> 3;
}

uint8_t Audio::getChannel4EnvelopeSweep()
{
    return memory->ioRegisters[0xFF21 - MEM_IO_START] & 0x7;
}

void Audio::setChannel4InitialVolumeEnvelope(uint8_t val)
{
    uint8_t nr42 = memory->ioRegisters[0xFF21 - MEM_IO_START];
    nr42 = nr42 & 0x0F;
    val = (val & 0xF) << 4;
    // memory->writemem(nr42 | val, 0xFF21, true);
    memory->ioRegisters[0xFF21 - MEM_IO_START] = nr42 | val;
}

void Audio::setChannel4EnvelopeDirection(uint8_t val) { memory->writebit(val, 3, 0xFF21, true); }

void Audio::setChannel4EnvelopeSweep(uint8_t val)
{
    uint8_t nr42 = memory->ioRegisters[0xFF21 - MEM_IO_START];
    nr42 = nr42 & 0xF8;
    val = val & 0x7;
    // memory->writemem(nr42 | val, 0xFF21, true);
    memory->ioRegisters[0xFF21 - MEM_IO_START] = nr42 | val;
}

/* NR43 - 0xFF22 */

uint8_t Audio::getChannel4ShiftClockFrequency()
{
    return (memory->ioRegisters[0xFF22 - MEM_IO_START] & 0xF0) >> 4;
}

uint8_t Audio::getChannel4CounterStep()
{
    return (memory->ioRegisters[0xFF22 - MEM_IO_START] & 0x8) >> 3;
}

uint8_t Audio::getChannel4DividingRatio()
{
    return memory->ioRegisters[0xFF22 - MEM_IO_START] & 0x7;
}

void Audio::setChannel4ShiftClockFrequency(uint8_t val)
{
    uint8_t nr43 = memory->ioRegisters[0xFF22 - MEM_IO_START];
    nr43 = nr43 & 0x0F;
    val = (val & 0xF) << 4;
    // memory->writemem(nr42 | val, 0xFF22, true);
    memory->ioRegisters[0xFF22 - MEM_IO_START] = nr43 | val;
}

void Audio::setChannel4CounterStep(uint8_t val) { memory->writebit(val, 3, 0xFF22, true); }

void Audio::setChannel4DividingRatio(uint8_t val)
{
    uint8_t nr43 = memory->ioRegisters[0xFF22 - MEM_IO_START];
    nr43 = nr43 & 0xF8;
    val = val & 0x7;
    // memory->writemem(nr43 | val, 0xFF22, true);
    memory->ioRegisters[0xFF22 - MEM_IO_START] = nr43 | val;
}

/* NR44 - 0xFF23 */

uint8_t Audio::getChannel4Initial()
{
    return (memory->ioRegisters[0xFF23 - MEM_IO_START] & 0x80) >> 7;
}

uint8_t Audio::getChannel4CounterSelection()
{
    return (memory->ioRegisters[0xFF23 - MEM_IO_START] & 0x40) >> 6;
}

void Audio::setChannel4Initial(uint8_t val) { memory->writebit(val, 7, 0xFF23, true); }

void Audio::setChannel4CounterSelection(uint8_t val) { memory->writebit(val, 6, 0xFF23, true); }

/* SOUND CONTROL */
/* NR50 - 0xFF24 */

uint8_t Audio::getChannelControl() { return memory->ioRegisters[0xFF24 - MEM_IO_START]; }

uint8_t Audio::getLeftChannelVolume()
{
    return (memory->ioRegisters[0xFF24 - MEM_IO_START] & 0x70) >> 4;
}

uint8_t Audio::getRightChannelVolume() { return memory->ioRegisters[0xFF24 - MEM_IO_START] & 7; }

void Audio::setChannelControl(uint8_t val) { memory->ioRegisters[0xFF24 - MEM_IO_START] = val; }

void Audio::setLeftChannelVolume(uint8_t val)
{
    uint8_t nr50 = memory->ioRegisters[0xFF24 - MEM_IO_START];
    nr50 = nr50 & 0x8F;
    val = (val & 0x7) << 4;
    memory->ioRegisters[0xFF24 - MEM_IO_START] = nr50 | val;
}

void Audio::setRightChannelVolume(uint8_t val)
{
    uint8_t nr50 = memory->ioRegisters[0xFF24 - MEM_IO_START];
    nr50 = nr50 & 0xF8;
    val = val & 0x7;
    // memory->writemem(nr50 | val, 0xFF24, true);
    memory->ioRegisters[0xFF24 - MEM_IO_START] = nr50 | val;
}

/* NR51 - 0xFF25 */

uint8_t Audio::getSoundOutputSelection() { return memory->ioRegisters[0xFF25 - MEM_IO_START]; }

uint8_t Audio::getChannel4LeftOutput()
{
    return (memory->ioRegisters[0xFF25 - MEM_IO_START] & 0x80) >> 7;
}

uint8_t Audio::getChannel3LeftOutput()
{
    return (memory->ioRegisters[0xFF25 - MEM_IO_START] & 0x40) >> 6;
}

uint8_t Audio::getChannel2LeftOutput()
{
    return (memory->ioRegisters[0xFF25 - MEM_IO_START] & 0x20) >> 5;
}

uint8_t Audio::getChannel1LeftOutput()
{
    return (memory->ioRegisters[0xFF25 - MEM_IO_START] & 0x10) >> 4;
}

uint8_t Audio::getChannel4RightOutput()
{
    return (memory->ioRegisters[0xFF25 - MEM_IO_START] & 0x8) >> 3;
}

uint8_t Audio::getChannel3RightOutput()
{
    return (memory->ioRegisters[0xFF25 - MEM_IO_START] & 0x4) >> 2;
}

uint8_t Audio::getChannel2RightOutput()
{
    return (memory->ioRegisters[0xFF25 - MEM_IO_START] & 0x2) >> 1;
}

uint8_t Audio::getChannel1RightOutput() { return memory->ioRegisters[0xFF25 - MEM_IO_START] & 0x1; }

void Audio::setSoundOutputSelection(uint8_t val)
{
    memory->ioRegisters[0xFF25 - MEM_IO_START] = val;
}

void Audio::setChannel4LeftOutput(uint8_t val) { memory->writebit(val, 7, 0xFF25, true); }

void Audio::setChannel3LeftOutput(uint8_t val) { memory->writebit(val, 6, 0xFF25, true); }

void Audio::setChannel2LeftOutput(uint8_t val) { memory->writebit(val, 5, 0xFF25, true); }

void Audio::setChannel1LeftOutput(uint8_t val) { memory->writebit(val, 4, 0xFF25, true); }

void Audio::setChannel4RightOutput(uint8_t val) { memory->writebit(val, 3, 0xFF25, true); }

void Audio::setChannel3RightOutput(uint8_t val) { memory->writebit(val, 2, 0xFF25, true); }

void Audio::setChannel2RightOutput(uint8_t val) { memory->writebit(val, 1, 0xFF25, true); }

void Audio::setChannel1RightOutput(uint8_t val) { memory->writebit(val, 0, 0xFF25, true); }

/* NR52 - 0xFF26 */

uint8_t Audio::getSoundOn() { return memory->ioRegisters[0xFF26 - MEM_IO_START]; }

uint8_t Audio::getAllSoundOn() { return (memory->ioRegisters[0xFF26 - MEM_IO_START] & 0x80) >> 7; }

uint8_t Audio::getChannel4SoundOn()
{
    return (memory->ioRegisters[0xFF26 - MEM_IO_START] & 0x8) >> 3;
}

uint8_t Audio::getChannel3SoundOn()
{
    return (memory->ioRegisters[0xFF26 - MEM_IO_START] & 0x4) >> 2;
}

uint8_t Audio::getChannel2SoundOn()
{
    return (memory->ioRegisters[0xFF26 - MEM_IO_START] & 0x2) >> 1;
}

uint8_t Audio::getChannel1SoundOn() { return memory->ioRegisters[0xFF26 - MEM_IO_START] & 0x1; }

void Audio::setSoundOn(uint8_t val) { memory->ioRegisters[0xFF26 - MEM_IO_START] = val; }

void Audio::setAllSoundOn(uint8_t val) { memory->writebit(val, 7, 0xFF26, true); }

void Audio::setChannel4SoundOn(uint8_t val) { memory->writebit(val, 3, 0xFF26, true); }

void Audio::setChannel3SoundOn(uint8_t val) { memory->writebit(val, 2, 0xFF26, true); }

void Audio::setChannel2SoundOn(uint8_t val) { memory->writebit(val, 1, 0xFF26, true); }

void Audio::setChannel1SoundOn(uint8_t val) { memory->writebit(val, 0, 0xFF26, true); }

void Audio::cycle(uint8_t numCycles)
{
    if (getAllSoundOn() != 0) {
        if (!initialInit) {
            initialInit = true;

            channel1.initCh();
            channel2.initCh();
            channel3.initCh();
            channel4.initCh();
        }

        currentWaitCycles += numCycles;

        channel1.cycleDuty(numCycles);
        channel2.cycleDuty(numCycles);
        channel3.cycle(numCycles);
        channel4.cycleLfsr(numCycles);

        if (currentWaitCycles >= AUDIO_WAIT_CYCLES) {
            currentWaitCycles -= AUDIO_WAIT_CYCLES;
            frameSequencer = (frameSequencer + 1) % 8;

            switch (frameSequencer) {
            case 0:
                channel1.cycleLength();
                channel2.cycleLength();
                channel3.cycleLength();
                channel4.cycleLength();
                break;
            case 2:
                channel1.cycleLength();
                channel1.cycleSweep();

                channel2.cycleLength();

                channel3.cycleLength();

                channel4.cycleLength();
                break;
            case 4:
                channel1.cycleLength();
                channel2.cycleLength();
                channel3.cycleLength();
                channel4.cycleLength();
                break;
            case 6:
                channel1.cycleLength();
                channel1.cycleSweep();

                channel2.cycleLength();

                channel3.cycleLength();

                channel4.cycleLength();
                break;
            case 7:
                channel1.cycleEnvelope();
                channel2.cycleEnvelope();
                channel4.cycleEnvelope();
                break;
            }
        }
    }

    currentCyclesUntilSampleCollection += numCycles;
    if (currentCyclesUntilSampleCollection >= AUDIO_CYCLES_UNTIL_SAMPLE_COLLECTION) {
        currentCyclesUntilSampleCollection -= AUDIO_CYCLES_UNTIL_SAMPLE_COLLECTION;

        // Collect sample
        float mixedLeftChannel = 0, mixedRightChannel = 0;
        float audioVolume = Config::getInstance()->getAudioVolume();
        int leftVol = (getLeftChannelVolume() * audioVolume * SDL_MIX_MAXVOLUME) / 7;
        int rightVol = (getRightChannelVolume() * audioVolume * SDL_MIX_MAXVOLUME) / 7;
        float aux;

        // Channel 1
        aux = ((float)channel1.getVolume()) / SDL_MIX_MAXVOLUME;

        if (getChannel1LeftOutput()) {
            SDL_MixAudioFormat((uint8_t *)&mixedLeftChannel, (uint8_t *)&aux, sdlAudioFormat,
                               sizeof(float), leftVol);
        }

        if (getChannel1RightOutput()) {
            SDL_MixAudioFormat((uint8_t *)&mixedRightChannel, (uint8_t *)&aux, sdlAudioFormat,
                               sizeof(float), rightVol);
        }

        // Channel 2
        aux = ((float)channel2.getVolume()) / SDL_MIX_MAXVOLUME;

        if (getChannel2LeftOutput()) {
            SDL_MixAudioFormat((uint8_t *)&mixedLeftChannel, (uint8_t *)&aux, sdlAudioFormat,
                               sizeof(float), leftVol);
        }

        if (getChannel2RightOutput()) {
            SDL_MixAudioFormat((uint8_t *)&mixedRightChannel, (uint8_t *)&aux, sdlAudioFormat,
                               sizeof(float), rightVol);
        }

        // Channel 3
        aux = ((float)channel3.getVolume()) / SDL_MIX_MAXVOLUME;

        if (getChannel3LeftOutput()) {
            SDL_MixAudioFormat((uint8_t *)&mixedLeftChannel, (uint8_t *)&aux, sdlAudioFormat,
                               sizeof(float), leftVol);
        }

        if (getChannel3RightOutput()) {
            SDL_MixAudioFormat((uint8_t *)&mixedRightChannel, (uint8_t *)&aux, sdlAudioFormat,
                               sizeof(float), rightVol);
        }

        // Channel 4
        aux = ((float)channel4.getVolume()) / SDL_MIX_MAXVOLUME;

        if (getChannel4LeftOutput()) {
            SDL_MixAudioFormat((uint8_t *)&mixedLeftChannel, (uint8_t *)&aux, sdlAudioFormat,
                               sizeof(float), leftVol);
        }

        if (getChannel4RightOutput()) {
            SDL_MixAudioFormat((uint8_t *)&mixedRightChannel, (uint8_t *)&aux, sdlAudioFormat,
                               sizeof(float), rightVol);
        }

        audioBuffer[currentAudioSamples++] = mixedLeftChannel;
        audioBuffer[currentAudioSamples++] = mixedRightChannel;
    }

    if (currentAudioSamples >= AUDIO_NUM_SAMPLES) {
        currentAudioSamples -= AUDIO_NUM_SAMPLES;

        // Queue audio
        SDL_QueueAudio(1, audioBuffer, AUDIO_NUM_SAMPLES * sizeof(float));
    }
}
