/*****************************************************************************
 *
 *	 m6502ops.h
 *	 Addressing mode and opcode macros for 6502,65c02,6510 CPUs
 *
 *	 Copyright (c) 1998 Juergen Buchmueller, all rights reserved.
 *
 *	 - This source code is released as freeware for non-commercial purposes.
 *	 - You are free to use and redistribute this code in modified or
 *	   unmodified form, provided you list me in the credits.
 *	 - If you modify this source code, you must add a notice to each modified
 *	   source file that it has been changed.  If you're a nice person, you
 *	   will clearly mark each change too.  :)
 *	 - If you wish to use this for commercial purposes, please contact me at
 *	   pullmoll@t-online.de
 *	 - The author of this copywritten work reserves the right to change the
 *     terms of its usage and license at any time, including retroactively
 *   - This entire notice must remain in the source code.
 *
 *****************************************************************************/

/* 6502 flags */
#define _fC 0x01
#define _fZ 0x02
#define _fI 0x04
#define _fD 0x08
#define _fB 0x10
#define _fT 0x20
#define _fV 0x40
#define _fN 0x80

/* some shortcuts for improved readability */
#define A	m6502.a
#define X	m6502.x
#define Y	m6502.y
#define P	m6502.p
#define S	m6502.sp.B.l
#define SPD m6502.sp.D

#if LAZY_FLAGS

#define NZ  m6502.nz

#define SET_NZ(n)   NZ = (((n) & _fN) << 8) | (n)
#define SET_Z(n)	NZ = (NZ & 0x100) | (n)

#else

#define SET_NZ(n)				\
	P = (P & ~(_fN | _fZ)) |  ((n) & _fN) | (((n) == 0) ? _fZ : 0)
#define SET_Z(n)				\
    P = (P & ~_fZ) | ((n) == 0) ? _fZ : 0)

#endif

#define EAL m6502.ea.B.l
#define EAH m6502.ea.B.h
#define EAW m6502.ea.W.l
#define EAD m6502.ea.D

#define ZPL m6502.zp.B.l
#define ZPH m6502.zp.B.h
#define ZPW m6502.zp.W.l
#define ZPD m6502.zp.D

#define PCL m6502.pc.B.l
#define PCH m6502.pc.B.h
#define PCW m6502.pc.W.l
#define PCD m6502.pc.D

#if FAST_MEMORY
extern  MHELE   *cur_mwhard;
extern	MHELE	*cur_mrhard;
extern	byte	*RAM;
#endif

/***************************************************************
 *  RDOP    read an opcode
 ***************************************************************/
#define RDOP() cpu_readop(PCW++)

/***************************************************************
 *  RDOPARG read an opcode argument
 ***************************************************************/
#define RDOPARG() cpu_readop_arg(PCW++)

/***************************************************************
 *  RDMEM   read memory
 ***************************************************************/
#if FAST_MEMORY
#define RDMEM(addr)                                             \
	((cur_mrhard[(addr) >> (ABITS2_16 + ABITS_MIN_16)]) ?		\
		cpu_readmem16(addr) : RAM[addr])
#else
#define RDMEM(addr) cpu_readmem16(addr)
#endif

/***************************************************************
 *  WRMEM   write memory
 ***************************************************************/
#if FAST_MEMORY
#define WRMEM(addr,data)                                        \
	if (cur_mwhard[(addr) >> (ABITS2_16 + ABITS_MIN_16)])		\
		cpu_writemem16(addr,data);								\
	else														\
		RAM[addr] = data
#else
#define WRMEM(addr,data) cpu_writemem16(addr,data)
#endif

/***************************************************************
 *	BRA  branch relative
 *	extra cycle if page boundary is crossed
 ***************************************************************/
#define BRA(cond)                                               \
	if (cond)													\
	{															\
		tmp = RDOPARG();										\
		EAW = PCW + (signed char)tmp;							\
		M6502_ICount -= (PCH == EAH) ? 3 : 4;					\
		PCD = EAD;												\
		change_pc16(PCD);										\
	}															\
	else														\
	{															\
		PCW++;													\
		M6502_ICount -= 2;										\
	}

