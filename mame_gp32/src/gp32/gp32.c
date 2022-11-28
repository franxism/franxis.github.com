/***************************************************************************

  osdepend.c

  OS dependant stuff (display handling, keyboard scan...)
  This is the only file which should me modified in order to port the
  emulator to a different system.

***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gpdef.h"
#include "gpstdio.h"
#include "gpstdlib.h"
#include "gpgraphic.h"
#include "gpfont.h"
#include "gpmem.h"
#include "driver.h"
#include "gp32_mame.h"
#include "gp32_menu.h"

int  msdos_init_seal (void);
int  msdos_init_sound(void);
void msdos_init_input(void);
void msdos_shutdown_sound(void);
void msdos_shutdown_input(void);
int  frontend_help (int argc, char **argv);
void parse_cmdline (int argc, char **argv, struct GameOptions *options, int game);

/* platform independent options go here */
struct GameOptions options;

/* From gp32_menu.c */
extern int gp32_freq;

/* enablecache.o */
extern void EnableCache(void);

/* put here anything you need to do when the program is started. Return 0 if */
/* initialization was successful, nonzero otherwise. */
int osd_init(void)
{
	gp32_text_log("GP32 Port Init...");
	if (msdos_init_sound())
		return 1;
	msdos_init_input();
	return 0;
}


/* put here cleanup routines to be executed when the program is terminated. */
void osd_exit(void)
{
	msdos_shutdown_sound();
	msdos_shutdown_input();
}

/* fuzzy string compare, compare short string against long string        */
/* e.g. astdel == "Asteroids Deluxe". The return code is the fuzz index, */
/* we simply count the gaps between maching chars.                       */
int fuzzycmp (const char *s, const char *l)
{
	int gaps = 0;
	int match = 0;
	int last = 1;

	for (; *s && *l; l++)
	{
		if (*s == *l)
			match = 1;
		else if (*s >= 'a' && *s <= 'z' && (*s - 'a') == (*l - 'A'))
			match = 1;
		else if (*s >= 'A' && *s <= 'Z' && (*s - 'A') == (*l - 'a'))
			match = 1;
		else
			match = 0;

		if (match)
			s++;

		if (match != last)
		{
			last = match;
			if (!match)
				gaps++;
		}
	}

	/* penalty if short string does not completely fit in */
	for (; *s; s++)
		gaps++;

	return gaps;
}



