#include "driver.h"

int use_mouse;
int joystick;
int ExKey;

void msdos_init_input (void)
{
	joystick = 1 /*JOY_TYPE_NONE*/;
	use_mouse = 0;
}


void msdos_shutdown_input(void)
{
}


/*
 * Check if a key is pressed. The keycode is the standard PC keyboard
 * code, as defined in osdepend.h. Return 0 if the key is not pressed,
 * nonzero otherwise. Handle pseudo keycodes.
 */
int osd_key_pressed(int keycode)
{
	if (keycode > OSD_MAX_KEY)
	{
		switch (keycode)
        	{
            		case OSD_KEY_CANCEL:
            			return ((ExKey & GPC_VK_FL) && (ExKey & GPC_VK_FR));
				/*key[OSD_KEY_ESC]*/;

            		case OSD_KEY_SHOW_GFX:
				return 0;

	    		case OSD_KEY_RESET_MACHINE:
	    			return ((ExKey & GPC_VK_START) && (ExKey & GPC_VK_SELECT));

            		case OSD_KEY_CHEAT_TOGGLE:
				return 0;

            		case OSD_KEY_JOY_CALIBRATE:
				return 0;

            		case OSD_KEY_FRAMESKIP:
                		return 0;

            		case OSD_KEY_THROTTLE:
                		return 0;

            		case OSD_KEY_SHOW_FPS:
                		return 0;

            		case OSD_KEY_SHOW_PROFILE:
                		return 0;

            		case OSD_KEY_CONFIGURE:
				return 0;

            		case OSD_KEY_ON_SCREEN_DISPLAY:
				return 0;

            		case OSD_KEY_DEBUGGER:
				return 0;

            		case OSD_KEY_PAUSE:
            		case OSD_KEY_UNPAUSE:
				ExKey=GpKeyGet();
				return (ExKey & GPC_VK_FR);
				/*key[OSD_KEY_P]*/;

	    		default:
				return 0;
        	}
    	}
	/*if (keycode == OSD_KEY_RCONTROL) keycode = KEY_RCONTROL;*/
	/*if (keycode == OSD_KEY_ALTGR) keycode = KEY_ALTGR;*/
	
	if (keycode==OSD_KEY_3) {
		/*int ExKey=GpKeyGet();*/
		return (ExKey & GPC_VK_FL);
	}
	
	if (keycode==OSD_KEY_1) {
		/*int ExKey=GpKeyGet();*/
		return (ExKey & GPC_VK_START);
	}
		
		
	
	/*return key[keycode];*/
	return 0;
}

static char memory[256];

/* Report a key as pressed only when the user hits it, not while it is */
/* being kept pressed. */
int osd_key_pressed_memory(int keycode)
{
	int res = 0;

	if (keycode == OSD_KEY_UNPAUSE) keycode = OSD_KEY_PAUSE;	/* we use the same key */

	if (osd_key_pressed(keycode))
	{
		if (memory[keycode] == 0) res = 1;
		memory[keycode] = 1;
	}
	else
		memory[keycode] = 0;

	return res;
}

/* report kay as pulsing while it is pressed */
int osd_key_pressed_memory_repeat(int keycode,int speed)
{
	static int counter;
	int res = 0;

	if (osd_key_pressed(keycode))
	{
		if (memory[keycode] == 0 || ++counter > speed * Machine->drv->frames_per_second / 60)
		{
			counter = 0;
			res = 1;
		}
		memory[keycode] = 1;
	}
	else
		memory[keycode] = 0;

	return res;
}


/* If the user presses a key return it, otherwise return OSD_KEY_NONE. */
/* DO NOT wait for the user to press a key */
int osd_read_key_immediate(void)
{
	int res;


	/* first of all, record keys which are NOT pressed */
	for (res = OSD_MAX_KEY;res > OSD_KEY_NONE;res--)
	{
		if (!osd_key_pressed(res))
		{
			memory[res] = 0;
			memory[key_to_pseudo_code(res)] = 0;
		}
	}

	for (res = OSD_MAX_KEY;res > OSD_KEY_NONE;res--)
	{
		if (osd_key_pressed(res))
		{
			if (memory[res] == 0)
			{
				memory[res] = 1;
				memory[key_to_pseudo_code(res)] = 1;
			}
			else res = OSD_KEY_NONE;
			break;
		}
	}

	return res;
}