/***************************************************************
 *
 * Helper macros to build the effective address
 *
 ***************************************************************/

/***************************************************************
 *  EA = zero page address
 ***************************************************************/
#define EA_ZPG													\
	ZPL = RDOPARG();											\
	EAD = ZPD

/***************************************************************
 *  EA = zero page address + X
 ***************************************************************/
#define EA_ZPX													\
	ZPL = RDOPARG() + X;										\
	EAD = ZPD

/***************************************************************
 *  EA = zero page address + Y
 ***************************************************************/
#define EA_ZPY													\
	ZPL = RDOPARG() + Y;										\
	EAD = ZPD

/***************************************************************
 *  EA = absolute address
 ***************************************************************/
#define EA_ABS													\
	EAL = RDOPARG();											\
	EAH = RDOPARG()

/***************************************************************
 *  EA = absolute address + X
 ***************************************************************/
#define EA_ABX													\
    EA_ABS;                                                     \
    EAW += X

/***************************************************************
 *	EA = absolute address + Y
 ***************************************************************/
#define EA_ABY                                                  \
	EA_ABS; 													\
	EAW += Y

/***************************************************************
 *	EA = zero page indirect (65c02 pre indexed w/o X)
 ***************************************************************/
#define EA_ZPI													\
	ZPL = RDOPARG();											\
	EAL = RDMEM(ZPD);											\
	ZPL++;														\
	EAH = RDMEM(ZPD)

/***************************************************************
 *  EA = zero page + X indirect (pre indexed)
 ***************************************************************/
#define EA_IDX													\
	ZPL = RDOPARG() + X;										\
	EAL = RDMEM(ZPD);											\
	ZPL++;														\
    EAH = RDMEM(ZPD)

/***************************************************************
 *  EA = zero page indirect + Y (post indexed)
 *	subtract 1 cycle if page boundary is crossed
 ***************************************************************/
#define EA_IDY													\
	ZPL = RDOPARG();											\
	EAL = RDMEM(ZPD);											\
	ZPL++;														\
	EAH = RDMEM(ZPD);											\
    if (EAL + Y > 0xff)                                         \
		M6502_ICount--; 										\
	EAW += Y

/***************************************************************
 *	EA = indirect (only used by JMP)
 ***************************************************************/
#define EA_IND													\
	EA_ABS; 													\
	tmp = RDMEM(EAD);											\
	EAL++;	/* booby trap: stay in same page! ;-) */			\
	EAH = RDMEM(EAD);											\
	EAL = tmp

/***************************************************************
 *	EA = indirect plus x (only used by 65c02 JMP)
 ***************************************************************/
#define EA_IAX                                                  \
	EA_IND; 													\
	if (EAL + X > 0xff) /* assumption; probably wrong ? */		\
		M6502_ICount--; 										\
    EAW += X

/* read a value into tmp */
#define RD_IMM	tmp = RDOPARG()
#define RD_ACC	tmp = A
#define RD_ZPG	EA_ZPG; tmp = RDMEM(EAD)
#define RD_ZPX	EA_ZPX; tmp = RDMEM(EAD)
#define RD_ZPY	EA_ZPY; tmp = RDMEM(EAD)
#define RD_ABS	EA_ABS; tmp = RDMEM(EAD)
#define RD_ABX	EA_ABX; tmp = RDMEM(EAD)
#define RD_ABY	EA_ABY; tmp = RDMEM(EAD)
#define RD_ZPI	EA_ZPI; tmp = RDMEM(EAD)
#define RD_IDX	EA_IDX; tmp = RDMEM(EAD)
#define RD_IDY	EA_IDY; tmp = RDMEM(EAD)

/* write a value from tmp */
#define WR_ZPG	EA_ZPG; WRMEM(EAD, tmp)
#define WR_ZPX	EA_ZPX; WRMEM(EAD, tmp)
#define WR_ZPY	EA_ZPY; WRMEM(EAD, tmp)
#define WR_ABS	EA_ABS; WRMEM(EAD, tmp)
#define WR_ABX	EA_ABX; WRMEM(EAD, tmp)
#define WR_ABY	EA_ABY; WRMEM(EAD, tmp)
#define WR_ZPI	EA_ZPI; WRMEM(EAD, tmp)
#define WR_IDX	EA_IDX; WRMEM(EAD, tmp)
#define WR_IDY	EA_IDY; WRMEM(EAD, tmp)

