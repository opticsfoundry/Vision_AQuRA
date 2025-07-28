#if !defined(__Framegbr_h)
#define __Framegbr_h

//*** CFrameGrabber ***
class CFrameGrabber {
public:
  double AtomicMass;
  double AtomicWavelength;
  double AtomicClebschGordon;
  double AtomicLinewidth;
  double AtomicMagneticMoment;
  char AtomicName[100];
  double ProbeDetuning;
  double ExpansionTime;
  bool FluorescencePicture;
  double MaxFluorescence;
  double Calibration;
  char FinalTrap[1000];
  char EndState[1000];
  char CameraDirection[1000];
  int NumberPictures;
  CFrameGrabber();
  ~CFrameGrabber();
  virtual unsigned long GetPictureData(long* PictureData, long size, int quadrant=0);
  virtual void SetImageParameters(int ahbin, int avbin, int axstart, int aystart, int awidth, int aheight, double EichX, double EichY,bool onlyoddlines=false);
  virtual void GetImageParameters(int &xh, int &yh, double &aEichX, double &aEichY,int &aBinningx, int &aBinningY,int &bitplanes);
  virtual int TakeTriggeredPicture(int quadrant=0);
  virtual void SetImageParameters(const char *CameraDirection, double aAtomicMass,
    double aAtomicWavelength, double aAtomicClebschGordon, double aAtomicLinewidth,
    double aAtomicMagneticMoment, char *aAtomicName,
    double aProbeDetuning, double aExpansionTime, bool aFluorescencePicture,
    double aMaxFluorescence, double aCalibration,
    char *aFinalTrap, char *aEndState);
};

#endif