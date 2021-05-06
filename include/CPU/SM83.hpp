#ifndef __SM83_H__
#define __SM83_H__

#pragma once
#include <cstdint>
#include <cstring>
#include "Enums.hpp"

#define SM83_VBLANK_INT 0x40
#define SM83_LCD_STAT_INT 0x48
#define SM83_TIMER_INT 0x50
#define SM83_SERIAL_INT 0x58
#define SM83_JOYPAD_INT 0x60

class Memory;
class GameBoy;
class SM83
{
  public:
    Memory *memory;
    GameBoy *gameboy;

    uint8_t A, F, B, C, D, E, H, L;
    uint16_t PC, SP;

    // The current cycle the current instruction being executed is on
    uint8_t instructionCycle;

    // Interrupt Master Enable
    uint8_t ime;

    // Variable that when is above 1, is decremented after each intruction
    // When it's 1, set the ime flag
    // If its value is 0, then ignore
    uint8_t ei_enable;

    // Variable that counts the remaining number of cycle an interrupt needs to disable ime, push
    // stack, and set the PC; an interrupt needs 5 cycles to be serviced
    // Ignore if its value is <0
    int8_t int_cycles;

    // Address of the interrupt to be called
    uint16_t int_addr;

    // Flag that says if the CPU is currently halted
    bool halted;

    // Flag that says if the HALT bug is in effect
    bool halt_bug;

    // Flag that says if the halt bug just started
    // Useful for not skipping the bug just after it has been triggered
    bool just_started_halt_bug;

    bool stop_signal;

    SM83();
    void initRegisters();

    void cycle();
    void executeOpcode(uint8_t opcode);
    void handleInterrupts();
    bool checkInterrupts(int8_t *int_cycles, uint16_t *int_addr);
    bool serviceInterrupt();

    /* MEMORY READ AND WRITE */

    uint8_t readmem_u8(uint16_t addr);
    uint16_t readmem_u16(uint16_t addr);
    void writemem_u8(uint8_t val, uint16_t addr);
    void writemem_u16(uint16_t val, uint16_t addr);

    bool checkInstructionCycle(uint8_t opcode_cycles, bool increment = true);
    void endInstruction(uint8_t opcode_length);

    /* FLAG GETTERS AND SETTERS */

    uint8_t getZeroFlag();
    uint8_t getSubtractFlag();
    uint8_t getHalfCarryFlag();
    uint8_t getCarryFlag();
    void setZeroFlag(uint8_t value);
    void setSubtractFlag(uint8_t value);
    void setHalfCarryFlag(uint8_t value);
    void setCarryFlag(uint8_t value);

    /* INTERRUPT FLAGS GETTERS AND SETTERS */

    uint8_t getInterruptEnable();
    uint8_t getVBlankInterruptEnable();
    uint8_t getLCDSTATInterruptEnable();
    uint8_t getTimerInterruptEnable();
    uint8_t getSerialInterruptEnable();
    uint8_t getJoypadInterruptEnable();

    uint8_t getInterruptFlag();
    uint8_t getVBlankInterruptFlag();
    uint8_t getLCDSTATInterruptFlag();
    uint8_t getTimerInterruptFlag();
    uint8_t getSerialInterruptFlag();
    uint8_t getJoypadInterruptFlag();

    void setImeFlag(uint8_t value);

    void setInterruptEnable(uint8_t value);
    void setVBlankInterruptEnable(uint8_t value);
    void setLCDSTATInterruptEnable(uint8_t value);
    void setTimerInterruptEnable(uint8_t value);
    void setSerialInterruptEnable(uint8_t value);
    void setJoypadInterruptEnable(uint8_t value);

