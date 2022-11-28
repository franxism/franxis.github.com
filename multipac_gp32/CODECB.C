/** Z80: portable Z80 emulator **********************************************/
/**                                                                        **/
/**                                CodeCB.c                                **/
/**                                                                        **/
/** This file contains the emulation of CB XX opcodes                      **/
/**                                                                        **/
/** Copyright (C) Marat Fayzullin 1994,1995,1996                           **/
/**               Marcel de Kogel 1996                                     **/
/**     You are not allowed to distribute this software commercially       **/
/**     Please, notify me, if you make any changes to this file            **/
/****************************************************************************/

#include "Z80.h"
#include <stdio.h>

typedef void (*FnPtr) (void);

static void rlc_b(void){ M_RLC(R.BC.B.h);}
static void rlc_c(void){ M_RLC(R.BC.B.l);}
static void rlc_d(void){ M_RLC(R.DE.B.h);}
static void rlc_e(void){ M_RLC(R.DE.B.l);}
static void rlc_h(void){ M_RLC(R.HL.B.h);}
static void rlc_l(void){ M_RLC(R.HL.B.l);}
static void rlc_xhl(void){byte I; I=M_RDMEM(R.HL.D);M_RLC(I);M_WRMEM(R.HL.D,I);}
static void rlc_a(void){ M_RLC(R.AF.B.h);}
        
static void rrc_b(void){ M_RRC(R.BC.B.h);}
static void rrc_c(void){ M_RRC(R.BC.B.l);}
static void rrc_d(void){ M_RRC(R.DE.B.h);}
static void rrc_e(void){ M_RRC(R.DE.B.l);}
static void rrc_h(void){ M_RRC(R.HL.B.h);}
static void rrc_l(void){ M_RRC(R.HL.B.l);}
static void rrc_xhl(void){byte I; I=M_RDMEM(R.HL.D);M_RRC(I);M_WRMEM(R.HL.D,I);}
static void rrc_a(void){ M_RRC(R.AF.B.h);}
        
static void rl_b(void){ M_RL(R.BC.B.h);}
static void rl_c(void){ M_RL(R.BC.B.l);}
static void rl_d(void){ M_RL(R.DE.B.h);}
static void rl_e(void){ M_RL(R.DE.B.l);}
static void rl_h(void){ M_RL(R.HL.B.h);}
static void rl_l(void){ M_RL(R.HL.B.l);}
static void rl_xhl(void){byte I; I=M_RDMEM(R.HL.D);M_RL(I);M_WRMEM(R.HL.D,I);}
static void rl_a(void){ M_RL(R.AF.B.h);}
        
static void rr_b(void){ M_RR(R.BC.B.h);}
static void rr_c(void){ M_RR(R.BC.B.l);}
static void rr_d(void){ M_RR(R.DE.B.h);}
static void rr_e(void){ M_RR(R.DE.B.l);}
static void rr_h(void){ M_RR(R.HL.B.h);}
static void rr_l(void){ M_RR(R.HL.B.l);}
static void rr_xhl(void){byte I; I=M_RDMEM(R.HL.D);M_RR(I);M_WRMEM(R.HL.D,I);}
static void rr_a(void){ M_RR(R.AF.B.h);}
        
static void sla_b(void){ M_SLA(R.BC.B.h);}
static void sla_c(void){ M_SLA(R.BC.B.l);}
static void sla_d(void){ M_SLA(R.DE.B.h);}
static void sla_e(void){ M_SLA(R.DE.B.l);}
static void sla_h(void){ M_SLA(R.HL.B.h);}
static void sla_l(void){ M_SLA(R.HL.B.l);}
static void sla_xhl(void){byte I; I=M_RDMEM(R.HL.D);M_SLA(I);M_WRMEM(R.HL.D,I);}
static void sla_a(void){ M_SLA(R.AF.B.h);}
        
static void sra_b(void){ M_SRA(R.BC.B.h);}
static void sra_c(void){ M_SRA(R.BC.B.l);}
static void sra_d(void){ M_SRA(R.DE.B.h);}
static void sra_e(void){ M_SRA(R.DE.B.l);}
static void sra_h(void){ M_SRA(R.HL.B.h);}
static void sra_l(void){ M_SRA(R.HL.B.l);}
static void sra_xhl(void){byte I; I=M_RDMEM(R.HL.D);M_SRA(I);M_WRMEM(R.HL.D,I);}
static void sra_a(void){ M_SRA(R.AF.B.h);}
        
