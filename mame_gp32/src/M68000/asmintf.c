/*
	Interface routine for 68kem <-> Mame

*/

#include "driver.h"
#include "M68000.H"

#ifdef WIN32
#define CONVENTION __cdecl
#else
#define CONVENTION
#endif

extern void CONVENTION M68KRUN(void);
extern void CONVENTION M68KRESET(void);


/********************************************/
/* Interface routines to link Mame -> 68KEM */
/********************************************/

void MC68000_Reset(void)
{
	gm_memset(&regs,0,sizeof(regs));

    regs.a[7] = regs.isp = get_long(0);
    regs.pc   = get_long(4) & 0xffffff;
    regs.sr_high = 0x27;
   	regs.sr = 0x2700;

    M68KRESET();
}


int  MC68000_Execute(int cycles)
{
	if (regs.IRQ_level == 0x80) return cycles;		/* STOP with no IRQs */

  	MC68000_ICount = cycles;

	M68KRUN();

    return (cycles - MC68000_ICount);
}


static regstruct Myregs;

void MC68000_SetRegs(MC68000_Regs *src)
{
 	regs = src->regs;
}

void MC68000_GetRegs(MC68000_Regs *dst)
{
  	dst->regs = regs;
}

void MC68000_Cause_Interrupt(int level)
{
	if (level >= 1 && level <= 7)
        regs.IRQ_level |= 1 << (level-1);
}

void MC68000_Clear_Pending_Interrupts(void)
{
	regs.IRQ_level = 0;
}

int  MC68000_GetPC(void)
{
    return regs.pc;
}

