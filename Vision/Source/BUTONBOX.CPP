#include "butonbox.h"
#include <math.h>
#include <groupbox.h>
#include <CheckBox.h>
#include <RADIOBUT.H>

//*** CButtonBox ***
CButtonBox::CButtonBox(TWindow* aparent,int ID,int x, int y, int w, int h,int alength,
    int anrblocks, int alistlength,char **atext,int *anrlist, bool aradio)
:TGroupBox(aparent,0,"",x,y-8,w,h+10) {
  radio=aradio;
  text=atext;
  length=alength;
  nrlist=anrlist;
  listlength=alistlength;
  nrblocks=anrblocks;
  const ButH=15;
  const ButW=15;
  const R=10;
  aktblock=0;
  if (radio) {
    RButton=new TRadioButton *[length];
    CButton=NULL;
  } else {
    CButton=new TCheckBox *[length];
    RButton=NULL;
  }
  Texte=new TStatic *[length];
  checked =new bool[listlength];
  for (int i=0;i<listlength;i++) checked[i]=false;
  if (h>w) {
	 for (int i=0;i<length;i++) {
      if (radio) RButton[i]=new TRadioButton(aparent,ID,"",x+R,y+R+i*ButH,ButW,ButH,this);
      else CButton[i]=new TCheckBox(aparent,ID,"",x+R,y+R+i*ButH,ButW,ButH,this);
      Texte[i]=new TStatic(aparent,ID,atext[nrlist[i]],x+5+R+ButW,y+R+i*ButH,20,ButH);
    }
  } else {
	 int ButW=(w-2*R)/length;
	 for (int i=0;i<length;i++) {
		if (radio) {
        RButton[i]=new TRadioButton(aparent,ID,atext[nrlist[i]],x+i*ButW+R,y+R/2,ButW,ButH,this);
        Texte[i]=NULL;
      } else {
        CButton[i]=new TCheckBox(aparent,ID,atext[nrlist[i]],x+i*ButW+R,y+R/2,ButW,ButH,this);
        Texte[i]=NULL;
      }
    }
  }
}

CButtonBox::~CButtonBox() {
 /* if (RButton) for (int i=0;i<length;i++) {
	  if (RButton[i]) delete RButton[i];
	  RButton[i]=NULL;
  }
  if (CButton) for (int i=0;i<length;i++) {
	  if (CButton[i]) delete CButton[i];
	  CButton[i]=NULL;
  }
  for (int i=0;i<length;i++) {
	if (Texte[i]) delete Texte[i];
	Texte[i]=NULL;
  }*/
  if (RButton) delete RButton;
  if (CButton) delete CButton;
  delete checked;
  delete Texte;
};

void CButtonBox::SetCheck(int anr, bool value) {
  if (anr<listlength) {
    checked[anr]=value;
    if (aktblock==anr/length) {
      if (radio) RButton[anr%length]->SetCheck(value);
      else CButton[anr%length]->SetCheck(value);
    }
  }
}

void CButtonBox::UpdateChecked() {
  for (int i=0;i<length;i++)
    if (aktblock*length+i<listlength) {
      if (radio) checked[aktblock*length+i]=RButton[i]->GetCheck()==BF_CHECKED;
      else checked[aktblock*length+i]=CButton[i]->GetCheck()==BF_CHECKED;
    }
}

unsigned long CButtonBox::GetChecked() {
  UpdateChecked();
  unsigned long h=0;
  if (radio) {
    for (int i=0;i<listlength;i++) if (checked[i]) h=i;
  } else {
    for (int i=0;i<listlength;i++) if (checked[i]) h|=(1 << i);
  }
  return h;
}

void CButtonBox::SetChecked(unsigned long Checked) {
  for (int i=0;i<listlength;i++) SetCheck(i,((Checked & (1 << i))>0));
}

void CButtonBox::GetCheckedAsString(char* buf) {
  for (int i=0;i<listlength;i++) buf[i]=(checked[i]) ? '1' : '0';
  buf[listlength]=0;
}

void CButtonBox::SetCheckedFromString(char* buf) {
  for (int i=0;i<listlength;i++) SetCheck(i,(buf[i]=='1') ? BF_CHECKED : BF_UNCHECKED);
}

void CButtonBox::SaveCheckedToStream(ofstream& out) {
  for (int i=0;i<listlength;i++) out<<((checked[i]) ? '1' : '0')<<endl;
}

void CButtonBox::LoadCheckedFromStream(ifstream& in) {
  for (int i=0;i<listlength;i++) {
    int h;
    in>>h;
    SetCheck(i,(h==1) ? BF_CHECKED : BF_UNCHECKED);
  }
}

void CButtonBox::Next() {
  UpdateChecked();
  int maxblock=ceil(listlength/length);
  if (aktblock<maxblock) {
    for (int i=0;i<length;i++) {
      if (((aktblock+1)*length+i)<listlength) {
        if (radio) RButton[i]->SetCheck(checked[nrlist[(aktblock+1)*length+i]]);
        else CButton[i]->SetCheck(checked[nrlist[(aktblock+1)*length+i]]);
        Texte[i]->SetText(text[nrlist[(aktblock+1)*length+i]]);
      } else {
        if (radio) RButton[i]->SetCheck(false);
        else CButton[i]->SetCheck(false);
        Texte[i]->SetText("");
      }
    }
    aktblock++;
  }
}

void CButtonBox::Last() {
  UpdateChecked();
  if (aktblock>0) {
    for (int i=0;i<length;i++) {
      if (radio) RButton[i]->SetCheck(checked[nrlist[(aktblock-1)*length+i]]);
      else CButton[i]->SetCheck(checked[nrlist[(aktblock-1)*length+i]]);
      Texte[i]->SetText(text[nrlist[(aktblock-1)*length+i]]);
    }
    aktblock--;
  }
}
