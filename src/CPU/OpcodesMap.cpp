#include "SM83.hpp"

void SM83::executeOpcode(uint8_t opcode)
{
    switch(opcode) { 
	case 0x00:
		OP_00();
		break;
	case 0x01:
		OP_01();
		break;
	case 0x02:
		OP_02();
		break;
	case 0x03:
		OP_03();
		break;
	case 0x04:
		OP_04();
		break;
	case 0x05:
		OP_05();
		break;
	case 0x06:
		OP_06();
		break;
	case 0x07:
		OP_07();
		break;
	case 0x08:
		OP_08();
		break;
	case 0x09:
		OP_09();
		break;
	case 0x0A:
		OP_0A();
		break;
	case 0x0B:
		OP_0B();
		break;
	case 0x0C:
		OP_0C();
		break;
	case 0x0D:
		OP_0D();
		break;
	case 0x0E:
		OP_0E();
		break;
	case 0x0F:
		OP_0F();
		break;
	case 0x10:
		OP_10();
		break;
	case 0x11:
		OP_11();
		break;
	case 0x12:
		OP_12();
		break;
	case 0x13:
		OP_13();
		break;
	case 0x14:
		OP_14();
		break;
	case 0x15:
		OP_15();
		break;
	case 0x16:
		OP_16();
		break;
	case 0x17:
		OP_17();
		break;
	case 0x18:
		OP_18();
		break;
	case 0x19:
		OP_19();
		break;
	case 0x1A:
		OP_1A();
		break;
	case 0x1B:
		OP_1B();
		break;
	case 0x1C:
		OP_1C();
		break;
	case 0x1D:
		OP_1D();
		break;
	case 0x1E:
		OP_1E();
		break;
	case 0x1F:
		OP_1F();
		break;
	case 0x20:
		OP_20();
		break;
	case 0x21:
		OP_21();
		break;
	case 0x22:
		OP_22();
		break;
	case 0x23:
		OP_23();
		break;
	case 0x24:
		OP_24();
		break;
	case 0x25:
		OP_25();
		break;
	case 0x26:
		OP_26();
		break;
	case 0x27:
		OP_27();
		break;
	case 0x28:
		OP_28();
		break;
	case 0x29:
		OP_29();
		break;
	case 0x2A:
		OP_2A();
		break;
	case 0x2B:
		OP_2B();
		break;
	case 0x2C:
		OP_2C();
		break;
	case 0x2D:
		OP_2D();
		break;
	case 0x2E:
		OP_2E();
		break;
	case 0x2F:
		OP_2F();
		break;
	case 0x30:
		OP_30();
		break;
	case 0x31:
		OP_31();
		break;
	case 0x32:
		OP_32();
		break;
	case 0x33:
		OP_33();
		break;
	case 0x34:
		OP_34();
		break;
	case 0x35:
		OP_35();
		break;
	case 0x36:
		OP_36();
		break;
	case 0x37:
		OP_37();
		break;
	case 0x38:
		OP_38();
		break;
	case 0x39:
		OP_39();
		break;
	case 0x3A:
		OP_3A();
		break;
	case 0x3B:
		OP_3B();
		break;
	case 0x3C:
		OP_3C();
		break;
	case 0x3D:
		OP_3D();
		break;
	case 0x3E:
		OP_3E();
		break;
	case 0x3F:
		OP_3F();
		break;
	case 0x40:
		OP_40();
		break;
	case 0x41:
		OP_41();
		break;
	case 0x42:
		OP_42();
		break;
	case 0x43:
		OP_43();
		break;
	case 0x44:
		OP_44();
		break;
	case 0x45:
		OP_45();
		break;
	case 0x46:
		OP_46();
		break;
	case 0x47:
		OP_47();
		break;
	case 0x48:
		OP_48();
		break;
	case 0x49:
		OP_49();
		break;
	case 0x4A:
		OP_4A();
		break;
	case 0x4B:
		OP_4B();
		break;
	case 0x4C:
		OP_4C();
		break;
	case 0x4D:
		OP_4D();
		break;
	case 0x4E:
		OP_4E();
		break;
	case 0x4F:
		OP_4F();
		break;
	case 0x50:
		OP_50();
		break;
	case 0x51:
		OP_51();
		break;
	case 0x52:
		OP_52();
		break;
	case 0x53:
		OP_53();
		break;
	case 0x54:
		OP_54();
		break;
	case 0x55:
		OP_55();
		break;
	case 0x56:
		OP_56();
		break;
	case 0x57:
		OP_57();
		break;
	case 0x58:
		OP_58();
		break;
	case 0x59:
		OP_59();
		break;
	case 0x5A:
		OP_5A();
		break;
	case 0x5B:
		OP_5B();
		break;
	case 0x5C:
		OP_5C();
		break;
	case 0x5D:
		OP_5D();
		break;
	case 0x5E:
		OP_5E();
		break;
	case 0x5F:
		OP_5F();
		break;
	case 0x60:
		OP_60();
		break;
	case 0x61:
		OP_61();
		break;
	case 0x62:
		OP_62();
		break;
	case 0x63:
		OP_63();
		break;
	case 0x64:
		OP_64();
		break;
	case 0x65:
		OP_65();
		break;
	case 0x66:
		OP_66();
		break;
	case 0x67:
		OP_67();
		break;
	case 0x68:
		OP_68();
		break;
	case 0x69:
		OP_69();
		break;
	case 0x6A:
		OP_6A();
		break;
	case 0x6B:
		OP_6B();
		break;
	case 0x6C:
		OP_6C();
		break;
	case 0x6D:
		OP_6D();
		break;
	case 0x6E:
		OP_6E();
		break;
	case 0x6F:
		OP_6F();
		break;
	case 0x70:
		OP_70();
		break;
	case 0x71:
		OP_71();
		break;
	case 0x72:
		OP_72();
		break;
	case 0x73:
		OP_73();
		break;
	case 0x74:
		OP_74();
		break;
	case 0x75:
		OP_75();
		break;
	case 0x76:
		OP_76();
		break;
	case 0x77:
		OP_77();
		break;
	case 0x78:
		OP_78();
		break;
	case 0x79:
		OP_79();
		break;
	case 0x7A:
		OP_7A();
		break;
	case 0x7B:
		OP_7B();
		break;
	case 0x7C:
		OP_7C();
		break;
	case 0x7D:
		OP_7D();
		break;
	case 0x7E:
		OP_7E();
		break;
	case 0x7F:
		OP_7F();
		break;
	case 0x80:
		OP_80();
		break;
	case 0x81:
		OP_81();
		break;
	case 0x82:
		OP_82();
		break;
	case 0x83:
		OP_83();
		break;
	case 0x84:
		OP_84();
		break;
	case 0x85:
		OP_85();
		break;
	case 0x86:
		OP_86();
		break;
	case 0x87:
		OP_87();
		break;
	case 0x88:
		OP_88();
		break;
	case 0x89:
		OP_89();
		break;
	case 0x8A:
		OP_8A();
		break;
	case 0x8B:
		OP_8B();
		break;
	case 0x8C:
		OP_8C();
		break;
	case 0x8D:
		OP_8D();
		break;
	case 0x8E:
		OP_8E();
		break;
	case 0x8F:
		OP_8F();
		break;
	case 0x90:
		OP_90();
		break;
	case 0x91:
		OP_91();
		break;
	case 0x92:
		OP_92();
		break;
	case 0x93:
		OP_93();
		break;
	case 0x94:
		OP_94();
		break;
	case 0x95:
		OP_95();
		break;
	case 0x96:
		OP_96();
		break;
	case 0x97:
		OP_97();
		break;
	case 0x98:
		OP_98();
		break;
	case 0x99:
		OP_99();
		break;
	case 0x9A:
		OP_9A();
		break;
	case 0x9B:
		OP_9B();
		break;
	case 0x9C:
		OP_9C();
		break;
	case 0x9D:
		OP_9D();
		break;
	case 0x9E:
		OP_9E();
		break;
	case 0x9F:
		OP_9F();
		break;
	case 0xA0:
		OP_A0();
		break;
	case 0xA1:
		OP_A1();
		break;
	case 0xA2:
		OP_A2();
		break;
	case 0xA3:
		OP_A3();
		break;
	case 0xA4:
		OP_A4();
		break;
	case 0xA5:
		OP_A5();
		break;
	case 0xA6:
		OP_A6();
		break;
	case 0xA7:
		OP_A7();
		break;
	case 0xA8:
		OP_A8();
		break;
	case 0xA9:
		OP_A9();
		break;
	case 0xAA:
		OP_AA();
		break;
	case 0xAB:
		OP_AB();
		break;
	case 0xAC:
		OP_AC();
		break;
	case 0xAD:
		OP_AD();
		break;
	case 0xAE:
		OP_AE();
		break;
	case 0xAF:
		OP_AF();
		break;
	case 0xB0:
		OP_B0();
		break;
	case 0xB1:
		OP_B1();
		break;
	case 0xB2:
		OP_B2();
		break;
	case 0xB3:
		OP_B3();
		break;
	case 0xB4:
		OP_B4();
		break;
	case 0xB5:
		OP_B5();
		break;
	case 0xB6:
		OP_B6();
		break;
	case 0xB7:
		OP_B7();
		break;
	case 0xB8:
		OP_B8();
		break;
	case 0xB9:
		OP_B9();
		break;
	case 0xBA:
		OP_BA();
		break;
	case 0xBB:
		OP_BB();
		break;
	case 0xBC:
		OP_BC();
		break;
	case 0xBD:
		OP_BD();
		break;
	case 0xBE:
		OP_BE();
		break;
	case 0xBF:
		OP_BF();
		break;
	case 0xC0:
		OP_C0();
		break;
	case 0xC1:
		OP_C1();
		break;
	case 0xC2:
		OP_C2();
		break;
	case 0xC3:
		OP_C3();
		break;
	case 0xC4:
		OP_C4();
		break;
	case 0xC5:
		OP_C5();
		break;
	case 0xC6:
		OP_C6();
		break;
	case 0xC7:
		OP_C7();
		break;
	case 0xC8:
		OP_C8();
		break;
	case 0xC9:
		OP_C9();
		break;
	case 0xCA:
		OP_CA();
		break;
	case 0xCB:
		OP_CB();
		break;
	case 0xCC:
		OP_CC();
		break;
	case 0xCD:
		OP_CD();
		break;
	case 0xCE:
		OP_CE();
		break;
	case 0xCF:
		OP_CF();
		break;
	case 0xD0:
		OP_D0();
		break;
	case 0xD1:
		OP_D1();
		break;
	case 0xD2:
		OP_D2();
		break;
	case 0xD3:
		OP_D3();
		break;
	case 0xD4:
		OP_D4();
		break;
	case 0xD5:
		OP_D5();
		break;
	case 0xD6:
		OP_D6();
		break;
	case 0xD7:
		OP_D7();
		break;
	case 0xD8:
		OP_D8();
		break;
	case 0xD9:
		OP_D9();
		break;
	case 0xDA:
		OP_DA();
		break;
	case 0xDB:
		OP_DB();
		break;
	case 0xDC:
		OP_DC();
		break;
	case 0xDD:
		OP_DD();
		break;
	case 0xDE:
		OP_DE();
		break;
	case 0xDF:
		OP_DF();
		break;
	case 0xE0:
		OP_E0();
		break;
	case 0xE1:
		OP_E1();
		break;
	case 0xE2:
		OP_E2();
		break;
	case 0xE3:
		OP_E3();
		break;
	case 0xE4:
		OP_E4();
		break;
	case 0xE5:
		OP_E5();
		break;
	case 0xE6:
		OP_E6();
		break;
	case 0xE7:
		OP_E7();
		break;
	case 0xE8:
		OP_E8();
		break;
	case 0xE9:
		OP_E9();
		break;
	case 0xEA:
		OP_EA();
		break;
	case 0xEB:
		OP_EB();
		break;
	case 0xEC:
		OP_EC();
		break;
	case 0xED:
		OP_ED();
		break;
	case 0xEE:
		OP_EE();
		break;
	case 0xEF:
		OP_EF();
		break;
	case 0xF0:
		OP_F0();
		break;
	case 0xF1:
		OP_F1();
		break;
	case 0xF2:
		OP_F2();
		break;
	case 0xF3:
		OP_F3();
		break;
	case 0xF4:
		OP_F4();
		break;
	case 0xF5:
		OP_F5();
		break;
	case 0xF6:
		OP_F6();
		break;
	case 0xF7:
		OP_F7();
		break;
	case 0xF8:
		OP_F8();
		break;
	case 0xF9:
		OP_F9();
		break;
	case 0xFA:
		OP_FA();
		break;
	case 0xFB:
		OP_FB();
		break;
	case 0xFC:
		OP_FC();
		break;
	case 0xFD:
		OP_FD();
		break;
	case 0xFE:
		OP_FE();
		break;
    }
}