/* write back a value from tmp to the last EA */
#define WB_ACC	A = (byte)tmp;
#define WB_EA	WRMEM(EAD, tmp)

/***************************************************************
 ***************************************************************
 *			Macros to emulate the plain 6502 opcodes
 ***************************************************************
 ***************************************************************/

/***************************************************************
 * compose the real flag register by
 * including N and Z and set any
 * SET and clear any CLR bits also
 ***************************************************************/
#if LAZY_FLAGS

#define COMPOSE_P												\
	P = (P & ~(_fN|_fZ)) | (NZ >> 8) | ((NZ&0xff) ? 0:_fZ)

#else

#define COMPOSE_P

#endif

/***************************************************************
 * push a register onto the stack
 ***************************************************************/
#define PUSH(Rg) WRMEM(SPD, Rg); S--

/***************************************************************
 * pull a register from the stack
 ***************************************************************/
#define PULL(Rg) S++; Rg = RDMEM(SPD)

/* 6502 ********************************************************
 *	ADC Add with carry
 ***************************************************************/
#define ADC 													\
	if (P & _fD)												\
	{															\
	int c = (P & _fC);											\
	int lo = (A & 0x0f) + (tmp & 0x0f) + c; 					\
	int hi = (A & 0xf0) + (tmp & 0xf0); 						\
		P &= ~(_fV | _fC);										\
		if (lo > 0x09)											\
		{														\
			hi += 0x10; 										\
			lo += 0x06; 										\
		}														\
		if (~(A^tmp) & (A^hi) & _fN)							\
			P |= _fV;											\
		if (hi > 0x90)											\
			hi += 0x60; 										\
		if (hi & 0xff00)										\
			P |= _fC;											\
		A = (lo & 0x0f) + (hi & 0xf0);							\
	}															\
	else														\
	{															\
	int c = (P & _fC);											\
	int sum = A + tmp + c;										\
		P &= ~(_fV | _fC);										\
		if (~(A^tmp) & (A^sum) & _fN)							\
			P |= _fV;											\
		if (sum & 0xff00)										\
			P |= _fC;											\
		A = (byte) sum; 										\
	}															\
	SET_NZ(A)

/* 6502 ********************************************************
 *	AND Logical and
 ***************************************************************/
#define AND 													\
	A = (byte)(A & tmp);										\
	SET_NZ(A)

/* 6502 ********************************************************
 *	ASL Arithmetic shift left
 ***************************************************************/
#define ASL 													\
	P = (P & ~_fC) | ((tmp >> 7) & _fC);						\
	tmp = (byte)(tmp << 1); 									\
	SET_NZ(tmp)

/* 6502 ********************************************************
 *	BCC Branch if carry clear
 ***************************************************************/
#define BCC BRA(!(P & _fC))

/* 6502 ********************************************************
 *	BCS Branch if carry set
 ***************************************************************/
#define BCS BRA(P & _fC)

/* 6502 ********************************************************
 *	BEQ Branch if equal
 ***************************************************************/
#if LAZY_FLAGS
#define BEQ BRA(!(NZ & 0xff))
#else
#define BEQ BRA(P & _fZ)
#endif

/* 6502 ********************************************************
 *	BIT Bit test
 ***************************************************************/
#if LAZY_FLAGS

#define BIT 													\
	P = (P & ~_fV) | (tmp & _fV);								\
	NZ = (tmp & _fN) << 8;										\
	NZ |= tmp & A /* mask to lazy Z */

#else

#define BIT 													\
	P &= ~(_fN|_fV|_fZ);										\
	P |= tmp & (_fN|_fV);										\
	if ((tmp & A) == 0) 										\
		P |= _fZ

#endif

