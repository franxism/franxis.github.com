/** Z80: portable Z80 emulator **********************************************/
/**                                                                        **/
/**                                CodeXX.c                                **/
/**                                                                        **/
/** This file contains function used by both DD XX and FD XX opcode        **/
/** emulation. It is included from CodeDD.c and CodeFD.c                   **/
/**                                                                        **/
/** Copyright (C) Marat Fayzullin 1994,1995,1996                           **/
/**               Marcel de Kogel 1996                                     **/
/**     You are not allowed to distribute this software commercially       **/
/**     Please, notify me, if you make any changes to this file            **/
/****************************************************************************/

static void rlc_xhl(pair J){byte I; I=M_RDMEM(J.W.l);M_RLC(I);M_WRMEM(J.W.l,I);}
static void rrc_xhl(pair J){byte I; I=M_RDMEM(J.W.l);M_RRC(I);M_WRMEM(J.W.l,I);}
static void rl_xhl(pair J){byte I;  I=M_RDMEM(J.W.l);M_RL(I);M_WRMEM(J.W.l,I);}
static void rr_xhl(pair J){byte I;  I=M_RDMEM(J.W.l);M_RR(I);M_WRMEM(J.W.l,I);}
static void sla_xhl(pair J){byte I; I=M_RDMEM(J.W.l);M_SLA(I);M_WRMEM(J.W.l,I);}
static void sra_xhl(pair J){byte I; I=M_RDMEM(J.W.l);M_SRA(I);M_WRMEM(J.W.l,I);}
static void sll_xhl(pair J){byte I; I=M_RDMEM(J.W.l);M_SLL(I);M_WRMEM(J.W.l,I);}
static void srl_xhl(pair J){byte I; I=M_RDMEM(J.W.l);M_SRL(I);M_WRMEM(J.W.l,I);}
static void bit0_xhl(pair J){byte I; I=M_RDMEM(J.W.l);M_BIT(0,I);}
static void bit1_xhl(pair J){byte I; I=M_RDMEM(J.W.l);M_BIT(1,I);}
static void bit2_xhl(pair J){byte I; I=M_RDMEM(J.W.l);M_BIT(2,I);}
static void bit3_xhl(pair J){byte I; I=M_RDMEM(J.W.l);M_BIT(3,I);}
static void bit4_xhl(pair J){byte I; I=M_RDMEM(J.W.l);M_BIT(4,I);}
static void bit5_xhl(pair J){byte I; I=M_RDMEM(J.W.l);M_BIT(5,I);}
static void bit6_xhl(pair J){byte I; I=M_RDMEM(J.W.l);M_BIT(6,I);}
static void bit7_xhl(pair J){byte I; I=M_RDMEM(J.W.l);M_BIT(7,I);}
static void res0_xhl(pair J){byte I; I=M_RDMEM(J.W.l);M_RES(0,I);M_WRMEM(J.W.l,I);}
static void res1_xhl(pair J){byte I; I=M_RDMEM(J.W.l);M_RES(1,I);M_WRMEM(J.W.l,I);}
static void res2_xhl(pair J){byte I; I=M_RDMEM(J.W.l);M_RES(2,I);M_WRMEM(J.W.l,I);}
static void res3_xhl(pair J){byte I; I=M_RDMEM(J.W.l);M_RES(3,I);M_WRMEM(J.W.l,I);}
static void res4_xhl(pair J){byte I; I=M_RDMEM(J.W.l);M_RES(4,I);M_WRMEM(J.W.l,I);}
static void res5_xhl(pair J){byte I; I=M_RDMEM(J.W.l);M_RES(5,I);M_WRMEM(J.W.l,I);}
static void res6_xhl(pair J){byte I; I=M_RDMEM(J.W.l);M_RES(6,I);M_WRMEM(J.W.l,I);}
static void res7_xhl(pair J){byte I; I=M_RDMEM(J.W.l);M_RES(7,I);M_WRMEM(J.W.l,I);}
static void set0_xhl(pair J){byte I; I=M_RDMEM(J.W.l);M_SET(0,I);M_WRMEM(J.W.l,I);}
static void set1_xhl(pair J){byte I; I=M_RDMEM(J.W.l);M_SET(1,I);M_WRMEM(J.W.l,I);}
static void set2_xhl(pair J){byte I; I=M_RDMEM(J.W.l);M_SET(2,I);M_WRMEM(J.W.l,I);}
static void set3_xhl(pair J){byte I; I=M_RDMEM(J.W.l);M_SET(3,I);M_WRMEM(J.W.l,I);}
static void set4_xhl(pair J){byte I; I=M_RDMEM(J.W.l);M_SET(4,I);M_WRMEM(J.W.l,I);}
static void set5_xhl(pair J){byte I; I=M_RDMEM(J.W.l);M_SET(5,I);M_WRMEM(J.W.l,I);}
static void set6_xhl(pair J){byte I; I=M_RDMEM(J.W.l);M_SET(6,I);M_WRMEM(J.W.l,I);}
static void set7_xhl(pair J){byte I; I=M_RDMEM(J.W.l);M_SET(7,I);M_WRMEM(J.W.l,I);}
static void fuck (pair J)
{
 Z80_BadOpcode ();
}