// NOP
void SM83::OP_00() { op_nop(); }

// LD BC, n16
void SM83::OP_01() { op_ld_r16_n16(B, C); }

// LD [BC], A
void SM83::OP_02() { op_ld_addr_r16_a(B, C); }

// INC BC
void SM83::OP_03() { op_inc_r16(B, C); }

// INC B
void SM83::OP_04() { op_inc_r8(B); }

// DEC B
void SM83::OP_05() { op_dec_r8(B); }

// LD B, n8
void SM83::OP_06() { op_ld_r8_n8(B); }

// RLCA
void SM83::OP_07() { op_rlca(); }

// LD [n16], SP
void SM83::OP_08() { op_ld_addr_n16_sp(); }

// ADD HL, BC
void SM83::OP_09() { op_add_hl_r16(B, C); }

// LD A, [BC]
void SM83::OP_0A() { op_ld_a_addr_r16(B, C); }

// DEC BC
void SM83::OP_0B() { op_dec_r16(B, C); }

// INC C
void SM83::OP_0C() { op_inc_r8(C); }

// DEC C
void SM83::OP_0D() { op_dec_r8(C); }

// LD C, n8
void SM83::OP_0E() { op_ld_r8_n8(C); }

// RRCA
void SM83::OP_0F() { op_rrca(); }

// STOP 0
void SM83::OP_10() { op_stop(); }

// LD DE, n16
void SM83::OP_11() { op_ld_r16_n16(D, E); }

// LD [DE], A
void SM83::OP_12() { op_ld_addr_r16_a(D, E); }

// INC DE
void SM83::OP_13() { op_inc_r16(D, E); }

// INC D
void SM83::OP_14() { op_inc_r8(D); }

// DEC D
void SM83::OP_15() { op_dec_r8(D); }

// LD D, n8
void SM83::OP_16() { op_ld_r8_n8(D); }

// RLA
void SM83::OP_17() { op_rla(); }

// JR e8
void SM83::OP_18() { op_jr_e8(); }

// ADD HL, DE
void SM83::OP_19() { op_add_hl_r16(D, E); }

// LD A, [DE]
void SM83::OP_1A() { op_ld_a_addr_r16(D, E); }

// DEC DE
void SM83::OP_1B() { op_dec_r16(D, E); }

// INC E
void SM83::OP_1C() { op_inc_r8(E); }

// DEC E
void SM83::OP_1D() { op_dec_r8(E); }

// LD E, n8
void SM83::OP_1E() { op_ld_r8_n8(E); }

// RRA
void SM83::OP_1F() { op_rra(); }

// JR NZ, e8
void SM83::OP_20() { op_jr_cc_e8(getZeroFlag(), 1); }

// LD HL, n16
void SM83::OP_21() { op_ld_r16_n16(H, L); }

// LD [HL+], A
void SM83::OP_22() { op_ld_addr_hl_inc_a(); }

// INC HL
void SM83::OP_23() { op_inc_r16(H, L); }

// INC H
void SM83::OP_24() { op_inc_r8(H); }

// DEC H
void SM83::OP_25() { op_dec_r8(H); }

// LD H, n8
void SM83::OP_26() { op_ld_r8_n8(H); }

// DAA
void SM83::OP_27() { op_daa(); }

// JR Z, e8
void SM83::OP_28() { op_jr_cc_e8(getZeroFlag(), 0); }

// ADD HL, HL
void SM83::OP_29() { op_add_hl_r16(H, L); }

// LD A, [HL+]
void SM83::OP_2A() { op_ld_a_addr_hl_inc(); }

// DEC HL
void SM83::OP_2B() { op_dec_r16(H, L); }

// INC L
void SM83::OP_2C() { op_inc_r8(L); }

// DEC L
void SM83::OP_2D() { op_dec_r8(L); }

// LD L, n8
void SM83::OP_2E() { op_ld_r8_n8(L); }

// CPL
void SM83::OP_2F() { op_cpl(); }

// JR NC, e8
void SM83::OP_30() { op_jr_cc_e8(getCarryFlag(), 0); }

// LD SP, n16
void SM83::OP_31() { op_ld_sp_n16(); }

// LD [HL-], A
void SM83::OP_32() { op_ld_addr_hl_dec_a(); }

// INC SP
void SM83::OP_33() { op_inc_sp(); }

// INC [HL]
void SM83::OP_34() { op_inc_addr_hl(); }

// DEC [HL]
void SM83::OP_35() { op_dec_addr_hl(); }

// LD [HL], n8
void SM83::OP_36() { op_ld_addr_hl_n8(); }

// SCF
void SM83::OP_37() { op_scf(); }

// JR C, e8
void SM83::OP_38() { op_jr_cc_e8(getCarryFlag(), 1); }

// ADD HL, SP
void SM83::OP_39() { op_add_hl_sp(); }

// LD A, [Hl-]
void SM83::OP_3A() { op_ld_a_addr_hl_dec(); }

// DEC SP
void SM83::OP_3B() { op_dec_sp(); }

// INC A
void SM83::OP_3C() { op_inc_r8(A); }

// DEC A
void SM83::OP_3D() { op_dec_r8(A); }

// LD A, n8
void SM83::OP_3E() { op_ld_r8_n8(A); }

// CCF
void SM83::OP_3F() { op_ccf(); }

// LD B, B
void SM83::OP_40() { op_ld_r8_r8(B, B); }

