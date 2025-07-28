#if !defined(__Andor_h)
#define __Andor_h
#include "framegbr.h"
#include <messageb.h>

//const AndorAquiringTimeOut = 40000;
const AndorAquiringTimeOut = 40000;

//*** CAndorFrameGrabber ***
class CAndorFrameGrabber : public CFrameGrabber {
public:
  int XPixels,YPixels;
  int MinTemp,MaxTemp;
  bool CoolerOn;
  unsigned long StartTime,StartAcquisitionTime;
  bool ready;
  bool initialized;
  int NrHorizontalSpeeds;
  int NrVerticalSpeeds;
  int NrFKVerticalSpeeds;
  int* HorizontalSpeed;
  int* VerticalSpeed;
  int* FKVerticalSpeed;
  int hbin,vbin,xstart,ystart,width,height;
  double EichX,EichY;
  int temperature;
  double ReadoutSpeed;
  bool FKS;
  int FKSExposedRows;
  int FKSImages;
  int FKSShiftSpeed;
  double FKSExposureTime;
  int TriggerMode;
private:
  unsigned long ReadDataTime;
public:
  CAndorFrameGrabber(char* directory);
  ~CAndorFrameGrabber();
  bool CanClose();
  void SetImageParameters(int ahbin, int avbin, int axstart,
    int aystart, int awidth, int aheight, double aEichX, double aEichY,
    bool aonlyoddlines,bool aFKS, int aFKExposedRows,int FKSImages,int aFKSShiftSpeed, double aFKExposureTime,int FKSTriggerExtern);
  virtual void GetImageParameters(int &xh, int &yh, double &aEichX, double &aEichY,int &bitplanes);
  void SwitchCoolerOn(int temperature);
  void SwitchCoolerOff(void);
  bool Ready(void);
  virtual int TakeTriggeredPicture(int quadrant=0);
  virtual unsigned long GetPictureData(long* PictureData, long size, int quadrant=0);
  void SetTriggerModeExtern();
  void SetTriggerModeIntern();
  void SetTriggerModeExternStart();
  void ShowCameraInformations();
  void SetHorizontalShiftSpeed(int speed);
  bool TemperatureStabilized(int &soll, int &ist);
  bool InFKSMode() {return FKS;}
  int GetFKSExposedRows() {return FKSExposedRows;}
  void DigitalOut(int port, int b); //port=1..4 b=0..1
  bool DigitalIn(int port); //port=1..4
};

#endif