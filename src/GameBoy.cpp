#include "GameBoy.hpp"
#include "Config.hpp"

GameBoy::GameBoy(EmulatorMode emulatorMode)
{
    this->emulatorMode = emulatorMode;

    memory.ppu = &ppu;
    memory.rom = &rom;
    memory.timer = &timer;
    memory.audio = &audio;
    memory.mode = emulatorMode;

    ppu.memory = &memory;
    ppu.cpu = &cpu;
    ppu.emulatorMode = emulatorMode;

    cpu.memory = &memory;
    cpu.gameboy = this;

    timer.cpu = &cpu;
    timer.memory = &memory;

    joypad.cpu = &cpu;
    joypad.memory = &memory;

    audio.memory = &memory;

    speedSwitchSleepCycles = 0;

    cpuWaitTCycles = 3;

    for (uint i = 0; i < 4; ++i) {
        currentKeysState[0][i] = false;
        currentKeysState[1][i] = false;
    }
}

GameBoy::~GameBoy() {}

void GameBoy::setDoubleSpeedMode(bool doubleSpeed, bool sleepDuringSwitch)
{
    if (!doubleSpeedMode && doubleSpeed) {
        std::cout << "go to double speed\n";
        // currentCycles *= 2;
        // numCyclesPerFrame *= 2;
    }

    else if (doubleSpeedMode && !doubleSpeed) {
        std::cout << "go to single speed\n";
        // currentCycles /= 2;
        // numCyclesPerFrame /= 2;
    }

    doubleSpeedMode = doubleSpeed;
    ppu.doubleSpeedMode = doubleSpeed;

    if (doubleSpeed)
        cycleDuration = 1.0 / GAMEBOY_DOUBLE_CLOCK_FREQUENCY * 1000;
    else
        cycleDuration = 1.0 / GAMEBOY_CLOCK_FREQUENCY * 1000;

    if (sleepDuringSwitch)
        speedSwitchSleepCycles = GAMEBOY_SPEED_SWITCH_CLOCKS;
}

double GameBoy::getDeltaTime(std::chrono::high_resolution_clock::time_point &tp1,
                             std::chrono::high_resolution_clock::time_point &tp2)
{
    return std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(tp2 - tp1).count();
}

void GameBoy::initSDL()
{
    int windowScale = Config::getInstance()->getWindowSize();
    windowWidth = PPU_SCREEN_WIDTH * windowScale;
    windowHeight = PPU_SCREEN_HEIGHT * windowScale;

    sdlWindow = SDL_CreateWindow("GameBoy Emu", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                 windowWidth, windowHeight, SDL_WINDOW_SHOWN);

    if (sdlWindow == NULL) {
        std::cerr << "initSDL() error: " << SDL_GetError() << "\n";
        exit(EXIT_FAILURE);
    }

    sdlRenderer =
        SDL_CreateRenderer(sdlWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    sdlTexture =
        SDL_CreateTexture(sdlRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING,
                          PPU_SCREEN_WIDTH, PPU_SCREEN_HEIGHT);

    sdlPixelFormat = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);

    SDL_RenderClear(sdlRenderer);
    SDL_RenderSetScale(sdlRenderer, windowScale, windowScale);
    SDL_RenderPresent(sdlRenderer);

    keyboardState = SDL_GetKeyboardState(NULL);

    SDL_GetCurrentDisplayMode(0, &sdlDisplayMode);
    refreshRate = sdlDisplayMode.refresh_rate;

    audio.initSDL();
}

bool GameBoy::getInput()
{
    SDL_PumpEvents();

    if (keyboardState[SDL_SCANCODE_ESCAPE])
        return true;

    // direction buttons
    joypad.keyState[0][0] = keyboardState[SDL_SCANCODE_RIGHT];
    joypad.keyState[0][1] = keyboardState[SDL_SCANCODE_LEFT];
    joypad.keyState[0][2] = keyboardState[SDL_SCANCODE_UP];
    joypad.keyState[0][3] = keyboardState[SDL_SCANCODE_DOWN];

    // action buttons
    joypad.keyState[1][0] = keyboardState[SDL_SCANCODE_X];
    joypad.keyState[1][1] = keyboardState[SDL_SCANCODE_Z];
    joypad.keyState[1][2] = keyboardState[SDL_SCANCODE_BACKSPACE];
    joypad.keyState[1][3] = keyboardState[SDL_SCANCODE_SPACE];

    return false;
}

