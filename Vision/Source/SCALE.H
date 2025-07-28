#if !defined(__scale_h)
#define __scale_h

#include <window.h>
#include <color.h>
#include <gdiobjec.h>

class CScale : public TWindow {
  char *text;
  int S;
  int T;
  int Horiz;
  int Side;
  double Distance;
  int Number;
  int StartNumber;
  double Scale;
  double Offset;
  double Wert0;
  int tposx;
  int tposy;
  TFont *bigfont;
public:
  CScale(TWindow* parent,int x, int y, int aw, int ah, int as, int at,
  int aHoriz, int aSide, double aDistance, int aNumber, int aStartNumber, double aScale,
  double aOffset, double aWert0, char* atext="",int atposx=0, int atposy=0);
  ~CScale();
  void Paint(TDC& adc, bool a, TRect& rec);
  bool EvEraseBkgnd(HDC hDC);

  DECLARE_RESPONSE_TABLE(CScale);
};

#endif