static void sll_b(void){ M_SLL(R.BC.B.h);}
static void sll_c(void){ M_SLL(R.BC.B.l);}
static void sll_d(void){ M_SLL(R.DE.B.h);}
static void sll_e(void){ M_SLL(R.DE.B.l);}
static void sll_h(void){ M_SLL(R.HL.B.h);}
static void sll_l(void){ M_SLL(R.HL.B.l);}
static void sll_xhl(void){byte I; I=M_RDMEM(R.HL.D);M_SLL(I);M_WRMEM(R.HL.D,I);}
static void sll_a(void){ M_SLL(R.AF.B.h);}
        
static void srl_b(void){ M_SRL(R.BC.B.h);}
static void srl_c(void){ M_SRL(R.BC.B.l);}
static void srl_d(void){ M_SRL(R.DE.B.h);}
static void srl_e(void){ M_SRL(R.DE.B.l);}
static void srl_h(void){ M_SRL(R.HL.B.h);}
static void srl_l(void){ M_SRL(R.HL.B.l);}
static void srl_xhl(void){byte I; I=M_RDMEM(R.HL.D);M_SRL(I);M_WRMEM(R.HL.D,I);}
static void srl_a(void){ M_SRL(R.AF.B.h);}
        
static void bit0_b(void){ M_BIT(0,R.BC.B.h);}
static void bit0_c(void){ M_BIT(0,R.BC.B.l);}
static void bit0_d(void){ M_BIT(0,R.DE.B.h);}
static void bit0_e(void){ M_BIT(0,R.DE.B.l);}
static void bit0_h(void){ M_BIT(0,R.HL.B.h);}
static void bit0_l(void){ M_BIT(0,R.HL.B.l);}
static void bit0_xhl(void){byte I; I=M_RDMEM(R.HL.D);M_BIT(0,I);}
static void bit0_a(void){ M_BIT(0,R.AF.B.h);}
        
static void bit1_b(void){ M_BIT(1,R.BC.B.h);}
static void bit1_c(void){ M_BIT(1,R.BC.B.l);}
static void bit1_d(void){ M_BIT(1,R.DE.B.h);}
static void bit1_e(void){ M_BIT(1,R.DE.B.l);}
static void bit1_h(void){ M_BIT(1,R.HL.B.h);}
static void bit1_l(void){ M_BIT(1,R.HL.B.l);}
static void bit1_xhl(void){byte I; I=M_RDMEM(R.HL.D);M_BIT(1,I);}
static void bit1_a(void){ M_BIT(1,R.AF.B.h);}
        
static void bit2_b(void){ M_BIT(2,R.BC.B.h);}
static void bit2_c(void){ M_BIT(2,R.BC.B.l);}
static void bit2_d(void){ M_BIT(2,R.DE.B.h);}
static void bit2_e(void){ M_BIT(2,R.DE.B.l);}
static void bit2_h(void){ M_BIT(2,R.HL.B.h);}
static void bit2_l(void){ M_BIT(2,R.HL.B.l);}
static void bit2_xhl(void){byte I; I=M_RDMEM(R.HL.D);M_BIT(2,I);}
static void bit2_a(void){ M_BIT(2,R.AF.B.h);}
        
static void bit3_b(void){ M_BIT(3,R.BC.B.h);}
static void bit3_c(void){ M_BIT(3,R.BC.B.l);}
static void bit3_d(void){ M_BIT(3,R.DE.B.h);}
static void bit3_e(void){ M_BIT(3,R.DE.B.l);}
static void bit3_h(void){ M_BIT(3,R.HL.B.h);}
static void bit3_l(void){ M_BIT(3,R.HL.B.l);}
static void bit3_xhl(void){byte I; I=M_RDMEM(R.HL.D);M_BIT(3,I);}
static void bit3_a(void){ M_BIT(3,R.AF.B.h);}
        
static void bit4_b(void){ M_BIT(4,R.BC.B.h);}
static void bit4_c(void){ M_BIT(4,R.BC.B.l);}
static void bit4_d(void){ M_BIT(4,R.DE.B.h);}
static void bit4_e(void){ M_BIT(4,R.DE.B.l);}
static void bit4_h(void){ M_BIT(4,R.HL.B.h);}
static void bit4_l(void){ M_BIT(4,R.HL.B.l);}
static void bit4_xhl(void){byte I; I=M_RDMEM(R.HL.D);M_BIT(4,I);}
static void bit4_a(void){ M_BIT(4,R.AF.B.h);}
        
