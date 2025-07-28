// TCPIPServer.cpp: implementation of the CTCPIPServer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AndorServer.h"
#include "TCPIPServer.h"
#include "clntsock.h"
#include "lstnsock.h"
#include "math.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTCPIPServer::CTCPIPServer() 
:CNetworkServer() {		
	LastTime=0;				
	HardwareAccess=true;	
	AndorServerDlg=NULL;
	LastCommand="";	
	TakingImageStatus=0;
	for (int i=0;i<30;i++) {
		Image[i]=NULL;
		ImageSize[i]=0;
	}
	AktNrImages=0;
//	FKSRotatedImage1=NULL;
	//FKSRotatedImage2=NULL;
}

CTCPIPServer::~CTCPIPServer()
{
	
}

void CTCPIPServer::SendReady() {		
	CString buf;
	buf.Format("Ready");
	SendMsg(buf);	
}

void CTCPIPServer::SetParameters() {	
	long hBin;
	long vBin;
	long xMin;
	long xMax;
	long yMin;
	long yMax;
	double ExposureTime;
	long CameraGain;
	long ExternalTrigger;
	long vSpeed;
	long CameraBrightness;
	long FKSMode;
	long FKSHeight;
	long FKSImages;
	long FKSDataImages;
	long FKSReferenceImages;
	long ReadoutDelay;
	long FKSExternalTriggerStart;	
	long FKSDirtyLines;
	long NumberPictures;
	long PrePictureL;
	long FlushWait;
	long Use16Bit;
	long NumberOfAdditionalReferenceImages;

	bool ok;
	long error=0;
	ok=true;
	ok=ok && ReadInt(hBin);
	ok=ok && ReadInt(vBin);
	ok=ok && ReadInt(xMin);
	ok=ok && ReadInt(xMax);
	ok=ok && ReadInt(yMin);
	ok=ok && ReadInt(yMax);
	long ExposureTimems;
	ok=ok && ReadInt(ExposureTimems);  //in ms
	ExposureTime=((double)ExposureTimems);  
	ok=ok && ReadInt(CameraGain);
	ok=ok && ReadInt(ExternalTrigger);	
	ok=ok && ReadInt(vSpeed);
	ok=ok && ReadInt(CameraBrightness);
	ok=ok && ReadInt(FKSMode);
	ok=ok && ReadInt(FKSHeight);
	//ok=ok && ReadInt(FKSImages);
	ok=ok && ReadInt(FKSDataImages);
	ok=ok && ReadInt(FKSReferenceImages);
	FKSImages=FKSDataImages+FKSReferenceImages;
	ok=ok && ReadInt(ReadoutDelay);
	ok=ok && ReadInt(FKSExternalTriggerStart);
	ok=ok && ReadInt(FKSDirtyLines);	
	ok=ok && ReadInt(NumberPictures);
	ok=ok && ReadInt(PrePictureL);
	ok=ok && ReadInt(FlushWait);	
	ok=ok && ReadInt(Use16Bit);
	ok=ok && ReadInt(NumberOfAdditionalReferenceImages);
	if (xMax>Andor->Width) {xMax=Andor->Width;error=1;}
	if (yMax>Andor->Height) {
		yMax=Andor->Height;
		error=2;
	}
	if (xMin<0) {xMin=0;error=3;}
	if (yMin<0) {yMin=0;error=4;}
	if (hBin<=0) {hBin=1;error=5;}
	if (vBin<=0) {vBin=1;error=6;}
	if (hBin>Andor->Width) {hBin=Andor->Width;error=7;}
	if (vBin>Andor->Height) {vBin=Andor->Height;error=8;}
	if (NumberPictures<0) {NumberPictures=1;error=9;}
	if (NumberPictures>30) {NumberPictures=30;error=10;}
	if (error!=0) {
		CString buf;
		buf.Format("CTCPIPServer::SetParameters : Error %i",error);
		AfxMessageBox(buf);
	}
	
	Andor->SetParameters(hBin, vBin,xMin+1,xMax+1,yMin+1,yMax+1,
		ExposureTime,CameraGain, (ExternalTrigger==1),
		vSpeed,CameraBrightness, 
		(FKSMode==1),FKSHeight, FKSImages, FKSDataImages, FKSReferenceImages, ReadoutDelay, (FKSExternalTriggerStart==1),FKSDirtyLines,
		FlushWait, NumberPictures,PrePictureL==1,Use16Bit==1,NumberOfAdditionalReferenceImages);
}
	