static void add_h(void){ M_ADD(R.XX.B.h);}
static void add_l(void){ M_ADD(R.XX.B.l);}          
static void add_xhl(void){byte I; I=M_RDMEM(R.XX.D+(offset)M_RDMEM_OPCODE());M_ADD(I);}           
static void sub_h(void){ M_SUB(R.XX.B.h);}          
static void sub_l(void){ M_SUB(R.XX.B.l);}          
static void sub_xhl(void){byte I; I=M_RDMEM(R.XX.D+(offset)M_RDMEM_OPCODE());M_SUB(I);}           
static void and_h(void){    M_AND(R.XX.B.h);}          
static void and_l(void){    M_AND(R.XX.B.l);}          
static void and_xhl(void){byte I;  I=M_RDMEM(R.XX.D+(offset)M_RDMEM_OPCODE());M_AND(I);}           
static void or_h(void){     M_OR(R.XX.B.h);}         
static void or_l(void){     M_OR(R.XX.B.l);}         
static void or_xhl(void){byte I;   I=M_RDMEM(R.XX.D+(offset)M_RDMEM_OPCODE());M_OR(I);}
static void adc_h(void){ M_ADC(R.XX.B.h);}
static void adc_l(void){ M_ADC(R.XX.B.l);}          
static void adc_xhl(void){byte I; I=M_RDMEM(R.XX.D+(offset)M_RDMEM_OPCODE());M_ADC(I);}           
static void sbc_h(void){ M_SBC(R.XX.B.h);}          
static void sbc_l(void){ M_SBC(R.XX.B.l);}          
static void sbc_xhl(void){byte I; I=M_RDMEM(R.XX.D+(offset)M_RDMEM_OPCODE());M_SBC(I);}           
static void xor_h(void){    M_XOR(R.XX.B.h);}          
static void xor_l(void){    M_XOR(R.XX.B.l);}          
static void xor_xhl(void){byte I;  I=M_RDMEM(R.XX.D+(offset)M_RDMEM_OPCODE());M_XOR(I);}           
static void cp_h(void){ M_CP(R.XX.B.h);}         
static void cp_l(void){ M_CP(R.XX.B.l);}         
static void cp_xhl(void){byte I; I=M_RDMEM(R.XX.D+(offset)M_RDMEM_OPCODE());M_CP(I);}          
static void ld_hl_word(void){ M_LDWORD(XX);}             
static void ld_pc_hl(void){ R.PC.D=R.XX.D;}             
static void ld_sp_hl(void){ R.SP.D=R.XX.D;}             
static void add_hl_bc(void){  M_ADDW(XX,BC);}            
static void add_hl_de(void){  M_ADDW(XX,DE);}            
static void add_hl_hl(void){  M_ADDW(XX,XX);}            
static void add_hl_sp(void){  M_ADDW(XX,SP);}            
static void dec_hl(void){   R.XX.W.l--;}           
static void inc_hl(void){   R.XX.W.l++;}           
static void dec_h(void){    M_DEC(R.XX.B.h);}          
static void dec_l(void){    M_DEC(R.XX.B.l);}          
static void dec_xhl(void)
{
 byte I;pair J;offset K;
 K=(offset)M_RDMEM_OPCODE();
 I=M_RDMEM(R.XX.D+K);M_DEC(I);
 J.W.l=R.XX.W.l+K;M_WRMEM(J.W.l,I);
}
static void inc_h(void){    M_INC(R.XX.B.h);}          
static void inc_l(void){    M_INC(R.XX.B.l);}          
static void inc_xhl(void)
{
 byte I;pair J; offset K;
 K=(offset)M_RDMEM_OPCODE();
 I=M_RDMEM(R.XX.D+K);M_INC(I);
 J.W.l=R.XX.W.l+K;M_WRMEM(J.W.l,I);
}
static void push_hl(void){  M_PUSH(XX);}    
static void pop_hl(void){   M_POP(XX);}   
static void ld_h_b(void){   R.XX.B.h=R.BC.B.h;}  
static void ld_l_b(void){   R.XX.B.l=R.BC.B.h;}  
static void ld_xhl_b(void){pair J; J.W.l=R.XX.W.l+(offset)M_RDMEM_OPCODE();M_WRMEM(J.W.l,R.BC.B.h);}   
static void ld_h_c(void){   R.XX.B.h=R.BC.B.l;}  
static void ld_l_c(void){   R.XX.B.l=R.BC.B.l;}  
static void ld_xhl_c(void){pair J; J.W.l=R.XX.W.l+(offset)M_RDMEM_OPCODE();M_WRMEM(J.W.l,R.BC.B.l);}   
static void ld_h_d(void){   R.XX.B.h=R.DE.B.h;}  
static void ld_l_d(void){   R.XX.B.l=R.DE.B.h;}  
static void ld_xhl_d(void){pair J; J.W.l=R.XX.W.l+(offset)M_RDMEM_OPCODE();M_WRMEM(J.W.l,R.DE.B.h);}   
static void ld_h_e(void){   R.XX.B.h=R.DE.B.l;}  
static void ld_l_e(void){   R.XX.B.l=R.DE.B.l;}  
static void ld_xhl_e(void){pair J; J.W.l=R.XX.W.l+(offset)M_RDMEM_OPCODE();M_WRMEM(J.W.l,R.DE.B.l);}   
      
