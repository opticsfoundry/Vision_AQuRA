#include "point.h"
#include <window.h>
#include <stdio.h>
#include "leakdetect.h"

const MaxNameLength=12;
const char *NullString="";
double Null=0;

//*** CMyPoint ***
CMyPoint::CMyPoint( int aNrX, int aNrY, int aNrText, char **aXName, char **aYName, char **aTextName) {
  Init( aNrX, aNrY, aNrText, aXName, aYName, aTextName);
}

void CMyPoint::Init(int aNrX, int aNrY, int aNrText, char **aXName, char **aYName, char **aTextName) {
  NrX=aNrX;
  NrY=aNrY;
  NrText=aNrText;
  try {
	  XName= new char* [NrX];
	  for (int i=0;i<NrX;i++) {
   	 XName[i]=new char[strlen(aXName[i])+1];
	    strcpy(XName[i],aXName[i]);
	  }
 		YName= new char* [NrY];
	  for (int i=0;i<NrY;i++) {
   	 YName[i]=new char[strlen(aYName[i])+1];
	    strcpy(YName[i],aYName[i]);
	  }
	  TextName= new char* [NrText];
	  for (int i=0;i<NrText;i++) {
	    TextName[i]=new char[strlen(aTextName[i])+1];
   	 strcpy(TextName[i],aTextName[i]);
	  }

	  Text= new char* [NrText];
	  for (int i=0;i<NrText;i++) Text[i]=NULL;

	  x=new double[NrX];
	  for (int i=0;i<NrX;i++) x[i]=0;
	  yb=new double[NrY];
	  for (int i=0;i<NrY;i++) yb[i]=0;
  }
  catch (xalloc) {
  		MessageBox(GetActiveWindow(),"CMyPoint::Init : couldn't reserve memory","Error",  MB_OK);
      exit(-1);
  }
  next=NULL;
  hnext=NULL;
  enabled=true;
  active=false;
  code[0]=0;
  PicNr=0;
  mycolor=0;
  InList=false;
  metapointer=NULL;

//  ofstream DebugFile("DebugPoint.dat", ios::app);
//  DebugFile<<"new "<<this<<endl;
//  DebugFile.close();

}

CMyPoint::~CMyPoint() {
//  ofstream DebugFile("DebugPoint.dat", ios::app);
//  DebugFile<<"delete "<<this<<endl;
//  DebugFile.close();
  
  if (metapointer) {
    (*metapointer)=new CMyPoint(this);
    #if defined(DetectLeaks)
	     LeakDetect.New((*metapointer),7,1);
  	 #endif
    (*metapointer)->metapointer=metapointer;
    for (int i=0;i<NrText;i++) {
      (*metapointer)->Text[i]=new char[strlen(Text[i])+1];;
      strcpy((*metapointer)->Text[i],Text[i]);
    }
    for (int i=0;i<NrX;i++) (*metapointer)->x[i]=x[i];
    for (int i=0;i<NrY;i++) (*metapointer)->yb[i]=yb[i];
    strcpy((*metapointer)->code,code);
  }
  //should remove recursion
  #if defined(DetectLeaks)
     LeakDetect.Delete(next);
  #endif
  if (next!=NULL) delete next;
  if (x) delete[] x;
  if (yb) delete[] yb;
  if (Text) {
    //error here 1 time
    for (int i=0;i<NrText;i++) if (Text[i]) delete[] Text[i];
    delete[] Text;
  }
  if (XName) {
    for (int i=0;i<NrX;i++) if (XName[i]) delete[] XName[i];
    delete[] XName;
  }
  if (YName) {
    for (int i=0;i<NrY;i++) if (YName[i]) delete[] YName[i];
    delete[] YName;
  }
  if (TextName) {
    for (int i=0;i<NrText;i++) if (TextName[i]) delete[] TextName[i];
    delete[] TextName;
  }
}

