/***************************************************************************

  machine.c

  Functions to emulate general aspects of the machine (RAM, ROM, interrupts,
  I/O ports)

***************************************************************************/

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "gpdef.h"
#include "gpstdlib.h"
#include "gpgraphic.h"
#include "gpstdio.h"
#include "gpfont.h"
#include "Z80.h"
#include "machine.h"
#include "vidhrdw.h"
#include "sndhrdw.h"
#include "roms.h"
#include "memmap.h"
#include "osdepend.h"

#define uclock_t clock_t
#define	uclock clock
#define UCLOCKS_PER_SEC CLOCKS_PER_SEC


/* CPU_SPEED is the speed of the CPU in Hz. It is used together with */
/* FRAMES_PER_SECOND to calculate how much CPU cycles must pass between */
/* interrupts. */
#define CPU_SPEED 3072000	/* 3.072 Mhz. Is this correct for Pac Man? */
#define FRAMES_PER_SECOND 60

extern char pause;

unsigned char RAM[0x10000];		/* 64k of RAM */

/* dip switches */
int dsw[2];
const struct DSW *dswsettings;


int speedcheat = 0;	/* a well known hack allows to make Pac Man run at four times */
					/* his usual speed. When we load the ROMs, we check if the */
					/* hack can be applied, and set this flag accordingly. */




/***************************************************************************

  Initialize the emulated machine (load the roms, initialize the various
  subsystems...). Returns 0 if successful.

***************************************************************************/
int init_machine(const char *gamename)
{
	int i;
	/* FILE *f; */
	/* char name[100]; */

	i = 0;
	while (gameinfo[i].name && strcmp(gamename,gameinfo[i].name) != 0)
		i++;

	if (readroms(RAM,gameinfo[i].rom,gamename) != 0)
		return 1;

	if (vh_init(gamename))
	{
		printf("Cannot initialize video emulation\n");
		return 1;
	}

	if (sh_init(gamename))
	{
		printf("Cannot initialize sound emulation\n");
		return 1;
	}

	dswsettings = gameinfo[i].dswsettings;
	dsw[0] = gameinfo[i].defaultdsw[0];
	dsw[1] = gameinfo[i].defaultdsw[1];

	/* read dipswitch settings from disk */
	/*
	sprintf(name,"%s/%s.dsw",gamename,gamename);
	if ((f = fopen(name,"rb")) != 0)
	{
		fread(dsw,1,2,f);
		fclose(f);
	}
	*/
	
	/* check if the loaded set of ROMs allows the Pac Man speed hack */
	if (RAM[0x180b] == 0xbe && RAM[0x1ffd] == 0x00)
		speedcheat = 1;
	else speedcheat = 0;

	return 0;
}



/***************************************************************************

  Run the emulation. Start the various subsystems and the CPU emulation.
  Returns non zero in case of error.

***************************************************************************/
int run_machine(const char *gamename)
{
	if (vh_start() == 0)	/* start the video hardware */
	{
		if (sh_start() == 0)	/* start the audio hardware */
		{
			reg StartRegs;
			/* FILE *f; */
			/* char name[100]; */


			IPeriod = CPU_SPEED / FRAMES_PER_SECOND;	/* Number of T-states per interrupt */
			ResetZ80(&StartRegs);
			Z80(&StartRegs);		/* start the CPU emulation */

			sh_stop();
			vh_stop();

			/* write dipswitch settings from disk */
			/*
			sprintf(name,"%s/%s.dsw",gamename,gamename);
			if ((f = fopen(name,"wb")) != 0)
			{
				fwrite(dsw,1,2,f);
				fclose(f);
			}
			*/
			
			return 0;
		}
		else printf("Unable to setup audio\n");

		vh_stop();
	}
	else printf("Unable to setup display\n");

	return 1;
}



/***************************************************************************

  Perform a memory read. This function is called by the CPU emulation.

***************************************************************************/
byte M_RDMEM (dword A)
{
	/* handle input ports (see memmap.h for details) */
	switch (A)
	{
		case IN0_PORT:
		{
			byte res = 0xff;


			osd_poll_joystick();
			if (osd_key_pressed(OSD_KEY_3)) {res &= ~IN0_CREDIT;}
			if (osd_key_pressed(OSD_KEY_F1)) res &= ~IN0_RACK_TEST;
			if (osd_key_pressed(OSD_KEY_DOWN) || osd_joy_down) {res &= ~IN0_DOWN;}
			if (osd_key_pressed(OSD_KEY_RIGHT) || osd_joy_right) {res &= ~IN0_RIGHT;}
			if (osd_key_pressed(OSD_KEY_LEFT) || osd_joy_left) {res &= ~IN0_LEFT;}
			if (osd_key_pressed(OSD_KEY_UP) || osd_joy_up) {res &= ~IN0_UP;}
			return res;
			break;
		}
		case IN1_PORT:
		{
			byte res = 0xff;


			if (osd_key_pressed(OSD_KEY_2)) res &= ~IN1_START2;
			if (osd_key_pressed(OSD_KEY_1)) {res &= ~IN1_START1;}
			if (osd_key_pressed(OSD_KEY_F2)) res &= ~IN1_TEST;
			return res;
			break;
		}

		case DSW1_PORT:
			return dsw[0];
			break;

		default:
			return RAM[A];
			break;
	}
}



