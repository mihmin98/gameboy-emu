#include "PPU.hpp"
#include "GameBoy.hpp"
#include "Memory.hpp"
#include "SM83.hpp"

PPU::PPU()
{
    bgFifo.ppu = this;
    spriteFifo.ppu = this;
    spriteFifo.bgFifo = &bgFifo;

    readyToDraw = false;
    xPos = 0;
    windowYCounter = 0;
    oamDmaActive = false;
    vramGeneralDmaActive = false;
    vramHblankDmaActive = false;
    currentModeTCycles = 0;
    hBlankModeLength = PPU_DEFAULT_HBLANK_T_CYCLES;
    drawModeLength = PPU_DEFAULT_DRAW_T_CYCLES;
}

/* GETTERS AND SETTERS */

/* LCD CONTROL REGISTER */

uint8_t PPU::getLcdDisplayEnable() { return (memory->readmem(0xFF40, true) & 0x80) >> 7; }

uint8_t PPU::getWindowTileMapDisplaySelect() { return (memory->readmem(0xFF40, true) & 0x40) >> 6; }

uint8_t PPU::getWindowDisplayEnable() { return (memory->readmem(0xFF40, true) & 0x20) >> 5; }

uint8_t PPU::getBgAndWindowTileDataSelect() { return (memory->readmem(0xFF40, true) & 0x10) >> 4; }

uint8_t PPU::getBgTileMapDisplaySelect() { return (memory->readmem(0xFF40, true) & 0x8) >> 3; }

uint8_t PPU::getObjSize() { return (memory->readmem(0xFF40, true) & 0x4) >> 2; }

uint8_t PPU::getObjDisplayEnable() { return (memory->readmem(0xFF40, true) & 0x2) >> 1; }

uint8_t PPU::getBgWindowDisplayPriority() { return memory->readmem(0xFF40, true) & 0x1; }

void PPU::setLcdControlRegister(uint8_t val) { memory->writemem(val, 0xFF40, true); }

void PPU::setLcdDisplayEnable(uint8_t val) { memory->writebit(val, 7, 0xFF40, true); }

void PPU::setWindowTileMapDisplaySelect(uint8_t val) { memory->writebit(val, 6, 0xFF40, true); }

void PPU::setWindowDisplayEnable(uint8_t val) { memory->writebit(val, 5, 0xFF40, true); }

void PPU::setBgAndWindowTileDataSelect(uint8_t val) { memory->writebit(val, 4, 0xFF40, true); }

void PPU::setBgTileMapDisplaySelect(uint8_t val) { memory->writebit(val, 3, 0xFF40, true); }

void PPU::setObjSize(uint8_t val) { memory->writebit(val, 2, 0xFF40, true); }

void PPU::setObjDisplayEnable(uint8_t val) { memory->writebit(val, 1, 0xFF40, true); }

void PPU::setBgWindowDisplayPriority(uint8_t val) { memory->writebit(val, 0, 0xFF40, true); }

/* LCD STATUS REGISTER */

uint8_t PPU::getLycLyCoincidence() { return (memory->readmem(0xFF41, true) & 0x40) >> 6; }

uint8_t PPU::getMode2OamInterrupt() { return (memory->readmem(0xFF41, true) & 0x20) >> 5; }

uint8_t PPU::getMode1VBlankInterrupt() { return (memory->readmem(0xFF41, true) & 0x10) >> 4; }

uint8_t PPU::getMode0HBlankInterrupt() { return (memory->readmem(0xFF41, true) & 0x8) >> 3; }

uint8_t PPU::getCoincidenceFlag() { return (memory->readmem(0xFF41, true) & 0x4) >> 2; }

uint8_t PPU::getModeFlag() { return memory->readmem(0xFF41, true) & 0x3; }

LcdMode PPU::getLcdMode() { return (LcdMode)getModeFlag(); }

void PPU::setLcdStatRegister(uint8_t val) { memory->writemem(val, 0xFF41, true); }

void PPU::setLycLyCoincidence(uint8_t val) { memory->writebit(val, 6, 0xFF41, true); }

