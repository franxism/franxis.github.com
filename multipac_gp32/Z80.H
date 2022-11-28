#ifndef __z80_h__
#define __z80_h__

/** Z80: portable Z80 emulator **********************************************/
/**                                                                        **/
/**                                 Z80.h                                  **/
/**                                                                        **/
/** This file contains the main Z80 emulation function prototypes and      **/
/** definitions                                                            **/
/**                                                                        **/
/** Copyright (C) Marat Fayzullin 1994,1995,1996                           **/
/**               Marcel de Kogel 1996                                     **/
/**     You are not allowed to distribute this software commercially       **/
/**     Please, notify me, if you make any changes to this file            **/
/****************************************************************************/

/****************************************************************************/
/** Machine dependent definitions                                          **/
/****************************************************************************/
/* #define DEBUG */                   /* Compile debugging version          */
/* #define LSB_FIRST */               /* Compile for low-endian CPU         */
/* #define X86_ASM */                 /* Use inline X86 assembly            */
/* #define __64BIT__ */               /* Compile for 64 bit machines        */
/* #define __128BIT__ */              /* Compile for 128 bit machines       */

/* How to define inline functions inside header files */
#define INLINE  extern __inline__
/* Pass parameters in registers to functions declared as FASTCALL */
#define FASTCALL __attribute__ ((regparm(3)))

/* sizeof(byte)=1, sizeof(word)=2, sizeof(dword)>=4 */
typedef unsigned char byte;
typedef unsigned short word;
typedef unsigned dword;
typedef signed char offset;

/* define a Z80 word. Higher bytes are always zero */
typedef union
{
#ifdef __128BIT__
 #ifdef LSB_FIRST
   struct { byte l,h,h2,h3,h4,h5,h6,h7,
                 h8,h9,h10,h11,h12,h13,h14,h15; } B;
   struct { word l,h,h2,h3,h4,h5,h6,h7; } W;
   dword D;
 #else
   struct { byte h15,h14,h13,h12,h11,h10,h9,h8,
                 h7,h6,h5,h4,h3,h2,h,l; } B;
   struct { word h7,h6,h5,h4,h3,h2,h,l; } W;
   dword D;
 #endif
#elif __64BIT__
 #ifdef LSB_FIRST
   struct { byte l,h,h2,h3,h4,h5,h6,h7; } B;
   struct { word l,h,h2,h3; } W;
   dword D;
 #else
   struct { byte h7,h6,h5,h4,h3,h2,h,l; } B;
   struct { word h3,h2,h,l; } W;
   dword D;
 #endif
#else
 #ifdef LSB_FIRST
   struct { byte l,h,h2,h3; } B;
   struct { word l,h; } W;
   dword D;
 #else
   struct { byte h3,h2,h,l; } B;
   struct { word h,l; } W;
   dword D;
 #endif
#endif
} pair;

/****************************************************************************/
/** End of machine dependent definitions                                   **/
/****************************************************************************/

#define S_FLAG      0x80
#define Z_FLAG      0x40
#define H_FLAG      0x10
#define P_FLAG      0x04
#define V_FLAG      0x04
#define N_FLAG      0x02
#define C_FLAG      0x01

/** Trace and Trap **********************************************************/
/** Switches to turn tracing on and off in DEBUG mode                      **/
/****************************************************************************/
#ifdef DEBUG
extern int  Trace;                    /* Tracing is on if Trace==1          */
extern int  Trap;                     /* When PC==Trap, set Trace=1         */
#endif

/** TrapBadOps **************************************************************/
/** When 1, print warnings of illegal Z80 instructions                     **/
/****************************************************************************/
extern byte TrapBadOps;

/** CPURunning **************************************************************/
/** When 0, execution terminates                                           **/
/****************************************************************************/
extern byte CPURunning;

typedef struct
{
  pair AF,BC,DE,HL,IX,IY,PC,SP;
  pair AF1,BC1,DE1,HL1;
  unsigned IFF,I,R,R2;
} reg;

extern reg R;

/** Interpret Z80 code: *****************************************************/
/** RAM starts at Addr and registers have initial values from Regs. PC     **/
/** value at which emulation stopped is returned by this function          **/
/****************************************************************************/
void ResetZ80 (reg *Regs);            /* Reset all registers                */
word Z80(reg *Regs);                  /* Start Z80 emulation                */
void Patch (reg *Regs);               /* Called when ED FE occurs           */
void Z80_RegisterDump (void);         /* Prints a dump to stdout            */

#ifdef DEBUG
/** Single-step debugger ****************************************************/
/** This function should exist if DEBUG is #defined. If Trace==1 it is     **/
/** called after each command executed by the CPU and given address of     **/
/** the address space and the register file                                **/
/****************************************************************************/
void Debug(reg *R);
#endif

/** Interrupt handler *******************************************************/
/** This function is called on each attempted interrupt and should return  **/
/** an interrupt address to proceed with interrupt or IGNORE_INT to        **/
/** continue execution                                                     **/ 
/****************************************************************************/
int Interrupt(void);

/** Interrupt handler *******************************************************/
/** This function is called when EI is executed and should return an       **/
/** interrupt address to proceed with interrupt or IGNORE_INT to continue  **/
/** execution                                                              **/ 
/****************************************************************************/
int InterruptsEnabled(void);

extern int IPeriod;                   /* Number of T-states per interrupt   */
extern int ICount;                    /* T-state count                      */
#define IGNORE_INT      -1            /* Ignore interrupt                   */
#define NMI_INT         -2            /* Execute NMI                        */

/* definitions of functions to read/write memory and I/O ports */
#include "Z80IO.h"

/* some handy macros */
INLINE unsigned M_RDMEM_WORD (dword A)
{
 int i;
 i=M_RDMEM (A);
 i+=M_RDMEM ((A+1)&0xFFFF)*256;
 return i;
}

INLINE void M_WRMEM_WORD (dword A,word V)
{
 M_WRMEM (A,V&255);
 M_WRMEM ((A+1)&0xFFFF,V>>8);
}

#include "Z80Codes.h"

#endif /*__z80_h__*/
