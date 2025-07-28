#include "profil.h"
#include <math.h>
#include "vision.rh"
#include "fitmath.h"
#include "LeakDetect.h"

#define GridWidth 25
#define GridHeight 20

//*** CProfilWindow ***
DEFINE_RESPONSE_TABLE1(CProfil,TWindow)
  EV_WM_LBUTTONUP,
  EV_WM_LBUTTONDOWN,
  EV_WM_RBUTTONUP,
  EV_WM_RBUTTONDOWN,
  EV_WM_MOUSEMOVE,
  EV_WM_KILLFOCUS,
END_RESPONSE_TABLE;

CProfil::CProfil(TWindow* aparent,int x, int y, int w, int h,int ahoriz,
  int awinwidth, int awinheight, CPicture** apicture, CColorTable* aPalette)
:TWindow(aparent,0,0) {
  endx=1;
  endy=1;
  winwidth=awinwidth;
  winheight=awinheight;
  parent=aparent;
  pos1[0]=0;
  pos2[0]=1;
  pos1[1]=0;
  pos2[1]=1;
  move=0;
  movenr=0;
  Attr.X=x;
  Attr.Y=y;
  Attr.W=w;
  Attr.H=h;
  horiz=ahoriz;
  picture=apicture;
  gridon=0;
  Palette=aPalette;
  profilbuf=NULL;
  pos=0;
  if (picture) {
	  if (horiz) pos=(*picture)->MaxY(); else pos=(*picture)->MaxX();
	  (*picture)->GetProfil(profilbuf,pos,EichH,OffsetH,Eich,Offset,horiz,length,winheight,winwidth,endx, endy);
  }
//  new TGroupBox(parent,0,"",x-1, y-1,w+2,h+2);
  DisplayFit=0;
  DisplayGeneralFit=0;
  Amp=0;
  X0=0;
  Sigma=1;
  FitOffset=0;
  Slope=0;
  design=1;
  AverageNrPixels=0;
}

CProfil::~CProfil() {
  #if defined(DetectLeaks)
    LeakDetect.Delete(profilbuf);
  #endif
  if (profilbuf) delete[] profilbuf;
}

void CProfil::Paint(TDC& adc, bool a, TRect& arect) {
  TWindow::Paint(adc,a,arect);
  Draw();
}

void CProfil::Draw() {
  try {
  TClientDC dc(*this);
  TBrush *brush;
  try {
	  if (design) brush=new TBrush(TColor(255,255,255));
	  else brush=new TBrush(TColor(0,0,0));
     #if defined(DetectLeaks)
       LeakDetect.New(brush,3,1);
     #endif  
  }
  catch (xalloc) {
  		MessageBox("CProfil::Draw : couldn't reserve memory","Error",  MB_OK);
      exit(-1);
  }
  dc.SelectObject(*brush);
  dc.FillRect(TRect(0,0,Attr.W,Attr.H),*brush);
  //error occured in following line
  #if defined(DetectLeaks)
    LeakDetect.Delete(brush);
  #endif  
  if (brush) delete brush;
  Grid();
  ShowFit();
  DrawProfil();
  Cursor();
  }
  catch (TGdiBase::TXGdi& x) {
    MessageBeep(0xFFFFFFFF);
    MessageBeep(MB_ICONASTERISK);
    MessageBeep(MB_ICONEXCLAMATION);
    MessageBeep(MB_ICONHAND);
	 MessageBeep(MB_ICONQUESTION);
    MessageBeep(MB_OK);
     ofstream DebugFile("DebugPicture.dat", ios::app);
     bool found;
     string s=x.ResourceIdToString(&found,x.GetErrorCode());
	  DebugFile<<"GDI error CProfil::Draw ";
     if (found) DebugFile<<s;
     DebugFile<<endl;
	  DebugFile.close();
   }
}