void PPU::setMode2OamInterrupt(uint8_t val) { memory->writebit(val, 5, 0xFF41, true); }

void PPU::setMode1VBlankInterrupt(uint8_t val) { memory->writebit(val, 4, 0xFF41, true); }

void PPU::setMode0HBlankInterrupt(uint8_t val) { memory->writebit(val, 3, 0xFF41, true); }

void PPU::setCoincidenceFlag(uint8_t val) { memory->writebit(val, 2, 0xFF41, true); }

void PPU::setModeFlag(uint8_t val)
{
    if (val > 3)
        return;

    uint8_t byte = memory->readmem(0xFF41, true) & 0xFC;
    byte |= val;
    memory->writemem(byte, 0xFF41, true);
}

/* LCD POSITION AND SCROLLING */

uint8_t PPU::getScrollY() { return memory->readmem(0xFF42, true); }

uint8_t PPU::getScrollX() { return memory->readmem(0xFF43, true); }

uint8_t PPU::getLy() { return memory->readmem(0xFF44, true); }

uint8_t PPU::getLyc() { return memory->readmem(0xFF45, true); }

uint8_t PPU::getWy() { return memory->readmem(0xFF4A, true); }

uint8_t PPU::getWx() { return memory->readmem(0xFF4B, true); }

void PPU::setScrollY(uint8_t val) { memory->writemem(val, 0xFF42, true); }

void PPU::setScrollX(uint8_t val) { memory->writemem(val, 0xFF43, true); }

void PPU::setLy(uint8_t val) { memory->writemem(val, 0xFF44, true); }

void PPU::setLyc(uint8_t val) { memory->writemem(val, 0xFF45, true); }

void PPU::setWy(uint8_t val) { memory->writemem(val, 0xFF4A, true); }

void PPU::setWx(uint8_t val) { memory->writemem(val, 0xFF4B, true); }

/* LCD MONOCHROME PALETTES */

uint8_t PPU::getBgPaletteData() { return memory->readmem(0xFF47, true); }

uint8_t PPU::getObjPalette0Data() { return memory->readmem(0xFF48, true); }

uint8_t PPU::getObjPalette1Data() { return memory->readmem(0xFF49, true); }

void PPU::setBgPaletteData(uint8_t val) { memory->writemem(val, 0xFF47, true); }

void PPU::setObjPalette0Data(uint8_t val) { memory->writemem(val, 0xFF48, true); }

void PPU::setObjPalette1Data(uint8_t val) { memory->writemem(val, 0xFF49, true); }

/* LCD COLOR PALETTES */

uint8_t PPU::getBgColorPaletteIndex() { return memory->readmem(0xFF68, true); }

uint8_t PPU::getBgColorPaletteData() { return memory->readmem(0xFF69, true); }

uint8_t PPU::getObjColorPaletteIndex() { return memory->readmem(0xFF6A, true); }

uint8_t PPU::getObjColorPaletteData() { return memory->readmem(0xFF6B, true); }

void PPU::setBgColorPaletteIndex(uint8_t val) { memory->writemem(val, 0xFF68, true); }

void PPU::setBgColorPaletteData(uint8_t val) { memory->writemem(val, 0xFF69, true); }

void PPU::setObjColorPaletteIndex(uint8_t val) { memory->writemem(val, 0xFF6A, true); }

void PPU::setObjColorPaletteData(uint8_t val) { memory->writemem(val, 0xFF6B, true); }

/* OAM DMA */

uint8_t PPU::getOamDma() { return memory->readmem(0xFF46, true); }

void PPU::setOamDma(uint8_t val) { memory->writemem(val, 0xFF46, true); }

/* VRAM DMA */

uint8_t PPU::getHdma1() { return memory->readmem(0xFF51, true); }

uint8_t PPU::getHdma2() { return memory->readmem(0xFF52, true); }

uint16_t PPU::getHdmaSrcAddress()
{
    uint16_t srcAddr = (getHdma1() << 8) | getHdma2();
    return (srcAddr & 0xFFF0);
}

uint8_t PPU::getHdma3() { return memory->readmem(0xFF53, true); }

