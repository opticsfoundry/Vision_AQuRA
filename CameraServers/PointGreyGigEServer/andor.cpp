// Andor.cpp : implementation file
//

#include "stdafx.h"
#include "Andor.h"
#include <afx.h>
#include <stdlib.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAndor

using namespace FlyCapture2;

CAndor::CAndor(bool aHardwareAccess)
{
	CameraSerialNumber=0;
	CameraNumber=0;
	MyIPPort=0;
	Debug=false;
	Use16Bit=false;
	NumberOfAdditionalReferenceImages=0;
	HardwareAccess=aHardwareAccess;
	hBin=1;
	vBin=1;
	xMax=1920;
	xMin=1;
	yMin=1;
	yMax=1200;
	CameraGain=10;
	LastTemperature=99;
	FlushWait=0;
	ExternalTrigger=false;
	MinTemp=99;
	MaxTemp=99;
	CoolerOn=false;
	StartTime=0;
	StartAcquisitionTime=0;
	initialized=false;
	FKSExternalTriggerStart=false;
	FKSDirtyLines=0;
	CameraBrightness=1;
	vSpeed=16;
	ReadoutDelay=2;
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
	Width=xMax-xMin+1;
	Height=yMax-yMin+1;
}

bool CAndor::InitCamera(char* directory,unsigned int InputCameraSerialNumber,int InputCameraNumber,int InputMyIPPort)
{	
	if (!HardwareAccess) return true;

	CameraSerialNumber=InputCameraSerialNumber;
	CameraNumber=InputCameraNumber;
	MyIPPort=InputMyIPPort;

	char aBuffer[2048];
	bool errorFlag;
	errorFlag=false;
	initialized=false;
	aBuffer[0]=0;
	strcat_s(aBuffer,2048,"Initialize Error\n");

	
   // Utilities::GetLibraryVersion( &fc2Version );
	//ostringstream version;
	/*version << "FlyCapture2 library version: " << fc2Version.major << "." << fc2Version.minor << "." << fc2Version.type << "." << fc2Version.build;
	cout << version.str() <<endl;
	
	cout << endl;
	cout << "*** CAMERA INFORMATION ***" << endl;
	cout << "Serial number -" << pCamInfo->serialNumber << endl;
    cout << "Camera model - " << pCamInfo->modelName << endl;
    cout << "Camera vendor - " << pCamInfo->vendorName << endl;
    cout << "Sensor - " << pCamInfo->sensorInfo << endl;
    cout << "Resolution - " << pCamInfo->sensorResolution << endl;
    cout << "Firmware version - " << pCamInfo->firmwareVersion << endl;
    cout << "Firmware build time - " << pCamInfo->firmwareBuildTime << endl << endl;*/
	
	

	Error error;

    BusManager busMgr;
    unsigned int numCameras;
    error = busMgr.GetNumOfCameras(&numCameras);
    if (error != PGRERROR_OK)
    {
        strcat_s(aBuffer,2048,error.GetDescription());
       errorFlag=TRUE;
    }

    //cout << "Number of cameras detected: " << numCameras <<endl; 

    if ( numCameras == 0 )
    {
       strcat_s(aBuffer,2048,"Insufficient number of cameras\n");
       errorFlag=TRUE;
    }

	PGRGuid guid;
	error = busMgr.GetCameraFromSerialNumber(CameraSerialNumber,&guid);
   // error = busMgr.GetCameraFromIndex(CameraNumber, &guid);
    if (error != PGRERROR_OK)
    {
        strcat_s(aBuffer,2048,error.GetDescription());
       errorFlag=TRUE;
    }



    // Connect to a camera
    error = cam.Connect(&guid);
    if (error != PGRERROR_OK)
    {
       strcat_s(aBuffer,2048,error.GetDescription());
       errorFlag=TRUE;
    }

	// Power on the camera
	const unsigned int k_cameraPower = 0x610;
	const unsigned int k_powerVal = 0x80000000;
	error  = cam.WriteRegister( k_cameraPower, k_powerVal );
	if (error != PGRERROR_OK)
	{
		 strcat_s(aBuffer,2048,error.GetDescription());
       errorFlag=TRUE;
	}

	const unsigned int millisecondsToSleep = 100;
	unsigned int regVal = 0;
	unsigned int retries = 10;

	// Wait for camera to complete power-up
	do 
	{
#if defined(WIN32) || defined(WIN64)
		Sleep(millisecondsToSleep);    
#else
		usleep(millisecondsToSleep * 1000);
#endif
		error = cam.ReadRegister(k_cameraPower, &regVal);
		if (error == PGRERROR_TIMEOUT)
		{
			// ignore timeout errors, camera may not be responding to
			// register reads during power-up
		}
		else if (error != PGRERROR_OK)
		{
			 strcat_s(aBuffer,2048,error.GetDescription());
			errorFlag=TRUE;
		}

		retries--;
	} while ((regVal & k_powerVal) == 0 && retries > 0);

	// Check for timeout errors after retrying
	if (error == PGRERROR_TIMEOUT)
	{
		 strcat_s(aBuffer,2048,error.GetDescription());
		 errorFlag=TRUE;
	}



    // Get the camera information

   /* error = cam.GetCameraInfo(&camInfo);
    if (error != PGRERROR_OK)
    {
        strcat_s(aBuffer,2048,error.GetDescription());
		 errorFlag=TRUE;
    }

	
	

    cout << endl;
	cout << "*** CAMERA INFORMATION ***" << endl;
	cout << "Serial number -" << pCamInfo->serialNumber << endl;
    cout << "Camera model - " << pCamInfo->modelName << endl;
    cout << "Camera vendor - " << pCamInfo->vendorName << endl;
    cout << "Sensor - " << pCamInfo->sensorInfo << endl;
    cout << "Resolution - " << pCamInfo->sensorResolution << endl;
    cout << "Firmware version - " << pCamInfo->firmwareVersion << endl;
    cout << "Firmware build time - " << pCamInfo->firmwareBuildTime << endl << endl;*/

		TransferParameters();
	initialized=true;	
	if (errorFlag) AfxMessageBox(aBuffer);
	return true;
}

