#if !defined(__picture_h)
#define __picture_h

#include <dc.h>
#include "colortab.h"
#include "framegbr.h"
#include "gdiobjec.h"

//*** CPicture ***
class CPicture {
public:
  long *bitmap;  //32 bit Fixkommazahlen. Um Wert zwischen 0 und 1 zu erhalten
                  //durch 64*65536 teilen.
                  //um aus [0..255] Skala zu kommen, mit 64*256 multiplizieren
                  //um aus [0..65535] Skala zu kommen mit 64 multiplizieren
  TBitmap *WinBitmap;
  //float **bitmap;
  int x,y,xh,yh,stepx,stepy,maxx,maxy,absorption;
  double EichX,EichY;
public:
  CPicture(int ax,int ay, int axh, int ayh, int astepx, int astepy,
	 int aabsorption,double aEichX, double aEichY);
  CPicture(const CPicture &pic);
  ~CPicture();
  void MakeWinBitmap();
  void Copy(const CPicture &pic);
  void ReadPicture(int quadrant, CFrameGrabber &framegrabber,double xmin, double xmax, double ymin, double ymax);
  int Save(const char* name,CColorTable *ColorTab);
  int Load(const char* name);
  void MandelPicture();
  void GaussPicture();
  unsigned char Mandel(int x,int y);
  void SearchMaxXMaxY(double axmin=0, double axmax=1, double aymin=0, double aymax=1);
  double MaxX();
  double MaxY();
  void SetMaxXMaxY(double aMaxX, double aMaxY);
  unsigned char GetPixel(double x, double y);
  double GetEichPixel(double x, double y);
  void InitBitmap(int axh,int ayh);
  void DeleteBitmap();
  friend CPicture operator-(const CPicture& pic1, const CPicture& pic2);
  friend CPicture operator+(const CPicture& pic1, const CPicture& pic2);
  friend CPicture operator*(const CPicture& pic1, const CPicture& pic2);
  friend CPicture operator/(const CPicture& pic1, const CPicture& pic2);
  CPicture& operator=(const CPicture& pic);
  const CPicture operator*(const double r) const;
  void NormalizeAbsorption(const CPicture &pic1,const CPicture &pic2,const CPicture &pic3,double xmin, double xmax, double ymin, double ymax);
  void Ln(const CPicture &pic1,const CPicture &pic2, const CPicture &pic3,double MaxDensity,double ProbeDetuning,double xmin, double xmax, double ymin, double ymax);
  friend void LnNormalizeAbsorption(const CPicture &pic1,const CPicture &pic2,const CPicture &pic3,CPicture &Abs,CPicture &Ln,
	 double MaxDensity,double ProbeDetuning,double xmin, double xmax, double ymin, double ymax);
  void GetList(int horiz, double xpos, double ypos, double pos1, double pos2, double * &x,double * &y, int &ndata);
  void GetSigma(int horiz, double xpos, double ypos, double &sigma, double &height);
  void SetEich(double aEichX, double aEichY)
	 { EichX=aEichX; EichY=aEichY;}
  double GetEichPosX(double xpos);
  double GetEichPosY(double ypos);
  void CalcNormalizedAmpX0SigmaX(double &AmpX, double &X0, double &SigmaX, double &Offset);
  void CalcNormalizedAmpY0SigmaY(double &AmpY, double &Y0, double &SigmaY, double &Offset);
  void GetProfil(int * &profilbuf,double pos,
			double &Eich,int horiz,int &length);
  void GetSum(int * &profilbuf,double pos1, double pos2,
			double &Eich,int horiz,int &length);
  double GetAbsorption(double x0,double y0);
  void CalcNormalizedX0Y0(double &X0, double &Y0);
  double Integral(double xmin, double ymin, double xmax, double ymax);
  int PictureGood();
private:
  int GetBitmapPosX(double xpos);
  int GetBitmapPosY(double ypos);
};

#endif