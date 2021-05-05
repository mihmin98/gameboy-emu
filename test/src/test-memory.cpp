#include "catch.hpp"

#include "Memory.hpp"
#include "TestConstants.hpp"
#include "PPU.hpp"
#include <cstdlib>
#include <experimental/filesystem>
#include <iostream>

namespace fs = std::experimental::filesystem;

TEST_CASE("Read Memory", "[MEM]")
{
    Memory mem;
    PPU ppu;
    ROM rom;

    mem.ppu = &ppu;
    ppu.memory = &mem;

    fs::path romDirPath = fs::current_path() / TestConstants::testRomsDir;

    rom.loadROM(romDirPath / "test_mbc5.gb");

    // Add data to ROM
    // Fill each rom bank with its number
    for (uint32_t i = 0x0150; i < rom.romSize; ++i)
        rom.rom[i] = i / 0x4000;

    // Fill each ram bank with its number
    for (uint32_t i = 0; i < rom.ramSize; ++i)
        rom.ram[i] = i / 0x2000;

    mem.rom = &rom;

    // Fill VRAM with its bank number
    for (uint32_t i = 0; i < 2 * 0x2000; ++i)
        mem.vram[i] = i / 0x2000;

    // Fill WRAM with its bank number
    for (uint32_t i = 0; i < 8 * 0x1000; ++i)
        mem.wram[i] = i / 0x1000;

    // Fill OAM
    for (uint32_t i = 0; i < 0xA0; ++i)
        mem.oam[i] = i;

    // Fill some I/O Registers
    for (uint32_t i = 0; i < 0x20; ++i)
        mem.ioRegisters[i] = i;

    // Fill HRAM
    for (uint32_t i = 0; i < 0x7F; ++i)
        mem.hram[i] = i;

    mem.ieRegister = 0x66;

    SECTION("ROM")
    {
        uint8_t val = mem.readmem(0x1000);
        REQUIRE(val == 0x00);

        val = mem.readmem(0x4000);
        REQUIRE(val == 0x01);
    }

    SECTION("External RAM")
    {
        rom.ramEnable = true;

        uint8_t val = mem.readmem(0xA000);
        REQUIRE(val == 0x00);
    }

    SECTION("VRAM")
    {
        // VRAM Bank 0
        uint8_t val = mem.readmem(0x8000);
        REQUIRE(val == 0x00);

        // VRAM Bank 1
        mem.setCurrentVramBank(1);
        val = mem.readmem(0x8000);
        REQUIRE(val == 0x01);
    }

    SECTION("WRAM Bank 0")
    {
        uint8_t val = mem.readmem(0xC000);
        REQUIRE(val == 0x00);
    }

    SECTION("Switchable WRAM Bank")
    {
        // WRAM Bank 1
        mem.setCurrentWramBank(1);
        uint8_t val = mem.readmem(0xD000);
        REQUIRE(val == 0x01);

        // WRAM Bank 3
        mem.setCurrentWramBank(3);
        val = mem.readmem(0xD000);
        REQUIRE(val == 0x03);
    }

    SECTION("ECHO RAM")
    {
        // Bank 0
        uint8_t val = mem.readmem(0xE000);
        REQUIRE(val == 0x00);

        // Bank 1
        mem.setCurrentWramBank(1);
        val = mem.readmem(0xF000);
        REQUIRE(val == 0x01);
    }

    SECTION("OAM")
    {
        uint8_t val = mem.readmem(0xFE66);
        REQUIRE(val == 0x66);
    }

    SECTION("Unused")
    {
        uint8_t val = mem.readmem(0xFEED);
        REQUIRE(val == 0x00);
    }

    SECTION("I/O Registers")
    {
        uint8_t val = mem.readmem(0xFF16);
        REQUIRE(val == 0x16);
    }

    SECTION("HRAM")
    {
        uint8_t val = mem.readmem(0xFF86);
        REQUIRE(val == 0x06);
    }

    SECTION("IE Register")
    {
        uint8_t val = mem.readmem(0xFFFF);
        REQUIRE(val == 0x66);
    }
}

TEST_CASE("Write Memory", "[MEM]")
{
    Memory mem;
    PPU ppu;
    ROM rom;

    mem.ppu = &ppu;
    ppu.memory = &mem;

    fs::path romDirPath = fs::current_path() / TestConstants::testRomsDir;

    rom.loadROM(romDirPath / "test_mbc5.gb");
    mem.rom = &rom;

    SECTION("ROM")
    {
        rom.ramEnable = false;
        mem.writemem(0x0A, 0x1000);
        REQUIRE(rom.ramEnable);
    }

    SECTION("External RAM")
    {
        rom.ramEnable = true;
        mem.writemem(0x66, 0xA000);
        REQUIRE(rom.readmem(0xA000) == 0x66);
    }

    SECTION("VRAM")
    {
        // WRAM Bank 0
        mem.setCurrentVramBank(0);
        mem.writemem(0x66, 0x8066);
        REQUIRE(mem.vram[0x0066] == 0x66);

        // WRAM Bank 1
        mem.setCurrentVramBank(1);
        mem.writemem(0x66, 0x8066);
        REQUIRE(mem.vram[0x2066] == 0x66);
    }

    SECTION("WRAM Bank 0")
    {
        mem.writemem(0x66, 0xC066);
        REQUIRE(mem.wram[0x66] == 0x66);
    }

    SECTION("Switchable WRAM Bank")
    {
        // WRAM Bank 1
        mem.setCurrentWramBank(1);
        mem.writemem(0x66, 0xD066);
        REQUIRE(mem.wram[0x1066] == 0x66);

        // WRAM Bank 3
        mem.setCurrentWramBank(3);
        mem.writemem(0x66, 0xD066);
        REQUIRE(mem.wram[0x3066] == 0x66);
    }

    SECTION("ECHO RAM")
    {
        // WRAM Bank 0
        mem.writemem(0x66, 0xE066);
        REQUIRE(mem.wram[0x66] == 0x66);

        // WRAM Bank 3
        mem.setCurrentWramBank(3);
        mem.writemem(0x66, 0xF066);
        REQUIRE(mem.wram[0x3066] == 0x66);
    }

    SECTION("OAM")
    {
        mem.writemem(0x66, 0xFE66);
        REQUIRE(mem.oam[0x66] == 0x66);
    }

    SECTION("I/O Registers")
    {
        mem.writemem(0x66, 0xFF16);
        REQUIRE(mem.ioRegisters[0x16] == 0x66);
    }

    SECTION("HRAM")
    {
        mem.writemem(0x66, 0xFFAC);
        REQUIRE(mem.hram[0x2C] == 0x66);
    }

    SECTION("IE Register")
    {
        mem.writemem(0x66, 0xFFFF);
        REQUIRE(mem.ieRegister == 0x66);
    }
}
