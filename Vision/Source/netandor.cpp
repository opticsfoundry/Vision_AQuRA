#include "netandor.h"
#include "stdio.h"
#include "math.h"
//#include <stdio.h>
//#include <math.h>
#include "leakdetect.h"

CNetAndorFrameGrabber::CNetAndorFrameGrabber(TWindow *aparent, char *aIPAdresse,  ushort aport, bool HardwareAccess, int aFrambeGrabberNumber)
  : CFrameGrabber() {
  FrambeGrabberNumber=aFrambeGrabberNumber;
  parent=aparent;
  strcpy(IPAdresse,aIPAdresse);
  port=aport;
  if (HardwareAccess) {
  		network=new CNetwork(parent,IPAdresse, port,false,false,true);
  		if (!network->IsConnected()) {
        char buff[400];
        sprintf(buff,"CNetAndorFrameGrabber:: not connected 1 Camera Nr %i IP:%s Port:%i",FrambeGrabberNumber,IPAdresse, port);
        MessageBox(GetActiveWindow(),buff,"Error",  MB_OK);
      } 
  } else network=NULL;
  #if defined(DetectLeaks)
     LeakDetect.New(network,8,1);
  #endif
   hbin=0;
   vbin=0;
   xstart=0;
   ystart=0;
   width=512;
   height=512;
   ExposureTime=20;
   Temperature=0;
   ExternalTrigger=0;
   vSpeed=16;
   hSpeed=1;
   FKSMode=0;
   FKSHeight=100;
   FKSDataImages=1;
   FKSReferenceImages=1;
   FKSvSpeed=2;
   FKSExternalTriggerStart=0;
   NumberPictures=1;
   PrePicture=false;
   DoInternalTriggerAfterExternalTrigger=false;
   FlushWait=100;
   EichX=1;
   EichY=1;
   LastUsed=GetTickCount();
   for (int i=0;i<MaxPictureData;i++) PictureData[i]=NULL;   
 	PictureHeight=0;
   PictureWidth=0;
}

CNetAndorFrameGrabber::~CNetAndorFrameGrabber() {
  if (network) {
//  		network->WriteCommand("GoodBye");
  		DebugOff();
      #if defined(DetectLeaks)
        LeakDetect.Delete(network);
      #endif
  		delete network;
  }
  for (int i=0; i<MaxPictureData;i++) {
     #if defined(DetectLeaks)
        LeakDetect.Delete(PictureData[i]);
     #endif
     if (PictureData[i]!=NULL) delete[] PictureData[i];
  }
}

void CNetAndorFrameGrabber::DebugOn(bool Append) {
	if (network) {
	   char DebugFilename[200];
      sprintf(DebugFilename,"NetAndorDebug%i.dat",FrambeGrabberNumber);
   	network->DebugOn(DebugFilename,Append);
   }
}

void CNetAndorFrameGrabber::DebugOff() {
	if (network) network->DebugOff();
}

void CNetAndorFrameGrabber::FlushNetwork() {
  if (!network) return;
  //Communication breaks down if none has occured for a few minutes
  //reset connection in that case
  if ((abs(GetTickCount()-LastUsed)>5*60*1000) ) {
		network->Flush(0);
      network->WriteCommand("GoodBye");
      network->DebugOff();
      #if defined(DetectLeaks)
        LeakDetect.Delete(network);
      #endif
  		delete network;
      DWORD Start=GetTickCount();
      while (abs(GetTickCount()-Start)<200) Sleep(10);
      network=new CNetwork(parent,IPAdresse, port,false,false,true);
      #if defined(DetectLeaks)
        LeakDetect.New(network,8,2);
      #endif
      Start=GetTickCount();
      while (abs(GetTickCount()-Start)<200) Sleep(10);
		if (!network->IsConnected()) MessageBox(GetActiveWindow(),"CNetAndorFrameGrabber:: not connected 2","Error",  MB_OK);
      network->Flush(500);
  }
  LastUsed=GetTickCount();
  network->Flush(0);
}

int CNetAndorFrameGrabber::TakeTriggeredPicture() {
  if (!network) return 1;
 // FlushNetwork();
  if (network->WriteCommand("Image")) {
  } else MessageBox(GetActiveWindow(),"CNetAndorFrameGrabber::TakeTriggeredPicture 1","Error",  MB_OK);
  return 1;
}


