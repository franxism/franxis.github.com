/***************************************************************************
 *   Copyright (C) 2010 by Blade_Arma                                      *
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
 *   51 Franklin Street, Fifth Floor, Boston, MA 02111-1307 USA.           *
 ***************************************************************************/

/*
 * Internal PSX counters.
 */

#include "psxcounters.h"
#include "profiler.h"

/******************************************************************************/

enum
{
    Rc0Gate           = 0x0001, // 0    not implemented
    Rc1Gate           = 0x0001, // 0    not implemented
    Rc2Disable        = 0x0001, // 0    partially implemented
    RcUnknown1        = 0x0002, // 1    ?
    RcUnknown2        = 0x0004, // 2    ?
    RcCountToTarget   = 0x0008, // 3
    RcIrqOnTarget     = 0x0010, // 4
    RcIrqOnOverflow   = 0x0020, // 5
    RcIrqRegenerate   = 0x0040, // 6
    RcUnknown7        = 0x0080, // 7    ?
    Rc0PixelClock     = 0x0100, // 8    fake implementation
    Rc1HSyncClock     = 0x0100, // 8
    Rc2Unknown8       = 0x0100, // 8    ?
    Rc0Unknown9       = 0x0200, // 9    ?
    Rc1Unknown9       = 0x0200, // 9    ?
    Rc2OneEighthClock = 0x0200, // 9
    RcUnknown10       = 0x0400, // 10   ?
    RcCountEqTarget   = 0x0800, // 11
    RcOverflow        = 0x1000, // 12
    RcUnknown13       = 0x2000, // 13   ? (always zero)
    RcUnknown14       = 0x4000, // 14   ? (always zero)
    RcUnknown15       = 0x8000, // 15   ? (always zero)
};

#define CounterQuantity           ( 4 )
//static const u32 CounterQuantity  = 4;

static const u32 CountToOverflow  = 0;
static const u32 CountToTarget    = 1;

static const u32 FrameRate[]      = { 60, 50 };
static const u32 VBlankStart[]    = { 240, 256 };
static const u32 HSyncTotal[]     = { 262, 312 };
#define SPU_UPD_INTERVAL 23

/******************************************************************************/

Rcnt rcnts[ CounterQuantity ];

static u32 hSyncCount = 0;
static u32 spuSyncCount = 0;

u32 psxNextCounter = 0, psxNextsCounter = 0;

/******************************************************************************/

INLINE void setIrq( u32 irq )
{
	psxHu32ref(0x1070) |= SWAPu32(irq);
// CHUI: A�ado ResetIoCycle para permite que en el proximo salto entre en psxBranchTest
    	ResetIoCycle();
}

/******************************************************************************/

INLINE void _psxRcntWcount( u32 index, u32 value )
{
    if( value > 0xffff )
    {
#ifdef DEBUG_BIOS
        dbgf("[RCNT %i] wcount > 0xffff: %x\n", index, value );
#endif
        value &= 0xffff;
    }

    rcnts[index].cycleStart  = psxRegs.cycle;
    rcnts[index].cycleStart -= value * rcnts[index].rate;

    // TODO: <=.
    if( value < rcnts[index].target )
    {
        rcnts[index].cycle = rcnts[index].target * rcnts[index].rate;
        rcnts[index].counterState = CountToTarget;
    }
    else
    {
        rcnts[index].cycle = 0xffff * rcnts[index].rate;
        rcnts[index].counterState = CountToOverflow;
    }
}

INLINE u32 _psxRcntRcount( u32 index )
{
    u32 count;

    count  = psxRegs.cycle;
    count -= rcnts[index].cycleStart;
    if (rcnts[index].rate > 1)
    	count = UDIV(count,rcnts[index].rate);

    if( count > 0xffff )
    {
#ifdef DEBUG_BIOS
        dbgf("[RCNT %i] rcount > 0xffff: %x\n", index, count );
#endif
        count &= 0xffff;
    }

    return count;
}

/******************************************************************************/

