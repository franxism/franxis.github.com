MultiPac GP32 Port 1.0 Source Code (15/04/2005)
-----------------------------------------------

http://www.talfi.net/gp32_franxis/

Development Environment: GP32iDE + DevKitAdvance

Please copy gpdef.h to next DevkitAdvance folder: arm-agb-elf\include\gp32
before compiling. I have commented next #defines: byte,word,dword.
You should modify Makefile and project.gpj to compile on yor computer also.

There are a lot of improvements that could be implemented in current MultiPac
version, but i'm now occupied in my MAME port. If someone want to continue the
work ;-)...

In example: Sound could be easily improved, and screen update can be optimized.
This could lead to only one FXE file at 133 MHz at 100% speed.

Thanks and regards.

- Franxis
