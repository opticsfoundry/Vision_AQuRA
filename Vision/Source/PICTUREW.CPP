#include "picturew.h"
#include <color.h>
#include "vision.rh"
#include "visiondg.h"
#include "LeakDetect.h"

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
//  new TGroupBox(parent,0,"",x-1, y-1,w+2,h+2);
  try {
  BkgndBrush = new TBrush(::GetSysColor(COLOR_WINDOW));
  #if defined(DetectLeaks)
    LeakDetect.New(BkgndBrush,4,1);
  #endif
   }
  catch (TGdiBase::TXGdi& x) {
/*    MessageBeep(0xFFFFFFFF);
    MessageBeep(MB_ICONASTERISK);
    MessageBeep(MB_ICONEXCLAMATION);
    MessageBeep(MB_ICONHAND);
	 MessageBeep(MB_ICONQUESTION);
    MessageBeep(MB_OK);*/
/*     ofstream DebugFile("Q:\\DebugPicture.dat", ios::app);
     bool found;
     string s=x.ResourceIdToString(&found,x.GetErrorCode());
	  DebugFile<<"GDI error CPictureWindow::CPictureWindow ";
     if (found) DebugFile<<s;
     DebugFile<<endl;
	  DebugFile.close();*/
   }
  MemoryDC=NULL;
  GetWindowRect(Rec);
}

CPictureWindow::~CPictureWindow() {
  #if defined(DetectLeaks)
    LeakDetect.Delete(BkgndBrush);
  #endif
  delete BkgndBrush;
  #if defined(DetectLeaks)
    LeakDetect.Delete(Bitmap);
  #endif
  if (Bitmap) delete Bitmap;
  if(MemoryDC) delete MemoryDC;
}

void CPictureWindow::Draw(int update) {
//  TRect Rec;
/*  try {
	  GetWindowRect(Rec);
  }
  catch (TGdiBase::TXGdi& x) {
		MessageBox("CPictureWindow::Draw : Catched GDI error!","Error!",MB_OK);
  }
*/
  if ((Rec.Width()<=0) || (Rec.Height()<=0) ||
      (Rec.Width()>512) || (Rec.Height()>512)) {
      //just for debug
/*    MessageBeep(0xFFFFFFFF);
    MessageBeep(MB_ICONASTERISK);
    MessageBeep(MB_ICONEXCLAMATION);
    MessageBeep(MB_ICONHAND);
	 MessageBeep(MB_ICONQUESTION);
    MessageBeep(MB_OK);*/
    return;
  }
  int Zoom;
  double ZoomCenterX,ZoomCenterY;
  ((TVisionDialog*)parent)->GetZoomParameters(Zoom,ZoomCenterX,ZoomCenterY);
  try {
    if (!Bitmap || update) (*picture)->GetBitmap(Bitmap,Rec.Width(),Rec.Height(),Palette, Zoom, ZoomCenterX,  ZoomCenterY);
    TClientDC dc(*this);
    TMemoryDC memDC(dc);
    if (Bitmap) {
	    memDC.SelectObject(*Bitmap);
	    dc.BitBlt(0, 0, Rec.Width(), Rec.Height(), memDC, 0, 0, SRCCOPY);
       // this code is just here to deselect the Bitmap since in the manual they write you should not delete a currently selected bitmap
       TPen *pen;
		 pen=new TPen(TColor(0,255,255));
       #if defined(DetectLeaks)
         LeakDetect.New(pen,4,2);
       #endif
       memDC.SelectObject(*pen);
       #if defined(DetectLeaks)
         LeakDetect.Delete(pen);
       #endif  
       delete pen;
    }
  }
  catch (TGdiBase::TXGdi& x) {
/*    MessageBeep(0xFFFFFFFF);
    MessageBeep(MB_ICONASTERISK);
    MessageBeep(MB_ICONEXCLAMATION);
    MessageBeep(MB_ICONHAND);
	 MessageBeep(MB_ICONQUESTION);
    MessageBeep(MB_OK);*/
/*    ofstream DebugFile("Q:\\DebugPicture.dat", ios::app);
     bool found;
     string s=x.ResourceIdToString(&found,x.GetErrorCode());
	  DebugFile<<"GDI error CPictureWindow::Draw ";
     if (found) DebugFile<<s;
     DebugFile<<endl;
	  DebugFile.close();*/
  }
}

void CPictureWindow::Paint(TDC& adc, bool a, TRect& arect) {
  Draw(0);
}

const GridDelta=25;

