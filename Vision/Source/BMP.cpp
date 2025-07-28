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
#include "bmp.h"

WriteBMPFile::WriteBMPFile(const char* name,long int width, long int height,long int colors,TColor *palette) {
  struct diskfree_t free;
  double avail;
  zeile=NULL;
  if (_dos_getdiskfree(0, &free)!=0) avail=0;
  else avail=(double) free.avail_clusters
		  * (double) free.bytes_per_sector
		  * (double) free.sectors_per_cluster;
  long int filesize,datasize;
  long int dataoffset,infoheaderlen,longbuffer;
  short int nullint=0;
  modwidth = ( 4 - width % 4 )%4;
  datasize=(width+modwidth)*height;
  dataoffset=14+40+colors*4;
  filesize=datasize+dataoffset;
  if (avail<filesize) {
  	open=0;
   MessageBox(GetActiveWindow(),"WriteBMPFile::WriteBMPFile : harddisk full","Error",  MB_OK);
   } else {
	 open=1;
	 stream=fopen(name,"wb");
	 fprintf(stream,"BM");
	 fwrite(&filesize,sizeof(long int),1, stream);
	 fwrite(&nullint,sizeof(short int),1, stream);
	 fwrite(&nullint,sizeof(short int),1, stream);
	 fwrite(&dataoffset,sizeof(long int),1, stream);
	 infoheaderlen=40;
	 fwrite(&infoheaderlen,sizeof(long int),1, stream);
	 fwrite(&width,sizeof(long int),1, stream);
	 fwrite(&height,sizeof(long int),1, stream);
	 nullint=1;
	 fwrite(&nullint,sizeof(short int),1, stream);
	 nullint=8;
	 fwrite(&nullint,sizeof(short int),1, stream);
	 nullint=0;
	 longbuffer=0;
	 fwrite(&longbuffer,sizeof(long int),1, stream);
	 fwrite(&datasize,sizeof(long int),1, stream);
	 longbuffer=0;
	 fwrite(&longbuffer,sizeof(long int),1, stream);
	 fwrite(&longbuffer,sizeof(long int),1, stream);
	 fwrite(&colors,sizeof(long int),1, stream);
	 fwrite(&colors,sizeof(long int),1, stream);
	 for (int i=0;i<colors;i++) {
		unsigned char red=palette[i].Red();
		unsigned char green=palette[i].Green();
		unsigned char blue=palette[i].Blue();
		unsigned char nullchar=0;
		fwrite(&blue,1,1,stream);
		fwrite(&green,1,1,stream);
		fwrite(&red,1,1,stream);
		fwrite(&nullchar,1,1,stream);
	 }
	 zeile=(unsigned char *)malloc(width);
	 pos=0;
	 maxpos=width;
  }
}

WriteBMPFile::~WriteBMPFile() {
  if (zeile!=NULL) free(zeile);
  if (open) fclose(stream);
}

void WriteBMPFile::Pixel(unsigned char col) {
  if (pos<maxpos) {
	 zeile[pos]=col;
	 pos++;
  }
}

void WriteBMPFile::WriteLine() {
  if (open) fwrite(zeile,1,maxpos,stream);
  ZeileAuffuellen();
}

void WriteBMPFile::NewLine() {
  pos=0;
}

void WriteBMPFile::ZeileAuffuellen() {
  unsigned char nullbyte=0;
  int x;
  if (open) for (x=0;x<modwidth;x++)  fwrite(&nullbyte,sizeof(char),1,stream);
}


//*** BMPZeile ***//
ReadBMPFile::ReadBMPFile(const char *filename,TColor *palette) {
  unsigned char *headerbuffer;
  char string[256];
  long int longbuffer[1];
  long int *lwidth, *lheight, *pcolors;
  TColor *newpalette;

  position=0;
  offen=0;
  bild=fopen(filename,"rb");
  ok=0;
  if (bild) {
	 offen=1;
    ok=1;
	 fread(string,1,10,bild);
	 if (strncmp(string,"BM",2)==0) {
		fread(longbuffer,sizeof(long int),1,bild);
		headerbuffer=(unsigned char*)malloc(*longbuffer);
		fread(headerbuffer,1,(size_t)40/*(*longbuffer)-14-1024*/,bild);
		pcolors=(long int *)(headerbuffer+32);
		colors=*pcolors;
		for (int i=0;i<colors;i++) {
		  unsigned char red;
		  unsigned char green;
		  unsigned char blue;
		  unsigned char nullchar;
		  fread(&blue,1,1,bild);
		  fread(&green,1,1,bild);
		  fread(&red,1,1,bild);
		  fread(&nullchar,1,1,bild);
		  if (i<256) palette[i]=TColor(red,green,blue);
		}
		lwidth=(long int *)(headerbuffer+4);
		lheight=(long int *)(headerbuffer+8);
		breite = (int)(*lwidth);
		hoehe = (int)(*lheight);
		modweite = ( 4 - breite % 4 )%4;
		free(headerbuffer);
		zeile=(unsigned char *)malloc(breite+modweite);
		if ((double(breite)+double(modweite))*double(hoehe)<65000.0) {
		  puffer=(unsigned char *)malloc((breite+modweite)*hoehe);
		  fread(puffer,1,(breite+modweite)*hoehe,bild);
		  fclose(bild);
		  offen=0;
		}
		HoleZeile();
	 } else {
		fclose(bild);
		offen=0;
	 }
  }
};

ReadBMPFile::~ReadBMPFile(void) {
  if (offen) fclose(bild);
  offen=0;
  free(zeile);
}

void ReadBMPFile::HoleZeile(void) {
  if (offen) fread(zeile,1,breite+modweite,bild);
  else for (int i=0;i<breite;i++) zeile[i]=puffer[position*(breite+modweite)+i];
  position++;
}

int ReadBMPFile::eof() {
  return (position>hoehe);
}


