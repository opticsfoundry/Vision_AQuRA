#if !defined(__measure_h)
#define __measure_h

#include <dc.h>
#include <window.h>
#include <messageb.h>
#include "textbox.h"
#include "butonbox.h"
#include "names.h"
#include "const.h"


//*** CPoint ***
class CPoint {
public:
  int nrX;
  int nrCalcY;
  double *x;
  double *yb;
  double *s;
  CPoint *next;
  int enabled,active;
  char **filename;
  int nrnames;
  char code[10];
  int PicNr;
  int mycolor;
public:
  CPoint(CPoint *anext,int aPicNr,char *acode,int anrnames, char **afilenames, int aEnabled,int anrX, int anrCalcY,
  double *ax, double *ay, double *as , int acolor );
  ~CPoint();
  void Draw(TClientDC *dc,double xmin,double xmax,double ymin,double ymax,int W,int H,int what);
  void GetMinMaxXY(double &xmin, double &ymin,double &xmax, double &ymax,int what);
  void GetMax(double &value, int aktwhat);
  void GetMin(double &value, int aktwhat);
  int number();
  void GetXY(double *x,double *y,int what, int pos);
  int PointNr(int what);
  int What(int what,int nr);
  virtual double y(int aktwhat);
  double sy(int aktwhat);
  void ToggleEnable();
  int Enabled() {return enabled;};
  void SetActive(int a);
  CPoint *GetNearestPoint(double ax, double ay,int what,double &h);
  char *GetFilename(int nr=0);
  char *GetCode() {return code;}
  void Save(ofstream &out);
  void DeletePoint(CPoint *point);
  int operator<(const CPoint& b);
  friend void Sort(CPoint **liste);
  void ShiftParameters();
  void SetColor(int color);
};

void Sort(CPoint *liste);

//*** CMessPoint ***
class CMessPoint: public CPoint {
public:
 CMessPoint(CPoint *anext, int aPicNr,char *acode,int anrnames,char **afilename,int Ok,int anrx,
     double *ax, double aX0,double aSigmaX, double aOffsetX, double aSlopeX, double aAmpX,
     double aY0,double aSigmaY, double aOffsetY, double aSlopeY,
     double aAmpY, double aAbs,double aOptDens,
     double aN, double aNGauss, double aNIoffe, double aTIoffe,
     double aFluorescence,double aProbeDetuning,double aTimeOfFlight,
     double aFinalTrapType, double aMOTCurrent, double aTransferCurrent,
     double aBarCurrent, double aGxOrOffset, double aGyOrGradient,
     double aGzOrCurvature, double aCalibration,
     double aPicEichX, double aPicEichY, int aBiningX, int aBiningY,int acolor);
     void SetData(double aX0,double aSigmaX,double aAmpX,
       double aY0,double aSigmaY,double aAmpY,double aAbs,double aOptDens, double aN, double aNGauss,double aNIoffe,double aTIoffe);
};

//*** CBigPoint ***
class CBigPoint: public CPoint {
public:
  CBigPoint::CBigPoint(CPoint *anext, int aMessNr,char *acode,int anrnames,char **afilename,int Ok,int anrx,
     double *ax,double TempX, double TempY, double Number, double SigmaX, double SigmaY,
     double n0, double r0, double Sigma0X, double Sigma0Y, double NT);
};


//*** CMessWindow ***
class CMessWindow : public TWindow {
public:
  double xmin,ymin,xmax,ymax;
  TWindow *parent;
  CPoint *points;
  CPoint *aktpoint;
  char MesName[200];
  double MesProbeDetuning;
  char **ParamNames;
  int ParamNr;
  CButtonBox *Buttons;
  TMessageBar *mb;
  CName *names;
  char name[200];
  char disk;
  TStatic *AxText;
  int aktColor;
public:
  CMessWindow::CMessWindow(TWindow* aparent,CName *anames,TMessageBar *amb,
	 CButtonBox *aButtons,TStatic *aAxText,int x, int y, int w, int h);
  CMessWindow::~CMessWindow();
  void Paint(TDC& dc, bool erase, TRect& rect);
  virtual void Draw();
  virtual void New();
  void EvRButtonDown(UINT,TPoint& Point);
  void EvLButtonDown(UINT,TPoint& Point);
  virtual void ButtonAction() {};
  char *GetMesName() { return MesName;}
  double GetMesProbeDetuning() { return MesProbeDetuning;}
  int GetParamNr() {return ParamNr;}
  char *GetName() {return name;}
  void SetMaxXY();
  void DeleteAktPoint();
  void SortSerie() { Sort(&points); };
  void ShiftParameters();
  TColor *GetAktColor();
  void ColorPoints();

