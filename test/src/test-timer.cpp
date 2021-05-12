#include "Memory.hpp"
#include "PPU.hpp"
#include "SM83.hpp"
#include "Timer.hpp"
#include "catch.hpp"

TEST_CASE("DIV", "[TIMER]")
{
    Timer timer;
    PPU ppu;
    Memory mem;
    SM83 cpu;

    timer.cpu = &cpu;
    timer.memory = &mem;
    ppu.cpu = &cpu;
    ppu.memory = &mem;
    mem.ppu = &ppu;
    cpu.memory = &mem;

    SECTION("Get")
    {
        mem.ioRegisters[0xFF04 - MEM_IO_START] = 0x66;
        REQUIRE(timer.getDividerRegister() == 0x66);
    }

    SECTION("Set")
    {
        mem.ioRegisters[0xFF04 - MEM_IO_START] = 0;
        timer.setDividerRegister(0x66);
        REQUIRE(mem.ioRegisters[0xFF04 - MEM_IO_START] == 0x66);
    }

    SECTION("Write to reset")
    {
        mem.ioRegisters[0xFF04 - MEM_IO_START] = 0x66;
        REQUIRE(mem.ioRegisters[0xFF04 - MEM_IO_START] == 0x66);
        mem.writemem(0x66, 0xFF04);
        REQUIRE(mem.ioRegisters[0xFF04 - MEM_IO_START] == 0);
    }

    SECTION("Cycle")
    {
        timer.setDividerRegister(127);

        for (uint i = timer.getDividerRegister(); i < 256; ++i) {
            // 256 ticks for an increase
            for (uint j = 0; j < 256; ++j) {
                timer.cycle();
            }
            REQUIRE(timer.getDividerRegister() == ((i + 1) & 0xFF));
        }

        REQUIRE(timer.getDividerRegister() == 0);
    }

    SECTION("STOP Instruction")
    {
        // TODO
    }
}

TEST_CASE("TIMA", "[TIMER]")
{
    Timer timer;
    PPU ppu;
    Memory mem;
    SM83 cpu;

    timer.cpu = &cpu;
    timer.memory = &mem;
    ppu.cpu = &cpu;
    ppu.memory = &mem;
    mem.ppu = &ppu;
    mem.timer = &timer;
    cpu.memory = &mem;

    timer.setTimerEnable(1);

    SECTION("Get")
    {
        mem.ioRegisters[0xFF05 - MEM_IO_START] = 0x66;
        REQUIRE(timer.getTimerCounter() == 0x66);
    }

    SECTION("Set")
    {
        mem.ioRegisters[0xFF05 - MEM_IO_START] = 0;
        timer.setTimerCounter(0x66);
        REQUIRE(mem.ioRegisters[0xFF05 - MEM_IO_START] == 0x66);
    }

    SECTION("Cycle")
    {
        SECTION("Normal Cycle")
        {
            timer.setTimerCounter(100);
            timer.setInputClockSelect(1); // 16 ticks

            for (uint i = 0; i < 16; ++i) {
                timer.cycle();

                if (i != 15)
                    REQUIRE(timer.getTimerCounter() == 100);
            }

            REQUIRE(timer.getTimerCounter() == 101);
        }

        SECTION("Overflow")
        {
            timer.setTimerCounter(255);
            timer.setInputClockSelect(1); // 16 ticks
            timer.setTimerModulo(100);
            cpu.setTimerInterruptFlag(0);

            for (uint i = 0; i < 16; ++i) {
                REQUIRE(timer.getTimerCounter() == 255);
                timer.cycle();
            }

            REQUIRE(timer.getTimerCounter() == 0);

            SECTION("Regular Overflow")
            {
                // 4 cycles of waiting
                for (uint i = 0; i < 4; ++i) {
                    REQUIRE(timer.getTimerCounter() == 0);
                    timer.cycle();
                }

                REQUIRE(timer.getTimerCounter() == 100);
                REQUIRE(cpu.getTimerInterruptFlag() == 1);
            }

            SECTION("Prevent TIMA Reload and Interrupt Trigger") 
            {
                // when a value is written to TIMA during delay
                timer.setTimerCounter(10);

                // 4 cycles of waiting
                for (uint i = 0; i < 4; ++i) {
                    timer.cycle();
                }

                REQUIRE(timer.getTimerCounter() == 10);
                REQUIRE(cpu.getTimerInterruptFlag() == 0);
            }

            SECTION("Ignored TIMA Write")
            {
                for (uint i = 0; i < 3; ++i) {
                    timer.cycle();
                }

                timer.setTimerCounter(66);
                timer.cycle();

                REQUIRE(timer.getTimerCounter() == 100);
                REQUIRE(cpu.getTimerInterruptFlag() == 1);
            }

            SECTION("Write TMA on last wait cycle")
            {
                for (uint i = 0; i < 3; ++i) {
                    timer.cycle();
                }

                timer.setTimerModulo(66);
                timer.cycle();

                REQUIRE(timer.getTimerCounter() == 66);
                REQUIRE(cpu.getTimerInterruptFlag() == 1);
            }
        }
    }
}

