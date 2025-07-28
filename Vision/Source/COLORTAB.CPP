#include "colortab.h"
#include "BMP.h"
#include "vision.rh"
#include <math.h>
#include <dc.h>
#include "setup.h"

//*** CColorTable ***
DEFINE_RESPONSE_TABLE1(CColorTable,TWindow)
  EV_WM_LBUTTONDOWN,
  EV_WM_LBUTTONUP,
  EV_WM_RBUTTONUP,
  EV_WM_RBUTTONDOWN,
  EV_WM_MOUSEMOVE,
  EV_WM_KILLFOCUS,
END_RESPONSE_TABLE;

CColorTable::CColorTable(TWindow* aparent,int x, int y, int w, int h)
:TWindow(aparent,0,0) {
	LoadColorTable1();
   AktColorTableNr=0;
  palette16=NULL;
  parent=aparent;
  Attr.X=x;
  Attr.Y=y;
  Attr.W=w;
  Attr.H=h;
//  new TGroupBox(parent,0,"",x-1, y-1,w+2,h+2);
  try {
	  palette = new TColor[256];
	  ProfilPalette = new TColor[256];
  }
  catch (xalloc) {
  		MessageBox("CColorTable::CColorTable : couldn't reserve memory","Error",  MB_OK);
      exit(-1);
  }
  stripes=0;
  pos1=0;
  pos2=1;
  move=0;
  for (unsigned long i=0;i<256;i++) palette[i]=AktColorTable(i*256);
  WinPalette=NULL;
  CalcProfilPalette();
  palette8bit=NULL;
  LowByte=false;
}

CColorTable::~CColorTable() {
  delete[] ProfilPalette;
  delete[] palette;
  if (palette16) delete[] palette16;
  if (palette8bit) delete[] palette8bit;
}


void CColorTable::LoadColorTable1() {
 char buf2[200];
  sprintf(buf2,"%sVisionColorTable1.dat",MainDirectory);
  ifstream in(buf2);

  in>>NrColors;
  for (int i=0;i<NrColors;i++) {
	  for (int j=0;j<3;j++) {
	    in.getline(buf2,4096,' ');
	    colors[i][j]=atoi(buf2);
	  }
     in>>colors[i][3];
  }
  in.close();
}

void CColorTable::RefreshPalette() {
  LowByte=false;
  for (int i=0;i<256;i++) palette[i]=AktColorTable(i*256);
  if (stripes) AddStripes();
  CalcProfilPalette();
  palette8bit=NULL;
  Update();
}

void CColorTable::SetStandard() {
  AktColorTableNr=0;
  RefreshPalette();
}

void CColorTable::SetStandard2() {
  AktColorTableNr=3;
  RefreshPalette();
}

void CColorTable::SetBlackWhite() {
  AktColorTableNr=2;
  RefreshPalette();
}

void CColorTable::SetWhiteBlack() {
  AktColorTableNr=1;
  RefreshPalette();
}

void CColorTable::SetColorTableNr(int Nr) {
  AktColorTableNr=Nr;
  RefreshPalette();
}

int CColorTable::GetColorTableNr() {
  return AktColorTableNr;
}

void CColorTable::SetLowByte() {
  LowByte=true;
  for (int i=0;i<256;i++) palette[i]=AktColorTable(i*256);
  if (stripes) AddStripes();
  CalcProfilPalette();
  palette8bit=NULL;
  Update();
}

void CColorTable::AddStripes() {
  for (int i=0;i<256;i+=50)
	 for (int j=0;j<5;j++) if (i+j<256) palette[i+j]=TColor(255,255,255);
  CalcProfilPalette();
  Update();
  palette8bit=NULL;
  stripes=1;
}

void CColorTable::EvRButtonDown(UINT,TPoint& Point) {
/*  if (stripes) {
	 stripes=0;
	 RefreshPalette();
  } else AddStripes();
  */
  pos1=0;
  pos2=1;
  RefreshPalette();
  Update();
  parent->PostMessage(IDM_LOADPALETTE);
}

