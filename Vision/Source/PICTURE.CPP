#include "picture.h"
#include "atommath.h"
#include "tga.h"
#include <complex.h>
#include <math.h>
#include "bmp.h"
#include "LeakDetect.h"

//*** CPicture ***
CPicture::CPicture(double aEichH, double aOffsetH) {
  EichX=0.05;
  EichY=0.05;
  EichH=aEichH;
  OffsetH=aOffsetH;
  maxx=0;maxy=0;
  bitplanes=8;
  bitmap=NULL;
  xh=0;
  yh=0;
  AktBitmapSize=0;
  InitBitmap(1,1);
}

CPicture::CPicture( int axh, int ayh,
	 double aEichX, double aEichY,double aEichH, double aOffsetH,long value) {
  EichX=aEichX;
  EichY=aEichY;
  EichH=aEichH;
  OffsetH=aOffsetH;
  maxx=0;maxy=0;
  bitmap=NULL;
  xh=0;
  yh=0;
  AktBitmapSize=0;
  InitBitmap(axh,ayh,value);
}

void CPicture::SetEich(double aEichX, double aEichY) {
  EichX=aEichX;
  EichY=aEichY;
}

void CPicture::GetEich(double &aEichX, double &aEichY) {
  aEichX=EichX;
  aEichY=EichY;
}

void CPicture::GetBitmap(TBitmap * &Bitmap, int width, int height, CColorTable *ColorTab, int Zoom, double ZoomCenterX,  double ZoomCenterY) {
int loops=0;
  bool finished=false;
  unsigned long * hbitmap=NULL;
  unsigned long * hbitmap2=NULL;
  while ((!finished) && (loops<10)) {
  if (bitmap==NULL) InitBitmap(1,1);
  if (xh>1) {
  double test=0;
  }
  #if defined(DetectLeaks)
    LeakDetect.Delete(Bitmap);
  #endif
  if (Bitmap) delete Bitmap;
  Bitmap=NULL;
  double sx=double(xh)*EichX;
  double sy=double(yh)*EichY;
  double scalex,scaley;
  int magwidth,magheight;
  if (sx/sy<double(width)/double(height)) {
    double widthnew=height*sx/sy;
    scalex=double(xh)/widthnew;
    scaley=double(yh)/double(height);
    magwidth=width-xh/scalex;
    magheight=height;
  } else {
    double heightnew=double(width)*sy/sx;
    scalex=double(xh)/width;
    scaley=double(yh)/heightnew;
    magwidth=width;
    magheight=height-yh/scaley-2;
  }
  int magcenterheight=height-magheight/2;
  int magcenterwidth=width-magwidth/2;
  //error here 4 times
  try {
	  hbitmap2= new unsigned long[(height+1)*(width+1)+8]; //FS 2025 07 31 added +1 and +8 to avoid access violation
     #if defined(DetectLeaks)
       LeakDetect.New(hbitmap2,2,1);
     #endif
  }
  catch (xalloc) {
      MessageBox(GetActiveWindow(),"CPicture::InitBitmap : couldn't reserve memory","Error",  MB_OK);
      exit(-1);
  }
  if (hbitmap2==NULL) {
  	 MessageBox(GetActiveWindow(),"CPicture::InitBitmap : couldn't reserve memory 2","Error",  MB_OK);
  }
  #if defined(DetectLeaks)
    LeakDetect.Delete(hbitmap);
  #endif
  if (hbitmap) delete hbitmap;
  hbitmap=hbitmap2;
  hbitmap2=NULL;
  int maxj=yh/scaley;
  for (int i=0;i<width;i++) for (int j=0;j<height;j++) {
    int aktx,akty;
    if (Zoom>1) {
    double ZoomValue=Zoom;
    	aktx=(i-width/2)*scalex/ZoomValue+ZoomCenterX/EichX+xh/2;
      akty=(j-height/2)*scaley/ZoomValue-ZoomCenterY/EichY+yh/2;
      bool Black=(aktx>=xh) || (akty>=yh) || (aktx<0) || (akty<0);
      if (Black) hbitmap[i+width*j]=0;
      else {
        TColor col=ColorTab->palette[bitmap[aktx+xh*akty]/(SHIFT*256)];
        hbitmap[i+width*j]=col.Red()*65536+col.Green()*256+col.Blue();
      }
    } else {
      aktx=i*scalex;
      akty=j*scaley;
      if (aktx>=xh || akty>=yh) {
        aktx=(i-magcenterwidth)*scalex/4+xh/2;
        akty=(j-magcenterheight)*scaley/4+yh/2;
        if ((aktx>=xh) || (akty>=yh) || (j<(maxj+2)) || (aktx<0) || (akty<0)) {
          hbitmap[i+width*j]=0;
        } else {
          TColor col=ColorTab->palette[bitmap[aktx+xh*akty]/(SHIFT*256)];
          hbitmap[i+width*j]=col.Red()*65536+col.Green()*256+col.Blue();
        }
      } else {
        if ((aktx>=xh) || (akty>=yh) || (aktx<0) || (akty<0)) {
          hbitmap[i+width*j]=0;
        } else {
          unsigned long pixel;
          if ((scalex>=1.5) || (scaley>=1.5)) {
	          pixel=0;
             int nrpix=0;
             for (int dx=0;dx<scalex;dx++) for (int dy=0;dy<scaley;dy++) {
               if ((aktx+dx<xh) && (akty+dy<yh)) {
                 //stalled here without reason, says can't access bitmap[21248] for a 256*256 bitmap
	             	pixel+=bitmap[aktx+dx+xh*(akty+dy)];
                  nrpix++;
               }
             }
             if (nrpix>0) pixel/=nrpix;
          } else pixel=bitmap[aktx+xh*akty];
          unsigned int upixel=pixel;
          TColor col=ColorTab->palette[upixel/(SHIFT*256)];
          //error occured here
          hbitmap[i+width*j]=col.Red()*65536+col.Green()*256+col.Blue();
        }
      }
    }
    if (hbitmap[i+width*j]>0xFFFFFF) hbitmap[i+width*j]=0xFFFFFF;
  }
  unsigned long StartMem=GetFreeMem();
   try {
     //error here
	  Bitmap=new TBitmap(width,height,1,32,hbitmap);
     #if defined(DetectLeaks)
       LeakDetect.New(Bitmap,2,2);
     #endif  
     finished=true;
     }
  catch (TGdiBase::TXGdi& x) {
    #if defined(DetectLeaks)
      LeakDetect.Delete(Bitmap);
    #endif  
    if (Bitmap) delete Bitmap;
    Bitmap=NULL;

    finished=false;
/*    MessageBeep(0xFFFFFFFF);
    MessageBeep(MB_ICONASTERISK);
    MessageBeep(MB_ICONEXCLAMATION);
    MessageBeep(MB_ICONHAND);
	 MessageBeep(MB_ICONQUESTION);
    MessageBeep(MB_OK);*/
   // MessageBox(GetActiveWindow(),"CPicture::GetBitmap : Catched GDI error!","Error!",MB_OK);
/*     ofstream DebugFile("DebugPicture.dat", ios::app);
     bool found;
     string s=x.ResourceIdToString(&found,x.GetErrorCode());
	  DebugFile<<"GDI error CPicture::GetBitmap "<<width<<" "<<height<<" "<<hbitmap<<" "<<loops<<" "<<StartMem<<" "<<GetFreeMem()<<" ";
     if (found) DebugFile<<s;
     DebugFile<<endl;
	  DebugFile.close();*/
  }
  loops++;
  }
  if ((Bitmap) && (loops>1)) {
/*     ofstream DebugFile("DebugPicture.dat", ios::app);
	  DebugFile<<"CPicture::GetBitmap bitmap ok "<<width<<" "<<height<<" "<<hbitmap<<" "<<loops<<" "<<endl;
	  DebugFile.close();*/
  }
  #if defined(DetectLeaks)
    LeakDetect.Delete(hbitmap);
  #endif  
  delete[] hbitmap;
  hbitmap=NULL;
}

CPicture::CPicture(const CPicture &pic,long value) {
  bitmap=NULL;
  AktBitmapSize=0;
  InitBitmap(pic.xh,pic.yh,value);
  if (value==1) Copy(pic);
  maxx=pic.maxx;maxy=pic.maxy;
  EichX=pic.EichX;EichY=pic.EichY;EichH=pic.EichH;
  OffsetH=pic.OffsetH;
}

void CPicture::Copy(const CPicture &pic) {
  maxx=pic.maxx;maxy=pic.maxy;
  EichX=pic.EichX;EichY=pic.EichY;EichH=pic.EichH;
  OffsetH=pic.OffsetH;
  InitBitmap(pic.xh,pic.yh,0);
  long *s=pic.bitmap;
  long *d=bitmap;
  for (long i=0;i<xh*yh;i++) {*d=*s; d++; s++;};
}

CPicture::~CPicture() {
  DeleteBitmap();
}