CMyPoint::CMyPoint(ifstream *in, int &err) {
  XName=NULL;
  YName=NULL;
  TextName=NULL;
  Text=NULL;
  char buf[4096];
  in->getline(buf,4096);
  err=0;
  if (strncmp(buf,"PicNr Ok Code",13)==0) {
	 char buf2[4096];
	 char buf3[4096];
	 NrX=0;
	 strcpy(buf2,&buf[13]);
	 do {
		while (buf2[0]==' ') strcpy(buf2,&buf2[1]);
      if (strchr(buf2,' ')>0) {
        int h=strchr(buf2,' ')-buf2;
        strncpy(buf3,buf2,h);
		  buf3[h]=0;
		  if (strcmp(buf3,"SigmaX")!=0) NrX++;
		  strcpy(buf2,strchr(buf2,' ')+1);
      } else {err=-2; return;}
	 } while ((strcmp(buf3,"SigmaX")!=0) && (strlen(buf2)>5));
    if (strcmp(buf3,"SigmaX")!=0) {err=-2; return;}
	 XName=new char* [NrX];
	 strcpy(buf2,&buf[13]);
	 for (int i=0;i<NrX;i++) {
		while (buf2[0]==' ') strcpy(buf2,&buf2[1]);
      int h=strchr(buf2,' ')-buf2;
      XName[i]=new char[h+1];
		strncpy(XName[i],buf2,h);
		XName[i][h]=0;
		strcpy(buf2,strchr(buf2,' ')+1);
	 }
    strcpy(buf,buf2);
    NrY=0;
    do {
		while (buf2[0]==' ') strcpy(buf2,&buf2[1]);
      if (strchr(buf2,' ')>0) {
  		  int h=strchr(buf2,' ')-buf2;
		  strncpy(buf3,buf2,h);
		  buf3[h]=0;
      } else strcpy(buf3,buf2);
		if (strncmp(buf3,"Filename",MaxNameLength)!=0) NrY++;
		if (strchr(buf2,' ')>0) strcpy(buf2,strchr(buf2,' ')+1);
	 } while ((strncmp(buf3,"Filename",MaxNameLength)!=0) && (strlen(buf2)>5));
    if (strcmp(buf3,"Filename")!=0) {err=-2; return;}
	 YName=new char* [NrY];
    strcpy(buf2,buf);
    for (int i=0;i<NrY;i++) {
		while (buf2[0]==' ') strcpy(buf2,&buf2[1]);
      if (strchr(buf2,' ')>0) {
		  int h=strchr(buf2,' ')-buf2;
        YName[i]=new char[h+1];
		  strncpy(YName[i],buf2,h);
		  YName[i][h]=0;
		  strcpy(buf2,strchr(buf2,' ')+1);
      } else {YName[i]=NULL;err=-3;return;}
	 }
    strcpy(buf,buf2);
    NrText=0;
    do {
		while (buf2[0]==' ') strcpy(buf2,&buf2[1]);
      if (strchr(buf2,' ')>0) {
        strcpy(buf2,strchr(buf2,' ')+1);
        NrText++;
      }
	 } while (strchr(buf2,' ')>0);
    if (strlen(buf2)>0) NrText++;
	 TextName=new char* [NrText];
    strcpy(buf2,buf);
    for (int i=0;i<NrText;i++) {
		while (buf2[0]==' ') strcpy(buf2,&buf2[1]);
      int h;
      if (strchr(buf2,' ')>0) h=strchr(buf2,' ')-buf2;
      else h=strlen(buf2);
		TextName[i]=new char[h+1];
		strncpy(TextName[i],buf2,h);
		TextName[i][h]=0;
		if (strchr(buf2,' ')>0) strcpy(buf2,strchr(buf2,' ')+1);
	 }
  } else {
    err=-1;
    NrX=0;
    NrY=0;
    NrText=0;
    XName=NULL;
    YName=NULL;
    TextName=NULL;
  }
  Text= new char* [NrText];
  for (int i=0;i<NrText;i++) Text[i]=NULL;
  x=new double[NrX];
  for (int i=0;i<NrX;i++) x[i]=0;
  yb=new double[NrY];
  for (int i=0;i<NrY;i++) yb[i]=0;
  next=NULL;
  hnext=NULL;
  enabled=true;
  active=false;
  code[0]=0;
  PicNr=0;
  mycolor=0;
  InList=false;
  metapointer=NULL;
}

