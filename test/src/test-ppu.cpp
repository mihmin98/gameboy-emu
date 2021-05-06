#include "catch.hpp"

#include "Memory.hpp"
#include "PPU.hpp"
#include <cstdlib>
#include <iostream>

TEST_CASE("LCD Control", "[PPU]")
{
    PPU ppu;
    Memory mem;

    ppu.memory = &mem;
    mem.ppu = &ppu;

    SECTION("LCD Display Enable")
    {
        SECTION("Get")
        {
            mem.writemem(0x80, 0xFF40);
            REQUIRE(ppu.getLcdDisplayEnable() == 1);

            mem.writemem(0x00, 0xFF40);
            REQUIRE(ppu.getLcdDisplayEnable() == 0);
        }

        SECTION("Set")
        {
            mem.writemem(0x83, 0xFF40);

            ppu.setLcdDisplayEnable(0);
            REQUIRE(mem.readmem(0xFF40) == 0x3);

            ppu.setLcdDisplayEnable(1);
            REQUIRE(mem.readmem(0xFF40) == 0x83);
        }
    }

    SECTION("Window Tile Map Display Select")
    {
        SECTION("Get")
        {
            mem.writemem(0x40, 0xFF40);
            REQUIRE(ppu.getWindowTileMapDisplaySelect() == 1);

            mem.writemem(0x00, 0xFF40);
            REQUIRE(ppu.getWindowTileMapDisplaySelect() == 0);
        }

        SECTION("Set")
        {
            mem.writemem(0x43, 0xFF40);

            ppu.setWindowTileMapDisplaySelect(0);
            REQUIRE(mem.readmem(0xFF40) == 0x3);

            ppu.setWindowTileMapDisplaySelect(1);
            REQUIRE(mem.readmem(0xFF40) == 0x43);
        }
    }

    SECTION("Window Display Enable")
    {
        SECTION("Get")
        {
            mem.writemem(0x20, 0xFF40);
            REQUIRE(ppu.getWindowDisplayEnable() == 1);

            mem.writemem(0x00, 0xFF40);
            REQUIRE(ppu.getWindowDisplayEnable() == 0);
        }

        SECTION("Set")
        {
            mem.writemem(0x23, 0xFF40);

            ppu.setWindowDisplayEnable(0);
            REQUIRE(mem.readmem(0xFF40) == 0x3);

            ppu.setWindowDisplayEnable(1);
            REQUIRE(mem.readmem(0xFF40) == 0x23);
        }
    }

    SECTION("BG and Window Tile Data Select")
    {
        SECTION("Get")
        {
            mem.writemem(0x10, 0xFF40);
            REQUIRE(ppu.getBgAndWindowTileDataSelect() == 1);

            mem.writemem(0x00, 0xFF40);
            REQUIRE(ppu.getBgAndWindowTileDataSelect() == 0);
        }

        SECTION("Set")
        {
            mem.writemem(0x13, 0xFF40);

            ppu.setBgAndWindowTileDataSelect(0);
            REQUIRE(mem.readmem(0xFF40) == 0x3);

            ppu.setBgAndWindowTileDataSelect(1);
            REQUIRE(mem.readmem(0xFF40) == 0x13);
        }
    }

    SECTION("BG Tile Map Display Select")
    {
        SECTION("Get")
        {
            mem.writemem(0x8, 0xFF40);
            REQUIRE(ppu.getBgTileMapDisplaySelect() == 1);

            mem.writemem(0x00, 0xFF40);
            REQUIRE(ppu.getBgTileMapDisplaySelect() == 0);
        }

        SECTION("Set")
        {
            mem.writemem(0x88, 0xFF40);

            ppu.setBgTileMapDisplaySelect(0);
            REQUIRE(mem.readmem(0xFF40) == 0x80);

            ppu.setBgTileMapDisplaySelect(1);
            REQUIRE(mem.readmem(0xFF40) == 0x88);
        }
    }

    SECTION("OBJ Size")
    {
        SECTION("Get")
        {
            mem.writemem(0x4, 0xFF40);
            REQUIRE(ppu.getObjSize() == 1);

            mem.writemem(0x00, 0xFF40);
            REQUIRE(ppu.getObjSize() == 0);
        }

        SECTION("Set")
        {
            mem.writemem(0x84, 0xFF40);

            ppu.setObjSize(0);
            REQUIRE(mem.readmem(0xFF40) == 0x80);

            ppu.setObjSize(1);
            REQUIRE(mem.readmem(0xFF40) == 0x84);
        }
    }

    SECTION("OBJ Display Enable")
    {
        SECTION("Get")
        {
            mem.writemem(0x2, 0xFF40);
            REQUIRE(ppu.getObjDisplayEnable() == 1);

            mem.writemem(0x00, 0xFF40);
            REQUIRE(ppu.getObjDisplayEnable() == 0);
        }

        SECTION("Set")
        {
            mem.writemem(0x82, 0xFF40);

            ppu.setObjDisplayEnable(0);
            REQUIRE(mem.readmem(0xFF40) == 0x80);

            ppu.setObjDisplayEnable(1);
            REQUIRE(mem.readmem(0xFF40) == 0x82);
        }
    }

    SECTION("BG/Window Display/Priority")
    {
        SECTION("Get")
        {
            mem.writemem(0x1, 0xFF40);
            REQUIRE(ppu.getBgWindowDisplayPriority() == 1);

            mem.writemem(0x00, 0xFF40);
            REQUIRE(ppu.getBgWindowDisplayPriority() == 0);
        }

        SECTION("Set")
        {
            mem.writemem(0x81, 0xFF40);

            ppu.setBgWindowDisplayPriority(0);
            REQUIRE(mem.readmem(0xFF40) == 0x80);

            ppu.setBgWindowDisplayPriority(1);
            REQUIRE(mem.readmem(0xFF40) == 0x81);
        }
    }
}

