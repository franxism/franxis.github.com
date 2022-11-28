#include "roms.h"


const struct RomModule genericrom[] =
{
	{ "%s.6e", 0x0000, 0x1000 },
	{ "%s.6f", 0x1000, 0x1000 },
	{ "%s.6h", 0x2000, 0x1000 },
	{ "%s.6j", 0x3000, 0x1000 },
	{ 0 }	/* end of table */
};
const struct RomModule pacmodrom[] =
{
	{ "6e.mod",    0x0000, 0x1000 },
	{ "pacman.6f", 0x1000, 0x1000 },
	{ "6h.mod",    0x2000, 0x1000 },
	{ "pacman.6j", 0x3000, 0x1000 },
	{ 0 }	/* end of table */
};
const struct RomModule mspacrom[] =
{
	{ "boot1", 0x0000, 0x1000 },
	{ "boot2", 0x1000, 0x1000 },
	{ "boot3", 0x2000, 0x1000 },
	{ "boot4", 0x3000, 0x1000 },
	{ "boot5", 0x8000, 0x1000 },
	{ "boot6", 0x9000, 0x1000 },
	{ 0 }	/* end of table */
};
const struct RomModule crushrom[] =
{
	{ "CR1", 0x0000, 0x0800 },
	{ "CR5", 0x0800, 0x0800 },
	{ "CR2", 0x1000, 0x0800 },
	{ "CR6", 0x1800, 0x0800 },
	{ "CR3", 0x2000, 0x0800 },
	{ "CR7", 0x2800, 0x0800 },
	{ "CR4", 0x3000, 0x0800 },
	{ "CR8", 0x3800, 0x0800 },
	{ 0 }	/* end of table */
};

const struct RomModule piranharom[] =
{
	{ "pr1.cpu",    0x0000, 0x1000 },
	{ "pacman.6f", 0x1000, 0x1000 },
	{ "pr3.cpu",    0x2000, 0x1000 },
	{ "pr4.cpu", 0x3000, 0x1000 },
	{ 0 }	/* end of table */
};


const struct DSW pacdsw[] =
{
	{ 0, 0x0c, "LIVES", { "1", "2", "3", "5" } },
	{ 0, 0x30, "BONUS", { "10000", "15000", "20000", "NONE" } },
	{ 0, 0x40, "DIFFICULTY", { "HARD", "NORMAL" }, 1 },
	{ 0, 0x80, "GHOST NAMES", { "ALTERNATE", "NORMAL" }, 1 },
	{ -1 }
};
const struct DSW mspacdsw[] =
{
	{ 0, 0x0c, "LIVES", { "1", "2", "3", "5" } },
	{ 0, 0x30, "BONUS", { "10000", "15000", "20000", "NONE" } },
	{ 0, 0x40, "DIFFICULTY", { "HARD", "NORMAL" }, 1 },
	{ -1 }
};
const struct DSW crushdsw[] =
{
	{ 0, 0x0c, "LIVES", { "\x03", "\x04", "\x05", "\x06" } },
	{ 0, 0x20, "TELEPORT HOLES", { "ON", "OFF" }, 1 },
	{ 0, 0x10, "FIRST PATTERN", { "HARD", "EASY" }, 1 },
	{ -1 }
};


