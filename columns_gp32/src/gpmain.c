/*******************************************************************
*              __________________________  ________                *
*             /  _____/\______   \_____  \ \_____  \               *
*            /   \  ___ |     ___/ _(__  <  /  ____/               *
*            \    \_\  \|    |    /       \/       \               *
*             \______  /|____|   /______  /\_______ \ iDE          *
*                    \/                 \/         \/              *
*                                           Project template       *
*                                                                  *
* Project name....:                                                *
* Author..........:                                                *
* Last revision...:                                                *
* Note............:                                                *
*                                                                  *
*                                                                  *
*******************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gpdef.h"
#include "gpstdio.h"
#include "gpstdlib.h"
#include "gpgraphic.h"
#include "gpmain.h"
#include "gpstdio.h"
#include "gpfont.h"
#include "gpmodplay.h"

#include "modfile.h"
#include "intro.h"
#include "fuentes.h"
#include "marcad.h"
#include "piezas.h"
#include "paleta.h"
#include "preview.h"
#include "end.h"

/* Fichero MOD */
MODPlay mp;

/* Controles */
char arriba=0;
char abajo=0;
char izquierda=0;
char derecha=0;
char disparo1=0;
char disparo2=0;

char tablero[6][13];           /* Mapa del tablero de juego */
int p1,p2,p3;                  /* Piezas que caen */
int pn1,pn2,pn3;               /* Piezas que caeran despues */
int px,py;                     /* posicion del bloque */
int a,b;                       /* Para bucles */
int fin_p;                     /* Indica que hay sacar nueva pieza */
double r;                      /* Para generar numeros aleatorios */
int con=1;                     /* Para el control (1)*/
int vcaida=1;                  /* Velocidad de caida en pixels (1)*/
int ccaida=2;                  /* N§ de periodos por caida (2)*/
int cc=0;                      /* Para ccaida */

char gameover;                 /* Indica fin de juego */
int nivel;                     /* Nivel de juego */
int joyas;                     /* N§ de joyas descartadas */
unsigned long puntos;          /* Puntuacion */

void saca_mapa(void);          /* Saca mapa de piezas por pantalla */
void control(void);            /* Control del juego */
int colision(int x,int y);     /* Para detectar colision entre piezas */
void fin_pieza(void);          /* Hace lo necesario cuando pieza colisiona */
void descarta(void);           /* Descarta piezas cuando pieza colisiona */
void saca_pn(void);            /* Saca piezas que caeran despues */
void saca_puntos(void);        /* Saca puntuacion */
void destruye(void);           /* Finaliza nivel, quitando todas las joyas */

char debug[100] = "";

/* Nuevas funciones de GP32 */
void GpMain( void *arg );
void Init( void );
void Delay( int millisec );
void lee_control(void);
void GameEngine( void );



void GpMain( void *arg )
{
	Init();
	GameEngine();
}

void Init( void )
{
	int i;

	/* 320x240, 256 colores */
		GpGraphicModeSet(8,0);

	/* Limpia las dos pantallas virtuales */
	for( i=0; i<2; i++ ) {
		GpLcdSurfaceGet( &gpDraw[i],i );
		GpRectFill( NULL, &gpDraw[i],0,0,gpDraw[i].buf_w,gpDraw[i].buf_h,0x00 );
	}

 	/* Carga paleta de colores */
		h_pal = GpPaletteCreate(256, (GP_PALETTEENTRY*)preview_Pal);
  GpPaletteSelect(h_pal);
  GpPaletteRealize();
  GpPaletteDelete(h_pal);
		h_pal = NULL;

		nflip = 0;
		GpBitBlt( NULL, &gpDraw[nflip],32,20,gpDraw[nflip].buf_w,gpDraw[nflip].buf_h,(unsigned char*)preview,0,0,preview_width,preview_height);
  GpSurfaceSet( &gpDraw[nflip] );

		Delay(1000);
		Delay(1000);
		Delay(1000);
		Delay(1000);
		Delay(1000);

 	/* Carga paleta de colores */
		h_pal = GpPaletteCreate(256, (GP_PALETTEENTRY*)paleta);
  GpPaletteSelect(h_pal);
  GpPaletteRealize();
  GpPaletteDelete(h_pal);
		h_pal = NULL;

	/* Reproducimos el MOD */
 	MODPlay_Init ( &mp );
 	MODPlay_SetMOD ( &mp, modfile );
 	MODPlay_Start ( &mp );
}

/* Espera millisec */
void Delay( int millisec )
{
	int nTick;
	nTick=GpTickCountGet();
	while( (GpTickCountGet()-nTick)<millisec )
		;
}

void lee_control(void)
{
  unsigned char ExKey;

		ExKey = GpKeyGet();

  if (ExKey & GPC_VK_LEFT) izquierda++; else izquierda=0;
  if (ExKey & GPC_VK_RIGHT) derecha++; else derecha=0;
  if (ExKey & GPC_VK_UP) arriba++; else arriba=0;
  if (ExKey & GPC_VK_DOWN) abajo++; else abajo=0;

		if (ExKey & GPC_VK_FA) disparo1++; else disparo1=0;
		if (ExKey & GPC_VK_FB) disparo2++; else disparo2=0;
}