/***************************************************************************

  Perform a memory write. This function is called by the CPU emulation.

***************************************************************************/
void M_WRMEM (dword A,byte V)
{
	A &= 0x7fff;	/* It looks like the Pac Man arcade machine has a 15 bit */
					/* path to RAM: we must clear the high bit otherwise CREDIT */
					/* and HIGHSCORE will not be displayed on the screen. */

	if (A <= ROM_END) return;	/* Do nothing, it's ROM */
	else if (vh_wrmem(A,V)) return;	/* the video hardware handled the write */
	else if (sh_wrmem(A,V)) return;	/* the sound hardware handled the write */
	else RAM[A] = V;
}



/***************************************************************************

  Interrupt handler. This function is called at regular intervals
  (determined by IPeriod) by the CPU emulation.

***************************************************************************/

int IntVector = 0;	/* Here we store the interrupt vector, if the code performs */
					/* an OUT to port $0. Not all games do it: many use */
					/* Interrupt Mode 1, which doesn't use an interrupt vector */
					/* (see Z80.c for details). */

int Interrupt(void)
{
	/*
	static uclock_t prev;
	uclock_t curr;
	*/
	
	static int prev;
	int curr;
	
	/* if the user pressed ESC, stop the emulation */
	if (osd_key_pressed(OSD_KEY_ESC)) CPURunning = 0;

	/* if TAB, go to dipswitch setup menu */
	if (osd_key_pressed(OSD_KEY_TAB)) setdipswitches(dsw,dswsettings);

	if (osd_key_pressed(OSD_KEY_P)) /* pause the game */
	{
		pause=1;
		struct DisplayText dt[] =
		{
			{ "PAUSED", RED_TEXT, 11, 20 },
			{ 0, 0, 0, 0 }
		};
		int key;


		displaytext(dt,0);

		while (osd_key_pressed(OSD_KEY_P));	/* wait for key release */
		do
		{
			key = osd_read_key();

			if (key == OSD_KEY_ESC) CPURunning = 0;
			else if (key == OSD_KEY_TAB)
			{
				setdipswitches(dsw,dswsettings);	/* might set CPURunning to 0 */
				displaytext(dt,0);
			}
		} while (CPURunning && key != OSD_KEY_P);
		while (osd_key_pressed(key));	/* wait for key release */
		pause=0;
	}

	vh_screenrefresh();	/* update screen */
	sh_update();	/* update sound */

	/* now wait until it's time to trigger the interrupt */
	do
	{
		curr = GpTickCountGet();
	} while ((curr - prev) < 1000/FRAMES_PER_SECOND);

	prev = curr;

	/* speed up cheat */
	if (speedcheat)
	{
		if (osd_key_pressed(OSD_KEY_CONTROL))
		{
			RAM[0x180b] = 0x01;
			RAM[0x1ffd] = 0xbd;
		}
		else
		{
			RAM[0x180b] = 0xbe;
			RAM[0x1ffd] = 0x00;
		}
	}

	if (RAM[INTERRUPT_ENABLE] == 0) return IGNORE_INT;
	else return IntVector;
}



/***************************************************************************

  This function is called by the CPU emulation when the EI instruction is
  executed. We don't need to do anything fancy.

***************************************************************************/
int InterruptsEnabled(void)
{
	return IGNORE_INT;
}



/***************************************************************************

  Execute an OUT instruction. This function is called by the CPU emulation.

  The Pac Man machine uses OUT to port $0 to set the interrupt vector, so
  we have to remember the value passed.

***************************************************************************/
void DoOut(byte A,byte V)
{
	if (sh_doout(A,V)) return;		/* the sound hardware handled the operation */

	/* OUT to port $0 is used to set the interrupt vector */
	if (A == 0) IntVector = V;
}



byte DoIn(byte A)
{
	byte res;


	if (sh_doin(A,&res)) return res;
	else return 0;
}



void Patch (reg *R)
{
}