uint8_t PPU::getHdma4() { return memory->readmem(0xFF54, true); }

uint16_t PPU::getHdmaDestAddress()
{
    uint16_t destAddr = (getHdma3() << 8) | getHdma4();
    return 0x8000 + (destAddr & 0x1FF0);
}

uint8_t PPU::getHdma5() { return memory->readmem(0xFF55, true); }

uint8_t PPU::getHdmaMode() { return memory->readmem(0xFF55, true) >> 7; }

uint8_t PPU::getHdmaLength() { return memory->readmem(0xFF55, true) & 0x7F; }

void PPU::setHdma1(uint8_t val) { memory->writemem(val, 0xFF51, true); }

void PPU::setHdma2(uint8_t val) { memory->writemem(val, 0xFF52, true); }

void PPU::setHdma3(uint8_t val) { memory->writemem(val, 0xFF53, true); }

void PPU::setHdma4(uint8_t val) { memory->writemem(val, 0xFF54, true); }

void PPU::setHdma5(uint8_t val) { memory->writemem(val, 0xFF55, true); }

Tile PPU::getTileByIndex(int index)
{
    uint16_t tileAddr;
    if (index >= 128 && index < 256) {
        // block 2
        tileAddr = 0x8800 + (index - 128) * 16;
    } else {
        if (getBgAndWindowTileDataSelect()) {
            // block 1
            tileAddr = 0x8000 + index * 16;
        } else {
            // block 3
            tileAddr = 0x9000 + index * 16;
        }
    }

    uint8_t tileBytes[16];
    for (int i = 0; i < 16; ++i) {
        tileBytes[i] = memory->readmem(tileAddr + i, true);
    }

    return Tile(tileBytes);
}

OAMSprite PPU::getSpriteByIndex(int index)
{
    uint8_t sprite[4];
    for (int i = 0; i < 4; ++i)
        sprite[i] = memory->readmem(MEM_OAM_START + index * 4 + i, true);

    OAMSprite oamSprite = OAMSprite(sprite);
    oamSprite.spriteIndex = index;
    return oamSprite;
}

BgMapAttributes PPU::getBgMapByIndex(int index, int tilemap)
{
    if (emulatorMode != EmulatorMode::CGB) {
        fprintf(stderr, "WARNING: Trying to read background map attributes in non-CGB mode\n");
        return BgMapAttributes();
    }

    // switch to vram bank 1
    uint8_t oldVramBank = memory->getCurrentVramBank();
    memory->setCurrentVramBank(1);

    uint16_t tilemapAttrAddr = (tilemap == 0 ? 0x9800 : 0x9C00) + index;
    uint8_t tilemapAttrByte = memory->readmem(tilemapAttrAddr, true);
    BgMapAttributes bgMapAttr = BgMapAttributes(tilemapAttrByte);

    // switch back to original vram bank
    memory->setCurrentVramBank(oldVramBank);

    return bgMapAttr;
}

void PPU::searchSpritesOnLine()
{
    // In 8x8 mode sprite is from yPos-16 to yPos-8, and in 8x16 form yPos-16 to yPos
    uint8_t spriteHeightDiff = getObjSize() == 0 ? 8 : 0;
    uint8_t line = getLy();

    // empty the found sprites array
    for (int i = 0; i < 10; ++i)
        spritesOnCurrentLine[i] = -1;

    numSpritesOnCurrentLine = 0;

    if (emulatorMode == EmulatorMode::DMG) {
        std::vector<OAMSprite> spritesOnCurrentLineVec;
        for (uint i = 0; i < PPU_NUM_SPRITES; ++i) {
            OAMSprite sprite = getSpriteByIndex(i);
            if (line >= sprite.yPos - 16 && line < sprite.yPos - spriteHeightDiff)
                spritesOnCurrentLineVec.push_back(sprite);
        }

        // Sort sprites by their xpos and index
        std::sort(spritesOnCurrentLineVec.begin(), spritesOnCurrentLineVec.end(),
                  [](const OAMSprite &a, const OAMSprite &b) {
                      if (a.xPos == b.xPos) {
                          return a.spriteIndex > b.spriteIndex;
                      } else {
                          return a.xPos < b.xPos;
                      }
                  });

        // Add sprites to spritesOnCurrentLine
        for (auto it = spritesOnCurrentLineVec.begin();
             it != spritesOnCurrentLineVec.end() &&
             numSpritesOnCurrentLine < PPU_MAX_SPRITES_ON_LINE;
             std::advance(it, 1)) {

            spritesOnCurrentLine[numSpritesOnCurrentLine++] = it->spriteIndex;
        }
    }

    else if (emulatorMode == EmulatorMode::CGB) {
        for (int i = 0; i < PPU_NUM_SPRITES && numSpritesOnCurrentLine < PPU_MAX_SPRITES_ON_LINE;
             ++i) {
            OAMSprite sprite = getSpriteByIndex(i);
            // TODO: Check if condition should be line <, or line <=
            if (line >= sprite.yPos - 16 && line < sprite.yPos - spriteHeightDiff)
                spritesOnCurrentLine[numSpritesOnCurrentLine++] = i;
        }
    }
}