void CPicture::InitBitmap(int axh,int ayh,long value) {
  if ((axh!=xh) || (yh!=ayh) || (bitmap==NULL)) {
	  xh=axh;yh=ayh;
     long NewSize=xh*yh;
     if (NewSize>AktBitmapSize) {
        #if defined(DetectLeaks)
  	       LeakDetect.Delete(bitmap);
        #endif  
		  if (bitmap!=NULL) delete[] bitmap;
		  //very often error here
   	  try {
			  bitmap = new long[(xh+1)*(yh+1)+8]; //FS 2025 07 31 added +1 and +8 to avoid access violation
           #if defined(DetectLeaks)
  	          LeakDetect.New(bitmap,2,3);
           #endif  
   	  }
		  catch (xalloc) {
		     MessageBox(GetActiveWindow(),"CPicture::InitBitmap : couldn't reserve memory","Error",  MB_OK);
   	   	exit(-1);
	     }
		  if (bitmap==NULL) {
			  MessageBox(GetActiveWindow(),"CPicture::InitBitmap : couldn't reserve memory","Error",  MB_OK);
		  }
        AktBitmapSize=NewSize;
      }
  }
  long *h=bitmap;
  //error here
  for (long j = 0; j < yh*xh; j++) {*h=value;h++;}
//  for (int x=0;x<xh;x++) for (int y=0;y<yh;y++) bitmap[x+y*xh]=value;
}

void CPicture::ClearBitmap(long value) {
  long *h=bitmap;
  for (long j = 0; j < yh*xh; j++) {*h=value;h++;}
//  for (int x=0;x<xh;x++) for (int y=0;y<yh;y++) bitmap[x+y*xh]=value;
}


void CPicture::DeleteBitmap() {
  //error here
  #if defined(DetectLeaks)
    LeakDetect.Delete(bitmap);
  #endif  
  if (bitmap) delete[] bitmap;
  bitmap=NULL;
}

unsigned char CPicture::GetPixel(double x, double y) {
  unsigned int x1=x*xh;
  if (x1>=xh) x1=xh-1;
  unsigned int y1=y*yh;
  if (y1>=yh) y1=yh-1;
  double h=bitmap[x1+y1*xh];
  if (h>(SHIFT*65535)) h=(SHIFT*65535);
  if (h<0) h=0;
  unsigned char h2=h/(256*SHIFT);
  return h2;
}

void CPicture::GetProfil(long * &profilbuf,double pos,double &aEichH,
 double &aOffsetH,double &Eich,double &Offset,int horiz,int &length,int winheight,int winwidth,  double &endx, double &endy) {
  double sx=double(xh)*EichX;
  double sy=double(yh)*EichY;
  int lengthx, lengthy;
  if (sx/sy<double(winwidth)/double(winheight)) {
    double widthnew=winheight*sx/sy;
    endx=double(winwidth)/widthnew;
    endy=1;
    lengthx=endx*xh;
    lengthy=yh;
  } else {
    double heightnew=double(winwidth)*sy/sx;
    lengthx=xh;
    endx=1;
    endy=double(winheight)/heightnew;
    lengthy=endy*yh;
  }

  aEichH=EichH;
  aOffsetH=OffsetH;
  #if defined(DetectLeaks)
    LeakDetect.Delete(profilbuf);
  #endif
  if (profilbuf!=NULL) delete[] profilbuf;
  profilbuf=NULL;
  if (horiz) {
	 Eich=EichX;
	 length=lengthx;
    if (lengthx>xh) Offset=length*(double(xh)/double(lengthx))*0.5;
    else Offset=length*0.5;
    try {
		 profilbuf=new long[lengthx+8]; //FS 2025 07 31 added some length to avoid access violation
       #if defined(DetectLeaks)
         LeakDetect.New(profilbuf,2,4);
       #endif  
    }
	  catch (xalloc) {
  			MessageBox(GetActiveWindow(),"CPicture::GetProfil : couldn't reserve memory 1","Error",  MB_OK);
      	exit(-1);
	  }
	 unsigned int y1=pos*endy*yh;
    if (xh>lengthx) {
      MessageBox(GetActiveWindow(),"CPicture::GetProfil : Error 1","Error",  MB_OK);
    }
	 if (y1>=yh) {
      long line=((yh*(3.0/4.0)-y1/4)-1);
      if (line<0) line=0;
      if (line>=yh) line=yh-1;
      long *d=profilbuf;
      long *s=&bitmap[xh*line];
      for (int i=0;i<xh;i++) {*d=*s;d++;s++;}
      if (lengthx>xh) for (int i=xh;i<lengthx;i++) *d=0;
    } else {
      long *d=profilbuf;
      long *s=&bitmap[xh*y1];
	   for (int i=0;i<xh;i++) {*d=*s;d++;s++;}
      if (lengthx>xh) for (int i=xh;i<lengthx;i++) *d=0;
    }
  } else {
    Eich=EichY;
	 length=lengthy;
    if (lengthy>yh) Offset=length*(1-(double(yh)/double(lengthy))*0.5);
    else Offset=length*0.5;
    try {
		 profilbuf=new long[lengthy+8]; //FS 2025 07 31 added some length to avoid access violation
       #if defined(DetectLeaks)
         LeakDetect.New(profilbuf,2,5);
       #endif
     }
	  catch (xalloc) {
  			MessageBox(GetActiveWindow(),"CPicture::GetProfil : couldn't reserve memory 2","Error",  MB_OK);
      	exit(-1);
	  }
	 unsigned int x1=pos*endx*xh;
	 if (x1>=xh) for (int i=0;i<lengthy;i++) profilbuf[i]=0;
    else {
    	if (yh>lengthy) {
          MessageBox(GetActiveWindow(),"CPicture::GetProfil : Error 2","Error",  MB_OK);
	   }
      for (int i=0;i<lengthy-yh;i++) {
        long line=((yh*(3.0/4.0)-i/4)-1);
        if (line<0) line=0;
        if (line>=yh) line=yh-1;
        profilbuf[i]=bitmap[x1+xh*line];
      }
	   for (int i=lengthy-yh;i<lengthy;i++) profilbuf[i]=bitmap[x1+xh*(yh-(i-(lengthy-yh))-1)];
    }
  }
}

int CPicture::PictureGood() {
  return 1;
  int good=0;
  for (int i=0;i<xh;i++) if (bitmap[i]!=0) good=1;
  return good;
}

void CPicture::GetPictureAsList(double xmin,double xmax,double ymin,double ymax,
  double * &x, double * &y, int &ndata, int winheight,int winwidth
    ,double &asx,double &asy,double &dx,double &dy,int &nx,int &ny) {

  double sxh=double(xh)*EichX;
  double syh=double(yh)*EichY;
  int lengthx,lengthy;
  double endx,endy;
  if (sxh/syh<double(winwidth)/double(winheight)) {   //portrait
    double widthnew=double(winheight)*sxh/syh;   //real width in DisplayPixel
    endx=widthnew/double(winwidth);     //Maximal useful value of xmin,xmax
    endy=1;
  } else {   //landscape
    double heightnew=double(winwidth)*syh/sxh;   //real height in DisplayPixel
    endx=1;
    endy=heightnew/double(winheight);   //Maximal useful value of ymin,ymax
  }

  int sx=xmin/endx*xh;
  int ex=xmax/endx*xh;
  int sy=ymin/endy*yh;
  int ey=ymax/endy*yh;
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
  nx=(ex-sx);
  ny=(ey-sy);
  ndata=nx*ny;

  asx=(sx-xh*0.5)*EichX/1000;
  asy=(sy-yh*0.5)*EichY/1000;
  dx=EichX/1000;
  dy=EichY/1000;
  try {
	  x=new double[ndata];
     #if defined(DetectLeaks)
       LeakDetect.New(x,2,6);
     #endif  
	  y=new double[ndata];
     #if defined(DetectLeaks)
       LeakDetect.New(y,2,7);
     #endif  
   }
   catch (xalloc) {
  		MessageBox(GetActiveWindow(),"CPicture::GetPictureAsList : couldn't reserve memory","Error",  MB_OK);
      exit(-1);
  }
  int point=0;
  int aktx=sx;
  int akty=sy;
  for (int i=sx;i<ex;i++) for (int j=sy;j<ey;j++) {
	 x[point]=point;
	 y[point]=(double(bitmap[i+xh*j])/(SHIFT*65535.0)-OffsetH)*EichH;
	 point++;
  }
}