const struct GameInfo gameinfo[] =
{
	{ "pacmod", pacmodrom, pacdsw, { 0xe9 } },
	{ "mspacman", mspacrom, mspacdsw, { 0xe9 } },
	{ "crush", crushrom, crushdsw, { 0xf1 } },
	/* Added Piranha */
	{ "piranha", piranharom, pacdsw, { 0xe9 } },
		/* More Ms Pac Man Hacks */
	{ "cookiem", mspacrom, mspacdsw, { 0xe9 } },
	{ "heartbrn", mspacrom, mspacdsw, { 0xe9 } },
	{ "mrpacman", mspacrom, mspacdsw, { 0xe9 } },
	{ "msbaby", mspacrom, mspacdsw, { 0xe9 } },
	{ "msberzk", mspacrom, mspacdsw, { 0xe9 } },
	{ "msdroid", mspacrom, mspacdsw, { 0xe9 } },
	{ "mselton", mspacrom, mspacdsw, { 0xe9 } },
	{ "msf1pac", mspacrom, mspacdsw, { 0xe9 } },
	{ "mshangly", mspacrom, mspacdsw, { 0xe9 } },
	{ "mshearts", mspacrom, mspacdsw, { 0xe9 } },
	{ "msmini", mspacrom, mspacdsw, { 0xe9 } },
	{ "mspacad", mspacrom, mspacdsw, { 0xe9 } },
	{ "mspaccm", mspacrom, mspacdsw, { 0xe9 } },
	{ "mspacdx", mspacrom, mspacdsw, { 0xe9 } },
	{ "mspacren", mspacrom, mspacdsw, { 0xe9 } },
	{ "mspacrip", mspacrom, mspacdsw, { 0xe9 } },
	{ "mspacsm", mspacrom, mspacdsw, { 0xe9 } },
	{ "msrumble", mspacrom, mspacdsw, { 0xe9 } },
	{ "msultra", mspacrom, mspacdsw, { 0xe9 } },
	{ "msvectr", mspacrom, mspacdsw, { 0xe9 } },
	{ "msyakman", mspacrom, mspacdsw, { 0xe9 } },
	{ "punleash", mspacrom, mspacdsw, { 0xe9 } },
	{ "sueworld", mspacrom, mspacdsw, { 0xe9 } },
	{ "genericm", mspacrom, mspacdsw, { 0xe9 } },
	/* Following the generic Pac Man */
	{ 0, genericrom, pacdsw, { 0xe9 } }	/* generic entry used for Pac Man and variations */
};




/* Here comes the definition of the names of the ROMs to load for the various games. */
/* We load character data at 0x0000-0x0fff, sprite data at 0x1000-0x1fff. */
const struct RomModule genericgfx[] =
{
	{ "%s.5e", 0x0000, 0x1000 },
	{ "%s.5f", 0x1000, 0x1000 },
	{ 0 }	/* end of table */
};
const struct RomModule mspacgfx[] =
{
	{ "5e", 0x0000, 0x1000 },
	{ "5f", 0x1000, 0x1000 },
	{ 0 }	/* end of table */
};
const struct RomModule crushgfx[] =
{
	{ "CRA", 0x0000, 0x0800 },
	{ "CRC", 0x0800, 0x0800 },
	{ "CRB", 0x1000, 0x0800 },
	{ "CRD", 0x1800, 0x0800 },
	{ 0 }	/* end of table */
};

const struct RomModule piranhagfx[] =
{
	{ "pr5.cpu", 0x0000, 0x0800 },
	{ "pr6.cpu", 0x0800, 0x0800 },
	{ "pr7.cpu", 0x1000, 0x0800 },
	{ "pr8.cpu", 0x1800, 0x0800 },
	{ 0 }	/* end of table */
};


struct GfxLayout charlayout =
{
	8,8,	/* 8*8 characters */
	256,	/* 256 characters */
	2,	/* 2 bits per pixel */
	4,	/* the two bitplanes for 4 pixels are packed into one byte */
	{ 7*8, 6*8, 5*8, 4*8, 3*8, 2*8, 1*8, 0*8 }, /* characters are rotated 90 degrees */
	{ 8*8+0, 8*8+1, 8*8+2, 8*8+3, 0, 1, 2, 3 },	/* bits are packed in groups of four */
	16*8	/* every char takes 16 bytes */
};
struct GfxLayout spritelayout =
{
	16,16,	/* 16*16 sprites */
	64,	/* 64 sprites */
	2,	/* 2 bits per pixel */
	4,	/* the two bitplanes for 4 pixels are packed into one byte */
	{ 39 * 8, 38 * 8, 37 * 8, 36 * 8, 35 * 8, 34 * 8, 33 * 8, 32 * 8,
			7 * 8, 6 * 8, 5 * 8, 4 * 8, 3 * 8, 2 * 8, 1 * 8, 0 * 8 },
	{ 8*8, 8*8+1, 8*8+2, 8*8+3, 16*8+0, 16*8+1, 16*8+2, 16*8+3,
			24*8+0, 24*8+1, 24*8+2, 24*8+3, 0, 1, 2, 3 },
	64*8	/* every sprite takes 64 bytes */
};



