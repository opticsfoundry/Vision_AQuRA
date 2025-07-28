#if !defined(__measure_h)
#define __measure_h

#include <dc.h>
#include <window.h>
#include <messageb.h>
#include "textbox.h"
#include "butonbox.h"
#include "names.h"
#include "const.h"
#include "Messpoint.h"
#include <edit.h>

//*** CMessWindow ***
class CMessWindow : public TWindow {
public:
  double xmin,ymin,xmax,ymax;
  TWindow *parent;
  CMyPoint *points;
  CMyPoint *aktpoint;
  char MesName[200];
  int MesIsotop;
//  double MesProbeDetuning;
  CButtonBox *Buttons;
  CButtonBox *CameraButtons;
  TMessageBar *mb;
  CName *names;
  char name[200];
  char MyName[200];
  char disk;
  TStatic *AxText;
  int aktColor;
  CMyPoint *spoints;
public:
  CMessWindow::CMessWindow(TWindow* aparent,CName *anames,TMessageBar *amb,
	 CButtonBox *aButtons,CButtonBox *aCameraButtons,TStatic *aAxText,int x, int y, int w, int h);
  CMessWindow::~CMessWindow();
  void Paint(TDC& dc, bool erase, TRect& rect);
  virtual void Draw();
  virtual void New();
  void EvRButtonDown(UINT,TPoint& Point);
  void EvLButtonDown(UINT,TPoint& Point);
  virtual void ButtonAction() {};
  char *GetMesName() { return MesName;}
  int GetMesIsotop() { return MesIsotop;}
//  double GetMesProbeDetuning() { return MesProbeDetuning;}
//  int GetParamNr() {return ParamNr;}
  char *GetName() {return name;}
  void SetMaxXY();
  void DeleteAktPoint();
  void SortSerie() { Sort(&points); spoints=points; };
  void ShiftParameters();
  TColor *GetAktColor();
  void ColorPoints();
  void ShowNext();
  void ShowLast();

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
  CMessPoint* LoadPoint;
  char oldseriename[200];
public:
  CTNMeasure(TWindow* aparent,CName *anames,TMessageBar *amb,
	 CButtonBox *aButtons,CButtonBox *aCameraButtons,TStatic *aAxText,CTextBox *aMessDataTextBox,CTextBox *aTNFitTextBox,CTextBox *aGaussFitTextBox,int x, int y, int w,int h);
  void AddPoint(int NoSerie, CMessPoint *MD,TEdit *LogbookEdit);
  CMessPoint* GetLoadPoint() {return LoadPoint;}
//  void Check();
  double GetTempX() {return TempX;}
  double GetTempY() {return TempY;}
  double GetSigma0X() {return Sigma0X;}
  double GetSigma0Y() {return Sigma0Y;}
  double GetN() {if (Nfited) return N; else if (SNfited) return SN[0]; else return 0.0;}
  virtual void Draw();
  void DrawTempFit(double Sigma0, double SpeedX);
//  void DrawNFit();
  void DrawChargingTimeFit(int ID);
  void DrawLifetimeFit(int ID);
  void DrawSNFit(int ID);
  void TemperatureFit(double aMasse);
  void ChargingTimeFit();
  void LifetimeFit();
//  void NumberFit();
  void SimpleNumberFit();
  void Load(const char*filename,int add);
  void EvRButtonDown(UINT,TPoint& Point);
  void EvLButtonDown(UINT,TPoint& Point);
  virtual void ButtonAction();
  void StartSerie(char *commentary,CMessPoint *MD);
  void StopSerie();
  void ContinueSerie();
  virtual void New();
  void SetProbeDetuning();
  void ReCalc();
  void Iterate();
//  void SetData(double aX0,double aSigmaX,double aAmpX,
//    double aY0,double aSigmaY,double aAmpY,double aAbs,double aOptDens,double aN, double aNGauss,double NIoffe, double TIoffe);
  void SaveSerie(const char* filename);
  void SetIsotop(int LithiumIsotop) {Masse=double(LithiumIsotop)*_mp;};
  void GetNumber(int PicNumber, double &Number, char *name);
  void SaveSerie();
};

//*** CBigMeasure ***
class CBigMeasure : public CMessWindow {
  CTextBox *TNFitTextBox;
  CTNMeasure *TNmeasure;
  CButtonBox *NSTButtons;
  char commentary[300];
  bool FirstPoint;
  char **ParamNames;
  int ParamNr;
public:
  CBigMeasure(TWindow* aparent,CTNMeasure *aTNmeasure,CName *anames,TMessageBar *amb,
  CButtonBox *aButtons,CButtonBox *aCameraButtons,CButtonBox *aNSTButtons,TStatic *aAxText, CTextBox *aTNFitTextBox,int x, int y, int w,int h);
  void CBigMeasure::AddPoint(double isotope, double *ax, double TempX, double TempY,
	double SigmaX, double SigmaY, double Number, double Sigma0X,double Sigma0Y, double NT,
	char *code, char *NumberName, char *SigmaName, char *TemperatureName,const char *FinalTrap);
  void Load(const char*filename,int add);
  void SaveSerie(const char* filename);
  virtual void ButtonAction();
  void StartBigSerie(char *acommentary,int aparamnr, char** aparamNames);
  void StopBigSerie();
  int GetParamNr() {return ParamNr;}
};

#endif