/*
 * Wait for a key press and return the keycode.
 * Translate certain keys (or key combinations) back to
 * the pseudo key values defined in osdepend.h.
 */
int osd_read_key(int translate)
{
	int res;

	if (translate)
	{
		/* wait for all keys to be released (including pseudo keys) */
		do
		{
			for (res = OSD_MAX_PSEUDO; res >= 0; res--)
				if (osd_key_pressed(res))
					break;
		} while (res >= 0);

		/* wait for a key press (including pseudo keys) */
		do
		{
			for (res = OSD_MAX_PSEUDO; res >= 0; res--)
				if (osd_key_pressed(res))
					break;
		} while (res < 0);
	}
	else
	{
		/* wait for all keys to be released (standard keys) */
		do
		{
			for (res = OSD_MAX_KEY; res >= 0; res--)
				if (osd_key_pressed(res))
					break;
		} while (res >= 0);

		/* wait for a key press (standard keys) */
		do
		{
			for (res = OSD_MAX_KEY; res >= 0; res--)
				if (osd_key_pressed(res))
					break;
        } while (res < 0);
    }

    /*if (res == KEY_RCONTROL) res = OSD_KEY_RCONTROL;*/
	/*if (res == KEY_ALTGR) res = OSD_KEY_ALTGR;*/
	return res;
}

/* translate a scancode to a pseudo key code */
int key_to_pseudo_code(int k)
{
    switch (k)
    {
        case OSD_KEY_ESC:
            return OSD_KEY_CANCEL;

        case OSD_KEY_F4:
            return OSD_KEY_SHOW_GFX;

		case OSD_KEY_F5:
			return OSD_KEY_CHEAT_TOGGLE;

		case OSD_KEY_F7:
			return OSD_KEY_JOY_CALIBRATE;

        case OSD_KEY_F8:
            return OSD_KEY_FRAMESKIP;

        case OSD_KEY_F3:
            return OSD_KEY_RESET_MACHINE;

        case OSD_KEY_F10:
            return OSD_KEY_THROTTLE;

        case OSD_KEY_F11:
            /*if ((key[OSD_KEY_LSHIFT] || key[OSD_KEY_RSHIFT]))*/
                /*return OSD_KEY_SHOW_PROFILE;*/
            return OSD_KEY_SHOW_FPS;

        case OSD_KEY_TAB:
            return OSD_KEY_CONFIGURE;

        case OSD_KEY_TILDE:
		return OSD_KEY_ON_SCREEN_DISPLAY;

        case OSD_KEY_P:
            return OSD_KEY_PAUSE;
    }
    return k;
}

/* Wait for a key press and return keycode.  Support repeat */
int osd_read_keyrepeat(int translate)
{
	int res=0;

	/*clear_keybuf();*/
	/*res = readkey() >> 8;*/

	/*if (res == KEY_RCONTROL) res = OSD_KEY_RCONTROL;*/
	/*if (res == KEY_ALTGR) res = OSD_KEY_ALTGR;*/

	if (translate)
		res = key_to_pseudo_code(res);
	return res;
}

int osd_debug_readkey(void)
{
	int res=0;

	/*clear_keybuf();*/
	/*res = readkey() >> 8;*/

	/*if (res == KEY_RCONTROL) res = OSD_KEY_RCONTROL;*/
	/*if (res == KEY_ALTGR) res = OSD_KEY_ALTGR;*/

	return res;
}


