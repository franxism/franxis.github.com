#include "defines.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "gpdef.h"
#include "gpstdio.h"
#include "gpstdlib.h"
#include "gpgraphic.h"
#include "gpfont.h"
#include "gpmem.h"
#include "gp32_mame.h"

BOOL gp32_fexists ( const char * filename )
  {
    GPFILEATTR attr;
    
    if (GpFileAttr(filename, &attr)!=SM_OK)
      return FALSE;
    
    return TRUE;
  }

GP32_FILE * gp32_fopen ( const char * filename, const char * mode )
  {
    GP32_FILE * fh;
    F_HANDLE gpfh;
    unsigned long m = 0;
    BOOL append=FALSE;
    BOOL change=FALSE;

    if (mode[0]=='\0')
      return NULL;
    else
    if (tolower(mode[0])=='r')
      m |= OPEN_R;
    else
    if (tolower(mode[0])=='w')
      m |= OPEN_W;
    else
    if (tolower(mode[0])=='a')
      append=TRUE;
    else
      return NULL;

    if (mode[1]!='\0')
      {
        if (mode[1]=='+')
          change=TRUE;
      }

    if (m&OPEN_W) /* W */
      {
        /* Flush file */
        if (GpFileCreate(filename, ALWAYS_CREATE, &gpfh)!=SM_OK)
          return NULL;
      } else
    if (append)   /* A */
      {
        if (gp32_fexists(filename))
          {
            long s;

            if (GpFileOpen(filename, OPEN_R|(change?OPEN_W:0), &gpfh)!=SM_OK)
              return NULL;
            GpFileSeek ( gpfh, FROM_END, 0, &s );
          } else
          {
            if (GpFileCreate(filename, ALWAYS_CREATE, &gpfh)!=SM_OK)
              return NULL;
          }
      } else      /* R */
    if (m&OPEN_R)
      {
        ERR_CODE err =GpFileOpen(filename, m|(change?OPEN_W:0), &gpfh);
        if (err!=SM_OK)
          return NULL;
      } else
      return NULL;

    if ( (fh = gm_zi_malloc ( sizeof(GP32_FILE) ))==NULL )
      {
        GpFileClose ( gpfh );
        return NULL;
      }
    
    *fh = gpfh;

    return fh;
  }

int gp32_fclose ( GP32_FILE * fh )
  {
    if (fh==NULL)
      return EOF;
    
    if (GpFileClose(*fh)!=SM_OK)
      return EOF;
    gm_free(fh);
    return 0;
  }

int gp32_getc ( GP32_FILE * fh )
  {
    unsigned char b;
    unsigned long r;

    if (fh==NULL)
      return EOF;
    
    if (GpFileRead(*fh, &b, 1, &r)!=SM_OK)
      return EOF;
    
    if (r!=1)
      return EOF;
    
    return b;
  }

int gp32_fgetc ( GP32_FILE * fh )
  {
    return gp32_getc(fh);
  }

int gp32_putc ( int c, GP32_FILE * fh )
  {
    unsigned char b = c;

    if (fh==NULL)
      return EOF;
    
    if (GpFileWrite(*fh, &b, 1)!=SM_OK)
      return EOF;
    
    return c;
  }

int gp32_fputc ( int c, GP32_FILE * fh )
  {
    return (gp32_putc(c,fh));
  }

long gp32_ftell ( GP32_FILE * fh )
  {
    long res;
    
    if (fh==NULL)
      return -1;
      
    if (GpFileSeek(*fh, FROM_CURRENT, 0, &res)!=SM_OK)
      return -1;
    
    return res;
  }

int gp32_fseek ( GP32_FILE * fh, long offset, int origin )
  {
    long old;
    long mode;

    if (fh==NULL)
      return -1;
    
    if (origin==SEEK_SET)
      mode = FROM_BEGIN;
    else
    if (origin==SEEK_CUR)
      mode = FROM_CURRENT;
    else
    if (origin==SEEK_END)
      {
        mode = FROM_END;
        offset = -offset;
      }
    else
      return -1;

    if (GpFileSeek(*fh, mode, offset, &old)!=SM_OK)
      return -1;
     
    return 0;
  }

size_t gp32_fread ( void * ptr, size_t size, size_t nobj, GP32_FILE * fh )
  {
    unsigned int obj;
    unsigned int ofs;
    
    if (nobj==0 || size==0)
      return 0;
      
    if (fh==NULL)
      return 0;
    
    for (obj=0,ofs=0;obj<nobj;obj++,ofs+=size)
      {
        unsigned long r;
        ERR_CODE err;
        err=GpFileRead(*fh, ((u8*)ptr)+ofs, size, &r);
        if (err!=SM_OK || r!=size)
          return obj;
      }
    return obj+1;
  }

size_t gp32_fwrite ( const void * ptr, size_t size, size_t nobj, GP32_FILE * fh )
  {
    int obj,ofs;

    if (fh==NULL)
      return 0;
    
    if (nobj==0 || size==0)
      return 0;
    
    for (obj=0,ofs=0;obj<nobj;obj++,ofs+=size)
      {
        ERR_CODE err;
        err=GpFileWrite ( *fh, (void*)(((u8*)ptr)+ofs), size );
        if (err!=SM_OK)
          return obj;
      }
    return obj+1;
  }

void gp32_rewind ( GP32_FILE * fh )
  {
    gp32_fseek ( fh, 0, SEEK_SET );
  }
