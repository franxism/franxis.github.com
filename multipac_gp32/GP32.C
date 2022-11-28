/***************************************************************************

  osdepend.c

  OS dependant stuff (display handling, keyboard scan...)
  This is the only file which should me modified in order to port the
  emulator to a different system.

***************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "osdepend.h"
#include "gpdef.h"
#include "gpstdio.h"
#include "gpstdlib.h"
#include "gpgraphic.h"
#include "gpfont.h"
#include "gpmem.h"
#include "gpsoundbuf.h"
#include "gp32_menu.h"

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#define SCREEN_MODE GFX_VESA1

/* Converts 8bit rgb values to a GP32 palette value */
#define GP_RGB24(r,g,b) (((((r>>3))&0x1f)<<11)|((((g>>3))&0x1f)<<6)|((((b>>3))&0x1f)<<1))

int bitmap_width,bitmap_height;
int first_free_pen;
unsigned char *bitmap;
int use_vesa;
int play_sound;
int noscanlines;
int use_joystick;


int osd_joy_up=0, osd_joy_down=0, osd_joy_left=0, osd_joy_right=0;
int osd_joy_b1=0, osd_joy_b2=0, osd_joy_b3=0, osd_joy_b4=0;

int orientation=1;

/* audio related stuff */
#define NUMVOICES 3
#define SOUND_DIV 765 /*3 voices x 255 volum.max */
#define SAMPLE_RATE 44100 /*44100*/
#define SAMPLE_BUFFER_LENGTH 50000
GPSOUNDBUF sb;
struct sound_buffer {
	signed char data[128];
	int len;
	int freq;
	int volume;
	int loop;
};
struct sound_buffer sound_buffers[NUMVOICES];
char pause=0;

/* GP32 related stuff */
int nflip;			/* Video buffer index */
GPDRAWSURFACE gpDraw[2];	/* Video Buffers */

/* Debug information function*/
void text_out(int x, int y, char *texto) {
	GpTextOut( NULL, &gpDraw[nflip],x,y,texto, 0xff );
}

/* Debug information function*/
void text_out_int(int x, int y, int entero) {
	char cad[50];
	sprintf(cad,"%d\n",entero);
	text_out(x,y,cad);
}


/* GP32 Sound Mixer Interrupt Function */
void mixer ( void * userdata, unsigned char * stream, int len )
{

	register unsigned short channel;
	register unsigned short i;
	register unsigned short idx;
	register unsigned short c;
	register signed char *str=(signed char *)stream;

	/* Fill buffer with silence */
 	memset ( str, 0x80, len );

 	if(pause==1) return;

	for (channel=0;channel<NUMVOICES;channel++) {

		i=0;
		idx=0;
		c=1;

		if( (sound_buffers[channel].len==0)  ||
		    	(sound_buffers[channel].freq==0) ||
			(sound_buffers[channel].volume==0) ) continue;

		while(i<len) {
			str[i]+=(sound_buffers[channel].data[idx] * sound_buffers[channel].volume) / SOUND_DIV;
			idx++;
			idx=idx%sound_buffers[channel].len;
			i=(c*SAMPLE_RATE)/(sound_buffers[channel].freq);
			c++;
		}

	}

}

/* put here anything you need to do when the program is started. Return 0 if */
/* initialization was successful, nonzero otherwise. */
int osd_init(int argc,char **argv)
{

 	int i=0;

	use_vesa = 0;
	play_sound = 1;
	noscanlines = 0;
	use_joystick = 0;

 	GpClockSpeedChange(132000000, 0x3a011, 3); /* 133 mhz */
 	/* GpClockSpeedChange(140000000, 0x3e011, 3); // 144 mhz */
 	/* GpClockSpeedChange(150000000, 0x43011, 3); // 150 mhz */
 	/* GpClockSpeedChange(156000000, 0x46011, 3); // 156 mhz */
 	/* GpClockSpeedChange(160000000, 0x48011, 3); // 160 mhz */
 	/* GpClockSpeedChange (166000000, 0x4b011, 3); // 166 mhz */

	GpGraphicModeSet(8,0);  /* 320x240, 256 colores */

	for( i=0; i<2; i++ ) {
		GpLcdSurfaceGet( &gpDraw[i],i );
		GpRectFill( NULL, &gpDraw[i],0,0,gpDraw[i].buf_w,gpDraw[i].buf_h,0x0 );
	}
	nflip=0;
	GpSurfaceSet( &gpDraw[nflip] );

/*
	for (i = 1;i < argc;i++)
	{
		if (stricmp(argv[i],"-vesa") == 0)
			use_vesa = 1;
		if (stricmp(argv[i],"-nosound") == 0)
			play_sound = 0;
		if (stricmp(argv[i],"-noscanlines") == 0)
			noscanlines = 1;
		if (stricmp(argv[i],"-nojoy") == 0)
			use_joystick = 0;
	}
*/

/*
	if (use_joystick)
	{
		if (initialise_joystick())
			use_joystick = 0;
	}
*/

	if (play_sound)
	{
   	/*sb.freq = PCM_M44;*/
   		sb.freq = PCM_M44;
		sb.format = PCM_8BIT;
		sb.samples = SAMPLE_RATE/60; /*SAMPLE_RATE/60;*/
		sb.userdata = NULL;
		sb.callback = mixer;
		sb.pollfreq = 60; /*60*/ /* 12*(44100/sb.samples); */
		/*GpPcmInit(PCM_M44,PCM_8BIT);*/
		GpPcmInit(PCM_M44,PCM_8BIT);
	}

	first_free_pen = 0;

	/* SMC Access Init */
 	GpFatInit();

	/* Initialize Game Listings */
 	game_list_init();

	return 0;
}



