/***************************************************************************

  vidhrdw.c

  Functions to emulate the video hardware of the machine.

***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Z80.h"
#include "machine.h"
#include "vidhrdw.h"
#include "roms.h"
#include "memmap.h"
#include "osdepend.h"


unsigned char chars[8 * 8 * TOTAL_CHARS];	/* character bitmaps */
unsigned char sprites[SPRITE_WIDTH * SPRITE_HEIGHT * TOTAL_SPRITES];	/* sprite bitmaps */
unsigned char tmpbitmap[BITMAP_WIDTH * BITMAP_HEIGHT];	/* temporary bitmap used to hold the */
														/* character mapped part of the screen */
unsigned char *scrbitmap;
unsigned char dirtybuffer[VIDEO_RAM_SIZE];	/* keep track of modified portions of the screen */
											/* to speed up video refresh */
const unsigned char *palette;
const unsigned char *colortable;

unsigned char remappedtable[4*COLOR_CODES];
unsigned char backgroundpen;



/***************************************************************************

  Initialize the video hardware. Returns 0 if successful.
  This usually involves no more than loading the graphics roms, decoding
  them and store the graphics somewhere.

  In the case of MultiPac, since we support several different games, we also
  pick the appropriate palette and lookup table.

***************************************************************************/
int vh_init(const char *gamename)
{
	unsigned char *tmpstorage;
	int i;


	if ((tmpstorage = gm_malloc(0x2000)) == 0)
		return 1;

	i = 0;
	while (gamevidinfo[i].name && strcmp(gamename,gamevidinfo[i].name) != 0)
		i++;

	if (readroms(tmpstorage,gamevidinfo[i].gfxrom,gamename) != 0)
	{
		free(tmpstorage);
		return 1;
	}

	gfxdecode(chars,tmpstorage,gamevidinfo[i].charlayout);
	gfxdecode(sprites,tmpstorage + 0x1000,gamevidinfo[i].spritelayout);

	free(tmpstorage);

	palette = gamevidinfo[i].palette;
	colortable = gamevidinfo[i].colortable;

	return 0;
}



/***************************************************************************

  Start the video hardware emulation, that is set up a gfx mode, load the
  appropriate palette, and return. Returns 0 if successful.

***************************************************************************/
int vh_start(void)
{
	int i;
	unsigned char pens[TOTAL_COLORS];


	if ((scrbitmap = osd_create_display(BITMAP_WIDTH,BITMAP_HEIGHT)) == 0)
		return 1;

	for (i = 0;i < TOTAL_COLORS;i++)
		pens[i] = osd_obtain_pen(palette[3*i],palette[3*i+1],palette[3*i+2]);

	backgroundpen = pens[0];
	for (i = 0;i < 4*COLOR_CODES;i++)
		remappedtable[i] = pens[colortable[i]];

	memset(scrbitmap,backgroundpen,BITMAP_WIDTH * BITMAP_HEIGHT);

	return 0;
}



/***************************************************************************

  Stop the video hardware emulation.

***************************************************************************/
void vh_stop(void)
{
	osd_close_display();
}



/***************************************************************************

  Handle a write to memory.
  This function is called when the emulated code writes to RAM.

  Arguments:
  dword A - Memory address to write to.
  byte V - Value to write into memory.

  If the address given concerns the video hardware, the write is performed
  (together with additional operations which might be required by the video
  hardware) and the function returns non zero. Otherwise, it returns 0.

  In the case of Pac Man, all we do is keep track of writes to Video or
  Color RAM, marking the location as 'dirty' so we can update the screen
  bitmap before redrawing it.

***************************************************************************/
int vh_wrmem(dword A,byte V)
{
	if (A >= VIDEO_RAM_START && A < VIDEO_RAM_START + VIDEO_RAM_SIZE)
	{
		if (RAM[A] != V)
		{
			dirtybuffer[A - VIDEO_RAM_START] = 1;

			RAM[A] = V;
		}

		return 1;
	}
	else if (A >= COLOR_RAM_START && A < COLOR_RAM_START + VIDEO_RAM_SIZE)
	{
		if (RAM[A] != V)
		{
			dirtybuffer[A - COLOR_RAM_START] = 1;

			RAM[A] = V;
		}

		return 1;
	}
	else return 0;
}



