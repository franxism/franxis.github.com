
#include "port.h"
#include "r3000a.h"
#include "plugins.h"
#include "wiz_lib.h"
#include "profiler.h"

enum {
	DKEY_SELECT = 0,
	DKEY_L3,
	DKEY_R3,
	DKEY_START,
	DKEY_UP,
	DKEY_RIGHT,
	DKEY_DOWN,
	DKEY_LEFT,
	DKEY_L2,
	DKEY_R2,
	DKEY_L1,
	DKEY_R1,
	DKEY_TRIANGLE,
	DKEY_CIRCLE,
	DKEY_CROSS,
	DKEY_SQUARE,

	DKEY_TOTAL
};

static char frontend[64]="";
void pcsx4all_exit(void)
{
	pcsx4all_prof_show();
	dbg_print_analysis();
	ReleasePlugins();
	psxShutdown();
	wiz_deinit();
	sync();
	if (frontend[0]!='\0') execl(frontend, frontend, "cache", NULL);
	exit(0);
}

static int autosavestate=0;

static unsigned short pad=0xffff;

void pad_update(void)
{
	unsigned key=wiz_joystick_read(0);
	pad=0xffff;
	if(key&WIZ_A) pad &= ~(1 << DKEY_SQUARE);
	if(key&WIZ_B) pad &= ~(1 << DKEY_CIRCLE);
	if(key&WIZ_X) pad &= ~(1 << DKEY_CROSS);
	if(key&WIZ_Y) pad &= ~(1 << DKEY_TRIANGLE);
	if((key&WIZ_L)&&(!(key&WIZ_SELECT))) pad &= ~(1 << DKEY_L1);
	if((key&WIZ_R)&&(!(key&WIZ_SELECT))) pad &= ~(1 << DKEY_R1);
	if((key&WIZ_L)&&( (key&WIZ_SELECT))) pad &= ~(1 << DKEY_L2);
	if((key&WIZ_R)&&( (key&WIZ_SELECT))) pad &= ~(1 << DKEY_R2);
	if(key&WIZ_MENU) pad &= ~(1 << DKEY_START);
	if(key&WIZ_SELECT) pad &= ~(1 << DKEY_SELECT);
	if(key&WIZ_LEFT) pad &= ~(1 << DKEY_LEFT);
	if(key&WIZ_RIGHT) pad &= ~(1 << DKEY_RIGHT);
	if(key&WIZ_UP) pad &= ~(1 << DKEY_UP);
	if(key&WIZ_DOWN) pad &= ~(1 << DKEY_DOWN);
	if((key & WIZ_L) && (key & WIZ_R) && (key & WIZ_MENU)) {
		if (autosavestate) {
			toExit=1;
			toSaveState=1;
		}else
			pcsx4all_exit();
	}
	#ifdef gpu_unai
	if((key & WIZ_L) && (key & WIZ_R) && (key & WIZ_SELECT))
	{
		extern bool show_fps; show_fps=!show_fps;
		do { key=wiz_joystick_read(0); } while ((key & WIZ_L) && (key & WIZ_R) && (key & WIZ_SELECT));
	}
	#endif
	#ifdef PROFILER_PCSX4ALL_RESET
		if(key & WIZ_L) { pcsx4all_prof_reset(); pcsx4all_prof_start(PCSX4ALL_PROF_CPU); }
	#endif
	
	if ((key&WIZ_SELECT) && (key&WIZ_VOLDOWN)) { if ((!toLoadState)&&(!toSaveState)) toLoadState=1; while ((key&WIZ_SELECT) || (key&WIZ_VOLDOWN)) key=wiz_joystick_read(0); }
	if ((key&WIZ_SELECT) && (key&WIZ_VOLUP)) { if ((!toLoadState)&&(!toSaveState)) toSaveState=1; while ((key&WIZ_SELECT) || (key&WIZ_VOLUP)) key=wiz_joystick_read(0); }
}

