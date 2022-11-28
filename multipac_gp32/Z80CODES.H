/** Z80: portable Z80 emulator **********************************************/
/**                                                                        **/
/**                              Z80Codes.h                                **/
/**                                                                        **/
/** This file contains the macros that are used by the various opcode      **/
/** emulations                                                             **/
/**                                                                        **/
/** Copyright (C) Marat Fayzullin 1994,1995,1996                           **/
/**               Marcel de Kogel 1996                                     **/
/**     You are not allowed to distribute this software commercially       **/
/**     Please, notify me, if you make any changes to this file            **/
/****************************************************************************/

#define S(Fl)        R.AF.B.l|=Fl
#define R(Fl)        R.AF.B.l&=~(Fl)
#define FLAGS(Rg,Fl) R.AF.B.l=Fl|(Rg? 0:Z_FLAG)|(Rg&S_FLAG)

extern byte PTable[512];
extern byte ZSTable[512];
extern byte ZSPTable[512];


#define M_RLC(Rg)      \
  R.AF.B.l=(Rg&0x80)>>7;Rg=(Rg<<1)|R.AF.B.l; \
  R.AF.B.l|=ZSPTable[Rg]
#define M_RRC(Rg)      \
  R.AF.B.l=Rg&0x01;Rg=(Rg>>1)|(R.AF.B.l<<7); \
  R.AF.B.l|=ZSPTable[Rg]
#define M_RL(Rg)       \
{                      \
 byte Q;               \
 Q=(Rg&0x80)>>7;       \
 Rg=(Rg<<1)|(R.AF.B.l&C_FLAG); \
 R.AF.B.l=ZSPTable[Rg]|Q; \
}
#define M_RR(Rg)       \
{                      \
 byte Q;               \
 Q=(Rg&0x01);          \
 Rg=(Rg>>1)|((R.AF.B.l&C_FLAG)<<7); \
 R.AF.B.l=ZSPTable[Rg]|Q; \
}
  
#define M_SLA(Rg)      \
  R.AF.B.l=(Rg&0x80)>>7;Rg<<=1;              \
  R.AF.B.l|=ZSPTable[Rg]
#define M_SRA(Rg)      \
  R.AF.B.l=Rg&C_FLAG;Rg=(Rg>>1)|(Rg&0x80);        \
  R.AF.B.l|=ZSPTable[Rg]

#define M_SLL(Rg)      \
  R.AF.B.l=(Rg&0x80)>>7;Rg=(Rg<<1)|0x01;     \
  R.AF.B.l|=ZSPTable[Rg]
#define M_SRL(Rg)      \
  R.AF.B.l=Rg&0x01;Rg>>=1;                        \
  R.AF.B.l|=ZSPTable[Rg]

#define M_BIT(Bit,Rg)  \
  R.AF.B.l=(R.AF.B.l&~(N_FLAG|Z_FLAG))|H_FLAG|(Rg&(1<<Bit)? 0:Z_FLAG)

#define M_SET(Bit,Rg) Rg|=1<<Bit
#define M_RES(Bit,Rg) Rg&=~(1<<Bit)

#define M_POP(Rg)      \
  R.Rg.B.l=M_RDMEM_FAST(R.SP.D);R.SP.W.l++; \
  R.Rg.B.h=M_RDMEM_FAST(R.SP.D);R.SP.W.l++;
#define M_PUSH(Rg)     \
  R.SP.W.l--;M_WRMEM_FAST(R.SP.D,R.Rg.B.h); \
  R.SP.W.l--;M_WRMEM_FAST(R.SP.D,R.Rg.B.l)

#define M_CALL         \
{ pair Q;              \
  Q.B.l=M_RDMEM_OPCODE();          \
  Q.B.h=M_RDMEM_OPCODE();          \
  R.SP.W.l--;M_WRMEM_FAST(R.SP.D,R.PC.B.h); \
  R.SP.W.l--;M_WRMEM_FAST(R.SP.D,R.PC.B.l); \
  R.PC.W.l=Q.W.l;   \
}

