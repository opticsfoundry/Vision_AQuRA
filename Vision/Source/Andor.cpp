#include <stdio.h>
#include "Andor.h"
#include "atmcd32d.h"                                                                          

const int AndorWidth=1024;
const int AndorHeight=1024;

CAndorFrameGrabber::CAndorFrameGrabber(char* directory) {
  char aBuffer[256];
  char aBuffer2[256];
  int	 errorValue;
  bool errorFlag;

  FKS=false;
  FKSExposedRows=100;
  FKSExposureTime=0.01;
  EichX=1;
  EichY=1;
  ReadoutSpeed=0;
  ready=false;
  errorFlag=false;
  CoolerOn=false;
  initialized=false;
  HorizontalSpeed=NULL;
  VerticalSpeed=NULL;
  errorValue=Initialize(directory);  // Initialize driver in current directory
  wsprintf(aBuffer,"Initialization errors:\n");
  if (errorValue!=DRV_SUCCESS) {
    strcat(aBuffer,"Initialize Error\n");
    errorFlag=true;
  }
  // Get detector information
  errorValue=GetDetector(&XPixels,&YPixels);
  if (errorValue!=DRV_SUCCESS){
    strcat(aBuffer,"Get Detector information Error\n");
    errorFlag=true;
  }
  hbin=1;
  vbin=1;
  xstart=1;
  ystart=1;
  width=XPixels;
  height=YPixels;
  errorValue=SetImage(hbin,vbin,xstart,xstart+width-1,ystart,ystart+height-1);
  if (errorValue!=DRV_SUCCESS) {
    errorFlag=true;
    sprintf(aBuffer2,"SetImage %i\n",errorValue);
    strcat(aBuffer,aBuffer2);
  }
  // Set acquisition mode to single scan
  errorValue=SetAcquisitionMode(1); //single Scan
  if (errorValue!=DRV_SUCCESS){
    strcat(aBuffer,"Set Acquisition Mode Error\n");
    errorFlag=true;
  }
  // Set read mode to full image
  errorValue=SetReadMode(4); //Image
  if (errorValue!=DRV_SUCCESS){
    strcat(aBuffer,"Set Read Mode Error\n");
    errorFlag=true;
  }
  //GetTemperatureRange
  errorValue=GetTemperatureRange(&MinTemp,&MaxTemp);
  if(errorValue!=DRV_SUCCESS) {
    strcat(aBuffer,"Temperature error\n");
    errorFlag=true;
  }
  if (errorFlag) MessageBox(GetActiveWindow(),aBuffer,"Error!",MB_OK);
  // Save Start Time, we have to Wait for 2 seconds to allow MCD to
  // calibrate fully before allowing an acquisition to begin
  StartTime=GetTickCount();
  if (!errorFlag) {
    wsprintf(aBuffer,"Initialization errors:\n");
    errorValue=SetShutter(1,1,1,1); //????? warum 0 ms nicht moeglich  ????
    if (errorValue!=DRV_SUCCESS) {
      sprintf(aBuffer2,"SetShutter error %i\n",errorValue);
      strcat(aBuffer,aBuffer2);
      errorFlag=true;
    };
    float ExpTime=0.001;
    if (SetExposureTime(ExpTime)!=DRV_SUCCESS) {       //1 ms
      strcat(aBuffer,"SetExposureTime\n");
      errorFlag=true;
    };
    if (SetTriggerMode(1)!=DRV_SUCCESS) {       //external
      strcat(aBuffer,"SetTriggerMode\n");
      errorFlag=true;
    };
    TriggerMode=1;
    float ActExpTime,AccTime,KinTime;
    if (GetAcquisitionTimings(&ActExpTime,&AccTime,&KinTime)!=DRV_SUCCESS) {
      strcat(aBuffer,"GetAcquisitionTimings\n");
      errorFlag=true;
    }
    if (ActExpTime!=ExpTime) {
      sprintf(aBuffer2,"ExposureTime changed to %f\n",ActExpTime);
      strcat(aBuffer,aBuffer2);
      //errorFlag=true;
    }
    errorFlag|=GetNumberHorizontalSpeeds(&NrHorizontalSpeeds)!=DRV_SUCCESS;
    errorFlag|=GetNumberVerticalSpeeds(&NrVerticalSpeeds)!=DRV_SUCCESS;
    errorFlag|=GetNumberFKVShiftSpeeds(&NrFKVerticalSpeeds)!=DRV_SUCCESS;
    HorizontalSpeed=new int[NrHorizontalSpeeds];
    VerticalSpeed=new int[NrVerticalSpeeds];
    FKVerticalSpeed=new int[NrFKVerticalSpeeds];
    for (int i=0;i<NrHorizontalSpeeds;i++) errorFlag|=GetHorizontalSpeed(i,&HorizontalSpeed[i])!=DRV_SUCCESS;
    for (int i=0;i<NrVerticalSpeeds;i++) errorFlag|=GetVerticalSpeed(i,&VerticalSpeed[i])!=DRV_SUCCESS;
    for (int i=0;i<NrFKVerticalSpeeds;i++) errorFlag|=GetFKVShiftSpeed(i,&FKVerticalSpeed[i])!=DRV_SUCCESS;
    SetHorizontalShiftSpeed(NrHorizontalSpeeds-1);  //Slowest (a verifier!)
    errorFlag|=SetVerticalSpeed(NrVerticalSpeeds-1)!=DRV_SUCCESS; //Slowest (a verifier!)
    if (errorFlag) MessageBox(GetActiveWindow(),aBuffer,"Error!",MB_OK);
  }
  initialized=!errorFlag;
  ReadDataTime=GetTickCount();
}

