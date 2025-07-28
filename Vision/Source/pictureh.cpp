#include "picture.h"
#include "atommath.h"
#include <complex.h>
#include <math.h>
#include "bmp.h"

//*** CPicture ***
CPicture::CPicture(int ax,int ay, int axh, int ayh, int astepx, int astepy,
	 int aabsorption,double aEichX, double aEichY) {
  EichX=aEichX;
  EichY=aEichY;
  x=ax;y=ay;stepx=astepx;stepy=astepy;maxx=0;maxy=0;
  absorption=aabsorption;
  InitBitmap(axh,ayh);
  WinBitmap=NULL;
  MakeWinBitmap();
}

void CPicture::MakeWinBitmap() {
//  if (WinBitmap) delete WinBitmap;
//  char far* hbitmap= new char[xh*yh];
//  for (int i=0;i<xh;i++) for (int j=0;j<yh;j++) hbitmap[i+xh*j]=bitmap[i][j];
 // WinBitmap=new TBitmap(xh,yh,1,8,hbitmap);
//  delete hbitmap;
}

CPicture::CPicture(const CPicture &pic) {
  InitBitmap(pic.xh,pic.yh);
  Copy(pic);
}

void CPicture::Copy(const CPicture &pic) {
  x=pic.x;y=pic.y;stepx=pic.stepx;stepy=pic.stepy;maxx=pic.maxx;maxy=pic.maxy;
  absorption=pic.absorption;EichX=pic.EichX;EichY=pic.EichY;
  for (int i=0;i<xh;i++) for (int j=0;j<yh;j++) bitmap[i][j]=pic.bitmap[i][j];
}

CPicture::~CPicture() {
  DeleteBitmap();
}

void CPicture::InitBitmap(int axh,int ayh) {
  xh=axh;yh=ayh;
  bitmap = new long [xh*yh];
  for (long j = 0; j < yh*ayh; j++) bitmap[j]=0;
}

void CPicture::DeleteBitmap() {
  delete[] bitmap;
  bitmap=NULL;
}

long CPicture::GetPixel(double x, double y) {  //x,y aus [0..1]
  unsigned int x1=x*xh;
  if (x1>=xh) x1=xh-1;
  unsigned int y1=y*yh;
  if (y1>=yh) y1=yh-1;
  return bitmap[x1+y1*xh];
}

void CPicture::GetProfil(long * &profilbuf,double pos,double &Eich,
    int horiz,int &length) {
  if (profilbuf!=NULL) delete profilbuf;
  if (horiz) {
	 Eich=EichX;
	 length=xh;
	 profilbuf=new long[xh];
	 unsigned long y1=pos*yh;
	 if (y1>=yh) y1=yh-1;
    y1*=xh;
	 for (unsigned long i=0;i<xh;i++) profilbuf[i]=bitmap[i+y1];
  } else {
	 Eich=EichY;
	 length=yh;
	 profilbuf=new long[yh];
	 unsigned long x1=pos*xh;
	 if (x1>=xh) x1=xh-1;
	 for (unsigned long i=(yh-1)*xh;i>=0;i-=xh) profilbuf[i]=bitmap[x1+i];
  }
}

int CPicture::PictureGood() {
  int good=0;
  for (unsigned long i=0;i<xh;i++) if (bitmap[i]!=0) good=1;
  return good;
}

