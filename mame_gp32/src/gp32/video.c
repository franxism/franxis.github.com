
#include "driver.h"

#include "gpdef.h"
#include "gpstdio.h"
#include "gpstdlib.h"
#include "gpgraphic.h"
#include "gpfont.h"
#include "gpmem.h"
#include "gp32_mame.h"

/* Converts 8bit rgb values to a GP32 palette value */
#define GP_RGB24(r,g,b) ((((((r>>3))&0x1f)<<11)|((((g>>3))&0x1f)<<6)|((((b>>3))&0x1f)<<1))&0xfffe)
/*#define GP_RGB24(r,g,b) (((((r>>3))&0x1f)<<11)|((((g>>3))&0x1f)<<6)|((((b>>3))&0x1f)<<1))*/
/*#define GP_RGB24(r,g,b) (((r>>3)<<11)|((g>>3)<<6)|((b>>3)<<1))*/

#define BACKGROUND 0

static void update_screen_dummy(void);
void (*update_screen)(void) = update_screen_dummy;

int gp32_timer; /* It is set by gp32_menu.c->config.c, 2000 for 133 Mhz i.e. */

static struct osd_bitmap *scrbitmap;
static unsigned char current_palette[256][3];
static unsigned char dirtycolor[256];
static int dirtypalette;

/* Timing variables */
#define MEMORY 10
int prev[MEMORY];
int memory;
extern int frameskip;

int skiplines=0;
int skipcolumns=0;
int gfx_width=0;
int gfx_height=0;
static int viswidth;
static int visheight;
static int skiplinesmax;
static int skipcolumnsmax;
static int skiplinesmin;
static int skipcolumnsmin;
static int gfx_xoffset;
static int gfx_yoffset;
static int gfx_display_lines;
static int gfx_display_columns;
int throttle = 1;

/* GP32 Graphic Display Variables */
int game_width;
int game_height;
int gp32_xoffset;
int gp32_yoffset;
int gp32_xscreen;
int gp32_yscreen;
int gp32_width;
int gp32_height;
int gp32_rotate=0;

/* GP32 related stuff */
GPDRAWSURFACE gpDraw[2];	/* Video Buffers */
int nflip;			/* Video buffer index */

/* GP32 Video Init */
void gp32_video_init(void) {

	int i=0;

	/* GP32 Video Init */
	GpGraphicModeSet(8,0);  /* 320x240, 256 colores */

	for( i=0; i<2; i++ ) {
		GpLcdSurfaceGet( &gpDraw[i],i );
		GpRectFill( NULL, &gpDraw[i],0,0,gpDraw[i].buf_w,gpDraw[i].buf_h,0x0 );
	}
	nflip=0;
	GpSurfaceSet( &gpDraw[nflip] );
	GpLcdEnable();
	GpSurfaceFlip( &gpDraw[nflip] );

	
}

void gp32_mame_palette(void) {

	GP_PALETTEENTRY colorgp;

	/* Color #0 is black */
	colorgp = GP_RGB24(0,0,0);
   	GpPaletteEntryChange(0,1,&colorgp,0);	

	/* Color #1 is white */
	colorgp = GP_RGB24(255,255,255);
   	GpPaletteEntryChange(1,1,&colorgp,0);	
	
}

