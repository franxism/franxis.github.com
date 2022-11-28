#include "driver.h"

/* Atari "Kangaroo hardware" games */
extern struct GameDriver kangaroo_driver;
extern struct GameDriver kangarob_driver;
extern struct GameDriver arabian_driver;

/* Atari "Missile Command hardware" games */
extern struct GameDriver missile_driver;
extern struct GameDriver missile2_driver;
extern struct GameDriver suprmatk_driver;

/* Atari b/w games */
extern struct GameDriver sprint1_driver;
extern struct GameDriver sprint2_driver;
extern struct GameDriver sbrkout_driver;
extern struct GameDriver dominos_driver;
extern struct GameDriver nitedrvr_driver;
extern struct GameDriver bsktball_driver;
extern struct GameDriver copsnrob_driver;
extern struct GameDriver avalnche_driver;
extern struct GameDriver subs_driver;

/* Atari System 1 games */
extern struct GameDriver marble_driver;
extern struct GameDriver marble2_driver;
extern struct GameDriver marblea_driver;

/* later Atari games */
extern struct GameDriver gauntlet_driver;
extern struct GameDriver gauntir1_driver;
extern struct GameDriver gauntir2_driver;
extern struct GameDriver gaunt2p_driver;
extern struct GameDriver gaunt2_driver;
extern struct GameDriver atetris_driver;
extern struct GameDriver atetrisa_driver;
extern struct GameDriver atetrisb_driver;
extern struct GameDriver atetcktl_driver;
extern struct GameDriver atetckt2_driver;
extern struct GameDriver klax_driver;
extern struct GameDriver klax2_driver;
extern struct GameDriver klax3_driver;
extern struct GameDriver xybots_driver;

extern struct GameDriver cloak_driver;
extern struct GameDriver foodf_driver;
extern struct GameDriver jedi_driver;

const struct GameDriver *drivers[] =
{
	&arabian_driver,	/* (c) 1983 */
	&avalnche_driver,	/* no copyright notice [1978] */
	&bsktball_driver,	/* no copyright notice */
	&cloak_driver,		/* (c) 1983 Atari */
	&copsnrob_driver,	/* [1976] */
	&dominos_driver,	/* no copyright notice */
	&foodf_driver,		/* (c) 1982 Atari */
	&gaunt2_driver,		/* (c) 1986 */
	&gaunt2p_driver,	/* (c) 1985 */
	&gauntir1_driver,	/* (c) 1985 */
	&gauntir2_driver,	/* (c) 1985 */
	&gauntlet_driver,	/* (c) 1985 */
	&kangarob_driver,	/* bootleg */
	&kangaroo_driver,	/* (c) 1982 */
	&klax2_driver,		/* (c) 1989 */
	&klax3_driver,		/* (c) 1989 */
	&klax_driver,		/* (c) 1989 */
	&marble2_driver,	/* (c) 1984 */
	&marble_driver,		/* (c) 1984 */
	&marblea_driver,	/* (c) 1984 */
	&missile2_driver,	/* (c) 1980 */
	&missile_driver,	/* (c) 1980 */
	&nitedrvr_driver,	/* no copyright notice [1976] */
	&jedi_driver,		/* (c) 1984 Atari */
	&sbrkout_driver,	/* no copyright notice */
	&sprint1_driver,	/* no copyright notice */
	&sprint2_driver,	/* no copyright notice */
	&subs_driver,		/* no copyright notice [1976] */
	&suprmatk_driver,	/* (c) 1980 + (c) 1981 Gencomp */
	&atetckt2_driver,	/* (c) 1989 */
	&atetcktl_driver,	/* (c) 1989 */
	&atetris_driver,	/* (c) 1988 */
	&atetrisa_driver,	/* (c) 1988 */
	&atetrisb_driver,	/* bootleg */
	&xybots_driver,		/* (c) 1987 */

	0	/* end of array */
};
