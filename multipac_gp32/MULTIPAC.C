/***************************************************************************

  PacMan arcade machine emulator

  Usage:

  multipac [name of the game to run] [options]

  for example

  multipac mspacman    will run Ms Pac Man

  options:
  -vesa   use standard 640x480x256 VESA mode instead of custom video mode
  -nosound  turn off sound
  -nojoy  don't poll joystick

  Games currently supported:
  - Pac Man
  - Ms Pac Man (bootleg version only)
  - Crush Roller (wrong colors)
  - various Pac Man variations, for example:
    - Pac Man modification (official update by Midway, harder)
    - Namco Pac Man (Pac Man with Namco copyright and different ghost names)
    - Pac Man 4x (Pac Man with speed hack)
    - Puck Man (Pac Man with different maze)
    - Hangly Man (another modified maze)
  You can run any Pac Man clone, even if it isn't directly supported. For
  example if you have six roms called foo.5e, foo.5f, foo.6e ... foo.6j,
  just create a directory named "foo", put the roms into it, and launch
  the emulator with "pacman foo".


  Z80 engine by Marat Fayzulin and Marcel de Kogel
  Emulator framework built by Allard van der Bas (avdbas@wi.leidenuniv.nl)
                          and Nicola Salmoria (MC6489@mclink.it)
  Very special thanks to Sergio Munoz for the precious information about
  the sound hardware.
  Video mode created using Tweak 1.6b by Robert Schmidt, who also wrote
  TwkUser.c.
  Thanks to Gary Walton for his help in making the Crush Roller colors better.
  Allegro library by Shawn Hargreaves, 1994/96
  SEAL Synthetic Audio Library API Interface Copyright (C) 1995, 1996
  Carlos Hasan. All Rights Reserved.


  If you find out something useful, don't hesitate to submit it to :
  The arcade emultion programming repository.

  So it will be available to everyone at :
  http://valhalla.ph.tn.tudelft.nl/emul8

  Send it by email to :
  avdbas@wi.leidenuniv.nl


  Known issues:
  - Sound is not perfect yet (music is ok, other sounds arent't). But it's a
    start.

  - Blinky and Pinky seem to be shifted one pixel to the right. This is really
    annoying, but I can't seem to be able to understand why. Maybe there is an
	additional "sprite offset" register somewhere? Or did the original just
	behave this way?
	Note that we can't fix it by just moving sprites 0 and 1 one pixel to the
	left, because when Pac Man eats a power pill the sprites order is changed
	so that Pac Man is drawn over the ghosts. It becomes sprite 0, and Blinky
	becomes sprite 4.

  - The palette of Crush Roller is hopelessly wrong. I played the game a
    couple of times some 15 years ago... can't remember anything.

***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gpdef.h"
#include "gpstdlib.h"
#include "gpgraphic.h"
#include "gpstdio.h"
#include "gpfont.h"
#include "machine.h"
#include "osdepend.h"

#define DEFAULT_NAME "pacman"

/*
		pacman             ->      Pac Man
		mspacman           ->      Ms Pac Man
		crush              ->      Crush Roller
		hangly             ->      Hangly Man
		pacmod             ->      Pac Man Mod
		puckman            ->      Puck Man
		namcopac           ->      Pac Man Namco
		pacplus            ->      Pac Man Plus
		piranha            ->      Piranha

----

		PACMAN HACKS:
		generic -> Pac Man (generic clone/hack)
		abortman -> 	Abort Man
		baby -> Baby Pac Man
		brakman -> Brakman
		crazypac -> Crazy Pac
		dizzy -> Dizzy Ghost
		europac -> Euro Pac
		hearts ->  Pac Man Hearts
		mazeman  	-> 	Maze Man
		pac2600		->	Pac 2600
		pacman3d 	-> 	Pac Man 3D
		pcrunchy 	->	Pac Man Crunchy
		pacstrm 	->	Pac Man Desert Storm
		pacinvis 	-> 	Pac Man Inviso
		pacjail 	-> 	Pac Man Jail
		pacmini 	-> 	Pac Man Mini
		pacshuf 	->	Pac Man Shuffle
		pacweird 	->	Pac Man Six Map Weird
		snowpac 	->	Pac Man Snowy Day
		pacspeed 	->	Pac Man Speedy
		pengman 	->	PengMan
		puckman2 	->	Puck Man 2
		puckren 	->	Puck Man Renaissance
		roboman 	->	Robo Man
		sumelton 	->	Summertime Elton
		ultrapac 	->	UltraPac
		ultra2 		->	Ultra Pacman TwoBit Score Mazes
		vectrpac 	->	Vector Pac Man
		xensrev 	->	Xens Revenge
		vectxens 	->	Xens Revenge Vector

----

		MS PACMAN HACKS:
		genericm -> Ms Pac Man (Generic clone/hack)
		cookiem -> Cookie Mon
		heartbrn -> Ms Heart Burn
		mrpacman  -> Mr Pac Man
		msbaby -> Ms Baby Pac Man
		msberzk -> Ms Pac Man Berzerk
		msdroid -> Ms Droid
		mselton -> Ms Elton
		msf1pac -> Ms F1 Pac Man
		mshangly -> Ms Hangly Man
		mshearts -> Ms Pac Man Hearts
		msmini -> Ms Pac Man Mini
		mspacad -> Ms Pac Man After Dark
		mspaccm -> Ms Pac Crazy Mazes
		mspacdx -> Ms Pac Man Deluxe
		mspacren -> Ms Pac Man Renaissance
		mspacrip -> Ms Pac Mortem
		mspacsm -> Ms Pac Strange Mazes
		msrumble -> Ms Pac Rumble
		msultra -> Ms Ultra Pac Man
		msvectr  -> Vector Ms Pac Man
		punleash -> Pac Man Unleashed
		sueworld -> Sue's World
		msyakman -> Ms Yak Man

----

	
*/