/* Update GP32 Screen */
void update_screen_gp32(void)
{
	register unsigned char *buffer_scr;
	register unsigned char *buffer_mem;
	register unsigned char *buffer_mem_line;
	register unsigned int buffer_mem_offset;
	register int x,y,px,pxi,py;

	buffer_scr = gpDraw[nflip].o_buffer;
	buffer_mem = scrbitmap->line[gp32_yoffset]+gp32_xoffset;
	buffer_mem_offset = (unsigned int)(scrbitmap->line[1] - scrbitmap->line[0]);
	
	if (!gp32_rotate) {
		
		y=0;
		pxi=gp32_xscreen*240;
		py=239-gp32_yscreen;
		while (y < gp32_height) {
			buffer_mem_line=buffer_mem;
			buffer_mem+=buffer_mem_offset;
			x=0;
			px=pxi;
			while (x < gp32_width) {
				buffer_scr[px+py] = *buffer_mem_line++;	
				x++;
				px+=240;
			}
			y++;
			py--;
		}
		
		
		/*
		for (y = 0; y < 256; y++)
		{
			for (x = 0; x < 256; x++)
			{
				buffer_scr[239-y+x*240] = scrbitmap->line[y][x];
			}
		}
		gp32_text_out_int(0,0,game_width);
		gp32_text_out_int(0,20,game_height);
		*/
		
	} else {
		y=0;
		pxi=239-gp32_xscreen;
		py=(319-gp32_yscreen)*240;
		while (y < gp32_height) {
			buffer_mem_line=buffer_mem;
			buffer_mem+=buffer_mem_offset;
			x=0;
			px=pxi;
			while (x < gp32_width) {
				buffer_scr[px+py] = *buffer_mem_line++;	
				x++;
				px--;
			}
			y++;
			py-=240;
			
		}
	}

	/* Flip Video Surface */
	GpSurfaceFlip( &gpDraw[nflip] );
	/*GpSurfaceSet( &gpDraw[nflip] );*/

	/* Change Video Buffer Number */
	nflip=!nflip;
	/*nflip=(nflip==0?1:0);*/
	/*nflip++;
	nflip=nflip%3;*/
}


void gp32_adjust_display(void) {

/*gp32_text_log_int(game_width);
gp32_text_log_int(game_height);
while(1);*/
	/* Image not rotated */
	if (gp32_rotate==0) {

		gp32_rotate=0;
		gp32_xoffset=0;
		gp32_yoffset=0;
		gp32_xscreen=0;
		gp32_yscreen=0;
		gp32_width=320;
		gp32_height=240;
		
		if (game_width>320)
			gp32_xoffset = (game_width-320) / 2;

		if (game_width<320)
			gp32_width = game_width;
			
		if (game_height>240) {
			gp32_yoffset = (game_height-240) / 2;
			if (game_width==336 && game_height==256) /* For Tetris */
				gp32_yoffset-=8;
			if (game_width==256 && game_height==512) /* For Slap Fight, Tiger Heli, Twin Cobra */ {
				gp32_xoffset+=8;
				gp32_yoffset+=112;
			}
		}
			
		if (game_height<240)
			gp32_height = game_height;		

		if (game_width<320)
			gp32_xscreen = (320-game_width) / 2;

		if (game_height<240)
			gp32_yscreen = (240-game_height) / 2;

	} else {
		/* Image has to be rotated */	
		gp32_rotate=1;
		gp32_xoffset=0;
		gp32_yoffset=0;
		gp32_xscreen=0;
		gp32_yscreen=0;
		gp32_width=240;
		gp32_height=320;
		
		if (game_width>240)
			gp32_xoffset = (game_width-240) / 2;

		if (game_width<240)
			gp32_width = game_width;
			
		if (game_height>320) {
			gp32_yoffset = (game_height-320) / 2;
			if (game_width==256 && game_height==512) /* For Slap Fight, Tiger Heli, Twin Cobra */ {
				gp32_xoffset+=8;
				gp32_yoffset+=96;
			}
		}
			
		if (game_height<320)
			gp32_height = game_height;

		if (game_width<240)
			gp32_xscreen = (240-game_width) / 2;

		if (game_height<320)
			gp32_yscreen = (320-game_height) / 2;
	}
}

/* Game List Text Out */
void gp32_gamelist_text_out(int x, int y, char *eltexto) {
	char texto[33];
	gm_strncpy(texto,eltexto,32);
	if (texto[0]!='-')
		GpTextOut( NULL, &gpDraw[nflip],x+1,y+1,texto, 0 /* 0xff */ );
	GpTextOut( NULL, &gpDraw[nflip],x,y,texto, 100 /* 0xff */ );
}

/* Debug information function*/
void gp32_text_out(int x, int y, char *texto) {
	GpTextOut( NULL, &gpDraw[nflip],x,y,texto, 0x01 /* 0xff */ );
	/*GpTextOut( NULL, &gpDraw[0],x,y,texto, 0x01 );
	GpTextOut( NULL, &gpDraw[1],x,y,texto, 0x01 );*/
}

/* Debug information function*/
void gp32_text_out_int(int x, int y, int entero) {
	char cad[50];
	sprintf(cad,"%d\n",entero);
	gp32_text_out(x,y,cad);
}

