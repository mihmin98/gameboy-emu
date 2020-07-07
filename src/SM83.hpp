#ifndef __SM83_H__
#define __SM83_H__

#pragma once
#include "GameBoy.hpp"

class SM83
{
  public:
    uint8_t *memory;
    uint8_t A, F, B, C, D, E, H, L;
    uint16_t PC, SP;
    uint8_t instructionCycle;

    SM83(uint8_t *memory);
    void InitRegisters();
    void Cycle();

    /* OPCODE INSTRUCTIONS */

    /* 8 BIT LOAD INSTRUCIONS */

    void op_ld_r1_r2(uint8_t &r1, const uint8_t &r2);
    void op_ld_r_n(uint8_t &r);
    void op_ld_r_addr_r16(uint8_t &r1, const uint8_t &r2_high, const uint8_t &r2_low);
    void op_ld_addr_r16_r(const uint8_t &r1_high, const uint8_t &r1_low, const uint8_t &r2);
    void op_ld_addr_r16_n(const uint8_t &r_high, const uint8_t &r_low);
    void op_ld_r_addr_nn(uint8_t &r);
    void op_ld_addr_nn_r(const uint8_t &r);
    void op_ldh_r_addr_r8(uint8_t &r1, const uint8_t &r2);
    void op_ldh_addr_r8_r(const uint8_t &r1, const uint8_t &r2);
    void op_ldh_r_addr_n(uint8_t &r);
    void op_ldh_addr_n_r(const uint8_t &r);
    void op_ld_r_addr_r16_dec(uint8_t &r1, uint8_t &r2_high, uint8_t &r2_low);
    void op_ld_addr_r16_dec_r(uint8_t &r1_high, uint8_t &r1_low, const uint8_t &r2);
    void op_ld_r_addr_r16_inc(uint8_t &r1, uint8_t &r2_high, uint8_t &r2_low);
    void op_ld_addr_r16_inc_r(uint8_t &r1_high, uint8_t &r1_low, const uint8_t &r2);

    /* 16 BIT LOAD INSTRUCIONS */
    void op_ld_rr_nn(uint8_t &r_high, uint8_t &r_low);
    void op_ld_addr_nn_sp();
    

    /* OPCODES */
};

#endif // __SM83_H__