#if !defined(__visiondg_h)
#define __visiondg_h

#include "setup.h"
#include <window.h>
#include <opensave.h>
#include <gdiobjec.h>
#include <messageb.h>
#include <opensave.h>
#include "picture.h"
#include "picturew.h"
//#include "matrox.h"
#if defined(ApogeeCameraUsed)
	#include "apogeegrab.h"
#endif
#if defined(AltaCameraUsed)
	#include "alta.h"
#endif
//#include "netprinceton.h"
//#include "netalta.h"
//#include "SingleAtomDetection.h"
#include "andor.h"
#include "colortab.h"
#include "textbox.h"
#include "serial.h"
#include "names.h"
#include "measure.h"
#include "butonbox.h"
#include "piclist.h"
//#include "hamamatsu.h"
#include "params.h"
//#include "matroxserver.h"
#include <edit.h>
#include "comm.h"
//#include "probelibrary.h"
#include "netandor.h"

//*** TVisionDialog ***
class CTNMeasure;

const MaxThisRunMD=32;
const NrROIs=9;
const NrUsedROIs=8;
const MaxAdditionalProbe=128;
class TVisionDialog: public TWindow {
public:
//  CProbeLibrary* NetAltaProbeLibrary;  
  double ROI[NrROIs*8];
  int ROICam[NrROIs];
  bool CloseVisionFlag;
  double	StoreTriggerDelay2;
  double SigmaXReference;
  double SigmaYReference;
  TOpenSaveDialog::TData *MatrixFileData;
  TOpenSaveDialog::TData *BMPFileData;
  TOpenSaveDialog::TData *TGAFileData;
  TOpenSaveDialog::TData *MESFileData;
  TOpenSaveDialog::TData *ROIFileData;
  TOpenSaveDialog::TData *SerieFileData;
  TOpenSaveDialog::TData *ProfilFileData;
  CRDSetup *RDSetup;
  CMessPoint *MD;
  CMessPoint *ThisRunMD[MaxThisRunMD];
  CMyPoint **MetaPointer;
  int filming;
  int FilmCameraNr;
  int TakingPicture;
  CColorTable *ColorTable;
  TBrush*   BkBrush;
  TMessageBar *mb;
  CPicture* picture;
  CPicture* abspicture;
  CPicture* pictureabs;
  CPicture* pictureprobe;
  CPicture* pictureAdditionalprobe[MaxAdditionalProbe];
  int AktAdditionalProbe;
  CPicture* picturenoiseabs;
  CPicture* picturenoiseprobe;
  CPicture* probenoisepicture;

  char FilenameFirstCam[200];
  char FilenameSecondCam[200];
  
  CPicture* Epictureabs;
  CPicture* Epictureprobe;
  CPicture* Epicturenoiseabs;
  CPicture* Epicturenoiseprobe;
  CPicture* opticalDensity;
  CPicture* Averagepictureabs;
  CPicture* Averagepictureprobe;
  CPicture* Averagepicturenoiseabs;
  CPicture* Averagepicturenoiseprobe;
  CPicture* Averageprobenoisepicture;
  int AveragePicNr;

  CPictureProfilWindow* PictureWin;

  CPictureButton* DetailPictureButton[NrDetailPictureButtons];
  CPicture* PictureOfDetailButton[NrDetailPictureButtons];
  TStatic *DetailPictureButtonTitle[NrDetailPictureButtons];

  //these pictures store the main result (e.g. the opt dens picture) of this run
  //so that it is possible to quickly switch between these overview pictures and the detailed results
  CPicture* MainPicture[MaxNrPictureLists*MaxPictureLines];

  /*CPictureButton* ProbeNoiseButton;
  CPictureButton* AbsButton;
  CPictureButton* OptDensButton;
  CPictureButton* RawAbsButton;
  CPictureButton* NoiseButton;
  CPictureButton* ProbeButton;*/

