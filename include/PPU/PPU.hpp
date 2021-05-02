#ifndef __PPU_H__
#define __PPU_H__

#pragma once
#include "BgFifo.hpp"
#include "BgMapAttributes.hpp"
#include "Color.hpp"
#include "GameBoy.hpp"
#include "Memory.hpp"
#include "OAMSprite.hpp"
#include "SM83.hpp"
#include "SpriteFifo.hpp"
#include "Tile.hpp"
#include <cstdio>

#define PPU_NUM_SPRITES 40
#define PPU_MAX_SPRITES_ON_LINE 10
#define PPU_OAM_SEARCH_T_CYCLES 80
#define PPU_LINE_T_CYCLES 456
#define PPU_VBLANK_T_CYCLES 4560
#define PPU_OAM_DMA_T_CYCLES 640
#define PPU_DEFAULT_DRAW_T_CYCLES 172
#define PPU_DEFAULT_HBLANK_T_CYCLES 204
#define PPU_VRAM_DMA_BLOCK_TRANSFER_DOUBLE_SPEED_T_CYCLES 64

class Memory;
class SM83;

enum LcdMode { H_BLANK = 0, V_BLANK = 1, OAM_SEARCH = 2, DRAW = 3 };

class PPU
{
  public:
    Memory *memory;
    SM83 *cpu;

    EmulatorMode emulatorMode;
    bool doubleSpeedMode;

    uint8_t drawModeLength;   // should be set to 172 when entering mode 3
    uint8_t hBlankModeLength; // this should be set to 204 when entering mode 3 and modified based
                              // on what the bg and pixel fifo are doing

    int8_t spritesOnCurrentLine[PPU_MAX_SPRITES_ON_LINE];
    uint8_t numSpritesOnCurrentLine;

    BgFifo bgFifo;
    SpriteFifo spriteFifo;

    bool oamDmaActive;
    uint16_t oamDmaCurrentCycles;

    bool vramGeneralDmaActive;
    bool vramHblankDmaActive;
    uint32_t vramDmaLength;
    uint32_t vramDmaTransferredBytes;
    uint32_t vramDmaCurrentCycles;

    uint32_t tCycles;
    uint32_t currentModeTCycles;
    uint16_t currentOamDmaTCycles;

    uint8_t xPos; // X position on the current line

    PPU();

    /**
     * LCD CONTROL REGISTER
     *
     * The LCD Control Register is located at 0xFF40
     *
     * Bit 7 - LCD Display Enable: Specifies if the LCD is on and the PPU active
     * Bit 6 - Window Tile Map Display Select: Controls which background map is used by the window
     *         0 = 0x9800; 1 = 0x9C00
     * Bit 5 - Window Display Enable: Controls whether the window is displayed or not
     * Bit 4 - BG & Window Tile Data Select: Controls which addressing mode the BG and Window use
     *         to pick tiles
     * Bit 3 - BG Tile Map Display Select: Similar to bit 6; 0 = 0x9800; 1 = 0x9C00
     * Bit 2 - OBJ (Sprite) Size: 0 = 8x8; 1 = 8x16
     * Bit 1 - OBJ (Sprite) Display Enable: Toggles if sprites are displayed
     * Bit 0 - BG/Window Display/Priority:
     *         - DMG Mode: BG Display: If 0 then both background and window become blank, only
     *                     sprites may be displayed
     *         - CGB Mode: BG & Window Master Priority: If 0, the background and window lose
     *                                                  priority; ths sprites will be displayed on
     *                                                  top independently of the priority flags
     */

    uint8_t getLcdDisplayEnable();
    uint8_t getWindowTileMapDisplaySelect();
    uint8_t getWindowDisplayEnable();
    uint8_t getBgAndWindowTileDataSelect();
    uint8_t getBgTileMapDisplaySelect();
    uint8_t getObjSize();
    uint8_t getObjDisplayEnable();
    uint8_t getBgWindowDisplayPriority();
    void setLcdControlRegister(uint8_t val);
    void setLcdDisplayEnable(uint8_t val);
    void setWindowTileMapDisplaySelect(uint8_t val);
    void setWindowDisplayEnable(uint8_t val);
    void setBgAndWindowTileDataSelect(uint8_t val);
    void setBgTileMapDisplaySelect(uint8_t val);
    void setObjSize(uint8_t val);
    void setObjDisplayEnable(uint8_t val);
    void setBgWindowDisplayPriority(uint8_t val);