// Sets the initial state after the bootrom
void GameBoy::setInitialState()
{
    /**
     *  SP = 0xFFFE
     *  Zero out 0x8000 - 0x9FFF (VRAM)
     *
     * Pe moment cred ca ar fi
     *  SP = 0xFFFE
     *  PC = 0x0100
     */

    cpu.PC = 0x0100;

    cpu.A = 0x01;
    cpu.F = 0xB0;
    cpu.B = 0x00;
    cpu.C = 0x13;
    cpu.D = 0x00;
    cpu.E = 0xD8;
    cpu.H = 0x01;
    cpu.L = 0x4D;
    cpu.SP = 0xFFFE;

    // Joypad
    memory.ioRegisters[0xFF00 - MEM_IO_START] = 0xCF;

    // Timer
    timer.setDividerCounter(0xABCC);
    memory.ioRegisters[0xFF05 - MEM_IO_START] = 0;
    memory.ioRegisters[0xFF06 - MEM_IO_START] = 0;
    memory.ioRegisters[0xFF07 - MEM_IO_START] = 0xF8;

    // Audio
    memory.ioRegisters[0xFF10 - MEM_IO_START] = 0x80;
    memory.ioRegisters[0xFF11 - MEM_IO_START] = 0xBF;
    memory.ioRegisters[0xFF12 - MEM_IO_START] = 0xF3;
    memory.ioRegisters[0xFF14 - MEM_IO_START] = 0xBF;
    memory.ioRegisters[0xFF16 - MEM_IO_START] = 0x3F;
    memory.ioRegisters[0xFF17 - MEM_IO_START] = 0x00;
    memory.ioRegisters[0xFF19 - MEM_IO_START] = 0xBF;
    memory.ioRegisters[0xFF1A - MEM_IO_START] = 0x7F;
    memory.ioRegisters[0xFF1B - MEM_IO_START] = 0xFF;
    memory.ioRegisters[0xFF1C - MEM_IO_START] = 0x9F;
    memory.ioRegisters[0xFF1E - MEM_IO_START] = 0xBF;
    memory.ioRegisters[0xFF20 - MEM_IO_START] = 0xFF;
    memory.ioRegisters[0xFF21 - MEM_IO_START] = 0x00;
    memory.ioRegisters[0xFF22 - MEM_IO_START] = 0x00;
    memory.ioRegisters[0xFF23 - MEM_IO_START] = 0xBF;
    memory.ioRegisters[0xFF24 - MEM_IO_START] = 0x77;
    memory.ioRegisters[0xFF25 - MEM_IO_START] = 0xF3;
    memory.ioRegisters[0xFF26 - MEM_IO_START] = 0xF1;

    // PPU
    memory.ioRegisters[0xFF40 - MEM_IO_START] = 0x91;
    memory.ioRegisters[0xFF42 - MEM_IO_START] = 0x00;
    memory.ioRegisters[0xFF43 - MEM_IO_START] = 0x00;
    memory.ioRegisters[0xFF45 - MEM_IO_START] = 0x00;
    memory.ioRegisters[0xFF47 - MEM_IO_START] = 0xE4;
    memory.ioRegisters[0xFF48 - MEM_IO_START] = 0xFF;
    memory.ioRegisters[0xFF49 - MEM_IO_START] = 0xFF;
    memory.ioRegisters[0xFF4A - MEM_IO_START] = 0x00;
    memory.ioRegisters[0xFF4B - MEM_IO_START] = 0x00;

    // Disable Bootrom
    memory.ioRegisters[0xFF50 - MEM_IO_START] = 0x01;

    // IE
    memory.ieRegister = 0x00;

    if (emulatorMode == CGB) {
        cpu.A = 0x11;
        cpu.B = 0;
    }

    ppu.setModeFlag(LcdMode::OAM_SEARCH);
    ppu.setLcdDisplayEnable(1);
}

