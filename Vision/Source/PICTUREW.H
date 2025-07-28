#if !defined(__picturew_h)
#define __picturew_h

#include <window.h>
#include <dc.h>
#include <groupbox.h>
#include "picture.h"
#include "profil.h"
#include <messageb.h>

//*** CPictureWindow ***
class CPictureWindow : public TWindow {
public:
  CPicture** picture;
  TBitmap *Bitmap;
  double xpos,ypos;
  int move;
  int gridon;
  CColorTable *Palette;
  TMessageBar *mb;
  TWindow *parent;
  TBrush*   BkgndBrush;
  TMemoryDC *MemoryDC;
  TRect Rec;
public:
  CPictureWindow(TWindow* parent,TMessageBar *amb,int x, int y, int w, int h,
	 CPicture** apicture, CColorTable *aPalette);
  ~CPictureWindow();
  virtual void Paint(TDC& dc, bool erase, TRect& rect);
  void Draw(int update);
  void GetExtensions(int &awidth,int &aheight);
};

//*** CPictureProfilWindow ***
class CPictureProfilWindow : public CPictureWindow {
public:
  CProfil* profilx;
  CProfil* profily;
  int Crossfixed;
public:
  CPictureProfilWindow(TWindow* parent,TMessageBar *amb,int x, int y, int w, int h,
	 CPicture** apicture, CProfil* aprofilx, CProfil* aprofily, CColorTable *aPalette);
  virtual void Paint(TDC& dc, bool erase, TRect& rect);
  void Update(int Average);
  void EvLButtonUp(UINT,TPoint&);
  void EvLButtonDown(UINT,TPoint&);
  void EvMouseMove(UINT modKeys, TPoint& point);
  void EvKillFocus(HWND hWndGetFocus);
  void Cross();
  void Grid();
  void GridOnOff();
  void NewPos(double axpos, double aypos);
  void GetPos(double &axpos, double &aypos);
  void GetRealPos(double &axpos, double &aypos);
  double GetHeight();
  void SetCross(double x0,double y0);
  void SetCrossfixed(int fixed,double fxpos, double fypos);

  DECLARE_RESPONSE_TABLE(CPictureProfilWindow);

};

//*** CPictureButton ***
class CPictureButton : public CPictureWindow {
public:
  int Command,Id;
public:
  CPictureButton(TWindow* parent,TMessageBar *amb,int x, int y, int w, int h,
	 CPicture** apicture, CColorTable *aPalette, int aCommand, int aId);
  void EvLButtonDown(UINT,TPoint&);

  DECLARE_RESPONSE_TABLE(CPictureButton);

};

#endif