unsigned long CNetAndorFrameGrabber::TransferPictureData(bool CreateSimulatedData) {
    long size=0;
    if (network->DebugFile) {
     (*network->DebugFile)<<GetTickCount()<<" << TransferPictureData CreateSuimulatedData = "<<(CreateSimulatedData) <<" network"<<(network!=NULL)<<endl;
     network->DebugFile->flush();
   }

    if (network && (!CreateSimulatedData)) network->WriteCommand("Transfer");
	//if (!FKSMode) {
    size=(height*width)/(hbin*vbin);

    // if Vision doesn't have the correct sizes alrady allocate the memory
    if ((PictureHeight!=(height/vbin)) || (PictureWidth!=(width/hbin))) {
//        char buff2[100];
//        sprintf(buff2, "height is:%u %u, width is:%u %u", PictureHeight, height, PictureWidth, width);
//        MessageBox(GetActiveWindow(), buff2, "Msg title", MB_OK | MB_ICONQUESTION);
//        //MessageBox(GetActiveWindow(), "Picture Size Problem detected", "Error", MB_OK);
      	PictureHeight=height/vbin;
        PictureWidth=width/hbin;
        for (int i=0; i<(4+NumberOfAdditionalReferenceImages);i++) {
            #if defined(DetectLeaks)
                LeakDetect.Delete(PictureData[i]);
            #endif
	        if (PictureData[i]!=NULL) delete[] PictureData[i];
            try {
	   	        PictureData[i]=new unsigned short[size];
            }
            catch (xalloc) {
			  	    MessageBox(GetActiveWindow(),"CNetAndorFrameGrabber::TransferPictureData : couldn't reserve memory","Error",  MB_OK);
			        exit(-1);
		    }
            #if defined(DetectLeaks)
                LeakDetect.New(PictureData[i],8,3);
            #endif
        }
	}


	char buff[100];
    ////string name = "stackoverflow";
    int numpics = (NumberPictures + NumberOfAdditionalReferenceImages);
    //// sprintf_s(buff, "name is:%s", name.c_str());
    //// sprintf(buff, "name is:%s", name.c_str());
    //sprintf(buff, "size is:%u, pics:%u", size, numpics);
    ////cout << buff;
    //MessageBox(GetActiveWindow(), buff, "Msg title", MB_OK | MB_ICONQUESTION);
    ////MessageBox(GetActiveWindow(), buff, "Msg title", MB_OK | MB_ICONQUESTION);

	  
	  
    for (int i=0;i<(NumberPictures+NumberOfAdditionalReferenceImages);i++) {
        if (network && (!CreateSimulatedData)) {
            if (!network->WriteCommand("Ready")) MessageBox(GetActiveWindow(),"CNetAndorFrameGrabber::TransferPictureData 1","Error",  MB_OK);
            network->ReadHugeData((char*)PictureData[i],2*size);
        } else {
            //create test pictures
            //sprintf(buff, "Test pic:%u", i);
            //MessageBox(GetActiveWindow(), buff, "Create Test Image", MB_OK | MB_ICONQUESTION);
             if (i==0) {
           	    for (long x=0;x<PictureWidth;x++) {
                        for (long y=0;y<PictureHeight;y++) PictureData[i][x+y*PictureWidth]=256*(x+y)+256*((double)rand())/RAND_MAX;
                }
            } else {
                for (long j=0;j<size;j++) PictureData[i][j]=0;
    //                for (long x = 0; x < PictureWidth; x++) {
    //                    for (long y = 0; y < PictureHeight; y++) PictureData[i][x + y * PictureWidth] = 0;
    //                }
                //               for (long j=0;j<size;j++) PictureData[i][j]=256*((double)rand())/RAND_MAX;
    ////               MessageBox(GetActiveWindow(), buff, "UnHappy 3", MB_OK | MB_ICONQUESTION);
            }
        }
    }

    //cheapy method to replace missing pictures
    //its main function is at the moment to double the noise picture in non FKS mode
    //can be made much better,e.g. hard disk buffer of old picture parts
//      if (NumberPictures<4) {
//        for (int i=NumberPictures;i<4;i++) {
    if (NumberPictures==3) {
        for (long j=0;j<size;j++) {
//            PictureData[i+NumberOfAdditionalReferenceImages][j]=PictureData[NumberPictures+NumberOfAdditionalReferenceImages-1][j];
        PictureData[3+NumberOfAdditionalReferenceImages][j]=PictureData[2+NumberOfAdditionalReferenceImages][j];

    //cheapy method to replace missing 4th ref picture
	//its function is to double the noise picture in non FKS mode
//      if (NumberPictures==3) {
//          for (long j=0;j<size;j++) {
//            PictureData[3+NumberOfAdditionalReferenceImages][j]=PictureData[2+NumberOfAdditionalReferenceImages][j];
//					}
        }
    }
    // } else {
    //    //for FKSimages, the CCD chip is turned by 90 degrees
    //    //->height and width are exchanged
    //    int FKSDirtyLinesCut=ceil(((double)FKSDirtyLines)/vbin);
	    //FKSDirtyLinesCut=FKSDirtyLinesCut*vbin;
	    //long SubImageSize=FKSDataImages*(width*(FKSHeight-FKSDirtyLinesCut))/(vbin*hbin);
    //    int NewPictureHeight=width/vbin;
    //    int NewPictureWidth=FKSDataImages*(FKSHeight-FKSDirtyLinesCut)/hbin;
    //    size=NewPictureHeight*NewPictureWidth;
	    // if ((PictureHeight!=NewPictureHeight) || (PictureWidth!=NewPictureWidth)) {
    //    	PictureHeight=NewPictureHeight;
    //       PictureWidth=NewPictureWidth;
    //       for (int i=0; i<(4+NumberOfAdditionalReferenceImages*2);i++) {
    //          #if defined(DetectLeaks)
    //            LeakDetect.Delete(PictureData[i]);
    //          #endif
	    //        if (PictureData[i]!=NULL) delete [] PictureData[i];
    //          try {
	    //           PictureData[i]=new unsigned short[size];
    //          }
	    //		catch (xalloc) {
    //		  		MessageBox(GetActiveWindow(),"CNetAndorFrameGrabber::TransferPictureData : couldn't reserve memory","Error",  MB_OK);
	    //	      exit(-1);
	    //	  }
    //         #if defined(DetectLeaks)
    //           LeakDetect.New(PictureData[i],8,3);
    //         #endif
    //       }
    //    }
	    // 
	    // int NumberDataPicture=NumberPictures+1+NumberOfAdditionalReferenceImages*2;  	  
    //    if (network && (!CreateSimulatedData)) {
    //    	//first FKS image contains raw absorption 
	    ////second and so on contains probe beam w/o atoms		  
    //    	//second to last contains noise to raw abs only
    //    	//last contains noise to probe beam only
	    //  //each AdditionalReferenceImages contains 2 images: one taken like the image w atoms, one taken like the image w/o atoms
    //    	for (int i=0;i<NumberDataPicture;i++) {
    //       	if (!network->WriteCommand("Ready")) MessageBox(GetActiveWindow(),"CNetAndorFrameGrabber::TransferPictureData 3","Error",  MB_OK);
    //       	if (NumberDataPicture<MaxPictureData) network->ReadHugeData((char*)PictureData[i],2*size);
	    //	else network->ReadHugeData((char*)PictureData[MaxPictureData-1],2*size);
    //    	}
    //    } else {
    //      //create pictures for testing
	    // if (NumberDataPicture<MaxPictureData) NumberDataPicture=MaxPictureData-1;
    //    	for (int i=0;i<NumberDataPicture;i++) {
    //      		if (i==0) {
    //             //raw abs
    //             int start=rand();
    //             int pictype=rand() % 7;
    //             int speed=16+64*rand()/RAND_MAX;
    //             for (long x=0;x<PictureWidth;x++) {
    //               for (long y=0;y<PictureHeight;y++) {
    //                 switch (pictype) {
    //                 	case 0:PictureData[i][x+y*PictureWidth]=30000-(speed*(start+x+y) % 30000);break;
    //                   case 1:PictureData[i][x+y*PictureWidth]=30000-(speed*(start+x) % 30000);break;
    //                   case 2:PictureData[i][x+y*PictureWidth]=30000-(speed*(start+y) % 30000);break;
    //                   case 3:PictureData[i][x+y*PictureWidth]=30000-(speed*(start+x-y*y) % 30000);break;
    //                   case 4:PictureData[i][x+y*PictureWidth]=30000-(speed*(start+x*x-y) % 30000);break;
    //                   case 5:PictureData[i][x+y*PictureWidth]=30000-(speed*(start+x*x+y*y) % 30000);break;
    //                   case 6:PictureData[i][x+y*PictureWidth]=30000-(speed*(start+x-y) % 30000);break;
    //                 }
    //               }
    //             }
    //      		} else if (i==1) {
    //             //probe
    //      			for (long j=0;j<size;j++) PictureData[i][j]=30000;//-128*((double)rand())/RAND_MAX;
    //      		} else {
    //             //noise
    //      			for (long j=0;j<size;j++) PictureData[i][j]=0;//16*((double)rand())/RAND_MAX;
    //      		}
    //       }
    //    }
    //    //cheapy method to replace missing pictures
    //    //can be made much better,e.g. hard disk buffer of old picture parts
    //    if (NumberDataPicture<(4+NumberOfAdditionalReferenceImages*2)) {
    //      for (int i=(NumberDataPicture+NumberOfAdditionalReferenceImages*2);i<(4+NumberOfAdditionalReferenceImages*2);i++) {
    //        if (i<MaxPictureData) for (long j=0;j<size;j++) {
    //          PictureData[i][j]=0;
    //        }
    //      }
    //    }
    // }
    return 0;
}

