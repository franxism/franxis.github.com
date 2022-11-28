/** Z80: portable Z80 emulator **********************************************/
/**                                                                        **/
/**                                 Z80.c                                  **/
/**                                                                        **/
/** This file contains the main engine of the Z80 emulator                 **/
/**                                                                        **/
/** Copyright (C) Marat Fayzullin 1994,1995,1996                           **/
/**               Marcel de Kogel 1996                                     **/
/**     You are not allowed to distribute this software commercially       **/
/**     Please, notify me, if you make any changes to this file            **/
/****************************************************************************/

/* Define this if you want Z80_RegisterDump() to print a PC trace */
/* #define TRACE */

#include "Z80.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

static void Z80_Interrupt (int J);

typedef void (*FnPtr)(void);
extern FnPtr FnMain[256],FnCB[256],FnDD[256],FnED[256],FnFD[256];
extern unsigned CyclesMain[256],CyclesCB[256],CyclesDD[256],CyclesED[256],CyclesFD[256];

/*** Registers ***********************************************/
/*** Z80 registers, RAM address, and running flag.         ***/
/*************************************************************/
reg R;
byte CPURunning;

/*** Interrupts **********************************************/
/*** Interrupt-related variables.                          ***/
/*************************************************************/
int IPeriod=50000;
int ICount;

/*** Trace and Trap ******************************************/
/*** Switches to turn tracing on and off in DEBUG mode.    ***/
/*************************************************************/
#ifdef DEBUG
int  Trace=0;       /* Tracing is on if Trace==1  */
int  Trap=-1;       /* When PC==Trap, set Trace=1 */
#endif

/*** TrapBadOps **********************************************/
/*** When 1, print warnings of illegal Z80 instructions.   ***/
/*************************************************************/
byte TrapBadOps=0;

byte PTable[512] =
{
 P_FLAG,0,0,P_FLAG,0,P_FLAG,P_FLAG,0,0,P_FLAG,P_FLAG,0,P_FLAG,0,0,P_FLAG,
 0,P_FLAG,P_FLAG,0,P_FLAG,0,0,P_FLAG,P_FLAG,0,0,P_FLAG,0,P_FLAG,P_FLAG,0,
 0,P_FLAG,P_FLAG,0,P_FLAG,0,0,P_FLAG,P_FLAG,0,0,P_FLAG,0,P_FLAG,P_FLAG,0,
 P_FLAG,0,0,P_FLAG,0,P_FLAG,P_FLAG,0,0,P_FLAG,P_FLAG,0,P_FLAG,0,0,P_FLAG,
 0,P_FLAG,P_FLAG,0,P_FLAG,0,0,P_FLAG,P_FLAG,0,0,P_FLAG,0,P_FLAG,P_FLAG,0,
 P_FLAG,0,0,P_FLAG,0,P_FLAG,P_FLAG,0,0,P_FLAG,P_FLAG,0,P_FLAG,0,0,P_FLAG,
 P_FLAG,0,0,P_FLAG,0,P_FLAG,P_FLAG,0,0,P_FLAG,P_FLAG,0,P_FLAG,0,0,P_FLAG,
 0,P_FLAG,P_FLAG,0,P_FLAG,0,0,P_FLAG,P_FLAG,0,0,P_FLAG,0,P_FLAG,P_FLAG,0,
 0,P_FLAG,P_FLAG,0,P_FLAG,0,0,P_FLAG,P_FLAG,0,0,P_FLAG,0,P_FLAG,P_FLAG,0,
 P_FLAG,0,0,P_FLAG,0,P_FLAG,P_FLAG,0,0,P_FLAG,P_FLAG,0,P_FLAG,0,0,P_FLAG,
 P_FLAG,0,0,P_FLAG,0,P_FLAG,P_FLAG,0,0,P_FLAG,P_FLAG,0,P_FLAG,0,0,P_FLAG,
 0,P_FLAG,P_FLAG,0,P_FLAG,0,0,P_FLAG,P_FLAG,0,0,P_FLAG,0,P_FLAG,P_FLAG,0,
 P_FLAG,0,0,P_FLAG,0,P_FLAG,P_FLAG,0,0,P_FLAG,P_FLAG,0,P_FLAG,0,0,P_FLAG,
 0,P_FLAG,P_FLAG,0,P_FLAG,0,0,P_FLAG,P_FLAG,0,0,P_FLAG,0,P_FLAG,P_FLAG,0,
 0,P_FLAG,P_FLAG,0,P_FLAG,0,0,P_FLAG,P_FLAG,0,0,P_FLAG,0,P_FLAG,P_FLAG,0,
 P_FLAG,0,0,P_FLAG,0,P_FLAG,P_FLAG,0,0,P_FLAG,P_FLAG,0,P_FLAG,0,0,P_FLAG
};

byte ZSTable[512];
byte ZSPTable[512];
static short DAATable[2048]=
{
 #include "DAA.h"
};

void Z80_BadOpcode (void)
{
 if(TrapBadOps)
  printf ( "Unrecognized instruction: %X at PC=%hX\n",
            M_RDMEM(R.PC.W.l-1),R.PC.W.l-1 );
}

