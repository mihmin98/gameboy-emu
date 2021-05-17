#include "catch.hpp"

#include "Memory.hpp"
#include "PPU.hpp"
#include "SM83.hpp"
#include <cstdlib>
#include <iostream>

TEST_CASE("BG Fifo", "[PPU_FIFO]")
{
    PPU ppu;
    Memory mem;

    ppu.memory = &mem;
    mem.ppu = &ppu;

    SECTION("DMG")
    {
        ppu.emulatorMode = EmulatorMode::DMG;
        mem.mode = EmulatorMode::DMG;

        SECTION("BG with Scroll")
        {
            ppu.setBgTileMapDisplaySelect(0);
            ppu.setBgAndWindowTileDataSelect(1);
            ppu.setBgWindowDisplayPriority(1);
            ppu.setScrollX(244);
            ppu.setScrollY(2);
            ppu.setLy(0);
            ppu.setWindowDisplayEnable(0);
            ppu.xPos = 0;

            uint16_t tile1Addr = 0x8000;
            for (uint8_t i = 0; i < 16; i += 8) {
                // 00 00 ff 00 00 ff ff ff - each line is "darker" than the previous
                mem.writemem(0x0, tile1Addr + i, true, true);
                mem.writemem(0x0, tile1Addr + i + 1, true, true);
                mem.writemem(0xFF, tile1Addr + i + 2, true, true);
                mem.writemem(0x0, tile1Addr + i + 3, true, true);
                mem.writemem(0x0, tile1Addr + i + 4, true, true);
                mem.writemem(0xFF, tile1Addr + i + 5, true, true);
                mem.writemem(0xFF, tile1Addr + i + 6, true, true);
                mem.writemem(0xFF, tile1Addr + i + 7, true, true);
            }

            uint16_t tile2Addr = 0x8010;
            for (uint8_t i = 0; i < 16; i += 8) {
                // ff ff 00 ff ff 00 00 00 - each line is "lighter" than the previous
                mem.writemem(0xFF, tile2Addr + i, true, true);
                mem.writemem(0xFF, tile2Addr + i + 1, true, true);
                mem.writemem(0x0, tile2Addr + i + 2, true, true);
                mem.writemem(0xFF, tile2Addr + i + 3, true, true);
                mem.writemem(0xFF, tile2Addr + i + 4, true, true);
                mem.writemem(0x0, tile2Addr + i + 5, true, true);
                mem.writemem(0x00, tile2Addr + i + 6, true, true);
                mem.writemem(0x00, tile2Addr + i + 7, true, true);
            }

            mem.writemem(0, 0x981E, true, true);
            mem.writemem(1, 0x981F, true, true);
            mem.writemem(0, 0x9800, true, true);
            mem.writemem(1, 0x9801, true, true);
            mem.writemem(0, 0x9802, true, true);
            mem.writemem(1, 0x9803, true, true);
            mem.writemem(0, 0x9804, true, true);

            FifoPixel *returnedPixel;

            // GET_TILE
            ppu.bgFifo.prepareForLine(ppu.getLy());
            REQUIRE(ppu.bgFifo.fetcherStage == GET_TILE);
            REQUIRE(ppu.bgFifo.fetcherStageCycles == 1);
            REQUIRE(ppu.bgFifo.pixelQueue.empty());
            REQUIRE(ppu.bgFifo.fetcherXPos == 0);

            ppu.bgFifo.cycle();
            ppu.bgFifo.cycle();

            REQUIRE(ppu.bgFifo.scxPixelsToDiscard == 4);

            // GET_TILE_DATA_LOW
            REQUIRE(ppu.bgFifo.fetcherStage == GET_TILE_DATA_LOW);
            REQUIRE(ppu.bgFifo.fetcherStageCycles == 1);

            ppu.bgFifo.cycle();
            ppu.bgFifo.cycle();

            // GET_TILE_DATA_HIGH
            REQUIRE(ppu.bgFifo.fetcherStage == GET_TILE_DATA_HIGH);
            REQUIRE(ppu.bgFifo.fetcherStageCycles == 1);

            ppu.bgFifo.cycle();
            returnedPixel = ppu.bgFifo.cycle();

            // PUSH
            // should discard 4 pixels
            REQUIRE(ppu.bgFifo.fetcherStage == GET_TILE);
            REQUIRE(ppu.bgFifo.fetcherStageCycles == 1);
            REQUIRE(ppu.bgFifo.pixelQueue.size() == 7);
            REQUIRE(returnedPixel == nullptr);
            REQUIRE(ppu.bgFifo.scxPixelsToDiscard == 3);
            REQUIRE(ppu.bgFifo.fetcherXPos == 4);

            while (ppu.bgFifo.scxPixelsToDiscard != 0) {
                uint scxToDiscard = ppu.bgFifo.scxPixelsToDiscard;
                returnedPixel = ppu.bgFifo.cycle();
                REQUIRE(ppu.bgFifo.scxPixelsToDiscard == scxToDiscard - 1);
                REQUIRE(returnedPixel == nullptr);
            }

            // There should be 4 pixels in the queue
            REQUIRE(ppu.bgFifo.pixelQueue.size() == 4);

            // pixel is from tile1
            for (uint i = 0; i < 4; ++i) {
                returnedPixel = ppu.bgFifo.cycle();
                REQUIRE(returnedPixel->color == 2);
            }

            // queue should be filled by now with 8 pixels
            REQUIRE(ppu.bgFifo.pixelQueue.size() == 8);
            REQUIRE(ppu.bgFifo.fetcherXPos == 12);

            // pixels from tile0
            for (uint i = 0; i < 8; ++i) {
                returnedPixel = ppu.bgFifo.cycle();
                REQUIRE(returnedPixel->color == 1);
            }

            // pixels form tile1
            for (uint i = 0; i < 8; ++i) {
                returnedPixel = ppu.bgFifo.cycle();
                REQUIRE(returnedPixel->color == 2);
            }

            // pixels from tile0
            for (uint i = 0; i < 6; ++i) {
                returnedPixel = ppu.bgFifo.cycle();
                REQUIRE(returnedPixel->color == 1);
            }

            // Fetcher should now sleep
            for (uint i = 0; i < 2; ++i) {
                returnedPixel = ppu.bgFifo.cycle();
                REQUIRE(returnedPixel->color == 1);
                REQUIRE(ppu.bgFifo.fetcherStage == SLEEP);
            }
        }

        SECTION("BG and Window")
        {
            ppu.setBgTileMapDisplaySelect(0);
            ppu.setWindowTileMapDisplaySelect(1);
            ppu.setBgAndWindowTileDataSelect(1);
            ppu.setBgWindowDisplayPriority(1);
            ppu.setScrollX(252);
            ppu.setScrollY(2);
            ppu.setLy(0);
            ppu.setWindowDisplayEnable(1);
            ppu.setWx(15);
            ppu.setWy(0);
            ppu.xPos = 0;

            uint16_t tile1Addr = 0x8000;
            for (uint8_t i = 0; i < 16; i += 8) {
                // 00 00 ff 00 00 ff ff ff - each line is "darker" than the previous
                mem.writemem(0x0, tile1Addr + i, true, true);
                mem.writemem(0x0, tile1Addr + i + 1, true, true);
                mem.writemem(0xFF, tile1Addr + i + 2, true, true);
                mem.writemem(0x0, tile1Addr + i + 3, true, true);
                mem.writemem(0x0, tile1Addr + i + 4, true, true);
                mem.writemem(0xFF, tile1Addr + i + 5, true, true);
                mem.writemem(0xFF, tile1Addr + i + 6, true, true);
                mem.writemem(0xFF, tile1Addr + i + 7, true, true);
            }

            uint16_t tile2Addr = 0x8010;
            for (uint8_t i = 0; i < 16; i += 8) {
                // ff ff 00 ff ff 00 00 00 - each line is "lighter" than the previous
                mem.writemem(0xFF, tile2Addr + i, true, true);
                mem.writemem(0xFF, tile2Addr + i + 1, true, true);
                mem.writemem(0x0, tile2Addr + i + 2, true, true);
                mem.writemem(0xFF, tile2Addr + i + 3, true, true);
                mem.writemem(0xFF, tile2Addr + i + 4, true, true);
                mem.writemem(0x0, tile2Addr + i + 5, true, true);
                mem.writemem(0x00, tile2Addr + i + 6, true, true);
                mem.writemem(0x00, tile2Addr + i + 7, true, true);
            }

            // bg tile map
            mem.writemem(0, 0x981F, true, true);
            mem.writemem(1, 0x9800, true, true);
            mem.writemem(0, 0x9801, true, true);
            mem.writemem(1, 0x9802, true, true);

            // window tile map
            mem.writemem(0, 0x9C00, true, true);
            mem.writemem(1, 0x9C01, true, true);
            mem.writemem(0, 0x9C02, true, true);
            mem.writemem(1, 0x9C03, true, true);

            FifoPixel *returnedPixel;
            BgFifo &bgFifo = ppu.bgFifo;

            bgFifo.prepareForLine(ppu.getLy());

            // bg: 4 pixels from tile 0, 4 pixels from tile 1
            // window: start with tile 0

            // 6 cycles to fetch tile, 6th cycle also pushes pixel
            // 5 + 4 + 4

            for (uint i = 0; i < 5; ++i) {
                returnedPixel = bgFifo.cycle();
                REQUIRE(returnedPixel == nullptr);
            }

            for (uint i = 0; i < 4; ++i) {
                returnedPixel = bgFifo.cycle();
                REQUIRE(returnedPixel == nullptr);
            }

            for (uint i = 0; i < 4; ++i) {
                returnedPixel = bgFifo.cycle();
                REQUIRE(returnedPixel->color == 2);
            }

            for (uint i = 0; i < 4; ++i) {
                returnedPixel = bgFifo.cycle();
                REQUIRE(returnedPixel->color == 1);
            }

            // Start rendering window
            returnedPixel = bgFifo.cycle();
            REQUIRE(returnedPixel == nullptr);
            REQUIRE(bgFifo.isDrawingWindow);
            REQUIRE(bgFifo.pixelQueue.size() == 0);

            for (uint i = 0; i < 5; ++i) {
                returnedPixel = bgFifo.cycle();
                REQUIRE(returnedPixel == nullptr);
            }

            for (uint i = 0; i < 8; ++i) {
                returnedPixel = bgFifo.cycle();
                REQUIRE(returnedPixel->color == 2);
            }

            for (uint i = 0; i < 8; ++i) {
                returnedPixel = bgFifo.cycle();
                REQUIRE(returnedPixel->color == 1);
            }
        }
    }

    SECTION("CGB")
    {
        ppu.emulatorMode = EmulatorMode::CGB;
        mem.mode = EmulatorMode::CGB;

        SECTION("BG with Scroll")
        {
            ppu.setBgTileMapDisplaySelect(0);
            ppu.setBgAndWindowTileDataSelect(1);
            ppu.setBgWindowDisplayPriority(1);
            ppu.setScrollX(244);
            ppu.setScrollY(2);
            ppu.setLy(0);
            ppu.setWindowDisplayEnable(0);
            ppu.xPos = 0;

            mem.setCurrentVramBank(0);

            uint16_t tile1Addr = 0x8000;
            for (uint8_t i = 0; i < 16; i += 8) {
                // 00 00 ff 00 00 ff ff ff - each line is "darker" than the previous
                mem.writemem(0x0, tile1Addr + i, true, true);
                mem.writemem(0x0, tile1Addr + i + 1, true, true);
                mem.writemem(0xFF, tile1Addr + i + 2, true, true);
                mem.writemem(0x0, tile1Addr + i + 3, true, true);
                mem.writemem(0x0, tile1Addr + i + 4, true, true);
                mem.writemem(0xFF, tile1Addr + i + 5, true, true);
                mem.writemem(0xFF, tile1Addr + i + 6, true, true);
                mem.writemem(0xFF, tile1Addr + i + 7, true, true);
            }

            uint16_t tile2Addr = 0x8010;
            for (uint8_t i = 0; i < 16; i += 8) {
                // ff ff 00 ff ff 00 00 00 - each line is "lighter" than the previous
                mem.writemem(0xFF, tile2Addr + i, true, true);
                mem.writemem(0xFF, tile2Addr + i + 1, true, true);
                mem.writemem(0x0, tile2Addr + i + 2, true, true);
                mem.writemem(0xFF, tile2Addr + i + 3, true, true);
                mem.writemem(0xFF, tile2Addr + i + 4, true, true);
                mem.writemem(0x0, tile2Addr + i + 5, true, true);
                mem.writemem(0x00, tile2Addr + i + 6, true, true);
                mem.writemem(0x00, tile2Addr + i + 7, true, true);
            }

            mem.setCurrentVramBank(1);

            // Write the tiles in vram bank 1

            // ff ff ff ff 00 ff 00 ff ff 00 ff 00 00 00 00 00
            // dark to light from top to bottom
            // good for vertical flip
            mem.writemem(0xFF, tile1Addr, true, true);
            mem.writemem(0xFF, tile1Addr + 1, true, true);
            mem.writemem(0xFF, tile1Addr + 2, true, true);
            mem.writemem(0xFF, tile1Addr + 3, true, true);
            mem.writemem(0x0, tile1Addr + 4, true, true);
            mem.writemem(0xFF, tile1Addr + 5, true, true);
            mem.writemem(0x0, tile1Addr + 6, true, true);
            mem.writemem(0xFF, tile1Addr + 7, true, true);
            mem.writemem(0xFF, tile1Addr + 8, true, true);
            mem.writemem(0x0, tile1Addr + 9, true, true);
            mem.writemem(0xFF, tile1Addr + 10, true, true);
            mem.writemem(0x0, tile1Addr + 11, true, true);
            mem.writemem(0x0, tile1Addr + 12, true, true);
            mem.writemem(0x0, tile1Addr + 13, true, true);
            mem.writemem(0x0, tile1Addr + 14, true, true);
            mem.writemem(0x0, tile1Addr + 15, true, true);

            for (uint8_t i = 0; i < 16; i += 2) {
                // 33 0f 33 0f 33 0f 33 0f 33 0f 33 0f 33 0f 33 0f
                // light to dark from left to right
                // good for horizontal flip
                mem.writemem(0x33, tile2Addr + i, true, true);
                mem.writemem(0x0F, tile2Addr + i + 1, true, true);
            }

            mem.setCurrentVramBank(0);

            mem.writemem(0, 0x981E, true, true);
            mem.writemem(1, 0x981F, true, true);
            mem.writemem(0, 0x9800, true, true);
            mem.writemem(1, 0x9801, true, true);
            mem.writemem(0, 0x9802, true, true);
            mem.writemem(1, 0x9803, true, true);
            mem.writemem(0, 0x9804, true, true);

            // first 3 tiles are from vram bank 0, the rest are from vram bank 1
            // the ones from vram 1 bank are good for flipping

            BgMapAttributes bgMapAttr;
            bgMapAttr.bgPaletteNumber = 2;
            bgMapAttr.bgToOamPriority = 1;
            bgMapAttr.horizontalFlip = false;
            bgMapAttr.verticalFlip = false;

            mem.setCurrentVramBank(1);

            // 1st 3 tiles
            bgMapAttr.tileVramBankNumber = 0;

            mem.writemem(bgMapAttr.getAsByte(), 0x981E, true, true);
            mem.writemem(bgMapAttr.getAsByte(), 0x981F, true, true);
            mem.writemem(bgMapAttr.getAsByte(), 0x9800, true, true);

            // rest of the tiles
            // tiles 0x9801 and 0x9802 from vram 1 are not flipped
            bgMapAttr.tileVramBankNumber = 1;

            mem.writemem(bgMapAttr.getAsByte(), 0x9801, true, true);
            mem.writemem(bgMapAttr.getAsByte(), 0x9802, true, true);

            // 0x9803 is flipped hor
            bgMapAttr.horizontalFlip = true;
            mem.writemem(bgMapAttr.getAsByte(), 0x9803, true, true);

            // 0x9804 is flipped vert
            bgMapAttr.horizontalFlip = false;
            bgMapAttr.verticalFlip = true;

            mem.writemem(bgMapAttr.getAsByte(), 0x9804, true, true);

            FifoPixel *returnedPixel;
            BgFifo &bgFifo = ppu.bgFifo;

            bgFifo.prepareForLine(ppu.getLy());

            // GET_TILE
            REQUIRE(ppu.bgFifo.fetcherStage == GET_TILE);
            REQUIRE(ppu.bgFifo.fetcherStageCycles == 1);
            REQUIRE(ppu.bgFifo.pixelQueue.empty());
            REQUIRE(ppu.bgFifo.fetcherXPos == 0);

            ppu.bgFifo.cycle();
            ppu.bgFifo.cycle();

            REQUIRE(ppu.bgFifo.scxPixelsToDiscard == 4);

            // GET_TILE_DATA_LOW
            REQUIRE(ppu.bgFifo.fetcherStage == GET_TILE_DATA_LOW);
            REQUIRE(ppu.bgFifo.fetcherStageCycles == 1);

            ppu.bgFifo.cycle();
            ppu.bgFifo.cycle();

            // GET_TILE_DATA_HIGH
            REQUIRE(ppu.bgFifo.fetcherStage == GET_TILE_DATA_HIGH);
            REQUIRE(ppu.bgFifo.fetcherStageCycles == 1);

            ppu.bgFifo.cycle();
            returnedPixel = ppu.bgFifo.cycle();

            while (ppu.bgFifo.scxPixelsToDiscard != 0) {
                uint scxToDiscard = ppu.bgFifo.scxPixelsToDiscard;
                returnedPixel = ppu.bgFifo.cycle();
                REQUIRE(ppu.bgFifo.scxPixelsToDiscard == scxToDiscard - 1);
                REQUIRE(returnedPixel == nullptr);
            }

            // There should be 4 pixels in the queue
            REQUIRE(ppu.bgFifo.pixelQueue.size() == 4);

            // pixel is from tile0 from vram bank 0
            for (uint i = 0; i < 4; ++i) {
                returnedPixel = ppu.bgFifo.cycle();
                REQUIRE(returnedPixel->color == 2);
                REQUIRE(returnedPixel->palette == bgMapAttr.bgPaletteNumber);
            }

            // queue should be filled by now with 8 pixels
            REQUIRE(ppu.bgFifo.pixelQueue.size() == 8);
            REQUIRE(ppu.bgFifo.fetcherXPos == 12);

            // pixels from tile1 from vram bank 0
            for (uint i = 0; i < 8; ++i) {
                returnedPixel = ppu.bgFifo.cycle();
                REQUIRE(returnedPixel->color == 1);
            }

            // pixels from tile0 from vram bank 0
            for (uint i = 0; i < 8; ++i) {
                returnedPixel = ppu.bgFifo.cycle();
                REQUIRE(returnedPixel->color == 2);
            }

            // from this point onwards, tiles are from vram bank 1

            // pixels are from tile1 from vram bank 1
            for (uint i = 0, color = 0; i < 8; ++i) {
                returnedPixel = ppu.bgFifo.cycle();
                REQUIRE(returnedPixel->color == color);

                if ((i + 1) % 2 == 0)
                    ++color;
            }

            // pixels are from tile0 from vram bank 1
            for (uint i = 0; i < 8; ++i) {
                returnedPixel = ppu.bgFifo.cycle();
                REQUIRE(returnedPixel->color == 2);
            }

            // pixels are form tile1 from vram bank 1, flipped hor
            for (uint i = 0, color = 3; i < 8; ++i) {
                returnedPixel = ppu.bgFifo.cycle();
                REQUIRE(returnedPixel->color == color);

                if ((i + 1) % 2 == 0)
                    --color;
            }

            // pixels are from tile0 from vram bank 1, flipped vert
            for (uint i = 0; i < 8; ++i) {
                returnedPixel = ppu.bgFifo.cycle();
                REQUIRE(returnedPixel->color == 1);
            }
        }

        SECTION("BG and Window")
        {
            ppu.setBgTileMapDisplaySelect(0);
            ppu.setWindowTileMapDisplaySelect(1);
            ppu.setBgAndWindowTileDataSelect(1);
            ppu.setBgWindowDisplayPriority(1);
            ppu.setScrollX(252);
            ppu.setScrollY(2);
            ppu.setLy(0);
            ppu.setWindowDisplayEnable(1);
            ppu.setWx(15);
            ppu.setWy(0);
            ppu.xPos = 0;

            BgFifo &bgFifo = ppu.bgFifo;

            // 4 pixel from tile0, 4 pixels from tile 1, start window
            // window: 2 tiles in vram bank 0, 4 tiles in vram bank 1, 2 normal, 1 flipped hor, 1
            // flipped vert

            mem.setCurrentVramBank(0);

            uint16_t tile1Addr = 0x8000;
            for (uint8_t i = 0; i < 16; i += 8) {
                // 00 00 ff 00 00 ff ff ff - each line is "darker" than the previous
                mem.writemem(0x0, tile1Addr + i, true, true);
                mem.writemem(0x0, tile1Addr + i + 1, true, true);
                mem.writemem(0xFF, tile1Addr + i + 2, true, true);
                mem.writemem(0x0, tile1Addr + i + 3, true, true);
                mem.writemem(0x0, tile1Addr + i + 4, true, true);
                mem.writemem(0xFF, tile1Addr + i + 5, true, true);
                mem.writemem(0xFF, tile1Addr + i + 6, true, true);
                mem.writemem(0xFF, tile1Addr + i + 7, true, true);
            }

            uint16_t tile2Addr = 0x8010;
            for (uint8_t i = 0; i < 16; i += 8) {
                // ff ff 00 ff ff 00 00 00 - each line is "lighter" than the previous
                mem.writemem(0xFF, tile2Addr + i, true, true);
                mem.writemem(0xFF, tile2Addr + i + 1, true, true);
                mem.writemem(0x0, tile2Addr + i + 2, true, true);
                mem.writemem(0xFF, tile2Addr + i + 3, true, true);
                mem.writemem(0xFF, tile2Addr + i + 4, true, true);
                mem.writemem(0x0, tile2Addr + i + 5, true, true);
                mem.writemem(0x00, tile2Addr + i + 6, true, true);
                mem.writemem(0x00, tile2Addr + i + 7, true, true);
            }

            mem.setCurrentVramBank(1);

            // Write the tiles in vram bank 1

            // ff ff ff ff 00 ff 00 ff ff 00 ff 00 00 00 00 00
            // dark to light from top to bottom
            // good for vertical flip
            mem.writemem(0xFF, tile1Addr, true, true);
            mem.writemem(0xFF, tile1Addr + 1, true, true);
            mem.writemem(0xFF, tile1Addr + 2, true, true);
            mem.writemem(0xFF, tile1Addr + 3, true, true);
            mem.writemem(0x0, tile1Addr + 4, true, true);
            mem.writemem(0xFF, tile1Addr + 5, true, true);
            mem.writemem(0x0, tile1Addr + 6, true, true);
            mem.writemem(0xFF, tile1Addr + 7, true, true);
            mem.writemem(0xFF, tile1Addr + 8, true, true);
            mem.writemem(0x0, tile1Addr + 9, true, true);
            mem.writemem(0xFF, tile1Addr + 10, true, true);
            mem.writemem(0x0, tile1Addr + 11, true, true);
            mem.writemem(0x0, tile1Addr + 12, true, true);
            mem.writemem(0x0, tile1Addr + 13, true, true);
            mem.writemem(0x0, tile1Addr + 14, true, true);
            mem.writemem(0x0, tile1Addr + 15, true, true);

            for (uint8_t i = 0; i < 16; i += 2) {
                // 33 0f 33 0f 33 0f 33 0f 33 0f 33 0f 33 0f 33 0f
                // light to dark from left to right
                // good for horizontal flip
                mem.writemem(0x33, tile2Addr + i, true, true);
                mem.writemem(0x0F, tile2Addr + i + 1, true, true);
            }

            BgMapAttributes bgMapAttr;
            bgMapAttr.bgPaletteNumber = 2;
            bgMapAttr.bgToOamPriority = 1;
            bgMapAttr.horizontalFlip = false;
            bgMapAttr.verticalFlip = false;
            bgMapAttr.tileVramBankNumber = 0;

            // bg tile map
            mem.setCurrentVramBank(0);
            mem.writemem(0, 0x981F, true, true);
            mem.writemem(1, 0x9800, true, true);
            mem.writemem(0, 0x9801, true, true);

            // bg tile map attr
            mem.setCurrentVramBank(1);

            mem.writemem(bgMapAttr.getAsByte(), 0x981F, true, true);
            mem.writemem(bgMapAttr.getAsByte(), 0x9800, true, true);
            mem.writemem(bgMapAttr.getAsByte(), 0x9801, true, true);

            // window tile map
            // these are in vram bank 0
            mem.setCurrentVramBank(0);
            mem.writemem(0, 0x9C00, true, true);
            mem.writemem(1, 0x9C01, true, true);

            // these are in vram bank 1, first 2 are not flipped
            mem.writemem(0, 0x9C02, true, true);
            mem.writemem(1, 0x9C03, true, true);
            mem.writemem(0, 0x9C04, true, true);
            mem.writemem(1, 0x9C05, true, true);

            // window tile map attr
            mem.setCurrentVramBank(1);

            bgMapAttr.tileVramBankNumber = 0;
            mem.writemem(bgMapAttr.getAsByte(), 0x9C00, true, true);
            mem.writemem(bgMapAttr.getAsByte(), 0x9C01, true, true);

            bgMapAttr.tileVramBankNumber = 1;
            mem.writemem(bgMapAttr.getAsByte(), 0x9C02, true, true);
            mem.writemem(bgMapAttr.getAsByte(), 0x9C03, true, true);

            bgMapAttr.verticalFlip = true;
            mem.writemem(bgMapAttr.getAsByte(), 0x9C04, true, true);

            bgMapAttr.verticalFlip = false;
            bgMapAttr.horizontalFlip = true;
            mem.writemem(bgMapAttr.getAsByte(), 0x9C05, true, true);

            // bg: 4 pixels from tile 0, 4 pixels from tile 1
            // window: start with tile 0

            // 6 cycles to fetch tile, 6th cycle also pushes pixel
            // 5 + 4 + 4

            FifoPixel *returnedPixel;

            bgFifo.prepareForLine(ppu.getLy());

            for (uint i = 0; i < 5; ++i) {
                returnedPixel = bgFifo.cycle();
                REQUIRE(returnedPixel == nullptr);
            }

            for (uint i = 0; i < 4; ++i) {
                returnedPixel = bgFifo.cycle();
                REQUIRE(returnedPixel == nullptr);
            }

            for (uint i = 0; i < 4; ++i) {
                returnedPixel = bgFifo.cycle();
                REQUIRE(returnedPixel->color == 2);
            }

            for (uint i = 0; i < 4; ++i) {
                returnedPixel = bgFifo.cycle();
                REQUIRE(returnedPixel->color == 1);
            }

            // Start rendering window
            returnedPixel = bgFifo.cycle();
            REQUIRE(returnedPixel == nullptr);
            REQUIRE(bgFifo.isDrawingWindow);
            REQUIRE(bgFifo.pixelQueue.size() == 0);

            for (uint i = 0; i < 5; ++i) {
                returnedPixel = bgFifo.cycle();
                REQUIRE(returnedPixel == nullptr);
            }

            // tile1 vram bank 0
            for (uint i = 0; i < 8; ++i) {
                returnedPixel = bgFifo.cycle();
                REQUIRE(returnedPixel->color == 2);
            }

            // tile2 vram bank 0
            for (uint i = 0; i < 8; ++i) {
                returnedPixel = bgFifo.cycle();
                REQUIRE(returnedPixel->color == 1);
            }

            // tile1 vram bank 1
            for (uint i = 0; i < 8; ++i) {
                returnedPixel = ppu.bgFifo.cycle();
                REQUIRE(returnedPixel->color == 2);
            }

            // tile2 vram bank 1
            for (uint i = 0, color = 0; i < 8; ++i) {
                returnedPixel = ppu.bgFifo.cycle();
                REQUIRE(returnedPixel->color == color);

                if ((i + 1) % 2 == 0)
                    ++color;
            }

            // tile1 vram bank 1 flipped vert
            for (uint i = 0; i < 8; ++i) {
                returnedPixel = ppu.bgFifo.cycle();
                REQUIRE(returnedPixel->color == 1);
            }

            // tile2 vram bank 1 flipped hor
            for (uint i = 0, color = 3; i < 8; ++i) {
                returnedPixel = ppu.bgFifo.cycle();
                REQUIRE(returnedPixel->color == color);

                if ((i + 1) % 2 == 0)
                    --color;
            }
        }
    }
}