#define M_JP           \
{ pair Q;      \
  Q.B.l=M_RDMEM_OPCODE();Q.B.h=M_RDMEM_OPCODE();R.PC.W.l=Q.W.l;     \
}

#define M_JR  R.PC.W.l+=(offset)M_RDMEM_OPCODE()

#define M_RET          \
  R.PC.B.l=M_RDMEM_FAST(R.SP.D);R.SP.W.l++; \
  R.PC.B.h=M_RDMEM_FAST(R.SP.D);R.SP.W.l++;

#define M_RST(Ad)      \
  R.SP.W.l--;M_WRMEM_FAST(R.SP.D,R.PC.B.h); \
  R.SP.W.l--;M_WRMEM_FAST(R.SP.D,R.PC.B.l); \
  R.PC.W.l=Ad

#define M_LDWORD(Rg)   \
  R.Rg.B.l=M_RDMEM_OPCODE();R.Rg.B.h=M_RDMEM_OPCODE()

INLINE void M_ADD (byte Rg)
{
#ifdef X86_ASM
  asm (
  " movb %2,%%al        \n"
  " addb %%al,%0        \n"
  " lahf                \n"
  " seto %%al           \n"
  " shlb $2,%%al        \n"
  " andb $0xD1,%%ah     \n"
  " orb %%ah,%%al       \n"
  " movb %%al,%1        \n"
  :"=g" (R.AF.B.h),
   "=g" (R.AF.B.l)
  :"g" (Rg),
   "g" (R.AF.B.h)
  :"eax"
  );
#else
  pair J;
  J.D=R.AF.B.h+Rg;
  R.AF.B.l=
    ((~(R.AF.B.h^Rg)&(Rg^J.B.l)&0x80)>>5)|
    ZSTable[J.D]|
    ((R.AF.B.h^Rg^J.B.l)&H_FLAG);
  R.AF.B.h=J.B.l;
#endif
}

INLINE void M_SUB (byte Rg)
{
#ifdef X86_ASM
  asm (
  " movb %2,%%al        \n"
  " subb %%al,%0        \n"
  " lahf                \n"
  " seto %%al           \n"
  " shlb $2,%%al        \n"
  " andb $0xD1,%%ah     \n"
  " orb %%ah,%%al       \n"
  " orb $2,%%al         \n"
  " movb %%al,%1        \n"
  :"=g" (R.AF.B.h),
   "=g" (R.AF.B.l)
  :"g" (Rg),
   "g" (R.AF.B.h)
  :"eax"
  );
#else
  pair J;
  J.D=R.AF.B.h-Rg;
  R.AF.B.l=
    (((R.AF.B.h^Rg)&(R.AF.B.h^J.B.l)&0x80)>>5)|
    N_FLAG|-J.B.h|ZSTable[J.B.l]|
    ((R.AF.B.h^Rg^J.B.l)&H_FLAG);
  R.AF.B.h=J.B.l;
#endif
}

INLINE void M_ADC (byte Rg)
{
#ifdef X86_ASM
  asm (
  " movb %3,%%al        \n"
  " shrb $1,%%al        \n"
  " movb %2,%%al        \n"
  " adcb %%al,%0        \n"
  " lahf                \n"
  " seto %%al           \n"
  " shlb $2,%%al        \n"
  " andb $0xD1,%%ah     \n"
  " orb %%ah,%%al       \n"
  " movb %%al,%1        \n"
  :"=g" (R.AF.B.h),
   "=g" (R.AF.B.l)
  :"g" (Rg),
   "g" (R.AF.B.l),
   "g" (R.AF.B.h)
  :"eax"
  );
#else
  pair J;
  J.D=R.AF.B.h+Rg+(R.AF.B.l&C_FLAG);
  R.AF.B.l=
    ((~(R.AF.B.h^Rg)&(Rg^J.B.l)&0x80)>>5)|
    ZSTable[J.D]|
    ((R.AF.B.h^Rg^J.B.l)&H_FLAG);
  R.AF.B.h=J.B.l;
#endif
}

