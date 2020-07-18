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

/* BIT OPERATIONS INSTRUCTIONS */

/**
 *  Test bit u3 in register r8, set 0 flag if bit not set
 *  Cycles: 2
 *  Length: 2
 *  Flags:
 *      Z: Set if selected bit is 0
 *      N: 0
 *      H: 1
 */
void SM83::op_bit_u3_r8(uint8_t u3, const uint8_t &r8)
{
    if (!checkInstructionCycle(2))
        return;

    uint8_t mask = 1 << u3;
    uint8_t bit = r8 & mask;

    setZeroFlag(bit == 0);
    setSubtractFlag(0);
    setHalfCarryFlag(1);

    endInstruction(2);
}

/**
 *  Test bit u3 in byte pointed by HL, set 0 flag if bit not set
 *  Cycles: 3
 *  Length: 2
 *  Flags:
 *      Z: Set if selected bit is 0
 *      N: 0
 *      H: 1
 */
void SM83::op_bit_u3_addr_hl(uint8_t u3)
{
    if (!checkInstructionCycle(3))
        return;

    uint16_t hl = (H << 0x8) | L;
    uint8_t byte = readmem_u8(hl);

    uint8_t mask = 1 << u3;
    uint8_t bit = byte & mask;

    setZeroFlag(bit == 0);
    setSubtractFlag(0);
    setHalfCarryFlag(1);

    endInstruction(2);
}

/**
 *  Set bit u3 in register r8 to 0
 *  Cycles: 2
 *  Length: 2
 *  Flags:
 *      None
 */
void SM83::op_res_u3_r8(uint8_t u3, uint8_t &r8)
{
    if (!checkInstructionCycle(2))
        return;

    uint8_t mask = ~(1 << u3);
    r8 &= mask;

    endInstruction(2);
}

/**
 *  Set bit u3 in byte pointed by HL to 0
 *  Cycles: 4
 *  Length: 2
 *  Flags:
 *      None
 */
void SM83::op_res_u3_addr_hl(uint8_t u3)
{
    if (!checkInstructionCycle(4))
        return;

    uint16_t hl = (H << 0x8) | L;
    uint8_t byte = readmem_u8(hl);

    uint8_t mask = ~(1 << u3);
    byte &= mask;

    writemem_u8(byte, hl);

    endInstruction(2);
}

/**
 *  Set bit u3 in register r8 to 1
 *  Cycles: 2
 *  Length: 2
 *  Flags:
 *      None
 */
void SM83::op_set_u3_r8(uint8_t u3, uint8_t &r8)
{
    if (!checkInstructionCycle(2))
        return;

    uint8_t mask = (1 << u3);
    r8 |= mask;

    endInstruction(2);
}

/**
 *  Set bit u3 in byte pointed by HL to 1
 *  Cycles: 4
 *  Length: 2
 *  Flags:
 *      None
 */
void SM83::op_set_u3_addr_hl(uint8_t u3)
{
    if (!checkInstructionCycle(4))
        return;

    uint16_t hl = (H << 0x8) | L;
    uint8_t byte = readmem_u8(hl);

    uint8_t mask = (1 << u3);
    byte |= mask;

    writemem_u8(byte, hl);

    endInstruction(2);
}

/**
 *  Swap upper 4 bits in register r8 and the lower 4 ones
 *  Cycles: 2
 *  Length: 2
 *  Flags:
 *      Z: Set if result is 0
 *      N: 0
 *      H: 0
 *      C: 0
 */
void SM83::op_swap_r8(uint8_t &r8)
{
    if (!checkInstructionCycle(2))
        return;

    r8 = ((r8 & 0xF0) >> 0x4) | ((r8 & 0x0F) << 0x4);

    setZeroFlag(r8 == 0);
    setSubtractFlag(0);
    setHalfCarryFlag(0);
    setCarryFlag(0);

    endInstruction(2);
}

/**
 *  Swap upper 4 bits in byte pointed by HL and the lower 4 ones
 *  Cycles: 4
 *  Length: 2
 *  Flags:
 *      Z: Set if result is 0
 *      N: 0
 *      H: 0
 *      C: 0
 */
void SM83::op_swap_addr_hl()
{
    if (!checkInstructionCycle(4))
        return;

    uint16_t hl = (H << 0x8) | L;
    uint8_t byte = readmem_u8(byte);

    byte = ((byte & 0xF0) >> 0x4) | ((byte & 0x0F) << 0x4);

    setZeroFlag(byte == 0);
    setSubtractFlag(0);
    setHalfCarryFlag(0);
    setCarryFlag(0);

    writemem_u8(byte, hl);

    endInstruction(2);
}

/* BIT SHIFT INSTRUCTIONS */

/**
 *  Rotate bits in register r8 left through carry
 *  Cycles: 2
 *  Length: 2
 *  Flags:
 *      Z: Set if result is 0
 *      N: 0
 *      H: 0
 *      C: Set according to result
 */