void GameEngine( void )
{
	int i;
 int nTick;

	/* Bucle infinito */
	while(1)
	{

 Presentacion:

	/* Limpia las dos pantallas virtuales */
	for( i=0; i<2; i++ ) {
		GpRectFill( NULL, &gpDraw[i],0,0,gpDraw[i].buf_w,gpDraw[i].buf_h,0x00 );
	}

		/* Limpiamos pantalla 0 */
		nflip=0;
		GpRectFill( NULL, &gpDraw[nflip],0,0,gpDraw[nflip].buf_w,gpDraw[nflip].buf_h,0x00 );
		GpSurfaceSet( &gpDraw[nflip] );

 	/* Con la 0 en negro pintamos en la 1 */
		nflip=1;
		GpRectFill( NULL, &gpDraw[nflip],0,0,gpDraw[nflip].buf_w,gpDraw[nflip].buf_h,0x00 );

		/* Pantalla de introduccion */
		GpBitBlt( NULL, &gpDraw[nflip],32,20,gpDraw[nflip].buf_w,gpDraw[nflip].buf_h,(unsigned char*)intro_image,0,0,intro_width,intro_height);
  GpSurfaceSet( &gpDraw[nflip] );

		/* Esperamos que se pulse disparo */
	 srand(10000);
		while (disparo1==0 && disparo2==0) {
		 r = rand();
			lee_control();
			Delay(100);
		}
		disparo1=0;
		disparo2=0;
		
		/* Limpiamos pantalla 0 */
		nflip=0;
		GpRectFill( NULL, &gpDraw[nflip],0,0,gpDraw[nflip].buf_w,gpDraw[nflip].buf_h,0x00 );
		GpSurfaceSet( &gpDraw[nflip] );
		/* Lo siguiente sera en la pantalla 1 */
		nflip=1;

		/* Inicializa variables */
 nivel=1;
 puntos=0;
 joyas=0;
 gameover=0;

	/* Limpia tablero */
 for (a=0;a<13;a++)
  for (b=0;b<6;b++)
   tablero[b][a]=0;

 pn1=(rand()%6) +1;
 pn2=(rand()%6) +1;
 pn3=(rand()%6) +1;

 /* Comienzo de cada nivel del juego */
 nuevo_nivel:
 if (nivel==1) {             joyas=20;  vcaida=1; ccaida=4; }
 if (nivel==2) { destruye(); joyas=20;  vcaida=1; ccaida=2; }
 if (nivel==3) {             joyas=30;  vcaida=1; ccaida=2; }
 if (nivel==4) { destruye(); joyas=40;  vcaida=1; ccaida=1; }
 if (nivel==5) {             joyas=50;  vcaida=1; ccaida=1; }
 if (nivel==6) { destruye(); joyas=50;  vcaida=2; ccaida=1; }
 if (nivel==7) { destruye(); joyas=100; vcaida=2; ccaida=1; }
 if (nivel==8) { destruye(); joyas=40;  vcaida=3; ccaida=1; }
 if (nivel==9) { destruye(); joyas=80;  vcaida=4; ccaida=1; }
 if (nivel==10) goto fin2;
 /*if (nivel==2) goto fin2;*/


	/* Inicializa variables */
 px=113;

 /* Bucle principal del juego */
 nueva_pieza:
 fin_p=0;
 p1=pn1;
 pn1=(rand()%6) +1;
 p2=pn2;
 pn2=(rand()%6) +1;
 p3=pn3;
 pn3=(rand()%6) +1;

 py=3;

 while(fin_p==0)
 {
		/* Start of Cycle 30 ms */
 	nTick=GpTickCountGet();

		/* Dibujamos marcador */
		GpBitBlt( NULL, &gpDraw[nflip],OFFSET_X,OFFSET_Y,gpDraw[nflip].buf_w,gpDraw[nflip].buf_h,(unsigned char*)marcad_image,0,0,marcad_width,marcad_height);

   saca_pn();
   saca_mapa();
   saca_puntos();

   control(); 

/*
 		GpRectFill( NULL, &gpDraw[nflip],0,0,gpDraw[nflip].buf_w,14,0x00 );
			sprintf(debug,"%d %d %d %d %d %d",arriba,abajo,izquierda,derecha,disparo1,disparo2);
			GpTextOut(NULL, &gpDraw[nflip],0,0,debug,255);
*/

			/* End of Cycle 30 ms */
			while( (GpTickCountGet()-nTick)<30 );

		/* Cambiamos de pantalla (visualizamos lo dibujado) */
		GpSurfaceSet( &gpDraw[nflip] );

		/* Cambiamos para que la próxima vez se dibuje en la otra pantalla */
		if(nflip==1) nflip=0; else nflip=1;

 }
 if (joyas==0) { cc=0; nivel++; goto nuevo_nivel; }
 if (gameover) goto fin;
 goto nueva_pieza;


 /* Game Over */
 fin:

	/* Dibujamos Game Over */
	if(nflip==1) nflip=0; else nflip=1;
	GpTransBlt( NULL, &gpDraw[nflip],(125),(107),gpDraw[nflip].buf_w,gpDraw[nflip].buf_h,(unsigned char*)game_over,0,0,game_over_width,game_over_height,0);
 Delay(5000);
 goto Presentacion;

 /* Fin del Juego */
 fin2:
	GpRectFill( NULL, &gpDraw[nflip],0,0,gpDraw[nflip].buf_w,gpDraw[nflip].buf_h,0x00 );
	GpBitBlt( NULL, &gpDraw[nflip],32,20,gpDraw[nflip].buf_w,gpDraw[nflip].buf_h,(unsigned char*)end,0,0,end_width,end_height);
	GpSurfaceSet( &gpDraw[nflip] );
 Delay(5000);

	}
}

/**************************************************************
 * void saca_mapa(void) : Saca mapa de piezas por la pantalla *
 **************************************************************/

