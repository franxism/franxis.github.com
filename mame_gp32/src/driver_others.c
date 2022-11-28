#include "driver.h"

/* UPL games */
extern struct GameDriver nova2001_driver;
extern struct GameDriver pkunwar_driver;
extern struct GameDriver pkunwarj_driver;
extern struct GameDriver ninjakd2_driver;
extern struct GameDriver ninjak2a_driver;

extern struct GameDriver spacefb_driver;
extern struct GameDriver blueprnt_driver;
extern struct GameDriver espial_driver;
extern struct GameDriver espiale_driver;
extern struct GameDriver exerion_driver;
extern struct GameDriver exeriont_driver;
extern struct GameDriver exerionb_driver;
extern struct GameDriver vastar_driver;
extern struct GameDriver vastar2_driver;
extern struct GameDriver citycon_driver;
extern struct GameDriver psychic5_driver;
extern struct GameDriver dday_driver;
extern struct GameDriver snowbros_driver;
extern struct GameDriver snowbro2_driver;
extern struct GameDriver gundealr_driver;
extern struct GameDriver gundeala_driver;
extern struct GameDriver leprechn_driver;
extern struct GameDriver potogold_driver;
extern struct GameDriver hexa_driver;
extern struct GameDriver redalert_driver;
extern struct GameDriver spiders_driver;
extern struct GameDriver goldstar_driver;
extern struct GameDriver goldstbl_driver;
extern struct GameDriver sharkatt_driver;
extern struct GameDriver exctsccr_driver;
extern struct GameDriver exctsccb_driver;
extern struct GameDriver speedbal_driver;
extern struct GameDriver sauro_driver;

/* Nichibutsu games */
extern struct GameDriver cop01_driver;
extern struct GameDriver cop01a_driver;
extern struct GameDriver terracre_driver;
extern struct GameDriver terracra_driver;

const struct GameDriver *drivers[] =
{
	&blueprnt_driver,	/* (c) 1982 Bally Midway */
	&citycon_driver,	/* (c) 1985 Jaleco */
	&cop01_driver,		/* (c) 1985 */
	&cop01a_driver,		/* (c) 1985 */
	&dday_driver,		/* (c) 1982 Centuri */
	&espial_driver,		/* (c) 1983 Thunderbolt */
	&espiale_driver,	/* (c) 1983 Thunderbolt */
	&exctsccb_driver,	/* bootleg */
	&exctsccr_driver,	/* (c) 1983 Alpha Denshi Co. */
	&exerion_driver,	/* (c) 1983 Jaleco */
	&exerionb_driver,	/* bootleg */
	&exeriont_driver,	/* (c) 1983 Jaleco + Taito America license */
	&goldstar_driver,
	&goldstbl_driver,
	&gundeala_driver,	/* (c) Dooyong */
	&gundealr_driver,	/* (c) 1990 Dooyong */
	&hexa_driver,		/* D. R. Korea */
	&leprechn_driver,	/* (c) 1982 Tong Electronic */
	&ninjak2a_driver,	/* (c) 1987 */
	&ninjakd2_driver,	/* (c) 1987 */
	&nova2001_driver,	/* (c) [1984?] + Universal license */
	&pkunwar_driver,	/* [1985?] */
	&pkunwarj_driver,	/* [1985?] */
	&potogold_driver,	/* (c) 1982 Tong Electronic */
	&psychic5_driver,	/* (c) 1987 Jaleco */
	&redalert_driver,	/* (c) 1981 Irem (GDI game) */
	&sauro_driver,		/* (c) 1987 Tecfri */
	&sharkatt_driver,	/* (c) Pacific Novelty */
	&snowbro2_driver,	/* (c) 1990 Toaplan + Romstar license */
	&snowbros_driver,	/* (c) 1990 Toaplan + Romstar license */
	&spacefb_driver,	/* 834-0031 (c) [1980?] Nintendo */
	&speedbal_driver,	/* (c) 1987 Tecfri */
	&spiders_driver,	/* (c) 1981 Sigma */
	&terracra_driver,	/* (c) 1985 */
	&terracre_driver,	/* (c) 1985 */
	&vastar2_driver,	/* (c) 1983 Sesame Japan */
	&vastar_driver,		/* (c) 1983 Sesame Japan */

	0	/* end of array */
};