int log=0;

/* Set Debug Log start at row 0 */
void gp32_gamelist_zero(void) {
	log=0;
}

/* Pause Text Message */
void gp32_text_pause(void) {
	nflip=(nflip==0?1:0);
	GpTextOut( NULL, &gpDraw[nflip],140,110,"PAUSE\0", 0x01);
}

/* Debug Log information funcion */
void gp32_text_log(char *texto) {
	if (log==0)
		GpRectFill( NULL, &gpDraw[nflip],0,0,gpDraw[nflip].buf_w,gpDraw[nflip].buf_h,0x0 );
		
	gp32_text_out(0,log,texto);
	log+=15;
	if(log>239) log=0;
}

/* Debug Log information funcion */
void gp32_text_log_int(int entero) {
	if (log==0)
		GpRectFill( NULL, &gpDraw[nflip],0,0,gpDraw[nflip].buf_w,gpDraw[nflip].buf_h,0x0 );
	
	gp32_text_out_int(0,log,entero);
	log+=15;
	if(log>239) log=0;
}

/* Create a bitmap. Also calls osd_clearbitmap() to appropriately initialize */
/* it to the background color. */
/* VERY IMPORTANT: the function must allocate also a "safety area" 8 pixels wide all */
/* around the bitmap. This is required because, for performance reasons, some graphic */
/* routines don't clip at boundaries of the bitmap. */
struct osd_bitmap *osd_new_bitmap(int width,int height,int depth)       /* ASG 980209 */
{
	struct osd_bitmap *bitmap;
	
	if (Machine->orientation & ORIENTATION_SWAP_XY)
	{
		int temp;

		temp = width;
		width = height;
		height = temp;
	}
	

	if ((bitmap = gm_zi_malloc(sizeof(struct osd_bitmap))) != 0)
	{
		int i,rowlen,rdwidth;
		unsigned char *bm;
		int safety;


		if (width > 32) safety = 8;
		else safety = 0;        /* don't create the safety area for GfxElement bitmaps */

		if (depth != 8 && depth != 16) depth = 8;

		bitmap->depth = depth;
		bitmap->width = width;
		bitmap->height = height;

		rdwidth = (width + 7) & ~7;     /* round width to a quadword */
		if (depth == 16)
			rowlen = 2 * (rdwidth + 2 * safety) * sizeof(unsigned char);
		else
			rowlen =     (rdwidth + 2 * safety) * sizeof(unsigned char);

		if ((bm = gm_zi_malloc((height + 2 * safety) * rowlen)) == 0)
		{
			gp32_text_log("osd_new_bitmap(): Out of Memory");
			gm_free(bitmap);
			return 0;
		}

		if ((bitmap->line = gm_zi_malloc(height * sizeof(unsigned char *))) == 0)
		{
			gp32_text_log("osd_new_bitmap(): Out of Memory");
			gm_free(bm);
			gm_free(bitmap);
			return 0;
		}

		for (i = 0;i < height;i++)
			bitmap->line[i] = &bm[(i + safety) * rowlen + safety];

		bitmap->_private = bm;

		osd_clearbitmap(bitmap);
	}
	
	if(bitmap==0)
		gp32_text_log("osd_new_bitmap(): Out of Memory");

	return bitmap;
}



/* set the bitmap to black */
void osd_clearbitmap(struct osd_bitmap *bitmap)
{
	int i,j;

	for (i = 0;i < bitmap->height;i++)
	{
		for (j = 0;j<bitmap->width;j++)
			bitmap->line[i][j]=BACKGROUND;
		/*gm_memset(bitmap->line[i],BACKGROUND,bitmap->width);*/
	}
}



void osd_free_bitmap(struct osd_bitmap *bitmap)
{
	if (bitmap)
	{
		gm_free(bitmap->line);
		gm_free(bitmap->_private);
		gm_free(bitmap);
	}
}

