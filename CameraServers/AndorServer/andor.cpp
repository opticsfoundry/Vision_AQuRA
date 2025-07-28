// Andor.cpp : implementation file
//

#include "stdafx.h"
#include "Andor.h"
#include "atmcd32d.h"
#include <afx.h>
#include <stdlib.h>
#include ".\andor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAndor

CAndor::CAndor(bool aHardwareAccess,unsigned short* ImagePtrVal,unsigned short* FKSImagePtrVal)
{
	DoInternalTriggerAfterExternalTrigger=false;
	NumberOfAdditionalReferenceImages=0;
	HardwareAccess=aHardwareAccess;
	hBin=1;
	vBin=1;
	xMax=512;
	xMin=1;
	yMin=1;
	yMax=512;
	GoalTemperature=0;
	LastTemperature=99;
	FlushWait=0;
	ExternalTrigger=false;
	MinTemp=99;
	MaxTemp=99;
	CoolerOn=false;
	StartTime=0;
	StartAcquisitionTime=0;
	initialized=false;
	NrHorizontalSpeeds=0;
	NrVerticalSpeeds=0;
	NrFKVerticalSpeeds=0;
	HorizontalSpeed=NULL;
	VerticalSpeed=NULL;
	FKSVerticalSpeed=NULL;	
	FKSExternalTriggerStart=false;
	FKSDirtyLines=0;
	hSpeed=2;
	vSpeed=16;
	FKSvSpeed=0;
	FKSMode=false;
	FKSHeight=512;
	FKSImages=2;
	FKSDataImages=1;
	FKSReferenceImages=1;
	NrADChannels=1;
	NrAmp=1;
	NrPreAmpGain=1;
	PixelSizeY=13;
	PixelSizeX=13;
	ExposureTime=10;
	NumberPictures=1;
	PrePicture=false;
	if (!HardwareAccess) {
		Width=1024;
		Height=1024;
	}
	ImagePtr=ImagePtrVal;
	ImageSize=1002*1004;
	FKSImagePtr=FKSImagePtrVal;
	FKSImageSize=1002*1004;
}