void SM83::op_rl_r8(uint8_t &r8)
{
    if (!checkInstructionCycle(2))
        return;

    uint8_t carry = getCarryFlag();

    setCarryFlag(r8 & 0x80);

    r8 = (r8 << 1) | carry;

    setZeroFlag(r8 == 0);
    setSubtractFlag(0);
    setHalfCarryFlag(0);

    endInstruction(2);
}

/**
 *  Rotate bits in byte pointed by HL left through carry
 *  Cycles: 4
 *  Length: 2
 *  Flags:
 *      Z: Set if result is 0
 *      N: 0
 *      H: 0
 *      C: Set according to result
 */
void SM83::op_rl_addr_hl()
{
    if (!checkInstructionCycle(4))
        return;

    uint16_t hl = (H << 0x8) | L;
    uint8_t byte = readmem_u8(hl);

    uint8_t carry = getCarryFlag();

    setCarryFlag(byte & 0x80);

    byte = (byte << 1) | carry;

    setZeroFlag(byte == 0);
    setSubtractFlag(0);
    setHalfCarryFlag(0);

    writemem_u8(byte, hl);

    endInstruction(2);
}

/**
 *  Rotate register A left through carry
 *  Cycles: 1
 *  Length: 1
 *  Flags:
 *      Z: 0
 *      N: 0
 *      H: 0
 *      C: Set according to result
 */
void SM83::op_rla()
{
    uint8_t carry = getCarryFlag();

    setCarryFlag(A & 0x80);

    A = (A << 1) | carry;

    setZeroFlag(0);
    setSubtractFlag(0);
    setHalfCarryFlag(0);

    endInstruction(1);
}

/**
 *  Rotate bits in register r8 left
 *  Cycles: 2
 *  Length: 2
 *  Flags:
 *      Z: Set if result is 0
 *      N: 0
 *      H: 0
 *      C: Set according to result
 */
void SM83::op_rlc_r8(uint8_t &r8)
{
    if (!checkInstructionCycle(2))
        return;

    uint8_t msb = (r8 & 0x80) >> 0x7;

    setCarryFlag(msb);

    r8 = (r8 << 1) | msb;

    setZeroFlag(r8 == 0);
    setSubtractFlag(0);
    setHalfCarryFlag(0);

    endInstruction(2);
}

/**
 *  Rotate bits in byte pointed by HL left
 *  Cycles: 4
 *  Length: 2
 *  Flags:
 *      Z: Set if result is 0
 *      N: 0
 *      H: 0
 *      C: Set according to result
 */
void SM83::op_rlc_addr_hl()
{
    if (!checkInstructionCycle(4))
        return;

    uint16_t hl = (H << 0x8) | L;
    uint8_t byte = readmem_u8(hl);

    uint8_t msb = (byte & 0x80) >> 0x7;

    setCarryFlag(msb);

    byte = (byte << 1) | msb;

    setZeroFlag(byte == 0);
    setSubtractFlag(0);
    setHalfCarryFlag(0);

    writemem_u8(byte, hl);

    endInstruction(2);
}

/**
 *  Rotate register A left
 *  Cycles: 1
 *  Length: 1
 *  Flags:
 *      Z: 0
 *      N: 0
 *      H: 0
 *      C: Set according to result
 */
void SM83::op_rlca()
{
    uint8_t msb = (A & 0x80) >> 0x7;

    setCarryFlag(msb);

    A = (A << 1) | msb;

    setZeroFlag(0);
    setSubtractFlag(0);
    setHalfCarryFlag(0);

    endInstruction(1);
}

/**
 *  Rotate register r8 right through carry
 *  Cycles: 2
 *  Length: 2
 *  Flags:
 *      Z: Set if result is 0
 *      N: 0
 *      H: 0
 *      C: Set according to result
 */
void SM83::op_rr_r8(uint8_t &r8)
{
    if (!checkInstructionCycle(2))
        return;

    uint8_t carry = getCarryFlag();

    setCarryFlag(r8 & 0x1);

    r8 = (r8 >> 1) | (carry << 7);

    setZeroFlag(r8 == 0);
    setSubtractFlag(0);
    setHalfCarryFlag(0);

    endInstruction(2);
}

/**
 *  Rotate byte pointed by HL right through carry
 *  Cycles: 4
 *  Length: 2
 *  Flags:
 *      Z: Set if result is 0
 *      N: 0
 *      H: 0
 *      C: Set according to result
 */
void SM83::op_rr_addr_hl()
{
    if (!checkInstructionCycle(4))
        return;

    uint16_t hl = (H << 0x8) | L;
    uint8_t byte = readmem_u8(hl);

    uint8_t carry = getCarryFlag();

    setCarryFlag(byte & 0x1);

    byte = (byte >> 1) | (carry << 7);

    setZeroFlag(byte == 0);
    setSubtractFlag(0);
    setHalfCarryFlag(0);

    writemem_u8(byte, hl);

    endInstruction(2);
}