  CProfil* profilH;
  CProfil* profilV;
  CNetAndorFrameGrabber *NetAndorFrameGrabber[MaxNrNetAndorCameras];
  //CNetPrincetonFrameGrabber *NetPrincetonFrameGrabber;
  //CNetAltaFrameGrabber *NetAltaFrameGrabber;
  //CSingleAtomDetectionFrameGrabber *SingleAtomDetectionFrameGrabber;
  //CMatroxFrameGrabber *MatroxFrameGrabber;
  //CAndorFrameGrabber *AndorFrameGrabber;
  #if defined(AltaCameraUsed)
    CAltaFrameGrabber *AltaFrameGrabber;
  #endif
  #if defined(ApogeeCameraUsed)
    CApogeeFrameGrabber *ApogeeFrameGrabber;
  #endif
  //CHamamatsuFrameGrabber *HamamatsuFrameGrabber;
  int autofit;
  CTextBox *CursorTextBox;
  CTextBox *MessDataTextBox;
  CTextBox *GaussFitTextBox;
  CTextBox *TNFitTextBox;
  CTextBox *Fit2DTextBox;
  CComm *serial;
  int checkingSerial;
  CName *Name;
  int InSerie;
  CButtonBox *MeasureTNButton;
  CButtonBox *CameraButton;
  CButtonBox *DetailButton;
  CPictureList *PictureList[MaxNrPictureLists];
  double MaxOptDensity;
  int CursorFixed;
  double FixedX0,FixedY0;
  char LastName[256];
  int CheckNumber;
  int AktCameraTyp;
  int AktCameraPosition;
  char AktCameraCode;
  double Calibration;
  char InitialDir[100];
  bool TempTextSet;
  int IteratePictureOccupation;
  bool LiveConnected;
  bool InTimer;
  char LivePicName[200];
  char LiveSerieName[200];
  bool LiveNewPicture;
  bool LiveNewSerie;
  bool LiveStopSerie;
  int MainPictureMode;
  int OptDensMethod;
  bool FitWithNumberRatio;
  bool FitGaussWithOffset;
  bool FitGaussWithSlope;
  bool FitPartialFermiL;
  bool FitPartialFermiTemp;
  bool FitPartialFermiNumber;
  bool FitFermiZeroTemp;
  bool Fit2DWithOffset;
  bool Fit2DWithSlope;
  bool Fit2DWithAngle;
  bool FitGaussXAmpEqualYAmp;
  int OptDensSource;
  int AktZoomMode;
 TStatic *FinalTrapText;
  bool ProfilDesign;
  bool DoIntensityCompensation;
  bool IsGridOnOff;
  double Fit2DParameters[9];
//  bool aktPic2Dfited;
  double (*akt2DFitFunc)(double, double,double []);
  bool IntegralWithOffset;
  bool Fit2DWithLongitudinalFlightCorrection;
  bool UpdateMeasurementAfterFit;
  bool DisplayMathErrors;
  bool Fit2DOnlyNotFitted;
  bool Display2DFitProfil;
  int BiningX;
  int BiningY;
  int FKBining;
  int FastKinetics;
  int FKSDistance;
  int FKSHeight;
  int FKSOffset;
  int InitialiseIoffeFit;
  bool ParamsInitialised;
  CParamList *ParamList;
  bool ParamsJustLoaded;
  bool Update2DFitDrawing;
  bool MatroxCameraServer;
  //CMatroxServer *MatroxServer;
  double CursorPosX;
  double CursorPosY;
  TEdit* LogbookEdit;
  int FitButtonFunction;
  int FirstFermiFit;
  bool Fit1DWithAmplitude;
  bool Fit1DWithWidth;
  bool FitFermiHorizontal;
  double TempBoson;
  struct time TakeAbsPictureStartTime;
  double AbsPictureTriggerDelay;
  bool AbsPictureTimerSet;
  int APNoiseMode;
  double APposx1,APposx2,APposy1,APposy2;
  CTNMeasure* TNmeasure;
  TStatic *PictureListNamePointer[MaxNrPictureLists];
  TStatic* AxText;
  bool SerialTimerActive;
  double MDXInformation[20];
  bool RunFinished;
public:
  void ReduceFringes();
  void SetROIFromProfilPos(int Nr);
  void GetProfilPosFromROI(int Nr);
  void ShowROIs();  
  void ShowROI(int Nr, bool DisplayError=true);
  void SetROI0();
  void SetROI1();
  void SetROI2();
  void SetROI3();
  void SetROI4();
  void SetROI5();
  void SetROI6();
  void SetROI7();
  void ShowROI0();
  void ShowROI1();
  void ShowROI2();
  void ShowROI3();
  void ShowROI4();
  void ShowROI5();
  void ShowROI6();
  void ShowROI7();
  void LoadROI0();
  void LoadROI1();
  void LoadROI2();
  void LoadROI3();
  void LoadROI4();
  void LoadROI5();
  void LoadROI6();
  void LoadROI7();
  void LoadROIs();
  void SaveROIs();
  void LoadROINr(int Nr);
  void LoadROIs(const char *FileName, int Nr);
  void SaveROIs(const char *FileName);

