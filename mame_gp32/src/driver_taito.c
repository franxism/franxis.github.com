#include "driver.h"

/* older Taito games */
extern struct GameDriver crbaloon_driver;
extern struct GameDriver crbalon2_driver;

/* Taito "Qix hardware" games */
extern struct GameDriver qix_driver;
extern struct GameDriver qix2_driver;
extern struct GameDriver sdungeon_driver;
extern struct GameDriver zookeep_driver;
extern struct GameDriver zookeepa_driver;
extern struct GameDriver kram_driver;
extern struct GameDriver kram2_driver;

/* Taito SJ System games */
extern struct GameDriver spaceskr_driver;
extern struct GameDriver junglek_driver;
extern struct GameDriver jhunt_driver;
extern struct GameDriver alpine_driver;
extern struct GameDriver alpinea_driver;
extern struct GameDriver timetunl_driver;
extern struct GameDriver frontlin_driver;
extern struct GameDriver elevator_driver;
extern struct GameDriver elevatob_driver;
extern struct GameDriver tinstar_driver;
extern struct GameDriver waterski_driver;
extern struct GameDriver bioatack_driver;

/* other Taito games */
extern struct GameDriver gsword_driver;
extern struct GameDriver gladiatr_driver;
extern struct GameDriver ogonsiro_driver;
extern struct GameDriver gcastle_driver;
extern struct GameDriver tokio_driver;
extern struct GameDriver tokiob_driver;
extern struct GameDriver bublbobl_driver;
extern struct GameDriver boblbobl_driver;
extern struct GameDriver sboblbob_driver;
extern struct GameDriver arkanoid_driver;
extern struct GameDriver arknoidu_driver;
extern struct GameDriver arkatayt_driver;
extern struct GameDriver superqix_driver;
extern struct GameDriver sqixbl_driver;
extern struct GameDriver twincobr_driver;
extern struct GameDriver twincobu_driver;
extern struct GameDriver ktiger_driver;
extern struct GameDriver arkanoi2_driver;
extern struct GameDriver ark2us_driver;
extern struct GameDriver tnzs_driver;
extern struct GameDriver tnzs2_driver;
extern struct GameDriver tigerh_driver;
extern struct GameDriver tigerhb1_driver;
extern struct GameDriver tigerhb2_driver;
extern struct GameDriver slapfigh_driver;
extern struct GameDriver slapbtjp_driver;
extern struct GameDriver slapbtuk_driver;
extern struct GameDriver getstar_driver;

const struct GameDriver *drivers[] =
{
	&alpine_driver,		/* (c) 1982 */
	&alpinea_driver,	/* (c) 1982 */
	&ark2us_driver,		/* (c) 1987 + Romstar license */
	&arkanoi2_driver,	/* (c) 1987 */
	&arkanoid_driver,	/* (c) 1986 Taito */
	&arkatayt_driver,	/* bootleg */
	&arknoidu_driver,	/* (c) 1986 Taito America + Romstar license */
	&bioatack_driver,	/* (c) 1983 + Fox Video Games license */
	&boblbobl_driver,	/* bootleg */
	&bublbobl_driver,	/* (c) 1986 */
	&crbalon2_driver,	/* (c) 1980 Taito */
	&crbaloon_driver,	/* (c) 1980 Taito */
	&elevatob_driver,	/* bootleg */
	&elevator_driver,	/* (c) 1983 */
	&frontlin_driver,	/* (c) 1982 */
	&gcastle_driver,	/* (c) 1986 */
	&getstar_driver,	/* (c) 1986 Taito, but bootleg */
	&gladiatr_driver,	/* (c) 1986 Taito America */
	&gsword_driver,		/* (c) 1984 */
	&jhunt_driver,		/* (c) 1982 Taito America */
	&junglek_driver,	/* (c) 1982 */
	&kram2_driver,		/* (c) 1982 Taito America */
	&kram_driver,		/* (c) 1982 Taito America */
	&ktiger_driver,		/* (c) 1987 */
	&ogonsiro_driver,	/* (c) 1986 */
	&qix2_driver,		/* (c) 1981 */
	&qix_driver,		/* (c) 1981 */
	&sboblbob_driver,	/* bootleg */
	&sdungeon_driver,	/* (c) 1981 Taito America */
	&slapbtjp_driver,	/* bootleg */
	&slapbtuk_driver,	/* bootleg */
	&slapfigh_driver,	/* (c) 1988 */
	&spaceskr_driver,	/* (c) 1981 */
	&sqixbl_driver,		/* bootleg? but (c) 1987 */
	&superqix_driver,	/* (c) 1987 */
	&tigerh_driver,		/* (c) 1985 */
	&tigerhb1_driver,	/* bootleg */
	&tigerhb2_driver,	/* bootleg */
	&timetunl_driver,	/* (c) 1982 */
	&tinstar_driver,	/* (c) 1983 */
	&tnzs2_driver,		/* (c) 1988 */
	&tnzs_driver,		/* (c) 1988 */
	&tokio_driver,		/* (c) 1986 */
	&tokiob_driver,		/* bootleg */
	&twincobr_driver,	/* (c) 1987 */
	&twincobu_driver,	/* (c) 1987 Taito America + Romstar license */
	&waterski_driver,	/* (c) 1983 */
	&zookeep_driver,	/* (c) 1982 Taito America */
	&zookeepa_driver,	/* (c) 1982 Taito America */

	0	/* end of array */
};
