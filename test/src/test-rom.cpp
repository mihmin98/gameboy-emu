#include "catch.hpp"

#include "ROM.hpp"
#include <cstdlib>
#include <iostream>

TEST_CASE("Load ROM and set flags", "[ROM]")
{
    ROM rom;

    SECTION("Load a ROM that should fail checks")
    {
        bool loadResult = rom.loadROM("test/build/test_bad_nintendo_logo.gb");

        REQUIRE(loadResult);
        REQUIRE_FALSE(rom.nintendoLogoOk);
        REQUIRE_FALSE(rom.headerChecksumOk);
        REQUIRE_FALSE(rom.globalchecksumOk);
    }

    SECTION("Load a ROM that should pass all checks")
    {
        bool loadResult = rom.loadROM("test/build/test_no_mbc_ram.gb");

        REQUIRE(loadResult);
        REQUIRE(rom.nintendoLogoOk);
        REQUIRE(rom.headerChecksumOk);
        REQUIRE(rom.globalchecksumOk);
    }

    SECTION("Load a ROM and check header info")
    {
        bool loadResult = rom.loadROM("test/build/test_no_mbc_ram.gb");

        REQUIRE(loadResult);

        REQUIRE(rom.gameTitle == "NOMBC RAM");
        REQUIRE(rom.gbcFlag == 0x80);
        REQUIRE(rom.sgbFlag == 0x00);
        REQUIRE(rom.mbc == MBC::None);
        REQUIRE(rom.cartridgeRam == true);
        REQUIRE(rom.romSize == 0x8000);
        REQUIRE(rom.romBanks == 2);
        REQUIRE(rom.ramSize == 0x1000);
        REQUIRE(rom.ramBanks == 1);
        REQUIRE(rom.romVersion == 0);
    }
}

TEST_CASE("No MBC and No RAM Read", "[ROM]")
{
    ROM rom;
    bool loadResult = rom.loadROM("test/build/test_no_mbc_no_ram.gb");

    REQUIRE(loadResult);

    // Add some data in ROM
    for (uint32_t i = 0x0150; i < rom.romSize; ++i)
        rom.rom[i] = (uint8_t)i;

    SECTION("Read nonexisting RAM")
    {
        uint8_t val = rom.readmem(0xA000);
        REQUIRE(val == 0xFF);
    }

    SECTION("Read invalid address")
    {
        uint8_t val = rom.readmem(0x8000);
        REQUIRE(val == 0xFF);
    }

    SECTION("Read ROM")
    {
        uint8_t val = rom.readmem(0x0160);
        REQUIRE(val == 0x60);
    }
}

TEST_CASE("No MBC and RAM Read and Write", "[ROM]")
{
    ROM rom;
    bool loadResult = rom.loadROM("test/build/test_no_mbc_ram.gb");

    REQUIRE(loadResult);

    // Add some data in ROM
    for (uint32_t i = 0x0150; i < rom.romSize; ++i)
        rom.rom[i] = (uint8_t)i;

    // Add some data in RAM
    for (uint32_t i = 0; i < rom.ramSize; ++i)
        rom.ram[i] = (uint8_t)i;

    SECTION("Memory reads")
    {
        SECTION("Invalid address")
        {
            uint8_t val = rom.readmem(0x8000);
            REQUIRE(val == 0xFF);
        }

        SECTION("ROM")
        {
            uint8_t val = rom.readmem(0x0160);
            REQUIRE(val == 0x60);
        }

        SECTION("Disabled RAM")
        {
            uint8_t val = rom.readmem(0xA000);
            REQUIRE(val == 0xFF);
        }

        SECTION("Enabled RAM")
        {
            rom.ramEnable = true;
            uint8_t val = rom.readmem(0xA066);
            REQUIRE(val == 0x66);
        }
    }

    SECTION("Memory writes")
    {
        SECTION("Enable RAM")
        {
            rom.ramEnable = false;
            rom.writemem(0x0A, 0x1000);
            REQUIRE(rom.ramEnable);
        }

        SECTION("Disable RAM")
        {
            rom.ramEnable = true;
            rom.writemem(0x00, 0x1000);
            REQUIRE_FALSE(rom.ramEnable);
        }

        SECTION("RAM")
        {
            rom.ramEnable = true;
            rom.writemem(0x66, 0xA066);
            REQUIRE(rom.ram[0x0066] == 0x66);
        }
    }
}