/**
 *  Rotate register A right through carry
 *  Cycles: 1
 *  Length: 1
 *  Flags:
 *      Z: 0
 *      N: 0
 *      H: 0
 *      C: Set according to result
 */
void SM83::op_rra()
{
    uint8_t carry = getCarryFlag();

    setCarryFlag(A & 0x1);

    A = (A >> 1) | (carry << 7);

    setZeroFlag(0);
    setSubtractFlag(0);
    setHalfCarryFlag(0);

    endInstruction(1);
}

/**
 *  Rotate bits in register r8 right
 *  Cycles: 2
 *  Length: 2
 *  Flags:
 *      Z: Set if result is 0
 *      N: 0
 *      H: 0
 *      C: Set according to result
 */
void SM83::op_rrc_r8(uint8_t &r8)
{
    if (!checkInstructionCycle(2))
        return;

    uint8_t lsb = r8 & 0x1;

    setCarryFlag(lsb);

    r8 = (r8 >> 1) | (lsb << 7);

    setZeroFlag(r8 == 0);
    setSubtractFlag(0);
    setHalfCarryFlag(0);

    endInstruction(2);
}

/**
 *  Rotate bits in byte pointed by HL right
 *  Cycles: 4
 *  Length: 2
 *  Flags:
 *      Z: Set if result is 0
 *      N: 0
 *      H: 0
 *      C: Set according to result
 */
void SM83::op_rrc_addr_hl()
{
    if (!checkInstructionCycle(4))
        return;

    uint16_t hl = (H << 0x8) | L;
    uint8_t byte = readmem_u8(hl);

    uint8_t lsb = byte & 0x1;

    setCarryFlag(lsb);

    byte = (byte >> 1) | (lsb << 7);

    setZeroFlag(byte == 0);
    setSubtractFlag(0);
    setHalfCarryFlag(0);

    writemem_u8(byte, hl);

    endInstruction(2);
}

/**
 *  Rotate bits in register A right
 *  Cycles: 1
 *  Length: 1
 *  Flags:
 *      Z: 0
 *      N: 0
 *      H: 0
 *      C: Set according to result
 */
void SM83::op_rrca()
{
    uint8_t lsb = A & 0x1;

    setCarryFlag(lsb);

    A = (A >> 1) | (lsb << 7);

    setZeroFlag(0);
    setSubtractFlag(0);
    setHalfCarryFlag(0);

    endInstruction(1);
}

/**
 *  Shift left arithmetic register r8
 *  Cycles: 2
 *  Length: 2
 *  Flags:
 *      Z: Set if result is 0
 *      N: 0
 *      H: 0
 *      C: Set according to result
 */
void SM83::op_sla_r8(uint8_t &r8)
{
    if (!checkInstructionCycle(2))
        return;

    setCarryFlag(r8 & 0x80);

    r8 <<= 1;

    setZeroFlag(r8 == 0);
    setSubtractFlag(0);
    setHalfCarryFlag(0);

    endInstruction(2);
}

/**
 *  Shift left arithmetic byte pointed by HL
 *  Cycles: 4
 *  Length: 2
 *  Flags:
 *      Z: Set if result is 0
 *      N: 0
 *      H: 0
 *      C: Set according to result
 */
void SM83::op_sla_addr_hl()
{
    if (!checkInstructionCycle(4))
        return;

    uint16_t hl = (H << 0x8) | L;
    uint8_t byte = readmem_u8(hl);

    setCarryFlag(byte & 0x80);

    byte <<= 1;

    setZeroFlag(byte == 0);
    setSubtractFlag(0);
    setHalfCarryFlag(0);

    writemem_u8(byte, hl);

    endInstruction(2);
}

/**
 *  Shift right arithmetic register r8
 *  Cycles: 2
 *  Length: 2
 *  Flags:
 *      Z: Set if result is 0
 *      N: 0
 *      H: 0
 *      C: Set according to result
 */
void SM83::op_sra_r8(uint8_t &r8)
{
    if (!checkInstructionCycle(2))
        return;

    setCarryFlag(r8 & 0x1);

    uint8_t msb = r8 & 0x80;

    r8 = (r8 >> 1) | msb;

    setZeroFlag(r8 == 0);
    setSubtractFlag(0);
    setHalfCarryFlag(0);

    endInstruction(2);
}

/**
 *  Shift right arithmetic byte pointed by HL
 *  Cycles: 4
 *  Length: 2
 *  Flags:
 *      Z: Set if result is 0
 *      N: 0
 *      H: 0
 *      C: Set according to result
 */
