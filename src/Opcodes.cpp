#include "SM83.hpp"

/* 8 BIT ARITHMETIC INSTRUCTIONS */

/**
 *  Add the value in r8 plus the carry flag to A.
 *  Cycles: 1
 *  Length: 1
 *  Flags:
 *      Z: Set if result is 0
 *      N: 0
 *      H: Set if overflow from bit 3
 *      C: Set if overflow from bit 7
 */
void SM83::op_adc_a_r8(const uint8_t &r8)
{
    uint8_t carry = getCarryFlag();
    uint16_t result = A + r8 + carry;

    setZeroFlag((result & 0xFF) == 0);
    setSubtractFlag(0);
    setHalfCarryFlag(((A & 0xF) + (r8 & 0xF) + carry) > 0xF);
    setCarryFlag(result > 0xFF);

    A = result & 0xFF;

    endInstruction(1);
}

/**
 *  Add the byte pointed by HL plus the carry flag to A.
 *  Cycles: 2
 *  Length: 1
 *  Flags:
 *      Z: Set if result is 0
 *      N: 0
 *      H: Set if overflow from bit 3
 *      C: Set if overflow from bit 7
 */
void SM83::op_adc_a_addr_hl()
{
    if (!checkInstructionCycle(2))
        return;

    uint16_t hl = (H << 0x8) | L;
    // uint8_t byte = memory[hl];
    uint8_t byte = readmem_u8(hl);

    uint8_t carry = getCarryFlag();
    uint16_t result = A + byte + carry;

    setZeroFlag((result & 0xFF) == 0);
    setSubtractFlag(0);
    setHalfCarryFlag(((A & 0xF) + (byte & 0xF) + carry) > 0xF);
    setCarryFlag(result > 0xFF);

    A = result & 0xFF;

    endInstruction(1);
}

/**
 *  Add the value n8 plus the carry flag to A.
 *  Cycles: 2
 *  Length: 2
 *  Flags:
 *      Z: Set if result is 0
 *      N: 0
 *      H: Set if overflow from bit 3
 *      C: Set if overflow from bit 7
 */
void SM83::op_adc_a_n8()
{
    if (!checkInstructionCycle(2))
        return;

    // uint8_t n8 = memory[PC + 1];
    uint8_t n8 = readmem_u8(PC + 1);

    uint8_t carry = getCarryFlag();
    uint16_t result = A + n8 + carry;

    setZeroFlag((result & 0xFF) == 0);
    setSubtractFlag(0);
    setHalfCarryFlag(((A & 0xF) + (n8 & 0xF) + carry) > 0xF);
    setCarryFlag(result > 0xFF);

    A = result & 0xFF;

    endInstruction(2);
}

/**
 *  Add the value in r8 to A.
 *  Cycles: 1
 *  Length: 1
 *  Flags:
 *      Z: Set if result is 0
 *      N: 0
 *      H: Set if overflow from bit 3
 *      C: Set if overflow from bit 7
 */
void SM83::op_add_a_r8(const uint8_t &r8)
{
    uint16_t result = A + r8;

    setZeroFlag((result & 0xFF) == 0);
    setSubtractFlag(0);
    setHalfCarryFlag(((A & 0xF) + (r8 & 0xF)) > 0xF);
    setCarryFlag(result > 0xFF);

    A = result & 0xFF;

    endInstruction(1);
}

/**
 *  Add the byte pointed by HL to A.
 *  Cycles: 2
 *  Length: 1
 *  Flags:
 *      Z: Set if result is 0
 *      N: 0
 *      H: Set if overflow from bit 3
 *      C: Set if overflow from bit 7
 */
void SM83::op_add_a_addr_hl()
{
    if (!checkInstructionCycle(2))
        return;

    uint16_t hl = (H << 0x8) | L;
    // uint8_t byte = memory[hl];
    uint8_t byte = readmem_u8(hl);

    uint16_t result = A + byte;

    setZeroFlag((result & 0xFF) == 0);
    setSubtractFlag(0);
    setHalfCarryFlag(((A & 0xF) + (byte & 0xF)) > 0xF);
    setCarryFlag(result > 0xFF);

    A = result & 0xFF;

    endInstruction(1);
}

