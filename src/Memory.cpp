#include "Memory.hpp"

Memory::Memory(EmulatorMode mode)
{
    this->mode = mode;
    currentVramBank = 0;
    currentWramBank = 1;
    setCurrentVramBank(0);
    setCurrentWramBank(1);
}

uint8_t Memory::readmem(uint16_t addr)
{
    // ROM + External RAM
    if (addr < 0x8000 || (addr >= 0xA000 && addr < 0xC000))
        return rom->readmem(addr);

    // VRAM
    if (addr >= 0x8000 && addr < 0xA000) {
        currentVramBank = getCurrentVramBank();
        uint16_t actualAddr = (addr - 0x8000) + currentVramBank * 0x2000;
        return vram[actualAddr];
    }

    // WRAM Bank 0
    if (addr >= 0xC000 && addr < 0xD000)
        return wram[addr - 0xC000];

    // Switchable WRAM Bank
    if (addr >= 0xD000 && addr < 0xE000) {
        currentWramBank = getCurrentWramBank();
        uint16_t actualAddr = (addr - 0xD000) + currentWramBank * 0x1000;
        return wram[actualAddr];
    }

    // ECHO RAM
    if (addr >= 0xE000 && addr < 0xFE00) {
        if (addr >= 0xE000 && addr < 0xF000)
            return wram[addr - 0xE000];

        if (addr >= 0xF000 && addr < 0xFE00) {
            currentWramBank = getCurrentWramBank();
            uint16_t actualAddr = (addr - 0xF000) + currentWramBank * 0x1000;
            return wram[actualAddr];
        }
    }

    // OAM
    if (addr >= 0xFE00 && addr < 0xFEA0)
        return oam[addr - 0xFE00];

    // Unused range
    if (addr >= 0xFEA0 && addr < 0xFF00)
        return 0x00;

    // I/O Registers
    if (addr >= 0xFF00 && addr < 0xFF80) {
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

        return ioRegisters[addr - 0xFF00];
    }

    // HRAM
    if (addr >= 0xFF80 && addr < 0xFFFF)
        return hram[addr - 0xFF80];

    // IE Register
    if (addr == 0xFFFF)
        return ieRegister;

    // Invalid address
    fprintf(stderr, "WARNING: Trying to read from invalid memory address: 0x%04X\n", addr);
    return 0xFF;
}

void Memory::writemem(uint8_t val, uint16_t addr)
{
    // ROM + External RAM
    if (addr < 0x8000 || (addr >= 0xA000 && addr < 0xC000))
        rom->writemem(val, addr);

    // VRAM
    if (addr >= 0x8000 && addr < 0xA000) {
        currentVramBank = getCurrentVramBank();
        uint16_t actualAddr = (addr - 0x8000) + currentVramBank * 0x2000;
        vram[actualAddr] = val;
    }

    // WRAM Bank 0
    if (addr >= 0xC000 && addr < 0xD000)
        wram[addr - 0xC000] = val;

    // Switchable WRAM Bank
    if (addr >= 0xD000 && addr < 0xE000) {
        currentWramBank = getCurrentWramBank();
        uint16_t actualAddr = (addr - 0xD000) + currentWramBank * 0x1000;
        wram[actualAddr] = val;
    }

    // ECHO RAM
    if (addr >= 0xE000 && addr < 0xFE00) {
        if (addr >= 0xE000 && addr < 0xF000)
            wram[addr - 0xE000] = val;

        if (addr >= 0xF000 && addr < 0xFE00) {
            currentWramBank = getCurrentWramBank();
            uint16_t actualAddr = (addr - 0xF000) + currentWramBank * 0x1000;
            wram[actualAddr] = val;
        }
    }

    // OAM
    if (addr >= 0xFE00 && addr < 0xFEA0)
        oam[addr - 0xFE00] = val;

    // Unused range
    if (addr >= 0xFEA0 && addr < 0xFF00)
        return;

    // I/O Registers
    if (addr >= 0xFF00 && addr < 0xFF80) {
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
            ioRegisters[addr - 0xFF00] = val;
        }
    }

    // HRAM
    if (addr >= 0xFF80 && addr < 0xFFFF)
        hram[addr - 0xFF80] = val;

    // IE Register
    if (addr == 0xFFFF)
        ieRegister = val;
}

uint8_t Memory::getCurrentVramBank() { return ioRegisters[0xFF4F - 0xFF00] & 0x1; }

uint8_t Memory::getCurrentWramBank()
{
    uint8_t wramBank = ioRegisters[0xFF70 - 0xFF00] & 0x7;
    if (wramBank == 0x0)
        return 0x1;
    return wramBank;
}

void Memory::setCurrentVramBank(uint8_t val) { ioRegisters[0xFF4F - 0xFF00] = val & 0x1; }

void Memory::setCurrentWramBank(uint8_t val) { ioRegisters[0xFF70 - 0xFF00] = val & 0x3; }
