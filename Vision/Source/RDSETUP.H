#if !defined(__rdsetup_h)
#define __rdsetup_h


//*** RDSetup ***
class CRDSetup {
public:
   //char MainDirectory[200];
   //char DataDirectory[200];
   bool HardwareAccess;
   char LiveDirectory;
   int LiveDisk;
   int DataDisk;
   double LiveConnectTime;
public:
  CRDSetup();
  ~CRDSetup();
};

#endif
