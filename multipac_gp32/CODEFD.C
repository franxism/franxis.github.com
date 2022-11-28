/** Z80: portable Z80 emulator **********************************************/
/**                                                                        **/
/**                                CodeFD.c                                **/
/**                                                                        **/
/** This file contains the emulation of FD XX opcodes                      **/
/**                                                                        **/
/** Copyright (C) Marat Fayzullin 1994,1995,1996                           **/
/**               Marcel de Kogel 1996                                     **/
/**     You are not allowed to distribute this software commercially       **/
/**     Please, notify me, if you make any changes to this file            **/
/****************************************************************************/

#include "Z80.h"
#include <stdio.h>

#define XX IY

typedef void (*FDCBFnPtr) (pair J);
typedef void (*FnPtr) (void);
extern void Z80_BadOpcode (void);
extern void Z80_Halt (void);

static void fuck2 (void)
{
 Z80_BadOpcode ();
}

static void rlc_xhl(pair J);
static void rrc_xhl(pair J);
static void rl_xhl(pair J);
static void rr_xhl(pair J);
static void sla_xhl(pair J);
static void sra_xhl(pair J);
static void sll_xhl(pair J);
static void srl_xhl(pair J);
static void bit0_xhl(pair J);
static void bit1_xhl(pair J);
static void bit2_xhl(pair J);
static void bit3_xhl(pair J);
static void bit4_xhl(pair J);
static void bit5_xhl(pair J);
static void bit6_xhl(pair J);
static void bit7_xhl(pair J);
static void res0_xhl(pair J);
static void res1_xhl(pair J);
static void res2_xhl(pair J);
static void res3_xhl(pair J);
static void res4_xhl(pair J);
static void res5_xhl(pair J);
static void res6_xhl(pair J);
static void res7_xhl(pair J);
static void set0_xhl(pair J);
static void set1_xhl(pair J);
static void set2_xhl(pair J);
static void set3_xhl(pair J);
static void set4_xhl(pair J);
static void set5_xhl(pair J);
static void set6_xhl(pair J);
static void set7_xhl(pair J);
static void fuck (pair J);

#include "CodeXX.c"

static FDCBFnPtr FnsFDCB[]=
{
  fuck,fuck,fuck,fuck,fuck,fuck,rlc_xhl,fuck,
  fuck,fuck,fuck,fuck,fuck,fuck,rrc_xhl,fuck,
  fuck,fuck,fuck,fuck,fuck,fuck,rl_xhl,fuck,
  fuck,fuck,fuck,fuck,fuck,fuck,rr_xhl,fuck,
  fuck,fuck,fuck,fuck,fuck,fuck,sla_xhl,fuck,
  fuck,fuck,fuck,fuck,fuck,fuck,sra_xhl,fuck,
  fuck,fuck,fuck,fuck,fuck,fuck,sll_xhl,fuck,
  fuck,fuck,fuck,fuck,fuck,fuck,srl_xhl,fuck,
  bit0_xhl,bit0_xhl,bit0_xhl,bit0_xhl,bit0_xhl,bit0_xhl,bit0_xhl,bit0_xhl,
  bit1_xhl,bit1_xhl,bit1_xhl,bit1_xhl,bit1_xhl,bit1_xhl,bit1_xhl,bit1_xhl,
  bit2_xhl,bit2_xhl,bit2_xhl,bit2_xhl,bit2_xhl,bit2_xhl,bit2_xhl,bit2_xhl,
  bit3_xhl,bit3_xhl,bit3_xhl,bit3_xhl,bit3_xhl,bit3_xhl,bit3_xhl,bit3_xhl,
  bit4_xhl,bit4_xhl,bit4_xhl,bit4_xhl,bit4_xhl,bit4_xhl,bit4_xhl,bit4_xhl,
  bit5_xhl,bit5_xhl,bit5_xhl,bit5_xhl,bit5_xhl,bit5_xhl,bit5_xhl,bit5_xhl,
  bit6_xhl,bit6_xhl,bit6_xhl,bit6_xhl,bit6_xhl,bit6_xhl,bit6_xhl,bit6_xhl,
  bit7_xhl,bit7_xhl,bit7_xhl,bit7_xhl,bit7_xhl,bit7_xhl,bit7_xhl,bit7_xhl,
  fuck,fuck,fuck,fuck,fuck,fuck,res0_xhl,fuck,
  fuck,fuck,fuck,fuck,fuck,fuck,res1_xhl,fuck,
  fuck,fuck,fuck,fuck,fuck,fuck,res2_xhl,fuck,
  fuck,fuck,fuck,fuck,fuck,fuck,res3_xhl,fuck,
  fuck,fuck,fuck,fuck,fuck,fuck,res4_xhl,fuck,
  fuck,fuck,fuck,fuck,fuck,fuck,res5_xhl,fuck,
  fuck,fuck,fuck,fuck,fuck,fuck,res6_xhl,fuck,
  fuck,fuck,fuck,fuck,fuck,fuck,res7_xhl,fuck,
  fuck,fuck,fuck,fuck,fuck,fuck,set0_xhl,fuck,
  fuck,fuck,fuck,fuck,fuck,fuck,set1_xhl,fuck,
  fuck,fuck,fuck,fuck,fuck,fuck,set2_xhl,fuck,
  fuck,fuck,fuck,fuck,fuck,fuck,set3_xhl,fuck,
  fuck,fuck,fuck,fuck,fuck,fuck,set4_xhl,fuck,
  fuck,fuck,fuck,fuck,fuck,fuck,set5_xhl,fuck,
  fuck,fuck,fuck,fuck,fuck,fuck,set6_xhl,fuck,
  fuck,fuck,fuck,fuck,fuck,fuck,set7_xhl,fuck
};