/* 6502 ********************************************************
 *	BMI Branch if minus
 ***************************************************************/
#if LAZY_FLAGS
#define BMI BRA(NZ & 0x8000)
#else
#define BMI BRA(P & _fN)
#endif

/* 6502 ********************************************************
 *	BNE Branch if not equal
 ***************************************************************/
#if LAZY_FLAGS
#define BNE BRA(NZ & 0xff)
#else
#define BNE BRA(!(P & _fZ))
#endif

/* 6502 ********************************************************
 *	BPL Branch if plus
 ***************************************************************/
#if LAZY_FLAGS
#define BPL BRA(!(NZ & 0x8000))
#else
#define BPL BRA(!(P & _fN))
#endif

/* 6502 ********************************************************
 *	BRK Break
 *	increment PC, push PC hi, PC lo, flags (with B bit set),
 *	set I flag, reset D flag and jump via IRQ vector
 ***************************************************************/
#define BRK 													\
	PCW++;														\
	PUSH(PCH);													\
	PUSH(PCL);													\
	COMPOSE_P;													\
	PUSH(P | _fB);												\
	P = (P | _fI) & ~_fD;										\
	PCL = RDMEM(M6502_IRQ_VEC); 								\
	PCH = RDMEM(M6502_IRQ_VEC+1);								\
	change_pc16(PCD)

/* 6502 ********************************************************
 * BVC	Branch if overflow clear
 ***************************************************************/
#define BVC BRA(!(P & _fV))

/* 6502 ********************************************************
 * BVS	Branch if overflow set
 ***************************************************************/
#define BVS BRA(P & _fV)

/* 6502 ********************************************************
 * CLC	Clear carry flag
 ***************************************************************/
#define CLC 													\
	P &= ~_fC

/* 6502 ********************************************************
 * CLD	Clear decimal flag
 ***************************************************************/
#define CLD 													\
	P &= ~_fD

/* 6502 ********************************************************
 * CLI	Clear interrupt flag
 ***************************************************************/
#define CLI 													\
	if (m6502.pending_irq && (P & _fI)) 						\
		m6502.after_cli = 1;									\
	P &= ~_fI

/* 6502 ********************************************************
 * CLV	Clear overflow flag
 ***************************************************************/
#define CLV 													\
	P &= ~_fV

/* 6502 ********************************************************
 *	CMP Compare accumulator
 ***************************************************************/
#define CMP 													\
	P &= ~_fC;													\
	if (A >= tmp)												\
		P |= _fC;												\
	SET_NZ((byte)(A - tmp))

/* 6502 ********************************************************
 *	CPX Compare index X
 ***************************************************************/
#define CPX 													\
	P &= ~_fC;													\
	if (X >= tmp)												\
		P |= _fC;												\
	SET_NZ((byte)(X - tmp))

/* 6502 ********************************************************
 *	CPY Compare index Y
 ***************************************************************/
#define CPY 													\
	P &= ~_fC;													\
	if (Y >= tmp)												\
		P |= _fC;												\
	SET_NZ((byte)(Y - tmp))

/* 6502 ********************************************************
 *	DEC Decrement memory
 ***************************************************************/
#define DEC 													\
	tmp = (byte)--tmp;											\
	SET_NZ(tmp)

/* 6502 ********************************************************
 *	DEX Decrement index X
 ***************************************************************/
#define DEX 													\
	X = (byte)--X;												\
	SET_NZ(X)

/* 6502 ********************************************************
 *	DEY Decrement index Y
 ***************************************************************/
#define DEY 													\
	Y = (byte)--Y;												\
	SET_NZ(Y)

/* 6502 ********************************************************
 *	EOR Logical exclusive or
 ***************************************************************/
#define EOR 													\
	A = (byte)(A ^ tmp);										\
	SET_NZ(A)

/* 6502 ********************************************************
 *	ILL Illegal opcode
 ***************************************************************/
#define ILL 													\
	if (0)												\
		printf("M6502 illegal opcode %04x: %02x\n",  \
			(PCW-1)&0xffff, cpu_readop((PCW-1)&0xffff))

