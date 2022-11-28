#include "driver.h"

/* Tehkan / Tecmo games (Tehkan became Tecmo in 1986) */
extern struct GameDriver bombjack_driver;
extern struct GameDriver starforc_driver;
extern struct GameDriver megaforc_driver;
extern struct GameDriver pbaction_driver;
extern struct GameDriver pbactio2_driver;
extern struct GameDriver tehkanwc_driver;
extern struct GameDriver gridiron_driver;
extern struct GameDriver solomon_driver;
extern struct GameDriver rygar_driver;
extern struct GameDriver rygar2_driver;
extern struct GameDriver rygarj_driver;
extern struct GameDriver gemini_driver;
extern struct GameDriver silkworm_driver;
extern struct GameDriver silkwrm2_driver;

const struct GameDriver *drivers[] =
{
	&bombjack_driver,	/* (c) 1984 Tehkan */
	&gemini_driver,		/* (c) 1987 Tecmo */
	&gridiron_driver,	/* (c) 1985 Tehkan */
	&megaforc_driver,	/* (c) 1985 Tehkan + Video Ware license */
	&pbactio2_driver,	/* (c) 1985 Tehkan */
	&pbaction_driver,	/* (c) 1985 Tehkan */
	&rygar2_driver,		/* 6002 - (c) 1986 Tecmo */
	&rygar_driver,		/* 6002 - (c) 1986 Tecmo */
	&rygarj_driver,		/* 6002 - (c) 1986 Tecmo */
	&silkworm_driver,	/* 6217 - (c) 1988 Tecmo */
	&silkwrm2_driver,	/* 6217 - (c) 1988 Tecmo */
	&solomon_driver,	/* (c) 1986 Tecmo */
	&starforc_driver,	/* (c) 1984 Tehkan */
	&tehkanwc_driver,	/* (c) 1985 Tehkan */

	0	/* end of array */
};
