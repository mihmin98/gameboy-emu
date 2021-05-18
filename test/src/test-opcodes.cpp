#include "catch.hpp"

#include "GameBoy.hpp"
#include "Memory.hpp"
#include "PPU.hpp"
#include "ROM.hpp"
#include "SM83.hpp"
#include <cstdlib>
#include <iostream>

TEST_CASE("ADC A, r8", "[OPCODE]")
{
    SM83 cpu;

    cpu.PC = 0x0150;
    cpu.instructionCycle = 0;
    cpu.F = 0x0;

    SECTION("Check Zero Flag")
    {
        cpu.A = 0;
        cpu.B = 0;

        cpu.op_adc_a_r8(cpu.B);
        REQUIRE(cpu.PC == 0x0151);

        REQUIRE(cpu.A == 0);
        REQUIRE(cpu.getZeroFlag() == 1);
        REQUIRE(cpu.getSubtractFlag() == 0);
        REQUIRE(cpu.getHalfCarryFlag() == 0);
        REQUIRE(cpu.getCarryFlag() == 0);
    }

    SECTION("Check Half Carry Flag")
    {
        cpu.A = 0x8;
        cpu.B = 0x7;
        cpu.setCarryFlag(1);

        cpu.op_adc_a_r8(cpu.B);
        REQUIRE(cpu.PC == 0x0151);

        REQUIRE(cpu.A == 0x10);
        REQUIRE(cpu.getZeroFlag() == 0);
        REQUIRE(cpu.getSubtractFlag() == 0);
        REQUIRE(cpu.getHalfCarryFlag() == 1);
        REQUIRE(cpu.getCarryFlag() == 0);
    }

    SECTION("Check Carry Flag")
    {
        cpu.A = 0x88;
        cpu.B = 0x89;

        cpu.op_adc_a_r8(cpu.B);
        REQUIRE(cpu.PC == 0x0151);

        REQUIRE(cpu.A == 0x11);
        REQUIRE(cpu.getZeroFlag() == 0);
        REQUIRE(cpu.getSubtractFlag() == 0);
        REQUIRE(cpu.getHalfCarryFlag() == 1);
        REQUIRE(cpu.getCarryFlag() == 1);
    }
}

TEST_CASE("ADC A, [HL]", "[OPCODE]")
{
    SM83 cpu;
    Memory mem;
    PPU ppu;

    cpu.memory = &mem;
    mem.ppu = &ppu;
    ppu.memory = &mem;
    ppu.cpu = &cpu;

    cpu.PC = 0x0150;
    cpu.instructionCycle = 0;
    cpu.F = 0x0;

    // HL points to first addr in WRAM
    cpu.H = 0xC0;
    cpu.L = 0x00;

    SECTION("Check Zero Flag")
    {
        cpu.A = 0;
        mem.wram[0] = 0;

        cpu.op_adc_a_addr_hl();
        REQUIRE(cpu.instructionCycle == 1);
        cpu.op_adc_a_addr_hl();
        REQUIRE(cpu.instructionCycle == 0);
        REQUIRE(cpu.PC == 0x0151);

        REQUIRE(cpu.A == 0);
        REQUIRE(cpu.getZeroFlag() == 1);
        REQUIRE(cpu.getSubtractFlag() == 0);
        REQUIRE(cpu.getHalfCarryFlag() == 0);
        REQUIRE(cpu.getCarryFlag() == 0);
    }

    SECTION("Check Half Carry Flag")
    {
        cpu.A = 0x8;
        mem.wram[0] = 0x7;
        cpu.setCarryFlag(1);

        cpu.op_adc_a_addr_hl();
        REQUIRE(cpu.instructionCycle == 1);
        cpu.op_adc_a_addr_hl();
        REQUIRE(cpu.instructionCycle == 0);
        REQUIRE(cpu.PC == 0x0151);

        REQUIRE(cpu.A == 0x10);
        REQUIRE(cpu.getZeroFlag() == 0);
        REQUIRE(cpu.getSubtractFlag() == 0);
        REQUIRE(cpu.getHalfCarryFlag() == 1);
        REQUIRE(cpu.getCarryFlag() == 0);
    }

    SECTION("Check Carry Flag")
    {
        cpu.A = 0x88;
        mem.wram[0] = 0x89;

        cpu.op_adc_a_addr_hl();
        REQUIRE(cpu.instructionCycle == 1);
        cpu.op_adc_a_addr_hl();
        REQUIRE(cpu.instructionCycle == 0);
        REQUIRE(cpu.PC == 0x0151);

        REQUIRE(cpu.A == 0x11);
        REQUIRE(cpu.getZeroFlag() == 0);
        REQUIRE(cpu.getSubtractFlag() == 0);
        REQUIRE(cpu.getHalfCarryFlag() == 1);
        REQUIRE(cpu.getCarryFlag() == 1);
    }
}

TEST_CASE("ADC A, n8", "[OPCODE]")
{
    SM83 cpu;
    Memory mem;
    PPU ppu;

    cpu.memory = &mem;
    mem.ppu = &ppu;
    ppu.memory = &mem;
    ppu.cpu = &cpu;

    // We will use WRAM to store the opcode and immediate value
    cpu.PC = 0xC000;
    cpu.instructionCycle = 0;
    cpu.F = 0x0;

    SECTION("Check Zero Flag")
    {
        cpu.A = 0;
        mem.wram[1] = 0;

        cpu.op_adc_a_n8();
        REQUIRE(cpu.instructionCycle == 1);
        cpu.op_adc_a_n8();
        REQUIRE(cpu.instructionCycle == 0);
        REQUIRE(cpu.PC == 0xC002);

        REQUIRE(cpu.A == 0);
        REQUIRE(cpu.getZeroFlag() == 1);
        REQUIRE(cpu.getSubtractFlag() == 0);
        REQUIRE(cpu.getHalfCarryFlag() == 0);
        REQUIRE(cpu.getCarryFlag() == 0);
    }

    SECTION("Check Half Carry Flag")
    {
        cpu.A = 0x8;
        mem.wram[1] = 0x7;
        cpu.setCarryFlag(1);

        cpu.op_adc_a_n8();
        REQUIRE(cpu.instructionCycle == 1);
        cpu.op_adc_a_n8();
        REQUIRE(cpu.instructionCycle == 0);
        REQUIRE(cpu.PC == 0xC002);

        REQUIRE(cpu.A == 0x10);
        REQUIRE(cpu.getZeroFlag() == 0);
        REQUIRE(cpu.getSubtractFlag() == 0);
        REQUIRE(cpu.getHalfCarryFlag() == 1);
        REQUIRE(cpu.getCarryFlag() == 0);
    }

    SECTION("Check Carry Flag")
    {
        cpu.A = 0x88;
        mem.wram[1] = 0x89;

        cpu.op_adc_a_n8();
        REQUIRE(cpu.instructionCycle == 1);
        cpu.op_adc_a_n8();
        REQUIRE(cpu.instructionCycle == 0);
        REQUIRE(cpu.PC == 0xC002);

        REQUIRE(cpu.A == 0x11);
        REQUIRE(cpu.getZeroFlag() == 0);
        REQUIRE(cpu.getSubtractFlag() == 0);
        REQUIRE(cpu.getHalfCarryFlag() == 1);
        REQUIRE(cpu.getCarryFlag() == 1);
    }
}

TEST_CASE("ADD A, r8", "[OPCODE]")
{
    SM83 cpu;

    cpu.PC = 0x0150;
    cpu.instructionCycle = 0;
    cpu.F = 0x0;

    SECTION("Check Zero Flag")
    {
        cpu.A = 0;
        cpu.B = 0;

        cpu.op_add_a_r8(cpu.B);
        REQUIRE(cpu.PC == 0x0151);

        REQUIRE(cpu.A == 0x0);
        REQUIRE(cpu.getZeroFlag() == 1);
        REQUIRE(cpu.getSubtractFlag() == 0);
        REQUIRE(cpu.getHalfCarryFlag() == 0);
        REQUIRE(cpu.getCarryFlag() == 0);
    }

    SECTION("Check Half Carry Flag")
    {
        cpu.A = 0x8;
        cpu.B = 0x9;

        cpu.op_add_a_r8(cpu.B);
        REQUIRE(cpu.PC == 0x0151);

        REQUIRE(cpu.A == 0x11);
        REQUIRE(cpu.getZeroFlag() == 0);
        REQUIRE(cpu.getSubtractFlag() == 0);
        REQUIRE(cpu.getHalfCarryFlag() == 1);
        REQUIRE(cpu.getCarryFlag() == 0);
    }

    SECTION("Check Carry Flag")
    {
        cpu.A = 0x88;
        cpu.B = 0x89;

        cpu.op_add_a_r8(cpu.B);
        REQUIRE(cpu.PC == 0x0151);

        REQUIRE(cpu.A == 0x11);
        REQUIRE(cpu.getZeroFlag() == 0);
        REQUIRE(cpu.getSubtractFlag() == 0);
        REQUIRE(cpu.getHalfCarryFlag() == 1);
        REQUIRE(cpu.getCarryFlag() == 1);
    }
}

TEST_CASE("ADD A, [HL]", "[OPCODE]")
{
    SM83 cpu;
    Memory mem;
    PPU ppu;

    cpu.memory = &mem;
    mem.ppu = &ppu;
    ppu.memory = &mem;
    ppu.cpu = &cpu;

    cpu.PC = 0x0150;
    cpu.instructionCycle = 0;
    cpu.F = 0x0;

    // HL points to first addr in WRAM
    cpu.H = 0xC0;
    cpu.L = 0x00;

    SECTION("Check Zero Flag")
    {
        cpu.A = 0;
        mem.wram[0] = 0;

        cpu.op_add_a_addr_hl();
        REQUIRE(cpu.instructionCycle == 1);
        cpu.op_add_a_addr_hl();
        REQUIRE(cpu.instructionCycle == 0);
        REQUIRE(cpu.PC == 0x0151);

        REQUIRE(cpu.A == 0x0);
        REQUIRE(cpu.getZeroFlag() == 1);
        REQUIRE(cpu.getSubtractFlag() == 0);
        REQUIRE(cpu.getHalfCarryFlag() == 0);
        REQUIRE(cpu.getCarryFlag() == 0);
    }

    SECTION("Check Half Carry Flag")
    {
        cpu.A = 0x8;
        mem.wram[0] = 0x9;

        cpu.op_add_a_addr_hl();
        REQUIRE(cpu.instructionCycle == 1);
        cpu.op_add_a_addr_hl();
        REQUIRE(cpu.instructionCycle == 0);
        REQUIRE(cpu.PC == 0x0151);

        REQUIRE(cpu.A == 0x11);
        REQUIRE(cpu.getZeroFlag() == 0);
        REQUIRE(cpu.getSubtractFlag() == 0);
        REQUIRE(cpu.getHalfCarryFlag() == 1);
        REQUIRE(cpu.getCarryFlag() == 0);
    }

    SECTION("Check Carry Flag")
    {
        cpu.A = 0x88;
        mem.wram[0] = 0x89;

        cpu.op_add_a_addr_hl();
        REQUIRE(cpu.instructionCycle == 1);
        cpu.op_add_a_addr_hl();
        REQUIRE(cpu.instructionCycle == 0);
        REQUIRE(cpu.PC == 0x0151);

        REQUIRE(cpu.A == 0x11);
        REQUIRE(cpu.getZeroFlag() == 0);
        REQUIRE(cpu.getSubtractFlag() == 0);
        REQUIRE(cpu.getHalfCarryFlag() == 1);
        REQUIRE(cpu.getCarryFlag() == 1);
    }
}

TEST_CASE("ADD A, n8", "[OPCODE]")
{
    SM83 cpu;
    Memory mem;
    PPU ppu;

    cpu.memory = &mem;
    mem.ppu = &ppu;
    ppu.memory = &mem;
    ppu.cpu = &cpu;

    // We will use WRAM to store the opcode and immediate value
    cpu.PC = 0xC000;
    cpu.instructionCycle = 0;
    cpu.F = 0x0;

    SECTION("Check Zero Flag")
    {
        cpu.A = 0;
        mem.wram[1] = 0;

        cpu.op_add_a_n8();
        REQUIRE(cpu.instructionCycle == 1);
        cpu.op_add_a_n8();
        REQUIRE(cpu.instructionCycle == 0);
        REQUIRE(cpu.PC == 0xC002);

        REQUIRE(cpu.A == 0x0);
        REQUIRE(cpu.getZeroFlag() == 1);
        REQUIRE(cpu.getSubtractFlag() == 0);
        REQUIRE(cpu.getHalfCarryFlag() == 0);
        REQUIRE(cpu.getCarryFlag() == 0);
    }

    SECTION("Check Half Carry Flag")
    {
        cpu.A = 0x8;
        mem.wram[1] = 0x9;

        cpu.op_add_a_n8();
        REQUIRE(cpu.instructionCycle == 1);
        cpu.op_add_a_n8();
        REQUIRE(cpu.instructionCycle == 0);
        REQUIRE(cpu.PC == 0xC002);

        REQUIRE(cpu.A == 0x11);
        REQUIRE(cpu.getZeroFlag() == 0);
        REQUIRE(cpu.getSubtractFlag() == 0);
        REQUIRE(cpu.getHalfCarryFlag() == 1);
        REQUIRE(cpu.getCarryFlag() == 0);
    }

    SECTION("Check Carry Flag")
    {
        cpu.A = 0x88;
        mem.wram[1] = 0x89;

        cpu.op_add_a_n8();
        REQUIRE(cpu.instructionCycle == 1);
        cpu.op_add_a_n8();
        REQUIRE(cpu.instructionCycle == 0);
        REQUIRE(cpu.PC == 0xC002);

        REQUIRE(cpu.A == 0x11);
        REQUIRE(cpu.getZeroFlag() == 0);
        REQUIRE(cpu.getSubtractFlag() == 0);
        REQUIRE(cpu.getHalfCarryFlag() == 1);
        REQUIRE(cpu.getCarryFlag() == 1);
    }
}

TEST_CASE("DEC r8", "[OPCODE]")
{
    SM83 cpu;

    cpu.PC = 0x0150;
    cpu.instructionCycle = 0;
    cpu.F = 0x0;

    SECTION("Check Zero Flag")
    {
        cpu.A = 1;

        cpu.op_dec_r8(cpu.A);
        REQUIRE(cpu.PC == 0x0151);

        REQUIRE(cpu.A == 0);
        REQUIRE(cpu.getZeroFlag() == 1);
        REQUIRE(cpu.getSubtractFlag() == 1);
        REQUIRE(cpu.getHalfCarryFlag() == 0);
    }

    SECTION("Check Half Carry Flag")
    {
        cpu.A = 0x10;

        cpu.op_dec_r8(cpu.A);
        REQUIRE(cpu.PC == 0x0151);

        REQUIRE(cpu.A == 0x0F);
        REQUIRE(cpu.getZeroFlag() == 0);
        REQUIRE(cpu.getSubtractFlag() == 1);
        REQUIRE(cpu.getHalfCarryFlag() == 1);
    }
}

TEST_CASE("DEC [HL]")
{
    SM83 cpu;
    Memory mem;
    PPU ppu;

    cpu.memory = &mem;
    mem.ppu = &ppu;
    ppu.memory = &mem;
    ppu.cpu = &cpu;

    cpu.PC = 0x0150;
    cpu.instructionCycle = 0;
    cpu.F = 0x0;

    // HL points to first address in WRAM
    cpu.H = 0xC0;
    cpu.L = 0x00;

    SECTION("Check Zero Flag")
    {
        mem.wram[0] = 1;

        cpu.op_dec_addr_hl();
        REQUIRE(cpu.instructionCycle == 1);
        cpu.op_dec_addr_hl();
        REQUIRE(cpu.instructionCycle == 2);
        cpu.op_dec_addr_hl();
        REQUIRE(cpu.instructionCycle == 0);
        REQUIRE(cpu.PC == 0x0151);

        REQUIRE(mem.wram[0] == 0x0);
        REQUIRE(cpu.getZeroFlag() == 1);
        REQUIRE(cpu.getSubtractFlag() == 1);
        REQUIRE(cpu.getHalfCarryFlag() == 0);
    }

    SECTION("Check Half Carry Flag")
    {
        mem.wram[0] = 0x10;

        cpu.op_dec_addr_hl();
        REQUIRE(cpu.instructionCycle == 1);
        cpu.op_dec_addr_hl();
        REQUIRE(cpu.instructionCycle == 2);
        cpu.op_dec_addr_hl();
        REQUIRE(cpu.instructionCycle == 0);
        REQUIRE(cpu.PC == 0x0151);

        REQUIRE(mem.wram[0] == 0x0F);
        REQUIRE(cpu.getZeroFlag() == 0);
        REQUIRE(cpu.getSubtractFlag() == 1);
        REQUIRE(cpu.getHalfCarryFlag() == 1);
    }
}

TEST_CASE("INC r8", "[OPCODE]")
{
    SM83 cpu;

    cpu.PC = 0x0150;
    cpu.instructionCycle = 0;
    cpu.F = 0x0;

    SECTION("Check Zero Flag")
    {
        cpu.A = 0xFF;

        cpu.op_inc_r8(cpu.A);
        REQUIRE(cpu.PC == 0x0151);

        REQUIRE(cpu.A == 0x0);
        REQUIRE(cpu.getZeroFlag() == 1);
        REQUIRE(cpu.getSubtractFlag() == 0);
        REQUIRE(cpu.getHalfCarryFlag() == 1);
    }

    SECTION("Check Half Carry Flag")
    {
        cpu.A = 0x0F;

        cpu.op_inc_r8(cpu.A);
        REQUIRE(cpu.PC == 0x0151);

        REQUIRE(cpu.A == 0x10);
        REQUIRE(cpu.getZeroFlag() == 0);
        REQUIRE(cpu.getSubtractFlag() == 0);
        REQUIRE(cpu.getHalfCarryFlag() == 1);
    }
}

TEST_CASE("INC [HL]", "[OPCODE]")
{
    SM83 cpu;
    Memory mem;
    PPU ppu;

    cpu.memory = &mem;
    mem.ppu = &ppu;
    ppu.memory = &mem;
    ppu.cpu = &cpu;

    cpu.PC = 0x0150;
    cpu.instructionCycle = 0;
    cpu.F = 0x0;

    // HL points to the first address in WRAM
    cpu.H = 0xC0;
    cpu.L = 0x00;

    SECTION("Check Zero Flag")
    {
        mem.wram[0] = 0xFF;

        cpu.op_inc_addr_hl();
        REQUIRE(cpu.instructionCycle == 1);
        cpu.op_inc_addr_hl();
        REQUIRE(cpu.instructionCycle == 2);
        cpu.op_inc_addr_hl();
        REQUIRE(cpu.instructionCycle == 0);
        REQUIRE(cpu.PC == 0x0151);

        REQUIRE(mem.wram[0] == 0x0);
        REQUIRE(cpu.getZeroFlag() == 1);
        REQUIRE(cpu.getSubtractFlag() == 0);
        REQUIRE(cpu.getHalfCarryFlag() == 1);
    }

    SECTION("Check Half Carry Flag")
    {
        mem.wram[0] = 0x0F;

        cpu.op_inc_addr_hl();
        REQUIRE(cpu.instructionCycle == 1);
        cpu.op_inc_addr_hl();
        REQUIRE(cpu.instructionCycle == 2);
        cpu.op_inc_addr_hl();
        REQUIRE(cpu.instructionCycle == 0);
        REQUIRE(cpu.PC == 0x0151);

        REQUIRE(mem.wram[0] == 0x10);
        REQUIRE(cpu.getZeroFlag() == 0);
        REQUIRE(cpu.getSubtractFlag() == 0);
        REQUIRE(cpu.getHalfCarryFlag() == 1);
    }
}