void CPictureWindow::GetExtensions(int &awidth,int &aheight) {
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
try {
  TClientDC dc(*this);
  TPen *pen;
  if (Crossfixed) pen=new TPen(TColor(0,255,255));
  else pen=new TPen(TColor(255,255,255));
  #if defined(DetectLeaks)
    LeakDetect.New(pen,4,3);
  #endif
  dc.SetROP2(R2_XORPEN);
  dc.SelectObject(*pen);

  if (ypos>1) ypos=1;
  if (ypos<0) ypos=0;
  if (xpos>1) xpos=1;
  if (xpos<0) xpos=0;
  dc.MoveTo(0,ypos*Attr.H);
  dc.LineTo(xpos*Attr.W-8,ypos*Attr.H);
  dc.MoveTo(xpos*Attr.W+8,ypos*Attr.H);
  dc.LineTo(Attr.W,ypos*Attr.H);
  dc.MoveTo(xpos*Attr.W,0);
  dc.LineTo(xpos*Attr.W,ypos*Attr.H-8);
  dc.MoveTo(xpos*Attr.W,ypos*Attr.H+8);
  dc.LineTo(xpos*Attr.W,Attr.H);
  #if defined(DetectLeaks)
    LeakDetect.Delete(pen);
  #endif
  delete pen;
   }
  catch (TGdiBase::TXGdi& x) {
/*    MessageBeep(0xFFFFFFFF);
    MessageBeep(MB_ICONASTERISK);
    MessageBeep(MB_ICONEXCLAMATION);
    MessageBeep(MB_ICONHAND);
	 MessageBeep(MB_ICONQUESTION);
    MessageBeep(MB_OK);*/
/*     ofstream DebugFile("Q:\\DebugPicture.dat", ios::app);
     bool found;
     string s=x.ResourceIdToString(&found,x.GetErrorCode());
	  DebugFile<<"GDI error CPictureProfilWindow::Cross ";
     if (found) DebugFile<<s;
     DebugFile<<endl;
	  DebugFile.close();*/
   }
}

void CPictureProfilWindow::Grid() {
  if (gridon) {
   try {
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
  catch (TGdiBase::TXGdi& x) {
/*    MessageBeep(0xFFFFFFFF);
    MessageBeep(MB_ICONASTERISK);
    MessageBeep(MB_ICONEXCLAMATION);
    MessageBeep(MB_ICONHAND);
	 MessageBeep(MB_ICONQUESTION);
    MessageBeep(MB_OK);*/
/*     ofstream DebugFile("Q:\\DebugPicture.dat", ios::app);
     bool found;
     string s=x.ResourceIdToString(&found,x.GetErrorCode());
	  DebugFile<<"GDI error CPictureProfilWindow::Grid ";
     if (found) DebugFile<<s;
     DebugFile<<endl;
	  DebugFile.close();*/
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

void CPictureProfilWindow::GetRealPos(double &axpos, double &aypos) {
  axpos=(*picture)->GetRealPosX(xpos,Attr.W,Attr.H);       //Attr.W and Attr.H might be wrong 
  aypos=(*picture)->GetRealPosY(ypos,Attr.W,Attr.H);
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

void CPictureProfilWindow::Update(int Average) {
  Draw(1);
//  xpos=(*picture)->WinCoordsX((*picture)->MaxX(),Attr.W,Attr.H);
//  ypos=(*picture)->WinCoordsY((*picture)->MaxY(),Attr.W,Attr.H);
  Grid();
  Cross();
  if (profilx!=NULL) {
	 profilx->RemoveFit();
	 profilx->Update(ypos,Average);
  }
  if (profily!=NULL) {
	 profily->RemoveFit();
	 profily->Update(xpos,Average);
  }
  parent->PostMessage(IDM_CURSORMOVED,0);
}

void CPictureProfilWindow::GridOnOff() {
  if (gridon) {
	 mb->SetText("Grid OFF");
	 gridon=0;
//	 TClientDC dc(*this);
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
  (*picture)->CalcNormalizedX0Y0(x0,y0,Attr.H,Attr.W);
  xpos=x0;
  ypos=y0;
  Cross();
}

void CPictureProfilWindow::SetCrossfixed(int fixed,double fxpos, double fypos) {
  Cross();
  (*picture)->CalcNormalizedX0Y0(fxpos,fypos,Attr.H,Attr.W);
  xpos=fxpos/Attr.W;
  ypos=fypos/Attr.H;
  Crossfixed=fixed;
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