static void bit5_b(void){ M_BIT(5,R.BC.B.h);}
static void bit5_c(void){ M_BIT(5,R.BC.B.l);}
static void bit5_d(void){ M_BIT(5,R.DE.B.h);}
static void bit5_e(void){ M_BIT(5,R.DE.B.l);}
static void bit5_h(void){ M_BIT(5,R.HL.B.h);}
static void bit5_l(void){ M_BIT(5,R.HL.B.l);}
static void bit5_xhl(void){byte I; I=M_RDMEM(R.HL.D);M_BIT(5,I);}
static void bit5_a(void){ M_BIT(5,R.AF.B.h);}
        
static void bit6_b(void){ M_BIT(6,R.BC.B.h);}
static void bit6_c(void){ M_BIT(6,R.BC.B.l);}
static void bit6_d(void){ M_BIT(6,R.DE.B.h);}
static void bit6_e(void){ M_BIT(6,R.DE.B.l);}
static void bit6_h(void){ M_BIT(6,R.HL.B.h);}
static void bit6_l(void){ M_BIT(6,R.HL.B.l);}
static void bit6_xhl(void){byte I; I=M_RDMEM(R.HL.D);M_BIT(6,I);}
static void bit6_a(void){ M_BIT(6,R.AF.B.h);}
        
static void bit7_b(void){ M_BIT(7,R.BC.B.h);}
static void bit7_c(void){ M_BIT(7,R.BC.B.l);}
static void bit7_d(void){ M_BIT(7,R.DE.B.h);}
static void bit7_e(void){ M_BIT(7,R.DE.B.l);}
static void bit7_h(void){ M_BIT(7,R.HL.B.h);}
static void bit7_l(void){ M_BIT(7,R.HL.B.l);}
static void bit7_xhl(void){byte I; I=M_RDMEM(R.HL.D);M_BIT(7,I);}
static void bit7_a(void){ M_BIT(7,R.AF.B.h);}
        
static void res0_b(void){ M_RES(0,R.BC.B.h);}
static void res0_c(void){ M_RES(0,R.BC.B.l);}
static void res0_d(void){ M_RES(0,R.DE.B.h);}
static void res0_e(void){ M_RES(0,R.DE.B.l);}
static void res0_h(void){ M_RES(0,R.HL.B.h);}
static void res0_l(void){ M_RES(0,R.HL.B.l);}
static void res0_xhl(void){byte I; I=M_RDMEM(R.HL.D);M_RES(0,I);M_WRMEM(R.HL.D,I);}
static void res0_a(void){ M_RES(0,R.AF.B.h);}
        
static void res1_b(void){ M_RES(1,R.BC.B.h);}
static void res1_c(void){ M_RES(1,R.BC.B.l);}
static void res1_d(void){ M_RES(1,R.DE.B.h);}
static void res1_e(void){ M_RES(1,R.DE.B.l);}
static void res1_h(void){ M_RES(1,R.HL.B.h);}
static void res1_l(void){ M_RES(1,R.HL.B.l);}
static void res1_xhl(void){byte I; I=M_RDMEM(R.HL.D);M_RES(1,I);M_WRMEM(R.HL.D,I);}
static void res1_a(void){ M_RES(1,R.AF.B.h);}
        
static void res2_b(void){ M_RES(2,R.BC.B.h);}
static void res2_c(void){ M_RES(2,R.BC.B.l);}
static void res2_d(void){ M_RES(2,R.DE.B.h);}
static void res2_e(void){ M_RES(2,R.DE.B.l);}
static void res2_h(void){ M_RES(2,R.HL.B.h);}
static void res2_l(void){ M_RES(2,R.HL.B.l);}
static void res2_xhl(void){byte I; I=M_RDMEM(R.HL.D);M_RES(2,I);M_WRMEM(R.HL.D,I);}
static void res2_a(void){ M_RES(2,R.AF.B.h);}
        
static void res3_b(void){ M_RES(3,R.BC.B.h);}
static void res3_c(void){ M_RES(3,R.BC.B.l);}
static void res3_d(void){ M_RES(3,R.DE.B.h);}
static void res3_e(void){ M_RES(3,R.DE.B.l);}
static void res3_h(void){ M_RES(3,R.HL.B.h);}
static void res3_l(void){ M_RES(3,R.HL.B.l);}
static void res3_xhl(void){byte I; I=M_RDMEM(R.HL.D);M_RES(3,I);M_WRMEM(R.HL.D,I);}
static void res3_a(void){ M_RES(3,R.AF.B.h);}
        