void Z80_Halt (void)
{
 R.PC.W.l--;R.IFF|=0x80;
 if (ICount>0)
  ICount=0;
}

static void pfx_cb(void)
{
 unsigned opcode;
 ++R.R;
 opcode=M_RDMEM_OPCODE();
 ICount-=CyclesCB[opcode];
 (*(FnCB[opcode]))();
}

static void pfx_ed(void)
{
 unsigned opcode;
 ++R.R;
 opcode=M_RDMEM_OPCODE();
 ICount-=CyclesED[opcode];
 (*(FnED[opcode]))();
}

static void pfx_fd (void)
{
 unsigned opcode;
 ++R.R;
 opcode=M_RDMEM_OPCODE();
 ICount-=CyclesFD[opcode];
 (*(FnFD[opcode]))();
}

static void pfx_dd (void)
{
 unsigned opcode;
 ++R.R;
 opcode=M_RDMEM_OPCODE();
 ICount-=CyclesDD[opcode];
 (*(FnDD[opcode]))();
}

static void halt (void)
{
 Z80_Halt ();
}

static void jr_nz(void)
{
 if(R.AF.B.l&Z_FLAG)
  R.PC.W.l++;
 else
 {
  M_JR;
  ICount-=5;
 }
}

static void jr_nc(void)
{
 if(R.AF.B.l&C_FLAG)
  R.PC.W.l++;
 else
 {
  M_JR;
  ICount-=5;
 }
}

static void jr_z(void)
{
 if(R.AF.B.l&Z_FLAG)
 {
  M_JR;
  ICount-=5;
 }
 else
  R.PC.W.l++;
}

static void jr_c(void)
{
 if(R.AF.B.l&C_FLAG)
 {
  M_JR;
  ICount-=5;
 }
 else
  R.PC.W.l++;
}
               
static void jp_nz(void){ if(R.AF.B.l&Z_FLAG)  R.PC.W.l+=2; else { M_JP; }  }  
static void jp_nc(void){ if(R.AF.B.l&C_FLAG)  R.PC.W.l+=2; else { M_JP; }  }  
static void jp_po(void){ if(R.AF.B.l&P_FLAG)  R.PC.W.l+=2; else { M_JP; }  }  
static void jp_p(void){  if(R.AF.B.l&S_FLAG)  R.PC.W.l+=2; else { M_JP; }  } 
static void jp_z(void){  if(R.AF.B.l&Z_FLAG)  { M_JP; }  else R.PC.W.l+=2; } 
static void jp_c(void){  if(R.AF.B.l&C_FLAG)  { M_JP; }  else R.PC.W.l+=2; } 
static void jp_pe(void){ if(R.AF.B.l&P_FLAG)  { M_JP; }  else R.PC.W.l+=2; }  
static void jp_m(void){  if(R.AF.B.l&S_FLAG)  { M_JP; }  else R.PC.W.l+=2; } 
               
static void ret_nz(void){ if(!(R.AF.B.l&Z_FLAG))  { M_RET;ICount-=6; }  }
static void ret_nc(void){ if(!(R.AF.B.l&C_FLAG))  { M_RET;ICount-=6; }  }     
static void ret_po(void){ if(!(R.AF.B.l&P_FLAG))  { M_RET;ICount-=6; }  }     
static void ret_p(void){  if(!(R.AF.B.l&S_FLAG))  { M_RET;ICount-=6; }  }    
static void ret_z(void){  if(R.AF.B.l&Z_FLAG)     { M_RET;ICount-=6; }  } 
static void ret_c(void){  if(R.AF.B.l&C_FLAG)     { M_RET;ICount-=6; }  } 
static void ret_pe(void){ if(R.AF.B.l&P_FLAG)     { M_RET;ICount-=6; }  }  
static void ret_m(void){  if(R.AF.B.l&S_FLAG)     { M_RET;ICount-=6; }  } 
               
static void call_nz(void){ if(R.AF.B.l&Z_FLAG)  R.PC.W.l+=2;  else  { M_CALL;ICount-=7; } }    
static void call_nc(void){ if(R.AF.B.l&C_FLAG)  R.PC.W.l+=2;  else  { M_CALL;ICount-=7; } }    
static void call_po(void){ if(R.AF.B.l&P_FLAG)  R.PC.W.l+=2;  else  { M_CALL;ICount-=7; } }    
static void call_p(void){  if(R.AF.B.l&S_FLAG)  R.PC.W.l+=2;  else  { M_CALL;ICount-=7; } }   
static void call_z(void){  if(R.AF.B.l&Z_FLAG)  { M_CALL;ICount-=7; } else R.PC.W.l+=2; }   
static void call_c(void){  if(R.AF.B.l&C_FLAG)  { M_CALL;ICount-=7; } else R.PC.W.l+=2; }   
static void call_pe(void){ if(R.AF.B.l&P_FLAG)  { M_CALL;ICount-=7; } else R.PC.W.l+=2; }    
static void call_m(void){  if(R.AF.B.l&S_FLAG)  { M_CALL;ICount-=7; } else R.PC.W.l+=2; }   
               
