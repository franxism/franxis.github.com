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
#include "preview1.h"
#include "preview2.h"

/* GAME LISTINGS -> */

					/* Original Games (9)*/
					/* Pac Man Hacks (30)*/
					/* Ms Pac Man Hacks (24)*/

					char *game_id[] = {
					"pacman", "mspacman", "crush", "hangly", "pacmod",
					"puckman", "namcopac", "pacplus", "piranha",
					
     					"generic", "abortman", "baby", "brakman", "crazypac",
					"dizzy", "europac", "hearts", "mazeman", "pac2600", "pacman3d", "pcrunchy",
					"pacstrm", "pacinvis", "pacjail", "pacmini", "pacshuf", "pacweird",
					"snowpac", "pacspeed", "pengman", "puckman2", "puckren", "roboman", "sumelton",
					"ultrapac", "ultra2", "vectrpac", "xensrev", "vectxens",

     					"genericm", "cookiem", "heartbrn", "mrpacman", "msbaby",
					"msberzk", "msdroid", "mselton", "msf1pac", "mshangly", "mshearts", "msmini",
					"mspacad", "mspaccm", "mspacdx", "mspacren", "mspacrip", "mspacsm", "msrumble",
					"msultra", "msvectr", "punleash", "sueworld", "msyakman"
					};

					char *game_name[] = {
					"Pac Man", "Ms Pac Man", "Crush Roller", "Hangly Man",
					"Pac Man Mod", "Puck Man", "Pac Man Namco", "Pac Man Plus", "Piranha",

     					"Pac Man (generic clone/hack)", "Abort Man",
					"Baby Pac Man", "Brakman", "Crazy Pac", "Dizzy Ghost", "Euro Pac",
					"Pac Man Hearts", "Maze Man", "Pac 2600", "Pac Man 3D", "Pac Man Crunchy",
					"Pac Man Desert Storm", "Pac Man Inviso", "Pac Man Jail", "Pac Man Mini",
					"Pac Man Shuffle", "Pac Man Six Map Weird", "Pac Man Snowy Day",
					"Pac Man Speedy", "PengMan", "Puck Man 2", "Puck Man Renaissance", "Robo Man",
					"Summertime Elton", "UltraPac", "Ultra Pacman TwoBit Score Mazes", "Vector Pac Man",
					"Xens Revenge", "Xens Revenge Vector",
					
					"Ms Pac Man (Generic clone/hack)", "Cookie Mon",
					"Ms Heart Burn", "Mr Pac Man", "Ms Baby Pac Man", "Ms Pac Man Berzerk",
					"Ms Droid", "Ms Elton", "Ms F1 Pac Man", "Ms Hangly Man", "Ms Pac Man Hearts",
					"Ms Pac Man Mini", "Ms Pac Man After Dark", "Ms Pac Crazy Mazes",
					"Ms Pac Man Deluxe", "Ms Pac Man Renaissance", "Ms Pac Mortem",
					"Ms Pac Strange Mazes", "Ms Pac Rumble", "Ms Ultra Pac Man", "Vector Ms Pac Man",
					"Pac Man Unleashed", "Sue's World", "Ms Yak Man"
					};

					char game_avail[] = {
					/*
					1,1,1,1,1,1,1,1,1,
					1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
					1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
					*/
					0,0,0,0,0,0,0,0,0,
					0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
					0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
					};

					int game_num = 63;
 					int game_num_avail=0;
 					/*int game_num_avail=11;*/
					int last_game_selected=0;

/* <- GAME LISTINGS */

