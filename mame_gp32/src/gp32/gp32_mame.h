#ifndef GP32_MAME_H
#define GP32_MAME_H

#include "gpdef.h"
#include "gpstdio.h"
#include "gpstdlib.h"
#include "gpgraphic.h"
#include "gpfont.h"
#include "gpmem.h"
#include "gp32_fileio.h"

void gp32_video_init(void);
void gp32_gamelist_text_out(int x, int y, char *texto);
void gp32_text_out(int x, int y, char *texto);
void gp32_text_out_int(int x, int y, int entero);
void gp32_text_log(char *texto);
void gp32_text_log_int(int entero);
void gp32_clear_screen(void);
void gp32_mame_palette(void);
void gp32_adjust_display(void);
void gp32_gamelist_zero(void);
void gp32_text_pause(void);

extern GPDRAWSURFACE gpDraw[2];	/* Video Buffers */
extern int nflip;		/* Video buffer index */
extern int gp32_rotate;		/* Screen Rotation */

#endif	/* defined GP32_MAME_H */