  DECLARE_RESPONSE_TABLE(CMessWindow);

};

//*** CTNMeasure ***
class CTNMeasure : public CMessWindow {
public:
  bool FirstPoint;
  double DeltaDetuning;
  double NSigmaX,NSigmaY;
  int Tempfited;
  int Nfited;
  int SNfited;
  int ChargingTimefited;
  int Lifetimefited;
  int NoSerie;
  double SN[2],N,TempX,SpeedX,Sigma0X,TempY,SpeedY,Sigma0Y,ChargingN[2],ChargingT[2],LifetimeN[2],LifetimeT[2];
  double X0,Y0,SigmaX,SigmaY,Abs,OptDens,fN,fNGauss,NIoffe,TIoffe,AmpX,AmpY;
  CTextBox *TNFitTextBox;
  CTextBox *MessDataTextBox;
  CTextBox *GaussFitTextBox;
  char commentary[300];
  double Masse;
public:
  CTNMeasure(TWindow* aparent,CName *anames,TMessageBar *amb,
	 CButtonBox *aButtons,TStatic *aAxText,CTextBox *aMessDataTextBox,CTextBox *aTNFitTextBox,CTextBox *aGaussFitTextBox,int x, int y, int w,int h);
  void AddPoint(double *ax, double aX0,double aSigmaX, double aOffsetX, double aSlopeX, double aAmpX,
     double aY0,double aSigmaY,double aOffsetY, double aSlopeY,double aAmpY,double aAbs,double aOptDens, double aFluorescence, double ProbeDetuning,
     double TimeOfFlight,double aFinalTrapType, double aMOTCurrent, double aTransferCurrent,
     double aBarCurrent, double aGxOrOffset, double aGyOrGradient,
     double aGzOrCurvature, double Calibration,double PicEichX, double PicEichY,
     int BiningX, int BiningY, double aN,double aNGauss,double aNIoffe, double aTIoffe,int NoSerie,
     int Ok,char *picturecode,char *aname,char *FinalTrap);
  double GetTempX() {return TempX;}
  double GetTempY() {return TempY;}
  double GetSigma0X() {return Sigma0X;}
  double GetSigma0Y() {return Sigma0Y;}
  double GetN() {if (Nfited) return N; else if (SNfited) return SN[0]; else return 0.0;}
  virtual void Draw();
  void DrawTempFit(double Sigma0, double SpeedX);
  void DrawNFit();
  void DrawChargingTimeFit(int ID);
  void DrawLifetimeFit(int ID);
  void DrawSNFit(int ID);
  void TemperatureFit();
  void ChargingTimeFit();
  void LifetimeFit();
  void NumberFit();
  void SimpleNumberFit();
  void Load(const char*filename,int add);
  void EvRButtonDown(UINT,TPoint& Point);
  void EvLButtonDown(UINT,TPoint& Point);
  virtual void ButtonAction();
  void StartSerie(char *commentary,int aparamnr, char** aparamNames);
  void StopSerie();
  virtual void New();
  void SetProbeDetuning();
  void ReCalc();
  void Iterate();
  void SetData(double aX0,double aSigmaX,double aAmpX,
    double aY0,double aSigmaY,double aAmpY,double aAbs,double aOptDens,double aN, double aNGauss,double NIoffe, double TIoffe);
  void SaveSerie(const char* filename);
  void SetIsotop(int LithiumIsotop) {Masse=double(LithiumIsotop)*_mp;};
  void GetNumber(int PicNumber, double &Number, char *name);
};

//*** CBigMeasure ***
class CBigMeasure : public CMessWindow {
  CTextBox *TNFitTextBox;
  CTNMeasure *TNmeasure;
  CButtonBox *NSTButtons;
  char commentary[300];
  bool FirstPoint;
public:
  CBigMeasure(TWindow* aparent,CTNMeasure *aTNmeasure,CName *anames,TMessageBar *amb,
  CButtonBox *aButtons,CButtonBox *aNSTButtons,TStatic *aAxText, CTextBox *aTNFitTextBox,int x, int y, int w,int h);
  void CBigMeasure::AddPoint(double isotope, double *ax, double TempX, double TempY,
	double SigmaX, double SigmaY, double Number, double Sigma0X,double Sigma0Y, double NT,
	char *code, char *NumberName, char *SigmaName, char *TemperatureName,char *FinalTrap);
  void Load(const char*filename,int add);
  void SaveSerie(const char* filename);
  virtual void ButtonAction();
  void StartBigSerie(char *acommentary,int aparamnr, char** aparamNames);
  void StopBigSerie();
};

#endif
