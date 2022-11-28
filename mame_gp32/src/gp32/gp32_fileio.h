#ifndef GP32_FILEIO_H
#define GP32_FILEIO_H

#include "defines.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "gpstdio.h"

typedef long GP32_FILE;

BOOL gp32_fexists ( const char * filename );
GP32_FILE * gp32_fopen ( const char * filename, const char * mode );
int gp32_fclose ( GP32_FILE * fh );
int gp32_getc ( GP32_FILE * fh );
int gp32_fgetc ( GP32_FILE * fh );
int gp32_putc ( int c, GP32_FILE * fh );
int gp32_fputc ( int c, GP32_FILE * fh );
long gp32_ftell ( GP32_FILE * fh );
int gp32_fseek ( GP32_FILE * fh, long offset, int origin );
size_t gp32_fread ( void * ptr, size_t size, size_t nobj, GP32_FILE * fh );
size_t gp32_fwrite ( const void * ptr, size_t size, size_t nobj, GP32_FILE * fh );
void gp32_rewind ( GP32_FILE * fh );

#endif