CAndor::~CAndor()
{
	ShutDown();
}

void CAndor::ShutDown() {
	bool errorFlag=false;
	if (!initialized) return;
	DWORD start=(long)(GetTickCount());
	char aBuffer[2048];
	aBuffer[0]=0;
	Error error;

	wsprintf(aBuffer,"Shutdown errors:\n");
	// Turn trigger mode off.
    triggerMode.onOff = false;    
    error = cam.SetTriggerMode( &triggerMode );
    if (error != PGRERROR_OK)
    {
        strcat_s(aBuffer,2048,error.GetDescription());
		errorFlag=TRUE;
    }

    // Stop capturing images
  /*  error = cam.StopCapture();
    if (error != PGRERROR_OK)
    {
        strcat_s(aBuffer,2048,error.GetDescription());
		errorFlag=TRUE;
    }     */ 

    // Disconnect the camera
    error = cam.Disconnect();
    if (error != PGRERROR_OK)
    {
        strcat_s(aBuffer,2048,error.GetDescription());
		errorFlag=TRUE;
    }
	
	if (errorFlag) AfxMessageBox(aBuffer);
}

BEGIN_MESSAGE_MAP(CAndor, CWnd)
	//{{AFX_MSG_MAP(CAndor)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAndor message handlers


void CAndor::SetParameters(int ahBin, int avBin, int axMin,int axMax,int ayMin,int ayMax,
		float aExposureTime,int aCameraGain, bool aExternalTrigger,
		int avSpeed, int aCameraBrightness, 
		bool aFKSMode, int aFKSHeight, int aFKSImages,int aFKSDataImages,
		int aFKSReferenceImages, int aReadoutDelay, bool aFKSExternalTriggerStart,int aFKSDirtyLines,
		unsigned int aFlushWait, int aNumberPictures, bool aPrePicture, bool aUse16Bit, int aNumberOfAdditionalReferenceImages) {
	NumberOfAdditionalReferenceImages=aNumberOfAdditionalReferenceImages;
	Use16Bit=aUse16Bit;
	hBin=ahBin;
	vBin=avBin;
	xMax=axMax;
	xMin=axMin;
	yMin=ayMin;
	yMax=ayMax;	
	//Width=xMax-xMin+1;  
	//Height=ayMax-ayMin+1;
	FlushWait=aFlushWait;
	CameraGain=aCameraGain;
	ExternalTrigger=aExternalTrigger;
	ExposureTime=aExposureTime;
	vSpeed=avSpeed;
	CameraBrightness=aCameraBrightness;
	FKSMode=aFKSMode;
	FKSHeight=aFKSHeight;
	FKSImages=aFKSImages;
	FKSDataImages=aFKSDataImages;	
	FKSReferenceImages=aFKSReferenceImages;
	ReadoutDelay=aReadoutDelay;
	FKSExternalTriggerStart=aFKSExternalTriggerStart;
	FKSDirtyLines=aFKSDirtyLines;
	FlushWait=aFlushWait;
	NumberPictures=aNumberPictures;
	PrePicture=aPrePicture;

	TransferParameters();
}