CMyPoint::CMyPoint(CMyPoint *last) {
  Init( last->NrX,last->NrY,last->NrText, last->XName, last->YName, last->TextName);
}

void CMyPoint::Copy(CMyPoint *source) {
  for (int i=0;i<NrY;i++) y(YName[i])=source->y(YName[i]);
  for (int i=0;i<NrX;i++) SetX(XName[i],source->GetX(XName[i]));
  for (int i=0;i<NrText;i++) SetText(TextName[i],source->GetText(TextName[i]));
}

void CMyPoint::CopyData(CMyPoint *source) {
  for (int i=0;i<NrY;i++) y(YName[i])=source->y(YName[i]);
  for (int i=0;i<NrText;i++) SetText(TextName[i],source->GetText(TextName[i]));
}

void CMyPoint::SetMetaPointer(CMyPoint **ametapointer) {
  //metapointer contains the address of the TVisionDialog objects MD,
  //which is the MD which is usually displayed
  metapointer=ametapointer;
}

void CMyPoint::SetNewXNames(int aNrX, char **aXName) {
  for (int i=0;i<NrX;i++) if (XName[i]) delete[] XName[i];
  if (XName) delete[] XName;
  if (x) delete[] x;
  NrX=aNrX;
  x=new double[NrX];
  XName= new char* [NrX];
  for (int i=0;i<NrX;i++) {
    XName[i]=new char[strlen(aXName[i])+1];
    strcpy(XName[i],aXName[i]);
    x[i]=0;
  }
}

int GetLine(ifstream *in, char *buf) {
  if (in->eof()) return -1;
  do {
    in->getline(buf,4096,' ');
    while (buf[0]==' ') strcpy(buf,&buf[1]);
  } while ((strlen(buf)==0) && (!in->eof()));
  if (strlen(buf)==0) return -1;
  else return 0;
}

int CMyPoint::LoadPoint(ifstream *in, int Nr) {
  if (in->eof()) return -1;
  char buf[4096];
  PicNr=Nr;
  if (Nr>=0) {
    bool ende=false;
    do {
      if (GetLine(in,buf)<0) return -1;
      if (buf[0]!=0) ende=Nr==atoi(buf);
      if (!ende && (!in->eof())) in->getline(buf,4096);
    } while (!ende);
    if (!ende) return -1;
  } else {
    if (GetLine(in,buf)<0) return -1;
    PicNr=atoi(buf);
  }
  if (GetLine(in,buf)<0) return -1;
  enabled=atoi(buf)==1;
  if (GetLine(in,code)<0) return -1;
  for (int i=0;i<NrX;i++) {
    if (GetLine(in,buf)<0) return -1;
    if (buf) x[i]=atof(buf);
    else {
      MessageBox(GetActiveWindow(),"CMyPoint::LoadPoint : Error reading file","Error!",MB_OK);
      return -2;
    }
  }
  for (int i=0;i<NrY;i++) {
    if (GetLine(in,buf)<0) return -1;
    yb[i]=atof(buf);
  }
  for (int i=0;i<NrText;i++) {
    if (i<(NrText-1)) {
      if (GetLine(in,buf)<0) return -1;
    } else in->getline(buf,4096);
    if (Text[i]) delete[] Text[i];
    Text[i]= new char[strlen(buf)+1];
    strcpy(Text[i],buf);
  }
  if (GetYNrFromName("Color")>=0) mycolor=y("Color");
  return 0;
}