static void psxRcntSet(void)
{
    s32 countToUpdate;
    u32 i;

    psxNextsCounter = psxRegs.cycle;
    psxNextCounter  = 0x7fffffff;

#ifdef USE_EXTRA_IO_CYCLES
// CHUI: A�ado ResetIoCycle para permite que en el proximo salto entre en psxBranchTest
    ResetIoCycle();
#endif
    for( i = 0; i < CounterQuantity; ++i )
    {
        countToUpdate = rcnts[i].cycle - (psxNextsCounter - rcnts[i].cycleStart);

        if( countToUpdate < 0 )
        {
            psxNextCounter = 0;
            break;
        }

        if( countToUpdate < (s32)psxNextCounter )
        {
            psxNextCounter = countToUpdate;
        }
    }
#ifndef USE_EXTRA_IO_CYCLES
    {
	const u32 value = psxRegs.cycle+psxNextCounter;
	if (psxRegs.io_cycle_counter>value)
		psxRegs.io_cycle_counter=value;
    }
#endif
}

/******************************************************************************/

static void psxRcntReset( u32 index )
{
    u32 count;

    if( rcnts[index].counterState == CountToTarget )
    {
        if( rcnts[index].mode & RcCountToTarget )
        {
            count  = psxRegs.cycle;
            count -= rcnts[index].cycleStart;
            if (rcnts[index].rate > 1)
				count = UDIV(count,rcnts[index].rate);
            count -= rcnts[index].target;
        }
        else
        {
            count = _psxRcntRcount( index );
        }

        _psxRcntWcount( index, count );

        if( rcnts[index].mode & RcIrqOnTarget )
        {
            if( (rcnts[index].mode & RcIrqRegenerate) || (!rcnts[index].irqState) )
            {
#ifdef DEBUG_BIOS
                dbgf("[RCNT %i] irq: %x\n", index, count );
#endif
                setIrq( rcnts[index].irq );
                rcnts[index].irqState = 1;
            }
        }

        rcnts[index].mode |= RcCountEqTarget;
    }
    else if( rcnts[index].counterState == CountToOverflow )
    {
        count  = psxRegs.cycle;
        count -= rcnts[index].cycleStart;
        if (rcnts[index].rate > 1)
			count = UDIV(count,rcnts[index].rate);
        count -= 0xffff;

        _psxRcntWcount( index, count );

        if( rcnts[index].mode & RcIrqOnOverflow )
        {
            if( (rcnts[index].mode & RcIrqRegenerate) || (!rcnts[index].irqState) )
            {
#ifdef DEBUG_BIOS
                dbgf("[RCNT %i] irq: %x\n", index, count );
#endif
                setIrq( rcnts[index].irq );
                rcnts[index].irqState = 1;
            }
        }

        rcnts[index].mode |= RcOverflow;
    }

    rcnts[index].mode |= RcUnknown10;

    psxRcntSet();
}