void CAndorFrameGrabber::SetImageParameters(int ahbin, int avbin, int axstart,
    int aystart, int awidth, int aheight, double aEichX, double aEichY,
    bool aonlyoddlines,bool aFKS, int aFKExposedRows, int aFKSImages, int aFKSShiftSpeed, double aFKExposureTime,int FKSTriggerExtern) {
  if (!initialized) return;
  FKS=aFKS;
  FKSImages=aFKSImages;
  FKSExposedRows=aFKExposedRows;
  FKSExposureTime=aFKExposureTime;
  FKSShiftSpeed=aFKSShiftSpeed;
  if (awidth+axstart>AndorWidth) {
    axstart=0;
    if (awidth+axstart>AndorWidth) awidth=AndorWidth;
  }
  if (aheight+aystart>AndorHeight) {
    aystart=0;
    if (aheight+aystart>AndorHeight) aheight=AndorHeight;
  }
  unsigned int errorValue=SetImage(ahbin,avbin,axstart+1,axstart+awidth,aystart+1,aystart+aheight);
  if (errorValue!=DRV_SUCCESS) {
    char buf[100];
    sprintf(buf,"SetImage error nr %i",errorValue);
    MessageBox(GetActiveWindow(),buf,"Error!",MB_OK);
  } else {
    hbin=ahbin;
    vbin=avbin;
    xstart=axstart;
    ystart=aystart;
    width=awidth;
    height=aheight;
    EichX=aEichX*hbin;
    EichY=aEichY*vbin;
//    if (aonlyoddlines) EichY*=2;
  }
  if (FKS) {
    if (FKSTriggerExtern) SetTriggerModeExtern(); else SetTriggerModeExternStart();
    unsigned int errorValue0=SetFKVShiftSpeed(FKSShiftSpeed);
    unsigned int errorValue1=SetAcquisitionMode(4);  //FKS mode
    unsigned int errorValue=SetFastKinetics(FKSExposedRows,FKSImages,FKSExposureTime,4,hbin,vbin);
    if (errorValue!=DRV_SUCCESS || errorValue1!=DRV_SUCCESS || errorValue0!=DRV_SUCCESS) {
      char buf[100];
      sprintf(buf,"SetFastKinetics error nr %i, %i, %i",errorValue,errorValue1,errorValue0);
      MessageBox(GetActiveWindow(),buf,"Error!",MB_OK);
    }
    float time;
    GetFKExposureTime(&time);
    if ((time-FKSExposureTime)>0.00001) {
      char buf[100];
      sprintf(buf,"Fast Kinetics exposure time changed to %.4f",time);
      MessageBox(GetActiveWindow(),buf,"Error!",MB_OK);
    }
  } else {
    SetTriggerModeExtern();
    unsigned int errorValue1=SetAcquisitionMode(1);  //normal mode
    if (errorValue!=DRV_SUCCESS) {
      char buf[100];
      sprintf(buf,"SetAcquisitionMode error nr %i",errorValue);
      MessageBox(GetActiveWindow(),buf,"Error!",MB_OK);
    }
  }
  int temp;
  GetTemperature(&temp);
  if (temp<-40) temp=-40;
  if (temp>30) temp=30;
  SwitchCoolerOn(temp);
}