bool CAndor::InitCamera(char* directory)
{	
	if (!HardwareAccess) return true;
	char aBuffer[2048];
	char aBuffer2[256];
	int errorValue;
	bool errorFlag;
	errorFlag=false;
	initialized=false;

	/*long TotalCameras;
	errorValue=GetAvailableCameras(&TotalCameras);
	if(errorValue!=DRV_SUCCESS){
      strcat_s(aBuffer,2048,"Initialize Error\n");
      errorFlag=TRUE;
    }
	long CameraHandle;
	errorValue=GetCameraHandle(0,&CameraHandle);
	if(errorValue!=DRV_SUCCESS){
      strcat_s(aBuffer,2048,"Initialize Error\n");
      errorFlag=TRUE;
    }
	errorValue=SetCurrentCamera(CameraHandle);
	if(errorValue!=DRV_SUCCESS){
      strcat_s(aBuffer,2048,"Initialize Error\n");
      errorFlag=TRUE;
    }*/
	
	 GetCurrentDirectory(2048,aBuffer);// Look in current working directory
                                    // for driver files
	errorValue=Initialize(aBuffer);  // Initialize driver using detector.ini in current directory
	wsprintf(aBuffer,"Initialization errors:\n");
	if (errorValue!=DRV_SUCCESS) {
		sprintf_s(aBuffer2,256,"Initialize Error %i\n",errorValue);
		strcat_s(aBuffer,2048,aBuffer2);
		errorFlag=true;
	}
	// Get detector information
	errorValue=GetDetector(&Width,&Height);
	if (errorValue!=DRV_SUCCESS){
		sprintf_s(aBuffer2,256,"Get Detector information Error %i\n",errorValue);
		strcat_s(aBuffer,2048,aBuffer2);
		errorFlag=true;
	}
	xMax=xMin+Width-1;
	yMax=yMin+Height-1;
	errorValue=SetImage(hBin,vBin,xMin,xMax,yMin,yMax);
	if (errorValue!=DRV_SUCCESS) {
		errorFlag=true;
		sprintf_s(aBuffer2,256,"SetImage3 %i\n",errorValue);
		sprintf_s(aBuffer2,256,"%i %i %i %i %i %i\n",hBin,vBin,xMin,xMax,yMin,yMax);
		strcat_s(aBuffer,2048,aBuffer2);
	}
	// Set read mode to full image
	errorValue=SetReadMode(4); 
	if (errorValue!=DRV_SUCCESS){
		sprintf_s(aBuffer2,256,"Set Read Mode Error %i\n",errorValue);
		strcat_s(aBuffer,2048,aBuffer2);
		errorFlag=true;
	}
	//GetTemperatureRange
	errorValue=GetTemperatureRange(&MinTemp,&MaxTemp);
	if(errorValue!=DRV_SUCCESS) {
		sprintf_s(aBuffer2,256,"Temperature error %i\n",errorValue);
		strcat_s(aBuffer,2048,aBuffer2);
		errorFlag=true;
	}
	// Save Start Time, we have to Wait for 2 seconds to allow MCD to
	// calibrate fully before allowing an acquisition to begin
	StartTime=GetTickCount();
	if (!errorFlag) {
		errorValue=SetShutter(1,1,1,1); 
		if (errorValue!=DRV_SUCCESS) {
			sprintf_s(aBuffer2,256,"SetShutter error %i\n",errorValue);
			strcat_s(aBuffer,2048,aBuffer2);
			errorFlag=true;
		};
		errorValue=GetNumberHSSpeeds(0,0,&NrHorizontalSpeeds);
		if (errorValue!=DRV_SUCCESS) {
			sprintf_s(aBuffer2,256,"GetNumberHSSpeeds error %i\n",errorValue);
			strcat_s(aBuffer,2048,aBuffer2);
			errorFlag=true;
		};
		errorValue=GetNumberVSSpeeds(&NrVerticalSpeeds);
		if (errorValue!=DRV_SUCCESS) {
			sprintf_s(aBuffer2,256,"GetNumberVerticalSpeeds error %i\n",errorValue);
			strcat_s(aBuffer,2048,aBuffer2);
			errorFlag=true;
		};
		errorValue=GetNumberFKVShiftSpeeds(&NrFKVerticalSpeeds);
		if (errorValue!=DRV_SUCCESS) {
			sprintf_s(aBuffer2,256,"GetNumberFKVShiftSpeeds error %i\n",errorValue);
			strcat_s(aBuffer,2048,aBuffer2);
			errorFlag=true;
		};
		HorizontalSpeed=new float[NrHorizontalSpeeds];
		VerticalSpeed=new int[NrVerticalSpeeds];
		FKSVerticalSpeed=new int[NrFKVerticalSpeeds];
		for (int i=0;i<NrHorizontalSpeeds;i++) {
			errorValue=GetHSSpeed(0,0,i,&HorizontalSpeed[i]);  
			if (errorValue!=DRV_SUCCESS) {
				sprintf_s(aBuffer2,256,"GetNumberFKVShiftSpeeds error %i\n",errorValue);
				strcat_s(aBuffer,2048,aBuffer2);
				errorFlag=true;
			};
		}
		//hspeed result=1,2,16,32
		for (int i=0;i<NrVerticalSpeeds;i++) errorFlag|=GetVerticalSpeed(i,&VerticalSpeed[i])!=DRV_SUCCESS;  
		//vspeed result=16
		for (int i=0;i<NrFKVerticalSpeeds;i++) errorFlag|=GetFKVShiftSpeed(i,&FKSVerticalSpeed[i])!=DRV_SUCCESS; 
		//FKSvspeed result=1,2,4,16
		SetHSSpeed(1,NrHorizontalSpeeds-1);  //Slowest (a verifier!)
		errorFlag|=SetVerticalSpeed(NrVerticalSpeeds-1)!=DRV_SUCCESS; //Slowest (a verifier!)
		errorFlag|=GetNumberADChannels(&NrADChannels)!=DRV_SUCCESS;
		errorFlag|=GetNumberAmp(&NrAmp)!=DRV_SUCCESS; //result=1
		errorFlag|=GetNumberPreAmpGains(&NrPreAmpGain)!=DRV_SUCCESS; //result=1
		errorFlag|=GetPixelSize(&PixelSizeX,&PixelSizeY)!=DRV_SUCCESS; //result=13.0 , 13.0	
		errorValue=SetCoolerMode(1); //temperature maintained on shut down
		if (errorValue!=DRV_SUCCESS) {
			sprintf_s(aBuffer2,256,"SetCoolerMode error %i\n",errorValue);
			strcat_s(aBuffer,2048,aBuffer2);
			errorFlag=true;
		};
		//errorFlag|=SetTemperature(GoalTemperature)!=DRV_SUCCESS; 
		//errorFlag|=CoolerON()!=DRV_SUCCESS; //Switch cooler on
		CoolerOn=true;
	//	errorFlag|=GetTemperatureF(&LastTemperature)!=DRV_SUCCESS;
		/*errorValue=GetTemperatureF(&LastTemperature)!=DRV_SUCCESS;
		if (errorValue!=DRV_SUCCESS) {
			sprintf_s(aBuffer2,256,"GetTemperature error %i\n",errorValue);
			strcat_s(aBuffer,2048,aBuffer2);
			errorFlag=true;
		};*/
		TransferParameters();
	}  
	initialized=!errorFlag;	
	if (errorFlag) AfxMessageBox(aBuffer);
	return true;
}