void CTCPIPServer::TakeImage() {

	//TakingImageStatus:
	//	0:Idle
	//	1:Recording
	//	2:Waiting
	//	3:Transfer
	
	TakingImageStatus=1;
	UpdateStatus();
	long hBin;
	long vBin;
	long xMin;
	long xMax;
	long yMin;
	long yMax;
	int NrImages;
	bool FKSMode;
	long FKSHeight;
	long FKSDirtyLines;
	int FKSImages;
	int FKSDataImages;
	int FKSReferenceImages;
	bool PrePicture;
	Andor->GetParameters(xMin,xMax,yMin,yMax,hBin,vBin,NrImages,PrePicture,FKSMode,FKSHeight,FKSDirtyLines,FKSImages,FKSDataImages,FKSReferenceImages);	
	bool ok;
	int error=0;
	ok=true;	
	long size=(xMax-xMin+1)*(yMax-yMin+1)/(hBin*vBin);	
	if (NrImages!=AktNrImages) {
		for (int i=0;i<AktNrImages;i++) {
			delete[] Image[i];
			Image[i]=NULL;
		}
		AktNrImages=0;
	}
	for (int i=0;i<NrImages;i++) {
		if ((Image[i]) && (ImageSize[i]!=size)) {
			delete Image[i];
			ImageSize[i]=0;
		}
		if (ImageSize[i]!=size) {
			Image[i]=new unsigned short[size];
			ImageSize[i]=size;
		}
		//for (long j=0;j<size;j++) Image[i][j]=0;
	}
	AktNrImages=NrImages;
	if (ok) ok=Andor->TakePicture(Image); else error=1;
	//just for debug:
	//DWORD Start=(long)(GetTickCount());
	//while (abs((long)(GetTickCount())-Start)<2000);

	TakingImageStatus=2;
	UpdateStatus();	
	SendReady();
//	PrepareTransferImage();
}

void CTCPIPServer::TransferImage() {
	long hBin;
	long vBin;
	long xMin;
	long xMax;
	long yMin;
	long yMax;
	int NrImages;
	bool FKSMode;
	long FKSHeight;
	long FKSDirtyLines;
	int FKSImages;
	int FKSDataImages;
	int FKSReferenceImages;
	bool PrePicture;
	Andor->GetParameters(xMin,xMax,yMin,yMax,hBin,vBin,NrImages,PrePicture,FKSMode,FKSHeight,FKSDirtyLines,FKSImages,FKSDataImages,FKSReferenceImages);
	bool ok;
	int error=0;
	ok=true;	
	long size=(xMax-xMin+1)*(yMax-yMin+1)/(hBin*vBin);
	/*for (int i=0;i<(NrImages);i++) {
		for (long j=0;j<size;j++) Image[i][j]=Image[i][j] << 2;
	}*/
	if (!FKSMode) {
		for (int i=0;i<(NrImages);i++) {
			if (ok) ok=CheckReady(); else error=2;
			if (ok) {		
				//no rotation:
				TakingImageStatus=3;
				UpdateStatus();	
				WriteInteger(2*size);			
				SendData((unsigned char*)Image[i],size*2);			
			/*
			unsigned short *RotatedImage;
			RotatedImage=new unsigned short[size];
			int MyWidth=((xMax-xMin+1)/hBin);
			int MyHeight=((yMax-yMin+1)/vBin);
			if (MyWidth!=MyHeight) {
				//sorry, can't rotate
				WriteInteger(2*size);			
				SendData((unsigned char*)Image[i],size*2);			
			} else {
				//We need to rotate for our camera orientation
				for (int x=0;x<MyWidth;x++) {
					for (int y=0;y<MyHeight;y++)
						RotatedImage[x+y*MyWidth]=Image[i][y+x*MyWidth];
				}
				WriteInteger(2*size);			
				SendData((unsigned char*)RotatedImage,size*2);			
			}
			delete[] RotatedImage;
			*/
			} else error=3;		
		} 
	} else {
		//in FKS mode, the first FKSDataImages number of subpictures of the first picture contain the absorption pictures and the FKSReferenceImages number of subictures contain probe pictures
		//the second picture contains the noise of the absorption picture
		//the third picture contains the noise of the probe picture.
		//we have to cut the pictures transmitted from the pictures recorded
		
		//in addition, if (NumberOfAdditionalReferenceImages>0) we have (FKSDataImages+FKSReferenceImages)*NumberOfAdditionalReferenceImages additional reference images, 
		//which we have to transmit
		
		//FKSImages=FKSDataImages+FKSReferenceImages;
		
		if ((yMax-yMin+1)!=FKSHeight*FKSImages) {
			ok=false;
			error=4;
		}
		//FKSDirtyLinesCut are the number of lines at the end of each picture with double exposure which look ugly and are thrown away
		int FKSDirtyLinesCut=ceil(((double)FKSDirtyLines)/vBin);
		FKSDirtyLinesCut=FKSDirtyLinesCut*vBin;
		int Width=(xMax-xMin)+1;
		long SubImageSize=FKSDataImages*(Width*(FKSHeight-FKSDirtyLinesCut))/(vBin*hBin);		
		for (int i=0;i<(NrImages);i++) {
			if (i<(NrImages-1)) {
				//Transmit images with atoms
				if (ok) ok=CheckReady(); else error=5;
				if (ok) {
					TakingImageStatus=3;
					UpdateStatus();	
					unsigned short *FKSRotatedImage1;
					FKSRotatedImage1=new unsigned short[SubImageSize];
					int MyWidth=Width/hBin;
					int MyHeight=(FKSHeight-FKSDirtyLinesCut)/vBin;
					int MyTotalHeight=FKSDataImages*MyHeight;
					//We need to rotate for our camera orientation
					long pos=0;
					for (int j=0;j<FKSDataImages;j++) {
						for (int x=0;x<MyHeight;x++) {
							for (int y=0;y<MyWidth;y++) {
								FKSRotatedImage1[x+y*MyTotalHeight+j*MyHeight]=Image[i][pos];
								pos++;
							}
						}
						pos=pos+(FKSDirtyLines*Width)/vBin;
					}
					WriteInteger(2*SubImageSize);			
					SendData((unsigned char*)FKSRotatedImage1,SubImageSize*2);						
					delete[] FKSRotatedImage1;
				} else error=6;
			}
			if (i!=(NrImages-2)) {
				//transmit reference images
				if (ok) ok=CheckReady(); else error=7;
				if (ok) {					
					long Offset=(Width*FKSHeight*FKSDataImages)/(vBin*hBin);
					TakingImageStatus=3;
					UpdateStatus();									
					unsigned short *FKSRotatedImage2;
					FKSRotatedImage2=new unsigned short[SubImageSize];
					int MyWidth=Width/hBin;
					int MyHeight=(FKSHeight-FKSDirtyLinesCut)/vBin;
					int MyTotalHeight=FKSDataImages*MyHeight;
					//We need to rotate for our camera orientation
					long pos=Offset;
					for (int j=0;j<FKSDataImages;j++) {
						for (int x=0;x<MyHeight;x++) {
							for (int y=0;y<MyWidth;y++) {
								FKSRotatedImage2[x+y*MyTotalHeight+j*MyHeight]=Image[i][pos];
								pos++;
							}
						}
						pos=pos+(FKSDirtyLines*Width)/vBin;
						if (pos>((FKSImages*FKSHeight*Width)/(vBin*hBin))) {
							pos=Offset;
						}
					}
					WriteInteger(2*SubImageSize);			
					SendData((unsigned char*)FKSRotatedImage2,SubImageSize*2);						
					delete[] FKSRotatedImage2;
				} else error=8;
			}
		} 		
	}
	
	if (error!=0) {
		CString buf;
		buf.Format("CAndorServerDlg::TransferImage : error %i",error);
		AfxMessageBox(buf);
	}
	/*for (int i=0;i<NrImages;i++) if (Image[i]) {
		delete Image[i];
		Image[i]=NULL;
		ImageSize[i]=0;
	}*/
	TakingImageStatus=0;
	UpdateStatus();
}

