#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gpdef.h"
#include "gpstdio.h"
#include "gpstdlib.h"
#include "gpgraphic.h"
#include "gpfont.h"
#include "gpmem.h"
#include "gp32splash.h"
#include "gp32menu.h"
#include "driver.h"
#include "gp32_mame.h"

int game_num=1024;
int game_num_avail=0;
char game_avail[1024];
int last_game_selected=0;

int gp32_freq = 133;
int gp32_frameskip = 2;
int gp32_sound = 1;

/* Initialize the list of available games in the SMC */
void game_list_init_old(void) {

    	unsigned int i;
	unsigned int c;
    	ulong num_entries;
    	ulong read_count;
    	GPDIRENTRY * direntry;
	char zipfile[16];
    	/* Get directory */
    	if ( GpDirEnumNum("gp:\\gpmm\\mame034\0", &num_entries)!=SM_OK )
      		return;
    	if (num_entries==0)
      		return;
    	direntry = gm_zi_malloc ( sizeof(GPDIRENTRY)*num_entries );
    	GpDirEnumList ( "gp:\\gpmm\\mame034\0", 0, num_entries, direntry, &read_count );
    	num_entries = read_count;
    	
    	/* To Lower Case */
    	for (i=0;i<num_entries;i++)
    		gm_lowercase(direntry[i].name,gm_lstrlen(direntry[i].name));

	/* Check Available Games */
	for (c=0;(c<game_num && drivers[c]);c++) {
		for (i=0;i<num_entries;i++) {
		    	if (gm_compare(drivers[c]->name,direntry[i].name)==0) {
				game_avail[c]=1;
				game_num_avail++;
				break;
			}
			sprintf(zipfile,"%s.zip\0",drivers[c]->name);
		    	if (gm_compare(zipfile,direntry[i].name)==0) {
				game_avail[c]=1;
				game_num_avail++;
				break;
			}
		}
	}

	/* Free Memory */
     	gm_free ( direntry );
}

/* Initialize the list of available games in the SMC */
void game_list_init(void) {

	unsigned int c;
	char filename[128];

	/* Check Available Games */
	for (c=0;(c<game_num && drivers[c]);c++) {
		
		sprintf(filename,"gp:\\gpmm\\mame034\\%s.zip\0",drivers[c]->name);
		if (gp32_fexists(filename)) {
			game_avail[c]=1;
			game_num_avail++;	
			continue;
		}

		sprintf(filename,"gp:\\gpmm\\mame034\\%s\0",drivers[c]->name);
		if (gp32_fexists(filename)) {
			game_avail[c]=1;
			game_num_avail++;
			continue;	
		}

	}
}

void game_list_view(int *pos) {

	int i;
	int view_pos;
	int aux_pos=0;
	int screen_y = 38;
	int screen_x = 40;

	/* Draw the menu*/
	GpBitBlt( NULL, &gpDraw[nflip],0,0,gpDraw[nflip].buf_w,gpDraw[nflip].buf_h,(unsigned char*)gp32menu,0,0,gp32menu_width,gp32menu_height);

	/* Check Limits */
	if (*pos>(game_num_avail-1))
	*pos=game_num_avail-1;
	if (*pos<0)
		*pos=0;
					   
	/* Set View Pos */
	if (*pos<5) {
		view_pos=0;
	} else {
		if (*pos>game_num_avail-7) {
			view_pos=game_num_avail-11;
			view_pos=(view_pos<0?0:view_pos);
		} else {
			view_pos=*pos-5;
		}
	}

	/* Show List */
	for (i=0;i<game_num;i++) {
		if (game_avail[i]==1) {
			if (aux_pos>=view_pos && aux_pos<=view_pos+10) {
				gp32_gamelist_text_out( screen_x, screen_y, (char *)drivers[i]->description );
				if (aux_pos==*pos) {
					gp32_gamelist_text_out( screen_x-10, screen_y,">" );
					gp32_gamelist_text_out( screen_x-13, screen_y,"-" );
				}
				screen_y+=15;
			}
			aux_pos++;
		}
	}

}

void game_list_select (int index, char *game) {
	int i;
	int aux_pos=0;
	for (i=0;i<game_num;i++) {
		if (game_avail[i]==1) {
			if(aux_pos==index) {
				gm_strcpy(game,drivers[i]->name);
			}
			aux_pos++;
		   }
	}
}

