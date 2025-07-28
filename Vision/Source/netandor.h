#if !defined(__NetAndor_h)
#define __NetAndor_h

#include "network.h"
#include "framegbr.h"

//const NetAndorAquiringTimeOut = 40000;
const NetAndorAquiringTimeOut = 40000;
const MaxPictureData=256;

//*** CNetAndorFrameGrabber ***
class CNetAndorFrameGrabber : public CFrameGrabber {
public:
  CNetwork *network;
  bool CameraUsed;
  int DisplayedPictureNumber;
  int hbin;
  int vbin;
  int xstart;
  int ystart;
  int width;
  int height;
  //int ExposureTime;
  double ExposureTime;
  int ExternalTrigger;
  int vSpeed;
  //int hSpeed;
  double hSpeed;
  int FKSMode;
  int FKSHeight;
  int FKSDataImages;
  int FKSReferenceImages;
  int FKSvSpeed;
  int FKSExternalTriggerStart;
  int FKSDirtyLines;
	int NumberOfAdditionalReferenceImages;
	int DepthOfReferenceImageLibrary;
  bool PrePicture;
  bool DoInternalTriggerAfterExternalTrigger;
  double EichX;
  double EichY;
  //int Temperature;
  double Temperature;
  int FlushWait;
  DWORD LastUsed;
  TWindow *parent;
  char IPAdresse[200];
  ushort port;
  // the following parameters are not sent to NetAndorServer, but just locally stored
  int TriggerDelay;
  //read buffer to store image data transfered to this class over ethernet
  //up to four images can be stored: abs, probe, noiseabs, noiseprobe
  unsigned short* PictureData[MaxPictureData];
  int PictureHeight;
  int PictureWidth;
  int MainCamera;
  int FrambeGrabberNumber;
public:
  CNetAndorFrameGrabber(TWindow *parent,char *IPAdresse,ushort aport, bool HardwareAccess, int aFrambeGrabberNumber);
  ~CNetAndorFrameGrabber();
  int GetNrAdditionalProbePictures();
  void DebugOn(bool Append);
  void DebugOff();
  void FlushNetwork();
  void SendImageParameters();
  void SetExternalTrigger(int aExternalTrigger);
  void SetImageParameters(const char *aCameraDirection, bool aCameraUsed, int aDisplayedPictureNumber,
      int MainCamera, int ahbin, int avbin, int axstart,
      int aystart, int awidth, int aheight, double aExposureTime, double aTemperature,
      int aExternalTrigger, int avSpeed, double ahSpeed, int aFKSMode, int aFKSHeight,
      int aFKSDataImages, int aFKSReferenceImages, int aFKSvSpeed, int aFKSExternalTriggerStart, int aFKSDirtyLines,
      int aNumberPictures, bool aPrePicture, int aFlushWait, bool aDoInternalTriggerAfterExternalTrigger, int aTriggerDelay, double aAtomicMass,
      double aAtomicWavelength, double aAtomicClebschGordon, double aAtomicLinewidth,
      double aAtomicMagneticMoment, char* aAtomicName,
      double aProbeDetuning, double aExpansionTime, bool aFluorescencePicture,
      double aMaxFluorescence, double aCalibration,
      char* aFinalTrap, char* aEndState, double aEichX, double aEichY, int aNumberOfAdditionalReferenceImages, int aDepthOfReferenceImageLibrary);
  //int MainCamera, int ahbin, int avbin, int axstart,
  //    int aystart, int awidth, int aheight, int aExposureTime, int aTemperature,
  //    int aExternalTrigger, int avSpeed, int ahSpeed, int aFKSMode, int aFKSHeight,
  //    int aFKSDataImages, int aFKSReferenceImages, int aFKSvSpeed, int aFKSExternalTriggerStart, int aFKSDirtyLines,
  //    int aNumberPictures, bool aPrePicture, int aFlushWait, bool aDoInternalTriggerAfterExternalTrigger, int aTriggerDelay, double aAtomicMass,
  //    double aAtomicWavelength, double aAtomicClebschGordon, double aAtomicLinewidth,
  //    double aAtomicMagneticMoment, char* aAtomicName,
  //    double aProbeDetuning, double aExpansionTime, bool aFluorescencePicture,
  //    double aMaxFluorescence, double aCalibration,
  //    char* aFinalTrap, char* aEndState, double aEichX, double aEichY, int aNumberOfAdditionalReferenceImages, int aDepthOfReferenceImageLibrary);
      virtual void GetImageParameters(int &xh, int &yh, double &aEichX, double &aEichY, int &aBinningX, int &aBinningY, int &bitplanes);
  virtual unsigned long GetPictureData(long* PictureData, const long size,int quadrant);
  virtual int TakeTriggeredPicture();
  virtual void ShowCameraInformations();
  bool CheckReady(unsigned long timeout);
  unsigned long TransferPictureData(bool CreateSimulatedData);
   bool TestReady();
};

#endif