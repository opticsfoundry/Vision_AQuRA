#include "vision.rh"
#include "const.h"
#include "fitmath.h"
#include <stdio.h>
#include <math.h>
#include <groupbox.h>
#include <dir.h>
#include "measure.h"
#include "atommath.h"
#include "setup.h"
#include <alloc.h>
#include "leakdetect.h"

//*** CMessWindow ***
DEFINE_RESPONSE_TABLE1(CMessWindow,TWindow)
  EV_WM_LBUTTONDOWN,
  EV_WM_RBUTTONDOWN,
END_RESPONSE_TABLE;

CMessWindow::CMessWindow(TWindow* aparent,CName *anames,TMessageBar *amb,
  CButtonBox *aButtons,CButtonBox *aCameraButtons,TStatic *aAxText,int x, int y, int w, int h)
:TWindow(aparent,0,0) {
  AxText=aAxText;
  names=anames;
  mb=amb;
  name[0]=0;
  Buttons=aButtons;
  CameraButtons=aCameraButtons;
  MesName[0]=0;
  MesIsotop=0;
  MyName[0]=0;
  aktpoint=NULL;
  parent=aparent;
  Attr.X=x;
  Attr.Y=y;
  Attr.W=w;
  Attr.H=h;
//  new TGroupBox(parent,0,"",x-1, y-1,w+2,h+2);
  points=NULL;
  spoints=NULL;
  xmin=0;
  xmax=1;
  ymin=0;
  ymax=1;
  disk=DataDirectory[0];
  aktColor=0;
}

CMessWindow::~CMessWindow() {
  #if defined(DetectLeaks)
     LeakDetect.Delete(points);
  #endif
  if (points!=NULL) delete points;
}

void CMessWindow::ColorPoints() {
  if (points!=NULL && aktColor==0) points->SetColor(1);
  if (aktColor==0) aktColor=1;
}

