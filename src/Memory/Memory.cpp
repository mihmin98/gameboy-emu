#include "Memory.hpp"
#include "GameBoy.hpp"
#include "PPU.hpp"

Memory::Memory(EmulatorMode mode)
{
    this->mode = mode;
    currentVramBank = 0;
    currentWramBank = 1;
    setCurrentVramBank(0);
    setCurrentWramBank(1);

    for (int i = 0; i < 0x40; ++i)
        cgbBgColorPalette[i] = 255;

    // Zero-out memory
    memset(vram, 0, 2 * 0x2000);
    memset(wram, 0, 8 * 0x1000);
    memset(oam, 0, 0xA0);
    memset(ioRegisters, 0, 0x80);
    memset(hram, 0, 0x7F);
    ieRegister = 0;
}

uint8_t Memory::readmem(uint16_t addr, bool bypass, bool bypassOamDma)
{
    // ROM + External RAM
    if (addr < MEM_VRAM_START || (addr >= MEM_EXT_RAM_START && addr < MEM_WRAM0_START)) {
        if (ppu->oamDmaActive && !bypass)
            return 0xFF;
        else
            return rom->readmem(addr);
    }

    // VRAM
    if (addr >= MEM_VRAM_START && addr < MEM_EXT_RAM_START) {
        if (ppu->oamDmaActive && !bypass) {
            return 0xFF;
        } else {
            currentVramBank = getCurrentVramBank();
            uint16_t actualAddr = (addr - MEM_VRAM_START) + currentVramBank * 0x2000;
            return vram[actualAddr];
        }
    }

    // WRAM Bank 0
    if (addr >= MEM_WRAM0_START && addr < MEM_WRAMX_START) {
        if (ppu->oamDmaActive && !bypass)
            return 0xFF;
        else
            return wram[addr - MEM_WRAM0_START];
    }

    // Switchable WRAM Bank
    if (addr >= MEM_WRAMX_START && addr < MEM_ECHO_START) {
        if (ppu->oamDmaActive && !bypass) {
            return 0xFF;
        } else {
            currentWramBank = getCurrentWramBank();
            uint16_t actualAddr = (addr - MEM_WRAMX_START) + currentWramBank * 0x1000;
            return wram[actualAddr];
        }
    }

    // ECHO RAM
    if (addr >= MEM_ECHO_START && addr < MEM_OAM_START) {
        if (ppu->oamDmaActive && !bypass) {
            return 0xFF;
        } else {
            // Mirror of WRAM Bank 0
            if (addr >= MEM_ECHO_START && addr < 0xF000)
                return wram[addr - MEM_ECHO_START];

            // Mirror of switchable WRAM Bank
            if (addr >= 0xF000 && addr < MEM_OAM_START) {
                currentWramBank = getCurrentWramBank();
                uint16_t actualAddr = (addr - 0xF000) + currentWramBank * 0x1000;
                return wram[actualAddr];
            }
        }
    }

    // OAM
    if (addr >= MEM_OAM_START && addr < MEM_UNUSED_START) {
        if (ppu->oamDmaActive && !bypass && !bypassOamDma) {
            return 0xFF;
        } else {
            LcdMode lcdMode = (LcdMode)getLcdMode();
            if ((lcdMode == OAM_SEARCH && !bypass) || (lcdMode == DRAW && !bypass))
                return 0xFF;
            else
                return oam[addr - MEM_OAM_START];
        }
    }

    // Unused range
    if (addr >= MEM_UNUSED_START && addr < MEM_IO_START) {
        if (ppu->oamDmaActive && !bypass)
            return 0xFF;
        else
            return 0x00;
    }

    // I/O Registers
    if (addr >= MEM_IO_START && addr < MEM_HRAM_START) {
        if (ppu->oamDmaActive && !bypass) {
            return 0xFF;
        } else {
            // TODO: Case for 0xFF55 (vram hblank dma remaining bytes - 1)
            // This should be done by the ppu? Already done?
            // there is no need for a special case or something

            LcdMode lcdMode = (LcdMode)getLcdMode();

            if (mode == EmulatorMode::CGB) {
                if (addr == 0xFF69) {
                    // BGPD (Backgroud Color Palette Data)
                    if (lcdMode == DRAW && !bypass) {
                        return 0xFF;
                    } else {
                        uint8_t index = ppu->getBgColorPaletteIndex() & 0x3F;
                        return cgbBgColorPalette[index];
                    }
                }

                if (addr == 0xFF6B) {
                    // OBPD (Object Color Palette Data)
                    if (lcdMode == DRAW && !bypass) {
                        return 0xFF;
                    } else {
                        uint8_t index = ppu->getObjColorPaletteIndex() & 0x3F;
                        return cgbObjColorPalette[index];
                    }
                }
            }

            return ioRegisters[addr - MEM_IO_START];
        }
    }

    // HRAM
    if (addr >= MEM_HRAM_START && addr < MEM_IE_REG)
        return hram[addr - MEM_HRAM_START];

    // IE Register
    if (addr == MEM_IE_REG) {
        if (ppu->oamDmaActive && !bypass)
            return 0xFF;
        else
            return ieRegister;
    }

    // Invalid address
    fprintf(stderr, "WARNING: Trying to read from invalid memory address: 0x%04X\n", addr);
    return 0xFF;
}