TEST_CASE("SBC A, r8", "[OPCODE]")
{
    SM83 cpu;

    cpu.PC = 0x0150;
    cpu.instructionCycle = 0;
    cpu.F = 0x0;

    SECTION("Check Zero Flag")
    {
        cpu.A = 0x8;
        cpu.B = 0x7;
        cpu.setCarryFlag(1);

        cpu.op_sbc_a_r8(cpu.B);
        REQUIRE(cpu.PC == 0x0151);

        REQUIRE(cpu.A == 0x0);
        REQUIRE(cpu.getZeroFlag() == 1);
        REQUIRE(cpu.getSubtractFlag() == 1);
        REQUIRE(cpu.getHalfCarryFlag() == 0);
        REQUIRE(cpu.getCarryFlag() == 0);
    }

    SECTION("Check Half Carry Flag")
    {
        cpu.A = 0x87;
        cpu.B = 0x77;
        cpu.setCarryFlag(1);

        cpu.op_sbc_a_r8(cpu.B);
        REQUIRE(cpu.PC == 0x0151);

        REQUIRE(cpu.A == 0xF);
        REQUIRE(cpu.getZeroFlag() == 0);
        REQUIRE(cpu.getSubtractFlag() == 1);
        REQUIRE(cpu.getHalfCarryFlag() == 1);
        REQUIRE(cpu.getCarryFlag() == 0);
    }

    SECTION("Check Carry Flag")
    {
        cpu.A = 0x87;
        cpu.B = 0x87;
        cpu.setCarryFlag(1);

        cpu.op_sbc_a_r8(cpu.B);
        REQUIRE(cpu.PC == 0x0151);

        REQUIRE(cpu.A == 0xFF);
        REQUIRE(cpu.getZeroFlag() == 0);
        REQUIRE(cpu.getSubtractFlag() == 1);
        REQUIRE(cpu.getHalfCarryFlag() == 1);
        REQUIRE(cpu.getCarryFlag() == 1);
    }
}

TEST_CASE("SBC A, [HL]", "[OPCODE]")
{
    SM83 cpu;
    Memory mem;
    PPU ppu;

    cpu.memory = &mem;
    mem.ppu = &ppu;
    ppu.memory = &mem;
    ppu.cpu = &cpu;

    cpu.PC = 0x0150;
    cpu.instructionCycle = 0;
    cpu.F = 0x0;

    // HL points to first address in WRAM
    cpu.H = 0xC0;
    cpu.L = 0x00;

    SECTION("Check Zero Flag")
    {
        cpu.A = 0x8;
        mem.wram[0] = 0x7;
        cpu.setCarryFlag(1);

        cpu.op_sbc_a_addr_hl();
        REQUIRE(cpu.instructionCycle == 1);
        cpu.op_sbc_a_addr_hl();
        REQUIRE(cpu.instructionCycle == 0);
        REQUIRE(cpu.PC == 0x0151);

        REQUIRE(cpu.A == 0x0);
        REQUIRE(cpu.getZeroFlag() == 1);
        REQUIRE(cpu.getSubtractFlag() == 1);
        REQUIRE(cpu.getHalfCarryFlag() == 0);
        REQUIRE(cpu.getCarryFlag() == 0);
    }

    SECTION("Check Half Carry Flag")
    {
        cpu.A = 0x87;
        mem.wram[0] = 0x77;
        cpu.setCarryFlag(1);

        cpu.op_sbc_a_addr_hl();
        REQUIRE(cpu.instructionCycle == 1);
        cpu.op_sbc_a_addr_hl();
        REQUIRE(cpu.instructionCycle == 0);
        REQUIRE(cpu.PC == 0x0151);

        REQUIRE(cpu.A == 0x0F);
        REQUIRE(cpu.getZeroFlag() == 0);
        REQUIRE(cpu.getSubtractFlag() == 1);
        REQUIRE(cpu.getHalfCarryFlag() == 1);
        REQUIRE(cpu.getCarryFlag() == 0);
    }

    SECTION("Check Carry Flag")
    {
        cpu.A = 0x87;
        mem.wram[0] = 0x87;
        cpu.setCarryFlag(1);

        cpu.op_sbc_a_addr_hl();
        REQUIRE(cpu.instructionCycle == 1);
        cpu.op_sbc_a_addr_hl();
        REQUIRE(cpu.instructionCycle == 0);
        REQUIRE(cpu.PC == 0x0151);

        REQUIRE(cpu.A == 0xFF);
        REQUIRE(cpu.getZeroFlag() == 0);
        REQUIRE(cpu.getSubtractFlag() == 1);
        REQUIRE(cpu.getHalfCarryFlag() == 1);
        REQUIRE(cpu.getCarryFlag() == 1);
    }
}

TEST_CASE("SBC A, n8", "[OPCODE]")
{
    SM83 cpu;
    Memory mem;
    PPU ppu;

    cpu.memory = &mem;
    mem.ppu = &ppu;
    ppu.memory = &mem;
    ppu.cpu = &cpu;

    cpu.PC = 0xC000;
    cpu.instructionCycle = 0;
    cpu.F = 0x0;

    SECTION("Check Zero Flag")
    {
        cpu.A = 0x8;
        mem.wram[1] = 0x7;
        cpu.setCarryFlag(1);

        cpu.op_sbc_a_n8();
        REQUIRE(cpu.instructionCycle == 1);
        cpu.op_sbc_a_n8();
        REQUIRE(cpu.instructionCycle == 0);
        REQUIRE(cpu.PC == 0xC002);

        REQUIRE(cpu.A == 0x0);
        REQUIRE(cpu.getZeroFlag() == 1);
        REQUIRE(cpu.getSubtractFlag() == 1);
        REQUIRE(cpu.getHalfCarryFlag() == 0);
        REQUIRE(cpu.getCarryFlag() == 0);
    }

    SECTION("Check Half Carry Flag")
    {
        cpu.A = 0x87;
        mem.wram[1] = 0x77;
        cpu.setCarryFlag(1);

        cpu.op_sbc_a_n8();
        REQUIRE(cpu.instructionCycle == 1);
        cpu.op_sbc_a_n8();
        REQUIRE(cpu.instructionCycle == 0);
        REQUIRE(cpu.PC == 0xC002);

        REQUIRE(cpu.A == 0x0F);
        REQUIRE(cpu.getZeroFlag() == 0);
        REQUIRE(cpu.getSubtractFlag() == 1);
        REQUIRE(cpu.getHalfCarryFlag() == 1);
        REQUIRE(cpu.getCarryFlag() == 0);
    }

    SECTION("Check Carry Flag")
    {
        cpu.A = 0x87;
        mem.wram[1] = 0x87;
        cpu.setCarryFlag(1);

        cpu.op_sbc_a_n8();
        REQUIRE(cpu.instructionCycle == 1);
        cpu.op_sbc_a_n8();
        REQUIRE(cpu.instructionCycle == 0);
        REQUIRE(cpu.PC == 0xC002);

        REQUIRE(cpu.A == 0xFF);
        REQUIRE(cpu.getZeroFlag() == 0);
        REQUIRE(cpu.getSubtractFlag() == 1);
        REQUIRE(cpu.getHalfCarryFlag() == 1);
        REQUIRE(cpu.getCarryFlag() == 1);
    }
}

TEST_CASE("SUB A, r8", "[OPCODE]")
{
    SM83 cpu;

    cpu.PC = 0x0150;
    cpu.instructionCycle = 0;
    cpu.F = 0x0;

    SECTION("Check Zero Flag")
    {
        cpu.A = 0x8;
        cpu.B = 0x8;

        cpu.op_sub_a_r8(cpu.B);
        REQUIRE(cpu.PC == 0x0151);

        REQUIRE(cpu.A == 0x0);
        REQUIRE(cpu.getZeroFlag() == 1);
        REQUIRE(cpu.getSubtractFlag() == 1);
        REQUIRE(cpu.getHalfCarryFlag() == 0);
        REQUIRE(cpu.getCarryFlag() == 0);
    }

    SECTION("Check Half Carry Flag")
    {
        cpu.A = 0x87;
        cpu.B = 0x78;

        cpu.op_sub_a_r8(cpu.B);
        REQUIRE(cpu.PC == 0x0151);

        REQUIRE(cpu.A == 0xF);
        REQUIRE(cpu.getZeroFlag() == 0);
        REQUIRE(cpu.getSubtractFlag() == 1);
        REQUIRE(cpu.getHalfCarryFlag() == 1);
        REQUIRE(cpu.getCarryFlag() == 0);
    }

    SECTION("Check Carry Flag")
    {
        cpu.A = 0x87;
        cpu.B = 0x88;

        cpu.op_sub_a_r8(cpu.B);
        REQUIRE(cpu.PC == 0x0151);

        REQUIRE(cpu.A == 0xFF);
        REQUIRE(cpu.getZeroFlag() == 0);
        REQUIRE(cpu.getSubtractFlag() == 1);
        REQUIRE(cpu.getHalfCarryFlag() == 1);
        REQUIRE(cpu.getCarryFlag() == 1);
    }
}

TEST_CASE("SUB A, [HL]", "[OPCODE]")
{
    SM83 cpu;
    Memory mem;
    PPU ppu;

    cpu.memory = &mem;
    mem.ppu = &ppu;
    ppu.memory = &mem;
    ppu.cpu = &cpu;

    cpu.PC = 0x0150;
    cpu.instructionCycle = 0;
    cpu.F = 0x0;

    // HL points to first address in WRAM
    cpu.H = 0xC0;
    cpu.L = 0x00;

    SECTION("Check Zero Flag")
    {
        cpu.A = 0x8;
        mem.wram[0] = 0x8;

        cpu.op_sub_a_addr_hl();
        REQUIRE(cpu.instructionCycle == 1);
        cpu.op_sub_a_addr_hl();
        REQUIRE(cpu.instructionCycle == 0);
        REQUIRE(cpu.PC == 0x0151);

        REQUIRE(cpu.A == 0x0);
        REQUIRE(cpu.getZeroFlag() == 1);
        REQUIRE(cpu.getSubtractFlag() == 1);
        REQUIRE(cpu.getHalfCarryFlag() == 0);
        REQUIRE(cpu.getCarryFlag() == 0);
    }

    SECTION("Check Half Carry Flag")
    {
        cpu.A = 0x87;
        mem.wram[0] = 0x78;

        cpu.op_sub_a_addr_hl();
        REQUIRE(cpu.instructionCycle == 1);
        cpu.op_sub_a_addr_hl();
        REQUIRE(cpu.instructionCycle == 0);
        REQUIRE(cpu.PC == 0x0151);

        REQUIRE(cpu.A == 0x0F);
        REQUIRE(cpu.getZeroFlag() == 0);
        REQUIRE(cpu.getSubtractFlag() == 1);
        REQUIRE(cpu.getHalfCarryFlag() == 1);
        REQUIRE(cpu.getCarryFlag() == 0);
    }

    SECTION("Check Carry Flag")
    {
        cpu.A = 0x87;
        mem.wram[0] = 0x88;

        cpu.op_sub_a_addr_hl();
        REQUIRE(cpu.instructionCycle == 1);
        cpu.op_sub_a_addr_hl();
        REQUIRE(cpu.instructionCycle == 0);
        REQUIRE(cpu.PC == 0x0151);

        REQUIRE(cpu.A == 0xFF);
        REQUIRE(cpu.getZeroFlag() == 0);
        REQUIRE(cpu.getSubtractFlag() == 1);
        REQUIRE(cpu.getHalfCarryFlag() == 1);
        REQUIRE(cpu.getCarryFlag() == 1);
    }
}

TEST_CASE("SUB A, n8", "[OPCODE]")
{
    SM83 cpu;
    Memory mem;
    PPU ppu;

    cpu.memory = &mem;
    mem.ppu = &ppu;
    ppu.memory = &mem;
    ppu.cpu = &cpu;

    cpu.PC = 0xC000;
    cpu.instructionCycle = 0;
    cpu.F = 0x0;

    SECTION("Check Zero Flag")
    {
        cpu.A = 0x8;
        mem.wram[1] = 0x8;

        cpu.op_sub_a_n8();
        REQUIRE(cpu.instructionCycle == 1);
        cpu.op_sub_a_n8();
        REQUIRE(cpu.instructionCycle == 0);
        REQUIRE(cpu.PC == 0xC002);

        REQUIRE(cpu.A == 0x0);
        REQUIRE(cpu.getZeroFlag() == 1);
        REQUIRE(cpu.getSubtractFlag() == 1);
        REQUIRE(cpu.getHalfCarryFlag() == 0);
        REQUIRE(cpu.getCarryFlag() == 0);
    }

    SECTION("Check Half Carry Flag")
    {
        cpu.A = 0x87;
        mem.wram[1] = 0x78;

        cpu.op_sub_a_n8();
        REQUIRE(cpu.instructionCycle == 1);
        cpu.op_sub_a_n8();
        REQUIRE(cpu.instructionCycle == 0);
        REQUIRE(cpu.PC == 0xC002);

        REQUIRE(cpu.A == 0x0F);
        REQUIRE(cpu.getZeroFlag() == 0);
        REQUIRE(cpu.getSubtractFlag() == 1);
        REQUIRE(cpu.getHalfCarryFlag() == 1);
        REQUIRE(cpu.getCarryFlag() == 0);
    }

    SECTION("Check Carry Flag")
    {
        cpu.A = 0x87;
        mem.wram[1] = 0x88;

        cpu.op_sub_a_n8();
        REQUIRE(cpu.instructionCycle == 1);
        cpu.op_sub_a_n8();
        REQUIRE(cpu.instructionCycle == 0);
        REQUIRE(cpu.PC == 0xC002);

        REQUIRE(cpu.A == 0xFF);
        REQUIRE(cpu.getZeroFlag() == 0);
        REQUIRE(cpu.getSubtractFlag() == 1);
        REQUIRE(cpu.getHalfCarryFlag() == 1);
        REQUIRE(cpu.getCarryFlag() == 1);
    }
}

TEST_CASE("AND A, r8", "[OPCODE]")
{
    SM83 cpu;

    cpu.PC = 0x0150;
    cpu.instructionCycle = 0;
    cpu.F = 0x0;

    SECTION("Non Zero Result")
    {
        cpu.A = 0x66;
        cpu.B = 0x43;

        cpu.op_and_a_r8(cpu.B);
        REQUIRE(cpu.PC == 0x0151);

        REQUIRE(cpu.A == 0x42);
        REQUIRE(cpu.getZeroFlag() == 0);
        REQUIRE(cpu.getSubtractFlag() == 0);
        REQUIRE(cpu.getHalfCarryFlag() == 1);
        REQUIRE(cpu.getCarryFlag() == 0);
    }

    SECTION("Zero Result")
    {
        cpu.A = 0x66;
        cpu.B = 0x0;

        cpu.op_and_a_r8(cpu.B);
        REQUIRE(cpu.PC == 0x0151);

        REQUIRE(cpu.A == 0x00);
        REQUIRE(cpu.getZeroFlag() == 1);
        REQUIRE(cpu.getSubtractFlag() == 0);
        REQUIRE(cpu.getHalfCarryFlag() == 1);
        REQUIRE(cpu.getCarryFlag() == 0);
    }
}

TEST_CASE("AND A, [HL]", "[OPCODE]")
{
    SM83 cpu;
    Memory mem;
    PPU ppu;

    cpu.memory = &mem;
    mem.ppu = &ppu;
    ppu.memory = &mem;
    ppu.cpu = &cpu;

    cpu.PC = 0x0150;
    cpu.instructionCycle = 0;
    cpu.F = 0x0;

    // HL points to the first addres in WRAM
    cpu.H = 0xC0;
    cpu.L = 0x00;

    SECTION("Non Zero Result")
    {
        cpu.A = 0x66;
        mem.wram[0] = 0x43;

        cpu.op_and_a_addr_hl();
        REQUIRE(cpu.instructionCycle == 1);
        cpu.op_and_a_addr_hl();
        REQUIRE(cpu.instructionCycle == 0);
        REQUIRE(cpu.PC == 0x0151);

        REQUIRE(cpu.A == 0x42);
        REQUIRE(cpu.getZeroFlag() == 0);
        REQUIRE(cpu.getSubtractFlag() == 0);
        REQUIRE(cpu.getHalfCarryFlag() == 1);
        REQUIRE(cpu.getCarryFlag() == 0);
    }

    SECTION("Zero Result")
    {
        cpu.A = 0x66;
        mem.wram[0] = 0x0;

        cpu.op_and_a_addr_hl();
        REQUIRE(cpu.instructionCycle == 1);
        cpu.op_and_a_addr_hl();
        REQUIRE(cpu.instructionCycle == 0);
        REQUIRE(cpu.PC == 0x0151);

        REQUIRE(cpu.A == 0x00);
        REQUIRE(cpu.getZeroFlag() == 1);
        REQUIRE(cpu.getSubtractFlag() == 0);
        REQUIRE(cpu.getHalfCarryFlag() == 1);
        REQUIRE(cpu.getCarryFlag() == 0);
    }
}

TEST_CASE("AND A, n8", "[OPCODE]")
{
    SM83 cpu;
    Memory mem;
    PPU ppu;

    cpu.memory = &mem;
    mem.ppu = &ppu;
    ppu.memory = &mem;
    ppu.cpu = &cpu;

    cpu.PC = 0xC000;
    cpu.instructionCycle = 0;
    cpu.F = 0x0;

    SECTION("Non Zero Result")
    {
        cpu.A = 0x66;
        mem.wram[1] = 0x43;

        cpu.op_and_a_n8();
        REQUIRE(cpu.instructionCycle == 1);
        cpu.op_and_a_n8();
        REQUIRE(cpu.instructionCycle == 0);
        REQUIRE(cpu.PC == 0xC002);

        REQUIRE(cpu.A == 0x42);
        REQUIRE(cpu.getZeroFlag() == 0);
        REQUIRE(cpu.getSubtractFlag() == 0);
        REQUIRE(cpu.getHalfCarryFlag() == 1);
        REQUIRE(cpu.getCarryFlag() == 0);
    }

    SECTION("Zero Result")
    {
        cpu.A = 0x66;
        mem.wram[1] = 0x0;

        cpu.op_and_a_n8();
        REQUIRE(cpu.instructionCycle == 1);
        cpu.op_and_a_n8();
        REQUIRE(cpu.instructionCycle == 0);
        REQUIRE(cpu.PC == 0xC002);

        REQUIRE(cpu.A == 0x00);
        REQUIRE(cpu.getZeroFlag() == 1);
        REQUIRE(cpu.getSubtractFlag() == 0);
        REQUIRE(cpu.getHalfCarryFlag() == 1);
        REQUIRE(cpu.getCarryFlag() == 0);
    }
}

