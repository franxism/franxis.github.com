# devkitadv base dir
export CCBASE=C:/GP32iDE/devkitadv/
# User options passed to the compiler
export CUSER=-DLITTLE_ENDIAN -DGP32 -W -Wall -ansi -pedantic
include $(CCBASE)/gp32.mk
#------------------------------

all: gpmain.fxe

gpmain.o: gpmain.c

gpmain.elf: gpmain.o libmodplay.a
	$(LINK)

gpmain.fxe: gpmain.gxb
	b2fxe -t "Columns GP32" -b "icon.bmp" -a "Franxis" -r "PD" gpmain.gxb gpmain.fxe


clean:
	del gpmain.gxb gpmain.fxe gpmain.elf gpmain.o