INLINE void M_SBC (byte Rg)
{
#ifdef X86_ASM
  asm (
  " movb %3,%%al        \n"
  " shrb $1,%%al        \n"
  " movb %2,%%al        \n"
  " sbbb %%al,%0        \n"
  " lahf                \n"
  " seto %%al           \n"
  " shlb $2,%%al        \n"
  " andb $0xD1,%%ah     \n"
  " orb %%ah,%%al       \n"
  " orb $2,%%al         \n"
  " movb %%al,%1        \n"
  :"=g" (R.AF.B.h),
   "=g" (R.AF.B.l)
  :"g" (Rg),
   "g" (R.AF.B.l),
   "g" (R.AF.B.h)
  :"eax"
  );
#else
  pair J;
  J.D=R.AF.B.h-Rg-(R.AF.B.l&C_FLAG);
  R.AF.B.l=
    (((R.AF.B.h^Rg)&(R.AF.B.h^J.B.l)&0x80)>>5)|
    N_FLAG|-J.B.h|ZSTable[J.B.l]|
    ((R.AF.B.h^Rg^J.B.l)&H_FLAG);
  R.AF.B.h=J.B.l;
#endif
}

INLINE void M_CP (byte Rg)
{
#ifdef X86_ASM
  asm (
  " movb %2,%%al        \n"
  " cmpb %%al,%0        \n"
  " lahf                \n"
  " seto %%al           \n"
  " shlb $2,%%al        \n"
  " andb $0xD1,%%ah     \n"
  " orb %%ah,%%al       \n"
  " orb $2,%%al         \n"
  " movb %%al,%1        \n"
  :"=g" (R.AF.B.h),
   "=g" (R.AF.B.l)
  :"g" (Rg),
   "g" (R.AF.B.l),
   "g" (R.AF.B.h)
  :"eax"
  );
#else
  pair J;
  J.D=R.AF.B.h-Rg;
  R.AF.B.l=
    (((R.AF.B.h^Rg)&(R.AF.B.h^J.B.l)&0x80)>>5)|
    N_FLAG|-J.B.h|ZSTable[J.B.l]|
    ((R.AF.B.h^Rg^J.B.l)&H_FLAG);
#endif
}

#define M_AND(Rg)       \
  R.AF.B.h&=Rg;         \
  R.AF.B.l=H_FLAG|ZSPTable[R.AF.B.h]

#define M_OR(Rg)        \
  R.AF.B.h|=Rg;         \
  R.AF.B.l=ZSPTable[R.AF.B.h]

#define M_XOR(Rg)       \
  R.AF.B.h^=Rg;         \
  R.AF.B.l=ZSPTable[R.AF.B.h]

#ifdef X86_ASM
 #define M_INC(Rg)              \
 asm (                          \
    " movb %3,%%al      \n"     \
    " shrb $1,%%al      \n"     \
    " incb %0           \n"     \
    " lahf              \n"     \
    " seto %%al         \n"     \
    " shlb $2,%%al      \n"     \
    " andb $0xD1,%%ah   \n"     \
    " orb %%ah,%%al     \n"     \
    " movb %%al,%1      \n"     \
    :"=g" (Rg),                 \
     "=g" (R.AF.B.l)            \
    :"g" (Rg),                  \
     "g" (R.AF.B.l)             \
    :"eax"                      \
    );
#else
  #define M_INC(Rg)             \
  Rg++;                         \
  R.AF.B.l=                     \
    (R.AF.B.l&C_FLAG)|ZSTable[Rg]| \
    (Rg==0x80? V_FLAG:0)|(Rg&0x0F? 0:H_FLAG);
#endif

#ifdef X86_ASM
 #define M_DEC(Rg)              \
 asm (                          \
    " movb %3,%%al      \n"     \
    " shrb $1,%%al      \n"     \
    " decb %0           \n"     \
    " lahf              \n"     \
    " seto %%al         \n"     \
    " shlb $2,%%al      \n"     \
    " andb $0xD1,%%ah   \n"     \
    " orb %%ah,%%al     \n"     \
    " orb $2,%%al       \n"     \
    " movb %%al,%1      \n"     \
    :"=g" (Rg),                 \
     "=g" (R.AF.B.l)            \
    :"g" (Rg),                  \
     "g" (R.AF.B.l)             \
    :"eax"                      \
    );
