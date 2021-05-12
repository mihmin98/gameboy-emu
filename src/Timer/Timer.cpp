#include "Timer.hpp"
#include "Memory.hpp"
#include "SM83.hpp"

Timer::Timer() : divTicks(0), timaTicks(0)
{
    clockSelectTicks[0] = 1024;
    clockSelectTicks[1] = 16;
    clockSelectTicks[2] = 64;
    clockSelectTicks[3] = 256;

    timaReloadTCyclesDelay = -1;

    timaChangedDuringWait = false;
}

uint8_t Timer::getDividerRegister() { return memory->readmem(0xFF04); }

void Timer::setDividerRegister(uint8_t val)
{
    // Set it like this because otherwise it would always be set to 0
    memory->ioRegisters[0xFF04 - MEM_IO_START] = val;
}

uint8_t Timer::getTimerCounter() { return memory->readmem(0xFF05); }

void Timer::setTimerCounter(uint8_t val, bool bypass)
{
    if (bypass)
        memory->ioRegisters[0xFF05 - MEM_IO_START] = val;
    else
        memory->writemem(val, 0xFF05);
}

uint8_t Timer::getTimerModulo() { return memory->readmem(0xFF06); }

void Timer::setTimerModulo(uint8_t val) { memory->writemem(val, 0xFF06); }

uint8_t Timer::getTimerControl() { return memory->readmem(0xFF07); }

uint8_t Timer::getTimerEnable() { return (memory->readmem(0xFF07) & 4) >> 2; }

uint8_t Timer::getInputClockSelect() { return memory->readmem(0xFF07) & 3; }

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
    ++divTicks;
    if (divTicks == 256) {
        divTicks = 0;

        uint8_t div = getDividerRegister();
        ++div;
        setDividerRegister(div);
    }

    // TIMA
    if (getTimerEnable()) {
        ++timaTicks;

        if (timaReloadTCyclesDelay != -1) {
            --timaReloadTCyclesDelay;
            if (timaReloadTCyclesDelay == 0) {
                // Reload TIMA and trigger INT
                if (!timaChangedDuringWait) {
                    cpu->setTimerInterruptFlag(1);
                    setTimerCounter(timaReloadValue, true);
                }
            }
        }

        else if (timaTicks == clockSelectTicks[getInputClockSelect()]) {
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
                setTimerCounter(tima);
            }
        }
    }

    tmaPreviousValue = getTimerModulo();
}