void psxRcntUpdate()
{
#ifdef DEBUG_ANALYSIS
	dbg_anacnt_psxRcntUpdate++;
#endif
    pcsx4all_prof_start_with_pause(PCSX4ALL_PROF_COUNTERS,PCSX4ALL_PROF_CPU);
    u32 cycle;

    cycle = psxRegs.cycle;

#ifdef USE_EXTRA_IO_CYCLES
// CHUI: A�ado ResetIoCycle para permite que en el proximo salto entre en psxBranchTest
    ResetIoCycle();
#endif
    // rcnt 0.
    if( cycle - rcnts[0].cycleStart >= rcnts[0].cycle )
    {
        psxRcntReset( 0 );
    }

    // rcnt 1.
    if( cycle - rcnts[1].cycleStart >= rcnts[1].cycle )
    {
        psxRcntReset( 1 );
    }

    // rcnt 2.
    if( cycle - rcnts[2].cycleStart >= rcnts[2].cycle )
    {
        psxRcntReset( 2 );
    }

    // rcnt base.
    if( cycle - rcnts[3].cycleStart >= rcnts[3].cycle )
    {
        psxRcntReset( 3 );

        spuSyncCount++;
        hSyncCount++;

        // Update spu.
        if( spuSyncCount >= SPU_UPD_INTERVAL )
        {
            spuSyncCount = 0;
            SPU_async();
        }
        /*
        // For the best times. :D
        // VSync irq.
        if( hSyncCount == VBlankStart[Config.PsxType] )
        {
            setIrq( 0x01 );
        }
        */
        // Update lace. (with InuYasha fix)
        if( hSyncCount >= (Config.VSyncWA ? UDIV(HSyncTotal[Config.PsxType],BIAS) : HSyncTotal[Config.PsxType]) )
        {
#ifdef DEBUG_BIOS
	    dbg("UpdateLace");
#endif
            hSyncCount = 0;

            setIrq( 0x01 );

            GPU_updateLace();
		pad_update();
#ifdef DEBUG_END_FRAME
		{
			static unsigned _endframe_=0;
			static unsigned _frametime_[DEBUG_END_FRAME+1];
			_frametime_[_endframe_]=(get_ticks()
#ifndef TIME_IN_MSEC
					/1000
#endif
					);
			_endframe_++;
			if (_endframe_>DEBUG_END_FRAME) {
				unsigned i;
				for(i=1;i<_endframe_;i++) 
					printf("FRAME %u = %u msec\n",i,_frametime_[i]-_frametime_[i-1]);
				pcsx4all_exit();
			}
		}
#endif

			if ((toSaveState)&&(SaveState_filename)) {
			toSaveState=0;
			SaveState(SaveState_filename);
			if (toExit)
				pcsx4all_exit();
			}
			if ((toLoadState)&&(SaveState_filename))
			{
				toLoadState=0;
				LoadState(SaveState_filename);
				pcsx4all_prof_reset();
#ifdef PROFILER_PCSX4ALL
				_pcsx4all_prof_end(PCSX4ALL_PROF_CPU,0);
#endif
				pcsx4all_prof_start(PCSX4ALL_PROF_CPU);
				psxCpu->Execute();
				pcsx4all_prof_end(PCSX4ALL_PROF_CPU);
				pcsx4all_exit();
			}
        }
    }
    pcsx4all_prof_end_with_resume(PCSX4ALL_PROF_COUNTERS,PCSX4ALL_PROF_CPU);
}

/******************************************************************************/

void psxRcntWcount( u32 index, u32 value )
{
#ifdef DEBUG_BIOS
    dbgf("[RCNT %i] wcount: %x\n", index, value );
#endif

    psxRcntUpdate();

    _psxRcntWcount( index, value );
    psxRcntSet();
}

static u32 rcnt_target;

void psxRcntWmode( u32 index, u32 value )
{
#ifdef DEBUG_BIOS
    dbgf("[RCNT %i] wmode: %x\n", index, value );
#endif

    psxRcntUpdate();

    rcnts[index].mode = value;
    rcnts[index].irqState = 0;

    switch( index )
    {
        case 0:
            if( value & Rc0PixelClock )
            {
                rcnts[index].rate = 5;
            }
            else
            {
                rcnts[index].rate = 1;
            }
        break;
        case 1:
            if( value & Rc1HSyncClock )
            {
                rcnts[index].rate = rcnt_target;
            }
            else
            {
                rcnts[index].rate = 1;
            }
        break;
        case 2:
            if( value & Rc2OneEighthClock )
            {
                rcnts[index].rate = 8;
            }
            else
            {
                rcnts[index].rate = 1;
            }

            // TODO: wcount must work.
            if( value & Rc2Disable )
            {
                rcnts[index].rate = 0xffffffff;
            }
        break;
    }

    _psxRcntWcount( index, 0 );
    psxRcntSet();
}

void psxRcntWtarget( u32 index, u32 value )
{
#ifdef DEBUG_BIOS
    dbgf("[RCNT %i] wtarget: %x\n", index, value );
#endif

    psxRcntUpdate();

    rcnts[index].target = value;

    _psxRcntWcount( index, _psxRcntRcount( index ) );
    psxRcntSet();
}

/******************************************************************************/