void CAndorFrameGrabber::SetTriggerModeIntern() {
  if (SetTriggerMode(0)!=DRV_SUCCESS) {
    MessageBox(GetActiveWindow(),"SetTriggerModeIntern","Error!",MB_OK);
  }
  TriggerMode=0;
}

void CAndorFrameGrabber::SetTriggerModeExtern() {
  if (SetTriggerMode(1)!=DRV_SUCCESS) {
    MessageBox(GetActiveWindow(),"SetTriggerModeExtern","Error!",MB_OK);
  }
  TriggerMode=1;
}

void CAndorFrameGrabber::SetTriggerModeExternStart() {
  if (SetTriggerMode(6)!=DRV_SUCCESS) {
    MessageBox(GetActiveWindow(),"SetTriggerModeExternStart","Error!",MB_OK);
  }
  TriggerMode=6;
}

void CAndorFrameGrabber::GetImageParameters(int &xh, int &yh, double &aEichX, double &aEichY,int &bitplanes) {
  if (FKS) {
    xh=width/hbin;
    yh=FKSExposedRows*FKSImages/vbin;
  } else {
    xh=width/hbin;
    yh=height/vbin;
  }
  aEichX=EichX;
  aEichY=EichY;
  bitplanes=16;
}

bool CAndorFrameGrabber::Ready(void) {
  int temp;
  if (!ready) ready=(GetTickCount()-StartTime>2000) && initialized &&
    (GetTemperature(&temp)==DRV_TEMP_STABILIZED);
  return ready;
}

void CAndorFrameGrabber::SwitchCoolerOn(int atemperature)
{
  char 		aBuffer[256];
  int 		errorValue;
  bool ErrorFlag=false;
  if (!initialized) return;
  wsprintf(aBuffer,"");
  temperature=atemperature;
  if (temperature<MinTemp) temperature=MinTemp;
  if (temperature>MaxTemp) temperature=20;
  if (temperature>MaxTemp) temperature=MaxTemp;
  //  switch on cooler and set temp
  errorValue=CoolerON();
  if(errorValue!=DRV_SUCCESS){
    wsprintf(aBuffer,"Could not switch cooler on");
    ErrorFlag=true;
  } else {
    CoolerOn=true;
    errorValue=SetTemperature(temperature);
    if(errorValue!=DRV_SUCCESS) {
      strcat(aBuffer,"Could not set temperature");
      ErrorFlag=true;
    }
  }
  if (ErrorFlag) MessageBox(GetActiveWindow(),aBuffer,"Error!",MB_OK);
}

void CAndorFrameGrabber::SwitchCoolerOff(void) {
  if(CoolerOFF()!=DRV_SUCCESS) {
    MessageBox(GetActiveWindow(),"Could not switch cooler off","Error!",MB_OK);
  }
}

bool CAndorFrameGrabber::CanClose() {
  if (!initialized) return true;
  bool errorFlag=false;
  SwitchCoolerOn(15);
  int temp;
  unsigned int errorValue=GetTemperature(&temp);
  if (!(errorValue==DRV_TEMP_OFF || errorValue==DRV_TEMP_STABILIZED || errorValue==DRV_TEMP_NOT_REACHED)) {
    errorFlag=true;
  }
  if (errorFlag) MessageBox(GetActiveWindow(),"Temperature","Error!",MB_OK);
  else if (temp<0) MessageBox(GetActiveWindow(),"Wait for Temperature to rise above 0 �C.","Wait!",MB_OK);
  return (temp>=0);
}