CAndor::~CAndor()
{
	bool errorFlag=false;
	if (HorizontalSpeed) delete(HorizontalSpeed);
	HorizontalSpeed=NULL;
	if (VerticalSpeed) delete(VerticalSpeed);
	VerticalSpeed=NULL;
	if (FKSVerticalSpeed) delete(FKSVerticalSpeed);
	FKSVerticalSpeed=NULL;
	if (!initialized) return;
	DWORD start=GetTickCount();
	float temp;
	char buffer[1024];
	char buf2[1024];
	wsprintf(buffer,"Shutdown errors:\n");
	int errorValue=DRV_SUCCESS;
	errorValue=SetTemperature(15);
	if (errorValue!=DRV_SUCCESS) {
		errorFlag=true;
		sprintf_s(buf2,1024,"SetTemperature %i\n",errorValue);
		strcat_s(buffer,1024,buf2);
	}
	GetTemperatureF(&temp);
	/*errorValue=GetTemperatureF(&temp);
	if (!(errorValue==DRV_TEMP_OFF || errorValue==DRV_TEMP_STABILIZED || errorValue==DRV_TEMP_NOT_REACHED)) {
		errorFlag=true;
		sprintf_s(buf2,1024,"GetTemperature %i\n",errorValue);
		strcat_s(buffer,1024,buf2);
	}*/
	if (!errorFlag && temp<0) {
		AfxMessageBox("Waiting for Temperature to rise above 0 °C.");
		do ; while (temp<0 && GetTemperatureF(&temp)==DRV_TEMP_NOT_REACHED && abs((double)(GetTickCount()-start))<600000);
	}
	if (temp<0) {
		errorFlag=true;
		strcat_s(buffer,1024,"Temperature < 0 °C\n");
	}
	errorValue=CoolerOFF();
	if (errorValue!=DRV_SUCCESS) {
		errorFlag=true;
		sprintf_s(buf2,1024,"CoolerOff %i\n",errorValue);
		strcat_s(buffer,1024,buf2);
	}
	errorValue=ShutDown();
	if (errorValue!=DRV_SUCCESS) {
		errorFlag=true;
		sprintf_s(buf2,1024,"ShutDown %i\n",errorValue);
		strcat_s(buffer,1024,buf2);
	}
	if (errorFlag) AfxMessageBox(buffer);
}

BEGIN_MESSAGE_MAP(CAndor, CWnd)
	//{{AFX_MSG_MAP(CAndor)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAndor message handlers


void CAndor::SetParameters(int ahBin, int avBin, int axMin,int axMax,int ayMin,int ayMax,
		float aExposureTime,int aTemperature, bool aExternalTrigger,
		int avSpeed, int ahSpeed, 
		bool aFKSMode, int aFKSHeight, int aFKSImages,int aFKSDataImages,
		int aFKSReferenceImages, int aFKSvSpeed, bool aFKSExternalTriggerStart,int aFKSDirtyLines,
		unsigned int aFlushWait, int aNumberPictures, bool aPrePicture, bool aDoInternalTriggerAfterExternalTrigger, int aNumberOfAdditionalReferenceImages) {
	NumberOfAdditionalReferenceImages=aNumberOfAdditionalReferenceImages;
	DoInternalTriggerAfterExternalTrigger=aDoInternalTriggerAfterExternalTrigger;
	hBin=ahBin;
	vBin=avBin;
	xMax=axMax;
	xMin=axMin;
	yMin=ayMin;
	yMax=ayMax;	
	FlushWait=aFlushWait;
	GoalTemperature=aTemperature;
	ExternalTrigger=aExternalTrigger;
	ExposureTime=aExposureTime;
	vSpeed=avSpeed;
	hSpeed=ahSpeed;
	FKSMode=aFKSMode;
	FKSHeight=aFKSHeight;
	FKSImages=aFKSImages;
	FKSDataImages=aFKSDataImages;	
	FKSReferenceImages=aFKSReferenceImages;
	FKSvSpeed=aFKSvSpeed;
	FKSExternalTriggerStart=aFKSExternalTriggerStart;
	FKSDirtyLines=aFKSDirtyLines;
	FlushWait=aFlushWait;
	NumberPictures=aNumberPictures;
	PrePicture=aPrePicture;

	TransferParameters();
}