Tile PPU::getSpriteTile(int index, int tileNo, int vramBank)
{
    uint16_t tileAddr;

    // start addr is 0x8000-0x8FFF
    if (getObjSize() == 0)
        // 8x8
        tileAddr = 0x8000 + index * 16;
    else
        // 8x16
        tileAddr = 0x8000 + (index & 0xFE) * 16 + tileNo * 16;

    uint8_t oldVramBank = memory->getCurrentVramBank();
    memory->setCurrentVramBank(vramBank);

    uint8_t tileBytes[16];
    for (int i = 0; i < 16; ++i)
        tileBytes[i] = memory->readmem(tileAddr + i, true);

    memory->setCurrentVramBank(oldVramBank);

    return Tile(tileBytes);
}

Color PPU::getColorFromFifoPixel(FifoPixel *fifoPixel, bool normalizeCgbColor)
{
    if (emulatorMode == EmulatorMode::DMG) {
        // DMG Mode
        if (!fifoPixel->isSprite) {
            // BG/Window Pixel
            uint8_t palette = getBgPaletteData();
            uint8_t color = (palette & (3 << (fifoPixel->color * 2))) >> (fifoPixel->color * 2);
            return Color::getDmgColor(color);
        } else {
            // Sprite Pixel
            uint8_t palette = fifoPixel->palette == 0 ? getObjPalette0Data() : getObjPalette1Data();
            uint8_t color;
            if (fifoPixel->color == 0)
                // Transparent pixel; lowest bits from palette are ignored
                color = 0;
            else
                color = (palette & (3 << (fifoPixel->color * 2))) >> (fifoPixel->color * 2);

            return Color::getDmgColor(color);
        }
    } else {
        // CGB Mode
        if (!fifoPixel->isSprite) {
            // BG/Window Pixel
            uint8_t *colorPaletteAddr =
                memory->cgbBgColorPalette + fifoPixel->palette * 8 + fifoPixel->color * 2;
            Color c = Color(colorPaletteAddr);

            if (normalizeCgbColor)
                return c.getNormalizedColor();
            else
                return c;
        } else {
            // Sprite Pixel
            if (fifoPixel->color == 0)
                // Transparent pixel; lowest bits from palette are ignored
                return Color(255, 255, 255);

            uint8_t *colorPaletteAddr =
                memory->cgbObjColorPalette + fifoPixel->palette * 8 + fifoPixel->color * 2;
            Color c = Color(colorPaletteAddr);

            if (normalizeCgbColor)
                return c.getNormalizedColor();
            else
                return c;
        }
    }
}