unsigned short pad_read(int num)
{
	if (num==0) return pad;	else return 0xffff;
}

void sound_init(void)
{
	#ifndef spu_null
	#ifdef spu_franxis
	extern bool nullspu; if (!nullspu)
	#endif
	wiz_sound_thread_start();
	#endif
}

void sound_close(void)
{
	#ifndef spu_null
	#ifdef spu_franxis
	extern bool nullspu; if (!nullspu)
	#endif
	wiz_sound_thread_stop();
	#endif
}

unsigned long sound_get(void)
{
	#ifndef spu_null
	return 0; // return 0 to get more bytes
	#else
	return 24193; // no more bytes
	#endif
}

void sound_set(unsigned char* pSound,long lBytes)
{
	#ifndef spu_null
	wiz_sound_play(pSound, lBytes);
	#endif
}

void video_flip(void)
{
}

void video_set(unsigned short* pVideo,unsigned int width,unsigned int height)
{
	int y;
	unsigned short *ptr=fb1_16bit;
	int w=(width>320?320:width);
	int h=(height>240?240:height);

	for (y=0;y<h;y++)
	{
		memcpy (ptr,pVideo,w*2);
		ptr+=320;
		pVideo+=width;
	}
}

void video_clear(void)
{
	memset(fb1_16bit,0,(320*240*2));
}

unsigned short *SCREEN=NULL;
static char savename[256];