void GameBoy::run()
{
    if (!rom.loadROM(romPath))
        return;

    if (Config::getInstance()->getUseBootrom() && emulatorMode != CGB) {
        rom.loadBootrom(Config::getInstance()->getBootromPath());
        cpu.PC = 0;
    } else {
        setInitialState();
    }

    // set PC and register values
    setDoubleSpeedMode(false, false);

    bool quit = false;
    tp1 = tp2 = std::chrono::high_resolution_clock::now();
    currentCycleTime = 0;

    std::cout << "PC: " << std::hex << cpu.PC << "\n";

    // uint numCyclesPerFrame = 70224.0 * 60.0 / 59.73;
    numCyclesPerFrame = 70224.0 * 59.73 / 60;
    // uint numCyclesPerFrame = 70224;
    currentCycles = 0;

    audioBatchCycles = Config::getInstance()->getAudioBatchCycles();
    bool printPerformanceInfo = Config::getInstance()->getPrintPerformanceInfo();

    while (!quit) {

        tp1 = std::chrono::high_resolution_clock::now();

        currentCycles = 0;
        while (currentCycles < numCyclesPerFrame) {
            if (currentCycles % 1000 == 0) {
                if ((quit = getInput()) == true) {
                    break;
                }
            }

            joypad.cycle();

            // DMA
            if (ppu.oamDmaActive) {
                ppu.oamDmaCycle();
            }

            if (emulatorMode == CGB && ppu.vramGeneralDmaActive) {
                ppu.vramDmaCycle();
            }

            // CPU
            if ((emulatorMode == EmulatorMode::DMG ||
                 !(ppu.vramGeneralDmaActive || ppu.vramHblankDmaActive)) &&
                speedSwitchSleepCycles == 0) {
                --cpuWaitTCycles;
                if (cpuWaitTCycles == 0) {
                    cpu.cycle();
                    cpuWaitTCycles = 4;
                }
            }

            // Audio
            if (doubleSpeedMode && currentCycles % (audioBatchCycles * 2) == 0) {
                audio.cycle(audioBatchCycles);
            } else if (currentCycles % audioBatchCycles == 0) {
                audio.cycle(audioBatchCycles);
            }

            // PPU
            if (ppu.getLcdDisplayEnable()) {
                ppu.cycle();
            }

            // Timer
            timer.cycle();

            if (ppu.readyToDraw) {
                savePpuBuffer();
                ppu.readyToDraw = false;
                // doCycles = false;
                // break;
            }

            // ROM RTC Timer
            if (rom.mbc == MBC::MBC3 && rom.cartridgeTimer) {
                if (doubleSpeedMode && currentCycles % (ROM_RTC_T_CYCLES_UNTIL_TICK * 2) == 0) {
                    rom.cycleRtc();
                } else if (!doubleSpeedMode && currentCycles % ROM_RTC_T_CYCLES_UNTIL_TICK == 0) {
                    rom.cycleRtc();
                }
            }

            if (speedSwitchSleepCycles > 0)
                --speedSwitchSleepCycles;

            ++currentCycles;
        }

        tp2 = std::chrono::high_resolution_clock::now();

        rom.saveRam();
        drawFrame();

        afterDraw = std::chrono::high_resolution_clock::now();

        if (printPerformanceInfo) {
            std::cout << std::dec << "Time to do " << (uint)currentCycles
                      << " cycles: " << getDeltaTime(tp1, tp2)
                      << "; time to wait/draw frame: " << getDeltaTime(tp2, afterDraw)
                      << "; refresh rate: " << (uint)refreshRate << "\n";
        }

        quit = getInput();
    }
}

void GameBoy::savePpuBuffer()
{
    memcpy(displayBuffer, ppu.display, PPU_SCREEN_HEIGHT * PPU_SCREEN_WIDTH * sizeof(Color));
}

void GameBoy::drawFrame()
{
    SDL_RenderClear(sdlRenderer);

    // update texture
    void *texturePixels;
    int texturePitch; // pitch is length of row in bytes
    SDL_LockTexture(sdlTexture, NULL, &texturePixels, &texturePitch);

    uint8_t pixelSize = texturePitch / PPU_SCREEN_WIDTH;

    if (sdlTexturePixels == nullptr)
        sdlTexturePixels = new uint8_t[texturePitch * PPU_SCREEN_HEIGHT];

    // Put the pixels from the PPU in sdlTexturePixels
    for (int i = 0; i < PPU_SCREEN_HEIGHT; ++i) {
        for (int j = 0; j < PPU_SCREEN_WIDTH; ++j) {
            Color &ppuPixel = displayBuffer[i][j];

            uint32_t rgbPixel =
                SDL_MapRGB(sdlPixelFormat, ppuPixel.red, ppuPixel.green, ppuPixel.blue);

            memcpy(sdlTexturePixels + i * texturePitch + j * pixelSize, &rgbPixel,
                   sizeof(uint32_t));
        }
    }

    // Copy sdlTexturePixels to texture pixels (the pointer from SDL_LockTexture)
    memcpy(texturePixels, sdlTexturePixels, texturePitch * PPU_SCREEN_HEIGHT * sizeof(uint8_t));

    SDL_UnlockTexture(sdlTexture);

    SDL_RenderCopy(sdlRenderer, sdlTexture, NULL, NULL);
    SDL_RenderPresent(sdlRenderer);
}
