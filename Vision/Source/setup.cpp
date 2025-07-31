#include "setup.h"

#ifdef SrBEC

const bool MatroxCameraExtern=true;
unsigned short MatroxCameraPort=2222;
char* MatroxCameraIPAdresse="192.168.88.101";
char* MatroxCameraComputerName="Enrico";

unsigned short NetPrincetonCameraPort=2654;
char* NetPrincetonCameraIPAdresse="192.168.88.101";

unsigned short NetAltaCameraPort=710;
char* NetAltaCameraIPAdresse="192.168.88.101";


//cameradata.serial={'15394769','15394754','15394770','15394761','16067721','16067733','16067717'};
//cameradata.name={'BMTop','BMCA','BMCW','ALO','ALC','TC','ALC2'};
//cameradata.rotate=[90,0,90,90,90,90,0];
//	CString help=m_lpCmdLine;
//	CameraNumber=0;CameraSerialNumber=15394769;MyIPPort=704;
//if (help == "BMTop") { CameraNumber = 0; CameraSerialNumber = 15394769; MyIPPort = 706; }; // BMTop
//if (help == "BMCA") { CameraNumber = 1; CameraSerialNumber = 15394754; MyIPPort = 707; }; // BMCA
//if (help == "BMCW") { CameraNumber = 2; CameraSerialNumber = 15394770; MyIPPort = 708; }; // BMCW
//if (help == "ALO") { CameraNumber = 3; CameraSerialNumber = 15394761; MyIPPort = 709; }; // ALO
//if (help == "ALC") { CameraNumber = 4; CameraSerialNumber = 16067721; MyIPPort = 710; }; // ALC
//if (help == "TC") { CameraNumber = 5; CameraSerialNumber = 16067733; MyIPPort = 711; }; // TC
//if (help == "ALC2") { CameraNumber = 6; CameraSerialNumber = 16067717; MyIPPort = 712; }; // ALC2
//if (help == "IQC0") { CameraNumber = 7; CameraSerialNumber = 18543660; MyIPPort = 713; }; // IQC0
//if (help == "IQC1") { CameraNumber = 8; CameraSerialNumber = 18543661; MyIPPort = 714; }; // IQC1
//if (help == "IQC2") { CameraNumber = 9; CameraSerialNumber = 20065978; MyIPPort = 715; }; // IQC2
//if (help == "IQC3") { CameraNumber = 10; CameraSerialNumber = 20068197; MyIPPort = 716; }; // IQC3

//if (help == "IQC0") { CameraNumber = 0; CameraSerialNumber = 18543660; MyIPPort = 713; }; // IQC0
//if (help == "IQC1") { CameraNumber = 1; CameraSerialNumber = 18543661; MyIPPort = 714; }; // IQC1
//if (help == "IQC2") { CameraNumber = 2; CameraSerialNumber = 20065978; MyIPPort = 715; }; // IQC2
//if (help == "IQC3") { CameraNumber = 3; CameraSerialNumber = 20068197; MyIPPort = 716; }; // IQC3


//const NrNetAndorCameras=8;
//unsigned short NetAndorCameraPort[NrNetAndorCameras]={704,703,712,707,709,708,710,706};
//char* NetAndorCameraIPAddress[NrNetAndorCameras]={"192.168.84.106","192.168.84.104","192.168.84.98","192.168.84.98","192.168.84.98","192.168.84.98","192.168.84.98","192.168.84.98"};

int NrNetAndorCameras=6;
unsigned short NetAndorCameraPort[MaxNrNetAndorCameras]={713,714,715,716,717,718};
char* NetAndorCameraIPAddress[MaxNrNetAndorCameras] = { "192.168.088.101","192.168.088.101","192.168.088.101","192.168.088.101","192.168.088.101","192.168.088.101" };
//char* NetAndorCameraIPAddress[NrNetAndorCameras] = { "192.168.88.103","192.168.88.103","192.168.88.103","192.168.88.103" };