CAndorFrameGrabber::~CAndorFrameGrabber() {
  bool errorFlag=false;
  if (HorizontalSpeed) delete(HorizontalSpeed);
  HorizontalSpeed=NULL;
  if (VerticalSpeed) delete(VerticalSpeed);
  VerticalSpeed=NULL;
  if (FKVerticalSpeed) delete(FKVerticalSpeed);
  FKVerticalSpeed=NULL;
  if (!initialized) return;
  DWORD start=GetTickCount();
  int temp;
  char buffer[1024];
  char buf2[1024];
  wsprintf(buffer,"Shutdown errors:\n");
  unsigned int errorValue=SetTemperature(15);
  if (errorValue!=DRV_SUCCESS) {
    errorFlag=true;
    sprintf(buf2,"SetTemperature %i\n",errorValue);
    strcat(buffer,buf2);
  }
  errorValue=GetTemperature(&temp);
  if (!(errorValue==DRV_TEMP_OFF || errorValue==DRV_TEMP_STABILIZED || errorValue==DRV_TEMP_NOT_REACHED)) {
    errorFlag=true;
    sprintf(buf2,"GetTemperature %i\n",errorValue);
    strcat(buffer,buf2);
  }
  if (!errorFlag && temp<0) {
    MessageBox(GetActiveWindow(),"Waiting for Temperature to rise above 0 �C.","Wait!",MB_OK);
    do Sleep(10); while (temp<0 && GetTemperature(&temp)==DRV_TEMP_NOT_REACHED && (GetTickCount()-start)<600000);
  }
  if (temp<0) {
    errorFlag=true;
    strcat(buffer,"Temperature < 0 �C\n");
  }
  errorValue=CoolerOFF();
  if (errorValue!=DRV_SUCCESS) {
    errorFlag=true;
    sprintf(buf2,"CoolerOff %i\n",errorValue);
    strcat(buffer,buf2);
  }
  errorValue=ShutDown();
  if (errorValue!=DRV_SUCCESS) {
    errorFlag=true;
    sprintf(buf2,"ShutDown %i\n",errorValue);
    strcat(buffer,buf2);
  }
  if (errorFlag) MessageBox(GetActiveWindow(),buffer,"Error!",MB_OK);
}

int CAndorFrameGrabber::TakeTriggeredPicture(int quadrant) {
  if (!initialized) return 0;
  DWORD start2=GetTickCount();
  do ; while (GetTickCount()-ReadDataTime<30 && GetTickCount()-start2<30);
  if (!Ready()) {
    int temp;
    if (GetTemperature(&temp)==DRV_TEMP_NOT_REACHED)
      MessageBox(GetActiveWindow(),"Waiting for temperature to stabilize!","Wait!",MB_OK);;
    DWORD start=GetTickCount();
    do Sleep(10); while (!Ready() && (GetTickCount()-start<1000));
    if (!Ready()) {
      MessageBox(GetActiveWindow(),"Temperature not yet stabilized!","Wait!",MB_OK);;
      return 0;
    }
  }

  DWORD start1=GetTickCount();
  int status=DRV_ACQUIRING;
  while (status!=DRV_IDLE && GetTickCount()-start1<1000) {
    Sleep(10);
    if (GetStatus(&status)!=DRV_SUCCESS) {
      MessageBox(GetActiveWindow(),"GetStatus","Error",MB_OK);
      return 0;
    }
  }

  if (status!=DRV_IDLE) {
    MessageBox(GetActiveWindow(),"Andor System not ready","Error",MB_OK);
    return 0;
  }

  StartAcquisitionTime=GetTickCount();
  if (StartAcquisition()!=DRV_SUCCESS) {
    MessageBox(GetActiveWindow(),"StartAcquisition","Error",MB_OK);
    return 0;
  }
  return 1;
}

unsigned long CAndorFrameGrabber::GetPictureData(long* PictureData, long size, int quadrant) {
  if (!initialized) return 0;
  DWORD start=GetTickCount();
  int status=DRV_ACQUIRING;
  while (status==DRV_ACQUIRING && GetTickCount()-start<AndorAquiringTimeOut) {
    Sleep(10);
    if (GetStatus(&status)!=DRV_SUCCESS) {
      MessageBox(GetActiveWindow(),"GetStatus 2","Error",MB_OK);
      return 999999;
    }
  }
  if (status==DRV_ACQUIRING) {
    MessageBox(GetActiveWindow(),"GetPictureData : Aquiring too long","Error",MB_OK);
    return 999999;
  }
  unsigned int errorValue;
  errorValue=GetAcquiredData(PictureData,size);
  if (errorValue!=DRV_SUCCESS) {
    char buffer[200];
    sprintf(buffer,"GetAcquiredData Error %u",errorValue);
    MessageBox(GetActiveWindow(),buffer,"Error",MB_OK);
    return 999999;
  }
  ReadDataTime=GetTickCount();
  return ReadDataTime-StartAcquisitionTime;
}