TEST_CASE("LCD STAT", "[PPU]")
{
    PPU ppu;
    Memory mem;

    ppu.memory = &mem;
    mem.ppu = &ppu;

    SECTION("LYC = LY Coincidence")
    {
        SECTION("Get")
        {
            mem.writemem(0x40, 0xFF41);
            REQUIRE(ppu.getLycLyCoincidence() == 1);

            mem.writemem(0x00, 0xFF41);
            REQUIRE(ppu.getLycLyCoincidence() == 0);
        }

        SECTION("Set")
        {
            mem.writemem(0x43, 0xFF41);

            ppu.setLycLyCoincidence(0);
            REQUIRE(mem.readmem(0xFF41) == 0x3);

            ppu.setLycLyCoincidence(1);
            REQUIRE(mem.readmem(0xFF41) == 0x43);
        }
    }

    SECTION("Mode 2 OAM Interrupt")
    {
        SECTION("Get")
        {
            mem.writemem(0x20, 0xFF41);
            REQUIRE(ppu.getMode2OamInterrupt() == 1);

            mem.writemem(0x00, 0xFF41);
            REQUIRE(ppu.getMode2OamInterrupt() == 0);
        }

        SECTION("Set")
        {
            mem.writemem(0x23, 0xFF41);

            ppu.setMode2OamInterrupt(0);
            REQUIRE(mem.readmem(0xFF41) == 0x3);

            ppu.setMode2OamInterrupt(1);
            REQUIRE(mem.readmem(0xFF41) == 0x23);
        }
    }

    SECTION("Mode 1 VBlank Interrupt")
    {
        SECTION("Get")
        {
            mem.writemem(0x10, 0xFF41);
            REQUIRE(ppu.getMode1VBlankInterrupt() == 1);

            mem.writemem(0x00, 0xFF41);
            REQUIRE(ppu.getMode1VBlankInterrupt() == 0);
        }

        SECTION("Set")
        {
            mem.writemem(0x13, 0xFF41);

            ppu.setMode1VBlankInterrupt(0);
            REQUIRE(mem.readmem(0xFF41) == 0x3);

            ppu.setMode1VBlankInterrupt(1);
            REQUIRE(mem.readmem(0xFF41) == 0x13);
        }
    }

    SECTION("Mode 0 HBlank Interrupt")
    {
        SECTION("Get")
        {
            mem.writemem(0x8, 0xFF41);
            REQUIRE(ppu.getMode0HBlankInterrupt() == 1);

            mem.writemem(0x00, 0xFF41);
            REQUIRE(ppu.getMode0HBlankInterrupt() == 0);
        }

        SECTION("Set")
        {
            mem.writemem(0x48, 0xFF41);

            ppu.setMode0HBlankInterrupt(0);
            REQUIRE(mem.readmem(0xFF41) == 0x40);

            ppu.setMode0HBlankInterrupt(1);
            REQUIRE(mem.readmem(0xFF41) == 0x48);
        }
    }

    SECTION("Coincidence Flag")
    {
        SECTION("Get")
        {
            mem.writemem(0x4, 0xFF41);
            REQUIRE(ppu.getCoincidenceFlag() == 1);

            mem.writemem(0x00, 0xFF41);
            REQUIRE(ppu.getCoincidenceFlag() == 0);
        }

        SECTION("Set")
        {
            mem.writemem(0x44, 0xFF41);

            ppu.setCoincidenceFlag(0);
            REQUIRE(mem.readmem(0xFF41) == 0x40);

            ppu.setCoincidenceFlag(1);
            REQUIRE(mem.readmem(0xFF41) == 0x44);
        }
    }

    SECTION("LCD Mode Flag")
    {
        SECTION("Get")
        {
            mem.writemem(0x0, 0xFF41);
            REQUIRE(ppu.getModeFlag() == 0);
            REQUIRE(ppu.getLcdMode() == LcdMode::H_BLANK);

            mem.writemem(0x1, 0xFF41);
            REQUIRE(ppu.getModeFlag() == 1);
            REQUIRE(ppu.getLcdMode() == LcdMode::V_BLANK);

            mem.writemem(0x2, 0xFF41);
            REQUIRE(ppu.getModeFlag() == 2);
            REQUIRE(ppu.getLcdMode() == LcdMode::OAM_SEARCH);

            mem.writemem(0x3, 0xFF41);
            REQUIRE(ppu.getModeFlag() == 3);
            REQUIRE(ppu.getLcdMode() == LcdMode::DRAW);
        }

        SECTION("Set")
        {
            mem.writemem(0x43, 0xFF41);

            ppu.setModeFlag(0);
            REQUIRE(mem.readmem(0xFF41) == 0x40);

            ppu.setModeFlag(1);
            REQUIRE(mem.readmem(0xFF41) == 0x41);

            ppu.setModeFlag(2);
            REQUIRE(mem.readmem(0xFF41) == 0x42);

            ppu.setModeFlag(3);
            REQUIRE(mem.readmem(0xFF41) == 0x43);
        }
    }
}

