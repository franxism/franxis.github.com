/*
 * Configuration routines.
 *
 * 971219 support for mame.cfg by Valerio Verrando
 * 980402 moved out of msdos.c (N.S.), generalized routines (BW)
 */

#include "driver.h"
#include <ctype.h>

/* from video.c */
extern int gfx_width, gfx_height;

/* from sound.c */
extern int soundcard;
extern int use_emulated_ym3812;

/* from input.c */
extern int use_mouse, joystick;

static int mame_argc;
static char **mame_argv;
static int game;

/* from gp32_menu.c */
extern int gp32_frameskip;
extern int gp32_sound;
extern int gp32_freq;

/* from video.c */
extern int gp32_timer;

void parse_cmdline (int argc, char **argv, struct GameOptions *options, int game_index)
{
	mame_argc = argc;
	mame_argv = argv;
	game = game_index;

	options->frameskip = gp32_frameskip; /* 1,2,3... */
	options->norotate  = 0;
	options->ror       = 0;
	options->rol       = 0;
	options->flipx     = 0;
	options->flipy     = 0;

	/* read sound configuration */
	soundcard=gp32_sound; /* 1 sound, 0 not emulated sound, 2 no sound but emulated */

	use_emulated_ym3812=1;
	options->samplerate = 22050;
	options->samplebits = 8;

	/* read input configuration */
	use_mouse = 0;
	joystick  = 0;

	/* misc configuration */
	options->cheat      = 0;
	/*options->mame_debug = 0;*/
	
	/* Number of GP32 ticks in a second, depending on GP32 Frequency */
	/*gp32_timer=(gp32_freq*1898)/133;*/
	/*gp32_timer=(gp32_freq*1950)/133;*/
	gp32_timer=(gp32_freq*1950)/133;
	
}