/* 6502 ********************************************************
 *	INC Increment memory
 ***************************************************************/
#define INC 													\
	tmp = (byte)++tmp;											\
	SET_NZ(tmp)

/* 6502 ********************************************************
 *	INX Increment index X
 ***************************************************************/
#define INX 													\
	X = (byte)++X;												\
	SET_NZ(X)

/* 6502 ********************************************************
 *	INY Increment index Y
 ***************************************************************/
#define INY 													\
	Y = (byte)++Y;												\
	SET_NZ(Y)

/* 6502 ********************************************************
 *	JMP Jump to address
 *	set PC to the effective address
 ***************************************************************/
#define JMP 													\
	PCD = EAD;													\
	change_pc16(PCD)

/* 6502 ********************************************************
 *	JSR Jump to subroutine
 *	decrement PC (sic!) push PC hi, push PC lo and set
 *	PC to the effective address
 ***************************************************************/
#define JSR 													\
	EAL = RDOPARG();											\
	PUSH(PCH);													\
	PUSH(PCL);													\
	EAH = RDOPARG();											\
	PCD = EAD;													\
	change_pc16(PCD)

/* 6502 ********************************************************
 *	LDA Load accumulator
 ***************************************************************/
#define LDA 													\
	A = (byte)tmp;												\
	SET_NZ(A)

/* 6502 ********************************************************
 *	LDX Load index X
 ***************************************************************/
#define LDX 													\
	X = (byte)tmp;												\
	SET_NZ(X)

/* 6502 ********************************************************
 *	LDY Load index Y
 ***************************************************************/
#define LDY 													\
	Y = (byte)tmp;												\
	SET_NZ(Y)

/* 6502 ********************************************************
 *	LSR Logic shift right
 *	0 -> [7][6][5][4][3][2][1][0] -> C
 ***************************************************************/
#define LSR 													\
	P = (P & ~_fC) | (tmp & _fC);								\
	tmp = (byte)tmp >> 1;										\
	SET_NZ(tmp)

/* 6502 ********************************************************
 *	NOP No operation
 ***************************************************************/
#define NOP

/* 6502 ********************************************************
 *	ORA Logical inclusive or
 ***************************************************************/
#define ORA 													\
	A = (byte)(A | tmp);										\
	SET_NZ(A)

/* 6502 ********************************************************
 *	PHA Push accumulator
 ***************************************************************/
#define PHA 													\
	PUSH(A)

/* 6502 ********************************************************
 *	PHP Push processor status (flags)
 ***************************************************************/
#define PHP 													\
	COMPOSE_P;													\
	PUSH(P)

/* 6502 ********************************************************
 *	PLA Pull accumulator
 ***************************************************************/
#define PLA 													\
	PULL(A);													\
	SET_NZ(A)


/* 6502 ********************************************************
 *	PLP Pull processor status (flags)
 ***************************************************************/
#if LAZY_FLAGS

#define PLP 													\
	if (P & _fI)												\
	{															\
		PULL(P);												\
		if (m6502.pending_irq && !(P & _fI))					\
			m6502.after_cli = 1;								\
	}															\
	else														\
	{															\
		PULL(P);												\
	}															\
	P |= _fT;													\
	NZ = ((P & _fN) << 8) | ((P & _fZ) ^ _fZ)

#else

#define PLP 													\
	if (P & _fI)												\
	{															\
		PULL(P);												\
		if (m6502.pending_irq && !(P & _fI))					\
			m6502.after_cli = 1;								\
	}															\
	else														\
	{															\
		PULL(P);												\
	}															\
	P |= _fT

#endif

/* 6502 ********************************************************
 * ROL	Rotate left
 *	new C <- [7][6][5][4][3][2][1][0] <- C
 ***************************************************************/
#define ROL 													\
	tmp = (tmp << 1) | (P & _fC);								\
	P = (P & ~_fC) | ((tmp >> 8) & _fC);						\
	tmp = (byte)tmp;											\
	SET_NZ(tmp)

/* 6502 ********************************************************
 * ROR	Rotate right
 *	C -> [7][6][5][4][3][2][1][0] -> new C
 ***************************************************************/