    void setInterruptFlag(uint8_t value);
    void setVBlankInterruptFlag(uint8_t value);
    void setLCDSTATInterruptFlag(uint8_t value);
    void setTimerInterruptFlag(uint8_t value);
    void setSerialInterruptFlag(uint8_t value);
    void setJoypadInterruptFlag(uint8_t value);

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
    void op_dec_r16(uint8_t &r16_high, uint8_t &r16_low);
    void op_inc_r16(uint8_t &r16_high, uint8_t &r16_low);

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
    void op_rst(uint16_t vec);

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
    void OP_00();
    void OP_01();
    void OP_02();
    void OP_03();
    void OP_04();
    void OP_05();
    void OP_06();
    void OP_07();
    void OP_08();
    void OP_09();
    void OP_0A();
    void OP_0B();
    void OP_0C();
    void OP_0D();
    void OP_0E();
    void OP_0F();
    void OP_10();
    void OP_11();
    void OP_12();
    void OP_13();
    void OP_14();
    void OP_15();
    void OP_16();
    void OP_17();
    void OP_18();
    void OP_19();
    void OP_1A();
    void OP_1B();
    void OP_1C();
    void OP_1D();
    void OP_1E();
    void OP_1F();
    void OP_20();
    void OP_21();
    void OP_22();
    void OP_23();
    void OP_24();
    void OP_25();
    void OP_26();
    void OP_27();
    void OP_28();
    void OP_29();
    void OP_2A();
    void OP_2B();
    void OP_2C();
    void OP_2D();
    void OP_2E();
    void OP_2F();
    void OP_30();
    void OP_31();
    void OP_32();
    void OP_33();
    void OP_34();
    void OP_35();
    void OP_36();
    void OP_37();
    void OP_38();
    void OP_39();
    void OP_3A();
    void OP_3B();
    void OP_3C();
    void OP_3D();
    void OP_3E();
    void OP_3F();
    void OP_40();
    void OP_41();
    void OP_42();
    void OP_43();
    void OP_44();
    void OP_45();
    void OP_46();
    void OP_47();
    void OP_48();
    void OP_49();
    void OP_4A();
    void OP_4B();
    void OP_4C();
    void OP_4D();
    void OP_4E();
    void OP_4F();
    void OP_50();
    void OP_51();
    void OP_52();
    void OP_53();
    void OP_54();
    void OP_55();
    void OP_56();
    void OP_57();
    void OP_58();
    void OP_59();
    void OP_5A();
    void OP_5B();
    void OP_5C();
    void OP_5D();
    void OP_5E();
    void OP_5F();
    void OP_60();
    void OP_61();
    void OP_62();
    void OP_63();
    void OP_64();
    void OP_65();
    void OP_66();
    void OP_67();
    void OP_68();
    void OP_69();
    void OP_6A();
    void OP_6B();
    void OP_6C();
    void OP_6D();
    void OP_6E();
    void OP_6F();
    void OP_70();
    void OP_71();
    void OP_72();
    void OP_73();
    void OP_74();
    void OP_75();
    void OP_76();
    void OP_77();
    void OP_78();
    void OP_79();
    void OP_7A();
    void OP_7B();
    void OP_7C();
    void OP_7D();
    void OP_7E();
    void OP_7F();
    void OP_80();
    void OP_81();
    void OP_82();
    void OP_83();
    void OP_84();
    void OP_85();
    void OP_86();
    void OP_87();
    void OP_88();
    void OP_89();
    void OP_8A();
    void OP_8B();
    void OP_8C();
    void OP_8D();
    void OP_8E();
    void OP_8F();
    void OP_90();
    void OP_91();
    void OP_92();
    void OP_93();
    void OP_94();
    void OP_95();
    void OP_96();
    void OP_97();
    void OP_98();
    void OP_99();
    void OP_9A();
    void OP_9B();
    void OP_9C();
    void OP_9D();
    void OP_9E();
    void OP_9F();
    void OP_A0();
    void OP_A1();
    void OP_A2();
    void OP_A3();
    void OP_A4();
    void OP_A5();
    void OP_A6();
    void OP_A7();
    void OP_A8();
    void OP_A9();
    void OP_AA();
    void OP_AB();
    void OP_AC();
    void OP_AD();
    void OP_AE();
    void OP_AF();
    void OP_B0();
    void OP_B1();
    void OP_B2();
    void OP_B3();
    void OP_B4();
    void OP_B5();
    void OP_B6();
    void OP_B7();
    void OP_B8();
    void OP_B9();
    void OP_BA();
    void OP_BB();
    void OP_BC();
    void OP_BD();
    void OP_BE();
    void OP_BF();
    void OP_C0();
    void OP_C1();
    void OP_C2();
    void OP_C3();
    void OP_C4();
    void OP_C5();
    void OP_C6();
    void OP_C7();
    void OP_C8();
    void OP_C9();
    void OP_CA();
    void OP_CB();
    void OP_CC();
    void OP_CD();
    void OP_CE();
    void OP_CF();
    void OP_D0();
    void OP_D1();
    void OP_D2();
    void OP_D3();
    void OP_D4();
    void OP_D5();
    void OP_D6();
    void OP_D7();
    void OP_D8();
    void OP_D9();
    void OP_DA();
    void OP_DB();
    void OP_DC();
    void OP_DD();
    void OP_DE();
    void OP_DF();
    void OP_E0();
    void OP_E1();
    void OP_E2();
    void OP_E3();
    void OP_E4();
    void OP_E5();
    void OP_E6();
    void OP_E7();
    void OP_E8();
    void OP_E9();
    void OP_EA();
    void OP_EB();
    void OP_EC();
    void OP_ED();
    void OP_EE();
    void OP_EF();
    void OP_F0();
    void OP_F1();
    void OP_F2();
    void OP_F3();
    void OP_F4();
    void OP_F5();
    void OP_F6();
    void OP_F7();
    void OP_F8();
    void OP_F9();
    void OP_FA();
    void OP_FB();
    void OP_FC();
    void OP_FD();
    void OP_FE();
    void OP_FF();