void CPicture::GetSum(int * &profilbuf,double pos1, double pos2,
   double &Eich,int horiz,int &length, long &min, long &max) {
  if (profilbuf!=NULL) delete profilbuf;
  if (horiz) {
	 Eich=EichX;
	 length=xh;
	 profilbuf=new long[xh];
	 int s=pos1*(yh-1);
	 int e=pos2*(yh-1);
	 if (s>=yh) s=yh-1;
	 if (e>=yh) e=yh-1;
	 max=-2000000000;
	 min=2000000000;
	 for (unsigned long i=0;i<xh;i++) {
		profilbuf[i]=0;
		for (int j=s*xh;j<=e*xh;j+=xh)  profilbuf[i]+=bitmap[i+j];
		profilbuf[i]/=(e-s+1);
		if (profilbuf[i]>max) max=profilbuf[i];
		if (profilbuf[i]<min) min=profilbuf[i];
	 }
	 /*if (min>0) min=0;
	 if (max-min<0.01) max=min+0.01;
	 if (max<0.01) max=0.01;
	 aOffsetH=-min/(max-min);
	 int offs=255.0*aOffsetH;
	 double factor=(255-offs)/max;
	 aEichH=EichH/factor;*/
  } else {
	 Eich=EichY;
	 length=yh;
	 profilbuf=new long[yh];
	 unsigned int s=pos1*(xh-1);
	 if (s>=xh) s=xh-1;
	 unsigned int e=pos2*(xh-1);
	 if (e>=xh) e=xh-1;
    max=-2000000000;
	 min=2000000000;
	 for (unsigned long i=0;i<yh;i++) {
		profilbuf[i]=0;
      unsigned long offset=yh-i-1;
		for (unsigned long j=s;j<=e;j++) profilbuf[i]+=bitmap[j+offset];
		profilbuf[i]/=(e-s+1);
		if (profilbuf[i]>max) max=profilbuf[i];
		if (profilbuf[i]<min) min=profilbuf[i];
	 }
/*	 if (min>0) min=0;
	 if (max-min<0.01) max=min+0.01;
	 if (max<0.01) max=0.01;
	 aOffsetH=-min/(max-min);
	 int offs=255.0*aOffsetH;
	 double factor=(255-offs)/max;
	 aEichH=EichH/factor;*/
  }
}

double CPicture::GetEichPixel(double x, double y) { //x in [0..1] Rueckgabe in [0..1]
  unsigned int x1=x*xh;
  if (x1>=xh) x1=xh-1;
  unsigned int y1=y*yh;
  if (y1>=yh) y1=yh-1;
  return double(bitmap[x1+y1*xh])/(64*65536);
}

void CPicture::ReadPicture(int quadrant, CFrameGrabber &framegrabber,
  double xmin, double xmax, double ymin, double ymax) {
  for (int i=0;i<xh;i++)
  for (int j=0;j<yh;j++)
	 bitmap[i][j]=framegrabber.GetPixel(quadrant,absorption,x+i*stepx,y+j*stepy);
  SearchMaxXMaxY(xmin,xmax,ymin,ymax);
  MakeWinBitmap();
}

void CPicture::SearchMaxXMaxY(double axmin, double axmax, double aymin, double aymax) {
  int xmin=axmin*(xh-2)+1;
  int ymin=aymin*(yh-2)+1;
  int xmax=axmax*(xh-2)+1;
  int ymax=aymax*(yh-2)+1;
  if (xmin<1) xmin=1; else if (xmin>xh-1) xmin=xh-1;
  if (ymin<1) ymin=1; else if (ymin>yh-1) ymin=yh-1;
  if (xmax<1) xmax=1; else if (xmax>xh-1) xmax=xh-1;
  if (ymax<1) ymax=1; else if (ymax>yh-1) ymax=yh-1;
  maxx=0;
  maxy=0;
  int max=0;
  for (int i=xmin;i<xmax;i++)
  for (int j=ymin;j<ymax;j++) {
	 int hmax=0;
	 for (int n=-1;n<=1;n++)
	 for (int m=-1;m<=1;m++) hmax+=bitmap[i+n][j+m];
	 if (hmax>max) {
		max=hmax;
		maxx=i;
		maxy=j;
	 }
  }
}

void CPicture::SetMaxXMaxY(double aMaxX, double aMaxY) {
  maxx=aMaxX*xh;
  maxy=aMaxY*yh;
}

double CPicture::MaxX() {
  return double(maxx)/xh;
}

double CPicture::MaxY() {
  return double(maxy)/yh;
}