// LD B, C
void SM83::OP_41() { op_ld_r8_r8(B, C); }

// LD B, D
void SM83::OP_42() { op_ld_r8_r8(B, D); }

// LD B, E
void SM83::OP_43() { op_ld_r8_r8(B, E); }

// LD B, H
void SM83::OP_44() { op_ld_r8_r8(B, H); }

// LD B, L
void SM83::OP_45() { op_ld_r8_r8(B, L); }

// LD B, [HL]
void SM83::OP_46() { op_ld_r8_addr_hl(B); }

// LD B, A
void SM83::OP_47() { op_ld_r8_r8(B, A); }

// LD C, B
void SM83::OP_48() { op_ld_r8_r8(C, B); }

// LD C, C
void SM83::OP_49() { op_ld_r8_r8(C, C); }

// LD C, D
void SM83::OP_4A() { op_ld_r8_r8(C, D); }

// LD C, E
void SM83::OP_4B() { op_ld_r8_r8(C, E); }

// LD C, H
void SM83::OP_4C() { op_ld_r8_r8(C, H); }

// LD C, L
void SM83::OP_4D() { op_ld_r8_r8(C, L); }

// LD C, [HL]
void SM83::OP_4E() { op_ld_r8_addr_hl(C); }

// LD C, A
void SM83::OP_4F() { op_ld_r8_r8(C, A); }

// LD D, B
void SM83::OP_50() { op_ld_r8_r8(D, B); }

// LD D, C
void SM83::OP_51() { op_ld_r8_r8(D, C); }

// LD D, D
void SM83::OP_52() { op_ld_r8_r8(D, D); }

// LD D, E
void SM83::OP_53() { op_ld_r8_r8(D, E); }

// LD D, H
void SM83::OP_54() { op_ld_r8_r8(D, H); }

// LD D, L
void SM83::OP_55() { op_ld_r8_r8(D, L); }

// LD D, [HL]
void SM83::OP_56() { op_ld_r8_addr_hl(D); }

// LD D, A
void SM83::OP_57() { op_ld_r8_r8(D, A); }

// LD E, B
void SM83::OP_58() { op_ld_r8_r8(E, B); }

// LD E, C
void SM83::OP_59() { op_ld_r8_r8(E, C); }

// LD E, D
void SM83::OP_5A() { op_ld_r8_r8(E, D); }

// LD E, E
void SM83::OP_5B() { op_ld_r8_r8(E, E); }

// LD E, H
void SM83::OP_5C() { op_ld_r8_r8(E, H); }

// LD E, L
void SM83::OP_5D() { op_ld_r8_r8(E, L); }

// LD E, [HL]
void SM83::OP_5E() { op_ld_r8_addr_hl(E); }

// LD E, A
void SM83::OP_5F() { op_ld_r8_r8(E, A); }

// LD H, B
void SM83::OP_60() { op_ld_r8_r8(H, B); }

// LD H, C
void SM83::OP_61() { op_ld_r8_r8(H, C); }

// LD H, D
void SM83::OP_62() { op_ld_r8_r8(H, D); }

// LD H, E
void SM83::OP_63() { op_ld_r8_r8(H, E); }

// LD H, H
void SM83::OP_64() { op_ld_r8_r8(H, H); }

// LD H, L
void SM83::OP_65() { op_ld_r8_r8(H, L); }

// LD H, [HL]
void SM83::OP_66() { op_ld_r8_addr_hl(H); }

// LD H, A
void SM83::OP_67() { op_ld_r8_r8(H, A); }

// LD L, B
void SM83::OP_68() { op_ld_r8_r8(L, B); }

// LD L, C
void SM83::OP_69() { op_ld_r8_r8(L, C); }

// LD L, D
void SM83::OP_6A() { op_ld_r8_r8(L, D); }

// LD L, E
void SM83::OP_6B() { op_ld_r8_r8(L, E); }

// LD L, H
void SM83::OP_6C() { op_ld_r8_r8(L, H); }

// LD L, L
void SM83::OP_6D() { op_ld_r8_r8(L, L); }

// LD L, [HL]
void SM83::OP_6E() { op_ld_r8_addr_hl(L); }

// LD L, A
void SM83::OP_6F() { op_ld_r8_r8(L, A); }

// LD [HL], B
void SM83::OP_70() { op_ld_addr_hl_r8(B); }

// LD [HL], C
void SM83::OP_71() { op_ld_addr_hl_r8(C); }

// LD [HL], D
void SM83::OP_72() { op_ld_addr_hl_r8(D); }

// LD [HL], E
void SM83::OP_73() { op_ld_addr_hl_r8(E); }

// LD [HL], H
void SM83::OP_74() { op_ld_addr_hl_r8(H); }

// LD [HL], L
void SM83::OP_75() { op_ld_addr_hl_r8(L); }

// HALT
void SM83::OP_76() { op_halt(); }

// LD [HL], A
void SM83::OP_77() { op_ld_addr_hl_r8(A); }

// LD A, B
void SM83::OP_78() { op_ld_r8_r8(A, B); }

// LD A, C
void SM83::OP_79() { op_ld_r8_r8(A, C); }

// LD A, D
void SM83::OP_7A() { op_ld_r8_r8(A, D); }

// LD A, E
void SM83::OP_7B() { op_ld_r8_r8(A, E); }

// LD A, H
void SM83::OP_7C() { op_ld_r8_r8(A, H); }

// LD A, L
void SM83::OP_7D() { op_ld_r8_r8(A, L); }

// LD A, [HL]
void SM83::OP_7E() { op_ld_r8_addr_hl(A); }

// LD A, A
void SM83::OP_7F() { op_ld_r8_r8(A, A); }

// ADD A, B
void SM83::OP_80() { op_add_a_r8(B); }

// ADD A, C
void SM83::OP_81() { op_add_a_r8(C); }

// ADD A, D
void SM83::OP_82() { op_add_a_r8(D); }

// ADD A, E
void SM83::OP_83() { op_add_a_r8(E); }

// ADD A, H
void SM83::OP_84() { op_add_a_r8(H); }

// ADD A, L
void SM83::OP_85() { op_add_a_r8(L); }

// ADD A, [HL]
void SM83::OP_86() { op_add_a_addr_hl(); }

// ADD A, A
void SM83::OP_87() { op_add_a_r8(A); }

// ADC A, B
void SM83::OP_88() { op_adc_a_r8(B); }

// ADC A, C
void SM83::OP_89() { op_adc_a_r8(C); }

// ADC A, D
void SM83::OP_8A() { op_adc_a_r8(D); }

// ADC A, E
void SM83::OP_8B() { op_adc_a_r8(E); }

// ADC A, H
void SM83::OP_8C() { op_adc_a_r8(H); }

// ADC A, L
void SM83::OP_8D() { op_adc_a_r8(L); }

// ADC A, [HL]
void SM83::OP_8E() { op_adc_a_addr_hl(); }

// ADC A, A
void SM83::OP_8F() { op_adc_a_r8(A); }

// SUB B
void SM83::OP_90() { op_sub_a_r8(B); }

// SUB C
void SM83::OP_91() { op_sub_a_r8(C); }

// SUB D
void SM83::OP_92() { op_sub_a_r8(D); }

// SUB E
void SM83::OP_93() { op_sub_a_r8(E); }

// SUB H
void SM83::OP_94() { op_sub_a_r8(H); }

// SUB L
void SM83::OP_95() { op_sub_a_r8(L); }

// SUB [HL]
void SM83::OP_96() { op_sub_a_addr_hl(); }

// SUB A
void SM83::OP_97() { op_sub_a_r8(A); }

// SBC A, B
void SM83::OP_98() { op_sbc_a_r8(B); }

// SBC A, C
void SM83::OP_99() { op_sbc_a_r8(C); }

// SBC A, D
void SM83::OP_9A() { op_sbc_a_r8(D); }

// SBC A, E
void SM83::OP_9B() { op_sbc_a_r8(E); }

// SBC A, H
void SM83::OP_9C() { op_sbc_a_r8(H); }

// SBC A, L
void SM83::OP_9D() { op_sbc_a_r8(L); }

// SBC A, [HL]
void SM83::OP_9E() { op_sbc_a_addr_hl(); }

// SBC A, A
void SM83::OP_9F() { op_sbc_a_r8(A); }

// AND B
void SM83::OP_A0() { op_and_a_r8(B); }

// AND C
void SM83::OP_A1() { op_and_a_r8(C); }

// AND D
void SM83::OP_A2() { op_and_a_r8(D); }

// AND E
void SM83::OP_A3() { op_and_a_r8(E); }

// AND H
void SM83::OP_A4() { op_and_a_r8(H); }

// AND L
void SM83::OP_A5() { op_and_a_r8(L); }

// AND [HL]
void SM83::OP_A6() { op_and_a_addr_hl(); }

// AND A
void SM83::OP_A7() { op_and_a_r8(A); }

// XOR B
void SM83::OP_A8() { op_xor_a_r8(B); }

// XOR C
void SM83::OP_A9() { op_xor_a_r8(C); }

// XOR D
void SM83::OP_AA() { op_xor_a_r8(D); }

// XOR E
void SM83::OP_AB() { op_xor_a_r8(E); }

