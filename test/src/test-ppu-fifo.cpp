#include "catch.hpp"

#include "Memory.hpp"
#include "PPU.hpp"
#include "SM83.hpp"
#include <cstdlib>
#include <iostream>

TEST_CASE("BG Fifo", "[PPU_FIFO]")
{
    // TODO

    /**
     * cases:
     *      draw bg
     *      draw window
     *      check timings
     *      cgb: check flipped tiles and stuff
     *      bg tiles with scroll
     */

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

            // ar trebui sa mut astea cu 3 mia la dreapta si 3 mai jos
            // sa le pun la capat ca sa vad ca face wrap around
            // sa setez scy sa fie 1 si scx sa fie 0x8000 + 254
            // scy cred ca e ok 2 sau 3, scx sa il pun la marginea eranului
            // la 255 cred se vede doar 1 pixel, deci
            // tile 1 e la pos 0, 2 si 3 sunt la 254, 255
            // pe o linie sunt 32 tile-uri: 0..31
            // ca sa se vada jumate din tile 2 trebuie scx = 256 - 12 =

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
            // cred ca ar fi 2 cazuri
            // 1 cand nu se scurteaza draw si unu in care se scurteaza
            // sa zicem ca am tile-uri cu scroll
            // si dupa incepe window
            // sa vad ca ii ia timp ok sa se reseteze

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
            // have the window be in vram0 and vram1

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
            // window: 1 tile in vram bank 0, 1 tile in vram bank 1, 1 flipped hor, 1 flipped vert

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

TEST_CASE("Sprite Fifo", "[PPU FIFO]")
{
    // TODO
}