TEST_CASE("LCD Position and Scrolling", "[PPU]")
{
    PPU ppu;
    Memory mem;

    ppu.memory = &mem;
    mem.ppu = &ppu;

    SECTION("Scroll Y")
    {
        SECTION("Get")
        {
            mem.writemem(0x66, 0xFF42);
            REQUIRE(ppu.getScrollY() == 0x66);
        }

        SECTION("Set")
        {
            mem.writemem(0x0, 0xFF42);
            ppu.setScrollY(0x66);
            REQUIRE(mem.readmem(0xFF42) == 0x66);
        }
    }

    SECTION("Scroll X")
    {
        SECTION("Get")
        {
            mem.writemem(0x66, 0xFF43);
            REQUIRE(ppu.getScrollX() == 0x66);
        }

        SECTION("Set")
        {
            mem.writemem(0x0, 0xFF43);
            ppu.setScrollX(0x66);
            REQUIRE(mem.readmem(0xFF43) == 0x66);
        }
    }

    SECTION("LY")
    {
        SECTION("Get")
        {
            mem.writemem(0x66, 0xFF44);
            REQUIRE(ppu.getLy() == 0x66);
        }

        SECTION("Set")
        {
            mem.writemem(0x0, 0xFF44);
            ppu.setLy(0x66);
            REQUIRE(mem.readmem(0xFF44) == 0x66);
        }
    }

    SECTION("LYC")
    {
        SECTION("Get")
        {
            mem.writemem(0x66, 0xFF45);
            REQUIRE(ppu.getLyc() == 0x66);
        }

        SECTION("Set")
        {
            mem.writemem(0x0, 0xFF45);
            ppu.setLyc(0x66);
            REQUIRE(mem.readmem(0xFF45) == 0x66);
        }
    }

    SECTION("Window Y")
    {
        SECTION("Get")
        {
            mem.writemem(0x66, 0xFF4A);
            REQUIRE(ppu.getWy() == 0x66);
        }

        SECTION("Set")
        {
            mem.writemem(0x0, 0xFF4A);
            ppu.setWy(0x66);
            REQUIRE(mem.readmem(0xFF4A) == 0x66);
        }
    }

    SECTION("Window X")
    {
        SECTION("Get")
        {
            mem.writemem(0x66, 0xFF4B);
            REQUIRE(ppu.getWx() == 0x66);
        }

        SECTION("Set")
        {
            mem.writemem(0x0, 0xFF4B);
            ppu.setWx(0x66);
            REQUIRE(mem.readmem(0xFF4B) == 0x66);
        }
    }
}

TEST_CASE("LCD Monochrome Palettes", "[PPU]")
{
    PPU ppu;
    Memory mem;

    ppu.memory = &mem;
    mem.ppu = &ppu;

    SECTION("BG Palette Data")
    {
        SECTION("Get")
        {
            mem.writemem(0x66, 0xFF47);
            REQUIRE(ppu.getBgPaletteData() == 0x66);
        }

        SECTION("Set")
        {
            mem.writemem(0x0, 0xFF47);
            ppu.setBgPaletteData(0x66);
            REQUIRE(mem.readmem(0xFF47) == 0x66);
        }
    }

    SECTION("OBJ Palette 0 Data")
    {
        SECTION("Get")
        {
            mem.writemem(0x66, 0xFF48);
            REQUIRE(ppu.getObjPalette0Data() == 0x66);
        }

        SECTION("Set")
        {
            mem.writemem(0x0, 0xFF48);
            ppu.setObjPalette0Data(0x66);
            REQUIRE(mem.readmem(0xFF48) == 0x66);
        }
    }

    SECTION("OBJ Palette 1 Data")
    {
        SECTION("Get")
        {
            mem.writemem(0x66, 0xFF49);
            REQUIRE(ppu.getObjPalette1Data() == 0x66);
        }

        SECTION("Set")
        {
            mem.writemem(0x0, 0xFF49);
            ppu.setObjPalette1Data(0x66);
            REQUIRE(mem.readmem(0xFF49) == 0x66);
        }
    }
}

TEST_CASE("LCD Color Palettes", "[PPU]")
{
    PPU ppu;
    Memory mem;

    ppu.memory = &mem;
    mem.ppu = &ppu;

    ppu.emulatorMode = EmulatorMode::CGB;
    mem.mode = EmulatorMode::CGB;

    SECTION("BG Color Palette Index")
    {
        SECTION("Get")
        {
            mem.writemem(0x66, 0xFF68);
            REQUIRE(ppu.getBgColorPaletteIndex() == 0x66);
        }

        SECTION("Set")
        {
            mem.writemem(0x0, 0xFF68);
            ppu.setBgColorPaletteIndex(0x66);
            REQUIRE(mem.readmem(0xFF68) == 0x66);
        }
    }

    SECTION("BG Color Palette Data")
    {
        SECTION("Get")
        {
            mem.writemem(0x3, 0xFF68);
            mem.cgbBgColorPalette[3] = 0x66;
            REQUIRE(ppu.getBgColorPaletteData() == 0x66);
        }

        SECTION("Set")
        {
            mem.writemem(0x83, 0xFF68);
            mem.cgbBgColorPalette[3] = 0;
            ppu.setBgColorPaletteData(0x66);
            REQUIRE(mem.cgbBgColorPalette[3] == 0x66);
            REQUIRE(mem.readmem(0xFF68) == 0x84);

            mem.writemem(0x04, 0xFF68);
            mem.cgbBgColorPalette[4] = 0;
            ppu.setBgColorPaletteData(0x66);
            REQUIRE(mem.cgbBgColorPalette[4] == 0x66);
            REQUIRE(mem.readmem(0xFF68) == 0x04);
        }
    }

    SECTION("OBJ Color Palette Index")
    {
        SECTION("Get")
        {
            mem.writemem(0x66, 0xFF6A);
            REQUIRE(ppu.getObjColorPaletteIndex() == 0x66);
        }

        SECTION("Set")
        {
            mem.writemem(0x0, 0xFF6A);
            ppu.setObjColorPaletteIndex(0x66);
            REQUIRE(mem.readmem(0xFF6A) == 0x66);
        }
    }

    SECTION("OBJ Color Palette Data")
    {
        SECTION("Get")
        {
            mem.writemem(0x3, 0xFF6A);
            mem.cgbObjColorPalette[3] = 0x66;
            REQUIRE(ppu.getObjColorPaletteData() == 0x66);
        }

        SECTION("Set")
        {
            mem.writemem(0x83, 0xFF6A);
            mem.cgbBgColorPalette[3] = 0;
            ppu.setObjColorPaletteData(0x66);
            REQUIRE(mem.cgbObjColorPalette[3] == 0x66);
            REQUIRE(mem.readmem(0xFF6A) == 0x84);

            mem.writemem(0x04, 0xFF6A);
            mem.cgbObjColorPalette[4] = 0;
            ppu.setObjColorPaletteData(0x66);
            REQUIRE(mem.cgbObjColorPalette[4] == 0x66);
            REQUIRE(mem.readmem(0xFF6A) == 0x04);
        }
    }
}