    /**
     * LCD STATUS REGISTER
     *
     * LCD STAT is located at 0xFF41
     *
     * Bit 6 - LYC=LY Coincidence Interrupt: Triggers LCD_STAT interrupt when LYC = LY
     * Bit 5 - Mode 2 OAM Interrupt
     * Bit 4 - Mode 1 VBlank Interrupt
     * Bit 3 - Mode 0 HBlank Interrupt
     * Bit 2 - Coincidence Flag: 0 = LYC != LY; 1 = LYC == LY
     * Bit 1-0 - Mode Flag: 0) During HBlank
     *                      1) During VBlank
     *                      2) During Searching OAM
     *                      3) During Transferring Data to LCD Driver (Pixel Transfer)
     */

    uint8_t getLycLyCoincidence();
    uint8_t getMode2OamInterrupt();
    uint8_t getMode1VBlankInterrupt();
    uint8_t getMode0HBlankInterrupt();
    uint8_t getCoincidenceFlag();
    uint8_t getModeFlag();
    LcdMode getLcdMode();
    void setLcdStatRegister(uint8_t val);
    void setLycLyCoincidence(uint8_t val);
    void setMode2OamInterrupt(uint8_t val);
    void setMode1VBlankInterrupt(uint8_t val);
    void setMode0HBlankInterrupt(uint8_t val);
    void setCoincidenceFlag(uint8_t val);
    void setModeFlag(uint8_t val);

    /**
     * LCD POSITION AND SCROLLING
     *
     * SCY: Scroll Y (0xFF42) / SCX: Scroll X (0xFF43) - Position to be displayed in the top-left
     *                                                   corner of the screen; Wrapping occurs
     *                                                   when drawing exceeds the lower right
     *                                                   border of the map area
     * LY: LCDC Y-Coordinate (0xFF44) - Current vertical line
     * LYC: LY Compare (0xFF45) - The value to which LY is compared
     * WY: Window Y Pos (0xFF4A) / WX: Window X Pos (0xFF4B) - Top-left position of the window
     *                                                         area; WX is specified as WX - 7;
     *                                                         WY=0, WX=7 is the top left corner
     */

    uint8_t getScrollY();
    uint8_t getScrollX();
    uint8_t getLy();
    uint8_t getLyc();
    uint8_t getWy();
    uint8_t getWx();
    void setScrollY(uint8_t val);
    void setScrollX(uint8_t val);
    void setLy(uint8_t val);
    void setLyc(uint8_t val);
    void setWy(uint8_t val);
    void setWx(uint8_t val);

    /**
     * LCD MONOCHROME PALETTES (NON CGB ONLY)
     *
     * BGP: BG Palette Data (0xFF47) - Gray shades assigned to the color numbers of the BG & Widnow
     *                                 tiles;
     *                                 Bit 7-6: Color 3
     *                                 Bit 5-4: Color 2
     *                                 Bit 3-2: Color 1
     *                                 Bit 1-0: Color 0
     * OBP0: Object Palette 0 Data (0xFF48) - It works similar to BGP, but the lower 2 bits aren't
     *                                        used; they represent transparency
     * OBP1: Object Palette 1 Data (0xFF49) - Same as OBP0
     */

    uint8_t getBgPaletteData();
    uint8_t getObjPalette0Data();
    uint8_t getObjPalette1Data();
    void setBgPaletteData(uint8_t val);
    void setObjPalette0Data(uint8_t val);
    void setObjPalette1Data(uint8_t val);

    /**
     * LCD COLOR PALETTES (CGB ONLY)
     *
     * BGPI: BG Color Palette Index (0xFF68) - Used to address a byte in the CGB Palette Memory
     *                                         Bit 7: Auto Increment after write
     *                                         Bit 5-0: Index (0x00-0x3F)
     *
     * BGPD: BG Color Palette Data (0xFF69) - Used to access byte selected by BGPI
     * OBPI: Object Color Palette Index (0xFF6A) - Similar to BGPI
     * OBPD: Object Color Palette Data (0xFF6B) - Similar to BGPD
     */