/* return the name of a key */
const char *osd_key_name(int keycode)
{
	static char *keynames[] =
	{
		"ESC", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "MINUS", "EQUAL", "BKSPACE",
		"TAB", "Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P", "OPBRACE", "CLBRACE", "ENTER",
		"LCTRL", "A", "S", "D", "F", "G", "H", "J", "K", "L", "COLON", "QUOTE", "TILDE",
		"LSHIFT", "Error", "Z", "X", "C", "V", "B", "N", "M", "COMMA", ".", "SLASH", "RSHIFT",
		"*", "ALT", "SPACE", "CAPSLOCK", "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10",
		"NUMLOCK", "SCRLOCK", "HOME", "UP", "PGUP", "MINUS PAD",
		"LEFT", "5 PAD", "RIGHT", "PLUS PAD", "END", "DOWN",
		"PGDN", "INS", "DEL", "RCTRL", "ALTGR", "Error",
		"F11", "F12", "Error", "Error",
		"Error", "Error", "Error", "Error", "Error",
		"Error", "Error", "Error", "Error", "Error",
		"1 PAD", "2 PAD", "3 PAD", "4 PAD", "Error",
		"6 PAD", "7 PAD", "8 PAD", "9 PAD", "0 PAD",
		". PAD", "= PAD", "/ PAD", "* PAD", "ENTER PAD",
		"Error", "Error", "Error", "Error", "Error",
        "Error", "Error", "PAUSE",
    };
	static char *nonedefined = "None";

	if (keycode && keycode <= OSD_MAX_KEY) return keynames[keycode-1];
	else return (char *)nonedefined;
}


/* return the name of a joystick button */
const char *osd_joy_name(int joycode)
{
	static char *joynames[] = {
		"Left", "Right", "Up", "Down", "Button A",
		"Button B", "Button C", "Button D", "Button E", "Button F",
		"Button G", "Button H", "Button I", "Button J", "Any Button",
		"J2 Left", "J2 Right", "J2 Up", "J2 Down", "J2 Button A",
		"J2 Button B", "J2 Button C", "J2 Button D", "J2 Button E", "J2 Button F",
		"J2 Button G", "J2 Button H", "J2 Button I", "J2 Button J", "J2 Any Button",
		"J3 Left", "J3 Right", "J3 Up", "J3 Down", "J3 Button A",
		"J3 Button B", "J3 Button C", "J3 Button D", "J3 Button E", "J3 Button F",
		"J3 Button G", "J3 Button H", "J3 Button I", "J3 Button J", "J3 Any Button",
		"J4 Left", "J4 Right", "J4 Up", "J4 Down", "J4 Button A",
		"J4 Button B", "J4 Button C", "J4 Button D", "J4 Button E", "J4 Button F",
		"J4 Button G", "J4 Button H", "J4 Button I", "J4 Button J", "J4 Any Button"
	};

	if (joycode == 0) return "None";
	else if (joycode <= OSD_MAX_JOY) return (char *)joynames[joycode-1];
	else return "Unknown";
}


void osd_poll_joystick(void)
{
}


