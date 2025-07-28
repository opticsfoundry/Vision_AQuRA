#include "picturew.h"
#include <color.h>
#include "vision.rh"

//*** CPictureWindow ***
CPictureWindow::CPictureWindow(TWindow* aparent,TMessageBar *amb,int x, int y, int w, int h,
  CPicture** apicture, CColorTable *aPalette)
:TWindow(aparent,0,0) {
  Bitmap=NULL;
  parent=aparent;
  Palette=aPalette;
  mb=amb;
  Attr.X=x;
  Attr.Y=y;
  Attr.W=w;
  Attr.H=h;
  picture=apicture;
  xpos=0;
  ypos=0;
  move=0;
  gridon=0;
  new TGroupBox(parent,0,"",x-1, y-1,w+2,h+2);
  BkgndBrush = new TBrush(::GetSysColor(COLOR_WINDOW));
  MemoryDC=NULL;
}

CPictureWindow::~CPictureWindow() {
  delete BkgndBrush;
}

void CPictureWindow::Draw(int update) {
  TClientDC dc(*this);
  TRect Rec;
  GetWindowRect(Rec);
  if (!Bitmap || update) (*picture)->GetBitmap(Bitmap,Rec.Width(),Rec.Height(),Palette);
  TMemoryDC memDC(dc);
  memDC.SelectObject(*Bitmap);
  dc.BitBlt(0, 0, Rec.Width(), Rec.Height(), memDC, 0, 0, SRCCOPY);
}

void CPictureWindow::Paint(TDC& adc, bool a, TRect& arect) {
  Draw(0);
}

const GridDelta=25;

void CPictureWindow::GetExtensions(int awidth,int aheight) {
  awidth=Attr.W;
  aheight=Attr.H;
}

//*** CPictureProfilWindow ***
DEFINE_RESPONSE_TABLE1(CPictureProfilWindow,CPictureWindow)
  EV_WM_LBUTTONUP,
  EV_WM_LBUTTONDOWN,
  EV_WM_MOUSEMOVE,
  EV_WM_KILLFOCUS,
END_RESPONSE_TABLE;

CPictureProfilWindow::CPictureProfilWindow(TWindow* parent,TMessageBar *amb,int x, int y, int w, int h,
  CPicture** apicture, CProfil* aprofilx, CProfil* aprofily, CColorTable *aPalette)
:CPictureWindow(parent,amb,x,y,w,h,apicture,aPalette) {
  profilx=aprofilx;
  profily=aprofily;
  xpos=(*picture)->MaxX();
  ypos=(*picture)->MaxY();
  Crossfixed=0;
}

void CPictureProfilWindow::Cross() {
  TClientDC dc(*this);
  TPen *pen;
  if (Crossfixed) pen=new TPen(TColor(0,255,255));
  else pen=new TPen(TColor(255,255,255));
  dc.SetROP2(R2_XORPEN);
  dc.SelectObject(*pen);
  dc.MoveTo(0,ypos*Attr.H);
  dc.LineTo(Attr.W,ypos*Attr.H);
  dc.MoveTo(xpos*Attr.W,0);
  dc.LineTo(xpos*Attr.W,Attr.H);
  delete pen;
}

void CPictureProfilWindow::SetCrossfixed(int fixed) {
  Cross();
  Crossfixed=fixed;
  Cross();
}

void CPictureProfilWindow::Grid() {
  if (gridon) {
	 TClientDC dc(*this);
	 TPen pen(TColor(255,255,255));
	 dc.SelectObject(pen);
	 for (int i=GridDelta;i<Attr.W;i+=GridDelta) {
		dc.MoveTo(0,i);
		dc.LineTo(Attr.W,i);
		dc.MoveTo(i,0);
		dc.LineTo(i,Attr.H);
	 }
  }
}

void CPictureProfilWindow::EvLButtonUp(UINT,TPoint& Point) {
  NewPos(double(Point.x)/Attr.W,double(Point.y)/Attr.H);
  move=0;
  parent->PostMessage(IDM_AUTOFIT,0);
}

void CPictureProfilWindow::EvLButtonDown(UINT,TPoint& Point) {
  move=1;
  NewPos(double(Point.x)/Attr.W,double(Point.y)/Attr.H);
}

void CPictureProfilWindow::EvMouseMove(UINT modKeys, TPoint& Point) {
  if (move) NewPos(double(Point.x)/Attr.W,double(Point.y)/Attr.H);
}

void CPictureProfilWindow::EvKillFocus(HWND hWndGetFocus) {
  move=0;
}

void CPictureProfilWindow::NewPos(double axpos, double aypos) {
  Cross();
  xpos=axpos;
  ypos=aypos;
  Cross();
  if (profilx!=NULL) {
	 profilx->RemoveFit();
	 profilx->Update(ypos);
  }
  if (profily!=NULL) {
	 profily->RemoveFit();
	 profily->Update(xpos);
  }
  parent->PostMessage(IDM_CURSORMOVED,0);
}

void CPictureProfilWindow::GetPos(double &axpos, double &aypos) {
  axpos=xpos;
  aypos=ypos;
}

double CPictureProfilWindow::GetHeight() {
  return (*picture)->GetEichPixel(xpos,ypos);
}

void CPictureProfilWindow::Paint(TDC& adc, bool a, TRect& arect) {
  CPictureWindow::Paint(adc,a,arect);
  Draw(0);
  Grid();
  Cross();
}

void CPictureProfilWindow::Update() {
  Draw(1);
  xpos=(*picture)->WinCoordsX((*picture)->MaxX(),Attr.W,Attr.H);
  ypos=(*picture)->WinCoordsY((*picture)->MaxY(),Attr.W,Attr.H);
  Grid();
  Cross();
  if (profilx!=NULL) {
	 profilx->RemoveFit();
	 profilx->Update(ypos);
  }
  if (profily!=NULL) {
	 profily->RemoveFit();
	 profily->Update(xpos);
  }
  parent->PostMessage(IDM_CURSORMOVED,0);
}

void CPictureProfilWindow::GridOnOff() {
  if (gridon) {
	 mb->SetText("Grid OFF");
	 gridon=0;
	 TClientDC dc(*this);
	 Draw(0);
	 Cross();
  } else {
	 mb->SetText("Grid ON");
	 gridon=1;
	 Cross();
	 Grid();
	 Cross();
  }
};

void CPictureProfilWindow::SetCross(double x0,double y0) {
  Cross();
  xpos=x0;
  ypos=y0;
  Cross();
}

//*** CPictureButton ***
DEFINE_RESPONSE_TABLE1(CPictureButton,CPictureWindow)
  EV_WM_LBUTTONDOWN,
END_RESPONSE_TABLE;

CPictureButton::CPictureButton(TWindow* aparent,TMessageBar *amb,int x, int y, int w, int h,
  CPicture** apicture, CColorTable *aPalette, int aCommand, int aId)
:CPictureWindow(aparent,amb,x,y,w,h,apicture,aPalette) {
  Command=aCommand;
  Id=aId;
}

void CPictureButton::EvLButtonDown(UINT,TPoint& Point) {
  parent->PostMessage(IDM_PICTURECHANGED,Command,Id);
}


