#ifndef __gpmain_h__
#define __gpmain_h__

int nflip;			/* Video buffer index */
GPDRAWSURFACE gpDraw[4];	/* Video Buffers */
GP_HPALETTE h_pal;

/* Offset de la pantalla real del juego */
#define OFFSET_X 32
#define OFFSET_Y 14

/**************************************
 * Functions used in the main program *
 **************************************/

void GpMain(void * arg);
void Init( void );
void GameEngine( void );

#endif /*__gpmain_h__*/