void CProfil::Update(double apos,int NrPixels) {
  if (!picture) return;
  pos=apos;
  (*picture)->GetProfil(profilbuf,pos,EichH,OffsetH,Eich,Offset,horiz,length,winheight,winwidth,endx, endy);
  if (NrPixels>=1) AverageNrPixels=NrPixels; else AverageNrPixels=1;
  Average(AverageNrPixels);
  Draw();
}

void CProfil::GetSum(double apos1, double apos2,int NrPixels) {
  if (!picture) return;
  (*picture)->GetSum(profilbuf,apos1,apos2,EichH,OffsetH,Eich,Offset,horiz,length,winheight,winwidth,endx, endy);
  if (NrPixels>=1) AverageNrPixels=NrPixels; else AverageNrPixels=1;
  Average(NrPixels);
  Draw();
}

void CProfil::Average(int NrPixel) {
  if (!profilbuf) return;
  long *helpprofil;
  try {
	  helpprofil=new long[length];
     #if defined(DetectLeaks)
       LeakDetect.New(helpprofil,3,2);
     #endif  
  }
  catch (xalloc) {
  		MessageBox("CProfil::Average : couldn't reserve memory","Error",  MB_OK);
      exit(-1);
  }
  long sum;
  int nr;
  for (int i=0;i<length;i++) {
    sum=0;
    nr=0;
    for (int j=-NrPixel;j<=NrPixel;j++) {
      int h=i+j;
      if ((0<=h) && (h<length)) {
        sum+=profilbuf[h];
        nr++;
      }
    }
    if (nr>0) helpprofil[i]=sum/nr; else helpprofil[i]=0;
  }
  #if defined(DetectLeaks)
    LeakDetect.Delete(profilbuf);
  #endif  
  delete[] profilbuf;
  profilbuf=helpprofil;
}

unsigned char CProfil::GetPixel(double x) {
  if (!profilbuf) return 0;
  unsigned int y1=x*length;
  if (y1>=length) y1=length-1;
  return profilbuf[y1]/(256*SHIFT);
}

void CProfil::ToggleDesign() {
  if (design) design=0; else design=1;
  Draw();
}

void CProfil::DrawProfil() {
try {
  TClientDC dc(*this);
  if (design) {
	 TPen pen(TColor(0,0,0));
	 dc.SelectObject(pen);
	 if (horiz) {
		dc.MoveTo(0,Attr.H-GetPixel(0/Attr.W)*Attr.H/256);
		for (double i=0;i<Attr.W;i++)
		  dc.LineTo(i,Attr.H-GetPixel(i/Attr.W)*Attr.H/256);
	 } else {
		dc.MoveTo(Attr.W-GetPixel(1-0/Attr.H)*Attr.W/256,0);
		for (double i=0;i<Attr.H;i++)
		  dc.LineTo(Attr.W-GetPixel(1-i/Attr.H)*Attr.W/256,i);
	 }
  } else {
	 if (horiz) {
		for (double i=0;i<Attr.W;i++) {
		  int start=Attr.H-GetPixel(i/Attr.W)*Attr.H/256;
		  int ziel=Attr.H-GetPixel((i+1)/Attr.W)*Attr.H/256;
		  int j=start;
		  do {
			 dc.SetPixel(i,j,Palette->ProfilColor((Attr.H-j)*255/Attr.H));
			 if (j!=ziel) j+=(ziel>start) ? 1 : -1;
		  } while (j!=ziel);
		}
	 } else {
		for (double i=0;i<Attr.H;i++) {
		  int start=Attr.W-GetPixel(1-i/Attr.H)*Attr.W/256;
		  int ziel=Attr.W-GetPixel(1-(i+1)/Attr.H)*Attr.W/256;
		  int j=start;
		  do {
			 dc.SetPixel(j,i,Palette->ProfilColor((Attr.W-j)*255/Attr.W));
			 if (j!=ziel) j+=(ziel>start) ? 1 : -1;
		  } while (j!=ziel);
		}
	 }
  }
  }
  catch (TGdiBase::TXGdi& x) {
    MessageBeep(0xFFFFFFFF);
    MessageBeep(MB_ICONASTERISK);
    MessageBeep(MB_ICONEXCLAMATION);
    MessageBeep(MB_ICONHAND);
	 MessageBeep(MB_ICONQUESTION);
    MessageBeep(MB_OK);
     ofstream DebugFile("DebugPicture.dat", ios::app);
     bool found;
     string s=x.ResourceIdToString(&found,x.GetErrorCode());
	  DebugFile<<"GDI error CProfil::DrawProfil ";
     if (found) DebugFile<<s;
     DebugFile<<endl;
	  DebugFile.close();
   }
}