static void add_b(void){ M_ADD(R.BC.B.h);}          
static void add_c(void){ M_ADD(R.BC.B.l);}          
static void add_d(void){ M_ADD(R.DE.B.h);}          
static void add_e(void){ M_ADD(R.DE.B.l);}          
static void add_h(void){ M_ADD(R.HL.B.h);}          
static void add_l(void){ M_ADD(R.HL.B.l);}          
static void add_a(void){ M_ADD(R.AF.B.h);}          
static void add_xhl(void){ byte I; I=M_RDMEM(R.HL.D);M_ADD(I);}            
static void add_byte(void){ byte I; I=M_RDMEM_OPCODE();M_ADD(I);}             
               
static void sub_b(void){ M_SUB(R.BC.B.h);}          
static void sub_c(void){ M_SUB(R.BC.B.l);}          
static void sub_d(void){ M_SUB(R.DE.B.h);}          
static void sub_e(void){ M_SUB(R.DE.B.l);}          
static void sub_h(void){ M_SUB(R.HL.B.h);}          
static void sub_l(void){ M_SUB(R.HL.B.l);}          
static void sub_a(void){ R.AF.B.h=0;R.AF.B.l=N_FLAG|Z_FLAG;}          
static void sub_xhl(void){byte I; I=M_RDMEM(R.HL.D);M_SUB(I);}            
static void sub_byte(void){byte I; I=M_RDMEM_OPCODE();M_SUB(I);}             
               
static void and_b(void){ M_AND(R.BC.B.h); }          
static void and_c(void){ M_AND(R.BC.B.l); }          
static void and_d(void){ M_AND(R.DE.B.h); }          
static void and_e(void){ M_AND(R.DE.B.l); }          
static void and_h(void){ M_AND(R.HL.B.h); }          
static void and_l(void){ M_AND(R.HL.B.l); }          
static void and_a(void){ M_AND(R.AF.B.h); }          
static void and_xhl(void){  byte I; I=M_RDMEM(R.HL.D);M_AND(I); }            
static void and_byte(void){ byte I; I=M_RDMEM_OPCODE();M_AND(I); }             
               
static void or_b(void){ M_OR(R.BC.B.h);}         
static void or_c(void){ M_OR(R.BC.B.l);}         
static void or_d(void){ M_OR(R.DE.B.h);}         
static void or_e(void){ M_OR(R.DE.B.l);}         
static void or_h(void){ M_OR(R.HL.B.h);}         
static void or_l(void){ M_OR(R.HL.B.l);}         
static void or_a(void){ M_OR(R.AF.B.h);}         
static void or_xhl(void){ byte I; I=M_RDMEM(R.HL.D);M_OR(I); }           
static void or_byte(void){ byte I; I=M_RDMEM_OPCODE();M_OR(I); }            
               
static void adc_b(void){ M_ADC(R.BC.B.h); }          
static void adc_c(void){ M_ADC(R.BC.B.l); }          
static void adc_d(void){ M_ADC(R.DE.B.h); }          
static void adc_e(void){ M_ADC(R.DE.B.l); }          
static void adc_h(void){ M_ADC(R.HL.B.h); }          
static void adc_l(void){ M_ADC(R.HL.B.l); }          
static void adc_a(void){ M_ADC(R.AF.B.h); }          
static void adc_xhl(void){  byte I; I=M_RDMEM(R.HL.D);M_ADC(I); }            
static void adc_byte(void){ byte I; I=M_RDMEM_OPCODE();M_ADC(I); }             
               
static void sbc_b(void){ M_SBC(R.BC.B.h); }          
static void sbc_c(void){ M_SBC(R.BC.B.l); }          
static void sbc_d(void){ M_SBC(R.DE.B.h); }          
static void sbc_e(void){ M_SBC(R.DE.B.l); }          
static void sbc_h(void){ M_SBC(R.HL.B.h); }          
static void sbc_l(void){ M_SBC(R.HL.B.l); }          
static void sbc_a(void){ M_SBC(R.AF.B.h); }          
static void sbc_xhl(void){ byte I; I=M_RDMEM(R.HL.D);M_SBC(I); }            
static void sbc_byte(void){ byte I; I=M_RDMEM_OPCODE();M_SBC(I); }             
               
static void xor_b(void){ M_XOR(R.BC.B.h); }          
static void xor_c(void){ M_XOR(R.BC.B.l); }          
static void xor_d(void){ M_XOR(R.DE.B.h); }          
static void xor_e(void){ M_XOR(R.DE.B.l); }          
static void xor_h(void){ M_XOR(R.HL.B.h); }          
static void xor_l(void){ M_XOR(R.HL.B.l); }          
static void xor_a(void){ R.AF.B.h=0;R.AF.B.l=P_FLAG|Z_FLAG; }          
static void xor_xhl(void){ byte I; I=M_RDMEM(R.HL.D);M_XOR(I); }            
static void xor_byte(void){ byte I; I=M_RDMEM_OPCODE();M_XOR(I); }             
               