/* center image inside the display based on the visual area */
static void adjust_display (int xmin, int ymin, int xmax, int ymax)
{
	int temp;
	int w,h;


	if (Machine->orientation & ORIENTATION_SWAP_XY)
	{
		temp = xmin; xmin = ymin; ymin = temp;
		temp = xmax; xmax = ymax; ymax = temp;
		w = Machine->drv->screen_height;
		h = Machine->drv->screen_width;
	}
	else
	{
		w = Machine->drv->screen_width;
		h = Machine->drv->screen_height;
	}

	if (Machine->orientation & ORIENTATION_FLIP_X)
	{
		temp = w - xmin - 1;
		xmin = w - xmax - 1;
		xmax = temp;
	}
	if (Machine->orientation & ORIENTATION_FLIP_Y)
	{
		temp = h - ymin - 1;
		ymin = h - ymax - 1;
		ymax = temp;
	}

	viswidth  = xmax - xmin + 1;
	visheight = ymax - ymin + 1;

	gfx_display_lines = visheight;
	gfx_display_columns = viswidth;
	gfx_yoffset = (gfx_height - visheight) / 2;
	if (gfx_display_lines > gfx_height)
		gfx_display_lines = gfx_height;
	gfx_xoffset = (gfx_width - viswidth) / 2;
	if (gfx_display_columns > gfx_width)
		gfx_display_columns = gfx_width;

	skiplinesmin = ymin;
	skiplinesmax = visheight - gfx_display_lines + ymin;
	skipcolumnsmin = xmin;
	skipcolumnsmax = viswidth - gfx_display_columns + xmin;

	/* Align on a quadword !*/
	gfx_xoffset &= ~7;

	/* the skipcolumns from mame.cfg/cmdline is relative to the visible area */
	skipcolumns = xmin + skipcolumns;
	skiplines   = ymin + skiplines;

	/* Just in case the visual area doesn't fit */
	if (gfx_xoffset < 0)
	{
		skipcolumns -= gfx_xoffset;
		gfx_xoffset = 0;
	}
	if (gfx_yoffset < 0)
	{
		skiplines   -= gfx_yoffset;
		gfx_yoffset = 0;
	}

	/* Failsafe against silly parameters */
	if (skiplines < skiplinesmin)
		skiplines = skiplinesmin;
	if (skipcolumns < skipcolumnsmin)
		skipcolumns = skipcolumnsmin;
	if (skiplines > skiplinesmax)
		skiplines = skiplinesmax;
	if (skipcolumns > skipcolumnsmax)
		skipcolumns = skipcolumnsmax;
}

/* Create a display screen, or window, large enough to accomodate a bitmap */
/* of the given dimensions. Attributes are the ones defined in driver.h. */
/* Return a osd_bitmap pointer or 0 in case of error. */
struct osd_bitmap *osd_create_display(int width,int height,int attributes)
{
	int     i;
	game_width = width;
	game_height = height;

	if (Machine->orientation & ORIENTATION_SWAP_XY)
	{
		int temp;

		temp = game_width;
		game_width = game_height;
		game_height = temp;
	}
	gp32_adjust_display();

	scrbitmap = osd_new_bitmap(width,height,8);
	if (!scrbitmap) return 0;

	for (i = 0;i < 256;i++)
	{
		dirtycolor[i] = 1;
	}
	dirtypalette = 1;

	/* center display based on visible area */
	{
		struct rectangle vis = Machine->drv->visible_area;
		adjust_display (vis.min_x, vis.min_y, vis.max_x, vis.max_y);
	}

	update_screen = update_screen_gp32;

    	return scrbitmap;
}


/* shut up the display */
void osd_close_display(void)
{
	if (scrbitmap)
	{
		osd_free_bitmap(scrbitmap);
		scrbitmap = NULL;
	}
}




