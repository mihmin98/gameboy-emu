#include "catch.hpp"

#include "Memory.hpp"
#include "SM83.hpp"
#include <iostream>

TEST_CASE("Cycle", "[SM83]")
{
    SM83 cpu;
    Memory mem;
    cpu.memory = &mem;

    cpu.PC = 0xC000;
    cpu.SP = 0xC108;
    cpu.instructionCycle = 0;
    cpu.halted = false;
    cpu.halt_bug = false;
    cpu.just_started_halt_bug = false;

    SECTION("Regular case")
    {
        // Add A, 0x66
        mem.wram[0] = 0xC6;
        mem.wram[1] = 0x66;

        cpu.cycle();
        cpu.cycle();

        REQUIRE(cpu.PC == 0xC002);
        REQUIRE(cpu.A == 0x66);
    }

    SECTION("Check Halted")
    {
        SECTION("IME Enabled")
        {
            cpu.ime = 1;
            cpu.setInterruptEnable(0);
            cpu.setInterruptFlag(0);

            // Halt
            mem.wram[0] = 0x76;

            cpu.cycle();
            REQUIRE(cpu.halted == true);
            REQUIRE(cpu.PC == 0xC001);

            cpu.cycle();
            REQUIRE(cpu.halted == true);
            REQUIRE(cpu.PC == 0xC001);

            // Trigger Interrupt
            cpu.setVBlankInterruptEnable(1);
            cpu.setVBlankInterruptFlag(1);

            cpu.cycle();
            REQUIRE(cpu.int_cycles == 4);
            REQUIRE(cpu.int_addr == SM83_VBLANK_INT);
            REQUIRE(cpu.getVBlankInterruptFlag() == 0);

            cpu.cycle();
            REQUIRE(cpu.int_cycles == 3);

            cpu.cycle();
            REQUIRE(cpu.int_cycles == 2);

            cpu.cycle();
            REQUIRE(cpu.int_cycles == 1);

            cpu.cycle();
            REQUIRE(cpu.int_cycles == -1);
            REQUIRE(cpu.ime == 0);
            REQUIRE(cpu.PC == SM83_VBLANK_INT);
            REQUIRE(cpu.SP == 0xC106);
            REQUIRE(mem.wram[0x107] == 0xC0);
            REQUIRE(mem.wram[0x106] == 0x01);
        }

        SECTION("IME Disabled")
        {
            cpu.ime = 0;

            SECTION("No pending interrupts (no bug)")
            {
                cpu.setInterruptEnable(0);
                cpu.setInterruptFlag(0);

                // Halt
                mem.wram[0] = 0x76;

                cpu.cycle();
                REQUIRE(cpu.halted == true);
                REQUIRE(cpu.PC == 0xC001);

                cpu.cycle();
                REQUIRE(cpu.halted == true);
                REQUIRE(cpu.PC == 0xC001);

                // Trigger Interrupt
                cpu.setVBlankInterruptEnable(1);
                cpu.setVBlankInterruptFlag(1);

                cpu.cycle();
                REQUIRE(cpu.int_cycles == 3);
                REQUIRE(cpu.getVBlankInterruptFlag() == 1);

                cpu.cycle();
                REQUIRE(cpu.int_cycles == 2);

                cpu.cycle();
                REQUIRE(cpu.int_cycles == 1);

                cpu.cycle();
                REQUIRE(cpu.int_cycles == -1);
                REQUIRE(cpu.PC == 0xC001);
            }

            SECTION("Pending interrupts (bug)")
            {
                cpu.A = 0;
                cpu.D = 0;

                // Halt
                mem.wram[0] = 0x76;

                // LD A, 14
                mem.wram[1] = 0x3E;
                mem.wram[2] = 0x14;

                // The bug will execute after the halt LD A, 3E; INC D;

                cpu.setInterruptEnable(0);
                cpu.setInterruptFlag(0);

                // Trigger Interrupt
                cpu.setVBlankInterruptEnable(1);
                cpu.setVBlankInterruptFlag(1);

                cpu.cycle();
                REQUIRE(cpu.halted == false);
                REQUIRE(cpu.halt_bug == true);
                REQUIRE(cpu.getVBlankInterruptFlag() == 1);
                REQUIRE(cpu.PC == 0xC001);

                // Now it should execute LD A, 14 but it will execute LD A, 3E
                cpu.cycle();
                cpu.cycle();
                REQUIRE(cpu.halt_bug == false);
                REQUIRE(cpu.PC == 0xC002);
                REQUIRE(cpu.A == 0x3E);

                // Now it will execute INC D
                cpu.cycle();
                REQUIRE(cpu.PC == 0xC003);
                REQUIRE(cpu.D == 1);
            }
        }
    }

    SECTION("Check Interrupts")
    {
        // Add some NOPs
        mem.wram[0] = 0x0;
        mem.wram[1] = 0x0;

        cpu.ime = 1;
        cpu.setInterruptEnable(0);
        cpu.setInterruptFlag(0);

        SECTION("VBlank Int")
        {
            cpu.setVBlankInterruptFlag(1);

            cpu.cycle();
            REQUIRE(cpu.PC == 0xC001);

            cpu.setVBlankInterruptEnable(1);

            cpu.cycle();
            REQUIRE(cpu.int_cycles == 4);
            REQUIRE(cpu.int_addr == SM83_VBLANK_INT);
            REQUIRE(cpu.getVBlankInterruptFlag() == 0);

            cpu.cycle();
            REQUIRE(cpu.int_cycles == 3);

            cpu.cycle();
            REQUIRE(cpu.int_cycles == 2);

            cpu.cycle();
            REQUIRE(cpu.int_cycles == 1);

            cpu.cycle();
            REQUIRE(cpu.int_cycles == -1);
            REQUIRE(cpu.PC == SM83_VBLANK_INT);
        }

        SECTION("LCD STAT Int")
        {
            cpu.setLCDSTATInterruptFlag(1);

            cpu.cycle();
            REQUIRE(cpu.PC == 0xC001);

            cpu.setLCDSTATInterruptEnable(1);

            cpu.cycle();
            REQUIRE(cpu.int_cycles == 4);
            REQUIRE(cpu.int_addr == SM83_LCD_STAT_INT);
            REQUIRE(cpu.getLCDSTATInterruptFlag() == 0);

            cpu.cycle();
            REQUIRE(cpu.int_cycles == 3);

            cpu.cycle();
            REQUIRE(cpu.int_cycles == 2);

            cpu.cycle();
            REQUIRE(cpu.int_cycles == 1);

            cpu.cycle();
            REQUIRE(cpu.int_cycles == -1);
            REQUIRE(cpu.PC == SM83_LCD_STAT_INT);
        }

        SECTION("Timer Int")
        {
            cpu.setTimerInterruptFlag(1);

            cpu.cycle();
            REQUIRE(cpu.PC == 0xC001);

            cpu.setTimerInterruptEnable(1);

            cpu.cycle();
            REQUIRE(cpu.int_cycles == 4);
            REQUIRE(cpu.int_addr == SM83_TIMER_INT);
            REQUIRE(cpu.getTimerInterruptFlag() == 0);

            cpu.cycle();
            REQUIRE(cpu.int_cycles == 3);

            cpu.cycle();
            REQUIRE(cpu.int_cycles == 2);

            cpu.cycle();
            REQUIRE(cpu.int_cycles == 1);

            cpu.cycle();
            REQUIRE(cpu.int_cycles == -1);
            REQUIRE(cpu.PC == SM83_TIMER_INT);
        }

        SECTION("Serial Int")
        {
            cpu.setSerialInterruptFlag(1);

            cpu.cycle();
            REQUIRE(cpu.PC == 0xC001);

            cpu.setSerialInterruptEnable(1);

            cpu.cycle();
            REQUIRE(cpu.int_cycles == 4);
            REQUIRE(cpu.int_addr == SM83_SERIAL_INT);
            REQUIRE(cpu.getSerialInterruptFlag() == 0);

            cpu.cycle();
            REQUIRE(cpu.int_cycles == 3);

            cpu.cycle();
            REQUIRE(cpu.int_cycles == 2);

            cpu.cycle();
            REQUIRE(cpu.int_cycles == 1);

            cpu.cycle();
            REQUIRE(cpu.int_cycles == -1);
            REQUIRE(cpu.PC == SM83_SERIAL_INT);
        }

        SECTION("Joypad Int")
        {
            cpu.setJoypadInterruptFlag(1);

            cpu.cycle();
            REQUIRE(cpu.PC == 0xC001);

            cpu.setJoypadInterruptEnable(1);

            cpu.cycle();
            REQUIRE(cpu.int_cycles == 4);
            REQUIRE(cpu.int_addr == SM83_JOYPAD_INT);
            REQUIRE(cpu.getJoypadInterruptFlag() == 0);

            cpu.cycle();
            REQUIRE(cpu.int_cycles == 3);

            cpu.cycle();
            REQUIRE(cpu.int_cycles == 2);

            cpu.cycle();
            REQUIRE(cpu.int_cycles == 1);

            cpu.cycle();
            REQUIRE(cpu.int_cycles == -1);
            REQUIRE(cpu.PC == SM83_JOYPAD_INT);
        }
    }
}

