/*********************************************************************

  common.h

  Generic functions used in different emulators.
  There's not much for now, but it could grow in the future... ;-)

*********************************************************************/


struct RomModule
{
	const char *name;	/* name of the file to load */
	int offset;			/* offset to load it to */
	int size;			/* length of the file */
};


struct GfxLayout
{
	int width,height;	/* width and height of chars/sprites */
	int total;	/* total numer of chars/sprites in the rom */
	int planes;	/* number of bitplanes */
	int planeincrement;	/* distance (in bits) between two adjacent bitplanes */
	int xoffset[32];	/* coordinates of the bit corresponding to the pixel */
	int yoffset[32];	/* of the given coordinates */
	int charincrement;	/* distance between two consecutive characters/sprites */
};



/* dipswitch setting definition */
struct DSW
{
	int num;	/* dispswitch pack affected */
				/* -1 terminates the array */
	int mask;	/* bits affected */
	const unsigned char *name;	/* name of the setting */
	const unsigned char *values[16];/* null terminated array of names for the values */
									/* the setting can have */
	int reverse; 	/* set to 1 to display values in reverse order */
};


int readroms(unsigned char *dest,const struct RomModule *romp,const char *basename);
void gfxdecode(unsigned char *dest,const unsigned char *src,const struct GfxLayout *gl);
void setdipswitches(int *dsw,const struct DSW *dswsettings);