TEST_CASE("CP A, r8", "[OPCODE]")
{
    SM83 cpu;

    cpu.PC = 0x0150;
    cpu.instructionCycle = 0;
    cpu.F = 0x0;

    SECTION("Check Zero Flag")
    {
        cpu.A = 0x8;
        cpu.B = 0x8;

        cpu.op_cp_a_r8(cpu.B);
        REQUIRE(cpu.PC == 0x0151);

        REQUIRE(cpu.getZeroFlag() == 1);
        REQUIRE(cpu.getSubtractFlag() == 1);
        REQUIRE(cpu.getHalfCarryFlag() == 0);
        REQUIRE(cpu.getCarryFlag() == 0);
    }

    SECTION("Check Half Carry Flag")
    {
        cpu.A = 0x88;
        cpu.B = 0x79;

        cpu.op_cp_a_r8(cpu.B);
        REQUIRE(cpu.PC == 0x0151);

        REQUIRE(cpu.getZeroFlag() == 0);
        REQUIRE(cpu.getSubtractFlag() == 1);
        REQUIRE(cpu.getHalfCarryFlag() == 1);
        REQUIRE(cpu.getCarryFlag() == 0);
    }

    SECTION("Check Carry Flag")
    {
        cpu.A = 0x88;
        cpu.B = 0x89;

        cpu.op_cp_a_r8(cpu.B);
        REQUIRE(cpu.PC == 0x0151);

        REQUIRE(cpu.getZeroFlag() == 0);
        REQUIRE(cpu.getSubtractFlag() == 1);
        REQUIRE(cpu.getHalfCarryFlag() == 1);
        REQUIRE(cpu.getCarryFlag() == 1);
    }
}

TEST_CASE("CP A, [HL]", "[OPCODE]")
{
    SM83 cpu;
    Memory mem;
    PPU ppu;

    cpu.memory = &mem;
    mem.ppu = &ppu;
    ppu.memory = &mem;
    ppu.cpu = &cpu;

    cpu.PC = 0x0150;
    cpu.instructionCycle = 0;
    cpu.F = 0x0;

    // HL points to the first address in WRAM
    cpu.H = 0xC0;
    cpu.L = 0x00;

    SECTION("Check Zero Flag")
    {
        cpu.A = 0x8;
        mem.wram[0] = 0x8;

        cpu.op_cp_a_addr_hl();
        REQUIRE(cpu.instructionCycle == 1);
        cpu.op_cp_a_addr_hl();
        REQUIRE(cpu.instructionCycle == 0);
        REQUIRE(cpu.PC == 0x0151);

        REQUIRE(cpu.getZeroFlag() == 1);
        REQUIRE(cpu.getSubtractFlag() == 1);
        REQUIRE(cpu.getHalfCarryFlag() == 0);
        REQUIRE(cpu.getCarryFlag() == 0);
    }

    SECTION("Check Half Carry Flag")
    {
        cpu.A = 0x88;
        mem.wram[0] = 0x79;

        cpu.op_cp_a_addr_hl();
        REQUIRE(cpu.instructionCycle == 1);
        cpu.op_cp_a_addr_hl();
        REQUIRE(cpu.instructionCycle == 0);
        REQUIRE(cpu.PC == 0x0151);

        REQUIRE(cpu.getZeroFlag() == 0);
        REQUIRE(cpu.getSubtractFlag() == 1);
        REQUIRE(cpu.getHalfCarryFlag() == 1);
        REQUIRE(cpu.getCarryFlag() == 0);
    }

    SECTION("Check Carry Flag")
    {
        cpu.A = 0x88;
        mem.wram[0] = 0x89;

        cpu.op_cp_a_addr_hl();
        REQUIRE(cpu.instructionCycle == 1);
        cpu.op_cp_a_addr_hl();
        REQUIRE(cpu.instructionCycle == 0);
        REQUIRE(cpu.PC == 0x0151);

        REQUIRE(cpu.getZeroFlag() == 0);
        REQUIRE(cpu.getSubtractFlag() == 1);
        REQUIRE(cpu.getHalfCarryFlag() == 1);
        REQUIRE(cpu.getCarryFlag() == 1);
    }
}

TEST_CASE("CP A, n8", "[OPCODE]")
{
    SM83 cpu;
    Memory mem;
    PPU ppu;

    cpu.memory = &mem;
    mem.ppu = &ppu;
    ppu.memory = &mem;
    ppu.cpu = &cpu;

    cpu.PC = 0xC000;
    cpu.instructionCycle = 0;
    cpu.F = 0x0;

    SECTION("Check Zero Flag")
    {
        cpu.A = 0x8;
        mem.wram[1] = 0x8;

        cpu.op_cp_a_n8();
        REQUIRE(cpu.instructionCycle == 1);
        cpu.op_cp_a_n8();
        REQUIRE(cpu.instructionCycle == 0);
        REQUIRE(cpu.PC == 0xC002);

        REQUIRE(cpu.getZeroFlag() == 1);
        REQUIRE(cpu.getSubtractFlag() == 1);
        REQUIRE(cpu.getHalfCarryFlag() == 0);
        REQUIRE(cpu.getCarryFlag() == 0);
    }

    SECTION("Check Half Carry Flag")
    {
        cpu.A = 0x88;
        mem.wram[1] = 0x79;

        cpu.op_cp_a_n8();
        REQUIRE(cpu.instructionCycle == 1);
        cpu.op_cp_a_n8();
        REQUIRE(cpu.instructionCycle == 0);
        REQUIRE(cpu.PC == 0xC002);

        REQUIRE(cpu.getZeroFlag() == 0);
        REQUIRE(cpu.getSubtractFlag() == 1);
        REQUIRE(cpu.getHalfCarryFlag() == 1);
        REQUIRE(cpu.getCarryFlag() == 0);
    }

    SECTION("Check Carry Flag")
    {
        cpu.A = 0x88;
        mem.wram[1] = 0x89;

        cpu.op_cp_a_n8();
        REQUIRE(cpu.instructionCycle == 1);
        cpu.op_cp_a_n8();
        REQUIRE(cpu.instructionCycle == 0);
        REQUIRE(cpu.PC == 0xC002);

        REQUIRE(cpu.getZeroFlag() == 0);
        REQUIRE(cpu.getSubtractFlag() == 1);
        REQUIRE(cpu.getHalfCarryFlag() == 1);
        REQUIRE(cpu.getCarryFlag() == 1);
    }
}

TEST_CASE("OR A, r8", "[OPCODE]")
{
    SM83 cpu;

    cpu.PC = 0x0150;
    cpu.instructionCycle = 0;
    cpu.F = 0x0;

    SECTION("Non Zero Result")
    {
        cpu.A = 0x2;
        cpu.B = 0x5;

        cpu.op_or_a_r8(cpu.B);
        REQUIRE(cpu.PC == 0x0151);

        REQUIRE(cpu.A == 0x7);
        REQUIRE(cpu.getZeroFlag() == 0);
        REQUIRE(cpu.getSubtractFlag() == 0);
        REQUIRE(cpu.getHalfCarryFlag() == 0);
        REQUIRE(cpu.getCarryFlag() == 0);
    }

    SECTION("Zero Result")
    {
        cpu.A = 0x0;
        cpu.B = 0x0;

        cpu.op_or_a_r8(cpu.B);
        REQUIRE(cpu.PC == 0x0151);

        REQUIRE(cpu.A == 0x0);
        REQUIRE(cpu.getZeroFlag() == 1);
        REQUIRE(cpu.getSubtractFlag() == 0);
        REQUIRE(cpu.getHalfCarryFlag() == 0);
        REQUIRE(cpu.getCarryFlag() == 0);
    }
}

TEST_CASE("OR A, [HL]", "[OPCODE]")
{
    SM83 cpu;
    Memory mem;
    PPU ppu;

    cpu.memory = &mem;
    mem.ppu = &ppu;
    ppu.memory = &mem;
    ppu.cpu = &cpu;

    cpu.PC = 0x0150;
    cpu.instructionCycle = 0;
    cpu.F = 0x0;

    // HL points to the first address in WRAM
    cpu.H = 0xC0;
    cpu.L = 0x00;

    SECTION("Non Zero Result")
    {
        cpu.A = 0x2;
        mem.wram[0] = 0x5;

        cpu.op_or_a_addr_hl();
        REQUIRE(cpu.instructionCycle == 1);
        cpu.op_or_a_addr_hl();
        REQUIRE(cpu.instructionCycle == 0);
        REQUIRE(cpu.PC == 0x0151);

        REQUIRE(cpu.A == 0x7);
        REQUIRE(cpu.getZeroFlag() == 0);
        REQUIRE(cpu.getSubtractFlag() == 0);
        REQUIRE(cpu.getHalfCarryFlag() == 0);
        REQUIRE(cpu.getCarryFlag() == 0);
    }

    SECTION("Zero Result")
    {
        cpu.A = 0x0;
        mem.wram[0] = 0x0;

        cpu.op_or_a_addr_hl();
        REQUIRE(cpu.instructionCycle == 1);
        cpu.op_or_a_addr_hl();
        REQUIRE(cpu.instructionCycle == 0);
        REQUIRE(cpu.PC == 0x0151);

        REQUIRE(cpu.A == 0x0);
        REQUIRE(cpu.getZeroFlag() == 1);
        REQUIRE(cpu.getSubtractFlag() == 0);
        REQUIRE(cpu.getHalfCarryFlag() == 0);
        REQUIRE(cpu.getCarryFlag() == 0);
    }
}

TEST_CASE("OR A, n8", "[OPCODE]")
{
    SM83 cpu;
    Memory mem;
    PPU ppu;

    cpu.memory = &mem;
    mem.ppu = &ppu;
    ppu.memory = &mem;
    ppu.cpu = &cpu;

    cpu.PC = 0xC000;
    cpu.instructionCycle = 0;
    cpu.F = 0x0;

    SECTION("Non Zero Result")
    {
        cpu.A = 0x2;
        mem.wram[1] = 0x5;

        cpu.op_or_a_n8();
        REQUIRE(cpu.instructionCycle == 1);
        cpu.op_or_a_n8();
        REQUIRE(cpu.instructionCycle == 0);
        REQUIRE(cpu.PC == 0xC002);

        REQUIRE(cpu.A == 0x7);
        REQUIRE(cpu.getZeroFlag() == 0);
        REQUIRE(cpu.getSubtractFlag() == 0);
        REQUIRE(cpu.getHalfCarryFlag() == 0);
        REQUIRE(cpu.getCarryFlag() == 0);
    }

    SECTION("Zero Result")
    {
        cpu.A = 0x0;
        mem.wram[1] = 0x0;

        cpu.op_or_a_n8();
        REQUIRE(cpu.instructionCycle == 1);
        cpu.op_or_a_n8();
        REQUIRE(cpu.instructionCycle == 0);
        REQUIRE(cpu.PC == 0xC002);

        REQUIRE(cpu.A == 0x0);
        REQUIRE(cpu.getZeroFlag() == 1);
        REQUIRE(cpu.getSubtractFlag() == 0);
        REQUIRE(cpu.getHalfCarryFlag() == 0);
        REQUIRE(cpu.getCarryFlag() == 0);
    }
}

TEST_CASE("XOR A, r8", "[OPCODE]")
{
    SM83 cpu;

    cpu.PC = 0x0150;
    cpu.instructionCycle = 0;
    cpu.F = 0x0;

    SECTION("Non Zero Result")
    {
        cpu.A = 0xAA;
        cpu.B = 0x55;

        cpu.op_xor_a_r8(cpu.B);
        REQUIRE(cpu.PC == 0x0151);

        REQUIRE(cpu.A == 0xFF);
        REQUIRE(cpu.getZeroFlag() == 0);
        REQUIRE(cpu.getSubtractFlag() == 0);
        REQUIRE(cpu.getHalfCarryFlag() == 0);
        REQUIRE(cpu.getCarryFlag() == 0);
    }

    SECTION("Zero Result")
    {
        cpu.A = 0xAA;
        cpu.B = 0xAA;

        cpu.op_xor_a_r8(cpu.B);
        REQUIRE(cpu.PC == 0x0151);

        REQUIRE(cpu.A == 0x0);
        REQUIRE(cpu.getZeroFlag() == 1);
        REQUIRE(cpu.getSubtractFlag() == 0);
        REQUIRE(cpu.getHalfCarryFlag() == 0);
        REQUIRE(cpu.getCarryFlag() == 0);
    }
}

TEST_CASE("XOR A, [HL]", "[OPCODE]")
{
    SM83 cpu;
    Memory mem;
    PPU ppu;

    cpu.memory = &mem;
    mem.ppu = &ppu;
    ppu.memory = &mem;
    ppu.cpu = &cpu;

    cpu.PC = 0x0150;
    cpu.instructionCycle = 0;
    cpu.F = 0x0;

    // HL points to the first address in WRAM
    cpu.H = 0xC0;
    cpu.L = 0x00;

    SECTION("Non Zero Result")
    {
        cpu.A = 0xAA;
        mem.wram[0] = 0x55;

        cpu.op_xor_a_addr_hl();
        REQUIRE(cpu.instructionCycle == 1);
        cpu.op_xor_a_addr_hl();
        REQUIRE(cpu.instructionCycle == 0);
        REQUIRE(cpu.PC == 0x0151);

        REQUIRE(cpu.A == 0xFF);
        REQUIRE(cpu.getZeroFlag() == 0);
        REQUIRE(cpu.getSubtractFlag() == 0);
        REQUIRE(cpu.getHalfCarryFlag() == 0);
        REQUIRE(cpu.getCarryFlag() == 0);
    }

    SECTION("Zero Result")
    {
        cpu.A = 0xAA;
        mem.wram[0] = 0xAA;

        cpu.op_xor_a_addr_hl();
        REQUIRE(cpu.instructionCycle == 1);
        cpu.op_xor_a_addr_hl();
        REQUIRE(cpu.instructionCycle == 0);
        REQUIRE(cpu.PC == 0x0151);

        REQUIRE(cpu.A == 0x0);
        REQUIRE(cpu.getZeroFlag() == 1);
        REQUIRE(cpu.getSubtractFlag() == 0);
        REQUIRE(cpu.getHalfCarryFlag() == 0);
        REQUIRE(cpu.getCarryFlag() == 0);
    }
}

TEST_CASE("XOR A, n8", "[OPCODE]")
{
    SM83 cpu;
    Memory mem;
    PPU ppu;

    cpu.memory = &mem;
    mem.ppu = &ppu;
    ppu.memory = &mem;
    ppu.cpu = &cpu;

    cpu.PC = 0xC000;
    cpu.instructionCycle = 0;
    cpu.F = 0x0;

    SECTION("Non Zero Result")
    {
        cpu.A = 0xAA;
        mem.wram[1] = 0x55;

        cpu.op_xor_a_n8();
        REQUIRE(cpu.instructionCycle == 1);
        cpu.op_xor_a_n8();
        REQUIRE(cpu.instructionCycle == 0);
        REQUIRE(cpu.PC == 0xC002);

        REQUIRE(cpu.A == 0xFF);
        REQUIRE(cpu.getZeroFlag() == 0);
        REQUIRE(cpu.getSubtractFlag() == 0);
        REQUIRE(cpu.getHalfCarryFlag() == 0);
        REQUIRE(cpu.getCarryFlag() == 0);
    }

    SECTION("Zero Result")
    {
        cpu.A = 0xAA;
        mem.wram[1] = 0xAA;

        cpu.op_xor_a_n8();
        REQUIRE(cpu.instructionCycle == 1);
        cpu.op_xor_a_n8();
        REQUIRE(cpu.instructionCycle == 0);
        REQUIRE(cpu.PC == 0xC002);

        REQUIRE(cpu.A == 0x0);
        REQUIRE(cpu.getZeroFlag() == 1);
        REQUIRE(cpu.getSubtractFlag() == 0);
        REQUIRE(cpu.getHalfCarryFlag() == 0);
        REQUIRE(cpu.getCarryFlag() == 0);
    }
}

TEST_CASE("ADD HL, r16", "[OPCODE]")
{
    SM83 cpu;

    cpu.PC = 0x0150;
    cpu.instructionCycle = 0;
    cpu.F = 0x0;

    SECTION("Check Half Carry Flag")
    {
        cpu.H = 0x20;
        cpu.L = 0xFF;

        cpu.B = 0x0F;
        cpu.C = 0xFF;

        cpu.op_add_hl_r16(cpu.B, cpu.C);
        REQUIRE(cpu.instructionCycle == 1);
        cpu.op_add_hl_r16(cpu.B, cpu.C);
        REQUIRE(cpu.instructionCycle == 0);
        REQUIRE(cpu.PC == 0x0151);

        REQUIRE(cpu.H == 0x30);
        REQUIRE(cpu.L == 0xFE);
        REQUIRE(cpu.getSubtractFlag() == 0);
        REQUIRE(cpu.getHalfCarryFlag() == 1);
        REQUIRE(cpu.getCarryFlag() == 0);
    }

    SECTION("Check Carry Flag")
    {
        cpu.H = 0x80;
        cpu.L = 0xFF;

        cpu.B = 0x8F;
        cpu.C = 0xFF;

        cpu.op_add_hl_r16(cpu.B, cpu.C);
        REQUIRE(cpu.instructionCycle == 1);
        cpu.op_add_hl_r16(cpu.B, cpu.C);
        REQUIRE(cpu.instructionCycle == 0);
        REQUIRE(cpu.PC == 0x0151);

        REQUIRE(cpu.H == 0x10);
        REQUIRE(cpu.L == 0xFE);
        REQUIRE(cpu.getSubtractFlag() == 0);
        REQUIRE(cpu.getHalfCarryFlag() == 1);
        REQUIRE(cpu.getCarryFlag() == 1);
    }
}

TEST_CASE("DEC r16", "[OPCODE]")
{
    SM83 cpu;

    cpu.PC = 0x0150;
    cpu.instructionCycle = 0;

    cpu.B = 0x02;
    cpu.C = 0x00;

    cpu.op_dec_r16(cpu.B, cpu.C);
    REQUIRE(cpu.instructionCycle == 1);
    cpu.op_dec_r16(cpu.B, cpu.C);
    REQUIRE(cpu.instructionCycle == 0);
    REQUIRE(cpu.PC == 0x0151);

    REQUIRE(cpu.B == 0x01);
    REQUIRE(cpu.C == 0xFF);
}