#else
  #define M_DEC(Rg)             \
  Rg--;                         \
  R.AF.B.l=                     \
    N_FLAG|(R.AF.B.l&C_FLAG)|ZSTable[Rg]| \
    (Rg==0x7F? V_FLAG:0)|((Rg&0x0F)==0x0F? H_FLAG:0);
#endif

#define M_ADDW(Rg1,Rg2)                                \
{ pair Q;                                              \
  Q.D=(R.Rg1.D+R.Rg2.D);                               \
  R.AF.B.l=                                            \
    (R.AF.B.l&~(N_FLAG|C_FLAG|H_FLAG))|                \
    ((R.Rg1.B.h^R.Rg2.B.h^Q.B.h)&H_FLAG)|              \
    (Q.B.h2);                                          \
  R.Rg1.W.l=Q.W.l; }                                       \

#define M_ADCW(Rg)      \
{ pair Q;       \
  Q.D=(R.HL.D+R.Rg.D+(R.AF.D&C_FLAG));                     \
  R.AF.B.l=                                                \
    (Q.B.h2)|                                              \
    (~(R.HL.D^R.Rg.D)&(R.Rg.D^Q.D)&0x8000? V_FLAG:0)|      \
    ((R.HL.B.h^R.Rg.B.h^Q.B.h)&H_FLAG)|                    \
    (Q.W.l? 0:Z_FLAG)|(Q.B.h&S_FLAG);                        \
  R.HL.W.l=Q.W.l;   \
}

#define M_SBCW(Rg)      \
{ pair Q;       \
  Q.D=(R.HL.D-R.Rg.D-(R.AF.D&C_FLAG));                       \
  R.AF.B.l=                                                  \
    N_FLAG|                                                  \
    (Q.B.h2&C_FLAG)|                                         \
    ((R.HL.D^R.Rg.D)&(R.HL.D^Q.D)&0x8000? V_FLAG:0)|         \
    ((R.HL.B.h^R.Rg.B.h^Q.B.h)&H_FLAG)|                      \
    (Q.W.l? 0:Z_FLAG)|(Q.B.h&S_FLAG);                          \
  R.HL.W.l=Q.W.l;   \
}

#define M_IN(Rg)        \
  Rg=DoIn(R.BC.B.l);    \
  R.AF.B.l=ZSPTable[Rg]|(R.AF.B.l&C_FLAG)