void CAndor::TransferParameters() {
	if (!initialized) return;
	if (xMax>Width) xMax=Width;
	if (yMax>Height) yMax=Height;
	char aBuffer[2048];
	char aBuffer2[256];
	int errorValue=DRV_SUCCESS;
	bool errorFlag;
	errorFlag=false;
	wsprintf(aBuffer,"TransferParameters Errors:\n");
	
//	errorFlag|=SetTemperature(GoalTemperature)!=DRV_SUCCESS;

//	GetTemperatureF(&LastTemperature);
	//errorFlag|=GetTemperatureF(&LastTemperature)!=DRV_SUCCESS;
	
	if (SetTriggerMode((ExternalTrigger) ? ((FKSMode && FKSExternalTriggerStart ) ? 6 : 1) : 0)!=DRV_SUCCESS) {      //0: internal , 1: external, 6: FKS external start
		strcat_s(aBuffer,2048,"SetTriggerMode\n");
		errorFlag=true;
	};



	//int i=0;
	//while ((HorizontalSpeed[i]!=hSpeed) && (i<NrHorizontalSpeeds)) i++;
	//if (HorizontalSpeed[i]!=hSpeed) errorValue=-1;
	//else errorValue=SetHSSpeed(0,i);
	//if (errorValue!=DRV_SUCCESS) {
	//	errorFlag=true;
	//	sprintf_s(aBuffer2,256,"SetHSSpeed %i\n",errorValue);
	//	strcat_s(aBuffer,2048,aBuffer2);
	//}




	if (FKSMode) {
		int vOffset=0; //added for FKS 24/04/2017

		yMin=1;
		xMin=1;
		xMax=1004;
		//yMax=1002;
		yMax=yMin+FKSHeight*(FKSImages-1)-1;
		if (FKSHeight == 501){
			vOffset = 501;
		}
		else {
			vOffset = 0;
		}
			
		//yMax=yMin+FKSHeight*FKSImages-1;
		//yMax=FKSHeight;		
//		if (vBin <= 501) vOffset = (FKSHeight-yMin);

		errorValue=SetAcquisitionMode(4); //fast kinetics
		if (errorValue!=DRV_SUCCESS){
			strcat_s(aBuffer,2048,"Set Acquisition Mode to Fast Kinetics Error\n");
			errorFlag=true;
		}

		errorValue=SetTriggerMode(6);
		if (errorValue!=DRV_SUCCESS) {      //0: internal , 1: external, 6: FKS external start
			strcat_s(aBuffer,2048,"SetTriggerMode\n");
			errorFlag=true;
	    };

		errorValue=SetFastExtTrigger(0); 
		if (errorValue!=DRV_SUCCESS){
			strcat_s(aBuffer,2048,"Set Fast external trigger error\n");
			errorFlag=true;
		}



		//errorValue=SetFastKinetics(FKSHeight,FKSImages,ExposureTime,4,1,1);  //commented out added for FKS 24/04/2017
		//if (vBin <= 501) vOffset=501; //added for FKS 24/04/2017
		errorValue=SetFastKineticsEx(FKSHeight,FKSImages,ExposureTime,4,hBin,vBin,vOffset); //added for FKS 24/04/2017
		//SetFastKineticsEx(int exposedRows, int seriesLength, float time, int mode, int hbin, int vbin, int offset); //added for FKS 24/04/2017
			sprintf_s(aBuffer2,256,"SFK %i %i %f %d %d %i\n",FKSHeight,FKSImages,ExposureTime,hBin,vBin,vOffset);
			strcat_s(aBuffer,2048,aBuffer2);
		if (errorValue!=DRV_SUCCESS) {
			errorFlag=true;
			sprintf_s(aBuffer2,256,"SetFastKineticsEx %i \n",errorValue);
			strcat_s(aBuffer,2048,aBuffer2);
		}

		//	CString buf;
		//buf.Format("Exp Time: %f",ExposureTime);
		//AfxMessageBox(buf);
	
		
		int i=0;
		while ((FKSVerticalSpeed[i]!=FKSvSpeed) && (i<NrFKVerticalSpeeds)) i++;

		if (FKSVerticalSpeed[i]!=FKSvSpeed) {
			errorValue=-1;
		} else {
			errorValue=SetFKVShiftSpeed(i);
		}




		if (errorValue!=DRV_SUCCESS) {
			errorFlag=true;
			sprintf_s(aBuffer2,256,"SetFKVShiftSpeed %i\n",errorValue);
			strcat_s(aBuffer,2048,aBuffer2);
		}


		//float speedTest;
		//errorFlag=GetFKVShiftSpeedF(i,&speedTest);
		//sprintf_s(aBuffer2,256,"GetFKVShiftSpeedF %i , %i, %f\n",errorValue,i,speedTest);
		//strcat_s(aBuffer,2048,aBuffer2);

		//int speedNbTest;
		//errorFlag=GetNumberFKVShiftSpeeds(&speedNbTest);
		//sprintf_s(aBuffer2,256,"GetNumberFKVShiftSpeeds %i , %i\n",errorValue,speedNbTest);
		//strcat_s(aBuffer,2048,aBuffer2);

	} else {	

		errorValue=SetAcquisitionMode(1); //single Scan
		if (errorValue!=DRV_SUCCESS){
			strcat_s(aBuffer,2048,"Set Acquisition Mode to single scan Error\n");
			errorFlag=true;
		}

		if (SetExposureTime(ExposureTime)!=DRV_SUCCESS) {       //in ms
			strcat_s(aBuffer,2048,"SetExposureTime\n");
			errorFlag=true;
		};

		float ActExpTime,AccTime,KinTime;
		if (GetAcquisitionTimings(&ActExpTime,&AccTime,&KinTime)!=DRV_SUCCESS) {
			strcat_s(aBuffer,2048,"GetAcquisitionTimings\n");
			//errorFlag=true;
		}

		if (ActExpTime!=ExposureTime) {
			sprintf_s(aBuffer2,256,"ExposureTime changed to %f\n",ActExpTime);
			strcat_s(aBuffer,2048,aBuffer2);
			//errorFlag=true;
		}

		/*int i=0;
		while ((VerticalSpeed[i]!=vSpeed) && (i<NrVerticalSpeeds)) i++;
		if (VerticalSpeed[i]!=vSpeed) errorValue=-1;
		else errorValue=SetVSSpeed(i);
		if (errorValue!=DRV_SUCCESS) {
			errorFlag=true;
			sprintf_s(aBuffer2,256,"SetVSSpeed %i\n",errorValue);
			strcat_s(aBuffer,2048,aBuffer2);
		}*/
	}
	//if (FKSMode) {
	//	errorValue=SetImage(hBin,vBin,xMin,xMax,yMin,FKSHeight);
	//} else {
	//	errorValue=SetImage(hBin,vBin,xMin,xMax,yMin,yMax);
	//}
	errorValue=SetImage(hBin,vBin,xMin,xMax,yMin,yMax);
	if (errorValue!=DRV_SUCCESS) {
		errorFlag=true;
		sprintf_s(aBuffer2,256,"SetImage2 %i\n",errorValue);
		strcat_s(aBuffer,2048,aBuffer2);
		sprintf_s(aBuffer2,256,"SI %i %i %i %i %i %i\n",hBin,vBin,xMin,xMax,yMin,yMax);
		strcat_s(aBuffer,2048,aBuffer2);
	}
	if (errorFlag) AfxMessageBox(aBuffer);		
}

