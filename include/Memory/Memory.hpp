#ifndef __MEMORY_H__
#define __MEMORY_H__

#pragma once
#include <cstdint>
#include <cstring>
#include <cstdio>
#include "ROM.hpp"
#include "GameBoy.hpp"
#include "PPU.hpp"

#define MEM_ROM0_START 0x0000
#define MEM_ROMX_START 0x4000
#define MEM_VRAM_START 0x8000
#define MEM_EXT_RAM_START 0xA000
#define MEM_WRAM0_START 0xC000
#define MEM_WRAMX_START 0xD000
#define MEM_ECHO_START 0xE000
#define MEM_OAM_START 0xFE00
#define MEM_UNUSED_START 0xFEA0
#define MEM_IO_START 0xFF00
#define MEM_HRAM_START 0xFF80
#define MEM_IE_REG 0xFFFF

class GameBoy;
class PPU;

class Memory
{
  public:

    EmulatorMode mode;
    ROM *rom; // Pointer to the ROM
    PPU *ppu;

    /* MEMORY AREAS */

    uint8_t vram[2 * 0x2000];
    uint8_t currentVramBank;
    uint8_t wram[8 * 0x1000];
    uint8_t currentWramBank;
    uint8_t oam[0xA0];
    uint8_t ioRegisters[0x80];
    uint8_t hram[0x7F];
    uint8_t ieRegister;

    uint8_t cgbBgColorPalette[0x40];
    uint8_t cgbObjColorPalette[0x40];

    Memory(EmulatorMode mode = EmulatorMode::DMG); 
    
    uint8_t readmem(uint16_t addr);
    void writemem(uint8_t val, uint16_t addr);
    void writebit(uint8_t val, uint8_t bit, uint16_t addr);

    uint8_t getCurrentVramBank();
    uint8_t getCurrentWramBank();

    void setCurrentVramBank(uint8_t val);
    void setCurrentWramBank(uint8_t val);
};
 
#endif // __MEMORY_H__