char *game_list_description (int index) {
	int i;
	int aux_pos=0;
	for (i=0;i<game_num;i++) {
		if (game_avail[i]==1) {
			if(aux_pos==index) {
				return((char *)drivers[i]->description);
			}
			aux_pos++;
		   }
	}
	return ((char *)0);
}


int game_options(void) {

	int ExKey;
	int pos_x = 33;
	int pos_y = 63;

while (1) {
		
	nflip=(nflip==1?0:1);
	GpRectFill( NULL, &gpDraw[nflip],0,0,gpDraw[nflip].buf_w,gpDraw[nflip].buf_h,0x0 );

	/* Draw the background */
	GpBitBlt( NULL, &gpDraw[nflip],0,0,gpDraw[nflip].buf_w,gpDraw[nflip].buf_h,(unsigned char*)gp32menu,0,0,gp32menu_width,gp32menu_height);

	/* Draw the options */
	gp32_gamelist_text_out(pos_x,pos_y,"Selected Game:\0");
	gp32_gamelist_text_out(pos_x,pos_y+15,game_list_description(last_game_selected));
	
	switch (gp32_freq) {
		case 133: gp32_gamelist_text_out(pos_x,pos_y+45, "Press L to change: 133 MHz\0");break;
		case 144: gp32_gamelist_text_out(pos_x,pos_y+45, "Press L to change: 144 MHz\0");break;
		case 150: gp32_gamelist_text_out(pos_x,pos_y+45, "Press L to change: 150 MHz\0");break;
		case 156: gp32_gamelist_text_out(pos_x,pos_y+45, "Press L to change: 156 MHz\0");break;
		case 160: gp32_gamelist_text_out(pos_x,pos_y+45, "Press L to change: 160 MHz\0");break;
		case 166: gp32_gamelist_text_out(pos_x,pos_y+45, "Press L to change: 166 MHz\0");break;
     		case 168: gp32_gamelist_text_out(pos_x,pos_y+45, "Press L to change: 168(Warning!)\0");break;
     		case 172: gp32_gamelist_text_out(pos_x,pos_y+45, "Press L to change: 172(Warning!)\0");break;
     		case 176: gp32_gamelist_text_out(pos_x,pos_y+45, "Press L to change: 176(Warning!)\0");break;
     		case 180: gp32_gamelist_text_out(pos_x,pos_y+45, "Press L to change: 180(Warning!)\0");break;
     		case 184: gp32_gamelist_text_out(pos_x,pos_y+45, "Press L to change: 184(Warning!)\0");break;
     		case 188: gp32_gamelist_text_out(pos_x,pos_y+45, "Press L to change: 188(Warning!)\0");break;
     		case 192: gp32_gamelist_text_out(pos_x,pos_y+45, "Press L to change: 192(Warning!)\0");break;
     		case 196: gp32_gamelist_text_out(pos_x,pos_y+45, "Press L to change: 196(Warning!)\0");break;
     		case 200: gp32_gamelist_text_out(pos_x,pos_y+45, "Press L to change: 200(Warning!)\0");break;
     		case 204: gp32_gamelist_text_out(pos_x,pos_y+45, "Press L to change: 204(Crazy!)\0");break;
     		case 208: gp32_gamelist_text_out(pos_x,pos_y+45, "Press L to change: 208(Crazy!)\0");break;
     		case 212: gp32_gamelist_text_out(pos_x,pos_y+45, "Press L to change: 212(Crazy!)\0");break;
     		case 216: gp32_gamelist_text_out(pos_x,pos_y+45, "Press L to change: 216(Crazy!)\0");break;
     		case 220: gp32_gamelist_text_out(pos_x,pos_y+45, "Press L to change: 220(Crazy!)\0");break;
     		case 224: gp32_gamelist_text_out(pos_x,pos_y+45, "Press L to change: 224(Crazy!)\0");break;
     		case 228: gp32_gamelist_text_out(pos_x,pos_y+45, "Press L to change: 228(Crazy!)\0");break;
     		case 232: gp32_gamelist_text_out(pos_x,pos_y+45, "Press L to change: 232(Crazy!)\0");break;
     		case 236: gp32_gamelist_text_out(pos_x,pos_y+45, "Press L to change: 236(Crazy!)\0");break;
     		case 240: gp32_gamelist_text_out(pos_x,pos_y+45, "Press L to change: 240(Crazy!)\0");break;
     		case 244: gp32_gamelist_text_out(pos_x,pos_y+45, "Press L to change: 244(Crazy!)\0");break;
     		case 248: gp32_gamelist_text_out(pos_x,pos_y+45, "Press L to change: 248(Crazy!)\0");break;
     		case 252: gp32_gamelist_text_out(pos_x,pos_y+45, "Press L to change: 252(Crazy!)\0");break;
     		case 256: gp32_gamelist_text_out(pos_x,pos_y+45, "Press L to change: 256(Crazy!)\0");break;
	}
	
	switch (gp32_frameskip) {
		case 0: gp32_gamelist_text_out(pos_x,pos_y+60, "Press R to change: Frame-Skip 0\0");break;
		case 1: gp32_gamelist_text_out(pos_x,pos_y+60, "Press R to change: Frame-Skip 1\0");break;
		case 2: gp32_gamelist_text_out(pos_x,pos_y+60, "Press R to change: Frame-Skip 2\0");break;
		case 3: gp32_gamelist_text_out(pos_x,pos_y+60, "Press R to change: Frame-Skip 3\0");break;
		case 4: gp32_gamelist_text_out(pos_x,pos_y+60, "Press R to change: Frame-Skip 4\0");break;
		case 5: gp32_gamelist_text_out(pos_x,pos_y+60, "Press R to change: Frame-Skip 5\0");break;
	}
	
	switch (gp32_sound) {
		case 0: gp32_gamelist_text_out(pos_x,pos_y+75, "Press S to change: Not Emulated\0");break;
		case 1: gp32_gamelist_text_out(pos_x,pos_y+75, "Press S to change: Sound ON\0");break;
		case 2: gp32_gamelist_text_out(pos_x,pos_y+75, "Press S to change: Sound OFF\0");break;
	}

	gp32_gamelist_text_out(pos_x,pos_y+105,"Press A to confirm, B to go back\0");

	GpSurfaceSet( &gpDraw[nflip] );
	while(GpKeyGet()!=GPC_VK_NONE) { }
	while((ExKey=GpKeyGet())==GPC_VK_NONE) { }
	
	if (ExKey & GPC_VK_FB) return 0;

	if (ExKey & GPC_VK_FL) {
		switch (gp32_freq) {
			case 133: gp32_freq=144; break;
			case 144: gp32_freq=150; break;
			case 150: gp32_freq=156; break;
			case 156: gp32_freq=160; break;
			case 160: gp32_freq=166; break;
			case 166: gp32_freq=168; break;
     			case 168: gp32_freq=172; break;
     			case 172: gp32_freq=176; break;
     			case 176: gp32_freq=180; break;
     			case 180: gp32_freq=184; break;
     			case 184: gp32_freq=188; break;
     			case 188: gp32_freq=192; break;
     			case 192: gp32_freq=196; break;
     			case 196: gp32_freq=200; break;
     			case 200: gp32_freq=204; break;
     			case 204: gp32_freq=208; break;
     			case 208: gp32_freq=212; break;
     			case 212: gp32_freq=216; break;
     			case 216: gp32_freq=220; break;
     			case 220: gp32_freq=224; break;
     			case 224: gp32_freq=228; break;
     			case 228: gp32_freq=232; break;
     			case 232: gp32_freq=236; break;
     			case 236: gp32_freq=240; break;
     			case 240: gp32_freq=244; break;
     			case 244: gp32_freq=248; break;
     			case 248: gp32_freq=252; break;
     			case 252: gp32_freq=256; break;
     			case 256: gp32_freq=133; break;
		}
	}		
	
	if (ExKey & GPC_VK_FR) {
		gp32_frameskip ++;
		gp32_frameskip = gp32_frameskip % 6;
	}
	
	if ((ExKey & GPC_VK_START) || (ExKey & GPC_VK_SELECT)) {
		gp32_sound ++;
		gp32_sound = gp32_sound % 3;
	}
	
	if (ExKey & GPC_VK_FA) {
		return 1;
	}
	
}
	
}