// XOR H
void SM83::OP_AC() { op_xor_a_r8(H); }

// XOR L
void SM83::OP_AD() { op_xor_a_r8(L); }

// XOR [HL]
void SM83::OP_AE() { op_xor_a_addr_hl(); }

// XOR A
void SM83::OP_AF() { op_xor_a_r8(A); }

// OR B
void SM83::OP_B0() { op_or_a_r8(B); }

// OR C
void SM83::OP_B1() { op_or_a_r8(C); }

// OR D
void SM83::OP_B2() { op_or_a_r8(D); }

// OR E
void SM83::OP_B3() { op_or_a_r8(E); }

// OR H
void SM83::OP_B4() { op_or_a_r8(H); }

// OR L
void SM83::OP_B5() { op_or_a_r8(L); }

// OR [HL]
void SM83::OP_B6() { op_or_a_addr_hl(); }

// OR A
void SM83::OP_B7() { op_or_a_r8(A); }

// CP B
void SM83::OP_B8() { op_cp_a_r8(B); }

// CP C
void SM83::OP_B9() { op_cp_a_r8(C); }

// CP D
void SM83::OP_BA() { op_cp_a_r8(D); }

// CP E
void SM83::OP_BB() { op_cp_a_r8(E); }

// CP H
void SM83::OP_BC() { op_cp_a_r8(H); }

// CP L
void SM83::OP_BD() { op_cp_a_r8(L); }

// CP [HL]
void SM83::OP_BE() { op_cp_a_addr_hl(); }

// CP A
void SM83::OP_BF() { op_cp_a_r8(A); }

// RET NZ
void SM83::OP_C0() { op_ret_cc(getZeroFlag(), 0); }

// POP BC
void SM83::OP_C1() { op_pop_r16(B, C); }

// JP NZ, n16
void SM83::OP_C2() { op_jp_cc_n16(getZeroFlag(), 0); }

// JP n16
void SM83::OP_C3() { op_jp_n16(); }

// CALL NZ, n16
void SM83::OP_C4() { op_call_cc_n16(getZeroFlag(), 0); }

// PUSH BC
void SM83::OP_C5() { op_push_r16(B, C); }

// ADD A, n8
void SM83::OP_C6() { op_add_a_n8(); }

// RST 0x00
void SM83::OP_C7() { op_rst(0x00); }

// RET Z
void SM83::OP_C8() { op_ret_cc(getZeroFlag(), 1); }

// RET
void SM83::OP_C9() { op_ret(); }

// JP Z, n16
void SM83::OP_CA() { op_jp_cc_n16(getZeroFlag(), 1); }

