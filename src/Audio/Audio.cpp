#include "Audio.hpp"
#include "Memory.hpp"

Audio::Audio()
{

    currentAudioSamples = 0;
    currentCyclesUntilSampleCollection = 0;
    memset(audioBuffer, 0, AUDIO_NUM_SAMPLES * sizeof(float));

    channel1.audio = this;
    channel2.audio = this;
    channel3.audio = this;
    channel4.audio = this;

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
    // delete channel2;
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

uint8_t Audio::getChannel1Sweep() { return memory->readmem(0xFF10, true); }

uint8_t Audio::getChannel1SweepTime() { return (memory->readmem(0xFF10, true) & 0x70) >> 4; }

uint8_t Audio::getChannel1SweepIncreaseDecrease()
{
    return (memory->readmem(0xFF10, true) & 0x8) >> 3;
}

uint8_t Audio::getChannel1SweepShift() { return memory->readmem(0xFF10, true) & 0x7; }

void Audio::setChannel1Sweep(uint8_t val) { memory->writemem(val, 0xFF10, true); }

void Audio::setChannel1SweepTime(uint8_t val)
{
    uint8_t nr10 = getChannel1Sweep();
    nr10 = nr10 & 0x88;
    val = (val & 7) << 4;
    memory->writemem(nr10 | val, 0xFF10, true);
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
    memory->writemem(nr10 | val, 0xFF10, true);
}

/* NR11 - 0xFF11 */

uint8_t Audio::getChannel1WavePatternDuty() { return (memory->readmem(0xFF11, true) & 0xC0) >> 6; }

uint8_t Audio::getChannel1SoundLengthData() { return memory->readmem(0xFF11, true) & 0x3F; }

void Audio::setChannel1WavePatternDuty(uint8_t val)
{
    uint8_t nr11 = memory->readmem(0xFF11, true);
    nr11 = nr11 & 0x3F;
    val = (val & 3) << 6;
    memory->writemem(nr11 | val, 0xFF11, true);
}

void Audio::setChannel1SoundLengthData(uint8_t val)
{
    uint8_t nr11 = memory->readmem(0xFF11, true);
    nr11 = nr11 & 0x3F;
    val = (val & 3) << 6;
    memory->writemem(nr11 | val, 0xFF11, true);
}

/* NR12 - 0xFF12 */

uint8_t Audio::getChannel1InitialVolumeEnvelope()
{
    return (memory->readmem(0xFF12, true) & 0xF0) >> 4;
}

uint8_t Audio::getChannel1EnvelopeDirection() { return (memory->readmem(0xFF12, true) & 0x8) >> 3; }

uint8_t Audio::getChannel1EnvelopeSweep() { return memory->readmem(0xFF12, true) & 0x7; }

void Audio::setChannel1InitialVolumeEnvelope(uint8_t val)
{
    uint8_t nr12 = memory->readmem(0xFF12, true);
    nr12 = nr12 & 0x0F;
    val = (val & 0xF) << 4;
    memory->writemem(nr12 | val, 0xFF12, true);
}

void Audio::setChannel1EnvelopeDirection(uint8_t val) { memory->writebit(val, 3, 0xFF12, true); }

void Audio::setChannel1EnvelopeSweep(uint8_t val)
{
    uint8_t nr12 = memory->readmem(0xFF12, true);
    nr12 = nr12 & 0xF0;
    val = val & 0x7;
    memory->writemem(nr12 | val, 0xFF12, true);
}

/* NR13 - 0xFF13 */

uint8_t Audio::getChannel1FrequencyLow() { return memory->readmem(0xFF13, true); }

void Audio::setChannel1FrequencyLow(uint8_t val) { memory->writemem(val, 0xFF13, true); }

/* NR14 - 0xFF14 */

uint8_t Audio::getChannel1FrequencyHigh() { return memory->readmem(0xFF14, true) & 0x7; }

uint8_t Audio::getChannel1Initial() { return (memory->readmem(0xFF14, true) & 0x80) >> 7; }

uint8_t Audio::getChannel1CounterSelection() { return (memory->readmem(0xFF14, true) & 0x40) >> 6; }

void Audio::setChannel1FrequencyHigh(uint8_t val)
{
    uint8_t nr14 = memory->readmem(0xFF14, true);
    nr14 = nr14 & 0xF8;
    val = val & 0x7;
    memory->writemem(nr14 | val, 0xFF14, true);
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

uint8_t Audio::getChannel2WavePatternDuty() { return (memory->readmem(0xFF16, true) & 0xC0) >> 6; }

uint8_t Audio::getChannel2SoundLengthData() { return memory->readmem(0xFF16, true) & 0x3F; }

void Audio::setChannel2WavePatternDuty(uint8_t val)
{
    uint8_t nr21 = memory->readmem(0xFF16, true);
    nr21 = nr21 & 0x3F;
    val = (val & 0x3) << 6;
    memory->writemem(nr21 | val, 0xFF16, true);
}

void Audio::setChannel2SoundLengthData(uint8_t val)
{
    uint8_t nr21 = memory->readmem(0xFF16, true);
    nr21 = nr21 & 0xC0;
    val = val & 0x3F;
    memory->writemem(nr21 | val, 0xFF16, true);
}

/* NR22 - 0xFF17 */

uint8_t Audio::getChannel2InitialVolumeEnvelope()
{
    return (memory->readmem(0xFF17, true) & 0xF0) >> 4;
}

uint8_t Audio::getChannel2EnvelopeDirection() { return (memory->readmem(0xFF17, true) & 0x8) >> 3; }

uint8_t Audio::getChannel2EnvelopeSweep() { return memory->readmem(0xFF17, true) & 0x7; }

void Audio::setChannel2InitialVolumeEnvelope(uint8_t val)
{
    uint8_t nr22 = memory->readmem(0xFF17, true);
    nr22 = nr22 & 0x0F;
    val = (val & 0xF) << 4;
    memory->writemem(nr22 | val, 0xFF17, true);
}

void Audio::setChannel2EnvelopeDirection(uint8_t val) { memory->writebit(val, 3, 0xFF17, true); }

void Audio::setChannel2EnvelopeSweep(uint8_t val)
{
    uint8_t nr22 = memory->readmem(0xFF17, true);
    nr22 = nr22 & 0xF8;
    val = val & 0x7;
    memory->writemem(nr22 | val, 0xFF17, true);
}

/* NR23 - 0xFF18 */

uint8_t Audio::getChannel2FrequencyLow() { return memory->readmem(0xFF18, true); }

void Audio::setChannel2FrequencyLow(uint8_t val) { memory->writemem(val, 0xFF18, true); }

/* NR24 - 0xFF19 */

uint8_t Audio::getChannel2FrequencyHigh() { return memory->readmem(0xFF19, true) & 0x7; }

uint8_t Audio::getChannel2Initial() { return (memory->readmem(0xFF19, true) & 0x80) >> 7; }

uint8_t Audio::getChannel2CounterSelection() { return (memory->readmem(0xFF19, true) & 0x40) >> 6; }

void Audio::setChannel2FrequencyHigh(uint8_t val)
{
    uint8_t nr24 = memory->readmem(0xFF19, true);
    nr24 = nr24 & 0xF8;
    val = val & 0x7;
    memory->writemem(nr24 | val, 0xFF19, true);
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

uint8_t Audio::getChannel3Enable() { return (memory->readmem(0xFF1A, true) & 0x80) >> 7; }

void Audio::setChannel3Enable(uint8_t val) { memory->writebit(val, 7, 0xFF1A, true); }

/* NR31 - 0xFF1B */

uint8_t Audio::getChannel3SoundLength() { return memory->readmem(0xFF1B, true); }

void Audio::setChannel3SoundLength(uint8_t val) { memory->writemem(val, 0xFF1B, true); }

/* NR32 - 0xFF1C */

uint8_t Audio::getChannel3OutputLevel() { return (memory->readmem(0xFF1C, true) & 0x60) >> 5; }

void Audio::setChannel3OutputLevel(uint8_t val)
{
    uint8_t nr32 = memory->readmem(0xFF1C, true);
    nr32 = nr32 & 0x9F;
    val = (val & 0x3) << 5;
    memory->writemem(nr32 | val, 0xFF1C, true);
}

/* NR33 - 0xFF1D */

uint8_t Audio::getChannel3FrequencyLow() { return memory->readmem(0xFF1D, true); }

void Audio::setChannel3FrequencyLow(uint8_t val) { memory->writemem(val, 0xFF1D, true); }

/* NR34 - 0xFF1E */

uint8_t Audio::getChannel3FrequencyHigh() { return memory->readmem(0xFF1E, true) & 0x7; }

uint8_t Audio::getChannel3Initial() { return (memory->readmem(0xFF1E, true) & 0x80) >> 7; }

uint8_t Audio::getChannel3CounterSelection() { return (memory->readmem(0xFF1E, true) & 0x40) >> 6; }

void Audio::setChannel3FrequencyHigh(uint8_t val)
{
    uint8_t nr34 = memory->readmem(0xFF1E, true);
    nr34 = nr34 & 0xF8;
    val = val & 0x7;
    memory->writemem(nr34 | val, 0xFF1E, true);
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

uint8_t Audio::getChannel4SoundLength() { return memory->readmem(0xFF20, true) & 0x3F; }

void Audio::setChannel4SoundLength(uint8_t val)
{
    uint8_t nr41 = memory->readmem(0xFF20, true);
    nr41 = nr41 & 0xC0;
    val = val & 0x3F;
    memory->writemem(nr41 | val, 0xFF20, true);
}

/* NR42 - 0xFF21 */

uint8_t Audio::getChannel4InitialVolumeEnvelope()
{
    return (memory->readmem(0xFF21, true) & 0xF0) >> 4;
}

uint8_t Audio::getChannel4EnvelopeDirection() { return (memory->readmem(0xFF21, true) & 0x8) >> 3; }

uint8_t Audio::getChannel4EnvelopeSweep() { return memory->readmem(0xFF21, true) & 0x7; }

void Audio::setChannel4InitialVolumeEnvelope(uint8_t val)
{
    uint8_t nr42 = memory->readmem(0xFF21, true);
    nr42 = nr42 & 0x0F;
    val = (val & 0xF) << 4;
    memory->writemem(nr42 | val, 0xFF21, true);
}

void Audio::setChannel4EnvelopeDirection(uint8_t val) { memory->writebit(val, 3, 0xFF21, true); }

void Audio::setChannel4EnvelopeSweep(uint8_t val)
{
    uint8_t nr42 = memory->readmem(0xFF21, true);
    nr42 = nr42 & 0xF8;
    val = val & 0x7;
    memory->writemem(nr42 | val, 0xFF21, true);
}

/* NR43 - 0xFF22 */

uint8_t Audio::getChannel4ShiftClockFrequency()
{
    return (memory->readmem(0xFF22, true) & 0xF0) >> 4;
}

uint8_t Audio::getChannel4CounterStep() { return (memory->readmem(0xFF22, true) & 0x8) >> 3; }

uint8_t Audio::getChannel4DividingRatio() { return memory->readmem(0xFF22, true) & 0x7; }

void Audio::setChannel4ShiftClockFrequency(uint8_t val)
{
    uint8_t nr42 = memory->readmem(0xFF22, true);
    nr42 = nr42 & 0x0F;
    val = (val & 0xF) << 4;
    memory->writemem(nr42 | val, 0xFF22, true);
}

void Audio::setChannel4CounterStep(uint8_t val) { memory->writebit(val, 3, 0xFF22, true); }

void Audio::setChannel4DividingRatio(uint8_t val)
{
    uint8_t nr42 = memory->readmem(0xFF22, true);
    nr42 = nr42 & 0xF8;
    val = val & 0x7;
    memory->writemem(nr42 | val, 0xFF22, true);
}

/* NR44 - 0xFF23 */

uint8_t Audio::getChannel4Initial() { return (memory->readmem(0xFF23, true) & 0x80) >> 7; }

uint8_t Audio::getChannel4CounterSelection() { return (memory->readmem(0xFF23, true) & 0x40) >> 6; }

void Audio::setChannel4Initial(uint8_t val) { memory->writebit(val, 7, 0xFF23, true); }

void Audio::setChannel4CounterSelection(uint8_t val) { memory->writebit(val, 6, 0xFF23, true); }

/* SOUND CONTROL */
/* NR50 - 0xFF24 */

uint8_t Audio::getChannelControl() { return memory->readmem(0xFF24, true); }

uint8_t Audio::getLeftChannelVolume() { return (memory->readmem(0xFF24, true) & 0x70) >> 4; }

uint8_t Audio::getRightChannelVolume() { return memory->readmem(0xFF24, true) & 7; }

void Audio::setChannelControl(uint8_t val) { memory->writemem(val, 0xFF24, true); }

void Audio::setLeftChannelVolume(uint8_t val)
{
    uint8_t nr50 = memory->readmem(0xFF24, true);
    nr50 = nr50 & 0x8F;
    val = (val & 0x7) << 4;
    memory->writemem(nr50 | val, 0xFF24, true);
}

void Audio::setRightChannelVolume(uint8_t val)
{
    uint8_t nr50 = memory->readmem(0xFF24, true);
    nr50 = nr50 & 0xF8;
    val = val & 0x7;
    memory->writemem(nr50 | val, 0xFF24, true);
}

/* NR51 - 0xFF25 */

uint8_t Audio::getSoundOutputSelection() { return memory->readmem(0xFF25, true); }

uint8_t Audio::getChannel4LeftOutput() { return (memory->readmem(0xFF25, true) & 0x80) >> 7; }

uint8_t Audio::getChannel3LeftOutput() { return (memory->readmem(0xFF25, true) & 0x40) >> 6; }

uint8_t Audio::getChannel2LeftOutput() { return (memory->readmem(0xFF25, true) & 0x20) >> 5; }

uint8_t Audio::getChannel1LeftOutput() { return (memory->readmem(0xFF25, true) & 0x10) >> 4; }

uint8_t Audio::getChannel4RightOutput() { return (memory->readmem(0xFF25, true) & 0x8) >> 3; }

uint8_t Audio::getChannel3RightOutput() { return (memory->readmem(0xFF25, true) & 0x4) >> 2; }

uint8_t Audio::getChannel2RightOutput() { return (memory->readmem(0xFF25, true) & 0x2) >> 1; }

uint8_t Audio::getChannel1RightOutput() { return memory->readmem(0xFF25, true) & 0x1; }

void Audio::setSoundOutputSelection(uint8_t val) { memory->writemem(val, 0xFF25, true); }

void Audio::setChannel4LeftOutput(uint8_t val) { memory->writebit(val, 7, 0xFF25, true); }

void Audio::setChannel3LeftOutput(uint8_t val) { memory->writebit(val, 6, 0xFF25, true); }

void Audio::setChannel2LeftOutput(uint8_t val) { memory->writebit(val, 5, 0xFF25, true); }

void Audio::setChannel1LeftOutput(uint8_t val) { memory->writebit(val, 4, 0xFF25, true); }

void Audio::setChannel4RightOutput(uint8_t val) { memory->writebit(val, 3, 0xFF25, true); }

void Audio::setChannel3RightOutput(uint8_t val) { memory->writebit(val, 2, 0xFF25, true); }

void Audio::setChannel2RightOutput(uint8_t val) { memory->writebit(val, 1, 0xFF25, true); }

void Audio::setChannel1RightOutput(uint8_t val) { memory->writebit(val, 0, 0xFF25, true); }

/* NR52 - 0xFF26 */

uint8_t Audio::getSoundOn() { return memory->readmem(0xFF26, true); }

uint8_t Audio::getAllSoundOn() { return (memory->readmem(0xFF26, true) & 0x80) >> 7; }

uint8_t Audio::getChannel4SoundOn() { return (memory->readmem(0xFF26, true) & 0x8) >> 3; }

uint8_t Audio::getChannel3SoundOn() { return (memory->readmem(0xFF26, true) & 0x4) >> 2; }

uint8_t Audio::getChannel2SoundOn() { return (memory->readmem(0xFF26, true) & 0x2) >> 1; }

uint8_t Audio::getChannel1SoundOn() { return memory->readmem(0xFF26, true) & 0x1; }

void Audio::setSoundOn(uint8_t val) { memory->writemem(val, 0xFF26, true); }

void Audio::setAllSoundOn(uint8_t val) { memory->writebit(val, 7, 0xFF26, true); }

void Audio::setChannel4SoundOn(uint8_t val) { memory->writebit(val, 3, 0xFF26, true); }

void Audio::setChannel3SoundOn(uint8_t val) { memory->writebit(val, 2, 0xFF26, true); }

void Audio::setChannel2SoundOn(uint8_t val) { memory->writebit(val, 1, 0xFF26, true); }

void Audio::setChannel1SoundOn(uint8_t val) { memory->writebit(val, 0, 0xFF26, true); }

void Audio::cycle()
{
    if (getAllSoundOn() != 0) {
        if (!initialInit) {
            initialInit = true;
            
            channel1.initCh();
            channel2.initCh();
            channel3.initCh();
            channel4.initCh();
        }

        channel1.cycle();
        channel2.cycle();
        channel3.cycle();
        channel4.cycle();
    }

    ++currentCyclesUntilSampleCollection;
    if (currentCyclesUntilSampleCollection == AUDIO_CYCLES_UNTIL_SAMPLE_COLLECTION) {
        currentCyclesUntilSampleCollection = 0;

        float mixedSample = 0;
        float aux;

        // Left Channel
        int vol = (getLeftChannelVolume() * SDL_MIX_MAXVOLUME) / 7;

        if (getChannel1LeftOutput()) {
            aux = ((float)channel1.outputVolume) / SDL_MIX_MAXVOLUME;
            SDL_MixAudioFormat((uint8_t *)&mixedSample, (uint8_t *)&aux, sdlAudioFormat,
                               sizeof(float), vol);
        }

        if (getChannel2LeftOutput()) {
            aux = ((float)channel2.outputVolume) / SDL_MIX_MAXVOLUME;
            SDL_MixAudioFormat((uint8_t *)&mixedSample, (uint8_t *)&aux, sdlAudioFormat,
                               sizeof(float), vol);
        }

        if (getChannel3LeftOutput()) {
            aux = ((float)channel3.outputVolume) / SDL_MIX_MAXVOLUME;
            SDL_MixAudioFormat((uint8_t *)&mixedSample, (uint8_t *)&aux, sdlAudioFormat,
                               sizeof(float), vol);
        }

        if (getChannel4LeftOutput()) {
            aux = ((float)channel4.outputVolume) / SDL_MIX_MAXVOLUME;
            SDL_MixAudioFormat((uint8_t *)&mixedSample, (uint8_t *)&aux, sdlAudioFormat,
                               sizeof(float), vol);
        }

        audioBuffer[currentAudioSamples++] = mixedSample;

        // Right Channel
        mixedSample = 0;
        vol = (getRightChannelVolume() * SDL_MIX_MAXVOLUME) / 7;

        if (getChannel1RightOutput()) {
            aux = ((float)channel1.outputVolume) / SDL_MIX_MAXVOLUME;
            SDL_MixAudioFormat((uint8_t *)&mixedSample, (uint8_t *)&aux, sdlAudioFormat,
                               sizeof(float), vol);
        }

        if (getChannel2RightOutput()) {
            aux = ((float)channel2.outputVolume) / SDL_MIX_MAXVOLUME;
            SDL_MixAudioFormat((uint8_t *)&mixedSample, (uint8_t *)&aux, sdlAudioFormat,
                               sizeof(float), vol);
        }

        if (getChannel3RightOutput()) {
            aux = ((float)channel3.outputVolume) / SDL_MIX_MAXVOLUME;
            SDL_MixAudioFormat((uint8_t *)&mixedSample, (uint8_t *)&aux, sdlAudioFormat,
                               sizeof(float), vol);
        }

        if (getChannel4RightOutput()) {
            aux = ((float)channel4.outputVolume) / SDL_MIX_MAXVOLUME;
            SDL_MixAudioFormat((uint8_t *)&mixedSample, (uint8_t *)&aux, sdlAudioFormat,
                               sizeof(float), vol);
        }

        audioBuffer[currentAudioSamples++] = mixedSample;
    }

    if (currentAudioSamples == AUDIO_NUM_SAMPLES) {
        currentAudioSamples = 0;
        
        // queue audio
        SDL_QueueAudio(1, audioBuffer, AUDIO_NUM_SAMPLES * sizeof(float));
    }
}
