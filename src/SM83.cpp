#include "SM83.hpp"

SM83::SM83() { int_cycles = -1; }

/**
 *  Main CPU loop
 */
void SM83::cycle()
{
    // Check if halted and not servicing an interrupt
    if (halted && int_cycles < 0) {
        // Check for pending interrupts
        // If IME = 1 then service the interrupt
        bool pending = false;
        if (ime == 1)
            pending = checkInterrupts(&int_cycles, &int_addr);
        // If IME = 0 do not service the interrupt; do not clear flags
        else
            pending = checkInterrupts(&int_cycles, nullptr);

        // If there aren't pending interrupts, continue to halt
        if (!pending)
            return;
    }

    // Check if an interrupt is being serviced and service it
    // If servicing interrupt, exit cycle
    if (int_cycles >= 0) {
        serviceInterrupt();
        return;
    }

    // Fetch opcode
    uint8_t opcode = readmem_u8(PC);

    // If the halt bug occurs, decrement PC so that it will be used twice
    if (halt_bug)
        --PC;

    // Decode and execute
    executeOpcode(opcode);

    if (halt_bug) {
        // If the instruction has not ended, increment PC so that it will be decremented on the next
        // cycle after the fetch, thus reading the correct current instruction opcode
        if (instructionCycle > 0)
            ++PC;

        // If the instruction has ended, then disable the halt bug
        else if (instructionCycle == 0)
            halt_bug = false;
    }

    // Check for IME enable after EI instruction
    if (instructionCycle == 0)
        if (ei_enable == 1)
            setImeFlag(1);
    if (ei_enable > 0 && instructionCycle == 0)
        --ei_enable;

    // If an instruction was just finished, check for interrupts
    if (instructionCycle == 0)
        handleInterrupts();
}

void SM83::handleInterrupts()
{
    // Exit if IME is disabled
    if (!ime)
        return;

    // Check for interrupts, going from LSB to MSB (VBlank -> Joypad)
    checkInterrupts(&int_cycles, &int_addr);
}

/**
 *  Check if there are pending interrupts. If yes, return true, otherwise, return false
 *  If there is a pending interrupt and the params are not nullptr, set them to their respective
 * values
 *  If IME is 0, int_addr should be nullptr
 */
bool SM83::checkInterrupts(int8_t *int_cycles, uint16_t *int_addr)
{
    // VBlank
    if (getVBlankInterruptEnable() && getVBlankInterruptFlag()) {
        if (int_addr != nullptr) {
            *int_cycles = 5;
            *int_addr = SM83_VBLANK_INT;
            setVBlankInterruptFlag(0);
        }

        if (ime == 0)
            *int_cycles = 4;

        return true;
    }

    // LCD_STAT
    if (getLCDSTATInterruptEnable() && getLCDSTATInterruptFlag()) {
        if (int_addr != nullptr) {
            *int_cycles = 5;
            *int_addr = SM83_LCD_STAT_INT;
            setLCDSTATInterruptFlag(0);
        }

        if (ime == 0)
            *int_cycles = 4;

        return true;
    }

    // Timer
    if (getTimerInterruptEnable() && getTimerInterruptFlag()) {
        if (int_addr != nullptr) {
            *int_cycles = 5;
            *int_addr = SM83_TIMER_INT;
            setTimerInterruptFlag(0);
        }

        if (ime == 0)
            *int_cycles = 4;

        return true;
    }

    // Serial
    if (getSerialInterruptEnable() && getSerialInterruptFlag()) {
        if (int_addr != nullptr) {
            *int_cycles = 5;
            *int_addr = SM83_SERIAL_INT;
            setSerialInterruptFlag(0);
        }

        if (ime == 0)
            *int_cycles = 4;

        return true;
    }

    // Joypad
    if (getJoypadInterruptEnable() && getJoypadInterruptFlag()) {
        if (int_addr != nullptr) {
            *int_cycles = 5;
            *int_addr = SM83_JOYPAD_INT;
            setJoypadInterruptFlag(0);
        }

        if (ime == 0)
            *int_cycles = 4;

        return true;
    }

    return false;
}

/**
 *  Services interrupt based on int_cycles and int_addr
 *  Should be called only when int_cycles >= 0
 *  int_cycles will never be 0; it will skip 0 and go to -1
 */
bool SM83::serviceInterrupt()
{
    --int_cycles;
    if (int_cycles > 0)
        return false;

    // Signal that there is no need to service an interrupt
    int_cycles = -1;

    // If IME is 0, do not jump
    if (ime == 0)
        return true;

    setImeFlag(0);

    // Push current PC on the stack
    writemem_u8((PC & 0xFF00) >> 0x8, --SP);
    writemem_u8(PC & 0xFF, --SP);

    PC = int_addr;

    return true;
}

/* MEMORY READ AND WRITE */

uint8_t SM83::readmem_u8(uint16_t addr) { return memory->readmem(addr); }

uint16_t SM83::readmem_u16(uint16_t addr)
{
    return memory->readmem(addr) | (memory->readmem(addr + 1) << 0x8);
}

void SM83::writemem_u8(uint8_t val, uint16_t addr) { memory->writemem(val, addr); }

void SM83::writemem_u16(uint16_t val, uint16_t addr)
{
    memory->writemem(val & 0x00FF, addr);
    memory->writemem((val & 0xFF00) >> 0x8, addr + 1);
}

/**
 *  Returns true if the current instruction cycle is the last cycle, based on the parameter.
 *  It also returns true if the instructionCycle is greated than the parameter
 *  Automatically increments instructionCycle if increment flag is not specified
 */
bool SM83::checkInstructionCycle(uint8_t opcode_cycles, bool increment)
{
    if (increment)
        return (++instructionCycle) >= opcode_cycles;
    else
        return instructionCycle >= opcode_cycles;
}

/**
 *  Sets the instructionCycle var to 0 and increments the PC based on the opcode_length
 *  This should be called on the last cycle of the instruction, after all operations have finished
 */
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