u32 psxRcntRcount( u32 index )
{
#ifdef DEBUG_ANALYSIS
	dbg_anacnt_psxRcntRcount++;
#endif
    pcsx4all_prof_start_with_pause(PCSX4ALL_PROF_COUNTERS,PCSX4ALL_PROF_CPU);
    u32 count;

#ifndef USE_NO_IDLE_LOOP
    if (autobias) {
	u32 cycle_next=rcnts[index].cycle+rcnts[index].cycleStart;
	if (cycle_next>psxRegs.cycle) {
		if (psxRegs.io_cycle_counter>(psxRegs.cycle+32) && psxRegs.io_cycle_counter<cycle_next) {
			cycle_next = psxRegs.io_cycle_counter - psxRegs.cycle - 24;
		} else {
			cycle_next = 12+((cycle_next - psxRegs.cycle)&0x1FF);
		}
	    	if (rcnts[index].target) {
			unsigned value=(((rcnts[index].target)*(rcnts[index].rate+1))/4)*BIAS;
			if (value<cycle_next)
				psxRegs.cycle+=value;
			else
				psxRegs.cycle+=cycle_next;
		} else {
			psxRegs.cycle+=cycle_next;
		}
    	}
    }
#endif
    psxRcntUpdate();

    count = _psxRcntRcount( index );

    // Parasite Eve 2 fix.
    if( Config.RCntFix )
    {
        if( index == 2 )
        {
            if( rcnts[index].counterState == CountToTarget )
            {
                count=UDIV(count,BIAS);
            }
        }
    }

#ifdef DEBUG_BIOS
    dbgf("[RCNT %i] rcount: %x\n", index, count );
#endif

    pcsx4all_prof_end_with_resume(PCSX4ALL_PROF_COUNTERS,PCSX4ALL_PROF_CPU);
    return count;
}

u32 psxRcntRmode( u32 index )
{
#ifdef DEBUG_ANALYSIS
	dbg_anacnt_psxRcntRmode++;
#endif
    pcsx4all_prof_start_with_pause(PCSX4ALL_PROF_COUNTERS,PCSX4ALL_PROF_CPU);
    u16 mode;

    psxRcntUpdate();

    mode = rcnts[index].mode;
    rcnts[index].mode &= 0xe7ff;

#ifdef DEBUG_BIOS
    dbgf("[RCNT %i] rmode: %x\n", index, mode );
#endif

    pcsx4all_prof_end_with_resume(PCSX4ALL_PROF_COUNTERS,PCSX4ALL_PROF_CPU);
    return mode;
}

u32 psxRcntRtarget( u32 index )
{
#ifdef DEBUG_BIOS
    dbgf("[RCNT %i] rtarget: %x\n", index, rcnts[index].target );
#endif

    return rcnts[index].target;
}

/******************************************************************************/

void psxRcntUVtarget(void)
{
	rcnt_target=UDIV(PSXCLK,(FrameRate[Config.PsxType] * HSyncTotal[Config.PsxType]));
}

void psxRcntInit(void)
{
    s32 i;

    // rcnt 0.
    rcnts[0].rate   = 1;
    rcnts[0].irq    = 0x10;

    // rcnt 1.
    rcnts[1].rate   = 1;
    rcnts[1].irq    = 0x20;

    // rcnt 2.
    rcnts[2].rate   = 1;
    rcnts[2].irq    = 0x40;

    // rcnt base.
    rcnts[3].rate   = 1;
    rcnts[3].mode   = RcCountToTarget;
	psxRcntUVtarget();
    rcnts[3].target = rcnt_target;

    for( i = 0; i < CounterQuantity; ++i )
    {
        _psxRcntWcount( i, 0 );
    }

    psxRcntSet();
}

/******************************************************************************/

s32 psxRcntFreeze( gzFile f, s32 Mode )
{
    gzfreeze( &rcnts, sizeof(rcnts) );
    gzfreeze( &hSyncCount, sizeof(hSyncCount) );
    gzfreeze( &spuSyncCount, sizeof(spuSyncCount) );
    gzfreeze( &psxNextCounter, sizeof(psxNextCounter) );
    gzfreeze( &psxNextsCounter, sizeof(psxNextsCounter) );

    return 0;
}

void psxSetSyncs(unsigned h_sync, unsigned s_sync){
	hSyncCount=h_sync;
	spuSyncCount=s_sync;
}

unsigned psxGetHSync(void){
	return hSyncCount;
}

unsigned psxGetSpuSync(void){
	return spuSyncCount;
}

/******************************************************************************/