bool CAndor::PollForTriggerReady( GigECamera* pCam )
{
    const unsigned int k_softwareTrigger = 0x62C;
    Error error;
    unsigned int regVal = 0;
	DWORD start=(long)(GetTickCount());
    do 
    {
        error = pCam->ReadRegister( k_softwareTrigger, &regVal );
        if (error != PGRERROR_OK)
        {
			AfxMessageBox(error.GetDescription());	
			return false;
        }

    } while ( ( (regVal >> 31) != 0 ) & ( ((long)(GetTickCount())-start)<1000 ) );
	if (((long)(GetTickCount())-start)>=3000 ) AfxMessageBox("CAndor::PollForTriggerReady : timeout");	
	return true;
}

bool CAndor::CheckSoftwareTriggerPresence( GigECamera* pCam )
{
	const unsigned int k_triggerInq = 0x530;

	Error error;
	unsigned int regVal = 0;

	error = pCam->ReadRegister( k_triggerInq, &regVal );

	if (error != PGRERROR_OK)
	{
		AfxMessageBox(error.GetDescription());	
		return false;
	}

	if( ( regVal & 0x10000 ) != 0x10000 )
	{
		return false;
	}

	return true;
}

bool CAndor::FireSoftwareTrigger( GigECamera* pCam )
{
    const unsigned int k_softwareTrigger = 0x62C;
    const unsigned int k_fireVal = 0x80000000;
    Error error;    

    error = pCam->WriteRegister( k_softwareTrigger, k_fireVal );
    if (error != PGRERROR_OK)
    {
        AfxMessageBox(error.GetDescription());	
        return false;
    }

    return true;
}