TEST_CASE("MBC1 Read and Write")
{
    ROM rom;
    bool loadResult = rom.loadROM("test/build/test_mbc1.gb");

    REQUIRE(loadResult);

    // Add some data
    // Fill each rom bank with its number
    for (uint32_t i = 0x0150; i < rom.romSize; ++i)
        rom.rom[i] = i / 0x4000;

    // Fill each ram bank with its number
    for (uint32_t i = 0; i < rom.ramSize; ++i)
        rom.ram[i] = i / 0x2000;

    SECTION("Memory reads")
    {
        SECTION("Bank Mode 0")
        {
            rom.bankMode = 0;

            SECTION("ROM Bank 0x00")
            {
                uint8_t val = rom.readmem(0x1000);
                REQUIRE(val == 0x00);
            }

            SECTION("Switchable ROM Bank")
            {
                // Select Bank 0x02
                rom.currentROMBank = 0x02;
                uint8_t val = rom.readmem(0x4000);
                REQUIRE(val == 0x02);

                // Select Bank 0x22
                rom.currentRAMBank = 0x01;
                val = rom.readmem(0x4000);
                REQUIRE(val == 0x22);
            }

            SECTION("RAM Bank 0")
            {
                rom.ramEnable = true;
                uint8_t val = rom.readmem(0xA000);
                REQUIRE(val == 0x00);
            }
        }

        SECTION("Bank Mode 1")
        {
            rom.bankMode = 1;

            SECTION("ROM Bank 0x20/0x40/0x60")
            {
                // Select Bank 0x20
                rom.currentRAMBank = 1;
                uint8_t val = rom.readmem(0x1000);
                REQUIRE(val == 0x20);

                // Select Bank 0x40
                rom.currentRAMBank = 2;
                val = rom.readmem(0x1000);
                REQUIRE(val == 0x40);
            }

            SECTION("Switchable ROM Bank")
            {
                // Select ROM Bank 0x01
                rom.currentROMBank = 1;
                uint8_t val = rom.readmem(0x5000);
                REQUIRE(val == 0x01);

                // Select ROM Bank 0x10
                rom.currentROMBank = 0x10;
                val = rom.readmem(0x4000);
                REQUIRE(val == 0x10);
            }

            SECTION("RAM Banks")
            {
                rom.ramEnable = true;

                // RAM Bank 1
                rom.currentRAMBank = 1;
                uint8_t val = rom.readmem(0xA000);
                REQUIRE(val == 0x01);

                // RAM Bank 2
                rom.currentRAMBank = 2;
                val = rom.readmem(0xA000);
                REQUIRE(val == 0x02);
            }
        }
    }

    SECTION("Memory writes")
    {
        SECTION("RAM Enable")
        {
            rom.ramEnable = false;
            rom.writemem(0x0A, 0x1000);
            REQUIRE(rom.ramEnable);

            rom.writemem(0x00, 0x1000);
            REQUIRE_FALSE(rom.ramEnable);
        }

        SECTION("ROM Bank Number")
        {
            rom.writemem(0x03, 0x3000);
            REQUIRE(rom.currentROMBank == 0x03);

            rom.writemem(0x00, 0x3000);
            REQUIRE(rom.currentROMBank == 0x01);
        }

        SECTION("RAM Bank / Secondary ROM Bank")
        {
            rom.writemem(0x02, 0x5000);
            REQUIRE(rom.currentRAMBank == 0x02);

            rom.writemem(0x66, 0x5000);
            REQUIRE(rom.currentRAMBank == 0x02);
        }

        SECTION("Bank Mode")
        {
            rom.writemem(0x01, 0x7000);
            REQUIRE(rom.bankMode == 0x01);

            rom.writemem(0x00, 0x7000);
            REQUIRE(rom.bankMode == 0x00);
        }

        SECTION("RAM")
        {
            SECTION("Bank Mode 0")
            {
                rom.bankMode = 0;
                rom.writemem(0x66, 0xA066);
                REQUIRE(rom.ram[0x66] == 0x66);
            }

            SECTION("Bank Mode 1")
            {
                rom.bankMode = 1;
                rom.currentRAMBank = 1;
                rom.writemem(0x66, 0xA066);
                REQUIRE(rom.ram[0x2066] == 0x66);
            }
        }
    }
}