int CNetAndorFrameGrabber::GetNrAdditionalProbePictures() {
	int AktNrAddRefImages;
	if (FKSMode) AktNrAddRefImages=NumberOfAdditionalReferenceImages*2;
	else AktNrAddRefImages=NumberOfAdditionalReferenceImages;
	return AktNrAddRefImages;
}

unsigned long CNetAndorFrameGrabber::GetPictureData(long* aPictureData,const long size,int quadrant) {
	//quadrant=0: pic with atoms
	//1: ref w/o atoms
	//2: noise of pic w atoms
	//3: noise of pic w/o atoms
	//4,... : additional ref pics w/o atoms

	int AktNrAddRefImages=GetNrAdditionalProbePictures();
	
	int PictureNr;
	if (quadrant<2) PictureNr=quadrant;  //case 0,1
	else if ((quadrant==3) || (quadrant==2)) PictureNr=quadrant+AktNrAddRefImages;  //case 2,3
   else PictureNr=quadrant-2;
  if (PictureNr>MaxPictureData) PictureNr=MaxPictureData;
  if (PictureNr<0) PictureNr=0;
  if (size!=(PictureWidth*PictureHeight)) return 1;
  for (int y=0;y<PictureHeight;y++) {
	  for (int x=0;x<PictureWidth;x++) {
      aPictureData[x+y*PictureWidth]=(long)(((unsigned short*)(PictureData[PictureNr]))[x+y*PictureWidth]);
    }
  }
  return 0;
}