double CPicture::GetEichPosX(double xpos) {
	return xpos*xh*EichX-xh*0.5*EichX;
}

double CPicture::GetEichPosY(double ypos) {
  return yh*0.5*EichY-ypos*yh*EichY;
}

int CPicture::GetBitmapPosX(double xpos) {
  int h=(xpos+xh*0.5*EichX)/EichX;
  if (h>xh) h=xh;
  if (h<0) h=0;
  return h;
}

int CPicture::GetBitmapPosY(double ypos) {
  int h=-(ypos-yh*0.5*EichY)/EichY;
  if (h>yh) h=yh;
  if (h<0) h=0;
  return h;
}

void CPicture::CalcNormalizedAmpX0SigmaX(double &AmpX, double &X0, double &SigmaX, double &Offset) {
  AmpX/=EichH;
  Offset=Offset/EichH+OffsetH;
  X0=(X0+xh*0.5*EichX)/EichX/xh;
  SigmaX/=EichX*xh;
}

void CPicture::CalcNormalizedAmpY0SigmaY(double &AmpY, double &Y0, double &SigmaY, double &Offset) {
  AmpY/=EichH;
  Offset=Offset/EichH+OffsetH;
  Y0=(Y0+yh*0.5*EichY)/EichY/yh;
  SigmaY/=EichY*yh;
}

void CPicture::CalcNormalizedX0Y0(double &X0, double &Y0) {
  Y0=1-(Y0+yh*0.5*EichY)/EichY/yh;
  X0=(X0+xh*0.5*EichX)/EichX/xh;
}

void CPicture::MandelPicture() {
  for (int i=0;i<xh;i++)
  for (int j=0;j<yh;j++)
  bitmap[i][j]=Mandel(i,j);
  SearchMaxXMaxY();
  MakeWinBitmap();
}

void CPicture::GaussPicture() {
  double xhd=xh;
  double yhd=yh;
  xhd*=xhd;
  yhd*=yhd;
  for (int i=0;i<xh;i++)
  for (int j=0;j<yh;j++) {
	 double x=(i-xh/2);
	 double y=(j-yh/2);
	 bitmap[i][j]=235*exp(-12*(x*x/xhd+y*y/yhd))+random(20);
  }
  SearchMaxXMaxY();
  MakeWinBitmap();
}

unsigned char CPicture::Mandel(int x,int y)
{
  double l=-2;
  double u=-1.3;
  double w=3;
  double h=2.5;
  double frontier=10000;
  complex z(l+w*x/xh,u+h*y/yh);
  complex c(z);
  int n=0;
  while ((n<63) && abs(z)<frontier)
  {
	 n++;
	 z=(z*z)+c;
  };
  return n*4;
};

int CPicture::Save(const char* name,CColorTable *ColorTab) {
  WriteBMPFile out(name,xh,yh,256,ColorTab->palette);
  if (!out.Open()) return -1;
  for (int i=yh-1;i>=0;i--) {
	 for (int j=0;j<xh;j++) out.Pixel(bitmap[j][i]);
	 out.WriteLine();
	 out.NewLine();
  }
  return 0;
}

int CPicture::Load(const char* name) {
  TColor *palette;
  palette = new TColor[256];
  ReadBMPFile in(name,palette);
  if (!in.Open()) return -1;
  delete palette;
  DeleteBitmap();
  InitBitmap(in.getBreite(),in.getHoehe());
  for (int i=yh-1;i>=0;i--) {
	 for (int j=0;j<xh;j++) bitmap[j][i]=in.getPixel(j);
	 if (!in.eof() && (i>0)) in.HoleZeile();
  }
  SearchMaxXMaxY();
  MakeWinBitmap();
  return 0;
}