bool ControlComputerSerial=false;
unsigned short ControlComputerPort=701;

//const char* ConfigFileName="Vision.cfg";
char ParamFileName[string_length]="VisionParams.dat";
char MainDirectory[string_length] = "";
char DataDirectory[string_length]="";

//const char* ConfigFileName = "Vision.cfg";
//const char* ParamFileName = "VisionParams.dat";
//char* InitMainDirectory = "";
//const char* InitDataDirectory = "Data";

//const char* ConfigFileName = "Vision.cfg";
//const char* ParamFileName = "VisionParams.dat";
//char* InitMainDirectory = "";
//const char* InitDataDirectory = "";

//const bool InitHardwareAccess=true;
//const char InitLiveDirectory='Live';
//const int InitLiveDisk=2;
//const int InitDataDisk=2;   //16 for Q disk
//const double InitLiveConnectTime=10000;

int NrPictureLists=6;
//const NrPictureLists=7;
//const int NrPictureLists=6;
//const int MaxPictureLines=2;
//const char *PictureListName[NrPictureLists]={{"0: MOT Side"},{"1: MOT Top"},{"2: Capt Side"},{"3: BM CA"},{"4: ALO"},{"5: BMCW"},{"6: ALC"}};
//const char *PictureListName[NrPictureLists]={{"0: IQC0"},{"1: IQC1"},{"2: IQC2"},{"3: IQC3"} ,{"4: IQC4"} ,{"5: IQC5"} };
//const NrCameraButtons=7;
int NrCameraButtons = 6;
//const int NrCameraButtons=6;
//char *CameraButtonText[NrCameraButtons]={{"0"},{"1"},{"2"},{"3"},{"4"},{"5"},{"6"}};
//char *CameraButtonText[NrCameraButtons]={{"0"},{"1"},{"2"},{"3"} ,{"4"} ,{"5"} };
//int CameraButtonList[NrCameraButtons]={0,1,2,3,4,5,6};
//int CameraButtonList[NrCameraButtons]={0,1,2,3,4,5};
//const int NrDetailButtons=2;
//char *DetailButtonText[NrDetailButtons]={{"Details of single picture"},{"Set of pictures"}};
//int DetailButtonList[NrDetailButtons]={0,1};
//const int NrDetailPictureButtons=6;
//char *DetailPictureButtonText[NrDetailButtons][NrDetailPictureButtons]={{{"Raw absorption"},{"Probe"},{"Absorption"},{"Absorption noise"},{"Probe noise"},{"Optical density"}},
//  {{"0: IQC0"},{"1: IQC1"},{"2: IQC2"},{"3: IQC3"},{"4: IQC4"},{"5: IQC5"}}};   //Do not add any more even if you have more cameras - Vision crashes -SB 29/11/16
//const int StartDetailButtonChecked=0;
//const int NrDetailPictureButtonX = 3;
//const int NrDetailPictureButtonX = 3;

