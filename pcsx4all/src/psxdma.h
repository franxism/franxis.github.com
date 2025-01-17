/***************************************************************************
 *   Copyright (C) 2007 Ryan Schultz, PCSX-df Team, PCSX team              *
 *   schultz.ryan@gmail.com, http://rschultz.ath.cx/code.php               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Steet, Fifth Floor, Boston, MA 02111-1307 USA.            *
 ***************************************************************************/

#ifndef __PSXDMA_H__
#define __PSXDMA_H__

#include "psxcommon.h"
#include "r3000a.h"
#include "psxhw.h"
#include "psxmem.h"

// CHUI: A�ado ResetIoCycle para permite que en el proximo salto entre en psxBranchTest
#define GPUDMA_INT(eCycle) { \
	ResetIoCycle(); \
	psxRegs.interrupt |= 0x01000000; \
	psxRegs.intCycle[3+24+1] = eCycle; \
	psxRegs.intCycle[3+24] = psxRegs.cycle; \
}

// CHUI: A�ado ResetIoCycle para permite que en el proximo salto entre en psxBranchTest
#define SPUDMA_INT(eCycle) { \
    ResetIoCycle(); \
    psxRegs.interrupt |= 0x04000000; \
    psxRegs.intCycle[1 + 24 + 1] = eCycle; \
    psxRegs.intCycle[1 + 24] = psxRegs.cycle; \
}

// CHUI: A�ado ResetIoCycle para permite que en el proximo salto entre en psxBranchTest
#define MDECOUTDMA_INT(eCycle) { \
	ResetIoCycle(); \
	psxRegs.interrupt |= 0x02000000; \
	psxRegs.intCycle[5+24+1] = eCycle; \
	psxRegs.intCycle[5+24] = psxRegs.cycle; \
}

extern void psxDma2(u32 madr, u32 bcr, u32 chcr);
extern void psxDma3(u32 madr, u32 bcr, u32 chcr);
extern void psxDma4(u32 madr, u32 bcr, u32 chcr);
extern void psxDma6(u32 madr, u32 bcr, u32 chcr);
extern void gpuInterrupt(void);
extern void spuInterrupt(void);

#endif /* __PSXDMA_H__ */