TEST_CASE("INC r16", "[OPCODE]")
{
    SM83 cpu;

    cpu.PC = 0x0150;
    cpu.instructionCycle = 0;

    cpu.B = 0x01;
    cpu.C = 0xFF;

    cpu.op_inc_r16(cpu.B, cpu.C);
    REQUIRE(cpu.instructionCycle == 1);
    cpu.op_inc_r16(cpu.B, cpu.C);
    REQUIRE(cpu.instructionCycle == 0);
    REQUIRE(cpu.PC == 0x0151);

    REQUIRE(cpu.B == 0x02);
    REQUIRE(cpu.C == 0x00);
}

TEST_CASE("BIT u3, r8", "[OPCODE]")
{
    SM83 cpu;

    cpu.PC = 0x0150;
    cpu.instructionCycle = 0;
    cpu.F = 0x0;

    SECTION("Non Zero Result")
    {
        cpu.A = 0x66;

        cpu.op_bit_u3_r8(2, cpu.A);
        REQUIRE(cpu.instructionCycle == 1);
        cpu.op_bit_u3_r8(2, cpu.A);
        REQUIRE(cpu.instructionCycle == 0);
        REQUIRE(cpu.PC == 0x0152);

        REQUIRE(cpu.getZeroFlag() == 0);
        REQUIRE(cpu.getSubtractFlag() == 0);
        REQUIRE(cpu.getHalfCarryFlag() == 1);
    }

    SECTION("Zero Result")
    {
        cpu.A = 0x66;

        cpu.op_bit_u3_r8(3, cpu.A);
        REQUIRE(cpu.instructionCycle == 1);
        cpu.op_bit_u3_r8(3, cpu.A);
        REQUIRE(cpu.instructionCycle == 0);
        REQUIRE(cpu.PC == 0x0152);

        REQUIRE(cpu.getZeroFlag() == 1);
        REQUIRE(cpu.getSubtractFlag() == 0);
        REQUIRE(cpu.getHalfCarryFlag() == 1);
    }
}

TEST_CASE("BIT u3, [HL]", "[OPCODE]")
{
    SM83 cpu;
    Memory mem;
    PPU ppu;

    cpu.memory = &mem;
    mem.ppu = &ppu;
    ppu.memory = &mem;
    ppu.cpu = &cpu;

    cpu.PC = 0x0150;
    cpu.instructionCycle = 0;
    cpu.F = 0x0;

    // HL points to the first address in WRAM
    cpu.H = 0xC0;
    cpu.L = 0x00;

    SECTION("Non Zero Result")
    {
        mem.wram[0] = 0x66;

        cpu.op_bit_u3_addr_hl(2);
        REQUIRE(cpu.instructionCycle == 1);
        cpu.op_bit_u3_addr_hl(2);
        REQUIRE(cpu.instructionCycle == 2);
        cpu.op_bit_u3_addr_hl(2);
        REQUIRE(cpu.instructionCycle == 0);
        REQUIRE(cpu.PC == 0x0152);

        REQUIRE(cpu.getZeroFlag() == 0);
        REQUIRE(cpu.getSubtractFlag() == 0);
        REQUIRE(cpu.getHalfCarryFlag() == 1);
    }

    SECTION("Zero Result")
    {
        mem.wram[0] = 0x66;

        cpu.op_bit_u3_addr_hl(3);
        REQUIRE(cpu.instructionCycle == 1);
        cpu.op_bit_u3_addr_hl(3);
        REQUIRE(cpu.instructionCycle == 2);
        cpu.op_bit_u3_addr_hl(3);
        REQUIRE(cpu.instructionCycle == 0);
        REQUIRE(cpu.PC == 0x0152);

        REQUIRE(cpu.getZeroFlag() == 1);
        REQUIRE(cpu.getSubtractFlag() == 0);
        REQUIRE(cpu.getHalfCarryFlag() == 1);
    }
}

TEST_CASE("RES u3, r8", "[OPCODE]")
{
    SM83 cpu;

    cpu.PC = 0x0150;
    cpu.instructionCycle = 0;

    cpu.A = 0xF;

    cpu.op_res_u3_r8(2, cpu.A);
    REQUIRE(cpu.instructionCycle == 1);
    cpu.op_res_u3_r8(2, cpu.A);
    REQUIRE(cpu.instructionCycle == 0);
    REQUIRE(cpu.PC == 0x0152);

    REQUIRE(cpu.A == 0xB);
}

TEST_CASE("RES u3, [HL]", "[OPCODE]")
{
    SM83 cpu;
    Memory mem;
    PPU ppu;

    cpu.memory = &mem;
    mem.ppu = &ppu;
    ppu.memory = &mem;
    ppu.cpu = &cpu;

    cpu.PC = 0x0150;
    cpu.instructionCycle = 0;

    // HL points to first addr in WRAM
    cpu.H = 0xC0;
    cpu.L = 0x00;

    mem.wram[0] = 0xF;

    cpu.op_res_u3_addr_hl(2);
    REQUIRE(cpu.instructionCycle == 1);
    cpu.op_res_u3_addr_hl(2);
    REQUIRE(cpu.instructionCycle == 2);
    cpu.op_res_u3_addr_hl(2);
    REQUIRE(cpu.instructionCycle == 3);
    cpu.op_res_u3_addr_hl(2);
    REQUIRE(cpu.instructionCycle == 0);
    REQUIRE(cpu.PC == 0x0152);

    REQUIRE(mem.wram[0] == 0xB);
}

TEST_CASE("SET u3, r8", "[OPCODE]")
{
    SM83 cpu;

    cpu.PC = 0x0150;
    cpu.instructionCycle = 0;

    cpu.A = 0xB;

    cpu.op_set_u3_r8(2, cpu.A);
    REQUIRE(cpu.instructionCycle == 1);
    cpu.op_set_u3_r8(2, cpu.A);
    REQUIRE(cpu.instructionCycle == 0);
    REQUIRE(cpu.PC == 0x0152);

    REQUIRE(cpu.A == 0xF);
}

TEST_CASE("SET u3, [HL]", "[OPCODE]")
{
    SM83 cpu;
    Memory mem;
    PPU ppu;

    cpu.memory = &mem;
    mem.ppu = &ppu;
    ppu.memory = &mem;
    ppu.cpu = &cpu;

    cpu.PC = 0x0150;
    cpu.instructionCycle = 0;

    // HL points to first addr in WRAM
    cpu.H = 0xC0;
    cpu.L = 0x00;

    mem.wram[0] = 0xB;

    cpu.op_set_u3_addr_hl(2);
    REQUIRE(cpu.instructionCycle == 1);
    cpu.op_set_u3_addr_hl(2);
    REQUIRE(cpu.instructionCycle == 2);
    cpu.op_set_u3_addr_hl(2);
    REQUIRE(cpu.instructionCycle == 3);
    cpu.op_set_u3_addr_hl(2);
    REQUIRE(cpu.instructionCycle == 0);
    REQUIRE(cpu.PC == 0x0152);

    REQUIRE(mem.wram[0] == 0xF);
}

TEST_CASE("SWAP r8", "[OPCODE]")
{
    SM83 cpu;

    cpu.PC = 0x0150;
    cpu.instructionCycle = 0;
    cpu.F = 0x0;

    SECTION("Non Zero Result")
    {
        cpu.A = 0x43;

        cpu.op_swap_r8(cpu.A);
        REQUIRE(cpu.instructionCycle == 1);
        cpu.op_swap_r8(cpu.A);
        REQUIRE(cpu.instructionCycle == 0);
        REQUIRE(cpu.PC == 0x0152);

        REQUIRE(cpu.A == 0x34);
        REQUIRE(cpu.getZeroFlag() == 0);
        REQUIRE(cpu.getSubtractFlag() == 0);
        REQUIRE(cpu.getHalfCarryFlag() == 0);
        REQUIRE(cpu.getCarryFlag() == 0);
    }

    SECTION("Zero Result")
    {
        cpu.A = 0x00;

        cpu.op_swap_r8(cpu.A);
        REQUIRE(cpu.instructionCycle == 1);
        cpu.op_swap_r8(cpu.A);
        REQUIRE(cpu.instructionCycle == 0);
        REQUIRE(cpu.PC == 0x0152);

        REQUIRE(cpu.A == 0x00);
        REQUIRE(cpu.getZeroFlag() == 1);
        REQUIRE(cpu.getSubtractFlag() == 0);
        REQUIRE(cpu.getHalfCarryFlag() == 0);
        REQUIRE(cpu.getCarryFlag() == 0);
    }
}

TEST_CASE("SWAP [HL]", "[OPCODE]")
{
    SM83 cpu;
    Memory mem;
    PPU ppu;

    cpu.memory = &mem;
    mem.ppu = &ppu;
    ppu.memory = &mem;
    ppu.cpu = &cpu;

    cpu.PC = 0x0150;
    cpu.instructionCycle = 0;

    // HL points to first addr in WRAM
    cpu.H = 0xC0;
    cpu.L = 0x00;

    SECTION("Non Zero Result")
    {
        mem.wram[0] = 0x43;

        cpu.op_swap_addr_hl();
        REQUIRE(cpu.instructionCycle == 1);
        cpu.op_swap_addr_hl();
        REQUIRE(cpu.instructionCycle == 2);
        cpu.op_swap_addr_hl();
        REQUIRE(cpu.instructionCycle == 3);
        cpu.op_swap_addr_hl();
        REQUIRE(cpu.instructionCycle == 0);
        REQUIRE(cpu.PC == 0x0152);

        REQUIRE(mem.wram[0] == 0x34);
        REQUIRE(cpu.getZeroFlag() == 0);
        REQUIRE(cpu.getSubtractFlag() == 0);
        REQUIRE(cpu.getHalfCarryFlag() == 0);
        REQUIRE(cpu.getCarryFlag() == 0);
    }

    SECTION("Zero Result")
    {
        mem.wram[0] = 0x00;

        cpu.op_swap_addr_hl();
        REQUIRE(cpu.instructionCycle == 1);
        cpu.op_swap_addr_hl();
        REQUIRE(cpu.instructionCycle == 2);
        cpu.op_swap_addr_hl();
        REQUIRE(cpu.instructionCycle == 3);
        cpu.op_swap_addr_hl();
        REQUIRE(cpu.instructionCycle == 0);
        REQUIRE(cpu.PC == 0x0152);

        REQUIRE(mem.wram[0] == 0x00);
        REQUIRE(cpu.getZeroFlag() == 1);
        REQUIRE(cpu.getSubtractFlag() == 0);
        REQUIRE(cpu.getHalfCarryFlag() == 0);
        REQUIRE(cpu.getCarryFlag() == 0);
    }
}

TEST_CASE("RL r8", "[OPCODE]")
{
    SM83 cpu;

    cpu.PC = 0x0150;
    cpu.instructionCycle = 0;
    cpu.F = 0x0;

    SECTION("Non Zero Result And Carry 0")
    {
        cpu.A = 0x7E;
        cpu.setCarryFlag(1);

        cpu.op_rl_r8(cpu.A);
        REQUIRE(cpu.instructionCycle == 1);
        cpu.op_rl_r8(cpu.A);
        REQUIRE(cpu.instructionCycle == 0);
        REQUIRE(cpu.PC == 0x0152);

        REQUIRE(cpu.A == 0xFD);
        REQUIRE(cpu.getZeroFlag() == 0);
        REQUIRE(cpu.getSubtractFlag() == 0);
        REQUIRE(cpu.getHalfCarryFlag() == 0);
        REQUIRE(cpu.getCarryFlag() == 0);
    }

    SECTION("Zero Result And Carry 1")
    {
        cpu.A = 0x80;
        cpu.setCarryFlag(0);

        cpu.op_rl_r8(cpu.A);
        REQUIRE(cpu.instructionCycle == 1);
        cpu.op_rl_r8(cpu.A);
        REQUIRE(cpu.instructionCycle == 0);
        REQUIRE(cpu.PC == 0x0152);

        REQUIRE(cpu.A == 0x00);
        REQUIRE(cpu.getZeroFlag() == 1);
        REQUIRE(cpu.getSubtractFlag() == 0);
        REQUIRE(cpu.getHalfCarryFlag() == 0);
        REQUIRE(cpu.getCarryFlag() == 1);
    }
}

TEST_CASE("RL [HL]", "[OPCODE]")
{
    SM83 cpu;
    Memory mem;
    PPU ppu;

    cpu.memory = &mem;
    mem.ppu = &ppu;
    ppu.memory = &mem;
    ppu.cpu = &cpu;

    cpu.PC = 0x0150;
    cpu.instructionCycle = 0;
    cpu.F = 0x0;

    // HL points to the first address in WRAM
    cpu.H = 0xC0;
    cpu.L = 0x00;

    SECTION("Non Zero Result And Carry 0")
    {
        mem.wram[0] = 0x7E;
        cpu.setCarryFlag(1);

        cpu.op_rl_addr_hl();
        REQUIRE(cpu.instructionCycle == 1);
        cpu.op_rl_addr_hl();
        REQUIRE(cpu.instructionCycle == 2);
        cpu.op_rl_addr_hl();
        REQUIRE(cpu.instructionCycle == 3);
        cpu.op_rl_addr_hl();
        REQUIRE(cpu.instructionCycle == 0);
        REQUIRE(cpu.PC == 0x0152);

        REQUIRE(mem.wram[0] == 0xFD);
        REQUIRE(cpu.getZeroFlag() == 0);
        REQUIRE(cpu.getSubtractFlag() == 0);
        REQUIRE(cpu.getHalfCarryFlag() == 0);
        REQUIRE(cpu.getCarryFlag() == 0);
    }

    SECTION("Non Zero Result And Carry 0")
    {
        mem.wram[0] = 0x80;
        cpu.setCarryFlag(0);

        cpu.op_rl_addr_hl();
        REQUIRE(cpu.instructionCycle == 1);
        cpu.op_rl_addr_hl();
        REQUIRE(cpu.instructionCycle == 2);
        cpu.op_rl_addr_hl();
        REQUIRE(cpu.instructionCycle == 3);
        cpu.op_rl_addr_hl();
        REQUIRE(cpu.instructionCycle == 0);
        REQUIRE(cpu.PC == 0x0152);

        REQUIRE(mem.wram[0] == 0x00);
        REQUIRE(cpu.getZeroFlag() == 1);
        REQUIRE(cpu.getSubtractFlag() == 0);
        REQUIRE(cpu.getHalfCarryFlag() == 0);
        REQUIRE(cpu.getCarryFlag() == 1);
    }
}

TEST_CASE("RLA", "[OPCODE]")
{
    SM83 cpu;

    cpu.PC = 0x0150;
    cpu.instructionCycle = 0;
    cpu.F = 0x0;

    SECTION("Carry 1")
    {
        cpu.A = 0x80;
        cpu.setCarryFlag(0);

        cpu.op_rla();
        REQUIRE(cpu.PC == 0x0151);

        REQUIRE(cpu.A == 0x00);
        REQUIRE(cpu.getZeroFlag() == 0);
        REQUIRE(cpu.getSubtractFlag() == 0);
        REQUIRE(cpu.getHalfCarryFlag() == 0);
        REQUIRE(cpu.getCarryFlag() == 1);
    }

    SECTION("Carry 0")
    {
        cpu.A = 0x7E;
        cpu.setCarryFlag(1);

        cpu.op_rla();
        REQUIRE(cpu.PC == 0x0151);

        REQUIRE(cpu.A == 0xFD);
        REQUIRE(cpu.getZeroFlag() == 0);
        REQUIRE(cpu.getSubtractFlag() == 0);
        REQUIRE(cpu.getHalfCarryFlag() == 0);
        REQUIRE(cpu.getCarryFlag() == 0);
    }
}

TEST_CASE("RLC r8", "[OPCODE]")
{
    SM83 cpu;

    cpu.PC = 0x0150;
    cpu.instructionCycle = 0;
    cpu.F = 0x0;

    SECTION("Non Zero Result and Carry 1")
    {
        cpu.A = 0x80;

        cpu.op_rlc_r8(cpu.A);
        REQUIRE(cpu.instructionCycle == 1);
        cpu.op_rlc_r8(cpu.A);
        REQUIRE(cpu.instructionCycle == 0);
        REQUIRE(cpu.PC == 0x0152);

        REQUIRE(cpu.A == 0x01);
        REQUIRE(cpu.getZeroFlag() == 0);
        REQUIRE(cpu.getSubtractFlag() == 0);
        REQUIRE(cpu.getHalfCarryFlag() == 0);
        REQUIRE(cpu.getCarryFlag() == 1);
    }

    SECTION("Zero Result and Carry 0")
    {
        cpu.A = 0x00;

        cpu.op_rlc_r8(cpu.A);
        REQUIRE(cpu.instructionCycle == 1);
        cpu.op_rlc_r8(cpu.A);
        REQUIRE(cpu.instructionCycle == 0);
        REQUIRE(cpu.PC == 0x0152);

        REQUIRE(cpu.A == 0x00);
        REQUIRE(cpu.getZeroFlag() == 1);
        REQUIRE(cpu.getSubtractFlag() == 0);
        REQUIRE(cpu.getHalfCarryFlag() == 0);
        REQUIRE(cpu.getCarryFlag() == 0);
    }
}

TEST_CASE("RLC [HL]", "[OPCODE]")
{
    SM83 cpu;
    Memory mem;
    PPU ppu;

    cpu.memory = &mem;
    mem.ppu = &ppu;
    ppu.memory = &mem;
    ppu.cpu = &cpu;

    cpu.PC = 0x0150;
    cpu.instructionCycle = 0;
    cpu.F = 0x0;

    // HL points to the first address in WRAM
    cpu.H = 0xC0;
    cpu.L = 0x00;

    SECTION("Non Zero Result and Carry 1")
    {
        mem.wram[0] = 0x80;

        cpu.op_rlc_addr_hl();
        REQUIRE(cpu.instructionCycle == 1);
        cpu.op_rlc_addr_hl();
        REQUIRE(cpu.instructionCycle == 2);
        cpu.op_rlc_addr_hl();
        REQUIRE(cpu.instructionCycle == 3);
        cpu.op_rlc_addr_hl();
        REQUIRE(cpu.instructionCycle == 0);
        REQUIRE(cpu.PC == 0x0152);

        REQUIRE(mem.wram[0] == 0x01);
        REQUIRE(cpu.getZeroFlag() == 0);
        REQUIRE(cpu.getSubtractFlag() == 0);
        REQUIRE(cpu.getHalfCarryFlag() == 0);
        REQUIRE(cpu.getCarryFlag() == 1);
    }

    SECTION("Zero Result and Carry 0")
    {
        mem.wram[0] = 0x00;

        cpu.op_rlc_addr_hl();
        REQUIRE(cpu.instructionCycle == 1);
        cpu.op_rlc_addr_hl();
        REQUIRE(cpu.instructionCycle == 2);
        cpu.op_rlc_addr_hl();
        REQUIRE(cpu.instructionCycle == 3);
        cpu.op_rlc_addr_hl();
        REQUIRE(cpu.instructionCycle == 0);
        REQUIRE(cpu.PC == 0x0152);

        REQUIRE(mem.wram[0] == 0x00);
        REQUIRE(cpu.getZeroFlag() == 1);
        REQUIRE(cpu.getSubtractFlag() == 0);
        REQUIRE(cpu.getHalfCarryFlag() == 0);
        REQUIRE(cpu.getCarryFlag() == 0);
    }
}