TEST_CASE("Handle Interrutps", "[SM83]")
{
    SM83 cpu;
    Memory mem;
    cpu.memory = &mem;

    cpu.setInterruptEnable(0);
    cpu.setInterruptFlag(0);

    cpu.setVBlankInterruptEnable(1);
    cpu.setVBlankInterruptFlag(1);

    cpu.int_cycles = -1;
    cpu.int_addr = 0;

    SECTION("IME Disabled")
    {
        cpu.ime = 0;

        cpu.handleInterrupts();

        REQUIRE(cpu.int_cycles == -1);
        REQUIRE(cpu.int_addr == 0);
    }

    SECTION("IME Enabled")
    {
        cpu.ime = 1;

        cpu.handleInterrupts();

        REQUIRE(cpu.int_cycles == 5);
        REQUIRE(cpu.int_addr == SM83_VBLANK_INT);
    }
}

TEST_CASE("Check Interrupts", "[SM83]")
{
    SM83 cpu;
    Memory mem;
    cpu.memory = &mem;

    cpu.int_cycles = -1;
    cpu.int_addr = 0;

    cpu.setInterruptEnable(0);
    cpu.setInterruptFlag(0);

    SECTION("VBlank")
    {
        cpu.setVBlankInterruptFlag(1);

        REQUIRE(cpu.checkInterrupts(&cpu.int_cycles, &cpu.int_addr) == false);

        cpu.setVBlankInterruptEnable(1);

        cpu.ime = 1;

        REQUIRE(cpu.checkInterrupts(&cpu.int_cycles, &cpu.int_addr) == true);
        REQUIRE(cpu.getVBlankInterruptFlag() == 0);
        REQUIRE(cpu.int_cycles == 5);
        REQUIRE(cpu.int_addr == SM83_VBLANK_INT);

        cpu.ime = 0;
        cpu.setVBlankInterruptFlag(1);

        REQUIRE(cpu.checkInterrupts(&cpu.int_cycles, nullptr) == true);
        REQUIRE(cpu.getVBlankInterruptFlag() == 1);
        REQUIRE(cpu.int_cycles == 4);
    }

    SECTION("LCD STAT")
    {
        cpu.setLCDSTATInterruptFlag(1);

        REQUIRE(cpu.checkInterrupts(&cpu.int_cycles, &cpu.int_addr) == false);

        cpu.setLCDSTATInterruptEnable(1);

        cpu.ime = 1;

        REQUIRE(cpu.checkInterrupts(&cpu.int_cycles, &cpu.int_addr) == true);
        REQUIRE(cpu.getLCDSTATInterruptFlag() == 0);
        REQUIRE(cpu.int_cycles == 5);
        REQUIRE(cpu.int_addr == SM83_LCD_STAT_INT);

        cpu.ime = 0;
        cpu.setLCDSTATInterruptFlag(1);

        REQUIRE(cpu.checkInterrupts(&cpu.int_cycles, nullptr) == true);
        REQUIRE(cpu.getLCDSTATInterruptFlag() == 1);
        REQUIRE(cpu.int_cycles == 4);
    }

    SECTION("Timer")
    {
        cpu.setTimerInterruptFlag(1);

        REQUIRE(cpu.checkInterrupts(&cpu.int_cycles, &cpu.int_addr) == false);

        cpu.setTimerInterruptEnable(1);

        cpu.ime = 1;

        REQUIRE(cpu.checkInterrupts(&cpu.int_cycles, &cpu.int_addr) == true);
        REQUIRE(cpu.getTimerInterruptFlag() == 0);
        REQUIRE(cpu.int_cycles == 5);
        REQUIRE(cpu.int_addr == SM83_TIMER_INT);

        cpu.ime = 0;
        cpu.setTimerInterruptFlag(1);

        REQUIRE(cpu.checkInterrupts(&cpu.int_cycles, nullptr) == true);
        REQUIRE(cpu.getTimerInterruptFlag() == 1);
        REQUIRE(cpu.int_cycles == 4);
    }

    SECTION("Serial")
    {
        cpu.setSerialInterruptFlag(1);

        REQUIRE(cpu.checkInterrupts(&cpu.int_cycles, &cpu.int_addr) == false);

        cpu.setSerialInterruptEnable(1);

        cpu.ime = 1;

        REQUIRE(cpu.checkInterrupts(&cpu.int_cycles, &cpu.int_addr) == true);
        REQUIRE(cpu.getSerialInterruptFlag() == 0);
        REQUIRE(cpu.int_cycles == 5);
        REQUIRE(cpu.int_addr == SM83_SERIAL_INT);

        cpu.ime = 0;
        cpu.setSerialInterruptFlag(1);

        REQUIRE(cpu.checkInterrupts(&cpu.int_cycles, nullptr) == true);
        REQUIRE(cpu.getSerialInterruptFlag() == 1);
        REQUIRE(cpu.int_cycles == 4);
    }

    SECTION("Joypad")
    {
        cpu.setJoypadInterruptFlag(1);

        REQUIRE(cpu.checkInterrupts(&cpu.int_cycles, &cpu.int_addr) == false);

        cpu.setJoypadInterruptEnable(1);

        cpu.ime = 1;

        REQUIRE(cpu.checkInterrupts(&cpu.int_cycles, &cpu.int_addr) == true);
        REQUIRE(cpu.getJoypadInterruptFlag() == 0);
        REQUIRE(cpu.int_cycles == 5);
        REQUIRE(cpu.int_addr == SM83_JOYPAD_INT);

        cpu.ime = 0;
        cpu.setJoypadInterruptFlag(1);

        REQUIRE(cpu.checkInterrupts(&cpu.int_cycles, nullptr) == true);
        REQUIRE(cpu.getJoypadInterruptFlag() == 1);
        REQUIRE(cpu.int_cycles == 4);
    }
}

