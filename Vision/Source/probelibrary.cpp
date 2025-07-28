#include "probelibrary.h"
#include "picture.h"
#include "ColorTab.h"
#include <stdio.h>

CProbeLibrary::CProbeLibrary(char *aFilename,int aNrImages) {
  strcpy(Filename,aFilename);
  NrImages=aNrImages;
  LastProbe=0;
  NrInList=0;
  axmin=0;
  axmax=1;
  aymin=0;
  aymax=1;
  axmini=0;
  axmaxi=1;
  aymini=0;
  aymaxi=1;
  for (int i=0;i<NrImages;i++) ProbeList[i]=new CPicture(1,0);
  LoadProbeLibrary();
}

void CProbeLibrary::SaveProbeLibrary(CColorTable *ColorTab) {
  char buf[200];
  for (int j=0;j<NrInList;j++) {
    sprintf(buf,"%s%i.TGA",Filename,j);
    ProbeList[j]->Save(buf,SAVE_AUTOMATIC,ColorTab);
    sprintf(buf,"%s%i.BMP",Filename,j);
    ProbeList[j]->Save(buf,SAVE_PALETTEBMP,ColorTab);
   // sprintf(buf,"%sLSB%i.BMP",Filename,j);
  //  ProbeList[j]->Save(buf,SAVE_LSBPALETTEBMP,ColorTab);
  }
  sprintf(buf,"%s.dat",Filename);
  ofstream out(buf);
  out<<NrInList<<endl;
  out<<LastProbe<<endl;
  out<<axmin<<endl;
  out<<axmax<<endl;
  out<<aymin<<endl;
  out<<aymax<<endl;
  out<<axmini<<endl;
  out<<axmaxi<<endl;
  out<<aymini<<endl;
  out<<aymaxi<<endl;
  out.close();
  ofstream out2("Q:\\ProbeLibraryDebug.txt",ios::app);
  out2<<"ImagesSaved="<<NrInList<<endl;
  out2.close();
}

void CProbeLibrary::LoadProbeLibrary() {
  char buf2[200];
  sprintf(buf2,"%s.dat",Filename);
  ifstream in(buf2);
  in>>NrInList;
  in>>LastProbe;
  char buf3[200];
  in>>buf3;
  axmin=atof(buf3);
  in>>buf3;
  axmax=atof(buf3);
  in>>buf3;
  aymin=atof(buf3);
  in>>buf3;
  aymax=atof(buf3);
  in>>buf3;
  axmini=atof(buf3);
  in>>buf3;
  axmaxi=atof(buf3);
  in>>buf3;
  aymini=atof(buf3);
  in>>buf3;
  aymaxi=atof(buf3);
//  in>>axmax;
 // in>>aymin;
//  in>>aymax;
  in.close();
  if (NrInList>NrImages) NrInList=NrImages;
  char buf[200];
  for (int j=0;j<NrInList;j++) {
    sprintf(buf,"%s%i.TGA",Filename,j);
    ProbeList[j]->Load(buf);
  }
//  LastProbe=NrInList;
  if (LastProbe>=NrImages) LastProbe=0;
  ofstream out2("ProbeLibraryDebug.txt",ios::app);
  out2<<"ImagesRead="<<NrInList<<endl;
  out2.close();
}


CProbeLibrary::~CProbeLibrary() {
  for (int i=0;i<NrImages;i++) delete ProbeList[i];
}

void CProbeLibrary::AddNewImage(CPicture *Probe) {
  if (NrInList<NrImages) NrInList++;
  ProbeList[LastProbe]->Copy(*Probe);
  ofstream out("ProbeLibraryDebug.txt",ios::app);
  out<<"ImageAdded="<<LastProbe<<endl;
  out.close();
  LastProbe++;
  if (LastProbe>=NrImages) LastProbe=0;
}