/* check if the joystick is moved in the specified direction, defined in */
/* osdepend.h. Return 0 if it is not pressed, nonzero otherwise. */
int osd_joy_pressed(int joycode)
{
	/* which joystick are we polling? */
	if (joycode < OSD_JOY_LEFT || joycode >= OSD_JOY3_LEFT)
		return 0;
  	
  	ExKey=GpKeyGet();

	switch (joycode)
	{
		case OSD_JOY_LEFT:
		  	if (!gp32_rotate)
		  		return (ExKey & GPC_VK_LEFT);
		  	else
		  		return (ExKey & GPC_VK_UP);
			/*return joy[joy_num].stick[0].axis[0].d1;*/
			break;
		case OSD_JOY_RIGHT:
		  	if (!gp32_rotate)
			  	return (ExKey & GPC_VK_RIGHT);
		  	else
		  		return (ExKey & GPC_VK_DOWN);
			/*return joy[joy_num].stick[0].axis[0].d2;*/
			break;
		case OSD_JOY_UP:
		  	if (!gp32_rotate)
			  	return (ExKey & GPC_VK_UP);
		  	else
			  	return (ExKey & GPC_VK_RIGHT);
			/*return joy[joy_num].stick[0].axis[1].d1;*/
			break;
		case OSD_JOY_DOWN:
		  	if (!gp32_rotate)
		  		return (ExKey & GPC_VK_DOWN);
		  	else 
		  		return (ExKey & GPC_VK_LEFT);
			/*return joy[joy_num].stick[0].axis[1].d2;*/
			break;
		case OSD_JOY_FIRE1:
		  	if (!gp32_rotate)
		  		return (ExKey & GPC_VK_FA);
		  	else
		  		return ((ExKey & GPC_VK_FA) || (ExKey & GPC_VK_START));
			/*return joy[joy_num].button[0].b;*/
			break;
		case OSD_JOY_FIRE2:
		  	return (ExKey & GPC_VK_FB);
			/*return joy[joy_num].button[1].b;*/
			break;
		case OSD_JOY_FIRE3:
		  	return (ExKey & GPC_VK_SELECT);
			/*return joy[joy_num].button[2].b;*/
			break;
		case OSD_JOY_FIRE4:
		  	return (ExKey & GPC_VK_FL);
			/*return joy[joy_num].button[3].b;*/
			break;
		case OSD_JOY_FIRE5:
		  	return (ExKey & GPC_VK_START);
			/*return joy[joy_num].button[4].b;*/
			break;
		case OSD_JOY_FIRE6:
		  	return (ExKey & GPC_VK_FR);
			/*return joy[joy_num].button[5].b;*/
			break;
		case OSD_JOY_FIRE7:
			/*return joy[joy_num].button[6].b;*/
			break;
		case OSD_JOY_FIRE8:
			/*return joy[joy_num].button[7].b;*/
			break;
		case OSD_JOY_FIRE9:
			/*return joy[joy_num].button[8].b;*/
			break;
		case OSD_JOY_FIRE10:
			/*return joy[joy_num].button[9].b;*/
			break;
		case OSD_JOY_FIRE:
			return ( (ExKey & GPC_VK_FA) || (ExKey & GPC_VK_FB) || (ExKey & GPC_VK_SELECT) ||
				(ExKey & GPC_VK_FL) || (ExKey & GPC_VK_START) || (ExKey & GPC_VK_FR) );
			break;
		case OSD_JOY2_LEFT:
		  	if (!gp32_rotate)
		  		return (ExKey & GPC_VK_LEFT);
		  	else
		  		return (ExKey & GPC_VK_UP);
			/*return joy[joy_num].stick[0].axis[0].d1;*/
			break;
		case OSD_JOY2_RIGHT:
		  	if (!gp32_rotate)
			  	return (ExKey & GPC_VK_RIGHT);
		  	else
		  		return (ExKey & GPC_VK_DOWN);
			/*return joy[joy_num].stick[0].axis[0].d2;*/
			break;
		case OSD_JOY2_UP:
		  	if (!gp32_rotate)
			  	return (ExKey & GPC_VK_UP);
		  	else
			  	return (ExKey & GPC_VK_RIGHT);
			/*return joy[joy_num].stick[0].axis[1].d1;*/
			break;
		case OSD_JOY2_DOWN:
		  	if (!gp32_rotate)
		  		return (ExKey & GPC_VK_DOWN);
		  	else 
		  		return (ExKey & GPC_VK_LEFT);
			/*return joy[joy_num].stick[0].axis[1].d2;*/
			break;
		case OSD_JOY2_FIRE1:
		  	if (!gp32_rotate)
		  		return (ExKey & GPC_VK_FA);
		  	else
		  		return ((ExKey & GPC_VK_FA) || (ExKey & GPC_VK_START));
			/*return joy[joy_num].button[0].b;*/
			break;
		case OSD_JOY2_FIRE2:
		  	return (ExKey & GPC_VK_FB);
			/*return joy[joy_num].button[1].b;*/
			break;
		case OSD_JOY2_FIRE3:
		  	return (ExKey & GPC_VK_SELECT);
			/*return joy[joy_num].button[2].b;*/
			break;
		case OSD_JOY2_FIRE4:
		  	return (ExKey & GPC_VK_FL);
			/*return joy[joy_num].button[3].b;*/
			break;
		case OSD_JOY2_FIRE5:
		  	return (ExKey & GPC_VK_START);
			/*return joy[joy_num].button[4].b;*/
			break;
		case OSD_JOY2_FIRE6:
		  	return (ExKey & GPC_VK_FR);
			/*return joy[joy_num].button[5].b;*/
			break;
		case OSD_JOY2_FIRE7:
			/*return joy[joy_num].button[6].b;*/
			break;
		case OSD_JOY2_FIRE8:
			/*return joy[joy_num].button[7].b;*/
			break;
		case OSD_JOY2_FIRE9:
			/*return joy[joy_num].button[8].b;*/
			break;
		case OSD_JOY2_FIRE10:
			/*return joy[joy_num].button[9].b;*/
			break;
		case OSD_JOY2_FIRE:
			return ( (ExKey & GPC_VK_FA) || (ExKey & GPC_VK_FB) || (ExKey & GPC_VK_SELECT) ||
				(ExKey & GPC_VK_FL) || (ExKey & GPC_VK_START) || (ExKey & GPC_VK_FR) );
			break;
	}
	return 0;
}

