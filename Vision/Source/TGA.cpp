#include <stdio.h>
#include <io.h>
#include <assert.h>
#include <stdlib.h>
#include <alloc.h>
#include <string.h>
#include <conio.h>
#include <ctype.h>
#include <dos.h>
#include <dir.h>
#include "TGA.h"
#include "leakdetect.h"

WriteTGAFile::WriteTGAFile(const char* name,long int width,long int height, bool atruecolor,TColor* apalette) {
  struct diskfree_t free;
  double avail;
  palette=apalette;
  truecolor=atruecolor;
  maxpos=width;
  zeile=NULL;
  if (_dos_getdiskfree(0, &free)!=0) avail=0; else
  avail=(double) free.avail_clusters
		  * (double) free.bytes_per_sector
		  * (double) free.sectors_per_cluster;
  long int filesize;
  if (truecolor) filesize=width*height*3+18;
  else filesize=width*height*2+18;
  if (avail<filesize) {
  	open=0;
   MessageBox(GetActiveWindow(),"WriteTGAFile::WriteTGAFile : harddisk full","Error",  MB_OK);
  } else {
	 open=1;
	 stream=fopen(name,"wb");
    putc(0,stream);
    putc(0,stream);
    if (truecolor) putc(2,stream);               /* uncompressed Color */
    else putc(3,stream);                         /* uncompressed BW */
    putc(0,stream); putc(0,stream);
    putc(0,stream); putc(0,stream);   putc(0,stream);
    putc(0,stream); putc(0,stream);           /* X origin */
    putc(0,stream); putc(0,stream);           /* y origin */
    putc((width & 0x00FF),stream);
    putc((width & 0xFF00) / 256,stream);
    putc((height & 0x00FF),stream);
    putc((height & 0xFF00) / 256,stream);
    if (truecolor) putc(24,stream);              /* 24 bit bitmap */
    else putc(16,stream);                        /* 16 bit bitmap */
    putc(0,stream);

	 if (truecolor) zeile=new unsigned char[3*width];
	 else zeile=new unsigned char[2*width];
    #if defined(DetectLeaks)
      LeakDetect.New(zeile,10,1);
    #endif
	 pos=0;
  }
}

WriteTGAFile::~WriteTGAFile() {
  #if defined(DetectLeaks)
     LeakDetect.Delete(zeile);
  #endif
  if (zeile!=NULL) delete zeile;
  zeile=NULL;
  if (open) fclose(stream);
}

void WriteTGAFile::PutPixel(unsigned long col) {
  if (zeile) {
	  if (pos<maxpos) {
   	 if (truecolor) {
      	if (col<=0xFFFF) {
	        zeile[pos*3]=palette[col].Blue();
   	     zeile[pos*3+1]=palette[col].Green();
      	  zeile[pos*3+2]=palette[col].Red();
	      } else zeile[pos*3]=zeile[pos*3+1]=zeile[pos*3+2]=0;
   	 } else {
      	zeile[pos*2]=(col & 0x00FF);
		   zeile[pos*2+1]=(col & 0xFF00) / 256 ;
   	 }
		 pos++;
	  }
  }
}

void WriteTGAFile::WriteLine() {
  if ((open) && (zeile)) {
    if (truecolor) fwrite(zeile,1,maxpos*3,stream);
    else fwrite(zeile,1,maxpos*2,stream);
    pos=0;
  }
}

//*** TGAZeile ***//
ReadTGAFile::ReadTGAFile(const char *filename) {
  offen=0;
  zeile=NULL;
  position=0;
  bild=fopen(filename,"rb");
  if (bild) {
	 offen=1;
    bool error;
    error=false;
    error|=getc(bild)!=0;
    error|=getc(bild)!=0;
    error|=getc(bild)!=3;
    for (int i=0;i<9;i++) error|=getc(bild)!=0;
    breite=getc(bild)+getc(bild)*256;
    hoehe=getc(bild)+getc(bild)*256;
    error|=getc(bild)!=16;
    error|=getc(bild)!=0;
    error|=breite<=0;
    error|=hoehe<=0;
    if (!error) {
      #if defined(DetectLeaks)
        LeakDetect.Delete(zeile);
      #endif
      if (zeile) delete zeile;
	   zeile=new unsigned char[2*breite];
      #if defined(DetectLeaks)
        LeakDetect.New(zeile,10,1);
      #endif
		HoleZeile();
	 } else {
		fclose(bild);
		offen=0;
	 }
  }
};

ReadTGAFile::~ReadTGAFile(void) {
  if (offen) fclose(bild);
  offen=0;
  #if defined(DetectLeaks)
     LeakDetect.Delete(zeile);
  #endif
  if (zeile) delete zeile;
  zeile=NULL;
}

void ReadTGAFile::HoleZeile(void) {
  if (zeile) {
    if (offen) fread(zeile,1,2*breite,bild);
    else for (int i=0;i<2*breite;i++) zeile[i]=0;
  }
  position++;
}

int ReadTGAFile::eof() {
  return (position>hoehe);
}