TEST_CASE("RLCA", "[OPCODE]")
{
    SM83 cpu;

    cpu.PC = 0x0150;
    cpu.instructionCycle = 0;
    cpu.F = 0x0;

    SECTION("Carry 1")
    {
        cpu.A = 0x80;

        cpu.op_rlca();
        REQUIRE(cpu.PC == 0x0151);

        REQUIRE(cpu.A == 0x01);
        REQUIRE(cpu.getZeroFlag() == 0);
        REQUIRE(cpu.getSubtractFlag() == 0);
        REQUIRE(cpu.getHalfCarryFlag() == 0);
        REQUIRE(cpu.getCarryFlag() == 1);
    }

    SECTION("Carry 0")
    {
        cpu.A = 0x00;

        cpu.op_rlca();
        REQUIRE(cpu.PC == 0x0151);

        REQUIRE(cpu.A == 0x00);
        REQUIRE(cpu.getZeroFlag() == 0);
        REQUIRE(cpu.getSubtractFlag() == 0);
        REQUIRE(cpu.getHalfCarryFlag() == 0);
        REQUIRE(cpu.getCarryFlag() == 0);
    }
}

TEST_CASE("RR r8", "[OPCODE]")
{
    SM83 cpu;

    cpu.PC = 0x0150;
    cpu.instructionCycle = 0;
    cpu.F = 0x0;

    SECTION("Non Zero Result and Carry Flag 0")
    {
        cpu.A = 0x02;
        cpu.setCarryFlag(1);

        cpu.op_rr_r8(cpu.A);
        REQUIRE(cpu.instructionCycle == 1);
        cpu.op_rr_r8(cpu.A);
        REQUIRE(cpu.instructionCycle == 0);
        REQUIRE(cpu.PC == 0x0152);

        REQUIRE(cpu.A == 0x81);
        REQUIRE(cpu.getZeroFlag() == 0);
        REQUIRE(cpu.getSubtractFlag() == 0);
        REQUIRE(cpu.getHalfCarryFlag() == 0);
        REQUIRE(cpu.getCarryFlag() == 0);
    }

    SECTION("Zero Result and Carry Flag 1")
    {
        cpu.A = 0x01;
        cpu.setCarryFlag(0);

        cpu.op_rr_r8(cpu.A);
        REQUIRE(cpu.instructionCycle == 1);
        cpu.op_rr_r8(cpu.A);
        REQUIRE(cpu.instructionCycle == 0);
        REQUIRE(cpu.PC == 0x0152);

        REQUIRE(cpu.A == 0x00);
        REQUIRE(cpu.getZeroFlag() == 1);
        REQUIRE(cpu.getSubtractFlag() == 0);
        REQUIRE(cpu.getHalfCarryFlag() == 0);
        REQUIRE(cpu.getCarryFlag() == 1);
    }
}

TEST_CASE("RR [HL]", "[OPCODE]")
{
    SM83 cpu;
    Memory mem;
    PPU ppu;

    cpu.memory = &mem;
    mem.ppu = &ppu;
    ppu.memory = &mem;
    ppu.cpu = &cpu;

    cpu.PC = 0x0150;
    cpu.instructionCycle = 0;
    cpu.F = 0x0;

    // HL points to the first address in WRAM
    cpu.H = 0xC0;
    cpu.L = 0x00;

    SECTION("Non Zero Result and Carry Flag 0")
    {
        mem.wram[0] = 0x02;
        cpu.setCarryFlag(1);

        cpu.op_rr_addr_hl();
        REQUIRE(cpu.instructionCycle == 1);
        cpu.op_rr_addr_hl();
        REQUIRE(cpu.instructionCycle == 2);
        cpu.op_rr_addr_hl();
        REQUIRE(cpu.instructionCycle == 3);
        cpu.op_rr_addr_hl();
        REQUIRE(cpu.instructionCycle == 0);
        REQUIRE(cpu.PC == 0x0152);

        REQUIRE(mem.wram[0] == 0x81);
        REQUIRE(cpu.getZeroFlag() == 0);
        REQUIRE(cpu.getSubtractFlag() == 0);
        REQUIRE(cpu.getHalfCarryFlag() == 0);
        REQUIRE(cpu.getCarryFlag() == 0);
    }

    SECTION("Non Zero Result and Carry Flag 0")
    {
        mem.wram[0] = 0x01;
        cpu.setCarryFlag(0);

        cpu.op_rr_addr_hl();
        REQUIRE(cpu.instructionCycle == 1);
        cpu.op_rr_addr_hl();
        REQUIRE(cpu.instructionCycle == 2);
        cpu.op_rr_addr_hl();
        REQUIRE(cpu.instructionCycle == 3);
        cpu.op_rr_addr_hl();
        REQUIRE(cpu.instructionCycle == 0);
        REQUIRE(cpu.PC == 0x0152);

        REQUIRE(mem.wram[0] == 0x00);
        REQUIRE(cpu.getZeroFlag() == 1);
        REQUIRE(cpu.getSubtractFlag() == 0);
        REQUIRE(cpu.getHalfCarryFlag() == 0);
        REQUIRE(cpu.getCarryFlag() == 1);
    }
}

TEST_CASE("RRA", "[OPCODE]")
{
    SM83 cpu;

    cpu.PC = 0x0150;
    cpu.instructionCycle = 0;
    cpu.F = 0x0;

    SECTION("Carry Flag 0")
    {
        cpu.A = 0x02;
        cpu.setCarryFlag(1);

        cpu.op_rra();
        REQUIRE(cpu.PC == 0x0151);

        REQUIRE(cpu.A == 0x81);
        REQUIRE(cpu.getZeroFlag() == 0);
        REQUIRE(cpu.getSubtractFlag() == 0);
        REQUIRE(cpu.getHalfCarryFlag() == 0);
        REQUIRE(cpu.getCarryFlag() == 0);
    }

    SECTION("Carry Flag 1")
    {
        cpu.A = 0x01;
        cpu.setCarryFlag(1);

        cpu.op_rra();
        REQUIRE(cpu.PC == 0x0151);

        REQUIRE(cpu.A == 0x80);
        REQUIRE(cpu.getZeroFlag() == 0);
        REQUIRE(cpu.getSubtractFlag() == 0);
        REQUIRE(cpu.getHalfCarryFlag() == 0);
        REQUIRE(cpu.getCarryFlag() == 1);
    }
}

TEST_CASE("RRC r8", "[OPCODE]")
{
    SM83 cpu;

    cpu.PC = 0x0150;
    cpu.instructionCycle = 0;
    cpu.F = 0x0;

    SECTION("Non Zero Result and Carry Flag 1")
    {
        cpu.A = 0x01;

        cpu.op_rrc_r8(cpu.A);
        REQUIRE(cpu.instructionCycle == 1);
        cpu.op_rrc_r8(cpu.A);
        REQUIRE(cpu.instructionCycle == 0);
        REQUIRE(cpu.PC == 0x0152);

        REQUIRE(cpu.A == 0x80);
        REQUIRE(cpu.getZeroFlag() == 0);
        REQUIRE(cpu.getSubtractFlag() == 0);
        REQUIRE(cpu.getHalfCarryFlag() == 0);
        REQUIRE(cpu.getCarryFlag() == 1);
    }

    SECTION("Zero Result and Carry Flag 0")
    {
        cpu.A = 0x00;

        cpu.op_rrc_r8(cpu.A);
        REQUIRE(cpu.instructionCycle == 1);
        cpu.op_rrc_r8(cpu.A);
        REQUIRE(cpu.instructionCycle == 0);
        REQUIRE(cpu.PC == 0x0152);

        REQUIRE(cpu.A == 0x00);
        REQUIRE(cpu.getZeroFlag() == 1);
        REQUIRE(cpu.getSubtractFlag() == 0);
        REQUIRE(cpu.getHalfCarryFlag() == 0);
        REQUIRE(cpu.getCarryFlag() == 0);
    }
}

TEST_CASE("RRC [HL]", "[OPCODE]")
{
    SM83 cpu;
    Memory mem;
    PPU ppu;

    cpu.memory = &mem;
    mem.ppu = &ppu;
    ppu.memory = &mem;
    ppu.cpu = &cpu;

    cpu.PC = 0x0150;
    cpu.instructionCycle = 0;
    cpu.F = 0x0;

    // HL points to the first address in WRAM
    cpu.H = 0xC0;
    cpu.L = 0x00;

    SECTION("Non Zero Result and Carry Flag 1")
    {
        mem.wram[0] = 0x01;

        cpu.op_rrc_addr_hl();
        REQUIRE(cpu.instructionCycle == 1);
        cpu.op_rrc_addr_hl();
        REQUIRE(cpu.instructionCycle == 2);
        cpu.op_rrc_addr_hl();
        REQUIRE(cpu.instructionCycle == 3);
        cpu.op_rrc_addr_hl();
        REQUIRE(cpu.instructionCycle == 0);
        REQUIRE(cpu.PC == 0x0152);

        REQUIRE(mem.wram[0] == 0x80);
        REQUIRE(cpu.getZeroFlag() == 0);
        REQUIRE(cpu.getSubtractFlag() == 0);
        REQUIRE(cpu.getHalfCarryFlag() == 0);
        REQUIRE(cpu.getCarryFlag() == 1);
    }

    SECTION("Zero REsult and Carry Flag 0")
    {
        mem.wram[0] = 0x00;

        cpu.op_rrc_addr_hl();
        REQUIRE(cpu.instructionCycle == 1);
        cpu.op_rrc_addr_hl();
        REQUIRE(cpu.instructionCycle == 2);
        cpu.op_rrc_addr_hl();
        REQUIRE(cpu.instructionCycle == 3);
        cpu.op_rrc_addr_hl();
        REQUIRE(cpu.instructionCycle == 0);
        REQUIRE(cpu.PC == 0x0152);

        REQUIRE(mem.wram[0] == 0x00);
        REQUIRE(cpu.getZeroFlag() == 1);
        REQUIRE(cpu.getSubtractFlag() == 0);
        REQUIRE(cpu.getHalfCarryFlag() == 0);
        REQUIRE(cpu.getCarryFlag() == 0);
    }
}

TEST_CASE("RRCA", "[OPCODE]")
{
    SM83 cpu;
    Memory mem;
    PPU ppu;

    cpu.memory = &mem;
    mem.ppu = &ppu;
    ppu.memory = &mem;
    ppu.cpu = &cpu;

    cpu.PC = 0x0150;
    cpu.instructionCycle = 0;
    cpu.F = 0x0;

    SECTION("Carry Flag 1")
    {
        cpu.A = 0x01;

        cpu.op_rrca();
        REQUIRE(cpu.PC == 0x0151);

        REQUIRE(cpu.A == 0x80);
        REQUIRE(cpu.getZeroFlag() == 0);
        REQUIRE(cpu.getSubtractFlag() == 0);
        REQUIRE(cpu.getHalfCarryFlag() == 0);
        REQUIRE(cpu.getCarryFlag() == 1);
    }

    SECTION("Carry Flag 0")
    {
        cpu.A = 0x00;

        cpu.op_rrca();
        REQUIRE(cpu.PC == 0x0151);

        REQUIRE(cpu.A == 0x00);
        REQUIRE(cpu.getZeroFlag() == 0);
        REQUIRE(cpu.getSubtractFlag() == 0);
        REQUIRE(cpu.getHalfCarryFlag() == 0);
        REQUIRE(cpu.getCarryFlag() == 0);
    }
}

TEST_CASE("SLA r8", "[OPCODE]")
{
    SM83 cpu;

    cpu.PC = 0x0150;
    cpu.instructionCycle = 0;
    cpu.F = 0x0;

    SECTION("Non Zero Result and Carry Flag 0")
    {
        cpu.A = 0x01;

        cpu.op_sla_r8(cpu.A);
        REQUIRE(cpu.instructionCycle == 1);
        cpu.op_sla_r8(cpu.A);
        REQUIRE(cpu.instructionCycle == 0);
        REQUIRE(cpu.PC == 0x0152);

        REQUIRE(cpu.A == 0x02);
        REQUIRE(cpu.getZeroFlag() == 0);
        REQUIRE(cpu.getSubtractFlag() == 0);
        REQUIRE(cpu.getHalfCarryFlag() == 0);
        REQUIRE(cpu.getCarryFlag() == 0);
    }

    SECTION("Zero Result and Carry Flag 1")
    {
        cpu.A = 0x80;

        cpu.op_sla_r8(cpu.A);
        REQUIRE(cpu.instructionCycle == 1);
        cpu.op_sla_r8(cpu.A);
        REQUIRE(cpu.instructionCycle == 0);
        REQUIRE(cpu.PC == 0x0152);

        REQUIRE(cpu.A == 0x00);
        REQUIRE(cpu.getZeroFlag() == 1);
        REQUIRE(cpu.getSubtractFlag() == 0);
        REQUIRE(cpu.getHalfCarryFlag() == 0);
        REQUIRE(cpu.getCarryFlag() == 1);
    }
}

TEST_CASE("SLA [HL]", "[OPCODE]")
{
    SM83 cpu;
    Memory mem;
    PPU ppu;

    cpu.memory = &mem;
    mem.ppu = &ppu;
    ppu.memory = &mem;
    ppu.cpu = &cpu;

    cpu.PC = 0x0150;
    cpu.instructionCycle = 0;
    cpu.F = 0x0;

    // HL points to the first address in WRAM
    cpu.H = 0xC0;
    cpu.L = 0x00;

    SECTION("Non Zero Result and Carry Flag 0")
    {
        mem.wram[0] = 0x01;

        cpu.op_sla_addr_hl();
        REQUIRE(cpu.instructionCycle == 1);
        cpu.op_sla_addr_hl();
        REQUIRE(cpu.instructionCycle == 2);
        cpu.op_sla_addr_hl();
        REQUIRE(cpu.instructionCycle == 3);
        cpu.op_sla_addr_hl();
        REQUIRE(cpu.instructionCycle == 0);
        REQUIRE(cpu.PC == 0x0152);

        REQUIRE(mem.wram[0] == 0x02);
        REQUIRE(cpu.getZeroFlag() == 0);
        REQUIRE(cpu.getSubtractFlag() == 0);
        REQUIRE(cpu.getHalfCarryFlag() == 0);
        REQUIRE(cpu.getCarryFlag() == 0);
    }

    SECTION("Zero Result and Carry Flag 1")
    {
        mem.wram[0] = 0x80;

        cpu.op_sla_addr_hl();
        REQUIRE(cpu.instructionCycle == 1);
        cpu.op_sla_addr_hl();
        REQUIRE(cpu.instructionCycle == 2);
        cpu.op_sla_addr_hl();
        REQUIRE(cpu.instructionCycle == 3);
        cpu.op_sla_addr_hl();
        REQUIRE(cpu.instructionCycle == 0);
        REQUIRE(cpu.PC == 0x0152);

        REQUIRE(mem.wram[0] == 0x00);
        REQUIRE(cpu.getZeroFlag() == 1);
        REQUIRE(cpu.getSubtractFlag() == 0);
        REQUIRE(cpu.getHalfCarryFlag() == 0);
        REQUIRE(cpu.getCarryFlag() == 1);
    }
}

TEST_CASE("SRA r8", "[OPCODE]")
{
    SM83 cpu;

    cpu.PC = 0x0150;
    cpu.instructionCycle = 0;
    cpu.F = 0x0;

    SECTION("Non Zero Result and Carry Flag 0")
    {
        cpu.A = 0x80;

        cpu.op_sra_r8(cpu.A);
        REQUIRE(cpu.instructionCycle == 1);
        cpu.op_sra_r8(cpu.A);
        REQUIRE(cpu.instructionCycle == 0);
        REQUIRE(cpu.PC == 0x0152);

        REQUIRE(cpu.A == 0xC0);
        REQUIRE(cpu.getZeroFlag() == 0);
        REQUIRE(cpu.getSubtractFlag() == 0);
        REQUIRE(cpu.getHalfCarryFlag() == 0);
        REQUIRE(cpu.getCarryFlag() == 0);
    }

    SECTION("Zero Result and Carry Flag 1")
    {
        cpu.A = 0x01;

        cpu.op_sra_r8(cpu.A);
        REQUIRE(cpu.instructionCycle == 1);
        cpu.op_sra_r8(cpu.A);
        REQUIRE(cpu.instructionCycle == 0);
        REQUIRE(cpu.PC == 0x0152);

        REQUIRE(cpu.A == 0x00);
        REQUIRE(cpu.getZeroFlag() == 1);
        REQUIRE(cpu.getSubtractFlag() == 0);
        REQUIRE(cpu.getHalfCarryFlag() == 0);
        REQUIRE(cpu.getCarryFlag() == 1);
    }
}

TEST_CASE("SRA [HL]", "[OPCODE]")
{
    SM83 cpu;
    Memory mem;
    PPU ppu;

    cpu.memory = &mem;
    mem.ppu = &ppu;
    ppu.memory = &mem;
    ppu.cpu = &cpu;

    cpu.PC = 0x0150;
    cpu.instructionCycle = 0;
    cpu.F = 0x0;

    // HL points to the first address in WRAM
    cpu.H = 0xC0;
    cpu.L = 0x00;

    SECTION("Non Zero Result and Carry Flag 0")
    {
        mem.wram[0] = 0x80;

        cpu.op_sra_addr_hl();
        REQUIRE(cpu.instructionCycle == 1);
        cpu.op_sra_addr_hl();
        REQUIRE(cpu.instructionCycle == 2);
        cpu.op_sra_addr_hl();
        REQUIRE(cpu.instructionCycle == 3);
        cpu.op_sra_addr_hl();
        REQUIRE(cpu.instructionCycle == 0);
        REQUIRE(cpu.PC == 0x0152);

        REQUIRE(mem.wram[0] == 0xC0);
        REQUIRE(cpu.getZeroFlag() == 0);
        REQUIRE(cpu.getSubtractFlag() == 0);
        REQUIRE(cpu.getHalfCarryFlag() == 0);
        REQUIRE(cpu.getCarryFlag() == 0);
    }

    SECTION("Zero Result and Carry Flag 1")
    {
        mem.wram[0] = 0x01;

        cpu.op_sra_addr_hl();
        REQUIRE(cpu.instructionCycle == 1);
        cpu.op_sra_addr_hl();
        REQUIRE(cpu.instructionCycle == 2);
        cpu.op_sra_addr_hl();
        REQUIRE(cpu.instructionCycle == 3);
        cpu.op_sra_addr_hl();
        REQUIRE(cpu.instructionCycle == 0);
        REQUIRE(cpu.PC == 0x0152);

        REQUIRE(mem.wram[0] == 0x00);
        REQUIRE(cpu.getZeroFlag() == 1);
        REQUIRE(cpu.getSubtractFlag() == 0);
        REQUIRE(cpu.getHalfCarryFlag() == 0);
        REQUIRE(cpu.getCarryFlag() == 1);
    }
}

