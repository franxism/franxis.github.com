/***************************************************************************

  common.c

  Generic functions used in different emulators.
  There's not much for now, but it could grow in the future... ;-)

***************************************************************************/

#include <stdio.h>
#include <string.h>
#include "gpdef.h"
#include "gpstdio.h"
#include "gpstdlib.h"
#include "gpgraphic.h"
#include "gpfont.h"
#include "gpmem.h"
#include "Z80.h"
#include "roms.h"
#include "vidhrdw.h"
#include "osdepend.h"

/*
#include "rom_pacman_5e.h"
#include "rom_pacman_5f.h"
#include "rom_pacman_6e.h"
#include "rom_pacman_6f.h"
#include "rom_pacman_6h.h"
#include "rom_pacman_6j.h"
*/

/***************************************************************************

  Read ROMs into memory.

  Arguments:
  unsigned char *dest - Pointer to the start of the memory region
                        where the ROMs will be loaded.

  const struct RomModule *romp - pointer to an array of Rommodule structures,
                                 as defined in common.h.

  const char *basename - Name of the directory where the files are
                         stored. The function also supports the
						 control sequence %s in file names: for example,
						 if the RomModule gives the name "%s.bar", and
						 the basename is "foo", the file "foo/foo.bar"
						 will be loaded.

***************************************************************************/
int readroms(unsigned char *dest,const struct RomModule *romp,const char *basename)
{
 F_HANDLE fHandle;
	unsigned long dwDummy;
	char buf[100];
	char name[100];

	while (romp->name)
	{
		sprintf(buf,romp->name,basename);
		sprintf(name,"gp:\\gpmm\\multipac\\%s\\%s",basename,buf);

/*
		if (strcmp(basename,"builtin")==0) {
			if(strcmp(buf,"builtin.5e")==0) {
				memcpy(dest+romp->offset,rom_pacman_5e,romp->size);
			}
			if(strcmp(buf,"builtin.5f")==0) {
				memcpy(dest+romp->offset,rom_pacman_5f,romp->size);
			}
			if(strcmp(buf,"builtin.6e")==0) {
			    	memcpy(dest+romp->offset,rom_pacman_6e,romp->size);
			}
			if(strcmp(buf,"builtin.6f")==0) {
			   	memcpy(dest+romp->offset,rom_pacman_6f,romp->size);
			}
			if(strcmp(buf,"builtin.6h")==0) {
			   	memcpy(dest+romp->offset,rom_pacman_6h,romp->size);
			}
			if(strcmp(buf,"builtin.6j")==0) {
				memcpy(dest+romp->offset,rom_pacman_6j,romp->size);
			}
		} else {
*/
     		if((GpFileOpen(name, OPEN_R, &fHandle))!=SM_OK) {
			text_out(0,0,"ERROR: ROM Not Found");
			text_out(0,20,name);
			Delay(3000);
			return 1;
		}

		if((GpFileRead(fHandle, dest + romp->offset, romp->size, &dwDummy))!=SM_OK) {
			text_out(0,0,"ERROR: Corrupt ROM");
			text_out(0,20,name);
			Delay(3000);
     			GpFileClose(fHandle);
			return 1;
		}
     		
     		GpFileClose(fHandle);

/*			}*/
			
		romp++;
	}

	return 0;
}



/***************************************************************************

  Function to convert the information stored in the graphic roms into a
  more usable format.

  Back in the early '80s, arcade machines didn't have the memory or the
  speed to handle bitmaps like we do today. They used "character maps",
  instead: they had one or more sets of characters (usually 8x8 pixels),
  which would be placed on the screen in order to form a picture. This was
  very fast: updating a character mapped display is, rougly speaking, 64
  times faster than updating an equivalent bitmap display, since you only
  modify groups of 8x8 pixels and not the single pixels. However, it was
  also much less versatile than a bitmap screen, since with only 256
  characters you had to do all of your graphics. To overcome this
  limitation, some special hardware graphics were used: "sprites". A sprite
  is essentially a bitmap, usually larger than a character, which can be
  placed anywhere on the screen (not limited to character boundaries) by
  just telling the hardware the coordinates. Moreover, sprites can be
  flipped along the major axis just by setting the appropriate bit (some
  machines can flip characters as well). This saves precious memory, since
  you need only one copy of the image instead of four.

  What about colors? Well, the early machines had a limited palette (let's
  say 16-32 colors) and each character or sprite could not use all of them
  at the same time. Characters and sprites data would use a limited amount
  of bits per pixel (typically 2, which allowed them to address only four
  different colors). You then needed some way to tell to the hardware which,
  among the available colors, were the four colors. This was done using a
  "color attribute", which typically was an index for a lookup table.

  OK, after this brief and incomplete introduction, let's come to the
  purpose of this section: how to interpret the data which is stored in
  the graphic roms. Unfortunately, there is no easy answer: it depends on
  the hardware. The easiest way to find how data is encoded, is to start by
  making a bit by bit dump of the rom. You will usually be able to
  immediately recognize some pattern: if you are lucky, you will see
  letters and numbers right away, otherwise you will see something which
  looks like letters and numbers, but with halves switched, dilated, or
  something like that. You'll then have to find a way to put it all
  together to obtain our standard one byte per pixel representation. Two
  things to remember:
  - keep in mind that every pixel has typically two (or more) bits
    associated with it, and they are not necessarily near to each other.
  - characters might be rotated 90 degrees. That's because many games used a
    tube rotated 90 degrees. Think how your monitor would look like if you
	put it on its side ;-)

  After you have successfully decoded the characters, you have to decode
  the sprites. This is usually more difficult, because sprites are larger,
  maybe have more colors, and are more difficult to recognize when they are
  messed up, since they are pure graphics without letters and numbers.
  However, with some work you'll hopefully be able to work them out as
  well. As a rule of thumb, the sprites should be encoded in a way not too
  dissimilar from the characters.

***************************************************************************/
int readbit(const unsigned char *src,int bitnum)
{
	int bits;


	bits = src[bitnum / 8];

	bitnum %= 8;
	while (bitnum-- > 0) bits <<= 1;

	if (bits & 0x80) return 1;
	else return 0;
}

