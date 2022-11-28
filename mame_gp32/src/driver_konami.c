#include "driver.h"

/* Konami games */
extern struct GameDriver pooyan_driver;
extern struct GameDriver pooyans_driver;
extern struct GameDriver pootan_driver;
extern struct GameDriver timeplt_driver;
extern struct GameDriver timepltc_driver;
extern struct GameDriver spaceplt_driver;
extern struct GameDriver rocnrope_driver;
extern struct GameDriver gyruss_driver;
extern struct GameDriver gyrussce_driver;
extern struct GameDriver venus_driver;
extern struct GameDriver trackfld_driver;
extern struct GameDriver trackflc_driver;
extern struct GameDriver hyprolym_driver;
extern struct GameDriver hyprolyb_driver;
extern struct GameDriver circusc_driver;
extern struct GameDriver circusc2_driver;
extern struct GameDriver tp84_driver;
extern struct GameDriver tp84a_driver;
extern struct GameDriver hyperspt_driver;
extern struct GameDriver sbasketb_driver;
extern struct GameDriver mikie_driver;
extern struct GameDriver mikiej_driver;
extern struct GameDriver mikiehs_driver;
extern struct GameDriver roadf_driver;
extern struct GameDriver roadf2_driver;
extern struct GameDriver yiear_driver;
extern struct GameDriver kicker_driver;
extern struct GameDriver shaolins_driver;
extern struct GameDriver pingpong_driver;
extern struct GameDriver gberet_driver;
extern struct GameDriver rushatck_driver;
extern struct GameDriver jailbrek_driver;
extern struct GameDriver ironhors_driver;
extern struct GameDriver jackal_driver;
extern struct GameDriver topgunr_driver;
extern struct GameDriver topgunbl_driver;
extern struct GameDriver contra_driver;
extern struct GameDriver contrab_driver;
extern struct GameDriver gryzorb_driver;

/* Konami "Nemesis hardware" games */
extern struct GameDriver nemesis_driver;
extern struct GameDriver nemesuk_driver;
extern struct GameDriver konamigt_driver;
extern struct GameDriver salamand_driver;
/* GX400 BIOS based games */
extern struct GameDriver rf2_driver;
extern struct GameDriver twinbee_driver;
extern struct GameDriver gradius_driver;
extern struct GameDriver gwarrior_driver;

extern struct GameDriver tutankhm_driver;
extern struct GameDriver tutankst_driver;
extern struct GameDriver junofrst_driver;

const struct GameDriver *drivers[] =
{
	&circusc2_driver,	/* GX380 (c) 1984 */
	&circusc_driver,	/* GX380 (c) 1984 */
	&contra_driver,		/* GX633 (c) 1987 */
	&contrab_driver,	/* bootleg */
	&gberet_driver,		/* GX577 (c) 1985 */
	&gradius_driver,	/* GX456 (c) 1985 */
	&gryzorb_driver,	/* bootleg */
	&gwarrior_driver,	/* GX578 (c) 1985 */
	&gyruss_driver,		/* GX347 (c) 1983 */
	&gyrussce_driver,	/* GX347 (c) 1983 + Centuri license */
	&hyperspt_driver,	/* GX330 (c) 1984 + Centuri */
	&hyprolyb_driver,	/* bootleg */
	&hyprolym_driver,	/* GX361 (c) 1983 */
	&ironhors_driver,	/* GX560 (c) 1986 */
	&jackal_driver,		/* GX631 (c) 1986 */
	&jailbrek_driver,	/* GX507 (c) 1986 */
	&junofrst_driver,	/* GX310 (c) 1983 Konami */
	&kicker_driver,		/* GX477 (c) 1985 */
	&konamigt_driver,	/* GX561 (c) 1985 */
	&mikie_driver,		/* GX469 (c) 1984 */
	&mikiehs_driver,	/* GX469 (c) 1984 */
	&mikiej_driver,		/* GX469 (c) 1984 */
	&nemesis_driver,	/* GX456 (c) 1985 */
	&nemesuk_driver,	/* GX456 (c) 1985 */
	&pingpong_driver,	/* GX555 (c) 1985 */
	&pootan_driver,		/* bootleg */
	&pooyan_driver,		/* GX320 (c) 1982 */
	&pooyans_driver,	/* GX320 (c) 1982 Stern */
	&rf2_driver,		/* GX561 (c) 1985 */
	&roadf2_driver,		/* GX461 (c) 1984 */
	&roadf_driver,		/* GX461 (c) 1984 */
	&rocnrope_driver,	/* GX364 (c) 1983 + Kosuka */
	&rushatck_driver,	/* GX577 (c) 1985 */
	&sbasketb_driver,	/* GX405 (c) 1984 */
	&shaolins_driver,	/* GX477 (c) 1985 */
	&spaceplt_driver,	/* bootleg */
	&timeplt_driver,	/* GX393 (c) 1982 */
	&timepltc_driver,	/* GX393 (c) 1982 + Centuri license*/
	&topgunbl_driver,	/* bootleg */
	&topgunr_driver,	/* GX631 (c) 1986 */
	&tp84_driver,		/* GX388 (c) 1984 */
	&tp84a_driver,		/* GX388 (c) 1984 */
	&trackflc_driver,	/* GX361 (c) 1983 + Centuri license */
	&trackfld_driver,	/* GX361 (c) 1983 */
	&tutankhm_driver,	/* GX350 (c) 1982 Konami */
	&twinbee_driver,	/* GX412 (c) 1985 */
	&venus_driver,		/* bootleg */
	&yiear_driver,		/* GX407 (c) 1985 */

	0	/* end of array */
};
