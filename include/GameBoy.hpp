#ifndef __GAMEBOY_H__
#define __GAMEBOY_H__

#define GAMEBOY_CLOCK_FREQUENCY 4194304
#define GAMEBOY_DOUBLE_CLOCK_FREQUENCY 8388608
#define GAMEBOY_SPEED_SWITCH_CLOCKS 8200
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
#include "Audio.hpp"
#include "Enums.hpp"
#include "Joypad.hpp"
#include "Memory.hpp"
#include "PPU.hpp"
#include "ROM.hpp"
#include "SM83.hpp"
#include "Timer.hpp"
#include <SDL2/SDL.h>
#include <chrono>
#include <cstring>
#include <iostream>
#include <stdint.h>
#include <string>
#include <thread>

class PPU;
class SM83;
class Memory;

class GameBoy
{
  public:
    GameBoy(EmulatorMode emulatorMode = EmulatorMode::DMG);
    ~GameBoy();

    std::string romPath;

    EmulatorMode emulatorMode;
    Memory memory;
    PPU ppu;
    SM83 cpu;
    ROM rom;
    Timer timer;
    Joypad joypad;
    Audio audio;

    bool doubleSpeedMode;
    uint32_t speedSwitchSleepCycles;
    std::chrono::high_resolution_clock::time_point tp1, tp2, afterDraw;
    double cycleDuration; // cycle duration in ms
    double currentCycleTime;

    uint currentCycles;
    uint numCyclesPerFrame;

    uint8_t cpuWaitTCycles; // cycle cpu once every 4 t cycles
    uint8_t audioBatchCycles;

    // float windowScale;
    int windowWidth, windowHeight;
    SDL_Window *sdlWindow;
    SDL_Surface *sdlSurface;
    SDL_Renderer *sdlRenderer;
    SDL_Texture *sdlTexture;
    SDL_PixelFormat *sdlPixelFormat;
    uint8_t *sdlTexturePixels = nullptr;

    SDL_DisplayMode sdlDisplayMode;
    uint refreshRate;

    const uint8_t *keyboardState;
    // [0][0-3] -> direction buttons; [1][0-3] -> action buttons
    bool currentKeysState[2][4];

    Color displayBuffer[144][160];

    void run();
    void initSDL();
    double getDeltaTime(std::chrono::high_resolution_clock::time_point &tp1,
                        std::chrono::high_resolution_clock::time_point &tp2);
    bool getInput();
    void savePpuBuffer();
    void drawFrame();
    void setInitialState();

    void setDoubleSpeedMode(bool doubleSpeed, bool sleepDuringSwitch = true);
};

#endif // __GAMEBOY_H__
