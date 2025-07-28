#include "profil.h"
#include <math.h>
#include "vision.rh"
#include "fitmath.h"

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
  if (horiz) pos=(*picture)->MaxY(); else pos=(*picture)->MaxX();
  (*picture)->GetProfil(profilbuf,pos,EichH,OffsetH,Eich,horiz,length,winheight,winwidth);
//  new TGroupBox(parent,0,"",x-1, y-1,w+2,h+2);
  DisplayFit=0;
  Amp=0;
  X0=0;
  Sigma=0;
  Offset=0;
  Slope=0;
  design=1;
}

CProfil::~CProfil() {
  if (profilbuf) delete profilbuf;
}

void CProfil::Paint(TDC& adc, bool a, TRect& arect) {
  TWindow::Paint(adc,a,arect);
  Draw();
}

void CProfil::Draw() {
  TClientDC dc(*this);
  TBrush *brush;
  if (design) brush=new TBrush(TColor(255,255,255));
  else brush=new TBrush(TColor(0,0,0));
  dc.SelectObject(*brush);
  dc.FillRect(TRect(0,0,Attr.W,Attr.H),*brush);
  delete brush;
  Grid();
  ShowFit();
  DrawProfil();
  Cursor();
}

void CProfil::Update(double apos) {
  pos=apos;
  (*picture)->GetProfil(profilbuf,pos,EichH,OffsetH,Eich,horiz,length,winheight,winwidth);
  Draw();
}

void CProfil::GetSum(double apos1, double apos2) {
  (*picture)->GetSum(profilbuf,apos1,apos2,EichH,OffsetH,Eich,horiz,length,winheight,winwidth);
  Draw();
}

unsigned char CProfil::GetPixel(double x) {
  unsigned int y1=x*length;
  if (y1>=length) y1=length-1;
  return profilbuf[y1]/(256*SHIFT);
}

void CProfil::ToggleDesign() {
  if (design) design=0; else design=1;
  Draw();
}