TEST_CASE("MBC2 Read and Write", "[ROM]")
{
    ROM rom;
    bool loadResult = rom.loadROM("test/build/test_mbc2.gb");

    REQUIRE(loadResult);

    // Add some data
    // Fill each rom bank with its number
    for (uint32_t i = 0x0150; i < rom.romSize; ++i)
        rom.rom[i] = i / 0x4000;

    // Fill ram
    for (uint32_t i = 0; i < rom.ramSize; ++i)
        rom.ram[i] = i & 0xF;

    SECTION("Memory reads")
    {
        SECTION("ROM Bank 0")
        {
            uint8_t val = rom.readmem(0x1000);
            REQUIRE(val == 0x00);
        }

        SECTION("Switchable ROM Bank")
        {
            // Select ROM Bank 0x01
            rom.currentROMBank = 1;
            uint8_t val = rom.readmem(0x4000);
            REQUIRE(val == 0x01);

            // Select ROM Bank 0x03
            rom.currentROMBank = 3;
            val = rom.readmem(0x4000);
            REQUIRE(val == 0x03);
        }

        SECTION("RAM")
        {
            rom.ramEnable = true;
            uint8_t val = rom.readmem(0xA001);
            REQUIRE(val == 0x01);
        }
    }

    SECTION("Memory writes")
    {
        SECTION("RAM Enable")
        {
            rom.ramEnable = false;

            rom.writemem(0x0A, 0x0100);
            REQUIRE_FALSE(rom.ramEnable);

            rom.writemem(0x0A, 0x0200);
            REQUIRE(rom.ramEnable);

            rom.writemem(0x00, 0x0100);
            REQUIRE(rom.ramEnable);

            rom.writemem(0x00, 0x0200);
            REQUIRE_FALSE(rom.ramEnable);
        }

        SECTION("ROM Bank")
        {
            rom.currentROMBank = 1;

            rom.writemem(0x02, 0x2200);
            REQUIRE(rom.currentROMBank == 1);

            rom.writemem(0x02, 0x2300);
            REQUIRE(rom.currentROMBank == 2);

            rom.writemem(0x00, 0x2300);
            REQUIRE(rom.currentROMBank == 1);
        }

        SECTION("RAM")
        {
            rom.ramEnable = true;

            rom.writemem(0x66, 0xA066);
            REQUIRE(rom.ram[0x66] == 0x06);
        }
    }
}