void saca_mapa(void)
{
 int a,b;
 for (a=0;a<6;a++)
 { 
  for (b=0;b<13;b++)
  {
    if (tablero[a][b]==1) GpBitBlt( NULL, &gpDraw[nflip],(OFFSET_X+(81+(a*15)+a)),(OFFSET_Y+(3+(b*15)+b)),gpDraw[nflip].buf_w,gpDraw[nflip].buf_h,(unsigned char*)pieza1,0,0,pieza1_width,pieza1_height);
    if (tablero[a][b]==2) GpBitBlt( NULL, &gpDraw[nflip],(OFFSET_X+(81+(a*15)+a)),(OFFSET_Y+(3+(b*15)+b)),gpDraw[nflip].buf_w,gpDraw[nflip].buf_h,(unsigned char*)pieza2,0,0,pieza2_width,pieza2_height);
    if (tablero[a][b]==3) GpBitBlt( NULL, &gpDraw[nflip],(OFFSET_X+(81+(a*15)+a)),(OFFSET_Y+(3+(b*15)+b)),gpDraw[nflip].buf_w,gpDraw[nflip].buf_h,(unsigned char*)pieza3,0,0,pieza3_width,pieza3_height);
    if (tablero[a][b]==4) GpBitBlt( NULL, &gpDraw[nflip],(OFFSET_X+(81+(a*15)+a)),(OFFSET_Y+(3+(b*15)+b)),gpDraw[nflip].buf_w,gpDraw[nflip].buf_h,(unsigned char*)pieza4,0,0,pieza4_width,pieza4_height);
    if (tablero[a][b]==5) GpBitBlt( NULL, &gpDraw[nflip],(OFFSET_X+(81+(a*15)+a)),(OFFSET_Y+(3+(b*15)+b)),gpDraw[nflip].buf_w,gpDraw[nflip].buf_h,(unsigned char*)pieza5,0,0,pieza5_width,pieza5_height);
    if (tablero[a][b]==6) GpBitBlt( NULL, &gpDraw[nflip],(OFFSET_X+(81+(a*15)+a)),(OFFSET_Y+(3+(b*15)+b)),gpDraw[nflip].buf_w,gpDraw[nflip].buf_h,(unsigned char*)pieza6,0,0,pieza6_width,pieza6_height);
  }
 }

 if (p1==1) GpTransBlt( NULL, &gpDraw[nflip],(OFFSET_X+px),(OFFSET_Y+py),gpDraw[nflip].buf_w,gpDraw[nflip].buf_h,(unsigned char*)pieza1,0,0,pieza1_width,pieza1_height,36);
 if (p1==2) GpTransBlt( NULL, &gpDraw[nflip],(OFFSET_X+px),(OFFSET_Y+py),gpDraw[nflip].buf_w,gpDraw[nflip].buf_h,(unsigned char*)pieza2,0,0,pieza2_width,pieza2_height,36);
 if (p1==3) GpTransBlt( NULL, &gpDraw[nflip],(OFFSET_X+px),(OFFSET_Y+py),gpDraw[nflip].buf_w,gpDraw[nflip].buf_h,(unsigned char*)pieza3,0,0,pieza3_width,pieza3_height,36);
 if (p1==4) GpTransBlt( NULL, &gpDraw[nflip],(OFFSET_X+px),(OFFSET_Y+py),gpDraw[nflip].buf_w,gpDraw[nflip].buf_h,(unsigned char*)pieza4,0,0,pieza4_width,pieza4_height,36);
 if (p1==5) GpTransBlt( NULL, &gpDraw[nflip],(OFFSET_X+px),(OFFSET_Y+py),gpDraw[nflip].buf_w,gpDraw[nflip].buf_h,(unsigned char*)pieza5,0,0,pieza5_width,pieza5_height,36);
 if (p1==6) GpTransBlt( NULL, &gpDraw[nflip],(OFFSET_X+px),(OFFSET_Y+py),gpDraw[nflip].buf_w,gpDraw[nflip].buf_h,(unsigned char*)pieza6,0,0,pieza6_width,pieza6_height,36);

 if (p2==1) GpTransBlt( NULL, &gpDraw[nflip],(OFFSET_X+px),(OFFSET_Y+py+16),gpDraw[nflip].buf_w,gpDraw[nflip].buf_h,(unsigned char*)pieza1,0,0,pieza1_width,pieza1_height,36);
 if (p2==2) GpTransBlt( NULL, &gpDraw[nflip],(OFFSET_X+px),(OFFSET_Y+py+16),gpDraw[nflip].buf_w,gpDraw[nflip].buf_h,(unsigned char*)pieza2,0,0,pieza2_width,pieza2_height,36);
 if (p2==3) GpTransBlt( NULL, &gpDraw[nflip],(OFFSET_X+px),(OFFSET_Y+py+16),gpDraw[nflip].buf_w,gpDraw[nflip].buf_h,(unsigned char*)pieza3,0,0,pieza3_width,pieza3_height,36);
 if (p2==4) GpTransBlt( NULL, &gpDraw[nflip],(OFFSET_X+px),(OFFSET_Y+py+16),gpDraw[nflip].buf_w,gpDraw[nflip].buf_h,(unsigned char*)pieza4,0,0,pieza4_width,pieza4_height,36);
 if (p2==5) GpTransBlt( NULL, &gpDraw[nflip],(OFFSET_X+px),(OFFSET_Y+py+16),gpDraw[nflip].buf_w,gpDraw[nflip].buf_h,(unsigned char*)pieza5,0,0,pieza5_width,pieza5_height,36);
 if (p2==6) GpTransBlt( NULL, &gpDraw[nflip],(OFFSET_X+px),(OFFSET_Y+py+16),gpDraw[nflip].buf_w,gpDraw[nflip].buf_h,(unsigned char*)pieza6,0,0,pieza6_width,pieza6_height,36);
     
 if (p3==1) GpTransBlt( NULL, &gpDraw[nflip],(OFFSET_X+px),(OFFSET_Y+py+32),gpDraw[nflip].buf_w,gpDraw[nflip].buf_h,(unsigned char*)pieza1,0,0,pieza1_width,pieza1_height,36);
 if (p3==2) GpTransBlt( NULL, &gpDraw[nflip],(OFFSET_X+px),(OFFSET_Y+py+32),gpDraw[nflip].buf_w,gpDraw[nflip].buf_h,(unsigned char*)pieza2,0,0,pieza2_width,pieza2_height,36);
 if (p3==3) GpTransBlt( NULL, &gpDraw[nflip],(OFFSET_X+px),(OFFSET_Y+py+32),gpDraw[nflip].buf_w,gpDraw[nflip].buf_h,(unsigned char*)pieza3,0,0,pieza3_width,pieza3_height,36);
 if (p3==4) GpTransBlt( NULL, &gpDraw[nflip],(OFFSET_X+px),(OFFSET_Y+py+32),gpDraw[nflip].buf_w,gpDraw[nflip].buf_h,(unsigned char*)pieza4,0,0,pieza4_width,pieza4_height,36);
 if (p3==5) GpTransBlt( NULL, &gpDraw[nflip],(OFFSET_X+px),(OFFSET_Y+py+32),gpDraw[nflip].buf_w,gpDraw[nflip].buf_h,(unsigned char*)pieza5,0,0,pieza5_width,pieza5_height,36);
 if (p3==6) GpTransBlt( NULL, &gpDraw[nflip],(OFFSET_X+px),(OFFSET_Y+py+32),gpDraw[nflip].buf_w,gpDraw[nflip].buf_h,(unsigned char*)pieza6,0,0,pieza6_width,pieza6_height,36);

}



/******************************************
 * void control(void) : Control del juego *
 ******************************************/