TEST_CASE("SRL r8", "[OPCODE]")
{
    SM83 cpu;

    cpu.PC = 0x0150;
    cpu.instructionCycle = 0;
    cpu.F = 0x0;

    SECTION("Non Zero Result and Carry Flag 0")
    {
        cpu.A = 0x82;

        cpu.op_srl_r8(cpu.A);
        REQUIRE(cpu.instructionCycle == 1);
        cpu.op_srl_r8(cpu.A);
        REQUIRE(cpu.instructionCycle == 0);
        REQUIRE(cpu.PC == 0x0152);

        REQUIRE(cpu.A == 0x41);
        REQUIRE(cpu.getZeroFlag() == 0);
        REQUIRE(cpu.getSubtractFlag() == 0);
        REQUIRE(cpu.getHalfCarryFlag() == 0);
        REQUIRE(cpu.getCarryFlag() == 0);
    }

    SECTION("Zero Result and Carry Flag 1")
    {
        cpu.A = 0x01;

        cpu.op_srl_r8(cpu.A);
        REQUIRE(cpu.instructionCycle == 1);
        cpu.op_srl_r8(cpu.A);
        REQUIRE(cpu.instructionCycle == 0);
        REQUIRE(cpu.PC == 0x0152);

        REQUIRE(cpu.A == 0x00);
        REQUIRE(cpu.getZeroFlag() == 1);
        REQUIRE(cpu.getSubtractFlag() == 0);
        REQUIRE(cpu.getHalfCarryFlag() == 0);
        REQUIRE(cpu.getCarryFlag() == 1);
    }
}

TEST_CASE("SRL [HL]", "[OPCODE]")
{
    SM83 cpu;
    Memory mem;
    PPU ppu;

    cpu.memory = &mem;
    mem.ppu = &ppu;
    ppu.memory = &mem;
    ppu.cpu = &cpu;

    cpu.PC = 0x0150;
    cpu.instructionCycle = 0;
    cpu.F = 0x0;

    // HL points to the first address in WRAM
    cpu.H = 0xC0;
    cpu.L = 0x00;

    SECTION("Non Zero Result and Carry Flag 0")
    {
        mem.wram[0] = 0x82;

        cpu.op_srl_addr_hl();
        REQUIRE(cpu.instructionCycle == 1);
        cpu.op_srl_addr_hl();
        REQUIRE(cpu.instructionCycle == 2);
        cpu.op_srl_addr_hl();
        REQUIRE(cpu.instructionCycle == 3);
        cpu.op_srl_addr_hl();
        REQUIRE(cpu.instructionCycle == 0);
        REQUIRE(cpu.PC == 0x0152);

        REQUIRE(mem.wram[0] == 0x41);
        REQUIRE(cpu.getZeroFlag() == 0);
        REQUIRE(cpu.getSubtractFlag() == 0);
        REQUIRE(cpu.getHalfCarryFlag() == 0);
        REQUIRE(cpu.getCarryFlag() == 0);
    }

    SECTION("Zero Result and Carry Flag 1")
    {
        mem.wram[0] = 0x01;

        cpu.op_srl_addr_hl();
        REQUIRE(cpu.instructionCycle == 1);
        cpu.op_srl_addr_hl();
        REQUIRE(cpu.instructionCycle == 2);
        cpu.op_srl_addr_hl();
        REQUIRE(cpu.instructionCycle == 3);
        cpu.op_srl_addr_hl();
        REQUIRE(cpu.instructionCycle == 0);
        REQUIRE(cpu.PC == 0x0152);

        REQUIRE(mem.wram[0] == 0x00);
        REQUIRE(cpu.getZeroFlag() == 1);
        REQUIRE(cpu.getSubtractFlag() == 0);
        REQUIRE(cpu.getHalfCarryFlag() == 0);
        REQUIRE(cpu.getCarryFlag() == 1);
    }
}

TEST_CASE("LD r8, r8", "[OPCODE]")
{
    SM83 cpu;

    cpu.PC = 0x0150;
    cpu.instructionCycle = 0;

    cpu.A = 0x01;
    cpu.B = 0x02;

    cpu.op_ld_r8_r8(cpu.A, cpu.B);
    REQUIRE(cpu.PC == 0x0151);

    REQUIRE(cpu.A == cpu.B);
}

TEST_CASE("LD r8, n8", "[OPCODE]")
{
    SM83 cpu;
    Memory mem;
    PPU ppu;

    cpu.memory = &mem;
    mem.ppu = &ppu;
    ppu.memory = &mem;
    ppu.cpu = &cpu;

    cpu.PC = 0xC000;
    cpu.instructionCycle = 0;

    mem.wram[1] = 0x66;

    cpu.op_ld_r8_n8(cpu.A);
    REQUIRE(cpu.instructionCycle == 1);
    cpu.op_ld_r8_n8(cpu.A);
    REQUIRE(cpu.instructionCycle == 0);
    REQUIRE(cpu.PC == 0xC002);

    REQUIRE(cpu.A == 0x66);
}

TEST_CASE("LD r16, n16", "[OPCODE]")
{
    SM83 cpu;
    Memory mem;
    PPU ppu;

    cpu.memory = &mem;
    mem.ppu = &ppu;
    ppu.memory = &mem;
    ppu.cpu = &cpu;

    cpu.PC = 0xC000;
    cpu.instructionCycle = 0;

    mem.wram[1] = 0xAD;
    mem.wram[2] = 0xDE;

    cpu.op_ld_r16_n16(cpu.B, cpu.C);
    REQUIRE(cpu.instructionCycle == 1);
    cpu.op_ld_r16_n16(cpu.B, cpu.C);
    REQUIRE(cpu.instructionCycle == 2);
    cpu.op_ld_r16_n16(cpu.B, cpu.C);
    REQUIRE(cpu.instructionCycle == 0);
    REQUIRE(cpu.PC == 0xC003);

    REQUIRE(cpu.B == 0xDE);
    REQUIRE(cpu.C == 0xAD);
}

TEST_CASE("LD [HL], r8", "[OPCODE]")
{
    SM83 cpu;
    Memory mem;
    PPU ppu;

    cpu.memory = &mem;
    mem.ppu = &ppu;
    ppu.memory = &mem;
    ppu.cpu = &cpu;

    cpu.PC = 0x0150;
    cpu.instructionCycle = 0;
    cpu.F = 0x0;

    // HL points to the first address in WRAM
    cpu.H = 0xC0;
    cpu.L = 0x00;

    cpu.A = 0x66;

    cpu.op_ld_addr_hl_r8(cpu.A);
    REQUIRE(cpu.instructionCycle == 1);
    cpu.op_ld_addr_hl_r8(cpu.A);
    REQUIRE(cpu.instructionCycle == 0);
    REQUIRE(cpu.PC == 0x0151);

    REQUIRE(mem.wram[0] == cpu.A);
}

TEST_CASE("LD [HL], n8", "[OPCODE]")
{
    SM83 cpu;
    Memory mem;
    PPU ppu;

    cpu.memory = &mem;
    mem.ppu = &ppu;
    ppu.memory = &mem;
    ppu.cpu = &cpu;

    cpu.PC = 0xC002;
    cpu.instructionCycle = 0;
    cpu.F = 0x0;

    // HL points to the first address in WRAM
    cpu.H = 0xC0;
    cpu.L = 0x00;

    mem.wram[3] = 0x66;

    cpu.op_ld_addr_hl_n8();
    REQUIRE(cpu.instructionCycle == 1);
    cpu.op_ld_addr_hl_n8();
    REQUIRE(cpu.instructionCycle == 2);
    cpu.op_ld_addr_hl_n8();
    REQUIRE(cpu.instructionCycle == 0);
    REQUIRE(cpu.PC == 0xC004);

    REQUIRE(mem.wram[0] == mem.wram[3]);
}

TEST_CASE("LD r8, [HL]", "[OPCODE]")
{
    SM83 cpu;
    Memory mem;
    PPU ppu;

    cpu.memory = &mem;
    mem.ppu = &ppu;
    ppu.memory = &mem;
    ppu.cpu = &cpu;

    cpu.PC = 0x0150;
    cpu.instructionCycle = 0;
    cpu.F = 0x0;

    // HL points to the first address in WRAM
    cpu.H = 0xC0;
    cpu.L = 0x00;

    mem.wram[0] = 0x66;

    cpu.op_ld_r8_addr_hl(cpu.A);
    REQUIRE(cpu.instructionCycle == 1);
    cpu.op_ld_r8_addr_hl(cpu.A);
    REQUIRE(cpu.instructionCycle == 0);
    REQUIRE(cpu.PC == 0x0151);

    REQUIRE(cpu.A == mem.wram[0]);
}

TEST_CASE("LD [r16], A", "[OPCODE]")
{
    SM83 cpu;
    Memory mem;
    PPU ppu;

    cpu.memory = &mem;
    mem.ppu = &ppu;
    ppu.memory = &mem;
    ppu.cpu = &cpu;

    cpu.PC = 0x0150;
    cpu.instructionCycle = 0;
    cpu.F = 0x0;

    // BC points to the first address in WRAM
    cpu.B = 0xC0;
    cpu.C = 0x00;

    cpu.A = 0x66;

    cpu.op_ld_addr_r16_a(cpu.B, cpu.C);
    REQUIRE(cpu.instructionCycle == 1);
    cpu.op_ld_addr_r16_a(cpu.B, cpu.C);
    REQUIRE(cpu.instructionCycle == 0);
    REQUIRE(cpu.PC == 0x0151);

    REQUIRE(mem.wram[0] == cpu.A);
}

TEST_CASE("LD [n16], A", "[OPCODE]")
{
    SM83 cpu;
    Memory mem;
    PPU ppu;

    cpu.memory = &mem;
    mem.ppu = &ppu;
    ppu.memory = &mem;
    ppu.cpu = &cpu;

    cpu.PC = 0xC002;
    cpu.instructionCycle = 0;
    cpu.F = 0x0;

    mem.wram[3] = 0x00;
    mem.wram[4] = 0xC0;

    cpu.A = 0x66;

    cpu.op_ld_addr_n16_a();
    REQUIRE(cpu.instructionCycle == 1);
    cpu.op_ld_addr_n16_a();
    REQUIRE(cpu.instructionCycle == 2);
    cpu.op_ld_addr_n16_a();
    REQUIRE(cpu.instructionCycle == 3);
    cpu.op_ld_addr_n16_a();
    REQUIRE(cpu.instructionCycle == 0);
    REQUIRE(cpu.PC == 0xC005);

    REQUIRE(mem.wram[0] == cpu.A);
}

TEST_CASE("LDH [n16], A", "[OPCODE]")
{
    SM83 cpu;
    Memory mem;
    PPU ppu;

    cpu.memory = &mem;
    mem.ppu = &ppu;
    ppu.memory = &mem;
    ppu.cpu = &cpu;

    cpu.PC = 0xC000;
    cpu.instructionCycle = 0;
    cpu.F = 0x0;

    mem.wram[1] = 0x66;

    cpu.A = 0x66;

    cpu.op_ldh_addr_n16_a();
    REQUIRE(cpu.instructionCycle == 1);
    cpu.op_ldh_addr_n16_a();
    REQUIRE(cpu.instructionCycle == 2);
    cpu.op_ldh_addr_n16_a();
    REQUIRE(cpu.instructionCycle == 0);
    REQUIRE(cpu.PC == 0xC002);

    REQUIRE(mem.readmem(0xFF66) == cpu.A);
}

TEST_CASE("LDH [C], A", "[OPCODE]")
{
    SM83 cpu;
    Memory mem;
    PPU ppu;

    cpu.memory = &mem;
    mem.ppu = &ppu;
    ppu.memory = &mem;
    ppu.cpu = &cpu;

    cpu.PC = 0x0150;
    cpu.instructionCycle = 0;
    cpu.F = 0x0;

    cpu.C = 0x66;
    cpu.A = 0x66;

    cpu.op_ldh_addr_c_a();
    REQUIRE(cpu.instructionCycle == 1);
    cpu.op_ldh_addr_c_a();
    REQUIRE(cpu.instructionCycle == 0);
    REQUIRE(cpu.PC == 0x0151);

    REQUIRE(mem.readmem(0xFF66) == cpu.A);
}

TEST_CASE("LD A, [r16]", "[OPCODE]")
{
    SM83 cpu;
    Memory mem;
    PPU ppu;

    cpu.memory = &mem;
    mem.ppu = &ppu;
    ppu.memory = &mem;
    ppu.cpu = &cpu;

    cpu.PC = 0x0150;
    cpu.instructionCycle = 0;
    cpu.F = 0x0;

    // BC points to the first address in WRAM
    cpu.B = 0xC0;
    cpu.C = 0x00;

    mem.wram[0] = 0x66;

    cpu.op_ld_a_addr_r16(cpu.B, cpu.C);
    REQUIRE(cpu.instructionCycle == 1);
    cpu.op_ld_a_addr_r16(cpu.B, cpu.C);
    REQUIRE(cpu.instructionCycle == 0);
    REQUIRE(cpu.PC == 0x0151);

    REQUIRE(cpu.A == mem.wram[0]);
}

TEST_CASE("LD A, [n16]", "[OPCODE]")
{
    SM83 cpu;
    Memory mem;
    PPU ppu;

    cpu.memory = &mem;
    mem.ppu = &ppu;
    ppu.memory = &mem;
    ppu.cpu = &cpu;

    cpu.PC = 0xC002;
    cpu.instructionCycle = 0;

    mem.wram[3] = 0x00;
    mem.wram[4] = 0xC0;

    mem.wram[0] = 0x66;

    cpu.op_ld_a_addr_n16();
    REQUIRE(cpu.instructionCycle == 1);
    cpu.op_ld_a_addr_n16();
    REQUIRE(cpu.instructionCycle == 2);
    cpu.op_ld_a_addr_n16();
    REQUIRE(cpu.instructionCycle == 3);
    cpu.op_ld_a_addr_n16();
    REQUIRE(cpu.instructionCycle == 0);
    REQUIRE(cpu.PC == 0xC005);

    REQUIRE(cpu.A == mem.wram[0]);
}

TEST_CASE("LDH A, [n16]", "[OPCODE]")
{
    SM83 cpu;
    Memory mem;
    PPU ppu;

    cpu.memory = &mem;
    mem.ppu = &ppu;
    ppu.memory = &mem;
    ppu.cpu = &cpu;

    cpu.PC = 0xC000;
    cpu.instructionCycle = 0;

    mem.wram[1] = 0x66;
    mem.writemem(0x66, 0xFF66);

    cpu.op_ldh_a_addr_n16();
    REQUIRE(cpu.instructionCycle == 1);
    cpu.op_ldh_a_addr_n16();
    REQUIRE(cpu.instructionCycle == 2);
    cpu.op_ldh_a_addr_n16();
    REQUIRE(cpu.instructionCycle == 0);
    REQUIRE(cpu.PC == 0xC002);

    REQUIRE(cpu.A == mem.readmem(0xFF66));
}

TEST_CASE("LDH A, [C]", "[OPCODE]")
{
    SM83 cpu;
    Memory mem;
    PPU ppu;

    cpu.memory = &mem;
    mem.ppu = &ppu;
    ppu.memory = &mem;
    ppu.cpu = &cpu;

    cpu.PC = 0x0150;
    cpu.instructionCycle = 0;

    cpu.C = 0x66;
    mem.writemem(0x66, 0xFF66);

    cpu.op_ldh_a_addr_c();
    REQUIRE(cpu.instructionCycle == 1);
    cpu.op_ldh_a_addr_c();
    REQUIRE(cpu.instructionCycle == 0);
    REQUIRE(cpu.PC == 0x0151);

    REQUIRE(cpu.A == mem.readmem(0xFF66));
}

TEST_CASE("LD [HL+], A", "[OPCODE]")
{
    SM83 cpu;
    Memory mem;
    PPU ppu;

    cpu.memory = &mem;
    mem.ppu = &ppu;
    ppu.memory = &mem;
    ppu.cpu = &cpu;

    cpu.PC = 0x0150;
    cpu.instructionCycle = 0;

    cpu.H = 0xC0;
    cpu.L = 0xFF;

    cpu.A = 0x66;

    cpu.op_ld_addr_hl_inc_a();
    REQUIRE(cpu.instructionCycle == 1);
    cpu.op_ld_addr_hl_inc_a();
    REQUIRE(cpu.instructionCycle == 0);
    REQUIRE(cpu.PC == 0x0151);

    REQUIRE(mem.readmem(0xC0FF) == 0x66);
    REQUIRE(cpu.H == 0xC1);
    REQUIRE(cpu.L == 0x00);
}

TEST_CASE("LD [HL-], A", "[OPCODE]")
{
    SM83 cpu;
    Memory mem;
    PPU ppu;

    cpu.memory = &mem;
    mem.ppu = &ppu;
    ppu.memory = &mem;
    ppu.cpu = &cpu;

    cpu.PC = 0x0150;
    cpu.instructionCycle = 0;

    cpu.H = 0xC1;
    cpu.L = 0x00;

    cpu.A = 0x66;

    cpu.op_ld_addr_hl_dec_a();
    REQUIRE(cpu.instructionCycle == 1);
    cpu.op_ld_addr_hl_dec_a();
    REQUIRE(cpu.instructionCycle == 0);
    REQUIRE(cpu.PC == 0x0151);

    REQUIRE(mem.readmem(0xC100) == 0x66);
    REQUIRE(cpu.H == 0xC0);
    REQUIRE(cpu.L == 0xFF);
}

TEST_CASE("LD A, [HL+]", "[OPCODE]")
{
    SM83 cpu;
    Memory mem;
    PPU ppu;

    cpu.memory = &mem;
    mem.ppu = &ppu;
    ppu.memory = &mem;
    ppu.cpu = &cpu;

    cpu.PC = 0x0150;
    cpu.instructionCycle = 0;

    cpu.H = 0xC0;
    cpu.L = 0xFF;

    mem.writemem(0x66, 0xC0FF);

    cpu.op_ld_a_addr_hl_inc();
    REQUIRE(cpu.instructionCycle == 1);
    cpu.op_ld_a_addr_hl_inc();
    REQUIRE(cpu.instructionCycle == 0);
    REQUIRE(cpu.PC == 0x0151);

    REQUIRE(cpu.A == mem.readmem(0xC0FF));
    REQUIRE(cpu.H == 0xC1);
    REQUIRE(cpu.L == 0x00);
}

