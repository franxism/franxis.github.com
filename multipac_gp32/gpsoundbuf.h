/* gpsoundbuf.h - Implements a ring buffer for sound mixing on the GamePark 32
 *
 * Copyright (c)2002 Christian Nowak <chnowak@web.de>
 *
 * Use it as you want without any obligations or restrictions,
 * but a notification via eMail would be nice.
 *
 */

#ifndef __GPSOUNDBUF_H__
#define __GPSOUNDBUF_H__
 
#include "defines.h"
#include "gpmm.h"

typedef struct GPSOUNDBUF
  {
    PCM_SR freq;              /* Taken from gpmm.h */
    PCM_BIT format;           /* Taken from gpmm.h */
    unsigned int samples;     /* Buffer length (in samples) */
    void * userdata;          /* Userdata which gets passed to the callback function */
    void (*callback)(         /* Callback function (just like in SDL) */
          void * userdata,    /* GPSOUNDBUF.userdata */
          u8 * stream,        /* Pointer to the buffer which needs to be refilled */
          int len);           /* Length of the buffer in bytes */
    unsigned int pollfreq;    /* Frequency of the timer interrupt which polls the playing position
                               * recommended value: 2*(playingfreq in Hz/GPSOUNDBUF.samples) */
    unsigned int samplesize;  /* Size of one sample (8bit mono->1, 16bit stereo->4) - don't touch this */
  } GPSOUNDBUF;

int GpSoundBufStart ( GPSOUNDBUF * );
void GpSoundBufStop ( void );

#endif
