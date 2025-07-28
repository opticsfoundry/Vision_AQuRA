#if !defined(__setup_h)
#define __setup_h

#define SrBEC

#ifdef SrBEC

#define NetAndorCameraUsed

const char string_length = 300;

extern const bool MatroxCameraExtern;
extern unsigned short MatroxCameraPort;
extern char* MatroxCameraIPAdresse;
extern char* MatroxCameraComputerName;

extern unsigned short NetPrincetonCameraPort;
extern char* NetPrincetonCameraIPAdresse;

extern unsigned short NetAltaCameraPort;
extern char* NetAltaCameraIPAdresse;


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

const int MaxNrNetAndorCameras = 6;
extern int NrNetAndorCameras;
extern unsigned short NetAndorCameraPort[MaxNrNetAndorCameras];
extern char* NetAndorCameraIPAddress[MaxNrNetAndorCameras];
//char* NetAndorCameraIPAddress[NrNetAndorCameras] = { "192.168.88.103","192.168.88.103","192.168.88.103","192.168.88.103" };

extern bool ControlComputerSerial;
extern unsigned short ControlComputerPort;

//const char* ConfigFileName="Vision.cfg";
//const char* ParamFileName="VisionParams.dat";
//char* InitMainDirectory="";
//const char* InitDataDirectory="S:\\Vision\\Data";

//const char* ConfigFileName = "Vision.cfg";
//const char* ParamFileName = "VisionParams.dat";
//char* InitMainDirectory = "";
//const char* InitDataDirectory = "Data";

const char* ConfigFileName = "Vision.cfg";
extern char ParamFileName[string_length];
extern char MainDirectory[string_length];
extern char DataDirectory[string_length];

const bool InitHardwareAccess=true;
const char InitLiveDirectory='Live';
const int InitLiveDisk=2;
const int InitDataDisk=2;   //16 for Q disk
const double InitLiveConnectTime=10000;

//const NrPictureLists=7;
const int MaxNrPictureLists=6;
extern int NrPictureLists;
const int MaxPictureLines=2;
//const char *PictureListName[NrPictureLists]={{"0: MOT Side"},{"1: MOT Top"},{"2: Capt Side"},{"3: BM CA"},{"4: ALO"},{"5: BMCW"},{"6: ALC"}};
const char *PictureListName[MaxNrPictureLists]={{"0: IQC0"},{"1: IQC1"},{"2: IQC2"},{"3: IQC3"} ,{"4: IQC4"} ,{"5: IQC5"} };
//const NrCameraButtons=7;
const int MaxNrCameraButtons=6;
extern int NrCameraButtons;
//char *CameraButtonText[NrCameraButtons]={{"0"},{"1"},{"2"},{"3"},{"4"},{"5"},{"6"}};
char *CameraButtonText[MaxNrCameraButtons]={{"0"},{"1"},{"2"},{"3"} ,{"4"} ,{"5"} };
//int CameraButtonList[NrCameraButtons]={0,1,2,3,4,5,6};
int CameraButtonList[MaxNrCameraButtons]={0,1,2,3,4,5};
const int NrDetailButtons=2;
char *DetailButtonText[NrDetailButtons]={{"Details of single picture"},{"Set of pictures"}};
int DetailButtonList[NrDetailButtons]={0,1};
const int NrDetailPictureButtons=6;
char *DetailPictureButtonText[NrDetailButtons][NrDetailPictureButtons]={{{"Raw absorption"},{"Probe"},{"Absorption"},{"Absorption noise"},{"Probe noise"},{"Optical density"}},
  {{"0: IQC0"},{"1: IQC1"},{"2: IQC2"},{"3: IQC3"},{"4: IQC4"},{"5: IQC5"}}};   //Do not add any more even if you have more cameras - Vision crashes -SB 29/11/16
const int StartDetailButtonChecked=0;
//const int NrDetailPictureButtonX = 3;
const int NrDetailPictureButtonX = 3;

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

extern int filmtime;
extern int testserialtime;
extern int twopicturelists;
extern double MaxOptDensity6Li;
extern double MaxOptDensity7Li;
extern double ClebschLi7;
extern double ClebschLi6;
extern double GammaLi7;
extern double GammaLi6;
extern double EichHeight;
extern double EichOffset;
extern double SigmaEl;
extern double CameraInclination;
extern double OffsetButtonScale;
extern double InitialOffsetShift;
extern int WindowSize; // 1695
//int WindowSize = 1915; // 1695
//int WindowSize = 1695; // 1695
//WindowSize=1915; // 1695
extern int WindowHeight; //1170
//int WindowOriginX = 1930;     //value:1930 adapted for 2nd monitor
extern int WindowOriginX;     //value:1930 adapted for 2nd monitor
//int WindowOriginX = 0;     //value:1930 adapted for 2nd monitor
extern int WindowOriginY;        //adapted for 2nd monitor
extern int R;
extern int   PH;//200 cu;
extern  int  PH2;//200;
extern  int  PH3;//300  parameter selection box
extern  int  D;
extern  int  D2;
extern  int  WH;//341/400;
extern  int  WV;//512;
extern  int  BB;
extern  int  Slash;
extern  int  Text;//10
extern  int  SB;
extern  int  SH;
extern  int  PictureListHeight;//1000;
extern  int    BoxWide; //60
extern  int  TH; //
extern  int  D3;
extern  int  MTextBoxH;//91  Measurement parameters box
extern  int  GTextBoxH;//111 Gaussian and integral fit box
extern  int  TTextBoxH;  // Temperature and atom number fit box
 extern  int   TextBoxHeight; //18
extern    int  TextBox3Width;
 extern   int  TextBox4Width;
 extern   int  TextBox4WidthSmall;
  extern  int  InitialOptDensMethod;
  extern  int  PictureListColumns;

extern int DX0;
extern int PBy;
 extern  int PBx;
 extern  int PBly;
 extern  int PBlx;
   extern  int TBW;
 extern  int Y1;
 extern  int Y2;
   extern  int X1;


extern unsigned int TestCameraWidth;
extern unsigned int TestCameraHeight;
extern bool TestCameraExternalTrigger;

#endif

#endif