CPicture* CProbeLibrary::FindBestFit(CPicture *Absorption,double &ImageQuality) {
  ImageQuality=0;
  if (NrInList==0) return Absorption;
  int pxh=Absorption->xh;
  int pyh=Absorption->yh;
  //get real pixels, outer walls
  int xmin=axmin*(pxh-2)+1;
  int ymin=aymin*(pyh-2)+1;
  int xmax=axmax*(pxh-2)+1;
  int ymax=aymax*(pyh-2)+1;
  if (xmin<1) xmin=1; else if (xmin>pxh-1) xmin=pxh-1;
  if (ymin<1) ymin=1; else if (ymin>pyh-1) ymin=pyh-1;
  if (xmax<1) xmax=1; else if (xmax>pxh-1) xmax=pxh-1;
  if (ymax<1) ymax=1; else if (ymax>pyh-1) ymax=pyh-1;

  //inner walls
  int xmini=axmini*(pxh-2)+1;
  int ymini=aymini*(pyh-2)+1;
  int xmaxi=axmaxi*(pxh-2)+1;
  int ymaxi=aymaxi*(pyh-2)+1;
  if (xmini<1) xmini=1; else if (xmini>pxh-1) xmini=pxh-1;
  if (ymini<1) ymini=1; else if (ymini>pyh-1) ymini=pyh-1;
  if (xmaxi<1) xmaxi=1; else if (xmaxi>pxh-1) xmaxi=pxh-1;
  if (ymaxi<1) ymaxi=1; else if (ymaxi>pyh-1) ymaxi=pyh-1;

  if ((pxh<0) || (xmin<0)) return Absorption;
  if ((pyh<0) || (ymin<0)) return Absorption;
  if (xmax>Absorption->xh) return Absorption;
  if (ymax>Absorption->yh) return Absorption;
  if ( ((xmax-xmin)<1) || ((ymax-ymin)<1) ) return Absorption;

  int NrBestFit=-1;
  double MinChiSq=9E99;
  double *Norm=new double[pxh*pyh];

  long NrPixels=double((xmax-xmin)*(ymax-ymin))-double((xmaxi-xmini)*(ymaxi-ymini));
  if (NrPixels==0) return Absorption;

  for (int i=0;i<NrInList;i++) {
    if ((Absorption->yh==ProbeList[i]->yh) && (Absorption->xh==ProbeList[i]->xh)) {

      //use this code if you want the high pass filtered image optimized on ChiSquared
//      double FilterSize=
      //Outer Average
      double FilterSize=6;

      double ChiSq=0;
      //go over FilterSize x FilterSize big boxes over image.
      //we could also use stepsize 1 in principle, gives slightly better result but is more timeconsuming
      for (int xs=xmin;xs<xmax;xs+=FilterSize) {
        for (int ys=ymin;ys<ymax;ys+=FilterSize) {
			 //Now go over this box and calculate its chisquare
          int NrPixels=0;
          double Average=0;
          for (int x=0;x<FilterSize;x++) {
            for (int y=0;y<FilterSize;y++) {
              int xa=xs+x;
              int ya=ys+y;
              // check if in valid range
              if ( (xa<xmax) && (ya<ymax) && ((xa>xmaxi) || (xa<xmini) || (ya>ymaxi) || (ya<ymini)) ) {
                if (double(ProbeList[i]->bitmap[xa+pxh*ya])!=0)
			          Norm[xa+pxh*ya]=double(Absorption->bitmap[xa+pxh*ya])/double(ProbeList[i]->bitmap[xa+pxh*ya]);
         		 else Norm[xa+pxh*ya]=1;
                Average=Average+Norm[xa+pxh*ya];
                NrPixels++;
              }
            }
          }
          if (NrPixels>0) {
            Average=Average/NrPixels;

	         double LocalChiSq=0;
   	      for (int x=0;x<FilterSize;x++) {
      	     for (int y=0;y<FilterSize;y++) {
                int xa=xs+x;
            	 int ya=ys+y;
 	             // check if in valid range
   	          if ( (xa<xmax) && (ya<ymax) && ((xa>xmaxi) || (xa<xmini) || (ya>ymaxi) || (ya<ymini)) ) {
      	         LocalChiSq=LocalChiSq+(Norm[xa+pxh*ya]-Average)*(Norm[xa+pxh*ya]-Average);
         	    }
              }
	         }
   	      LocalChiSq=LocalChiSq/NrPixels;
            ChiSq=ChiSq+LocalChiSq;
          }
        }
      }

      //ofstream out("ProbeLibraryDebug.txt",ios::app);
      //out<<"Fit="<<i<<" ChiSq="<<ChiSq<<endl;
      //out.close();

      if (ChiSq<MinChiSq) {
        MinChiSq=ChiSq;
        NrBestFit=i;
      }

      //end high pass filtered chih squared

      /*use this code if you want the image optimized on ChiSquared
      //Outer Average
      double Average=0;
      for (int x=xmin;x<xmax;x++) {
        for (int y=ymin;y<ymax;y++) {
          if (double(ProbeList[i]->bitmap[x+pxh*y])!=0)
            Norm[x+pxh*y]=double(Absorption->bitmap[x+pxh*y])/double(ProbeList[i]->bitmap[x+pxh*y]);
          else Norm[x+pxh*y]=1;
          Average=Average+Norm[x+pxh*y];
        }
      }

      for (int x=xmini;x<xmaxi;x++) {
        for (int y=ymini;y<ymaxi;y++) {
          Average=Average-Norm[x+pxh*y];
        }
      }

      Average=Average/NrPixels;

      double ChiSq=0;
      for (int x=xmin;x<xmax;x++) {
        for (int y=ymin;y<ymax;y++) {
            ChiSq=ChiSq+(Norm[x+pxh*y]-Average)*(Norm[x+pxh*y]-Average);
        }
      }
      for (int x=xmini;x<xmaxi;x++) {
        for (int y=ymini;y<ymaxi;y++) {
          ChiSq=ChiSq-(Norm[x+pxh*y]-Average)*(Norm[x+pxh*y]-Average);
        }
      }

      if (ChiSq<MinChiSq) {
        MinChiSq=ChiSq;
        NrBestFit=i;
      }

      */

    }
  }
  delete [] Norm;
  ofstream out("ProbeLibraryDebug.txt",ios::app);
  out<<"BestFit="<<NrBestFit<<" ChiSq="<<MinChiSq<<endl;
  out.close();
  ImageQuality=MinChiSq;
  if (NrBestFit>-1) return ProbeList[NrBestFit];
  else return Absorption;
}

void CProbeLibrary::SetRegionOfInterest(double aaxmin,double aaxmax,double aaymin,double aaymax,double aaxmini,double aaxmaxi,double aaymini,double aaymaxi) {
  //normalized in 0..1 meaning 0..xh and so on
	axmin=aaxmin;
	aymin=aaymin;
   axmax=aaxmax;
   aymax=aaymax;
   axmini=aaxmini;
	aymini=aaymini;
   axmaxi=aaxmaxi;
   aymaxi=aaymaxi;
}