TEST_CASE("OAM DMA", "[PPU]")
{
    PPU ppu;
    Memory mem;

    ppu.memory = &mem;
    mem.ppu = &ppu;

    SECTION("Get")
    {
        mem.ioRegisters[0xFF46 - MEM_IO_START] = 0x66;
        REQUIRE(ppu.getOamDma() == 0x66);
    }

    SECTION("Set")
    {
        ppu.setOamDma(0x66);
        REQUIRE(mem.readmem(0xFF46, true, true) == 0x66);
        REQUIRE(ppu.oamDmaActive == true);
    }
}

TEST_CASE("VRAM DMA", "[PPU]")
{
    PPU ppu;
    Memory mem;

    ppu.memory = &mem;
    mem.ppu = &ppu;

    ppu.emulatorMode = EmulatorMode::CGB;
    mem.mode = EmulatorMode::CGB;

    SECTION("HDMA1")
    {
        SECTION("Get")
        {
            mem.writemem(0x66, 0xFF51);
            REQUIRE(ppu.getHdma1() == 0x66);
        }

        SECTION("Set")
        {
            mem.writemem(0x0, 0xFF51);
            ppu.setHdma1(0x66);
            REQUIRE(mem.readmem(0xFF51) == 0x66);
        }
    }

    SECTION("HDMA2")
    {
        SECTION("Get")
        {
            mem.writemem(0x66, 0xFF52);
            REQUIRE(ppu.getHdma2() == 0x66);
        }

        SECTION("Set")
        {
            mem.writemem(0x0, 0xFF52);
            ppu.setHdma2(0x66);
            REQUIRE(mem.readmem(0xFF52) == 0x66);
        }
    }

    SECTION("HDMA3")
    {
        SECTION("Get")
        {
            mem.writemem(0x66, 0xFF53);
            REQUIRE(ppu.getHdma3() == 0x66);
        }

        SECTION("Set")
        {
            mem.writemem(0x0, 0xFF53);
            ppu.setHdma3(0x66);
            REQUIRE(mem.readmem(0xFF53) == 0x66);
        }
    }

    SECTION("HDMA4")
    {
        SECTION("Get")
        {
            mem.writemem(0x66, 0xFF54);
            REQUIRE(ppu.getHdma4() == 0x66);
        }

        SECTION("Set")
        {
            mem.writemem(0x0, 0xFF54);
            ppu.setHdma4(0x66);
            REQUIRE(mem.readmem(0xFF54) == 0x66);
        }
    }

    SECTION("HDMA5")
    {
        SECTION("Get")
        {
            mem.writemem(0x66, 0xFF55);
            REQUIRE(ppu.getHdma5() == 0x66);
        }

        SECTION("Set")
        {
            mem.writemem(0x0, 0xFF55);
            ppu.setHdma5(0x66);
            REQUIRE(mem.readmem(0xFF55) == 0x66);
        }
    }

    SECTION("HDMA Source Address")
    {
        mem.writemem(0x22, 0xFF51);
        mem.writemem(0x33, 0xFF52);
        REQUIRE(ppu.getHdmaSrcAddress() == 0x2230);
    }

    SECTION("HDMA Destination Address")
    {
        mem.writemem(0x22, 0xFF53);
        mem.writemem(0x33, 0xFF54);
        REQUIRE(ppu.getHdmaDestAddress() == 0x8230);
    }

    SECTION("HDMA Mode")
    {
        SECTION("Get")
        {
            mem.ioRegisters[0xFF55 - MEM_IO_START] = 0x80;
            REQUIRE(ppu.getHdmaMode() == 1);

            mem.ioRegisters[0xFF55 - MEM_IO_START] = 0x0;
            REQUIRE(ppu.getHdmaMode() == 0);
        }
    }

    SECTION("HDMA Length")
    {
        SECTION("Get")
        {
            mem.ioRegisters[0xFF55 - MEM_IO_START] = 0x83;
            REQUIRE(ppu.getHdmaLength() == 3);
        }
    }

    SECTION("Start HDMA")
    {
        SECTION("General DMA")
        {
            ppu.setHdma5(0x3);
            REQUIRE(ppu.vramGeneralDmaActive);
            REQUIRE(ppu.vramDmaLength == 3);
            REQUIRE(mem.readmem(0xFF55, true, true) == 0x3);
        }

        SECTION("HBlank DMA")
        {
            ppu.setHdma5(0x83);
            REQUIRE(ppu.vramHblankDmaActive);
            REQUIRE(ppu.vramDmaLength == 3);
            REQUIRE(mem.readmem(0xFF55, true, true) == 0x3);

            mem.writemem(0, 0xFF55, true, true);
            REQUIRE_FALSE(ppu.vramHblankDmaActive);
            REQUIRE(mem.readmem(0xFF55, true, true) == 0x83);
        }
    }
}

