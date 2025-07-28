#if !defined(__colortab_h)
#define __colortab_h

#include <window.h>
#include <windows.h>
#include <gdiobjec.h>
#include <groupbox.h>
#include <color.h>

//*** CColorTable ***
class CColorTable : public TWindow {
public:
  TColor* palette;
  TColor* palette16;
  TColor* ProfilPalette;
  TPalette *palette8bit;
  TWindow *parent;
  int stripes;
  double pos1;
  double pos2;
  int move;
  bool LowByte;
  TPalette *WinPalette;
  int AktColorTableNr;
  int colors[255][4];
  int NrColors;
public:
  CColorTable(TWindow* parent,int x, int y, int w, int h);
  ~CColorTable();
  void LoadColorTable1();
  void EvLButtonDown(UINT,TPoint& Point);
  void EvRButtonDown(UINT,TPoint& Point);
  void EvLButtonUp(UINT,TPoint& Point);
  void EvMouseMove(UINT modKeys, TPoint& Point);
  void EvKillFocus(HWND hWndGetFocus);
  void NewPos(double apos1, double apos2);
  void GetPos(double &apos1, double &apos2);
  virtual void Paint(TDC& dc, bool erase, TRect& rect);
  TColor Color(unsigned char col);
  TColor ProfilColor(unsigned char col);
  void Update();
  void LoadPalette(const char* name);
  TColor ColorTable1(unsigned long col);
  TColor ColorTable2(unsigned long col);
  TColor *GetPalette16bit();
  void MakePalette8bit();
  TPalette *GetPalette8bit();
  void CalcProfilPalette();
  void SetStandard();
  void SetStandard2();
  void AddStripes();
  void SetLowByte();
  void SetBlackWhite();
  void SetWhiteBlack();
  void SetColorTableNr(int Nr);
  int GetColorTableNr();
  TColor ColorTableBlackWhite(unsigned long col);
  TColor ColorTableWhiteBlack(unsigned long col);
  void RefreshPalette();
  TColor AktColorTable(unsigned long col);

  DECLARE_RESPONSE_TABLE(CColorTable);
};

#endif