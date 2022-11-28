int sh_init(const char *gamename);
int sh_start(void);
void sh_stop(void);
int sh_wrmem(dword A,byte V);
int sh_doout(byte A,byte V);
int sh_doin(byte A,byte *V);
void sh_update(void);
