#include "driver.h"

/* Capcom games */
extern struct GameDriver vulgus_driver;
extern struct GameDriver vulgus2_driver;
extern struct GameDriver sonson_driver;
extern struct GameDriver higemaru_driver;
extern struct GameDriver c1942_driver;
extern struct GameDriver c1942a_driver;
extern struct GameDriver c1942b_driver;
extern struct GameDriver exedexes_driver;
extern struct GameDriver savgbees_driver;
extern struct GameDriver commando_driver;
extern struct GameDriver commandu_driver;
extern struct GameDriver commandj_driver;
extern struct GameDriver gng_driver;
extern struct GameDriver gngt_driver;
extern struct GameDriver gngcross_driver;
extern struct GameDriver gngjap_driver;
extern struct GameDriver diamond_driver;
extern struct GameDriver gunsmoke_driver;
extern struct GameDriver gunsmrom_driver;
extern struct GameDriver gunsmokj_driver;
extern struct GameDriver gunsmoka_driver;
extern struct GameDriver sectionz_driver;
extern struct GameDriver trojan_driver;
extern struct GameDriver trojanj_driver;
extern struct GameDriver srumbler_driver;
extern struct GameDriver srumblr2_driver;
extern struct GameDriver lwings_driver;
extern struct GameDriver lwings2_driver;
extern struct GameDriver lwingsjp_driver;
extern struct GameDriver sidearms_driver;
extern struct GameDriver sidearmr_driver;
extern struct GameDriver sidearjp_driver;
extern struct GameDriver bionicc_driver;
extern struct GameDriver bionicc2_driver;
extern struct GameDriver c1943_driver;
extern struct GameDriver c1943jap_driver;
extern struct GameDriver c1943kai_driver;

const struct GameDriver *drivers[] =
{
	&c1942_driver,		/* (c) 1984 */
	&c1942a_driver,		/* (c) 1984 */
	&c1942b_driver,		/* (c) 1984 */
	&c1943_driver,		/* (c) 1987 */
	&c1943jap_driver,	/* (c) 1987 */
	&c1943kai_driver,	/* (c) 1987 */
	&bionicc2_driver,	/* (c) 1987 */
	&bionicc_driver,	/* (c) 1987 */	/* aka Top Secret */
	&commandj_driver,	/* (c) 1985 */
	&commando_driver,	/* (c) 1985 */
	&commandu_driver,	/* (c) 1985 */
	&diamond_driver,	/* (c) 1989 KH Video (NOT A CAPCOM GAME but runs on GnG hardware) */
	&exedexes_driver,	/* (c) 1985 */
	&gng_driver,		/* (c) 1985 */
	&gngcross_driver,	/* (c) 1985 */
	&gngjap_driver,		/* (c) 1985 */
	&gngt_driver,		/* (c) 1985 */
	&gunsmoka_driver,	/* (c) 1985 */
	&gunsmoke_driver,	/* (c) 1985 */
	&gunsmokj_driver,	/* (c) 1985 */
	&gunsmrom_driver,	/* (c) 1985 + Romstar */
	&higemaru_driver,	/* (c) 1984 */
	&lwings2_driver,	/* (c) 1986 */
	&lwings_driver,		/* (c) 1986 */
	&lwingsjp_driver,	/* (c) 1986 */
	&savgbees_driver,	/* (c) 1985 + Memetron license */
	&sectionz_driver,	/* (c) 1985 */
	&sidearjp_driver,	/* (c) 1986 */
	&sidearmr_driver,	/* (c) 1986 + Romstar license */
	&sidearms_driver,	/* (c) 1986 */
	&sonson_driver,		/* (c) 1984 */
	&srumbler_driver,	/* (c) 1986 */	/* aka Rush'n Crash */
	&srumblr2_driver,	/* (c) 1986 */
	&trojan_driver,		/* (c) 1986 + Romstar */
	&trojanj_driver,	/* (c) 1986 */
	&vulgus2_driver,	/* (c) 1984 */
	&vulgus_driver,		/* (c) 1984 */

	0	/* end of array */
};