void CPicture::GetSum(long * &profilbuf,double pos1, double pos2,double &aEichH,
 double &aOffsetH,double &Eich,double &Offset,int horiz,int &length,int winheight,int winwidth,  double &endx, double &endy ) {
  double sx=double(xh)*EichX;
  double sy=double(yh)*EichY;
  int lengthx, lengthy;
  if (sx/sy<double(winwidth)/double(winheight)) {
    double widthnew=winheight*sx/sy;                  //portrait
    endx=double(winwidth)/widthnew;
    endy=1;
    lengthx=endx*xh;
    lengthy=yh;
  } else {
    double heightnew=double(winwidth)*sy/sx;          //paysage
    lengthx=xh;
    endx=1;
    endy=double(winheight)/heightnew;
    lengthy=endy*yh;
  }

  aEichH=EichH;
  aOffsetH=OffsetH;
  #if defined(DetectLeaks)
    LeakDetect.Delete(profilbuf);
  #endif  
  if (profilbuf!=NULL) delete[] profilbuf;
  profilbuf=NULL;
  if (horiz) {
	 Eich=EichX;
	 length=lengthx;
    if (lengthx>xh) Offset=length*(double(xh)/double(lengthx))*0.5;
    else Offset=length*0.5;
    double * hbuf;
    try {
		 profilbuf=new long[lengthx+8]; //FS 2025 07 31 added some length to avoid access violation
       #if defined(DetectLeaks)
         LeakDetect.New(profilbuf,2,8);
       #endif
       hbuf=new double[lengthx+8]; //FS 2025 07 31 added some length to avoid access violation
       #if defined(DetectLeaks)
         LeakDetect.New(hbuf,2,9);
       #endif  
    }
	  catch (xalloc) {
  			MessageBox(GetActiveWindow(),"CPicture::GetSum : couldn't reserve memory","Error",  MB_OK);
	      exit(-1);
	  }
	 int s=pos1*(yh-1);
	 int e=pos2*(yh-1);
	 if (s>=yh) s=yh-1;
	 if (e>=yh) e=yh-1;
	 double max=0;
	 double min=1E10;
	 for (int i=0;i<xh;i++) {
		hbuf[i]=0;
		for (int j=s*xh;j<=e*xh;j+=xh)  hbuf[i]+=bitmap[i+j];
		hbuf[i]-=(e-s+1)*(SHIFT*65535.0)*OffsetH;
      hbuf[i]*=EichX;
		if (hbuf[i]>max) max=hbuf[i];
		if (hbuf[i]<min) min=hbuf[i];
	 }
    if (lengthx>xh) for (int i=xh;i<lengthx;i++) hbuf[i]=0;
	 if (min>0) min=0;
	 if (max-min<0.01) max=min+0.01;
	 if (max<0.01) max=0.01;
	 aOffsetH=-min/(max-min);
	 long offs=(SHIFT*65535.0)*aOffsetH;
	 double factor=((SHIFT*65535)-offs)/max;
	 aEichH=EichH/factor;
	 for (int i=0;i<lengthx;i++) {
		double h=factor*hbuf[i]+offs;
		if (h>(SHIFT*65535)) h=(SHIFT*65535);
		if (h<0) h=0;
		profilbuf[i]=h;
	 }
    #if defined(DetectLeaks)
      LeakDetect.Delete(hbuf);
    #endif  
	 delete[] hbuf;
    hbuf=NULL;
  } else {
	 Eich=EichY;
	 length=lengthy;
    if (lengthy>yh) Offset=length*(1-(double(yh)/double(lengthy))*0.5);
    else Offset=length*0.5;
    double * hbuf;
    try {
		 profilbuf=new long[lengthy+8]; //FS 2025 07 31 added some length to avoid access violation
       #if defined(DetectLeaks)
   		 LeakDetect.New(profilbuf,2,10);
       #endif  
		 hbuf=new double[lengthy+8];  //FS 2025 07 31 added some length to avoid access violation
       #if defined(DetectLeaks)
         LeakDetect.New(hbuf,2,11);
       #endif  
    }
    catch (xalloc) {
  			MessageBox(GetActiveWindow(),"CPicture::GetSum : couldn't reserve memory 2","Error",  MB_OK);
	      exit(-1);
	  }
	 unsigned int s=pos1*(xh-1);
	 if (s>=xh) s=xh-1;
	 unsigned int e=pos2*(xh-1);
	 if (e>=xh) e=xh-1;
	 double max=0;
	 double min=1E10;
    if (lengthy>yh) for (int i=0;i<lengthy-yh;i++) {
      hbuf[i]=0;
      long line=((yh*(3.0/4.0)-i/4)-1);
      if (line<0) line=0;
      if (line>=yh) line=yh-1;
      long offs=xh*line;
		for (int j=s;j<=e;j++) hbuf[i]+=bitmap[j+offs];
		hbuf[i]-=(e-s+1)*(SHIFT*65535.0)*OffsetH;
      hbuf[i]*=EichX;
		if (hbuf[i]>max) max=hbuf[i];
		if (hbuf[i]<min) min=hbuf[i];
    }
    for (int i=lengthy-yh;i<lengthy;i++) {
		hbuf[i]=0;
      long offs=xh*(yh-(i-(lengthy-yh))-1);
		for (int j=s;j<=e;j++) hbuf[i]+=bitmap[j+offs];
		hbuf[i]-=(e-s+1)*(SHIFT*65535.0)*OffsetH;
      hbuf[i]*=EichX;
		if (hbuf[i]>max) max=hbuf[i];
		if (hbuf[i]<min) min=hbuf[i];
	 }
	 if (min>0) min=0;
	 if (max-min<0.01) max=min+0.01;
	 if (max<0.01) max=0.01;
	 aOffsetH=-min/(max-min);
	 long offs=(SHIFT*65535.0)*aOffsetH;
	 double factor=((SHIFT*65535)-offs)/max;
	 aEichH=EichH/factor;
	 for (int i=0;i<lengthy;i++) {
		double h=factor*hbuf[i]+offs;
		if (h>(SHIFT*65535)) h=(SHIFT*65535);
		if (h<0) h=0;
		profilbuf[i]=h;
	 }
    #if defined(DetectLeaks)
      LeakDetect.Delete(hbuf);
    #endif  
	 delete[] hbuf;
    hbuf=NULL;
  }
}

double CPicture::GetEichPixel(double x, double y) {
  unsigned int x1=x*xh;
  if (x1>=xh) x1=xh-1;
  unsigned int y1=y*yh;
  if (y1>=yh) y1=yh-1;
  return (double(bitmap[x1+xh*y1])/(SHIFT*65535)-OffsetH)*EichH;
}

unsigned long CPicture::ReadPicture( CFrameGrabber *framegrabber,
    double xmin, double xmax, double ymin, double ymax, int quadrant) {
  //DeleteBitmap();
  if (((framegrabber->NumberPictures==1) && (quadrant>0)) || ((framegrabber->NumberPictures==2) && (quadrant>1)) )return 0;
  int BinningX;
  int BinningY;
  int axh;
  int ayh;
  framegrabber->GetImageParameters(axh,ayh,EichX,EichY,BinningX,BinningY,bitplanes);
  InitBitmap(axh,ayh);
  DWORD time=framegrabber->GetPictureData(bitmap,xh*yh,quadrant);
  long * h=bitmap;
  for (long i=0;i<xh*yh;i++) {*h*=SHIFT;h++;};
  SearchMaxXMaxY(xmin,xmax,ymin,ymax);
  return time;
}

long CPicture::SearchMaxXMaxY(double axmin, double axmax, double aymin, double aymax) {
  int xmin=axmin*(xh-2)+1;
  int ymin=aymin*(yh-2)+1;
  int xmax=axmax*(xh-2)+1;
  int ymax=aymax*(yh-2)+1;
  if (xmin<1) xmin=1; else if (xmin>xh-2) xmin=xh-2;
  if (ymin<1) ymin=1; else if (ymin>yh-2) ymin=yh-2;
  if (xmax<1) xmax=1; else if (xmax>xh-2) xmax=xh-2;
  if (ymax<1) ymax=1; else if (ymax>yh-2) ymax=yh-2;
  maxx=0;
  maxy=0;
  long max=0;
  for (int i=xmin;i<xmax;i++)
  for (int j=ymin;j<ymax;j++) {
	 long hmax=0;
	 for (int n=-1;n<=1;n++)
	 for (int m=-1;m<=1;m++) hmax+=bitmap[i+n+xh*(j+m)];
	 if (hmax>max) {
		max=hmax;
		maxx=i;
		maxy=j;
	 }
  }
  return max;
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

void CPicture::CalcNormalizedX0Y0(double &X0, double &Y0,int winheight,int winwidth) {
  double sx=double(xh)*EichX;
  double sy=double(yh)*EichY;
  int lengthx,lengthy;
  double endx,endy;
  if (sx/sy<double(winwidth)/double(winheight)) {
    double widthnew=winheight*sx/sy;                  //portrait
    endx=double(winwidth)/widthnew;
    endy=1;
    lengthx=endx*xh;
    lengthy=yh;
  } else {
    double heightnew=double(winwidth)*sy/sx;          //paysage
    lengthx=xh;
    endx=1;
    endy=double(winheight)/heightnew;
    lengthy=endy*yh;
  }
  Y0=(-Y0+yh*0.5*EichY)/EichY/double(lengthy);
  X0=(X0+xh*0.5*EichX)/EichX/double(lengthx);
}

void CPicture::MandelPicture() {
  for (int i=0;i<xh;i++)
  for (int j=0;j<yh;j++)
  bitmap[i+xh*j]=Mandel(i,j)*SHIFT*256;
  SearchMaxXMaxY();
}

void CPicture::Clear() {
  for (int i=0;i<xh;i++)
  for (int j=0;j<yh;j++)
  bitmap[i+xh*j]=0;
  SearchMaxXMaxY();
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
	 bitmap[i+xh*j]=(20+116*exp(-80*(x*x/xhd+y*y/yhd))+random(10)+100*(i+j)/(xh+yh))*256*SHIFT;
  }
  SearchMaxXMaxY();
}

