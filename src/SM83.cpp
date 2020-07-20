#include "SM83.hpp"

void SM83::cycle()
{
    // Check if an interrupt is being called
    if (int_cycles > 0) {
        --int_cycles;
        return;
    } else if (int_cycles == 0) {
        setImeFlag(0);

        // Push current PC on the stack
        writemem_u8(PC & 0xFF, --SP);
        writemem_u8((PC & 0xFF00) >> 0x8, --SP);

        PC = int_addr;

        --int_cycles;
        return;
    }

    uint8_t opcode = readmem_u8(PC);

    executeOpcode(opcode);

    // Check for IME enable after EI instruction
    if (instructionCycle == 0)
        if (ei_enable == 1)
            setImeFlag(1);
    if (ei_enable > 0)
        --ei_enable;

    if (instructionCycle == 0)
        handleInterrupts();
}

void SM83::handleInterrupts()
{
    // Exit if IME is disabled
    if (!ime)
        return;

    // Check for interrupts, going from LSB to MSB (VBlank -> Joypad)
    // VBlank
    if (getVBlankInterruptEnable() && getVBlankInterruptFlag()) {
        int_cycles = 4;
        int_addr = SM83_VBLANK_INT;
        setVBlankInterruptFlag(0);
        return;
    }

    // LCD_STAT
    if (getLCDSTATInterruptEnable() && getLCDSTATInterruptFlag()) {
        int_cycles = 4;
        int_addr = SM83_LCD_STAT_INT;
        setLCDSTATInterruptFlag(0);
        return;
    }
    
    // Timer
    if (getTimerInterruptEnable() && getTimerInterruptFlag()) {
        int_cycles = 4;
        int_addr = SM83_TIMER_INT;
        setTimerInterruptFlag(0);
        return;
    }
    
    // Serial
    if (getSerialInterruptEnable() && getSerialInterruptFlag()) {
        int_cycles = 4;
        int_addr = SM83_SERIAL_INT;
        setSerialInterruptFlag(0);
        return;
    }
    
    // Joypad
    if (getJoypadInterruptEnable() && getJoypadInterruptFlag()) {
        int_cycles = 4;
        int_addr = SM83_JOYPAD_INT;
        setJoypadInterruptFlag(0);
        return;
    }
}

bool SM83::checkInstructionCycle(uint8_t opcode_cycles)
{
    return instructionCycle++ == opcode_cycles;
}

void SM83::endInstruction(uint8_t opcode_length)
{
    instructionCycle = 0;
    PC += opcode_length;
}

/* FLAG GETTERS AND SETTERS */

uint8_t SM83::getZeroFlag() { return (F & 0x80) >> 0x7; }

uint8_t SM83::getSubtractFlag() { return (F & 0x40) >> 0x6; }

uint8_t SM83::getHalfCarryFlag() { return (F & 0x20) >> 0x5; }

uint8_t SM83::getCarryFlag() { return (F & 0x10) >> 0x4; }

void SM83::setZeroFlag(uint8_t value)
{
    uint8_t mask = value != 0 ? 1 << 0x7 : ~(1 << 0x7);

    F = value != 0 ? F | mask : F & mask;
}

void SM83::setSubtractFlag(uint8_t value)
{
    uint8_t mask = value != 0 ? 1 << 0x6 : ~(1 << 0x6);

    F = value != 0 ? F | mask : F & mask;
}

void SM83::setHalfCarryFlag(uint8_t value)
{
    uint8_t mask = value != 0 ? 1 << 0x5 : ~(1 << 0x5);

    F = value != 0 ? F | mask : F & mask;
}

void SM83::setCarryFlag(uint8_t value)
{
    uint8_t mask = value != 0 ? 1 << 0x4 : ~(1 << 0x4);

    F = value != 0 ? F | mask : F & mask;
}

/* INTERRUPT FLAGS GETTERS AND SETTERS */

uint8_t SM83::getInterruptEnable() { return readmem_u8(0xFFFF); }

uint8_t SM83::getVBlankInterruptEnable() { return getInterruptEnable() & 0x1; }

uint8_t SM83::getLCDSTATInterruptEnable() { return (getInterruptEnable() & 0x2) >> 0x1; }

uint8_t SM83::getTimerInterruptEnable() { return (getInterruptEnable() & 0x4) >> 0x2; }