void GpMain(void *arg)
{
	/* Command Line Arguments Emulation */
	int argc=2;
	char *argv[] = {"mame","builtinn"};
	char text[64];

	int res, i, j, game_index;

	/* CPU Alignment Cache */
	EnableCache();

	/* SMC Access Init */
 	GpFatInit();

	/* Clock Speed 133 mhz */
	GpClockSpeedChange(132000000, 0x3a011, 3);
	
	/* GP32 Video Init */
	gp32_video_init();

	/* Show Intro Screen */
	intro_screen();

	/* Initialize Game Listings */
 	game_list_init();

	while(1) {

		/* Clock Speed 66 MHz */
		GpClockSpeedChange(67500000,0x25002,2);
		
		/* GP32 Video Init */
		gp32_video_init();

		/* Select Game */
		select_game(argv);

		/* Select Clock Speed */
		switch(gp32_freq) {
			case 133: /* Clock Speed 133 mhz */
				GpClockSpeedChange(132000000, 0x3a011, 3);
				break;
			case 144: /* Clock Speed 144 mhz */
 				GpClockSpeedChange(140000000, 0x3e011, 3);
				break;
			case 150: /* Clock Speed 150 mhz */
 				GpClockSpeedChange(150000000, 0x43011, 3);
				break;
			case 156: /* Clock Speed 156 mhz */
				GpClockSpeedChange(156000000, 0x46011, 3);
				break;
			case 160: /* Clock Speed 160 mhz */
 				GpClockSpeedChange(160000000, 0x48011, 3);
				break;
			case 166: /* Clock Speed 166 mhz */
				GpClockSpeedChange(166000000, 0x4b011, 3); 
				break;
			case 168: /* Clock Speed 168 mhz */
				GpClockSpeedChange(168000000, 0x14000, 3); 
				break;
			case 172: /* Clock Speed 172 mhz */
				GpClockSpeedChange(172000000, 0x23010, 3); 
				break;
			case 176: /* Clock Speed 176 mhz */
				GpClockSpeedChange(176000000, 0x24010, 3); 
				break;
			case 180: /* Clock Speed 180 mhz */
				GpClockSpeedChange(180000000, 0x16000, 3); 
				break;
			case 184: /* Clock Speed 184 mhz */
				GpClockSpeedChange(184000000, 0x26010, 3); 
				break;
			case 188: /* Clock Speed 188 mhz */
				GpClockSpeedChange(188000000, 0x27010, 3); 
				break;
			case 192: /* Clock Speed 192 mhz */
				GpClockSpeedChange(192000000, 0x18000, 3); 
				break;
			case 196: /* Clock Speed 196 mhz */
				GpClockSpeedChange(196000000, 0x29010, 3); 
				break;
			case 200: /* Clock Speed 200 mhz */
				GpClockSpeedChange(200000000, 0x2A010, 3); 
				break;
			case 204: /* Clock Speed 204 mhz */
				GpClockSpeedChange(204000000, 0x2b010, 3); 
				break;
			case 208: /* Clock Speed 208 mhz */
				GpClockSpeedChange(208000000, 0x2c010, 3); 
				break;
			case 212: /* Clock Speed 212 mhz */
				GpClockSpeedChange(212000000, 0x2d010, 3); 
				break;
			case 216: /* Clock Speed 216 mhz */
				GpClockSpeedChange(216000000, 0x2e010, 3); 
				break;
			case 220: /* Clock Speed 220 mhz */
				GpClockSpeedChange(220000000, 0x2f010, 3); 
				break;
			case 224: /* Clock Speed 224 mhz */
				GpClockSpeedChange(224000000, 0x30010, 3); 
				break;
			case 228: /* Clock Speed 228 mhz */
				GpClockSpeedChange(228000000, 0x1e000, 3); 
				break;
			case 232: /* Clock Speed 232 mhz */
				GpClockSpeedChange(232000000, 0x32010, 3); 
				break;
			case 236: /* Clock Speed 236 mhz */
				GpClockSpeedChange(236000000, 0x33010, 3); 
				break;
			case 240: /* Clock Speed 240 mhz */
				GpClockSpeedChange(240000000, 0x20000, 3); 
				break;
			case 244: /* Clock Speed 244 mhz */
				GpClockSpeedChange(244000000, 0x35010, 3); 
				break;
			case 248: /* Clock Speed 248 mhz */
				GpClockSpeedChange(248000000, 0x36010, 3); 
				break;
			case 252: /* Clock Speed 252 mhz */
				GpClockSpeedChange(252000000, 0x22000, 3); 
				break;
			case 256: /* Clock Speed 256 mhz */
				GpClockSpeedChange(256000000, 0x38010, 3); 
				break;
		}
		
		/* GP32 Video Init */
		gp32_video_init();

		/* Initialize the audio library */
		msdos_init_seal();
		
		/* Restore MAME Palette */
		gp32_mame_palette();
		
		/* Set Log Messages start at row 0 */
		gp32_gamelist_zero();
		
		/* take the first commandline argument without "-" as the game name */
		for (j = 1; j < argc; j++)
			if (argv[j][0] != '-') break;
	
		gm_lowercase(argv[j],gm_lstrlen(argv[j]));
		game_index = -1;
	
		/* do we have a driver for this? */
		for (i = 0; drivers[i] && (game_index == -1); i++)
		{
			/*if (strcasecmp(argv[j],drivers[i]->name) == 0)*/
			if (gm_compare(argv[j],drivers[i]->name) == 0)
			{
				game_index = i;
				break;
			}
		}
	
		if (game_index == -1)
		{
			sprintf(text,"Game \"%s\" not supported\0",argv[1]);
			gp32_text_log(text);
			while(1); /* MAME never ends xD */
		}

		/* parse generic (os-independent) options */
		parse_cmdline (argc, argv, &options, game_index);
	
		{	/* Mish:  I need sample rate initialised _before_ rom loading for optional rom regions */
			extern int soundcard;

			if (soundcard == 0) {    /* silence, this would be -1 if unknown in which case all roms are loaded */
				Machine->sample_rate = 0; /* update the Machine structure to show that sound is disabled */
				options.samplerate=0;
			}
		}

		sprintf(text,"Loading \"%s\"...",drivers[game_index]->description);
		gp32_text_log(text);
	
		/* go for it */
		res = run_game (game_index , &options);
		
		/* Wait 5 seconds */
		if (res!=0)
			Delay(10000);

	} /* MAME never ends xD */
}