void gfxdecode(unsigned char *dest,const unsigned char *src,const struct GfxLayout *gl)
{
	int c,plane,x,y,offs;


	memset(dest,0,gl->total * gl->width * gl->height);		/* clear destination array */
	for (c = 0;c < gl->total;c++)
	{
		for (plane = 0;plane < gl->planes;plane++)
		{
			offs = c * gl->charincrement + plane * gl->planeincrement;
			for (y = 0;y < gl->height;y++)
			{
				for (x = 0;x < gl->width;x++)
				{
					dest[c * gl->width * gl->height + y * gl->width + x] <<= 1;
					dest[c * gl->width * gl->height + y * gl->width + x] +=
							readbit(src,offs + gl->yoffset[y] + gl->xoffset[x]);
				}
			}
		}
	}
}



void setdipswitches(int *dsw,const struct DSW *dswsettings)
{
	struct DisplayText dt[40];
	int settings[20];
	int i,s,key;
	int total;


	total = 0;
	while (dswsettings[total].num != -1)
	{
		int msk,val;


		msk = dswsettings[total].mask;
		if (msk == 0) return;	/* error in DSW definition, quit */
		val = dsw[dswsettings[total].num];
		while ((msk & 1) == 0)
		{
			val >>= 1;
			msk >>= 1;
		}
		settings[total] = val & msk;

		total++;
	}

	s = 0;
	do
	{
		for (i = 0;i < total;i++)
		{
			dt[2 * i].color = (i == s) ? YELLOW_TEXT : WHITE_TEXT;
			dt[2 * i].text = dswsettings[i].name;
			dt[2 * i].x = 2 + FIRST_VISIBLE_COLUMN;
			dt[2 * i].y = 2 * i + (V_CHARS - 2 * total - 1) / 2;
			dt[2 * i + 1].color = (i == s) ? YELLOW_TEXT : WHITE_TEXT;
			dt[2 * i + 1].text = dswsettings[i].values[settings[i]];
			dt[2 * i + 1].x = LAST_VISIBLE_COLUMN - 2 - strlen(dt[2 * i + 1].text);
			dt[2 * i + 1].y = dt[2 * i].y;
		}
		dt[2 * i].text = 0;	/* terminate array */

		displaytext(dt,1);

		key = osd_read_key();

		switch (key)
		{
			case OSD_KEY_DOWN:
				if (s < total - 1) s++;
				break;

			case OSD_KEY_UP:
				if (s > 0) s--;
				break;

			case OSD_KEY_RIGHT:
				if (dswsettings[s].reverse == 0)
				{
					if (dswsettings[s].values[settings[s] + 1] != 0) settings[s]++;
				}
				else
				{
					if (settings[s] > 0) settings[s]--;
				}
				break;

			case OSD_KEY_LEFT:
				if (dswsettings[s].reverse == 0)
				{
					if (settings[s] > 0) settings[s]--;
				}
				else
				{
					if (dswsettings[s].values[settings[s] + 1] != 0) settings[s]++;
				}
				break;
		}
	} while (key != OSD_KEY_TAB && key != OSD_KEY_ESC);

	while (osd_key_pressed(key));	/* wait for key release */

	if (key == OSD_KEY_ESC) CPURunning = 0;

	while (--total >= 0)
	{
		int msk;


		msk = dswsettings[total].mask;
		while ((msk & 1) == 0)
		{
			settings[total] <<= 1;
			msk >>= 1;
		}

		dsw[dswsettings[total].num] = (dsw[dswsettings[total].num] & ~dswsettings[total].mask) | settings[total];
	}

	vh_screenrefresh();
}
