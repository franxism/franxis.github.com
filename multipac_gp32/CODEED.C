/** Z80: portable Z80 emulator **********************************************/
/**                                                                        **/
/**                                CodeED.c                                **/
/**                                                                        **/
/** This file contains the emulation of ED XX opcodes                      **/
/**                                                                        **/
/** Copyright (C) Marat Fayzullin 1994,1995,1996                           **/
/**               Marcel de Kogel 1996                                     **/
/**     You are not allowed to distribute this software commercially       **/
/**     Please, notify me, if you make any changes to this file            **/
/****************************************************************************/

#include "Z80.h"
#include <stdio.h>

typedef void (*FnPtr)(void);

extern void Z80_BadOpcode (void);

static void adc_hl_bc(void){ M_ADCW(BC);}
static void adc_hl_de(void){ M_ADCW(DE);}
static void adc_hl_hl(void){ M_ADCW(HL);}
static void adc_hl_sp(void){ M_ADCW(SP);}

static void sbc_hl_bc(void){ M_SBCW(BC);}
static void sbc_hl_de(void){ M_SBCW(DE);}
static void sbc_hl_hl(void){ M_SBCW(HL);}
static void sbc_hl_sp(void){ M_SBCW(SP);}

static void fuck (void)
{
 Z80_BadOpcode ();
}

static void ld_xworde_hl(void){pair J;
  J.B.l=M_RDMEM_OPCODE();
  J.B.h=M_RDMEM_OPCODE();
  M_WRMEM(J.W.l,R.HL.B.l);J.W.l++;
  M_WRMEM(J.W.l,R.HL.B.h);
  }
static void ld_xworde_de(void){pair J;
  J.B.l=M_RDMEM_OPCODE();
  J.B.h=M_RDMEM_OPCODE();
  M_WRMEM(J.W.l,R.DE.B.l);J.W.l++;
  M_WRMEM(J.W.l,R.DE.B.h);
  }
static void ld_xworde_bc(void){pair J;
  J.B.l=M_RDMEM_OPCODE();
  J.B.h=M_RDMEM_OPCODE();
  M_WRMEM(J.W.l,R.BC.B.l);J.W.l++;
  M_WRMEM(J.W.l,R.BC.B.h);
  }
static void ld_xworde_sp(void){pair J;
  J.B.l=M_RDMEM_OPCODE();
  J.B.h=M_RDMEM_OPCODE();
  M_WRMEM(J.W.l,R.SP.B.l);J.W.l++;
  M_WRMEM(J.W.l,R.SP.B.h);
  }

static void ld_hl_xworde(void){pair J;
  J.B.l=M_RDMEM_OPCODE();
  J.B.h=M_RDMEM_OPCODE();
  R.HL.B.l=M_RDMEM((J.W.l++));
  R.HL.B.h=M_RDMEM(J.W.l);
  }
static void ld_de_xworde(void){pair J;
  J.B.l=M_RDMEM_OPCODE();
  J.B.h=M_RDMEM_OPCODE();
  R.DE.B.l=M_RDMEM((J.W.l++));
  R.DE.B.h=M_RDMEM(J.W.l);
  }
static void ld_bc_xworde(void){pair J;
  J.B.l=M_RDMEM_OPCODE();
  J.B.h=M_RDMEM_OPCODE();
  R.BC.B.l=M_RDMEM((J.W.l++));
  R.BC.B.h=M_RDMEM(J.W.l);
  }
static void ld_sp_xworde(void){pair J;
  J.B.l=M_RDMEM_OPCODE();
  J.B.h=M_RDMEM_OPCODE();
  R.SP.B.l=M_RDMEM((J.W.l++));
  R.SP.B.h=M_RDMEM(J.W.l);
  }

static void rrd(void){ byte I;pair J;
  I=M_RDMEM(R.HL.D);
  J.B.l=(I>>4)|(R.AF.B.h<<4);
  M_WRMEM(R.HL.D,J.B.l);
  R.AF.B.h=(I&0x0F)|(R.AF.B.h&0xF0);
  R.AF.B.l=
    ZSPTable[R.AF.B.h]|(R.AF.B.l&C_FLAG);
  }
static void rld(void){ byte I;pair J;
  I=M_RDMEM(R.HL.D);
  J.B.l=(I<<4)|(R.AF.B.h&0x0F);
  M_WRMEM(R.HL.D,J.B.l);
  R.AF.B.h=(I>>4)|(R.AF.B.h&0xF0);
  R.AF.B.l=
    ZSPTable[R.AF.B.h]|(R.AF.B.l&C_FLAG);
  }