void SM83::op_sra_addr_hl()
{
    if (!checkInstructionCycle(4))
        return;

    uint16_t hl = (H << 0x8) | L;
    uint8_t byte = readmem_u8(hl);

    setCarryFlag(byte & 0x80);

    uint8_t msb = byte & 0x80;

    byte = (byte >> 1) | msb;

    setZeroFlag(byte == 0);
    setSubtractFlag(0);
    setHalfCarryFlag(0);

    writemem_u8(byte, hl);

    endInstruction(2);
}

/**
 *  Shift right logic register r8
 *  Cycles: 2
 *  Length: 2
 *  Flags:
 *      Z: Set if result is 0
 *      N: 0
 *      H: 0
 *      C: Set according to result
 */
void SM83::op_srl_r8(uint8_t &r8)
{
    if (!checkInstructionCycle(2))
        return;

    setCarryFlag(r8 & 0x1);

    r8 >>= 1;

    setZeroFlag(r8 == 0);
    setSubtractFlag(0);
    setHalfCarryFlag(0);

    endInstruction(2);
}

/**
 *  Shift right logic byte pointed by HL
 *  Cycles: 4
 *  Length: 2
 *  Flags:
 *      Z: Set if result is 0
 *      N: 0
 *      H: 0
 *      C: Set according to result
 */
void SM83::op_srl_addr_hl()
{
    if (!checkInstructionCycle(4))
        return;

    uint16_t hl = (H << 0x8) | L;
    uint8_t byte = readmem_u8(hl);

    setCarryFlag(byte & 0x1);

    byte >>= 1;

    setZeroFlag(byte == 0);
    setSubtractFlag(0);
    setHalfCarryFlag(0);

    writemem_u8(byte, hl);

    endInstruction(2);
}

/* LOAD INSTRUCTIONS */

/**
 *  Load value in register r8_2 into register r8_1
 *  Cycles: 1
 *  Length: 1
 *  Flags:
 *      None
 */
void SM83::op_ld_r8_r8(uint8_t &r8_1, const uint8_t &r8_2)
{
    r8_1 = r8_2;

    endInstruction(1);
}

/**
 *  Load value n8 into register r8
 *  Cycles: 2
 *  Length: 2
 *  Flags:
 *      None
 */
void SM83::op_ld_r8_n8(uint8_t &r8)
{
    if (!checkInstructionCycle(2))
        return;

    uint8_t n8 = readmem_u8(PC + 1);

    r8 = n8;

    endInstruction(2);
}

/**
 *  Load value n16 into register r16
 *  Cycles: 3
 *  Length: 3
 *  Flags:
 *      None
 */
void SM83::op_ld_r16_n16(uint8_t &r16_high, uint8_t &r16_low)
{
    if (!checkInstructionCycle(3))
        return;

    uint16_t n16 = readmem_u16(PC + 1);

    r16_high = (n16 & 0xFF00) >> 0x8;
    r16_low = n16 & 0x00FF;

    endInstruction(3);
}

/**
 *  Store value in r8 into byte pointed by HL
 *  Cycles: 2
 *  Length: 1
 *  Flags:
 *      None
 */
void SM83::op_ld_addr_hl_r8(const uint8_t &r8)
{
    if (!checkInstructionCycle(2))
        return;

    uint16_t hl = (H << 0x8) | L;

    writemem_u8(r8, hl);

    endInstruction(1);
}

/**
 *  Store value n8 into byte pointed by HL
 *  Cycles: 3
 *  Length: 2
 *  Flags:
 *      None
 */
void SM83::op_ld_addr_hl_n8()
{
    if (!checkInstructionCycle(3))
        return;

    uint16_t hl = (H << 0x8) | L;
    uint8_t n8 = readmem_u8(PC + 1);

    writemem_u8(n8, hl);

    endInstruction(2);
}

/**
 *  Load value into register r8 from byte pointed by HL
 *  Cycles: 2
 *  Length: 1
 *  Flags:
 *      None
 */
void SM83::op_ld_r8_addr_hl(uint8_t &r8)
{
    if (!checkInstructionCycle(2))
        return;

    uint16_t hl = (H << 0x8) | L;

    r8 = readmem_u8(hl);

    endInstruction(1);
}

/**
 *  Store value in register A into byte pointed by r16
 *  Cycles: 2
 *  Length: 1
 *  Flags:
 *      None
 */
void SM83::op_ld_addr_r16_a(const uint8_t &r16_high, const uint8_t &r16_low)
{
    if (!checkInstructionCycle(2))
        return;

    uint16_t r16 = (r16_high << 0x8) | r16_low;

    writemem_u8(A, r16);

    endInstruction(1);
}

/**
 *  Store value in register A into byte at address n16
 *  Cycles: 4
 *  Length: 3
 *  Flags:
 *      None
 */
void SM83::op_ld_addr_n16_a()
{
    if (!checkInstructionCycle(4))
        return;

    uint16_t n16 = readmem_u16(PC + 1);

    writemem_u8(A, n16);

    endInstruction(3);
}

