#include "driver.h"

/* Sega G-80 raster games */
extern struct GameDriver astrob_driver;
extern struct GameDriver astrob1_driver;
extern struct GameDriver s005_driver;
extern struct GameDriver monsterb_driver;
extern struct GameDriver spaceod_driver;

/* Sega "Zaxxon hardware" games */
extern struct GameDriver zaxxon_driver;
extern struct GameDriver szaxxon_driver;
extern struct GameDriver futspy_driver;
extern struct GameDriver congo_driver;
extern struct GameDriver tiptop_driver;

/* Sega System 8 games */
extern struct GameDriver starjack_driver;
extern struct GameDriver starjacs_driver;
extern struct GameDriver regulus_driver;
extern struct GameDriver regulusu_driver;
extern struct GameDriver upndown_driver;
extern struct GameDriver mrviking_driver;
extern struct GameDriver swat_driver;
extern struct GameDriver flicky_driver;
extern struct GameDriver flicky2_driver;
extern struct GameDriver bullfgtj_driver;
extern struct GameDriver pitfall2_driver;
extern struct GameDriver pitfallu_driver;
extern struct GameDriver seganinj_driver;
extern struct GameDriver seganinu_driver;
extern struct GameDriver nprinces_driver;
extern struct GameDriver nprinceb_driver;
extern struct GameDriver imsorry_driver;
extern struct GameDriver imsorryj_driver;
extern struct GameDriver teddybb_driver;
extern struct GameDriver hvymetal_driver;
extern struct GameDriver myhero_driver;
extern struct GameDriver myheroj_driver;
extern struct GameDriver chplft_driver;
extern struct GameDriver chplftb_driver;
extern struct GameDriver chplftbl_driver;
extern struct GameDriver fdwarrio_driver;
extern struct GameDriver brain_driver;
extern struct GameDriver wboy_driver;
extern struct GameDriver wboy2_driver;
extern struct GameDriver wboy4_driver;
extern struct GameDriver wboyu_driver;
extern struct GameDriver wboy4u_driver;
extern struct GameDriver wbdeluxe_driver;
extern struct GameDriver gardia_driver;
extern struct GameDriver tokisens_driver;
extern struct GameDriver wbml_driver;

extern struct GameDriver bankp_driver;
extern struct GameDriver champbas_driver;
extern struct GameDriver stactics_driver;

const struct GameDriver *drivers[] =
{
	&astrob1_driver,	/* (c) 1981 */
	&astrob_driver,		/* (c) 1981 */
	&bankp_driver,		/* (c) 1984 Sega */
	&brain_driver,		/* (c) 1986 Coreland / Sega */
	&bullfgtj_driver,	/* 834-5478 (c) 1984 Sega / Coreland */
	&champbas_driver,	/* (c) 1983 Sega */
	&chplft_driver,		/* 834-5795 (c) 1985, (c) 1982 Dan Gorlin */
	&chplftb_driver,	/* 834-5795 (c) 1985, (c) 1982 Dan Gorlin */
	&chplftbl_driver,	/* bootleg */
	&congo_driver,		/* 605-5167 (c) 1983 */
	&fdwarrio_driver,	/* 834-5918 (c) 1985 Coreland / Sega */
	&flicky2_driver,	/* (c) 1984 */
	&flicky_driver,		/* (c) 1984 */
	&futspy_driver,		/* (c) 1984 */
	&gardia_driver,		/* 834-6119 */
	&hvymetal_driver,	/* 834-5745 (c) 1985 */
	&imsorry_driver,	/* 834-5707 (c) 1985 Coreland / Sega */
	&imsorryj_driver,	/* 834-5707 (c) 1985 Coreland / Sega */
	&monsterb_driver,	/* (c) 1982 */
	&mrviking_driver,	/* 834-5383 (c) 1984 */
	&myhero_driver,		/* 834-5755 (c) 1985 */
	&myheroj_driver,	/* 834-5755 (c) 1985 Coreland / Sega */
	&nprinceb_driver,	/* 834-5677 (c) 1985 */
	&nprinces_driver,	/* 834-5677 (c) 1985 */
	&pitfall2_driver,	/* 834-5627 [1985?] reprogrammed, (c) 1984 Activision */
	&pitfallu_driver,	/* 834-5627 [1985?] reprogrammed, (c) 1984 Activision */
	&regulus_driver,	/* 834-5328 (c) 1983 */
	&regulusu_driver,	/* 834-5328 (c) 1983 */
	&s005_driver,		/* (c) 1981 */
	&seganinj_driver,	/* 834-5677 (c) 1985 */
	&seganinu_driver,	/* 834-5677 (c) 1985 */
	&spaceod_driver,	/* (c) 1981 */
	&stactics_driver,	/* [1981 Sega] */
	&starjack_driver,	/* 834-5191 (c) 1983 */
	&starjacs_driver,	/* (c) 1983 Stern */
	&swat_driver,		/* 834-5388 (c) 1984 Coreland / Sega */
	&szaxxon_driver,	/* (c) 1982 */
	&teddybb_driver,	/* 834-5712 (c) 1985 */
	&tiptop_driver,		/* 605-5167 (c) 1983 */
	&tokisens_driver,	/* (c) 1987 (from a bootleg board) */
	&upndown_driver,	/* (c) 1983 */
	&wbdeluxe_driver,	/* (c) 1986 + Escape license */
	&wbml_driver,		/* bootleg */
	&wboy2_driver,		/* 834-5984 (c) 1986 + Escape license */
	&wboy4_driver,		/* 834-5984 (c) 1986 + Escape license */
	&wboy4u_driver,		/* 834-5984 (c) 1986 + Escape license */
	&wboy_driver,		/* 834-5984 (c) 1986 + Escape license */
	&wboyu_driver,		/* 834-5753 (? maybe a conversion) (c) 1986 + Escape license */
	&zaxxon_driver,		/* (c) 1982 */

	0	/* end of array */
};