void CProfil::DrawProfil() {
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

void CProfil::Grid() {
  if (gridon) {
	 TClientDC dc(*this);
	 TPen *pen;
	 if (design) pen=new TPen(TColor(0,0,0));
	 else pen=new TPen(TColor(255,255,255));
	 dc.SelectObject(*pen);
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
}

double SumPos1;
double SumPos2;
double SumDelta;
double Dist2DParams[10];
double (*DisplayFunc)(double, double, double []);

void CProfil::ShowFit() {
  if (DisplayFit==1) {
	 TClientDC dc(*this);
	 TPen pen(TColor(255,0,0));
	 dc.SelectObject(pen);
	 if (horiz) {
		double a=X0/Sigma;
		dc.MoveTo(0,Attr.H-(Amp*exp(-0.5*a*a)+Offset)*Attr.H);
		for (int i=0;i<Attr.W;i++) {
        double x=double(i)/Attr.W;
		  a=(x-X0)/Sigma;
		  dc.LineTo(i,Attr.H-(Amp*exp(-0.5*a*a)+Offset+Slope*x)*Attr.H);
		}
	 } else {
      double x=1;
		double a=(x-X0)/Sigma;
		dc.MoveTo(Attr.W-(Amp*exp(-0.5*a*a)+Offset+Slope*x)*Attr.W,0);
		for (int i=0;i<Attr.H;i++) {
        double x=double(Attr.H-i)/Attr.H;
		  a=(x-X0)/Sigma;
		  dc.LineTo(Attr.W-(Amp*exp(-0.5*a*a)+Offset+Slope*x)*Attr.W,i);
		}
	 }
  }
  if (DisplayFit==2) {
	 TClientDC dc(*this);
	 TPen pen(TColor(255,0,0));
	 dc.SelectObject(pen);
	 if (horiz) {
		double a=X0/Sigma;
		dc.MoveTo(0,Attr.H-(Amp*exp(-a)+Offset)*Attr.H);
		for (int i=0;i<Attr.W;i++) {
        double x=double(i)/Attr.W;
		  a=fabs(x-X0)/Sigma;
		  dc.LineTo(i,Attr.H-(Amp*exp(-a)+Offset+Slope*x)*Attr.H);
		}
	 } else {
      double x=1;
		double a=fabs(1-X0)/Sigma;
		dc.MoveTo(Attr.W-(Amp*exp(-a)+Offset+Slope*x)*Attr.W,0);
		for (int i=0;i<Attr.H;i++) {
        x=double(Attr.H-i)/Attr.H;
		  a=fabs(x-X0)/Sigma;
		  dc.LineTo(Attr.W-(Amp*exp(-a)+Offset+Slope*x)*Attr.W,i);
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
}

void CProfil::GridOnOff() {
  if (gridon) {
	 gridon=0;
	 Update(pos);
  } else {
	 gridon=1;
	 Grid();
  }
}

double CProfil::XLocalToReal(int x) {
  return (*picture)->GetRealPosX(double(x)/Attr.W,winwidth,winheight)/1000 ;

}

double CProfil::YLocalToReal(int y) {
  return (*picture)->GetRealPosY(double(y)/Attr.H,winwidth,winheight)/1000 ;
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

void CProfil::SetGauss(double aAmp,double aX0, double aSigma, double aOffset,double aSlope) {
  Amp=aAmp/EichH;
  Offset=(aOffset-0.5*aSlope*Eich*length)/EichH+OffsetH;
  Slope=aSlope/EichH*(Eich*length);
  X0=(aX0+length*0.5*Eich)/Eich/length;
  Sigma=aSigma/(Eich*length);
  DisplayFit=1;
  Draw();
}

void CProfil::SetExponential(double aAmp,double aX0, double aSigma, double aOffset, double aSlope) {
  Amp=aAmp/EichH;
  Offset=(aOffset-0.5*aSlope*Eich*length)/EichH+OffsetH;
  Slope=aSlope/EichH*(Eich*length);
  X0=(aX0+length*0.5*Eich)/Eich/length;
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
  Update(pos);
}

void CProfil::NewPos(double apos11, double apos21,double apos12, double apos22) {
  Cursor();
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

void CProfil::GetRealPos(double &apos1, double &apos2,int nr) {
  if (horiz) {
    apos1=(*picture)->GetRealPosX(pos1[nr],winwidth,winheight);
    apos2=(*picture)->GetRealPosX(pos2[nr],winwidth,winheight);
  } else {
    apos1=(*picture)->GetRealPosY(pos1[nr],winwidth,winheight);
    apos2=(*picture)->GetRealPosY(pos2[nr],winwidth,winheight);
  }
//  apos1=(pos1[nr]-0.5)*length*Eich;
//  apos2=(pos2[nr]-0.5)*length*Eich;
}

void CProfil::Cursor() {
  TClientDC dc(*this);
  dc.SetROP2(R2_XORPEN);
  for (int nr=0;nr<2;nr++) {
    TPen *pen;
    if (nr==0) pen=new TPen(TColor(255,255,0));
    else pen=new TPen(TColor(0,255,255));
    dc.SelectObject(*pen);
    if (horiz) {
    	dc.MoveTo(pos1[nr]*Attr.W,0);
	   dc.LineTo(pos1[nr]*Attr.W,Attr.H);
	   dc.MoveTo(pos2[nr]*Attr.W,0);
	   dc.LineTo(pos2[nr]*Attr.W,Attr.H);
    } else {
	   dc.MoveTo(0,pos1[nr]*Attr.H);
	   dc.LineTo(Attr.W,pos1[nr]*Attr.H);
	   dc.MoveTo(0,pos2[nr]*Attr.H);
	   dc.LineTo(Attr.W,pos2[nr]*Attr.H);
    }
    delete pen;
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
  if (horiz) aktpos=double(Point.x)/Attr.W;
  else aktpos=double(Point.y)/Attr.H;
  switch (move) {
	 case 1:NewPos(aktpos,pos2[nr-1],nr-1);break;
	 case 2:NewPos(pos1[nr-1],aktpos,nr-1);break;
  }
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
  if (horiz) aktpos=double(Point.x)/Attr.W;
  else aktpos=double(Point.y)/Attr.H;
  if (move>0) {
    movenr=3;
    if (move=1) NewPos(aktpos,pos1[nr-1],nr-1);
    else NewPos(aktpos,pos2[nr-1],nr-1);
  } else {
    if (fabs(aktpos-pos1[nr-1])<fabs(aktpos-pos2[nr-1])) {
  	   move=1;
      movenr=nr;
	   NewPos(aktpos,pos2[nr-1],nr-1);
    } else {
	   move=2;
      movenr=nr;
	   NewPos(pos1[nr-1],aktpos,nr-1);
    }
  }
}

void CProfil::EvMouseMove(UINT modKeys, TPoint& Point) {
  double aktpos;
  if (horiz) aktpos=double(Point.x)/Attr.W;
  else aktpos=double(Point.y)/Attr.H;
  switch (move) {
	 case 1:
      if ((movenr & 1)==1) NewPos(aktpos,pos2[0],0);
      if ((movenr & 2)==2) NewPos(aktpos,pos2[1],1);
    break;
	 case 2:
      if ((movenr & 1)==1) NewPos(pos1[0],aktpos,0);
      if ((movenr & 2)==2) NewPos(pos1[1],aktpos,1);
    break;
  }
}

void CProfil::EvKillFocus(HWND hWndGetFocus) {
  move=0;
}

void CProfil::GetCursor(double &apos1, double &apos2,int nr) {
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

void CProfil::GetList(double * &x, double * &y, int &ndata, double &x0,
							 double &sigma, double &height, double &offset) {
  int s,e;
  if (horiz) {
	 s=GetProfilPos(pos1[0]);
	 e=GetProfilPos(pos2[0]);
  } else {
	 s=GetProfilPos(1-pos2[0]);
	 e=GetProfilPos(1-pos1[0]);
  }
  ndata=e-s;
  x=new double[ndata];
  y=new double[ndata];
  int iheight=0;
  int hpos=0;
  for (int i=s;i<e;i++) {
	 x[i-s]=double(i)*Eich-length*0.5*Eich;
	 y[i-s]=(double(profilbuf[i])/(65536.0*SHIFT)-OffsetH)*EichH;
	 if (profilbuf[i]>iheight) {
		iheight=profilbuf[i];
		hpos=i;
	 }
  }
  int vgl=iheight*0.6;
  height=(double(iheight)/(65536.0*SHIFT)-OffsetH)*EichH;
  x0=(hpos-length/2)*Eich;
  int lx=hpos;
  while ((lx>0) && (profilbuf[lx]>vgl)) lx--;
  int rx=hpos;
  while ((rx<length) && (profilbuf[rx]>vgl)) rx++;
  sigma=Eich*double(rx-lx)/2.0;
  offset=OffsetH;
}

void CProfil::Save(const char *name) {
  double *x;
  double *y;
  double x0,sigma,height,offset;
  int ndata;
  char filename[256];
  GetList(x,y,ndata,x0,sigma,height,offset);
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
  out<<"X Y"<<endl;
  for (int i=0;i<ndata;i++) out<<x[i]<<' '<<y[i]<<endl;
  delete x;
  delete y;
}