TEST_CASE("MBC3 Read and Write", "[ROM]")
{
    ROM rom;
    bool loadResult = rom.loadROM("test/build/test_mbc3.gb");

    REQUIRE(loadResult);

    // Add some data
    // Fill each rom bank with its number
    for (uint32_t i = 0x0150; i < rom.romSize; ++i)
        rom.rom[i] = i / 0x4000;

    // Fill each ram bank with its number
    for (uint32_t i = 0; i < rom.ramSize; ++i)
        rom.ram[i] = i / 0x2000;

    // Set RTC registers
    rom.rtcS = 0x08;
    rom.rtcM = 0x09;
    rom.rtcH = 0x0A;
    rom.rtcDL = 0x0B;
    rom.rtcDH = 0x0C;

    SECTION("Memory reads")
    {
        SECTION("ROM Bank 0")
        {
            uint8_t val = rom.readmem(0x1000);
            REQUIRE(val == 0x00);
        }

        SECTION("Switchable ROM Bank")
        {
            // Select ROM Bank 1
            rom.currentROMBank = 1;
            uint8_t val = rom.readmem(0x4000);
            REQUIRE(val == 0x01);

            // Selct ROM Bank 2
            rom.currentROMBank = 2;
            val = rom.readmem(0x4000);
            REQUIRE(val == 0x02);
        }

        SECTION("RAM")
        {
            rom.ramEnable = true;

            // Select RAM Bank 0
            rom.currentRAMBank = 0;
            uint8_t val = rom.readmem(0xA000);
            REQUIRE(val == 0x00);

            // Select RAM Bank 1
            rom.currentRAMBank = 1;
            val = rom.readmem(0xA000);
            REQUIRE(val == 0x01);
        }

        SECTION("RTC Registers")
        {
            rom.ramEnable = true;

            SECTION("RTC S")
            {
                rom.currentRAMBank = 0x08;
                uint8_t val = rom.readmem(0xA000);
                REQUIRE(val == 0x08);
            }

            SECTION("RTC M")
            {
                rom.currentRAMBank = 0x09;
                uint8_t val = rom.readmem(0xA000);
                REQUIRE(val == 0x09);
            }

            SECTION("RTC H")
            {
                rom.currentRAMBank = 0x0A;
                uint8_t val = rom.readmem(0xA000);
                REQUIRE(val == 0x0A);
            }

            SECTION("RTC DL")
            {
                rom.currentRAMBank = 0x0B;
                uint8_t val = rom.readmem(0xA000);
                REQUIRE(val == 0x0B);
            }

            SECTION("RTC DH")
            {
                rom.currentRAMBank = 0x0C;
                uint8_t val = rom.readmem(0xA000);
                REQUIRE(val == 0x0C);
            }
        }
    }

    SECTION("Memory writes")
    {
        SECTION("RAM / Timer Enable")
        {
            rom.ramEnable = false;

            rom.writemem(0x0A, 0x1000);
            REQUIRE(rom.ramEnable);

            rom.writemem(0x00, 0x1000);
            REQUIRE_FALSE(rom.ramEnable);
        }

        SECTION("ROM Bank Number")
        {
            rom.writemem(0x02, 0x3000);
            REQUIRE(rom.currentROMBank == 0x02);

            rom.writemem(0x00, 0x3000);
            REQUIRE(rom.currentROMBank == 0x01);
        }

        SECTION("RAM Bank Number / RTC Register Select")
        {
            rom.writemem(0x02, 0x5000);
            REQUIRE(rom.currentRAMBank == 0x02);
        }

        SECTION("Latch Clock Data")
        {
            rom.rtcLatch = false;

            rom.writemem(0x00, 0x7000);
            REQUIRE(rom.rtcLatchClockLastWritten == 0x00);
            REQUIRE(rom.rtcLatch == false);
            rom.writemem(0x01, 0x7000);
            REQUIRE(rom.rtcLatchClockLastWritten == 0x01);
            REQUIRE(rom.rtcLatch == true);

            rom.writemem(0x00, 0x7000);
            REQUIRE(rom.rtcLatchClockLastWritten == 0x00);
            REQUIRE(rom.rtcLatch == true);
            rom.writemem(0x01, 0x7000);
            REQUIRE(rom.rtcLatchClockLastWritten == 0x01);
            REQUIRE(rom.rtcLatch == false);
        }

        SECTION("RAM")
        {
            rom.ramEnable = true;

            // Select RAM Bank 0
            rom.currentRAMBank = 0;
            rom.writemem(0x66, 0xA066);
            REQUIRE(rom.ram[0x66] == 0x66);

            // Select RAM Bank 1
            rom.currentRAMBank = 1;
            rom.writemem(0x66, 0xA066);
            REQUIRE(rom.ram[0x2066] == 0x66);
        }

        SECTION("RTC Registers")
        {
            rom.ramEnable = true;

            SECTION("RTC S")
            {
                rom.currentRAMBank = 0x08;
                rom.writemem(0x66, 0xA000);
                REQUIRE(rom.rtcS == 0x66);
            }

            SECTION("RTC M")
            {
                rom.currentRAMBank = 0x09;
                rom.writemem(0x66, 0xA000);
                REQUIRE(rom.rtcM == 0x66);
            }

            SECTION("RTC H")
            {
                rom.currentRAMBank = 0x0A;
                rom.writemem(0x66, 0xA000);
                REQUIRE(rom.rtcH == 0x66);
            }

            SECTION("RTC DL")
            {
                rom.currentRAMBank = 0x0B;
                rom.writemem(0x66, 0xA000);
                REQUIRE(rom.rtcDL == 0x66);
            }

            SECTION("RTC DH")
            {
                rom.currentRAMBank = 0x0C;
                rom.writemem(0x66, 0xA000);
                REQUIRE(rom.rtcDH == 0x66);
            }
        }
    }
}

