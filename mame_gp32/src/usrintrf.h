/*********************************************************************

  usrintrf.h

  Functions used to handle MAME's crude user interface.

*********************************************************************/

#ifndef MAME_USRINTRF_H
#define MAME_USRINTRF_H

#define DT_COLOR_WHITE 0
#define DT_COLOR_YELLOW 1
#define DT_COLOR_RED 2

int showcopyright(void);
void showcredits(void);
int showgamewarnings(void);

int handle_user_interface(void);

#endif
