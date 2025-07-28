#if !defined(__ProbeLibrary_h)
#define __ProbeLibrary_h

class CPicture;
class CColorTable;

//*** CProbeLibrary ***
class CProbeLibrary {
public:
private:
   CPicture* ProbeList[1024];
   int NrInList;
   int LastProbe;
   int NrImages;
	char Filename[200];
   double axmin;
   double axmax;
   double aymin;
   double aymax;
   double axmini;
   double axmaxi;
   double aymini;
   double aymaxi;
public:
  CProbeLibrary(char *aFilename,int NrImages);
  ~CProbeLibrary();
  void AddNewImage(CPicture *Probe);
  CPicture* FindBestFit(CPicture *Absorption,double &ImageQuality);
  void SetRegionOfInterest(double aaxmin,double aaxmax,double aymin,double aaymax,
    double aaxmini,double aaxmaxi,double aymini,double aaymaxi);
  void SaveProbeLibrary(CColorTable *ColorTab);
  void LoadProbeLibrary();
};

#endif