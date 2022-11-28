#include "driver.h"

/* Data East "Burger Time hardware" games */
extern struct GameDriver lnc_driver;
extern struct GameDriver zoar_driver;
extern struct GameDriver btime_driver;
extern struct GameDriver btimea_driver;
extern struct GameDriver cookrace_driver;
extern struct GameDriver bnj_driver;
extern struct GameDriver brubber_driver;
extern struct GameDriver caractn_driver;
extern struct GameDriver eggs_driver;
extern struct GameDriver scregg_driver;
extern struct GameDriver tagteam_driver;

/* other Data East games */
extern struct GameDriver astrof_driver;
extern struct GameDriver astrof2_driver;
extern struct GameDriver astrof3_driver;
extern struct GameDriver tomahawk_driver;
extern struct GameDriver tomahaw5_driver;
extern struct GameDriver kchamp_driver;
extern struct GameDriver kchampvs_driver;
extern struct GameDriver karatedo_driver;
extern struct GameDriver firetrap_driver;
extern struct GameDriver firetpbl_driver;
extern struct GameDriver brkthru_driver;
extern struct GameDriver darwin_driver;
extern struct GameDriver shootout_driver;
extern struct GameDriver sidepckt_driver;
extern struct GameDriver exprraid_driver;
extern struct GameDriver wexpress_driver;
extern struct GameDriver wexpresb_driver;

/* Data East 8-bit games */
extern struct GameDriver ghostb_driver;
extern struct GameDriver mazeh_driver;
extern struct GameDriver cobracom_driver;
extern struct GameDriver oscar_driver;
extern struct GameDriver oscarj_driver;

/* Data East 16-bit games */
extern struct GameDriver karnov_driver;
extern struct GameDriver karnovj_driver;
extern struct GameDriver chelnov_driver;
extern struct GameDriver chelnovj_driver;

extern struct GameDriver kingofb_driver;
extern struct GameDriver kingofbj_driver;
extern struct GameDriver ringking_driver;
extern struct GameDriver ringkin2_driver;

const struct GameDriver *drivers[] =
{
	&astrof2_driver,	/* (c) [1980?] */
	&astrof3_driver,	/* (c) [1980?] */
	&astrof_driver,		/* (c) [1980?] */
	&bnj_driver,		/* (c) 1982 + Midway */
	&brkthru_driver,	/* (c) 1986 */
	&brubber_driver,	/* (c) 1982 */
	&btime_driver,		/* (c) 1982 + Midway */
	&btimea_driver,		/* (c) 1982 */
	&caractn_driver,	/* bootleg */
	&chelnov_driver,	/* (c) 1988 Data East USA */
	&chelnovj_driver,	/* (c) 1988 Data East Corporation */
	&cobracom_driver,	/* (c) 1988 Data East Corporation */
	&cookrace_driver,	/* bootleg */
	&darwin_driver,
	&eggs_driver,		/* (c) 1983 Universal USA */
	&exprraid_driver,	/* (c) 1986 Data East USA */
	&firetpbl_driver,	/* bootleg */
	&firetrap_driver,	/* (c) 1986 */
	&ghostb_driver,		/* (c) 1987 Data East USA */
	&karatedo_driver,	/* (c) 1984 Data East Corporation */
	&karnov_driver,		/* (c) 1987 Data East USA */
	&karnovj_driver,	/* (c) 1987 Data East Corporation */
	&kchamp_driver,		/* (c) 1984 Data East USA */
	&kchampvs_driver,	/* (c) 1984 Data East USA */
	&kingofb_driver,	/* (c) 1985 Woodplace Inc. */
	&kingofbj_driver,	/* (c) 1985 Woodplace Inc. */
	&lnc_driver,		/* (c) 1981 */
	&mazeh_driver,		/* (c) 1987 Data East Corporation */
	&oscar_driver,		/* (c) 1988 Data East USA */
	&oscarj_driver,		/* (c) 1988 Data East Corporation */
	&ringkin2_driver,	/* (c) 1985 Data East USA */
	&ringking_driver,	/* (c) 1985 Data East USA */
	&scregg_driver,		/* TA-0001 (c) 1983 Technos Japan */
	&shootout_driver,	/* (c) 1985 */
	&sidepckt_driver,	/* (c) 1986 */
	&tagteam_driver,	/* TA-0007 (c) 1983 + Technos Japan license */
	&tomahaw5_driver,	/* (c) [1980?] */
	&tomahawk_driver,	/* (c) [1980?] */
	&wexpresb_driver,	/* bootleg */
	&wexpress_driver,	/* (c) 1986 Data East Corporation */
	&zoar_driver,		/* (c) 1982 */

	0	/* end of array */
};
