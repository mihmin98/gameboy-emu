#include "Timer.hpp"
#include "Memory.hpp"
#include "SM83.hpp"

Timer::Timer() : divCounter(0), timaTicks(0), timaSelectedBitPreviousValue(0)
{
    clockSelectTicks[0] = 1024;
    clockSelectTicks[1] = 16;
    clockSelectTicks[2] = 64;
    clockSelectTicks[3] = 256;

    clockSelectBitMask[0] = 1 << 9;
    clockSelectBitMask[1] = 1 << 3;
    clockSelectBitMask[2] = 1 << 5;
    clockSelectBitMask[3] = 1 << 7;

    timaReloadTCyclesDelay = -1;

    timaChangedDuringWait = false;
}

uint8_t Timer::getDividerRegister() { return memory->ioRegisters[0xFF04 - MEM_IO_START]; }

void Timer::setDividerRegister(uint8_t val)
{
    // Set it like this because otherwise it would always be set to 0
    memory->ioRegisters[0xFF04 - MEM_IO_START] = val;
}

void Timer::setDividerCounter(uint16_t val)
{
    divCounter = val;
    // setDividerRegister((divCounter & 0xFF00) >> 8);
    memory->ioRegisters[0xFF04 - MEM_IO_START] = (divCounter & 0xFF00) >> 8;
}

uint8_t Timer::getTimerCounter() { return memory->ioRegisters[0xFF05 - MEM_IO_START]; }

void Timer::setTimerCounter(uint8_t val, bool bypass)
{
    if (bypass)
        memory->ioRegisters[0xFF05 - MEM_IO_START] = val;
    else
        memory->writemem(val, 0xFF05);
}

uint8_t Timer::getTimerModulo() { return memory->ioRegisters[0xFF06 - MEM_IO_START]; }

void Timer::setTimerModulo(uint8_t val) { memory->writemem(val, 0xFF06); }

uint8_t Timer::getTimerControl() { return memory->ioRegisters[0xFF07 - MEM_IO_START] & 7; }

uint8_t Timer::getTimerEnable() { return (memory->ioRegisters[0xFF07 - MEM_IO_START] & 4) >> 2; }

uint8_t Timer::getInputClockSelect() { return memory->ioRegisters[0xFF07 - MEM_IO_START] & 3; }

void Timer::setTimerControl(uint8_t val) { memory->writemem(val, 0xFF07); }

void Timer::setTimerEnable(uint8_t val) { memory->writebit(val, 2, 0xFF07); }

void Timer::setInputClockSelect(uint8_t val)
{
    uint8_t tac = getTimerControl() & 0xFC;
    tac |= (val & 3);

    setTimerControl(tac);
}

void Timer::cycle()
{   
    
    // TODO: DIV should not tick after a stop instruction??
    ++divCounter;
    uint8_t div = (divCounter & 0xFF00) >> 8;
    setDividerRegister(div);

    uint8_t timerEnabled = getTimerEnable() != 0 ? 1 : 0;
    uint8_t timaSelectedBit = divCounter & clockSelectBitMask[getInputClockSelect()];
    timaSelectedBit = timaSelectedBit != 0 ? 1 : 0;
    timaSelectedBit &= timerEnabled;

    if (timaSelectedBit == 0 && timaSelectedBitPreviousValue == 1) {
        // Increase TIMA
        // check for overflow etc

        if (timaReloadTCyclesDelay != -1) {
            --timaReloadTCyclesDelay;
            if (timaReloadTCyclesDelay == 0) {
                // Reload TIMA and trigger INT
                if (!timaChangedDuringWait) {
                    cpu->setTimerInterruptFlag(1);
                    setTimerCounter(timaReloadValue, true);
                }

                timaReloadTCyclesDelay = -1;
            }
        }

        // Increase TIMA
        uint8_t tima = getTimerCounter();

        if (tima == 0xFF) {
            // TIMA Overflow
            setTimerCounter(0);
            timaReloadTCyclesDelay = TIMER_TIMA_RELOAD_T_CYCLES_DELAY;
            timaChangedDuringWait = false;
            timaReloadValue = getTimerModulo();
        } else {
            ++tima;
            if (timaReloadTCyclesDelay == -1) {
                // Do not change TIMA during reload
                setTimerCounter(tima);
            }
        }
    }

    timaSelectedBitPreviousValue = timaSelectedBit;
}