    void OP_CB_00();
    void OP_CB_01();
    void OP_CB_02();
    void OP_CB_03();
    void OP_CB_04();
    void OP_CB_05();
    void OP_CB_06();
    void OP_CB_07();
    void OP_CB_08();
    void OP_CB_09();
    void OP_CB_0A();
    void OP_CB_0B();
    void OP_CB_0C();
    void OP_CB_0D();
    void OP_CB_0E();
    void OP_CB_0F();
    void OP_CB_10();
    void OP_CB_11();
    void OP_CB_12();
    void OP_CB_13();
    void OP_CB_14();
    void OP_CB_15();
    void OP_CB_16();
    void OP_CB_17();
    void OP_CB_18();
    void OP_CB_19();
    void OP_CB_1A();
    void OP_CB_1B();
    void OP_CB_1C();
    void OP_CB_1D();
    void OP_CB_1E();
    void OP_CB_1F();
    void OP_CB_20();
    void OP_CB_21();
    void OP_CB_22();
    void OP_CB_23();
    void OP_CB_24();
    void OP_CB_25();
    void OP_CB_26();
    void OP_CB_27();
    void OP_CB_28();
    void OP_CB_29();
    void OP_CB_2A();
    void OP_CB_2B();
    void OP_CB_2C();
    void OP_CB_2D();
    void OP_CB_2E();
    void OP_CB_2F();
    void OP_CB_30();
    void OP_CB_31();
    void OP_CB_32();
    void OP_CB_33();
    void OP_CB_34();
    void OP_CB_35();
    void OP_CB_36();
    void OP_CB_37();
    void OP_CB_38();
    void OP_CB_39();
    void OP_CB_3A();
    void OP_CB_3B();
    void OP_CB_3C();
    void OP_CB_3D();
    void OP_CB_3E();
    void OP_CB_3F();
    void OP_CB_40();
    void OP_CB_41();
    void OP_CB_42();
    void OP_CB_43();
    void OP_CB_44();
    void OP_CB_45();
    void OP_CB_46();
    void OP_CB_47();
    void OP_CB_48();
    void OP_CB_49();
    void OP_CB_4A();
    void OP_CB_4B();
    void OP_CB_4C();
    void OP_CB_4D();
    void OP_CB_4E();
    void OP_CB_4F();
    void OP_CB_50();
    void OP_CB_51();
    void OP_CB_52();
    void OP_CB_53();
    void OP_CB_54();
    void OP_CB_55();
    void OP_CB_56();
    void OP_CB_57();
    void OP_CB_58();
    void OP_CB_59();
    void OP_CB_5A();
    void OP_CB_5B();
    void OP_CB_5C();
    void OP_CB_5D();
    void OP_CB_5E();
    void OP_CB_5F();
    void OP_CB_60();
    void OP_CB_61();
    void OP_CB_62();
    void OP_CB_63();
    void OP_CB_64();
    void OP_CB_65();
    void OP_CB_66();
    void OP_CB_67();
    void OP_CB_68();
    void OP_CB_69();
    void OP_CB_6A();
    void OP_CB_6B();
    void OP_CB_6C();
    void OP_CB_6D();
    void OP_CB_6E();
    void OP_CB_6F();
    void OP_CB_70();
    void OP_CB_71();
    void OP_CB_72();
    void OP_CB_73();
    void OP_CB_74();
    void OP_CB_75();
    void OP_CB_76();
    void OP_CB_77();
    void OP_CB_78();
    void OP_CB_79();
    void OP_CB_7A();
    void OP_CB_7B();
    void OP_CB_7C();
    void OP_CB_7D();
    void OP_CB_7E();
    void OP_CB_7F();
    void OP_CB_80();
    void OP_CB_81();
    void OP_CB_82();
    void OP_CB_83();
    void OP_CB_84();
    void OP_CB_85();
    void OP_CB_86();
    void OP_CB_87();
    void OP_CB_88();
    void OP_CB_89();
    void OP_CB_8A();
    void OP_CB_8B();
    void OP_CB_8C();
    void OP_CB_8D();
    void OP_CB_8E();
    void OP_CB_8F();
    void OP_CB_90();
    void OP_CB_91();
    void OP_CB_92();
    void OP_CB_93();
    void OP_CB_94();
    void OP_CB_95();
    void OP_CB_96();
    void OP_CB_97();
    void OP_CB_98();
    void OP_CB_99();
    void OP_CB_9A();
    void OP_CB_9B();
    void OP_CB_9C();
    void OP_CB_9D();
    void OP_CB_9E();
    void OP_CB_9F();
    void OP_CB_A0();
    void OP_CB_A1();
    void OP_CB_A2();
    void OP_CB_A3();
    void OP_CB_A4();
    void OP_CB_A5();
    void OP_CB_A6();
    void OP_CB_A7();
    void OP_CB_A8();
    void OP_CB_A9();
    void OP_CB_AA();
    void OP_CB_AB();
    void OP_CB_AC();
    void OP_CB_AD();
    void OP_CB_AE();
    void OP_CB_AF();
    void OP_CB_B0();
    void OP_CB_B1();
    void OP_CB_B2();
    void OP_CB_B3();
    void OP_CB_B4();
    void OP_CB_B5();
    void OP_CB_B6();
    void OP_CB_B7();
    void OP_CB_B8();
    void OP_CB_B9();
    void OP_CB_BA();
    void OP_CB_BB();
    void OP_CB_BC();
    void OP_CB_BD();
    void OP_CB_BE();
    void OP_CB_BF();
    void OP_CB_C0();
    void OP_CB_C1();
    void OP_CB_C2();
    void OP_CB_C3();
    void OP_CB_C4();
    void OP_CB_C5();
    void OP_CB_C6();
    void OP_CB_C7();
    void OP_CB_C8();
    void OP_CB_C9();
    void OP_CB_CA();
    void OP_CB_CB();
    void OP_CB_CC();
    void OP_CB_CD();
    void OP_CB_CE();
    void OP_CB_CF();
    void OP_CB_D0();
    void OP_CB_D1();
    void OP_CB_D2();
    void OP_CB_D3();
    void OP_CB_D4();
    void OP_CB_D5();
    void OP_CB_D6();
    void OP_CB_D7();
    void OP_CB_D8();
    void OP_CB_D9();
    void OP_CB_DA();
    void OP_CB_DB();
    void OP_CB_DC();
    void OP_CB_DD();
    void OP_CB_DE();
    void OP_CB_DF();
    void OP_CB_E0();
    void OP_CB_E1();
    void OP_CB_E2();
    void OP_CB_E3();
    void OP_CB_E4();
    void OP_CB_E5();
    void OP_CB_E6();
    void OP_CB_E7();
    void OP_CB_E8();
    void OP_CB_E9();
    void OP_CB_EA();
    void OP_CB_EB();
    void OP_CB_EC();
    void OP_CB_ED();
    void OP_CB_EE();
    void OP_CB_EF();
    void OP_CB_F0();
    void OP_CB_F1();
    void OP_CB_F2();
    void OP_CB_F3();
    void OP_CB_F4();
    void OP_CB_F5();
    void OP_CB_F6();
    void OP_CB_F7();
    void OP_CB_F8();
    void OP_CB_F9();
    void OP_CB_FA();
    void OP_CB_FB();
    void OP_CB_FC();
    void OP_CB_FD();
    void OP_CB_FE();
    void OP_CB_FF();
};

#endif // __SM83_H__