// Prefix CB
void SM83::OP_CB()
{
    uint8_t cb_opcode = readmem_u8(PC + 1);

    switch (cb_opcode) {
    case 0x00:
        OP_CB_00();
        break;
    case 0x01:
        OP_CB_01();
        break;
    case 0x02:
        OP_CB_02();
        break;
    case 0x03:
        OP_CB_03();
        break;
    case 0x04:
        OP_CB_04();
        break;
    case 0x05:
        OP_CB_05();
        break;
    case 0x06:
        OP_CB_06();
        break;
    case 0x07:
        OP_CB_07();
        break;
    case 0x08:
        OP_CB_08();
        break;
    case 0x09:
        OP_CB_09();
        break;
    case 0x0A:
        OP_CB_0A();
        break;
    case 0x0B:
        OP_CB_0B();
        break;
    case 0x0C:
        OP_CB_0C();
        break;
    case 0x0D:
        OP_CB_0D();
        break;
    case 0x0E:
        OP_CB_0E();
        break;
    case 0x0F:
        OP_CB_0F();
        break;
    case 0x10:
        OP_CB_10();
        break;
    case 0x11:
        OP_CB_11();
        break;
    case 0x12:
        OP_CB_12();
        break;
    case 0x13:
        OP_CB_13();
        break;
    case 0x14:
        OP_CB_14();
        break;
    case 0x15:
        OP_CB_15();
        break;
    case 0x16:
        OP_CB_16();
        break;
    case 0x17:
        OP_CB_17();
        break;
    case 0x18:
        OP_CB_18();
        break;
    case 0x19:
        OP_CB_19();
        break;
    case 0x1A:
        OP_CB_1A();
        break;
    case 0x1B:
        OP_CB_1B();
        break;
    case 0x1C:
        OP_CB_1C();
        break;
    case 0x1D:
        OP_CB_1D();
        break;
    case 0x1E:
        OP_CB_1E();
        break;
    case 0x1F:
        OP_CB_1F();
        break;
    case 0x20:
        OP_CB_20();
        break;
    case 0x21:
        OP_CB_21();
        break;
    case 0x22:
        OP_CB_22();
        break;
    case 0x23:
        OP_CB_23();
        break;
    case 0x24:
        OP_CB_24();
        break;
    case 0x25:
        OP_CB_25();
        break;
    case 0x26:
        OP_CB_26();
        break;
    case 0x27:
        OP_CB_27();
        break;
    case 0x28:
        OP_CB_28();
        break;
    case 0x29:
        OP_CB_29();
        break;
    case 0x2A:
        OP_CB_2A();
        break;
    case 0x2B:
        OP_CB_2B();
        break;
    case 0x2C:
        OP_CB_2C();
        break;
    case 0x2D:
        OP_CB_2D();
        break;
    case 0x2E:
        OP_CB_2E();
        break;
    case 0x2F:
        OP_CB_2F();
        break;
    case 0x30:
        OP_CB_30();
        break;
    case 0x31:
        OP_CB_31();
        break;
    case 0x32:
        OP_CB_32();
        break;
    case 0x33:
        OP_CB_33();
        break;
    case 0x34:
        OP_CB_34();
        break;
    case 0x35:
        OP_CB_35();
        break;
    case 0x36:
        OP_CB_36();
        break;
    case 0x37:
        OP_CB_37();
        break;
    case 0x38:
        OP_CB_38();
        break;
    case 0x39:
        OP_CB_39();
        break;
    case 0x3A:
        OP_CB_3A();
        break;
    case 0x3B:
        OP_CB_3B();
        break;
    case 0x3C:
        OP_CB_3C();
        break;
    case 0x3D:
        OP_CB_3D();
        break;
    case 0x3E:
        OP_CB_3E();
        break;
    case 0x3F:
        OP_CB_3F();
        break;
    case 0x40:
        OP_CB_40();
        break;
    case 0x41:
        OP_CB_41();
        break;
    case 0x42:
        OP_CB_42();
        break;
    case 0x43:
        OP_CB_43();
        break;
    case 0x44:
        OP_CB_44();
        break;
    case 0x45:
        OP_CB_45();
        break;
    case 0x46:
        OP_CB_46();
        break;
    case 0x47:
        OP_CB_47();
        break;
    case 0x48:
        OP_CB_48();
        break;
    case 0x49:
        OP_CB_49();
        break;
    case 0x4A:
        OP_CB_4A();
        break;
    case 0x4B:
        OP_CB_4B();
        break;
    case 0x4C:
        OP_CB_4C();
        break;
    case 0x4D:
        OP_CB_4D();
        break;
    case 0x4E:
        OP_CB_4E();
        break;
    case 0x4F:
        OP_CB_4F();
        break;
    case 0x50:
        OP_CB_50();
        break;
    case 0x51:
        OP_CB_51();
        break;
    case 0x52:
        OP_CB_52();
        break;
    case 0x53:
        OP_CB_53();
        break;
    case 0x54:
        OP_CB_54();
        break;
    case 0x55:
        OP_CB_55();
        break;
    case 0x56:
        OP_CB_56();
        break;
    case 0x57:
        OP_CB_57();
        break;
    case 0x58:
        OP_CB_58();
        break;
    case 0x59:
        OP_CB_59();
        break;
    case 0x5A:
        OP_CB_5A();
        break;
    case 0x5B:
        OP_CB_5B();
        break;
    case 0x5C:
        OP_CB_5C();
        break;
    case 0x5D:
        OP_CB_5D();
        break;
    case 0x5E:
        OP_CB_5E();
        break;
    case 0x5F:
        OP_CB_5F();
        break;
    case 0x60:
        OP_CB_60();
        break;
    case 0x61:
        OP_CB_61();
        break;
    case 0x62:
        OP_CB_62();
        break;
    case 0x63:
        OP_CB_63();
        break;
    case 0x64:
        OP_CB_64();
        break;
    case 0x65:
        OP_CB_65();
        break;
    case 0x66:
        OP_CB_66();
        break;
    case 0x67:
        OP_CB_67();
        break;
    case 0x68:
        OP_CB_68();
        break;
    case 0x69:
        OP_CB_69();
        break;
    case 0x6A:
        OP_CB_6A();
        break;
    case 0x6B:
        OP_CB_6B();
        break;
    case 0x6C:
        OP_CB_6C();
        break;
    case 0x6D:
        OP_CB_6D();
        break;
    case 0x6E:
        OP_CB_6E();
        break;
    case 0x6F:
        OP_CB_6F();
        break;
    case 0x70:
        OP_CB_70();
        break;
    case 0x71:
        OP_CB_71();
        break;
    case 0x72:
        OP_CB_72();
        break;
    case 0x73:
        OP_CB_73();
        break;
    case 0x74:
        OP_CB_74();
        break;
    case 0x75:
        OP_CB_75();
        break;
    case 0x76:
        OP_CB_76();
        break;
    case 0x77:
        OP_CB_77();
        break;
    case 0x78:
        OP_CB_78();
        break;
    case 0x79:
        OP_CB_79();
        break;
    case 0x7A:
        OP_CB_7A();
        break;
    case 0x7B:
        OP_CB_7B();
        break;
    case 0x7C:
        OP_CB_7C();
        break;
    case 0x7D:
        OP_CB_7D();
        break;
    case 0x7E:
        OP_CB_7E();
        break;
    case 0x7F:
        OP_CB_7F();
        break;
    case 0x80:
        OP_CB_80();
        break;
    case 0x81:
        OP_CB_81();
        break;
    case 0x82:
        OP_CB_82();
        break;
    case 0x83:
        OP_CB_83();
        break;
    case 0x84:
        OP_CB_84();
        break;
    case 0x85:
        OP_CB_85();
        break;
    case 0x86:
        OP_CB_86();
        break;
    case 0x87:
        OP_CB_87();
        break;
    case 0x88:
        OP_CB_88();
        break;
    case 0x89:
        OP_CB_89();
        break;
    case 0x8A:
        OP_CB_8A();
        break;
    case 0x8B:
        OP_CB_8B();
        break;
    case 0x8C:
        OP_CB_8C();
        break;
    case 0x8D:
        OP_CB_8D();
        break;
    case 0x8E:
        OP_CB_8E();
        break;
    case 0x8F:
        OP_CB_8F();
        break;
    case 0x90:
        OP_CB_90();
        break;
    case 0x91:
        OP_CB_91();
        break;
    case 0x92:
        OP_CB_92();
        break;
    case 0x93:
        OP_CB_93();
        break;
    case 0x94:
        OP_CB_94();
        break;
    case 0x95:
        OP_CB_95();
        break;
    case 0x96:
        OP_CB_96();
        break;
    case 0x97:
        OP_CB_97();
        break;
    case 0x98:
        OP_CB_98();
        break;
    case 0x99:
        OP_CB_99();
        break;
    case 0x9A:
        OP_CB_9A();
        break;
    case 0x9B:
        OP_CB_9B();
        break;
    case 0x9C:
        OP_CB_9C();
        break;
    case 0x9D:
        OP_CB_9D();
        break;
    case 0x9E:
        OP_CB_9E();
        break;
    case 0x9F:
        OP_CB_9F();
        break;
    case 0xA0:
        OP_CB_A0();
        break;
    case 0xA1:
        OP_CB_A1();
        break;
    case 0xA2:
        OP_CB_A2();
        break;
    case 0xA3:
        OP_CB_A3();
        break;
    case 0xA4:
        OP_CB_A4();
        break;
    case 0xA5:
        OP_CB_A5();
        break;
    case 0xA6:
        OP_CB_A6();
        break;
    case 0xA7:
        OP_CB_A7();
        break;
    case 0xA8:
        OP_CB_A8();
        break;
    case 0xA9:
        OP_CB_A9();
        break;
    case 0xAA:
        OP_CB_AA();
        break;
    case 0xAB:
        OP_CB_AB();
        break;
    case 0xAC:
        OP_CB_AC();
        break;
    case 0xAD:
        OP_CB_AD();
        break;
    case 0xAE:
        OP_CB_AE();
        break;
    case 0xAF:
        OP_CB_AF();
        break;
    case 0xB0:
        OP_CB_B0();
        break;
    case 0xB1:
        OP_CB_B1();
        break;
    case 0xB2:
        OP_CB_B2();
        break;
    case 0xB3:
        OP_CB_B3();
        break;
    case 0xB4:
        OP_CB_B4();
        break;
    case 0xB5:
        OP_CB_B5();
        break;
    case 0xB6:
        OP_CB_B6();
        break;
    case 0xB7:
        OP_CB_B7();
        break;
    case 0xB8:
        OP_CB_B8();
        break;
    case 0xB9:
        OP_CB_B9();
        break;
    case 0xBA:
        OP_CB_BA();
        break;
    case 0xBB:
        OP_CB_BB();
        break;
    case 0xBC:
        OP_CB_BC();
        break;
    case 0xBD:
        OP_CB_BD();
        break;
    case 0xBE:
        OP_CB_BE();
        break;
    case 0xBF:
        OP_CB_BF();
        break;
    case 0xC0:
        OP_CB_C0();
        break;
    case 0xC1:
        OP_CB_C1();
        break;
    case 0xC2:
        OP_CB_C2();
        break;
    case 0xC3:
        OP_CB_C3();
        break;
    case 0xC4:
        OP_CB_C4();
        break;
    case 0xC5:
        OP_CB_C5();
        break;
    case 0xC6:
        OP_CB_C6();
        break;
    case 0xC7:
        OP_CB_C7();
        break;
    case 0xC8:
        OP_CB_C8();
        break;
    case 0xC9:
        OP_CB_C9();
        break;
    case 0xCA:
        OP_CB_CA();
        break;
    case 0xCB:
        OP_CB_CB();
        break;
    case 0xCC:
        OP_CB_CC();
        break;
    case 0xCD:
        OP_CB_CD();
        break;
    case 0xCE:
        OP_CB_CE();
        break;
    case 0xCF:
        OP_CB_CF();
        break;
    case 0xD0:
        OP_CB_D0();
        break;
    case 0xD1:
        OP_CB_D1();
        break;
    case 0xD2:
        OP_CB_D2();
        break;
    case 0xD3:
        OP_CB_D3();
        break;
    case 0xD4:
        OP_CB_D4();
        break;
    case 0xD5:
        OP_CB_D5();
        break;
    case 0xD6:
        OP_CB_D6();
        break;
    case 0xD7:
        OP_CB_D7();
        break;
    case 0xD8:
        OP_CB_D8();
        break;
    case 0xD9:
        OP_CB_D9();
        break;
    case 0xDA:
        OP_CB_DA();
        break;
    case 0xDB:
        OP_CB_DB();
        break;
    case 0xDC:
        OP_CB_DC();
        break;
    case 0xDD:
        OP_CB_DD();
        break;
    case 0xDE:
        OP_CB_DE();
        break;
    case 0xDF:
        OP_CB_DF();
        break;
    case 0xE0:
        OP_CB_E0();
        break;
    case 0xE1:
        OP_CB_E1();
        break;
    case 0xE2:
        OP_CB_E2();
        break;
    case 0xE3:
        OP_CB_E3();
        break;
    case 0xE4:
        OP_CB_E4();
        break;
    case 0xE5:
        OP_CB_E5();
        break;
    case 0xE6:
        OP_CB_E6();
        break;
    case 0xE7:
        OP_CB_E7();
        break;
    case 0xE8:
        OP_CB_E8();
        break;
    case 0xE9:
        OP_CB_E9();
        break;
    case 0xEA:
        OP_CB_EA();
        break;
    case 0xEB:
        OP_CB_EB();
        break;
    case 0xEC:
        OP_CB_EC();
        break;
    case 0xED:
        OP_CB_ED();
        break;
    case 0xEE:
        OP_CB_EE();
        break;
    case 0xEF:
        OP_CB_EF();
        break;
    case 0xF0:
        OP_CB_F0();
        break;
    case 0xF1:
        OP_CB_F1();
        break;
    case 0xF2:
        OP_CB_F2();
        break;
    case 0xF3:
        OP_CB_F3();
        break;
    case 0xF4:
        OP_CB_F4();
        break;
    case 0xF5:
        OP_CB_F5();
        break;
    case 0xF6:
        OP_CB_F6();
        break;
    case 0xF7:
        OP_CB_F7();
        break;
    case 0xF8:
        OP_CB_F8();
        break;
    case 0xF9:
        OP_CB_F9();
        break;
    case 0xFA:
        OP_CB_FA();
        break;
    case 0xFB:
        OP_CB_FB();
        break;
    case 0xFC:
        OP_CB_FC();
        break;
    case 0xFD:
        OP_CB_FD();
        break;
    case 0xFE:
        OP_CB_FE();
        break;
    case 0xFF:
        OP_CB_FF();
        break;
    }
}

// CALL Z, n16
void SM83::OP_CC() { op_call_cc_n16(getZeroFlag(), 1); }

// CALL n16
void SM83::OP_CD() { op_call_n16(); }

// ADC A, n8
void SM83::OP_CE() { op_adc_a_n8(); }

// RST 0x08
void SM83::OP_CF() { op_rst(0x08); }

// RET NC
void SM83::OP_D0() { op_ret_cc(getCarryFlag(), 0); }

// POP DE
void SM83::OP_D1() { op_pop_r16(D, E); }

// JP NC, n16
void SM83::OP_D2() { op_jp_cc_n16(getCarryFlag(), 0); }

// Illegal opcode
void SM83::OP_D3() {}

// CALL NC, n16
void SM83::OP_D4() { op_call_cc_n16(getCarryFlag(), 0); }

// PUSH DE
void SM83::OP_D5() { op_push_r16(D, E); }