/* Initialize the list of available games in the SMC */
void game_list_init(void) {

    	unsigned int i;
	unsigned	int c;
    	ulong num_entries;
    	ulong read_count;
    	GPDIRENTRY * direntry;

    	/* Get directory */
    	if ( GpDirEnumNum("gp:\\gpmm\\multipac\0", &num_entries)!=SM_OK )
    	  	return;
    	if (num_entries==0)
    	  	return;
    	direntry = malloc ( sizeof(GPDIRENTRY)*num_entries );
    	GpDirEnumList ( "gp:\\gpmm\\multipac\0", 0, num_entries, direntry, &read_count );
    	num_entries = read_count;

	/* Check Available Games */
	for (c=0;c<game_num;c++) {
		for (i=0;i<num_entries;i++) {
		    	if (strcasecmp(game_id[c],direntry[i].name)==0) {
	       			game_avail[c]=1;
				game_num_avail++;
			}
		}
	}
	/* Free Memory */
     	free ( direntry );
}

void game_list_view(int *pos) {

	int i;
	int view_pos;
	int aux_pos=0;
	int screen_y = 38;
	int screen_x = 40;

	/* Draw the menu*/
	GpBitBlt( NULL, &gpDraw[nflip],0,0,gpDraw[nflip].buf_w,gpDraw[nflip].buf_h,(unsigned char*)preview2,0,0,preview2_width,preview2_height);

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
				text_out( screen_x, screen_y, game_name[i] );
			  	if (aux_pos==*pos) {
					     text_out( screen_x-10, screen_y,">" );
					     text_out( screen_x-13, screen_y,"-" );
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
				strcpy(game,game_id[i]);
			}
			aux_pos++;
		}
	}
}


void select_game(char *argv[]) {

	int ExKey;
	int c;

	/* No Selected game */
	strcpy(argv[1],"builtin");

	/* Clean screen*/
     	GpRectFill( NULL, &gpDraw[0],0,0,gpDraw[0].buf_w,gpDraw[0].buf_h,0x0 );
     	GpRectFill( NULL, &gpDraw[1],0,0,gpDraw[1].buf_w,gpDraw[1].buf_h,0x0 );

	/* Wait until no key pressed */
	while(GpKeyGet()!=GPC_VK_NONE) { }

	/* Available games? */
	if(game_num_avail==0) {
	   	GpTextOut( NULL, &gpDraw[nflip],35,110,"ERROR: NO AVAILABLE GAMES FOUND", 0x80 );
		while(1) { }
	}

	for (c=0;c<256;c++) {
    		GpPaletteEntryChange(c,1,&preview2_Pal[c],0);
	}

	/* Wait until user selects a game */
	while(1) {
		nflip=(nflip==1?0:1);
		GpRectFill( NULL, &gpDraw[nflip],0,0,gpDraw[nflip].buf_w,gpDraw[nflip].buf_h,0x0 );
		game_list_view(&last_game_selected);
		GpSurfaceSet( &gpDraw[nflip] );
		while(GpKeyGet()!=GPC_VK_NONE) { }
		while((ExKey=GpKeyGet())==GPC_VK_NONE) { }
		if (ExKey & GPC_VK_UP) last_game_selected--;
		if (ExKey & GPC_VK_DOWN) last_game_selected++;
		if ((ExKey & GPC_VK_FL) && (!(ExKey & GPC_VK_FR))) last_game_selected-=11;
		if ((ExKey & GPC_VK_FR) && (!(ExKey & GPC_VK_FL))) last_game_selected+=11;
		if ((ExKey & GPC_VK_FL) && (ExKey & GPC_VK_FR)) GpAppExit();
		if ((ExKey & GPC_VK_FA) || (ExKey & GPC_VK_FB)) break;
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
    		GpPaletteEntryChange(c,1,&preview1_Pal[c],0);
	}
					
	/* Draw the screen*/
	GpBitBlt( NULL, &gpDraw[nflip],0,0,gpDraw[nflip].buf_w,gpDraw[nflip].buf_h,(unsigned char*)preview1,0,0,preview1_width,preview1_height);

	/* Wait 5 seconds */
	Delay(10000);

    	/* Clean screen*/
    	GpRectFill( NULL, &gpDraw[0],0,0,gpDraw[0].buf_w,gpDraw[0].buf_h,0x0 );
    	GpRectFill( NULL, &gpDraw[1],0,0,gpDraw[1].buf_w,gpDraw[1].buf_h,0x0 );
}