TColor *CColorTable::GetPalette16bit() {
  if (palette16) delete[] palette16;
  try {
	  palette16=new TColor[65536];
  }
  catch (xalloc) {
  		MessageBox("CColorTable::GetPalette16bit : couldn't reserve memory","Error",  MB_OK);
      exit(-1);
  }
  if (LowByte) {
    for (unsigned int i=0;i<65536;i++) palette16[i]=AktColorTable(i*256);
  } else {
    for (unsigned int i=0;i<65536;i++) palette16[i]=AktColorTable(i);
  }
  if (stripes) for (int i=0;i<256*256;i+=50*256)
	 for (int j=0;j<5*256;j++) if (i+j<256*256) palette16[i+j]=TColor(255,255,255);
  return palette16;
}

void CColorTable::MakePalette8bit() {
  PALETTEENTRY pal[256];
  for (int i=0;i<256;i++) {
    pal[i].peRed=palette[i].Red();
    pal[i].peBlue=palette[i].Blue();
    pal[i].peGreen=palette[i].Green();
    pal[i].peFlags=0;
  }
  if (palette8bit) delete palette8bit;
  palette8bit=new TPalette(pal,true);
}

TPalette *CColorTable::GetPalette8bit() {
  if (!palette8bit) MakePalette8bit();
  return palette8bit;
}

TColor CColorTable::AktColorTable(unsigned long col)
{
  switch (AktColorTableNr) {
    case 0:return ColorTable1(col);
    case 3:return ColorTable2(col);
    case 1:return ColorTableWhiteBlack(col);
    case 2:return ColorTableBlackWhite(col);
  }
  return ColorTable1(col);
}

TColor CColorTable::ColorTable1(unsigned long col)
{
  if (col>=0xFF00) return TColor(255,255,255);
  if (col<0x00FF) return TColor(128,128,128);
  if (col<(pos1*0xFFFF)) col=0;
  else if (col>(pos2*0xFFFF)) col=0xFFFF;
  else col=(col-pos1*0xFFFF)/(pos2-pos1);
  int i=0;
  while ((colors[i][0]*256<col) && (i<NrColors)) i++;
  if (colors[i][0]==col) return TColor(colors[i][1],colors[i][2],colors[i][3]);
  unsigned char r[3];
  for (int j=1;j<4;j++) r[j-1]=colors[i-1][j]+(colors[i][j]-colors[i-1][j])*(col/256.0-colors[i-1][0])/(colors[i][0]-colors[i-1][0]);
  return TColor(r[0],r[1],r[2]);
}

const int maxcol2=8;
const int colors2[maxcol2][4]={{0,0,0,0},{30,128,128,128},{82,0,0,255},{115,0,255,255},{147,0,255,0},{180,255,255,0},{220,255,0,0},{255,255,0,255}};

TColor CColorTable::ColorTable2(unsigned long col)
{
  if (col>=0xFF00) return TColor(255,255,255);
  if (col<0x00FF) return TColor(128,128,128);
  if (col<(pos1*0xFFFF)) col=0;
  else if (col>(pos2*0xFFFF)) col=0xFFFF;
  else col=(col-pos1*0xFFFF)/(pos2-pos1);
  int i=0;
  while ((colors2[i][0]*256<col) && (i<maxcol2)) i++;
  if (colors2[i][0]==col) return TColor(colors2[i][1],colors2[i][2],colors2[i][3]);
  unsigned char r[3];
  for (int j=1;j<4;j++) r[j-1]=colors2[i-1][j]+(colors2[i][j]-colors2[i-1][j])*(col/256.0-colors2[i-1][0])/(colors2[i][0]-colors2[i-1][0]);
  return TColor(r[0],r[1],r[2]);
}

TColor CColorTable::ColorTableBlackWhite(unsigned long col)
{
  if (col>=0xFF00) return TColor(255,0,0);
  if (col<0x00FF) return TColor(0,0,255);
  if (col<(pos1*0xFFFF)) col=0;
  else if (col>(pos2*0xFFFF)) col=0xFFFF;
  else col=(col-pos1*0xFFFF)/(pos2-pos1);
  int colo=col/256;
  return TColor(colo,colo,colo);
}

TColor CColorTable::ColorTableWhiteBlack(unsigned long col)
{
  if (col>=0xFF00) return TColor(255,0,0);
  if (col<0x00FF) return TColor(0,0,255);
  if (col<(pos1*0xFFFF)) col=0;
  else if (col>(pos2*0xFFFF)) col=0xFFFF;
  else col=(col-pos1*0xFFFF)/(pos2-pos1);
  int colo=255-(col/256);
  return TColor(colo,colo,colo);
}

