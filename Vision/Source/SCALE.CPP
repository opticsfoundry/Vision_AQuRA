#include "scale.h"
#include <string.h>
#include <dc.h>
#include <stdio.h>

//*** CScale ***
CScale::CScale(TWindow* parent,int x, int y, int aw, int ah, int as, int at,
  int aHoriz, int aSide, double aDistance, int aNumber, int aStartNumber, double aScale,
  double aOffset, double aWert0, char* atext,int atposx, int atposy)
		 :TWindow(parent) {
  text = new char[strlen(atext)];
  tposx=atposx;
  tposy=atposy;
  strcpy(text,atext);
  Attr.X=x;
  Attr.Y=y;
  Attr.W= (aw<15) ? 15 : aw;
  Attr.H= (ah<15) ? 15 : ah;
  S=as;
  T=at;
  Horiz=aHoriz;
  Side=aSide;
  Distance=aDistance;
  Number=aNumber;
  StartNumber=aStartNumber;
  Scale=aScale;
  Offset=aOffset;
  Wert0=aWert0;
  bigfont = new TFont("BIGFONT",-11);
};

DEFINE_RESPONSE_TABLE1(CScale, TWindow)
  EV_WM_ERASEBKGND,
END_RESPONSE_TABLE;

CScale::~CScale() {
  delete text;
  delete bigfont;
}

void CScale::Paint(TDC& adc, bool a, TRect& arec) {
  try {
  TWindow::Paint(adc,a,arec);
  if (text!="") adc.TextOut(tposx,tposy,text);
  TPen pen(TColor(0,0,0),2);
  TPen pen2(TColor(0,0,0),1);
  adc.SelectObject(pen);
  if (Horiz) {
	 double mult;
	 double Pos;
	 if (Side) {
		mult=1;
		Pos=0;
	 } else {
		mult=-1;
		Pos=Attr.H;
	 }
	 adc.MoveTo(0,Pos);
	 adc.LineTo(Attr.W,Pos);
	 double aktpos=Wert0;
	 double maxwert=Attr.W*Scale+Wert0;
	 int n=StartNumber;
	 while (((aktpos<maxwert) && (Distance>0)) || ((aktpos>maxwert) && (Distance<0))) {
		int pixelpos=abs((aktpos-Offset)/Scale);
		if (n==0) {
		  adc.SelectObject(*bigfont);
		  char buf[10];
		  sprintf(buf,"%.0f",aktpos);
		  adc.TextOut(pixelpos-5,Pos+mult*T,buf);
		}
		adc.SelectObject(pen2);
		adc.MoveTo(pixelpos,Pos);
		adc.LineTo(pixelpos,Pos+mult*S);
		aktpos+=Distance;
		n++;
		if (n>=Number) n=0;
	 }
  } else {
	 double mult;
	 double Pos;
	 double TPos;
	 if (Side) {
		mult=1;
		Pos=0;
		TPos=T;
	 } else {
		mult=-1;
		Pos=Attr.W;
		TPos=0;
	 }
	 adc.MoveTo(Pos,0);
	 adc.LineTo(Pos,Attr.H);
	 double aktpos=Wert0;
	 double maxwert=Attr.H*Scale+Wert0;
	 int n=StartNumber;
	 while (((aktpos<maxwert) && (Distance>0)) || ((aktpos>maxwert) && (Distance<0))) {
		int pixelpos=abs((aktpos-Offset)/Scale);
		if (n==0) {
		  adc.SelectObject(*bigfont);
		  char buf[10];
		  sprintf(buf,"%.0f",aktpos);
		  adc.TextOut(TPos,pixelpos-6,buf);
		}
		adc.SelectObject(pen2);
		adc.MoveTo(Pos,pixelpos);
		adc.LineTo(Pos+mult*S,pixelpos);
		aktpos+=Distance;
		n++;
		if (n>=Number) n=0;
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
	  DebugFile<<"GDI error CScale::Paint ";
     if (found) DebugFile<<s;
     DebugFile<<endl;
	  DebugFile.close();
   }
}

bool CScale::EvEraseBkgnd(HDC hDC)
{
  try {
  TDC dc(hDC);
  dc.TextRect(GetClientRect(), ::GetSysColor(COLOR_BTNFACE));
  return TRUE;
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
	  DebugFile<<"GDI error CScale::EvEraseBkgnd ";
     if (found) DebugFile<<s;
     DebugFile<<endl;
	  DebugFile.close();
     return TRUE;
   }
}