static void ld_a_i(void){
  R.AF.B.h=R.I;
  R.AF.B.l=
    (R.AF.B.l&C_FLAG)|(R.IFF&0x40? P_FLAG:0)|
    (R.AF.B.h? 0:Z_FLAG)|(R.AF.B.h&S_FLAG);
  }

static void ld_a_r(void){
  R.AF.B.h=(R.R&0x7F)|(R.R2&0x80);
  R.AF.B.l=
    (R.AF.B.l&C_FLAG)|(R.IFF&0x40? P_FLAG:0)|
    (R.AF.B.h? 0:Z_FLAG)|(R.AF.B.h&S_FLAG);
  }

static void ld_i_a(void){   R.I=R.AF.B.h;}
static void ld_r_a(void){   R.R=R.R2=R.AF.B.h; }

static void im_0(void)
{
 R.IFF&=0xF9;
}

static void im_1(void)
{
 R.IFF=(R.IFF&0xF9)|2;
}

static void im_2(void)
{
 R.IFF=(R.IFF&0xF9)|4;
}
       
static void reti(void)
{
 M_RET;
}
static void retn(void)
{
 if (R.IFF&0x40)
  R.IFF|=1;
 else
  R.IFF&=0xFE;
 M_RET;
}
       
static void neg(void){ byte I; I=R.AF.B.h;R.AF.B.h=0;M_SUB(I);}
       
static void in_b_xc(void){  M_IN(R.BC.B.h);}    
static void in_c_xc(void){  M_IN(R.BC.B.l);}    
static void in_d_xc(void){  M_IN(R.DE.B.h);}    
static void in_e_xc(void){  M_IN(R.DE.B.l);}    
static void in_h_xc(void){  M_IN(R.HL.B.h);}    
static void in_l_xc(void){  M_IN(R.HL.B.l);}    
static void in_a_xc(void){  M_IN(R.AF.B.h);}    
static void in_f_xc(void){pair J; M_IN(J.B.l);}    
       
static void out_xc_b(void){ DoOut(R.BC.B.l,R.BC.B.h);}     
static void out_xc_c(void){ DoOut(R.BC.B.l,R.BC.B.l);}     
static void out_xc_d(void){ DoOut(R.BC.B.l,R.DE.B.h);}     
static void out_xc_e(void){ DoOut(R.BC.B.l,R.DE.B.l);}     
static void out_xc_h(void){ DoOut(R.BC.B.l,R.HL.B.h);}     
static void out_xc_l(void){ DoOut(R.BC.B.l,R.HL.B.l);}     
static void out_xc_a(void){ DoOut(R.BC.B.l,R.AF.B.h);}     
       
static void ini(void)
{
 M_WRMEM(R.HL.D,DoIn(R.BC.B.l));R.HL.W.l++;R.BC.B.h--;
 R.AF.B.l=N_FLAG|(R.BC.B.h? 0:Z_FLAG);
}

static void inir(void)
{
 R.R-=2;
 do
 {
  R.R+=2;
  M_WRMEM(R.HL.D,DoIn(R.BC.B.l));R.HL.W.l++;R.BC.B.h--;
  ICount-=21;
 }
 while (R.BC.B.h && ICount>0);
 R.AF.B.l=N_FLAG|(R.BC.B.h? 0:Z_FLAG);
 if (R.BC.B.h)
  R.PC.W.l-=2;
 else
  ICount+=5;
}

static void ind(void)
{
 M_WRMEM(R.HL.D,DoIn(R.BC.B.l));R.HL.W.l--;R.BC.B.h--;
 R.AF.B.l=N_FLAG|(R.BC.B.h? 0:Z_FLAG);
}

static void indr(void)
{
 R.R-=2;
 do
 {
  R.R+=2;
  M_WRMEM(R.HL.D,DoIn(R.BC.B.l));R.HL.W.l--;R.BC.B.h--;
  ICount-=21;
 }
 while (R.BC.B.h && ICount>0);
 R.AF.B.l=N_FLAG|(R.BC.B.h? 0:Z_FLAG);
 if (R.BC.B.h)
  R.PC.W.l-=2;
 else
  ICount+=5;
}

static void outi(void)
{
 DoOut(R.BC.B.l,M_RDMEM(R.HL.D));R.HL.W.l++;R.BC.B.h--;
 R.AF.B.l=N_FLAG|(R.BC.B.h? 0:Z_FLAG);
}

