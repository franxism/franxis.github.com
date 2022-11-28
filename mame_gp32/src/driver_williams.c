#include "driver.h"

/* Williams games */
extern struct GameDriver robotron_driver;
extern struct GameDriver robotryo_driver;
extern struct GameDriver stargate_driver;
extern struct GameDriver joust_driver;
extern struct GameDriver joustr_driver;
extern struct GameDriver joustwr_driver;
extern struct GameDriver sinistar_driver;
extern struct GameDriver sinista1_driver;
extern struct GameDriver sinista2_driver;
extern struct GameDriver bubbles_driver;
extern struct GameDriver bubblesr_driver;
extern struct GameDriver defender_driver;
extern struct GameDriver splat_driver;
extern struct GameDriver blaster_driver;
extern struct GameDriver colony7_driver;
extern struct GameDriver colony7a_driver;
extern struct GameDriver lottofun_driver;
extern struct GameDriver defcmnd_driver;
extern struct GameDriver defence_driver;

const struct GameDriver *drivers[] =
{
	/* Williams games */
	&blaster_driver,	/* (c) 1983 */
	&bubbles_driver,	/* (c) 1982 */
	&bubblesr_driver,	/* (c) 1982 */
	&colony7_driver,	/* (c) 1981 Taito */
	&colony7a_driver,	/* (c) 1981 Taito */
	&defcmnd_driver,	/* bootleg */
	&defence_driver,	/* bootleg */
	&defender_driver,	/* (c) 1980 */
	&joust_driver,		/* (c) 1982 */
	&joustr_driver,		/* (c) 1982 */
	&joustwr_driver,	/* (c) 1982 */
	&lottofun_driver,	/* (c) 1987 H.A.R. Management */
	&robotron_driver,	/* (c) 1982 */
	&robotryo_driver,	/* (c) 1982 */
	&sinista1_driver,	/* (c) 1982 */
	&sinista2_driver,	/* (c) 1982 */
	&sinistar_driver,	/* (c) 1982 */
	&splat_driver,		/* (c) 1982 */
	&stargate_driver,	/* (c) 1981 */

	0	/* end of array */
};