void CMyPoint::SetX(char *name, double value) {
  int i=0;
  while ((i<NrX) && strncmp(name,XName[i],MaxNameLength)!=0) i++;
  if (i<NrX) x[i]=value;
  else {
    char buf[300];
    sprintf(buf,"CMyPoint::SetX : X value not defined %s\n",name);
    MessageBox(GetActiveWindow(),buf,"Error!",MB_OK);
  }
}

void CMyPoint::SetAllX(double *value) {
  for (int i=0;i<NrX;i++) x[i]=value[i];
}

double CMyPoint::GetXNr(int Nr) {
  if (Nr<NrX) return x[Nr];
  else {
    char buf[300];
    sprintf(buf,"CMyPoint::GetXNr : X not defined Nr %i\n",Nr);
    MessageBox(GetActiveWindow(),buf,"Error!",MB_OK);
    return 0;
  }
}

const char *CMyPoint::GetXName(int Nr) {
  if (Nr<NrX) return XName[Nr];
  else {
    char buf[300];
    sprintf(buf,"CMyPoint::GetXName : X not defined Nr %i\n",Nr);
    MessageBox(GetActiveWindow(),buf,"Error!",MB_OK);
    return NullString;
  }
}

void CMyPoint::SetY(char *name, double value) {
  int i=0;
  while ((i<NrY) && strncmp(name,YName[i],MaxNameLength)!=0) i++;
  if (i<NrY) yb[i]=value;
  else {
    char buf[300];
    sprintf(buf,"CMyPoint::SetY : Y value not defined %s\n",name);
    MessageBox(GetActiveWindow(),buf,"Error!",MB_OK);
  }
}

void CMyPoint::SetText(char *name,const char *atext) {
  int i=0;
  while ((i<NrText) && strncmp(name,TextName[i],MaxNameLength)!=0) i++;
  if (i<NrText) {
    if (Text[i]) delete[] Text[i];
    Text[i]=new char[strlen(atext)+1];
    strcpy(Text[i],atext);
  } else {
    char buf[300];
    sprintf(buf,"CMyPoint::SetText : 0 Text not defined %s\n",name);
    MessageBox(GetActiveWindow(),buf,"Error!",MB_OK);
  }
}

double CMyPoint::GetX(char *name) {
  int i=0;
  while ((i<NrX) && strncmp(name,XName[i],MaxNameLength)!=0) i++;
  if (i<NrX) return x[i];
  else {
    char buf[300];
    sprintf(buf,"CMyPoint::GetX : X Value not defined %s\n",name);
    MessageBox(GetActiveWindow(),buf,"Error!",MB_OK);
    return 0;
  }
}

double CMyPoint::GetY(char *name) {
  int i=0;
  while ((i<NrY) && strncmp(name,YName[i],MaxNameLength)!=0) i++;
  if (i<NrY) return yb[i];
  else {
    char buf[300];
    sprintf(buf,"CMyPoint::GetY : Y Value not defined %s\n",name);
    MessageBox(GetActiveWindow(),buf,"Error!",MB_OK);
    return 0;
  }
}

double & CMyPoint::operator[](const char *name) {
  int i=0;
  while ((i<NrY) && strncmp(name,YName[i],MaxNameLength)!=0) i++;
  if (i<NrY) return yb[i];
  else {
    char buf[300];
    sprintf(buf,"CMyPoint::operator[] : Y Value not defined %s\n",name);
    MessageBox(GetActiveWindow(),buf,"Error!",MB_OK);
    return Null;
  }
}

double & CMyPoint::y(const char *name) {
  int i=0;
  while ((i<NrY) && (strncmp(name,YName[i],MaxNameLength)!=0)) i++;
  if (i<NrY) return yb[i];
  else {
    char buf[300];
    sprintf(buf,"CMyPoint::y : Y Value not defined %s\n",name);
    MessageBox(GetActiveWindow(),buf,"Error!",MB_OK);
    return Null;
  }
}