bool CAndor::TakePicture() {	
	CString buf;
	unsigned long h=0;
	int i=0;
	unsigned short* Image=ImagePtr;
	unsigned short* FKSImage=FKSImagePtr;

	if ((!HardwareAccess) || (!initialized)) {
		//for(i=0;i<NumberPictures;i++) {						
		//	for (h=0;h<ImageSize;h++) {*Image=0; Image++;}
		//}
		return true;
	}
	int errorValue;
	bool errorFlag;
	errorFlag=false;
	int NumberPicturesTaken;
	int picimnum=0;


	int FKSDirtyLinesCut=ceil(((double)FKSDirtyLines)/(double)vBin);
	FKSDirtyLinesCut=FKSDirtyLinesCut*vBin;
	int Width=(xMax-xMin)+1;
	int MyWidth=ceil(((double)Width)/(double)hBin);
	int MyHeight=ceil(((double)FKSHeight-FKSDirtyLinesCut)/(double)vBin);
	int MyTotalHeight=FKSDataImages*MyHeight;

	int	FKSWidth=1004;
	int MyFKSHeight=(FKSHeight-FKSDirtyLinesCut);
//	unsigned short FKStempimage[1004*10004]; //added to fix FKS problems 25/04/2017
//	int FKSimagenumarray[128]; //added to fix FKS problems 25/04/2017
//	unsigned int FKSsubimage=0;
	int imagenum=0;
	unsigned long FKSSubImageSize=MyTotalHeight*MyWidth;
//	unsigned long FKSFullImageSize=1004*2004;
//	unsigned short* FKSImagePtr=&FKStempimage[0];

	unsigned long offsetpos=0;
	int Status=99;
	int x=0;
	int y=0;
	DWORD start;

	unsigned long Size=(xMax-xMin+1)*(yMax-yMin+1);
//	long outoffset=FKSHeight*Width;
//	long MyLength=MyHeight*MyWidth;

					//buf.Format("%i %i",NumberPictures,NumberOfAdditionalReferenceImages);
					//AfxMessageBox(buf);
	if (PrePicture) NumberPicturesTaken=NumberPictures+1+NumberOfAdditionalReferenceImages;	else NumberPicturesTaken=NumberPictures+NumberOfAdditionalReferenceImages;
	for(i=0;i<NumberPicturesTaken;i++) {						
		start=GetTickCount();
		while ((Status!=DRV_IDLE)  && (abs((double)(GetTickCount()-start))<5000) && (!errorFlag)) {
			errorFlag|=GetStatus(&Status)!=DRV_SUCCESS;
		}
		if ((Status!=DRV_IDLE) || errorFlag) {
			buf.Format("CAndor::TakePicture : camera getting ready, status %i",Status);
			AfxMessageBox(buf);
			return false;	
		}
		errorValue=StartAcquisition();
		if (errorValue!=DRV_SUCCESS) {
			buf.Format("CAndor::StartAcquisition : error %i",errorValue);
			AfxMessageBox(buf);
			return false;
		}
	/*	errorValue=WaitForAcquisition();
		if (errorValue!=DRV_SUCCESS) {
			CString buf;
			buf.Format("CAndor::WaitForAcquisition : error %i",errorValue);
			AfxMessageBox(buf);
			return false;
		}*/
		start=GetTickCount();
		Status=99;
		while ((Status!=DRV_IDLE)  && (abs((double)(GetTickCount()-start))<2000000) && (!errorFlag)) {
			errorFlag|=GetStatus(&Status)!=DRV_SUCCESS;
		}
		if ((Status!=DRV_IDLE) || errorFlag) {
			errorFlag|=AbortAcquisition()!=DRV_SUCCESS;
			buf.Format("CAndor::TakePicture : camera not ready, status %i",Status);
			AfxMessageBox(buf);
			return false;	
		}

		/*errorValue=GetStatus(&Status);
		if ((errorValue!=DRV_SUCCESS) || (Status!=DRV_IDLE)) {
			CString buf;
			buf.Format("CAndor::GetStatus : error %i, Status %i",errorValue, Status);
			AfxMessageBox(buf);
			return false;
		}*/

		//Size=(xMax-xMin+1)*(yMax-yMin+1);
		//if (!FKSMode) Size=(xMax-xMin+1)*(yMax-yMin+1);
		//else Size=MyWidth*MyHeight;
		//long Size=(xMax-xMin+1)*(yMax-yMin+1)/(vBin*hBin);

		if (!FKSMode) {
			Image=ImagePtr+ImageSize*i;
			if (PrePicture) {
				if (i==0) {
					errorValue=GetAcquiredData16(Image,Size);
				} else {
					errorValue=GetAcquiredData16(Image,Size);
				}
			} else {
				errorValue=GetAcquiredData16(Image,Size);
			}
			if (errorValue!=DRV_SUCCESS) {
				buf.Format("CAndor::GetAcquiredData16 : error %i",errorValue);
				AfxMessageBox(buf);
				return false;
			}
		} else {
//			if (PrePicture) {FKSimagenumarray[FKSsubimage]=0;} else {FKSimagenumarray[FKSsubimage]=1;}
//			FKSsubimage++;
//			if ((i==(NumberPicturesTaken-1))||(FKSsubimage==FKSImages)) {
//				FKSImagePtr=&FKStempimage[0];
				//errorValue=GetAcquiredData16(Image,Size);
				//FKSImage=FKSImagePtr;
//				Image=ImagePtr+ImageSize*i;
//				FKSImage=FKSImagePtr;
				
			
				FKSImage=FKSImagePtr;
//				FKSImage=FKSImagePtr+FKSImageSize*i*2;
				errorValue=GetAcquiredData16(FKSImage,FKSWidth*FKSHeight*2);

	//			// non-rotated edition
	//			for (int imnum=0;imnum<FKSImages;imnum++) {
	//				FKSImage=FKSImagePtr+imnum*FKSWidth*FKSHeight;
	////				FKSImage=FKSImagePtr+FKSImageSize*i*2;1.820
	//				for (int x=0;x<FKSHeight;x++) {
	//					for (int y=0;y<FKSWidth;y++) {
	//						Image=ImagePtr+i*ImageSize+x+y*FKSHeight;
	//						*Image=*FKSImage;
	//						FKSImage++;
	//					}
	//				}
	//				i++;
	//			}
	//			i--;


				//rotated version
				for (int imnum=0;imnum<FKSImages;imnum++) {
					FKSImage=FKSImagePtr+imnum*FKSWidth*FKSHeight;
	//				FKSImage=FKSImagePtr+FKSImageSize*i*2;1.820
					for (int x=0;x<FKSWidth;x++) {
						for (int y=0;y<FKSWidth;y++) {
							Image=ImagePtr+i*ImageSize+x*FKSHeight+y;
							if (y<FKSHeight) {
								*Image=*FKSImage;
								FKSImage++;
							} else {
								FKSImage++;
							}
						}
						if (x>=(FKSHeight-1-FKSDIRTYLINESTOCUT)) FKSImage=FKSImage-FKSWidth;
					}
					i++;
				}
				i--;

	//			//rotated version
	//			long sumval;
	//			short meanval;
	//			for (int imnum=0;imnum<FKSImages;imnum++) {
	//				FKSImage=FKSImagePtr+imnum*FKSWidth*FKSHeight;
	////				FKSImage=FKSImagePtr+FKSImageSize*i*2;1.820
	//				for (int x=0;x<FKSWidth;x++) {
	//					for (int y=0;y<FKSWidth;y++) {
	//						Image=ImagePtr+i*ImageSize+x*FKSHeight+y;
	//						if ((y<FKSHeight) && (x<FKSHeight)) {
	//							*Image=*FKSImage;
	//							if (x==(FKSHeight-1)) sumval=sumval+*FKSImage;
	//							FKSImage++;
	//						} else if (x<FKSHeight) {
	//							FKSImage++;
	//						} else {
	//							*Image=meanval;
	//						}
	//					}
	//					if (x==(FKSHeight-1)) meanval=ceil(sumval/(double)FKSHeight);
	//				}
	//				i++;
	//			}
	//			i--;





				//for (h=0;h<1004*FKSHeight;h++) {
				//	*Image=*FKSImage;
				//	FKSImage++;
				//	Image++;
				//}
				
//				errorValue=GetAcquiredData16(Image,1004*FKSHeight);
//				errorValue=GetAcquiredData16(FKSImage,1004*2004);
			//	errorValue=GetAcquiredData16(FKSImagePtr,1004*FKSHeight);
//				if (errorValue!=DRV_SUCCESS) {
////					buf.Format("CAndor::GetAcquiredData16 : error %i",errorValue);
//					buf.Format(" %i %i %i",i,NumberPicturesTaken,FKSImages);
//					AfxMessageBox(buf);
//					return false;
//				}
//				for (picimnum=0;picimnum<FKSsubimage;picimnum++) {
				//buf.Format("fksval %i %i",*(FKSImage+1004*500+500),*(FKSImage+1004*1500+500));
				//AfxMessageBox(buf);
//				for (picimnum=0;picimnum<FKSImages;picimnum++) {
					 //assumes vbin=1
//					if (FKSimagenumarray[picimnum]==1) {
						//We need to rotate for our camera orientation
//						offsetpos=ceil(((double)Width*FKSHeight*picimnum)/(double)(vBin*hBin));

//						for (int j=0;j<FKSDataImages;j++) {

						// rotated edition
						//for (int x=0;x<MyHeight;x++) {
						//	for (int y=0;y<MyWidth;y++) {
						//		Image[i][x+y*MyTotalHeight+picimnum*MyHeight]=FKStempimage[offsetpos];
						//		offsetpos++;
						//	}
						//}

						// non-rotated edition
	//					imagenum=i+picimnum;
	//					if (PrePicture) imagenum=imagenum-1;
	//					if (imagenum>=0) {
	//						Image=ImagePtr+imagenum*ImageSize;
	//						FKSImage=FKSImagePtr+1004*FKSHeight*picimnum;
	////						FKSImage=FKSImagePtr+Width*FKSHeight*picimnum;
	////						FKSImage=FKSImagePtr+offsetpos;
	//						for (h=0;h<FKSSubImageSize;h++) {*Image=*FKSImage; FKSImage++; Image++;}
	//					}
//					}
//				}
				//i=i+FKSImages-1;
				//FKSsubimage=0;
			//}
		}

		if (DoInternalTriggerAfterExternalTrigger) {
			//TestDebug
			if (SetTriggerMode(0)!=DRV_SUCCESS) {      //0: internal , 1: external, 6: FKS external start
				
			};

			start=GetTickCount();
			Status=99;
			while ((Status!=DRV_IDLE)  && (abs((double)(GetTickCount()-start))<5000) && (!errorFlag)) {
				errorFlag|=GetStatus(&Status)!=DRV_SUCCESS;
			}
			if ((Status!=DRV_IDLE) || errorFlag) {
				buf.Format("CAndor::TakePicture : camera getting ready, status %i",Status);
				AfxMessageBox(buf);
				return false;	
			}
			errorValue=StartAcquisition();
			if (errorValue!=DRV_SUCCESS) {
				buf.Format("CAndor::StartAcquisition : error %i",errorValue);
				AfxMessageBox(buf);
				return false;
			}
			start=GetTickCount();
			Status=99;
			while ((Status!=DRV_IDLE)  && (abs((double)(GetTickCount()-start))<2000000) && (!errorFlag)) {
				errorFlag|=GetStatus(&Status)!=DRV_SUCCESS;
			}
			if ((Status!=DRV_IDLE) || errorFlag) {
				errorFlag|=AbortAcquisition()!=DRV_SUCCESS;
				buf.Format("CAndor::TakePicture : camera not ready, status %i",Status);
				AfxMessageBox(buf);
				return false;	
			}

			//End TestDebug
		}
		if ((i<(NumberPicturesTaken-1)) && (FlushWait>0)) {
			DWORD Start=GetTickCount();
			while ((unsigned long)abs((double)(GetTickCount()-Start))<FlushWait);
		}
		if (DoInternalTriggerAfterExternalTrigger) {
			//TestDebug
			if (SetTriggerMode((ExternalTrigger) ? ((FKSMode && FKSExternalTriggerStart ) ? 6 : 1) : 0)!=DRV_SUCCESS) {      //0: internal , 1: external, 6: FKS external start
			
			};
		}
	}

	//TestImage
	/*int width=xMax-xMin;
	for(long i=0;i<NrImages;i++) {		
		for (unsigned short x=xMin;x<xMax;x++)
			for (unsigned short y=yMin;y<yMax;y++)
				Image[i][x+y*width]=(unsigned short)(128*(x+y));
	}*/
	return true;
}