TColor CColorTable::Color(unsigned char col) {
  return palette[col];
}

TColor CColorTable::ProfilColor(unsigned char col) {
  return ProfilPalette[col];
}

void CColorTable::Paint(TDC& adc, bool a, TRect& arect) {
  TWindow::Paint(adc,a,arect);
  Update();
}

void CColorTable::Update() {
try {
  TClientDC dc(*this);
  TRect Rec;
  GetWindowRect(Rec);
  for (int x=0; x<=Rec.Width(); x++)
  for (int y=0; y<=Rec.Height(); y++)
  dc.SetPixel(x,y,Color(255-y*255.0/Rec.Height()));
  }
  catch (TGdiBase::TXGdi& x) {
    MessageBeep(0xFFFFFFFF);
    MessageBeep(MB_ICONASTERISK);
    MessageBeep(MB_ICONEXCLAMATION);
    MessageBeep(MB_ICONHAND);
	 MessageBeep(MB_ICONQUESTION);
    MessageBeep(MB_OK);
     ofstream DebugFile("C:\\DebugPicture.dat", ios::app);
     bool found;
     string s=x.ResourceIdToString(&found,x.GetErrorCode());
	  DebugFile<<"GDI error CColorTable::Update ";
     if (found) DebugFile<<s;
     DebugFile<<endl;
	  DebugFile.close();
   }
}

void CColorTable::LoadPalette(const char* name) {
  LowByte=false;
  ReadBMPFile in(name,palette);
  CalcProfilPalette();
  Update();
}

void CColorTable::CalcProfilPalette() {
  PALETTEENTRY hWinPalette[256];
  for (int i=0;i<256;i++) {
	 hWinPalette[i].peRed=palette[i].Red();
	 hWinPalette[i].peGreen=palette[i].Green();
	 hWinPalette[i].peBlue=palette[i].Blue();
	 hWinPalette[i].peFlags=NULL;
  }
  if (WinPalette) delete WinPalette;
  WinPalette=new TPalette(hWinPalette,256);
  for (int i=0;i<256;i++) {
	 double r=palette[i].Red();
	 double g=palette[i].Green();
	 double b=palette[i].Blue();
	 double n=r*r+g*g+b*b;
	 if (n<255.0*255.0) {
		unsigned char m;
		if (n>0.5) m=147-sqrt(n)*0.57; else m=147;
		ProfilPalette[i]=TColor(r+m,g+m,b+m);
	 } else ProfilPalette[i]=palette[i];
  }
}

void CColorTable::NewPos(double apos1, double apos2) {
  if (apos1<0) apos1=0;
  if (apos2<0) apos2=0;
  if (apos1<0.9) pos1=apos1; else pos1=0.9;
  if (apos2>0.1) pos2=apos2; else pos2=0.1;
  if (pos2-pos1<0.1) pos2=pos1+0.1;
  RefreshPalette();
  Update();
}

void CColorTable::GetPos(double &apos1, double &apos2) {
  apos1=pos1;
  apos2=pos2;
}

void CColorTable::EvLButtonUp(UINT,TPoint& Point) {
  double aktpos;
  aktpos=1-double(Point.y)/Attr.H;
  switch (move) {
	 case 1:NewPos(aktpos,pos2);break;
	 case 2:NewPos(pos1,aktpos);break;
  }
  move=0;
  parent->PostMessage(IDM_LOADPALETTE);
}

void CColorTable::EvLButtonDown(UINT,TPoint& Point) {
  double aktpos;
  aktpos=1-double(Point.y)/Attr.H;
  if (fabs(aktpos-pos1)<fabs(aktpos-pos2)) {
	 move=1;
	 NewPos(aktpos,pos2);
  } else {
	 move=2;
	 NewPos(pos1,aktpos);
  }
}

void CColorTable::EvMouseMove(UINT modKeys, TPoint& Point) {
  double aktpos;
  aktpos=1-double(Point.y)/Attr.H;
  switch (move) {
	 case 1:NewPos(aktpos,pos2);break;
	 case 2:NewPos(pos1,aktpos);break;
  }
}

void CColorTable::EvKillFocus(HWND hWndGetFocus) {
  move=0;
}
