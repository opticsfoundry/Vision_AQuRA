#if !defined(__TGA_h)
#define __TGA_h

#include <stdio.h>
#include <color.h>

//*** WriteTGAFile ***//
class WriteTGAFile {
  int pos,maxpos;
  unsigned char *zeile;
  FILE *stream;
  int open;
  bool truecolor;
  TColor *palette;
public:
  WriteTGAFile(const char* name,long int width,long int height, bool atruecolor,TColor* apalette);
  ~WriteTGAFile();
  void PutPixel(unsigned long col);
  void WriteLine();
  int Open() {return open;};
};

//*** ReadTGAFile ***//
class ReadTGAFile {
  FILE* bild;
  int hoehe,breite,position;
  unsigned char *zeile;
  int offen;
public:
  ReadTGAFile(const char *filename);
  ~ReadTGAFile(void);
  int eof(void);
  void HoleZeile(void);
  unsigned long getPixel(int pos) {unsigned long h=zeile[pos*2+1];h=h*256+zeile[pos*2];return h;}
  int getBreite() {return breite;}
  int getHoehe() {return hoehe;}
  int Open() {return offen;};
};

#endif