enum Codes
{
  NOP,LD_BC_WORD,LD_xBC_A,INC_BC,INC_B,DEC_B,LD_B_BYTE,RLCA,
  EX_AF_AF,ADD_HL_BC,LD_A_xBC,DEC_BC,INC_C,DEC_C,LD_C_BYTE,RRCA,
  DJNZ,LD_DE_WORD,LD_xDE_A,INC_DE,INC_D,DEC_D,LD_D_BYTE,RLA,
  JR,ADD_HL_DE,LD_A_xDE,DEC_DE,INC_E,DEC_E,LD_E_BYTE,RRA,
  JR_NZ,LD_HL_WORD,LD_xWORD_HL,INC_HL,INC_H,DEC_H,LD_H_BYTE,DAA,
  JR_Z,ADD_HL_HL,LD_HL_xWORD,DEC_HL,INC_L,DEC_L,LD_L_BYTE,CPL,
  JR_NC,LD_SP_WORD,LD_xWORD_A,INC_SP,INC_xHL,DEC_xHL,LD_xHL_BYTE,SCF,
  JR_C,ADD_HL_SP,LD_A_xWORD,DEC_SP,INC_A,DEC_A,LD_A_BYTE,CCF,
  LD_B_B,LD_B_C,LD_B_D,LD_B_E,LD_B_H,LD_B_L,LD_B_xHL,LD_B_A,
  LD_C_B,LD_C_C,LD_C_D,LD_C_E,LD_C_H,LD_C_L,LD_C_xHL,LD_C_A,
  LD_D_B,LD_D_C,LD_D_D,LD_D_E,LD_D_H,LD_D_L,LD_D_xHL,LD_D_A,
  LD_E_B,LD_E_C,LD_E_D,LD_E_E,LD_E_H,LD_E_L,LD_E_xHL,LD_E_A,
  LD_H_B,LD_H_C,LD_H_D,LD_H_E,LD_H_H,LD_H_L,LD_H_xHL,LD_H_A,
  LD_L_B,LD_L_C,LD_L_D,LD_L_E,LD_L_H,LD_L_L,LD_L_xHL,LD_L_A,
  LD_xHL_B,LD_xHL_C,LD_xHL_D,LD_xHL_E,LD_xHL_H,LD_xHL_L,HALT,LD_xHL_A,
  LD_A_B,LD_A_C,LD_A_D,LD_A_E,LD_A_H,LD_A_L,LD_A_xHL,LD_A_A,
  ADD_B,ADD_C,ADD_D,ADD_E,ADD_H,ADD_L,ADD_xHL,ADD_A,
  ADC_B,ADC_C,ADC_D,ADC_E,ADC_H,ADC_L,ADC_xHL,ADC_A,
  SUB_B,SUB_C,SUB_D,SUB_E,SUB_H,SUB_L,SUB_xHL,SUB_A,
  SBC_B,SBC_C,SBC_D,SBC_E,SBC_H,SBC_L,SBC_xHL,SBC_A,
  AND_B,AND_C,AND_D,AND_E,AND_H,AND_L,AND_xHL,AND_A,
  XOR_B,XOR_C,XOR_D,XOR_E,XOR_H,XOR_L,XOR_xHL,XOR_A,
  OR_B,OR_C,OR_D,OR_E,OR_H,OR_L,OR_xHL,OR_A,
  CP_B,CP_C,CP_D,CP_E,CP_H,CP_L,CP_xHL,CP_A,
  RET_NZ,POP_BC,JP_NZ,JP,CALL_NZ,PUSH_BC,ADD_BYTE,RST00,
  RET_Z,RET,JP_Z,PFX_CB,CALL_Z,CALL,ADC_BYTE,RST08,
  RET_NC,POP_DE,JP_NC,OUTA,CALL_NC,PUSH_DE,SUB_BYTE,RST10,
  RET_C,EXX,JP_C,INA,CALL_C,PFX_DD,SBC_BYTE,RST18,
  RET_PO,POP_HL,JP_PO,EX_HL_xSP,CALL_PO,PUSH_HL,AND_BYTE,RST20,
  RET_PE,LD_PC_HL,JP_PE,EX_DE_HL,CALL_PE,PFX_ED,XOR_BYTE,RST28,
  RET_P,POP_AF,JP_P,DI,CALL_P,PUSH_AF,OR_BYTE,RST30,
  RET_M,LD_SP_HL,JP_M,EI,CALL_M,PFX_FD,CP_BYTE,RST38
};

