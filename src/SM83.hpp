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

    /* 8 BIT ARITHMETIC INSTRUCTIONS */
    void op_adc_a_r8(const uint8_t &r8);
    void op_adc_a_addr_hl();
    void op_adc_a_n8();
    void op_add_a_r8(const uint8_t &r8);
    void op_add_a_addr_hl();
    void op_add_a_n8();
    void op_dec_r8(uint8_t &r8);
    void op_dec_addr_hl();
    void op_inc_r8(uint8_t &r8);
    void op_inc_addr_hl();
    void op_sbc_a_r8(const uint8_t &r8);
    void op_sbc_a_addr_hl();
    void op_sbc_a_n8();
    void op_sub_a_r8(const uint8_t &r8);
    void op_sub_a_addr_hl();
    void op_sub_a_n8();

    /* 8 BIT LOGIC INSTRUCTIONS */
    void op_and_a_r8(const uint8_t &r8);
    void op_and_a_addr_hl();
    void op_and_a_n8();
    void op_cp_a_r8(const uint8_t &r8);
    void op_cp_a_addr_hl();
    void op_cp_a_n8();
    void op_or_a_r8(const uint8_t &r8);
    void op_or_a_addr_hl();
    void op_or_a_n8();
    void op_xor_a_r8(const uint8_t &r8);
    void op_xor_a_addr_hl();
    void op_xor_a_n8();

    /* 16 BIT ARITHMETIC INSTRUCTIONS */
    void op_add_hl_r16(const uint8_t &r16_high, const uint8_t &r16_low);
    void op_dec_r16(uint8_t &r16);
    void op_inc_r16(uint8_t &r16);

    /* BIT OPERATIONS INSTRUCTIONS */
    void op_bit_u3_r8(uint8_t u3, const uint8_t &r8);
    void op_bit_u3_addr_hl(uint8_t u3);
    void op_res_u3_r8(uint8_t u3, uint8_t &r8);
    void op_res_u3_addr_hl(uint8_t u3);
    void op_set_u3_r8(uint8_t u3, uint8_t &r8);
    void op_set_u3_addr_hl(uint8_t u3);
    void op_swap_r8(uint8_t &r8);
    void op_swap_addr_hl();

    /* BIT SHIFT INSTRUCTIONS */
    void op_rl_r8(uint8_t &r8);
    void op_rl_addr_hl();
    void op_rla();
    void op_rlc_r8(uint8_t &r8);
    void op_rlc_addr_hl();
    void op_rlca();
    void op_rr_r8(uint8_t &r8);
    void op_rr_addr_hl();
    void op_rra();
    void op_rrc_r8(uint8_t &r8);
    void op_rrc_addr_hl();
    void op_rrca();
    void op_sla_r8(uint8_t &r8);
    void op_sla_addr_hl();
    void op_sra_r8(uint8_t &r8);
    void op_sra_addr_hl();
    void op_srl_r8(uint8_t &r8);
    void op_srl_addr_hl();

    /* LOAD INSTRUCTIONS */
    void op_ld_r8_r8(uint8_t &r8_1, const uint8_t &r8_2);
    void op_ld_r8_n8(uint8_t &r8);
    void op_ld_r16_n16(uint8_t &r16_high, uint8_t &r16_low);
    void op_ld_addr_hl_r8(const uint8_t &r8);
    void op_ld_addr_hl_n8();
    void op_ld_r8_addr_hl(uint8_t &r8);
    void op_ld_addr_r16_a(const uint8_t &r16_high, const uint8_t &r16_low);
    void op_ld_addr_n16_a();
    void op_ldh_addr_n16_a();
    void op_ldh_addr_c_a();
    void op_ld_a_addr_r16(const uint8_t &r16_high, const uint8_t &r16_low);
    void op_ld_a_addr_n16();
    void op_ldh_a_addr_n16();
    void op_ldh_a_addr_c();
    void op_ld_addr_hl_inc_a();
    void op_ld_addr_hl_dec_a();
    void op_ld_a_addr_hl_inc();
    void op_ld_a_addr_hl_dec();

    /* JUMP AND SUBROUTINE INSTRUCTIONS */
    void op_call_n16();
    void op_call_cc_n16(uint8_t flag_bit, uint8_t required_value);
    void op_jp_hl();
    void op_jp_n16();
    void op_jp_cc_n16(uint8_t flag_bit, uint8_t required_value);
    void op_jr_e8();
    void op_jr_cc_e8(uint8_t flag_bit, uint8_t required_value);
    void op_ret_cc(uint8_t flag_bit, uint8_t required_value);
    void op_ret();
    void op_reti();
    void op_rst(uint16_t addr);

    /* STACK OPERATIONS INSTRUCTIONS */
    void op_add_hl_sp();
    void op_add_sp_e8();
    void op_dec_sp();
    void op_inc_sp();
    void op_ld_sp_n16();
    void op_ld_addr_n16_sp();
    void op_ld_hl_sp_plus_e8();
    void op_ld_sp_hl();
    void op_pop_af();
    void op_pop_r16(uint8_t &r16_high, uint8_t &r16_low);
    void op_push_af();
    void op_push_r16(const uint8_t &r16_high, const uint8_t &r16_low);

    /* MISC INSTRUCTIONS */
    void op_ccf();
    void op_cpl();
    void op_daa();
    void op_di();
    void op_ei();
    void op_halt();
    void op_nop();
    void op_scf();
    void op_stop();

    /* OPCODES */
};

#endif // __SM83_H__