void PPU::cycle()
{
    LcdMode currentMode = getLcdMode();
    FifoPixel *bgPixel = nullptr;
    FifoPixel *spritePixel = nullptr;
    Color *colorPixel = nullptr;

    switch (currentMode) {
    case OAM_SEARCH:
        if (currentModeTCycles == 0) {
            // Set window trigger on current line
            windowXTrigger = false;
            windowYTrigger = getLy() >= getWy();

            // check ly==lyc
            if (getLy() == getLyc()) {
                setCoincidenceFlag(1);
                if (getLycLyCoincidence())
                    cpu->setLCDSTATInterruptFlag(1);
            } else {
                setCoincidenceFlag(0);
            }

            // check oam interrupt
            if (getMode2OamInterrupt())
                cpu->setLCDSTATInterruptFlag(1);
        }

        ++currentModeTCycles;

        // check for transition to next mode
        if (currentModeTCycles == PPU_OAM_SEARCH_T_CYCLES) {
            searchSpritesOnLine();
            setModeFlag(DRAW);
            xPos = 0;
            currentModeTCycles = 0;
        }

        break;

    case DRAW:
        // Note: Mode 3 can be lengthened by 117 t cycles
        // 1 sprite lengthens mode 3 by 11 cycles, and the SCX penalty lengthens mode 3 by a maximum
        // of 7 t cycles

        if (currentModeTCycles == 0) {
            bgFifo.prepareForLine(getLy());
            spriteFifo.prepareForLine();
            xPos = 0;
        }
        /**
         * BG FIFO: cycle() returneaza pixel sau null
         * Sprite FIFO: cycle() returneaza pixel sau null (ar trebui sa modifice el singur durata la
         * DRAW)
         *
         * pt fiecare ar trebui sa dau prepare for line la inceputul modului
         *
         * sa vad cazurile in care display e disabled
         *
         * trebuie sa fac si mixingul de la pixeli
         */

        ++currentModeTCycles;

        spriteFifo.checkForSprite();

        spritePixel = spriteFifo.cycle();
        bgPixel = bgFifo.cycle();

        colorPixel = mixPixels(bgPixel, spritePixel);

        if (colorPixel != nullptr) {
            display[getLy()][xPos++] = *colorPixel;

            spriteFifo.fetcherXPos = xPos;

            if (xPos + 7 >= getWx()) {
                windowXTrigger = true;
            }
        }

        // put pixels on screen?
        // trebuie sa vad care fifo asteapta pe care, sa fac sleepurile alea
        // sleep-uri: spriteFifo oamPenalty?
        // during penalty the ppu should do nothing?, except of dma?
        // TODO: check if sprites or background is enabled

        // BG is always enabled

        // Mixing: Check priorities

        if (bgPixel != nullptr) {
            delete bgPixel;
            bgPixel = nullptr;
        }

        if (spritePixel != nullptr) {
            delete spritePixel;
            spritePixel = nullptr;
        }

        if (colorPixel != nullptr) {
            delete colorPixel;
            colorPixel = nullptr;
        }

        // check for transition to next mode
        if (xPos >= PPU_SCREEN_WIDTH) {
            // TODO: Set hblank cycles based on how many cycles have been in draw mode???
            if (bgFifo.isDrawingWindow) {
                ++windowYCounter;
            }

            setModeFlag(H_BLANK);
            hBlankModeLength = PPU_LINE_T_CYCLES - PPU_OAM_SEARCH_T_CYCLES - currentModeTCycles;
            currentModeTCycles = 0;
        }

        break;

    case H_BLANK:
        if (currentModeTCycles == 0) {
            // Check HBlank interrupt
            if (getMode0HBlankInterrupt())
                cpu->setLCDSTATInterruptFlag(1);
        }

        ++currentModeTCycles;

        // Check for transition to next mode
        if (currentModeTCycles >= hBlankModeLength) {
            // HBlank DMA
            if (vramHblankDmaActive) {
                // Transfers 0x10 bytes per hblank
                uint16_t srcAddr = getHdmaSrcAddress() + vramDmaTransferredBytes;
                uint16_t destAddr = getHdmaDestAddress() + vramDmaTransferredBytes;

                for (uint8_t i = 0; i < 16; ++i) {
                    uint8_t val = memory->readmem(srcAddr + i, true, true);
                    memory->writemem(val, destAddr + i, true, true);
                }

                // Update 0xFF55 remaining blocks
                uint8_t vramDmaRemainingLength = memory->readmem(0xFF55, true, true);
                --vramDmaRemainingLength;
                memory->ioRegisters[0xFF55 - MEM_IO_START] = vramDmaRemainingLength;

                vramDmaTransferredBytes += 16;

                // Check if transfer has ended; Set 0xFF55 to 0xFF
                if ((vramDmaTransferredBytes / 16) - 1 >= vramDmaLength) {
                    vramHblankDmaActive = false;
                    vramDmaTransferredBytes = 0;
                    memory->ioRegisters[0xFF55 - MEM_IO_START] = 0xFF;
                }
            }

            // Increase Line
            if (!lcdWasShutDown) {
                setLy(getLy() + 1);
            } else {
                lcdWasShutDown = false;
            }

            currentModeTCycles = 0;
            
            if (getLy() == 144) {
                // Go to VBlank
                setModeFlag((uint8_t)V_BLANK);
            } else {
                // Go to OAM Search
                setModeFlag((uint8_t)OAM_SEARCH);
            }
        }

        break;

    case V_BLANK:
        if (currentModeTCycles == 0) {
            // trigger vblank interrupt
            cpu->setVBlankInterruptFlag(1);
            if (getMode1VBlankInterrupt())
                cpu->setLCDSTATInterruptFlag(1);
            // Set that frame is ready to be drawn
            readyToDraw = true;
            ++renderedFrames;
        }

        if (currentModeTCycles == 0 || currentModeTCycles % PPU_LINE_T_CYCLES == 0) {
            // check ly==lyc
            if (getLy() == getLyc()) {
                setCoincidenceFlag(1);
                if (getLycLyCoincidence())
                    cpu->setLCDSTATInterruptFlag(1);
            } else {
                setCoincidenceFlag(0);
            }
        }

        ++currentModeTCycles;

        if (currentModeTCycles != 0 && currentModeTCycles % PPU_LINE_T_CYCLES == 0) {
            if (currentModeTCycles == PPU_VBLANK_T_CYCLES) {
                // Set LY to 0
                setLy(0);
                windowYCounter = 0;

                // Change mode
                setModeFlag(OAM_SEARCH);
                currentModeTCycles = 0;

                // Reset HBlank and Draw duration
                drawModeLength = PPU_DEFAULT_DRAW_T_CYCLES;
                hBlankModeLength = PPU_DEFAULT_HBLANK_T_CYCLES;
            } else {
                // increase ly
                setLy(getLy() + 1);
                if (bgFifo.isDrawingWindow && getWindowDisplayEnable())
                    ++windowYCounter;
            }
        }

        // check for transition to next mode
        // when reaching end of final line final line, or 4560 t cycles in vblank
        // already did it??
        break;
    }

    ++tCycles;
    // oare dupa switch ar trebui sa verific alea de dma?

    // OAM DMA
    if (oamDmaActive) {
        ++oamDmaCurrentCycles;
        if (oamDmaCurrentCycles == PPU_OAM_DMA_T_CYCLES) {
            // Copy the data from src to dest
            uint16_t srcAddr = memory->readmem(0xFF46, true, true) << 8;
            for (uint8_t i = 0; i <= 0x9F; ++i) {
                uint8_t val = memory->readmem(srcAddr + i, true, true);
                memory->writemem(val, MEM_OAM_START + i, true, true);
            }

            // Cleanup
            oamDmaActive = false;
            oamDmaCurrentCycles = 0;
        }
    }

    // General VRAM DMA
    if (vramGeneralDmaActive) {
        // It takes 8 M-cycles in nomral speed mode and 16 M-cycles in double speed
        // 32 T-cycles in normal, or 64 T-cycles in double speed
        // to transfer 0x10 bytes
        // inc by 2 in normal speed mode, and by 1 in double speed mode
        // does not modify 0xFF55 because all data is transferred at once and the program is halted
        if (doubleSpeedMode)
            ++vramDmaCurrentCycles;
        else
            vramDmaCurrentCycles += 2;

        if (vramDmaCurrentCycles == PPU_VRAM_DMA_BLOCK_TRANSFER_DOUBLE_SPEED_T_CYCLES) {
            // Transfer 0x10 bytes
            uint16_t srcAddr = getHdmaSrcAddress() + vramDmaTransferredBytes;
            uint16_t destAddr = getHdmaDestAddress() + vramDmaTransferredBytes;
            for (uint8_t i = 0; i < 16; ++i) {
                uint8_t val = memory->readmem(srcAddr + i, true, true);
                memory->writemem(val, destAddr + i, true, true);
            }

            vramDmaCurrentCycles = 0;
            vramDmaTransferredBytes += 16;

            // Check if transfer has ended
            if ((vramDmaTransferredBytes / 16) - 1 >= vramDmaLength) {
                vramGeneralDmaActive = false;
                vramDmaTransferredBytes = 0;
                memory->ioRegisters[0xFF55 - MEM_IO_START] = 0xFF;
            }
        }
    }

}