#define ROR 													\
	tmp |= (P & _fC) << 8;										\
	P = (P & ~_fC) | (tmp & _fC);								\
	tmp = (byte)(tmp >> 1); 									\
	SET_NZ(tmp)

/* 6502 ********************************************************
 * RTI	Return from interrupt
 * pull flags, pull PC lo, pull PC hi and increment PC
 *	PCW++;
 ***************************************************************/
#if LAZY_FLAGS

#define RTI 													\
	PULL(P);													\
	PULL(PCL);													\
    PULL(PCH);                                                  \
    P |= _fT;                                                   \
	NZ = ((P & _fN) << 8) | ((P & _fZ) ^ _fZ);					\
	if (m6502.pending_irq && !(P & _fI))						\
		m6502.after_cli = 1;									\
	change_pc16(PCD)

#else

#define RTI 													\
	PULL(P);													\
	PULL(PCL);													\
    PULL(PCH);                                                  \
    P |= _fT;                                                   \
    if (m6502.pending_irq && !(P & _fI))                        \
		m6502.after_cli = 1;									\
    change_pc16(PCD)

#endif

/* 6502 ********************************************************
 *	RTS Return from subroutine
 *	pull PC lo, PC hi and increment PC
 ***************************************************************/
#define RTS 													\
	PULL(PCL);													\
	PULL(PCH);													\
	PCW++;														\
	change_pc16(PCD)

/* 6502 ********************************************************
 *	SBC Subtract with carry
 ***************************************************************/
#define SBC 													\
	if (P & _fD)												\
	{															\
	int c = (P & _fC) ^ _fC;									\
	int sum = A - tmp - c;										\
	int lo = (A & 0x0f) - (tmp & 0x0f) - c; 					\
	int hi = (A & 0xf0) - (tmp & 0xf0); 						\
		P &= ~(_fV | _fC);										\
		if ((A^tmp) & (A^sum) & _fN)							\
			P |= _fV;											\
		if (lo & 0xf0)											\
			lo -= 6;											\
		if (lo & 0x80)											\
			hi -= 0x10; 										\
		if (hi & 0x0f00)										\
			hi -= 0x60; 										\
		if ((sum & 0xff00) == 0)								\
			P |= _fC;											\
		A = (lo & 0x0f) + (hi & 0xf0);							\
	}															\
	else														\
	{															\
	int c = (P & _fC) ^ _fC;									\
	int sum = A - tmp - c;										\
		P &= ~(_fV | _fC);										\
		if ((A^tmp) & (A^sum) & _fN)							\
			P |= _fV;											\
		if ((sum & 0xff00) == 0)								\
			P |= _fC;											\
		A = (byte) sum; 										\
	}															\
	SET_NZ(A)

/* 6502 ********************************************************
 *	SEC Set carry flag
 ***************************************************************/
#define SEC 													\
	P |= _fC

/* 6502 ********************************************************
 *	SED Set decimal flag
 ***************************************************************/
#define SED 													\
	P |= _fD

/* 6502 ********************************************************
 *	SEI Set interrupt flag
 ***************************************************************/
#define SEI 													\
	P |= _fI

/* 6502 ********************************************************
 * STA	Store accumulator
 ***************************************************************/
#define STA 													\
	tmp = A

/* 6502 ********************************************************
 * STX	Store index X
 ***************************************************************/
#define STX 													\
	tmp = X

/* 6502 ********************************************************
 * STY	Store index Y
 ***************************************************************/
#define STY 													\
	tmp = Y

/* 6502 ********************************************************
 * TAX	Transfer accumulator to index X
 ***************************************************************/
#define TAX 													\
	X = A;														\
	SET_NZ(X)

/* 6502 ********************************************************
 * TAY	Transfer accumulator to index Y
 ***************************************************************/
#define TAY 													\
	Y = A;														\
	SET_NZ(Y)

/* 6502 ********************************************************
 * TSX	Transfer stack LSB to index X
 ***************************************************************/
#define TSX 													\
	X = S;														\
	SET_NZ(X)

