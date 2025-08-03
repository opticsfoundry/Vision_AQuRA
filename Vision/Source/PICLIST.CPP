#include "piclist.h"
#include <groupbox.h>
#include <stdio.h>

//*** CPictureList ***
CPictureList::CPictureList(TWindow* aparent,TMessageBar *amb,int PicListID,
    int x, int y, int w, int h, int th,int nrX,CPicture *prototyp,CColorTable *colortable,
    CPictureList **aPictureList,int aNrPictureLists) {
  const D2=10;
  const D3=6;
  int height=h+D3;
  AktPicNr=0;
  PicNrY=th/(h+D3);
  PicNrX=nrX;
  PicNr=PicNrX*PicNrY;
  PictureWin=new CPictureButton* [PicNr];
  Pictures=new CPicture* [PicNr];
  TextList=new TStatic* [PicNr];
  TitleList=new TStatic* [PicNr];
  NameList=new char* [PicNr];
  OldAkt=-1;
  for (int i=0;i<PicNrX;i++) for (int j=0;j<PicNrY;j++) {
	 Pictures[j+PicNrY*i]=new CPicture(*prototyp);
	 TextList[j+PicNrY*i]=new TStatic(aparent,0,"",x+(w+D3)*i,y+j*height,D2,15);
    TitleList[j+PicNrY*i]=new TStatic(aparent,0,"",x+D2+(w+D3)*i,y+(j+1)*height-D2-D3,w-D2,15);
    NameList[j+PicNrY*i]=NULL;
    PictureWin[j+PicNrY*i]=new CPictureButton(aparent,amb,x+D2+(w+D3)*i,y+j*height,w-D2,h-D2,&Pictures[j+PicNrY*i],colortable,PicListID,j+PicNrY*i);
  }
  SyncList=aPictureList;
  NrPictureLists=aNrPictureLists;
}

CPictureList::~CPictureList() {
  for (int i=0;i<PicNrX;i++) for (int j=0;j<PicNrY;j++) {
    delete Pictures[j+PicNrY*i];
    delete TitleList[j+PicNrY*i];
    delete TextList[j+PicNrY*i];
    delete PictureWin[j+PicNrY*i];
    if (NameList[j+PicNrY*i]) delete NameList[j+PicNrY*i];
  }
  delete[] NameList;
  delete[] PictureWin;
  delete[] Pictures;
  delete[] TextList;
  delete[] TitleList;
}

void CPictureList::AddPicture(CPicture *picture, const char *name,
		const char *AtomName, const char *CameraDirection, bool NewLine) {
  if (PicNr>0) {
     /*if (OldAkt>=0) TextList[OldAkt]->SetText("");
	  TextList[AktPicNr]->SetText(">");
     OldAkt=AktPicNr;*/
    char buf[300];
    char buf2[300];
    char buf3[300];
    //error name=NULL occured once
    strcpy(buf,name);
    strcpy(buf2,buf);
    while (strchr(buf,'\\')>0) {
      strcpy(buf2,buf);
      strcpy(buf,strchr(buf,'\\')+1);
    }
    if ((strchr(buf,'.')>0) && strlen(buf)>6) {
      buf[strlen(buf)-7]=0;
      strcpy(buf3,buf);
      sprintf(buf,"%s",buf3);//,AtomName,CameraDirection);
      if ((buf2[0]=='S') && (strchr(buf2,'\\')>0)) {
        *strchr(buf2,'\\')=0;
        strcpy(buf3,buf);;
        sprintf(buf,"%s%s",buf3,buf2);
      }
      TitleList[AktPicNr]->SetText(buf);
    }

	 (*Pictures[AktPicNr])=(*picture);
    PictureWin[AktPicNr]->Draw(1);
    if (NameList[AktPicNr]) delete NameList[AktPicNr];
    NameList[AktPicNr]=new char[strlen(name)+1];
    strcpy(NameList[AktPicNr],name);
    if (NewLine) for (int i=0;i<NrPictureLists;i++) {
    	SyncList[i]->SetPosition(AktPicNr);
      SyncList[i]->SetMarker(AktPicNr);
      if (SyncList[i]!=this) SyncList[i]->ClearAktPos();
    }
	 AktPicNr++;
	 if (AktPicNr>=PicNr) AktPicNr=0;
  }
}

void CPictureList::SynchronizePosition() {
  for (int i=0;i<NrPictureLists;i++) SyncList[i]->SetPosition(AktPicNr);
}

int CPictureList::GetPosition() {
  return AktPicNr;
}

void CPictureList::SetPosition(double aPosition) {
  AktPicNr=aPosition;
  if (AktPicNr>=PicNr) AktPicNr=0;
}

void CPictureList::SetMarker(double aPosition) {
  if (OldAkt!=AktPicNr) {
    	 if (OldAkt>=0) TextList[OldAkt]->SetText("");
		 TextList[AktPicNr]->SetText(">");
       OldAkt=AktPicNr;
  }
}

void CPictureList::ClearAktPos() {
	 TitleList[AktPicNr]->SetText("");
	 Pictures[AktPicNr]->ClearBitmap();
    PictureWin[AktPicNr]->Draw(1);
    if (NameList[AktPicNr]) delete NameList[AktPicNr];
    NameList[AktPicNr]=new char[1];
    strcpy(NameList[AktPicNr],"");
}

const char* CPictureList::GetName(LPARAM nr) {
  if (nr>PicNr) return NULL;
  return NameList[nr];
}

void CPictureList::Update() {
  for (int i=0;i<PicNr;i++) {
    PictureWin[i]->Draw(1);
  }
}