void CAndor::GetParameters(long &axMin,long &axMax,long &ayMin,long &ayMax,long &ahBin, long &avBin,int &aNrImages,
						   bool &aPrePicture,bool &aFKSMode,
							long &aFKSHeight,long &aFKSDirtyLines,int &aFKSImages,int &aFKSDataImages,int &aFKSReferenceImages) {
	axMin=xMin;
	axMax=xMax;
	ayMin=yMin;
	ayMax=yMax;
	ahBin=hBin;
	avBin=vBin;
	aNrImages=NumberPictures+NumberOfAdditionalReferenceImages;
	aPrePicture=PrePicture;
	aFKSMode=FKSMode,
	aFKSHeight=FKSHeight;
	aFKSDirtyLines=FKSDirtyLines;
	aFKSImages=FKSImages;
	aFKSDataImages=FKSDataImages;
	aFKSReferenceImages=FKSReferenceImages;
}

void CAndor::TestTriggerLine()
{
	for (int j=0;j<5;j++) {
		DWORD Start=GetTickCount();
		for( int i=1;i<5;i++) OutAuxPort(i,1);
		while (abs((double)(GetTickCount()-Start))<1000) ;
		Start=GetTickCount();
		for( int i=1;i<5;i++) OutAuxPort(i,0);
		while (abs((double)(GetTickCount()-Start))<1000) ;
	}
}