TEST_CASE("LD A, [HL-]", "[OPCODE]")
{
    SM83 cpu;
    Memory mem;
    PPU ppu;

    cpu.memory = &mem;
    mem.ppu = &ppu;
    ppu.memory = &mem;
    ppu.cpu = &cpu;

    cpu.PC = 0x0150;
    cpu.instructionCycle = 0;

    cpu.H = 0xC1;
    cpu.L = 0x00;

    mem.writemem(0x66, 0xC100);

    cpu.op_ld_a_addr_hl_dec();
    REQUIRE(cpu.instructionCycle == 1);
    cpu.op_ld_a_addr_hl_dec();
    REQUIRE(cpu.instructionCycle == 0);
    REQUIRE(cpu.PC == 0x0151);

    REQUIRE(cpu.A == mem.readmem(0xC100));
    REQUIRE(cpu.H == 0xC0);
    REQUIRE(cpu.L == 0xFF);
}

TEST_CASE("CALL n16", "[OPCODE]")
{
    SM83 cpu;
    Memory mem;
    PPU ppu;

    cpu.memory = &mem;
    mem.ppu = &ppu;
    ppu.memory = &mem;
    ppu.cpu = &cpu;

    cpu.PC = 0xC000;
    cpu.instructionCycle = 0;
    cpu.SP = 0xC008;

    mem.wram[2] = 0x01;
    mem.wram[1] = 0x50;

    cpu.op_call_n16();
    REQUIRE(cpu.instructionCycle == 1);
    cpu.op_call_n16();
    REQUIRE(cpu.instructionCycle == 2);
    cpu.op_call_n16();
    REQUIRE(cpu.instructionCycle == 3);
    cpu.op_call_n16();
    REQUIRE(cpu.instructionCycle == 4);
    cpu.op_call_n16();
    REQUIRE(cpu.instructionCycle == 5);
    cpu.op_call_n16();
    REQUIRE(cpu.instructionCycle == 0);

    REQUIRE(cpu.PC == 0x0150);
    REQUIRE(cpu.SP == 0xC006);
    REQUIRE(mem.wram[7] == 0xC0);
    REQUIRE(mem.wram[6] == 0x03);
}

TEST_CASE("CALL cc, n16", "[OPCODE]")
{
    SM83 cpu;
    Memory mem;
    PPU ppu;

    cpu.memory = &mem;
    mem.ppu = &ppu;
    ppu.memory = &mem;
    ppu.cpu = &cpu;

    cpu.PC = 0xC000;
    cpu.instructionCycle = 0;
    cpu.SP = 0xC008;

    mem.wram[2] = 0x01;
    mem.wram[1] = 0x50;
    cpu.F = 0x0;

    SECTION("Condition Met")
    {
        cpu.setZeroFlag(1);

        cpu.op_call_cc_n16(cpu.getZeroFlag(), 1);
        REQUIRE(cpu.instructionCycle == 1);
        cpu.op_call_cc_n16(cpu.getZeroFlag(), 1);
        REQUIRE(cpu.instructionCycle == 2);
        cpu.op_call_cc_n16(cpu.getZeroFlag(), 1);
        REQUIRE(cpu.instructionCycle == 3);
        cpu.op_call_cc_n16(cpu.getZeroFlag(), 1);
        REQUIRE(cpu.instructionCycle == 4);
        cpu.op_call_cc_n16(cpu.getZeroFlag(), 1);
        REQUIRE(cpu.instructionCycle == 5);
        cpu.op_call_cc_n16(cpu.getZeroFlag(), 1);
        REQUIRE(cpu.instructionCycle == 0);

        REQUIRE(cpu.PC == 0x0150);
        REQUIRE(cpu.SP == 0xC006);
        REQUIRE(mem.wram[7] == 0xC0);
        REQUIRE(mem.wram[6] == 0x03);
    }

    SECTION("Condition Not Met")
    {
        cpu.setZeroFlag(0);

        cpu.op_call_cc_n16(cpu.getZeroFlag(), 1);
        REQUIRE(cpu.instructionCycle == 1);
        cpu.op_call_cc_n16(cpu.getZeroFlag(), 1);
        REQUIRE(cpu.instructionCycle == 2);
        cpu.op_call_cc_n16(cpu.getZeroFlag(), 1);
        REQUIRE(cpu.instructionCycle == 0);

        REQUIRE(cpu.PC == 0xC003);
    }
}

TEST_CASE("JP HL", "[OPCODE]")
{
    SM83 cpu;

    cpu.PC = 0x0150;
    cpu.instructionCycle = 0;

    cpu.H = 0x12;
    cpu.L = 0x34;

    cpu.op_jp_hl();

    REQUIRE(cpu.PC == 0x1234);
}

TEST_CASE("JP n16", "[OPCODE]")
{
    SM83 cpu;
    Memory mem;
    PPU ppu;

    cpu.memory = &mem;
    mem.ppu = &ppu;
    ppu.memory = &mem;
    ppu.cpu = &cpu;

    cpu.PC = 0xC000;
    cpu.instructionCycle = 0;

    mem.wram[2] = 0x12;
    mem.wram[1] = 0x34;

    cpu.op_jp_n16();
    REQUIRE(cpu.instructionCycle == 1);
    cpu.op_jp_n16();
    REQUIRE(cpu.instructionCycle == 2);
    cpu.op_jp_n16();
    REQUIRE(cpu.instructionCycle == 3);
    cpu.op_jp_n16();
    REQUIRE(cpu.instructionCycle == 0);

    REQUIRE(cpu.PC == 0x1234);
}

TEST_CASE("JP cc, n16", "[OPCODE]")
{
    SM83 cpu;
    Memory mem;
    PPU ppu;

    cpu.memory = &mem;
    mem.ppu = &ppu;
    ppu.memory = &mem;
    ppu.cpu = &cpu;

    cpu.PC = 0xC000;
    cpu.instructionCycle = 0;
    cpu.F = 0x0;

    mem.wram[2] = 0x12;
    mem.wram[1] = 0x34;

    SECTION("Condition Met")
    {
        cpu.setZeroFlag(1);

        cpu.op_jp_cc_n16(cpu.getZeroFlag(), 1);
        REQUIRE(cpu.instructionCycle == 1);
        cpu.op_jp_cc_n16(cpu.getZeroFlag(), 1);
        REQUIRE(cpu.instructionCycle == 2);
        cpu.op_jp_cc_n16(cpu.getZeroFlag(), 1);
        REQUIRE(cpu.instructionCycle == 3);
        cpu.op_jp_cc_n16(cpu.getZeroFlag(), 1);
        REQUIRE(cpu.instructionCycle == 0);

        REQUIRE(cpu.PC == 0x1234);
    }

    SECTION("Condition Not Met")
    {
        cpu.setZeroFlag(0);

        cpu.op_jp_cc_n16(cpu.getZeroFlag(), 1);
        REQUIRE(cpu.instructionCycle == 1);
        cpu.op_jp_cc_n16(cpu.getZeroFlag(), 1);
        REQUIRE(cpu.instructionCycle == 2);
        cpu.op_jp_cc_n16(cpu.getZeroFlag(), 1);
        REQUIRE(cpu.instructionCycle == 0);

        REQUIRE(cpu.PC == 0xC003);
    }
}

TEST_CASE("JR e8", "[OPCODE]")
{
    SM83 cpu;
    Memory mem;
    PPU ppu;

    cpu.memory = &mem;
    mem.ppu = &ppu;
    ppu.memory = &mem;
    ppu.cpu = &cpu;

    cpu.PC = 0xC000;
    cpu.instructionCycle = 0;

    SECTION("Positive Jump")
    {
        mem.wram[1] = 0x10;

        cpu.op_jr_e8();
        REQUIRE(cpu.instructionCycle == 1);
        cpu.op_jr_e8();
        REQUIRE(cpu.instructionCycle == 2);
        cpu.op_jr_e8();
        REQUIRE(cpu.instructionCycle == 0);

        REQUIRE(cpu.PC == 0xC012);
    }

    SECTION("Negative Jump")
    {
        mem.wram[1] = (uint8_t)(-16);

        cpu.op_jr_e8();
        REQUIRE(cpu.instructionCycle == 1);
        cpu.op_jr_e8();
        REQUIRE(cpu.instructionCycle == 2);
        cpu.op_jr_e8();
        REQUIRE(cpu.instructionCycle == 0);

        REQUIRE(cpu.PC == 0xBFF2);
    }
}

TEST_CASE("JR cc, e8", "[OPCODE]")
{
    SM83 cpu;
    Memory mem;
    PPU ppu;

    cpu.memory = &mem;
    mem.ppu = &ppu;
    ppu.memory = &mem;
    ppu.cpu = &cpu;

    cpu.PC = 0xC000;
    cpu.instructionCycle = 0;
    cpu.F = 0x0;

    SECTION("Positive Jump")
    {
        mem.wram[1] = 0x10;
        cpu.setZeroFlag(1);

        cpu.op_jr_cc_e8(cpu.getZeroFlag(), 1);
        REQUIRE(cpu.instructionCycle == 1);
        cpu.op_jr_cc_e8(cpu.getZeroFlag(), 1);
        REQUIRE(cpu.instructionCycle == 2);
        cpu.op_jr_cc_e8(cpu.getZeroFlag(), 1);
        REQUIRE(cpu.instructionCycle == 0);

        REQUIRE(cpu.PC == 0xC012);
    }

    SECTION("Negative Jump")
    {
        mem.wram[1] = (uint8_t)(-16);
        cpu.setZeroFlag(1);

        cpu.op_jr_cc_e8(cpu.getZeroFlag(), 1);
        REQUIRE(cpu.instructionCycle == 1);
        cpu.op_jr_cc_e8(cpu.getZeroFlag(), 1);
        REQUIRE(cpu.instructionCycle == 2);
        cpu.op_jr_cc_e8(cpu.getZeroFlag(), 1);
        REQUIRE(cpu.instructionCycle == 0);

        REQUIRE(cpu.PC == 0xBFF2);
    }

    SECTION("Condition Not Met")
    {
        cpu.setZeroFlag(0);

        cpu.op_jr_cc_e8(cpu.getZeroFlag(), 1);
        REQUIRE(cpu.instructionCycle == 1);
        cpu.op_jr_cc_e8(cpu.getZeroFlag(), 1);
        REQUIRE(cpu.instructionCycle == 0);

        REQUIRE(cpu.PC == 0xC002);
    }
}

TEST_CASE("RET cc", "[OPCODE]")
{
    SM83 cpu;
    Memory mem;
    PPU ppu;

    cpu.memory = &mem;
    mem.ppu = &ppu;
    ppu.memory = &mem;
    ppu.cpu = &cpu;

    cpu.PC = 0x0150;
    cpu.instructionCycle = 0;
    cpu.F = 0x0;

    cpu.SP = 0xC000;
    mem.wram[1] = 0x12;
    mem.wram[0] = 0x34;

    SECTION("Condition Met")
    {
        cpu.setZeroFlag(1);

        cpu.op_ret_cc(cpu.getZeroFlag(), 1);
        REQUIRE(cpu.instructionCycle == 1);
        cpu.op_ret_cc(cpu.getZeroFlag(), 1);
        REQUIRE(cpu.instructionCycle == 2);
        cpu.op_ret_cc(cpu.getZeroFlag(), 1);
        REQUIRE(cpu.instructionCycle == 3);
        cpu.op_ret_cc(cpu.getZeroFlag(), 1);
        REQUIRE(cpu.instructionCycle == 4);
        cpu.op_ret_cc(cpu.getZeroFlag(), 1);
        REQUIRE(cpu.instructionCycle == 0);

        REQUIRE(cpu.PC == 0x1234);
        REQUIRE(cpu.SP == 0xC002);
    }

    SECTION("Condition Not Met")
    {
        cpu.setZeroFlag(0);

        cpu.op_ret_cc(cpu.getZeroFlag(), 1);
        REQUIRE(cpu.instructionCycle == 1);
        cpu.op_ret_cc(cpu.getZeroFlag(), 1);
        REQUIRE(cpu.instructionCycle == 0);

        REQUIRE(cpu.PC == 0x0151);
    }
}

TEST_CASE("RET", "[OPCODE]")
{
    SM83 cpu;
    Memory mem;
    PPU ppu;

    cpu.memory = &mem;
    mem.ppu = &ppu;
    ppu.memory = &mem;
    ppu.cpu = &cpu;

    cpu.PC = 0x0150;
    cpu.instructionCycle = 0;
    cpu.F = 0x0;

    cpu.SP = 0xC000;
    mem.wram[1] = 0x12;
    mem.wram[0] = 0x34;

    cpu.op_ret();
    REQUIRE(cpu.instructionCycle == 1);
    cpu.op_ret();
    REQUIRE(cpu.instructionCycle == 2);
    cpu.op_ret();
    REQUIRE(cpu.instructionCycle == 3);
    cpu.op_ret();
    REQUIRE(cpu.instructionCycle == 0);

    REQUIRE(cpu.PC == 0x1234);
    REQUIRE(cpu.SP == 0xC002);
}

TEST_CASE("RETI", "[OPCODE]")
{
    SM83 cpu;
    Memory mem;
    PPU ppu;

    cpu.memory = &mem;
    mem.ppu = &ppu;
    ppu.memory = &mem;
    ppu.cpu = &cpu;

    cpu.PC = 0x0150;
    cpu.instructionCycle = 0;
    cpu.F = 0x0;

    cpu.SP = 0xC000;
    mem.wram[1] = 0x12;
    mem.wram[0] = 0x34;
    cpu.setImeFlag(0);

    cpu.op_reti();
    REQUIRE(cpu.instructionCycle == 1);
    cpu.op_reti();
    REQUIRE(cpu.instructionCycle == 2);
    cpu.op_reti();
    REQUIRE(cpu.instructionCycle == 3);
    cpu.op_reti();
    REQUIRE(cpu.instructionCycle == 0);

    REQUIRE(cpu.PC == 0x1234);
    REQUIRE(cpu.SP == 0xC002);
    REQUIRE(cpu.ime == 1);
}

TEST_CASE("RST", "[OPCODE]")
{
    SM83 cpu;
    Memory mem;
    PPU ppu;

    cpu.memory = &mem;
    mem.ppu = &ppu;
    ppu.memory = &mem;
    ppu.cpu = &cpu;

    cpu.PC = 0x0150;
    cpu.instructionCycle = 0;
    cpu.F = 0x0;

    cpu.SP = 0xC008;

    cpu.op_rst(0x20);
    REQUIRE(cpu.instructionCycle == 1);
    cpu.op_rst(0x20);
    REQUIRE(cpu.instructionCycle == 2);
    cpu.op_rst(0x20);
    REQUIRE(cpu.instructionCycle == 3);
    cpu.op_rst(0x20);
    REQUIRE(cpu.instructionCycle == 0);

    REQUIRE(cpu.PC == 0x20);
    REQUIRE(cpu.SP == 0xC006);
    REQUIRE(mem.wram[6] == 0x51);
    REQUIRE(mem.wram[7] == 0x01);
}

TEST_CASE("ADD HL, SP", "[OPCODE]")
{
    SM83 cpu;

    cpu.PC = 0x0150;
    cpu.instructionCycle = 0;
    cpu.F = 0x0;

    SECTION("Check Half Carry Flag")
    {
        cpu.SP = 0x1F00;
        cpu.H = 0x01;
        cpu.L = 0x66;

        cpu.op_add_hl_sp();
        REQUIRE(cpu.instructionCycle == 1);
        cpu.op_add_hl_sp();
        REQUIRE(cpu.instructionCycle == 0);
        REQUIRE(cpu.PC == 0x0151);

        REQUIRE(cpu.H == 0x20);
        REQUIRE(cpu.L == 0x66);
        REQUIRE(cpu.getSubtractFlag() == 0);
        REQUIRE(cpu.getHalfCarryFlag() == 1);
        REQUIRE(cpu.getCarryFlag() == 0);
    }

    SECTION("Check Carry Flag")
    {
        cpu.SP = 0xFF00;
        cpu.H = 0x02;
        cpu.L = 0x66;

        cpu.op_add_hl_sp();
        REQUIRE(cpu.instructionCycle == 1);
        cpu.op_add_hl_sp();
        REQUIRE(cpu.instructionCycle == 0);
        REQUIRE(cpu.PC == 0x0151);

        REQUIRE(cpu.H == 0x01);
        REQUIRE(cpu.L == 0x66);
        REQUIRE(cpu.getSubtractFlag() == 0);
        REQUIRE(cpu.getHalfCarryFlag() == 1);
        REQUIRE(cpu.getCarryFlag() == 1);
    }
}

TEST_CASE("ADD SP, e8", "[OPCODE]")
{
    SM83 cpu;
    Memory mem;
    PPU ppu;

    cpu.memory = &mem;
    mem.ppu = &ppu;
    ppu.memory = &mem;
    ppu.cpu = &cpu;

    cpu.PC = 0xC000;
    cpu.instructionCycle = 0;
    cpu.F = 0x0;

    SECTION("Check Half Carry Flag")
    {
        mem.wram[1] = 0x09;
        cpu.SP = 0x2008;

        cpu.op_add_sp_e8();
        REQUIRE(cpu.instructionCycle == 1);
        cpu.op_add_sp_e8();
        REQUIRE(cpu.instructionCycle == 2);
        cpu.op_add_sp_e8();
        REQUIRE(cpu.instructionCycle == 3);
        cpu.op_add_sp_e8();
        REQUIRE(cpu.instructionCycle == 0);
        REQUIRE(cpu.PC == 0xC002);

        REQUIRE(cpu.SP == 0x2011);
        REQUIRE(cpu.getZeroFlag() == 0);
        REQUIRE(cpu.getSubtractFlag() == 0);
        REQUIRE(cpu.getHalfCarryFlag() == 1);
        REQUIRE(cpu.getCarryFlag() == 0);
    }

    SECTION("Check Carry Flag")
    {
        mem.wram[1] = 0x01;
        cpu.SP = 0x20FF;

        cpu.op_add_sp_e8();
        REQUIRE(cpu.instructionCycle == 1);
        cpu.op_add_sp_e8();
        REQUIRE(cpu.instructionCycle == 2);
        cpu.op_add_sp_e8();
        REQUIRE(cpu.instructionCycle == 3);
        cpu.op_add_sp_e8();
        REQUIRE(cpu.instructionCycle == 0);
        REQUIRE(cpu.PC == 0xC002);

        REQUIRE(cpu.SP == 0x2100);
        REQUIRE(cpu.getZeroFlag() == 0);
        REQUIRE(cpu.getSubtractFlag() == 0);
        REQUIRE(cpu.getHalfCarryFlag() == 1);
        REQUIRE(cpu.getCarryFlag() == 1);
    }
}

