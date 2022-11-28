#include "common.h"


struct GameInfo
{
	const char *name;
	const struct RomModule *rom;
	const struct DSW *dswsettings;
	int defaultdsw[2];	/* default dipswitch settings */
};



struct GameVidInfo
{
	const char *name;
	const struct RomModule *gfxrom;
	const struct GfxLayout *charlayout;
	const struct GfxLayout *spritelayout;
	const unsigned char *palette;
	const unsigned char *colortable;
};


extern const struct GameInfo gameinfo[];
extern const struct GameVidInfo gamevidinfo[];


#define TOTAL_COLORS 16
#define COLOR_CODES 32

/* color codes used to print text on the screen */
#define WHITE_TEXT 0x0f
#define YELLOW_TEXT 0x09
#define RED_TEXT 0x01
/* start of numbers and letters in the character roms */
#define NUMBERS_START '0'
#define LETTERS_START 'A'