/**
 *  Store value in register A into byte at address n16, n16 between 0xFF00-0xFFFF
 *  Cycles: 3
 *  Length: 2
 *  Flags:
 *      None
 */
void SM83::op_ldh_addr_n16_a()
{
    if (!checkInstructionCycle(3))
        return;

    uint8_t n8 = readmem_u8(PC + 1);

    writemem_u8(A, 0xFF00 + n8);

    endInstruction(2);
}

/**
 *  Store value in register A into byte at address 0xFF00 + C
 *  Cycles: 2
 *  Length: 1
 *  Flags:
 *      None
 */
void SM83::op_ldh_addr_c_a()
{
    if (!checkInstructionCycle(2))
        return;

    writemem_u8(A, 0xFF00 + C);

    endInstruction(1);
}

/**
 *  Load value in register A from byte pointed by r16
 *  Cycles: 2
 *  Length: 1
 *  Flags:
 *      None
 */
void SM83::op_ld_a_addr_r16(const uint8_t &r16_high, const uint8_t &r16_low)
{
    if (!checkInstructionCycle(2))
        return;

    uint16_t r16 = (r16_high << 0x8) | r16_low;

    A = readmem_u8(r16);

    endInstruction(1);
}

/**
 *  Load value in register A from byte pointed by n16
 *  Cycles: 4
 *  Length: 3
 *  Flags:
 *      None
 */
void SM83::op_ld_a_addr_n16()
{
    if (!checkInstructionCycle(4))
        return;

    uint16_t n16 = readmem_u16(PC + 1);

    A = readmem_u8(n16);

    endInstruction(3);
}

/**
 *  Load value in register A from byte at address n16, n16 between 0xFF00-0xFFFF
 *  Cycles: 3
 *  Length: 2
 *  Flags:
 *      None
 */
void SM83::op_ldh_a_addr_n16()
{
    if (!checkInstructionCycle(3))
        return;

    uint8_t n8 = readmem_u8(PC + 1);

    A = readmem_u8(0xFF00 + n8);

    endInstruction(2);
}

/**
 *  Load value in register A from byte at address 0xFF00 + C
 *  Cycles: 2
 *  Length: 1
 *  Flags:
 *      None
 */
void SM83::op_ldh_a_addr_c()
{
    if (!checkInstructionCycle(2))
        return;

    A = readmem_u8(0xFF00 + C);

    endInstruction(1);
}

/**
 *  Store value in register A into byte pointed by HL and increment HL
 *  Cycles: 2
 *  Length: 1
 *  Flags:
 *      None
 */
void SM83::op_ld_addr_hl_inc_a()
{
    if (!checkInstructionCycle(2))
        return;

    uint16_t hl = (H << 0x8) | L;

    writemem_u8(A, hl++);

    H = (hl & 0xFF00) >> 0x8;
    L = hl & 0x00FF;

    endInstruction(1);
}

/**
 *  Store value in register A into byte pointed by HL and decrement HL
 *  Cycles: 2
 *  Length: 1
 *  Flags:
 *      None
 */
void SM83::op_ld_addr_hl_dec_a()
{
    if (!checkInstructionCycle(2))
        return;

    uint16_t hl = (H << 0x8) | L;

    writemem_u8(A, hl--);

    H = (hl & 0xFF00) >> 0x8;
    L = hl & 0x00FF;

    endInstruction(1);
}

/**
 *  Load value in register A from byte pointed by HL and increment HL
 *  Cycles: 2
 *  Length: 1
 *  Flags:
 *      None
 */
void SM83::op_ld_a_addr_hl_inc()
{
    if (!checkInstructionCycle(2))
        return;

    uint16_t hl = (H << 0x8) | L;

    A = readmem_u8(hl++);

    H = (hl & 0xFF00) >> 0x8;
    L = hl & 0x00FF;

    endInstruction(1);
}

/**
 *  Load value in register A from byte pointed by HL and decrement HL
 *  Cycles: 2
 *  Length: 1
 *  Flags:
 *      None
 */
void SM83::op_ld_a_addr_hl_dec()
{
    if (!checkInstructionCycle(2))
        return;

    uint16_t hl = (H << 0x8) | L;

    A = readmem_u8(hl--);

    H = (hl & 0xFF00) >> 0x8;
    L = hl & 0x00FF;

    endInstruction(1);
}

/* JUMP AND SUBROUTINE INSTRUCTIONS */

/**
 *  Call address n16. Push the address of the instruction after the call on the stack
 *      and jump to n16
 *  Cycles: 6
 *  Length: 3
 *  Flags:
 *      None
 */
void SM83::op_call_n16()
{
    if (!checkInstructionCycle(6))
        return;

    uint16_t n16 = readmem_u16(PC + 1);
    uint16_t ret_addr = PC + 3;

    writemem_u8(ret_addr & 0xFF, --SP);
    writemem_u8((ret_addr & 0xFF00) >> 0x8, --SP);

    PC = n16;

    endInstruction(0);
}

