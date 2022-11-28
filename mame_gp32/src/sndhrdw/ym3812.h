#ifndef __YM3812_H_
#define __YM3812_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifdef SIGNED_SAMPLES
#define ym3812_Sign8(n) (n)
#define ym3812_Sign16(n) (n)
#else
#define ym3812_Sign8(n) (n^0x80)
#define ym3812_Sign16(n) (n^0x8000)
#endif

#define ym3812_StdClock 3579545

#define ym3812_StdVolume 128

#define ym3812_STFLAG2	0x20
#define ym3812_STFLAG1	0x40
#define ym3812_STIRQ	0x80

#define ym3812_TCST1	0x01
#define ym3812_TCST2	0x02
#define ym3812_TCMASK2	0x20
#define ym3812_TCMASK1	0x40
#define ym3812_TCIRQRES	0x80

enum EnvStat { ADSR_Silent, ADSR_Attack, ADSR_Decay, ADSR_Sustain, ADSR_Release };

typedef struct ym3812_s {

	

	unsigned char	nReg;				
	int				nStatus;			
	double			vTimer1;			
	double			vTimer2;			
	int				nTimerCtrl;			
	int				nYM3812Clk;			
	int				nYM3812DivClk;		

	

	int				fWave;				
	int				nDepthRhythm;		
	int				nCSM;				
	int				fEGTyp[18];			
	int				fVibrato[18];		
	int				fAM[18];			
	int				fKSR[18];			
	int				nTotalLevel[18];	
	int				nKSL[18];			
	int				nMulti[18];			
	int				nWave[18];			
	int				nAttack[18];		
	int				nDecay[18];			
	int				nSustain[18];		
	int				nRelease[18];		
	int				nEnvState[18];		
	float			vEnvTime[18];		
	int				nFNumber[9];		
	int				nOctave[9];			
	int				nFeedback[9];		
	int				fConnection[9];		
	int				fKeyDown[9];		

	

	unsigned int	nCurrPos[18];		
	int				nVibratoOffs;		
	int				nAMDepthOffs;		
	int				nSinValue[18];		

	

	char			aRegArray[256];		

	
	int				nSubDivide;			
	int				f16Bit;				
	int				aVolumes[256];		
	signed char		*pDrum[5];			
	int				nDrumSize[5];		
	int				nDrumOffs[5];		
	int				nDrumRate[5];		
	int				nReplayFrq;			
	void			(*SetTimer)(int, double, struct ym3812_s *, int);	
} ym3812;

#ifndef cFALSE
#define cFALSE 0
#endif
#ifndef cTRUE
#define cTRUE 1
#endif

extern ym3812* ym3812_Init( int nReplayFrq, int nClock, int f16Bit );
extern ym3812* ym3812_DeInit( ym3812 *pOPL );
extern void ym3812_Update_stream( ym3812* pOPL, void *pBuffer_in, int nLength );
extern int ym3812_ReadStatus( ym3812 *pOPL );
extern int	ym3812_ReadReg( ym3812 *pOPL );
extern void ym3812_SetReg( ym3812 *pOPL, unsigned char nReg );
extern void ym3812_WriteReg( ym3812 *pOPL, unsigned char nData);
extern int ym3812_TimerEvent( ym3812 *pOPL, int nTimer );

#ifdef __cplusplus
};
#endif
#endif


