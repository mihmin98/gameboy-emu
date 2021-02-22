#ifndef __GAMEBOY_H__
#define __GAMEBOY_H__

#define GAMEBOY_CLOCK_FREQUENCY 4194304
#define GAMEBOY_CYCLE_FREQUENCY 1048576
#define GAMEBOY_MEMORY_SIZE 65536

// Memory addresses
#define GAMEBOY_ROM0_START 0x0000
#define GAMEBOY_ROMX_START 0x4000
#define GAMEBOY_VRAM_START 0x8000
#define GAMEBOY_SRAM_START 0xA000
#define GAMEBOY_WRAM0_START 0xC000
#define GAMEBOY_WRAMX_START 0xD000
#define GAMEBOY_ECHO_START 0xE000
#define GAMEBOY_OAM_START 0xFE00
#define GAMEBOY_UNUSED_START 0xFEA0
#define GAMEBOY_IO_START 0xFF00
#define GAMEBOY_HRAM_START 0xFF80

#pragma once
#include <stdint.h>

enum EmulatorMode { DMG, CGB };

class GameBoy
{
  public:
    GameBoy();
    ~GameBoy();
};

#endif // __GAMEBOY_H__
