#include "textbox.h"
#include <groupbox.h>
#include <stdio.h>
#include <color.h>
#include <gdiobjec.h>

//*** CTextBox ***
CTextBox::CTextBox(TWindow* aparent,int x, int y, int w, int h,int dx,int dy,
  const char *title,int anx, int any,int antot,char **atext,bool avisible)
:TWindow(aparent,0,0) {
  visible=avisible;
  Attr.X=x;
  Attr.Y=y;
  Attr.W=w;
  Attr.H=h;
  new TGroupBox(aparent,0,"",x-1, y-7,w+2,h+13);
  nx=anx;
  ny=any;
  ntot=antot;
  text=atext;
  const ry=2;
  const rx=4;
  int n=0;
  int d;
  new TStatic(this,0,"",0,0,w,h);
  if (title!="") {
	 new TStatic(this,0,title,rx,ry,w-rx,dy);
	 d=1;
  } else d=0;
  Texte=new TStatic *[ntot];
  for (int i=0;i<ny;i++) for (int j=0;j<nx;j++) {
	 char buf[100];
	 sprintf(buf,text[n],0.0);
	 if (n<ntot) Texte[n]=new TStatic(this,0,buf,rx+j*dx,ry+(i+d)*dy,dx,dy);
	 n++;
  }
}

CTextBox::~CTextBox() {
}

void CTextBox::SetText(int nr, char *atext) {
  if (nr<ntot) {
    char buf[200];
    strcpy(buf,atext);
    buf[8]=0;
    strcat(buf,"=%.2f");
  //  strcpy(text[nr],buf);
    Texte[nr]->SetText("");
  }
}

void CTextBox::SetValue(int nr, double value) {
  if (nr<ntot) {
	 char buf[100];
	 sprintf(buf,text[nr],value);
	 Texte[nr]->SetText(buf);
  }
}

void CTextBox::Paint(TDC& adc, bool a, TRect& arect) {
  if (visible) TWindow::Paint(adc,a,arect);
}