static void cp_b(void){ M_CP(R.BC.B.h); }         
static void cp_c(void){ M_CP(R.BC.B.l); }         
static void cp_d(void){ M_CP(R.DE.B.h); }         
static void cp_e(void){ M_CP(R.DE.B.l); }         
static void cp_h(void){ M_CP(R.HL.B.h); }         
static void cp_l(void){ M_CP(R.HL.B.l); }         
static void cp_a(void){ R.AF.B.l=N_FLAG|Z_FLAG; }         
static void cp_xhl(void){ byte I; I=M_RDMEM(R.HL.D);M_CP(I); }           
static void cp_byte(void){ byte I; I=M_RDMEM_OPCODE();M_CP(I); }            
               
static void ld_bc_word(void){ M_LDWORD(BC); }             
static void ld_de_word(void){ M_LDWORD(DE); }             
static void ld_hl_word(void){ M_LDWORD(HL); }             
static void ld_sp_word(void){ M_LDWORD(SP); }             
               
static void ld_pc_hl(void){ R.PC.D=R.HL.D; }             
static void ld_sp_hl(void){ R.SP.D=R.HL.D; }             
static void ld_a_xbc(void){ R.AF.B.h=M_RDMEM(R.BC.D); }             
static void ld_a_xde(void){ R.AF.B.h=M_RDMEM(R.DE.D); }             
               
static void add_hl_bc(void){ M_ADDW(HL,BC); }            
static void add_hl_de(void){ M_ADDW(HL,DE); }            
static void add_hl_hl(void){ M_ADDW(HL,HL); }            
static void add_hl_sp(void){ M_ADDW(HL,SP); }            
               
static void dec_bc(void){ R.BC.W.l--; }           
static void dec_de(void){ R.DE.W.l--; }           
static void dec_hl(void){ R.HL.W.l--; }           
static void dec_sp(void){ R.SP.W.l--; }           
               
static void inc_bc(void){ R.BC.W.l++; }           
static void inc_de(void){ R.DE.W.l++; }           
static void inc_hl(void){ R.HL.W.l++; }           
static void inc_sp(void){ R.SP.W.l++; }           
               
static void dec_b(void){ M_DEC(R.BC.B.h); }          
static void dec_c(void){ M_DEC(R.BC.B.l); }          
static void dec_d(void){ M_DEC(R.DE.B.h); }          
static void dec_e(void){ M_DEC(R.DE.B.l); }          
static void dec_h(void){ M_DEC(R.HL.B.h); }          
static void dec_l(void){ M_DEC(R.HL.B.l); }          
static void dec_a(void){ M_DEC(R.AF.B.h); }          
static void dec_xhl(void){ byte I; I=M_RDMEM(R.HL.D);M_DEC(I);M_WRMEM(R.HL.D,I); }            
               
static void inc_b(void){ M_INC(R.BC.B.h);}          
static void inc_c(void){ M_INC(R.BC.B.l);}          
static void inc_d(void){ M_INC(R.DE.B.h);}          
static void inc_e(void){ M_INC(R.DE.B.l);}          
static void inc_h(void){ M_INC(R.HL.B.h);}          
static void inc_l(void){ M_INC(R.HL.B.l);}          
static void inc_a(void){ M_INC(R.AF.B.h);}          
static void inc_xhl(void){ byte I; I=M_RDMEM(R.HL.D);M_INC(I);M_WRMEM(R.HL.D,I); }            

static void rlca(void) {byte I;
  I=(R.AF.B.h&0x80)>>7;
  R.AF.B.h=(R.AF.B.h<<1)|I;
  R.AF.B.l=(R.AF.B.l&~(C_FLAG|N_FLAG|H_FLAG))|I;
  }
static void rla(void) {byte I;
  I=(R.AF.B.h&0x80)>>7;
  R.AF.B.h=(R.AF.B.h<<1)|(R.AF.B.l&C_FLAG);
  R.AF.B.l=(R.AF.B.l&~(C_FLAG|N_FLAG|H_FLAG))|I;
  }
static void rrca(void) {byte I;
  I=R.AF.B.h&0x01;
  R.AF.B.h=(R.AF.B.h>>1)|(I<<7);
  R.AF.B.l=(R.AF.B.l&~(C_FLAG|N_FLAG|H_FLAG))|I; 
  }
static void rra(void) {byte I;
  I=R.AF.B.h&0x01;
  R.AF.B.h=(R.AF.B.h>>1)|((R.AF.B.l&C_FLAG)<<7);
  R.AF.B.l=(R.AF.B.l&~(C_FLAG|N_FLAG|H_FLAG))|I;
  }
 
static void rst00(void){ M_RST(0x0000); }  
static void rst08(void){ M_RST(0x0008); }  
static void rst10(void){ M_RST(0x0010); }  
static void rst18(void){ M_RST(0x0018); }  
static void rst20(void)
{
 M_RST(0x0020);
}
static void rst28(void){ M_RST(0x0028); }  
static void rst30(void){ M_RST(0x0030); }  
static void rst38(void)
{
 M_RST(0x0038);
}
       
