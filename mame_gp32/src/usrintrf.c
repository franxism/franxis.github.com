/*********************************************************************

  usrintrf.c

  Functions used to handle MAME's crude user interface.

*********************************************************************/

#include "driver.h"
#include "gp32_menu.h"

extern int need_to_clear_bitmap;	/* used to tell updatescreen() to clear the bitmap */

int showcopyright(void)
{
	/*dt[0].text = "PLEASE DO NOT DISTRIBUTE THE SOURCE CODE AND/OR THE EXECUTABLE "
			"APPLICATION WITH ANY ROM IMAGES.\n"
			"DOING AS SUCH WILL HARM ANY FURTHER DEVELOPMENT OF MAME AND COULD "
			"RESULT IN LEGAL ACTION BEING TAKEN BY THE LAWFUL COPYRIGHT HOLDERS "
			"OF ANY ROM IMAGES.";*/

	/* key = osd_read_key(1);
	while (osd_key_pressed(key)) ;*/	/* wait for key release */


	/*if (key == OSD_KEY_FAST_EXIT ||
		key == OSD_KEY_CANCEL ||
		key == OSD_KEY_ESC)
		return 1;*/

	osd_clearbitmap(Machine->scrbitmap);
	osd_update_display();
	return 0;
}


void showcredits(void)
{

	/*sprintf(buf,"The following people\ncontributed to this driver:\n\n%s",Machine->gamedrv->credits);
	displaymessagewindow(buf);*/
	
	need_to_clear_bitmap = 1;
	osd_update_display();
}

