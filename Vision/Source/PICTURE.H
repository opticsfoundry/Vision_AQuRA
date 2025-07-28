#if !defined(__picture_h)
#define __picture_h

#include <dc.h>
#include "colortab.h"
#include "framegbr.h"
#include "gdiobjec.h"

#define SAVE_AUTOMATIC 0
#define SAVE_PALETTEBMP 1
#define SAVE_LSBPALETTEBMP 2
#define SAVE_GRAYSCALETGA 3
#define SAVE_TRUECOLORTGA 4
#define SAVE_MATRIX 5


#define SHIFT 1024

//*** CPicture ***
class CPicture {
public:
  long AktBitmapSize;
  long *bitmap;
  int xh,yh;
  long maxx,maxy;
  double EichX,EichY,EichH,OffsetH;
  int bitplanes;
public:
	double GetMaxUsefulHeight(int winheight,int winwidth);
	double GetMaxUsefulWidth(int winheight,int winwidth);
  void Line(double ax0,double ay0,double ax1, double ay1,long color, int width=1);
	void ShiftColumn(int nr);
	void ShiftRow(int nr);
  void TurnPicture();
  CPicture(double aEichH, double aOffsetH);
  CPicture( int axh, int ayh,
	 double aEichX, double aEichY,double aEichH, double aOffsetH,long value=600000);
  CPicture(const CPicture &pic,long value=1);
  void SetEich(double aEichX, double aEichY);
  void GetEich(double &aEichX, double &aEichY);
  ~CPicture();
  void GetBitmap(TBitmap * &Bitmap, int width, int height, CColorTable *ColorTab, int Zoom, double ZoomCenterX,  double ZoomCenterY);
  int GetBitplanes() {return bitplanes;}
  void Copy(const CPicture &pic);
  unsigned long ReadPicture( CFrameGrabber *framegrabber,
    double xmin, double xmax, double ymin, double ymax, int quadrant=0);
  int Save(char* name,int modus, CColorTable *ColorTab, int binning=1, bool floatingbinning=false);
  int Load(const char* name);
  void MandelPicture();
  void GaussPicture();
  void Clear();
  long Mandel(int x,int y);
  long SearchMaxXMaxY(double axmin=0, double axmax=1, double aymin=0, double aymax=1);
  double MaxX();
  double MaxY();
  void SetMaxXMaxY(double aMaxX, double aMaxY);
  unsigned char GetPixel(double x, double y);
  double GetEichPixel(double x, double y);
  void InitBitmap(int axh,int ayh,long value=600000);
  void ClearBitmap(long value=600000);
  void DeleteBitmap();
  friend CPicture operator-(const CPicture& pic1, const CPicture& pic2);
  friend CPicture operator+(const CPicture& pic1, const CPicture& pic2);
  friend CPicture operator*(const CPicture& pic1, const CPicture& pic2);
  friend CPicture operator/(const CPicture& pic1, const CPicture& pic2);
  CPicture& operator=(const CPicture& pic);
  const CPicture operator*(const double r) const;
  void NormalizeAbsorption(const CPicture &pic1,const CPicture &pic2,const CPicture &pic3,double xmin, double xmax, double ymin, double ymax);
  void Ln(const CPicture &pic1,const CPicture &pic2, const CPicture &pic3,double MaxDensity,double ProbeDetuning, double Gamma, double Lambda, double Clebsch,double xmin, double xmax, double ymin, double ymax);
  friend void LnNormalizeAbsorption(const CPicture &pic1,const CPicture &pic2,CPicture *pic3,CPicture *pic4,CPicture &Abs,CPicture &Ln,
	 double MaxDensity, double xmin, double xmax, double ymin, double ymax,int FluoImage, double MaxFluorescence, bool DoIntensityCompensation, double *Roi,int winheight,int winwidth,bool FKS);
  friend void SubstractNoise(const CPicture &pic1,const CPicture &pic2,const CPicture &pic3,CPicture &Abs,CPicture &Ln);
  void GetList(int horiz, double xpos, double ypos, double pos1, double pos2, double * &x,double * &y, int &ndata);
  void GetSigma(int horiz, double xpos, double ypos, double &sigma, double &height);
  void SetEich(double aEichX, double aEichY, double aEichH, double aOffsetH)
	 { EichX=aEichX; EichY=aEichY; EichH=aEichH; OffsetH=aOffsetH;}
  double GetEichPosX(double xpos);
  double GetEichPosY(double ypos);
  void GetProfil(long * &profilbuf,double pos,double &aEichH,
    double &aOffsetH,double &Eich,double &Offset,int horiz,int &length,int winheight,int winwidth, double &endx, double &endy);
  void GetSum(long * &profilbuf,double pos1, double pos2,double &aEichH,
    double &aOffsetH,double &Eich,double &Offset,int horiz,int &length,int winheight,int winwidth, double &endx, double &endy);
  double GetAbsorption(double x0,double y0);
  void CalcNormalizedX0Y0(double &X0, double &Y0,int winheight,int winwidth);
  double Integral(double xmin, double ymin, double xmax, double ymax,
    bool FitWithOffset,double xminOf, double yminOf, double xmaxOf, double ymaxOf,
    int winheight,int winwidth,double &Offset);
  double RMSNoise(double xmin, double ymin, double xmax, double ymax,
    bool FitWithOffset,double xminOf, double yminOf, double xmaxOf, double ymaxOf,
    int winheight,int winwidth);
  double Offset(double xmin, double ymin, double xmax, double ymax,
    double xminOf, double yminOf, double xmaxOf, double ymaxOf,
    int winheight,int winwidth);
  double CalcRMSNoise(double xmin, double ymin, double xmax, double ymax);
  void GetMinMax(double xmin, double ymin, double xmax, double ymax,double &min,double &max);
  int PictureGood();
  double WinCoordsX(double PicCoordsX,int winwidth, int winheight);
  double WinCoordsY(double PicCoordsY,int winwidth, int winheight);
  double PicCoordsX(double WinCoordsX,int winwidth, int winheight);
  double PicCoordsY(double WinCoordsY,int winwidth, int winheight);
  void GetPictureAsList(double xmin,double xmax,double ymin,double ymax,
    double * &x, double * &y, int &ndata, int &nx, int &ny, int winheight,int winwidth);
  void DisplayFunction(double Params[], void (*func)(double, double, double []));
  void GetPictureAsList(double xmin,double xmax,double ymin,double ymax,
    double * &x, double * &y, int &ndata, int winheight,int winwidth,
    double &asx,double &asy,double &dx,double &dy,int &nx,int &ny);
  void DisplayFunction(double Params[], double (*func)(double, double, double []));
  double GetRealPosX(double x,int winheight,int winwidth);
  double GetRealPosY(double x,int winheight,int winwidth);
  friend void Substract(const CPicture &pic1,const CPicture &pic2, CPicture &pic3);
  friend void Copy(const CPicture &pic1, CPicture &pic2);
  void Extract(CPicture **result,int FKSOffset,int FKSHeight,int FKSDistance,int FKBining,
    int LineShift, double PixelFractionX, double PixelFractionY, int subpicture );
  void Add(const CPicture &pic2);
  void Divide(long d);
  unsigned long GetFreeMem();
private:
  int GetBitmapPosX(double xpos);
  int GetBitmapPosY(double ypos);
};

#endif