TEST_CASE("Get Tile By Index", "[PPU]")
{
    PPU ppu;
    Memory mem;

    ppu.memory = &mem;
    mem.ppu = &ppu;

    SECTION("BG Tile Data Mode 0")
    {
        ppu.setBgAndWindowTileDataSelect(0);

        SECTION("Block 1")
        {
            // 0x8800-0x8FFF
            uint8_t tileIndex = 130;
            uint16_t tileAddr = 0x8800 + (tileIndex - 128) * 16;

            for (uint8_t i = 0; i < 16; i += 8) {
                // 00 00 ff 00 00 ff ff ff - each line is "darker" than the previous
                mem.writemem(0x0, tileAddr + i, true, true);
                mem.writemem(0x0, tileAddr + i + 1, true, true);
                mem.writemem(0xFF, tileAddr + i + 2, true, true);
                mem.writemem(0x0, tileAddr + i + 3, true, true);
                mem.writemem(0x0, tileAddr + i + 4, true, true);
                mem.writemem(0xFF, tileAddr + i + 5, true, true);
                mem.writemem(0xFF, tileAddr + i + 6, true, true);
                mem.writemem(0xFF, tileAddr + i + 7, true, true);
            }

            Tile tile = ppu.getTileByIndex(tileIndex);
            uint8_t tilePixels[8];

            for (uint8_t i = 0; i < 2; ++i) {
                for (uint8_t j = 0; j < 4; ++j) {
                    tile.getTileRow(i * 4 + j, tilePixels);
                    for (uint8_t k = 0; k < 8; ++k)
                        REQUIRE(tilePixels[k] == j);
                }
            }
        }

        SECTION("Block 2")
        {
            // 0x9000-0x97FF
            uint8_t tileIndex = 66;
            uint16_t tileAddr = 0x9000 + tileIndex * 16;

            for (uint8_t i = 0; i < 16; i += 8) {
                // 00 00 ff 00 00 ff ff ff - each line is "darker" than the previous
                mem.writemem(0x0, tileAddr + i, true, true);
                mem.writemem(0x0, tileAddr + i + 1, true, true);
                mem.writemem(0xFF, tileAddr + i + 2, true, true);
                mem.writemem(0x0, tileAddr + i + 3, true, true);
                mem.writemem(0x0, tileAddr + i + 4, true, true);
                mem.writemem(0xFF, tileAddr + i + 5, true, true);
                mem.writemem(0xFF, tileAddr + i + 6, true, true);
                mem.writemem(0xFF, tileAddr + i + 7, true, true);
            }

            Tile tile = ppu.getTileByIndex(tileIndex);
            uint8_t tilePixels[8];

            for (uint8_t i = 0; i < 2; ++i) {
                for (uint8_t j = 0; j < 4; ++j) {
                    tile.getTileRow(i * 4 + j, tilePixels);
                    for (uint8_t k = 0; k < 8; ++k)
                        REQUIRE(tilePixels[k] == j);
                }
            }
        }
    }

    SECTION("BG Tile Data Mode 1")
    {
        ppu.setBgAndWindowTileDataSelect(1);

        SECTION("Block 0")
        {
            // 0x8000-0x87FF
            uint8_t tileIndex = 66;
            uint16_t tileAddr = 0x8000 + tileIndex * 16;

            for (uint8_t i = 0; i < 16; i += 8) {
                // 00 00 ff 00 00 ff ff ff - each line is "darker" than the previous
                mem.writemem(0x0, tileAddr + i, true, true);
                mem.writemem(0x0, tileAddr + i + 1, true, true);
                mem.writemem(0xFF, tileAddr + i + 2, true, true);
                mem.writemem(0x0, tileAddr + i + 3, true, true);
                mem.writemem(0x0, tileAddr + i + 4, true, true);
                mem.writemem(0xFF, tileAddr + i + 5, true, true);
                mem.writemem(0xFF, tileAddr + i + 6, true, true);
                mem.writemem(0xFF, tileAddr + i + 7, true, true);
            }

            Tile tile = ppu.getTileByIndex(tileIndex);
            uint8_t tilePixels[8];

            for (uint8_t i = 0; i < 2; ++i) {
                for (uint8_t j = 0; j < 4; ++j) {
                    tile.getTileRow(i * 4 + j, tilePixels);
                    for (uint8_t k = 0; k < 8; ++k)
                        REQUIRE(tilePixels[k] == j);
                }
            }
        }

        SECTION("Block 1")
        {
            // 0x8800-0x8FFF
            uint8_t tileIndex = 130;
            uint16_t tileAddr = 0x8800 + (tileIndex - 128) * 16;

            for (uint8_t i = 0; i < 16; i += 8) {
                // 00 00 ff 00 00 ff ff ff - each line is "darker" than the previous
                mem.writemem(0x0, tileAddr + i, true, true);
                mem.writemem(0x0, tileAddr + i + 1, true, true);
                mem.writemem(0xFF, tileAddr + i + 2, true, true);
                mem.writemem(0x0, tileAddr + i + 3, true, true);
                mem.writemem(0x0, tileAddr + i + 4, true, true);
                mem.writemem(0xFF, tileAddr + i + 5, true, true);
                mem.writemem(0xFF, tileAddr + i + 6, true, true);
                mem.writemem(0xFF, tileAddr + i + 7, true, true);
            }

            Tile tile = ppu.getTileByIndex(tileIndex);
            uint8_t tilePixels[8];

            for (uint8_t i = 0; i < 2; ++i) {
                for (uint8_t j = 0; j < 4; ++j) {
                    tile.getTileRow(i * 4 + j, tilePixels);
                    for (uint8_t k = 0; k < 8; ++k)
                        REQUIRE(tilePixels[k] == j);
                }
            }
        }
    }
}