/* 6502 ********************************************************
 * TXA	Transfer index X to accumulator
 ***************************************************************/
#define TXA 													\
	A = X;														\
	SET_NZ(A)

/* 6502 ********************************************************
 * TXS	Transfer index X to stack LSB
 * no flags changed (sic!)
 ***************************************************************/
#define TXS 													\
	S = X

/* 6502 ********************************************************
 * TYA	Transfer index Y to accumulator
 ***************************************************************/
#define TYA 													\
	A = Y;														\
	SET_NZ(A)

/***************************************************************
 ***************************************************************
 *			Macros to emulate the 65C02 opcodes
 ***************************************************************
 ***************************************************************/

/* 65C02 *******************************************************
 *	BBR Branch if bit is reset
 ***************************************************************/
#define BBR(bit)												\
	BRA(!(tmp & (1<<bit)))

/* 65C02 *******************************************************
 *	BBS Branch if bit is set
 ***************************************************************/
#define BBS(bit)												\
	BRA(tmp & (1<<bit))

/* 65C02 *******************************************************
 *	DEA Decrement accumulator
 ***************************************************************/
#define DEA 													\
	A = (byte)--A;												\
	SET_NZ(A)

/* 65C02 *******************************************************
 *	INA Increment accumulator
 ***************************************************************/
#define INA 													\
	A = (byte)++A;												\
	SET_NZ(A)

/* 65C02 *******************************************************
 *	PHX Push index X
 ***************************************************************/
#define PHX 													\
	PUSH(X)

/* 65C02 *******************************************************
 *	PHY Push index Y
 ***************************************************************/
#define PHY 													\
	PUSH(Y)

/* 65C02 *******************************************************
 *	PLX Pull index X
 ***************************************************************/
#define PLX 													\
	PULL(X)

/* 65C02 *******************************************************
 *	PLY Pull index Y
 ***************************************************************/
#define PLY 													\
	PULL(Y)

/* 65C02 *******************************************************
 *	RMB Reset memory bit
 ***************************************************************/
#define RMB(bit)												\
	tmp &= ~(1<<bit)

/* 65C02 *******************************************************
 *	SMB Set memory bit
 ***************************************************************/
#define SMB(bit)												\
	tmp |= (1<<bit)

/* 65C02 *******************************************************
 * STZ	Store zero
 ***************************************************************/
#define STZ                                                     \
    tmp = 0

/* 65C02 *******************************************************
 * TRB	Test and reset bits
 ***************************************************************/
#define TRB                                                     \
	SET_Z(tmp&A);												\
	tmp &= ~A

/* 65C02 *******************************************************
 * TSB	Test and set bits
 ***************************************************************/
#define TSB 													\
	SET_NZ(tmp&A);												\
	tmp |= A

/***************************************************************
 ***************************************************************
 *			Macros to emulate the 6510 opcodes
 ***************************************************************
 ***************************************************************/

/* 6510 ********************************************************
 *	ANC logical and, set carry from bit of A
 ***************************************************************/
#define ANC 													\
	P &= ~_fC;													\
	A = (byte)(A & tmp);										\
	if (A & 0x80)												\
		P |= _fC;												\
	SET_NZ(A)

/* 6510 ********************************************************
 *	ASR logical and, logical shift right
 ***************************************************************/
#define ASR 													\
	tmp = (byte)(A & tmp);										\
	LSR

/* 6510 ********************************************************
 * AST	and stack; transfer to accumulator and index X
 * logical and stack (LSB) with data, transfer result to S
 * transfer result to accumulator and index X also
 ***************************************************************/
#define AST 													\
	S &= tmp;													\
	A = X = S;													\
	SET_NZ(A)

/* 6510 ********************************************************
 *	ARR logical and, rotate right
 ***************************************************************/
#define ARR 													\
	tmp = (byte)(A & tmp);										\
	ROR

/* 6510 ********************************************************
 *	ASX logical and X w/ A, subtract data from X
 ***************************************************************/
#define ASX 													\
	P &= ~_fC;													\
	X &= A; 													\
	if (X >= tmp)												\
		P |= _fC;												\
	X = (byte)(X - tmp);										\
	SET_NZ(X)