void control(void)
{
 int a;
 
 if (con) 
 { 
    lee_control();
    /*con=0;*/
    if (disparo1==1)  { a=p3; p3=p2; p2=p1; p1=a; }
    if (disparo2==1)  { a=p1; p1=p2; p2=p3; p3=a; }
    
    if (izquierda==1 || izquierda==5 || izquierda>8)
     { if ( (px>81) && (colision(px-16,py+15)==0) ) px=px-16; }
    
    if (derecha==1 || derecha>5 || derecha>8)
     { if ( (px<161) && (colision(px+16,py+15)==0) ) px=px+16; }
    
    if (abajo==1 || abajo>4)
    {
      if ( (py<163) && (colision(px,py+16)==0) )
      py=py+8; /**/ puntos=puntos+25; /**/
    }
 }
/*
 else
 { 
    lee_control();
    con=1; 
    if (abajo)
    {
      if ( (py<163) && (colision(px,py+16)==0) )
      py=py+8; puntos=puntos+25;
    }
 }
*/

 if ( (py<163) && (colision(px,py+16)==0) )
  { cc++; if (cc==ccaida) {cc=0; py=py+vcaida;} }
 else 
  fin_pieza();
}



/****************************************************************
 * int colision(int x,int y) : Indica si hay colision de piezas *
 ****************************************************************/

int colision(int x,int y)
{
 int cx,cy;
 cx=(x-81)/16;
 cy=(y-3)/16;
 if (tablero[cx][cy]!=0) return(1);
 if (tablero[cx][cy+1]!=0) return(1);
 if (tablero[cx][cy+2]!=0) return(1);
 return(0);
}



/*******************************************************************
 * void fin_pieza(void) : Hace lo necesario cuando pieza colisiona *
 *******************************************************************/

void fin_pieza(void)
{
 int cx,cy;
 cx=(px-81)/16;
 cy=(py-3)/16;
 if ((tablero[cx][cy]!=0)||(tablero[cx][cy+1]!=0)) gameover=1;
 tablero[cx][cy]=p1;
 tablero[cx][cy+1]=p2;
 tablero[cx][cy+2]=p3;
 fin_p=1;
 descarta();
} 



/****************************************************************
 * void descarta(void) : Descarta piezas cuando pieza colisiona *
 ****************************************************************/

