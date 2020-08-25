#include "PPU.hpp"

/* GETTERS AND SETTERS */

/* LCD CONTROL REGISTER */

uint8_t PPU::getLcdDisplayEnable() { return (memory->readmem(0xFF40) & 0x80) >> 7; }

uint8_t PPU::getWindowTileMapDisplaySelect() { return (memory->readmem(0xFF40) & 0x40) >> 6; }

uint8_t PPU::getWindowDisplayEnable() { return (memory->readmem(0xFF40) & 0x20) >> 5; }

uint8_t PPU::getBgAndWindowTileDataSelect() { return (memory->readmem(0xFF40) & 0x10) >> 4; }

uint8_t PPU::getBgTileMapDisplaySelect() { return (memory->readmem(0xFF40) & 0x8) >> 3; }

uint8_t PPU::getObjSize() { return (memory->readmem(0xFF40) & 0x4) >> 2; }

uint8_t PPU::getObjDisplayEnable() { return (memory->readmem(0xFF40) & 0x2) >> 1; }

uint8_t PPU::getBgWindowDisplayPriority() { return memory->readmem(0xFF40) & 0x1; }

void PPU::setLcdControlRegister(uint8_t val) { memory->writemem(val, 0xFF40); }

void PPU::setLcdDisplayEnable(uint8_t val) { memory->writebit(val, 7, 0xFF40); }

void PPU::setWindowTileMapDisplaySelect(uint8_t val) { memory->writebit(val, 6, 0xFF40); }

void PPU::setWindowDisplayEnable(uint8_t val) { memory->writebit(val, 5, 0xFF40); }

void PPU::setBgAndWindowTileDataSelect(uint8_t val) { memory->writebit(val, 4, 0xFF40); }

void PPU::setBgTileMapDisplaySelect(uint8_t val) { memory->writebit(val, 3, 0xFF40); }

void PPU::setObjSize(uint8_t val) { memory->writebit(val, 2, 0xFF40); }

void PPU::setObjDisplayEnable(uint8_t val) { memory->writebit(val, 1, 0xFF40); }

void PPU::setBgWindowDisplayPriority(uint8_t val) { memory->writebit(val, 0, 0xFF40); }

/* LCD STATUS REGISTER */

uint8_t PPU::getLycLyCoincidence() { return (memory->readmem(0xFF41) & 0x40) >> 6; }

uint8_t PPU::getMode2OamInterrupt() { return (memory->readmem(0xFF41) & 0x20) >> 5; }

uint8_t PPU::getMode1VBlankInterrupt() { return (memory->readmem(0xFF41) & 0x10) >> 4; }

uint8_t PPU::getMode0HBlankInterrupt() { return (memory->readmem(0xFF41) & 0x8) >> 3; }

uint8_t PPU::getCoincidenceFlag() { return (memory->readmem(0xFF41) & 0x4) >> 2; }

uint8_t PPU::getModeFlag() { return memory->readmem(0xFF41) & 0x2; }

void PPU::setLcdStatRegister(uint8_t val) { memory->writemem(val, 0xFF41); }

void PPU::setLycLyCoincidence(uint8_t val) { memory->writebit(val, 6, 0xFF41); }

void PPU::setMode2OamInterrupt(uint8_t val) { memory->writebit(val, 5, 0xFF41); }

void PPU::setMode1VBlankInterrupt(uint8_t val) { memory->writebit(val, 4, 0xFF41); }

void PPU::setMode0HBlankInterrupt(uint8_t val) { memory->writebit(val, 3, 0xFF41); }

void PPU::setCoincidenceFlag(uint8_t val) { memory->writebit(val, 2, 0xFF41); }

void PPU::setModeFlag(uint8_t val)
{
    if (val > 3)
        return;

    uint8_t byte = memory->readmem(0xFF41) & 0xFC;
    byte |= val;
    memory->writemem(byte, 0xFF41);
}

/* LCD POSITION AND SCROLLING */

uint8_t PPU::getScrollY() { return memory->readmem(0xFF42); }

uint8_t PPU::getScrollX() { return memory->readmem(0xFF43); }

uint8_t PPU::getLy() { return memory->readmem(0xFF44); }

uint8_t PPU::getLyc() { return memory->readmem(0xFF45); }

uint8_t PPU::getWy() { return memory->readmem(0xFF4A); }

uint8_t PPU::getWx() { return memory->readmem(0xFF4B); }

void PPU::setScrollY(uint8_t val) { memory->writemem(val, 0xFF42); }

void PPU::setScrollX(uint8_t val) { memory->writemem(val, 0xFF43); }

void PPU::setLy(uint8_t val) { memory->writemem(val, 0xFF44); }

void PPU::setLyc(uint8_t val) { memory->writemem(val, 0xFF45); }

void PPU::setWy(uint8_t val) { memory->writemem(val, 0xFF4A); }

void PPU::setWx(uint8_t val) { memory->writemem(val, 0xFF4B); }

/* LCD MONOCHROME PALETTES */

uint8_t PPU::getBgPaletteData() { return memory->readmem(0xFF47); }

uint8_t PPU::getObjPalette0Data() { return memory->readmem(0xFF48); }

uint8_t PPU::getObjPalette1Data() { return memory->readmem(0xFF49); }

void PPU::setBgPaletteData(uint8_t val) { memory->writemem(val, 0xFF47); }

void PPU::setObjPalette0Data(uint8_t val) { memory->writemem(val, 0xFF48); }

void PPU::setObjPalette1Data(uint8_t val) { memory->writemem(val, 0xFF49); }

/* LCD COLOR PALETTES */

uint8_t PPU::getBgColorPaletteIndex() { return memory->readmem(0xFF68); }

uint8_t PPU::getBgColorPaletteData() { return memory->readmem(0xFF69); }

uint8_t PPU::getObjColorPaletteIndex() { return memory->readmem(0xFF6A); }

uint8_t PPU::getObjColorPaletteData() { return memory->readmem(0xFF6B); }

void PPU::setBgColorPaletteIndex(uint8_t val) { memory->writemem(val, 0xFF68); }

void PPU::setBgColorPaletteData(uint8_t val) { memory->writemem(val, 0xFF69); }

void PPU::setObjColorPaletteIndex(uint8_t val) { memory->writemem(val, 0xFF6A); }

void PPU::setObjColorPaletteData(uint8_t val) { memory->writemem(val, 0xFF6B); }