/* 6510 ********************************************************
 *	AXA transfer index X to accumulator, logical and
 ***************************************************************/
#define AXA 													\
	tmp = (byte)(X & tmp);										\
	SET_NZ(tmp)

/* 6510 ********************************************************
 *	DCP decrement data and compare
 ***************************************************************/
#define DCP 													\
	tmp = (byte)--tmp;											\
	P &= ~_fC;													\
	if (A >= tmp)												\
		P |= _fC;												\
    SET_NZ((byte)(A - tmp))

/* 6502 ********************************************************
 *	DOP double no operation
 ***************************************************************/
#define DOP 													\
	PCW++

/* 6510 ********************************************************
 *	ISB increment and subtract with carry
 ***************************************************************/
#define ISB 													\
	tmp = (byte)++tmp;											\
    SBC

/* 6510 ********************************************************
 *  LAX load accumulator and index X
 ***************************************************************/
#define LAX 													\
	A = X = (byte)tmp;											\
	SET_NZ(A)

/* 6510 ********************************************************
 * RLA	rotate left and logical and accumulator
 *	new C <- [7][6][5][4][3][2][1][0] <- C
 ***************************************************************/
#define RLA 													\
	tmp = (tmp << 1) | (P & _fC);								\
	P = (P & ~_fC) | ((tmp >> 8) & _fC);						\
	tmp = (byte)tmp;											\
	A &= tmp;													\
    SET_NZ(A)

/* 6510 ********************************************************
 * RRA	rotate right and add with carry
 *  C -> [7][6][5][4][3][2][1][0] -> C
 ***************************************************************/
#define RRA 													\
	tmp |= (P & _fC) << 8;										\
	P = (P & ~_fC) | (tmp & _fC);								\
	tmp = (byte)(tmp >> 1); 									\
    ADC

/* 6510 ********************************************************
 * SAX	logical and accumulator with index X and store
 ***************************************************************/
#define SAX 													\
	tmp = A & X;												\
	SET_NZ(tmp)

/* 6510 ********************************************************
 *	SLO shift left and logical or
 ***************************************************************/
#define SLO 													\
	P = (P & ~_fC) | ((tmp >> 7) & _fC);						\
	tmp = (byte)(tmp << 1); 									\
	A |= tmp;													\
    SET_NZ(A)

/* 6510 ********************************************************
 *	SRE logical shift right and logical exclusive or
 *  0 -> [7][6][5][4][3][2][1][0] -> C
 ***************************************************************/
#define SRE 													\
	P = (P & ~_fC) | (tmp & _fC);								\
	tmp = (byte)tmp >> 1;										\
	A ^= tmp;													\
    SET_NZ(A)

/* 6510 ********************************************************
 * SAH	store accumulator and index X and high + 1
 * result = accumulator and index X and memory [PC+1] + 1
 ***************************************************************/
#define SAH 													\
	tmp = A & X;												\
	tmp &= (cpu_readop_arg((PCW + 1) & 0xffff) + 1)

/* 6510 ********************************************************
 * SSH	store stack high
 * logical and accumulator with index X, transfer result to S
 * logical and result with memory [PC+1] + 1
 ***************************************************************/
#define SSH 													\
	tmp = S = A & X;											\
	tmp &= (byte)(cpu_readop_arg((PCW + 1) & 0xffff) + 1)

/* 6510 ********************************************************
 * SXH	store index X high
 * logical and index X with memory[PC+1] and store the result
 ***************************************************************/
#define SXH 													\
	tmp = X & (byte)(cpu_readop_arg((PCW + 1) & 0xffff)

/* 6510 ********************************************************
 * SYH	store index Y and (high + 1)
 * logical and index Y with memory[PC+1] + 1 and store the result
 ***************************************************************/
#define SYH 													\
	tmp = Y & (byte)(cpu_readop_arg((PCW + 1) & 0xffff) + 1)

/* 6510 ********************************************************
 *	TOP triple no operation
 ***************************************************************/
#define TOP 													\
	PCW += 2