void descarta(void)
{
  int a,b,c,d,e;
  int descar[6][13];
  int resul;
  int mas_puntos;

  ides:
  
  mas_puntos=0;
  resul=0;
  for (a=0;a<6;a++)
   for (b=0;b<13;b++)
    descar[a][b]=0;

  for (b=0;b<13;b++)               /* Comprobamos filas */
  {
   for (a=0;a<4;a++)
   {
     if ( (tablero[a][b]==tablero[a+1][b]) && (tablero[a][b]==tablero[a+2][b]) )
       { descar[a][b]=1; descar[a+1][b]=1; descar[a+2][b]=1; }
   }
  }

  for (a=0;a<6;a++)                /* Comprobamos columnas */
  {
   for (b=0;b<11;b++)
   {
     if ( (tablero[a][b]==tablero[a][b+1]) && (tablero[a][b]==tablero[a][b+2]) )
       { descar[a][b]=1; descar[a][b+1]=1; descar[a][b+2]=1; }
   }
  }

  for (a=0;a<4;a++)                /* Comprobamos Oblicuas \ */
  {
   for (b=0;b<11;b++)
   {
     if ( (tablero[a][b]==tablero[a+1][b+1]) && (tablero[a][b]==tablero[a+2][b+2]) )
       { descar[a][b]=1; descar[a+1][b+1]=1; descar[a+2][b+2]=1; }
    }
   }

  for (a=2;a<6;a++)                /* Comprobamos Oblicuas / */
  {
   for (b=0;b<11;b++)
   {
     if ( (tablero[a][b]==tablero[a-1][b+1]) && (tablero[a][b]==tablero[a-2][b+2]) )
       { descar[a][b]=1; descar[a-1][b+1]=1; descar[a-2][b+2]=1; }
   }
  }

  
  c=0;
  for (a=0;a<6;a++)                /* Recuadramos fichas a descartar */
  {
    for (b=0;b<13;b++)
    {
      if ( (descar[a][b]==1)&&(tablero[a][b]!=0) ) 
        { 
          c++; 
          if (c==1) 
            {
														GpSurfaceSet( &gpDraw[nflip] );
														if(nflip==1) nflip=0; else nflip=1;

														/* Dibujamos marcador */
														GpBitBlt( NULL, &gpDraw[nflip],OFFSET_X,OFFSET_Y,gpDraw[nflip].buf_w,gpDraw[nflip].buf_h,(unsigned char*)marcad_image,0,0,marcad_width,marcad_height);
              saca_pn();
              saca_puntos();
              for (d=0;d<6;d++)
              { 
                for (e=0;e<13;e++)
                {

    													if (tablero[d][e]==1) GpBitBlt( NULL, &gpDraw[nflip],(OFFSET_X+(81+(d*15)+d)),(OFFSET_Y+(3+(e*15)+e)),gpDraw[nflip].buf_w,gpDraw[nflip].buf_h,(unsigned char*)pieza1,0,0,pieza1_width,pieza1_height);
 																if (tablero[d][e]==2) GpBitBlt( NULL, &gpDraw[nflip],(OFFSET_X+(81+(d*15)+d)),(OFFSET_Y+(3+(e*15)+e)),gpDraw[nflip].buf_w,gpDraw[nflip].buf_h,(unsigned char*)pieza2,0,0,pieza2_width,pieza2_height);
    													if (tablero[d][e]==3) GpBitBlt( NULL, &gpDraw[nflip],(OFFSET_X+(81+(d*15)+d)),(OFFSET_Y+(3+(e*15)+e)),gpDraw[nflip].buf_w,gpDraw[nflip].buf_h,(unsigned char*)pieza3,0,0,pieza3_width,pieza3_height);
    													if (tablero[d][e]==4) GpBitBlt( NULL, &gpDraw[nflip],(OFFSET_X+(81+(d*15)+d)),(OFFSET_Y+(3+(e*15)+e)),gpDraw[nflip].buf_w,gpDraw[nflip].buf_h,(unsigned char*)pieza4,0,0,pieza4_width,pieza4_height);
    													if (tablero[d][e]==5) GpBitBlt( NULL, &gpDraw[nflip],(OFFSET_X+(81+(d*15)+d)),(OFFSET_Y+(3+(e*15)+e)),gpDraw[nflip].buf_w,gpDraw[nflip].buf_h,(unsigned char*)pieza5,0,0,pieza5_width,pieza5_height);
    													if (tablero[d][e]==6) GpBitBlt( NULL, &gpDraw[nflip],(OFFSET_X+(81+(d*15)+d)),(OFFSET_Y+(3+(e*15)+e)),gpDraw[nflip].buf_w,gpDraw[nflip].buf_h,(unsigned char*)pieza6,0,0,pieza6_width,pieza6_height);
                }
              }
            }

          GpTransBlt( NULL, &gpDraw[nflip],(OFFSET_X+(a*16+80)),(OFFSET_Y+(b*16+2)),gpDraw[nflip].buf_w,gpDraw[nflip].buf_h,(unsigned char*)piezab,0,0,piezab_width,piezab_height,36);
        }
    }
  }
  mas_puntos=c;
		GpSurfaceSet( &gpDraw[nflip] );
		/* if(nflip==1) nflip=0; else nflip=1;*/

                  
  if (c!=0)                        /* Parpadeo de fichas a descartar */
  {
    for (c=1;c<4;c++)
     {
       for (a=0;a<6;a++)
       {
         for (b=0;b<13;b++)
         {
           if ( (descar[a][b]==1)&&(tablero[a][b]!=0) )
            { 

    										if (tablero[a][b]==1) GpBitBlt( NULL, &gpDraw[nflip],(OFFSET_X+(81+(a*15)+a)),(OFFSET_Y+(3+(b*15)+b)),gpDraw[nflip].buf_w,gpDraw[nflip].buf_h,(unsigned char*)pieza1,0,0,pieza1_width,pieza1_height);
 													if (tablero[a][b]==2) GpBitBlt( NULL, &gpDraw[nflip],(OFFSET_X+(81+(a*15)+a)),(OFFSET_Y+(3+(b*15)+b)),gpDraw[nflip].buf_w,gpDraw[nflip].buf_h,(unsigned char*)pieza2,0,0,pieza2_width,pieza2_height);
    										if (tablero[a][b]==3) GpBitBlt( NULL, &gpDraw[nflip],(OFFSET_X+(81+(a*15)+a)),(OFFSET_Y+(3+(b*15)+b)),gpDraw[nflip].buf_w,gpDraw[nflip].buf_h,(unsigned char*)pieza3,0,0,pieza3_width,pieza3_height);
    										if (tablero[a][b]==4) GpBitBlt( NULL, &gpDraw[nflip],(OFFSET_X+(81+(a*15)+a)),(OFFSET_Y+(3+(b*15)+b)),gpDraw[nflip].buf_w,gpDraw[nflip].buf_h,(unsigned char*)pieza4,0,0,pieza4_width,pieza4_height);
    										if (tablero[a][b]==5) GpBitBlt( NULL, &gpDraw[nflip],(OFFSET_X+(81+(a*15)+a)),(OFFSET_Y+(3+(b*15)+b)),gpDraw[nflip].buf_w,gpDraw[nflip].buf_h,(unsigned char*)pieza5,0,0,pieza5_width,pieza5_height);
    										if (tablero[a][b]==6) GpBitBlt( NULL, &gpDraw[nflip],(OFFSET_X+(81+(a*15)+a)),(OFFSET_Y+(3+(b*15)+b)),gpDraw[nflip].buf_w,gpDraw[nflip].buf_h,(unsigned char*)pieza6,0,0,pieza6_width,pieza6_height);
            }
         }
       }
       Delay(200);

							/* GpSurfaceSet( &gpDraw[nflip] ); */
							/* if(nflip==1) nflip=0; else nflip=1; */

       for (a=0;a<6;a++)
       {
         for (b=0;b<13;b++)
         {
           if ( (descar[a][b]==1)&&(tablero[a][b]!=0) )
											  GpBitBlt( NULL, &gpDraw[nflip],(OFFSET_X+(81+(a*15)+a)),(OFFSET_Y+(3+(b*15)+b)),gpDraw[nflip].buf_w,gpDraw[nflip].buf_h,(unsigned char*)pieza0,0,0,pieza0_width,pieza0_height);
             /*putImage( (81+(a*15)+a),(3+(b*15)+b), pieza0);*/
         }
       }
       Delay(300);
							/* GpSurfaceSet( &gpDraw[nflip] ); */
							/* if(nflip==1) nflip=0; else nflip=1; */
     }
  }
  

  for (a=0;a<6;a++)                /* Quitamos fichas descartables */
  { 
   for (b=0;b<13;b++)
    { 
      if (descar[a][b]==1) 
      {
        if (tablero[a][b]!=0) resul=1;
        tablero[a][b]=0;
      }
    }
  }

  for (a=0;a<6;a++)                /* Desplazamos hacia abajo */
  {
   for (b=0;b<12;b++)
   {
    if ( (tablero[a][b]!=0) && (tablero[a][b+1]==0) )
     {
       for (c=b+1;c>0;c--)
        tablero[a][c]=tablero[a][c-1];
     }
   }
  }

 if (mas_puntos==3) puntos=puntos+1250;
 if (mas_puntos==4) puntos=puntos+2500;
 if (mas_puntos==5) puntos=puntos+5000;
 if (mas_puntos==6) puntos=puntos+10000;
 if (mas_puntos==7) puntos=puntos+20000;
 if (mas_puntos==8) puntos=puntos+40000;
 if (mas_puntos==9) puntos=puntos+80000;
 if (mas_puntos>=10) puntos=puntos+160000;
 if (mas_puntos>joyas) joyas=0; else joyas=joyas-mas_puntos;

 if (resul==1) goto ides;
}