void CProfil::Grid() {
  if (gridon) {
  try {
	 TClientDC dc(*this);
	 TPen *pen;
    try {
		 if (design) pen=new TPen(TColor(0,0,0));
		 else pen=new TPen(TColor(255,255,255));
       #if defined(DetectLeaks)
  		   LeakDetect.New(pen,3,3);
       #endif  
    }
	 catch (xalloc) {
  		MessageBox("CProfil::Grid : couldn't reserve memory","Error",  MB_OK);
      exit(-1);
    }
	 dc.SelectObject(*pen);
    #if defined(DetectLeaks)
      LeakDetect.Delete(pen);
    #endif  
	 delete pen;
	 int Dx,Dy;
	 if (horiz) {
		Dx=GridWidth;
		Dy=GridHeight;
	 } else {
		Dy=GridWidth;
		Dx=GridHeight;
	 }
	 for (int i=Dx;i<Attr.W;i+=Dx) {
		dc.MoveTo(Attr.W-i,0);
		dc.LineTo(Attr.W-i,Attr.H);
	 }
	 for (int i=Dy;i<Attr.H;i+=Dy) {
		dc.MoveTo(0,Attr.H-i);
		dc.LineTo(Attr.W,Attr.H-i);
	 }
    }
  catch (TGdiBase::TXGdi& x) {
    MessageBeep(0xFFFFFFFF);
    MessageBeep(MB_ICONASTERISK);
    MessageBeep(MB_ICONEXCLAMATION);
    MessageBeep(MB_ICONHAND);
	 MessageBeep(MB_ICONQUESTION);
    MessageBeep(MB_OK);
     ofstream DebugFile("DebugPicture.dat", ios::app);
     bool found;
     string s=x.ResourceIdToString(&found,x.GetErrorCode());
	  DebugFile<<"GDI error CProfil::Grid ";
     if (found) DebugFile<<s;
     DebugFile<<endl;
	  DebugFile.close();
   }
  }
}

double SumPos1;
double SumPos2;
double SumDelta;
double Dist2DParams[10];
double (*DisplayFunc)(double, double, double []);