static unsigned CyclesFDCB[]=
{
  0,0,0,0,0,0,23,0,
  0,0,0,0,0,0,23,0,
  0,0,0,0,0,0,23,0,
  0,0,0,0,0,0,23,0,
  0,0,0,0,0,0,23,0,
  0,0,0,0,0,0,23,0,
  0,0,0,0,0,0,23,0,
  0,0,0,0,0,0,23,0,
  20,20,20,20,20,20,20,20,
  20,20,20,20,20,20,20,20,
  20,20,20,20,20,20,20,20,
  20,20,20,20,20,20,20,20,
  20,20,20,20,20,20,20,20,
  20,20,20,20,20,20,20,20,
  20,20,20,20,20,20,20,20,
  20,20,20,20,20,20,20,20,
  0,0,0,0,0,0,23,0,
  0,0,0,0,0,0,23,0,
  0,0,0,0,0,0,23,0,
  0,0,0,0,0,0,23,0,
  0,0,0,0,0,0,23,0,
  0,0,0,0,0,0,23,0,
  0,0,0,0,0,0,23,0,
  0,0,0,0,0,0,23,0,
  0,0,0,0,0,0,23,0,
  0,0,0,0,0,0,23,0,
  0,0,0,0,0,0,23,0,
  0,0,0,0,0,0,23,0,
  0,0,0,0,0,0,23,0,
  0,0,0,0,0,0,23,0,
  0,0,0,0,0,0,23,0,
  0,0,0,0,0,0,23,0
};

static void pfx_cb (void)
{
 pair J;
 unsigned opcode;
 J.W.l=R.XX.W.l+(offset)M_RDMEM_OPCODE();
 opcode=M_RDMEM_OPCODE();
 ICount-=CyclesFDCB[opcode];
 (*(FnsFDCB[opcode]))(J);
}