void CMessWindow::Draw() {
try {
  TClientDC dc(*this);
  TBrush brush(TColor(255,255,255));
  dc.SelectObject(brush);
  dc.FillRect(TRect(0,0,Attr.W,Attr.H),brush);
  SetMaxXY();
	if (points!=NULL) {
   	CMyPoint *hpoint=points;
      while (hpoint) {
	   	hpoint->Draw(&dc,xmin,xmax,ymin,ymax,Attr.W,Attr.H,Buttons->GetChecked(),CameraButtons->GetChecked());
         hpoint=hpoint->next;
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
	  DebugFile<<"GDI error CMessWindow::Draw ";
     if (found) DebugFile<<s;
     DebugFile<<endl;
	  DebugFile.close();
   }
}

void CMessWindow::Paint(TDC& adc, bool a, TRect& arect) {
  TWindow::Paint(adc,a,arect);
  Draw();
}

void CMessWindow::New() {
  //error occured in following line
  #if defined(DetectLeaks)
     LeakDetect.Delete(points);
  #endif
  if (points!=NULL) delete points;
  points=NULL;
  spoints=NULL;
  aktpoint=NULL;
  xmin=0;
  ymin=0;
  xmax=0;
  ymax=0;
  aktColor=0;
  MyName[0]=0;
}

void CMessWindow::EvLButtonDown(UINT,TPoint& Point) {
  if (points!=NULL) {
	 if (aktpoint!=NULL) aktpoint->SetActive(0);
	 double ax=xmin+double(Point.x)/double(Attr.W)*(xmax-xmin);
	 double ay=ymin+double(Attr.H-Point.y)/double(Attr.H)*(ymax-ymin);
	 double h=1E10;
	 aktpoint=points->GetNearestPoint(xmax-xmin,ymax-ymin,ax,ay,Buttons->GetChecked(),CameraButtons->GetChecked(),h);
	 aktpoint->SetActive(1);
	 Draw();
	 ButtonAction();
  }
}

void CMessWindow::ShowNext() {
  if (points!=NULL) {
	 if (aktpoint) {
      if (aktpoint->next) {
        aktpoint->SetActive(0);
        aktpoint=aktpoint->next;
	     aktpoint->SetActive(1);
        Draw();
        ButtonAction();
      }
    } else {
      aktpoint=points;
      aktpoint->SetActive(1);
      Draw();
      ButtonAction();
    }
  }
}

void CMessWindow::ShowLast() {
  if (points!=NULL) {
	 if (aktpoint) {
      CMyPoint *last=aktpoint->GetLastPoint(points);
      if (last) {
        aktpoint->SetActive(0);
        aktpoint=last;
	     aktpoint->SetActive(1);
	     Draw();
	     ButtonAction();
      }
    } else {
      aktpoint=points;
	   aktpoint->SetActive(1);
	   Draw();
	   ButtonAction();
    }
  }
}

void CMessWindow::EvRButtonDown(UINT,TPoint& Point) {
  if (points!=NULL) {
	 double ax=xmin+double(Point.x)/double(Attr.W)*(xmax-xmin);
	 double ay=ymin+double(Attr.H-Point.y)/double(Attr.H)*(ymax-ymin);
	 double h=1E10;
	 CMyPoint *hpoint=points->GetNearestPoint(xmax-xmin,ymax-ymin,ax,ay,Buttons->GetChecked(),CameraButtons->GetChecked(),h);
	 hpoint->ToggleEnable();
    try {
	 TClientDC dc(*this);
    hpoint->Draw(&dc,xmin,xmax,ymin,ymax,Attr.W,Attr.H,Buttons->GetChecked(),CameraButtons->GetChecked());
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
	  DebugFile<<"GDI error CMessWindow::EvRButtonDown ";
     if (found) DebugFile<<s;
     DebugFile<<endl;
	  DebugFile.close();
   }
  }
}

void CMessWindow::DeleteAktPoint() {
   if (points!=NULL && aktpoint!=NULL) {
      aktpoint->SetActive(0);
      if (aktpoint!=points) {
         CMyPoint* helpPoint=aktpoint;
         aktpoint=aktpoint->next;
         if (aktpoint==NULL) aktpoint=points;
         points->DeletePoint(helpPoint);
      } else {
         CMyPoint *hpoint=points;
         points=points->next;
         spoints=points;
         hpoint->next=NULL;
         hpoint->hnext=NULL;
         #if defined(DetectLeaks)
         LeakDetect.Delete(hpoint);
         #endif
         delete hpoint;
         aktpoint=points;
      }
      Draw();
   }
}

void CMessWindow::SetMaxXY() {
  xmin=1E10;
  ymin=1E10;
  xmax=-1E10;
  ymax=-1E10;
  CMyPoint *HelpPoint;
  HelpPoint=points;
  if (points!=spoints) {
      MessageBox("CMessWindow::SetMaxXY : Error 0", "Error", MB_OK);
  }
  while (HelpPoint!=NULL) {
     //error correction seems necessary
     if ((HelpPoint->next==points) || (HelpPoint->next==HelpPoint)) {
     		HelpPoint->next=NULL;
         MessageBox("CMessWindow::SetMaxXY : Error 2", "Error", MB_OK);
     }
     if (points->next!=points->hnext) {
         MessageBox("CMessWindow::SetMaxXY : Error 1", "Error", MB_OK);
     }
  	  if (HelpPoint->next!=HelpPoint->hnext) {
         MessageBox("CMessWindow::SetMaxXY : Error 3", "Error", MB_OK);
     }
	  HelpPoint->GetMinMaxXY(xmin,ymin,xmax,ymax,Buttons->GetChecked(),CameraButtons->GetChecked());

     HelpPoint=HelpPoint->next;
  }
  if (ymin>0) ymin=0;
  if (xmax-xmin<1E-50) xmax=1E-50;
  if (ymax-ymin<1E-50) ymax=1E-50;
}

void CMessWindow::ShiftParameters() {
  if (points) {
    points->ShiftParameters();
    AxText->SetText(points->GetXName(0));
  }
  Draw();
}

//*** CTNMeasure ***
const ID_SigmaX=1;
const ID_SigmaY=2;
const ID_AmpX=4;
const ID_Abs=4;
const ID_AmpY=8;
const ID_OptDens=16;
const ID_NG=32;
const ID_NI=64;
const ID_X0=256;
const ID_Y0=512;
const NR_NG=5;
const NR_NI=6;

CTNMeasure::CTNMeasure(TWindow* aparent,CName *anames,TMessageBar *amb,
  CButtonBox *aButtons,CButtonBox *aCameraButtons,TStatic *aAxText,CTextBox *aMessDataTextBox,CTextBox *aTNFitTextBox,CTextBox *aGaussFitTextBox,int x, int y, int w,int h)
:CMessWindow(aparent,anames,amb,aButtons,aCameraButtons,aAxText,x,y,w,h) {
  FirstPoint=false;
  Masse=87*_mp;
  X0=0;
  Y0=0;
  SigmaX=0;
  SigmaY=0;
  Abs=0;
  OptDens=0;
  DeltaDetuning=0;
  TNFitTextBox=aTNFitTextBox;
  GaussFitTextBox=aGaussFitTextBox;
  MessDataTextBox=aMessDataTextBox;
  Tempfited=0;
  Nfited=0;
  ChargingTimefited=0;
  ChargingN[0]=1E6;
  ChargingT[0]=1000;
  ChargingN[1]=1E6;
  ChargingT[1]=1000;
  Lifetimefited=0;
  LifetimeN[0]=1E6;
  LifetimeN[1]=1E6;
  LifetimeT[0]=1000;
  LifetimeT[1]=1000;
  SNfited=0;
  SpeedX=0;
  Sigma0X=0.1;
  TempX=0;
  SpeedY=0;
  Sigma0Y=0.1;
  TempY=0;
  N=0;
  SN[0]=0;
  SN[1]=0;
  NoSerie=1;
  NSigmaX=0;
  NSigmaY=0;
//  MesProbeDetuning=0;
  MesName[0]=0;
  MesIsotop=0;
  LoadPoint=NULL;
}

void CTNMeasure::Draw() {
if (points!=spoints) {
      MessageBox("CTNMeasure::Draw : Error 0", "Error", MB_OK);
  }

  CMessWindow::Draw();
  int h=Buttons->GetChecked();
  if (Tempfited) {
	 if (h & ID_SigmaX) DrawTempFit(Sigma0X,SpeedX);
	 if (h & ID_SigmaY) DrawTempFit(Sigma0Y,SpeedY);
  }
  if (SNfited && (h & ID_NI)) DrawSNFit(ID_NI);
  if (SNfited && (h & ID_NG)) DrawSNFit(ID_NG);
//  if (Nfited && (h & ID_Abs)) DrawNFit();
  if (ChargingTimefited && (h & ID_NI)) DrawChargingTimeFit(ID_NI);
  if (ChargingTimefited && (h & ID_NG)) DrawChargingTimeFit(ID_NG);
  if (Lifetimefited && (h & ID_NI)) DrawLifetimeFit(ID_NI);
  if (Lifetimefited && (h & ID_NG)) DrawLifetimeFit(ID_NG);
}

void CTNMeasure::DrawTempFit(double aSigma0, double Speed) {
  try {
  TClientDC dc(*this);
  TPen pen(TColor(255,0,0));
  dc.SelectObject(pen);
  int first=1;
  for (int i=0;i<Attr.W;i++) {
	 double x=xmin+double(i)/double(Attr.W)*(xmax-xmin);
	 double y=sqrt(aSigma0*aSigma0+Speed*Speed*x*x);
	 int yh=Attr.H-(y-ymin)/(ymax-ymin)*Attr.H;
	 if (yh>Attr.H-1) yh=Attr.H-1;
	 if (yh<0) yh=0;
	 if (first) {
		dc.MoveTo(i,yh);
		first=0;
	 } else dc.LineTo(i,yh);
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
	  DebugFile<<"GDI error CTNMeasure::DrawTempFit ";
     if (found) DebugFile<<s;
     DebugFile<<endl;
	  DebugFile.close();
   }
}

/*
void CTNMeasure::DrawNFit() {
  TClientDC dc(*this);
  TPen pen(TColor(255,0,0));
  dc.SelectObject(pen);
  int first=1;
  double h2=N/(NSigmaX*1E-3)/(NSigmaY*1E-3)/2/M_PI;
  for (int i=0;i<Attr.W;i++) {
	 double x=xmin+double(i)/double(Attr.W)*(xmax-xmin);
	 double h=(x+DeltaDetuning)*1E6/(GetGamma())/4/M_PI;
	 double y=h2*(GetClebsch())*(Lambda)*(Lambda)/2/M_PI/(1+h*h);
	 int yh=Attr.H-(y-ymin)/(ymax-ymin)*Attr.H;
	 if (first) {
		dc.MoveTo(i,yh);
		first=0;
	 } else dc.LineTo(i,yh);
  }
}
*/

void CTNMeasure::DrawSNFit(int ID) {
  try {
  TClientDC dc(*this);
  TPen pen((ID==ID_NI) ? TColor(255,0,0) : TColor(0,255,0));
  dc.SelectObject(pen);
  int yh=Attr.H-(SN[(ID==ID_NI) ? 0 : 1]-ymin)/(ymax-ymin)*Attr.H;
  dc.MoveTo(0,yh);
  dc.LineTo(Attr.W,yh);
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
	  DebugFile<<"GDI error CTNMeasure::DrawSNFit ";
     if (found) DebugFile<<s;
     DebugFile<<endl;
	  DebugFile.close();
   }
}

/*void CTNMeasure::Check() {
//	if( heapcheck() == _HEAPCORRUPT )
  //    MessageBox("CMessWindow::Check Heap is corrupted." , "Error", MB_OK);

   if (points!=spoints) {
      MessageBox("CMessWindow::Check : Error 0", "Error", MB_OK);
  }
  if (points) {
    if (points->hnext!=points->next) {
   	MessageBox("CMessWindow::Check : Error 1", "Error", MB_OK);
   }
   CMyPoint *HelpPoint;
  HelpPoint=points;
  int i=0;
  while (HelpPoint!=NULL) {
     //error correction seems necessary
     if ((HelpPoint->next==points) || (HelpPoint->next==HelpPoint)) {
     		HelpPoint->next=NULL;
         char buf[200];
         sprintf(buf,"CMessWindow::SetMaxXY : Error 2 %i",i);
         MessageBox(buf, "Error", MB_OK);
     }
     if (points->next!=points->hnext) {
         char buf[200];
         sprintf(buf,"CMessWindow::SetMaxXY : Error 3 %i",i);
         MessageBox(buf, "Error", MB_OK);
     }
  	  if (HelpPoint->next!=HelpPoint->hnext) {
         char buf[200];
         sprintf(buf,"CMessWindow::SetMaxXY : Error 4 %i",i);
         MessageBox(buf, "Error", MB_OK);
     }
     HelpPoint=HelpPoint->next;
     i++;
  }
   }
} */

void CTNMeasure::AddPoint(int NoSerie, CMessPoint *MD,TEdit *LogbookEdit) {
if (points!=spoints) {
      MessageBox("CMessWindow::AddPoint : Error 0", "Error", MB_OK);
  }
  //this was 2
  char name[200];
  names->GetRegisterName(name,MD->y("Camera"));
  if (NoSerie) MD->SetX("Nr",names->PictureNumber());
  MD->SetPicNr(names->PictureNumber());
//  MD->SetText("Filename",name);
	if (points!=spoints) {
      MessageBox("CMessWindow::AddPoint : Error 0", "Error", MB_OK);
  }
  if (points) {
    if (points->hnext!=points->next) {
   	MessageBox("CMessWindow::AddPoint : Error 1", "Error", MB_OK);
   }
   }
  points=MD->PutIntoList(points);
  spoints=points;
  SetMaxXY();
  Draw();
  struct ffblk ffblk;
  int fileexists=findfirst(name,&ffblk,0);
  ofstream out(name,ios::app);
  if (fileexists) {
    out<<"\\\\"<<name<<endl;
    MD->SaveHeader(out);
  }
  MD->Save(out);
  if (FirstPoint) {
    FirstPoint=false;
    char LogName[200];
    char SerieName[200];
    names->GetSerieName(SerieName);
    names->GetLogbookName(LogName);
    ofstream out2(LogName,ios::app);
    out2<<names->GetDirName()<<" S"<<(names->GetSerialNumber()-1)<<' '<<commentary<<" @ "<<MD->GetText("Path")<<" "<<MD->GetText("Endstate")<<" Parameters: ";
    for (int i=0;i<MD->GetNrX();i++) out2<<MD->GetXName(i)<<' ';
    out2<<endl;
    names->GetImageLogbookName(LogName);
    ofstream out3(LogName,ios::app);
    char buf[300];
    sprintf(buf,"# S%i %s @ %s %s Parameters: ",(names->GetSerialNumber()-1),commentary,MD->GetText("Path"),MD->GetText("Endstate"));
    out3<<buf;//"# S"<<(names->GetSerialNumber()-1)<<' '<<commentary<<" @ "<<MD->GetText("Path")<<" "<<MD->GetText("Endstate")<<" Parameters: ";
    if (LogbookEdit) LogbookEdit->Insert(buf);
    for (int i=0;i<MD->GetNrX();i++) {
      sprintf(buf,"%s ",MD->GetXName(i));
      out3<<buf;//MD->GetXName(i)<<' ';
      if (LogbookEdit) LogbookEdit->Insert(buf);
    }
    out3<<endl;
    if (LogbookEdit) LogbookEdit->Insert("\r\n");
  }
}

void CTNMeasure::New() {
if (points!=spoints) {
      MessageBox("CMessWindow::AddPoint : Error 0", "Error", MB_OK);
  }
  CMessWindow::New();
  Tempfited=0;
  Nfited=0;
  ChargingTimefited=0;
  SNfited=0;
  NoSerie=1;
}

/*
void CTNMeasure::NumberFit() {
  if (points!=NULL) {
	 int npoints=points->number(CameraButtons->GetChecked());
	 if (npoints>1) {
		Nfited=1;
		double *x=new double[npoints];
		double *y=new double[npoints];
		points->GetXY(x,y,2,0,CameraButtons->GetChecked());
		double *sig=new double[npoints];
		for (int i=0;i<npoints;i++) sig[i]=1;
		double *a=new double[2];
      double *v=new double[2];
		CMyPoint *hpoints;
		CMyPoint *maxPoint;
		hpoints=points;
		double maxamp=points->y(2);
		maxPoint=points;
		while (hpoints->next!=NULL) {
		  hpoints=hpoints->next;
		  if (hpoints->y(2)>maxamp) {
			 maxamp=hpoints->y(2);
			 maxPoint=hpoints;
		  }
		}
		NSigmaX=maxPoint->y(0);
		NSigmaY=maxPoint->y(1);
		a[0]=maxPoint->y(6)/(NSigmaX*1E-3)/(NSigmaY*1E-3);
		a[1]=0;
      v[0]=1;
      v[1]=1;
		double chisq;
		double *pchisq=&chisq;
      int iter;
      double change;
		if (fit(x,y,sig,npoints,a,v,2,pchisq,20,1,1E-5,1E-30,1E-5,iter,change,number)) {
        a[0]=maxPoint->y(6)/(NSigmaX*1E-3)/(NSigmaY*1E-3);
   	  a[1]=0;
        mb->SetText("FitError NumberFit");
      }
		N=a[0]*(NSigmaX*1E-3)*(NSigmaY*1E-3);
		DeltaDetuning=a[1]*1E-6;
		Nfited=1;
		delete a;
      delete v;
		delete x;
		delete y;
		delete sig;
//		TNFitTextBox->SetValue(7,N);
//		TNFitTextBox->SetValue(8,DeltaDetuning);
//		TNFitTextBox->SetValue(9,NSigmaX);
//		TNFitTextBox->SetValue(10,NSigmaY);
		Draw();
	 }
  }
}
*/

void CTNMeasure::SimpleNumberFit() {
  if (points!=NULL) {
	 int npoints=points->number(CameraButtons->GetChecked());
	 if (npoints>1) {
		 double *x=new double[npoints];
		 double *y=new double[npoints];
		 double *sig=new double[npoints];
		 for (int i=0;i<npoints;i++) sig[i]=1;
		 double *a=new double[2];
       double *v=new double[2];
       v[0]=1;
       v[1]=1;
		 a[0]=points->y(6);
		 double chisq;
		 double *pchisq=&chisq;
       const int table[9]={2,6,0,0,1,5,8,4,10};
       for (int i=0;i<9;i++) {
         a[0]=points->y(i);
         points->GetXY(x,y,i,0,CameraButtons->GetChecked());
         int iter;
         double change;
         if (fit(x,y,sig,npoints,a,v,1,pchisq,20,1,1E-5,1E-30,1E-5,iter,change,linear)) {
           mb->SetText("FitError 1 SimpleNumberFit");
         }
         GaussFitTextBox->SetValue(table[i],a[0]);
       }
       a[0]=points->y(NR_NG);
       points->GetXY(x,y,NR_NG,0,CameraButtons->GetChecked());
       int iter;
       double change;
       if (fit(x,y,sig,npoints,a,v,1,pchisq,20,1,1E-5,1E-30,1E-5,iter,change,linear)) {
         mb->SetText("FitError 2 SimpleNumberFit");
       }
       TNFitTextBox->SetValue(6,a[0]);
		 SN[0]=a[0];
       double chisqNG=chisq;
       points->GetXY(x,y,NR_NI,0,CameraButtons->GetChecked());
		 fit(x,y,sig,npoints,a,v,1,pchisq,20,1,1E-5,1E-30,1E-5,iter,change,linear);
		 SN[1]=a[0];
		 SNfited=1;
		 delete[] a;
       delete[] v;
		 delete[] x;
		 delete[] y;
		 delete[] sig;
		 TNFitTextBox->SetValue(6,SN[0]);
       char buf[100];
		 sprintf(buf,"NG=%.2e chiNG=%.2e, NI=%.2e chiNI=%.2e",SN[0],sqrt(chisqNG), SN[1],sqrt(chisq));
		 mb->SetText(buf);
		 Draw();
	  }
  }
}

void CTNMeasure::TemperatureFit(double aMasse) {
  Masse=aMasse;
  if (points!=NULL) {
	 int npoints=points->number(CameraButtons->GetChecked());
	 if (npoints>1) {
		 double *x=new double[npoints];
		 double *y=new double[npoints];
		 points->GetXY(x,y,0,0,CameraButtons->GetChecked());
		 double *sig=new double[npoints];
		 for (int i=0;i<npoints;i++) sig[i]=1;
		 double *a=new double[2];
       double *v=new double[2];
       v[0]=1;
       v[1]=1;
		 a[0]=1;
		 a[1]=10;
		 double chisq;
		 double *pchisq=&chisq;
       int iter;
       double change;
		 if (fit(x,y,sig,npoints,a,v,2,pchisq,20,1,1E-5,1E-30,1E-5,iter,change,temperature)) {
         mb->SetText("FitError TemperatureFit");
       }
		 char buf[100];
		 sprintf(buf,"chisq=%.2f",chisq);
		 mb->SetText(buf);
		 Sigma0X=a[0];
		 SpeedX=a[1];
		 TempX=Masse*SpeedX*SpeedX/_kB;
		 points->GetXY(x,y,1,0,CameraButtons->GetChecked());
		 a[0]=1;
		 a[1]=10;
		 fit(x,y,sig,npoints,a,v,2,pchisq,20,1,1E-5,1E-30,1E-5,iter,change,temperature);
		 Sigma0Y=a[0];
		 SpeedY=a[1];
		 TempY=Masse*SpeedY*SpeedY/_kB;
		 delete[] a;
       delete[] v;
		 delete[] x;
		 delete[] y;
		 delete[] sig;
		 TNFitTextBox->SetValue(0,Sigma0X);
		 TNFitTextBox->SetValue(1,SpeedX);
		 TNFitTextBox->SetValue(2,TempX*1E6);
		 TNFitTextBox->SetValue(3,Sigma0Y);
		 TNFitTextBox->SetValue(4,SpeedY);
		 TNFitTextBox->SetValue(5,TempY*1E6);
		 Tempfited=1;
		 Draw();
	  }
  }
}

void CTNMeasure::ChargingTimeFit() {
  if (points!=NULL) {
	 int npoints=points->number(CameraButtons->GetChecked());
	 if (npoints>1) {
		 double *x=new double[npoints];
		 double *y=new double[npoints];

		 double *sig=new double[npoints];
		 for (int i=0;i<npoints;i++) sig[i]=1;
		 double *a=new double[2];
       double *v=new double[2];
       v[0]=1;
       v[1]=1;
		 double h=0;
       double chisq;
		 double *pchisq=&chisq;

       points->GetXY(x,y,NR_NG,0,CameraButtons->GetChecked());
		 points->GetMax(h,NR_NG,CameraButtons->GetChecked());
		 a[0]=h;
		 a[1]=500;
       int iter;
       double change;
		 fit(x,y,sig,npoints,a,v,2,pchisq,20,1,1E-5,1E-30,1E-5,iter,change,ChargingTime);
		 ChargingN[0]=a[0];
		 ChargingT[0]=a[1];

       points->GetXY(x,y,NR_NI,0,CameraButtons->GetChecked());
		 points->GetMax(h,NR_NI,CameraButtons->GetChecked());
		 a[0]=h;
		 a[1]=500;
		 fit(x,y,sig,npoints,a,v,2,pchisq,20,1,1E-5,1E-30,1E-5,iter,change,ChargingTime);
		 ChargingN[1]=a[0];
		 ChargingT[1]=a[1];

		 delete[] a;
       delete[] v;
		 delete[] x;
		 delete[] y;
		 delete[] sig;
		 char buf[200];
		 sprintf(buf,"NGauss0=%.2E   TauGauss=%.2f ms   NInt0=%.2E   TauInt=%.2f ms",ChargingN[0],ChargingT[0],ChargingN[1],ChargingT[1]);
		 mb->SetText(buf);
		 ChargingTimefited=1;
		 Draw();
	  }
  }
}

void CTNMeasure::DrawChargingTimeFit(int ID) {
  try {
  TClientDC dc(*this);
  TPen pen((ID==ID_NI) ? TColor(255,0,0) : TColor(0,255,0));
  dc.SelectObject(pen);
  int first=1;
  for (int i=0;i<Attr.W;i++) {
	 double x=xmin+double(i)/double(Attr.W)*(xmax-xmin);
	 double y=ChargingN[(ID==ID_NI) ? 1 : 0]*(1-exp(-x/ChargingT[(ID==ID_NI) ? 1 : 0]));
	 int yh=Attr.H-(y-ymin)/(ymax-ymin)*Attr.H;
	 if (first) {
		dc.MoveTo(i,yh);
		first=0;
	 } else dc.LineTo(i,yh);
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
	  DebugFile<<"GDI error CTNMeasure::DrawChargingTimeFit ";
     if (found) DebugFile<<s;
     DebugFile<<endl;
	  DebugFile.close();
   }
}

void CTNMeasure::LifetimeFit() {
  if (points!=NULL) {
	 int npoints=points->number(CameraButtons->GetChecked());
	 if (npoints>1) {
		 double *x=new double[npoints];
		 double *y=new double[npoints];

		 double *sig=new double[npoints];
		 for (int i=0;i<npoints;i++) sig[i]=1;
		 double *a=new double[2];
       double *v=new double[2];
       v[0]=1;
       v[1]=1;
		 double h=0;
		 double chisq;
		 double *pchisq=&chisq;

       points->GetXY(x,y,NR_NG,0,CameraButtons->GetChecked());
		 points->GetMax(h,NR_NG,CameraButtons->GetChecked());
		 a[0]=h;
		 a[1]=500;
       int iter;
       double change;
		 fit(x,y,sig,npoints,a,v,2,pchisq,20,1,1E-5,1E-30,1E-5,iter,change,Lifetime);
		 LifetimeN[0]=a[0];
		 LifetimeT[0]=a[1];

       points->GetXY(x,y,NR_NI,0,CameraButtons->GetChecked());
		 points->GetMax(h,NR_NI,CameraButtons->GetChecked());
		 a[0]=h;
		 a[1]=500;
		 fit(x,y,sig,npoints,a,v,2,pchisq,20,1,1E-5,1E-30,1E-5,iter,change,Lifetime);
		 LifetimeN[1]=a[0];
		 LifetimeT[1]=a[1];

		 delete[] a;
       delete[] v;
		 delete[] x;
		 delete[] y;
		 delete[] sig;
		 char buf[200];
		 sprintf(buf,"N0Gauss=%.2E   TauGauss=%.2f ms    N0Int=%.2E   TauInt=%.2f ms",LifetimeN[0],LifetimeT[0],LifetimeN[1],LifetimeT[1]);
		 mb->SetText(buf);
		 Lifetimefited=1;
		 Draw();
	  }
  }
}

void CTNMeasure::DrawLifetimeFit(int ID) {
try {
  TClientDC dc(*this);
  TPen pen((ID==ID_NI) ? TColor(255,0,0) : TColor(0,255,0));
  dc.SelectObject(pen);
  int first=1;
  for (int i=0;i<Attr.W;i++) {
	 double x=xmin+double(i)/double(Attr.W)*(xmax-xmin);
	 double y=LifetimeN[(ID==ID_NI) ? 1 : 0]*exp(-x/LifetimeT[(ID==ID_NI) ? 1 : 0]);
	 int yh=Attr.H-(y-ymin)/(ymax-ymin)*Attr.H;
	 if (first) {
		dc.MoveTo(i,yh);
		first=0;
	 } else dc.LineTo(i,yh);
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
	  DebugFile<<"GDI error CTNMeasure::DrawLifetimeFit ";
     if (found) DebugFile<<s;
     DebugFile<<endl;
	  DebugFile.close();
   }
}

void CTNMeasure::Load(const char*filename,int add) {
  char path[200];
  int i=strlen(filename);
  while ((i>0) && (filename[i]!='\\')) i--;
  strncpy(path,filename,i+1);
  path[i+1]=0;
  ifstream *in=new ifstream(filename);
  if (!in) {
	 char buf[200];
	 sprintf(buf,"Could not open file %s",filename);
	 MessageBox(buf,"Error");
	 return;
  }
  in->getline(commentary,300);
  mb->SetText(commentary);
  int err;
  CMyPoint *hpoints=new CMessPoint(in,err);
  #if defined(DetectLeaks)
      LeakDetect.New(hpoints,6,2);
    #endif
  if (err==0) {
    if (!add) New();
    else {
      ColorPoints();
      aktColor++;
    }
    disk=filename[0];
    CMessPoint *Prototyp=new CMessPoint(hpoints);
    #if defined(DetectLeaks)
      LeakDetect.New(Prototyp,6,3);
    #endif
    CMessPoint *DefaultPoint=new CMessPoint();
    #if defined(DetectLeaks)
      LeakDetect.New(DefaultPoint,6,4);
    #endif
    while (hpoints->LoadPoint(in)==0) {
      hpoints->AddDefaultValues(DefaultPoint);
  /*  This code messes up the file location
      char buf[200];
      strcpy(buf,hpoints->GetText("Filename"));
		int i=strlen(buf)-2;
	   while ((i>0) && (buf[i]!='\\')) i--;
      char help[200];
      char help2[200];
      strcpy(help,path);
      strcpy(help2,&buf[i+1]);
      strcat(help,help2);
      hpoints->SetText("Filename",help);*/
      hpoints->OrderParameters(DefaultPoint);
      if (add) hpoints->SetThisColor(aktColor);
      points=hpoints->PutIntoList(points);
      spoints=points;
      hpoints=new CMessPoint(Prototyp);
      #if defined(DetectLeaks)
        LeakDetect.New(hpoints,6,5);
      #endif
    }
    aktColor=0;
    if (points!=NULL) points->GetMaxColor(aktColor);
    #if defined(DetectLeaks)
      LeakDetect.Delete(Prototyp);
    #endif
    delete Prototyp;
    #if defined(DetectLeaks)
      LeakDetect.Delete(DefaultPoint);
    #endif
    delete DefaultPoint;
  } else MessageBox("This is not a Vision Serie!","Error");
  #if defined(DetectLeaks)
     LeakDetect.Delete(hpoints);
  #endif
  delete hpoints;
  delete in;
  if (points) AxText->SetText(points->GetXName(0));
  Draw();
  strcpy(MyName,filename);
}

void CTNMeasure::ButtonAction() {
  sprintf(MesName,aktpoint->GetText("Filename"),'A');
  MesIsotop=aktpoint->y("Camera");
//  MesProbeDetuning=aktpoint->y(8);
  MesName[0]=disk;
  LoadPoint=(CMessPoint *)(aktpoint);
  parent->PostMessage(IDM_LOADMEASUREMENT,2);
}

void CTNMeasure::StartSerie(char *acommentary,CMessPoint *MD) {
  StopSerie();
  New();
  NoSerie=0;
  names->NewSerie();
  names->GetRegisterName(name,MD->y("Camera"));
  ofstream out(name);
  out<<acommentary<<endl;
  strcpy(commentary,acommentary);
  MD->SaveHeader(out);
  AxText->SetText(MD->GetXName(0));
  FirstPoint=true;
  strcpy(MyName,name);
}

void CTNMeasure::StopSerie() {
  names->StopSerie();
  strcpy(oldseriename,name);
  name[0]=0;
  NoSerie=1;
}

void CTNMeasure::ContinueSerie() {
  names->ContinueSerie();
  strcpy(name,oldseriename);
  NoSerie=0;
}

void CTNMeasure::ReCalc() {
  if (points!=NULL) {
	 CMessPoint *hpoint;
	 hpoint=(CMessPoint *)(points);
	 while (hpoint!=NULL) {
		sprintf(MesName,hpoint->GetText("Filename"),'A');
      MesName[0]=disk;
      MesIsotop=hpoint->y("Camera");
		if (hpoint->Enabled()) {
        LoadPoint=hpoint;
		  parent->SendMessage(IDM_LOADMEASUREMENT,0);  //Calculates and Stores data here
//		  hpoint->SetData(X0,SigmaX,AmpX,Y0,SigmaY,AmpY,Abs,OptDens,fN,fNGauss,NIoffe,TIoffe);
		  Draw();
		}
		hpoint=(CMessPoint *)(hpoint->next);
	 }
  }
}

void CTNMeasure::Iterate() {
  if (points!=NULL) {
	 CMessPoint *hpoint;
	 hpoint=(CMessPoint *)(points);
	 while (hpoint!=NULL) {
		sprintf(MesName,hpoint->GetText("Filename"),'A');
      MesName[0]=disk;
      MesIsotop=hpoint->y("Camera");
		if (hpoint->Enabled()) {
        LoadPoint=hpoint;
		  parent->SendMessage(IDM_LOADMEASUREMENT,0);  //Calculates and Stores data here
        parent->SendMessage(IDM_ITERATEPICTURE);
		}
		hpoint=(CMessPoint *)(hpoint->next);
	 }
  }
}

void CTNMeasure::SaveSerie(const char* filename) {
  if (points!=NULL) {
	 ofstream out(filename);
	 out<<commentary<<endl;
    points->SaveHeader(out);
    points->SavePoints(out);
    strcpy(MyName,filename);
  }
}

void CTNMeasure::SaveSerie() {
  if (strlen(MyName)>0) SaveSerie(MyName);
}

void CTNMeasure::GetNumber(int PicNumber, double &Number, char *name) {
if (points!=spoints) {
      MessageBox("CMessWindow::AddPoint : Error 0", "Error", MB_OK);
  }
  if (points) {
	 Sort(&points);
	 spoints=points;
	 CMyPoint *hpoint=points;
	 while (hpoint->next) hpoint=hpoint->next;
	 for (int i=1;i<PicNumber;i++) {
		CMyPoint *hpoint2=points;
		while ((hpoint2->next!=hpoint) && hpoint2->next) hpoint2=hpoint2->next;
		hpoint=hpoint2;
	 }
	 Number=hpoint->y(6);
    strcpy(name,hpoint->GetText("Filename"));
  } else {
    Number=0;
    name=NULL;
  }
}