TEST_CASE("Get Sprite By Index", "[PPU]")
{
    PPU ppu;
    Memory mem;

    ppu.memory = &mem;
    mem.ppu = &ppu;

    uint8_t spriteIndex = 10;
    uint16_t spriteAddr = MEM_OAM_START + spriteIndex * 4;

    mem.writemem(0x11, spriteAddr, true, true);
    mem.writemem(0x22, spriteAddr + 1, true, true);
    mem.writemem(0x33, spriteAddr + 2, true, true);
    mem.writemem(0xAA, spriteAddr + 3, true, true);

    OAMSprite sprite = ppu.getSpriteByIndex(spriteIndex);

    REQUIRE(sprite.yPos == 0x11);
    REQUIRE(sprite.xPos == 0x22);
    REQUIRE(sprite.tileNumber == 0x33);
    REQUIRE(sprite.objToBgPriority == true);
    REQUIRE(sprite.yFlip == false);
    REQUIRE(sprite.xFlip == true);
    REQUIRE(sprite.dmgPaletteNumber == 0);
    REQUIRE(sprite.tileVramBank == 1);
    REQUIRE(sprite.cgbPaletteNumber == 2);
}

TEST_CASE("Get BG Map By Index", "[PPU]")
{
    PPU ppu;
    Memory mem;

    ppu.memory = &mem;
    mem.ppu = &ppu;

    ppu.emulatorMode = EmulatorMode::CGB;
    mem.mode = EmulatorMode::CGB;

    SECTION("Tilemap 0")
    {
        // 0x9800-0x9BFF
        uint8_t mapTileIndex = 3;
        uint16_t mapTileAddr = 0x9800 + mapTileIndex;

        ppu.setBgTileMapDisplaySelect(0);

        mem.setCurrentVramBank(1);
        mem.writemem(0xAA, mapTileAddr, true, true);
        mem.setCurrentVramBank(0);

        BgMapAttributes mapAttr = ppu.getBgMapByIndex(mapTileIndex, 0);

        REQUIRE(mapAttr.bgToOamPriority == 1);
        REQUIRE(mapAttr.verticalFlip == 0);
        REQUIRE(mapAttr.horizontalFlip == 1);
        REQUIRE(mapAttr.tileVramBankNumber == 1);
        REQUIRE(mapAttr.bgPaletteNumber == 2);
        REQUIRE(mem.getCurrentVramBank() == 0);
    }

    SECTION("Tilemap 1")
    {
        // 0x9C00-0x9FFF
        uint8_t mapTileIndex = 3;
        uint16_t mapTileAddr = 0x9C00 + mapTileIndex;

        ppu.setBgTileMapDisplaySelect(1);

        mem.setCurrentVramBank(1);
        mem.writemem(0xAA, mapTileAddr, true, true);

        BgMapAttributes mapAttr = ppu.getBgMapByIndex(mapTileIndex, 1);

        REQUIRE(mapAttr.bgToOamPriority == 1);
        REQUIRE(mapAttr.verticalFlip == 0);
        REQUIRE(mapAttr.horizontalFlip == 1);
        REQUIRE(mapAttr.tileVramBankNumber == 1);
        REQUIRE(mapAttr.bgPaletteNumber == 2);
        REQUIRE(mem.getCurrentVramBank() == 1);
    }
}