TEST_CASE("MBC5 Read and Write", "[ROM]")
{
    ROM rom;
    bool loadResult = rom.loadROM("test/build/test_mbc5.gb");

    REQUIRE(loadResult);

    // Add some data
    // Fill each rom bank with its number
    for (uint32_t i = 0x0150; i < rom.romSize; ++i)
        rom.rom[i] = i / 0x4000;

    // Fill each ram bank with its number
    for (uint32_t i = 0; i < rom.ramSize; ++i)
        rom.ram[i] = i / 0x2000;

    SECTION("Memory reads")
    {
        SECTION("ROM Bank 0")
        {
            uint8_t val = rom.readmem(0x1000);
            REQUIRE(val == 0x00);
        }

        SECTION("Switchable ROM Bank")
        {
            // Select ROM Bank 1
            rom.currentROMBank = 1;
            uint8_t val = rom.readmem(0x4000);
            REQUIRE(val == 0x01);

            // Select ROM Bank 2
            rom.currentROMBank = 2;
            val = rom.readmem(0x4000);
            REQUIRE(val == 0x02);
        }

        SECTION("RAM")
        {
            rom.ramEnable = true;

            // Select RAM Bank 0
            rom.currentRAMBank = 0;
            uint8_t val = rom.readmem(0xA000);
            REQUIRE(val == 0x00);

            // Select RAM Bank 1
            rom.currentRAMBank = 1;
            val = rom.readmem(0xA000);
            REQUIRE(val == 0x01);
        }
    }

    SECTION("Memory writes")
    {
        SECTION("RAM Enable")
        {
            rom.ramEnable = false;

            rom.writemem(0x0A, 0x1000);
            REQUIRE(rom.ramEnable);

            rom.writemem(0x00, 0x1000);
            REQUIRE_FALSE(rom.ramEnable);
        }

        SECTION("ROM Bank Number low bits")
        {
            rom.currentROMBank = 0x105;

            rom.writemem(0x07, 0x2800);
            REQUIRE(rom.currentROMBank == 0x107);
        }

        SECTION("ROM Bank Number high bit")
        {
            rom.currentROMBank = 0x66;

            rom.writemem(0x01, 0x3800);
            REQUIRE(rom.currentROMBank == 0x166);
        }

        SECTION("RAM Bank Number")
        {
            rom.currentRAMBank = 0;

            rom.writemem(0x02, 0x5000);
            REQUIRE(rom.currentRAMBank == 2);
        }

        SECTION("RAM")
        {
            rom.ramEnable = true;

            // Select RAM Bank 0
            rom.currentRAMBank = 0;
            rom.writemem(0x66, 0xA066);
            REQUIRE(rom.ram[0x66] == 0x66);

            // Select RAM Bank 1
            rom.currentRAMBank = 1;
            rom.writemem(0x66, 0xA066);
            REQUIRE(rom.ram[0x2066] == 0x66);
        }
    }
}