void CPicture::DisplayFunction(double Params[], double (*func)(double, double, double [])) {
  for (int i=0;i<xh;i++)
  for (int j=0;j<yh;j++) {
	 double x=(i-xh/2)*EichX/1000;
	 double y=(j-yh/2)*EichY/1000;
    double h=(func(x,y,Params)/EichH+OffsetH)*(SHIFT*65535.0);
    if (h>(SHIFT*65535)) h=(SHIFT*65535); else if (h<0) h=0;
	 bitmap[i+xh*j]=h;
  }
  SearchMaxXMaxY();
}

long CPicture::Mandel(int x,int y)
{
  double l=-0.5;
  double u=-0.64;
  double w=0.02;
  double h=0.02;
  double frontier=10000;
  complex z(l+w*x/xh,u+h*y/xh);
  complex c(z);
  int n=0;
  while ((n<255) && abs(z)<frontier)
  {
	 n++;
	 z=(z*z)+c;
  };
  return n;
};

int CPicture::Save(char* name,int modus, CColorTable *ColorTab, int binning, bool floatingbinning) {
  if (modus==SAVE_AUTOMATIC) if (bitplanes==8) modus=SAVE_PALETTEBMP; else modus=SAVE_GRAYSCALETGA;
  if (modus==SAVE_PALETTEBMP || modus==SAVE_LSBPALETTEBMP) {
	  int coarsegrainbinning=1;
	  if (!floatingbinning) coarsegrainbinning=binning;
    WriteBMPFile out(name,xh/coarsegrainbinning,yh/coarsegrainbinning,256,ColorTab->palette);
    if (!out.Open()) return -1;
	
    for (int i=yh/coarsegrainbinning-1;i>=0;i--) {
		if (modus==SAVE_PALETTEBMP) for (int j=0;j<xh/coarsegrainbinning;j++) {
			long help=0;
			for (int x=0;x<binning;x++) for (int y=0;y<binning;y++) {
				long pos=(j+x)*coarsegrainbinning+xh*(i+y)*coarsegrainbinning;
				if (pos<xh*yh) help+=bitmap[pos]/(SHIFT*256);
			}
			help=help/(binning*binning);
			out.Pixel(help); 
	  }
		else for (int j=0;j<xh/coarsegrainbinning;j++) {
			long help=0;
			for (int x=0;x<binning;x++) for (int y=0;y<binning;y++) {
				long pos=(j+x)*coarsegrainbinning+xh*(i+y)*coarsegrainbinning;
				if (pos<xh*yh) help+=bitmap[pos];
			}
			help=help/(binning*binning);
			out.Pixel(0xFF & help/(SHIFT));
		}
	   out.WriteLine();
	   out.NewLine();
    }
  } else if (modus==SAVE_MATRIX) {
    name[strlen(name)-4]=0;
    strcat(name,".DAT");
    ofstream out(name);
//    if (!out.Open()) return -1;
    for (int i=0;i<yh;i++) {
	   for (int j=0;j<xh;j++) out<<(double(bitmap[j+xh*i])/(SHIFT*65535.0)-OffsetH)*EichH<<' ';
	   out<<endl;
    }
  } else {
    TColor *palette=NULL;
    if (modus==SAVE_TRUECOLORTGA) palette=ColorTab->GetPalette16bit();
    name[strlen(name)-4]=0;
    strcat(name,".TGA");
    WriteTGAFile out(name,xh,yh,modus==SAVE_TRUECOLORTGA,palette);
    if (!out.Open()) return -1;
    for (int i=0;i<yh;i++) {
	   for (int j=0;j<xh;j++) out.PutPixel(bitmap[j+xh*i]/SHIFT);
	   out.WriteLine();
    }
  }
  return 0;
}

int CPicture::Load(const char* name) {
  char help[300];
  strcpy(help,name);
  char buf[4];
  while (help[strlen(help)-1]==' ') help[strlen(help)-1]=0;
  strcpy(buf,&help[strlen(help)-3]);
  if (strcmp(buf,"BMP")==0 || strcmp(buf,"bmp")==0) {
    bitplanes=8;
    TColor *palette;
    try {
    	palette = new TColor[256];
      #if defined(DetectLeaks)
        LeakDetect.New(palette,2,12);
      #endif  
    }
    catch (xalloc) {
  			MessageBox(GetActiveWindow(),"CPicture::Load : couldn't reserve memory","Error",  MB_OK);
	      exit(-1);
	  }
    ReadBMPFile in(name,palette);
    #if defined(DetectLeaks)
      LeakDetect.Delete(palette);
    #endif  
    delete[] palette;
    palette=NULL;
    if (!in.Ok()) return -1;
    //DeleteBitmap();
    InitBitmap(in.getBreite(),in.getHoehe());
    for (int i=yh-1;i>=0;i--) {
	   for (int j=0;j<xh;j++) {
  		unsigned long h=in.getPixel(j);
  		bitmap[j+xh*i]=h*(256*SHIFT);
		}
	   if (!in.eof() && (i>0)) in.HoleZeile();
    }
  } else if (strcmp(buf,"TGA")==0 || strcmp(buf,"tga")==0) {
    bitplanes=16;
    ReadTGAFile in(name);
    if (!in.Open()) return -1;
    //DeleteBitmap();
    InitBitmap(in.getBreite(),in.getHoehe());
    for (int i=0;i<yh;i++) {
	   for (int j=0;j<xh;j++) bitmap[j+xh*i]=in.getPixel(j)*SHIFT;
	   if (!in.eof()) in.HoleZeile();
    }
  } else return -1;
//  TurnPicture();
  SearchMaxXMaxY();
  return 0;
}

void CPicture::TurnPicture() {
	long *helpbitmap;
   try {
	   helpbitmap = new long[(xh+1)*(yh+1)+8]; //FS 2025 07 31 added +1 and +8 to avoid access violation
      #if defined(DetectLeaks)
        LeakDetect.New(helpbitmap,2,13);
      #endif  
   }
   catch (xalloc) {
  			MessageBox(GetActiveWindow(),"CPicture::TurnPicture : couldn't reserve memory","Error",  MB_OK);
	      exit(-1);
   }
   for (int i=0;i<yh;i++) {
		helpbitmap[i]=65535/2; //to compensate for noisy line of princeton camera
    	for (int j=1;j<xh;j++) {
      	helpbitmap[i+j*yh]=bitmap[j+i*xh];
      }
    }
    #if defined(DetectLeaks)
      LeakDetect.Delete(bitmap);
    #endif  
  delete[] bitmap;
  bitmap=helpbitmap;
}

CPicture operator-(const CPicture& pic1, const CPicture& pic2) {
  CPicture m(pic1.xh,pic1.yh,pic1.EichX,pic1.EichY,pic1.EichH,pic1.OffsetH);
  for (int i=0;i<pic1.xh;i++) for (int j=0;j<pic1.yh;j++) {
	 m.bitmap[i+pic1.xh*j]=pic1.bitmap[i+pic1.xh*j]-pic2.bitmap[i+pic1.xh*j];
  }
  return m;
}

CPicture operator+(const CPicture& pic1, const CPicture& pic2) {
  CPicture m(pic1.xh,pic1.yh,pic1.EichX,pic1.EichY,pic1.EichH,pic1.OffsetH);
  for (int i=0;i<pic1.xh;i++) for (int j=0;j<pic1.yh;j++) {
	 m.bitmap[i+pic1.xh*j]=pic1.bitmap[i+pic1.xh*j]+pic2.bitmap[i+pic1.xh*j];
  }
  return m;
}

CPicture operator*(const CPicture& pic1, const CPicture& pic2) {
  CPicture m(pic1.xh,pic1.yh,pic1.EichX,pic1.EichY,pic1.EichH,pic1.OffsetH);
  int width=pic1.xh;
  for (int i=0;i<pic1.xh;i++) for (int j=0;j<pic1.yh;j++) {
	 m.bitmap[i+width*j]=pic1.bitmap[i+width*j]*pic2.bitmap[i+width*j];
  }
  return m;
}