static void push_bc(void){ M_PUSH(BC); }    
static void push_de(void){ M_PUSH(DE); }    
static void push_hl(void){ M_PUSH(HL); }    
static void push_af(void){ M_PUSH(AF); }    
       
static void pop_bc(void){ M_POP(BC); }   
static void pop_de(void){ M_POP(DE); }   
static void pop_hl(void){ M_POP(HL); }   
static void pop_af(void){ M_POP(AF); }   
       
static void djnz(void){ if(--R.BC.B.h) { M_JR; } else R.PC.W.l++; }
static void jp(void){ M_JP; }
static void jr(void){ M_JR; }
static void call(void){ M_CALL; }
static void ret(void){ M_RET; }

static void scf(void){  S(C_FLAG);R(N_FLAG|H_FLAG); }
static void cpl(void){  R.AF.B.h=~R.AF.B.h;S(N_FLAG|H_FLAG); }
static void ccf(void)
{
 R.AF.B.l=((R.AF.B.l&0xED)|((R.AF.B.l&1)<<4))^1;
}
static void nop(void){  }    
static void ei(void)
{
 unsigned opcode;
 R.IFF|=0x41;
 opcode=M_RDMEM_OPCODE();
 ICount-=CyclesMain[opcode];
 (*(FnMain[opcode]))();
 Z80_Interrupt (InterruptsEnabled());
}
static void di(void){ R.IFF&=0xBE; }
static void outa(void){ DoOut(M_RDMEM_OPCODE(),R.AF.B.h); }
static void ina(void){ R.AF.B.h=DoIn(M_RDMEM_OPCODE()); } 

static void exx(void){pair J;
  J.D=R.BC.D;R.BC.D=R.BC1.D;R.BC1.D=J.D;
  J.D=R.DE.D;R.DE.D=R.DE1.D;R.DE1.D=J.D;
  J.D=R.HL.D;R.HL.D=R.HL1.D;R.HL1.D=J.D;
  }

static void ex_de_hl(void){ pair J; J.D=R.DE.D;R.DE.D=R.HL.D;R.HL.D=J.D; }
static void ex_af_af(void){ pair J; J.D=R.AF.D;R.AF.D=R.AF1.D;R.AF1.D=J.D; }
     
static void ld_b_b(void){ R.BC.B.h=R.BC.B.h; }
static void ld_c_b(void){ R.BC.B.l=R.BC.B.h; }
static void ld_d_b(void){ R.DE.B.h=R.BC.B.h; }
static void ld_e_b(void){ R.DE.B.l=R.BC.B.h; }
static void ld_h_b(void){ R.HL.B.h=R.BC.B.h; }
static void ld_l_b(void){ R.HL.B.l=R.BC.B.h; }
static void ld_a_b(void){ R.AF.B.h=R.BC.B.h; }
static void ld_xhl_b(void){ M_WRMEM(R.HL.D,R.BC.B.h); }
     
static void ld_b_c(void){ R.BC.B.h=R.BC.B.l; }
static void ld_c_c(void){ R.BC.B.l=R.BC.B.l; }
static void ld_d_c(void){ R.DE.B.h=R.BC.B.l; }
static void ld_e_c(void){ R.DE.B.l=R.BC.B.l; }
static void ld_h_c(void){ R.HL.B.h=R.BC.B.l; }
static void ld_l_c(void){ R.HL.B.l=R.BC.B.l; }
static void ld_a_c(void){ R.AF.B.h=R.BC.B.l; }
static void ld_xhl_c(void){ M_WRMEM(R.HL.D,R.BC.B.l); }
     
static void ld_b_d(void){ R.BC.B.h=R.DE.B.h; }
static void ld_c_d(void){ R.BC.B.l=R.DE.B.h; }
static void ld_d_d(void){ R.DE.B.h=R.DE.B.h; }
static void ld_e_d(void){ R.DE.B.l=R.DE.B.h; }
static void ld_h_d(void){ R.HL.B.h=R.DE.B.h; }
static void ld_l_d(void){ R.HL.B.l=R.DE.B.h; }
static void ld_a_d(void){ R.AF.B.h=R.DE.B.h; }
static void ld_xhl_d(void){ M_WRMEM(R.HL.D,R.DE.B.h); }
     
static void ld_b_e(void){ R.BC.B.h=R.DE.B.l; }
static void ld_c_e(void){ R.BC.B.l=R.DE.B.l; }
static void ld_d_e(void){ R.DE.B.h=R.DE.B.l; }
static void ld_e_e(void){ R.DE.B.l=R.DE.B.l; }
static void ld_h_e(void){ R.HL.B.h=R.DE.B.l; }
static void ld_l_e(void){ R.HL.B.l=R.DE.B.l; }
static void ld_a_e(void){ R.AF.B.h=R.DE.B.l; }
static void ld_xhl_e(void){ M_WRMEM(R.HL.D,R.DE.B.l); }
     