void CProfil::ShowFit() {
try {
  if (fabs(Sigma)<1E-10) Sigma=1E-10;
  if (Sigma>1E20) Sigma=1E20;
  if (Sigma<-1E20) Sigma=-1E20;
  if (DisplayFit==1) {
	 TClientDC dc(*this);
	 TPen pen(TColor(255,0,0));
	 dc.SelectObject(pen);
	 if (horiz) {
      if (Sigma<1E-10) Sigma=1E-10;
		double a=X0/Sigma;
      if (fabs(a)>10) a=10;
      double help=Attr.H-(Amp*exp(-0.5*a*a)+FitOffset)*Attr.H;
      if (help<0) help=0;
      if (help>Attr.H) help=Attr.H;
		dc.MoveTo(0,help);
		for (int i=0;i<Attr.W;i++) {
        double x=double(i)/Attr.W;
		  a=(x-X0)/Sigma;
        if (fabs(a)>10) a=10;
        help=Attr.H-(Amp*exp(-0.5*a*a)+FitOffset+Slope*x)*Attr.H;
        if (help<0) help=0;
	     if (help>Attr.H) help=Attr.H;
		  dc.LineTo(i,help);
		}
	 } else {
      double x=1;
		double a=(x-X0)/Sigma;
      if (fabs(a)>10) a=10;
      double help=Attr.W-(Amp*exp(-0.5*a*a)+FitOffset+Slope*x)*Attr.W;
      if (help<0) help=0;
      if (help>Attr.W) help=Attr.W;
		dc.MoveTo(help,0);
		for (int i=0;i<Attr.H;i++) {
        double x=double(Attr.H-i)/Attr.H;
		  a=(x-X0)/Sigma;
        if (fabs(a)>10) a=10;
        help=Attr.W-(Amp*exp(-0.5*a*a)+FitOffset+Slope*x)*Attr.W;
        if (help<0) help=0;
        if (help>Attr.W) help=Attr.W;
  		  dc.LineTo(help,i);
		}
	 }
  }
  if (DisplayFit==2) {
	 TClientDC dc(*this);
	 TPen pen(TColor(255,0,0));
	 dc.SelectObject(pen);
	 if (horiz) {
		double a=X0/Sigma;
      if (fabs(a)>100) a=100;
      double help=Attr.H-(Amp*exp(-a)+FitOffset)*Attr.H;
      if (help<0) help=0;
      if (help>Attr.H) help=Attr.H;
		dc.MoveTo(0,help);
		for (int i=0;i<Attr.W;i++) {
        double x=double(i)/Attr.W;
		  a=fabs(x-X0)/Sigma;
        if (fabs(a)>100) a=100;
        double help=Attr.H-(Amp*exp(-a)+FitOffset+Slope*x)*Attr.H;
        if (help<0) help=0;
        if (help>Attr.H) help=Attr.H;
		  dc.LineTo(i,help);
		}
	 } else {
      double x=1;
		double a=fabs(1-X0)/Sigma;
      if (a>100) a=100;
      double help=Attr.W-(Amp*exp(-a)+FitOffset+Slope*x)*Attr.W;
      if (help<0) help=0;
      if (help>Attr.W) help=Attr.W;
		dc.MoveTo(help,0);
		for (int i=0;i<Attr.H;i++) {
        x=double(Attr.H-i)/Attr.H;
		  a=fabs(x-X0)/Sigma;
        if (a>100) a=100;
        help=Attr.W-(Amp*exp(-a)+FitOffset+Slope*x)*Attr.W;
        if (help<0) help=0;
        if (help>Attr.W) help=Attr.W;
		  dc.LineTo(help,i);
		}
	 }
  }
  if (DisplayFit==3) {
	 TClientDC dc(*this);
	 TPen pen(TColor(255,0,0));
	 dc.SelectObject(pen);
	 if (horiz) {
		dc.MoveTo(0,YRealToLocal(SumY(XLocalToReal(0),SumPos1,SumPos2,SumDelta,Dist2DParams,DisplayFunc)));
		for (int i=0;i<Attr.W;i++) dc.LineTo(i,YRealToLocal(SumY(XLocalToReal(i),SumPos1,SumPos2,SumDelta,Dist2DParams,DisplayFunc)));
	 } else {
      dc.MoveTo(YRealToLocal(SumX(SumPos1,SumPos2,SumDelta,YLocalToReal(0),Dist2DParams,DisplayFunc)),0);
		for (int i=0;i<Attr.H;i++) dc.LineTo(YRealToLocal(SumX(SumPos1,SumPos2,SumDelta,YLocalToReal(i),Dist2DParams,DisplayFunc)),i);
	 }
  }
  if (DisplayGeneralFit==1) {
	 TClientDC dc(*this);
	 TPen pen(TColor(0,255,0));
	 dc.SelectObject(pen);
	 if (horiz) {
		dc.MoveTo(0,YRealToLocal(GeneralProfilFunc(XLocalToReal(0)*1000,GeneralProfilParams)));
		for (int i=0;i<Attr.W;i++) {
			double hx=XLocalToReal(i)*1000;
			if ((hx>endcenter) || (hx<startcenter)) {
				TPen pen(TColor(0,255,0));
       			dc.SelectObject(pen);
			} else {
				TPen pen(TColor(0,0,255));
      			dc.SelectObject(pen);
			}
			dc.LineTo(i,YRealToLocal(GeneralProfilFunc(hx,GeneralProfilParams)));
		}
	 } else {
      double help;
      dc.MoveTo(YRealToLocal(GeneralProfilFunc(YLocalToReal(0)*1000,GeneralProfilParams)),0);
		for (int i=0;i<Attr.H;i++) {
        double hy=YLocalToReal(i)*1000;
        if ((hy>endcenter) || (hy<startcenter)) {
          TPen pen(TColor(0,255,0));
       	 dc.SelectObject(pen);
        } else {
          TPen pen(TColor(0,0,255));
      	 dc.SelectObject(pen);
        }
        dc.LineTo(YRealToLocal(/*help=*/GeneralProfilFunc(hy,GeneralProfilParams)),i);
        //if (help==-1000) i=Attr.H;
      }
	 }
  }
}
  catch (TGdiBase::TXGdi& x) {
    MessageBeep(0xFFFFFFFF);
    MessageBeep(MB_ICONASTERISK);
    MessageBeep(MB_ICONEXCLAMATION);
    MessageBeep(MB_ICONHAND);
	 MessageBeep(MB_ICONQUESTION);
    MessageBeep(MB_OK);
     ofstream DebugFile("DebugPicture.dat", ios::app);
     bool found;
     string s=x.ResourceIdToString(&found,x.GetErrorCode());
	  DebugFile<<"GDI error CProfil::ShowFit ";
     if (found) DebugFile<<s;
     DebugFile<<endl;
	  DebugFile.close();
   }
}