static void res4_b(void){ M_RES(4,R.BC.B.h);}
static void res4_c(void){ M_RES(4,R.BC.B.l);}
static void res4_d(void){ M_RES(4,R.DE.B.h);}
static void res4_e(void){ M_RES(4,R.DE.B.l);}
static void res4_h(void){ M_RES(4,R.HL.B.h);}
static void res4_l(void){ M_RES(4,R.HL.B.l);}
static void res4_xhl(void){byte I; I=M_RDMEM(R.HL.D);M_RES(4,I);M_WRMEM(R.HL.D,I);}
static void res4_a(void){ M_RES(4,R.AF.B.h);}
        
static void res5_b(void){ M_RES(5,R.BC.B.h);}
static void res5_c(void){ M_RES(5,R.BC.B.l);}
static void res5_d(void){ M_RES(5,R.DE.B.h);}
static void res5_e(void){ M_RES(5,R.DE.B.l);}
static void res5_h(void){ M_RES(5,R.HL.B.h);}
static void res5_l(void){ M_RES(5,R.HL.B.l);}
static void res5_xhl(void){byte I; I=M_RDMEM(R.HL.D);M_RES(5,I);M_WRMEM(R.HL.D,I);}
static void res5_a(void){ M_RES(5,R.AF.B.h);}
        
static void res6_b(void){ M_RES(6,R.BC.B.h);}
static void res6_c(void){ M_RES(6,R.BC.B.l);}
static void res6_d(void){ M_RES(6,R.DE.B.h);}
static void res6_e(void){ M_RES(6,R.DE.B.l);}
static void res6_h(void){ M_RES(6,R.HL.B.h);}
static void res6_l(void){ M_RES(6,R.HL.B.l);}
static void res6_xhl(void){byte I; I=M_RDMEM(R.HL.D);M_RES(6,I);M_WRMEM(R.HL.D,I);}
static void res6_a(void){ M_RES(6,R.AF.B.h);}
        
static void res7_b(void){ M_RES(7,R.BC.B.h);}
static void res7_c(void){ M_RES(7,R.BC.B.l);}
static void res7_d(void){ M_RES(7,R.DE.B.h);}
static void res7_e(void){ M_RES(7,R.DE.B.l);}
static void res7_h(void){ M_RES(7,R.HL.B.h);}
static void res7_l(void){ M_RES(7,R.HL.B.l);}
static void res7_xhl(void){byte I; I=M_RDMEM(R.HL.D);M_RES(7,I);M_WRMEM(R.HL.D,I);}
static void res7_a(void){ M_RES(7,R.AF.B.h);}
        
static void set0_b(void){ M_SET(0,R.BC.B.h);}
static void set0_c(void){ M_SET(0,R.BC.B.l);}
static void set0_d(void){ M_SET(0,R.DE.B.h);}
static void set0_e(void){ M_SET(0,R.DE.B.l);}
static void set0_h(void){ M_SET(0,R.HL.B.h);}
static void set0_l(void){ M_SET(0,R.HL.B.l);}
static void set0_xhl(void){byte I; I=M_RDMEM(R.HL.D);M_SET(0,I);M_WRMEM(R.HL.D,I);}
static void set0_a(void){ M_SET(0,R.AF.B.h);}
        
static void set1_b(void){ M_SET(1,R.BC.B.h);}
static void set1_c(void){ M_SET(1,R.BC.B.l);}
static void set1_d(void){ M_SET(1,R.DE.B.h);}
static void set1_e(void){ M_SET(1,R.DE.B.l);}
static void set1_h(void){ M_SET(1,R.HL.B.h);}
static void set1_l(void){ M_SET(1,R.HL.B.l);}
static void set1_xhl(void){byte I; I=M_RDMEM(R.HL.D);M_SET(1,I);M_WRMEM(R.HL.D,I);}
static void set1_a(void){ M_SET(1,R.AF.B.h);}
        