static void ld_b_h(void){ R.BC.B.h=R.HL.B.h; }
static void ld_c_h(void){ R.BC.B.l=R.HL.B.h; }
static void ld_d_h(void){ R.DE.B.h=R.HL.B.h; }
static void ld_e_h(void){ R.DE.B.l=R.HL.B.h; }
static void ld_h_h(void){ R.HL.B.h=R.HL.B.h; }
static void ld_l_h(void){ R.HL.B.l=R.HL.B.h; }
static void ld_a_h(void){ R.AF.B.h=R.HL.B.h; }
static void ld_xhl_h(void){ M_WRMEM(R.HL.D,R.HL.B.h); }
     
static void ld_b_l(void){ R.BC.B.h=R.HL.B.l; }
static void ld_c_l(void){ R.BC.B.l=R.HL.B.l; }
static void ld_d_l(void){ R.DE.B.h=R.HL.B.l; }
static void ld_e_l(void){ R.DE.B.l=R.HL.B.l; }
static void ld_h_l(void){ R.HL.B.h=R.HL.B.l; }
static void ld_l_l(void){ R.HL.B.l=R.HL.B.l; }
static void ld_a_l(void){ R.AF.B.h=R.HL.B.l; }
static void ld_xhl_l(void){ M_WRMEM(R.HL.D,R.HL.B.l); }
     
static void ld_b_a(void){ R.BC.B.h=R.AF.B.h; }
static void ld_c_a(void){ R.BC.B.l=R.AF.B.h; }
static void ld_d_a(void){ R.DE.B.h=R.AF.B.h; }
static void ld_e_a(void){ R.DE.B.l=R.AF.B.h; }
static void ld_h_a(void){ R.HL.B.h=R.AF.B.h; }
static void ld_l_a(void){ R.HL.B.l=R.AF.B.h; }
static void ld_a_a(void){ R.AF.B.h=R.AF.B.h; }
static void ld_xhl_a(void){ M_WRMEM(R.HL.D,R.AF.B.h); }
     
static void ld_xbc_a(void){ M_WRMEM(R.BC.D,R.AF.B.h); }
static void ld_xde_a(void){ M_WRMEM(R.DE.D,R.AF.B.h); }
     
static void ld_b_xhl(void){ R.BC.B.h=M_RDMEM(R.HL.D); }
static void ld_c_xhl(void){ R.BC.B.l=M_RDMEM(R.HL.D); }
static void ld_d_xhl(void){ R.DE.B.h=M_RDMEM(R.HL.D); }
static void ld_e_xhl(void){ R.DE.B.l=M_RDMEM(R.HL.D); }
static void ld_h_xhl(void){ R.HL.B.h=M_RDMEM(R.HL.D); }
static void ld_l_xhl(void){ R.HL.B.l=M_RDMEM(R.HL.D); }
static void ld_a_xhl(void){ R.AF.B.h=M_RDMEM(R.HL.D); }
     
static void ld_b_byte(void){ R.BC.B.h=M_RDMEM_OPCODE(); }
static void ld_c_byte(void){ R.BC.B.l=M_RDMEM_OPCODE(); }
static void ld_d_byte(void){ R.DE.B.h=M_RDMEM_OPCODE(); }
static void ld_e_byte(void){ R.DE.B.l=M_RDMEM_OPCODE(); }
static void ld_h_byte(void){ R.HL.B.h=M_RDMEM_OPCODE(); }
static void ld_l_byte(void){ R.HL.B.l=M_RDMEM_OPCODE(); }
static void ld_a_byte(void){ R.AF.B.h=M_RDMEM_OPCODE(); }
static void ld_xhl_byte(void){byte I; I=M_RDMEM_OPCODE();M_WRMEM(R.HL.D,I); }

static void ld_xword_hl(void){pair J;
  J.B.l=M_RDMEM_OPCODE();
  J.B.h=M_RDMEM_OPCODE();
  M_WRMEM(J.W.l,R.HL.B.l);J.W.l++;
  M_WRMEM(J.W.l,R.HL.B.h);
  }

static void ld_hl_xword(void){pair J;
  J.B.l=M_RDMEM_OPCODE();
  J.B.h=M_RDMEM_OPCODE();
  R.HL.B.l=M_RDMEM(J.W.l++);
  R.HL.B.h=M_RDMEM(J.W.l);
  }

static void ld_a_xword(void){pair J;
  J.B.l=M_RDMEM_OPCODE();
  J.B.h=M_RDMEM_OPCODE(); 
  R.AF.B.h=M_RDMEM(J.W.l);
  }

static void ld_xword_a(void){pair J;
  J.B.l=M_RDMEM_OPCODE();
  J.B.h=M_RDMEM_OPCODE();
  M_WRMEM(J.W.l,R.AF.B.h);
  }

static void ex_hl_xsp(void){pair J;
  J.B.l=M_RDMEM(R.SP.D);M_WRMEM(R.SP.D,R.HL.B.l);R.SP.W.l++;
  J.B.h=M_RDMEM(R.SP.D);M_WRMEM(R.SP.D,R.HL.B.h);R.SP.W.l--;
  R.HL.W.l=J.W.l;
  }

