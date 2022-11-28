#include "driver.h"

/* Technos games */
extern struct GameDriver mystston_driver;
extern struct GameDriver matmania_driver;
extern struct GameDriver excthour_driver;
extern struct GameDriver renegade_driver;
extern struct GameDriver kuniokub_driver;
extern struct GameDriver xsleena_driver;
extern struct GameDriver solarwar_driver;
extern struct GameDriver ddragon_driver;
extern struct GameDriver ddragonb_driver;
extern struct GameDriver ddragon2_driver;
extern struct GameDriver blockout_driver;

const struct GameDriver *drivers[] =
{
	&blockout_driver,	/* TA-0029 (c) 1989 + California Dreams */
	&ddragon2_driver,	/* TA-0026 (c) 1988 */
	&ddragon_driver,
	&ddragonb_driver,	/* TA-0021 bootleg */
	&excthour_driver,	/* TA-0015 (c) 1985 + Taito license */
	&kuniokub_driver,	/* TA-0018 bootleg */
	&matmania_driver,	/* TA-0015 (c) 1985 + Taito America license */
	&mystston_driver,	/* (c) 1984 */
	&renegade_driver,
	&solarwar_driver,	/* TA-0019 (c) 1986 + Memetron license */
	&xsleena_driver,	/* TA-0019 (c) 1986 */

	0	/* end of array */
};