void select_game(char *argv[]) {

	int ExKey;
	int c;

	/* No Selected game */
	gm_strcpy(argv[1],"builtinn");

	/* Clean screen*/
     	GpRectFill( NULL, &gpDraw[0],0,0,gpDraw[0].buf_w,gpDraw[0].buf_h,0x0 );
     	GpRectFill( NULL, &gpDraw[1],0,0,gpDraw[1].buf_w,gpDraw[1].buf_h,0x0 );

	/* Wait until no key pressed */
	while(GpKeyGet()!=GPC_VK_NONE) { }

	/* Available games? */
	if(game_num_avail==0) {
		GpTextOut( NULL, &gpDraw[nflip],35,110,"ERROR: NO AVAILABLE GAMES FOUND", 254 );
		while(1) { }
	}

	for (c=0;c<256;c++) {
    		GpPaletteEntryChange(c,1,&gp32menu_Pal[c],0);
	}

	/* Wait until user selects a game */
	while(1) {
		nflip=(nflip==1?0:1);
		GpRectFill( NULL, &gpDraw[nflip],0,0,gpDraw[nflip].buf_w,gpDraw[nflip].buf_h,0x0 );
		game_list_view(&last_game_selected);
		GpSurfaceSet( &gpDraw[nflip] );
		while((ExKey=GpKeyGet())!=GPC_VK_NONE) {
			/* Reset GP32 */
			if ((ExKey & GPC_VK_FL) && (ExKey & GPC_VK_FR)) {
		 	 	GpClockSpeedChange(67800000, 0x69032, 3);
				__asm("swi #4;");
			}
		}
		while((ExKey=GpKeyGet())==GPC_VK_NONE) {}

		/* Reset GP32 */
		if ((ExKey & GPC_VK_FL) && (ExKey & GPC_VK_FR)) {
	 	 	GpClockSpeedChange(67800000, 0x69032, 3);
			__asm("swi #4;");
		}
		if (ExKey & GPC_VK_UP) last_game_selected--;
		if (ExKey & GPC_VK_DOWN) last_game_selected++;
		if (ExKey & GPC_VK_FL) last_game_selected-=11;
		if (ExKey & GPC_VK_FR) last_game_selected+=11;
		if (!(ExKey & GPC_VK_FA) && !(ExKey & GPC_VK_FB)) continue;
		/* Emulation Options */
		if (game_options()) break;
	}

	/* Select the game */
	game_list_select(last_game_selected,argv[1]);

	 /* Clean screen */
  	GpRectFill( NULL, &gpDraw[0],0,0,gpDraw[0].buf_w,gpDraw[0].buf_h,0x0 );
  	GpRectFill( NULL, &gpDraw[1],0,0,gpDraw[1].buf_w,gpDraw[1].buf_h,0x0 );
}