static void otir(void)
{
 R.R-=2;
 do
 {
  R.R+=2;
  DoOut(R.BC.B.l,M_RDMEM(R.HL.D));R.HL.W.l++;R.BC.B.h--;
  ICount-=21;
 }
 while (R.BC.B.h && ICount>0);
 R.AF.B.l=N_FLAG|(R.BC.B.h? 0:Z_FLAG);
 if (R.BC.B.h)
  R.PC.W.l-=2;
 else
  ICount+=5;
}

static void outd(void)
{
 DoOut(R.BC.B.l,M_RDMEM(R.HL.D));R.HL.W.l--;R.BC.B.h--;
 R.AF.B.l=N_FLAG|(R.BC.B.h? 0:Z_FLAG);
}

static void otdr(void)
{
 R.R-=2;
 do
 {
  R.R+=2;
  DoOut(R.BC.B.l,M_RDMEM(R.HL.D));R.HL.W.l--;R.BC.B.h--;
  ICount-=21;
 }
 while (R.BC.B.h && ICount>0);
 R.AF.B.l=N_FLAG|(R.BC.B.h? 0:Z_FLAG);
 if (R.BC.B.h)
  R.PC.W.l-=2;
 else
  ICount+=5;
}

static void ldi(void)
{
 byte I;
 I=M_RDMEM(R.HL.D);R.HL.W.l++;M_WRMEM(R.DE.D,I);R.DE.W.l++;R.BC.W.l--;
 R.AF.B.l=(R.AF.B.l&~(N_FLAG|H_FLAG|P_FLAG))|(R.BC.W.l? P_FLAG:0);
}

static void ldir(void)
{
 byte I;
 R.R-=2;
 do
 {
  R.R+=2;
  I=M_RDMEM(R.HL.D);R.HL.W.l++;M_WRMEM(R.DE.D,I);R.DE.W.l++;R.BC.W.l--;
  ICount-=21;
 }
 while (R.BC.W.l && ICount>0);
 R.AF.B.l=(R.AF.B.l&~(N_FLAG|H_FLAG|P_FLAG))|(R.BC.W.l? P_FLAG:0);
 if (R.BC.W.l)
  R.PC.W.l-=2;
 else
  ICount+=5;
}

static void ldd(void)
{
 byte I;
 I=M_RDMEM(R.HL.D);R.HL.W.l--;M_WRMEM(R.DE.D,I);R.DE.W.l--;R.BC.W.l--;
 R.AF.B.l=(R.AF.B.l&~(N_FLAG|H_FLAG|P_FLAG))|(R.BC.W.l? P_FLAG:0);
}

static void lddr(void)
{
 byte I;
 R.R-=2;
 do
 {
  R.R+=2;
  I=M_RDMEM(R.HL.D);R.HL.W.l--;M_WRMEM(R.DE.D,I);R.DE.W.l--;R.BC.W.l--;
  ICount-=21;
 }
 while (R.BC.W.l && ICount>0);
 R.AF.B.l=(R.AF.B.l&~(N_FLAG|H_FLAG|P_FLAG))|(R.BC.W.l? P_FLAG:0);
 if (R.BC.W.l)
  R.PC.W.l-=2;
 else
  ICount+=5;
}

static void cpi(void)
{
 byte I,J;
 I=M_RDMEM(R.HL.D);R.HL.W.l++;J=R.AF.B.h-I;R.BC.W.l--;
 R.AF.B.l=
  N_FLAG|(R.AF.B.l&C_FLAG)|ZSTable[J]|
  ((R.AF.B.h^I^J)&H_FLAG)|(R.BC.W.l? P_FLAG:0);
}

static void cpir(void)
{
 byte I,J;
 R.R-=2;
 do
 {
  R.R+=2;
  I=M_RDMEM(R.HL.D);R.HL.W.l++;J=R.AF.B.h-I;R.BC.W.l--;
  ICount-=21;
 }
 while (R.BC.W.l && J && ICount>0);
 R.AF.B.l=
  N_FLAG|(R.AF.B.l&C_FLAG)|ZSTable[J]|
  ((R.AF.B.h^I^J)&H_FLAG)|(R.BC.W.l? P_FLAG:0);
 if (R.BC.W.l && J)
  R.PC.W.l-=2;
 else
  ICount+=5;
}

static void cpd(void)
{
 byte I,J;
 I=M_RDMEM(R.HL.D);R.HL.W.l--;J=R.AF.B.h-I;R.BC.W.l--;
 R.AF.B.l=
  N_FLAG|(R.AF.B.l&C_FLAG)|ZSTable[J]|
  ((R.AF.B.h^I^J)&H_FLAG)|(R.BC.W.l? P_FLAG:0);
}