void GpMain(void *arg)
{
int argc=2;
char *argv[] = {"multipac","builtin"};
int ExKey;

osd_init(argc,argv);

/* Show Intro Screen */
intro_screen();

while(1) {

	/* GP32 Game Selection Menu */
	select_game(argv);

	if (init_machine(argc > 1 && argv[1][0] != '-' ? argv[1] : DEFAULT_NAME) == 0)
	{
/*		extern int speedcheat; */
/*
		printf("\nPLEASE DO NOT DISTRIBUTE THE SOURCE FILES OR THE EXECUTABLE WITH ROM IMAGES.\n"
			   "DOING SO WILL HARM FURTHER EMULATOR DEVELOPMENT AND WILL CONSIDERABLY ANNOY\n"
			   "THE RIGHTFUL COPYRIGHT HOLDERS OF THOSE ROM IMAGES AND CAN RESULT IN LEGAL\n"
			   "ACTION UNDERTAKEN BY EARLIER MENTIONED COPYRIGHT HOLDERS.\n"
			   "\n\n"
			   "Quick keys : 3       Insert coin\n"
			   "             1       Start 1 player game\n"
			   "             2       Start 2 player game\n"
			   "             Arrows  Move around\n"
			   "             F1      Skip level\n"
			   "             F2      Test mode\n"
			   "             Tab     Change dip switch settings\n");
		if (speedcheat)
			printf("             CTRL    Speed up cheat\n");
		printf("             P       Pause\n"
			   "             F12     Save a screen snapshot\n"
			   "             ESC     Exit emulator\n"
			   "\n\n"
			   "Press <ENTER> to continue.\n");

		getchar();
*/
		if (1 /*osd_init(argc,argv) == 0 */)
		{
			if (run_machine(argc > 1 && argv[1][0] != '-' ? argv[1] : DEFAULT_NAME) != 0)
    text_out(250,10,"Error1");

			osd_exit();
		}
		else text_out(250,10,"Error2");
	}
	else text_out(250,10,"Error3");

}
}