TEST_CASE("Sprite Fifo", "[PPU_FIFO]")
{
    PPU ppu;
    Memory mem;

    ppu.memory = &mem;
    mem.ppu = &ppu;

    BgFifo &bgFifo = ppu.bgFifo;
    SpriteFifo &spriteFifo = ppu.spriteFifo;

    SECTION("DMG")
    {
        ppu.emulatorMode = EmulatorMode::DMG;
        mem.mode = EmulatorMode::DMG;

        ppu.setLcdDisplayEnable(1);
        ppu.setWindowDisplayEnable(0);
        ppu.setBgAndWindowTileDataSelect(1);
        ppu.setObjSize(0);
        ppu.setObjDisplayEnable(1);
        ppu.setBgWindowDisplayPriority(1);
        ppu.setLy(0);
        ppu.xPos = 0;

        mem.setCurrentVramBank(0);

        // sprite and bg tiles at 0x8000
        uint16_t tile0Addr = 0x8000;
        uint16_t tile1Addr = 0x8010;
        uint16_t tile2Addr = 0x8020;
        uint16_t tile3Addr = 0x8030;

        // write tiles

        // Tile 0
        // ff ff ff ff 00 ff 00 ff ff 00 ff 00 00 00 00 00
        // dark to light from top to bottom
        // good for vertical flip
        mem.writemem(0xFF, tile0Addr, true, true);
        mem.writemem(0xFF, tile0Addr + 1, true, true);
        mem.writemem(0xFF, tile0Addr + 2, true, true);
        mem.writemem(0xFF, tile0Addr + 3, true, true);
        mem.writemem(0x0, tile0Addr + 4, true, true);
        mem.writemem(0xFF, tile0Addr + 5, true, true);
        mem.writemem(0x0, tile0Addr + 6, true, true);
        mem.writemem(0xFF, tile0Addr + 7, true, true);
        mem.writemem(0xFF, tile0Addr + 8, true, true);
        mem.writemem(0x0, tile0Addr + 9, true, true);
        mem.writemem(0xFF, tile0Addr + 10, true, true);
        mem.writemem(0x0, tile0Addr + 11, true, true);
        mem.writemem(0x0, tile0Addr + 12, true, true);
        mem.writemem(0x0, tile0Addr + 13, true, true);
        mem.writemem(0x0, tile0Addr + 14, true, true);
        mem.writemem(0x0, tile0Addr + 15, true, true);

        // Tile 1
        for (uint8_t i = 0; i < 16; i += 2) {
            // 33 0f 33 0f 33 0f 33 0f 33 0f 33 0f 33 0f 33 0f
            // light to dark from left to right
            // good for horizontal flip
            mem.writemem(0x33, tile1Addr + i, true, true);
            mem.writemem(0x0F, tile1Addr + i + 1, true, true);
        }

        // Tile 2
        // light gray tile
        for (uint8_t i = 0; i < 16; i += 2) {
            mem.writemem(0xFF, tile2Addr + i, true, true);
            mem.writemem(0x00, tile2Addr + i + 1, true, true);
        }

        // Tile 3
        // dark gray tile
        for (uint8_t i = 0; i < 16; i += 2) {
            mem.writemem(0x00, tile3Addr + i, true, true);
            mem.writemem(0xFF, tile3Addr + i + 1, true, true);
        }

        // Bg Tile Map
        // Tiles will be Dark Gray (Tile 3)
        for (uint i = 0; i < 32; ++i) {
            mem.writemem(3, 0x9800 + i, true, true);
        }

        SECTION("Single Sprite")
        {
            OAMSprite oamSprite;

            // Sprite 0, Light Gray (Tile 2)
            oamSprite.yPos = 16;
            oamSprite.xPos = 12;
            oamSprite.dmgPaletteNumber = 1;
            oamSprite.tileNumber = 2;
            oamSprite.objToBgPriority = 0;
            oamSprite.xFlip = 0;
            oamSprite.yFlip = 0;
            oamSprite.writeAsBytes(mem.oam);

            // Sprite 1, Flipped Vertically (Tile 0)
            oamSprite.xPos = 24;
            oamSprite.tileNumber = 0;
            oamSprite.yFlip = 1;
            oamSprite.writeAsBytes(mem.oam + 4);

            // Sprite 2, Flipped Horizontally (Tile 1)
            oamSprite.xPos = 36;
            oamSprite.tileNumber = 1;
            oamSprite.yFlip = 0;
            oamSprite.xFlip = 1;
            oamSprite.writeAsBytes(mem.oam + 8);

            bgFifo.prepareForLine(ppu.getLy());
            spriteFifo.prepareForLine();

            FifoPixel *bgPixel, *spritePixel;

            // PPU OAM Search
            ppu.setModeFlag(OAM_SEARCH);
            for (uint i = 0; i < PPU_OAM_SEARCH_T_CYCLES; ++i) {
                REQUIRE(ppu.getLcdMode() == OAM_SEARCH);
                ppu.cycle();
            }
            REQUIRE(ppu.getLcdMode() == DRAW);

            // Check found sprites on line
            REQUIRE(ppu.numSpritesOnCurrentLine == 3);
            REQUIRE(ppu.spritesOnCurrentLine[0] == 0);
            REQUIRE(ppu.spritesOnCurrentLine[1] == 1);
            REQUIRE(ppu.spritesOnCurrentLine[2] == 2);
            REQUIRE(ppu.spritesOnCurrentLine[3] == -1);

            // Draw 4 bg pixels
            for (uint i = 0; i < 5; ++i) {
                spriteFifo.checkForSprite();
                REQUIRE_FALSE(spriteFifo.fetchingSprite);

                spritePixel = spriteFifo.cycle();
                REQUIRE(spritePixel == nullptr);

                spriteFifo.checkForSprite();
                REQUIRE_FALSE(spriteFifo.fetchingSprite);

                bgPixel = bgFifo.cycle();
                REQUIRE(bgPixel == nullptr);
            }

            for (uint i = 0; i < 4; ++i) {
                spriteFifo.checkForSprite();
                REQUIRE_FALSE(spriteFifo.fetchingSprite);

                spritePixel = spriteFifo.cycle();
                REQUIRE(spritePixel == nullptr);

                bgPixel = bgFifo.cycle();
                REQUIRE(bgPixel != nullptr);
                REQUIRE(bgPixel->color == 2);

                ++ppu.xPos;
                spriteFifo.fetcherXPos = ppu.xPos;

                spriteFifo.checkForSprite();
                if (i < 3)
                    REQUIRE_FALSE(spriteFifo.fetchingSprite);
                else
                    REQUIRE(spriteFifo.fetchingSprite);
            }

            REQUIRE(spriteFifo.fetchingSprite);
            REQUIRE(spriteFifo.fetcherStage == 1);

            REQUIRE(bgFifo.spriteFetchingActive);
            REQUIRE(bgFifo.fetcherStage == GET_TILE);
            REQUIRE(bgFifo.fetcherStageCycles == 1);

            // Stage 1, do 5 cycles
            for (uint i = 0; i < 5; ++i) {
                spriteFifo.checkForSprite();

                REQUIRE(spriteFifo.fetcherStage == 1);
                spritePixel = spriteFifo.cycle();
                REQUIRE(spritePixel == nullptr);

                bgPixel = bgFifo.cycle();
                REQUIRE(bgPixel == nullptr);

                spriteFifo.checkForSprite();
            }

            REQUIRE(spriteFifo.fetcherStage == 2);
            REQUIRE(spriteFifo.oamPenalty == 5);

            // Stage 2
            spriteFifo.checkForSprite();
            bgPixel = bgFifo.cycle();
            REQUIRE(bgPixel == nullptr);
            spriteFifo.checkForSprite();

            spritePixel = spriteFifo.cycle();
            REQUIRE(spritePixel == nullptr);
            REQUIRE(spriteFifo.fetcherStage == 3);
            REQUIRE(spriteFifo.oamPenalty == 6);

            // Stage 3
            for (uint i = 0; i < 3; ++i) {
                spriteFifo.checkForSprite();

                REQUIRE(spriteFifo.fetcherStage == 3);
                spritePixel = spriteFifo.cycle();
                REQUIRE(spritePixel == nullptr);
                REQUIRE(spriteFifo.oamPenalty == 7 + i);

                bgPixel = bgFifo.cycle();
                REQUIRE(bgPixel == nullptr);

                spriteFifo.checkForSprite();
            }
            REQUIRE(spriteFifo.fetcherStage == 4);

            // Stage 4
            spriteFifo.checkForSprite();

            spritePixel = spriteFifo.cycle();
            REQUIRE(spritePixel == nullptr);
            REQUIRE(spriteFifo.fetcherStage == 5);
            REQUIRE(spriteFifo.oamPenalty == 10);

            bgPixel = bgFifo.cycle();
            REQUIRE(bgPixel == nullptr);

            spriteFifo.checkForSprite();

            // Stage 5
            spriteFifo.checkForSprite();

            spritePixel = spriteFifo.cycle();
            REQUIRE(spritePixel == nullptr);
            REQUIRE(spriteFifo.fetcherStage == 6);
            REQUIRE(spriteFifo.oamPenalty == 11);

            bgPixel = bgFifo.cycle();
            REQUIRE(bgPixel == nullptr);

            spriteFifo.checkForSprite();

            // Stage 6, should render pixel
            for (uint i = 0; i < 8; ++i) {
                spriteFifo.checkForSprite();

                spritePixel = spriteFifo.cycle();
                REQUIRE(spritePixel != nullptr);
                REQUIRE(spritePixel->color == 1);

                bgPixel = bgFifo.cycle();
                REQUIRE(bgPixel != nullptr);
                REQUIRE(bgPixel->color == 2);

                ++ppu.xPos;
                spriteFifo.fetcherXPos = ppu.xPos;

                spriteFifo.checkForSprite();
            }

            // Render 4 more bg pixels
            for (uint i = 0; i < 4; ++i) {
                spriteFifo.checkForSprite();

                spritePixel = spriteFifo.cycle();
                REQUIRE(spritePixel == nullptr);
                REQUIRE_FALSE(spriteFifo.fetchingSprite);

                bgPixel = bgFifo.cycle();
                REQUIRE(bgPixel != nullptr);
                REQUIRE(bgPixel->color == 2);

                ++ppu.xPos;
                spriteFifo.fetcherXPos = ppu.xPos;

                spriteFifo.checkForSprite();
            }

            REQUIRE(spriteFifo.fetchingSprite);

            // Fetch vertically flipped sprite
            for (uint i = 0; i < 11; ++i) {
                spriteFifo.checkForSprite();

                spritePixel = spriteFifo.cycle();
                REQUIRE(spritePixel == nullptr);
                REQUIRE(spriteFifo.fetchingSprite);

                bgPixel = bgFifo.cycle();
                REQUIRE(bgPixel == nullptr);

                spriteFifo.checkForSprite();
            }

            // Draw vertically flipped sprite
            for (uint i = 0; i < 8; ++i) {
                spriteFifo.checkForSprite();

                spritePixel = spriteFifo.cycle();
                REQUIRE(spritePixel != nullptr);
                REQUIRE(spritePixel->color == 0);

                bgPixel = bgFifo.cycle();
                REQUIRE(bgPixel != nullptr);
                REQUIRE(bgPixel->color == 2);

                ++ppu.xPos;
                spriteFifo.fetcherXPos = ppu.xPos;

                spriteFifo.checkForSprite();
            }

            // Render 4 more bg pixels
            for (uint i = 0; i < 4; ++i) {
                spriteFifo.checkForSprite();

                spritePixel = spriteFifo.cycle();
                REQUIRE(spritePixel == nullptr);
                REQUIRE_FALSE(spriteFifo.fetchingSprite);

                bgPixel = bgFifo.cycle();
                REQUIRE(bgPixel != nullptr);
                REQUIRE(bgPixel->color == 2);

                ++ppu.xPos;
                spriteFifo.fetcherXPos = ppu.xPos;

                spriteFifo.checkForSprite();
            }

            // Fetch horizontally flipped sprite
            for (uint i = 0; i < 11; ++i) {
                spriteFifo.checkForSprite();

                spritePixel = spriteFifo.cycle();
                REQUIRE(spritePixel == nullptr);
                REQUIRE(spriteFifo.fetchingSprite);

                bgPixel = bgFifo.cycle();
                REQUIRE(bgPixel == nullptr);

                spriteFifo.checkForSprite();
            }

            // Draw horizontally flipped sprite
            for (uint i = 0, color = 3; i < 8; ++i) {
                spriteFifo.checkForSprite();

                spritePixel = spriteFifo.cycle();
                REQUIRE(spritePixel != nullptr);
                REQUIRE(spritePixel->color == color);

                if ((i + 1) % 2 == 0)
                    --color;

                bgPixel = bgFifo.cycle();
                REQUIRE(bgPixel != nullptr);
                REQUIRE(bgPixel->color == 2);

                ++ppu.xPos;
                spriteFifo.fetcherXPos = ppu.xPos;

                spriteFifo.checkForSprite();
            }
        }

        SECTION("Sprite Overlap")
        {
            OAMSprite oamSprite;

            // Sprite 2, Light Gray (Tile 2)
            oamSprite.yPos = 16;
            oamSprite.xPos = 12;
            oamSprite.dmgPaletteNumber = 1;
            oamSprite.tileNumber = 2;
            oamSprite.objToBgPriority = 0;
            oamSprite.xFlip = 0;
            oamSprite.yFlip = 0;
            oamSprite.writeAsBytes(mem.oam + 8);

            // Sprite 1, Tile 0
            oamSprite.xPos = 16;
            oamSprite.tileNumber = 0;
            oamSprite.writeAsBytes(mem.oam + 4);

            // Sprite 3, Dark Gray (Tile 3)
            oamSprite.xPos = 20;
            oamSprite.tileNumber = 3;
            oamSprite.writeAsBytes(mem.oam + 12);

            bgFifo.prepareForLine(ppu.getLy());
            spriteFifo.prepareForLine();

            FifoPixel *bgPixel, *spritePixel;

            // PPU OAM Search
            ppu.setModeFlag(OAM_SEARCH);
            for (uint i = 0; i < PPU_OAM_SEARCH_T_CYCLES; ++i) {
                REQUIRE(ppu.getLcdMode() == OAM_SEARCH);
                ppu.cycle();
            }
            REQUIRE(ppu.getLcdMode() == DRAW);

            // Check found sprites on line
            REQUIRE(ppu.numSpritesOnCurrentLine == 3);
            REQUIRE(ppu.spritesOnCurrentLine[0] == 1);
            REQUIRE(ppu.spritesOnCurrentLine[1] == 2);
            REQUIRE(ppu.spritesOnCurrentLine[2] == 3);
            REQUIRE(ppu.spritesOnCurrentLine[3] == -1);

            // Draw 4 bg pixels
            for (uint i = 0; i < 5; ++i) {
                spriteFifo.checkForSprite();

                spritePixel = spriteFifo.cycle();
                REQUIRE(spritePixel == nullptr);
                REQUIRE_FALSE(spriteFifo.fetchingSprite);

                bgPixel = bgFifo.cycle();
                REQUIRE(bgPixel == nullptr);

                spriteFifo.checkForSprite();
            }

            for (uint i = 0; i < 4; ++i) {
                spriteFifo.checkForSprite();

                spritePixel = spriteFifo.cycle();
                REQUIRE(spritePixel == nullptr);
                REQUIRE_FALSE(spriteFifo.fetchingSprite);

                bgPixel = bgFifo.cycle();
                REQUIRE(bgPixel != nullptr);
                REQUIRE(bgPixel->color == 2);

                ++ppu.xPos;
                spriteFifo.fetcherXPos = ppu.xPos;

                spriteFifo.checkForSprite();
            }

            REQUIRE(spriteFifo.fetchingSprite);
            REQUIRE(spriteFifo.fetcherStage == 1);

            REQUIRE(bgFifo.spriteFetchingActive);
            REQUIRE(bgFifo.fetcherStage == GET_TILE);
            REQUIRE(bgFifo.fetcherStageCycles == 1);

            // Fetch Sprite 2 tile
            for (uint i = 0; i < 11; ++i) {
                spriteFifo.checkForSprite();

                spritePixel = spriteFifo.cycle();
                REQUIRE(spritePixel == nullptr);
                REQUIRE(spriteFifo.fetchingSprite);

                bgPixel = bgFifo.cycle();
                REQUIRE(bgPixel == nullptr);

                spriteFifo.checkForSprite();
            }

            // Draw 4 pixels from sprite 2
            for (uint i = 0; i < 4; ++i) {
                spriteFifo.checkForSprite();

                spritePixel = spriteFifo.cycle();
                REQUIRE(spritePixel != nullptr);
                REQUIRE(spritePixel->color == 1);
                REQUIRE_FALSE(spriteFifo.fetchingSprite);

                bgPixel = bgFifo.cycle();
                REQUIRE(bgPixel != nullptr);
                REQUIRE(bgPixel->color == 2);

                ++ppu.xPos;
                spriteFifo.fetcherXPos = ppu.xPos;

                spriteFifo.checkForSprite();
            }

            // Fetch Sprite 1 tile
            for (uint i = 0; i < 11; ++i) {
                spriteFifo.checkForSprite();

                spritePixel = spriteFifo.cycle();
                REQUIRE(spritePixel == nullptr);
                REQUIRE(spriteFifo.fetchingSprite);

                bgPixel = bgFifo.cycle();
                REQUIRE(bgPixel == nullptr);

                spriteFifo.checkForSprite();
            }

            // Draw remaining 4 pixels from sprite 2, in DMG pririty is determined by x pos
            for (uint i = 0; i < 4; ++i) {
                spriteFifo.checkForSprite();

                spritePixel = spriteFifo.cycle();
                REQUIRE(spritePixel != nullptr);
                REQUIRE(spritePixel->color == 1);
                REQUIRE_FALSE(spriteFifo.fetchingSprite);

                bgPixel = bgFifo.cycle();
                REQUIRE(bgPixel != nullptr);
                REQUIRE(bgPixel->color == 2);

                ++ppu.xPos;
                spriteFifo.fetcherXPos = ppu.xPos;

                spriteFifo.checkForSprite();
            }

            // Fetch Sprite 3
            for (uint i = 0; i < 11; ++i) {
                spriteFifo.checkForSprite();

                spritePixel = spriteFifo.cycle();
                REQUIRE(spritePixel == nullptr);
                REQUIRE(spriteFifo.fetchingSprite);

                bgPixel = bgFifo.cycle();
                REQUIRE(bgPixel == nullptr);

                spriteFifo.checkForSprite();
            }

            // Draw remaining 4 pixels from Sprite 1
            for (uint i = 0; i < 4; ++i) {
                spriteFifo.checkForSprite();

                spritePixel = spriteFifo.cycle();
                REQUIRE(spritePixel != nullptr);
                REQUIRE(spritePixel->color == 3);
                REQUIRE_FALSE(spriteFifo.fetchingSprite);

                bgPixel = bgFifo.cycle();
                REQUIRE(bgPixel != nullptr);
                REQUIRE(bgPixel->color == 2);

                ++ppu.xPos;
                spriteFifo.fetcherXPos = ppu.xPos;

                spriteFifo.checkForSprite();
            }

            // Draw Sprite 3
            for (uint i = 0; i < 4; ++i) {
                spriteFifo.checkForSprite();

                spritePixel = spriteFifo.cycle();
                REQUIRE(spritePixel != nullptr);
                REQUIRE(spritePixel->color == 2);
                REQUIRE_FALSE(spriteFifo.fetchingSprite);

                bgPixel = bgFifo.cycle();
                REQUIRE(bgPixel != nullptr);
                REQUIRE(bgPixel->color == 2);

                ++ppu.xPos;
                spriteFifo.fetcherXPos = ppu.xPos;

                spriteFifo.checkForSprite();
            }

            REQUIRE_FALSE(spriteFifo.fetchingSprite);
            REQUIRE_FALSE(bgFifo.spriteFetchingActive);
        }

        SECTION("Partial Sprite")
        {
            SECTION("Beginning of the screen")
            {
                OAMSprite oamSprite;

                // Sprite 0, Tile 1
                oamSprite.yPos = 16;
                oamSprite.xPos = 4;
                oamSprite.dmgPaletteNumber = 1;
                oamSprite.tileNumber = 1;
                oamSprite.objToBgPriority = 0;
                oamSprite.xFlip = 0;
                oamSprite.yFlip = 0;
                oamSprite.writeAsBytes(mem.oam);

                bgFifo.prepareForLine(ppu.getLy());
                spriteFifo.prepareForLine();

                FifoPixel *bgPixel, *spritePixel;

                // PPU OAM Search
                ppu.setModeFlag(OAM_SEARCH);
                for (uint i = 0; i < PPU_OAM_SEARCH_T_CYCLES; ++i) {
                    REQUIRE(ppu.getLcdMode() == OAM_SEARCH);
                    ppu.cycle();
                }
                REQUIRE(ppu.getLcdMode() == DRAW);

                // Check found sprites on line
                REQUIRE(ppu.numSpritesOnCurrentLine == 1);
                REQUIRE(ppu.spritesOnCurrentLine[0] == 0);
                REQUIRE(ppu.spritesOnCurrentLine[1] == -1);

                // Start fetching BG and Sprite
                for (uint i = 0; i < 6; ++i) {
                    spriteFifo.checkForSprite();

                    spritePixel = spriteFifo.cycle();
                    REQUIRE(spritePixel == nullptr);
                    REQUIRE(spriteFifo.fetchingSprite);

                    REQUIRE(bgFifo.pixelQueue.empty());
                    bgPixel = bgFifo.cycle();
                    REQUIRE(bgPixel == nullptr);

                    spriteFifo.checkForSprite();
                }

                REQUIRE_FALSE(bgFifo.pixelQueue.empty());

                for (uint i = 0; i < 7; ++i) {
                    spriteFifo.checkForSprite();

                    spritePixel = spriteFifo.cycle();
                    REQUIRE(spritePixel == nullptr);
                    REQUIRE(spriteFifo.fetchingSprite);

                    bgPixel = bgFifo.cycle();
                    REQUIRE(bgPixel == nullptr);

                    spriteFifo.checkForSprite();
                }

                for (uint i = 0, color = 2; i < 4; ++i) {
                    spriteFifo.checkForSprite();

                    spritePixel = spriteFifo.cycle();
                    REQUIRE(spritePixel != nullptr);
                    REQUIRE(spritePixel->color);

                    if ((i + 1) % 2 == 0)
                        ++color;

                    bgPixel = bgFifo.cycle();
                    REQUIRE(bgPixel != nullptr);
                    REQUIRE(bgPixel->color == 2);

                    ++ppu.xPos;
                    spriteFifo.fetcherXPos = ppu.xPos;

                    spriteFifo.checkForSprite();
                }

                // Check for blank pixels in sprite fifo
                for (uint i = 0; i < 4; ++i) {
                    spriteFifo.checkForSprite();

                    spritePixel = spriteFifo.cycle();
                    REQUIRE(spritePixel != nullptr);
                    REQUIRE(spritePixel->color == 0);

                    bgPixel = bgFifo.cycle();
                    REQUIRE(bgPixel != nullptr);
                    REQUIRE(bgPixel->color == 2);

                    ++ppu.xPos;
                    spriteFifo.fetcherXPos = ppu.xPos;

                    spriteFifo.checkForSprite();
                }

                REQUIRE(spriteFifo.pixelQueue.size() == 0);
            }

            SECTION("End of the screen")
            {
                OAMSprite oamSprite;
                oamSprite.yPos = 16;
                oamSprite.xPos = PPU_SCREEN_WIDTH + 8 - 4;
                oamSprite.dmgPaletteNumber = 1;
                oamSprite.tileNumber = 2;
                oamSprite.objToBgPriority = 0;
                oamSprite.xFlip = 0;
                oamSprite.yFlip = 0;
                oamSprite.writeAsBytes(mem.oam);

                oamSprite.writeAsBytes(mem.oam);

                bgFifo.prepareForLine(ppu.getLy());
                spriteFifo.prepareForLine();

                FifoPixel *bgPixel, *spritePixel;

                // PPU OAM Search
                ppu.setModeFlag(OAM_SEARCH);
                for (uint i = 0; i < PPU_OAM_SEARCH_T_CYCLES; ++i) {
                    REQUIRE(ppu.getLcdMode() == OAM_SEARCH);
                    ppu.cycle();
                }
                REQUIRE(ppu.getLcdMode() == DRAW);

                // Check found sprites on line
                REQUIRE(ppu.numSpritesOnCurrentLine == 1);
                REQUIRE(ppu.spritesOnCurrentLine[0] == 0);
                REQUIRE(ppu.spritesOnCurrentLine[1] == -1);

                // Go to the end of the screen
                ppu.xPos = PPU_SCREEN_WIDTH - 8;
                bgFifo.fetcherXPos = ppu.xPos;
                spriteFifo.fetcherXPos = ppu.xPos;

                // Fetch BG Tile and draw 4 bg pixels
                for (uint i = 0; i < 5; ++i) {
                    spriteFifo.checkForSprite();
                    REQUIRE_FALSE(spriteFifo.fetchingSprite);

                    spritePixel = spriteFifo.cycle();
                    REQUIRE(spritePixel == nullptr);

                    spriteFifo.checkForSprite();
                    REQUIRE_FALSE(spriteFifo.fetchingSprite);

                    bgPixel = bgFifo.cycle();
                    REQUIRE(bgPixel == nullptr);
                }

                for (uint i = 0; i < 4; ++i) {
                    spriteFifo.checkForSprite();
                    REQUIRE_FALSE(spriteFifo.fetchingSprite);

                    spritePixel = spriteFifo.cycle();
                    REQUIRE(spritePixel == nullptr);

                    bgPixel = bgFifo.cycle();
                    REQUIRE(bgPixel != nullptr);
                    REQUIRE(bgPixel->color == 2);

                    ++ppu.xPos;
                    spriteFifo.fetcherXPos = ppu.xPos;

                    spriteFifo.checkForSprite();
                }

                REQUIRE(spriteFifo.fetchingSprite);

                // Fetch sprite
                for (uint i = 0; i < 11; ++i) {
                    spriteFifo.checkForSprite();

                    spritePixel = spriteFifo.cycle();
                    REQUIRE(spritePixel == nullptr);
                    REQUIRE(spriteFifo.fetchingSprite);

                    bgPixel = bgFifo.cycle();
                    REQUIRE(bgPixel == nullptr);

                    spriteFifo.checkForSprite();
                }

                // Draw pixels
                for (uint i = 0; i < 4; ++i) {
                    spriteFifo.checkForSprite();

                    spritePixel = spriteFifo.cycle();
                    REQUIRE(spritePixel != nullptr);
                    REQUIRE(spritePixel->color == 1);

                    bgPixel = bgFifo.cycle();
                    REQUIRE(bgPixel != nullptr);
                    REQUIRE(bgPixel->color == 2);

                    ++ppu.xPos;
                    spriteFifo.fetcherXPos = ppu.xPos;

                    spriteFifo.checkForSprite();
                }
            }
        }

        SECTION("Position 0 penalty")
        {
            ppu.setScrollX(7);

            OAMSprite oamSprite;

            // Sprite 0, Tile 2
            oamSprite.yPos = 16;
            oamSprite.xPos = 8;
            oamSprite.dmgPaletteNumber = 1;
            oamSprite.tileNumber = 2;
            oamSprite.objToBgPriority = 0;
            oamSprite.xFlip = 0;
            oamSprite.yFlip = 0;
            oamSprite.writeAsBytes(mem.oam);

            bgFifo.prepareForLine(ppu.getLy());
            spriteFifo.prepareForLine();

            FifoPixel *bgPixel, *spritePixel;

            // PPU OAM Search
            ppu.setModeFlag(OAM_SEARCH);
            for (uint i = 0; i < PPU_OAM_SEARCH_T_CYCLES; ++i) {
                REQUIRE(ppu.getLcdMode() == OAM_SEARCH);
                ppu.cycle();
            }
            REQUIRE(ppu.getLcdMode() == DRAW);

            // Check found sprites on line
            REQUIRE(ppu.numSpritesOnCurrentLine == 1);
            REQUIRE(ppu.spritesOnCurrentLine[0] == 0);
            REQUIRE(ppu.spritesOnCurrentLine[1] == -1);

            // Start fetching bg tile and apply sprite x pos 0 penalty
            for (uint i = 0; i < 7; ++i) {
                spriteFifo.checkForSprite();

                REQUIRE_FALSE(spriteFifo.appliedPenaltyAtXPos0);
                REQUIRE(spriteFifo.xPos0Penalty == 7 - i);
                spritePixel = spriteFifo.cycle();
                REQUIRE(spritePixel == nullptr);
                REQUIRE(spriteFifo.fetchingSprite);

                bgPixel = bgFifo.cycle();
                REQUIRE(bgPixel == nullptr);

                spriteFifo.checkForSprite();
            }

            REQUIRE(spriteFifo.appliedPenaltyAtXPos0);

            for (uint i = 0; i < 11; ++i) {
                spriteFifo.checkForSprite();

                spritePixel = spriteFifo.cycle();
                REQUIRE(spritePixel == nullptr);
                REQUIRE(spriteFifo.fetchingSprite);

                bgPixel = bgFifo.cycle();
                REQUIRE(bgPixel == nullptr);

                spriteFifo.checkForSprite();
            }

            // Draw sprite and bg
            for (uint i = 0; i < 8; ++i) {
                spriteFifo.checkForSprite();

                spritePixel = spriteFifo.cycle();
                REQUIRE(spritePixel != nullptr);
                REQUIRE(spritePixel->color == 1);

                bgPixel = bgFifo.cycle();
                REQUIRE(bgPixel != nullptr);
                REQUIRE(bgPixel->color == 2);

                ++ppu.xPos;
                spriteFifo.fetcherXPos = ppu.xPos;

                spriteFifo.checkForSprite();
            }
        }

        SECTION("Fetch Abort")
        {
            // TODO
        }
    }

    SECTION("CGB")
    {
        ppu.emulatorMode = EmulatorMode::CGB;
        mem.mode = EmulatorMode::CGB;

        ppu.setLcdDisplayEnable(1);
        ppu.setWindowDisplayEnable(0);
        ppu.setBgAndWindowTileDataSelect(1);
        ppu.setObjSize(0);
        ppu.setObjDisplayEnable(1);
        ppu.setBgWindowDisplayPriority(1);
        ppu.setLy(0);
        ppu.xPos = 0;

        // sprite and bg tiles at 0x8000
        // Tiles 0 and 1 are in bank 0, 2 and 3 are in bank 1
        uint16_t tile0Addr = 0x8000;
        uint16_t tile1Addr = 0x8010;
        uint16_t tile2Addr = 0x8020;
        uint16_t tile3Addr = 0x8030;

        // Write tiles

        mem.setCurrentVramBank(0);

        // Tile 0
        // ff ff ff ff 00 ff 00 ff ff 00 ff 00 00 00 00 00
        // dark to light from top to bottom
        // good for vertical flip
        mem.writemem(0xFF, tile0Addr, true, true);
        mem.writemem(0xFF, tile0Addr + 1, true, true);
        mem.writemem(0xFF, tile0Addr + 2, true, true);
        mem.writemem(0xFF, tile0Addr + 3, true, true);
        mem.writemem(0x0, tile0Addr + 4, true, true);
        mem.writemem(0xFF, tile0Addr + 5, true, true);
        mem.writemem(0x0, tile0Addr + 6, true, true);
        mem.writemem(0xFF, tile0Addr + 7, true, true);
        mem.writemem(0xFF, tile0Addr + 8, true, true);
        mem.writemem(0x0, tile0Addr + 9, true, true);
        mem.writemem(0xFF, tile0Addr + 10, true, true);
        mem.writemem(0x0, tile0Addr + 11, true, true);
        mem.writemem(0x0, tile0Addr + 12, true, true);
        mem.writemem(0x0, tile0Addr + 13, true, true);
        mem.writemem(0x0, tile0Addr + 14, true, true);
        mem.writemem(0x0, tile0Addr + 15, true, true);

        // Tile 1
        for (uint8_t i = 0; i < 16; i += 2) {
            // 33 0f 33 0f 33 0f 33 0f 33 0f 33 0f 33 0f 33 0f
            // light to dark from left to right
            // good for horizontal flip
            mem.writemem(0x33, tile1Addr + i, true, true);
            mem.writemem(0x0F, tile1Addr + i + 1, true, true);
        }

        mem.setCurrentVramBank(1);

        // Tile 2
        // light gray tile
        for (uint8_t i = 0; i < 16; i += 2) {
            mem.writemem(0xFF, tile2Addr + i, true, true);
            mem.writemem(0x00, tile2Addr + i + 1, true, true);
        }

        // Tile 3
        // dark gray tile
        for (uint8_t i = 0; i < 16; i += 2) {
            mem.writemem(0x00, tile3Addr + i, true, true);
            mem.writemem(0xFF, tile3Addr + i + 1, true, true);
        }

        // Write BG Tilemap
        mem.setCurrentVramBank(0);

        // Tiles will be Dark Gray (Tile 3)
        for (uint i = 0; i < 32; ++i) {
            mem.writemem(3, 0x9800 + i, true, true);
        }

        // Write BG Map Attributes
        BgMapAttributes bgMapAttr;
        bgMapAttr.bgPaletteNumber = 0;
        bgMapAttr.horizontalFlip = 0;
        bgMapAttr.verticalFlip = 0;
        bgMapAttr.tileVramBankNumber = 1;

        mem.setCurrentVramBank(1);
        for (uint i = 0; i < 32; ++i) {
            mem.writemem(bgMapAttr.getAsByte(), 0x9800 + i, true, true);
        }

        SECTION("Single Sprite")
        {
            mem.setCurrentVramBank(0);

            OAMSprite oamSprite;

            // Sprite 0, Light Gray (Tile 2)
            oamSprite.yPos = 16;
            oamSprite.xPos = 12;
            oamSprite.cgbPaletteNumber = 0;
            oamSprite.tileVramBank = 1;
            oamSprite.tileNumber = 2;
            oamSprite.objToBgPriority = 0;
            oamSprite.xFlip = 0;
            oamSprite.yFlip = 0;
            oamSprite.writeAsBytes(mem.oam);

            // Sprite 1, Flipped Vertically (Tile 0)
            oamSprite.xPos = 24;
            oamSprite.tileNumber = 0;
            oamSprite.tileVramBank = 0;
            oamSprite.yFlip = 1;
            oamSprite.writeAsBytes(mem.oam + 4);

            // Sprite 2, Flipped Horizontally (Tile 1)
            oamSprite.xPos = 36;
            oamSprite.tileNumber = 1;
            oamSprite.tileVramBank = 0;
            oamSprite.yFlip = 0;
            oamSprite.xFlip = 1;
            oamSprite.writeAsBytes(mem.oam + 8);

            bgFifo.prepareForLine(ppu.getLy());
            spriteFifo.prepareForLine();

            FifoPixel *bgPixel, *spritePixel;

            // PPU OAM Search
            ppu.setModeFlag(OAM_SEARCH);
            for (uint i = 0; i < PPU_OAM_SEARCH_T_CYCLES; ++i) {
                REQUIRE(ppu.getLcdMode() == OAM_SEARCH);
                ppu.cycle();
            }
            REQUIRE(ppu.getLcdMode() == DRAW);

            // Check found sprites on line
            REQUIRE(ppu.numSpritesOnCurrentLine == 3);
            REQUIRE(ppu.spritesOnCurrentLine[0] == 0);
            REQUIRE(ppu.spritesOnCurrentLine[1] == 1);
            REQUIRE(ppu.spritesOnCurrentLine[2] == 2);
            REQUIRE(ppu.spritesOnCurrentLine[3] == -1);

            // Draw 4 bg pixels
            for (uint i = 0; i < 5; ++i) {
                spriteFifo.checkForSprite();
                REQUIRE_FALSE(spriteFifo.fetchingSprite);

                spritePixel = spriteFifo.cycle();
                REQUIRE(spritePixel == nullptr);

                spriteFifo.checkForSprite();
                REQUIRE_FALSE(spriteFifo.fetchingSprite);

                bgPixel = bgFifo.cycle();
                REQUIRE(bgPixel == nullptr);
            }

            for (uint i = 0; i < 4; ++i) {
                spriteFifo.checkForSprite();
                REQUIRE_FALSE(spriteFifo.fetchingSprite);

                spritePixel = spriteFifo.cycle();
                REQUIRE(spritePixel == nullptr);

                bgPixel = bgFifo.cycle();
                REQUIRE(bgPixel != nullptr);
                REQUIRE(bgPixel->color == 2);

                ++ppu.xPos;
                spriteFifo.fetcherXPos = ppu.xPos;

                spriteFifo.checkForSprite();
                if (i < 3)
                    REQUIRE_FALSE(spriteFifo.fetchingSprite);
                else
                    REQUIRE(spriteFifo.fetchingSprite);
            }

            REQUIRE(spriteFifo.fetchingSprite);
            REQUIRE(spriteFifo.fetcherStage == 1);

            REQUIRE(bgFifo.spriteFetchingActive);
            REQUIRE(bgFifo.fetcherStage == GET_TILE);
            REQUIRE(bgFifo.fetcherStageCycles == 1);

            // Stage 1, do 5 cycles
            for (uint i = 0; i < 5; ++i) {
                spriteFifo.checkForSprite();

                REQUIRE(spriteFifo.fetcherStage == 1);
                spritePixel = spriteFifo.cycle();
                REQUIRE(spritePixel == nullptr);

                bgPixel = bgFifo.cycle();
                REQUIRE(bgPixel == nullptr);

                spriteFifo.checkForSprite();
            }

            REQUIRE(spriteFifo.fetcherStage == 2);
            REQUIRE(spriteFifo.oamPenalty == 5);

            // Stage 2
            spriteFifo.checkForSprite();
            bgPixel = bgFifo.cycle();
            REQUIRE(bgPixel == nullptr);
            spriteFifo.checkForSprite();

            spritePixel = spriteFifo.cycle();
            REQUIRE(spritePixel == nullptr);
            REQUIRE(spriteFifo.fetcherStage == 3);
            REQUIRE(spriteFifo.oamPenalty == 6);

            // Stage 3
            for (uint i = 0; i < 3; ++i) {
                spriteFifo.checkForSprite();

                REQUIRE(spriteFifo.fetcherStage == 3);
                spritePixel = spriteFifo.cycle();
                REQUIRE(spritePixel == nullptr);
                REQUIRE(spriteFifo.oamPenalty == 7 + i);

                bgPixel = bgFifo.cycle();
                REQUIRE(bgPixel == nullptr);

                spriteFifo.checkForSprite();
            }
            REQUIRE(spriteFifo.fetcherStage == 4);

            // Stage 4
            spriteFifo.checkForSprite();

            spritePixel = spriteFifo.cycle();
            REQUIRE(spritePixel == nullptr);
            REQUIRE(spriteFifo.fetcherStage == 5);
            REQUIRE(spriteFifo.oamPenalty == 10);

            bgPixel = bgFifo.cycle();
            REQUIRE(bgPixel == nullptr);

            spriteFifo.checkForSprite();

            // Stage 5
            spriteFifo.checkForSprite();

            spritePixel = spriteFifo.cycle();
            REQUIRE(spritePixel == nullptr);
            REQUIRE(spriteFifo.fetcherStage == 6);
            REQUIRE(spriteFifo.oamPenalty == 11);

            bgPixel = bgFifo.cycle();
            REQUIRE(bgPixel == nullptr);

            spriteFifo.checkForSprite();

            // Stage 6, should render pixel
            for (uint i = 0; i < 8; ++i) {
                spriteFifo.checkForSprite();

                spritePixel = spriteFifo.cycle();
                REQUIRE(spritePixel != nullptr);
                REQUIRE(spritePixel->color == 1);

                bgPixel = bgFifo.cycle();
                REQUIRE(bgPixel != nullptr);
                REQUIRE(bgPixel->color == 2);

                ++ppu.xPos;
                spriteFifo.fetcherXPos = ppu.xPos;

                spriteFifo.checkForSprite();
            }

            // Render 4 more bg pixels
            for (uint i = 0; i < 4; ++i) {
                spriteFifo.checkForSprite();

                spritePixel = spriteFifo.cycle();
                REQUIRE(spritePixel == nullptr);
                REQUIRE_FALSE(spriteFifo.fetchingSprite);

                bgPixel = bgFifo.cycle();
                REQUIRE(bgPixel != nullptr);
                REQUIRE(bgPixel->color == 2);

                ++ppu.xPos;
                spriteFifo.fetcherXPos = ppu.xPos;

                spriteFifo.checkForSprite();
            }

            REQUIRE(spriteFifo.fetchingSprite);

            // Fetch vertically flipped sprite
            for (uint i = 0; i < 11; ++i) {
                spriteFifo.checkForSprite();

                spritePixel = spriteFifo.cycle();
                REQUIRE(spritePixel == nullptr);
                REQUIRE(spriteFifo.fetchingSprite);

                bgPixel = bgFifo.cycle();
                REQUIRE(bgPixel == nullptr);

                spriteFifo.checkForSprite();
            }

            // Draw vertically flipped sprite
            for (uint i = 0; i < 8; ++i) {
                spriteFifo.checkForSprite();

                spritePixel = spriteFifo.cycle();
                REQUIRE(spritePixel != nullptr);
                REQUIRE(spritePixel->color == 0);

                bgPixel = bgFifo.cycle();
                REQUIRE(bgPixel != nullptr);
                REQUIRE(bgPixel->color == 2);

                ++ppu.xPos;
                spriteFifo.fetcherXPos = ppu.xPos;

                spriteFifo.checkForSprite();
            }

            // Render 4 more bg pixels
            for (uint i = 0; i < 4; ++i) {
                spriteFifo.checkForSprite();

                spritePixel = spriteFifo.cycle();
                REQUIRE(spritePixel == nullptr);
                REQUIRE_FALSE(spriteFifo.fetchingSprite);

                bgPixel = bgFifo.cycle();
                REQUIRE(bgPixel != nullptr);
                REQUIRE(bgPixel->color == 2);

                ++ppu.xPos;
                spriteFifo.fetcherXPos = ppu.xPos;

                spriteFifo.checkForSprite();
            }

            // Fetch horizontally flipped sprite
            for (uint i = 0; i < 11; ++i) {
                spriteFifo.checkForSprite();

                spritePixel = spriteFifo.cycle();
                REQUIRE(spritePixel == nullptr);
                REQUIRE(spriteFifo.fetchingSprite);

                bgPixel = bgFifo.cycle();
                REQUIRE(bgPixel == nullptr);

                spriteFifo.checkForSprite();
            }

            // Draw horizontally flipped sprite
            for (uint i = 0, color = 3; i < 8; ++i) {
                spriteFifo.checkForSprite();

                spritePixel = spriteFifo.cycle();
                REQUIRE(spritePixel != nullptr);
                REQUIRE(spritePixel->color == color);

                if ((i + 1) % 2 == 0)
                    --color;

                bgPixel = bgFifo.cycle();
                REQUIRE(bgPixel != nullptr);
                REQUIRE(bgPixel->color == 2);

                ++ppu.xPos;
                spriteFifo.fetcherXPos = ppu.xPos;

                spriteFifo.checkForSprite();
            }
        }

        SECTION("Sprite Overlap")
        {
            OAMSprite oamSprite;

            // Sprite 2, Light Gray (Tile 2)
            oamSprite.yPos = 16;
            oamSprite.xPos = 12;
            oamSprite.dmgPaletteNumber = 1;
            oamSprite.tileNumber = 2;
            oamSprite.tileVramBank = 1;
            oamSprite.objToBgPriority = 0;
            oamSprite.xFlip = 0;
            oamSprite.yFlip = 0;
            oamSprite.writeAsBytes(mem.oam + 8);

            // Sprite 1, Tile 0
            oamSprite.xPos = 16;
            oamSprite.tileNumber = 0;
            oamSprite.tileVramBank = 0;
            oamSprite.writeAsBytes(mem.oam + 4);

            // Sprite 3, Dark Gray (Tile 3)
            oamSprite.xPos = 20;
            oamSprite.tileNumber = 3;
            oamSprite.tileVramBank = 1;
            oamSprite.writeAsBytes(mem.oam + 12);

            bgFifo.prepareForLine(ppu.getLy());
            spriteFifo.prepareForLine();

            FifoPixel *bgPixel, *spritePixel;

            // PPU OAM Search
            ppu.setModeFlag(OAM_SEARCH);
            for (uint i = 0; i < PPU_OAM_SEARCH_T_CYCLES; ++i) {
                REQUIRE(ppu.getLcdMode() == OAM_SEARCH);
                ppu.cycle();
            }
            REQUIRE(ppu.getLcdMode() == DRAW);

            // Check found sprites on line
            REQUIRE(ppu.numSpritesOnCurrentLine == 3);
            REQUIRE(ppu.spritesOnCurrentLine[0] == 1);
            REQUIRE(ppu.spritesOnCurrentLine[1] == 2);
            REQUIRE(ppu.spritesOnCurrentLine[2] == 3);
            REQUIRE(ppu.spritesOnCurrentLine[3] == -1);

            // Draw 4 bg pixels
            for (uint i = 0; i < 5; ++i) {
                spriteFifo.checkForSprite();

                spritePixel = spriteFifo.cycle();
                REQUIRE(spritePixel == nullptr);
                REQUIRE_FALSE(spriteFifo.fetchingSprite);

                bgPixel = bgFifo.cycle();
                REQUIRE(bgPixel == nullptr);

                spriteFifo.checkForSprite();
            }

            for (uint i = 0; i < 4; ++i) {
                spriteFifo.checkForSprite();

                spritePixel = spriteFifo.cycle();
                REQUIRE(spritePixel == nullptr);
                REQUIRE_FALSE(spriteFifo.fetchingSprite);

                bgPixel = bgFifo.cycle();
                REQUIRE(bgPixel != nullptr);
                REQUIRE(bgPixel->color == 2);

                ++ppu.xPos;
                spriteFifo.fetcherXPos = ppu.xPos;

                spriteFifo.checkForSprite();
            }

            REQUIRE(spriteFifo.fetchingSprite);
            REQUIRE(spriteFifo.fetcherStage == 1);

            REQUIRE(bgFifo.spriteFetchingActive);
            REQUIRE(bgFifo.fetcherStage == GET_TILE);
            REQUIRE(bgFifo.fetcherStageCycles == 1);

            // Fetch Sprite 2 tile
            for (uint i = 0; i < 11; ++i) {
                spriteFifo.checkForSprite();

                spritePixel = spriteFifo.cycle();
                REQUIRE(spritePixel == nullptr);
                REQUIRE(spriteFifo.fetchingSprite);

                bgPixel = bgFifo.cycle();
                REQUIRE(bgPixel == nullptr);

                spriteFifo.checkForSprite();
            }

            // Draw 4 pixels from sprite 2
            for (uint i = 0; i < 4; ++i) {
                spriteFifo.checkForSprite();

                spritePixel = spriteFifo.cycle();
                REQUIRE(spritePixel != nullptr);
                REQUIRE(spritePixel->color == 1);
                REQUIRE_FALSE(spriteFifo.fetchingSprite);

                bgPixel = bgFifo.cycle();
                REQUIRE(bgPixel != nullptr);
                REQUIRE(bgPixel->color == 2);

                ++ppu.xPos;
                spriteFifo.fetcherXPos = ppu.xPos;

                spriteFifo.checkForSprite();
            }

            // Fetch Sprite 1 tile
            for (uint i = 0; i < 11; ++i) {
                spriteFifo.checkForSprite();

                spritePixel = spriteFifo.cycle();
                REQUIRE(spritePixel == nullptr);
                REQUIRE(spriteFifo.fetchingSprite);

                bgPixel = bgFifo.cycle();
                REQUIRE(bgPixel == nullptr);

                spriteFifo.checkForSprite();
            }

            // Draw 4 pixels from sprite 1, because of higher priority
            for (uint i = 0; i < 4; ++i) {
                spriteFifo.checkForSprite();

                spritePixel = spriteFifo.cycle();
                REQUIRE(spritePixel != nullptr);
                REQUIRE(spritePixel->color == 3);
                REQUIRE_FALSE(spriteFifo.fetchingSprite);

                bgPixel = bgFifo.cycle();
                REQUIRE(bgPixel != nullptr);
                REQUIRE(bgPixel->color == 2);

                ++ppu.xPos;
                spriteFifo.fetcherXPos = ppu.xPos;

                spriteFifo.checkForSprite();
            }

            // Fetch Sprite 3
            for (uint i = 0; i < 11; ++i) {
                spriteFifo.checkForSprite();

                spritePixel = spriteFifo.cycle();
                REQUIRE(spritePixel == nullptr);
                REQUIRE(spriteFifo.fetchingSprite);

                bgPixel = bgFifo.cycle();
                REQUIRE(bgPixel == nullptr);

                spriteFifo.checkForSprite();
            }

            // Draw remaining 4 pixels from Sprite 1
            for (uint i = 0; i < 4; ++i) {
                spriteFifo.checkForSprite();

                spritePixel = spriteFifo.cycle();
                REQUIRE(spritePixel != nullptr);
                REQUIRE(spritePixel->color == 3);
                REQUIRE_FALSE(spriteFifo.fetchingSprite);

                bgPixel = bgFifo.cycle();
                REQUIRE(bgPixel != nullptr);
                REQUIRE(bgPixel->color == 2);

                ++ppu.xPos;
                spriteFifo.fetcherXPos = ppu.xPos;

                spriteFifo.checkForSprite();
            }

            // Draw remaining 4 pixels from Sprite 3
            for (uint i = 0; i < 4; ++i) {
                spriteFifo.checkForSprite();

                spritePixel = spriteFifo.cycle();
                REQUIRE(spritePixel != nullptr);
                REQUIRE(spritePixel->color == 2);
                REQUIRE_FALSE(spriteFifo.fetchingSprite);

                bgPixel = bgFifo.cycle();
                REQUIRE(bgPixel != nullptr);
                REQUIRE(bgPixel->color == 2);

                ++ppu.xPos;
                spriteFifo.fetcherXPos = ppu.xPos;

                spriteFifo.checkForSprite();
            }

            REQUIRE_FALSE(spriteFifo.fetchingSprite);
            REQUIRE_FALSE(bgFifo.spriteFetchingActive);
        }

        SECTION("Partial Sprite")
        {
            SECTION("Beginning of the screen")
            {
                OAMSprite oamSprite;

                // Sprite 0, Tile 1
                oamSprite.yPos = 16;
                oamSprite.xPos = 4;
                oamSprite.dmgPaletteNumber = 1;
                oamSprite.tileNumber = 1;
                oamSprite.tileVramBank = 0;
                oamSprite.objToBgPriority = 0;
                oamSprite.xFlip = 0;
                oamSprite.yFlip = 0;
                oamSprite.writeAsBytes(mem.oam);

                bgFifo.prepareForLine(ppu.getLy());
                spriteFifo.prepareForLine();

                FifoPixel *bgPixel, *spritePixel;

                // PPU OAM Search
                ppu.setModeFlag(OAM_SEARCH);
                for (uint i = 0; i < PPU_OAM_SEARCH_T_CYCLES; ++i) {
                    REQUIRE(ppu.getLcdMode() == OAM_SEARCH);
                    ppu.cycle();
                }
                REQUIRE(ppu.getLcdMode() == DRAW);

                // Check found sprites on line
                REQUIRE(ppu.numSpritesOnCurrentLine == 1);
                REQUIRE(ppu.spritesOnCurrentLine[0] == 0);
                REQUIRE(ppu.spritesOnCurrentLine[1] == -1);

                // Start fetching BG and Sprite
                for (uint i = 0; i < 6; ++i) {
                    spriteFifo.checkForSprite();

                    spritePixel = spriteFifo.cycle();
                    REQUIRE(spritePixel == nullptr);
                    REQUIRE(spriteFifo.fetchingSprite);

                    REQUIRE(bgFifo.pixelQueue.empty());
                    bgPixel = bgFifo.cycle();
                    REQUIRE(bgPixel == nullptr);

                    spriteFifo.checkForSprite();
                }

                REQUIRE_FALSE(bgFifo.pixelQueue.empty());

                for (uint i = 0; i < 7; ++i) {
                    spriteFifo.checkForSprite();

                    spritePixel = spriteFifo.cycle();
                    REQUIRE(spritePixel == nullptr);
                    REQUIRE(spriteFifo.fetchingSprite);

                    bgPixel = bgFifo.cycle();
                    REQUIRE(bgPixel == nullptr);

                    spriteFifo.checkForSprite();
                }

                for (uint i = 0, color = 2; i < 4; ++i) {
                    spriteFifo.checkForSprite();

                    spritePixel = spriteFifo.cycle();
                    REQUIRE(spritePixel != nullptr);
                    REQUIRE(spritePixel->color);

                    if ((i + 1) % 2 == 0)
                        ++color;

                    bgPixel = bgFifo.cycle();
                    REQUIRE(bgPixel != nullptr);
                    REQUIRE(bgPixel->color == 2);

                    ++ppu.xPos;
                    spriteFifo.fetcherXPos = ppu.xPos;

                    spriteFifo.checkForSprite();
                }

                // Check for blank pixels in sprite fifo
                for (uint i = 0; i < 4; ++i) {
                    spriteFifo.checkForSprite();

                    spritePixel = spriteFifo.cycle();
                    REQUIRE(spritePixel != nullptr);
                    REQUIRE(spritePixel->color == 0);

                    bgPixel = bgFifo.cycle();
                    REQUIRE(bgPixel != nullptr);
                    REQUIRE(bgPixel->color == 2);

                    ++ppu.xPos;
                    spriteFifo.fetcherXPos = ppu.xPos;

                    spriteFifo.checkForSprite();
                }

                REQUIRE(spriteFifo.pixelQueue.size() == 0);
            }

            SECTION("End of the screen")
            {
                OAMSprite oamSprite;
                oamSprite.yPos = 16;
                oamSprite.xPos = PPU_SCREEN_WIDTH + 8 - 4;
                oamSprite.dmgPaletteNumber = 1;
                oamSprite.tileNumber = 2;
                oamSprite.tileVramBank = 1;
                oamSprite.objToBgPriority = 0;
                oamSprite.xFlip = 0;
                oamSprite.yFlip = 0;
                oamSprite.writeAsBytes(mem.oam);

                oamSprite.writeAsBytes(mem.oam);

                bgFifo.prepareForLine(ppu.getLy());
                spriteFifo.prepareForLine();

                FifoPixel *bgPixel, *spritePixel;

                // PPU OAM Search
                ppu.setModeFlag(OAM_SEARCH);
                for (uint i = 0; i < PPU_OAM_SEARCH_T_CYCLES; ++i) {
                    REQUIRE(ppu.getLcdMode() == OAM_SEARCH);
                    ppu.cycle();
                }
                REQUIRE(ppu.getLcdMode() == DRAW);

                // Check found sprites on line
                REQUIRE(ppu.numSpritesOnCurrentLine == 1);
                REQUIRE(ppu.spritesOnCurrentLine[0] == 0);
                REQUIRE(ppu.spritesOnCurrentLine[1] == -1);

                // Go to the end of the screen
                ppu.xPos = PPU_SCREEN_WIDTH - 8;
                bgFifo.fetcherXPos = ppu.xPos;
                spriteFifo.fetcherXPos = ppu.xPos;

                // Fetch BG Tile and draw 4 bg pixels
                for (uint i = 0; i < 5; ++i) {
                    spriteFifo.checkForSprite();
                    REQUIRE_FALSE(spriteFifo.fetchingSprite);

                    spritePixel = spriteFifo.cycle();
                    REQUIRE(spritePixel == nullptr);

                    spriteFifo.checkForSprite();
                    REQUIRE_FALSE(spriteFifo.fetchingSprite);

                    bgPixel = bgFifo.cycle();
                    REQUIRE(bgPixel == nullptr);
                }

                for (uint i = 0; i < 4; ++i) {
                    spriteFifo.checkForSprite();
                    REQUIRE_FALSE(spriteFifo.fetchingSprite);

                    spritePixel = spriteFifo.cycle();
                    REQUIRE(spritePixel == nullptr);

                    bgPixel = bgFifo.cycle();
                    REQUIRE(bgPixel != nullptr);
                    REQUIRE(bgPixel->color == 2);

                    ++ppu.xPos;
                    spriteFifo.fetcherXPos = ppu.xPos;

                    spriteFifo.checkForSprite();
                }

                REQUIRE(spriteFifo.fetchingSprite);

                // Fetch sprite
                for (uint i = 0; i < 11; ++i) {
                    spriteFifo.checkForSprite();

                    spritePixel = spriteFifo.cycle();
                    REQUIRE(spritePixel == nullptr);
                    REQUIRE(spriteFifo.fetchingSprite);

                    bgPixel = bgFifo.cycle();
                    REQUIRE(bgPixel == nullptr);

                    spriteFifo.checkForSprite();
                }

                // Draw pixels
                for (uint i = 0; i < 4; ++i) {
                    spriteFifo.checkForSprite();

                    spritePixel = spriteFifo.cycle();
                    REQUIRE(spritePixel != nullptr);
                    REQUIRE(spritePixel->color == 1);

                    bgPixel = bgFifo.cycle();
                    REQUIRE(bgPixel != nullptr);
                    REQUIRE(bgPixel->color == 2);

                    ++ppu.xPos;
                    spriteFifo.fetcherXPos = ppu.xPos;

                    spriteFifo.checkForSprite();
                }
            }
        }

        SECTION("Position 0 penalty")
        {
            ppu.setScrollX(7);

            OAMSprite oamSprite;

            // Sprite 0, Tile 2
            oamSprite.yPos = 16;
            oamSprite.xPos = 8;
            oamSprite.dmgPaletteNumber = 1;
            oamSprite.tileNumber = 2;
            oamSprite.tileVramBank = 1;
            oamSprite.objToBgPriority = 0;
            oamSprite.xFlip = 0;
            oamSprite.yFlip = 0;
            oamSprite.writeAsBytes(mem.oam);

            bgFifo.prepareForLine(ppu.getLy());
            spriteFifo.prepareForLine();

            FifoPixel *bgPixel, *spritePixel;

            // PPU OAM Search
            ppu.setModeFlag(OAM_SEARCH);
            for (uint i = 0; i < PPU_OAM_SEARCH_T_CYCLES; ++i) {
                REQUIRE(ppu.getLcdMode() == OAM_SEARCH);
                ppu.cycle();
            }
            REQUIRE(ppu.getLcdMode() == DRAW);

            // Check found sprites on line
            REQUIRE(ppu.numSpritesOnCurrentLine == 1);
            REQUIRE(ppu.spritesOnCurrentLine[0] == 0);
            REQUIRE(ppu.spritesOnCurrentLine[1] == -1);

            // Start fetching bg tile and apply sprite x pos 0 penalty
            for (uint i = 0; i < 7; ++i) {
                spriteFifo.checkForSprite();

                REQUIRE_FALSE(spriteFifo.appliedPenaltyAtXPos0);
                REQUIRE(spriteFifo.xPos0Penalty == 7 - i);
                spritePixel = spriteFifo.cycle();
                REQUIRE(spritePixel == nullptr);
                REQUIRE(spriteFifo.fetchingSprite);

                bgPixel = bgFifo.cycle();
                REQUIRE(bgPixel == nullptr);

                spriteFifo.checkForSprite();
            }

            REQUIRE(spriteFifo.appliedPenaltyAtXPos0);

            for (uint i = 0; i < 11; ++i) {
                spriteFifo.checkForSprite();

                spritePixel = spriteFifo.cycle();
                REQUIRE(spritePixel == nullptr);
                REQUIRE(spriteFifo.fetchingSprite);

                bgPixel = bgFifo.cycle();
                REQUIRE(bgPixel == nullptr);

                spriteFifo.checkForSprite();
            }

            // Draw sprite and bg
            for (uint i = 0; i < 8; ++i) {
                spriteFifo.checkForSprite();

                spritePixel = spriteFifo.cycle();
                REQUIRE(spritePixel != nullptr);
                REQUIRE(spritePixel->color == 1);

                bgPixel = bgFifo.cycle();
                REQUIRE(bgPixel != nullptr);
                REQUIRE(bgPixel->color == 2);

                ++ppu.xPos;
                spriteFifo.fetcherXPos = ppu.xPos;

                spriteFifo.checkForSprite();
            }
        }

        SECTION("Fetch abort")
        {
            // TODO
            // nu cred ca se face abort pe cgb?
        }

        SECTION("Sprites Disabled")
        {
            ppu.setObjDisplayEnable(0);

            OAMSprite oamSprite;

            // Sprite 0, Tile 2
            oamSprite.yPos = 16;
            oamSprite.xPos = 8;
            oamSprite.dmgPaletteNumber = 1;
            oamSprite.tileNumber = 2;
            oamSprite.tileVramBank = 1;
            oamSprite.objToBgPriority = 0;
            oamSprite.xFlip = 0;
            oamSprite.yFlip = 0;
            oamSprite.writeAsBytes(mem.oam);

            bgFifo.prepareForLine(ppu.getLy());
            spriteFifo.prepareForLine();

            FifoPixel *bgPixel, *spritePixel;

            // PPU OAM Search
            ppu.setModeFlag(OAM_SEARCH);
            for (uint i = 0; i < PPU_OAM_SEARCH_T_CYCLES; ++i) {
                REQUIRE(ppu.getLcdMode() == OAM_SEARCH);
                ppu.cycle();
            }
            REQUIRE(ppu.getLcdMode() == DRAW);

            // Check found sprites on line
            REQUIRE(ppu.numSpritesOnCurrentLine == 1);
            REQUIRE(ppu.spritesOnCurrentLine[0] == 0);
            REQUIRE(ppu.spritesOnCurrentLine[1] == -1);

            // Fetch bg and sprite tile
            for (uint i = 0; i < 13; ++i) {
                spriteFifo.checkForSprite();

                REQUIRE(spriteFifo.fetchingSprite);
                spritePixel = spriteFifo.cycle();
                REQUIRE(spritePixel == nullptr);

                bgPixel = bgFifo.cycle();
                REQUIRE(bgPixel == nullptr);

                spriteFifo.checkForSprite();
            }

            // Draw bg
            for (uint i = 0; i < 8; ++i) {               
                spriteFifo.checkForSprite();

                spritePixel = spriteFifo.cycle();
                REQUIRE(spritePixel == nullptr);
                if (i < 7)
                    REQUIRE_FALSE(spriteFifo.pixelQueue.empty());

                bgPixel = bgFifo.cycle();
                REQUIRE(bgPixel != nullptr);
                REQUIRE(bgPixel->color == 2);

                ++ppu.xPos;
                spriteFifo.fetcherXPos = ppu.xPos;

                spriteFifo.checkForSprite();
            }
        }
    }
}