void CAndor::TransferParameters() {
	if (!initialized) return;
	if (xMax>Width) xMax=Width;
	if (yMax>Height) yMax=Height;
	char aBuffer[2048];
	aBuffer[0]=0;
	char aReadbackBuffer[2048];
	
	Error error;
	bool errorFlag;
	bool errorOnReadback;
	bool success=false;
	int trialCounter=0;

	while ((!success) && (trialCounter<5)) {
		trialCounter++;
		errorOnReadback=false;
		errorFlag=false;
		aReadbackBuffer[0]=0;
	

		wsprintf(aBuffer,"TransferParameters Errors:\n");
		wsprintf(aReadbackBuffer,"TransferParameters readback errors:\n");

		if (!ExternalTrigger) {
			// Check for external trigger support
			TriggerModeInfo triggerModeInfo;
			error = cam.GetTriggerModeInfo( &triggerModeInfo );
			if (error != PGRERROR_OK)
			{
				 strcat_s(aBuffer,2048,error.GetDescription());
				 errorFlag=true;
			}

			if ( triggerModeInfo.present != true )
			{
				strcat_s(aBuffer,2048,"Camera does not support external trigger!");
				errorFlag=true;
			}
		}


		// Get image format

		// Query for available Format 7 modes
		Format7Info fmt7Info;
		bool supported;
		const Mode k_fmt7Mode = MODE_7;
		//const PixelFormat k_fmt7PixFmt = PIXEL_FORMAT_MONO8;


		fmt7Info.mode = k_fmt7Mode;
		//error = cam.GetFormat7Info( &fmt7Info, &supported );
		//if (error != PGRERROR_OK)
		//{
		//	PrintError( error );
		//	return -1;
		//}

		//PrintFormat7Capabilities( fmt7Info );

		//if ( (k_fmt7PixFmt & fmt7Info.pixelFormatBitField) == 0 )
		//{
		//	// Pixel format not supported!
		//	cout << "Pixel format is not supported" << endl;
		//	return -1;
		//}

		//Format7ImageSettings fmt7ImageSettings;
		fmt7ImageSettings.mode = k_fmt7Mode;
		//fmt7ImageSettings.offsetX = 0;
		//fmt7ImageSettings.offsetY = 0;
		//fmt7ImageSettings.width = fmt7Info.maxWidth;
		//fmt7ImageSettings.height = fmt7Info.maxHeight;
		//fmt7ImageSettings.pixelFormat = k_fmt7PixFmt;

		//bool valid;
		//Format7PacketInfo fmt7PacketInfo;

		//// Validate the settings to make sure that they are valid
		//error = cam.ValidateFormat7Settings(
		//		&fmt7ImageSettings,
		//		&valid,
		//		&fmt7PacketInfo );
		//if (error != PGRERROR_OK)
		//{
		//	PrintError( error );
		//	return -1;
		//}

		//if ( !valid )
		//{
		//	// Settings are not valid
		//	cout << "Format7 settings are not valid" << endl;
		//	return -1;
		//}

		//// Set the settings to the camera
		//error = cam.SetFormat7Configuration(
		//		&fmt7ImageSettings,
		//		fmt7PacketInfo.recommendedBytesPerPacket );
		//if (error != PGRERROR_OK)
		//{
		//	PrintError( error );
		//	return -1;
		//}



		// Get current trigger settings
		error = cam.GetTriggerMode( &triggerMode );
		if (error != PGRERROR_OK)
		{
			strcat_s(aBuffer,2048,error.GetDescription());
			 errorFlag=true;
		}

		 // Set camera to trigger mode 0
		triggerMode.onOff = true;
		triggerMode.mode = 0;  //global shutter
		triggerMode.parameter = 0;


		if (!ExternalTrigger) {
			// A source of 7 means software trigger
			triggerMode.source = 7;
		} else {
			// Triggering the camera externally using source 0.
			triggerMode.source = 0;
		}
    

		error = cam.SetTriggerMode( &triggerMode );
		if (error != PGRERROR_OK)
		{
		   strcat_s(aBuffer,2048,error.GetDescription());
			 errorFlag=true;
		}

		// Poll to ensure camera is ready
		bool retVal = PollForTriggerReady( &cam );
		if( !retVal )
		{
			strcat_s(aBuffer,2048,"Error polling for trigger ready!");
			errorFlag=true;
		}



		// Get the camera configuration
		FC2Config config;
		error = cam.GetConfiguration( &config );
		if (error != PGRERROR_OK)
		{
			strcat_s(aBuffer,2048,error.GetDescription());
			 errorFlag=true;
		} 
    
		// Set the grab timeout to 15 seconds
		config.grabTimeout = 15000;

		// Set the camera configuration
		error = cam.SetConfiguration( &config );
		if (error != PGRERROR_OK)
		{
			strcat_s(aBuffer,2048,error.GetDescription());
			 errorFlag=true;
		} 



		FlyCapture2::GigEConfig config2;
		  error = cam.GetGigEConfig(&config2);
		  if (error != FlyCapture2::PGRERROR_OK)
		  {
			strcat_s(aBuffer,2048,error.GetDescription());
			 errorFlag=true;
		  }

		  config2.enablePacketResend = true;	
	  
		  cam.SetGigEConfig(&config2);
		  if (error != FlyCapture2::PGRERROR_OK)
		  {
			strcat_s(aBuffer,2048,error.GetDescription());
			 errorFlag=true;
		  }

		GigEImageSettings ImageSettings;

		cam.GetGigEImageSettings(&ImageSettings);
		bool RangeError=false;
		ImageSettings.pixelFormat = (Use16Bit) ? PIXEL_FORMAT_RAW16 :  PIXEL_FORMAT_RAW8;// change the bit depth  modified by ChunChia


		if (((xMin-1)>=0) & ((xMin-1)<Width)) ImageSettings.offsetX=xMin-1; else RangeError=true;
		if (((yMin-1)>=0) & ((yMin-1)<Height)) ImageSettings.offsetY=yMin-1; else RangeError=true;
		if ((yMax>=0) & (yMax<=Height)) ImageSettings.height=yMax-ImageSettings.offsetY; else RangeError=true;
		if ((xMax>=0) & (xMax<=Width)) ImageSettings.width=xMax-ImageSettings.offsetX; else RangeError=true;

		if (RangeError) {
			 strcat_s(aBuffer,2048,"Image area out of range/n");
			 errorFlag=true;
		} else cam.SetGigEImageSettings(&ImageSettings);


		//Declare a Property struct.
		Property prop;
		//Define the property to adjust.
		prop.type = SHUTTER;
		//Ensure the property is on.
		prop.onOff = true;
		//Ensure auto-adjust mode is off.
		prop.autoManualMode = false;
		//Ensure the property is set up to use absolute value control.
		prop.absControl = true;
		//Set the absolute value of shutter to 20 ms.
		prop.absValue = ExposureTime;
		//Set the property.
		error = cam.SetProperty( &prop );
		if (error != FlyCapture2::PGRERROR_OK)
		{
		strcat_s(aBuffer,2048,error.GetDescription());
			errorFlag=true;
		}


		//Define the property to adjust.
		prop.type = GAIN;
		//Ensure auto-adjust mode is off.
		prop.autoManualMode = false;
		//Ensure the property is set up to use absolute value control.
		prop.absControl = true;
		//Set the absolute value of gain to 10.5 dB.
		prop.absValue = CameraGain;
		//Set the property.
		error = cam.SetProperty( &prop );
		if (error != FlyCapture2::PGRERROR_OK)
		{
		strcat_s(aBuffer,2048,error.GetDescription());
			errorFlag=true;
		}

		//Define the property to adjust.
		prop.type = BRIGHTNESS;
		//Ensure the property is set up to use absolute value control.
		prop.absControl = true;
		//Set the absolute value of brightness to 0.5%.
		prop.absValue = CameraBrightness;
		//Set the property.
		error = cam.SetProperty( &prop );
		if (error != FlyCapture2::PGRERROR_OK)
		{
		strcat_s(aBuffer,2048,error.GetDescription());
			errorFlag=true;
		}

		if (errorFlag) AfxMessageBox(aBuffer);	



		bool LocalErrorOnReadback;
		LocalErrorOnReadback=false;


		//readback trigger mode
		error = cam.GetTriggerMode( &triggerMode );
		if (error != PGRERROR_OK)
		{
			strcat_s(aBuffer,2048,error.GetDescription());
			 errorFlag=true;
		}
		LocalErrorOnReadback|= triggerMode.onOff != true;
		LocalErrorOnReadback|= triggerMode.mode != 0; 
		LocalErrorOnReadback|= triggerMode.parameter != 0;
		if (!ExternalTrigger) {
			// A source of 7 means software trigger
			LocalErrorOnReadback|= triggerMode.source != 7;
		} else {
			// Triggering the camera externally using source 0.
			LocalErrorOnReadback|= triggerMode.source != 0;
		}
		if (LocalErrorOnReadback) {
			strcat_s(aReadbackBuffer,2048,"Trigger\n");
			errorOnReadback=true;
		}

		LocalErrorOnReadback=false;
		//Define the property to adjust.
		prop.type = SHUTTER;
		//Get the property.
		error = cam.GetProperty( &prop );
		if (error != FlyCapture2::PGRERROR_OK)
		{
		strcat_s(aBuffer,2048,error.GetDescription());
			errorFlag=true;
		}
		LocalErrorOnReadback|=prop.onOff != true;
		//Ensure auto-adjust mode is off.
		LocalErrorOnReadback|=prop.autoManualMode != false;
		//Ensure the property is set up to use absolute value control.
		LocalErrorOnReadback|=prop.absControl != true;
		//Set the absolute value of shutter to 20 ms.
		LocalErrorOnReadback|=prop.absValue != ExposureTime; 
		if (LocalErrorOnReadback) {
			strcat_s(aReadbackBuffer,2048,"ExposureTime\n");
			errorOnReadback=true;
		}

		LocalErrorOnReadback=false;
		//Define the property to adjust.
		prop.type = GAIN;
		error = cam.GetProperty( &prop );
		if (error != FlyCapture2::PGRERROR_OK)
		{
		strcat_s(aBuffer,2048,error.GetDescription());
			errorFlag=true;
		}
		//Ensure auto-adjust mode is off.
		LocalErrorOnReadback|=prop.autoManualMode != false;
		//Ensure the property is set up to use absolute value control.
		LocalErrorOnReadback|=prop.absControl != true;
		//Set the absolute value of gain to 10.5 dB.
		LocalErrorOnReadback|=prop.absValue != CameraGain;
		if (LocalErrorOnReadback) {
			strcat_s(aReadbackBuffer,2048,"CameraGain\n");
			errorOnReadback=true;
		}


		LocalErrorOnReadback=false;
		//Define the property to adjust.
		prop.type = BRIGHTNESS;
		error = cam.GetProperty( &prop );
		if (error != FlyCapture2::PGRERROR_OK)
		{
		strcat_s(aBuffer,2048,error.GetDescription());
			errorFlag=true;
		}
		//Ensure the property is set up to use absolute value control.
		LocalErrorOnReadback|=prop.absControl != true;
		//Set the absolute value of brightness to 0.5%.
		LocalErrorOnReadback|=prop.absValue != CameraBrightness;
		//Set the property.
		if (LocalErrorOnReadback) {
			strcat_s(aReadbackBuffer,2048,"CameraBrightness\n");
			errorOnReadback=true;
		}

		success=!errorOnReadback;
	}
	//if (errorOnReadback) AfxMessageBox(aReadbackBuffer);	

}