/* put here cleanup routines to be executed when the program is terminated. */
void osd_exit(void)
{
	if (play_sound) {
				GpSoundBufStop();
	}
}

/* Create a display screen, or window, large enough to accomodate a bitmap */
/* of the given dimensions. I don't do any test here (224x288 will just do */
/* for now) but one could e.g. open a window of the exact dimensions */
/* provided. Return a bitmap pointer or 0 in case of error. */
unsigned char *osd_create_display(int width,int height)
{

	bitmap = 0;
	bitmap_width = width;
	bitmap_height = height;

	bitmap = gm_malloc(width * height * sizeof(unsigned char));
	first_free_pen = 0;
	sound_buffers[0].len=0;
	sound_buffers[1].len=0;
	sound_buffers[2].len=0;
	GpSoundBufStart(&sb);

	return bitmap;
}



/* shut up the display */
void osd_close_display(void)
{
	gm_free(bitmap);
}



int osd_obtain_pen(unsigned char red, unsigned char green, unsigned char blue)
{
	GP_PALETTEENTRY colorgp;
	colorgp = GP_RGB24(red,green,blue);
    	GpPaletteEntryChange(first_free_pen,1,&colorgp,0);
	return first_free_pen++;
}



/* Update the display. */
/* As an additional bonus, this function also saves the screen as a PCX file */
/* when the user presses F5. This is not required for porting. */
void osd_update_display(void)
{

	register unsigned char *buffer_scr;
	register unsigned char *buffer_var = bitmap; /* 224 x 288 */
	register int x,y;
	nflip=(nflip==0?1:0);
	buffer_scr=gpDraw[nflip].o_buffer;

/* ORIGINAL
	for (y=0;y<bitmap_height;y++) {
		for (x=0;x<bitmap_width;x++) {
			buffer_scr[239-y+x*240] = buffer_var[x+(y*bitmap_width)];
		}
	}
*/

/* STRETCH STANDARD
	for (y=0;y<bitmap_height;y++) {
		if ((y+3)%6==0) continue;
		for (x=0;x<bitmap_width;x++) {
			buffer_scr[239-cy+(x+48)*240] = buffer_var[x+(y*bitmap_width)];
		}
		cy++;
	}

*/
	if (orientation==0) {
		/* Horizontal */
		buffer_var+=(28*bitmap_width);
		for (y=28;y<bitmap_height-20;y++) {
			for (x=0;x<bitmap_width;x++) {
				/*buffer_scr[239-(y-28)+(x+48)*240] = buffer_var[x+(y*bitmap_width)];*/
				buffer_scr[267-y+(x+48)*240] = *buffer_var++;
			}
		}
	} else {
		/* Vertical */
		for (y=0;y<bitmap_height;y++) {
			for (x=0;x<bitmap_width;x++) {
				/*buffer_scr[239-x-8+(319-y-16)*240] = buffer_var[x+(y*bitmap_width)];*/
				buffer_scr[231-x+(303-y)*240] = *buffer_var++;
			}
		}
	}
		

	GpSurfaceSet( &gpDraw[nflip] );

}




void osd_update_audio(void)
{
/*	AUpdateAudio(); */
}