void CNetAndorFrameGrabber::SetImageParameters(const char* aCameraDirection,
    bool aCameraUsed, int aDisplayedPictureNumber, int aMainCamera, int ahbin, int avbin, int axstart,
    int aystart, int awidth, int aheight, double aExposureTime, double aTemperature,
    int aExternalTrigger, int avSpeed, double ahSpeed, int aFKSMode, int aFKSHeight,
    int aFKSDataImages, int aFKSReferenceImages, int aFKSvSpeed, int aFKSExternalTriggerStart, int aFKSDirtyLines,
    int aNumberPictures, bool aPrePicture, int aFlushWait, bool aDoInternalTriggerAfterExternalTrigger, int aTriggerDelay, double aAtomicMass,
    double aAtomicWavelength, double aAtomicClebschGordon, double aAtomicLinewidth,
    double aAtomicMagneticMoment, char* aAtomicName,
    double aProbeDetuning, double aExpansionTime, bool aFluorescencePicture,
    double aMaxFluorescence, double aCalibration,
    char* aFinalTrap, char* aEndState, double aEichX, double aEichY, int aNumberOfAdditionalReferenceImages, int aDepthOfReferenceImageLibrary) {
    //void CNetAndorFrameGrabber::SetImageParameters(const char* aCameraDirection,
    //    bool aCameraUsed, int aDisplayedPictureNumber, int aMainCamera, int ahbin, int avbin, int axstart,
    //    int aystart, int awidth, int aheight, int aExposureTime, int aTemperature,
    //    int aExternalTrigger, int avSpeed, int ahSpeed, int aFKSMode, int aFKSHeight,
    //    int aFKSDataImages, int aFKSReferenceImages, int aFKSvSpeed, int aFKSExternalTriggerStart, int aFKSDirtyLines,
    //    int aNumberPictures, bool aPrePicture, int aFlushWait, bool aDoInternalTriggerAfterExternalTrigger, int aTriggerDelay, double aAtomicMass,
    //    double aAtomicWavelength, double aAtomicClebschGordon, double aAtomicLinewidth,
    //    double aAtomicMagneticMoment, char* aAtomicName,
    //    double aProbeDetuning, double aExpansionTime, bool aFluorescencePicture,
    //    double aMaxFluorescence, double aCalibration,
    //    char* aFinalTrap, char* aEndState, double aEichX, double aEichY, int aNumberOfAdditionalReferenceImages, int aDepthOfReferenceImageLibrary) {

    CFrameGrabber::SetImageParameters(aCameraDirection,aAtomicMass,aAtomicWavelength,aAtomicClebschGordon,aAtomicLinewidth,
    aAtomicMagneticMoment,aAtomicName,aProbeDetuning,aExpansionTime,aFluorescencePicture,
    aMaxFluorescence,aCalibration,aFinalTrap,aEndState);

   MainCamera=aMainCamera;
   hbin=ahbin;
   ExternalTrigger=aExternalTrigger;
   vbin=avbin;
   xstart=axstart;
   ystart=aystart;
   width=awidth;
   height=aheight;
   EichX=aEichX*hbin;
   EichY=aEichY*vbin;
   ExposureTime=aExposureTime;
   Temperature=aTemperature;
   FlushWait=aFlushWait;
   ExternalTrigger=aExternalTrigger;
   vSpeed=avSpeed;
   hSpeed=ahSpeed;
   FKSMode=aFKSMode;
   FKSHeight=aFKSHeight;
   FKSDataImages=aFKSDataImages;
   FKSReferenceImages=aFKSReferenceImages;
   FKSvSpeed=aFKSvSpeed;
   FKSExternalTriggerStart=aFKSExternalTriggerStart;
   FKSDirtyLines=aFKSDirtyLines;
	NumberPictures=aNumberPictures;
   PrePicture=aPrePicture;
   DoInternalTriggerAfterExternalTrigger=aDoInternalTriggerAfterExternalTrigger;
   TriggerDelay=aTriggerDelay;
   CameraUsed=aCameraUsed;
   DisplayedPictureNumber=aDisplayedPictureNumber;
   NumberOfAdditionalReferenceImages=aNumberOfAdditionalReferenceImages;
	DepthOfReferenceImageLibrary=aDepthOfReferenceImageLibrary;
//   if (xstart>1024) xstart=1024;
   if (xstart>20480) xstart=20480;
   if (xstart<0) xstart=0;
   if (ystart<0) ystart=0;
   if (ystart>20480) ystart=20480;
//   if (ystart>1024) ystart=1024;
   if (xstart+width>20480) width=20480-xstart;
   if (ystart+height>20480) height=20480-ystart;
//   if (xstart+width>1024) width=1024-xstart;
//   if (ystart+height>1024) height=1024-ystart;
   SendImageParameters();
}