void Memory::writemem(uint8_t val, uint16_t addr, bool bypass, bool bypassOamDma)
{
    // ROM + External RAM
    if (addr < MEM_VRAM_START || (addr >= MEM_EXT_RAM_START && addr < MEM_WRAM0_START)) {
        if (!ppu->oamDmaActive || bypass)
            rom->writemem(val, addr);
    }

    // VRAM
    if (addr >= MEM_VRAM_START && addr < MEM_EXT_RAM_START) {
        if (!ppu->oamDmaActive || bypass) {
            currentVramBank = getCurrentVramBank();
            uint16_t actualAddr = (addr - MEM_VRAM_START) + currentVramBank * 0x2000;
            vram[actualAddr] = val;
        }
    }

    // WRAM Bank 0
    if (addr >= MEM_WRAM0_START && addr < MEM_WRAMX_START) {
        if (!ppu->oamDmaActive || bypass)
            wram[addr - MEM_WRAM0_START] = val;
    }

    // Switchable WRAM Bank
    if (addr >= MEM_WRAMX_START && addr < MEM_ECHO_START) {
        if (!ppu->oamDmaActive || bypass) {
            currentWramBank = getCurrentWramBank();
            uint16_t actualAddr = (addr - MEM_WRAMX_START) + currentWramBank * 0x1000;
            wram[actualAddr] = val;
        }
    }

    // ECHO RAM
    if (addr >= MEM_ECHO_START && addr < MEM_OAM_START) {
        if (!ppu->oamDmaActive || bypass) {
            // Mirror of WRAM Bank 0
            if (addr >= MEM_ECHO_START && addr < 0xF000)
                wram[addr - MEM_ECHO_START] = val;

            // Mirror of switchable WRAM Bank
            if (addr >= 0xF000 && addr < MEM_OAM_START) {
                currentWramBank = getCurrentWramBank();
                uint16_t actualAddr = (addr - 0xF000) + currentWramBank * 0x1000;
                wram[actualAddr] = val;
            }
        }
    }

    // OAM
    if (addr >= MEM_OAM_START && addr < MEM_UNUSED_START) {
        if (!ppu->oamDmaActive || (bypass && bypassOamDma)) {
            LcdMode lcdMode = (LcdMode)getLcdMode();
            if ((lcdMode != OAM_SEARCH || (bypass && bypassOamDma)) &&
                (lcdMode != DRAW || (bypass && bypassOamDma)))
                oam[addr - MEM_OAM_START] = val;
        }
    }

    // Unused range
    if (addr >= MEM_UNUSED_START && addr < MEM_IO_START)
        return;

    // I/O Registers
    if (addr >= MEM_IO_START && addr < MEM_HRAM_START) {
        if (!ppu->oamDmaActive || bypass) {
            LcdMode lcdMode = (LcdMode)getLcdMode();

            if (addr == 0xFF46) {
                // Trigger OAM DMA
                ppu->oamDmaActive = true;
                ppu->oamDmaCurrentCycles = 0;
                ioRegisters[addr - MEM_IO_START] = val;
            }

            if (mode == EmulatorMode::CGB) {
                if (addr == 0xFF55) {
                    // VRAM DMA
                    if (ppu->vramHblankDmaActive) {
                        if ((val & 0x80) == 0) {
                            // Stop dma if bit 7 is 0, afterwards set bit 7 to 1
                            ppu->vramHblankDmaActive = false;
                            ioRegisters[addr - MEM_IO_START] =
                                (1 << 7) | (ioRegisters[addr - MEM_IO_START] & 0x7F);
                        }
                    } else if (!ppu->vramHblankDmaActive && !ppu->vramGeneralDmaActive) {
                        // Start dma
                        ppu->vramDmaLength = val & 0x7F;
                        ppu->vramDmaTransferredBytes = 0;
                        ppu->vramDmaCurrentCycles = 0;
                        if (val & 80)
                            ppu->vramHblankDmaActive = true;
                        else
                            ppu->vramGeneralDmaActive = true;

                        // Set bit 7 to 0
                        ioRegisters[addr - MEM_IO_START] = val & 0x7F;
                    }
                }

                if (addr == 0xFF69) {
                    // BGPD (Backgroud Color Palette Data)
                    if (lcdMode != DRAW || bypass) {
                        uint8_t index = ppu->getBgColorPaletteIndex();
                        cgbBgColorPalette[index & 0x3F] = val;
                        // AutoIncrement
                        if (index & 0x80)
                            ppu->setBgColorPaletteIndex(0x80 | (((index & 0x3F) + 1) & 0x3F));
                    }
                }

                if (addr == 0xFF6B) {
                    // OBPD (Object Color Palette Data)
                    if (lcdMode != DRAW || bypass) {
                        uint8_t index = ppu->getObjColorPaletteIndex();
                        cgbObjColorPalette[index & 0x3F] = val;
                        // AutoIncrement
                        if (index & 0x80)
                            ppu->setObjColorPaletteIndex(0x80 | (((index & 0x3F) + 1) & 0x3F));
                    }
                }
            }

            else {
                ioRegisters[addr - MEM_IO_START] = val;
            }
        }
    }

    // HRAM
    if (addr >= MEM_HRAM_START && addr < MEM_IE_REG)
        hram[addr - MEM_HRAM_START] = val;

    // IE Register
    if (addr == MEM_IE_REG)
        if (!ppu->oamDmaActive || bypass)
            ieRegister = val;
}

