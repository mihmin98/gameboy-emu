#ifndef __TIMER_H__
#define __TIMER_H__

#pragma once
#include <cstdint>

#define TIMER_TIMA_RELOAD_T_CYCLES_DELAY 4

class SM83;
class Memory;

class Timer
{
  public:
    Timer();

    Memory *memory;
    SM83 *cpu;

    uint16_t divCounter;
    uint16_t timaTicks;
    uint8_t timaSelectedBitPreviousValue;

    uint8_t tmaPreviousValue;
    uint8_t timaReloadValue;

    // These are -1 when they are not being used
    int8_t timaReloadTCyclesDelay;
    bool timaChangedDuringWait;

    uint16_t clockSelectTicks[4];
    uint16_t clockSelectBitMask[4];

    void cycle();

    // DIV - 0xFF04
    uint8_t getDividerRegister();
    void setDividerRegister(uint8_t val);
    void setDividerCounter(uint16_t val);

    // TIMA - 0xFF05
    uint8_t getTimerCounter();
    void setTimerCounter(uint8_t val, bool bypass = false);

    // TMA - 0xFF06
    uint8_t getTimerModulo();
    void setTimerModulo(uint8_t val);

    // TAC - 0xFF07
    uint8_t getTimerControl();
    uint8_t getTimerEnable();
    uint8_t getInputClockSelect();
    void setTimerControl(uint8_t val);
    void setTimerEnable(uint8_t val);
    void setInputClockSelect(uint8_t val);
};

#endif // __TIMER_H__
