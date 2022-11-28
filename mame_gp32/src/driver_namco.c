#include "driver.h"

/* Namco games */
extern struct GameDriver warpwarp_driver;
extern struct GameDriver warpwar2_driver;
extern struct GameDriver rallyx_driver;
extern struct GameDriver nrallyx_driver;
extern struct GameDriver jungler_driver;
extern struct GameDriver junglers_driver;
extern struct GameDriver locomotn_driver;
extern struct GameDriver commsega_driver;
extern struct GameDriver bosco_driver;
extern struct GameDriver bosconm_driver;
extern struct GameDriver galaga_driver;
extern struct GameDriver galagamw_driver;
extern struct GameDriver galagads_driver;
extern struct GameDriver gallag_driver;
extern struct GameDriver galagab2_driver;
extern struct GameDriver digdug_driver;
extern struct GameDriver digdugnm_driver;
extern struct GameDriver xevious_driver;
extern struct GameDriver xeviousa_driver;
extern struct GameDriver xevios_driver;
extern struct GameDriver sxevious_driver;
extern struct GameDriver superpac_driver;
extern struct GameDriver superpcn_driver;
extern struct GameDriver pacnpal_driver;
extern struct GameDriver mappy_driver;
extern struct GameDriver mappyjp_driver;
extern struct GameDriver digdug2_driver;
extern struct GameDriver todruaga_driver;
extern struct GameDriver motos_driver;
extern struct GameDriver pacland_driver;
extern struct GameDriver pacland2_driver;
extern struct GameDriver pacland3_driver;
extern struct GameDriver paclandm_driver;

extern struct GameDriver tankbatt_driver;

const struct GameDriver *drivers[] =
{
	&bosco_driver,		/* (c) 1981 Midway */
	&bosconm_driver,	/* (c) 1981 */
	&commsega_driver,	/* (c) 1983 Sega */
	&digdug2_driver,	/* (c) 1985 */
	&digdug_driver,		/* (c) 1982 Atari */
	&digdugnm_driver,	/* (c) 1982 */
	&galaga_driver,		/* (c) 1981 */
	&galagab2_driver,	/* bootleg */
	&galagads_driver,	/* hack */
	&galagamw_driver,	/* (c) 1981 Midway */
	&gallag_driver,		/* bootleg */
	&jungler_driver,	/* GX327 (c) 1981 Konami */
	&junglers_driver,	/* GX327 (c) 1981 Stern */
	&locomotn_driver,	/* GX359 (c) 1982 Konami + Centuri license */
	&mappy_driver,		/* (c) 1983 */
	&mappyjp_driver,	/* (c) 1983 */
	&motos_driver,		/* (c) 1985 */
	&nrallyx_driver,	/* (c) 1981 Namco */
	&pacland2_driver,	/* (c) 1984 */
	&pacland3_driver,	/* (c) 1984 */
	&pacland_driver,	/* (c) 1984 */
	&paclandm_driver,	/* (c) 1984 Midway */
	&pacnpal_driver,	/* (c) 1983 */
	&rallyx_driver,		/* (c) 1980 Midway */
	&superpac_driver,	/* (c) 1982 Midway */
	&superpcn_driver,	/* (c) 1982 */
	&sxevious_driver,	/* (c) 1984 */
	&tankbatt_driver,	/* (c) 1980 Namco */
	&todruaga_driver,	/* (c) 1984 */
	&warpwar2_driver,	/* (c) 1981 Rock-ola - different hardware */
	&warpwarp_driver,	/* (c) 1981 Rock-ola - different hardware */
	&xevios_driver,		/* bootleg */
	&xevious_driver,	/* (c) 1982 */
	&xeviousa_driver,	/* (c) 1982 + Atari license */

	0	/* end of array */
};