CPicture operator/(const CPicture& pic1, const CPicture& pic2) {
  CPicture m(pic1.xh,pic1.yh
  ,pic1.EichX,pic1.EichY,pic1.EichH,pic1.OffsetH);
  for (int i=0;i<pic1.xh;i++) for (int j=0;j<pic1.yh;j++) {
	 if (pic2.bitmap[i+pic1.xh*j]!=0) m.bitmap[i+pic1.xh*j]=pic1.bitmap[i+pic1.xh*j]/pic2.bitmap[i+pic1.xh*j];
	 else m.bitmap[i+pic1.xh*j]=(SHIFT*65535);
  }
  return m;
}

CPicture& CPicture::operator=(const CPicture& pic) {
  if (xh!=pic.xh || yh!=pic.yh) {
	 //DeleteBitmap();
	 InitBitmap(pic.xh,pic.yh);
  }
  Copy(pic);
  return *this;
}

const CPicture CPicture::operator*(const double r) const {
  CPicture m(*this);
  for (int i=0;i<xh;i++) for (int j=0;j<yh;j++) {
	 m.bitmap[i+xh*j]=r*bitmap[i+xh*j];
  }
  return m;
}

void CPicture::NormalizeAbsorption(const CPicture &pic1,const CPicture &pic2,const CPicture &pic3,
  double xmin, double xmax, double ymin, double ymax) {
  EichX=pic1.EichX;
  EichY=pic1.EichY;
  InitBitmap(pic1.xh,pic1.yh);
  for (int i=0;i<xh;i++)
  for (int j=0;j<yh;j++) {
	 if ((pic2.bitmap[i+xh*j]-pic3.bitmap[i+xh*j])!=0) {
		double h=double(pic1.bitmap[i+xh*j]-pic3.bitmap[i+xh*j])/double(pic2.bitmap[i+xh*j]-pic3.bitmap[i+xh*j]);
		h=1-h;
		h=h/EichH+OffsetH;
		if (h>1) h=1; else if (h<0) h=0;
		h=h*(SHIFT*65535)*1;
		if (h>(SHIFT*65535)) h=(SHIFT*65535);
		bitmap[i+xh*j]=h;
	 } else bitmap[i+xh*j]=(SHIFT*65535);
  }
  SearchMaxXMaxY(xmin,xmax,ymin,ymax);
}

void CPicture::Ln(const CPicture &pic1,const CPicture &pic2,const CPicture &pic3,
  double MaxDensity,double ProbeDetuning, double Gamma, double Lambda, double Clebsch,
  double xmin, double xmax, double ymin, double ymax) {
  double factor=1/Sigma(ProbeDetuning, Gamma, Lambda, Clebsch);
  EichX=pic1.EichX;
  EichY=pic1.EichY;
  InitBitmap(pic1.xh,pic1.yh);
  EichH=MaxDensity;
  for (int i=0;i<xh;i++)
  for (int j=0;j<yh;j++) {
	 double h;
	 if ((pic2.bitmap[i+xh*j]-pic3.bitmap[i+xh*j])!=0) {
		h=double(pic1.bitmap[i+xh*j]-pic3.bitmap[i+xh*j])/double(pic2.bitmap[i+xh*j]-pic3.bitmap[i+xh*j]);
		if (h<=0) h=0.000000001;
		h=-log(h)*factor;
		h=h/EichH+OffsetH;
		h=h*(SHIFT*65535.0);
		if (h>(SHIFT*65535)) h=(SHIFT*65535); else if (h<0) h=0;
		bitmap[i+xh*j]=h;
	 }
	 else bitmap[i+xh*j]=(SHIFT*65535);
  }
  SearchMaxXMaxY(xmin,xmax,ymin,ymax);
}

void LnNormalizeAbsorption(const CPicture &pic1,const CPicture &pic2,CPicture *pic3,CPicture *pic4,CPicture &Abs,CPicture &Ln,
    double MaxDensity, double xmin, double xmax, double ymin, double ymax,int FluoImage, double MaxFluorescence, bool DoIntensityCompensation, double *ROI,int winheight,int winwidth,bool FKS) {
  CPicture *noise1;
  noise1=pic3;
  CPicture *noise2;
  if (pic4!=NULL) noise2=pic4; else noise2=pic3;
  Ln.InitBitmap(pic1.xh,pic1.yh);
  Abs.InitBitmap(pic1.xh,pic1.yh);
  Ln.SetEich(pic1.EichX,pic1.EichY);
  Abs.SetEich(pic1.EichX,pic1.EichY);
  //if (OffsetShift==0) OffsetShift=1;
  Ln.EichH=MaxDensity;

  bool err=false;
  int refxh=pic1.xh;
  int refyh=pic1.yh;
  err=err || (refxh!=pic2.xh);
  err=err || (refyh!=pic2.yh);
  if (!FluoImage) {
      err=err || (refxh!=noise1->xh);
      err=err || (refyh!=noise1->yh);
      err=err || (refxh!=noise2->xh);
      err=err || (refyh!=noise2->yh);
  }
  if (err) {
    MessageBox(GetActiveWindow(),"CPicture::LnNormalizeAbsorption : picture format error!","Error!",MB_OK);
    return;
  }
  if (FluoImage) {
    long max=-SHIFT*10000;
    long min=SHIFT*10000;
    Ln.EichH=1;
    for (int i=0;i<Ln.xh;i++)
    for (int j=0;j<Ln.yh;j++) {
	   long h2=pic1.bitmap[i+pic1.xh*j]-pic2.bitmap[i+pic1.xh*j];
      if (h2<min) min=h2;
      if (h2>max) max=h2;
      double h=h2;
      h/=SHIFT;//*MaxFluorescence;//16384;
      if (MaxFluorescence<=0) MaxFluorescence=1;
      h/=MaxFluorescence;
		h=h/(1+2*Ln.OffsetH)+Ln.OffsetH;
		if (h>1) h=1; else if (h<0) h=0;
		h=h*(SHIFT*65535)*1;
		if (h>(SHIFT*65535)) h=(SHIFT*65535);
		Ln.bitmap[i+pic1.xh*j]=h;
   }
   Abs.EichH=1;
   if (max==min) max=min+1;
   for (int i=0;i<Ln.xh;i++)
   for (int j=0;j<Ln.yh;j++) {
      double h=pic1.bitmap[i+pic1.xh*j]-pic2.bitmap[i+pic1.xh*j];
      h/=(double)(max-min);
		h=h/(1+2*Abs.OffsetH)+Abs.OffsetH;
		if (h>1) h=1; else if (h<0) h=0;
		h=h*(SHIFT*65535)*1;
		if (h>(SHIFT*65535)) h=(SHIFT*65535);
     	Abs.bitmap[i+pic1.xh*j]=h;
    }
  } else {
	  double ReScale=1;
	  if (DoIntensityCompensation) {
       double NoiseOffset1=noise1->Offset(
			ROI[2],
			ROI[0],
			ROI[3],
			ROI[1],
			ROI[6],
			ROI[4],
			ROI[7],
			ROI[5],
			winheight,winwidth);
	   double NoiseOffset2;
	   if (FKS) {
		   NoiseOffset2=noise2->Offset(
			ROI[2],
			ROI[0],
			ROI[3],
			ROI[1],
			ROI[6],
			ROI[4],
			ROI[7],
			ROI[5],
			winheight,winwidth);
	   } else NoiseOffset2=NoiseOffset1;

      double help=pic2.Offset(
			ROI[2],
			ROI[0],
			ROI[3],
			ROI[1],
			ROI[6],
			ROI[4],
			ROI[7],
			ROI[5],
			winheight,winwidth)-NoiseOffset2;
      if (fabs(help)<1E-10) help=1;
		ReScale=(pic1.Offset(
			ROI[2],
			ROI[0],
			ROI[3],
			ROI[1],
			ROI[6],
			ROI[4],
			ROI[7],
			ROI[5],
			winheight,winwidth)-NoiseOffset1)/help;
	  }
	for (int i=0;i<Ln.xh;i++)
    for (int j=0;j<Ln.yh;j++) {
    double h, h2;
  	 	if ((pic2.bitmap[i+pic1.xh*j]-noise2->bitmap[i+pic1.xh*j])!=0) {
    		h2=h=double(pic1.bitmap[i+pic1.xh*j]-noise1->bitmap[i+pic1.xh*j])/
            (ReScale*(double(pic2.bitmap[i+pic1.xh*j])-double(noise2->bitmap[i+pic1.xh*j])));
			h2=1-h2;
			h2=h2/Abs.EichH+Abs.OffsetH;
			if (h2>1) h2=1; else if (h2<0) h2=0;
			h2=h2*(SHIFT*65535)*1;
			if (h2>(SHIFT*65535)) h2=(SHIFT*65535);
			Abs.bitmap[i+pic1.xh*j]=h2;

		  //	if (OpticalDensityMethod==0) {
   	     if (h<=0) h=0.000000001;
      	  h=-log(h);
	        h=h/Ln.EichH+Ln.OffsetH;
	        h=h*(SHIFT*65535.0);
   	     if (h>(SHIFT*65535)) h=(SHIFT*65535); else if (h<0) h=0;
      	  Ln.bitmap[i+pic1.xh*j]=h;
	      /*} else {
   	     h=OpticalDensity(double(pic1.bitmap[i+pic1.xh*j]-noise1->bitmap[i+pic1.xh*j]),
      	                   double(pic2.bitmap[i+pic1.xh*j]-noise2->bitmap[i+pic1.xh*j]));
	        h=h/Ln.EichH+Ln.OffsetH;
           h=h*(SHIFT*65535.0);
	        if (h>(SHIFT*65535)) h=(SHIFT*65535); else if (h<0) h=0;
   	     Ln.bitmap[i+pic1.xh*j]=h;
      	} */
		 } else {
			Ln.bitmap[i+pic1.xh*j]=(SHIFT*65535);
			Abs.bitmap[i+pic1.xh*j]=(SHIFT*65535);
	 	 }
    }
  }
  Ln.SearchMaxXMaxY(xmin,xmax,ymin,ymax);
  Abs.SetMaxXMaxY(Ln.MaxX(),Ln.MaxY());
}