void saca_pn(void)
{
  if (pn1==1) GpBitBlt( NULL, &gpDraw[nflip],(OFFSET_X+193),(OFFSET_Y+19),gpDraw[nflip].buf_w,gpDraw[nflip].buf_h,(unsigned char*)pieza1,0,0,pieza1_width,pieza1_height);
  if (pn1==2) GpBitBlt( NULL, &gpDraw[nflip],(OFFSET_X+193),(OFFSET_Y+19),gpDraw[nflip].buf_w,gpDraw[nflip].buf_h,(unsigned char*)pieza2,0,0,pieza2_width,pieza2_height);
  if (pn1==3) GpBitBlt( NULL, &gpDraw[nflip],(OFFSET_X+193),(OFFSET_Y+19),gpDraw[nflip].buf_w,gpDraw[nflip].buf_h,(unsigned char*)pieza3,0,0,pieza3_width,pieza3_height);
  if (pn1==4) GpBitBlt( NULL, &gpDraw[nflip],(OFFSET_X+193),(OFFSET_Y+19),gpDraw[nflip].buf_w,gpDraw[nflip].buf_h,(unsigned char*)pieza4,0,0,pieza4_width,pieza4_height);
  if (pn1==5) GpBitBlt( NULL, &gpDraw[nflip],(OFFSET_X+193),(OFFSET_Y+19),gpDraw[nflip].buf_w,gpDraw[nflip].buf_h,(unsigned char*)pieza5,0,0,pieza5_width,pieza5_height);
  if (pn1==6) GpBitBlt( NULL, &gpDraw[nflip],(OFFSET_X+193),(OFFSET_Y+19),gpDraw[nflip].buf_w,gpDraw[nflip].buf_h,(unsigned char*)pieza6,0,0,pieza6_width,pieza6_height);

  if (pn2==1) GpBitBlt( NULL, &gpDraw[nflip],(OFFSET_X+193),(OFFSET_Y+35),gpDraw[nflip].buf_w,gpDraw[nflip].buf_h,(unsigned char*)pieza1,0,0,pieza1_width,pieza1_height);
  if (pn2==2) GpBitBlt( NULL, &gpDraw[nflip],(OFFSET_X+193),(OFFSET_Y+35),gpDraw[nflip].buf_w,gpDraw[nflip].buf_h,(unsigned char*)pieza2,0,0,pieza2_width,pieza2_height);
  if (pn2==3) GpBitBlt( NULL, &gpDraw[nflip],(OFFSET_X+193),(OFFSET_Y+35),gpDraw[nflip].buf_w,gpDraw[nflip].buf_h,(unsigned char*)pieza3,0,0,pieza3_width,pieza3_height);
  if (pn2==4) GpBitBlt( NULL, &gpDraw[nflip],(OFFSET_X+193),(OFFSET_Y+35),gpDraw[nflip].buf_w,gpDraw[nflip].buf_h,(unsigned char*)pieza4,0,0,pieza4_width,pieza4_height);
  if (pn2==5) GpBitBlt( NULL, &gpDraw[nflip],(OFFSET_X+193),(OFFSET_Y+35),gpDraw[nflip].buf_w,gpDraw[nflip].buf_h,(unsigned char*)pieza5,0,0,pieza5_width,pieza5_height);
  if (pn2==6) GpBitBlt( NULL, &gpDraw[nflip],(OFFSET_X+193),(OFFSET_Y+35),gpDraw[nflip].buf_w,gpDraw[nflip].buf_h,(unsigned char*)pieza6,0,0,pieza6_width,pieza6_height);

  if (pn3==1) GpBitBlt( NULL, &gpDraw[nflip],(OFFSET_X+193),(OFFSET_Y+51),gpDraw[nflip].buf_w,gpDraw[nflip].buf_h,(unsigned char*)pieza1,0,0,pieza1_width,pieza1_height);
  if (pn3==2) GpBitBlt( NULL, &gpDraw[nflip],(OFFSET_X+193),(OFFSET_Y+51),gpDraw[nflip].buf_w,gpDraw[nflip].buf_h,(unsigned char*)pieza2,0,0,pieza2_width,pieza2_height);
  if (pn3==3) GpBitBlt( NULL, &gpDraw[nflip],(OFFSET_X+193),(OFFSET_Y+51),gpDraw[nflip].buf_w,gpDraw[nflip].buf_h,(unsigned char*)pieza3,0,0,pieza3_width,pieza3_height);
  if (pn3==4) GpBitBlt( NULL, &gpDraw[nflip],(OFFSET_X+193),(OFFSET_Y+51),gpDraw[nflip].buf_w,gpDraw[nflip].buf_h,(unsigned char*)pieza4,0,0,pieza4_width,pieza4_height);
  if (pn3==5) GpBitBlt( NULL, &gpDraw[nflip],(OFFSET_X+193),(OFFSET_Y+51),gpDraw[nflip].buf_w,gpDraw[nflip].buf_h,(unsigned char*)pieza5,0,0,pieza5_width,pieza5_height);
  if (pn3==6) GpBitBlt( NULL, &gpDraw[nflip],(OFFSET_X+193),(OFFSET_Y+51),gpDraw[nflip].buf_w,gpDraw[nflip].buf_h,(unsigned char*)pieza6,0,0,pieza6_width,pieza6_height);
}



/***********************************************************
 * void saca_puntos(void) : Saca puntacion por la pantalla *
 ***********************************************************/