FnPtr FnFD[256]=
{
  no_op,no_op,no_op,no_op,no_op,no_op,no_op,no_op,
  no_op,add_hl_bc,no_op,no_op,no_op,no_op,no_op,no_op,
  no_op,no_op,no_op,no_op,no_op,no_op,no_op,no_op,
  no_op,add_hl_de,no_op,no_op,no_op,no_op,no_op,no_op,
  no_op,ld_hl_word,ld_xword_hl,inc_hl,inc_h,dec_h,ld_h_byte,no_op,
  no_op,add_hl_hl,ld_hl_xword,dec_hl,inc_l,dec_l,ld_l_byte,no_op,
  no_op,no_op,no_op,no_op,inc_xhl,dec_xhl,ld_xhl_byte,no_op,
  no_op,add_hl_sp,no_op,no_op,no_op,no_op,no_op,no_op,
  no_op,no_op,no_op,no_op,ld_b_h,ld_b_l,ld_b_xhl,no_op,
  no_op,no_op,no_op,no_op,ld_c_h,ld_c_l,ld_c_xhl,no_op,
  no_op,no_op,no_op,no_op,ld_d_h,ld_d_l,ld_d_xhl,no_op,
  no_op,no_op,no_op,no_op,ld_e_h,ld_e_l,ld_e_xhl,no_op,
  ld_h_b,ld_h_c,ld_h_d,ld_h_e,ld_h_h,ld_h_l,ld_h_xhl,ld_h_a,
  ld_l_b,ld_l_c,ld_l_d,ld_l_e,ld_l_h,ld_l_l,ld_l_xhl,ld_l_a,
  ld_xhl_b,ld_xhl_c,ld_xhl_d,ld_xhl_e,ld_xhl_h,ld_xhl_l,no_op,ld_xhl_a,
  no_op,no_op,no_op,no_op,ld_a_h,ld_a_l,ld_a_xhl,no_op,
  no_op,no_op,no_op,no_op,add_h,add_l,add_xhl,no_op,
  no_op,no_op,no_op,no_op,adc_h,adc_l,adc_xhl,no_op,
  no_op,no_op,no_op,no_op,sub_h,sub_l,sub_xhl,no_op,
  no_op,no_op,no_op,no_op,sbc_h,sbc_l,sbc_xhl,no_op,
  no_op,no_op,no_op,no_op,and_h,and_l,and_xhl,no_op,
  no_op,no_op,no_op,no_op,xor_h,xor_l,xor_xhl,no_op,
  no_op,no_op,no_op,no_op,or_h,or_l,or_xhl,no_op,
  no_op,no_op,no_op,no_op,cp_h,cp_l,cp_xhl,no_op,
  no_op,no_op,no_op,no_op,no_op,no_op,no_op,no_op,
  no_op,no_op,no_op,pfx_cb,no_op,no_op,no_op,no_op,
  no_op,no_op,no_op,no_op,no_op,no_op,no_op,no_op,
  no_op,no_op,no_op,no_op,no_op,no_op,no_op,no_op,
  no_op,pop_hl,no_op,ex_hl_xsp,no_op,push_hl,no_op,no_op,
  no_op,ld_pc_hl,no_op,no_op,no_op,fuck2,no_op,no_op,
  no_op,no_op,no_op,no_op,no_op,no_op,no_op,no_op,
  no_op,ld_sp_hl,no_op,no_op,no_op,no_op,no_op,no_op
};

/* undocumented opcodes */
#define _inc_h          9       /* INC IYh */
#define _ld_h_byte      9       /* LD  IYh,30 */
#define _ld_r_h         9       /* LD  A,IYh */
#define _add_h          9       /* ADD IYh */
unsigned CyclesFD[256]=
{
  0,0,0,0,0,0,0,0,
  0,15,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,15,0,0,0,0,0,0,
  0,14,20,10,_inc_h,_inc_h,_ld_h_byte,0,
  0,15,20,10,_inc_h,_inc_h,_ld_h_byte,0,
  0,0,0,0,23,23,19,0,
  0,15,0,0,0,0,0,0,
  0,0,0,0,_ld_r_h,_ld_r_h,19,0,
  0,0,0,0,_ld_r_h,_ld_r_h,19,0,
  0,0,0,0,_ld_r_h,_ld_r_h,19,0,
  0,0,0,0,_ld_r_h,_ld_r_h,19,0,
  _ld_r_h,_ld_r_h,_ld_r_h,_ld_r_h,_ld_r_h,_ld_r_h,_ld_r_h,_ld_r_h,
  _ld_r_h,_ld_r_h,_ld_r_h,_ld_r_h,_ld_r_h,_ld_r_h,_ld_r_h,_ld_r_h,
  19,19,19,19,19,19,19,19,
  0,0,0,0,_ld_r_h,_ld_r_h,19,0,
  0,0,0,0,_add_h,_add_h,19,0,
  0,0,0,0,_add_h,_add_h,19,0,
  0,0,0,0,_add_h,_add_h,19,0,
  0,0,0,0,_add_h,_add_h,19,0,
  0,0,0,0,_add_h,_add_h,19,0,
  0,0,0,0,_add_h,_add_h,19,0,
  0,0,0,0,_add_h,_add_h,19,0,
  0,0,0,0,_add_h,_add_h,19,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,14,0,23,0,15,0,0,
  0,8,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,10,0,0,0,0,0,0
};