// SUB n8
void SM83::OP_D6() { op_sub_a_n8(); }

// RST 0x10
void SM83::OP_D7() { op_rst(0x10); }

// RET C
void SM83::OP_D8() { op_ret_cc(getCarryFlag(), 1); }

// RETI
void SM83::OP_D9() { op_reti(); }

// JP C, n16
void SM83::OP_DA() { op_jp_cc_n16(getCarryFlag(), 1); }

// Illegal opcode
void SM83::OP_DB() {}

// CALL C, n16
void SM83::OP_DC() { op_call_cc_n16(getCarryFlag(), 1); }

// Illegal opcode
void SM83::OP_DD() {}

// SBC A, n8
void SM83::OP_DE() { op_sbc_a_n8(); }

// RST 0x18
void SM83::OP_DF() { op_rst(0x18); }

// LDH [n8], A
void SM83::OP_E0() { op_ldh_addr_n16_a(); }

// POP HL
void SM83::OP_E1() { op_pop_r16(H, L); }

// LD [C], A
void SM83::OP_E2() { op_ldh_addr_c_a(); }

// Illegal opcode
void SM83::OP_E3() {}

// Illegal opcode
void SM83::OP_E4() {}

// PUSH HL
void SM83::OP_E5() { op_push_r16(H, L); }

// AND n8
void SM83::OP_E6() { op_and_a_n8(); }

// RST 0x20
void SM83::OP_E7() { op_rst(0x20); }

// ADD SP, e8
void SM83::OP_E8() { op_add_sp_e8(); }

// JP [HL]
void SM83::OP_E9() { op_jp_hl(); }

// LD [n16], A
void SM83::OP_EA() { op_ld_addr_n16_a(); }

// Illegal opcode
void SM83::OP_EB() {}

// Illegal opcode
void SM83::OP_EC() {}

// Illegal opcode
void SM83::OP_ED() {}

// XOR n8
void SM83::OP_EE() { op_xor_a_n8(); }

// RST 0x28
void SM83::OP_EF() { op_rst(0x28); }

// LDH A, [n8]
void SM83::OP_F0() { op_ldh_a_addr_n16(); }

// POP AF
void SM83::OP_F1() { op_pop_af(); }

// LD A, [C]
void SM83::OP_F2() { op_ldh_a_addr_c(); }

// DI
void SM83::OP_F3() { op_di(); }

// Illegal opcode
void SM83::OP_F4() {}

// PUSH AF
void SM83::OP_F5() { op_push_af(); }

// OR n8
void SM83::OP_F6() { op_or_a_n8(); }

// RST 0x30
void SM83::OP_F7() { op_rst(0x30); }

// LD HL, SP + e8
void SM83::OP_F8() { op_ld_hl_sp_plus_e8(); }

// LD SP, HL
void SM83::OP_F9() { op_ld_sp_hl(); }

// LD A, [n16]
void SM83::OP_FA() { op_ld_a_addr_n16(); }

// EI
void SM83::OP_FB() { op_ei(); }

// Illegal opcode
void SM83::OP_FC() {}

// Ilegal opcode
void SM83::OP_FD() {}

// CP n8
void SM83::OP_FE() { op_cp_a_n8(); }

// RST 0x38
void SM83::OP_FF() { op_rst(0x38); }

// RLC B
void SM83::OP_CB_00() { op_rlc_r8(B); }

// RLC C
void SM83::OP_CB_01() { op_rlc_r8(C); }

// RLC D
void SM83::OP_CB_02() { op_rlc_r8(D); }

// RLC E
void SM83::OP_CB_03() { op_rlc_r8(E); }

// RLC H
void SM83::OP_CB_04() { op_rlc_r8(H); }

// RLC L
void SM83::OP_CB_05() { op_rlc_r8(L); }

// RLC [HL]
void SM83::OP_CB_06() { op_rlc_addr_hl(); }

// RLC A
void SM83::OP_CB_07() { op_rlc_r8(A); }

// RRC B
void SM83::OP_CB_08() { op_rrc_r8(B); }

// RRC C
void SM83::OP_CB_09() { op_rrc_r8(C); }

// RRC D
void SM83::OP_CB_0A() { op_rrc_r8(D); }

// RRC E
void SM83::OP_CB_0B() { op_rrc_r8(E); }

// RRC H
void SM83::OP_CB_0C() { op_rrc_r8(H); }

// RRC L
void SM83::OP_CB_0D() { op_rrc_r8(L); }

// RRC [HL]
void SM83::OP_CB_0E() { op_rrc_addr_hl(); }

// RRC A
void SM83::OP_CB_0F() { op_rrc_r8(A); }

// RL B
void SM83::OP_CB_10() { op_rl_r8(B); }

// RL C
void SM83::OP_CB_11() { op_rl_r8(C); }

// RL D
void SM83::OP_CB_12() { op_rl_r8(D); }

// RL E
void SM83::OP_CB_13() { op_rl_r8(E); }

// RL H
void SM83::OP_CB_14() { op_rl_r8(H); }

// RL L
void SM83::OP_CB_15() { op_rl_r8(L); }

// RL [HL]
void SM83::OP_CB_16() { op_rl_addr_hl(); }

// RL A
void SM83::OP_CB_17() { op_rl_r8(A); }

// RR B
void SM83::OP_CB_18() { op_rr_r8(B); }

// RR C
void SM83::OP_CB_19() { op_rr_r8(C); }

// RR D
void SM83::OP_CB_1A() { op_rr_r8(D); }

// RR E
void SM83::OP_CB_1B() { op_rr_r8(E); }

// RR H
void SM83::OP_CB_1C() { op_rr_r8(H); }

// RR L
void SM83::OP_CB_1D() { op_rr_r8(L); }

// RR [HL]
void SM83::OP_CB_1E() { op_rr_addr_hl(); }

// RR A
void SM83::OP_CB_1F() { op_rr_r8(A); }

// SLA B
void SM83::OP_CB_20() { op_sla_r8(B); }

// SLA C
void SM83::OP_CB_21() { op_sla_r8(C); }

// SLA D
void SM83::OP_CB_22() { op_sla_r8(D); }

// SLA E
void SM83::OP_CB_23() { op_sla_r8(E); }

// SLA H
void SM83::OP_CB_24() { op_sla_r8(H); }

// SLA L
void SM83::OP_CB_25() { op_sla_r8(L); }

// SLA [HL]
void SM83::OP_CB_26() { op_sla_addr_hl(); }

// SLA A
void SM83::OP_CB_27() { op_sla_r8(A); }

// SRA B
void SM83::OP_CB_28() { op_sra_r8(B); }

// SRA C
void SM83::OP_CB_29() { op_sra_r8(C); }

// SRA D
void SM83::OP_CB_2A() { op_sra_r8(D); }

// SRA E
void SM83::OP_CB_2B() { op_sra_r8(E); }

// SRA H
void SM83::OP_CB_2C() { op_sra_r8(H); }

// SRA L
void SM83::OP_CB_2D() { op_sra_r8(L); }

// SRA [HL]
void SM83::OP_CB_2E() { op_sra_addr_hl(); }

// SRA A
void SM83::OP_CB_2F() { op_sra_r8(A); }

// SWAP B
void SM83::OP_CB_30() { op_swap_r8(B); }

// SWAP C
void SM83::OP_CB_31() { op_swap_r8(C); }

// SWAP D
void SM83::OP_CB_32() { op_swap_r8(D); }

// SWAP E
void SM83::OP_CB_33() { op_swap_r8(E); }

// SWAP H
void SM83::OP_CB_34() { op_swap_r8(H); }

// SWAP L
void SM83::OP_CB_35() { op_swap_r8(L); }

// SWAP [HL]
void SM83::OP_CB_36() { op_swap_addr_hl(); }

// SWAP A
void SM83::OP_CB_37() { op_swap_r8(A); }

// SRL B
void SM83::OP_CB_38() { op_srl_r8(B); }

// SRL C
void SM83::OP_CB_39() { op_srl_r8(C); }

// SRL D
void SM83::OP_CB_3A() { op_srl_r8(D); }

// SRL E
void SM83::OP_CB_3B() { op_srl_r8(E); }

// SRL H
void SM83::OP_CB_3C() { op_srl_r8(H); }

// SRL L
void SM83::OP_CB_3D() { op_srl_r8(L); }

// SRL [HL]
void SM83::OP_CB_3E() { op_srl_addr_hl(); }

// SRL A
void SM83::OP_CB_3F() { op_srl_r8(A); }

// BIT 0, B
void SM83::OP_CB_40() { op_bit_u3_r8(0, B); }

// BIT 0, C
void SM83::OP_CB_41() { op_bit_u3_r8(0, C); }

// BIT 0, D
void SM83::OP_CB_42() { op_bit_u3_r8(0, D); }

// BIT 0, E
void SM83::OP_CB_43() { op_bit_u3_r8(0, E); }

// BIT 0, H
void SM83::OP_CB_44() { op_bit_u3_r8(0, H); }

// BIT 0, L
void SM83::OP_CB_45() { op_bit_u3_r8(0, L); }