/*static int displaygameinfo(int selected)
{
	int i;
	char buf[2048];
	static char *cpunames[] =
	{
		"",
		"Z80",
		"I8085",
		"6502",
		"8086",
		"8035",
		"6803",
		"6805",
		"6809",
		"68000",
		"T-11",
		"S2650",
		"TMS34010",
		"TMS9900"
	};
	static char *soundnames[] =
	{
		"",
		"<custom>",
		"<samples>",
		"DAC",
		"AY-3-8910",
		"YM2203",
		"YM2151",
		"YM2151",
		"YM2413",
		"YM2610",
		"YM3812",
		"YM3526",
		"SN76496",
		"Pokey",
		"NES",
		"Astrocade 'IO' chip",
		"Namco",
		"TMS5220",
		"VLM5030",
		"ADPCM samples",
		"OKIM6295 ADPCM",
		"MSM5205 ADPCM",
		"HC-55516 CVSD",
	};
	int sel;


	sel = selected - 1;


	sprintf(buf,"%s\n%s %s\n\nCPU:\n",Machine->gamedrv->description,Machine->gamedrv->year,Machine->gamedrv->manufacturer);
	i = 0;
	while (i < MAX_CPU && Machine->drv->cpu[i].cpu_type)
	{
		sprintf(&buf[strlen(buf)],"%s %d.%06d MHz",
				cpunames[Machine->drv->cpu[i].cpu_type & ~CPU_FLAGS_MASK],
				Machine->drv->cpu[i].cpu_clock / 1000000,
				Machine->drv->cpu[i].cpu_clock % 1000000);

		if (Machine->drv->cpu[i].cpu_type & CPU_AUDIO_CPU)
			gm_strcat(buf," (sound)");

		gm_strcat(buf,"\n");

		i++;
	}

	gm_strcat(buf,"\nSound");
	if (Machine->drv->sound_attributes & SOUND_SUPPORTS_STEREO)
		sprintf(&buf[strlen(buf)]," (stereo)");
	gm_strcat(buf,":\n");

	i = 0;
	while (i < MAX_SOUND && Machine->drv->sound[i].sound_type)
	{
		if (Machine->drv->sound[i].sound_type >= SOUND_AY8910 &&
				Machine->drv->sound[i].sound_type < SOUND_NAMCO)
			sprintf(&buf[strlen(buf)],"%d x ",((struct AY8910interface *)Machine->drv->sound[i].sound_interface)->num);

		sprintf(&buf[strlen(buf)],"%s",
				soundnames[Machine->drv->sound[i].sound_type]);

		if (Machine->drv->sound[i].sound_type >= SOUND_AY8910 &&
				Machine->drv->sound[i].sound_type < SOUND_NAMCO)
			sprintf(&buf[strlen(buf)]," %d.%06d MHz",
					((struct AY8910interface *)Machine->drv->sound[i].sound_interface)->baseclock / 1000000,
					((struct AY8910interface *)Machine->drv->sound[i].sound_interface)->baseclock % 1000000);

		gm_strcat(buf,"\n");

		i++;
	}

	if (Machine->drv->video_attributes & VIDEO_TYPE_VECTOR)
		sprintf(&buf[strlen(buf)],"\nVector Game\n");
	else
	{
		sprintf(&buf[strlen(buf)],"\nScreen resolution:\n");
		if (Machine->gamedrv->orientation & ORIENTATION_SWAP_XY)
			sprintf(&buf[strlen(buf)],"%d x %d (vert) %d Hz\n",
					Machine->drv->visible_area.max_y - Machine->drv->visible_area.min_y + 1,
					Machine->drv->visible_area.max_x - Machine->drv->visible_area.min_x + 1,
					Machine->drv->frames_per_second);
		else
			sprintf(&buf[strlen(buf)],"%d x %d (horz) %d Hz\n",
					Machine->drv->visible_area.max_x - Machine->drv->visible_area.min_x + 1,
					Machine->drv->visible_area.max_y - Machine->drv->visible_area.min_y + 1,
					Machine->drv->frames_per_second);
		sprintf(&buf[strlen(buf)],"%d colors ",Machine->drv->total_colors);
		if (Machine->drv->video_attributes & VIDEO_SUPPORTS_16BIT)
			gm_strcat(buf,"(16-bit required)\n");
		else if (Machine->drv->video_attributes & VIDEO_MODIFIES_PALETTE)
			gm_strcat(buf,"(dynamic)\n");
		else gm_strcat(buf,"(static)\n");
	}

	gm_strcat(buf,"\n\tMAME ");	// \t means that the line will be centered
	gm_strcat(buf,mameversion);


	displaymessagewindow(buf);

	if (osd_key_pressed_memory(OSD_KEY_FAST_EXIT)
			|| osd_key_pressed_memory (OSD_KEY_CANCEL)
			|| osd_key_pressed_memory(OSD_KEY_ENTER)
			|| osd_key_pressed_memory(OSD_KEY_LEFT)
			|| osd_key_pressed_memory(OSD_KEY_RIGHT)
			|| osd_key_pressed_memory(OSD_KEY_UP)
			|| osd_key_pressed_memory(OSD_KEY_DOWN))
		sel = -1;

	if (osd_key_pressed_memory(OSD_KEY_CONFIGURE))
		sel = -2;

	if (sel == -1 || sel == -2)
	{
		// tell updatescreen() to clean after us
		need_to_clear_bitmap = 1;
	}

	return sel + 1;
}*/