/* "What is the palette doing here", you might ask, "it's not a ROM!" */
/* Well, actually the palette and lookup table were stored in PROMs, whose */
/* image, unfortunately, is usually unavailable. So we have to supply our */
/* own. */

const unsigned char pacpalette[3 * TOTAL_COLORS] =
{
	0x00,0x00,0x00,	/* BLACK */
	0xdb,0x00,0x00,	/* RED */
	0xdb,0x92,0x49,	/* BROWN */
	0xff,0xb6,0xdb,	/* PINK */
	0x00,0x00,0x00,	/* UNUSED */
	0x00,0xff,0xdb,	/* CYAN */
	0x49,0xb6,0xdb,	/* DKCYAN */
	0xff,0xb6,0x49,	/* DKORANGE */
	0x00,0x00,0x00,	/* UNUSED */
	0xff,0xff,0x00,	/* YELLOW */
	0x00,0x00,0x00,	/* UNUSED */
	0x24,0x24,0xdb,	/* BLUE */
	0x00,0xff,0x00,	/* GREEN */
	0x49,0xb6,0x92,	/* DKGREEN */
	0xff,0xb6,0x92,	/* LTORANGE */
	0xdb,0xdb,0xdb	/* WHITE */
};

enum {BLACK,RED,BROWN,PINK,UNUSED1,CYAN,DKCYAN,DKORANGE,
		UNUSED2,YELLOW,UNUSED3,BLUE,GREEN,DKGREEN,LTORANGE,WHITE};
#define UNUSED BLACK

/* The color of each character/sprite is determined by a 5 bit value, which */
/* selects the entry to use in this lookup table, and by the 2 bits of the */
/* chracter/sprite data. Additionally, bit 7 of the byte stored in Color RAM */
/* seems to be set for text characters; the reason is yet unknown. */
const unsigned char paccolortable[4 * COLOR_CODES] =
{
	BLACK,BLACK,BLACK,BLACK,		/* 0x00 Background in intermissions */
	BLACK,WHITE,BLUE,RED,			/* 0x01 - SHADOW  "BLINKY" */
	UNUSED,UNUSED,UNUSED,UNUSED,	/* 0x02 Unused */
	BLACK,WHITE,BLUE,PINK,			/* 0x03 - SPEEDY  "PINKY" */
	UNUSED,UNUSED,UNUSED,UNUSED,	/* 0x04 Unused */
	BLACK,WHITE,BLUE,CYAN,			/* 0x05 - BASHFUL  "INKY" */
	UNUSED,UNUSED,UNUSED,UNUSED,	/* 0x06 Unused */
	BLACK,WHITE,BLUE,DKORANGE,		/* 0x07 - POKEY  "CLYDE"; Ms Pac Man 4th maze */
	UNUSED,UNUSED,UNUSED,UNUSED,	/* 0x08 Unused */
	BLACK,BLUE,RED,YELLOW,			/* 0x09 Lives left; Bird; Pac Man */
	UNUSED,UNUSED,UNUSED,UNUSED,	/* 0x0a Unused */
	UNUSED,UNUSED,UNUSED,UNUSED,	/* 0x0b Unused */
	UNUSED,UNUSED,UNUSED,UNUSED,	/* 0x0c Unused */
	UNUSED,UNUSED,UNUSED,UNUSED,	/* 0x0d Unused */
	BLACK,WHITE,BLACK,LTORANGE,		/* 0x0e BONUS PAC-MAN for xx000 PTS */
	BLACK,RED,GREEN,WHITE,			/* 0x0f White text; Strawberry */
	BLACK,LTORANGE,BLACK,BLUE,		/* 0x10 Background; Maze walls & dots */
	BLACK,GREEN,BLUE,LTORANGE,		/* 0x11 Blue ghosts */
	BLACK,GREEN,WHITE,RED,			/* 0x12 White ghosts */
	UNUSED,UNUSED,UNUSED,UNUSED,	/* 0x13 Unused */
	BLACK,RED,BROWN,WHITE,			/* 0x14 Cherry; Apple; Ms Pac Man 3rd maze */
	BLACK,DKORANGE,GREEN,BROWN,		/* 0x15 Orange */
	BLACK,YELLOW,DKCYAN,WHITE,		/* 0x16 Bell; Key; Ms Pac Man 2nd maze */
	BLACK,DKGREEN,GREEN,WHITE,		/* 0x17 Grape; Ms Pac Man pear */
	BLACK,CYAN,PINK,YELLOW,			/* 0x18 Barn door; Points when eating ghost */
									/*      Ms Pac Man 5th maze */
	BLACK,WHITE,BLUE,BLACK,			/* 0x19 Ghost eyes */
	BLACK,LTORANGE,UNUSED,UNUSED,	/* 0x1a Dots around starting position */
	BLACK,LTORANGE,BLACK,BLUE,		/* 0x1b Tunnel */
	UNUSED,UNUSED,UNUSED,UNUSED,	/* 0x1c Unused */
	BLACK,WHITE,LTORANGE,RED,		/* 0x1d Ghost without sheet (2nd intermission) */
									/*      Ms Pac Man 1st maze */
	BLACK,WHITE,BLUE,LTORANGE,		/* 0x1e Ghost without sheet (3rd intermission) */
	BLACK,LTORANGE,BLACK,WHITE		/* 0x1f 10 pts, 50 pts; White maze walls when level complete */
};