void saca_puntos(void)
{
 int a;
 char punt[12];
 char nive[3];
 char joya[5];

	sprintf(punt,"%8d",puntos);
 sprintf(nive,"%d",nivel);
 sprintf(joya,"%3d",joyas);

 for(a=0;a<strlen(punt);a++)
 {
   if (punt[a]=='0') GpBitBlt( NULL, &gpDraw[nflip],(OFFSET_X+(13+a*7)),(OFFSET_Y+(88)),gpDraw[nflip].buf_w,gpDraw[nflip].buf_h,(unsigned char*)cero,0,0,cero_width,cero_height);
   if (punt[a]=='1') GpBitBlt( NULL, &gpDraw[nflip],(OFFSET_X+(13+a*7)),(OFFSET_Y+(88)),gpDraw[nflip].buf_w,gpDraw[nflip].buf_h,(unsigned char*)uno,0,0,cero_width,cero_height);
   if (punt[a]=='2') GpBitBlt( NULL, &gpDraw[nflip],(OFFSET_X+(13+a*7)),(OFFSET_Y+(88)),gpDraw[nflip].buf_w,gpDraw[nflip].buf_h,(unsigned char*)dos,0,0,cero_width,cero_height);
   if (punt[a]=='3') GpBitBlt( NULL, &gpDraw[nflip],(OFFSET_X+(13+a*7)),(OFFSET_Y+(88)),gpDraw[nflip].buf_w,gpDraw[nflip].buf_h,(unsigned char*)tres,0,0,cero_width,cero_height);
   if (punt[a]=='4') GpBitBlt( NULL, &gpDraw[nflip],(OFFSET_X+(13+a*7)),(OFFSET_Y+(88)),gpDraw[nflip].buf_w,gpDraw[nflip].buf_h,(unsigned char*)cuatro,0,0,cero_width,cero_height);
   if (punt[a]=='5') GpBitBlt( NULL, &gpDraw[nflip],(OFFSET_X+(13+a*7)),(OFFSET_Y+(88)),gpDraw[nflip].buf_w,gpDraw[nflip].buf_h,(unsigned char*)cinco,0,0,cero_width,cero_height);
   if (punt[a]=='6') GpBitBlt( NULL, &gpDraw[nflip],(OFFSET_X+(13+a*7)),(OFFSET_Y+(88)),gpDraw[nflip].buf_w,gpDraw[nflip].buf_h,(unsigned char*)seis,0,0,cero_width,cero_height);
   if (punt[a]=='7') GpBitBlt( NULL, &gpDraw[nflip],(OFFSET_X+(13+a*7)),(OFFSET_Y+(88)),gpDraw[nflip].buf_w,gpDraw[nflip].buf_h,(unsigned char*)siete,0,0,cero_width,cero_height);
   if (punt[a]=='8') GpBitBlt( NULL, &gpDraw[nflip],(OFFSET_X+(13+a*7)),(OFFSET_Y+(88)),gpDraw[nflip].buf_w,gpDraw[nflip].buf_h,(unsigned char*)ocho,0,0,cero_width,cero_height);
   if (punt[a]=='9') GpBitBlt( NULL, &gpDraw[nflip],(OFFSET_X+(13+a*7)),(OFFSET_Y+(88)),gpDraw[nflip].buf_w,gpDraw[nflip].buf_h,(unsigned char*)nueve,0,0,cero_width,cero_height);
 }

 for(a=0;a<strlen(nive);a++)
 {
   if (nive[a]=='0') GpBitBlt( NULL, &gpDraw[nflip],(OFFSET_X+(59+a*7)),(OFFSET_Y+(136)),gpDraw[nflip].buf_w,gpDraw[nflip].buf_h,(unsigned char*)cero,0,0,cero_width,cero_height);
   if (nive[a]=='1') GpBitBlt( NULL, &gpDraw[nflip],(OFFSET_X+(59+a*7)),(OFFSET_Y+(136)),gpDraw[nflip].buf_w,gpDraw[nflip].buf_h,(unsigned char*)uno,0,0,cero_width,cero_height);
   if (nive[a]=='2') GpBitBlt( NULL, &gpDraw[nflip],(OFFSET_X+(59+a*7)),(OFFSET_Y+(136)),gpDraw[nflip].buf_w,gpDraw[nflip].buf_h,(unsigned char*)dos,0,0,cero_width,cero_height);
   if (nive[a]=='3') GpBitBlt( NULL, &gpDraw[nflip],(OFFSET_X+(59+a*7)),(OFFSET_Y+(136)),gpDraw[nflip].buf_w,gpDraw[nflip].buf_h,(unsigned char*)tres,0,0,cero_width,cero_height);
   if (nive[a]=='4') GpBitBlt( NULL, &gpDraw[nflip],(OFFSET_X+(59+a*7)),(OFFSET_Y+(136)),gpDraw[nflip].buf_w,gpDraw[nflip].buf_h,(unsigned char*)cuatro,0,0,cero_width,cero_height);
   if (nive[a]=='5') GpBitBlt( NULL, &gpDraw[nflip],(OFFSET_X+(59+a*7)),(OFFSET_Y+(136)),gpDraw[nflip].buf_w,gpDraw[nflip].buf_h,(unsigned char*)cinco,0,0,cero_width,cero_height);
   if (nive[a]=='6') GpBitBlt( NULL, &gpDraw[nflip],(OFFSET_X+(59+a*7)),(OFFSET_Y+(136)),gpDraw[nflip].buf_w,gpDraw[nflip].buf_h,(unsigned char*)seis,0,0,cero_width,cero_height);
   if (nive[a]=='7') GpBitBlt( NULL, &gpDraw[nflip],(OFFSET_X+(59+a*7)),(OFFSET_Y+(136)),gpDraw[nflip].buf_w,gpDraw[nflip].buf_h,(unsigned char*)siete,0,0,cero_width,cero_height);
   if (nive[a]=='8') GpBitBlt( NULL, &gpDraw[nflip],(OFFSET_X+(59+a*7)),(OFFSET_Y+(136)),gpDraw[nflip].buf_w,gpDraw[nflip].buf_h,(unsigned char*)ocho,0,0,cero_width,cero_height);
   if (nive[a]=='9') GpBitBlt( NULL, &gpDraw[nflip],(OFFSET_X+(59+a*7)),(OFFSET_Y+(136)),gpDraw[nflip].buf_w,gpDraw[nflip].buf_h,(unsigned char*)nueve,0,0,cero_width,cero_height);
 }

 for(a=0;a<strlen(joya);a++)
 {
   if (joya[a]=='0') GpBitBlt( NULL, &gpDraw[nflip],(OFFSET_X+(45+a*7)),(OFFSET_Y+(184)),gpDraw[nflip].buf_w,gpDraw[nflip].buf_h,(unsigned char*)cero,0,0,cero_width,cero_height);
   if (joya[a]=='1') GpBitBlt( NULL, &gpDraw[nflip],(OFFSET_X+(45+a*7)),(OFFSET_Y+(184)),gpDraw[nflip].buf_w,gpDraw[nflip].buf_h,(unsigned char*)uno,0,0,cero_width,cero_height);
   if (joya[a]=='2') GpBitBlt( NULL, &gpDraw[nflip],(OFFSET_X+(45+a*7)),(OFFSET_Y+(184)),gpDraw[nflip].buf_w,gpDraw[nflip].buf_h,(unsigned char*)dos,0,0,cero_width,cero_height);
   if (joya[a]=='3') GpBitBlt( NULL, &gpDraw[nflip],(OFFSET_X+(45+a*7)),(OFFSET_Y+(184)),gpDraw[nflip].buf_w,gpDraw[nflip].buf_h,(unsigned char*)tres,0,0,cero_width,cero_height);
   if (joya[a]=='4') GpBitBlt( NULL, &gpDraw[nflip],(OFFSET_X+(45+a*7)),(OFFSET_Y+(184)),gpDraw[nflip].buf_w,gpDraw[nflip].buf_h,(unsigned char*)cuatro,0,0,cero_width,cero_height);
   if (joya[a]=='5') GpBitBlt( NULL, &gpDraw[nflip],(OFFSET_X+(45+a*7)),(OFFSET_Y+(184)),gpDraw[nflip].buf_w,gpDraw[nflip].buf_h,(unsigned char*)cinco,0,0,cero_width,cero_height);
   if (joya[a]=='6') GpBitBlt( NULL, &gpDraw[nflip],(OFFSET_X+(45+a*7)),(OFFSET_Y+(184)),gpDraw[nflip].buf_w,gpDraw[nflip].buf_h,(unsigned char*)seis,0,0,cero_width,cero_height);
   if (joya[a]=='7') GpBitBlt( NULL, &gpDraw[nflip],(OFFSET_X+(45+a*7)),(OFFSET_Y+(184)),gpDraw[nflip].buf_w,gpDraw[nflip].buf_h,(unsigned char*)siete,0,0,cero_width,cero_height);
   if (joya[a]=='8') GpBitBlt( NULL, &gpDraw[nflip],(OFFSET_X+(45+a*7)),(OFFSET_Y+(184)),gpDraw[nflip].buf_w,gpDraw[nflip].buf_h,(unsigned char*)ocho,0,0,cero_width,cero_height);
   if (joya[a]=='9') GpBitBlt( NULL, &gpDraw[nflip],(OFFSET_X+(45+a*7)),(OFFSET_Y+(184)),gpDraw[nflip].buf_w,gpDraw[nflip].buf_h,(unsigned char*)nueve,0,0,cero_width,cero_height);
 }
} 