int main (int argc, char **argv)
{
	char filename[256];
	const char *cdrfilename=GetIsoFile();
	filename[0] = '\0'; /* Executable file name */
	savename[0] = '\0'; /* SaveState file name */
	
	// PCSX default values
	Config.Xa=1; /* 0=XA enabled, 1=XA disabled */
	Config.Sio=0; /* 1=Sio Irq Always Enabled */
	Config.Mdec=0; /* 0=Black&White Mdecs Only Disabled, 1=Black&White Mdecs Only Enabled */
	Config.PsxAuto=1; /* 1=autodetect system (pal or ntsc) */
	Config.PsxType=0; /* PSX_TYPE_NTSC=ntsc, PSX_TYPE_PAL=pal */
	Config.Cdda=1; /* 0=Enable Cd audio, 1=Disable Cd audio */
	Config.HLE=1; /* 1=HLE, 0=bios */
	Config.Cpu=0; /* 0=recompiler, 1=interpreter */
	Config.SpuIrq=0; /* 1=Spu Irq Always Enabled */
	Config.RCntFix=0; /* 1=Parasite Eve 2, Vandal Hearts 1/2 Fix */
	Config.VSyncWA=0; /* 1=InuYasha Sengoku Battle Fix */

	/*
	// spu_dfxsound
	#ifdef spu_dfxsound
	{
	extern int iDisStereo; iDisStereo=1; // 0=stereo, 1=mono
	extern int iUseInterpolation; iUseInterpolation=0; // 0=disabled, 1=enabled
	extern int iUseReverb; iUseReverb=0; // 0=disabled, 1=enabled
	wiz_sound_rate=44100;
	}
	#endif
	*/

	/*
	// gpu_dfxvideo
	#ifdef gpu_dfxvideo
	extern int UseFrameLimit; UseFrameLimit=1; // limit fps 1=on, 0=off
	extern int UseFrameSkip; UseFrameSkip=1; // frame skip 1=on, 0=off
	extern int iFrameLimit; iFrameLimit=2; // fps limit 2=auto 1=fFrameRate, 0=off
	extern float fFrameRate; fFrameRate=200.0f; // fps
	extern int iUseDither; iUseDither=0; // 0=off, 1=game dependant, 2=always
	extern int iUseFixes; iUseFixes=0; // use game fixes
	extern uint32_t dwCfgFixes; dwCfgFixes=0; // game fixes
	#endif
	*/
	/*
	 1=odd/even hack (Chrono Cross)
	 2=expand screen width (Capcom fighting games)
	 4=ignore brightness color (black screens in Lunar)
	 8=disable coordinate check (compatibility mode)
	 16=disable cpu saving (for precise framerate)
	 32=PC fps calculation (better fps limit in some games)
	 64=lazy screen update (Pandemonium 2)
	 128=old frame skipping (skip every second frame)
	 256=repeated flat tex triangles (Dark Forces)
	 512=draw quads with triangles (better g-colors, worse textures)
	*/

	/*
	// gpu_drhell
	#ifdef gpu_drhell
	extern unsigned int autoFrameSkip; autoFrameSkip=1; // auto frameskip
	extern signed int framesToSkip; framesToSkip=0; // frames to skip
	#endif
	*/
	
	// gpu_unai
	/*
	#ifdef gpu_unai
	extern int skipCount; skipCount=2; // frame skip (0,1,2,3...)
	extern bool enableAbbeyHack; enableAbbeyHack=false; // Abe's Odyssey hack
	extern int linesInterlace_user; linesInterlace_user=0; // interlace
	#endif
	*/
	
	//SetIsoFile("cotton.iso");
	//SetIsoFile("ridge.bin");
	//SetIsoFile("bustmove.bin");
	//SetIsoFile("tekken3.bin");
	//SetIsoFile("bubble.bin");
	//SetIsoFile("castle.bin");
	//SetIsoFile("pang.iso");
	//SetIsoFile("castle.bin");
	//SetIsoFile("yoyo.bin");
	
	//wiz_clock=700;
	//extern int wiz_ram_tweaks;
	//wiz_ram_tweaks=1;

	// command line options
	for (int i=1;i<argc;i++)
	{
		// PCSX
		if (strcmp(argv[i],"-xa")==0) Config.Xa=0; // XA enabled
		if (strcmp(argv[i],"-sioirq")==0) Config.Sio=1; // Sio Irq Always Enabled
		if (strcmp(argv[i],"-bwmdec")==0) Config.Mdec=1; // Black & White MDEC
		if (strcmp(argv[i],"-pal")==0) { Config.PsxAuto=0; Config.PsxType=1; } // Force PAL system
		if (strcmp(argv[i],"-ntsc")==0) { Config.PsxAuto=0; Config.PsxType=0; } // Force NTSC system
		if (strcmp(argv[i],"-cdda")==0) Config.Cdda=0; // CD audio enabled
		if (strcmp(argv[i],"-bios")==0) Config.HLE=0; // BIOS enabled
		if (strcmp(argv[i],"-interpreter")==0) Config.Cpu=1; // Interpreter enabled
		if (strcmp(argv[i],"-spuirq")==0) Config.SpuIrq=1; // Spu Irq Always Enabled
		if (strcmp(argv[i],"-rcntfix")==0) Config.RCntFix=1; // Parasite Eve 2, Vandal Hearts 1/2 Fix
		if (strcmp(argv[i],"-vsyncwa")==0) Config.VSyncWA=1; // InuYasha Sengoku Battle Fix
		if (strcmp(argv[i],"-iso")==0) SetIsoFile(argv[i+1]); // Set ISO file
		if (strcmp(argv[i],"-file")==0) strcpy(filename,argv[i+1]); // Set executable file
		if (strcmp(argv[i],"-savestate")==0) strcpy(savename,argv[i+1]); // Set SaveState file
		if (strcmp(argv[i],"-autosavestate")==0) autosavestate=1; // Autosavestate
		if (strcmp(argv[i],"-bias")==0) {
			BIAS=atoi(argv[i+1]); // Set BIAS
			if (((int)BIAS)<1) {
				autobias=1;
				BIAS=2;
			} else {
				autobias=0;
			}
		}
		if (strcmp(argv[i],"-adjust")==0) { PSXCLK=(u32)((double)PSXCLK*atof(argv[i+1])); }
		
		// GPU
		if (strcmp(argv[i],"-framelimit")==0) { extern bool frameLimit; frameLimit=true; } // frames to wait
		if (strcmp(argv[i],"-skip")==0) { extern int skipCount; skipCount=atoi(argv[i+1]); } // frame skip (0,1,2,3...)
		if (strcmp(argv[i],"-abbey")==0) { extern bool enableAbbeyHack; enableAbbeyHack=true; } // Abe's Odyssey hack
		if (strcmp(argv[i],"-interlace")==0) { extern int linesInterlace_user; linesInterlace_user=1; } // interlace
		if (strcmp(argv[i],"-progressive")==0) { extern bool progressInterlace; progressInterlace=true; } // progressive interlace
		if (strcmp(argv[i],"-alt_fps")==0) { extern bool alt_fps; alt_fps=true; } // alternate FPS algorithm
		if (strcmp(argv[i],"-no_blend")==0) { extern bool blend; blend=false; } // disable blending
		if (strcmp(argv[i],"-no_light")==0) { extern bool light; light=false; } // disable lighting

		if (strcmp(argv[i],"-secure_writes")==0) { extern bool rec_secure_writes; rec_secure_writes=true; } // securized writes
		// SPU
	#ifndef spu_null
		if (strcmp(argv[i],"-silent")==0) { extern bool nullspu; nullspu=true; } // No sound
	#endif

		// WIZ
		if (strcmp(argv[i],"-ramtweaks")==0) { extern int wiz_ram_tweaks; wiz_ram_tweaks=1; } // RAM tweaks
		if (strcmp(argv[i],"-clock")==0) { wiz_clock=atoi(argv[i+1]); } // WIZ clock
		if (strcmp(argv[i],"-frontend")==0) { strcpy(frontend,argv[i+1]); } // Frontend
	}
	
	if (wiz_init(16, wiz_sound_rate, 16, wiz_sound_stereo) != 0) {
		printf("wiz_lib couldn't be initialized.\n");
		return -1;
	}
	
	SCREEN=fb1_16bit;
	
	if (psxInit() == -1) {
		printf("PSX emulator couldn't be initialized.\n");
		return -1;
	}

	if (LoadPlugins() == -1) {
		printf("Failed loading plugins.\n");
		return -1;
	}

	psxReset();	

	if (cdrfilename[0]!='\0') { if (CheckCdrom() == -1) { printf("Failed checking ISO image.\n"); SetIsoFile(NULL); }
	else { if (LoadCdrom() == -1) { printf("Failed loading ISO image.\n"); SetIsoFile(NULL); } } }
	if (filename[0]!='\0') { if (Load(filename) == -1) { printf("Failed loading executable.\n"); filename[0]='\0'; } }
	if (cdrfilename[0]!='\0') { printf("Running ISO image: %s.\n",cdrfilename); }
	if (filename[0]!='\0') { printf("Running executable: %s.\n",filename); }
	if ((cdrfilename[0]=='\0') && (filename[0]=='\0') && (Config.HLE==0)) { printf("Running BIOS.\n"); }

	if ((cdrfilename[0]!='\0') || (filename[0]!='\0') || (Config.HLE==0))
	{
		if (savename[0]) SaveState_filename=(char*)&savename;
#ifdef DEBUG_PCSX4ALL
		if (savename[0]) LoadState(savename); // Load save-state
#else
		if ((autosavestate) && (savename[0])) LoadState(savename); // Load save-state
#endif
		pcsx4all_prof_start(PCSX4ALL_PROF_CPU);
		psxCpu->Execute();
		pcsx4all_prof_end(PCSX4ALL_PROF_CPU);
	}

	pcsx4all_exit(); // exit
	
	return 0;
}