void CProfil::GridOnOff() {
  if (gridon) {
	 gridon=0;
	 Update(pos,AverageNrPixels);
  } else {
	 gridon=1;
	 Grid();
  }
}

double CProfil::XLocalToReal(int x) {
  if (!picture) return 0;
  return (*picture)->GetRealPosX(double(x)/Attr.W,winheight,winwidth)/1000 ;
}

double CProfil::YLocalToReal(int y) {
  if (!picture) return 0;
  return (*picture)->GetRealPosY(double(y)/Attr.H,winheight,winwidth)/1000 ;
}

int CProfil::YRealToLocal(double y) {
  if (y>(EichH*(Attr.H+Attr.W))) y=(EichH*(Attr.H+Attr.W));
  if (EichH<1E-30) EichH=1;
  y=y/EichH+OffsetH;
  double help1=Attr.H;
  double help2=Attr.W;
  double out;
  if (horiz) out=(help1-y*help1);
  else out=(help2-y*help2);
  int out2=out;
  return out2;
}

void CProfil::SetGeneralProfil(double (*func)(double, double []),double a[],int na,
    double astartcenter, double aendcenter) {
  for (int i=0;i<na;i++) GeneralProfilParams[i+1]=a[i];  //i+1 because of numerical recipes standard
  GeneralProfilFunc=func;
  DisplayGeneralFit=(func) ? 1: 0;
  if (astartcenter<aendcenter) {
    startcenter=astartcenter;
    endcenter=aendcenter;
  } else {
    endcenter=astartcenter;
    startcenter=aendcenter;
  }
  Draw();
}

void CProfil::SetGauss(double aAmp,double aX0, double aSigma, double aOffset,double aSlope) {
  Amp=aAmp/EichH;
  FitOffset=(aOffset-Offset*aSlope*Eich)/EichH+OffsetH;
  Slope=aSlope/EichH*(Eich*length);
  X0=(aX0+Offset*Eich)/Eich/length;
  Sigma=aSigma/(Eich*length);
  if (Sigma<0.0001) Sigma=0.0001;  //too big?
  DisplayFit=1;
  Draw();
}

void CProfil::SetExponential(double aAmp,double aX0, double aSigma, double aOffset, double aSlope) {
  Amp=aAmp/EichH;
  FitOffset=(aOffset-Offset*aSlope*Eich)/EichH+OffsetH;
  Slope=aSlope/EichH*(Eich*length);
  X0=(aX0+Offset*Eich)/Eich/length;
  Sigma=aSigma/(Eich*length);
  DisplayFit=2;
  Draw();
}