/**
 *  Call address n16 if condition cc is met. Push the address of the instruction
 *      after the call on the stack and jump to n16
 *  Cycles: 6 taken / 3 untaken
 *  Length: 3
 *  Flags:
 *      None
 */
void SM83::op_call_cc_n16(uint8_t flag_bit, uint8_t required_value)
{
    if (!checkInstructionCycle(3))
        return;

    if (flag_bit != required_value)
        endInstruction(3);

    if (!checkInstructionCycle(6))
        return;

    uint16_t n16 = readmem_u16(PC + 1);
    uint16_t ret_addr = PC + 3;

    writemem_u8(ret_addr & 0xFF, --SP);
    writemem_u8((ret_addr & 0xFF00) >> 0x8, --SP);

    PC = n16;

    endInstruction(0);
}

/**
 *  Jump to address in HL
 *  Cycles: 1
 *  Length: 1
 *  Flags:
 *      None
 */
void SM83::op_jp_hl()
{
    uint16_t hl = (H << 0x8) | L;

    PC = hl;

    endInstruction(0);
}

/**
 *  Jump to address n16
 *  Cycles: 4
 *  Length: 3
 *  Flags:
 *      None
 */
void SM83::op_jp_n16()
{
    if (!checkInstructionCycle(4))
        return;

    uint16_t n16 = readmem_u16(PC + 1);

    PC = n16;

    endInstruction(0);
}

/**
 *  Jump to address n16 if condition cc is met
 *  Cycles: 4 taken / 3 untaken
 *  Length: 3
 *  Flags:
 *      None
 */
void SM83::op_jp_cc_n16(uint8_t flag_bit, uint8_t required_value)
{
    if (!checkInstructionCycle(3))
        return;

    if (flag_bit != required_value)
        endInstruction(3);

    if (!checkInstructionCycle(4))
        return;

    uint16_t n16 = readmem_u16(PC + 1);

    PC = n16;

    endInstruction(0);
}

/**
 *  Relative jump by adding e8 to the address of the instruction following the jr.
 *      e8 is signed byte
 *  Cycles: 3
 *  Length: 2
 *  Flags:
 *      None
 */
void SM83::op_jr_e8()
{
    if (!checkInstructionCycle(3))
        return;

    uint8_t ue8 = readmem_u8(PC + 1);
    int8_t e8 = *(int8_t *)&ue8;

    uint16_t addr = PC + 2;

    PC = addr + e8;

    endInstruction(0);
}

/**
 *  Relative jump if cc is met by adding e8 to the address of the instruction following the jr.
 *      e8 is signed byte
 *  Cycles: 3 taken / 2 untaken
 *  Length: 2
 *  Flags:
 *      None
 */
void SM83::op_jr_cc_e8(uint8_t flag_bit, uint8_t required_value)
{
    if (!checkInstructionCycle(2))
        return;

    if (flag_bit != required_value)
        endInstruction(2);

    if (!checkInstructionCycle(3))
        return;

    uint8_t ue8 = readmem_u8(PC + 1);
    int8_t e8 = *(int8_t *)&ue8;

    uint16_t addr = PC + 2;

    PC = addr + e8;

    endInstruction(0);
}

/**
 *  Return from subroutine if condition cc is met
 *  Cycles: 5 taken / 2 untaken
 *  Length: 1
 *  Flags:
 *      None
 */
void SM83::op_ret_cc(uint8_t flag_bit, uint8_t required_value)
{
    if (!checkInstructionCycle(2))
        return;

    if (flag_bit != required_value)
        endInstruction(1);

    if (!checkInstructionCycle(5))
        return;

    uint8_t ret_addr_low, ret_addr_high;
    ret_addr_low = readmem_u8(SP++);
    ret_addr_high = readmem_u8(SP++);

    PC = (ret_addr_high << 0x8) | ret_addr_low;

    endInstruction(0);
}

/**
 *  Return from subroutine
 *  Cycles: 4
 *  Length: 1
 *  Flags:
 *      None
 */
void SM83::op_ret()
{
    if (!checkInstructionCycle(4))
        return;

    uint8_t ret_addr_low, ret_addr_high;
    ret_addr_low = readmem_u8(SP++);
    ret_addr_high = readmem_u8(SP++);

    PC = (ret_addr_high << 0x8) | ret_addr_low;

    endInstruction(0);
}

/**
 *  Return from subroutine and enable interrupts
 *  Cycles: 4
 *  Length: 1
 *  Flags:
 *      None
 */
void SM83::op_reti()
{
    if (!checkInstructionCycle(4))
        return;

    setInterruptEnable(0xFF);

    uint8_t ret_addr_low, ret_addr_high;
    ret_addr_low = readmem_u8(SP++);
    ret_addr_high = readmem_u8(SP++);

    PC = (ret_addr_high << 0x8) | ret_addr_low;

    endInstruction(0);
}