unsigned get_ticks(void)
{
	return clock();
}
void wait_ticks(unsigned s)
{
	unsigned target=clock()+s;
	while (clock()<target) spend_cycles(1024);
}

void port_printf(int x,int y,char *text)
{
	static const unsigned char fontdata8x8[] =
	{
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x3C,0x42,0x99,0xBD,0xBD,0x99,0x42,0x3C,0x3C,0x42,0x81,0x81,0x81,0x81,0x42,0x3C,
		0xFE,0x82,0x8A,0xD2,0xA2,0x82,0xFE,0x00,0xFE,0x82,0x82,0x82,0x82,0x82,0xFE,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x38,0x64,0x74,0x7C,0x38,0x00,0x00,
		0x80,0xC0,0xF0,0xFC,0xF0,0xC0,0x80,0x00,0x01,0x03,0x0F,0x3F,0x0F,0x03,0x01,0x00,
		0x18,0x3C,0x7E,0x18,0x7E,0x3C,0x18,0x00,0xEE,0xEE,0xEE,0xCC,0x00,0xCC,0xCC,0x00,
		0x00,0x00,0x30,0x68,0x78,0x30,0x00,0x00,0x00,0x38,0x64,0x74,0x7C,0x38,0x00,0x00,
		0x3C,0x66,0x7A,0x7A,0x7E,0x7E,0x3C,0x00,0x0E,0x3E,0x3A,0x22,0x26,0x6E,0xE4,0x40,
		0x18,0x3C,0x7E,0x3C,0x3C,0x3C,0x3C,0x00,0x3C,0x3C,0x3C,0x3C,0x7E,0x3C,0x18,0x00,
		0x08,0x7C,0x7E,0x7E,0x7C,0x08,0x00,0x00,0x10,0x3E,0x7E,0x7E,0x3E,0x10,0x00,0x00,
		0x58,0x2A,0xDC,0xC8,0xDC,0x2A,0x58,0x00,0x24,0x66,0xFF,0xFF,0x66,0x24,0x00,0x00,
		0x00,0x10,0x10,0x38,0x38,0x7C,0xFE,0x00,0xFE,0x7C,0x38,0x38,0x10,0x10,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1C,0x1C,0x1C,0x18,0x00,0x18,0x18,0x00,
		0x6C,0x6C,0x24,0x00,0x00,0x00,0x00,0x00,0x00,0x28,0x7C,0x28,0x7C,0x28,0x00,0x00,
		0x10,0x38,0x60,0x38,0x0C,0x78,0x10,0x00,0x40,0xA4,0x48,0x10,0x24,0x4A,0x04,0x00,
		0x18,0x34,0x18,0x3A,0x6C,0x66,0x3A,0x00,0x18,0x18,0x20,0x00,0x00,0x00,0x00,0x00,
		0x30,0x60,0x60,0x60,0x60,0x60,0x30,0x00,0x0C,0x06,0x06,0x06,0x06,0x06,0x0C,0x00,
		0x10,0x54,0x38,0x7C,0x38,0x54,0x10,0x00,0x00,0x18,0x18,0x7E,0x18,0x18,0x00,0x00,
		0x00,0x00,0x00,0x00,0x18,0x18,0x30,0x00,0x00,0x00,0x00,0x00,0x3E,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x18,0x18,0x00,0x00,0x00,0x04,0x08,0x10,0x20,0x40,0x00,0x00,
		0x38,0x4C,0xC6,0xC6,0xC6,0x64,0x38,0x00,0x18,0x38,0x18,0x18,0x18,0x18,0x7E,0x00,
		0x7C,0xC6,0x0E,0x3C,0x78,0xE0,0xFE,0x00,0x7E,0x0C,0x18,0x3C,0x06,0xC6,0x7C,0x00,
		0x1C,0x3C,0x6C,0xCC,0xFE,0x0C,0x0C,0x00,0xFC,0xC0,0xFC,0x06,0x06,0xC6,0x7C,0x00,
		0x3C,0x60,0xC0,0xFC,0xC6,0xC6,0x7C,0x00,0xFE,0xC6,0x0C,0x18,0x30,0x30,0x30,0x00,
		0x78,0xC4,0xE4,0x78,0x86,0x86,0x7C,0x00,0x7C,0xC6,0xC6,0x7E,0x06,0x0C,0x78,0x00,
		0x00,0x00,0x18,0x00,0x00,0x18,0x00,0x00,0x00,0x00,0x18,0x00,0x00,0x18,0x18,0x30,
		0x1C,0x38,0x70,0xE0,0x70,0x38,0x1C,0x00,0x00,0x7C,0x00,0x00,0x7C,0x00,0x00,0x00,
		0x70,0x38,0x1C,0x0E,0x1C,0x38,0x70,0x00,0x7C,0xC6,0xC6,0x1C,0x18,0x00,0x18,0x00,
		0x3C,0x42,0x99,0xA1,0xA5,0x99,0x42,0x3C,0x38,0x6C,0xC6,0xC6,0xFE,0xC6,0xC6,0x00,
		0xFC,0xC6,0xC6,0xFC,0xC6,0xC6,0xFC,0x00,0x3C,0x66,0xC0,0xC0,0xC0,0x66,0x3C,0x00,
		0xF8,0xCC,0xC6,0xC6,0xC6,0xCC,0xF8,0x00,0xFE,0xC0,0xC0,0xFC,0xC0,0xC0,0xFE,0x00,
		0xFE,0xC0,0xC0,0xFC,0xC0,0xC0,0xC0,0x00,0x3E,0x60,0xC0,0xCE,0xC6,0x66,0x3E,0x00,
		0xC6,0xC6,0xC6,0xFE,0xC6,0xC6,0xC6,0x00,0x7E,0x18,0x18,0x18,0x18,0x18,0x7E,0x00,
		0x06,0x06,0x06,0x06,0xC6,0xC6,0x7C,0x00,0xC6,0xCC,0xD8,0xF0,0xF8,0xDC,0xCE,0x00,
		0x60,0x60,0x60,0x60,0x60,0x60,0x7E,0x00,0xC6,0xEE,0xFE,0xFE,0xD6,0xC6,0xC6,0x00,
		0xC6,0xE6,0xF6,0xFE,0xDE,0xCE,0xC6,0x00,0x7C,0xC6,0xC6,0xC6,0xC6,0xC6,0x7C,0x00,
		0xFC,0xC6,0xC6,0xC6,0xFC,0xC0,0xC0,0x00,0x7C,0xC6,0xC6,0xC6,0xDE,0xCC,0x7A,0x00,
		0xFC,0xC6,0xC6,0xCE,0xF8,0xDC,0xCE,0x00,0x78,0xCC,0xC0,0x7C,0x06,0xC6,0x7C,0x00,
		0x7E,0x18,0x18,0x18,0x18,0x18,0x18,0x00,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0x7C,0x00,
		0xC6,0xC6,0xC6,0xEE,0x7C,0x38,0x10,0x00,0xC6,0xC6,0xD6,0xFE,0xFE,0xEE,0xC6,0x00,
		0xC6,0xEE,0x3C,0x38,0x7C,0xEE,0xC6,0x00,0x66,0x66,0x66,0x3C,0x18,0x18,0x18,0x00,
		0xFE,0x0E,0x1C,0x38,0x70,0xE0,0xFE,0x00,0x3C,0x30,0x30,0x30,0x30,0x30,0x3C,0x00,
		0x60,0x60,0x30,0x18,0x0C,0x06,0x06,0x00,0x3C,0x0C,0x0C,0x0C,0x0C,0x0C,0x3C,0x00,
		0x18,0x3C,0x66,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,
		0x30,0x30,0x18,0x00,0x00,0x00,0x00,0x00,0x00,0x3C,0x06,0x3E,0x66,0x66,0x3C,0x00,
		0x60,0x7C,0x66,0x66,0x66,0x66,0x7C,0x00,0x00,0x3C,0x66,0x60,0x60,0x66,0x3C,0x00,
		0x06,0x3E,0x66,0x66,0x66,0x66,0x3E,0x00,0x00,0x3C,0x66,0x66,0x7E,0x60,0x3C,0x00,
		0x1C,0x30,0x78,0x30,0x30,0x30,0x30,0x00,0x00,0x3E,0x66,0x66,0x66,0x3E,0x06,0x3C,
		0x60,0x7C,0x76,0x66,0x66,0x66,0x66,0x00,0x18,0x00,0x38,0x18,0x18,0x18,0x18,0x00,
		0x0C,0x00,0x1C,0x0C,0x0C,0x0C,0x0C,0x38,0x60,0x60,0x66,0x6C,0x78,0x6C,0x66,0x00,
		0x38,0x18,0x18,0x18,0x18,0x18,0x18,0x00,0x00,0xEC,0xFE,0xFE,0xFE,0xD6,0xC6,0x00,
		0x00,0x7C,0x76,0x66,0x66,0x66,0x66,0x00,0x00,0x3C,0x66,0x66,0x66,0x66,0x3C,0x00,
		0x00,0x7C,0x66,0x66,0x66,0x7C,0x60,0x60,0x00,0x3E,0x66,0x66,0x66,0x3E,0x06,0x06,
		0x00,0x7E,0x70,0x60,0x60,0x60,0x60,0x00,0x00,0x3C,0x60,0x3C,0x06,0x66,0x3C,0x00,
		0x30,0x78,0x30,0x30,0x30,0x30,0x1C,0x00,0x00,0x66,0x66,0x66,0x66,0x6E,0x3E,0x00,
		0x00,0x66,0x66,0x66,0x66,0x3C,0x18,0x00,0x00,0xC6,0xD6,0xFE,0xFE,0x7C,0x6C,0x00,
		0x00,0x66,0x3C,0x18,0x3C,0x66,0x66,0x00,0x00,0x66,0x66,0x66,0x66,0x3E,0x06,0x3C,
		0x00,0x7E,0x0C,0x18,0x30,0x60,0x7E,0x00,0x0E,0x18,0x0C,0x38,0x0C,0x18,0x0E,0x00,
		0x18,0x18,0x18,0x00,0x18,0x18,0x18,0x00,0x70,0x18,0x30,0x1C,0x30,0x18,0x70,0x00,
		0x00,0x00,0x76,0xDC,0x00,0x00,0x00,0x00,0x10,0x28,0x10,0x54,0xAA,0x44,0x00,0x00,
	};
	unsigned short *screen=(SCREEN+x+y*320);
	for (int i=0;i<strlen(text);i++) {
		
		for (int l=0;l<8;l++) {
			screen[l*320+0]=(fontdata8x8[((text[i])*8)+l]&0x80)?0xffff:0x0000;
			screen[l*320+1]=(fontdata8x8[((text[i])*8)+l]&0x40)?0xffff:0x0000;
			screen[l*320+2]=(fontdata8x8[((text[i])*8)+l]&0x20)?0xffff:0x0000;
			screen[l*320+3]=(fontdata8x8[((text[i])*8)+l]&0x10)?0xffff:0x0000;
			screen[l*320+4]=(fontdata8x8[((text[i])*8)+l]&0x08)?0xffff:0x0000;
			screen[l*320+5]=(fontdata8x8[((text[i])*8)+l]&0x04)?0xffff:0x0000;
			screen[l*320+6]=(fontdata8x8[((text[i])*8)+l]&0x02)?0xffff:0x0000;
			screen[l*320+7]=(fontdata8x8[((text[i])*8)+l]&0x01)?0xffff:0x0000;
		}
		screen+=8;
	} 
	
}

void port_sync(void)
{
	sync();
}

void port_mute(void)
{
	wiz_sound_thread_mute();
}