//const NrPictureLists=7;
//const MaxPictureLines=2;
//const char *PictureListName[NrPictureLists]={{"0: MOT Side"},{"1: MOT Top"},{"2: Capt Side"},{"3: BM CA"},{"4: ALO"},{"5: BMCW"},{"6: ALC"}};
////const char *PictureListName[NrPictureLists]={{"0: MOT Side"},{"1: MOT Top"},{"2: Capt Side"},{"3: BM CA"},{"4: ALO"},{"5: ALC"}};
////const char *PictureListName[NrPictureLists]={{"0: MOT Andor"},{"1:Capture Andor"},{"2: MOT Top"},{"3: BM CW"},{"4: Capture ALC"},{"5: BM WC"},{"6: BM Top"}};
//const NrCameraButtons=7;
//char *CameraButtonText[NrCameraButtons]={{"0"},{"1"},{"2"},{"3"},{"4"},{"5"},{"6"}};
////char *CameraButtonText[NrCameraButtons]={{"0"},{"1"},{"2"},{"3"},{"4"},{"5"}};
////char *CameraButtonText[NrCameraButtons]={{"0:ALY"},{"1:CpY"},{"2:ALX"},{"3:BCW"},{"4:CpX"},{"5:BWC"},{"6:BTp"}};
//int CameraButtonList[NrCameraButtons]={0,1,2,3,4,5,6};
////int CameraButtonList[NrCameraButtons]={0,1,2,3,4,5};
//const NrDetailButtons=2;
//char *DetailButtonText[NrDetailButtons]={{"Details of single picture"},{"Set of pictures"}};
//int DetailButtonList[NrDetailButtons]={0,1};
//const NrDetailPictureButtons=6;
//char *DetailPictureButtonText[NrDetailButtons][NrDetailPictureButtons]={{{"Raw absorption"},{"Probe"},{"Absorption"},{"Absorption noise"},{"Probe noise"},{"Optical density"}},
//  {{"0: MOT SIde"},{"1: MOT Top"},{"2: Capt Side"},{"3: BM CA"},{"4: ALO"},{"5: BM CW"}}};   //Do not add any more even if you have more cameras - Vision crashes -SB 29/11/16
//const StartDetailButtonChecked=0;
//const NrDetailPictureButtonX=3;

   unsigned int TestCameraWidth = 1024;
   unsigned int TestCameraHeight =1024;
  bool TestCameraExternalTrigger=false;
  bool DebugControlComm = false;

int filmtime=500;
int testserialtime=50;
int twopicturelists=1;
double MaxOptDensity6Li=4.5;
double MaxOptDensity7Li=4.5;
double ClebschLi7=1.0/2.0;
double ClebschLi6=1.0/2.0;
double GammaLi7=122E6;
double GammaLi6=122E6;
double EichHeight=1/0.7;
double EichOffset=0.1;
double SigmaEl=6.9E-16;
double CameraInclination=0;
double OffsetButtonScale=0.01;
double InitialOffsetShift=1;
int WindowSize = 1810; // 1695
//int WindowSize = 1915; // 1695
//int WindowSize = 1695; // 1695
//WindowSize=1915; // 1695
int WindowHeight=1045; //1170
//int WindowOriginX = 1930;     //value:1930 adapted for 2nd monitor
int WindowOriginX=100;     //value:1930 adapted for 2nd monitor
//int WindowOriginX = 0;     //value:1930 adapted for 2nd monitor
int WindowOriginY=0;        //adapted for 2nd monitor
int R=10;
int   PH=200;//200 cu;
 int  PH2=200;//200;
 int  PH3=300;//300  parameter selection box
 int  D=5;
 int  D2=10;
 int  WH=341;//341/400;
 int  WV=400;//512;
 int  BB=16;
 int  Slash=6;
 int  Text=8;//10
 int  SB=25;
 int  SH=25;
 int  PictureListHeight=1000;//1000;
 int    BoxWide=70; //60
 int  TH=16; //
 int  D3=5;
 int  MTextBoxH=85;//91  Measurement parameters box
 int  GTextBoxH=120;//111 Gaussian and integral fit box
 int  TTextBoxH=91;  // Temperature and atom number fit box
  int   TextBoxHeight=18; //18
   int  TextBox3Width=140;
   int  TextBox4Width=115;
   int  TextBox4WidthSmall=95;
   int  InitialOptDensMethod=0;
   int  PictureListColumns=1;

int DX0=R+PH+D+SB+D2+WH+D2+BoxWide+D2;
int PBy=(WV-2*(TH+D3))/2.1;
  int PBx=PBy*WH/WV;
  int PBly=(WV-2*(TH+D3))/3.1;
  int PBlx=PBly*WH/WV;
    int TBW=PH+D+SB+D2+WH;
  int Y1=R+WV+D+SH+D2;
  int Y2=R+WV+D+SH+D2+PH+D2;
    int X1=R+PH+D+SB+D2+WH+D2;

#endif