void CProfil::Set2DDistribution(double a2DDistParams[8],double pos1,double pos2,double delta,
    double (*funcs)(double,double, double [])) {
  for (int i=0;i<9;i++) Dist2DParams[i+1]=a2DDistParams[i];
  SumPos1=pos1/1000;   // Conversion mm -> m
  SumPos2=pos2/1000;   // Conversion mm -> m
  SumDelta=delta/1000; // Conversion mm -> m
  DisplayFit=3;
  DisplayFunc=funcs;
  Draw();
}

void CProfil::RemoveFit() {
  DisplayFit=0;
  Update(pos,AverageNrPixels);
}

/*void CProfil::NewPos(double apos1, double apos2, int nr) {
  Cursor();
  if (apos1>apos2) {
	 double hpos=apos1;
	 apos1=apos2;
	 apos2=hpos;
  }
  pos1[nr]=apos1;
  pos2[nr]=apos2;
  Cursor();
  parent->PostMessage(IDM_PROFILCURSORMOVED,0);
}*/

void CProfil::NewPos(int moveleft, int moveblue, double aktpos) {
  if (aktpos>1) aktpos=1;
  Cursor();
  double apos11=pos1[0]; //left  blue
  double apos21=pos2[0]; //right blue
  double apos12=pos1[1]; //left  red
  double apos22=pos2[1]; //right red
  if (moveleft==1) {
    if (moveblue==1) apos11=aktpos; else apos12=aktpos;
  } else {
    if (moveblue==1) apos21=aktpos; else apos22=aktpos;
  }
  if (apos11>apos21) {
	 double hpos=apos11;
	 apos11=apos21;
	 apos21=hpos;
  }
  pos1[0]=apos11;
  pos2[0]=apos21;
  if (apos12>apos22) {
	 double hpos=apos12;
	 apos12=apos22;
	 apos22=hpos;
  }
  pos1[1]=apos12;
  pos2[1]=apos22;
  Cursor();
  parent->PostMessage(IDM_PROFILCURSORMOVED,0);
}

void CProfil::GetPos(double &apos1, double &apos2,int nr) {
  apos1=pos1[nr];
  apos2=pos2[nr];
}

void CProfil::SetPos(double &apos1, double &apos2,int nr) {
  Cursor();
  pos1[nr]=apos1;
  pos2[nr]=apos2;
  Cursor();
  parent->PostMessage(IDM_PROFILCURSORMOVED,0);
}

void CProfil::GetRealPos(double &apos1, double &apos2,int nr) {
  if (!picture) return;
  if (horiz) {
    apos1=(*picture)->GetRealPosX(pos1[nr],winheight,winwidth);
    apos2=(*picture)->GetRealPosX(pos2[nr],winheight,winwidth);
  } else {
    apos1=(*picture)->GetRealPosY(pos2[nr],winheight,winwidth);
    apos2=(*picture)->GetRealPosY(pos1[nr],winheight,winwidth);
  }
//  apos1=(pos1[nr]-0.5)*length*Eich;
//  apos2=(pos2[nr]-0.5)*length*Eich;
}