/******************************************************************
 * void destruye(void) : Finaliza nivel, quitando todas las joyas *
 ******************************************************************/

void destruye(void)    
{
  int a,b,c;
		nivel--;

  for (c=0;c<13;c++)
  {

			/* Dibujamos marcador */
			GpBitBlt( NULL, &gpDraw[nflip],OFFSET_X,OFFSET_Y,gpDraw[nflip].buf_w,gpDraw[nflip].buf_h,(unsigned char*)marcad_image,0,0,marcad_width,marcad_height);
  	saca_pn();
  	saca_puntos();

    for (a=0;a<6;a++)
    {
      for (b=0;b<13;b++)
      {

    				if (tablero[a][b]==1) GpBitBlt( NULL, &gpDraw[nflip],(OFFSET_X+(81+(a*15)+a)),(OFFSET_Y+(3+(b*15)+b)),gpDraw[nflip].buf_w,gpDraw[nflip].buf_h,(unsigned char*)pieza1,0,0,pieza1_width,pieza1_height);
 							if (tablero[a][b]==2) GpBitBlt( NULL, &gpDraw[nflip],(OFFSET_X+(81+(a*15)+a)),(OFFSET_Y+(3+(b*15)+b)),gpDraw[nflip].buf_w,gpDraw[nflip].buf_h,(unsigned char*)pieza2,0,0,pieza2_width,pieza2_height);
    				if (tablero[a][b]==3) GpBitBlt( NULL, &gpDraw[nflip],(OFFSET_X+(81+(a*15)+a)),(OFFSET_Y+(3+(b*15)+b)),gpDraw[nflip].buf_w,gpDraw[nflip].buf_h,(unsigned char*)pieza3,0,0,pieza3_width,pieza3_height);
    				if (tablero[a][b]==4) GpBitBlt( NULL, &gpDraw[nflip],(OFFSET_X+(81+(a*15)+a)),(OFFSET_Y+(3+(b*15)+b)),gpDraw[nflip].buf_w,gpDraw[nflip].buf_h,(unsigned char*)pieza4,0,0,pieza4_width,pieza4_height);
    				if (tablero[a][b]==5) GpBitBlt( NULL, &gpDraw[nflip],(OFFSET_X+(81+(a*15)+a)),(OFFSET_Y+(3+(b*15)+b)),gpDraw[nflip].buf_w,gpDraw[nflip].buf_h,(unsigned char*)pieza5,0,0,pieza5_width,pieza5_height);
    				if (tablero[a][b]==6) GpBitBlt( NULL, &gpDraw[nflip],(OFFSET_X+(81+(a*15)+a)),(OFFSET_Y+(3+(b*15)+b)),gpDraw[nflip].buf_w,gpDraw[nflip].buf_h,(unsigned char*)pieza6,0,0,pieza6_width,pieza6_height);
      }
    }
  
    Delay(100);
				GpSurfaceSet( &gpDraw[nflip] );
				if(nflip==1) nflip=0; else nflip=1;

				/* Dibujamos marcador */
				GpBitBlt( NULL, &gpDraw[nflip],OFFSET_X,OFFSET_Y,gpDraw[nflip].buf_w,gpDraw[nflip].buf_h,(unsigned char*)marcad_image,0,0,marcad_width,marcad_height);
  		saca_pn();
  		saca_puntos();

    for (a=0;a<6;a++)
    {
      for (b=0;b<13;b++)
      {
						  GpBitBlt( NULL, &gpDraw[nflip],(OFFSET_X+(81+(a*15)+a)),(OFFSET_Y+(3+(b*15)+b)),gpDraw[nflip].buf_w,gpDraw[nflip].buf_h,(unsigned char*)pieza0,0,0,pieza0_width,pieza0_height);
        /*putImage( (81+(a*15)+a),(3+(b*15)+b), pieza0);*/
      }
    }
    Delay(100);
				GpSurfaceSet( &gpDraw[nflip] );
				if(nflip==1) nflip=0; else nflip=1;
  }

  for (a=0;a<13;a++)
   for (b=0;b<6;b++)
    tablero[b][a]=0;

		nivel++;
}