/**
 *  Call address vec
 *  Cycles: 4
 *  Length: 1
 *  Flags:
 *      None
 */
void SM83::op_rst(uint16_t vec)
{
    if (!checkInstructionCycle(4))
        return;

    uint16_t ret_addr = PC + 1;

    writemem_u8(ret_addr & 0xFF, --SP);
    writemem_u8((ret_addr & 0xFF00) >> 0x8, --SP);

    PC = vec;

    endInstruction(0);
}

/* STACK OPERATIONS INSTRUCTIONS */

/**
 *  Add the value in SP to HL
 *  Cycles: 2
 *  Length: 1
 *  Flags:
 *      N: 0
 *      H: Set if overflow from bit 11
 *      C: Set if overflow from bit 15
 */
void SM83::op_add_hl_sp()
{
    if (!checkInstructionCycle(2))
        return;

    uint16_t hl = (H << 0x8) | L;

    uint32_t result = hl + SP;

    setSubtractFlag(0);
    setHalfCarryFlag((hl & 0xFFF) + (SP & 0xFFF) > 0xFFF);
    setCarryFlag(result > 0xFFFF);

    H = (result & 0xFF00) >> 0x8;
    L = result & 0x00FF;

    endInstruction(1);
}

/**
 *  Add signed value e8 to SP
 *  Cycles: 4
 *  Length: 2
 *  Flags:
 *      Z: 0
 *      N: 0
 *      H: Set if overflow from bit 3
 *      C: Set if overflow from bit 7
 */
void SM83::op_add_sp_e8()
{
    if (!checkInstructionCycle(4))
        return;

    uint8_t ue8 = readmem_u8(PC + 1);
    int8_t e8 = *(int8_t *)&ue8;

    uint32_t result = SP + e8;

    setZeroFlag(0);
    setSubtractFlag(0);
    setHalfCarryFlag((SP ^ e8 ^ (result & 0xFFFF) & 0x10) == 0x10);
    setCarryFlag((SP ^ e8 ^ (result & 0xFFFF) & 0x100) == 0x100);

    SP = result;

    endInstruction(2);
}

/**
 *  Decrement value in SP by 1
 *  Cycles: 2
 *  Length: 1
 *  Flags:
 *      None
 */
void SM83::op_dec_sp()
{
    if (!checkInstructionCycle(2))
        return;

    --SP;

    endInstruction(1);
}

/**
 *  Increment value in SP by 1
 *  Cycles: 2
 *  Length: 1
 *  Flags:
 *      None
 */
void SM83::op_inc_sp()
{
    if (!checkInstructionCycle(2))
        return;

    ++SP;

    endInstruction(1);
}

/**
 *  Load value n16 into SP
 *  Cycles: 3
 *  Length: 3
 *  Flags:
 *      None
 */
void SM83::op_ld_sp_n16()
{
    if (!checkInstructionCycle(3))
        return;

    uint16_t n16 = readmem_u16(PC + 1);

    SP = n16;

    endInstruction(3);
}

/**
 *  Store SP at address n16
 *  Cycles: 5
 *  Length: 3
 *  Flags:
 *      None
 */
void SM83::op_ld_addr_n16_sp()
{
    if (!checkInstructionCycle(5))
        return;

    uint16_t n16 = readmem_u16(PC + 1);

    writemem_u16(SP, n16);

    endInstruction(3);
}

/**
 *  Add the signed value e8 to SP and store the result in HL
 *  Cycles: 3
 *  Length: 2
 *  Flags:
 *      Z: 0
 *      N: 0
 *      H: Set if overflow from bit 3
 *      C: Set if overflow from bit 7
 */
void SM83::op_ld_hl_sp_plus_e8()
{
    if (!checkInstructionCycle(3))
        return;

    uint8_t ue8 = readmem_u8(PC + 1);
    int8_t e8 = *(int8_t *)&ue8;

    uint32_t result = SP + e8;

    setZeroFlag(0);
    setSubtractFlag(0);
    setHalfCarryFlag((SP ^ e8 ^ (result & 0xFFFF) & 0x10) == 0x10);
    setCarryFlag((SP ^ e8 ^ (result & 0xFFFF) & 0x100) == 0x100);

    SP = result;

    uint16_t hl = (H << 0x8) | L;

    writemem_u16(SP, hl);

    endInstruction(2);
}

/**
 *  Load register HL into SP
 *  Cycles: 2
 *  Length: 1
 *  Flags:
 *      None
 */
void SM83::op_ld_sp_hl()
{
    if (!checkInstructionCycle(2))
        return;

    uint16_t hl = (H << 0x8) | L;
    SP = hl;

    endInstruction(1);
}

/**
 *  Pop register AF from stack
 *  Cycles: 3
 *  Length: 1
 *  Flags:
 *      Z: Set from bit 7 of popped low byte
 *      N: Set from bit 6 of popped low byte
 *      H: Set from bit 5 of popped low byte
 *      C: Set from bit 4 of popped low byte
 *      (These flags will be automatically set when the F register is loaded)
 */