/* Wait millisecs */
void Delay( int millisec )
{
	int nTick;
	nTick=GpTickCountGet();
	while( (GpTickCountGet()-nTick)<millisec );
}


void intro_screen(void) {

	int c;

	/* Clean screen*/
     	GpRectFill( NULL, &gpDraw[0],0,0,gpDraw[0].buf_w,gpDraw[0].buf_h,0x0 );
     	GpRectFill( NULL, &gpDraw[1],0,0,gpDraw[1].buf_w,gpDraw[1].buf_h,0x0 );

	for (c=0;c<256;c++) {
    		GpPaletteEntryChange(c,1,&gp32splash_Pal[c],0);
	}
					
	/* Draw the screen*/
	GpBitBlt( NULL, &gpDraw[nflip],0,0,gpDraw[nflip].buf_w,gpDraw[nflip].buf_h,(unsigned char*)gp32splash,0,0,gp32splash_width,gp32splash_height);

	/* Wait 5 seconds */
	/*Delay(10000);*/
	/*Delay(1000);*/

    	/* Clean screen*/
    	/*GpRectFill( NULL, &gpDraw[0],0,0,gpDraw[0].buf_w,gpDraw[0].buf_h,0x0 );*/
    	/*GpRectFill( NULL, &gpDraw[1],0,0,gpDraw[1].buf_w,gpDraw[1].buf_h,0x0 );*/
}