bool CAndor::TakePicture(unsigned short* aImage[]) {	
	if ((!HardwareAccess) || (!initialized)) {
		long Size=(xMax-xMin+1)*(yMax-yMin+1);
		for(int i=0;i<NumberPictures;i++) {						
			unsigned short* p=aImage[i];
			for (long h=0;h<Size;h++) {*p=0;p++;}
		}
		return true;
	}
	bool errorFlag;
	char aBuffer[2048];
	aBuffer[0]=0;
	errorFlag=false;	
	int NumberPicturesTaken;
	Error error;
	Image image;

	strcat_s(aBuffer,2048,"TakePicture Errors\n");

	if (PrePicture) NumberPicturesTaken=NumberPictures+1+NumberOfAdditionalReferenceImages;
	else NumberPicturesTaken=NumberPictures+NumberOfAdditionalReferenceImages;


	 // Camera is ready, start capturing images
    error = cam.StartCapture();
    if (error != PGRERROR_OK)
    {
		strcat_s(aBuffer,2048,error.GetDescription());
		 errorFlag=TRUE;
    }   


	if (!ExternalTrigger) {
		if (!CheckSoftwareTriggerPresence( &cam ))
		{
			 strcat_s(aBuffer,2048,"SOFT_ASYNC_TRIGGER not implemented on this camera!");
			 errorFlag=TRUE;
		}
	} 

	for(int i=0;i<NumberPicturesTaken;i++) {						
		long start=(long)(GetTickCount());
		int Status=99;


		
if (!ExternalTrigger) {
		// Check that the trigger is ready
		PollForTriggerReady( &cam);

        // Fire software trigger
        bool retVal = FireSoftwareTrigger( &cam );
        if ( !retVal )
        {
			strcat_s(aBuffer,2048,"Error firing software trigger");
			 errorFlag=TRUE;       
		}
}


		//Sleep(ReadoutDelay);

        // Grab image        
		error = cam.RetrieveBuffer( &image );
        if (error != PGRERROR_OK)
        {
			strcat_s(aBuffer,2048,"Error 1: ");
             strcat_s(aBuffer,2048,error.GetDescription());
			// errorFlag=TRUE;
        }


		unsigned int aktCols=image.GetCols();
		unsigned int aktRows=image.GetRows();
		unsigned int aktBitsPerPixel=image.GetBitsPerPixel();
		
		
		if (Debug) {
			BMPOption MyBMPOption;	
			image.Save("c:\\SrCAL\\TestImage.bmp",&MyBMPOption);
		}
		if (((xMax-xMin+1)<=aktCols) && (xMin<=xMax) && ((yMax-yMin+1)<=aktRows) && (yMin<=yMax)) {
			if ((!PrePicture) || (i!=0)) {
				int PicCount=(PrePicture) ? i-1 : i;
				/*int width=xMax-xMin+1;
				for (unsigned short x=0;x<aktCols;x++) {
					for (unsigned short y=0;y<aktRows;y++)
						aImage[PicCount][x+y*width]=DataPtr[x+y*width];

				}*/

				unsigned short* helpptr=aImage[PicCount];
				
				if (aktBitsPerPixel==16) {
					unsigned short* DataPtr16Bit=(unsigned short*)(image.GetData());
					for (unsigned int n=0;n<aktCols*aktRows;n++) {
						/*helpptr=DataPtr;
						helpptr++;
						DataPtr++;*/

						helpptr[n]=DataPtr16Bit[n];
					}
				} else if (aktBitsPerPixel==8) {
					unsigned char* DataPtr8Bit=(unsigned char*)(image.GetData());
					for (unsigned int n=0;n<aktCols*aktRows;n++) helpptr[n]=DataPtr8Bit[n]<<8;
				}
				
				//aImage[PicCount]=(unsigned short*)(image.GetData());
			}
		} else {
			CString buffer;
			buffer.Format("Image area weird: aktCols=%u aktRows=%u\n",aktCols,aktRows);
			strcat_s(aBuffer,2048,buffer);
			 errorFlag=TRUE;
		}
	}

	 // Camera is ready, start capturing images
    error = cam.StopCapture();
    if (error != PGRERROR_OK)
    {
		strcat_s(aBuffer,2048,error.GetDescription());
		 errorFlag=TRUE;
    }   
	if (errorFlag) AfxMessageBox(aBuffer);
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
		long Start=(long)(GetTickCount());
		//for( int i=1;i<5;i++) OutAuxPort(i,1);
		while (abs(((long)(GetTickCount())-Start))<1000) ;
		Start=(long)(GetTickCount());
		//for( int i=1;i<5;i++) OutAuxPort(i,0);
		while (abs(((long)(GetTickCount())-Start))<1000) ;
	}
}

