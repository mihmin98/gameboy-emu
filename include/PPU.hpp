#ifndef __PPU_H__
#define __PPU_H__

#pragma once
#include "Memory.hpp"

class Memory;

class PPU
{
  public:
    Memory *memory;

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
     *                                         Bit 0-5: Index (0x00-0x3F)
     *                                         Bit 7: Auto Increment after write
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

};

#endif // __PPU_H__