int pos_analog_x=0, pos_analog_y=0;

/* return a value in the range -128 .. 128 (yes, 128, not 127) */
void osd_analogjoy_read(int *analog_x, int *analog_y)
{

	if (!gp32_rotate) {
		if(ExKey & GPC_VK_LEFT) pos_analog_x-=10;
	  	if(ExKey & GPC_VK_RIGHT) pos_analog_x+=10;
		if( (!(ExKey & GPC_VK_LEFT)) && (!(ExKey & GPC_VK_RIGHT)) ) pos_analog_x=0;
	  	if(ExKey & GPC_VK_UP) pos_analog_y-=10; 
	 	if(ExKey & GPC_VK_DOWN) pos_analog_y+=10;
		if( (!(ExKey & GPC_VK_UP)) && (!(ExKey & GPC_VK_DOWN)) ) pos_analog_y=0;
	} else {
		if(ExKey & GPC_VK_UP) pos_analog_x-=10;
		if(ExKey & GPC_VK_DOWN) pos_analog_x+=10;
		if( (!(ExKey & GPC_VK_UP)) && (!(ExKey & GPC_VK_DOWN)) ) pos_analog_x=0;
	  	if(ExKey & GPC_VK_RIGHT) pos_analog_y-=10; 
		if(ExKey & GPC_VK_LEFT) pos_analog_y+=10;
		if( (!(ExKey & GPC_VK_LEFT)) && (!(ExKey & GPC_VK_RIGHT)) ) pos_analog_y=0;
	}
	
	if (pos_analog_x<-128) pos_analog_x=-128;
	if (pos_analog_x>128) pos_analog_x=128;
	if (pos_analog_y<-128) pos_analog_y=-128;
	if (pos_analog_y>128) pos_analog_y=128;
	
	*analog_x = pos_analog_x;
	*analog_y = pos_analog_y;

}

void osd_trak_read(int *deltax,int *deltay)
{
	*deltax = *deltay = 0;
	if (!gp32_rotate) {
		if(ExKey & GPC_VK_LEFT) *deltax=-20;
	  	if(ExKey & GPC_VK_RIGHT) *deltax=20;
	  	if(ExKey & GPC_VK_UP) *deltay=20; 
	 	if(ExKey & GPC_VK_DOWN) *deltay=-20;
	} else {
		if(ExKey & GPC_VK_UP) *deltax=-20;
		if(ExKey & GPC_VK_DOWN) *deltax=20;
	  	if(ExKey & GPC_VK_RIGHT) *deltay=20; 
		if(ExKey & GPC_VK_LEFT) *deltay=-20;
	}
}

void osd_led_w(int led,int on) {
}