void SM83::op_pop_af()
{
    if (!checkInstructionCycle(3))
        return;

    A = readmem_u8(SP++);
    F = readmem_u8(SP++);

    endInstruction(1);
}

/**
 *  Pop register r16 from the stack
 *  Cycles: 3
 *  Length: 1
 *  Flags:
 *      None
 */
void SM83::op_pop_r16(uint8_t &r16_high, uint8_t &r16_low)
{
    if (!checkInstructionCycle(3))
        return;

    r16_low = readmem_u8(SP++);
    r16_high = readmem_u8(SP++);

    endInstruction(1);
}

/**
 *  Push register AF into the stack
 *  Cycles: 4
 *  Length: 1
 *  Flags:
 *      None
 */
void SM83::op_push_af()
{
    if (!checkInstructionCycle(4))
        return;

    writemem_u8(A, --SP);
    writemem_u8(F, --SP);

    endInstruction(1);
}

/**
 *  Push register r16 into the stack
 *  Cycles: 4
 *  Length: 1
 *  Flags:
 *      None
 */
void SM83::op_push_r16(const uint8_t &r16_high, const uint8_t &r16_low)
{
    if (!checkInstructionCycle(4))
        return;

    writemem_u8(r16_high, --SP);
    writemem_u8(r16_low, --SP);

    endInstruction(1);
}

/* MISC INSTRUCTIONS */

/**
 *  Complement carry flag
 *  Cycles: 1
 *  Length: 1
 *  Flags:
 *      C: Inverted
 */
void SM83::op_ccf()
{
    uint8_t carry = getCarryFlag();

    setCarryFlag((~carry) & 0x1);

    endInstruction(1);
}

/**
 *  Complement Accumulator
 *  Cycles: 1
 *  Length: 1
 *  Flags:
 *      N: 1
 *      H: 1
 */
void SM83::op_cpl()
{
    A = ~A;

    endInstruction(1);
}

/**
 *  Decimal Adjust Accumulator to get a correct BCD representation after an arithmetic intruction
 *  Cycles: 1
 *  Length: 1
 *  Flags:
 *      Z: Set if result is 0
 *      H: 0
 *      C: Set or reset depending on operation
 */
void SM83::op_daa()
{
    uint8_t subtractFlag = getSubtractFlag();

    if (!subtractFlag) {
        // Addition
        if (getCarryFlag() || A > 0x99) {
            A += 0x60;
            setCarryFlag(1);
        }
        if (getHalfCarryFlag() || (A & 0x0F) > 0x09) {
            A += 0x06;
        }
    } else {
        // Subtraction
        if (getCarryFlag())
            A -= 0x60;

        if (getHalfCarryFlag())
            A -= 0x06;
    }

    setZeroFlag(A == 0);
    setHalfCarryFlag(0);

    endInstruction(1);
}

/**
 *  Disable Interrupts by clearing IME flag
 *  Cycles: 1
 *  Length: 1
 *  Flags:
 *      None
 */
void SM83::op_di()
{
    setInterruptEnable(0);

    endInstruction(1);
}

/**
 *  Enable Interrupts by setting the IME flag. The flag is set only after the instruction following
 * EI Cycles: 1 Length: 1 Flags: None
 */
void SM83::op_ei()
{
    ei_enable = 2;

    endInstruction(1);
}

/**
 *  Enter CPU low-power mode until an interrupt occurs
 *  If IME is set: The CPU enters low-power mode until after an interrupt is about to be serviced
 *                 The handler is executed normally and the CPU resumes execution after the HALT
 *                 when that returns
 *  If IME is not set: The behaviour depends wether an interrupt is pending:
 *                          If none pending: As soon as an interrupt becomes pending
 *                          the CPU resumes execution. The handler is not called
 *                          If some pending: The CPU continues esxecution after the HALT
 *                          but the byte after is read twice in a row (PC is not incremented)
 *  Cycles: -
 *  Length: 1
 *  Flags:
 *      None
 */
void SM83::op_halt()
{
    // TODO
}

/**
 *  No OPeration
 *  Cycles: 1
 *  Length: 1
 *  Flags:
 *      None
 */
void SM83::op_nop() { endInstruction(1); }

/**
 *  Set carry flag
 *  Cycles: 1
 *  Length: 1
 *  Flags:
 *      N: 0
 *      H: 0
 *      C: 1
 */
void SM83::op_scf()
{
    setSubtractFlag(0);
    setHalfCarryFlag(0);
    setCarryFlag(1);

    endInstruction(1);
}

/**
 *  Enter CPU very low power mode. Also used to switch between x1 and x2 speed CPU mode in GBC
 *  Cycles: 1
 *  Length: 2
 *  Flags:
 *      None
 */
void SM83::op_stop()
{
    // TODO
}