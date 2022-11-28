#include "driver.h"

/* Irem games */
extern struct GameDriver mpatrol_driver;
extern struct GameDriver mpatrolw_driver;
extern struct GameDriver mranger_driver;
extern struct GameDriver yard_driver;
extern struct GameDriver vsyard_driver;
extern struct GameDriver kungfum_driver;
extern struct GameDriver kungfud_driver;
extern struct GameDriver kungfub_driver;
extern struct GameDriver kungfub2_driver;
extern struct GameDriver travrusa_driver;
extern struct GameDriver motorace_driver;
extern struct GameDriver vigilant_driver;
extern struct GameDriver vigilntj_driver;

const struct GameDriver *drivers[] =
{
	&kungfub2_driver,	/* bootleg */
	&kungfub_driver,	/* bootleg */
	&kungfud_driver,	/* (c) 1984 + Data East license */
	&kungfum_driver,	/* (c) 1984 */
	&motorace_driver,	/* (c) 1983 Williams license */
	&mpatrol_driver,	/* (c) 1982 */
	&mpatrolw_driver,	/* (c) 1982 + Williams license */
	&mranger_driver,	/* bootleg */
	&travrusa_driver,	/* (c) 1983 */
	&vigilant_driver,	/* (c) 1988 */
	&vigilntj_driver,	/* (c) 1988 */
	&vsyard_driver,		/* (c) 1983/1984 */
	&yard_driver,		/* (c) 1983 */

	0	/* end of array */
};