void SubstractNoise(const CPicture &pic1,const CPicture &pic2,const CPicture &pic3,
  CPicture &Abs,CPicture &Ln) {
  Ln.InitBitmap(pic1.xh,pic1.yh);
  Abs.InitBitmap(pic1.xh,pic1.yh);
  Ln.SetEich(pic1.EichX,pic1.EichY);
  Abs.SetEich(pic1.EichX,pic1.EichY);
  Ln.EichH=pic1.EichH;
  long *a,*l,*p1,*p2,*p3;
  a=Abs.bitmap;
  l=Ln.bitmap;
  p1=pic1.bitmap;
  p2=pic2.bitmap;
  p3=pic3.bitmap;
  for (int n=0;n<Ln.xh*Ln.yh;n++) {
    *a++=((32768+128)*SHIFT)+*p1++ -*p3;
    *l++=((32768+128)*SHIFT)+*p2++ -*p3++;
  }
}

void Substract(const CPicture &pic1,const CPicture &pic2,CPicture &pic3) {
  pic3.InitBitmap(pic1.xh,pic1.yh);
  pic3.SetEich(pic1.EichX,pic1.EichY);
  pic3.EichH=pic1.EichH;
  pic3.OffsetH=pic1.OffsetH;
  long *p1,*p2,*p3;
  p1=pic1.bitmap;
  p2=pic2.bitmap;
  p3=pic3.bitmap;
  for (int n=0;n<pic3.xh*pic3.yh;n++) {
    *p3++=((32768+128)*SHIFT)+(*p1++ -*p2++)*5;
  }
}

void Copy(const CPicture &pic1, CPicture &pic2) {
  pic2.InitBitmap(pic1.xh,pic1.yh);
  pic2.SetEich(pic1.EichX,pic1.EichY);
  pic2.EichH=pic1.EichH;
  pic2.OffsetH=pic1.OffsetH;
  long *p1,*p2;
  p1=pic1.bitmap;
  p2=pic2.bitmap;
  for (int n=0;n<pic2.xh*pic2.yh;n++) {
    *p2++=*p1++;
  }
}

double CPicture::GetAbsorption(double x0,double y0) {
  if (EichX<1E-10) EichX=1;
  if (EichY<1E-10) EichY=1;
  if (y0>1E10) y0=1E10;
  if (y0<-1E10) y0=-1E10;
  if (x0>1E10) x0=1E10;
  if (x0<-1E10) x0=-1E10;
  int xp=(x0/EichX)+xh/2;
  if (xp<2) xp=2;
  if (xp>=xh-2) xp=xh-3;
  int yp=y0-(y0/EichY)+yh/2;
  if (yp<2) yp=2;
  if (yp>=yh-3) yp=yh-3;
  double sum=0;
  for (int i=-2;i<=2;i++) for (int j=-2;j<=2;j++) sum+=bitmap[xp+i+xh*(yp+j)];
  sum/=25;
  return (sum/(SHIFT*65535.0)-OffsetH)*EichH;
}

double CPicture::GetRealPosX(double ax,int winheight,int winwidth) {     //ax from 0...1
  double sxh=double(xh)*EichX;
  double syh=double(yh)*EichY;
  double endx,endy;
  if (sxh/syh<double(winwidth)/double(winheight)) {   //portrait screen window relative to cameras picture
    double widthnew=double(winheight)*sxh/syh;   //real width in DisplayPixel
    endx=widthnew/double(winwidth);     //Maximal useful value of xmin,xmax
  } else {   //landscape
    double heightnew=double(winwidth)*syh/sxh;   //real height in DisplayPixel
    endx=1;
  }
  int sx=ax/endx*xh;
  if (sx>xh) sx=xh;
  if (sx<0) sx=0;
  return (sx-0.5*xh)*EichX;
}

double CPicture::GetRealPosY(double ay,int winheight,int winwidth) {
  double sxh=double(xh)*EichX;
  double syh=double(yh)*EichY;
  double endx,endy;
  if (sxh/syh<double(winwidth)/double(winheight)) {   //portrait
    double widthnew=double(winheight)*sxh/syh;   //real width in DisplayPixel
    endy=1;
  } else {   //landscape
    double heightnew=double(winwidth)*syh/sxh;   //real height in DisplayPixel
    endy=heightnew/double(winheight);   //Maximal useful value of ymin,ymax
  }
  int sy=(endy-ay)/endy*yh;
  if (sy>yh) sy=yh;
  if (sy<0) sy=0;
  return (sy-0.5*yh)*EichY;
}                                                        

double CPicture::GetMaxUsefulHeight(int winheight,int winwidth) {
	double sxh=double(xh)*EichX;
  double syh=double(yh)*EichY;
  if (sxh/syh<double(winwidth)/double(winheight)) {   //portrait
	  return 1;
  } else {   //landscape
     double heightnew=double(winwidth)*syh/sxh;   //real height in DisplayPixel
     return heightnew/double(winheight);   //Maximal useful value of ymin,ymax
  }
}

double CPicture::GetMaxUsefulWidth(int winheight,int winwidth) {
	double sxh=double(xh)*EichX;
  double syh=double(yh)*EichY;
   if (sxh/syh<double(winwidth)/double(winheight)) {   //portrait
    double widthnew=double(winheight)*sxh/syh;   //real width in DisplayPixel
    return widthnew/double(winwidth);     //Maximal useful value of xmin,xmax
  } else {   //landscape
    return 1;
  }
}

double CPicture::Integral(double xmin, double ymin, double xmax, double ymax,
    bool FitWithOffset,double xminOf, double yminOf, double xmaxOf, double ymaxOf,
    int winheight,int winwidth,double &Offset) {

  double sxh=double(xh)*EichX;
  double syh=double(yh)*EichY;
  int lengthx,lengthy;
  double endx,endy;
  if (sxh/syh<double(winwidth)/double(winheight)) {   //portrait
    double widthnew=double(winheight)*sxh/syh;   //real width in DisplayPixel
    endx=widthnew/double(winwidth);     //Maximal useful value of xmin,xmax
    endy=1;
  } else {   //landscape
    double heightnew=double(winwidth)*syh/sxh;   //real height in DisplayPixel
    endx=1;
    endy=heightnew/double(winheight);   //Maximal useful value of ymin,ymax
  }

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

  int sxOf=xminOf*xh;
  int exOf=xmaxOf*xh;
  int syOf=yminOf*yh;
  int eyOf=ymaxOf*yh;
  if (exOf<sxOf) exOf=sxOf+1;
  if (eyOf<syOf) eyOf=syOf+1;
  if (sxOf>xh) sxOf=xh;
  if (exOf>xh) exOf=xh;
  if (syOf>yh) syOf=yh;
  if (eyOf>yh) eyOf=yh;
  if (sxOf<0) sxOf=0;
  if (syOf<0) syOf=0;
  if (exOf<0) exOf=0;
  if (eyOf<0) eyOf=0;

  double sum=0;
  for (int i=sx;i<ex;i++) for (int j=sy;j<ey;j++) sum+=double(bitmap[i+xh*j]);
  double divisor=double(ex-sx)*double(ey-sy);
  if (fabs(divisor)<0.00001) divisor=1;
  sum/=divisor;
  sum=(sum/(SHIFT*65535.0)-OffsetH)*EichH;
  if (FitWithOffset) {
    double sumOf=0;
    double NumOf=0;
    for (int i=sxOf;i<exOf;i++) for (int j=syOf;j<eyOf;j++) {
      if (i<sx || i>=ex || j<sy || j>=ey) {
        NumOf+=1;
        sumOf+=double(bitmap[i+xh*j]);
      }
    }
    if (NumOf!=0) {
      sumOf/=NumOf;
      sumOf=(sumOf/(SHIFT*65535.0)-OffsetH)*EichH;
    } else sumOf=0;
    Offset=sumOf;
    sum-=sumOf;
  } else {
    Offset=0;
  }
  sum*=double(ex-sx)*double(ey-sy)*EichX*EichY;
  return sum;
}