const char *CMyPoint::GetText(char *name) {
  int i=0;
  while ((i<NrText) && strncmp(name,TextName[i],MaxNameLength)!=0) i++;
  if (i<NrText) return Text[i];
  else {
    char buf[300];
    sprintf(buf,"CMyPoint::GetText : 1 Text not defined %s\n",name);
    MessageBox(GetActiveWindow(),buf,"Error!",MB_OK);
    return NullString;
  }
}

const char *CMyPoint::GetTextNr(int i) {
  if (i<NrText) return Text[i];
  else {
    char buf[300];
    sprintf(buf,"CMyPoint::GetTextNr : 2 Text not defined %i\n",i);
    MessageBox(GetActiveWindow(),buf,"Error!",MB_OK);
    return NullString;
  }
}

void FillString(const char *in, char *out, int len) {
  if (in==NULL) {
    for (int i=0;i<len;i++) out[i]=' ';
    out[len-1]='*';
  } else {
    if (strlen(in)<len) {
      for (int i=0;i<len;i++) out[i]=' ';
      strcpy(&out[len-strlen(in)],in);
    } else strncpy(out,in,len);
  }
  out[len]=0;
}

void CMyPoint::SaveHeader(ofstream &out) {
  char buf[200];
  out<<"PicNr Ok Code ";
  for (int i=0;i<NrX;i++) {
    FillString(XName[i],buf,MaxNameLength);
    out<<buf<<' ';
  }
  for (int i=0;i<NrY;i++) {
    FillString(YName[i],buf,MaxNameLength);
    out<<buf<<' ';
  }
  for (int i=0;i<NrText;i++) {
    FillString(TextName[i],buf,MaxNameLength);
    out<<buf<<' ';
  }
  out<<endl;
}

void CMyPoint::SavePoints(ofstream &out) {
  if (GetYNrFromName("Color")>=0) y("Color")=mycolor;
  Save(out);
  //have to eliminate recursion
  if (next) next->SavePoints(out);
}

CMyPoint *CMyPoint::PutIntoList(CMyPoint* lastpoint) {
  next=lastpoint;
  hnext=lastpoint;
  InList=true;
  return this;
}

void CMyPoint::DeletePoint(CMyPoint *point) {
  CMyPoint *hpoint=this;
  while (hpoint->next!=NULL && hpoint->next!=point) hpoint=hpoint->next;
  if (hpoint->next==point) {
	 hpoint->next=point->next;
    hpoint->hnext=point->hnext;
	 point->next=NULL;
    point->hnext=NULL;
    #if defined(DetectLeaks)
	     LeakDetect.Delete(point);
  	 #endif
	 delete point;
  }
}

int CMyPoint::PointNr(unsigned long what) {
  int h=0;
  for (int j=0; j<32; j++) if (what & (1<<j)) h++;
  return h;
}

int CMyPoint::What(unsigned long what,int nr) {
  int h=-1;
  int j=-1;
  do {
	 j++;
	 if (what & (1<<j)) h++;
  } while ((j<32) && (h<nr));
  return j;
}

bool CMyPoint::Displayed(int nr) {
  bool Disp=false;
  //fehler hier 2 mal
  for (int i=0;i<10;i++) Disp=Disp || (
    (code[2]==(char)('0'+i)) && (((nr>>i) & 1)==1)
  );
  return Disp;
}

