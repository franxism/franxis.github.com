# devkitadv base dir
export CCBASE=C:/GP32iDE/devkitadv/
# User options passed to the compiler
export CUSER=-DLITTLE_ENDIAN -DGP32 -DLSB_FIRST -fsigned-char -funroll-loops -fomit-frame-pointer -O3 -W -Wall -ansi -pedantic
include $(CCBASE)/gp32.mk
#------------------------------

all: gpmain.fxe

codecb.o: codecb.c
codedd.o: codedd.c
codeed.o: codeed.c
codefd.o: codefd.c
common.o: common.c roms.h vidhrdw.h
debug.o:  debug.c
machine.o: machine.c memmap.h roms.h
gp32.o: gp32.c
gp32_menu.o: gp32_menu.c
multipac.o: multipac.c
roms.o: roms.c roms.h
sndhrdw.o: sndhrdw.c
vidhrdw.o: vidhrdw.c memmap.h roms.h vidhrdw.h
gpsoundbuf.o: gpsoundbuf.c gpsoundbuf.h
z80.o: z80.c

gpmain.elf: multipac.o machine.o vidhrdw.o sndhrdw.o roms.o common.o gp32.o \
         z80.o codecb.o codedd.o codeed.o codefd.o gpsoundbuf.o gp32_menu.o
	$(LINK)

gpmain.fxe: gpmain.gxb
	b2fxe -t "MultiPac GP32 1.0 (133 MHz)" -b "icon.bmp" -a "Franxis" -r "PD" gpmain.gxb gpmain.fxe


clean:
	del gpmain.gxb gpmain.fxe gpmain.elf gpmain.o