TEST_CASE("DEC SP", "[OPCODE]")
{
    SM83 cpu;

    cpu.PC = 0x0150;
    cpu.instructionCycle = 0;

    cpu.SP = 0x2100;

    cpu.op_dec_sp();
    REQUIRE(cpu.instructionCycle == 1);
    cpu.op_dec_sp();
    REQUIRE(cpu.instructionCycle == 0);
    REQUIRE(cpu.PC == 0x0151);

    REQUIRE(cpu.SP == 0x20FF);
}

TEST_CASE("INC SP", "[OPCODE]")
{
    SM83 cpu;

    cpu.PC = 0x0150;
    cpu.instructionCycle = 0;

    cpu.SP = 0x20FF;

    cpu.op_inc_sp();
    REQUIRE(cpu.instructionCycle == 1);
    cpu.op_inc_sp();
    REQUIRE(cpu.instructionCycle == 0);
    REQUIRE(cpu.PC == 0x0151);

    REQUIRE(cpu.SP == 0x2100);
}

TEST_CASE("LD SP, n16", "[OPCODE]")
{
    SM83 cpu;
    Memory mem;
    PPU ppu;

    cpu.memory = &mem;
    mem.ppu = &ppu;
    ppu.memory = &mem;
    ppu.cpu = &cpu;

    cpu.PC = 0xC000;
    cpu.instructionCycle = 0;

    mem.wram[2] = 0x01;
    mem.wram[1] = 0x50;

    cpu.op_ld_sp_n16();
    REQUIRE(cpu.instructionCycle == 1);
    cpu.op_ld_sp_n16();
    REQUIRE(cpu.instructionCycle == 2);
    cpu.op_ld_sp_n16();
    REQUIRE(cpu.instructionCycle == 0);
    REQUIRE(cpu.PC == 0xC003);

    REQUIRE(cpu.SP == 0x0150);
}

TEST_CASE("LD [n16], SP", "[OPCODE]")
{
    SM83 cpu;
    Memory mem;
    PPU ppu;

    cpu.memory = &mem;
    mem.ppu = &ppu;
    ppu.memory = &mem;
    ppu.cpu = &cpu;

    cpu.PC = 0xC000;
    cpu.instructionCycle = 0;

    cpu.SP = 0x1234;
    mem.wram[2] = 0xC0;
    mem.wram[1] = 0x08;

    cpu.op_ld_addr_n16_sp();
    REQUIRE(cpu.instructionCycle == 1);
    cpu.op_ld_addr_n16_sp();
    REQUIRE(cpu.instructionCycle == 2);
    cpu.op_ld_addr_n16_sp();
    REQUIRE(cpu.instructionCycle == 3);
    cpu.op_ld_addr_n16_sp();
    REQUIRE(cpu.instructionCycle == 4);
    cpu.op_ld_addr_n16_sp();
    REQUIRE(cpu.instructionCycle == 0);
    REQUIRE(cpu.PC == 0xC003);

    REQUIRE(mem.wram[9] == 0x12);
    REQUIRE(mem.wram[8] == 0x34);
}

TEST_CASE("LD HL, SP+e8", "[OPCODE]")
{
    SM83 cpu;
    Memory mem;
    PPU ppu;

    cpu.memory = &mem;
    mem.ppu = &ppu;
    ppu.memory = &mem;
    ppu.cpu = &cpu;

    cpu.PC = 0xC000;
    cpu.instructionCycle = 0;
    cpu.F = 0x0;

    SECTION("Check Half Carry Flag")
    {
        mem.wram[1] = 0x09;
        cpu.SP = 0x2008;

        cpu.op_ld_hl_sp_plus_e8();
        REQUIRE(cpu.instructionCycle == 1);
        cpu.op_ld_hl_sp_plus_e8();
        REQUIRE(cpu.instructionCycle == 2);
        cpu.op_ld_hl_sp_plus_e8();
        REQUIRE(cpu.instructionCycle == 0);
        REQUIRE(cpu.PC == 0xC002);

        REQUIRE(cpu.H == 0x20);
        REQUIRE(cpu.L == 0x11);
        REQUIRE(cpu.getZeroFlag() == 0);
        REQUIRE(cpu.getSubtractFlag() == 0);
        REQUIRE(cpu.getHalfCarryFlag() == 1);
        REQUIRE(cpu.getCarryFlag() == 0);
    }

    SECTION("Check Carry Flag")
    {
        mem.wram[1] = 0x01;
        cpu.SP = 0x20FF;

        cpu.op_ld_hl_sp_plus_e8();
        REQUIRE(cpu.instructionCycle == 1);
        cpu.op_ld_hl_sp_plus_e8();
        REQUIRE(cpu.instructionCycle == 2);
        cpu.op_ld_hl_sp_plus_e8();
        REQUIRE(cpu.instructionCycle == 0);
        REQUIRE(cpu.PC == 0xC002);

        REQUIRE(cpu.H == 0x21);
        REQUIRE(cpu.L == 0x00);
        REQUIRE(cpu.getZeroFlag() == 0);
        REQUIRE(cpu.getSubtractFlag() == 0);
        REQUIRE(cpu.getHalfCarryFlag() == 1);
        REQUIRE(cpu.getCarryFlag() == 1);
    }
}

TEST_CASE("LD SP, HL", "[OPCODE]")
{
    SM83 cpu;

    cpu.PC = 0x0150;
    cpu.instructionCycle = 0;

    cpu.H = 0x12;
    cpu.L = 0x34;

    cpu.op_ld_sp_hl();
    REQUIRE(cpu.instructionCycle == 1);
    cpu.op_ld_sp_hl();
    REQUIRE(cpu.instructionCycle == 0);
    REQUIRE(cpu.PC == 0x0151);

    REQUIRE(cpu.SP == 0x1234);
}

TEST_CASE("POP AF", "[OPCODE]")
{
    SM83 cpu;
    Memory mem;
    PPU ppu;

    cpu.memory = &mem;
    mem.ppu = &ppu;
    ppu.memory = &mem;
    ppu.cpu = &cpu;

    cpu.PC = 0x0150;
    cpu.instructionCycle = 0;

    cpu.SP = 0xC000;
    mem.wram[0] = 0x12;
    mem.wram[1] = 0x34;

    cpu.op_pop_af();
    REQUIRE(cpu.instructionCycle == 1);
    cpu.op_pop_af();
    REQUIRE(cpu.instructionCycle == 2);
    cpu.op_pop_af();
    REQUIRE(cpu.instructionCycle == 0);
    REQUIRE(cpu.PC == 0x0151);

    REQUIRE(cpu.SP == 0xC002);
    REQUIRE(cpu.A == 0x34);
    REQUIRE(cpu.F == 0x10);
}

TEST_CASE("POP r16", "[OPCODE]")
{
    SM83 cpu;
    Memory mem;
    PPU ppu;

    cpu.memory = &mem;
    mem.ppu = &ppu;
    ppu.memory = &mem;
    ppu.cpu = &cpu;

    cpu.PC = 0x0150;
    cpu.instructionCycle = 0;

    cpu.SP = 0xC000;
    mem.wram[0] = 0x12;
    mem.wram[1] = 0x34;

    cpu.op_pop_r16(cpu.B, cpu.C);
    REQUIRE(cpu.instructionCycle == 1);
    cpu.op_pop_r16(cpu.B, cpu.C);
    REQUIRE(cpu.instructionCycle == 2);
    cpu.op_pop_r16(cpu.B, cpu.C);
    REQUIRE(cpu.instructionCycle == 0);
    REQUIRE(cpu.PC == 0x0151);

    REQUIRE(cpu.SP == 0xC002);
    REQUIRE(cpu.B == 0x34);
    REQUIRE(cpu.C == 0x12);
}

TEST_CASE("PUSH AF", "[OPCODE]")
{
    SM83 cpu;
    Memory mem;
    PPU ppu;

    cpu.memory = &mem;
    mem.ppu = &ppu;
    ppu.memory = &mem;
    ppu.cpu = &cpu;

    cpu.PC = 0x0150;
    cpu.instructionCycle = 0;

    cpu.SP = 0xC008;
    cpu.A = 0x12;
    cpu.F = 0x34;

    cpu.op_push_af();
    REQUIRE(cpu.instructionCycle == 1);
    cpu.op_push_af();
    REQUIRE(cpu.instructionCycle == 2);
    cpu.op_push_af();
    REQUIRE(cpu.instructionCycle == 3);
    cpu.op_push_af();
    REQUIRE(cpu.instructionCycle == 0);
    REQUIRE(cpu.PC == 0x0151);

    REQUIRE(cpu.SP == 0xC006);
    REQUIRE(mem.wram[7] == 0x12);
    REQUIRE(mem.wram[6] == 0x34);
}

TEST_CASE("PUSH r16", "[OPCODE]")
{
    SM83 cpu;
    Memory mem;
    PPU ppu;

    cpu.memory = &mem;
    mem.ppu = &ppu;
    ppu.memory = &mem;
    ppu.cpu = &cpu;

    cpu.PC = 0x0150;
    cpu.instructionCycle = 0;

    cpu.SP = 0xC008;
    cpu.B = 0x12;
    cpu.C = 0x34;

    cpu.op_push_r16(cpu.B, cpu.C);
    REQUIRE(cpu.instructionCycle == 1);
    cpu.op_push_r16(cpu.B, cpu.C);
    REQUIRE(cpu.instructionCycle == 2);
    cpu.op_push_r16(cpu.B, cpu.C);
    REQUIRE(cpu.instructionCycle == 3);
    cpu.op_push_r16(cpu.B, cpu.C);
    REQUIRE(cpu.instructionCycle == 0);
    REQUIRE(cpu.PC == 0x0151);

    REQUIRE(cpu.SP == 0xC006);
    REQUIRE(mem.wram[7] == 0x12);
    REQUIRE(mem.wram[6] == 0x34);
}

TEST_CASE("CCF", "[OPCODE]")
{
    SM83 cpu;

    cpu.PC = 0x0150;
    cpu.instructionCycle = 0;

    cpu.setCarryFlag(1);
    cpu.setSubtractFlag(1);
    cpu.setHalfCarryFlag(1);

    cpu.op_ccf();
    REQUIRE(cpu.PC == 0x0151);

    REQUIRE(cpu.getCarryFlag() == 0);
    REQUIRE(cpu.getSubtractFlag() == 0);
    REQUIRE(cpu.getHalfCarryFlag() == 0);
}

TEST_CASE("CPL", "[OPCODE]")
{
    SM83 cpu;

    cpu.PC = 0x0150;
    cpu.instructionCycle = 0;
    cpu.F = 0x0;

    cpu.A = 0x66;

    cpu.op_cpl();
    REQUIRE(cpu.PC == 0x0151);

    REQUIRE(cpu.A == 0x99);
    REQUIRE(cpu.getSubtractFlag() == 1);
    REQUIRE(cpu.getHalfCarryFlag() == 1);
}

TEST_CASE("DAA", "[OPCODE]")
{
    SM83 cpu;

    cpu.PC = 0x0150;
    cpu.instructionCycle = 0;
    cpu.F = 0x0;

    SECTION("Addition")
    {
        SECTION("No Flags")
        {
            cpu.A = 0x23;
            cpu.B = 0x14;

            cpu.op_add_a_r8(cpu.B);

            cpu.op_daa();
            REQUIRE(cpu.PC == 0x0152);

            REQUIRE(cpu.A == 0x37);
            REQUIRE(cpu.getZeroFlag() == 0);
            REQUIRE(cpu.getHalfCarryFlag() == 0);
        }

        SECTION("Half Carry Flag")
        {
            cpu.A = 0x23;
            cpu.B = 0x18;

            cpu.op_add_a_r8(cpu.B);

            cpu.op_daa();
            REQUIRE(cpu.PC == 0x0152);

            REQUIRE(cpu.A == 0x41);
            REQUIRE(cpu.getZeroFlag() == 0);
            REQUIRE(cpu.getHalfCarryFlag() == 0);
        }

        SECTION("Carry Flag and Zero Flag")
        {
            cpu.A = 0x64;
            cpu.B = 0x36;

            cpu.op_add_a_r8(cpu.B);

            cpu.op_daa();
            REQUIRE(cpu.PC == 0x0152);

            REQUIRE(cpu.A == 0x00);
            REQUIRE(cpu.getZeroFlag() == 1);
            REQUIRE(cpu.getHalfCarryFlag() == 0);
            REQUIRE(cpu.getCarryFlag() == 1);
        }
    }

    SECTION("Subtraction")
    {
        SECTION("No Flags")
        {
            cpu.A = 0x67;
            cpu.B = 0x36;

            cpu.op_sub_a_r8(cpu.B);

            cpu.op_daa();
            REQUIRE(cpu.PC == 0x0152);

            REQUIRE(cpu.A == 0x31);
            REQUIRE(cpu.getZeroFlag() == 0);
            REQUIRE(cpu.getHalfCarryFlag() == 0);
            REQUIRE(cpu.getCarryFlag() == 0);
        }

        SECTION("Half Carry Flag")
        {
            cpu.A = 0x64;
            cpu.B = 0x36;

            cpu.op_sub_a_r8(cpu.B);

            cpu.op_daa();
            REQUIRE(cpu.PC == 0x0152);

            REQUIRE(cpu.A == 0x28);
            REQUIRE(cpu.getZeroFlag() == 0);
            REQUIRE(cpu.getHalfCarryFlag() == 0);
            REQUIRE(cpu.getCarryFlag() == 0);
        }

        SECTION("Carry Flag")
        {
            cpu.A = 0x64;
            cpu.B = 0x90;

            cpu.op_sub_a_r8(cpu.B);

            cpu.op_daa();
            REQUIRE(cpu.PC == 0x0152);

            REQUIRE(cpu.A == 0x74);
            REQUIRE(cpu.getZeroFlag() == 0);
            REQUIRE(cpu.getHalfCarryFlag() == 0);
            REQUIRE(cpu.getCarryFlag() == 1);
        }
    }
}

TEST_CASE("DI", "[OPCODE]")
{
    SM83 cpu;

    cpu.PC = 0x0150;
    cpu.instructionCycle = 0;

    cpu.ime = 1;

    cpu.op_di();
    REQUIRE(cpu.PC == 0x0151);

    REQUIRE(cpu.ime == 0);
}

TEST_CASE("EI", "[OPCODE]")
{
    SM83 cpu;
    Memory mem;
    PPU ppu;

    cpu.memory = &mem;
    mem.ppu = &ppu;
    ppu.memory = &mem;
    ppu.cpu = &cpu;

    cpu.PC = 0xC000;
    cpu.instructionCycle = 0;

    cpu.ime = 0;
    mem.wram[0] = 0xFB;
    mem.wram[1] = 0x00;

    cpu.cycle();
    REQUIRE(cpu.PC == 0xC001);

    REQUIRE(cpu.ime == 0);

    cpu.cycle();
    REQUIRE(cpu.ime == 1);
}

TEST_CASE("HALT", "[OPCODE]")
{
    SM83 cpu;
    Memory mem;
    PPU ppu;

    cpu.memory = &mem;
    mem.ppu = &ppu;
    ppu.memory = &mem;
    ppu.cpu = &cpu;

    cpu.PC = 0x0150;
    cpu.instructionCycle = 0;

    SECTION("IME Enabled")
    {
        cpu.ime = 1;

        cpu.op_halt();
        REQUIRE(cpu.PC == 0x0151);

        REQUIRE(cpu.halted == true);
    }

    SECTION("IME Disabled")
    {
        cpu.ime = 0;

        SECTION("No pending interrupts")
        {
            mem.ieRegister = 0x00;

            cpu.op_halt();
            REQUIRE(cpu.PC == 0x0151);

            REQUIRE(cpu.halted == true);
        }

        SECTION("Pending interrupts")
        {
            cpu.setVBlankInterruptEnable(1);
            cpu.setVBlankInterruptFlag(1);

            cpu.op_halt();
            REQUIRE(cpu.PC == 0x0151);

            REQUIRE(cpu.halted == false);
            REQUIRE(cpu.halt_bug == true);
        }
    }
}

TEST_CASE("NOP", "[OPCODE]")
{
    SM83 cpu;

    cpu.PC = 0x0150;
    cpu.instructionCycle = 0;

    cpu.op_nop();

    REQUIRE(cpu.PC == 0x0151);
}

TEST_CASE("SCF", "[OPCODE]")
{
    SM83 cpu;

    cpu.PC = 0x0150;
    cpu.instructionCycle = 0;
    cpu.F = 0x0;

    cpu.op_scf();

    REQUIRE(cpu.PC == 0x0151);
    REQUIRE(cpu.getSubtractFlag() == 0);
    REQUIRE(cpu.getHalfCarryFlag() == 0);
    REQUIRE(cpu.getCarryFlag() == 1);
}

TEST_CASE("STOP", "[OPCODE]")
{
    GameBoy gameboy;
    SM83 cpu;
    Memory mem;
    PPU ppu;

    cpu.memory = &mem;
    cpu.gameboy = &gameboy;
    mem.ppu = &ppu;
    ppu.memory = &mem;
    ppu.cpu = &cpu;

    cpu.PC = 0x0150;
    cpu.instructionCycle = 0;

    SECTION("DMG")
    {
        gameboy.emulatorMode = EmulatorMode::DMG;

        cpu.op_stop();

        REQUIRE(cpu.PC == 0x0152);
        REQUIRE(cpu.stop_signal == true);
    }

    SECTION("CGB")
    {
        gameboy.emulatorMode = EmulatorMode::CGB;
        mem.writebit(1, 0, 0xFF4D);

        cpu.op_stop();

        REQUIRE(cpu.PC == 0x0152);
        REQUIRE(gameboy.doubleSpeedMode);
        REQUIRE(gameboy.speedSwitchSleepCycles == GAMEBOY_SPEED_SWITCH_CLOCKS);
        REQUIRE(mem.readmem(0xFF4D) == 0x80);

        mem.writebit(1, 0, 0xFF4D);
        cpu.op_stop();

        REQUIRE(cpu.PC == 0x0154);
        REQUIRE_FALSE(gameboy.doubleSpeedMode);
        REQUIRE(gameboy.speedSwitchSleepCycles == GAMEBOY_SPEED_SWITCH_CLOCKS);
        REQUIRE(mem.readmem(0xFF4D) == 0);

        cpu.op_stop();
        REQUIRE(cpu.PC == 0x0156);
        REQUIRE_FALSE(gameboy.doubleSpeedMode);
        REQUIRE(mem.readmem(0xFF4D) == 0);
        REQUIRE(cpu.stop_signal);
    }
}