/**
 *  Add the value n8 to A.
 *  Cycles: 2
 *  Length: 2
 *  Flags:
 *      Z: Set if result is 0
 *      N: 0
 *      H: Set if overflow from bit 3
 *      C: Set if overflow from bit 7
 */
void SM83::op_add_a_n8()
{
    if (!checkInstructionCycle(2))
        return;

    // uint8_t n8 = memory[PC + 1];
    uint8_t n8 = readmem_u8(PC + 1);

    uint16_t result = A + n8;

    setZeroFlag((result & 0xFF) == 0);
    setSubtractFlag(0);
    setHalfCarryFlag(((A & 0xF) + (n8 & 0xF)) > 0xF);
    setCarryFlag(result > 0xFF);

    A = result & 0xFF;

    endInstruction(2);
}

/**
 *  Decrement value in r8 by 1.
 *  Cycles: 1
 *  Length: 1
 *  Flags:
 *      Z: Set if result is 0
 *      N: 1
 *      H: Set if borrow from bit 4
 */
void SM83::op_dec_r8(uint8_t &r8)
{
    --r8;

    setZeroFlag(r8 == 0);
    setSubtractFlag(1);
    setHalfCarryFlag((r8 & 0xF) == 0xF);

    endInstruction(1);
}

/**
 *  Decrement byte pointed by HL by 1.
 *  Cycles: 3
 *  Length: 1
 *  Flags:
 *      Z: Set if result is 0
 *      N: 1
 *      H: Set if borrow from bit 4
 */
void SM83::op_dec_addr_hl()
{
    if (!checkInstructionCycle(3))
        return;

    uint16_t hl = (H << 0x8) | L;
    // uint8_t &byte = *(memory + hl);
    uint8_t byte = readmem_u8(hl);
    writemem_u8(--byte, hl);
    // --byte;

    setZeroFlag(byte == 0);
    setSubtractFlag(1);
    setHalfCarryFlag((byte & 0xF) == 0xF);

    endInstruction(1);
}

/**
 *  Increment value in r8 by 1.
 *  Cycles: 1
 *  Length: 1
 *  Flags:
 *      Z: Set if result is 0
 *      N: 0
 *      H: Set if overflow from bit 3
 */
void SM83::op_inc_r8(uint8_t &r8)
{
    ++r8;

    setZeroFlag(r8 == 0);
    setSubtractFlag(0);
    setHalfCarryFlag((r8 & 0xF) == 0x0);

    endInstruction(1);
}

/**
 *  Increment byte pointed by HL by 1.
 *  Cycles: 3
 *  Length: 1
 *  Flags:
 *      Z: Set if result is 0
 *      N: 0
 *      H: Set if overflow from bit 3
 */
void SM83::op_inc_addr_hl()
{
    if (!checkInstructionCycle(3))
        return;

    uint16_t hl = (H << 0x8) | L;
    // uint8_t &byte = *(memory + hl);
    uint8_t byte = readmem_u8(hl);
    writemem_u8(++byte, hl);

    //++byte;

    setZeroFlag(byte == 0);
    setSubtractFlag(0);
    setHalfCarryFlag((byte & 0xF) == 0x0);

    endInstruction(1);
}

/**
 *  Subtract the value in r8 and the carry flag from A.
 *  Cycles: 1
 *  Length: 1
 *  Flags:
 *      Z: Set if result is 0
 *      N: 1
 *      H: Set if borrow from bit 4
 *      C: Set if borrow
 */
void SM83::op_sbc_a_r8(const uint8_t &r8)
{
    uint8_t carry = getCarryFlag();
    uint8_t result = A - r8 - carry;

    setZeroFlag(result == 0);
    setSubtractFlag(1);
    setHalfCarryFlag((A & 0xF) < (r8 & 0xF) + carry);
    setCarryFlag(A < r8 + carry);

    A = result;

    endInstruction(1);
}

/**
 *  Subtract the byte pointed by HL and the carry flag from A.
 *  Cycles: 2
 *  Length: 1
 *  Flags:
 *      Z: Set if result is 0
 *      N: 1
 *      H: Set if borrow from bit 4
 *      C: Set if borrow
 */