double CPicture::RMSNoise(double xmin, double ymin, double xmax, double ymax,
    bool FitWithOffset,double xminOf, double yminOf, double xmaxOf, double ymaxOf,
    int winheight,int winwidth) {
  double sxh=double(xh)*EichX;
  double syh=double(yh)*EichY;
  int lengthx,lengthy;
  double endx,endy;
  if (sxh/syh<double(winwidth)/double(winheight)) {   //portrait
    double widthnew=double(winheight)*sxh/syh;   //real width in DisplayPixel
    endx=widthnew/double(winwidth);     //Maximal useful value of xmin,xmax
    endy=1;
  } else {   //landscape
    double heightnew=double(winwidth)*syh/sxh;   //real height in DisplayPixel
    endx=1;
    endy=heightnew/double(winheight);   //Maximal useful value of ymin,ymax
  }

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

  int sxOf=xminOf*xh;
  int exOf=xmaxOf*xh;
  int syOf=yminOf*yh;
  int eyOf=ymaxOf*yh;
  if (exOf<sxOf) exOf=sxOf+1;
  if (eyOf<syOf) eyOf=syOf+1;
  if (sxOf>xh) sxOf=xh;
  if (exOf>xh) exOf=xh;
  if (syOf>yh) syOf=yh;
  if (eyOf>yh) eyOf=yh;
  if (sxOf<0) sxOf=0;
  if (syOf<0) syOf=0;
  if (exOf<0) exOf=0;
  if (eyOf<0) eyOf=0;

	double sumOf=0;
	double NumOf=0;
	for (int i=sxOf;i<exOf;i++) for (int j=syOf;j<eyOf;j++) {
	  if (i<sx || i>=ex || j<sy || j>=ey) {
		NumOf+=1;
		sumOf+=double(bitmap[i+xh*j]);
	  }
	}
	
	if (NumOf!=0) {
	  sumOf/=NumOf;
	} else sumOf=0;
	
	double sumMS=0;	
	for (int i=sxOf;i<exOf;i++) for (int j=syOf;j<eyOf;j++) {
	  if (i<sx || i>=ex || j<sy || j>=ey) {
		NumOf+=1;
		double h=(double(bitmap[i+xh*j])-sumOf);
		sumMS+=h*h;
	  }
	}
	if (NumOf!=0) {
	  sumMS/=NumOf;
	}
  return sumMS;
}

double CPicture::Offset(double xmin, double ymin, double xmax, double ymax,
    double xminOf, double yminOf, double xmaxOf, double ymaxOf,
    int winheight,int winwidth) {
  double sxh=double(xh)*EichX;
  double syh=double(yh)*EichY;
  int lengthx,lengthy;
  double endx,endy;
  if (sxh/syh<double(winwidth)/double(winheight)) {   //portrait
    double widthnew=double(winheight)*sxh/syh;   //real width in DisplayPixel
    endx=widthnew/double(winwidth);     //Maximal useful value of xmin,xmax
    endy=1;
  } else {   //landscape
    double heightnew=double(winwidth)*syh/sxh;   //real height in DisplayPixel
    endx=1;
    endy=heightnew/double(winheight);   //Maximal useful value of ymin,ymax
  }

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

  int sxOf=xminOf*xh;
  int exOf=xmaxOf*xh;
  int syOf=yminOf*yh;
  int eyOf=ymaxOf*yh;
  if (exOf<sxOf) exOf=sxOf+1;
  if (eyOf<syOf) eyOf=syOf+1;
  if (sxOf>xh) sxOf=xh;
  if (exOf>xh) exOf=xh;
  if (syOf>yh) syOf=yh;
  if (eyOf>yh) eyOf=yh;
  if (sxOf<0) sxOf=0;
  if (syOf<0) syOf=0;
  if (exOf<0) exOf=0;
  if (eyOf<0) eyOf=0;

	double sumOf=0;
	double NumOf=0;
	for (int i=sxOf;i<exOf;i++) for (int j=syOf;j<eyOf;j++) {
	  if (i<sx || i>=ex || j<sy || j>=ey) {
		NumOf+=1;
		sumOf+=double(bitmap[i+xh*j]);
	  }
	}
	
	if (NumOf!=0) {
	  sumOf/=NumOf;
	} else sumOf=0;
	
  return sumOf;
}

/*
double CPicture::Integral(double xmin, double ymin, double xmax, double ymax,
  bool FitWithOffset/*,int winheight,int winwidth) {

 /* double sxh=double(xh)*EichX;
  double syh=double(yh)*EichY;
  int lengthx,lengthy;
  double endx,endy;
  if (sxh/syh<double(winwidth)/double(winheight)) {
    double widthnew=winheight*sxh/syh;
    endx=double(winwidth)/widthnew;
    endy=1;
    lengthx=endx*xh;
    lengthy=yh;
  } else {
    double heightnew=double(winwidth)*syh/sxh;
    lengthx=xh;
    endx=1;
    endy=double(winheight)/heightnew;
    lengthy=endy*yh;
  }

  int sy=ymin*endy*yh;
  int ey=ymax*endy*yh;
  if (sy>yh) sy=yh;
  if (ey>yh) ey=yh;

  int sx=xmin*endx*xh;
  int ex=xmax*endx*xh;
  if (sx>xh) sx=xh;
  if (ex>xh) ex=xh;


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
  for (int i=sx;i<ex;i++) for (int j=sy;j<ey;j++) sum+=double(bitmap[i+xh*j]);
  sum/=double(ex-sx)*double(ey-sy);
  if (FitWithOffset) {
    double sumrest=0;
    int nrest=0;
    for (int j=0;j<sy;j++) for (int i=0;i<xh;i++)  sumrest+=double(bitmap[i+xh*j]);
    nrest+=(sy-0)*(xh-0);
    for (int j=sy;j<ey;j++) {
      for (int i=0;i<sx;i++)  sumrest+=double(bitmap[i+xh*j]);
      for (int i=ex;i<xh;i++)  sumrest+=double(bitmap[i+xh*j]);
    }
    nrest+=(sy-ey)*(sx-0);
    nrest+=(sy-ey)*(xh-ex);
    for (int j=ey;j<yh;j++) for (int i=0;i<xh;i++)  sumrest+=double(bitmap[i+xh*j]);
    nrest+=(yh-ey)*(xh-0);
    if (nrest!=0) {
      sumrest/=double(nrest);
      sum-=sumrest;
    }
  }
  sum=(sum/(SHIFT*65535.0)-OffsetH)*EichH;
  sum*=double(ex-sx)*double(ey-sy)*EichX*EichY;
  return sum;
} */

double CPicture::CalcRMSNoise(double xmin, double ymin, double xmax, double ymax) {
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
  double mittel=0;
  for (int i=sx;i<ex;i++) for (int j=sy;j<ey;j++) mittel+=double(bitmap[i+xh*j]);
  mittel/=(ex-sx)*(ey-sy);
  mittel=(mittel/(SHIFT*65535.0)-OffsetH)*EichH;
  double sum=0;
  for (int i=sx;i<ex;i++) for (int j=sy;j<ey;j++) {
    double deviation=(double(bitmap[i+xh*j])/(SHIFT*65535.0)-OffsetH)*EichH-mittel;
    sum+=deviation*deviation;
  }
  sum/=(ex-sx)*(ey-sy);
  return sqrt(sum);
}

void CPicture::GetMinMax(double xmin, double ymin, double xmax, double ymax,double &min,double &max) {
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
  min=0xFFFF;
  max=0;
  for (int i=sx;i<ex;i++) for (int j=sy;j<ey;j++) {
    double h=double(bitmap[i+xh*j]);
    if (h>max) max=h;
    if (h<min) min=h;
  }
  min=(min/(SHIFT*65535.0)-OffsetH)*EichH;
  max=(max/(SHIFT*65535.0)-OffsetH)*EichH;
}

double CPicture::WinCoordsX(double PicCoordsX,int winwidth, int winheight) {
  if (double(xh)/double(yh)<double(winwidth)/double(winheight)) {
    return PicCoordsX*double(xh)/double(yh);
  } else {
    return PicCoordsX;
  }
}

double CPicture::WinCoordsY(double PicCoordsY,int winwidth, int winheight) {
  if (double(xh)/double(yh)<double(winwidth)/double(winheight)) {
    return PicCoordsY;
  } else {
    return PicCoordsY*double(yh)/double(xh);
  }
}