uint8_t SM83::getSerialInterruptEnable() { return (getInterruptEnable() & 0x8) >> 0x3; }

uint8_t SM83::getJoypadInterruptEnable() { return (getInterruptEnable() & 0x10) >> 0x4; }

uint8_t SM83::getInterruptFlag() { return readmem_u8(0xFF0F); }

uint8_t SM83::getVBlankInterruptFlag() { return getInterruptFlag() & 0x1; }

uint8_t SM83::getLCDSTATInterruptFlag() { return (getInterruptFlag() & 0x2) >> 0x1; }

uint8_t SM83::getTimerInterruptFlag() { return (getInterruptFlag() & 0x4) >> 0x2; }

uint8_t SM83::getSerialInterruptFlag() { return (getInterruptFlag() & 0x8) >> 0x3; }

uint8_t SM83::getJoypadInterruptFlag() { return (getInterruptFlag() & 0x10) >> 0x4; }

void SM83::setImeFlag(uint8_t value) { ime = value != 0 ? 1 : 0; }

void SM83::setInterruptEnable(uint8_t value) { writemem_u8(value, 0xFFFF); }

void SM83::setVBlankInterruptEnable(uint8_t value)
{
    uint8_t mask = value != 0 ? 1 : ~1;
    uint8_t ie = getInterruptEnable();

    ie = value != 0 ? ie | mask : ie & mask;
    setInterruptEnable(ie);
}

void SM83::setLCDSTATInterruptEnable(uint8_t value)
{
    uint8_t mask = value != 0 ? 1 << 0x1 : ~(1 << 0x1);
    uint8_t ie = getInterruptEnable();

    ie = value != 0 ? ie | mask : ie & mask;
    setInterruptEnable(ie);
}

void SM83::setTimerInterruptEnable(uint8_t value)
{
    uint8_t mask = value != 0 ? 1 << 0x2 : ~(1 << 0x2);
    uint8_t ie = getInterruptEnable();

    ie = value != 0 ? ie | mask : ie & mask;
    setInterruptEnable(ie);
}

void SM83::setSerialInterruptEnable(uint8_t value)
{
    uint8_t mask = value != 0 ? 1 << 0x3 : ~(1 << 0x3);
    uint8_t ie = getInterruptEnable();

    ie = value != 0 ? ie | mask : ie & mask;
    setInterruptEnable(ie);
}

void SM83::setJoypadInterruptEnable(uint8_t value)
{
    uint8_t mask = value != 0 ? 1 << 0x4 : ~(1 << 0x4);
    uint8_t ie = getInterruptEnable();

    ie = value != 0 ? ie | mask : ie & mask;
    setInterruptEnable(ie);
}

void SM83::setInterruptFlag(uint8_t value) { writemem_u8(value, 0xFF0F); }

void SM83::setVBlankInterruptFlag(uint8_t value)
{
    uint8_t mask = value != 0 ? 1 : ~1;
    uint8_t iflag = getInterruptFlag();

    iflag = value != 0 ? iflag | mask : iflag & mask;
    setInterruptFlag(iflag);
}

void SM83::setLCDSTATInterruptFlag(uint8_t value)
{
    uint8_t mask = value != 0 ? 1 << 0x1 : ~(1 << 0x1);
    uint8_t iflag = getInterruptFlag();

    iflag = value != 0 ? iflag | mask : iflag & mask;
    setInterruptFlag(iflag);
}

void SM83::setTimerInterruptFlag(uint8_t value)
{
    uint8_t mask = value != 0 ? 1 << 0x2 : ~(1 << 0x2);
    uint8_t iflag = getInterruptFlag();

    iflag = value != 0 ? iflag | mask : iflag & mask;
    setInterruptFlag(iflag);
}

void SM83::setSerialInterruptFlag(uint8_t value)
{
    uint8_t mask = value != 0 ? 1 << 0x3 : ~(1 << 0x3);
    uint8_t iflag = getInterruptFlag();

    iflag = value != 0 ? iflag | mask : iflag & mask;
    setInterruptFlag(iflag);
}

void SM83::setJoypadInterruptFlag(uint8_t value)
{
    uint8_t mask = value != 0 ? 1 << 0x4 : ~(1 << 0x4);
    uint8_t iflag = getInterruptFlag();

    iflag = value != 0 ? iflag | mask : iflag & mask;
    setInterruptFlag(iflag);
}