static void daa(void)
{
 int offset;
 offset=R.AF.B.h;
 if (R.AF.B.l&C_FLAG) offset|=256;
 if (R.AF.B.l&H_FLAG) offset|=512;
 if (R.AF.B.l&N_FLAG) offset|=1024;
 R.AF.W.l=DAATable[offset];
}

static FnPtr FnMain[256]=
{
  nop,ld_bc_word,ld_xbc_a,inc_bc,inc_b,dec_b,ld_b_byte,rlca,
  ex_af_af,add_hl_bc,ld_a_xbc,dec_bc,inc_c,dec_c,ld_c_byte,rrca,
  djnz,ld_de_word,ld_xde_a,inc_de,inc_d,dec_d,ld_d_byte,rla,
  jr,add_hl_de,ld_a_xde,dec_de,inc_e,dec_e,ld_e_byte,rra,
  jr_nz,ld_hl_word,ld_xword_hl,inc_hl,inc_h,dec_h,ld_h_byte,daa,
  jr_z,add_hl_hl,ld_hl_xword,dec_hl,inc_l,dec_l,ld_l_byte,cpl,
  jr_nc,ld_sp_word,ld_xword_a,inc_sp,inc_xhl,dec_xhl,ld_xhl_byte,scf,
  jr_c,add_hl_sp,ld_a_xword,dec_sp,inc_a,dec_a,ld_a_byte,ccf,
  ld_b_b,ld_b_c,ld_b_d,ld_b_e,ld_b_h,ld_b_l,ld_b_xhl,ld_b_a,
  ld_c_b,ld_c_c,ld_c_d,ld_c_e,ld_c_h,ld_c_l,ld_c_xhl,ld_c_a,
  ld_d_b,ld_d_c,ld_d_d,ld_d_e,ld_d_h,ld_d_l,ld_d_xhl,ld_d_a,
  ld_e_b,ld_e_c,ld_e_d,ld_e_e,ld_e_h,ld_e_l,ld_e_xhl,ld_e_a,
  ld_h_b,ld_h_c,ld_h_d,ld_h_e,ld_h_h,ld_h_l,ld_h_xhl,ld_h_a,
  ld_l_b,ld_l_c,ld_l_d,ld_l_e,ld_l_h,ld_l_l,ld_l_xhl,ld_l_a,
  ld_xhl_b,ld_xhl_c,ld_xhl_d,ld_xhl_e,ld_xhl_h,ld_xhl_l,halt,ld_xhl_a,
  ld_a_b,ld_a_c,ld_a_d,ld_a_e,ld_a_h,ld_a_l,ld_a_xhl,ld_a_a,
  add_b,add_c,add_d,add_e,add_h,add_l,add_xhl,add_a,
  adc_b,adc_c,adc_d,adc_e,adc_h,adc_l,adc_xhl,adc_a,
  sub_b,sub_c,sub_d,sub_e,sub_h,sub_l,sub_xhl,sub_a,
  sbc_b,sbc_c,sbc_d,sbc_e,sbc_h,sbc_l,sbc_xhl,sbc_a,
  and_b,and_c,and_d,and_e,and_h,and_l,and_xhl,and_a,
  xor_b,xor_c,xor_d,xor_e,xor_h,xor_l,xor_xhl,xor_a,
  or_b,or_c,or_d,or_e,or_h,or_l,or_xhl,or_a,
  cp_b,cp_c,cp_d,cp_e,cp_h,cp_l,cp_xhl,cp_a,
  ret_nz,pop_bc,jp_nz,jp,call_nz,push_bc,add_byte,rst00,
  ret_z,ret,jp_z,pfx_cb,call_z,call,adc_byte,rst08,
  ret_nc,pop_de,jp_nc,outa,call_nc,push_de,sub_byte,rst10,
  ret_c,exx,jp_c,ina,call_c,pfx_dd,sbc_byte,rst18,
  ret_po,pop_hl,jp_po,ex_hl_xsp,call_po,push_hl,and_byte,rst20,
  ret_pe,ld_pc_hl,jp_pe,ex_de_hl,call_pe,pfx_ed,xor_byte,rst28,
  ret_p,pop_af,jp_p,di,call_p,push_af,or_byte,rst30,
  ret_m,ld_sp_hl,jp_m,ei,call_m,pfx_fd,cp_byte,rst38
};
static unsigned CyclesMain[256]=
{
  4,10,7,6,4,4,7,4,
  4,11,7,6,4,4,7,4,
  8,10,7,6,4,4,7,4,
  12,11,7,6,4,4,7,4,
  7,10,16,6,4,4,7,4,
  7,11,16,6,4,4,7,4,
  7,10,13,6,11,11,10,4,
  7,11,13,6,4,4,7,4,
  4,4,4,4,4,4,7,4,
  4,4,4,4,4,4,7,4,
  4,4,4,4,4,4,7,4,
  4,4,4,4,4,4,7,4,
  4,4,4,4,4,4,7,4,
  4,4,4,4,4,4,7,4,
  7,7,7,7,7,7,4,7,
  4,4,4,4,4,4,7,4,
  4,4,4,4,4,4,7,4,
  4,4,4,4,4,4,7,4,
  4,4,4,4,4,4,7,4,
  4,4,4,4,4,4,7,4,
  4,4,4,4,4,4,7,4,
  4,4,4,4,4,4,7,4,
  4,4,4,4,4,4,7,4,
  4,4,4,4,4,4,7,4,
  5,10,10,10,10,11,7,11,
  5,10,10,0,10,17,7,11,
  5,10,10,11,10,11,7,11,
  5,4,10,11,10,0,7,11,
  5,10,10,19,10,11,7,11,
  5,4,10,4,10,0,7,11,
  5,10,10,4,10,11,7,11,
  5,6,10,4,10,0,7,11
};