void CNetAndorFrameGrabber::SetExternalTrigger(int aExternalTrigger) {
	ExternalTrigger=aExternalTrigger;
}

void CNetAndorFrameGrabber::SendImageParameters() {
	if (!network) return;
//   FlushNetwork();
   if (network->WriteCommand("Parameters",/*DisplayErrorMessage*/false)) {
    network->WriteInt(hbin);
    network->WriteInt(vbin);
    network->WriteInt(xstart);
    network->WriteInt(xstart+width-1);    //xmax
    network->WriteInt(ystart);
    network->WriteInt(ystart+height-1);   //ymax
    network->WriteDouble(ExposureTime);  //in ms
    //network->WriteInt(ExposureTime);  //in ms
    //network->WriteInt(Temperature);
    network->WriteDouble(Temperature); // gain[dB]
    network->WriteInt(ExternalTrigger);
    network->WriteInt(vSpeed);
    //network->WriteInt(hSpeed);
    network->WriteDouble(hSpeed); // brightness
    network->WriteInt(FKSMode);
    network->WriteInt(FKSHeight);
    network->WriteInt(FKSDataImages);
	network->WriteInt(FKSReferenceImages);
    network->WriteInt(FKSvSpeed);
    network->WriteInt(FKSExternalTriggerStart);
    network->WriteInt(FKSDirtyLines);
    network->WriteInt(NumberPictures);
    network->WriteInt((PrePicture) ? 1 : 0);
    network->WriteInt(FlushWait);
	network->WriteInt((DoInternalTriggerAfterExternalTrigger) ? 1 : 0);
	network->WriteInt(NumberOfAdditionalReferenceImages);	
  } //else MessageBox(GetActiveWindow(),"CNetAndorFrameGrabber::SendImageParameters","Error 1",  MB_OK);
}