TEST_CASE("Service Interrupt", "[SM83]")
{
    SM83 cpu;
    Memory mem;
    cpu.memory = &mem;

    cpu.int_cycles = 5;
    cpu.int_addr = SM83_VBLANK_INT;

    cpu.PC = 0xC001;
    cpu.SP = 0xC008;

    mem.wram[6] = 0x0;
    mem.wram[7] = 0x0;

    SECTION("IME Enabled")
    {
        cpu.ime = 1;

        cpu.serviceInterrupt();
        REQUIRE(cpu.int_cycles == 4);

        cpu.serviceInterrupt();
        REQUIRE(cpu.int_cycles == 3);

        cpu.serviceInterrupt();
        REQUIRE(cpu.int_cycles == 2);

        cpu.serviceInterrupt();
        REQUIRE(cpu.int_cycles == 1);

        cpu.serviceInterrupt();
        REQUIRE(cpu.int_cycles == -1);

        REQUIRE(cpu.ime == 0);
        REQUIRE(mem.wram[7] == 0xC0);
        REQUIRE(mem.wram[6] == 0x01);
        REQUIRE(cpu.PC == SM83_VBLANK_INT);
    }

    SECTION("IME Disabled")
    {
        cpu.ime = 0;

        cpu.serviceInterrupt();
        REQUIRE(cpu.int_cycles == 4);

        cpu.serviceInterrupt();
        REQUIRE(cpu.int_cycles == 3);

        cpu.serviceInterrupt();
        REQUIRE(cpu.int_cycles == 2);

        cpu.serviceInterrupt();
        REQUIRE(cpu.int_cycles == 1);

        cpu.serviceInterrupt();
        REQUIRE(cpu.int_cycles == -1);

        REQUIRE(cpu.PC == 0xC001);
        REQUIRE(mem.wram[7] == 0x0);
        REQUIRE(mem.wram[6] == 0x0);
    }
}