// BIT 0, [HL]
void SM83::OP_CB_46() { op_bit_u3_addr_hl(0); }

// BIT 0, A
void SM83::OP_CB_47() { op_bit_u3_r8(0, A); }

// BIT 1, B
void SM83::OP_CB_48() { op_bit_u3_r8(1, B); }

// BIT 1, C
void SM83::OP_CB_49() { op_bit_u3_r8(1, C); }

// BIT 1, D
void SM83::OP_CB_4A() { op_bit_u3_r8(1, D); }

// BIT 1, E
void SM83::OP_CB_4B() { op_bit_u3_r8(1, E); }

// BIT 1, H
void SM83::OP_CB_4C() { op_bit_u3_r8(1, H); }

// BIT 1, L
void SM83::OP_CB_4D() { op_bit_u3_r8(1, L); }

// BIT 1, [HL]
void SM83::OP_CB_4E() { op_bit_u3_addr_hl(1); }

// BIT 1, A
void SM83::OP_CB_4F() { op_bit_u3_r8(1, A); }

// BIT 2, B
void SM83::OP_CB_50() { op_bit_u3_r8(2, B); }

// BIT 2, C
void SM83::OP_CB_51() { op_bit_u3_r8(2, C); }

// BIT 2, D
void SM83::OP_CB_52() { op_bit_u3_r8(2, D); }

// BIT 2, E
void SM83::OP_CB_53() { op_bit_u3_r8(2, E); }

// BIT 2, H
void SM83::OP_CB_54() { op_bit_u3_r8(2, H); }

// BIT 2, L
void SM83::OP_CB_55() { op_bit_u3_r8(2, L); }

// BIT 2, [HL]
void SM83::OP_CB_56() { op_bit_u3_addr_hl(2); }

// BIT 2, A
void SM83::OP_CB_57() { op_bit_u3_r8(2, A); }

// BIT 3, B
void SM83::OP_CB_58() { op_bit_u3_r8(3, B); }

// BIT 3, C
void SM83::OP_CB_59() { op_bit_u3_r8(3, C); }

// BIT 3, D
void SM83::OP_CB_5A() { op_bit_u3_r8(3, D); }

// BIT 3, E
void SM83::OP_CB_5B() { op_bit_u3_r8(3, E); }

// BIT 3, H
void SM83::OP_CB_5C() { op_bit_u3_r8(3, H); }

// BIT 3, L
void SM83::OP_CB_5D() { op_bit_u3_r8(3, L); }

// BIT 3, [HL]
void SM83::OP_CB_5E() { op_bit_u3_addr_hl(3); }

// BIT 3, A
void SM83::OP_CB_5F() { op_bit_u3_r8(3, A); }

// BIT 4, B
void SM83::OP_CB_60() { op_bit_u3_r8(4, B); }

// BIT 4, C
void SM83::OP_CB_61() { op_bit_u3_r8(4, C); }

// BIT 4, D
void SM83::OP_CB_62() { op_bit_u3_r8(4, D); }

// BIT 4, E
void SM83::OP_CB_63() { op_bit_u3_r8(4, E); }

// BIT 4, H
void SM83::OP_CB_64() { op_bit_u3_r8(4, H); }

// BIT 4, L
void SM83::OP_CB_65() { op_bit_u3_r8(4, L); }

// BIT 4, [HL]
void SM83::OP_CB_66() { op_bit_u3_addr_hl(4); }

// BIT 4, A
void SM83::OP_CB_67() { op_bit_u3_r8(4, A); }

// BIT 5, B
void SM83::OP_CB_68() { op_bit_u3_r8(5, B); }

// BIT 5, C
void SM83::OP_CB_69() { op_bit_u3_r8(5, C); }

// BIT 5, D
void SM83::OP_CB_6A() { op_bit_u3_r8(5, D); }

// BIT 5, E
void SM83::OP_CB_6B() { op_bit_u3_r8(5, E); }

// BIT 5, H
void SM83::OP_CB_6C() { op_bit_u3_r8(5, H); }

// BIT 5, L
void SM83::OP_CB_6D() { op_bit_u3_r8(5, L); }

// BIT 5, [HL]
void SM83::OP_CB_6E() { op_bit_u3_addr_hl(5); }

// BIT 5, A
void SM83::OP_CB_6F() { op_bit_u3_r8(5, A); }

// BIT 6, B
void SM83::OP_CB_70() { op_bit_u3_r8(6, B); }

// BIT 6, C
void SM83::OP_CB_71() { op_bit_u3_r8(6, C); }

// BIT 6, D
void SM83::OP_CB_72() { op_bit_u3_r8(6, D); }

// BIT 6, E
void SM83::OP_CB_73() { op_bit_u3_r8(6, E); }

// BIT 6, H
void SM83::OP_CB_74() { op_bit_u3_r8(6, H); }

// BIT 6, L
void SM83::OP_CB_75() { op_bit_u3_r8(6, L); }

// BIT 6, [HL]
void SM83::OP_CB_76() { op_bit_u3_addr_hl(6); }

// BIT 6, A
void SM83::OP_CB_77() { op_bit_u3_r8(6, A); }

// BIT 7, B
void SM83::OP_CB_78() { op_bit_u3_r8(7, B); }

// BIT 7, C
void SM83::OP_CB_79() { op_bit_u3_r8(7, C); }

// BIT 7, D
void SM83::OP_CB_7A() { op_bit_u3_r8(7, D); }

// BIT 7, E
void SM83::OP_CB_7B() { op_bit_u3_r8(7, E); }

// BIT 7, H
void SM83::OP_CB_7C() { op_bit_u3_r8(7, H); }

// BIT 7, L
void SM83::OP_CB_7D() { op_bit_u3_r8(7, L); }

// BIT 7, [HL]
void SM83::OP_CB_7E() { op_bit_u3_addr_hl(7); }

// BIT 7, A
void SM83::OP_CB_7F() { op_bit_u3_r8(7, A); }

// RES 0, B
void SM83::OP_CB_80() { op_res_u3_r8(0, B); }

// RES 0, C
void SM83::OP_CB_81() { op_res_u3_r8(0, C); }

// RES 0, D
void SM83::OP_CB_82() { op_res_u3_r8(0, D); }

// RES 0, E
void SM83::OP_CB_83() { op_res_u3_r8(0, E); }

// RES 0, H
void SM83::OP_CB_84() { op_res_u3_r8(0, H); }

// RES 0, L
void SM83::OP_CB_85() { op_res_u3_r8(0, L); }

// RES 0, [HL]
void SM83::OP_CB_86() { op_res_u3_addr_hl(0); }

// RES 0, A
void SM83::OP_CB_87() { op_res_u3_r8(0, A); }

// RES 1, B
void SM83::OP_CB_88() { op_res_u3_r8(1, B); }

// RES 1, C
void SM83::OP_CB_89() { op_res_u3_r8(1, C); }

// RES 1, D
void SM83::OP_CB_8A() { op_res_u3_r8(1, D); }

// RES 1, E
void SM83::OP_CB_8B() { op_res_u3_r8(1, E); }

// RES 1, H
void SM83::OP_CB_8C() { op_res_u3_r8(1, H); }

// RES 1, L
void SM83::OP_CB_8D() { op_res_u3_r8(1, L); }

// RES 1, [HL]
void SM83::OP_CB_8E() { op_res_u3_addr_hl(1); }

// RES 1, A
void SM83::OP_CB_8F() { op_res_u3_r8(1, A); }

// RES 2, B
void SM83::OP_CB_90() { op_res_u3_r8(2, B); }

// RES 2, C
void SM83::OP_CB_91() { op_res_u3_r8(2, C); }

// RES 2, D
void SM83::OP_CB_92() { op_res_u3_r8(2, D); }

// RES 2, E
void SM83::OP_CB_93() { op_res_u3_r8(2, E); }

// RES 2, H
void SM83::OP_CB_94() { op_res_u3_r8(2, H); }

// RES 2, L
void SM83::OP_CB_95() { op_res_u3_r8(2, L); }

// RES 2, [HL]
void SM83::OP_CB_96() { op_res_u3_addr_hl(2); }

// RES 2, A
void SM83::OP_CB_97() { op_res_u3_r8(2, A); }

// RES 3, B
void SM83::OP_CB_98() { op_res_u3_r8(3, B); }

// RES 3, C
void SM83::OP_CB_99() { op_res_u3_r8(3, C); }

// RES 3, D
void SM83::OP_CB_9A() { op_res_u3_r8(3, D); }

// RES 3, E
void SM83::OP_CB_9B() { op_res_u3_r8(3, E); }

// RES 3, H
void SM83::OP_CB_9C() { op_res_u3_r8(3, H); }

// RES 3, L
void SM83::OP_CB_9D() { op_res_u3_r8(3, L); }

// RES 3, [HL]
void SM83::OP_CB_9E() { op_res_u3_addr_hl(3); }

// RES 3, A
void SM83::OP_CB_9F() { op_res_u3_r8(3, A); }

// RES 4, B
void SM83::OP_CB_A0() { op_res_u3_r8(4, B); }

// RES 4, C
void SM83::OP_CB_A1() { op_res_u3_r8(4, C); }