static void set2_b(void){ M_SET(2,R.BC.B.h);}
static void set2_c(void){ M_SET(2,R.BC.B.l);}
static void set2_d(void){ M_SET(2,R.DE.B.h);}
static void set2_e(void){ M_SET(2,R.DE.B.l);}
static void set2_h(void){ M_SET(2,R.HL.B.h);}
static void set2_l(void){ M_SET(2,R.HL.B.l);}
static void set2_xhl(void){byte I; I=M_RDMEM(R.HL.D);M_SET(2,I);M_WRMEM(R.HL.D,I);}
static void set2_a(void){ M_SET(2,R.AF.B.h);}
        
static void set3_b(void){ M_SET(3,R.BC.B.h);}
static void set3_c(void){ M_SET(3,R.BC.B.l);}
static void set3_d(void){ M_SET(3,R.DE.B.h);}
static void set3_e(void){ M_SET(3,R.DE.B.l);}
static void set3_h(void){ M_SET(3,R.HL.B.h);}
static void set3_l(void){ M_SET(3,R.HL.B.l);}
static void set3_xhl(void){byte I; I=M_RDMEM(R.HL.D);M_SET(3,I);M_WRMEM(R.HL.D,I);}
static void set3_a(void){ M_SET(3,R.AF.B.h);}
        
static void set4_b(void){ M_SET(4,R.BC.B.h);}
static void set4_c(void){ M_SET(4,R.BC.B.l);}
static void set4_d(void){ M_SET(4,R.DE.B.h);}
static void set4_e(void){ M_SET(4,R.DE.B.l);}
static void set4_h(void){ M_SET(4,R.HL.B.h);}
static void set4_l(void){ M_SET(4,R.HL.B.l);}
static void set4_xhl(void){byte I; I=M_RDMEM(R.HL.D);M_SET(4,I);M_WRMEM(R.HL.D,I);}
static void set4_a(void){ M_SET(4,R.AF.B.h);}
        
static void set5_b(void){ M_SET(5,R.BC.B.h);}
static void set5_c(void){ M_SET(5,R.BC.B.l);}
static void set5_d(void){ M_SET(5,R.DE.B.h);}
static void set5_e(void){ M_SET(5,R.DE.B.l);}
static void set5_h(void){ M_SET(5,R.HL.B.h);}
static void set5_l(void){ M_SET(5,R.HL.B.l);}
static void set5_xhl(void){byte I; I=M_RDMEM(R.HL.D);M_SET(5,I);M_WRMEM(R.HL.D,I);}
static void set5_a(void){ M_SET(5,R.AF.B.h);}
        
static void set6_b(void){ M_SET(6,R.BC.B.h);}
static void set6_c(void){ M_SET(6,R.BC.B.l);}
static void set6_d(void){ M_SET(6,R.DE.B.h);}
static void set6_e(void){ M_SET(6,R.DE.B.l);}
static void set6_h(void){ M_SET(6,R.HL.B.h);}
static void set6_l(void){ M_SET(6,R.HL.B.l);}
static void set6_xhl(void){byte I; I=M_RDMEM(R.HL.D);M_SET(6,I);M_WRMEM(R.HL.D,I);}
static void set6_a(void){ M_SET(6,R.AF.B.h);}

static void set7_b(void){ M_SET(7,R.BC.B.h);}     
static void set7_c(void){ M_SET(7,R.BC.B.l);}
static void set7_d(void){ M_SET(7,R.DE.B.h);}     
static void set7_e(void){ M_SET(7,R.DE.B.l);}
static void set7_h(void){ M_SET(7,R.HL.B.h);}     
static void set7_l(void){ M_SET(7,R.HL.B.l);}
static void set7_xhl(void){byte I; I=M_RDMEM(R.HL.D);M_SET(7,I);M_WRMEM(R.HL.D,I);}
static void set7_a(void){ M_SET(7,R.AF.B.h);}