void Memory::writebit(uint8_t val, uint8_t bit, uint16_t addr, bool bypass, bool bypassOamDma)
{
    if (bit > 7) {
        fprintf(stderr, "WARNING: Trying to write to bit %d, which is not in range [0, 8)\n", bit);
        return;
    }

    uint8_t mask = val != 0 ? 1 << bit : ~(1 << bit);
    uint8_t byte = this->readmem(addr);
    byte = val != 0 ? byte | mask : byte & mask;

    this->writemem(byte, addr, bypass, bypassOamDma);
}

uint8_t Memory::getCurrentVramBank() { return ioRegisters[0xFF4F - MEM_IO_START] & 0x1; }

uint8_t Memory::getCurrentWramBank()
{
    uint8_t wramBank = ioRegisters[0xFF70 - MEM_IO_START] & 0x7;
    if (wramBank == 0x0)
        return 0x1;
    return wramBank;
}

void Memory::setCurrentVramBank(uint8_t val) { ioRegisters[0xFF4F - MEM_IO_START] = val & 0x1; }

void Memory::setCurrentWramBank(uint8_t val) { ioRegisters[0xFF70 - MEM_IO_START] = val & 0x3; }

uint8_t Memory::getLcdMode() { return (ioRegisters[0xFF41 - MEM_IO_START] & 0x2); }