double CAndor::GetCCDTemperature() {

	//GetTemperatureF(&LastTemperature);
	return LastTemperature;
}

CString CAndor::GetStatusString()
{	
	CString Status;
	Status.Format("not initialized");
	if (!initialized) return Status;	
	char aBuffer[2048];
	aBuffer[0]=0;
	bool errorFlag;
	errorFlag=false;
	wsprintf(aBuffer,"GetStatusString Errors:\n");

	float ActExpTime=0;
	float AccTime=0;
	float KinTime=0;
	CString TriggerStatus;
	if (FKSMode && ExternalTrigger) {		
		if (FKSExternalTriggerStart) TriggerStatus="External start"; else TriggerStatus="External";
	} else {
		if (ExternalTrigger) TriggerStatus="External"; else TriggerStatus="Internal";
	}
	int In0=0;
	Status.Format("CameraSerialNumber=%i\nCameraNumber=%i\nIP Port=%i\nGain=%idB\nBrightness=%i    ReadoutDelay=%i\nxMin=%i xMax=%i xBin=%i ChipWidth=%i\nyMin=%i yMax=%i yBin=%i ChipHeight=%i\nExposure time: Goal=%.3f Normal=%.3f FKS=%.3f\nSpeeds:vertNorm=%i \nTrigger: %s\nMode: %s\nFKSHeight=%i FKSImages=%i\nFKSDataImages=%i\nFKSReferenceImages=%i\nFlush wait=%i\nNumber Pictures=%i\nPrePicture=%i\nInPort=%i", 
		CameraSerialNumber,
		CameraNumber,
		MyIPPort,
		CameraGain,
		CameraBrightness,
		ReadoutDelay,
		xMin,xMax,hBin,Width,
		yMin,yMax,vBin,Height,
		ExposureTime,ActExpTime,KinTime,
		vSpeed,
		TriggerStatus,
		(FKSMode) ? "Fast Kinetics" : "Normal",
		FKSHeight,
		FKSImages,
		FKSDataImages,
		FKSReferenceImages,
		FlushWait,
		NumberPictures,
		(PrePicture==0) ? 0 : 1,
		(In0==0) ? 0 : 1	
		);
	if (errorFlag) AfxMessageBox(aBuffer);
	return Status;
}