FnPtr FnCB[256]=
{
  rlc_b,rlc_c,rlc_d,rlc_e,rlc_h,rlc_l,rlc_xhl,rlc_a,
  rrc_b,rrc_c,rrc_d,rrc_e,rrc_h,rrc_l,rrc_xhl,rrc_a,
  rl_b,rl_c,rl_d,rl_e,rl_h,rl_l,rl_xhl,rl_a,
  rr_b,rr_c,rr_d,rr_e,rr_h,rr_l,rr_xhl,rr_a,
  sla_b,sla_c,sla_d,sla_e,sla_h,sla_l,sla_xhl,sla_a,
  sra_b,sra_c,sra_d,sra_e,sra_h,sra_l,sra_xhl,sra_a,
  sll_b,sll_c,sll_d,sll_e,sll_h,sll_l,sll_xhl,sll_a,
  srl_b,srl_c,srl_d,srl_e,srl_h,srl_l,srl_xhl,srl_a,
  bit0_b,bit0_c,bit0_d,bit0_e,bit0_h,bit0_l,bit0_xhl,bit0_a,
  bit1_b,bit1_c,bit1_d,bit1_e,bit1_h,bit1_l,bit1_xhl,bit1_a,
  bit2_b,bit2_c,bit2_d,bit2_e,bit2_h,bit2_l,bit2_xhl,bit2_a,
  bit3_b,bit3_c,bit3_d,bit3_e,bit3_h,bit3_l,bit3_xhl,bit3_a,
  bit4_b,bit4_c,bit4_d,bit4_e,bit4_h,bit4_l,bit4_xhl,bit4_a,
  bit5_b,bit5_c,bit5_d,bit5_e,bit5_h,bit5_l,bit5_xhl,bit5_a,
  bit6_b,bit6_c,bit6_d,bit6_e,bit6_h,bit6_l,bit6_xhl,bit6_a,
  bit7_b,bit7_c,bit7_d,bit7_e,bit7_h,bit7_l,bit7_xhl,bit7_a,
  res0_b,res0_c,res0_d,res0_e,res0_h,res0_l,res0_xhl,res0_a,
  res1_b,res1_c,res1_d,res1_e,res1_h,res1_l,res1_xhl,res1_a,
  res2_b,res2_c,res2_d,res2_e,res2_h,res2_l,res2_xhl,res2_a,
  res3_b,res3_c,res3_d,res3_e,res3_h,res3_l,res3_xhl,res3_a,
  res4_b,res4_c,res4_d,res4_e,res4_h,res4_l,res4_xhl,res4_a,
  res5_b,res5_c,res5_d,res5_e,res5_h,res5_l,res5_xhl,res5_a,
  res6_b,res6_c,res6_d,res6_e,res6_h,res6_l,res6_xhl,res6_a,
  res7_b,res7_c,res7_d,res7_e,res7_h,res7_l,res7_xhl,res7_a,  
  set0_b,set0_c,set0_d,set0_e,set0_h,set0_l,set0_xhl,set0_a,
  set1_b,set1_c,set1_d,set1_e,set1_h,set1_l,set1_xhl,set1_a,
  set2_b,set2_c,set2_d,set2_e,set2_h,set2_l,set2_xhl,set2_a,
  set3_b,set3_c,set3_d,set3_e,set3_h,set3_l,set3_xhl,set3_a,
  set4_b,set4_c,set4_d,set4_e,set4_h,set4_l,set4_xhl,set4_a,
  set5_b,set5_c,set5_d,set5_e,set5_h,set5_l,set5_xhl,set5_a,
  set6_b,set6_c,set6_d,set6_e,set6_h,set6_l,set6_xhl,set6_a,
  set7_b,set7_c,set7_d,set7_e,set7_h,set7_l,set7_xhl,set7_a
};

unsigned CyclesCB[256]=
{
  8,8,8,8,8,8,15,8,
  8,8,8,8,8,8,15,8,
  8,8,8,8,8,8,15,8,
  8,8,8,8,8,8,15,8,
  8,8,8,8,8,8,15,8,
  8,8,8,8,8,8,15,8,
  8,8,8,8,8,8,15,8,
  8,8,8,8,8,8,15,8,
  8,8,8,8,8,8,12,8,
  8,8,8,8,8,8,12,8,
  8,8,8,8,8,8,12,8,
  8,8,8,8,8,8,12,8,
  8,8,8,8,8,8,12,8,
  8,8,8,8,8,8,12,8,
  8,8,8,8,8,8,12,8,
  8,8,8,8,8,8,12,8,
  8,8,8,8,8,8,15,8,
  8,8,8,8,8,8,15,8,
  8,8,8,8,8,8,15,8,
  8,8,8,8,8,8,15,8,
  8,8,8,8,8,8,15,8,
  8,8,8,8,8,8,15,8,
  8,8,8,8,8,8,15,8,
  8,8,8,8,8,8,15,8,
  8,8,8,8,8,8,15,8,
  8,8,8,8,8,8,15,8,
  8,8,8,8,8,8,15,8,
  8,8,8,8,8,8,15,8,
  8,8,8,8,8,8,15,8,
  8,8,8,8,8,8,15,8,
  8,8,8,8,8,8,15,8,
  8,8,8,8,8,8,15,8,
};