TEST_CASE("TMA", "[TIMER]")
{
    Timer timer;
    PPU ppu;
    Memory mem;
    SM83 cpu;

    timer.cpu = &cpu;
    timer.memory = &mem;
    ppu.cpu = &cpu;
    ppu.memory = &mem;
    mem.ppu = &ppu;
    mem.timer = &timer;
    cpu.memory = &mem;

    SECTION("Get")
    {
        mem.ioRegisters[0xFF06 - MEM_IO_START] = 0x66;
        REQUIRE(timer.getTimerModulo() == 0x66);
    }

    SECTION("Set")
    {
        mem.ioRegisters[0xFF06 - MEM_IO_START] = 0;
        timer.setTimerModulo(0x66);
        REQUIRE(mem.ioRegisters[0xFF06 - MEM_IO_START] == 0x66);
    }
}

TEST_CASE("TAC", "[TIMER]")
{
    Timer timer;
    PPU ppu;
    Memory mem;
    SM83 cpu;

    timer.cpu = &cpu;
    timer.memory = &mem;
    ppu.cpu = &cpu;
    ppu.memory = &mem;
    mem.ppu = &ppu;
    cpu.memory = &mem;

    SECTION("Timer Control")
    {
        SECTION("Get")
        {
            mem.ioRegisters[0xFF07 - MEM_IO_START] = 0x66;
            REQUIRE(timer.getTimerControl() == 0x66);
        }

        SECTION("Set")
        {
            mem.ioRegisters[0xFF07 - MEM_IO_START] = 0;
            timer.setTimerControl(0x66);
            REQUIRE(mem.ioRegisters[0xFF07 - MEM_IO_START] == 0x66);
        }
    }

    SECTION("Timer Enable")
    {
        SECTION("Get")
        {
            mem.ioRegisters[0xFF07 - MEM_IO_START] = 0x6;
            REQUIRE(timer.getTimerEnable() == 1);

            mem.ioRegisters[0xFF07 - MEM_IO_START] = 0x2;
            REQUIRE(timer.getTimerEnable() == 0);
        }

        SECTION("Set")
        {
            mem.ioRegisters[0xFF07 - MEM_IO_START] = 0x2;
            timer.setTimerEnable(1);
            REQUIRE(mem.ioRegisters[0xFF07 - MEM_IO_START] == 0x6);

            mem.ioRegisters[0xFF07 - MEM_IO_START] = 0x6;
            timer.setTimerEnable(0);
            REQUIRE(mem.ioRegisters[0xFF07 - MEM_IO_START] == 0x2);
        }
    }

    SECTION("Input Clock Select")
    {
        SECTION("Get")
        {
            mem.ioRegisters[0xFF07 - MEM_IO_START] = 0x6;
            REQUIRE(timer.getInputClockSelect() == 2);

            mem.ioRegisters[0xFF07 - MEM_IO_START] = 0x7;
            REQUIRE(timer.getInputClockSelect() == 3);
        }

        SECTION("Set")
        {
            mem.ioRegisters[0xFF07 - MEM_IO_START] = 0x4;
            timer.setInputClockSelect(3);
            REQUIRE(mem.ioRegisters[0xFF07 - MEM_IO_START] == 0x7);

            timer.setInputClockSelect(1);
            REQUIRE(mem.ioRegisters[0xFF07 - MEM_IO_START] == 0x5);
        }
    }
}
