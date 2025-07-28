#if !defined(__profil_h)
#define __profil_h

#include <window.h>
#include <dc.h>
#include <groupbox.h>
#include "picture.h"

//*** CProfil ***
class CProfil : public TWindow {
public:
  double endx, endy;
  int winwidth,winheight;
  CPicture** picture;
  long *profilbuf;
  int horiz;
  double pos;
  int gridon;
  CColorTable* Palette;
  double Amp,X0,Sigma;
  int DisplayFit;
  int DisplayGeneralFit;
  int move;
  int movenr;
  double pos1[2];
  double pos2[2];
  TWindow* parent;
  double FitOffset,Offset,OffsetH,Eich,EichH,Slope;
  int length;
  int design;
  double SumPos1;
  double SumPos2;
  double (*GeneralProfilFunc)(double, double []);
  double GeneralProfilParams[10];
  double startcenter;
  double endcenter;
  int AverageNrPixels;
public:
  CProfil(TWindow* parent,int x, int y, int w, int h,int ahoriz, int awinwidth,
    int awinheight, CPicture** apicture, CColorTable* aPalette);
  ~CProfil();
  virtual void Paint(TDC& dc, bool erase, TRect& rect);
  void Update(double apos,int NrPixels=0);
  void DrawProfil();
  void Grid();
  void ShowFit();
  void SetGauss(double aAmp,double aX0, double aSigma, double aOffset, double aSlope);
  void SetExponential(double aAmp,double aX0, double aSigma, double aOffset, double aSlope);
  void RemoveFit();
  void GridOnOff();
  void Cursor();
  void NewPos(int moveleft, int moveblue, double aktpos);
  void GetPos(double &apos1, double &apos2,int nr=0);
  void SetPos(double &apos1, double &apos2,int nr);
  void GetRealPos(double &apos1, double &apos2,int nr=0);
  void EvLButtonUp(UINT,TPoint&);
  void EvLButtonDown(UINT,TPoint&);
  void ButtonUp(TPoint&,int nr);
  void ButtonDown(TPoint&,int nr);
  void EvRButtonUp(UINT,TPoint&);
  void EvRButtonDown(UINT,TPoint&);
  void EvMouseMove(UINT modKeys, TPoint& point);
  void EvKillFocus(HWND hWndGetFocus);
  void GetCursor(double &pos1, double &pos2,int nr=0);
  unsigned char GetPixel(double x);
  void GetList(double * &x, double * &y,int winheight,int winwidth, int &ndata, double &x0,
							 double &sigma, double &height, double &offset);
  void ToggleDesign();
  void Draw();
  void GetSum(double apos1, double apos2,int NrPixels);
  void Save(const char *name,int winheight,int winwidth);
  double GetEich() {return Eich;}
  double XLocalToReal(int x);
  double YLocalToReal(int y);
  int YRealToLocal(double y);
  void Set2DDistribution(double a2DDistParams[8],double pos1,double pos2,double delta,
    double (*funcs)(double,double, double []));
  void SetGeneralProfil(double (*func)(double, double [])=NULL,double a[]=NULL,int na=0, double astartcenter=0, double aendcenter=0);
  void Average(int NrPixels);
private:
  int GetProfilPos(double xpos);

  DECLARE_RESPONSE_TABLE(CProfil);

};

#endif