int showgamewarnings(void)
{
	/*int i;
	int key;
	int counter;
	char buf[2048];
	struct DisplayText dt[3];*/

	/*
	if (Machine->gamedrv->flags)
	{
		gm_strcpy(buf, "There are known problems with this game:\n\n");

		if (Machine->gamedrv->flags & GAME_IMPERFECT_COLORS)
		{
			gm_strcat(buf, "The colors aren't 100% accurate.\n");
		}

		if (Machine->gamedrv->flags & GAME_WRONG_COLORS)
		{
			gm_strcat(buf, "The colors are completely wrong.\n");
		}

		if (Machine->gamedrv->flags & GAME_NOT_WORKING)
		{
			const struct GameDriver *main;
			int foundworking;

			gm_strcpy(buf,"THIS GAME DOESN'T WORK PROPERLY");
			if (Machine->gamedrv->clone_of) main = Machine->gamedrv->clone_of;
			else main = Machine->gamedrv;

			foundworking = 0;
			i = 0;
			while (drivers[i])
			{
				if (drivers[i] == main || drivers[i]->clone_of == main)
				{
					if ((drivers[i]->flags & GAME_NOT_WORKING) == 0)
					{
						if (foundworking == 0)
							gm_strcat(buf,"\n\n\nThere are clones of this game which work. They are:\n\n");
						foundworking = 1;

						sprintf(&buf[strlen(buf)],"%s\n",drivers[i]->name);
					}
				}
				i++;
			}
		}

		gm_strcat(buf,"\n\n\nType OK to continue");

		dt[0].text = buf;
		dt[0].color = DT_COLOR_RED;
		dt[0].x = 0;
		dt[0].y = 0;
		dt[1].text = 0;
		displaytext(dt,1,1);

		do
		{
			key = osd_read_key(1);
			if (key == OSD_KEY_ESC ||
				key == OSD_KEY_CANCEL ||
				key == OSD_KEY_FAST_EXIT)
			{
				while (osd_key_pressed(key)) ;	// wait for key release
				return 1;
			}
		} while (key != OSD_KEY_O && key != OSD_KEY_LEFT);

		while (osd_key_pressed(OSD_KEY_K) == 0 && osd_key_pressed(OSD_KEY_RIGHT) == 0) ;
		while (osd_key_pressed(OSD_KEY_K) || osd_key_pressed(OSD_KEY_RIGHT)) ;
	}


	osd_clearbitmap(Machine->scrbitmap);

	// don't stay on screen for more than 10 seconds
	counter = 10 * Machine->drv->frames_per_second;

	while (displaygameinfo(1) == 1 && --counter > 0)
		osd_update_display();
	*/

	osd_clearbitmap(Machine->scrbitmap);
	osd_update_display();
	return 0;
}

int handle_user_interface(void)
{
	int i;
	/*if (osd_key_pressed_memory (OSD_KEY_FAST_EXIT)) return 1;*/

	/* if the user pressed ESC, stop the emulation */
	/*if (osd_key_pressed_memory(OSD_KEY_CANCEL))*/
	/*	return 1; */

	/* if the user pressed F3, reset the emulation */
	if (osd_key_pressed_memory(OSD_KEY_RESET_MACHINE))
		machine_reset();


	if (osd_key_pressed(OSD_KEY_PAUSE)) /* pause the game */
	{
		for(i=0;i<15;i++) {
			if (osd_key_pressed(OSD_KEY_CANCEL)) return 1;
			if (!osd_key_pressed(OSD_KEY_PAUSE)) break;
			Delay(100);
		}
		
		/* Screen change orientation */
		if(i<15) {
			gp32_rotate=(gp32_rotate==0?1:0);
			gp32_adjust_display();
			gp32_clear_screen();
			return 0;
		}

		osd_sound_enable(0);
		
		gp32_text_pause();
		while(osd_key_pressed(OSD_KEY_PAUSE))
			Delay(100);
		
		/* Wait Unpause */
		while(!osd_key_pressed(OSD_KEY_PAUSE)) {
			if (osd_key_pressed(OSD_KEY_CANCEL)) return 1;
			Delay(100);

			/*
			if (need_to_clear_bitmap)
			{
				osd_clearbitmap(Machine->scrbitmap);
				need_to_clear_bitmap = 0;
			}
			(*Machine->drv->vh_update)(Machine->scrbitmap,0);

			osd_update_display();
			osd_update_audio();
			*/

		}
		while(osd_key_pressed(OSD_KEY_PAUSE))
			Delay(100);
			
		osd_sound_enable(1);
	}

	return 0;
}