enum CodesCB
{
  RLC_B,RLC_C,RLC_D,RLC_E,RLC_H,RLC_L,RLC_xHL,RLC_A,
  RRC_B,RRC_C,RRC_D,RRC_E,RRC_H,RRC_L,RRC_xHL,RRC_A,
  RL_B,RL_C,RL_D,RL_E,RL_H,RL_L,RL_xHL,RL_A,
  RR_B,RR_C,RR_D,RR_E,RR_H,RR_L,RR_xHL,RR_A,
  SLA_B,SLA_C,SLA_D,SLA_E,SLA_H,SLA_L,SLA_xHL,SLA_A,
  SRA_B,SRA_C,SRA_D,SRA_E,SRA_H,SRA_L,SRA_xHL,SRA_A,
  SLL_B,SLL_C,SLL_D,SLL_E,SLL_H,SLL_L,SLL_xHL,SLL_A,
  SRL_B,SRL_C,SRL_D,SRL_E,SRL_H,SRL_L,SRL_xHL,SRL_A,
  BIT0_B,BIT0_C,BIT0_D,BIT0_E,BIT0_H,BIT0_L,BIT0_xHL,BIT0_A,
  BIT1_B,BIT1_C,BIT1_D,BIT1_E,BIT1_H,BIT1_L,BIT1_xHL,BIT1_A,
  BIT2_B,BIT2_C,BIT2_D,BIT2_E,BIT2_H,BIT2_L,BIT2_xHL,BIT2_A,
  BIT3_B,BIT3_C,BIT3_D,BIT3_E,BIT3_H,BIT3_L,BIT3_xHL,BIT3_A,
  BIT4_B,BIT4_C,BIT4_D,BIT4_E,BIT4_H,BIT4_L,BIT4_xHL,BIT4_A,
  BIT5_B,BIT5_C,BIT5_D,BIT5_E,BIT5_H,BIT5_L,BIT5_xHL,BIT5_A,
  BIT6_B,BIT6_C,BIT6_D,BIT6_E,BIT6_H,BIT6_L,BIT6_xHL,BIT6_A,
  BIT7_B,BIT7_C,BIT7_D,BIT7_E,BIT7_H,BIT7_L,BIT7_xHL,BIT7_A,
  RES0_B,RES0_C,RES0_D,RES0_E,RES0_H,RES0_L,RES0_xHL,RES0_A,
  RES1_B,RES1_C,RES1_D,RES1_E,RES1_H,RES1_L,RES1_xHL,RES1_A,
  RES2_B,RES2_C,RES2_D,RES2_E,RES2_H,RES2_L,RES2_xHL,RES2_A,
  RES3_B,RES3_C,RES3_D,RES3_E,RES3_H,RES3_L,RES3_xHL,RES3_A,
  RES4_B,RES4_C,RES4_D,RES4_E,RES4_H,RES4_L,RES4_xHL,RES4_A,
  RES5_B,RES5_C,RES5_D,RES5_E,RES5_H,RES5_L,RES5_xHL,RES5_A,
  RES6_B,RES6_C,RES6_D,RES6_E,RES6_H,RES6_L,RES6_xHL,RES6_A,
  RES7_B,RES7_C,RES7_D,RES7_E,RES7_H,RES7_L,RES7_xHL,RES7_A,  
  SET0_B,SET0_C,SET0_D,SET0_E,SET0_H,SET0_L,SET0_xHL,SET0_A,
  SET1_B,SET1_C,SET1_D,SET1_E,SET1_H,SET1_L,SET1_xHL,SET1_A,
  SET2_B,SET2_C,SET2_D,SET2_E,SET2_H,SET2_L,SET2_xHL,SET2_A,
  SET3_B,SET3_C,SET3_D,SET3_E,SET3_H,SET3_L,SET3_xHL,SET3_A,
  SET4_B,SET4_C,SET4_D,SET4_E,SET4_H,SET4_L,SET4_xHL,SET4_A,
  SET5_B,SET5_C,SET5_D,SET5_E,SET5_H,SET5_L,SET5_xHL,SET5_A,
  SET6_B,SET6_C,SET6_D,SET6_E,SET6_H,SET6_L,SET6_xHL,SET6_A,
  SET7_B,SET7_C,SET7_D,SET7_E,SET7_H,SET7_L,SET7_xHL,SET7_A
};
  