void ResetZ80(reg *Regs)
{
 memset (Regs,0,sizeof(reg));
 Regs->PC.W.l=0x0000;Regs->SP.W.l=0xF000;
 Regs->AF.W.l=Regs->BC.W.l=Regs->DE.W.l=Regs->HL.W.l=0x0000;
 Regs->AF1.W.l=Regs->BC1.W.l=Regs->DE1.W.l=Regs->HL1.W.l=0x0000;
 Regs->IX.W.l=Regs->IY.W.l=0x0000;
 Regs->I=0x00;Regs->IFF=0x00;
 Regs->R=rand()&255;
 ICount=IPeriod;
}

static void InitTables (void)
{
 static int InitTables_virgin=1;
 byte zs;
 int i;
 if (!InitTables_virgin) return;
 InitTables_virgin=0;
 for (i=0;i<256;++i)
 {
  zs=0;
  if (i==0)
   zs|=Z_FLAG;
  if (i&0x80)
   zs|=S_FLAG;
  ZSTable[i]=zs;
  ZSPTable[i]=zs|PTable[i];
 }
 for (i=0;i<256;++i)
 {
  ZSTable[i+256]=ZSTable[i]|C_FLAG;
  ZSPTable[i+256]=ZSPTable[i]|C_FLAG;
  PTable[i+256]=PTable[i]|C_FLAG;
 }
}

static void Z80_Interrupt (int J)
{
 if (J==IGNORE_INT) return;
 if (J==NMI_INT || R.IFF&0x01)
 {
  if(R.IFF&0x80)           /* CPU was halted */
  {
   R.PC.W.l++;
   R.IFF&=0x7F;
  }
  if(J==NMI_INT)
  {
   M_PUSH(PC);
   R.IFF&=0xFE;
   R.PC.D=0x0066;
  }
  else
  {
   R.IFF&=0xBE;
   if(R.IFF&0x04)
   {
    /* Interrupt mode 2. Jump to [I:J.B.l] */
    M_PUSH(PC);
    J=(J&255)|(R.I<<8);
    R.PC.B.l=M_RDMEM(J);
    R.PC.B.h=M_RDMEM((J+1)&0xFFFF);
   }
   else
    if(R.IFF&0x02)
    {
     /* Interrupt mode 1. Execute RST38 */
     ICount-=CyclesMain[0xFF];
     (*(FnMain[0xFF]))();
    }
    else
    {
     /* Interrupt mode 0. Execute instruction placed on databus */
     J&=255;
     ICount-=CyclesMain[J];
     (*(FnMain[J]))();
    }
  }
 }
}

#ifdef TRACE
static unsigned pc_trace[256];
static unsigned pc_count=0;
#endif

word Z80(reg *Regs)
{
 unsigned opcode;
 R=*Regs;
 CPURunning=1;
 InitTables ();
 do
 {
#ifdef TRACE
  pc_trace[pc_count]=R.PC.D;
  pc_count=(pc_count+1)&255;
#endif
#ifdef DEBUG
  if(R.PC.D==Trap) Trace=1;  /*** Turn tracing on if trapped ***/
  if(Trace) Debug(&R);       /*** Call single-step debugger  ***/
  if (!CPURunning) break;
#endif
  ++R.R;
  opcode=M_RDMEM_OPCODE();
  ICount-=CyclesMain[opcode];
  (*(FnMain[opcode]))();
  if (ICount<=0)
  {
   if (!CPURunning)
    break;
   ICount+=IPeriod;
   Z80_Interrupt (Interrupt());
  }
 }
 while (1);
 return(R.PC.W.l);
}

void Z80_RegisterDump (void)
{
 int i;
 printf
 (
   "AF:%04X HL:%04X DE:%04X BC:%04X PC:%04X SP:%04X IX:%04X IY:%04X\n",
   R.AF.W.l,R.HL.W.l,R.DE.W.l,R.BC.W.l,R.PC.W.l,R.SP.W.l,R.IX.W.l,R.IY.W.l
 ); 
 printf ("STACK: ");
 for (i=0;i<10;++i) printf ("%04X ",M_RDMEM_WORD((R.SP.D+i*2)&0xFFFF));
 puts ("");
#ifdef TRACE
 puts ("PC TRACE:");
 for (i=1;i<=256;++i) printf ("%04X\n",pc_trace[(pc_count-i)&255]);
#endif
}