const unsigned char crushpalette[3 * TOTAL_COLORS] =
{
	0x00,0x00,0x00,	/* BLACK */
	0xdb,0x00,0x00,	/* RED */
	0xdb,0x92,0x49,	/* BROWN */
	0xff,0xb6,0xdb,	/* PINK */
	0x00,0x00,0x00,	/* UNUSED */
	0x00,0xdb,0xdb,	/* CYAN */
	0x49,0xb6,0xdb,	/* DKCYAN */
	0xff,0xb6,0x49,	/* DKORANGE */
	0x88,0x88,0x88,	/* UNUSED */
	0xdb,0xdb,0x00,	/* YELLOW */
	0xff,0x00,0xdb,	/* UNUSED */
	0x24,0x24,0xdb,	/* BLUE */
	0x00,0xdb,0x00,	/* GREEN */
	0x49,0xb6,0x92,	/* DKGREEN */
	0xff,0xb6,0x92,	/* LTORANGE */
	0xdb,0xdb,0xdb	/* GREY */
};

enum {CBLACK,CRED,CBROWN,CPINK,CUNUSED1,CCYAN,CDKCYAN,CDKORANGE,
		CUNUSED2,CYELLOW,CUNUSED3,CBLUE,CGREEN,CDKGREEN,CLTORANGE,CGREY};
#define CUNUSED CRED

#define WALLS 11
#define GROUND 15
#define CAR CCYAN

const unsigned char crushcolortable[4 * COLOR_CODES] =
{
	CBLACK,2,WALLS,4,	/* squasher */
	CBLACK,CGREY,CBLUE,CRED,	/* brush */
	CBLACK,GROUND,WALLS,CPINK,	/* 2nd level paint */
	CBLACK,GROUND,WALLS,GROUND,	/* background, text, maze */
	CBLACK,GROUND,WALLS,CDKORANGE,	/* 3rd level paint */
	CBLACK,1,CBLUE,CCYAN,	/* monster 2 */
	CBLACK,GROUND,WALLS,CGREEN,	/* 1st level paint */
	CBLACK,1,CBLUE,CYELLOW,	/* monster 1 / cat */
	0,6,7,8,	/* evil cat (I think) */
	CBLACK,WALLS,CRED,CYELLOW,	/* squashed cat, closed barn (sprite), mouse */
	10,10,10,10,
	CBLACK,CBROWN,CAR,CRED,	/* driver's head */
	CBLACK,13,CAR,CRED,	/* car door */
	0,CAR,WALLS,CYELLOW,	/* car */
	14,14,14,14,
	CBLACK,CRED,CGREEN,CGREY,	/* bird */
	7,CBLACK,9,10,	/* squashed monster */
	2,2,2,2,
	1,1,1,1,
	3,3,3,3,
	CBLACK,CRED,CYELLOW,13,	/* bird */
	5,5,5,5,
	6,6,6,6,
	7,7,7,7,
	15,15,15,15,
	0,GROUND,4,WALLS,	/* barn */
	10,GROUND,4,WALLS,	/* closed barn */
	CBLACK,WALLS,CCYAN,CYELLOW,	/* big cat */
	CBLACK,WALLS,CGREEN,CGREY,	/* tree */
	CBLACK,WALLS,CDKORANGE,CYELLOW,	/* cat, bird's head */
	9,CBROWN,WALLS,12,	/* under cat gone */
	9,CBROWN,WALLS,12,	/* under cat */
};