void drawchar(unsigned char *bitmap,int charcode,int color,int flipx,int flipy,int sx,int sy)
{
	int x,y;
	const unsigned char *chardata;
	const unsigned char *paldata;
	unsigned char *bm;


	if (sx < FIRST_VISIBLE_COLUMN || sx >= LAST_VISIBLE_COLUMN ||
			sy < FIRST_VISIBLE_ROW || sy >= LAST_VISIBLE_ROW)
		return;

	chardata = &chars[8*8 * (charcode % TOTAL_CHARS)];
	paldata = &remappedtable[4 * (color % COLOR_CODES)];
	bm = &bitmap[8 * (BITMAP_WIDTH * sy + sx)];

	if (flipx)
	{
		if (flipy)	/* XY flip */
		{
			bm += 7 * BITMAP_WIDTH + 8;
			for (y = 0;y < 8;y++)
			{
				for (x = 0;x < 8;x++)
				{
					*(--bm) = paldata[*(chardata++)];
				}
				bm -= BITMAP_WIDTH - 8;
			}
		}
		else 	/* X flip */
		{
			bm += 8;
			for (y = 0;y < 8;y++)
			{
				for (x = 0;x < 8;x++)
				{
					*(--bm) = paldata[*(chardata++)];
				}
				bm += BITMAP_WIDTH + 8;
			}
		}
	}
	else
	{
		if (flipy)	/* Y flip */
		{
			bm += 7 * BITMAP_WIDTH;
			for (y = 0;y < 8;y++)
			{
				for (x = 0;x < 8;x++)
				{
					*(bm++) = paldata[*(chardata++)];
				}
				bm -= BITMAP_WIDTH + 8;
			}
		}
		else		/* normal */
		{
			for (y = 0;y < 8;y++)
			{
				for (x = 0;x < 8;x++)
				{
					*(bm++) = paldata[*(chardata++)];
				}
				bm += BITMAP_WIDTH - 8;
			}
		}
	}
}



void drawsprite(unsigned char *bitmap,int spritecode,int color,int flipx,int flipy,int sx,int sy)
{
	int ox,oy,ex,ey,x,y;
	const unsigned char *spritedata,*sd;
	const unsigned char *paldata;
	unsigned char *bm;
	int col;


	ox = sx;
	oy = sy;
	ex = sx + SPRITE_WIDTH;
	if (sx < 8 * FIRST_VISIBLE_COLUMN) sx = 8 * FIRST_VISIBLE_COLUMN;
	if (ex > 8 * LAST_VISIBLE_COLUMN) ex = 8 * LAST_VISIBLE_COLUMN;
	ey = sy + SPRITE_HEIGHT;
	if (sy < 8 * FIRST_VISIBLE_ROW) sy = 8 * FIRST_VISIBLE_ROW;
	if (ey > 8 * LAST_VISIBLE_ROW) ey = 8 * LAST_VISIBLE_ROW;
	spritedata = &sprites[SPRITE_WIDTH * SPRITE_HEIGHT * (spritecode % TOTAL_SPRITES)];
	paldata = &remappedtable[4 * (color % COLOR_CODES)];

	if (flipx)
	{
		if (flipy)	/* XY flip */
		{
			for (y = sy;y < ey;y++)
			{
				bm = &bitmap[BITMAP_WIDTH * y + sx];
				sd = &spritedata[SPRITE_WIDTH * (SPRITE_WIDTH-1-y+oy) + SPRITE_HEIGHT-1-sx+ox];
				for (x = sx;x < ex;x++)
				{
					col = paldata[*(sd--)];
					if (col != backgroundpen) *bm = col;
					bm++;
				}
			}
		}
		else 	/* X flip */
		{
			for (y = sy;y < ey;y++)
			{
				bm = &bitmap[BITMAP_WIDTH * y + sx];
				sd = &spritedata[SPRITE_WIDTH * (y-oy) + SPRITE_HEIGHT-1-sx+ox];
				for (x = sx;x < ex;x++)
				{
					col = paldata[*(sd--)];
					if (col != backgroundpen) *bm = col;
					bm++;
				}
			}
		}
	}
	else
	{
		if (flipy)	/* Y flip */
		{
			for (y = sy;y < ey;y++)
			{
				bm = &bitmap[BITMAP_WIDTH * y + sx];
				sd = &spritedata[SPRITE_WIDTH * (SPRITE_WIDTH-1-y+oy) + sx-ox];
				for (x = sx;x < ex;x++)
				{
					col = paldata[*(sd++)];
					if (col != backgroundpen) *bm = col;
					bm++;
				}
			}
		}
		else		/* normal */
		{
			for (y = sy;y < ey;y++)
			{
				bm = &bitmap[BITMAP_WIDTH * y + sx];
				sd = &spritedata[SPRITE_WIDTH * (y-oy) + sx-ox];
				for (x = sx;x < ex;x++)
				{
					col = paldata[*(sd++)];
					if (col != backgroundpen) *bm = col;
					bm++;
				}
			}
		}
	}
}