enum CodesED
{
  FUCK00,FUCK01,FUCK02,FUCK03,FUCK04,FUCK05,FUCK06,FUCK07,
  FUCK08,FUCK09,FUCK0A,FUCK0B,FUCK0C,FUCK0D,FUCK0E,FUCK0F,
  FUCK10,FUCK11,FUCK12,FUCK13,FUCK14,FUCK15,FUCK16,FUCK17,
  FUCK18,FUCK19,FUCK1A,FUCK1B,FUCK1C,FUCK1D,FUCK1E,FUCK1F,
  FUCK20,FUCK21,FUCK22,FUCK23,FUCK24,FUCK25,FUCK26,FUCK27,
  FUCK28,FUCK29,FUCK2A,FUCK2B,FUCK2C,FUCK2D,FUCK2E,FUCK2F,
  FUCK30,FUCK31,FUCK32,FUCK33,FUCK34,FUCK35,FUCK36,FUCK37,
  FUCK38,FUCK39,FUCK3A,FUCK3B,FUCK3C,FUCK3D,FUCK3E,FUCK3F,
  IN_B_xC,OUT_xC_B,SBC_HL_BC,LD_xWORDe_BC,NEG,RETN,IM_0,LD_I_A,
  IN_C_xC,OUT_xC_C,ADC_HL_BC,LD_BC_xWORDe,FUCK4C,RETI,FUCK,LD_R_A,
  IN_D_xC,OUT_xC_D,SBC_HL_DE,LD_xWORDe_DE,FUCK54,FUCK55,IM_1,LD_A_I,
  IN_E_xC,OUT_xC_E,ADC_HL_DE,LD_DE_xWORDe,FUCK5C,FUCK5D,IM_2,LD_A_R,
  IN_H_xC,OUT_xC_H,SBC_HL_HL,LD_xWORDe_HL,FUCK64,FUCK65,FUCK66,RRD,
  IN_L_xC,OUT_xC_L,ADC_HL_HL,LD_HL_xWORDe,FUCK6C,FUCK6D,FUCK6E,RLD,
  IN_F_xC,FUCK71,SBC_HL_SP,LD_xWORDe_SP,FUCK74,FUCK75,FUCK76,FUCK77,
  IN_A_xC,OUT_xC_A,ADC_HL_SP,LD_SP_xWORDe,FUCK7C,FUCK7D,FUCK7E,FUCK7F,
  FUCK80,FUCK81,FUCK82,FUCK83,FUCK84,FUCK85,FUCK86,FUCK87,
  FUCK88,FUCK89,FUCK8A,FUCK8B,FUCK8C,FUCK8D,FUCK8E,FUCK8F,
  FUCK90,FUCK91,FUCK92,FUCK93,FUCK94,FUCK95,FUCK96,FUCK97,
  FUCK98,FUCK99,FUCK9A,FUCK9B,FUCK9C,FUCK9D,FUCK9E,FUCK9F,
  LDI,CPI,INI,OUTI,FUCKA4,FUCKA5,FUCKA6,FUCKA7,
  LDD,CPD,IND,OUTD,FUCKAC,FUCKAD,FUCKAE,FUCKAF,
  LDIR,CPIR,INIR,OTIR,FUCKB4,FUCKB5,FUCKB6,FUCKB7,
  LDDR,CPDR,INDR,OTDR,FUCKBC,FUCKBD,FUCKBE,FUCKBF,
  FUCKC0,FUCKC1,FUCKC2,FUCKC3,FUCKC4,FUCKC5,FUCKC6,FUCKC7,
  FUCKC8,FUCKC9,FUCKCA,FUCKCB,FUCKCC,FUCKCD,FUCKCE,FUCKCF,
  FUCKD0,FUCKD1,FUCKD2,FUCKD3,FUCKD4,FUCKD5,FUCKD6,FUCKD7,
  FUCKD8,FUCKD9,FUCKDA,FUCKDB,FUCKDC,FUCKDD,FUCKDE,FUCKDF,
  FUCKE0,FUCKE1,FUCKE2,FUCKE3,FUCKE4,FUCKE5,FUCKE6,FUCKE7,
  FUCKE8,FUCKE9,FUCKEA,FUCKEB,FUCKEC,FUCKED,FUCKEE,FUCKEF,
  FUCKF0,FUCKF1,FUCKF2,FUCKF3,FUCKF4,FUCKF5,FUCKF6,FUCKF7,
  FUCKF8,FUCKF9,FUCKFA,FUCKFB,FUCKFC,FUCKFD,FUCKFE,FUCKFF
};

