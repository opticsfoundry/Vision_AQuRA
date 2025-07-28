#include "framegbr.h"
#include <string.h>

CFrameGrabber::CFrameGrabber() {
}

CFrameGrabber::~CFrameGrabber() {
}

unsigned long CFrameGrabber::GetPictureData(long* PictureData, long size, int quadrant) {
  return 0;
};

void CFrameGrabber::SetImageParameters(int ahbin, int avbin, int axstart, int aystart, int awidth, int aheight, double EichX, double EichY,bool onlyoddlines) {
};

void CFrameGrabber::GetImageParameters(int &xh, int &yh, double &aEichX, double &aEichY,int &aBinningx, int &aBinningY, int &bitplanes) {
};

int CFrameGrabber::TakeTriggeredPicture(int quadrant) {
  return 0;
};

void CFrameGrabber::SetImageParameters(const char *aCameraDirection,double aAtomicMass,
    double aAtomicWavelength, double aAtomicClebschGordon, double aAtomicLinewidth,
    double aAtomicMagneticMoment, char *aAtomicName,
    double aProbeDetuning, double aExpansionTime, bool aFluorescencePicture,
    double aMaxFluorescence,double aCalibration,
    char *aFinalTrap, char *aEndState) {
   strcpy(CameraDirection,aCameraDirection);
   AtomicMass=aAtomicMass;
   AtomicWavelength=aAtomicWavelength;
   AtomicClebschGordon=aAtomicClebschGordon;
   AtomicLinewidth=aAtomicLinewidth;
   AtomicMagneticMoment=aAtomicMagneticMoment;
   strcpy(AtomicName,aAtomicName);
   ProbeDetuning=aProbeDetuning;
   ExpansionTime=aExpansionTime;
   FluorescencePicture=aFluorescencePicture;
   MaxFluorescence=aMaxFluorescence;
   Calibration=aCalibration;
   strcpy(FinalTrap,aFinalTrap);
   strcpy(EndState,aEndState);
}
