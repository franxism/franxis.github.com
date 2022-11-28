struct DisplayText
{
	const unsigned char *text;	/* 0 marks the end of the array */
	int color;
	int x;
	int y;
};


int vh_init(const char *gamename);
int vh_start(void);
void vh_stop(void);
int vh_wrmem(dword A,byte V);
void vh_screenrefresh(void);
void displaytext(const struct DisplayText *dt,int erase);


/* Pac Man has a 28x36 characters screen */
#define H_CHARS	28
#define V_CHARS 36
#define BITMAP_WIDTH (8*H_CHARS)
#define BITMAP_HEIGHT (8*V_CHARS)

#define FIRST_VISIBLE_ROW 0
#define LAST_VISIBLE_ROW V_CHARS
#define FIRST_VISIBLE_COLUMN 0
#define LAST_VISIBLE_COLUMN H_CHARS

#define TOTAL_CHARS 256
#define TOTAL_SPRITES 64
#define SPRITE_WIDTH 16
#define SPRITE_HEIGHT 16