/* palette is an array of 'totalcolors' R,G,B triplets. The function returns */
/* in *pens the pen values corresponding to the requested colors. */
/* If 'totalcolors' is 32768, 'palette' is ignored and the *pens array is filled */
/* with pen values corresponding to a 5-5-5 15-bit palette */
void osd_allocate_colors(unsigned int totalcolors,const unsigned char *palette,unsigned short *pens)
{

	int i;

	/* initialize the palette */
	for (i = 0;i < 256;i++)
		current_palette[i][0] = current_palette[i][1] = current_palette[i][2] = 0;

	if (totalcolors >= 255)
	{
		int bestblack,bestwhite;
		int bestblackscore,bestwhitescore;

		for (i = 0;i < totalcolors;i++)
			pens[i] = i;

		bestblack = bestwhite = 0;
		bestblackscore = 3*255*255;
		bestwhitescore = 0;
		for (i = 0;i < totalcolors;i++)
		{
			int r,g,b,score;

			r = palette[3*i];
			g = palette[3*i+1];
			b = palette[3*i+2];
			score = r*r + g*g + b*b;

			if (score < bestblackscore)
			{
				bestblack = i;
				bestblackscore = score;
			}
			if (score > bestwhitescore)
			{
				bestwhite = i;
				bestwhitescore = score;
			}
		}

		/*Machine->uifont->colortable[0] = pens[bestblack];
		Machine->uifont->colortable[1] = pens[bestwhite];
		Machine->uifont->colortable[2] = pens[bestwhite];
		Machine->uifont->colortable[3] = pens[bestblack];*/
	}
	else
	{
		/* reserve color 1 for the user interface text */
		current_palette[1][0] = current_palette[1][1] = current_palette[1][2] = 0xff;
		/*Machine->uifont->colortable[0] = 0;
		Machine->uifont->colortable[1] = 1;
		Machine->uifont->colortable[2] = 1;
		Machine->uifont->colortable[3] = 0;*/

		/* fill the palette starting from the end, so we mess up badly written */
		/* drivers which don't go through Machine->pens[] */
		for (i = 0;i < totalcolors;i++)
			pens[i] = 255-i;
	}

	for (i = 0;i < totalcolors;i++)
	{
		current_palette[pens[i]][0] = palette[3*i];
		current_palette[pens[i]][1] = palette[3*i+1];
		current_palette[pens[i]][2] = palette[3*i+2];
	}

	/* do a first screen update to pick the */
	/* background pen before the copyright screen is displayed. */
	osd_update_display();
	/* the first call picked the color, the second call actually refreshes the screen */
	osd_update_display();
}


void osd_modify_pen(int pen,unsigned char red, unsigned char green, unsigned char blue)
{
	if (scrbitmap->depth != 8)
	{
		return;
	}


	if (current_palette[pen][0] != red ||
			current_palette[pen][1] != green ||
			current_palette[pen][2] != blue)
	{
		current_palette[pen][0] = red;
		current_palette[pen][1] = green;
		current_palette[pen][2] = blue;

		dirtycolor[pen] = 1;
		dirtypalette = 1;
	}
}


void osd_get_pen(int pen,unsigned char *red, unsigned char *green, unsigned char *blue)
{
	*red = current_palette[pen][0];
	*green = current_palette[pen][1];
	*blue = current_palette[pen][2];
}


void update_screen_dummy(void)
{
}

void gp32_clear_screen(void)
{
	int i;
	for(i=0;i<2;i++)
		GpRectFill( NULL, &gpDraw[i],0,0,gpDraw[i].buf_w,gpDraw[i].buf_h,0x0 );
}


int osd_skip_this_frame(int recommend)
{
	return recommend;
}

/* Update the display. */
void osd_update_display(void)
{
	int i;
	int curr;

	/* now wait until it's time to update the screen */
	if (throttle)
	{
		do
		{
			curr = GpTickCountGet();
			__asm("nop;nop;nop;nop;");
			/*4150 = ( 166 MHz / 40 ) * 1000 */
		} while ((curr - prev[memory]) < (frameskip+1) * gp32_timer/Machine->drv->frames_per_second);
	}
	else curr = GpTickCountGet();

	memory = (memory+1) % MEMORY;

	prev[memory] = curr;

	if (dirtypalette)
	{
		dirtypalette = 0;

		for (i = 0;i < 256;i++)
		{
			if (dirtycolor[i])
			{
				int r,g,b;
				dirtycolor[i] = 0;

				r = current_palette[i][0];
				g = current_palette[i][1];
				b = current_palette[i][2];

				{
					GP_PALETTEENTRY colorgp;
					colorgp = GP_RGB24(r,g,b);
   						GpPaletteEntryChange(i,1,&colorgp,GPC_PAL_ASYNC_REALIZE); /* GPC_PAL_ASYNC_REALIZE=1, GPC_PAL_SYNC_REALIZE=0 */
				}
				
			}
		}
	}

	/* copy the bitmap to screen memory */
	update_screen();
}