CPicture operator-(const CPicture& pic1, const CPicture& pic2) {
  CPicture m(pic1.x,pic1.y,pic1.xh,pic1.yh,pic1.stepx,pic1.stepy,pic1.absorption,pic1.EichX,pic1.EichY,pic1.EichH,pic1.OffsetH);
  for (int i=0;i<pic1.xh;i++) for (int j=0;j<pic1.yh;j++) {
	 m.bitmap[i][j]=pic1.bitmap[i][j]-pic2.bitmap[i][j];
  }
  return m;
}

CPicture operator+(const CPicture& pic1, const CPicture& pic2) {
  CPicture m(pic1.x,pic1.y,pic1.xh,pic1.yh,pic1.stepx,pic1.stepy,pic1.absorption,pic1.EichX,pic1.EichY,pic1.EichH,pic1.OffsetH);
  for (int i=0;i<pic1.xh;i++) for (int j=0;j<pic1.yh;j++) {
	 m.bitmap[i][j]=pic1.bitmap[i][j]+pic2.bitmap[i][j];
  }
  return m;
}

CPicture operator*(const CPicture& pic1, const CPicture& pic2) {
  CPicture m(pic1.x,pic1.y,pic1.xh,pic1.yh,pic1.stepx,pic1.stepy,pic1.absorption,pic1.EichX,pic1.EichY,pic1.EichH,pic1.OffsetH);
  for (int i=0;i<pic1.xh;i++) for (int j=0;j<pic1.yh;j++) {
	 m.bitmap[i][j]=pic1.bitmap[i][j]*pic2.bitmap[i][j];
  }
  return m;
}

CPicture operator/(const CPicture& pic1, const CPicture& pic2) {
  CPicture m(pic1.x,pic1.y,pic1.xh,pic1.yh,pic1.stepx,pic1.stepy
  ,pic1.absorption,pic1.EichX,pic1.EichY,pic1.EichH,pic1.OffsetH);
  for (int i=0;i<pic1.xh;i++) for (int j=0;j<pic1.yh;j++) {
	 if (pic2.bitmap[i][j]!=0) m.bitmap[i][j]=pic1.bitmap[i][j]/pic2.bitmap[i][j];
	 else m.bitmap[i][j]=255;
  }
  return m;
}

CPicture& CPicture::operator=(const CPicture& pic) {
  if (xh!=pic.xh || yh!=pic.yh) {
	 DeleteBitmap();
	 InitBitmap(pic.xh,pic.yh);
	 xh=pic.xh;
	 yh=pic.yh;
  }
  Copy(pic);
  return *this;
}

const CPicture CPicture::operator*(const double r) const {
  CPicture m(*this);
  for (int i=0;i<xh;i++) for (int j=0;j<yh;j++) {
	 m.bitmap[i][j]=r*bitmap[i][j];
  }
  return m;
}

void CPicture::NormalizeAbsorption(const CPicture &pic1,const CPicture &pic2,const CPicture &pic3,
  double xmin, double xmax, double ymin, double ymax) {
  for (int i=0;i<xh;i++)
  for (int j=0;j<yh;j++) {
	 if ((pic2.bitmap[i][j]-pic3.bitmap[i][j])!=0) {
		double h=double(pic1.bitmap[i][j]-pic3.bitmap[i][j])/double(pic2.bitmap[i][j]-pic3.bitmap[i][j]);
		h=1-h;
		h=h/EichH+OffsetH;
		if (h>1) h=1; else if (h<0) h=0;
		h=h*255*1;
		if (h>255) h=255;
		bitmap[i][j]=h;
	 } else bitmap[i][j]=255;
  }
  SearchMaxXMaxY(xmin,xmax,ymin,ymax);
  MakeWinBitmap();
}

