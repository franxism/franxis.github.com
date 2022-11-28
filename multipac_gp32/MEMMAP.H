/***************************************************************************

  memmap.h

  This include file contains definitions of memory addresses, I/O ports,
  dip switch settings, used by the emulation

***************************************************************************/

/***************************************************************************

Pac Man memory map (preliminary)

0000-3fff ROM
4000-43ff Video RAM (V RAM)
4400-47ff Color RAM (C RAM)
4c00-4fff RAM (W RAM)
8000-9fff ROM (Ms Pac Man only)

memory mapped ports:

read:
5000      IN0
5040      IN1
5080      DSW1

write:
4ff2-4ffd 6 pairs of two bytes:
          the first byte contains the sprite image number (bits 2-7), Y flip (bit 0),
		  X flip (bit 1); the second byte the color
5000      interrupt enable
5001      sound enable
5002      ????
5003      flip screen
5004      probably used to make the 1 player start button flash on the arcade machine
5005      same for player 2
5006      related to the credits. don't know what it was used for.
5007      coin counter (pulsing it would increase the counter on the machine)
5040-5044 sound voice 1 accumulator (nibbles) (used by the sound hardware only)
5045      sound voice 1 waveform (nibble)
5046-5049 sound voice 2 accumulator (nibbles) (used by the sound hardware only)
504a      sound voice 2 waveform (nibble)
504b-504e sound voice 3 accumulator (nibbles) (used by the sound hardware only)
504f      sound voice 3 waveform (nibble)
5050-5054 sound voice 1 frequency (nibbles)
5055      sound voice 1 volume (nibble)
5056-5059 sound voice 2 frequency (nibbles)
505a      sound voice 2 volume (nibble)
505b-505e sound voice 3 frequency (nibbles)
505f      sound voice 3 volume (nibble)
5062-506d Sprite coordinates, x/y pairs for 6 sprites
50c0      Watchdog reset

I/O ports:
OUT on port $0 sets the interrupt vector

***************************************************************************/


#define ROM_END 0x3fff

#define INTERRUPT_ENABLE 0x5000

#define VIDEO_RAM_START 0x4000
#define VIDEO_RAM_SIZE 0x400
#define COLOR_RAM_START (VIDEO_RAM_START+VIDEO_RAM_SIZE)

#define SPRITE_COORDS_BASE 0x5062
#define SPRITE_CODES_BASE 0x4ff2

#define SOUND_ENABLE 0x5001
#define SOUND_BASE 0x5040


/*
 * IN0 (all bits are inverted)
 * bit 7 : CREDIT
 * bit 6 : COIN 2
 * bit 5 : COIN 1
 * bit 4 : DIP SWITCH 7  RACK TEST (Pac Man and Ms Pac Man)
 * bit 4 : DIP SWITCH 7  TABLE or STANDUP cabinet (Crush Roller) 0 = STANDUP
 * bit 3 : DOWN player 1
 * bit 2 : RIGHT player 1
 * bit 1 : LEFT player 1
 * bit 0 : UP player 1
 */
#define IN0_PORT 0x5000
#define IN0_CREDIT (1<<7)
#define IN0_COIN2 (1<<6)
#define IN0_COIN1 (1<<5)
#define IN0_RACK_TEST (1<<4)
#define IN0_DOWN (1<<3)
#define IN0_RIGHT (1<<2)
#define IN0_LEFT (1<<1)
#define IN0_UP (1<<0)

/*
 * IN1 (all bits are inverted)
 * bit 7 : TABLE or STANDUP cabinet (1 = STANDUP)
 * bit 6 : START 2
 * bit 5 : START 1
 * bit 4 : TEST SWITCH (not Crush Roller)
 * bit 3 : DOWN player 2 (TABLE only)
 * bit 2 : RIGHT player 2 (TABLE only)
 * bit 1 : LEFT player 2 (TABLE only)
 * bit 0 : UP player 2 (TABLE ony)
 */
#define IN1_PORT 0x5040
#define IN1_TABLE (1<<7)
#define IN1_START2 (1<<6)
#define IN1_START1 (1<<5)
#define IN1_TEST (1<<4)
#define IN1_DOWN (1<<3)
#define IN1_RIGHT (1<<2)
#define IN1_LEFT (1<<1)
#define IN1_UP (1<<0)

/*
 * DSW1 (all bits are inverted)
 * bit 7 : SOLDER PAD 2  (PacMan only) selects the names for the ghosts
 *                       1 = Normal 0 = Alternate
 * bit 6 : SOLDER PAD 1  difficulty level
 *                       1 = Normal  0 = Harder
 * bit 5 : DIP SWITCH 6\ bonus pac at xx000 pts (Pac Man and Ms Pac Man)
 * bit 4 : DIP SWITCH 5/ 00 = 10000  01 = 15000  10 = 20000  11 = none
 * bit 5 : DIP SWITCH 6  Teleport holes 0 = on 1 = off (Crush Roller)
 * bit 4 : DIP SWITCH 5  First pattern difficulty 1 = easy 0 = hard (Crush Roller)
 * bit 3 : DIP SWITCH 4\ nr of lives
 * bit 2 : DIP SWITCH 3/ 00 = 1  01 = 2  10 = 3  11 = 5 (Pac Man and Ms Pac Man)
 *                       00 = 3  01 = 4  10 = 5  11 = 6 (Crush Roller)
 * bit 1 : DIP SWITCH 2\ play mode
 * bit 0 : DIP SWITCH 1/ 00 = free play   01 = 1 coin 1 credit
 *                       10 = 1 coin 2 credits   11 = 2 coins 1 credit
 */
#define DSW1_PORT 0x5080
#define DSW1_SP2 (1<<7)
#define DSW1_SP1 (1<<6)
#define DSW1_SW6 (1<<5)
#define DSW1_SW5 (1<<4)
#define DSW1_SW4 (1<<3)
#define DSW1_SW3 (1<<2)
#define DSW1_SW2 (1<<1)
#define DSW1_SW1 (1<<0)