void SM83::op_sbc_a_addr_hl()
{
    if (!checkInstructionCycle(2))
        return;

    uint16_t hl = (H << 0x8) | L;
    // uint8_t byte = memory[hl];
    uint8_t byte = readmem_u8(hl);

    uint8_t carry = getCarryFlag();
    uint8_t result = A - byte - carry;

    setZeroFlag(result == 0);
    setSubtractFlag(1);
    setHalfCarryFlag((A & 0xF) < (byte & 0xF) + carry);
    setCarryFlag(A < byte + carry);

    A = result;

    endInstruction(1);
}

/**
 *  Subtract the value n8 and the carry flag from A.
 *  Cycles: 2
 *  Length: 2
 *  Flags:
 *      Z: Set if result is 0
 *      N: 1
 *      H: Set if borrow from bit 4
 *      C: Set if borrow
 */
void SM83::op_sbc_a_n8()
{
    if (!checkInstructionCycle(2))
        return;

    // uint8_t n8 = memory[PC + 1];
    uint8_t n8 = readmem_u8(PC + 1);

    uint8_t carry = getCarryFlag();
    uint8_t result = A - n8 - carry;

    setZeroFlag(result == 0);
    setSubtractFlag(1);
    setHalfCarryFlag((A & 0xF) < (n8 & 0xF) + carry);
    setCarryFlag(A < n8 + carry);

    A = result;

    endInstruction(2);
}

/**
 *  Subtract the value in r8 from A.
 *  Cycles: 1
 *  Length: 1
 *  Flags:
 *      Z: Set if result is 0
 *      N: 1
 *      H: Set if borrow from bit 4
 *      C: Set if borrow
 */
void SM83::op_sub_a_r8(const uint8_t &r8)
{
    uint8_t result = A - r8;

    setZeroFlag(result == 0);
    setSubtractFlag(1);
    setHalfCarryFlag((A & 0xF) < (r8 & 0xF));
    setCarryFlag(A < r8);

    A = result;

    endInstruction(1);
}

/**
 *  Subtract the byte pointed by HL from A.
 *  Cycles: 2
 *  Length: 1
 *  Flags:
 *      Z: Set if result is 0
 *      N: 1
 *      H: Set if borrow from bit 4
 *      C: Set if borrow
 */
void SM83::op_sub_a_addr_hl()
{
    if (!checkInstructionCycle(2))
        return;

    uint16_t hl = (H << 0x8) | L;
    uint8_t byte = readmem_u8(hl);

    uint8_t result = A - byte;

    setZeroFlag(result == 0);
    setSubtractFlag(1);
    setHalfCarryFlag((A & 0xF) < (byte & 0xF));
    setCarryFlag(A < byte);

    A = result;

    endInstruction(1);
}

/**
 *  Subtract the value n8 from A.
 *  Cycles: 2
 *  Length: 2
 *  Flags:
 *      Z: Set if result is 0
 *      N: 1
 *      H: Set if borrow from bit 4
 *      C: Set if borrow
 */
void SM83::op_sub_a_n8()
{
    if (!checkInstructionCycle(2))
        return;

    uint8_t n8 = readmem_u8(PC + 1);
    uint8_t result = A - n8;

    setZeroFlag(result == 0);
    setSubtractFlag(1);
    setHalfCarryFlag((A & 0xF) < (n8 & 0xF));
    setCarryFlag(A < n8);

    A = result;

    endInstruction(2);
}

/* 8 BIT LOGIC INSTRUCTIONS */

/**
 *  Bitwise AND between the value in r8 and A
 *  Cycles: 1
 *  Length: 1
 *  Flags:
 *      Z: Set if result is 0
 *      N: 0
 *      H: 1
 *      C: 0
 */
void SM83::op_and_a_r8(const uint8_t &r8)
{
    A &= r8;

    setZeroFlag(A == 0);
    setSubtractFlag(0);
    setHalfCarryFlag(1);
    setCarryFlag(0);

    endInstruction(1);
}

/**
 *  Bitwise AND between the byte pointed by HL and A
 *  Cycles: 2
 *  Length: 1
 *  Flags:
 *      Z: Set if result is 0
 *      N: 0
 *      H: 1
 *      C: 0
 */