void osd_play_sample(int channel,unsigned char *data,int len,int freq,int volume,int loop)
{

	if (play_sound == 0 || channel >= NUMVOICES) return;

	memcpy(sound_buffers[channel].data,data,len);
	sound_buffers[channel].len=len;
	sound_buffers[channel].freq=freq;
	sound_buffers[channel].volume=volume;
	sound_buffers[channel].loop=loop;

}



void osd_play_streamed_sample(int channel,unsigned char *data,int len,int freq,int volume)
{

	if (play_sound == 0 || channel >= NUMVOICES) return;

	memcpy(sound_buffers[channel].data,data,len);
	sound_buffers[channel].len=len;
	sound_buffers[channel].freq=freq;
	sound_buffers[channel].volume=volume;
	sound_buffers[channel].loop=1;

}



void osd_adjust_sample(int channel,int freq,int volume)
{
	if (play_sound == 0 || channel >= NUMVOICES) return;

	sound_buffers[channel].freq=freq;
	sound_buffers[channel].volume=volume;
}



void osd_stop_sample(int channel)
{
	if (play_sound == 0 || channel >= NUMVOICES) return;

	sound_buffers[channel].len=0;

}



/* check if a key is pressed. The keycode is the standard PC keyboard code, as */
/* defined in osdepend.h. Return 0 if the key is not pressed, nonzero otherwise. */
int osd_key_pressed(int keycode)
{
  	int ExKey=GpKeyGet();

	/* Screen Orientation */
	if ((ExKey & GPC_VK_FR) && !(ExKey & GPC_VK_FL)) {
	 orientation=(orientation==0?1:0);
	 while (GpKeyGet() & GPC_VK_FR);
		GpRectFill( NULL, &gpDraw[0],0,0,gpDraw[0].buf_w,gpDraw[0].buf_h,0x0 );
		GpRectFill( NULL, &gpDraw[1],0,0,gpDraw[1].buf_w,gpDraw[1].buf_h,0x0 );
	}

	if(keycode==OSD_KEY_1) {
		return ((ExKey & GPC_VK_START) || (ExKey & GPC_VK_FA));
	}
	if(keycode==OSD_KEY_3) {
		return ((ExKey & GPC_VK_SELECT) || (ExKey & GPC_VK_FB));
	}
 	if(keycode==OSD_KEY_P) {
	  if((ExKey & GPC_VK_FL) && !(ExKey & GPC_VK_FR)) {
			  while (GpKeyGet() & GPC_VK_FL);
			  return(1);
	  } else {
	    return (0);
	  }
 	}

	if(keycode==OSD_KEY_ESC) {
		return ((ExKey & GPC_VK_FL) && (ExKey & GPC_VK_FR));
	}

	if(orientation==0) {
	  if(keycode==OSD_KEY_DOWN) {
		   return (ExKey & GPC_VK_DOWN);
	  }
	  if(keycode==OSD_KEY_RIGHT) {
		  return (ExKey & GPC_VK_RIGHT);
	  }
	  if(keycode==OSD_KEY_LEFT) {
		  return (ExKey & GPC_VK_LEFT);
	  }
	  if(keycode==OSD_KEY_UP) {
		  return (ExKey & GPC_VK_UP);
	  }
	} else {
	  if(keycode==OSD_KEY_DOWN) {
		   return (ExKey & GPC_VK_LEFT);
	  }
	  if(keycode==OSD_KEY_RIGHT) {
		  return (ExKey & GPC_VK_DOWN);
	  }
	  if(keycode==OSD_KEY_LEFT) {
		  return (ExKey & GPC_VK_UP);
	  }
	  if(keycode==OSD_KEY_UP) {
		  return (ExKey & GPC_VK_RIGHT);
	  }
	}
	return 0;
}



/* wait for a key press and return the keycode */
int osd_read_key(void)
{
  	int ExKey=GpKeyGet();
	while ((ExKey & GPC_VK_FL)==0) {
	 	ExKey=GpKeyGet();
	}
	return(OSD_KEY_P);
}



void osd_poll_joystick(void)
{
	if (use_joystick == 1)
	{
   		int ExKey=GpKeyGet();

		osd_joy_up    = (ExKey & GPC_VK_UP);
		osd_joy_down  = (ExKey & GPC_VK_DOWN);
		osd_joy_left  = (ExKey & GPC_VK_LEFT);
		osd_joy_right = (ExKey & GPC_VK_RIGHT);
		osd_joy_b1    = (ExKey & GPC_VK_FA);
		osd_joy_b2    = (ExKey & GPC_VK_FB);
		osd_joy_b3    = (ExKey & GPC_VK_FL);
		osd_joy_b4    = (ExKey & GPC_VK_FR);
	}
}



