#include "Memory.hpp"

Memory::Memory(EmulatorMode mode)
{
    this->mode = mode;
    currentVramBank = 0;
    currentWramBank = 1;
    setCurrentVramBank(0);
    setCurrentWramBank(1);

    for (int i = 0; i < 0x40; ++i)
        cgbBgColorPalette[i] = 255;
}

uint8_t Memory::readmem(uint16_t addr)
{
    // ROM + External RAM
    if (addr < MEM_VRAM_START || (addr >= MEM_EXT_RAM_START && addr < MEM_WRAM0_START))
        return rom->readmem(addr);

    // VRAM
    if (addr >= MEM_VRAM_START && addr < MEM_EXT_RAM_START) {
        currentVramBank = getCurrentVramBank();
        uint16_t actualAddr = (addr - MEM_VRAM_START) + currentVramBank * 0x2000;
        return vram[actualAddr];
    }

    // WRAM Bank 0
    if (addr >= MEM_WRAM0_START && addr < MEM_WRAMX_START)
        return wram[addr - MEM_WRAM0_START];

    // Switchable WRAM Bank
    if (addr >= MEM_WRAMX_START && addr < MEM_ECHO_START) {
        currentWramBank = getCurrentWramBank();
        uint16_t actualAddr = (addr - MEM_WRAMX_START) + currentWramBank * 0x1000;
        return wram[actualAddr];
    }

    // ECHO RAM
    if (addr >= MEM_ECHO_START && addr < MEM_OAM_START) {
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

    // OAM
    if (addr >= MEM_OAM_START && addr < MEM_UNUSED_START)
        return oam[addr - MEM_OAM_START];

    // Unused range
    if (addr >= MEM_UNUSED_START && addr < MEM_IO_START)
        return 0x00;

    // I/O Registers
    if (addr >= MEM_IO_START && addr < MEM_HRAM_START) {
        if (addr == 0xFF69) {
            // BGPD (Backgroud Color Palette Data)
            uint8_t index = ppu->getBgColorPaletteIndex() & 0x3F;
            return cgbBgColorPalette[index];
        }

        if (addr == 0xFF6B) {
            // OBPD (Object Color Palette Data)
            uint8_t index = ppu->getObjColorPaletteIndex() & 0x3F;
            return cgbObjColorPalette[index];
        }

        return ioRegisters[addr - MEM_IO_START];
    }

    // HRAM
    if (addr >= MEM_HRAM_START && addr < MEM_IE_REG)
        return hram[addr - MEM_HRAM_START];

    // IE Register
    if (addr == MEM_IE_REG)
        return ieRegister;

    // Invalid address
    fprintf(stderr, "WARNING: Trying to read from invalid memory address: 0x%04X\n", addr);
    return 0xFF;
}

void Memory::writemem(uint8_t val, uint16_t addr)
{
    // ROM + External RAM
    if (addr < MEM_VRAM_START || (addr >= MEM_EXT_RAM_START && addr < MEM_WRAM0_START))
        rom->writemem(val, addr);

    // VRAM
    if (addr >= MEM_VRAM_START && addr < MEM_EXT_RAM_START) {
        currentVramBank = getCurrentVramBank();
        uint16_t actualAddr = (addr - MEM_VRAM_START) + currentVramBank * 0x2000;
        vram[actualAddr] = val;
    }

    // WRAM Bank 0
    if (addr >= MEM_WRAM0_START && addr < MEM_WRAMX_START)
        wram[addr - MEM_WRAM0_START] = val;

    // Switchable WRAM Bank
    if (addr >= MEM_WRAMX_START && addr < MEM_ECHO_START) {
        currentWramBank = getCurrentWramBank();
        uint16_t actualAddr = (addr - MEM_WRAMX_START) + currentWramBank * 0x1000;
        wram[actualAddr] = val;
    }

    // ECHO RAM
    if (addr >= MEM_ECHO_START && addr < MEM_OAM_START) {
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

    // OAM
    if (addr >= MEM_OAM_START && addr < MEM_UNUSED_START)
        oam[addr - MEM_OAM_START] = val;

    // Unused range
    if (addr >= MEM_UNUSED_START && addr < MEM_IO_START)
        return;

    // I/O Registers
    if (addr >= MEM_IO_START && addr < MEM_HRAM_START) {
        // TODO: Add special case for DMA Transfer (0xFF46) and others for CGB

        if (addr == 0xFF69) {
            // BGPD (Backgroud Color Palette Data)
            uint8_t index = ppu->getBgColorPaletteIndex();
            cgbBgColorPalette[index & 0x3F] = val;
            // AutoIncrement
            if (index & 0x80)
                ppu->setBgColorPaletteIndex(0x80 | (((index & 0x3F) + 1) & 0x3F));
        }

        if (addr == 0xFF6B) {
            // OBPD (Object Color Palette Data)
            uint8_t index = ppu->getObjColorPaletteIndex();
            cgbObjColorPalette[index & 0x3F] = val;
            // AutoIncrement
            if (index & 0x80)
                ppu->setObjColorPaletteIndex(0x80 | (((index & 0x3F) + 1) & 0x3F));
        }

        else {
            ioRegisters[addr - MEM_IO_START] = val;
        }
    }

    // HRAM
    if (addr >= MEM_HRAM_START && addr < MEM_IE_REG)
        hram[addr - MEM_HRAM_START] = val;

    // IE Register
    if (addr == MEM_IE_REG)
        ieRegister = val;
}

void Memory::writebit(uint8_t val, uint8_t bit, uint16_t addr)
{
    if (bit > 7)
        return;

    uint8_t mask = val != 0 ? 1 << bit : ~(1 << bit);
    uint8_t byte = this->readmem(addr);
    byte = val != 0 ? byte | mask : byte | mask;

    this->writemem(byte, addr);
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