bool CNetAndorFrameGrabber::TestReady() {
  if (!network) return true;
  bool ok=true;
  if (network->WriteCommand("Ready",/*DisplayErrorMessage*/false)) {
		if (!network->CheckReady(1000)) {
         ok=false;
	      FlushNetwork();
         network->WriteCommand("Ready");
	      if (!network->CheckReady(1000)) {
            char err[400];
       		sprintf(err,"CNetAndorFrameGrabber::TestReady 1 Camera Nr %i IP:%s",FrambeGrabberNumber,IPAdresse);
            MessageBox(GetActiveWindow(),err,"Error 1",  MB_OK);
         } else {
				DebugOn(true);
            network->DebugWrite("Communication reset");
         }
      }
  } else {
  		char err[400];
	 	sprintf(err,"CNetAndorFrameGrabber::TestReady 2 Camera Nr %i IP:%s",FrambeGrabberNumber,IPAdresse);
     // MessageBox(GetActiveWindow(),err,"Error 2",  MB_OK);
      ok=false;
  }
  return ok;
}

void CNetAndorFrameGrabber::GetImageParameters(int &xh, int &yh, double &aEichX, double &aEichY, int &aBinningX, int &aBinningY, int &bitplanes) {
    if (!FKSMode) {
    	yh=height/hbin;
    	xh=width/vbin;
    	aEichX=EichX;
    	aEichY=EichY;
      aBinningX=hbin;
      aBinningY=vbin;
    	bitplanes=16;
    } else {
      //in FKSMode, picture is turned by 90 degrees
      int FKSDirtyLinesCut=ceil(((double)FKSDirtyLines)/vbin);
	  FKSDirtyLinesCut=FKSDirtyLinesCut*vbin;
      int NewPictureHeight=width/vbin;
      int NewPictureWidth=FKSDataImages*(FKSHeight-FKSDirtyLinesCut)/hbin;
      xh=NewPictureWidth;
    	yh=NewPictureHeight;
    	aEichX=EichX;
    	aEichY=EichY;
    	bitplanes=16;
      aBinningX=hbin;
      aBinningY=vbin;
    }
}

void CNetAndorFrameGrabber::ShowCameraInformations() {
  bool ready=false;
  if (network) {
    if (!network->WriteCommand("Ready")) MessageBox(GetActiveWindow(),"CNetAndorFrameGrabber::ShowCameraInformations","Error",  MB_OK);
  	   ready=network->CheckReady();
  }
  int temp;
  char buffer[1024];
  char buffer2[1024];
  wsprintf(buffer,"");
  sprintf(buffer2,"Horizontal Start=%i Vertical Start=%i\n",xstart,ystart);
  strcat(buffer,buffer2);
  sprintf(buffer2,"Image width=%i Image Height=%i\n",width,height);
  strcat(buffer,buffer2);
  sprintf(buffer2,"Horizontal Calibration=%.2f �m/Pixel\nVertical Calibration=%.2f �m/Pixel\n",EichX*1000,EichY*1000);
  strcat(buffer,buffer2);
  if (ready) strcat(buffer,"Camera ready");
  else strcat(buffer,"Camera not ready");
  MessageBox(GetActiveWindow(),buffer,"Net Andor Camera Information",MB_OK);
}

bool CNetAndorFrameGrabber::CheckReady(unsigned long timeout) {
  if (network) return network->CheckReady(timeout);
  else return false;
}