static void ld_b_h(void){   R.BC.B.h=R.XX.B.h;}  
static void ld_c_h(void){   R.BC.B.l=R.XX.B.h;}  
static void ld_d_h(void){   R.DE.B.h=R.XX.B.h;}  
static void ld_e_h(void){   R.DE.B.l=R.XX.B.h;}  
static void ld_h_h(void){   R.XX.B.h=R.XX.B.h;}  
static void ld_l_h(void){   R.XX.B.l=R.XX.B.h;}  
static void ld_a_h(void){   R.AF.B.h=R.XX.B.h;}  
static void ld_xhl_h(void){pair J; J.W.l=R.XX.W.l+(offset)M_RDMEM_OPCODE();M_WRMEM(J.W.l,R.HL.B.h);}   
      
static void ld_b_l(void){   R.BC.B.h=R.XX.B.l;}  
static void ld_c_l(void){   R.BC.B.l=R.XX.B.l;}  
static void ld_d_l(void){   R.DE.B.h=R.XX.B.l;}  
static void ld_e_l(void){   R.DE.B.l=R.XX.B.l;}  
static void ld_h_l(void){   R.XX.B.h=R.XX.B.l;}  
static void ld_l_l(void){   R.XX.B.l=R.XX.B.l;}  
static void ld_a_l(void){   R.AF.B.h=R.XX.B.l;}  
static void ld_xhl_l(void){pair J; J.W.l=R.XX.W.l+(offset)M_RDMEM_OPCODE();M_WRMEM(J.W.l,R.HL.B.l);}   
static void ld_h_a(void){   R.XX.B.h=R.AF.B.h;}  
static void ld_l_a(void){   R.XX.B.l=R.AF.B.h;}  
static void ld_xhl_a(void){pair J; J.W.l=R.XX.W.l+(offset)M_RDMEM_OPCODE();M_WRMEM(J.W.l,R.AF.B.h);}   
static void ld_b_xhl(void){    R.BC.B.h=M_RDMEM(R.XX.D+(offset)M_RDMEM_OPCODE());}
static void ld_c_xhl(void){    R.BC.B.l=M_RDMEM(R.XX.D+(offset)M_RDMEM_OPCODE());}
static void ld_d_xhl(void){    R.DE.B.h=M_RDMEM(R.XX.D+(offset)M_RDMEM_OPCODE());}
static void ld_e_xhl(void){    R.DE.B.l=M_RDMEM(R.XX.D+(offset)M_RDMEM_OPCODE());}
static void ld_h_xhl(void){    R.HL.B.h=M_RDMEM(R.XX.D+(offset)M_RDMEM_OPCODE());}
static void ld_l_xhl(void){    R.HL.B.l=M_RDMEM(R.XX.D+(offset)M_RDMEM_OPCODE());}
static void ld_a_xhl(void){    R.AF.B.h=M_RDMEM(R.XX.D+(offset)M_RDMEM_OPCODE());}
static void ld_h_byte(void){   R.XX.B.h=M_RDMEM_OPCODE();}  
static void ld_l_byte(void){   R.XX.B.l=M_RDMEM_OPCODE();}  
static void ld_xhl_byte(void){byte I;pair J; J.W.l=R.XX.W.l+(offset)M_RDMEM_OPCODE();I=M_RDMEM_OPCODE();M_WRMEM(J.W.l,I);}

static void ld_xword_hl(void){pair J;
  J.B.l=M_RDMEM_OPCODE();
  J.B.h=M_RDMEM_OPCODE();
  M_WRMEM(J.W.l,R.XX.B.l);J.W.l++;
  M_WRMEM(J.W.l,R.XX.B.h);
  }

static void ld_hl_xword(void){pair J;
  J.B.l=M_RDMEM_OPCODE();
  J.B.h=M_RDMEM_OPCODE();
  R.XX.B.l=M_RDMEM(J.W.l++);
  R.XX.B.h=M_RDMEM(J.W.l);
  }

static void ex_hl_xsp(void){pair J;
  J.B.l=M_RDMEM(R.SP.D);M_WRMEM(R.SP.D,R.XX.B.l);R.SP.W.l++;
  J.B.h=M_RDMEM(R.SP.D);M_WRMEM(R.SP.D,R.XX.B.h);R.SP.W.l--;
  R.XX.W.l=J.W.l;
  }

static void no_op (void)
{
 R.PC.W.l--;
}