void CProfil::Cursor() {
try {
  TClientDC dc(*this);
  dc.SetROP2(R2_XORPEN);
  for (int nr=0;nr<2;nr++) {
    TPen *pen;
    if (nr==0) pen=new TPen(TColor(255,255,0));
    else pen=new TPen(TColor(0,255,255));
    #if defined(DetectLeaks)
      LeakDetect.New(pen,3,4);
    #endif
    dc.SelectObject(*pen);
    if (horiz) {
    	dc.MoveTo(pos1[nr]*Attr.W/endx,0);
	   dc.LineTo(pos1[nr]*Attr.W/endx,Attr.H);
	   dc.MoveTo(pos2[nr]*Attr.W/endx,0);
	   dc.LineTo(pos2[nr]*Attr.W/endx,Attr.H);
    } else {
	   dc.MoveTo(0,pos1[nr]*Attr.H/endy);
	   dc.LineTo(Attr.W,pos1[nr]*Attr.H/endy);
	   dc.MoveTo(0,pos2[nr]*Attr.H/endy);
	   dc.LineTo(Attr.W,pos2[nr]*Attr.H/endy);
    }
    #if defined(DetectLeaks)
      LeakDetect.Delete(pen);
    #endif
    delete pen;
  }
}
  catch (TGdiBase::TXGdi& x) {
    MessageBeep(0xFFFFFFFF);
    MessageBeep(MB_ICONASTERISK);
    MessageBeep(MB_ICONEXCLAMATION);
    MessageBeep(MB_ICONHAND);
	 MessageBeep(MB_ICONQUESTION);
    MessageBeep(MB_OK);
     ofstream DebugFile("DebugPicture.dat", ios::app);
     bool found;
     string s=x.ResourceIdToString(&found,x.GetErrorCode());
	  DebugFile<<"GDI error CProfil::Cursor ";
     if (found) DebugFile<<s;
     DebugFile<<endl;
	  DebugFile.close();
   }
}

void CProfil::EvLButtonUp(UINT,TPoint& Point) {
  ButtonUp(Point,1);
}

void CProfil::EvRButtonUp(UINT,TPoint& Point) {
  ButtonUp(Point,2);
}

void CProfil::ButtonUp(TPoint& Point,int nr) {
  double aktpos;
  if (horiz) aktpos=endx*(double(Point.x)/Attr.W);
  else aktpos=endy*(double(Point.y)/Attr.H);
  NewPos(move,nr,aktpos);
  if (nr==1) {
    if (movenr==3) movenr=2;
    else movenr=0;
  } else {
    if (movenr==3) movenr=1;
    else movenr=0;
  }
//  movenr=movenr & (!nr);
  if (movenr==0) move=0;
  parent->PostMessage(IDM_AUTOFIT,0);
}

void CProfil::EvLButtonDown(UINT,TPoint& Point) {
  ButtonDown(Point,1);
}

void CProfil::EvRButtonDown(UINT,TPoint& Point) {
  ButtonDown(Point,2);
}

void CProfil::ButtonDown(TPoint& Point,int nr) {
  double aktpos;
  if (horiz) aktpos=endx*(double(Point.x)/Attr.W);
  else aktpos=endy*(double(Point.y)/Attr.H);
  if (move>0) {
    movenr=3;
    NewPos(move,nr,aktpos);
  } else {
    if (fabs(aktpos-pos1[nr-1])<fabs(aktpos-pos2[nr-1])) {
  	   move=1;
      movenr=nr;
      NewPos(move,nr,aktpos);
    } else {
	   move=2;
      movenr=nr;
      NewPos(move,nr,aktpos);
    }
  }
}

void CProfil::EvMouseMove(UINT modKeys, TPoint& Point) {
  double aktpos;
  if (horiz) aktpos=endx*(double(Point.x)/Attr.W);
  else aktpos=endy*(double(Point.y)/Attr.H);
  if ((movenr & 1)==1) NewPos(move,1,aktpos);
  if ((movenr & 2)==2) NewPos(move,2,aktpos);
}

void CProfil::EvKillFocus(HWND hWndGetFocus) {
  move=0;
}

void CProfil::GetCursor(double &apos1, double &apos2,int nr) {
  if (!picture) return;
  if (horiz) {
	 apos1=(*picture)->GetEichPosX(pos1[nr]);
	 apos2=(*picture)->GetEichPosX(pos2[nr]);
  } else {
	 apos1=(*picture)->GetEichPosY(pos1[nr]);
	 apos2=(*picture)->GetEichPosY(pos2[nr]);
  }
}

int CProfil::GetProfilPos(double xpos) {
  int h=xpos*length;
  if (h>length) h=length;
  if (h<0) h=0;
  return h;
}