TEST_CASE("Read Memory", "[SM83]")
{
    SM83 cpu;
    Memory mem;
    cpu.memory = &mem;

    mem.wram[0] = 0x12;
    mem.wram[1] = 0x34;

    SECTION("1 byte") { REQUIRE(cpu.readmem_u8(0xC000) == 0x12); }

    SECTION("2 bytes") { REQUIRE(cpu.readmem_u16(0xC000) == 0x3412); }
}

TEST_CASE("Write Memory", "[SM83]")
{
    SM83 cpu;
    Memory mem;
    cpu.memory = &mem;

    SECTION("1 byte")
    {
        cpu.writemem_u8(0x12, 0xC000);
        REQUIRE(mem.wram[0] == 0x12);
    }

    SECTION("2 bytes")
    {
        cpu.writemem_u16(0x1234, 0xC000);
        REQUIRE(mem.wram[0] == 0x34);
        REQUIRE(mem.wram[1] == 0x12);
    }
}

TEST_CASE("Check Instruction Cycle", "[SM83]")
{
    SM83 cpu;

    cpu.instructionCycle = 0;

    REQUIRE(cpu.checkInstructionCycle(3) == false);
    REQUIRE(cpu.checkInstructionCycle(3) == false);
    REQUIRE(cpu.checkInstructionCycle(3) == true);

    cpu.instructionCycle = 5;
    REQUIRE(cpu.checkInstructionCycle(3) == true);
}