TEST_CASE("Get Sprite Tile", "[PPU]")
{
    PPU ppu;
    Memory mem;

    ppu.memory = &mem;
    mem.ppu = &ppu;

    ppu.emulatorMode = EmulatorMode::CGB;
    mem.mode = EmulatorMode::CGB;

    SECTION("8x8")
    {
        ppu.setObjSize(0);

        SECTION("VRAM Bank 0")
        {
            mem.setCurrentVramBank(0);

            uint8_t tileIndex = 0x66;
            uint16_t tileAddr = 0x8000 + tileIndex * 16;

            for (uint8_t i = 0; i < 16; i += 8) {
                // 00 00 ff 00 00 ff ff ff - each line is "darker" than the previous
                mem.writemem(0x0, tileAddr + i, true, true);
                mem.writemem(0x0, tileAddr + i + 1, true, true);
                mem.writemem(0xFF, tileAddr + i + 2, true, true);
                mem.writemem(0x0, tileAddr + i + 3, true, true);
                mem.writemem(0x0, tileAddr + i + 4, true, true);
                mem.writemem(0xFF, tileAddr + i + 5, true, true);
                mem.writemem(0xFF, tileAddr + i + 6, true, true);
                mem.writemem(0xFF, tileAddr + i + 7, true, true);
            }

            mem.setCurrentVramBank(1);

            Tile tile = ppu.getSpriteTile(tileIndex, 0, 0);
            uint8_t tilePixels[8];

            for (uint8_t i = 0; i < 2; ++i) {
                for (uint8_t j = 0; j < 4; ++j) {
                    tile.getTileRow(i * 4 + j, tilePixels);
                    for (uint8_t k = 0; k < 8; ++k)
                        REQUIRE(tilePixels[k] == j);
                }
            }

            REQUIRE(mem.getCurrentVramBank() == 1);
        }

        SECTION("VRAM Bank 1")
        {
            mem.setCurrentVramBank(1);

            uint8_t tileIndex = 0x66;
            uint16_t tileAddr = 0x8000 + tileIndex * 16;

            for (uint8_t i = 0; i < 16; i += 8) {
                // 00 00 ff 00 00 ff ff ff - each line is "darker" than the previous
                mem.writemem(0x0, tileAddr + i, true, true);
                mem.writemem(0x0, tileAddr + i + 1, true, true);
                mem.writemem(0xFF, tileAddr + i + 2, true, true);
                mem.writemem(0x0, tileAddr + i + 3, true, true);
                mem.writemem(0x0, tileAddr + i + 4, true, true);
                mem.writemem(0xFF, tileAddr + i + 5, true, true);
                mem.writemem(0xFF, tileAddr + i + 6, true, true);
                mem.writemem(0xFF, tileAddr + i + 7, true, true);
            }

            Tile tile = ppu.getSpriteTile(tileIndex, 0, 1);
            uint8_t tilePixels[8];

            for (uint8_t i = 0; i < 2; ++i) {
                for (uint8_t j = 0; j < 4; ++j) {
                    tile.getTileRow(i * 4 + j, tilePixels);
                    for (uint8_t k = 0; k < 8; ++k)
                        REQUIRE(tilePixels[k] == j);
                }
            }

            REQUIRE(mem.getCurrentVramBank() == 1);
        }
    }

    SECTION("8x16")
    {
        ppu.setObjSize(1);

        SECTION("Top Tile")
        {
            uint8_t topTileIndex = 0x10;
            uint16_t topTileAddr = 0x8000 + topTileIndex * 16 * 2;

            for (uint8_t i = 0; i < 16; i += 8) {
                // 00 00 ff 00 00 ff ff ff - each line is "darker" than the previous
                mem.writemem(0x0, topTileAddr + i, true, true);
                mem.writemem(0x0, topTileAddr + i + 1, true, true);
                mem.writemem(0xFF, topTileAddr + i + 2, true, true);
                mem.writemem(0x0, topTileAddr + i + 3, true, true);
                mem.writemem(0x0, topTileAddr + i + 4, true, true);
                mem.writemem(0xFF, topTileAddr + i + 5, true, true);
                mem.writemem(0xFF, topTileAddr + i + 6, true, true);
                mem.writemem(0xFF, topTileAddr + i + 7, true, true);
            }

            Tile tile = ppu.getSpriteTile(topTileIndex, 0, 0);
            uint8_t tilePixels[8];

            for (uint8_t i = 0; i < 2; ++i) {
                for (uint8_t j = 0; j < 4; ++j) {
                    tile.getTileRow(i * 4 + j, tilePixels);
                    for (uint8_t k = 0; k < 8; ++k)
                        REQUIRE(tilePixels[k] == j);
                }
            }
        }

        SECTION("Bottom Tile")
        {
            uint8_t bottomTileIndex = 0x10;
            uint16_t bottomTileAddr = 0x8000 + bottomTileIndex * 16 * 2 + 16;

            for (uint8_t i = 0; i < 16; i += 8) {
                // 00 00 ff 00 00 ff ff ff - each line is "darker" than the previous
                mem.writemem(0x0, bottomTileAddr + i, true, true);
                mem.writemem(0x0, bottomTileAddr + i + 1, true, true);
                mem.writemem(0xFF, bottomTileAddr + i + 2, true, true);
                mem.writemem(0x0, bottomTileAddr + i + 3, true, true);
                mem.writemem(0x0, bottomTileAddr + i + 4, true, true);
                mem.writemem(0xFF, bottomTileAddr + i + 5, true, true);
                mem.writemem(0xFF, bottomTileAddr + i + 6, true, true);
                mem.writemem(0xFF, bottomTileAddr + i + 7, true, true);
            }

            Tile tile = ppu.getSpriteTile(bottomTileIndex, 1, 0);
            uint8_t tilePixels[8];

            for (uint8_t i = 0; i < 2; ++i) {
                for (uint8_t j = 0; j < 4; ++j) {
                    tile.getTileRow(i * 4 + j, tilePixels);
                    for (uint8_t k = 0; k < 8; ++k)
                        REQUIRE(tilePixels[k] == j);
                }
            }
        }
    }
}