double CPicture::PicCoordsY(double WinCoordsY,int winwidth, int winheight) {
  if ((winheight==0) || (yh==0)) return WinCoordsY;
  double sxh=double(xh)*EichX;
  double syh=double(yh)*EichY;
  if (sxh/syh<double(winwidth)/double(winheight)) {   //portrait
    return WinCoordsY;
  } else {   //landscape
    double heightnew=double(winwidth)*syh/sxh;   //real height in DisplayPixel
    double endy=heightnew/double(winheight);   //Maximal useful value of ymin,ymax
    WinCoordsY/=endy;
    if (WinCoordsY>1) WinCoordsY=1;
    return WinCoordsY;
  }
}

double CPicture::PicCoordsX(double WinCoordsX,int winwidth, int winheight) {
  if ((yh==0) || (winheight==0)) return WinCoordsX;
  double sxh=double(xh)*EichX;
  double syh=double(yh)*EichY;
  if (sxh/syh<double(winwidth)/double(winheight)) {   //portrait
    double widthnew=double(winheight)*sxh/syh;   //real width in DisplayPixel
    double endx=widthnew/double(winwidth);     //Maximal useful value of xmin,xmax
    WinCoordsX/=endx;
    if (WinCoordsX>1) WinCoordsX=1;
    return WinCoordsX;
  } else {   //landscape
    return WinCoordsX;
  }
}

void CPicture::Extract(CPicture **result,int FKSOffset,int FKSHeight,int FKSDistance,
    int FKBining,int LineShift, double PixelFractionX, double PixelFractionY, int subpicture ) {
  if ((FKSHeight%FKBining!=0) || (yh<2*FKSHeight)) {
    MessageBox(GetActiveWindow(),"CPicture::Extract parameters wrong","Error!",MB_OK);
    try {
	    *result=new CPicture(*this);
       #if defined(DetectLeaks)
         LeakDetect.New(result,2,14);
       #endif  
    }
    catch (xalloc) {
  			MessageBox(GetActiveWindow(),"CPicture::Extract : couldn't reserve memory","Error",  MB_OK);
	      exit(-1);
	  }
    return;
  }
  try {
	  *result=new CPicture(xh/FKBining,FKSHeight/FKBining,EichX*FKBining,EichY*FKBining,EichH,OffsetH);
     #if defined(DetectLeaks)
       LeakDetect.New(result,2,15);
     #endif  
  }
  catch (xalloc) {
  			MessageBox(GetActiveWindow(),"CPicture::Extract : couldn't reserve memory 2","Error",  MB_OK);
	      exit(-1);
	  }
  long size=xh*yh;
  long help;
  int RowShift=0;
  if (PixelFractionX<0) {
    RowShift=-1-floor(-PixelFractionX);
    PixelFractionX=1-(-PixelFractionX+floor(-PixelFractionX));
  } else {
    RowShift=floor(PixelFractionX);
    PixelFractionX-=RowShift;
  }
  if (PixelFractionY<0) {
    LineShift+=-1-floor(-PixelFractionY);
    PixelFractionY=1-(-PixelFractionY+floor(-PixelFractionY));
  } else {
    LineShift+=floor(PixelFractionY);
    PixelFractionY-=floor(PixelFractionY);
  }
  double px=PixelFractionX;
  double py=PixelFractionY;
  double px1=1-PixelFractionX;
  double py1=1-PixelFractionY;
  if (PixelFractionY!=0 || PixelFractionX!=0) {
    for (int i=0;i<xh/FKBining;i++) for (int j=0;j<FKSHeight/FKBining;j++) {
      long sum=0;
      for (int i2=0;i2<FKBining;i2++) for (int j2=0;j2<FKBining;j2++) {
        long xp=i*FKBining+i2+RowShift;
        long yp=subpicture*FKSDistance+(j+FKSOffset)*FKBining+j2+LineShift;
        help=xp+xh*yp;
        double p00=0;
        double p01=0;
        double p10=0;
        double p11=0;
        if ((0<=help) && (help<size)) p00=bitmap[help];
        help+=1;
        if ((0<=help) && (help<size)) p01=bitmap[help];
        help+=xh;
        if ((0<=help) && (help<size)) p11=bitmap[help];
        help-=1;
        if ((0<=help) && (help<size)) p10=bitmap[help];
        sum+=p00*(px*py)+p01*(px1*py)+p10*(px*py1)+p11*(px1*py1);
      }
      (*result)->bitmap[i+j*xh/FKBining]=sum/(FKBining*FKBining);
    }
  } else {
    for (int i=0;i<xh/FKBining;i++) for (int j=0;j<FKSHeight/FKBining;j++) {
      long sum=0;
      for (int i2=0;i2<FKBining;i2++) for (int j2=0;j2<FKBining;j2++)
        help=subpicture*FKSDistance*xh+i*FKBining+i2+RowShift+((j+FKSOffset)*FKBining+j2+LineShift)*xh;
        if ((0<=help) && (help<size)) sum+=bitmap[help];
      (*result)->bitmap[i+j*xh/FKBining]=sum/(FKBining*FKBining);
    }
  }
}

/*void CPicture::Extract(CPicture **result,int FKSHeight,int FKSDistance,int FKBining,int LineShift, int subpicture ) {
  if ((FKSHeight%FKBining!=0) || (yh<2*FKSHeight)) {
    MessageBox(GetActiveWindow(),"CPicture::Extract parameters wrong","Error!",MB_OK);
    *result=new CPicture(*this);
    return;
  }
  *result=new CPicture(xh/FKBining,FKSHeight/FKBining,EichX*FKBining,EichY*FKBining,EichH,OffsetH);
  long size=xh*yh;
  long help;
  for (int i=0;i<xh/FKBining;i++) for (int j=0;j<FKSHeight/FKBining;j++) {
    long sum=0;
    for (int i2=0;i2<FKBining;i2++) for (int j2=0;j2<FKBining;j2++)
      help=subpicture*FKSDistance*xh+i*FKBining+i2+(j*FKBining+j2+LineShift)*xh;
      if ((0<=help) && (help<size)) sum+=bitmap[help];
    (*result)->bitmap[i+j*xh/FKBining]=sum/(FKBining*FKBining);
  }
}*/


void CPicture::Add(const CPicture &pic2) {
  for (int i=0;i<xh;i++) for (int j=0;j<yh;j++) {
	 bitmap[i+xh*j]+=pic2.bitmap[i+xh*j];
  }
}

void CPicture::Divide(long d) {
  if (d>0) for (int i=0;i<xh;i++) for (int j=0;j<yh;j++) {
	 bitmap[i+xh*j]/=d;
  }
}

void CPicture::ShiftColumn(int nr) {
   if (nr>xh) return;
   for (int i=0;i<xh;i++) for (int j=0;j<yh;j++) {
     if ((nr+i)<xh)
    	  bitmap[i+xh*j]=bitmap[i+nr+xh*j];
     else bitmap[i+xh*j]=bitmap[i+nr-xh+xh*j];
  }
}

void CPicture::ShiftRow(int nr) {
   if (nr>yh) return;
   for (int i=0;i<xh;i++) for (int j=0;j<yh;j++) {
     if ((nr+j)<yh)
    	  bitmap[i+xh*j]=bitmap[i+xh*(j+nr)];
     else bitmap[i+xh*j]=bitmap[i+xh*(j+nr-yh)];
  }
}

void CPicture::Line(double ax0,double ay0,double ax1, double ay1,long color, int width) {
  int x0=ax0*(xh-2)+1;
  int y0=ay0*(yh-2)+1;
  int x1=ax1*(xh-2)+1;
  int y1=ay1*(yh-2)+1;

  if ((x0<0) || (x0>=xh)) return;
  if ((y0<0) || (y0>=yh)) return;
  double dx=x1-x0;
  double dy=y1-y0;
  double Length=sqrt(dx*dx+dy*dy);
  if (Length<1) return;
  dx=dx/Length;
  dy=dy/Length;
  for (int i=0;i<int(Length);i++) {
    int x=x0+i*dx;
    int y=y0+i*dy;
	for (int w=0;w<width;w++) {		
		if ((x<0) || (x>=xh)) return;
		if ((y<0) || (y>=yh)) return;
		bitmap[x+xh*y]=color;
		x++;
		y++;
	}	
  }
}

unsigned long CPicture::GetFreeMem() {
/*typedef struct _MEMORYSTATUS { // mst
    DWORD dwLength;        // sizeof(MEMORYSTATUS)
    DWORD dwMemoryLoad;    // percent of memory in use
    DWORD dwTotalPhys;     // bytes of physical memory
    DWORD dwAvailPhys;     // free physical memory bytes
    DWORD dwTotalPageFile; // bytes of paging file
    DWORD dwAvailPageFile; // free bytes of paging file
    DWORD dwTotalVirtual;  // user bytes of address space
    DWORD dwAvailVirtual;  // free user bytes
} MEMORYSTATUS, *LPMEMORYSTATUS; */
 LPMEMORYSTATUS  mem;
 mem=new MEMORYSTATUS;
 GlobalMemoryStatus(mem);
 unsigned long FreeMem=mem->dwAvailPhys;
 delete mem;
 return FreeMem;
 }