void CTCPIPServer::ProcessMessage(CString Message) {
	((CAndorServerApp*)AfxGetApp())->WakeUp();
	LastCommand=Message;
	UpdateStatus();
	if (Message=="Goodbye") StopServer();
	else if (Message=="Ready") SendReady();
	else if (Message=="Image") TakeImage();
	else if (Message=="Transfer") TransferImage();
	else if (Message=="Parameters") SetParameters();
	else {
		CString buf;
		buf.Format("CTCPIPServer::ProcessMessage : unknown message\n%s",Message);
		AfxMessageBox(buf);	
	}
	UpdateStatus();
}

void CTCPIPServer::StopServer() {
	DeleteClientSocket();
	if (DebugFile) (*DebugFile)<<"DeleteClientSocket"<<endl;
}

void CTCPIPServer::Disable()
{
	HardwareAccess=false;
}

void CTCPIPServer::SetCamera(CAndor *aAndor)
{
	Andor=aAndor;
}

void CTCPIPServer::UpdateStatus()
{
	if (AndorServerDlg) AndorServerDlg->UpdateStatus();
}

void CTCPIPServer::SetAndorServerDialog(CAndorServerDlg *aAndorServerDlg)
{
	AndorServerDlg=aAndorServerDlg;
}

CString CTCPIPServer::GetStatus() {
	CString TakingImage;
	switch (TakingImageStatus) {
		case 0:TakingImage="Idle";break;
		case 1:TakingImage="Recording";break;
		case 2:TakingImage="Waiting";break;
		case 3:TakingImage="Transfer";break;
	}
	CString Status;
	Status.Format("%s\n Last command: %s\n Debug %s\n Image: %s",(Connected) ? "Connected" : "Listening",LastCommand,(DebugFile==NULL) ? "Off" : "On",TakingImage);
	return Status;
}