void SM83::op_and_a_addr_hl()
{
    if (!checkInstructionCycle(2))
        return;

    uint16_t hl = (H << 0x8) | L;
    uint8_t byte = readmem_u8(hl);

    A &= byte;

    setZeroFlag(A == 0);
    setSubtractFlag(0);
    setHalfCarryFlag(1);
    setCarryFlag(0);

    endInstruction(1);
}

/**
 *  Bitwise AND between the value n8 and A
 *  Cycles: 2
 *  Length: 2
 *  Flags:
 *      Z: Set if result is 0
 *      N: 0
 *      H: 1
 *      C: 0
 */
void SM83::op_and_a_n8()
{
    if (!checkInstructionCycle(2))
        return;

    uint8_t n8 = readmem_u8(PC + 1);

    A &= n8;

    setZeroFlag(A == 0);
    setSubtractFlag(0);
    setHalfCarryFlag(1);
    setCarryFlag(0);

    endInstruction(2);
}

/**
 *  Subtract the value in r8 from A and sets the flags, do not store the result
 *  Cycles: 1
 *  Length: 1
 *  Flags:
 *      Z: Set if result is 0
 *      N: 1
 *      H: Set if borrow from bit 4
 *      C: Set if borrow
 */
void SM83::op_cp_a_r8(const uint8_t &r8)
{
    uint8_t result = A - r8;

    setZeroFlag(result == 0);
    setSubtractFlag(1);
    setHalfCarryFlag((A & 0xF) < (r8 & 0xF));
    setCarryFlag(A < r8);

    endInstruction(1);
}

/**
 *  Subtract the byte pointed by HL from A and sets the flags, do not store the result
 *  Cycles: 2
 *  Length: 1
 *  Flags:
 *      Z: Set if result is 0
 *      N: 1
 *      H: Set if borrow from bit 4
 *      C: Set if borrow
 */
void SM83::op_cp_a_addr_hl()
{
    if (!checkInstructionCycle(2))
        return;

    uint16_t hl = (H << 0x8) | L;
    uint8_t byte = readmem_u8(hl);

    uint8_t result = A - byte;

    setZeroFlag(result == 0);
    setSubtractFlag(1);
    setHalfCarryFlag((A & 0xF) < (byte & 0xF));
    setCarryFlag(A < byte);

    endInstruction(1);
}

/**
 *  Subtract the value n8 from A and sets the flags, do not store the result
 *  Cycles: 2
 *  Length: 2
 *  Flags:
 *      Z: Set if result is 0
 *      N: 1
 *      H: Set if borrow from bit 4
 *      C: Set if borrow
 */
void SM83::op_cp_a_n8()
{
    if (!checkInstructionCycle(2))
        return;

    uint8_t n8 = readmem_u8(PC + 1);

    uint8_t result = A - n8;

    setZeroFlag(result == 0);
    setSubtractFlag(1);
    setHalfCarryFlag((A & 0xF) < (n8 & 0xF));
    setCarryFlag(A < n8);

    endInstruction(2);
}

/**
 *  Store in A the bitwise OR of the value in r8 and A
 *  Cycles: 1
 *  Length: 1
 *  Flags:
 *      Z: Set if result is 0
 *      N: 0
 *      H: 0
 *      C: 0
 */
void SM83::op_or_a_r8(const uint8_t &r8)
{
    A |= r8;

    setZeroFlag(A == 0);
    setSubtractFlag(0);
    setHalfCarryFlag(0);
    setCarryFlag(0);

    endInstruction(1);
}

/**
 *  Store in A the bitwise OR of the byte pointed by HL and A
 *  Cycles: 2
 *  Length: 1
 *  Flags:
 *      Z: Set if result is 0
 *      N: 0
 *      H: 0
 *      C: 0
 */
void SM83::op_or_a_addr_hl()
{
    if (!checkInstructionCycle(2))
        return;

    uint16_t hl = (H << 0x8) | L;
    uint8_t byte = readmem_u8(hl);

    A |= byte;

    setZeroFlag(A == 0);
    setSubtractFlag(0);
    setHalfCarryFlag(0);
    setCarryFlag(0);

    endInstruction(1);
}