TEST_CASE("End Instruction", "[SM83]")
{
    SM83 cpu;

    cpu.instructionCycle = 4;
    cpu.PC = 0x0150;

    cpu.endInstruction(3);

    REQUIRE(cpu.instructionCycle == 0);
    REQUIRE(cpu.PC == 0x0153);
}

TEST_CASE("Get Flags", "[SM83]")
{
    SM83 cpu;
    cpu.F = 0x0;

    SECTION("Zero Flag")
    {
        cpu.F = 0x80;
        REQUIRE(cpu.getZeroFlag() == 1);
    }

    SECTION("Subtract Flag")
    {
        cpu.F = 0x40;
        REQUIRE(cpu.getSubtractFlag() == 1);
    }

    SECTION("Half Carry Flag")
    {
        cpu.F = 0x20;
        REQUIRE(cpu.getHalfCarryFlag() == 1);
    }

    SECTION("Carry Flag")
    {
        cpu.F = 0x10;
        REQUIRE(cpu.getCarryFlag() == 1);
    }
}

TEST_CASE("Set Flags", "[SM83]")
{
    SM83 cpu;
    cpu.F = 0x0;

    SECTION("Zero Flag")
    {
        cpu.setZeroFlag(1);
        REQUIRE(cpu.F == 0x80);
    }

    SECTION("Subtract Flag")
    {
        cpu.setSubtractFlag(1);
        REQUIRE(cpu.F == 0x40);
    }

    SECTION("Half Carry Flag")
    {
        cpu.setHalfCarryFlag(1);
        REQUIRE(cpu.F == 0x20);
    }

    SECTION("Carry Flag")
    {
        cpu.setCarryFlag(1);
        REQUIRE(cpu.F == 0x10);
    }
}

