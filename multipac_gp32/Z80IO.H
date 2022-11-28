/** Z80: portable Z80 emulator **********************************************/
/**                                                                        **/
/**                                Z80IO.h                                 **/
/**                                                                        **/
/** This file contains the memory and I/O read and write function          **/
/** prototypes. They can be replaced by inline functions or macros to      **/
/** speed up emulation                                                     **/
/**                                                                        **/
/** Copyright (C) Marat Fayzullin 1994,1995,1996                           **/
/**               Marcel de Kogel 1996                                     **/
/**     You are not allowed to distribute this software commercially       **/
/**     Please, notify me, if you make any changes to this file            **/
/****************************************************************************/

/****************************************************************************/
/*** These are called on IN and OUT commands                              ***/
/****************************************************************************/
byte DoIn (byte Port);
void DoOut (byte Port,byte Value);

/****************************************************************************/
/*** These functions are called when read or write to memory occurs       ***/
/****************************************************************************/
byte M_RDMEM (dword A);
void M_WRMEM (dword A,byte V);
/* Used by PUSH and POP instructions */
/*#define M_RDMEM_FAST(A)        M_RDMEM(A)*/
/*#define M_WRMEM_FAST(A,V)      M_WRMEM(A,V)*/
INLINE byte M_RDMEM_FAST (dword A)
{
 extern byte RAM[];
 return RAM[A];
}
INLINE byte M_WRMEM_FAST (dword A,byte V)
{
 extern byte RAM[];
 RAM[A] = V;
}
/* Get next opcode and increment program counter */
INLINE byte M_RDMEM_OPCODE (void)
{
 byte retval;
 retval=M_RDMEM_FAST(R.PC.D);
 R.PC.W.l++;
 return retval;
}