static void cpdr(void)
{
 byte I,J;
 R.R-=2;
 do
 {
  R.R+=2;
  I=M_RDMEM(R.HL.D);R.HL.W.l--;J=R.AF.B.h-I;R.BC.W.l--;
  ICount-=21;
 }
 while (R.BC.W.l && J && ICount>0);
 R.AF.B.l=
  N_FLAG|(R.AF.B.l&C_FLAG)|ZSTable[J]|
  ((R.AF.B.h^I^J)&H_FLAG)|(R.BC.W.l? P_FLAG:0);
 if (R.BC.W.l && J)
  R.PC.W.l-=2;
 else
  ICount+=5;
}

static void patch (void)
{
 Patch (&R);
}

FnPtr FnED[256]=
{
  fuck,fuck,fuck,fuck,fuck,fuck,fuck,fuck,
  fuck,fuck,fuck,fuck,fuck,fuck,fuck,fuck,
  fuck,fuck,fuck,fuck,fuck,fuck,fuck,fuck,
  fuck,fuck,fuck,fuck,fuck,fuck,fuck,fuck,
  fuck,fuck,fuck,fuck,fuck,fuck,fuck,fuck,
  fuck,fuck,fuck,fuck,fuck,fuck,fuck,fuck,
  fuck,fuck,fuck,fuck,fuck,fuck,fuck,fuck,
  fuck,fuck,fuck,fuck,fuck,fuck,fuck,fuck,
  in_b_xc,out_xc_b,sbc_hl_bc,ld_xworde_bc,neg,retn,im_0,ld_i_a,
  in_c_xc,out_xc_c,adc_hl_bc,ld_bc_xworde,fuck,reti,fuck,ld_r_a,
  in_d_xc,out_xc_d,sbc_hl_de,ld_xworde_de,fuck,fuck,im_1,ld_a_i,
  in_e_xc,out_xc_e,adc_hl_de,ld_de_xworde,fuck,fuck,im_2,ld_a_r,
  in_h_xc,out_xc_h,sbc_hl_hl,ld_xworde_hl,fuck,fuck,fuck,rrd,
  in_l_xc,out_xc_l,adc_hl_hl,ld_hl_xworde,fuck,fuck,fuck,rld,
  in_f_xc,fuck,sbc_hl_sp,ld_xworde_sp,fuck,fuck,fuck,fuck,
  in_a_xc,out_xc_a,adc_hl_sp,ld_sp_xworde,fuck,fuck,fuck,fuck,
  fuck,fuck,fuck,fuck,fuck,fuck,fuck,fuck,
  fuck,fuck,fuck,fuck,fuck,fuck,fuck,fuck,
  fuck,fuck,fuck,fuck,fuck,fuck,fuck,fuck,
  fuck,fuck,fuck,fuck,fuck,fuck,fuck,fuck,
  ldi,cpi,ini,outi,fuck,fuck,fuck,fuck,
  ldd,cpd,ind,outd,fuck,fuck,fuck,fuck,
  ldir,cpir,inir,otir,fuck,fuck,fuck,fuck,
  lddr,cpdr,indr,otdr,fuck,fuck,fuck,fuck,
  fuck,fuck,fuck,fuck,fuck,fuck,fuck,fuck,
  fuck,fuck,fuck,fuck,fuck,fuck,fuck,fuck,
  fuck,fuck,fuck,fuck,fuck,fuck,fuck,fuck,
  fuck,fuck,fuck,fuck,fuck,fuck,fuck,fuck,
  fuck,fuck,fuck,fuck,fuck,fuck,fuck,fuck,
  fuck,fuck,fuck,fuck,fuck,fuck,fuck,fuck,
  fuck,fuck,fuck,fuck,fuck,fuck,fuck,fuck,
  fuck,fuck,fuck,fuck,fuck,fuck,patch,fuck
};

unsigned CyclesED[256]=
{
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  12,12,15,20,8,14,8,9,
  12,12,15,20,0,14,0,9,
  12,12,15,20,0,0,8,9,
  12,12,15,20,0,0,8,9,
  12,12,15,20,0,0,0,18,
  12,12,15,20,0,0,0,18,
  12,00,15,20,0,0,0,0,
  12,12,15,20,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  16,16,16,16,0,0,0,0,
  16,16,16,16,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
};