void CProfil::GetList(double * &x, double * &y,int winheight,int winwidth, int &ndata, double &x0,
							 double &sigma, double &height, double &offset) {
  if (!profilbuf) return;
  if (!picture) return;
  int s,e,sinner,einner;
  if (horiz) {
    double mw=(*picture)->GetMaxUsefulWidth(winheight, winwidth);
	 s=GetProfilPos(mw*pos1[1]);
	 sinner=GetProfilPos(mw*pos1[0]);
	 e=GetProfilPos(mw*pos2[1]);
	 einner=GetProfilPos(mw*pos2[0]);
  } else {
    double mh=(*picture)->GetMaxUsefulHeight(winheight, winwidth);
	 s=GetProfilPos(1-mh*pos2[1]);
	 sinner=GetProfilPos(1-mh*pos2[0]);
	 e=GetProfilPos(1-mh*pos1[1]);
	 einner=GetProfilPos(1-mh*pos1[0]);
  }
  ndata=e-s;
  try {
	  x=new double[ndata];
     #if defined(DetectLeaks)
       LeakDetect.New(x,3,5);
     #endif
	  y=new double[ndata];
     #if defined(DetectLeaks)
       LeakDetect.New(y,3,6);
     #endif
  }
  catch (xalloc) {
  		MessageBox("CProfil::GetList : couldn't reserve memory","Error",  MB_OK);
      exit(-1);
  }
  int iheight=0;
  int hpos=0;
  double OffsetVert=0;
  long OffsetCount=0;
  for (int i=s;i<e;i++) {
	 x[i-s]=double(i)*Eich-Offset*Eich;
	 y[i-s]=(double(profilbuf[i])/(65536.0*SHIFT)-OffsetH)*EichH;
	 if (profilbuf[i]>iheight) {
		iheight=profilbuf[i];
		hpos=i;
	 }
	 if ((i<sinner) || (i>einner)) {
		 OffsetVert=OffsetVert+profilbuf[i];
		 OffsetCount++;
	 }
  }
  if (OffsetCount>0) OffsetVert=OffsetVert/OffsetCount;
  int vgl=(iheight-OffsetVert)*0.6+OffsetVert;
  height=(double(iheight)/(65536.0*SHIFT)-OffsetH)*EichH;
  x0=(hpos-Offset)*Eich;
  int lx=hpos;
  while ((lx>0) && (profilbuf[lx]>vgl)) lx--;
  int rx=hpos;
  while ((rx<length) && (profilbuf[rx]>vgl)) rx++;
  sigma=Eich*double(rx-lx)/2.0;
  offset=(double(OffsetVert)/(65536.0*SHIFT)-OffsetH)*EichH;
}

void CProfil::Save(const char *name,int winheight,int winwidth) {
  double *x;
  double *y;
  double x0,sigma,height,offset;
  int ndata;
  char filename[256];
  GetList(x,y,winheight,winwidth,ndata,x0,sigma,height,offset);
  strcpy(filename,name);
  while (filename[strlen(filename)-1]==' ') filename[strlen(filename)-1]=0;
  if (strchr(filename,'.')) {
    if (horiz) strcpy(strchr(filename,'.'),"X.DAT");
    else strcpy(strchr(filename,'.'),"Y.DAT");
  } else {
    if (horiz) strcat(filename,"X.DAT");
    else strcat(filename,"Y.DAT");
  };
  ofstream out(filename);
  out<<"X Y";
  if (DisplayGeneralFit==1) {
    out<<" GeneralFit";
  }
  out<<endl;
  for (int i=0;i<ndata;i++) {
    out<<x[i]<<' '<<y[i];
    if (DisplayGeneralFit==1) {
      out<<' '<<GeneralProfilFunc(x[i],GeneralProfilParams);
    }
    out<<endl;
  }
  #if defined(DetectLeaks)
    LeakDetect.Delete(x);
  #endif  
  delete[] x;
  #if defined(DetectLeaks)
    LeakDetect.Delete(y);
  #endif
  delete[] y;
}