double CAndor::GetCCDTemperature() {

	GetTemperatureF(&LastTemperature);
	return LastTemperature;
}

CString CAndor::GetStatusString()
{	
	CString Status;
	Status.Format("not initialized");
	if (!initialized) return Status;	
	char aBuffer[2048];
	char aBuffer2[256];
	bool errorFlag;
	errorFlag=false;
	long errorValue;
	wsprintf(aBuffer,"GetStatusString Errors:\n");

	GetTemperatureF(&LastTemperature);
	/*errorValue=GetTemperatureF(&LastTemperature)!=DRV_SUCCESS;
	if (errorValue!=DRV_SUCCESS) {
		sprintf_s(aBuffer2,256,"GetTemperature error %i\n",errorValue);
		strcat_s(aBuffer,2048,aBuffer2);
		errorFlag=true;
	};*/

	float ActExpTime,AccTime,KinTime;
	errorValue=GetAcquisitionTimings(&ActExpTime,&AccTime,&KinTime);
	if (errorValue!=DRV_SUCCESS) {
		sprintf_s(aBuffer2,256,"GetAcquisitionTimings %i\n",errorValue);
		strcat_s(aBuffer,2048,aBuffer2);
		//errorFlag=true;
	}		
	CString TriggerStatus;
	if (FKSMode && ExternalTrigger) {		
		if (FKSExternalTriggerStart) TriggerStatus="External start"; else TriggerStatus="External";
	} else {
		if (ExternalTrigger) TriggerStatus="External"; else TriggerStatus="Internal";
	}
	int In0,In1,In2,In3;
	errorValue=InAuxPort(1,&In0);
	if (errorValue!=DRV_SUCCESS) {
		sprintf_s(aBuffer2,256,"InAuxPort1 %i\n",errorValue);
		strcat_s(aBuffer,2048,aBuffer2);
		errorFlag=true;
	}
	errorValue=InAuxPort(2,&In1);
	if (errorValue!=DRV_SUCCESS) {
		sprintf_s(aBuffer2,256,"InAuxPort2 %i\n",errorValue);
		strcat_s(aBuffer,2048,aBuffer2);
		errorFlag=true;
	}
	errorValue=InAuxPort(3,&In2);
	if (errorValue!=DRV_SUCCESS) {
		sprintf_s(aBuffer2,256,"InAuxPort3 %i\n",errorValue);
		strcat_s(aBuffer,2048,aBuffer2);
		errorFlag=true;
	}
	errorValue=InAuxPort(4,&In3);
	if (errorValue!=DRV_SUCCESS) {
		sprintf_s(aBuffer2,256,"InAuxPort4 %i\n",errorValue);
		strcat_s(aBuffer,2048,aBuffer2);
		errorFlag=true;
	}
	Status.Format("Set Temperature=%i °C\nTemperature=%.2f °C\nxMin=%i xMax=%i xBin=%i SizeX=%i\nyMin=%i yMax=%i yBin=%i SizeY=%i\nExposure time: Goal=%.3f Normal=%.3f FKS=%.3f\nSpeeds: horiz=%i vertNorm=%i vertFKS=%i\nTrigger: %s\nMode: %s\nFKSHeight=%i FKSImages=%i\nFKSDataImages=%i\nFKSReferenceImages=%i\nFlush wait=%i\nNumber Pictures=%i\nPrePicture=%i\nInPort=%i %i %i %i", 
		GoalTemperature,
		LastTemperature,
		xMin,xMax,hBin,Width,
		yMin,yMax,vBin,Height,
		ExposureTime,ActExpTime,KinTime,
		hSpeed,vSpeed,FKSvSpeed,
		TriggerStatus,
		(FKSMode) ? "Fast Kinetics" : "Normal",
		FKSHeight,
		FKSImages,
		FKSDataImages,
		FKSReferenceImages,
		FlushWait,
		NumberPictures,
		(PrePicture==0) ? 0 : 1,
		(In0==0) ? 0 : 1,
		(In1==0) ? 0 : 1,
		(In2==0) ? 0 : 1,
		(In3==0) ? 0 : 1		
		);
	if (errorFlag) AfxMessageBox(aBuffer);
	return Status;
}
