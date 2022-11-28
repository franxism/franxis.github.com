#include "driver.h"
#include "gpsoundbuf.h"

/* audio related stuff */
#define NUMVOICES 16
#define SAMPLE_RATE 22050
#define SAMPLE_NUMBER 1600

GPSOUNDBUF sb;

struct lpWaves {
	signed char lpData[SAMPLE_NUMBER];
	int len; /* length of sound buffer */
	int freq; /* frequency of sound buffer */
	int active; /* 1 active, 0 inactive */
	int volume; /* volume 0 - 64 */
	int loop; /* loop 1 yes, 0 no */
	unsigned int pos; /* position into the sample */
	unsigned int step; /* step of the sample */
};
static struct lpWaves lpWave[NUMVOICES];
int sound_enable = 0;
int soundcard;
static int attenuation = 0;
static int mixer_count = 0;
static int mixer_count_ok = 0;
static int nominal_sample_rate;
static int chn_max=0; /* Max Number of channels */

void gp32_sound_mixer ( void * userdata, unsigned char * stream, int len )
{
	register int channel;
	register int i;
	signed long mix;
	int t;

	/* Sound Enable ? */	
	if (!sound_enable) { 
		/* Buffer Set to Zero */
		for (i=0;i<len;i++) 
			stream[i]=0x80;
		/*gm_memset ( stream, 0x80, len );*/
		return;
	}
	
	/* Initialize position in origin samples to 0 */
	t=0; /* Calculate max channels */
	for (channel=0;channel<NUMVOICES;channel++)
  	{
		/* Channel active ? */
		if (!lpWave[channel].active)
			continue;
			
		/* initial position of the sample */
		lpWave[channel].pos=0;
		
		/* Calculate step of the sample */
		if(lpWave[channel].freq>20000 && lpWave[channel].freq<25000) {
			/* Sound Updated ? */
			if(mixer_count==mixer_count_ok) return;
			lpWave[channel].step=0; /* pos++ */
		} else {
			lpWave[channel].step=1;
		}
			
		t++;
  	}
	if (t==0) return;
	if (t>chn_max)
		chn_max=t;

	/* Fill Sound Buffer */
	for(i=0;i<len;i++) {
	
		/* Initialize value of mixed sample to 0 */
		mix=0;
		
		/* For each of the channels */
		for (channel=0;channel<NUMVOICES;channel++) {

			/* Channel active ? */
			if (!lpWave[channel].active)
				continue;
		
  			if (!lpWave[channel].step) {
	  			/* Update sample mix */
  				mix+=(lpWave[channel].lpData[lpWave[channel].pos]*lpWave[channel].volume)>>6;
				/* Increase sample position */
				lpWave[channel].pos++;
				if (lpWave[channel].pos==lpWave[channel].len-1)
					lpWave[channel].pos=0;
  			} else {
				/* Update sample position*/
				lpWave[channel].pos = (i*lpWave[channel].freq)/SAMPLE_RATE;
				if (lpWave[channel].pos>lpWave[channel].len)
					lpWave[channel].pos%=lpWave[channel].len;
				else if (lpWave[channel].pos==lpWave[channel].len)
					lpWave[channel].pos=0;
	  			/* Update sample mix */
	  			mix+=(lpWave[channel].lpData[lpWave[channel].pos]*lpWave[channel].volume)>>6;
			}

		}
		
		/* Scale mixed sample and assign */
		/*stream[i]= (mix/chn_max)^0x80;*/
		switch (chn_max) {
			case 1: stream[i]=mix^0x80; break;
			case 8: stream[i]=(mix>>3)^0x80; break;
			case 4: stream[i]=(mix>>2)^0x80; break;
			case 2: stream[i]=(mix>>1)^0x80; break;
			case 16: stream[i]=(mix>>4)^0x80; break;
			default: stream[i]= (mix/chn_max)^0x80;	break;
		}
	
	}
	
	/* Update status */
	for (channel=0;channel<NUMVOICES;channel++) {

		/* Channel active ? */
		if (!lpWave[channel].active)
			continue;

		/* If no loop -> Not active */
		if (!lpWave[channel].loop) {
			lpWave[channel].active=0;
		}
		
	}
	
	/* Sound Updated ! */
	mixer_count_ok = mixer_count;
}

int msdos_init_seal (void)
{
   	sb.freq = PCM_M22;
	sb.format = PCM_8BIT;
	sb.samples = SAMPLE_RATE/60;
	/*sb.samples = SAMPLE_RATE/50;*/
	sb.userdata = NULL;
	sb.callback = gp32_sound_mixer;
	/*sb.pollfreq = 60;*/
	/*sb.pollfreq = 5000/60;*/
	sb.pollfreq = 120;
	GpPcmInit(PCM_M22,PCM_8BIT);
	return 0;
}