  void UpdateParameterList(CParamList *paramlist);
  void SaveParameterList(CParamList *paramlist, ofstream &out, bool SaveAll, bool Update, bool Distance, TEdit* LogbookEdit, bool FirstLine, bool SkipUnused, bool Skipping);
  void ReceiveDataFile();
  bool Close();
  void SaveParams();
  void LoadParams();
  void CopyAtomicDataFromFramegrabberToMessPoint(int FrameGrabberNr);
  void NetAltaReferencePicture();
  void NetAltaFluoPicture();
  void SetProfilPos();
  void CeEnableProbeLibrary(TCommandEnabler& ce);
  void EnableProbeLibrary();
  void CeEnableIntensityCompensation(TCommandEnabler& ce);  
  void EnableIntensityCompensation();
  void SavePictureAsBMPIcon();
  void SaveImage(bool FirstImage, unsigned int CameraNumber,unsigned int ImagePosition);
  TVisionDialog(TMessageBar *amb, bool HardwareAccess);
  ~TVisionDialog();
  bool CanClose();
  void GetZoomParameters(int &Zoom, double &ZoomCenterX, double &ZoomCenterY);
  void SetupWindow();
  bool EvEraseBkgnd(HDC);
  HBRUSH EvCtlColor(HDC, HWND hWndChild, UINT ctlType);
  void EvSysColorChange();
  void About();
  void TreatPicture(int SavaData,int insert, bool NewLine);
  void AbsorptionPicture(double TriggerDelay,double TriggerDelay2, bool TimerTriggered);
  void AbsorptionPictureWithoutDelay();
  void StartFilming(int Camera);
  void StopFilming();
  void NetPrincetonFilm();
  void NetAltaFilm();
  void AndorFilm();
  void AltaFilm();
  void ApogeeFilm();
  void HamamatsuFilm();
  void FilmAbsorption();
  void EvTimer(UINT timerId);
  void GridOnOff();
  void MandelBrot();
  void Gauss();
  void SaveProfil();
  void SavePicture();
  void SavePictureAsBMPNoBinning();
  void SavePictureAsBMP2Binning();
  void SavePictureAsBMP3Binning();
  void SavePictureAsBMP4Binning();
  void SavePictureAsBMP2FloatingBinning();
  void SavePictureAsBMP3FloatingBinning();
  void SavePictureAsBMP4FloatingBinning();
  void SavePictureAsBMP(int binning=1,bool floatingbinning=false);
  void SaveLowByteAsBMP();
  void SavePictureAsBWTGA();
  void SavePictureAsRGBTGA();
  void SavePictureAs(char *name,int aswhat);
  void LoadBMPPicture();
  void LoadTGAPicture();
  void LoadMeasurement();
  void DeleteMeasurement();
  void LoadMeasurement(const char *aname,bool insert=true,bool DataAlreadyLoaded=false,
    int LoadIsotop=0, bool OnlyOne=false, bool SpareMD=false);
  void LoadPictureData(const char *aname);
  void LoadPalette();
  void Fit1DGauss();
  void FitPartialFermi();
  void FitGaussWings(bool direction);
  void FitGaussWingsBothDirections();
  void FitFermi();
  void FitBose();
  void FitFermiOrBose(bool FitFermi);
  void Fit1DExponential();
  void FitTemperature();
  void FitAtomNumber();
  void FitSimpleAtomNumber();
  void FitChargingTime();
  void IntegralAtomNumber();
  void CalculateFormulas();
  void Fit1DHV(int horiz,double &Amp, double &X0, double &Sigma,
    double &Offset, double &Slope,double &RootMeanSquare,void (*funcs)(double, double [], double *, double [], int), double &err, double &chi);
  void ShowFluorescence();
  void ShowAbsorption();
  void ShowOpticalDensity();
  void ShowAbsDirect();
  void ShowProbe();
  void ShowNoise();
  void ShowPicture(int picNr);
  void LoadSerie();
  void AddSerie();
  void LoadSerieAllCameras();
  void AddSerieAllCameras();
  void LoadAddSerieAllCameras(int LoadAdd);
  void SaveSerie();
  void NewSerie();
  void StartSerie();
  void StopSerie();
  void ContinueSerie();
  LRESULT PictureChanged(WPARAM ,LPARAM);
  LRESULT PaletteChanged(WPARAM ,LPARAM);
  LRESULT Fit1DGauss(WPARAM ,LPARAM);
  LRESULT CursorMoved(WPARAM ,LPARAM);
  LRESULT LoadMess(WPARAM ,LPARAM);
  LRESULT IteratePicture(WPARAM ,LPARAM);
  void UpdateAll();
  void CreateFilm();
  void SetStandardPalette();
  void SetStandardPalette2();
  void SetLowBytePalette();
  void AddStripes();
  void CheckSerial();
  bool IdleAction(long idleCount);
  void RegisterInformation(int NoSerie,CMessPoint *aMD);
  void ToggleProfilDesign();
  void Fit1DGaussToSum();
  void FitLifeTime();
  void FitAbsorption();
  void MeasureTNButtonClicked();
  void CameraButtonClicked();
  void DetailButtonClicked();
  LRESULT ProfilCursorMoved(WPARAM newpicture,LPARAM);
  void ReCalc();
  void CalcRMSNoise();
  void SetProbeDetuning(double aProbeDetuning);
  void ProbePicture();
  void CheckDiskSpace();
  void FixCursor();
  void SortSerie();
  void ShowAndorCameraInformation();
  void ShowAltaCameraInformation();
  void ShowApogeeCameraInformation();
  void ShowNetPrincetonCameraInformation();
  void ShowNetAltaCameraInformation();
  void ShowHamamatsuCameraInformation();
  void delay(unsigned int time);
  void CreateSubstractNoisePictures();
  void ShiftTNmeasureParameters();
  void ToggleLiveConnection();
  void ZoomIn2();
  void CeZoomIn2(TCommandEnabler& ce);
  void ZoomIn4();
  void CeZoomIn4(TCommandEnabler& ce);
  void MainPictureMode0();
  void MainPictureMode1();
  void MainPictureMode2();
  void MainPictureMode3();
  void MainPictureMode4();
  void OptDensNormal();
  void OptDensAlice();
  void CeMainPictureMode0(TCommandEnabler& ce);
  void CeMainPictureMode1(TCommandEnabler& ce);
  void CeMainPictureMode2(TCommandEnabler& ce);
  void CeMainPictureMode3(TCommandEnabler& ce);
  void CeMainPictureMode4(TCommandEnabler& ce);
  void CeOptDensAlice(TCommandEnabler& ce);
  void CeOptDensNormal(TCommandEnabler& ce);
  void CeLiveConnection(TCommandEnabler& ce);
  bool CheckNoArgCommands(char *Command);
  void ChangeGaussXAmpEqualYAmp();
  void ChangeOptDensFromSample();
  void ChangeOptDensFromXandYGaussFit();
  void ChangeOptDensFromXGaussFit();
  void ChangeOptDensFromYGaussFit();
  void CeGaussFitWithOffset(TCommandEnabler& ce);
  void CeGaussFitWithSlope(TCommandEnabler& ce);
  void CePartialFermiFitL(TCommandEnabler& ce);
  void CeFermiZeroTemp(TCommandEnabler& ce);
  void CePartialFermiFitTemperature(TCommandEnabler& ce);
  void CePartialFermiFitNumber(TCommandEnabler& ce);
  void Ce2DFitWithOffset(TCommandEnabler& Ce);
  void Ce2DFitWithSlope(TCommandEnabler& Ce);
  void Ce2DFitWithAngle(TCommandEnabler& Ce);
  void Ce2DFitWithLongitudinalFlightCorrection(TCommandEnabler& Ce);
  void CeGaussXAmpEqualYAmp(TCommandEnabler& ce);
  void CeOptDensFromSample(TCommandEnabler& ce);
  void CeOptDensFromXandYGaussFit(TCommandEnabler& ce);
  void CeOptDensFromXGaussFit(TCommandEnabler& ce);
  void CeOptDensFromYGaussFit(TCommandEnabler& ce);
  void CeGridOnOff(TCommandEnabler& ce);
  void CeToggleProfilDesign(TCommandEnabler& ce);
  void CeFixCursor(TCommandEnabler& ce);
  void GetPictureData();
  void GetOpticalDensity();
  void FitIoffeDistribution();
  void IoffePicture();
  void Fit2DDistribution(int Function,
    void (*funcs)(double, double [], double *, double [], int),
    double (*FitFunc)(double, double,double []));
  void DisplayIoffeFit(bool updateall=true);
  void SetTrapParameters();
  void Display2DFitResults();
  void Display2DProfils();
  void DoAutomaticFit();
  void DisplayCutOf2DProfils();
  void ChangeFitPartialFermiL();
  void ChangeFitFermiZeroTemp();
  void ChangeFitPartialFermiTemp();
  void ChangeFitPartialFermiNumber();
  void ChangeFitFermiZeroTempNumber();
  void ChangeFitGaussWithOffset();
  void ChangeFitGaussWithSlope();
  void ChangeFit2DWithOffset();
  void ChangeFit2DWithSlope();
  void ChangeFit2DWithAngle();
  void ChangeFit2DWithLongitudinalFlightCorrection();
  void ChangeIntegralWithOffset();
  void CeIntegralWithOffset(TCommandEnabler& ce);
  void TNBackButtonClicked();
  void TNForwardButtonClicked();
  void DisplayMessData();
  void GetProfilPos();
  void MakePictureCode();
  void DisplayGaussFitResults();
  void DisplayFermiFitResults();
  void Store2DFitParams();
  void TNMessBackButtonClicked();
  void TNMessForwardButtonClicked();
  void SavePictureAsMatrix();
  void CeUpdateMeasurementAfterFit(TCommandEnabler& ce);
  void ChangeUpdateMeasurementAfterFit();
  void ReCalcIoffeFit();
  void ResetFitParameters(int Function);
  void Load2DFitParams();
  void CeDisplayMathErrors(TCommandEnabler& ce);
  void CeFit2DOnlyNotFitted(TCommandEnabler& ce);
  void ChangeDisplayMathErrors();
  void ChangeFit2DOnlyNotFitted();
  void T2DBackButtonClicked();
  void T2DForwardButtonClicked();
  void N2DBackButtonClicked();
  void N2DForwardButtonClicked();
  void Update2DFitParams();
  void DisplayIoffeFitUpdateAll();
  void Sz2DBackButtonClicked();
  void Sz2DForwardButtonClicked();
  void Sy2DBackButtonClicked();
  void Sy2DForwardButtonClicked();
  void Sx2DBackButtonClicked();
  void Sx2DForwardButtonClicked();
  void O2DBackButtonClicked();
  void O2DForwardButtonClicked();
  void ChangeDisplay2DFitProfil();
  void SetProbeLibraryROI();
  void SetProbeIntensityROI();
  void ShowProbeIntensityROI();
  void CeDisplay2DFitProfil(TCommandEnabler& ce);
  void Fit2DGaussDistribution();
  void ReCalc2DGauss();
  void MDSetCameraParameters();
  void Gauss2DPicture();
  void Fermi2DPicture();
  void InitialiseIoffeFitParams();
  void SetInitialiseIoffeFitFromGaussFit();
  void SetInitialiseIoffeFitFromFile();
  void SetInitialiseIoffeFitFromLastFit();
  void CeInitialiseIoffeFitFromGaussFit(TCommandEnabler& ce);
  void CeInitialiseIoffeFitFromFile(TCommandEnabler& ce);
  void CeInitialiseIoffeFitFromLastFit(TCommandEnabler& ce);
  void Set2DFitTrapParameters();
  void SynchroniseParameters();
  void LineUpButtonClicked();
  void LineDownButtonClicked();
  void OffsetUpButtonClicked();
  void OffsetDownButtonClicked();
  LRESULT NetworkRead(WPARAM ,LPARAM );
  void AveragePicture();
  void AverageOpticalDensity();
  void CalculateTOFTemperature();
  void ReCalcPartialFermi();
  void ReCalcFermi();
  void ReCalcBose();
  void ReCalcGaussWings();
  bool NewMD();
  bool NewMDForAbsPicture();
  void ShowFit();
  void Ce1DFitWithAmplitude(TCommandEnabler& Ce);
  void Ce1DFitWithWidth(TCommandEnabler& Ce);
  void Change1DFitWithAmplitude();
  void Change1DFitWithWidth();
  void CalculateTrapFrequencies(double &frad,double &fax);
  void CalculateT_TF();
  void Calculaten0rho0();
  void ChangeFitWithNumberRatio();
  void CeWithNumberRatio(TCommandEnabler& ce);
  void CeFermiFitHorizontal(TCommandEnabler& ce);
  void ChangeFermiFitHorizontal();
  void SetBlackWhitePalette();
  void SetWhiteBlackPalette();
  DWORD GetFreeMem();
  void LoadSerie(char* FileName);
  void NetAndorPicture(int CameraUsed);
  void ShowNetAndorCameraInformation();
  void MovePicturesToDetailPictureButtons(unsigned long c);
  void MovePicturesToDetailPictureButtonsAndRedraw();
  void NetAndorTest();
  void SelectMainPicture();
  void StoreReferenceSize();
  void PictureCamera(int Nr);
  void PictureCamera0();
  void PictureCamera1();
  void PictureCamera2();
  void PictureCamera3();
  void PictureCamera4();
  void PictureCamera5();
//  void PictureCamera6();
//  void PictureCamera7();
  void FilmCamera(int Nr);
  void FilmCamera0();
  void FilmCamera1();
  void FilmCamera2();
  void FilmCamera3();
  void FilmCamera4();
  void FilmCamera5();
//  void FilmCamera6();
//  void FilmCamera7();
private:
  DECLARE_RESPONSE_TABLE(TVisionDialog);
};

#endif
