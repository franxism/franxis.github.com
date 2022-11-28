/* gpsoundbuf.c - Implements a ring buffer for sound mixing on the GamePark 32
 *
 * Copyright (c)2002 Christian Nowak <chnowak@web.de>
 *
 * Use it as you want without any obligations or restrictions,
 * but a notification via eMail would be nice.
 *
 */


 /* Header files */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "defines.h"
#include "gpmm.h"
#include "gpos_def.h"
#include "gpsoundbuf.h"

/* Global variables */
unsigned int frame;
unsigned int * soundPos;
volatile int idx_buf;
unsigned int shiftVal;
void * buffer;
GPSOUNDBUF soundBuf;



/* This routine gets called by the timer interrupt and
 * polls the current playing position within the buffer.
 */
void soundtimer ( void )
  {
    unsigned int t = (((unsigned int)(*soundPos) - (unsigned int)buffer)>>shiftVal) >= soundBuf.samples ? 1 : 0;
    if (t!=frame)
      {
        unsigned int offs = ((frame==1) ? (soundBuf.samples<<shiftVal) : 0);
        soundBuf.callback(soundBuf.userdata, (u8*)((unsigned int)buffer+offs), soundBuf.samples<<shiftVal);
        frame = t;
      }
  }

int GpSoundBufStart ( GPSOUNDBUF * sb )
  {
    frame = 0;

    /* Copy the structure */
    gm_memcpy ( &soundBuf, sb, sizeof(GPSOUNDBUF) );

    /* Calculate size of a single sample in bytes
     * and a corresponding shift value
     */
    shiftVal = 0;
    switch (soundBuf.freq)
      {
        case PCM_S11:
        case PCM_S22:
        case PCM_S44:
          shiftVal++;
          break;
      }
    if (soundBuf.format == PCM_16BIT)
      shiftVal++;
    soundBuf.samplesize = 1<<shiftVal;

    /* Allocate memory for the playing buffer */
    buffer = gm_zi_malloc ( soundBuf.samplesize*soundBuf.samples*2 );
    gm_memset ( buffer, 0x80, soundBuf.samplesize*soundBuf.samples*2 );

    /* Set timer interrupt #0 */
    /*GpTimerOptSet ( 0, soundBuf.pollfreq, 0, soundtimer );
    GpTimerSet ( 0 );*/
    if ( GPOS_ERR_ALREADY_USED ==
	 GpTimerOptSet ( 0, soundBuf.pollfreq, 0, soundtimer ) )
    {
      GpTimerKill(0);
    }
    GpTimerSet ( 0 );
    
    /* Start playing */
    GpPcmPlay ( (unsigned short*)buffer, soundBuf.samples*soundBuf.samplesize*2, 1 );
    GpPcmLock ( (unsigned short*)buffer, (int*)&idx_buf, (unsigned int*)&soundPos );

    return 0;
  }


void GpSoundBufStop ( void )
  {
    GpPcmStop();
    GpPcmRemove ( (unsigned short*)buffer );
    GpTimerKill ( 0 );
    gm_free ( buffer );
  }