int msdos_init_sound(void)
{
	int i;
	sound_enable=0;

	/* Initialize Sound Buffer Structure */
	for (i = 0; i < NUMVOICES; i++)
	{
		lpWave[i].len = 0;
		lpWave[i].freq = 0;
		lpWave[i].active = 0;
		lpWave[i].volume = 0;
		lpWave[i].loop = 0;
		lpWave[i].pos = 0;
	}
	mixer_count = 0;
	mixer_count_ok = -1;
	chn_max=0;

	if (soundcard == 0)     /* Sound Deactivated and not emulated */
	{
		gp32_text_log("GP32 Not Emulated Sound Init...");
		/* update the Machine structure to show that sound is disabled */
		Machine->sample_rate = 0;
		return 0;
	}

	if (soundcard == 2) /* Sound Deactivated but emulated */
	{
		gp32_text_log("GP32 No Sound Init...");
		return 0;
	}

	gp32_text_log("GP32 Sound Init...");
	sound_enable = 1;

	/* update the Machine structure to reflect the actual sample rate */
	nominal_sample_rate = Machine->sample_rate;
	Machine->sample_rate = SAMPLE_RATE;
	GpSoundBufStart(&sb);

	return 0;
}

void msdos_shutdown_sound(void)
{
	sound_enable = 0;
	
	if (Machine->sample_rate != 0)
	{
		int i;
		GpSoundBufStop();

		/* stop and release voices */
		for (i = 0; i < NUMVOICES; i++)
		{
			lpWave[i].len = 0;
			lpWave[i].freq = 0;
			lpWave[i].active = 0;
			lpWave[i].volume = 0;
			lpWave[i].loop = 0;
			lpWave[i].pos = 0;
		}
	}

	mixer_count = 0;
	mixer_count_ok = -1;
	chn_max=0;
}


void osd_update_audio(void)
{

	/* Sound Enable ? */
	if (Machine->sample_rate == 0) return;

	/* Sound updated ? */
	if (mixer_count!=mixer_count_ok) return;

	/* Sound updated ! */
	mixer_count++;
	mixer_count=mixer_count & 0xff;
	/*mixer_count=mixer_count%256;*/
}



static void playsample(int channel,signed char *data,int len,int freq,int volume,int loop, int pan, int bits)
{
	int i;
	if (Machine->sample_rate == 0 || channel >= NUMVOICES) return;

	if (len<1 || freq < 16 || volume < 4) {
		/*lpWave[channel].active=0;*/
		return;
	}

	lpWave[channel].active=0;

	/* backwards compatibility with old 0-255 volume range */
	if (volume > 100) volume = volume * 25 / 255;

	if (volume > 0) {
		volume = (volume<<6)/100;
		if (pan != OSD_PAN_CENTER) volume=volume>>1;
		if (volume>0) volume+=20;
		if (volume>64) volume=64;
	}

	lpWave[channel].volume=volume;

	if (bits==8) {
		for (i=0;i<len;i++)
			lpWave[channel].lpData[i]= data[i];
		lpWave[channel].len=len;
	} else {
		for (i=0;i<len;i++) {
			i++; /* ignore low 8 bits */
			lpWave[channel].lpData[i]= data[i];
		}
		lpWave[channel].len=len>>1;
	}

	lpWave[channel].freq=freq;
	lpWave[channel].loop=loop;
	lpWave[channel].active=1;
}

void osd_play_sample(int channel,signed char *data,int len,int freq,int volume,int loop)
{
	playsample(channel,data,len,freq,volume,loop,OSD_PAN_CENTER,8);
}

void osd_play_sample_16(int channel,signed short *data,int len,int freq,int volume,int loop)
{
	playsample(channel,(signed char *)data,len,freq,volume,loop,OSD_PAN_CENTER,16);
}

void osd_play_streamed_sample(int channel,signed char *data,int len,int freq,int volume,int pan)
{
	playsample(channel,data,len,freq,volume,1,pan,8);
}

void osd_play_streamed_sample_16(int channel,signed short *data,int len,int freq,int volume,int pan)
{
	playsample(channel,(signed char *)data,len,freq,volume,1,pan,16);
}


void osd_adjust_sample(int channel,int freq,int volume)
{
	
	if (Machine->sample_rate == 0 || channel >= NUMVOICES) return;

	/* backwards compatibility with old 0-255 volume range */
	if (volume > 100) volume = volume * 25 / 255;

	if (freq != -1)
		lpWave[channel].freq=freq;
	if (volume != -1) {
		volume = (volume<<6)/100;
		if (volume>0) volume+=20;
		if (volume>64) volume=64;
		lpWave[channel].volume = volume;
	}
}


void osd_stop_sample(int channel)
{
	if (Machine->sample_rate == 0 || channel >= NUMVOICES) return;
	lpWave[channel].active = 0;
}


void osd_restart_sample(int channel)
{
	if (Machine->sample_rate == 0 || channel >= NUMVOICES) return;
	lpWave[channel].active = 1;
}


int osd_get_sample_status(int channel)
{
	if (Machine->sample_rate == 0 || channel >= NUMVOICES) return -1;
	return !lpWave[channel].active;
}


void osd_ym2203_write(int n, int r, int v)
{
	return;
}


void osd_ym2203_update(void)
{
      return;
}

int osd_get_mastervolume(void)
{
	return attenuation;
}

void osd_sound_enable(int enable_it)
{
	sound_enable = enable_it;
}

static void tenmicrosec(void)
{
	return;
}

void osd_ym3812_control(int reg)
{
	return;
}

void osd_ym3812_write(int data)
{
	return;
}