void CPicture::Ln(const CPicture &pic1,const CPicture &pic2,const CPicture &pic3,
  double MaxDensity,double ProbeDetuning,
  double xmin, double xmax, double ymin, double ymax) {
  double factor=1/SigmaDet(ProbeDetuning);
  EichH=MaxDensity;
  for (int i=0;i<xh;i++)
  for (int j=0;j<yh;j++) {
	 double h;
	 if ((pic2.bitmap[i][j]-pic3.bitmap[i][j])!=0) {
		h=double(pic1.bitmap[i][j]-pic3.bitmap[i][j])/double(pic2.bitmap[i][j]-pic3.bitmap[i][j]);
		if (h<=0) h=0.000000001;
		h=-log(h)*factor;
		h=h/EichH+OffsetH;
		h=h*255.0;
		if (h>255) h=255; else if (h<0) h=0;
		bitmap[i][j]=h;
	 }
	 else bitmap[i][j]=255;
  }
  SearchMaxXMaxY(xmin,xmax,ymin,ymax);
  MakeWinBitmap();
}

void LnNormalizeAbsorption(const CPicture &pic1,const CPicture &pic2,const CPicture &pic3,CPicture &Abs,CPicture &Ln,
  double MaxDensity,double ProbeDetuning,
  double xmin, double xmax, double ymin, double ymax) {
  double factor=1/SigmaDet(ProbeDetuning);
  Ln.EichH=MaxDensity;
  for (int i=0;i<Ln.xh;i++)
  for (int j=0;j<Ln.yh;j++) {
	 double h,h2;
	 if ((pic2.bitmap[i][j]-pic3.bitmap[i][j])!=0) {
		h2=h=double(pic1.bitmap[i][j]-pic3.bitmap[i][j])/double(pic2.bitmap[i][j]-pic3.bitmap[i][j]);

		h2=1-h2;
		h2=h2/Abs.EichH+Abs.OffsetH;
		if (h2>1) h2=1; else if (h2<0) h2=0;
		h2=h2*255*1;
		if (h2>255) h2=255;
		Abs.bitmap[i][j]=h2;

		if (h<=0) h=0.000000001;
		h=-log(h)*factor;
		h=h/Ln.EichH+Ln.OffsetH;
		h=h*255.0;
		if (h>255) h=255; else if (h<0) h=0;
		Ln.bitmap[i][j]=h;
	 }
	 else {
		Ln.bitmap[i][j]=255;
		Abs.bitmap[i][j]=255;
	 }
  }
  Ln.SearchMaxXMaxY(xmin,xmax,ymin,ymax);
  Abs.SetMaxXMaxY(Ln.MaxX(),Ln.MaxY());
  Ln.MakeWinBitmap();
  Abs.MakeWinBitmap();
}

double CPicture::GetAbsorption(double x0,double y0) {
  if (EichX<1E-10) EichX=1;
  if (EichY<1E-10) EichY=1;
  int xp=(x0/EichX)+xh/2;
  if (xp<2) xp=2;
  if (xp>=xh-2) xp=xh-3;
  int yp=y0-(y0/EichY)+yh/2;
  if (yp<2) yp=2;
  if (yp>=yh-3) yp=yh-3;
  double sum=0;
  for (int i=-2;i<=2;i++) for (int j=-2;j<=2;j++) sum+=bitmap[xp+i][yp+j];
  sum/=25;
  return (sum/255.0-OffsetH)*EichH;
}

double CPicture::Integral(double xmin, double ymin, double xmax, double ymax) {
  int sx=xmin*xh;
  int ex=xmax*xh;
  int sy=ymin*yh;
  int ey=ymax*yh;
  if (ex<sx) ex=sx+1;
  if (ey<sy) ey=sy+1;
  if (sx>xh) sx=xh;
  if (ex>xh) ex=xh;
  if (sy>yh) sy=yh;
  if (ey>yh) ey=yh;
  if (sx<0) sx=0;
  if (sy<0) sy=0;
  if (ex<0) ex=0;
  if (ey<0) ey=0;
  double sum=0;
  for (int i=sx;i<ex;i++) for (int j=sy;j<ey;j++) sum+=double(bitmap[i][j]);
  sum/=double(ex-sx)*double(ey-sy);
  sum=(sum/255.0-OffsetH)*EichH;
  sum*=double(ex-sx)*double(ey-sy)*EichX*EichY;
  return sum;
}