/***************************************************************************

  Redraw the screen.

***************************************************************************/
void vh_screenrefresh(void)
{
	int i,offs;


	/* for every character in the Video RAM, check if it has been modified */
	/* since last time and update it accordingly. */
	for (offs = 0;offs < VIDEO_RAM_SIZE;offs++)
	{
		int sx,sy,mx,my;


		if (dirtybuffer[offs])
		{
			dirtybuffer[offs] = 0;

	/* Even if Pac Man's screen is 28x36, the memory layout is 32x32. We therefore */
	/* have to convert the memory coordinates into screen coordinates. */
	/* Note that 32*32 = 1024, while 28*36 = 1008: therefore 16 bytes of Video RAM */
	/* don't map to a screen position. We don't check that here, however: range */
	/* checking is performed by drawchar(). */

			mx = offs / 32;
			my = offs % 32;

			if (mx <= 1)
			{
				sx = 29 - my;
				sy = mx + 34;
			}
			else if (mx >= 30)
			{
				sx = 29 - my;
				sy = mx - 30;
			}
			else
			{
				sx = 29 - mx;
				sy = my + 2;
			}

			drawchar(tmpbitmap,RAM[VIDEO_RAM_START + offs],RAM[COLOR_RAM_START + offs],0,0,sx,sy);
		}
	}

	memcpy(scrbitmap,tmpbitmap,BITMAP_WIDTH * BITMAP_HEIGHT);

	/* Draw the sprites. Note that it is important to draw them exactly in this */
	/* order, to have the correct priorities. */
	for (i = 5;i >= 2;i--)
	{
		drawsprite(scrbitmap,RAM[SPRITE_CODES_BASE + 2*i] >> 2,RAM[SPRITE_CODES_BASE+1 + 2*i],
				RAM[SPRITE_CODES_BASE + 2*i] & 2,RAM[SPRITE_CODES_BASE + 2*i] & 1,
				239-RAM[SPRITE_COORDS_BASE + 2*i],272-RAM[SPRITE_COORDS_BASE+1 + 2*i]);
	}
	/* "fix" the sprite offset bug by drawing them 1 pixel to the left. This doesn't */
	/* work when Pac Man eats an energy pill, but solves other problems like the */
	/* bridges in Crush Roller. */
	for (i = 1;i >= 0;i--)
	{
		drawsprite(scrbitmap,RAM[SPRITE_CODES_BASE + 2*i] >> 2,RAM[SPRITE_CODES_BASE+1 + 2*i],
				RAM[SPRITE_CODES_BASE + 2*i] & 2,RAM[SPRITE_CODES_BASE + 2*i] & 1,
				238-RAM[SPRITE_COORDS_BASE + 2*i],272-RAM[SPRITE_COORDS_BASE+1 + 2*i]);
	}

	osd_update_display();
}



/***************************************************************************

  Display text on the screen. If erase is 0, it superimposes the text on
  the last frame displayed.

***************************************************************************/
void displaytext(const struct DisplayText *dt,int erase)
{
	if (erase) memset(scrbitmap,backgroundpen,BITMAP_WIDTH * BITMAP_HEIGHT);

	while (dt->text)
	{
		int x;
		const unsigned char *c;


		x = dt->x;
		c = dt->text;

		while (*c)
		{
			if (*c != ' ')
			{
				if (*c >= '0' && *c <= '9')
					drawchar(scrbitmap,*c - '0' + NUMBERS_START,dt->color,0,0,x,dt->y);
				else drawchar(scrbitmap,*c - 'A' + LETTERS_START,dt->color,0,0,x,dt->y);
			}
			x++;
			c++;
		}

		dt++;
	}

	osd_update_display();
}