TEST_CASE("Get Interrupt Enable Flags", "[SM83]")
{
    SM83 cpu;
    Memory mem;
    cpu.memory = &mem;

    mem.ieRegister = 0;

    SECTION("VBlank")
    {
        mem.ieRegister = 0x1;
        REQUIRE(cpu.getVBlankInterruptEnable() == 1);
    }

    SECTION("LCD STAT")
    {
        mem.ieRegister = 0x2;
        REQUIRE(cpu.getLCDSTATInterruptEnable() == 1);
    }

    SECTION("Timer")
    {
        mem.ieRegister = 0x4;
        REQUIRE(cpu.getTimerInterruptEnable() == 1);
    }

    SECTION("Serial")
    {
        mem.ieRegister = 0x8;
        REQUIRE(cpu.getSerialInterruptEnable() == 1);
    }

    SECTION("Joypad")
    {
        mem.ieRegister = 0x10;
        REQUIRE(cpu.getJoypadInterruptEnable() == 1);
    }
}

TEST_CASE("Get Interrupt Flags", "[SM83]")
{
    SM83 cpu;
    Memory mem;
    cpu.memory = &mem;

    mem.writemem(0, 0xFF0F);

    SECTION("VBlank")
    {
        mem.writemem(0x1, 0xFF0F);
        REQUIRE(cpu.getVBlankInterruptFlag() == 1);
    }

    SECTION("LCD STAT")
    {
        mem.writemem(0x2, 0xFF0F);
        REQUIRE(cpu.getLCDSTATInterruptFlag() == 1);
    }

    SECTION("Timer")
    {
        mem.writemem(0x4, 0xFF0F);
        REQUIRE(cpu.getTimerInterruptFlag() == 1);
    }

    SECTION("Serial")
    {
        mem.writemem(0x8, 0xFF0F);
        REQUIRE(cpu.getSerialInterruptFlag() == 1);
    }

    SECTION("Joypad")
    {
        mem.writemem(0x10, 0xFF0F);
        REQUIRE(cpu.getJoypadInterruptFlag() == 1);
    }
}

TEST_CASE("Set Interrupt Enable Flags", "[SM83]")
{
    SM83 cpu;
    Memory mem;
    cpu.memory = &mem;

    mem.ieRegister = 0;

    SECTION("VBlank")
    {
        cpu.setVBlankInterruptEnable(1);
        REQUIRE(mem.ieRegister == 0x1);
    }

    SECTION("LCD STAT")
    {
        cpu.setLCDSTATInterruptEnable(1);
        REQUIRE(mem.ieRegister == 0x2);
    }

    SECTION("Timer")
    {
        cpu.setTimerInterruptEnable(1);
        REQUIRE(mem.ieRegister == 0x4);
    }

    SECTION("Serial")
    {
        cpu.setSerialInterruptEnable(1);
        REQUIRE(mem.ieRegister == 0x8);
    }

    SECTION("Joypad")
    {
        cpu.setJoypadInterruptEnable(1);
        REQUIRE(mem.ieRegister == 0x10);
    }
}

TEST_CASE("Set Interrupt Flags", "[SM83]")
{
    SM83 cpu;
    Memory mem;
    cpu.memory = &mem;

    mem.writemem(0, 0xFF0F);

    SECTION("VBlank")
    {
        cpu.setVBlankInterruptFlag(1);
        REQUIRE(mem.readmem(0xFF0F) == 0x1);
    }

    SECTION("LCD STAT")
    {
        cpu.setLCDSTATInterruptFlag(1);
        REQUIRE(mem.readmem(0xFF0F) == 0x2);
    }

    SECTION("Timer")
    {
        cpu.setTimerInterruptFlag(1);
        REQUIRE(mem.readmem(0xFF0F) == 0x4);
    }

    SECTION("Serial")
    {
        cpu.setSerialInterruptFlag(1);
        REQUIRE(mem.readmem(0xFF0F) == 0x8);
    }

    SECTION("Joypad")
    {
        cpu.setJoypadInterruptFlag(1);
        REQUIRE(mem.readmem(0xFF0F) == 0x10);
    }
}
