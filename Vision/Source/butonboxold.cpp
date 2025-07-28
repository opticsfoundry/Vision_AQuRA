#include "butonbox.h"
#include <groupbox.h>
#include <CheckBox.h>
#include <RADIOBUT.H>


//*** CButtonBox ***
CButtonBox::CButtonBox(TWindow* aparent,int ID,int x, int y, int w, int h,int anr,	char **atext,bool aradio)
:TGroupBox(aparent,0,"",x,y,w,h) {
  radio=aradio;
  nr=anr;
  const ButH=15;
  const R=10;
  /*if (radio) Button=new TRadioButton *[nr];
  else */Button=new TCheckBox *[nr];
  if (h>w) {
	 for (int i=0;i<nr;i++)
      if (radio) Button[i]=new TRadioButton(aparent,ID,atext[i],x+R,y+R+i*ButH,w-2*R,ButH,this);
      else Button[i]=new TCheckBox(aparent,ID,atext[i],x+R,y+R+i*ButH,w-2*R,ButH,this);
  } else {
	 int ButW=(w-2*R)/nr;
	 for (int i=0;i<nr;i++)
		if (radio) Button[i]=new TRadioButton(aparent,ID,atext[i],x+i*ButW+R,y+R/2,ButW,h-R,this);
      else Button[i]=new TCheckBox(aparent,ID,atext[i],x+i*ButW+R,y+R/2,ButW,h-R,this);
  }
}

void CButtonBox::SetCheck(int anr, int value) {
  if (anr<=nr) Button[anr-1]->SetCheck(value);
}

int CButtonBox::GetChecked() {
  int h=0;
  if (radio) {
    for (int i=0;i<nr;i++) if (Button[i]->GetCheck()==BF_CHECKED) h=i;
  } else {
    for (int i=0;i<nr;i++) if (Button[i]->GetCheck()==BF_CHECKED) h|=(1 << i);
  }
  return h;
}