TEST_CASE("Search Sprites on Line", "[PPU]")
{
    PPU ppu;
    Memory mem;

    ppu.memory = &mem;
    mem.ppu = &ppu;

    SECTION("8x8")
    {
        ppu.setObjSize(0);
        ppu.setLy(100);

        OAMSprite sprite;

        // Sprite above LY
        sprite.xPos = 10;
        sprite.yPos = 40;
        sprite.writeAsBytes(mem.oam + 0 * 4);

        // Sprite below LY
        sprite.yPos = 140;
        sprite.writeAsBytes(mem.oam + 1 * 4);

        // Sprites on LY
        sprite.xPos = 100;
        sprite.yPos = 109;
        sprite.writeAsBytes(mem.oam + 2 * 4);

        sprite.xPos = 120;
        sprite.yPos = 109;
        sprite.writeAsBytes(mem.oam + 3 * 4);

        sprite.xPos = 130;
        sprite.yPos = 110;
        sprite.writeAsBytes(mem.oam + 4 * 4);

        sprite.xPos = 30;
        sprite.yPos = 111;
        sprite.writeAsBytes(mem.oam + 5 * 4);

        sprite.xPos = 40;
        sprite.yPos = 112;
        sprite.writeAsBytes(mem.oam + 6 * 4);

        sprite.xPos = 50;
        sprite.yPos = 113;
        sprite.writeAsBytes(mem.oam + 7 * 4);

        sprite.xPos = 60;
        sprite.yPos = 114;
        sprite.writeAsBytes(mem.oam + 8 * 4);

        sprite.xPos = 70;
        sprite.yPos = 115;
        sprite.writeAsBytes(mem.oam + 9 * 4);

        sprite.xPos = 80;
        sprite.yPos = 116;
        sprite.writeAsBytes(mem.oam + 10 * 4);

        sprite.xPos = 90;
        sprite.yPos = 116;
        sprite.writeAsBytes(mem.oam + 11 * 4);

        // Extra sprite on LY
        sprite.xPos = 80;
        sprite.yPos = 110;
        sprite.writeAsBytes(mem.oam + 12 * 4);

        uint8_t goodSpriteIndexes[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11};

        ppu.searchSpritesOnLine();

        REQUIRE(ppu.numSpritesOnCurrentLine == 10);

        for (uint8_t i = 0; i < ppu.numSpritesOnCurrentLine; ++i)
            REQUIRE(ppu.spritesOnCurrentLine[i] == goodSpriteIndexes[i]);
    }

    SECTION("8x16")
    {
        ppu.setObjSize(1);
        ppu.setLy(100);

        OAMSprite sprite;

        // Sprite above LY
        sprite.xPos = 10;
        sprite.yPos = 99;
        sprite.writeAsBytes(mem.oam + 0 * 4);

        // Sprite below LY
        sprite.xPos = 20;
        sprite.yPos = 117;
        sprite.writeAsBytes(mem.oam + 1 * 4);

        // Sprites on LY
        sprite.xPos = 30;
        sprite.yPos = 101;
        sprite.writeAsBytes(mem.oam + 2 * 4);

        sprite.xPos = 40;
        sprite.yPos = 107;
        sprite.writeAsBytes(mem.oam + 3 * 4);

        sprite.xPos = 50;
        sprite.yPos = 109;
        sprite.writeAsBytes(mem.oam + 4 * 4);

        sprite.xPos = 100;
        sprite.yPos = 116;
        sprite.writeAsBytes(mem.oam + 5 * 4);

        sprite.xPos = 100;
        sprite.yPos = 108;
        sprite.writeAsBytes(mem.oam + 6 * 4);

        uint8_t goodSpriteIndexes[] = {2, 3, 4, 5, 6};

        ppu.searchSpritesOnLine();

        REQUIRE(ppu.numSpritesOnCurrentLine == 5);

        for (uint8_t i = 0; i < ppu.numSpritesOnCurrentLine; ++i)
            REQUIRE(ppu.spritesOnCurrentLine[i] == goodSpriteIndexes[i]);
    }
}

TEST_CASE("Get Color from FifoPixel", "[PPU]")
{
    PPU ppu;
    Memory mem;

    ppu.memory = &mem;
    mem.ppu = &ppu;

    SECTION("DMG Mode")
    {
        ppu.emulatorMode = EmulatorMode::DMG;
        mem.mode = EmulatorMode::DMG;

        SECTION("BG or Window Pixel")
        {
            ppu.setBgPaletteData(0x1B); // 3: 00; 2: 01; 1: 10; 0: 11

            FifoPixel pixel = FifoPixel(2, 0, 0, 0, false);
            Color color = ppu.getColorFromFifoPixel(&pixel);

            REQUIRE(color.red == 170);
            REQUIRE(color.green == 170);
            REQUIRE(color.blue == 170);
        }

        SECTION("Sprite Pixel")
        {
            ppu.setObjPalette0Data(0x1B); // 3: 00; 2: 01; 1: 10; 0: 11

            FifoPixel pixel = FifoPixel(2, 0, 0, 0, true);
            Color color = ppu.getColorFromFifoPixel(&pixel);

            REQUIRE(color.red == 170);
            REQUIRE(color.green == 170);
            REQUIRE(color.blue == 170);

            pixel.color = 0;
            color = ppu.getColorFromFifoPixel(&pixel);

            REQUIRE(color.red == 255);
            REQUIRE(color.green == 255);
            REQUIRE(color.blue == 255);
        }
    }

    SECTION("GCB Mode")
    {
        ppu.emulatorMode = EmulatorMode::CGB;
        mem.mode = EmulatorMode::CGB;

        SECTION("BG or Window Pixel")
        {
            // Color 1 is R:0x10, G:0x12, B:0x14
            mem.cgbBgColorPalette[2] = 0x84;
            mem.cgbBgColorPalette[3] = 0xA8;
            
            FifoPixel pixel = FifoPixel(1, 0, 0, 0, false);
            Color color = ppu.getColorFromFifoPixel(&pixel, false);

            REQUIRE(color.red == 0x10);
            REQUIRE(color.green == 0x12);
            REQUIRE(color.blue == 0x14);

            color = ppu.getColorFromFifoPixel(&pixel, true);

            REQUIRE(color.red == 131);
            REQUIRE(color.green == 148);
            REQUIRE(color.blue == 164);

        }

        SECTION("Sprite Pixel")
        {
            // Color 1 is R:0x10, G:0x12, B:0x14
            mem.cgbObjColorPalette[2] = 0x84;
            mem.cgbObjColorPalette[3] = 0xA8;

            FifoPixel pixel = FifoPixel(1, 0, 0, 0, true);
            Color color = ppu.getColorFromFifoPixel(&pixel, false);

            REQUIRE(color.red == 0x10);
            REQUIRE(color.green == 0x12);
            REQUIRE(color.blue == 0x14);

            color = ppu.getColorFromFifoPixel(&pixel, true);

            REQUIRE(color.red == 131);
            REQUIRE(color.green == 148);
            REQUIRE(color.blue == 164);

            pixel.color = 0;
            color = ppu.getColorFromFifoPixel(&pixel);

            REQUIRE(color.red == 255);
            REQUIRE(color.green == 255);
            REQUIRE(color.blue == 255);
        }
    }
}

TEST_CASE("PPU Cycle", "[PPU]")
{
    // TODO ??
}