const struct GameVidInfo gamevidinfo[] =
{
	{ "pacmod", mspacgfx, &charlayout, &spritelayout, pacpalette, paccolortable },
	{ "mspacman", mspacgfx, &charlayout, &spritelayout, pacpalette, paccolortable },
	{ "crush", crushgfx, &charlayout, &spritelayout, crushpalette, crushcolortable },
	/* Added Piranha*/
	{ "piranha", piranhagfx, &charlayout, &spritelayout, pacpalette, paccolortable },
	/* Added some Ms Pacman Hacks*/
	{ "cookiem", mspacgfx, &charlayout, &spritelayout, pacpalette, paccolortable },
	{ "heartbrn", mspacgfx, &charlayout, &spritelayout, pacpalette, paccolortable },
	{ "mrpacman", mspacgfx, &charlayout, &spritelayout, pacpalette, paccolortable },
	{ "msbaby", mspacgfx, &charlayout, &spritelayout, pacpalette, paccolortable },
	{ "msberzk", mspacgfx, &charlayout, &spritelayout, pacpalette, paccolortable },
	{ "msdroid", mspacgfx, &charlayout, &spritelayout, pacpalette, paccolortable },
	{ "mselton", mspacgfx, &charlayout, &spritelayout, pacpalette, paccolortable },
	{ "msf1pac", mspacgfx, &charlayout, &spritelayout, pacpalette, paccolortable },
	{ "mshangly", mspacgfx, &charlayout, &spritelayout, pacpalette, paccolortable },
	{ "mshearts", mspacgfx, &charlayout, &spritelayout, pacpalette, paccolortable },
	{ "msmini", mspacgfx, &charlayout, &spritelayout, pacpalette, paccolortable },
	{ "mspacad", mspacgfx, &charlayout, &spritelayout, pacpalette, paccolortable },
	{ "mspaccm", mspacgfx, &charlayout, &spritelayout, pacpalette, paccolortable },
	{ "mspacdx", mspacgfx, &charlayout, &spritelayout, pacpalette, paccolortable },
	{ "mspacren", mspacgfx, &charlayout, &spritelayout, pacpalette, paccolortable },
	{ "mspacrip", mspacgfx, &charlayout, &spritelayout, pacpalette, paccolortable },
	{ "mspacsm", mspacgfx, &charlayout, &spritelayout, pacpalette, paccolortable },
	{ "msrumble", mspacgfx, &charlayout, &spritelayout, pacpalette, paccolortable },
	{ "msultra", mspacgfx, &charlayout, &spritelayout, pacpalette, paccolortable },
	{ "msvectr", mspacgfx, &charlayout, &spritelayout, pacpalette, paccolortable },
	{ "msyakman", mspacgfx, &charlayout, &spritelayout, pacpalette, paccolortable },
	{ "punleash", mspacgfx, &charlayout, &spritelayout, pacpalette, paccolortable },
	{ "sueworld", mspacgfx, &charlayout, &spritelayout, pacpalette, paccolortable },
	{ "genericm", mspacgfx, &charlayout, &spritelayout, pacpalette, paccolortable },
	{ 0, genericgfx, &charlayout, &spritelayout, pacpalette, paccolortable }	/* generic entry used for Pac Man and variations */
};