void CMyPoint::Draw(TClientDC *dc,double xmin,double xmax,double ymin,double ymax,int W,int H,unsigned long  what,int Selection) {

  /*ofstream DebugFile("DebugPoint.dat", ios::app);
  DebugFile<<"displayed "<<this<<endl;
  DebugFile.close();*/
  try {
  if (Displayed(Selection)) {
     TColor *color;
     for (int n=0;n<PointNr(what);n++) {
       int aktwhat=What(what,n);
       int thiscolor= (mycolor==0) ? aktwhat%7 : (mycolor-1)%7;
       switch (thiscolor) {
         case 0:
           color=new TColor(0,0,255);
         break;
         case 1:
           color=new TColor(0,255,0);
         break;
         case 2:
           color=new TColor(255,0,0);
         break;
         case 3:
           color=new TColor(70,240,250);
         break;
         case 4:
           color=new TColor(200,0,200);
         break;
         case 5:
           color=new TColor(255,255,0);
         break;
         case 6:
           color=new TColor(255,180,0);
         break;
         default: color=new TColor(0,255,0);
       }
       aktwhat=What(what,n);
       if (enabled) {
         dc->SelectObject(TBrush(*color));
       } else dc->SelectObject(TBrush(TColor(255,255,255)));
       if (active) {
         delete color;
         color=new TColor(0,0,0);
       }
       dc->SelectObject(TPen(*color));
       int xp=double(W)*(x[0]-xmin)/(xmax-xmin);
       int yp=H-double(H)*(yb[aktwhat]-ymin)/(ymax-ymin);
       if ((xp>=0) && (xp<=W) && (yp>=0) && (yp<=H)) {
         if (code[2]=='6') dc->Ellipse(xp-2,yp-2,xp+2,yp+2);
         else dc->Rectangle(xp-2,yp-2,xp+2,yp+2);
       }
       delete color;
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
	  DebugFile<<"GDI error CMyPoint::Draw ";
     if (found) DebugFile<<s;
     DebugFile<<endl;
	  DebugFile.close();
   }
  //if (next!=NULL) next->Draw(dc,xmin,xmax,ymin,ymax,W,H,what,Selection);
}

void CMyPoint::GetMaxColor(int &color) {
  if (mycolor>color) color=mycolor;
  //have to eliminate recursion
  if (next!=NULL) next->GetMaxColor(color);
}

void CMyPoint::SetColor(int color) {
  mycolor=color;
  //have to eliminate recursion
  if (next!=NULL) next->SetColor(color);
}

void CMyPoint::SetThisColor(int color) {
  mycolor=color;
}

void CMyPoint::GetMinMaxXY(double &xmin, double &ymin,double &xmax, double &ymax,unsigned long  what,int Selection) {
  if (Displayed(Selection)) {
     if (xmax<x[0]) xmax=x[0];
     if (xmin>x[0]) xmin=x[0];
     for (int n=0;n<PointNr(what);n++) {
       double h=yb[What(what,n)];
       if (ymax<h) ymax=h;
       if (ymin>h) ymin=h;
     }
  }
}

void CMyPoint::GetMax(double &value, int  aktwhat,int Selection) {
  //have to eliminate recursion
  if (next!=NULL) next->GetMax(value,aktwhat,Selection);
  if (Displayed(Selection)) {
    double h=yb[aktwhat];
    if (value<h) value=h;
  }
}

void CMyPoint::GetMin(double &value, int aktwhat,int Selection) {
  //have to eliminate recursion
  if (next!=NULL) next->GetMin(value,aktwhat,Selection);
  if (Displayed(Selection)) {
    double h=yb[aktwhat];
    if (value>h) value=h;
  }
}


int CMyPoint::number(int Selection) {
  //have to eliminate recursion
  int h=(next!=NULL) ? next->number(Selection) : 0;
  if (Displayed(Selection)) {
    if (enabled) h++;
  }
  return h;
}

void CMyPoint::ToggleEnable() {
  if (enabled) enabled=0; else enabled=1;
}

void CMyPoint::SetActive(int a) {
  active=a;
}

void CMyPoint::GetXY(double *ax,double *ay,int aktwhat, int pos,int Selection) {
  if (Displayed(Selection) && enabled) {
	 ax[pos]=x[0];
	 ay[pos]=yb[aktwhat];
	 pos++;
  }
  //have to eliminate recursion
  if (next!=NULL) next->GetXY(ax,ay,aktwhat,pos++,Selection);
}

CMyPoint *CMyPoint::GetNearestPoint(double xspan, double yspan, double ax, double ay,unsigned long  what, int Selection,double &h) {
  CMyPoint *hp;
  hp=NULL;
  //have to eliminate recursion
  if (next!=NULL) hp=next->GetNearestPoint(xspan,yspan,ax,ay,what,Selection,h); else h=1E10;
  if (Displayed(Selection)) {
    double dist=1E10;
    for (int n=0;n<PointNr(what);n++) {
  	   double hdist=(yb[What(what,n)]-ay)/yspan;
   	hdist=hdist*hdist+(x[0]-ax)*(x[0]-ax)/(xspan*xspan);
	   if (hdist<dist) dist=hdist;
    }
    if (h<dist) return hp;
    else {
	   h=dist;
  	   return this;
    }
  } else {
    if (hp) return hp;
    else return this;
  }
}

void CMyPoint::Save(ofstream &out) {
  char buf[20];
  sprintf(buf,"%5i",PicNr);
  out<<buf<<"  ";
  out<<((enabled) ? 1 : 0)<<' '<<code<<' ';
  for (int i=0;i<NrX;i++) {
    char buf[20];
    sprintf(buf,"%12.10g",x[i]);
    out<<buf<<' ';
  }
  for (int i=0;i<NrY;i++) {
    char buf[20];
    sprintf(buf,"%12.10g",yb[i]);
    out<<buf<<' ';
  }
  for (int i=0;i<NrText;i++) out<<Text[i]<<' ';
  out<<endl;
}

int CMyPoint::operator<(const CMyPoint& b) {
  int ende=0;
  int smaller=0;
  int n=0;
  do {
	 if (b.x[n]<x[n]) {
		ende=1;
		smaller=1;
	 } else if (b.x[n]>x[n]) {
		ende=1;
	 } else {
		n++;
	 }
  } while (!ende && n<NrX);
  return smaller;
}

void Sort(CMyPoint * *liste) {
  if (*liste) {
	 CMyPoint *v;
	 CMyPoint *a;
	 CMyPoint *b;
	 int ende;
	 do {
		v=NULL;
		a=*liste;
		b=a->next;
		ende=1;
		while (b) {
		  if ((*b)<(*a)) {
			 if (v) {
          	v->next=b;
            v->hnext=b;
          }
			 else *liste=b;
			 a->next=b->next;
          a->hnext=b->hnext;
			 b->next=a;
          b->hnext=a;
			 v=b;
			 b=a->next;
			 ende=0;
		  } else {
			 v=a;
			 a=b;
			 b=a->next;
		  }
		}
	 } while (!ende);
  }
}

void CMyPoint::ShiftParameters() {
  if (NrX>1) {
  //have to eliminate recursion
    if (next!=NULL) next->ShiftParameters();
    double h=x[0];
    char *hname=XName[0];
    for (int i=0;i<NrX-1;i++) {
      x[i]=x[i+1];
      XName[i]=XName[i+1];
    }
    x[NrX-1]=h;
    XName[NrX-1]=hname;
  }
}

double CMyPoint::y(int i) {
  if (i<NrY) return yb[i]; else {
    char buf[300];
    sprintf(buf,"CMyPoint::y : X not defined Nr %i\n",i);
    MessageBox(GetActiveWindow(),buf,"Error!",MB_OK);
    return 0;
  }
}

int CMyPoint::GetXNrFromName(char *name) {
  int i=0;
  while ((i<NrX) && strncmp(name,XName[i],MaxNameLength)!=0) i++;
  if (i<NrX) return i;
  else return -1;
}

int CMyPoint::GetYNrFromName(char *name) {
  int i=0;
  while ((i<NrY) && strncmp(name,YName[i],MaxNameLength)!=0) i++;
  if (i<NrY) return i;
  else return -1;
}

int CMyPoint::GetTextNrFromName(char *name) {
  int i=0;
  while ((i<NrText) && strncmp(name,TextName[i],MaxNameLength)!=0) i++;
  if (i<NrText) return i;
  else return -1;
}

void CMyPoint::AddXValue(char *name, double value) {
  double *hx=new double[NrX+1];
  char **hxname=new char* [NrX+1];
  for (int i=0;i<NrX;i++) {
    hx[i]=x[i];
    hxname[i]=XName[i];//new char[strlen(XName[i])+1];
//    strcpy(hxname[i],XName[i]);
//    delete[] ;
  }
  delete[] x;
  delete[] XName;
  x=hx;
  XName=hxname;
  x[NrX]=value;
  XName[NrX]=new char[strlen(name)+1];
  strcpy(XName[NrX],name);
  NrX++;
}

void CMyPoint::AddYValue(char *name, double value) {
  double *hy=new double[NrY+1];
  char **hyname=new char* [NrY+1];
  for (int i=0;i<NrY;i++) {
    hy[i]=yb[i];
    hyname[i]=YName[i];//new char[strlen(YName[i])+1];
//    strcpy(hyname[i],YName[i]);
//    delete[] ;
  }
  delete[] yb;
  delete[] YName;
  yb=hy;
  YName=hyname;
  yb[NrY]=value;
  YName[NrY]=new char[strlen(name)+1];
  strcpy(YName[NrY],name);
  NrY++;
}

void CMyPoint::AddTextValue(char *name, char *atext) {
  char **hText=new char*[NrText+1];
  char **hTextname=new char*[NrText+1];
  for (int i=0;i<NrText;i++) {
    if (Text[i]) {
      hText[i]=Text[i];//new char[strlen(Text[i])+1];
//      strcpy(hText[i],Text[i]);
//      delete[] ;
    } else hText[i]=NULL;
    hTextname[i]=TextName[i];//new char[strlen(TextName[i])+1];
//    strcpy(hTextname[i],TextName[i]);
//    delete[] ;
  }
  delete[] Text;
  delete[] TextName;
  Text=hText;
  TextName=hTextname;
  Text[NrText]=new char[strlen(atext)+1];
  strcpy(Text[NrText],atext);
  TextName[NrText]=new char[strlen(name)+1];
  strcpy(TextName[NrText],name);
  NrText++;
}

void CMyPoint::AddDefaultValues(CMyPoint *DefaultPoint) {
/*for (int i=0;i<DefaultPoint->NrX;i++) {
    if (GetXNrFromName(DefaultPoint->XName[i])<0)
      AddXValue(DefaultPoint->XName[i],DefaultPoint->x[i]);
  }*/
  for (int i=0;i<DefaultPoint->NrY;i++) {
    if (GetYNrFromName(DefaultPoint->YName[i])<0)
      AddYValue(DefaultPoint->YName[i],DefaultPoint->yb[i]);
  }
  for (int i=0;i<DefaultPoint->NrText;i++) {
    if (GetTextNrFromName(DefaultPoint->TextName[i])<0)
      AddTextValue(DefaultPoint->TextName[i],DefaultPoint->Text[i]);
  }
}

void CMyPoint::OrderParameters(CMyPoint *DefaultPoint) {
/*  if (NrY>DefaultPoint->NrY) {
    MessageBox(GetActiveWindow(),"Unregistered DataPoints","Error!",MB_OK);
    return;
  }*/
  double *hy=new double[DefaultPoint->NrY];
  char **hyname=new char*[DefaultPoint->NrY];
  for (int i=0;i<DefaultPoint->NrY;i++) {
    hyname[i]=new char[strlen(DefaultPoint->YName[i])+1];
    strcpy(hyname[i],DefaultPoint->YName[i]);
    hy[i]=GetY(DefaultPoint->YName[i]);
  }
  for (int i=0;i<NrY;i++) delete[] YName[i];
  delete[] YName;
  delete[] yb;
  YName=hyname;
  yb=hy;
  NrY=DefaultPoint->NrY;
}

CMyPoint *CMyPoint::GetLastPoint(CMyPoint *FirstPoint) {
  if (FirstPoint) {
    while ((FirstPoint->next) && (FirstPoint->next!=this)) FirstPoint=FirstPoint->next;
    if (FirstPoint->next==this) return FirstPoint; else return NULL;
  } else return NULL;
}