void CAndorFrameGrabber::ShowCameraInformations() {
  int temp;
  unsigned int flag=GetTemperature(&temp);
  char buffer[1024];
  char buffer2[1024];
  wsprintf(buffer,"");
  sprintf(buffer2,"Goal Temperature=%i �C\n",temperature);
  strcat(buffer,buffer2);
  switch (flag) {
    case DRV_TEMP_OFF:strcat(buffer,"Temperature regulation is off\n"); break;
    case DRV_TEMP_STABILIZED:strcat(buffer,"Temperature stabilized\n");break;
    case DRV_TEMP_NOT_REACHED:strcat(buffer,"Temperature not yet reached\n");break;
    default : strcat(buffer,"Temperature error\n");
  }
  sprintf(buffer2,"Temperature=%i �C\n",temp);
  strcat(buffer,buffer2);
  float exposure;
  float acc;
  float kin;
  sprintf(buffer2,"Horizontal Bining=%i Vertical Bining=%i\n",hbin,vbin);
  strcat(buffer,buffer2);
  sprintf(buffer2,"Horizontal Start=%i Vertical Start=%i\n",xstart,ystart);
  strcat(buffer,buffer2);
  sprintf(buffer2,"Image Width=%i Image Height=%i\n",width,height);
  strcat(buffer,buffer2);
  sprintf(buffer2,"Horizontal Calibration=%.2f �m/Pixel\nVertical Calibration=%.2f �m/Pixel\n",EichX*1000,EichY*1000);
  strcat(buffer,buffer2);
  sprintf(buffer2,"Readout Speed=%.2f kHz\n",ReadoutSpeed);
  strcat(buffer,buffer2);
  if (FKS) {
    strcat(buffer,"Fast Kinetic Series\n");
    if (FKSShiftSpeed<NrFKVerticalSpeeds) sprintf(buffer2,"FKS Vertical Shift Speed=%.2f kHz\n",1000.0/FKVerticalSpeed[FKSShiftSpeed]);
    else sprintf(buffer2,"Vertical shift speed not valid\n");
    strcat(buffer,buffer2);
    sprintf(buffer2,"Exposed rows =%i\n",FKSExposedRows);
    strcat(buffer,buffer2);
    float exposure;
    int erreur2=GetFKExposureTime(&exposure);
    if (erreur2==DRV_SUCCESS) {
      sprintf(buffer2,"FKS Exposure Time=%.2f ms\n",exposure*1000);
    } else sprintf(buffer2,"GetFKSExposureTime error %i\n",erreur2);
    strcat(buffer,buffer2);
  } else {
    int erreur1=GetAcquisitionTimings(&exposure,&acc,&kin);
    if (erreur1==DRV_SUCCESS) {
      sprintf(buffer2,"Exposure Time=%.2f ms\n",exposure*1000);
      strcat(buffer,buffer2);
    } else {
      sprintf(buffer2,"GetAcquisitionTimings error %i\n",erreur1);
      strcat(buffer,buffer2);
    }
    strcat(buffer,"No Fast Kinetic Series\n");
  }
  if (TriggerMode==0) strcat(buffer,"Trigger Intern\n");
  else if (TriggerMode==1) strcat(buffer,"Trigger Extern\n");
  else if (TriggerMode==6) strcat(buffer,"Trigger Extern Start\n");
  MessageBox(GetActiveWindow(),buffer,"Andor Camera Information",MB_OK);
}

void CAndorFrameGrabber::SetHorizontalShiftSpeed(int speed) {
  if (!initialized) return;
  if ((0<=speed) && (speed<NrHorizontalSpeeds)) {
    if (SetHorizontalSpeed(speed)!=DRV_SUCCESS) MessageBox(GetActiveWindow(),"SetHorizontalShiftSpeed","Error!",MB_OK);
    else ReadoutSpeed=1000/HorizontalSpeed[speed];
  }
}

bool CAndorFrameGrabber::TemperatureStabilized(int &soll, int &ist) {
  if (!initialized) return true;
  unsigned int errorValue=GetTemperature(&ist);
  if (!(errorValue==DRV_TEMP_OFF || errorValue==DRV_TEMP_STABILIZED || errorValue==DRV_TEMP_NOT_REACHED))
    MessageBox(GetActiveWindow(),"Temperature","Error!",MB_OK);
  soll=temperature;
  return errorValue==DRV_TEMP_STABILIZED;
}

void CAndorFrameGrabber::DigitalOut(int port, int b) { //port=1..4 b=0..1
  OutAuxPort(1, b);
}

bool CAndorFrameGrabber::DigitalIn(int port) { //port=1..4
  int state;
  InAuxPort(port, &state);
  return state!=0;
}