    uint8_t getBgColorPaletteIndex();
    uint8_t getBgColorPaletteData();
    uint8_t getObjColorPaletteIndex();
    uint8_t getObjColorPaletteData();
    void setBgColorPaletteIndex(uint8_t val);
    void setBgColorPaletteData(uint8_t val);
    void setObjColorPaletteIndex(uint8_t val);
    void setObjColorPaletteData(uint8_t val);

    /**
     * OAM DMA
     *
     * DMA: DMA Transfer and Start Address (0xFF46) - Launch OAM DMA and specify source address
     *                                                Src Addess format: XX00 (XX is a value
     *                                                between 0x00 and 0xF1)
     *                                                Copies bytes from XX00-XX9F to 0xFE00-0xFE9F
     */

    uint8_t getOamDma();
    void setOamDma(uint8_t val);

    /**
     * VRAM DMA (CGB Only)
     * HDMA1: DMA Source High Byte (0xFF51)
     * HDMA2: DMA Source Low Byte (0xFF52)
     *        These 2 registers are combined to create the DMA source address. The 4 lower bits are
     * ignored HDMA3: DMA Destination High Byte (0xFF53) HDMA4: DMA Destination Low Byte (0xFF54)
     *        These 2 registers are combined to create the DMA destination address. Only bits 12-4
     * are used HDMA5: DMA Length, Mode, Start (0xFF55) - Writing to this register starts the DMA
     * transfer Bit 7: Mode: General=0 or HBlank=1 Bit 6-0: Transfer length divided by 0x10 minus 1
     *                                           In General mode all data is transferred at once
     *                                           In HBlank mode data is transferred during HBlank
     * mode
     */

    uint8_t getHdma1();
    uint8_t getHdma2();
    uint16_t getHdmaSrcAddress();
    uint8_t getHdma3();
    uint8_t getHdma4();
    uint16_t getHdmaDestAddress();
    uint8_t getHdma5();
    uint8_t getHdmaMode();
    uint8_t getHdmaLength();
    void setHdma1(uint8_t val);
    void setHdma2(uint8_t val);
    void setHdma3(uint8_t val);
    void setHdma4(uint8_t val);
    void setHdma5(uint8_t val);

    Tile getTileByIndex(int index);
    OAMSprite getSpriteByIndex(int index);
    BgMapAttributes getBgMapByIndex(int index, int tilemap); // tilemap 0 = 0x9800; 1 = 0x9C00

    // Returns a sprite tile (0x8000-0x8FFF) based on its index. If LCDC.2 is set (8x16 mode) then
    // the tileNo argument is used to decide what tile of the sprite should be returned, if LCDC.2
    // is 0 then tileNo is ignored.
    Tile getSpriteTile(int index, int tileNo = 0, int vramBank = 0);

    // Searches the sprites that will be displayed on the current line and puts their index in
    // spritesOnCurrentLine. Also sets numSpritesOnCurrentLine
    void searchSpritesOnLine();

    void cycle();

    // Some other stuff

    // vram bg maps: bg map tile numbers? in VRAM0, bg map attributes cgb only,
    // in VRAM1 (create a struct for it?) flip tile? sau ar trebui sa fie in
    // pixel fetcher/pixel fifo;

    // get window tile sau cv de genu?

    // sa ma apuc sa fac loopul principal, in mare am cam facut toate getterele si setterele

    // dma registers, dma flags, and code for cycle copy vram dma registers, and flags

    // pixel fifo: bg fifo si sprite fifo. ar trebui sa fie 2 clase? care mostenesc o clasa parinte
    // fifo? pixel fetcher:

    // PPU ar trebui sa cicleze odata pe T-cycle (4 MHz), ca unele operatii sunt in functie de
    // T-cycles nu doar M-cycles create separate src folder for ppu i guess
};

#endif // __PPU_H__