// RES 4, D
void SM83::OP_CB_A2() { op_res_u3_r8(4, D); }

// RES 4, E
void SM83::OP_CB_A3() { op_res_u3_r8(4, E); }

// RES 4, H
void SM83::OP_CB_A4() { op_res_u3_r8(4, H); }

// RES 4, L
void SM83::OP_CB_A5() { op_res_u3_r8(4, L); }

// RES 4, [HL]
void SM83::OP_CB_A6() { op_res_u3_addr_hl(4); }

// RES 4, A
void SM83::OP_CB_A7() { op_res_u3_r8(4, A); }

// RES 5, B
void SM83::OP_CB_A8() { op_res_u3_r8(5, B); }

// RES 5, C
void SM83::OP_CB_A9() { op_res_u3_r8(5, C); }

// RES 5, D
void SM83::OP_CB_AA() { op_res_u3_r8(5, D); }

// RES 5, E
void SM83::OP_CB_AB() { op_res_u3_r8(5, E); }

// RES 5, H
void SM83::OP_CB_AC() { op_res_u3_r8(5, H); }

// RES 5, L
void SM83::OP_CB_AD() { op_res_u3_r8(5, L); }

// RES 5, [HL]
void SM83::OP_CB_AE() { op_res_u3_addr_hl(5); }

// RES 5, A
void SM83::OP_CB_AF() { op_res_u3_r8(5, A); }

// RES 6, B
void SM83::OP_CB_B0() { op_res_u3_r8(6, B); }

// RES 6, C
void SM83::OP_CB_B1() { op_res_u3_r8(6, C); }

// RES 6, D
void SM83::OP_CB_B2() { op_res_u3_r8(6, D); }

// RES 6, E
void SM83::OP_CB_B3() { op_res_u3_r8(6, E); }

// RES 6, H
void SM83::OP_CB_B4() { op_res_u3_r8(6, H); }

// RES 6, L
void SM83::OP_CB_B5() { op_res_u3_r8(6, L); }

// RES 6, [HL]
void SM83::OP_CB_B6() { op_res_u3_addr_hl(6); }

// RES 6, A
void SM83::OP_CB_B7() { op_res_u3_r8(6, A); }

// RES 7, B
void SM83::OP_CB_B8() { op_res_u3_r8(7, B); }

// RES 7, C
void SM83::OP_CB_B9() { op_res_u3_r8(7, C); }

// RES 7, D
void SM83::OP_CB_BA() { op_res_u3_r8(7, D); }

// RES 7, E
void SM83::OP_CB_BB() { op_res_u3_r8(7, E); }

// RES 7, H
void SM83::OP_CB_BC() { op_res_u3_r8(7, H); }

// RES 7, L
void SM83::OP_CB_BD() { op_res_u3_r8(7, L); }

// RES 7, [HL]
void SM83::OP_CB_BE() { op_res_u3_addr_hl(7); }

// RES 7, A
void SM83::OP_CB_BF() { op_res_u3_r8(7, A); }

// SET 0, B
void SM83::OP_CB_C0() { op_set_u3_r8(0, B); }

// SET 0, C
void SM83::OP_CB_C1() { op_set_u3_r8(0, C); }

// SET 0, D
void SM83::OP_CB_C2() { op_set_u3_r8(0, D); }

// SET 0, E
void SM83::OP_CB_C3() { op_set_u3_r8(0, E); }

// SET 0, H
void SM83::OP_CB_C4() { op_set_u3_r8(0, H); }

// SET 0, L
void SM83::OP_CB_C5() { op_set_u3_r8(0, L); }

// SET 0, [HL]
void SM83::OP_CB_C6() { op_set_u3_addr_hl(0); }

// SET 0, A
void SM83::OP_CB_C7() { op_set_u3_r8(0, A); }

// SET 1, B
void SM83::OP_CB_C8() { op_set_u3_r8(1, B); }

// SET 1, C
void SM83::OP_CB_C9() { op_set_u3_r8(1, C); }

// SET 1, D
void SM83::OP_CB_CA() { op_set_u3_r8(1, D); }

// SET 1, E
void SM83::OP_CB_CB() { op_set_u3_r8(1, E); }

// SET 1, H
void SM83::OP_CB_CC() { op_set_u3_r8(1, H); }

// SET 1, L
void SM83::OP_CB_CD() { op_set_u3_r8(1, L); }

// SET 1, [HL]
void SM83::OP_CB_CE() { op_set_u3_addr_hl(1); }

// SET 1, A
void SM83::OP_CB_CF() { op_set_u3_r8(1, A); }

// SET 2, B
void SM83::OP_CB_D0() { op_set_u3_r8(2, B); }

// SET 2, C
void SM83::OP_CB_D1() { op_set_u3_r8(2, C); }

// SET 2, D
void SM83::OP_CB_D2() { op_set_u3_r8(2, D); }

// SET 2, E
void SM83::OP_CB_D3() { op_set_u3_r8(2, E); }

// SET 2, H
void SM83::OP_CB_D4() { op_set_u3_r8(2, H); }

// SET 2, L
void SM83::OP_CB_D5() { op_set_u3_r8(2, L); }

// SET 2, [HL]
void SM83::OP_CB_D6() { op_set_u3_addr_hl(2); }

// SET 2, A
void SM83::OP_CB_D7() { op_set_u3_r8(2, A); }

// SET 3, B
void SM83::OP_CB_D8() { op_set_u3_r8(3, B); }

// SET 3, C
void SM83::OP_CB_D9() { op_set_u3_r8(3, C); }

// SET 3, D
void SM83::OP_CB_DA() { op_set_u3_r8(3, D); }

// SET 3, E
void SM83::OP_CB_DB() { op_set_u3_r8(3, E); }

// SET 3, H
void SM83::OP_CB_DC() { op_set_u3_r8(3, H); }

// SET 3, L
void SM83::OP_CB_DD() { op_set_u3_r8(3, L); }

// SET 3, [HL]
void SM83::OP_CB_DE() { op_set_u3_addr_hl(3); }

// SET 3, A
void SM83::OP_CB_DF() { op_set_u3_r8(3, A); }

// SET 4, B
void SM83::OP_CB_E0() { op_set_u3_r8(4, B); }

// SET 4, C
void SM83::OP_CB_E1() { op_set_u3_r8(4, C); }

// SET 4, D
void SM83::OP_CB_E2() { op_set_u3_r8(4, D); }

// SET 4, E
void SM83::OP_CB_E3() { op_set_u3_r8(4, E); }

// SET 4, H
void SM83::OP_CB_E4() { op_set_u3_r8(4, H); }

// SET 4, L
void SM83::OP_CB_E5() { op_set_u3_r8(4, L); }

// SET 4, [HL]
void SM83::OP_CB_E6() { op_set_u3_addr_hl(4); }

// SET 4, A
void SM83::OP_CB_E7() { op_set_u3_r8(4, A); }

// SET 5, B
void SM83::OP_CB_E8() { op_set_u3_r8(5, B); }

// SET 5, C
void SM83::OP_CB_E9() { op_set_u3_r8(5, C); }

// SET 5, D
void SM83::OP_CB_EA() { op_set_u3_r8(5, D); }

// SET 5, E
void SM83::OP_CB_EB() { op_set_u3_r8(5, E); }

// SET 5, H
void SM83::OP_CB_EC() { op_set_u3_r8(5, H); }

// SET 5, L
void SM83::OP_CB_ED() { op_set_u3_r8(5, L); }

// SET 5, [HL]
void SM83::OP_CB_EE() { op_set_u3_addr_hl(5); }

// SET 5, A
void SM83::OP_CB_EF() { op_set_u3_r8(5, A); }

// SET 6, B
void SM83::OP_CB_F0() { op_set_u3_r8(6, B); }

// SET 6, C
void SM83::OP_CB_F1() { op_set_u3_r8(6, C); }

// SET 6, D
void SM83::OP_CB_F2() { op_set_u3_r8(6, D); }

// SET 6, E
void SM83::OP_CB_F3() { op_set_u3_r8(6, E); }

// SET 6, H
void SM83::OP_CB_F4() { op_set_u3_r8(6, H); }

// SET 6, L
void SM83::OP_CB_F5() { op_set_u3_r8(6, L); }

// SET 6, [HL]
void SM83::OP_CB_F6() { op_set_u3_addr_hl(6); }

// SET 6, A
void SM83::OP_CB_F7() { op_set_u3_r8(6, A); }

// SET 7, B
void SM83::OP_CB_F8() { op_set_u3_r8(7, B); }

// SET 7, C
void SM83::OP_CB_F9() { op_set_u3_r8(7, C); }

// SET 7, D
void SM83::OP_CB_FA() { op_set_u3_r8(7, D); }

// SET 7, E
void SM83::OP_CB_FB() { op_set_u3_r8(7, E); }

// SET 7, H
void SM83::OP_CB_FC() { op_set_u3_r8(7, H); }

// SET 7, L
void SM83::OP_CB_FD() { op_set_u3_r8(7, L); }

// SET 7, [HL]
void SM83::OP_CB_FE() { op_set_u3_addr_hl(7); }

// SET 7, A
void SM83::OP_CB_FF() { op_set_u3_r8(7, A); }