/**
 *  Store in A the bitwise OR of the value n8 and A
 *  Cycles: 2
 *  Length: 2
 *  Flags:
 *      Z: Set if result is 0
 *      N: 0
 *      H: 0
 *      C: 0
 */
void SM83::op_or_a_n8()
{
    if (!checkInstructionCycle(2))
        return;

    uint8_t n8 = readmem_u8(PC + 1);

    A |= n8;

    setZeroFlag(A == 0);
    setSubtractFlag(0);
    setHalfCarryFlag(0);
    setCarryFlag(0);

    endInstruction(2);
}

/**
 *  Bitwise XOR between the value in r8 and A
 *  Cycles: 1
 *  Length: 1
 *  Flags:
 *      Z: Set if result is 0
 *      N: 0
 *      H: 0
 *      C: 0
 */
void SM83::op_xor_a_r8(const uint8_t &r8)
{
    A ^= r8;

    setZeroFlag(A == 0);
    setSubtractFlag(0);
    setHalfCarryFlag(0);
    setCarryFlag(0);

    endInstruction(1);
}

/**
 *  Bitwise XOR between the byte pointed by HL and A
 *  Cycles: 2
 *  Length: 1
 *  Flags:
 *      Z: Set if result is 0
 *      N: 0
 *      H: 0
 *      C: 0
 */
void SM83::op_xor_a_addr_hl()
{
    if (!checkInstructionCycle(2))
        return;

    uint16_t hl = (H << 0x8) | L;
    uint8_t byte = readmem_u8(hl);

    A ^= byte;

    setZeroFlag(A == 0);
    setSubtractFlag(0);
    setHalfCarryFlag(0);
    setCarryFlag(0);

    endInstruction(1);
}

/**
 *  Bitwise XOR between the value n8 and A
 *  Cycles: 2
 *  Length: 2
 *  Flags:
 *      Z: Set if result is 0
 *      N: 0
 *      H: 0
 *      C: 0
 */
void SM83::op_xor_a_n8()
{
    if (!checkInstructionCycle(2))
        return;

    uint8_t n8 = readmem_u8(PC + 1);

    A ^= n8;

    setZeroFlag(A == 0);
    setSubtractFlag(0);
    setHalfCarryFlag(0);
    setCarryFlag(0);

    endInstruction(2);
}

/* 16 BIT ARITHMETIC INSTRUCTIONS */

/**
 *  Add the value in r16 to HL
 *  Cycles: 2
 *  Length: 1
 *  Flags:
 *      N: 0
 *      H: Set if overflow from bit 11
 *      C: Set if overflow from bit 15
 */
void SM83::op_add_hl_r16(const uint8_t &r16_high, const uint8_t &r16_low)
{
    if (!checkInstructionCycle(2))
        return;

    uint16_t r16 = (r16_high << 0x8) | r16_low;
    uint16_t hl = (H << 0x8) | L;

    uint32_t result = hl + r16;

    setSubtractFlag(0);
    setHalfCarryFlag((hl & 0xFFF) + (r16 & 0xFFF) > 0xFFF);
    setCarryFlag(result > 0xFFFF);

    H = (result & 0xFF00) >> 0x8;
    L = result & 0x00FF;

    endInstruction(1);
}

/**
 *  Decrement the value in r16 by 1
 *  Cycles: 2
 *  Length: 1
 *  Flags:
 *      None
 */
void SM83::op_dec_r16(uint8_t &r16_high, uint8_t &r16_low)
{
    if (!checkInstructionCycle(2))
        return;

    uint16_t r16 = (r16_high << 0x8) | r16_low;
    --r16;

    r16_high = (r16 & 0xFF00) >> 0x8;
    r16_low = r16 & 0x00FF;

    endInstruction(1);
}

/**
 *  Increment the value in r16 by 1
 *  Cycles: 2
 *  Length: 1
 *  Flags:
 *      None
 */
void SM83::op_inc_r16(uint8_t &r16_high, uint8_t &r16_low)
{
    if (!checkInstructionCycle(2))
        return;

    uint16_t r16 = (r16_high << 0x8) | r16_low;
    ++r16;

    r16_high = (r16 & 0xFF00) >> 0x8;
    r16_low = r16 & 0x00FF;

    endInstruction(1);
}