Color *PPU::mixPixels(FifoPixel *bgPixel, FifoPixel *spritePixel)
{
    // If there is no bg pixel, return null by default
    if (bgPixel == nullptr) {
        return nullptr;
    }

    // std::cout << "bg pixel color: " << (uint)bgPixel->color << "\n";

    // TODO: Do I need to check if sprites are enabled?
    if (spritePixel == nullptr || !getObjDisplayEnable()) {
        if (emulatorMode == EmulatorMode::DMG && getBgWindowDisplayPriority() == 0) {
            // Only sprites can be displayed, bg and window become blank (white)
            return new Color(255, 255, 255);
        } else {
            return new Color(getColorFromFifoPixel(bgPixel));
        }
    }

    // If there is a bg pixel and a sprite pixel
    if (emulatorMode == EmulatorMode::DMG) {
        // Transparent sprite pixel
        if (spritePixel->color == 0) {
            if (getBgWindowDisplayPriority() == 0) {
                return new Color(255, 255, 255);
            } else {
                return new Color(getColorFromFifoPixel(bgPixel));
            }
        }

        // LCDC.0 = 0, only sprites can be displayed, bg and window become blank
        if (getBgWindowDisplayPriority() == 0) {
            return new Color(getColorFromFifoPixel(spritePixel));
        }

        if (spritePixel->spriteBgAndWindowOverObjPriority == 0) {
            return new Color(getColorFromFifoPixel(spritePixel));
        }

        else if (spritePixel->spriteBgAndWindowOverObjPriority == 1) {
            if (bgPixel->color == 0) {
                return new Color(getColorFromFifoPixel(spritePixel));
            } else {
                return new Color(getColorFromFifoPixel(bgPixel));
            }
        }
    }

    else if (emulatorMode == EmulatorMode::CGB) {
        // priority order:
        // LCDC.0 > bg map attr > oam priority

        // Transparent Sprite Pixel
        if (spritePixel->color == 0) {
            return new Color(getColorFromFifoPixel(bgPixel));
        }

        // LCDC.0 = 0
        if (getBgWindowDisplayPriority() == 0) {
            return new Color(getColorFromFifoPixel(spritePixel));
        }

        // Bg Map Attr priority
        if (bgPixel->bgPriority == 1) {
            return new Color(getColorFromFifoPixel(bgPixel));
        }

        if (spritePixel->spriteBgAndWindowOverObjPriority == 0) {
            return new Color(getColorFromFifoPixel(spritePixel));
        }

        else if (spritePixel->spriteBgAndWindowOverObjPriority == 1) {
            if (bgPixel->color == 0) {
                return new Color(getColorFromFifoPixel(spritePixel));
            } else {
                return new Color(getColorFromFifoPixel(bgPixel));
            }
        }
    }

    return nullptr;
}

// DONE
// TODO: in timpul la oam dma in modurile 2, 3, ppu citeste 0xff
// cum fac chestia asta? mai bag un flag la readmem? sau nu implementez? (ar fi cam bad sa nu
// imnplemetnez)
// cred ca o sa mai bag un flag, bypassOamPpu sau cv de genu, care ar trebui sa fie true by
// default? sau false? exista cazul in care cpu sau alta componenta sa aib si in getterele si
// setterele din ppu le dau cu false deja am chestia asta oarecum accesul
