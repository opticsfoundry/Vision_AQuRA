#if !defined(__bmp_h)
#define __bmp_h

#include <stdio.h>
#include <color.h>

//*** WriteBMPFile ***//
class WriteBMPFile {
  int modwidth,pos,maxpos;
  unsigned char *zeile;
  FILE *stream;
  int open;
public:
  WriteBMPFile(const char* name,long int width, long int height,long int colors,TColor* palette);
  ~WriteBMPFile();
  void Pixel(unsigned char col);
  void WriteLine();
  void NewLine();
  void ZeileAuffuellen();
  int Open() {return open;};
};

//*** ReadBMPFile ***//
class ReadBMPFile {
  FILE* bild;
  int hoehe,breite,modweite,position;
  unsigned char *zeile;
  unsigned char *puffer;
  long int colors;
  int offen,ok;
public:
  ReadBMPFile(const char *filename,TColor *palette);
  ~ReadBMPFile(void);
  int eof(void);
  void HoleZeile(void);
  unsigned char getPixel(int pos) {return zeile[pos];}
  int getBreite() {return breite;}
  int getHoehe() {return hoehe;}
  long int getColors() {return colors;}
  int Ok() {return ok;};
};

#endif
