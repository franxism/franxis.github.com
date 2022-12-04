/***************************************************************************
*   Copyright (C) 2010 PCSX4ALL Team                                      *
*   Copyright (C) 2010 Franxis                                            *
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

////////////////////////////////////////////////////////////////////////
// ADSR func
////////////////////////////////////////////////////////////////////////

unsigned int RateTable[160];

INLINE void InitADSR(void)                                    // INIT ADSR
{
 unsigned int r,rs,rd;int i;

	memset(RateTable,0,sizeof(unsigned long)*160);        // build the rate table according to Neill's rules (see at bottom of file)

	r=3;rs=1;rd=0;

	for(i=32;i<160;i++)                                   // we start at pos 32 with the real values... everything before is 0
	{
		if(r<0x3FFFFFFF)
		{
			r+=rs;
			rd++;if(rd==5) {rd=1;rs*=2;}
		}
		if(r>0x3FFFFFFF) r=0x3FFFFFFF;

		RateTable[i]=r;
	}
}

////////////////////////////////////////////////////////////////////////

INLINE int MixADSR(SPUCHAN *l_chan)                             // MIX ADSR
{
	static const char ratetable_offset[8] = { 0, 4, 6, 8, 9, 10, 11, 12 };
	int rto;

	if(l_chan->bStop)                                  // should be stopped:
	{                                                    // do release
		if(l_chan->ADSRX.ReleaseModeExp)
		{
			rto=ratetable_offset[(l_chan->ADSRX.EnvelopeVol>>28)&0x7];
		}
		else
		{
			rto=12;
		}
		l_chan->ADSRX.EnvelopeVol-=RateTable[(4*(l_chan->ADSRX.ReleaseRate^0x1F))-0x18 + rto + 32];

		if(l_chan->ADSRX.EnvelopeVol<0) 
		{
			l_chan->ADSRX.EnvelopeVol=0;
			// don't stop if this chan can still cause irqs
			if(!(spuCtrl&0x40) || (l_chan->pCurr > pSpuIrq && l_chan->pLoop > pSpuIrq))
			//l_chan->bOn=0;
			l_chan->pCurr=(unsigned char *)-1;
			//l_chan->bReverb=0;
			//l_chan->bNoise=0;
		}

		goto done;
	}

	switch(l_chan->ADSRX.State)                       // not stopped yet
	{
		case 0:                                             // -> attack
			rto=8;
			if(l_chan->ADSRX.AttackModeExp&&l_chan->ADSRX.EnvelopeVol>=0x60000000)
			rto = 0;
			l_chan->ADSRX.EnvelopeVol+=RateTable[(l_chan->ADSRX.AttackRate^0x7F)-0x18 + rto + 32];

			if(l_chan->ADSRX.EnvelopeVol<0) 
			{
				l_chan->ADSRX.EnvelopeVol=0x7FFFFFFF;
				l_chan->ADSRX.State=1;
			}
			break;

			//--------------------------------------------------//
		case 1:                                             // -> decay
			rto=ratetable_offset[(l_chan->ADSRX.EnvelopeVol>>28)&0x7];
			l_chan->ADSRX.EnvelopeVol-=RateTable[(4*(l_chan->ADSRX.DecayRate^0x1F))-0x18+ rto + 32];

			if(l_chan->ADSRX.EnvelopeVol<0) l_chan->ADSRX.EnvelopeVol=0;
			if(((l_chan->ADSRX.EnvelopeVol>>27)&0xF) <= l_chan->ADSRX.SustainLevel)
			{
				l_chan->ADSRX.State=2;
			}
			break;

			//--------------------------------------------------//
		case 2:                                             // -> sustain
			if(l_chan->ADSRX.SustainIncrease)
			{
				rto=8;
				if(l_chan->ADSRX.SustainModeExp&&l_chan->ADSRX.EnvelopeVol>=0x60000000)
				rto=0;
				l_chan->ADSRX.EnvelopeVol+=RateTable[(l_chan->ADSRX.SustainRate^0x7F)-0x18 + rto + 32];

				if(l_chan->ADSRX.EnvelopeVol<0) 
				{
					l_chan->ADSRX.EnvelopeVol=0x7FFFFFFF;
				}
			}
			else
			{
				if(l_chan->ADSRX.SustainModeExp)
				{
					rto=ratetable_offset[(l_chan->ADSRX.EnvelopeVol>>28)&0x7];
				}
				else
				{
					rto=12;
				}
				l_chan->ADSRX.EnvelopeVol-=RateTable[((l_chan->ADSRX.SustainRate^0x7F))-0x1B + rto + 32];

				if(l_chan->ADSRX.EnvelopeVol<0) 
				{
					l_chan->ADSRX.EnvelopeVol=0;
				}
			}
			break;
	}

	done:
	return l_chan->ADSRX.EnvelopeVol>>21;
}
