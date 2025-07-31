#include "visiondg.h"
#include <malloc.h>
#include <color.h>
#include <static.h>
#include <stdio.h>
#include <math.h>
#include "vision.rh"
#include "picturew.h"
#include "profil.h"
#include "scale.h"
#include "fitmath.h"
#include "atommath.h"
#include <checkbox.h>
#include "setup.h"
#include "rdsetup.h"
#include <dos.h>
#include <dir.h>
#if defined(NetAndorCameraUsed)                               
	#include "NetAndor.h"
#endif

#include "densprof.h"
#include <time.h>
#include "serialnet.h"
#include "messpoint.h"
#include "leakdetect.h"

const UINT ID_FILMTIMER = 1;
const UINT ID_TESTSERIALTIMER = 2;
const UINT ID_LIVECONNECTIONTIMER = 3;
const UINT ID_ANDORCAMERATIMER = 4;

//*** TLaserDialog ***
DEFINE_RESPONSE_TABLE1(TVisionDialog, TWindow)
  EV_WM_ERASEBKGND,
  EV_WM_CTLCOLOR,
  EV_WM_SYSCOLORCHANGE,
  EV_BN_CLICKED(ID_MEASURETNBUTTON,MeasureTNButtonClicked),
  EV_BN_CLICKED(ID_CAMERABUTTON,CameraButtonClicked),
  EV_BN_CLICKED(ID_DETAILBUTTON,DetailButtonClicked),
  EV_BN_CLICKED(ID_2DSZBACK,Sz2DBackButtonClicked),
  EV_BN_CLICKED(ID_2DSZFORWARD,Sz2DForwardButtonClicked),
  EV_BN_CLICKED(ID_2DSYBACK,Sy2DBackButtonClicked),
  EV_BN_CLICKED(ID_2DSYFORWARD,Sy2DForwardButtonClicked),
  EV_BN_CLICKED(ID_2DSXBACK,Sx2DBackButtonClicked),
  EV_BN_CLICKED(ID_2DSXFORWARD,Sx2DForwardButtonClicked),
  EV_BN_CLICKED(ID_2DOBACK,O2DBackButtonClicked),
  EV_BN_CLICKED(ID_2DOFORWARD,O2DForwardButtonClicked),
  EV_BN_CLICKED(ID_2DNBACK,N2DBackButtonClicked),
  EV_BN_CLICKED(ID_2DNFORWARD,N2DForwardButtonClicked),
  EV_BN_CLICKED(ID_2DTBACK,T2DBackButtonClicked),
  EV_BN_CLICKED(ID_2DTFORWARD,T2DForwardButtonClicked),
  EV_BN_CLICKED(ID_TNBACK,TNBackButtonClicked),
  EV_BN_CLICKED(ID_TNFORWARD,TNForwardButtonClicked),
  EV_BN_CLICKED(ID_TNMESSBACK,TNMessBackButtonClicked),
  EV_BN_CLICKED(ID_TNMESSFORWARD,TNMessForwardButtonClicked),
  EV_BN_CLICKED(ID_LINEUP,LineUpButtonClicked),
  EV_BN_CLICKED(ID_LINEDOWN,LineDownButtonClicked),
  EV_BN_CLICKED(ID_OFFSETUP,OffsetUpButtonClicked),
  EV_BN_CLICKED(ID_OFFSETDOWN,OffsetDownButtonClicked),
  EV_COMMAND(CM_ABOUT, About),
  EV_COMMAND(CM_ZOOM_IN2, ZoomIn2),
  EV_COMMAND(CM_ZOOM_IN4, ZoomIn4),
  EV_COMMAND(CM_AVERAGEPICTURE, AveragePicture),
  EV_COMMAND(CM_AVERAGEOPTDENS,AverageOpticalDensity),
  EV_COMMAND(CM_STOPFILM, StopFilming),
  EV_COMMAND(CM_GRIDONOFF, GridOnOff),
  EV_COMMAND(CM_MANDELBROT, MandelBrot),
  EV_COMMAND(CM_GAUSS, Gauss2DPicture),
  EV_COMMAND(CM_FERMIPICTURE, Fermi2DPicture),
  EV_COMMAND(CM_FIT1DGAUSS, Fit1DGauss),
  EV_COMMAND(CM_FITPARTIALFERMI, FitPartialFermi),
  EV_COMMAND(CM_FITBOSE, FitBose),
  EV_COMMAND(CM_FITFERMI, FitFermi),
  EV_COMMAND(CM_FITGAUSSWINGS, FitGaussWingsBothDirections),
  EV_COMMAND(CM_FIT1DEXPONENTIAL, Fit1DExponential),
  EV_COMMAND(CM_FITIOFFEDISTRIBUTION, FitIoffeDistribution),
  EV_COMMAND(CM_FIT2DGAUSSDISTRIBUTION, Fit2DGaussDistribution),
  EV_COMMAND(CM_DISPLAYIOFFEFIT, DisplayIoffeFitUpdateAll),
  EV_COMMAND(CM_RECALC, ReCalc),
  EV_COMMAND(CM_REDUCEFRINGES, ReduceFringes),
  EV_COMMAND(CM_CREATEFILM, CreateFilm),
  EV_COMMAND(CM_RECALCIOFFEFIT, ReCalcIoffeFit),
  EV_COMMAND(CM_RECALC2DGAUSS, ReCalc2DGauss),
  EV_COMMAND(CM_RECALCFERMI, ReCalcFermi),
  EV_COMMAND(CM_RECALCBOSE, ReCalcBose),
  EV_COMMAND(CM_RECALCPARTIALFERMI, ReCalcPartialFermi),
  EV_COMMAND(CM_RECALCGAUSSWINGS, ReCalcGaussWings),
  EV_COMMAND(CM_CALCRMSNOISE, CalcRMSNoise),
  EV_COMMAND(CM_FIT1DGAUSSTOSUM, Fit1DGaussToSum),
  EV_COMMAND(CM_FITTEMPERATURE, FitTemperature),
  EV_COMMAND(CM_FITATOMNUMBER, FitAtomNumber),
  EV_COMMAND(CM_INTEGRALATOMNUMBER, IntegralAtomNumber),
  EV_COMMAND(CM_FITSIMPLEATOMNUMBER, FitSimpleAtomNumber),
  EV_COMMAND(CM_FITCHARGINGTIME, FitChargingTime),
  EV_COMMAND(CM_FITLIFETIME, FitLifeTime),
  EV_COMMAND(CM_SAVEPICTURE, SavePicture),
  EV_COMMAND(CM_SAVEPICTUREASMATRIX, SavePictureAsMatrix),
  EV_COMMAND(CM_SAVEPICTUREASBMPNOBINNING, SavePictureAsBMPNoBinning),
  EV_COMMAND(CM_SAVEPICTUREASBMP2BINNING, SavePictureAsBMP2Binning),
  EV_COMMAND(CM_SAVEPICTUREASBMP3BINNING, SavePictureAsBMP3Binning),
  EV_COMMAND(CM_SAVEPICTUREASBMP4BINNING, SavePictureAsBMP4Binning),
  EV_COMMAND(CM_SAVEPICTUREASBMP2FLOATINGBINNING, SavePictureAsBMP2FloatingBinning),
  EV_COMMAND(CM_SAVEPICTUREASBMP3FLOATINGBINNING, SavePictureAsBMP3FloatingBinning),
  EV_COMMAND(CM_SAVEPICTUREASBMP4FLOATINGBINNING, SavePictureAsBMP4FloatingBinning),
  EV_COMMAND(CM_SAVELOWBYTEASBMP, SaveLowByteAsBMP),
  EV_COMMAND(CM_SAVEPICTUREASRGBTGA, SavePictureAsRGBTGA),
  EV_COMMAND(CM_SAVEPICTUREASBWTGA, SavePictureAsBWTGA),
  EV_COMMAND(CM_LOADBMPPICTURE, LoadBMPPicture),
  EV_COMMAND(CM_LOADTGAPICTURE, LoadTGAPicture),
  EV_COMMAND(CM_LOADMEASUREMENT, LoadMeasurement),
  EV_COMMAND(CM_DELETEMEASUREMENT, DeleteMeasurement),
  EV_COMMAND(CM_LOADSERIE, LoadSerie),
  EV_COMMAND(CM_NEWSERIE, NewSerie),
  EV_COMMAND(CM_ADDSERIE, AddSerie),
  EV_COMMAND(CM_LOADSERIEALLCAMERAS, LoadSerieAllCameras),
  EV_COMMAND(CM_ADDSERIEALLCAMERAS, AddSerieAllCameras),
  EV_COMMAND(CM_SAVESERIE, SaveSerie),
  EV_COMMAND(CM_STARTSERIE, StartSerie),
  EV_COMMAND(CM_STOPSERIE, StopSerie),
  EV_COMMAND(CM_CONTINUESERIE, ContinueSerie),
  EV_COMMAND(CM_SORTSERIE, SortSerie),
  EV_COMMAND(CM_LOADPALETTE, LoadPalette),
  EV_COMMAND(CM_ABSORPTIONPICTURE, AbsorptionPictureWithoutDelay),
  EV_COMMAND(CM_SHOWFLUORESCENCE, ShowFluorescence),
  EV_COMMAND(CM_SHOWABSORPTION, ShowAbsorption),
  EV_COMMAND(CM_SHOWOPTICALDENSITY, ShowOpticalDensity),
  EV_COMMAND(CM_SHOWABSDIRECT, ShowAbsDirect),
  EV_COMMAND(CM_SHOWPROBE, ShowProbe),
  EV_COMMAND(CM_SHOWNOISE, ShowNoise),
  EV_COMMAND(CM_PROFILDESIGN, ToggleProfilDesign),
  EV_COMMAND(CM_ANDORCAMERAINFORMATION, ShowNetAndorCameraInformation),
  EV_COMMAND(CM_PICTURECAMERA0, PictureCamera0),
  EV_COMMAND(CM_PICTURECAMERA1, PictureCamera1),
  EV_COMMAND(CM_PICTURECAMERA2, PictureCamera2),
  EV_COMMAND(CM_PICTURECAMERA3, PictureCamera3),
  EV_COMMAND(CM_PICTURECAMERA4, PictureCamera4),
  EV_COMMAND(CM_PICTURECAMERA5, PictureCamera5),
//  EV_COMMAND(CM_PICTURECAMERA6, PictureCamera6),
//  EV_COMMAND(CM_PICTURECAMERA7, PictureCamera7),
  EV_COMMAND(CM_FILMCAMERA0, FilmCamera0),
  EV_COMMAND(CM_FILMCAMERA1, FilmCamera1),
  EV_COMMAND(CM_FILMCAMERA2, FilmCamera2),
  EV_COMMAND(CM_FILMCAMERA3, FilmCamera3),
  EV_COMMAND(CM_FILMCAMERA4, FilmCamera4),
  EV_COMMAND(CM_FILMCAMERA5, FilmCamera5),
//  EV_COMMAND(CM_FILMCAMERA6, FilmCamera6),
//  EV_COMMAND(CM_FILMCAMERA7, FilmCamera7),
  EV_COMMAND(CM_ANDORTEST, NetAndorTest),
  EV_COMMAND(CM_STOREREFERENCESIZE, StoreReferenceSize),
  EV_MESSAGE(IDM_PICTURECHANGED,PictureChanged),
  EV_MESSAGE(IDM_LOADPALETTE,PaletteChanged),
  EV_MESSAGE(IDM_AUTOFIT,Fit1DGauss),
  EV_MESSAGE(IDM_LOADMEASUREMENT,LoadMess),
  EV_MESSAGE(IDM_ITERATEPICTURE,IteratePicture),
  EV_MESSAGE(IDM_PROFILCURSORMOVED,ProfilCursorMoved),
  EV_MESSAGE(IDM_CURSORMOVED,CursorMoved),
  EV_COMMAND(CM_PALETTESTANDARD, SetStandardPalette),
  EV_COMMAND(CM_PALETTESTANDARD2, SetStandardPalette2),
  EV_COMMAND(CM_PALETTEBLACKWHITE, SetBlackWhitePalette),
  EV_COMMAND(CM_PALETTEWHITEBLACK, SetWhiteBlackPalette),
  EV_COMMAND(CM_PALETTELOWBYTE, SetLowBytePalette),
  EV_COMMAND(CM_PALETTESTRIPES, AddStripes),
  EV_COMMAND(CM_FIXCURSOR, FixCursor),
  EV_COMMAND(CM_CREATESUBSTRACTNOISEPICTURE, CreateSubstractNoisePictures),
  EV_COMMAND(CM_SHIFTTNMEASUREPARAMETERS, ShiftTNmeasureParameters),
  EV_COMMAND(CM_SAVEPROFIL, SaveProfil),
  EV_COMMAND(CM_TOGGLELIVECONNECTION, ToggleLiveConnection),
  EV_COMMAND(CM_MAINPICTUREMODE0, MainPictureMode0),
  EV_COMMAND(CM_MAINPICTUREMODE1, MainPictureMode1),
  EV_COMMAND(CM_MAINPICTUREMODE2, MainPictureMode2),
  EV_COMMAND(CM_MAINPICTUREMODE3, MainPictureMode3),
  EV_COMMAND(CM_MAINPICTUREMODE4, MainPictureMode4),
  EV_COMMAND(CM_OPTDENSNORMAL, OptDensNormal),
  EV_COMMAND(CM_OPTDENSALICE, OptDensAlice),
  EV_COMMAND(CM_1DFITWITHAMPLITUDE, Change1DFitWithAmplitude),
  EV_COMMAND(CM_1DFITWITHWIDTH, Change1DFitWithWidth),
  EV_COMMAND(CM_FERMIFITHORIZONTAL, ChangeFermiFitHorizontal),
  EV_COMMAND(CM_FERMIZEROTEMP, ChangeFitFermiZeroTemp),
  EV_COMMAND(CM_PARTIALFERMIFITL, ChangeFitPartialFermiL),
  EV_COMMAND(CM_PARTIALFERMIFITNUMBER, ChangeFitPartialFermiNumber),
  EV_COMMAND(CM_PARTIALFERMIFITTEMP, ChangeFitPartialFermiTemp),
  EV_COMMAND(CM_FERMIFITWITHNUMBERRATIO, ChangeFitWithNumberRatio),
  EV_COMMAND(CM_GAUSSFITWITHOFFSET, ChangeFitGaussWithOffset),
  EV_COMMAND(CM_INTEGRALWITHOFFSET, ChangeIntegralWithOffset),
  EV_COMMAND(CM_GAUSSFITWITHSLOPE, ChangeFitGaussWithSlope),
  EV_COMMAND(CM_2DFITWITHOFFSET, ChangeFit2DWithOffset),
  EV_COMMAND(CM_2DFITWITHSLOPE, ChangeFit2DWithSlope),
  EV_COMMAND(CM_2DFITWITHANGLE, ChangeFit2DWithAngle),
  EV_COMMAND(CM_2DFITWITHLONGITUDINALFLIGHTCORRECTION, ChangeFit2DWithLongitudinalFlightCorrection),
  EV_COMMAND(CM_INIT2DFITFROMGAUSSFIT, SetInitialiseIoffeFitFromGaussFit),
  EV_COMMAND(CM_INIT2DFITFROMFILE, SetInitialiseIoffeFitFromFile),
  EV_COMMAND(CM_INIT2DFITFROMLASTFIT, SetInitialiseIoffeFitFromLastFit),
  EV_COMMAND(CM_GAUSSXAMPEQUALYAMP, ChangeGaussXAmpEqualYAmp),
  EV_COMMAND(CM_OPTICALDENSITYFROMSAMPLE, ChangeOptDensFromSample),
  EV_COMMAND(CM_OPTICALDENSITYFROMXANDYGAUSSFIT, ChangeOptDensFromXandYGaussFit),
  EV_COMMAND(CM_OPTICALDENSITYFROMXGAUSSFIT, ChangeOptDensFromXGaussFit),
  EV_COMMAND(CM_OPTICALDENSITYFROMYGAUSSFIT, ChangeOptDensFromYGaussFit),
  EV_COMMAND(CM_IOFFEPICTURE, IoffePicture),
  EV_COMMAND(CM_UPDATEMEASUREMENTAFTERFIT, ChangeUpdateMeasurementAfterFit),
  EV_COMMAND(CM_DISPLAYMATHERRORS, ChangeDisplayMathErrors),
  EV_COMMAND(CM_FIT2DONLYNOTFITTED, ChangeFit2DOnlyNotFitted),
  EV_COMMAND(CM_DISPLAY2DFITPROFIL, ChangeDisplay2DFitProfil),
  EV_COMMAND(CM_SETPROBELIBRARYROI, SetProbeLibraryROI),
  EV_COMMAND(CM_ENABLEPROBELIBRARY,EnableProbeLibrary),
  EV_COMMAND_ENABLE(CM_ENABLEPROBELIBRARY, CeEnableProbeLibrary),

  EV_COMMAND(CM_SETPROBEINTENSITYROI, SetProbeIntensityROI),
  EV_COMMAND(CM_SHOWPROBEINTENSITYROI, ShowProbeIntensityROI),
  EV_COMMAND(CM_ENABLEINTENSITYCOMPENSATION,EnableIntensityCompensation),
  EV_COMMAND_ENABLE(CM_ENABLEINTENSITYCOMPENSATION, CeEnableIntensityCompensation),

  EV_COMMAND(CM_SHOWROIS, ShowROIs),
  EV_COMMAND(CM_SETROI0, SetROI0),
  EV_COMMAND(CM_SETROI1, SetROI1),
  EV_COMMAND(CM_SETROI2, SetROI2),
  EV_COMMAND(CM_SETROI3, SetROI3),
  EV_COMMAND(CM_SETROI4, SetROI4),
  EV_COMMAND(CM_SETROI5, SetROI5),
  EV_COMMAND(CM_SETROI6, SetROI6),
  EV_COMMAND(CM_SETROI7, SetROI7),
  EV_COMMAND(CM_SHOWROI0, ShowROI0),
  EV_COMMAND(CM_SHOWROI1, ShowROI1),
  EV_COMMAND(CM_SHOWROI2, ShowROI2),
  EV_COMMAND(CM_SHOWROI3, ShowROI3),
  EV_COMMAND(CM_SHOWROI4, ShowROI4),
  EV_COMMAND(CM_SHOWROI5, ShowROI5),
  EV_COMMAND(CM_SHOWROI6, ShowROI6),
  EV_COMMAND(CM_SHOWROI7, ShowROI7),

  EV_COMMAND(CM_LOADROI0, LoadROI0),
  EV_COMMAND(CM_LOADROI1, LoadROI1),
  EV_COMMAND(CM_LOADROI2, LoadROI2),
  EV_COMMAND(CM_LOADROI3, LoadROI3),
  EV_COMMAND(CM_LOADROI4, LoadROI4),
  EV_COMMAND(CM_LOADROI5, LoadROI5),
  EV_COMMAND(CM_LOADROI6, LoadROI6),
  EV_COMMAND(CM_LOADROI7, LoadROI7),
  EV_COMMAND(CM_LOADROIS, LoadROIs),
  EV_COMMAND(CM_SAVEROIS, SaveROIs),

  EV_COMMAND_ENABLE(CM_ZOOM_IN2, CeZoomIn2),
  EV_COMMAND_ENABLE(CM_ZOOM_IN4, CeZoomIn4),
  EV_COMMAND_ENABLE(CM_OPTDENSNORMAL, CeOptDensNormal),
  EV_COMMAND_ENABLE(CM_OPTDENSALICE, CeOptDensAlice),
  EV_COMMAND_ENABLE(CM_MAINPICTUREMODE0, CeMainPictureMode0),
  EV_COMMAND_ENABLE(CM_MAINPICTUREMODE1, CeMainPictureMode1),
  EV_COMMAND_ENABLE(CM_MAINPICTUREMODE2, CeMainPictureMode2),
  EV_COMMAND_ENABLE(CM_MAINPICTUREMODE3, CeMainPictureMode3),
  EV_COMMAND_ENABLE(CM_MAINPICTUREMODE4, CeMainPictureMode4),
  EV_COMMAND_ENABLE(CM_FERMIFITHORIZONTAL, CeFermiFitHorizontal),
  EV_COMMAND_ENABLE(CM_FERMIZEROTEMP, CeFermiZeroTemp),
  EV_COMMAND_ENABLE(CM_PARTIALFERMIFITL, CePartialFermiFitL),
  EV_COMMAND_ENABLE(CM_PARTIALFERMIFITTEMP, CePartialFermiFitTemperature),
  EV_COMMAND_ENABLE(CM_PARTIALFERMIFITNUMBER, CePartialFermiFitNumber),
  EV_COMMAND_ENABLE(CM_FERMIFITWITHNUMBERRATIO, CeWithNumberRatio),
  EV_COMMAND_ENABLE(CM_GAUSSFITWITHOFFSET, CeGaussFitWithOffset),
  EV_COMMAND_ENABLE(CM_INTEGRALWITHOFFSET, CeIntegralWithOffset),
  EV_COMMAND_ENABLE(CM_GAUSSFITWITHSLOPE, CeGaussFitWithSlope),
  EV_COMMAND_ENABLE(CM_1DFITWITHAMPLITUDE, Ce1DFitWithAmplitude),
  EV_COMMAND_ENABLE(CM_1DFITWITHWIDTH, Ce1DFitWithWidth),
  EV_COMMAND_ENABLE(CM_2DFITWITHOFFSET, Ce2DFitWithOffset),
  EV_COMMAND_ENABLE(CM_2DFITWITHSLOPE, Ce2DFitWithSlope),
  EV_COMMAND_ENABLE(CM_2DFITWITHANGLE, Ce2DFitWithAngle),
  EV_COMMAND_ENABLE(CM_2DFITWITHLONGITUDINALFLIGHTCORRECTION, Ce2DFitWithLongitudinalFlightCorrection),
  EV_COMMAND_ENABLE(CM_GAUSSXAMPEQUALYAMP, CeGaussXAmpEqualYAmp),
  EV_COMMAND_ENABLE(CM_OPTICALDENSITYFROMSAMPLE, CeOptDensFromSample),
  EV_COMMAND_ENABLE(CM_OPTICALDENSITYFROMXANDYGAUSSFIT, CeOptDensFromXandYGaussFit),
  EV_COMMAND_ENABLE(CM_OPTICALDENSITYFROMXGAUSSFIT, CeOptDensFromXGaussFit),
  EV_COMMAND_ENABLE(CM_OPTICALDENSITYFROMYGAUSSFIT, CeOptDensFromYGaussFit),
  EV_COMMAND_ENABLE(CM_TOGGLELIVECONNECTION, CeLiveConnection),
  EV_COMMAND_ENABLE(CM_GRIDONOFF, CeGridOnOff),
  EV_COMMAND_ENABLE(CM_PROFILDESIGN, CeToggleProfilDesign),
  EV_COMMAND_ENABLE(CM_FIXCURSOR, CeFixCursor),
  EV_COMMAND_ENABLE(CM_UPDATEMEASUREMENTAFTERFIT, CeUpdateMeasurementAfterFit),
  EV_COMMAND_ENABLE(CM_DISPLAYMATHERRORS, CeDisplayMathErrors),
  EV_COMMAND_ENABLE(CM_FIT2DONLYNOTFITTED, CeFit2DOnlyNotFitted),
  EV_COMMAND_ENABLE(CM_DISPLAY2DFITPROFIL, CeDisplay2DFitProfil),
  EV_COMMAND_ENABLE(CM_INIT2DFITFROMGAUSSFIT, CeInitialiseIoffeFitFromGaussFit),
  EV_COMMAND_ENABLE(CM_INIT2DFITFROMFILE, CeInitialiseIoffeFitFromFile),
  EV_COMMAND_ENABLE(CM_INIT2DFITFROMLASTFIT, CeInitialiseIoffeFitFromLastFit),
  EV_WM_TIMER,
END_RESPONSE_TABLE;
HBRUSH TVisionDialog::EvCtlColor(HDC hDC, HWND , UINT )
{
try {
  TColor h=::GetSysColor(COLOR_BTNFACE);
  ::SetBkColor(hDC, ::GetSysColor(COLOR_BTNFACE));
  return *BkBrush;
  }
  catch (TGdiBase::TXGdi& x) {
    MessageBeep(0xFFFFFFFF);
    MessageBeep(MB_ICONASTERISK);
    MessageBeep(MB_ICONEXCLAMATION);
    MessageBeep(MB_ICONHAND);
	 MessageBeep(MB_ICONQUESTION);
    MessageBeep(MB_OK);
     ofstream DebugFile("DebugPicture.dat", ios::app);
     bool found;
     string s=x.ResourceIdToString(&found,x.GetErrorCode());
	  DebugFile<<"GDI error TVisionDialog::EvCtlColor ";
     if (found) DebugFile<<s;
     DebugFile<<endl;
	  DebugFile.close();
     return NULL;
   }
}

void TVisionDialog::EvSysColorChange()
{
  #if defined(DetectLeaks)
     LeakDetect.Delete(BkBrush);
  #endif
  delete BkBrush;
  try {
  BkBrush = new TBrush(::GetSysColor(COLOR_BTNFACE));
  #if defined(DetectLeaks)
    LeakDetect.New(BkBrush,5,1);
  #endif
  }
  catch (TGdiBase::TXGdi& x) {
    MessageBeep(0xFFFFFFFF);
    MessageBeep(MB_ICONASTERISK);
    MessageBeep(MB_ICONEXCLAMATION);
    MessageBeep(MB_ICONHAND);
	 MessageBeep(MB_ICONQUESTION);
    MessageBeep(MB_OK);
     ofstream DebugFile("DebugPicture.dat", ios::app);
     bool found;
     string s=x.ResourceIdToString(&found,x.GetErrorCode());
	  DebugFile<<"GDI error TVisionDialog::EvSysColorChange ";
     if (found) DebugFile<<s;
     DebugFile<<endl;
	  DebugFile.close();
   }
}

bool TVisionDialog::EvEraseBkgnd(HDC hDC)
{
  try {
  TDC dc(hDC);
  dc.TextRect(GetClientRect(), ::GetSysColor(COLOR_BTNFACE));
  TBrush highlight(::GetSysColor(COLOR_BTNHIGHLIGHT));
  dc.SelectObject(highlight);
  dc.PatBlt(0, 0, Attr.W, 2);
  dc.PatBlt(0, 2, 2, Attr.H-2);
  TBrush shadow(::GetSysColor(COLOR_BTNSHADOW));
  dc.SelectObject(shadow);
  dc.PatBlt(1, Attr.H-2, Attr.W-1, 2);
  dc.PatBlt(Attr.W-2, 1, 2, Attr.H-2-1);
  }
  catch (TGdiBase::TXGdi& x) {
    MessageBeep(0xFFFFFFFF);
    MessageBeep(MB_ICONASTERISK);
    MessageBeep(MB_ICONEXCLAMATION);
    MessageBeep(MB_ICONHAND);
	 MessageBeep(MB_ICONQUESTION);
    MessageBeep(MB_OK);
     ofstream DebugFile("DebugPicture.dat", ios::app);
     bool found;
     string s=x.ResourceIdToString(&found,x.GetErrorCode());
	  DebugFile<<"GDI error TVisionDialog::EvEraseBkgnd ";
     if (found) DebugFile<<s;
     DebugFile<<endl;
	  DebugFile.close();
     return NULL;
   }
  return TRUE;
}

TSocketManager* gSocketManager;

char *CursorText[7]={{"X=%.3f mm"},{"Y=%.3f mm"},{"H=%.3f"},{"XP=%.3f"},{"DX=%.3f"},{"YP=%.3f"},{"DY=%.3f"}};
char *MessDataText[12]={{"Camera=%.0f"},{"Det=%.2f MHz"},{"TOF=%.2f ms"},{"Camera=%.0f"},
                       {"Trap=%.0f"},{"B=%.2f G"},{"G=%.0f G/cm"},{"C=%.0f G/cm^2"},
                        {"P1=%.4f               "},{"P2=%.4f               "},{"P3=%.4f               "},{"P4=%.4f               "}};
char *GaussFitText[25]={{"Dens=%.2f"},{"X0=%.3f"},{"SX=%.1f"},{"OX=%.2f"},{"F0=%.2E"},
					    {"NI=%.2E "},{"Y0=%.3f"},{"SY=%.1f"},{"OY=%.2f"},{"F1=%.2E"},
						{"NG=%.2E"},{"n0=%.2e cm-3"},{"Fl=%.1f"},{"NOf=%.2f"},{"F2=%.2E"},
                        {"N0=%.2E"},{"N1=%.2E"},{"N2=%.2E"},{"N3=%.2E"},{"F3=%.2E"},
                        {"N4=%.2E"},{"N5=%.2E"},{"N6=%.2E"},{"N7=%.2E"},{"F4=%.2E"}};
                       //{"ReFl=%.0f %"},{"L=%.2f"},{"TF/TC=%.2f  microK"},{"Rx=%.2E"}};
char *TNFitText[12]={{"Sigma0X=%.2E mm"},{"Speed=%.2E m/s"},{"TempX=%.3f microK"},
  {"Sigma0Y=%.2E mm"},{"Gamma=%.2f"},{"TempY=%.3f microK"},{"eta=%.2E"},{"n0=%.2E"},{"r0=%.2E"},
  {"fx=%.1f Hz"},{"fy=%.1f Hz"},{"NT=%.2E"}};
char *Fit2DText[13]={{"X0=%.2f mm"},{"Y0=%.2f mm"},{"Tr=%.3f mK"},{"N=%.2E"},
  				        {"Off=%.2f"},{"SlX=%.2f"},{"SlY=%.2f"},{"phi=%.2f"},{"zCorr=%.2f"},
                    {"n0=%.2E"},{"r0=%.2E"},{"G=%.2f"},{"Ta=%.3f mK"}};
const MeasureButtonNr=120;
char *MeasureTNButtonText[MeasureButtonNr]={{"Sx"},{"Sy"},{"Ax"},{"Ay"},{"OD"},{"NG"},{"NI"},
                               {"NO"},
                               {"N0"},{"N1"},{"N2"},{"N3"},{"N4"},{"N5"},{"N6"},{"N7"},
                               {"F0"},{"F1"},{"F2"},{"F3"},{"F4"},
                               {"O0"},{"O1"},{"O2"},{"O3"},{"O4"},{"O5"},{"O6"},{"O7"},
                               {"X0"},{"Y0"},{"Px"},{"Py"},{"0x"},{"0y"},{"Rx"},{"Ry"},{"n0"},
                               {"RF"},{"N2"},{"Tr"},{"Ta"},{"n0"},{"r0"},{"Ga"},{"X0"},{"Y0"},{"0f"},
                               {"Px"},{"Py"},{"ph"},{"zc"},{"Fl"},{"Lo"}, {"L"},{"Tx"},{"Ty"},{"TF"},
                               {"T7"},{"TT"},{"RF"},{"RR"},{"Ec"},{"et"},{"NR"},{"#A"},{"Av"},{"Of"},
                               {"D0"},{"D1"},{"D2"},{"D3"},{"D4"},{"D5"},{"D6"},{"D7"},{"D8"},{"D9"},
                               {"DA"},{"DB"},{"DC"},{"DD"},{"DE"},{"DF"},{"DG"},{"DH"},{"DI"},{"DJ"},
                               {"DK"},{"DL"},{"DM"},{"DN"},{"DO"},{"DP"},{"DQ"},{"DR"},{"DS"},{"DT"},
                               {"DU"},{"DV"},{"DW"},{"DX"},{"DY"},{"DZ"},{"Da"},{"Db"},{"Dc"},{"Dd"},
                               {"De"},{"Df"},{"Dg"},{"Dh"},{"Di"},{"Dj"},{"Dk"},{"Dl"},{"Dm"},{"Dn"},
                               {"IQ"},{"Me"}};
int MeasureTNButtonList[MeasureButtonNr]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116};
const EichH=0.01;
const EichV=0.01;

TVisionDialog::TVisionDialog(TMessageBar *amb, bool HardwareAccessAllowed)
				 :TWindow(0,0,0)
				 {
  for (int i=0;i<NrROIs*8;i++) ROI[i]=0;  
  for (int i=0;i<NrROIs;i++) ROICam[i]=0;  
//  NetAltaProbeLibrary=NULL;
  RDSetup=new CRDSetup();


  if (!HardwareAccessAllowed) RDSetup->HardwareAccess=false;
  #if defined(DetectLeaks)
    LeakDetect.New(RDSetup,5,2);
  #endif
  gSocketManager = new TSocketManager();
  #if defined(DetectLeaks)
    LeakDetect.New(gSocketManager,5,3);
  #endif
  char szName[128];
  THostInfoManager myHostInfoManager;
  myHostInfoManager.GetHostName(szName, 128);

   #if defined(NetAndorCameraUsed)
   for (int i=0; i<NrNetAndorCameras;i++) {
   //ATTENTION: change the /*RDSetup->HardwareAccess*//(i==0) || (i==2) || (i==3) parameter back to RDSetup->HardwareAccess when all cameras are back
      NetAndorFrameGrabber[i]=new CNetAndorFrameGrabber(this,NetAndorCameraIPAddress[i],NetAndorCameraPort[i],
        RDSetup->HardwareAccess,i);
      #if defined(DetectLeaks)
  		    LeakDetect.New(NetAndorFrameGrabber[i],5,4);
 	   #endif
      //NetAndorFrameGrabber[i]->DebugOn(false);  //FS 2023 10 18 uncomment this line to obtain a camera server communication protocol
   }
	#endif

	Name =new CName(RDSetup);
   #if defined(DetectLeaks)
     LeakDetect.New(Name,5,5);
   #endif
	Name->GetPath(InitialDir);
	strcat(InitialDir,"\\");
	MatrixFileData=new TOpenSaveDialog::TData(OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT |
								  OFN_NOCHANGEDIR | OFN_EXTENSIONDIFFERENT,
								  "Picture (*.DAT) | *.DAT",0,InitialDir);
   #if defined(DetectLeaks)
     LeakDetect.New(MatrixFileData,5,6);
   #endif
  BMPFileData=new TOpenSaveDialog::TData(OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT |
								  OFN_NOCHANGEDIR | OFN_EXTENSIONDIFFERENT,
								  "Picture (*.BMP) | *.BMP",0,InitialDir);
  #if defined(DetectLeaks)
     LeakDetect.New(BMPFileData,5,7);
   #endif
  TGAFileData=new TOpenSaveDialog::TData(OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT |
								  OFN_NOCHANGEDIR | OFN_EXTENSIONDIFFERENT,
								  "Picture (*.TGA) | *.TGA",0,InitialDir);
  #if defined(DetectLeaks)
     LeakDetect.New(TGAFileData,5,8);
   #endif
  SerieFileData=new TOpenSaveDialog::TData(OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT |
								  OFN_NOCHANGEDIR | OFN_EXTENSIONDIFFERENT,
								  "Serie (*.DAT) | *.DAT",0,InitialDir);
  #if defined(DetectLeaks)
     LeakDetect.New(SerieFileData,5,9);
   #endif
  MESFileData=new TOpenSaveDialog::TData(OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT |
								  OFN_NOCHANGEDIR | OFN_EXTENSIONDIFFERENT,
								  "Measurement | ??????A?.TGA",0,InitialDir);
  #if defined(DetectLeaks)
     LeakDetect.New(MESFileData,5,10);
  #endif
  ROIFileData=new TOpenSaveDialog::TData(OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT |
								  OFN_NOCHANGEDIR | OFN_EXTENSIONDIFFERENT,
								  "ROI | *.ROI",0,InitialDir);
  #if defined(DetectLeaks)
     LeakDetect.New(ROIFileData,5,10);
  #endif
  ProfilFileData=new TOpenSaveDialog::TData(OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT |
								  OFN_NOCHANGEDIR | OFN_EXTENSIONDIFFERENT,
								  "Profil (*.DAT) | *.DAT",0,InitialDir);
  #if defined(DetectLeaks)
     LeakDetect.New(ProfilFileData,5,11);
  #endif
  AbsPictureTimerSet=false;
  TempBoson=0;
  FitFermiHorizontal=true;
  FitButtonFunction=0;
  FitPartialFermiL=1;
  FitFermiZeroTemp=0;
  FitPartialFermiTemp=0;
  FitPartialFermiNumber=0;
  CursorPosX=0;
  CursorPosY=0;
  Update2DFitDrawing=false;
  ParamsInitialised=false;
  ParamsJustLoaded=true;
  ParamList=NULL;
  InitialiseIoffeFit=0;
	BiningX=1;
  BiningY=1;
  FKBining=4;
  FastKinetics=0;
  FKSDistance=256;
  FKSHeight=256;
  FKSOffset=0;
  Display2DFitProfil=true;
  DisplayMathErrors=false;
  SetMathErrorMessage(DisplayMathErrors);
  Fit2DOnlyNotFitted=true;
  UpdateMeasurementAfterFit=true;
  MetaPointer=&((CMyPoint *)(MD));
  MD=new CMessPoint();
  #if defined(DetectLeaks)
     LeakDetect.New(MD,5,12);
  #endif
  for (int i=0;i<MaxThisRunMD;i++) ThisRunMD[i]=NULL;
  MD->SetMetaPointer(MetaPointer);
  MDSetCameraParameters();
  Fit2DParameters[0]=0/1000; // X0 [m]
  Fit2DParameters[1]=0/1000; // Y0 [m]
  Fit2DParameters[2]=3E-3;  // Temp [K]
  Fit2DParameters[3]=1E8;  // N [atoms]
  Fit2DParameters[4]=0;     // Offset
  Fit2DParameters[5]=0;     // SlopeX
  Fit2DParameters[6]=0;     // SlopeY
  Fit2DParameters[7]=CameraInclination/360.0*2*M_PI;     // Angle
  Fit2DParameters[8]=1;     // z Flight correction */
  ProfilDesign=false;
  DoIntensityCompensation=false;
  InTimer=false;
  IsGridOnOff=false;
  IntegralWithOffset=true;
  FitGaussWithOffset=true;
  FitWithNumberRatio=false;
  FitGaussWithSlope=false;
  Fit1DWithAmplitude=true;
  Fit1DWithWidth=true;
  Fit2DWithOffset=true;
  Fit2DWithSlope=true;
  Fit2DWithAngle=false;
  Fit2DWithLongitudinalFlightCorrection=true;
  OptDensSource=0;
  OptDensMethod=InitialOptDensMethod;
//  FinalPictureName[0]=0;
  MainPictureMode=4;
  LiveConnected=false;
  LivePicName[0]=0;
  LiveSerieName[0]=0;
  LiveNewPicture=false;
  LiveNewSerie=false;
  LiveStopSerie=false;
  IteratePictureOccupation=0;
//  Fluorescence=0;
  TempTextSet=true;
  MaxOptDensity=MaxOptDensity7Li;
//  ExpansionTime=0;
//  Calibration=1;
  AktCameraTyp=0;
  AktCameraPosition=0;
  CheckNumber=0;
//  Isotop=7;
  CursorFixed=0;
	FixedX0=0;
  FixedY0=0;
  AktZoomMode=1;
  CloseVisionFlag=false;
  if (RDSetup->HardwareAccess) {
		if (ControlComputerSerial) serial=new CSerial();
      else {
      	serial=new CSerialNet(this,ControlComputerPort,amb,&CloseVisionFlag);
         if (DebugControlComm) serial->DebugOn("DebugControlComm.dat",false);  //comment out in mormal use
      }
      #if defined(DetectLeaks)
        LeakDetect.New(serial,5,13);
      #endif
  } else serial=NULL;
  checkingSerial=0;
  autofit=1;
  mb=amb;
  filming=0;
  FilmCameraNr=0;
  TakingPicture=0;
  ColorTable=new CColorTable(this,R+PH+D+SB+D2+WH+D2,R,BB,WV);
  #if defined(DetectLeaks)
     LeakDetect.New(ColorTable,5,14);
  #endif
  probenoisepicture=new CPicture(1,0);
  #if defined(DetectLeaks)
     LeakDetect.New(probenoisepicture,5,15);
  #endif
  abspicture=new CPicture(EichHeight,EichOffset);
  #if defined(DetectLeaks)
     LeakDetect.New(abspicture,5,16);
  #endif
  pictureprobe=new CPicture(1,0);
  AktAdditionalProbe=0;
  for (int i=0;i<MaxAdditionalProbe;i++) pictureAdditionalprobe[i]=NULL;
  #if defined(DetectLeaks)
     LeakDetect.New(pictureprobe,5,17);
  #endif
  picturenoiseabs=new CPicture(1,0);
  #if defined(DetectLeaks)
     LeakDetect.New(picturenoiseabs,5,18);
  #endif
  picturenoiseprobe=new CPicture(1,0);
  #if defined(DetectLeaks)
     LeakDetect.New(picturenoiseprobe,5,19);
  #endif
  pictureabs=new CPicture(1,0);
  #if defined(DetectLeaks)
     LeakDetect.New(pictureabs,5,20);
  #endif
  for (int i=0;i<NrPictureLists*MaxPictureLines;i++) {
    MainPicture[i]=new CPicture(1,0);
    #if defined(DetectLeaks)
      LeakDetect.New(MainPicture[i],5,21);
    #endif
   }
  Epictureabs=NULL;
  Epictureprobe=NULL;
  Epicturenoiseabs=NULL;
  Epicturenoiseprobe=NULL;
  opticalDensity=new CPicture(MaxOptDensity,EichOffset);
  #if defined(DetectLeaks)
      LeakDetect.New(opticalDensity,5,22);
  #endif
  AveragePicNr=0;
  Averagepictureabs=NULL;
  Averagepictureprobe=NULL;
  Averagepicturenoiseabs=NULL;
  Averagepicturenoiseprobe=NULL;
  Averageprobenoisepicture=NULL;

  int Fit2DX=X1;
  int Fit2DY=1500;//Y2+GTextBoxH+D2+MTextBoxH+D2;
  int X5=X1+BoxWide+D2+PBx+2*(PBx+D2);
  int Y5=1500;//Y2+GTextBoxH+D2+MTextBoxH+D2-20;
  const D5=18;

  Fit2DTextBox=new CTextBox(this,Fit2DX,Fit2DY,BoxWide+D2+PBx+2*(PBx+D2),TTextBoxH,TextBox4WidthSmall,TextBoxHeight,"2D Fit Result",4,4,13,Fit2DText,false);
  #if defined(DetectLeaks)
      LeakDetect.New(Fit2DTextBox,5,23);
  #endif
  GaussFitTextBox=new CTextBox(this,R,Y2+MTextBoxH+D2,TBW,GTextBoxH,TextBox4Width,TextBoxHeight,"Gauss and Integral Fit Result",5,5,25,GaussFitText);
  #if defined(DetectLeaks)
      LeakDetect.New(GaussFitTextBox,5,24);
  #endif
  TNFitTextBox=new CTextBox(this,R,Y2+GTextBoxH+D2+MTextBoxH+D2,TBW,TTextBoxH,TextBox3Width,TextBoxHeight,"Temperature and Atom Number Fit Result",3,4,12,TNFitText);
  #if defined(DetectLeaks)
      LeakDetect.New(TNFitTextBox,5,25);
  #endif
  MessDataTextBox=new CTextBox(this,R,Y2,TBW,MTextBoxH,TextBox4Width,TextBoxHeight,"Measurement Parameters",4,3,12,MessDataText);
  #if defined(DetectLeaks)
      LeakDetect.New(MessDataTextBox,5,26);
  #endif
  AxText=new TStatic(this,0,"",DX0,Y1+PH2-15,PBx+2*(PBx+D2)-3*13-5,15);
  #if defined(DetectLeaks)
      LeakDetect.New(AxText,5,27);
  #endif
  CameraButton =new CButtonBox(this,ID_CAMERABUTTON,DX0,R+WV+D,PBx+2*(PBx+D2),SH,NrCameraButtons,1,NrCameraButtons,CameraButtonText,CameraButtonList,false);
  #if defined(DetectLeaks)
      LeakDetect.New(CameraButton,5,28);
  #endif
  MeasureTNButton =new CButtonBox(this,ID_MEASURETNBUTTON,X1,Y1,BoxWide,PH2,11,4,MeasureButtonNr,MeasureTNButtonText,MeasureTNButtonList,false);
  #if defined(DetectLeaks)
      LeakDetect.New(MeasureTNButton,5,29);
  #endif
  TNmeasure=new CTNMeasure(this,Name,mb,MeasureTNButton,CameraButton,AxText,MessDataTextBox,TNFitTextBox,GaussFitTextBox,DX0,Y1,PBx+2*(PBx+D2),PH2-15);
  #if defined(DetectLeaks)
      LeakDetect.New(TNmeasure,5,30);
  #endif
  for (int i=0;i<NrDetailPictureButtons;i++) {
      PictureOfDetailButton[i]=NULL;
  		int x=i%NrDetailPictureButtonX;
	   int y=i/NrDetailPictureButtonX;
      DetailPictureButtonTitle[i]=new TStatic(this,ID_DETAILPICTUREBUTTON0+i,DetailPictureButtonText[StartDetailButtonChecked][i],DX0+x*(PBx+D2),R+y*(PBy+D3+TH),PBx,TH);
      #if defined(DetectLeaks)
        LeakDetect.New(DetailPictureButtonTitle[i],5,32);
      #endif
	   DetailPictureButton[i]=new CPictureButton(this,mb,DX0+x*(PBx+D2),R+TH+y*(PBy+D3+TH),PBx,PBy,&PictureOfDetailButton[i],ColorTable,ID_DETAILPICTUREBUTTON0+i,0);
      #if defined(DetectLeaks)
        LeakDetect.New(DetailPictureButton[i],5,33);
      #endif
  }
  MovePicturesToDetailPictureButtons(0);
  picture=PictureOfDetailButton[0];
  int x=0;
  int y=2;
  DetailButton =new CButtonBox(this,ID_DETAILBUTTON,DX0+x*(PBx+D2),R+y*(PBy+D3+TH),PBx+2*(PBx+D2),SH,NrDetailButtons,1,NrDetailButtons,DetailButtonText,DetailButtonList,true);
  #if defined(DetectLeaks)
     LeakDetect.New(DetailButton,5,34);
  #endif
  CScale *help=new CScale(this,R+PH+D,R,SB,WV,Slash,Text,0,1,-0.5,2,1,-EichV,-WV*EichV/2.0,1.5);
  help=new CScale(this,R+PH+D+SB+D2,R+WV+D,WH,SH,Slash,Text,1,1,0.5,2,-1,EichH,-WH*EichH/2.0,-1.5);
  help=new CScale(this,R,R+WV+D,PH,SH,Slash,Text,1,1,-50,2,-1,-2,255,250);
  help=new CScale(this,R+PH+D,R+WV+D+SH+D2,SB,PH,Slash,Text,0,0,-50,2,-1,-2,255,250);
  help=new CScale(this,R+PH+D+SB+D2+WH+D2+BB+D,R,SB,WV,Slash,Text,0,1,-50,2,-1,-256.0/WV,255,250);
  profilH=new CProfil(this,R+PH+D+SB+D2,R+WV+D+SH+D2,WH,PH,1,WH,WV,&picture,ColorTable);
  #if defined(DetectLeaks)
     LeakDetect.New(profilH,5,40);
  #endif
  profilV=new CProfil(this,R,R,PH,WV,0,WH,WV,&picture,ColorTable);
  #if defined(DetectLeaks)
     LeakDetect.New(profilV,5,41);
  #endif
  PictureWin=new CPictureProfilWindow(this,mb,R+PH+D+SB+D2,R,WH,WV,&picture,profilH,profilV,ColorTable);
  #if defined(DetectLeaks)
     LeakDetect.New(PictureWin,5,42);
  #endif
  BkBrush =new TBrush(::GetSysColor(COLOR_BTNFACE));
  #if defined(DetectLeaks)
     LeakDetect.New(BkBrush,5,43);
  #endif

  CursorTextBox=new CTextBox(this,R,R+WV+D+SH+D2,PH,PH,TextBox3Width,TextBoxHeight,"Cursor Position",1,7,7,CursorText);
  #if defined(DetectLeaks)
     LeakDetect.New(CursorTextBox,5,44);
  #endif
  FinalTrapText=new TStatic(this,0,"an atoms life",R+4,Y2+2+TextBoxHeight*4,TBW-8,15);
  #if defined(DetectLeaks)
     LeakDetect.New(FinalTrapText,5,45);
  #endif
  TButton* help2=new TButton(this, ID_OFFSETUP, "<",  X5-22*D5,Y5, 13, 13);
  help2=new TButton(this, ID_OFFSETDOWN, ">", X5-21*D5,Y5, 13, 13);

  help2=new TButton(this, ID_LINEUP, "^",  X5-20*D5,Y5, 13, 13);
  help2=new TButton(this, ID_LINEDOWN, "v", X5-19*D5,Y5, 13, 13);
  new TStatic(this,0,"N",               X5-18*D5,Y5,15,15);
  new TButton(this, ID_2DNBACK, "<",    X5-17*D5,Y5, 13, 13);
  new TButton(this, ID_2DNFORWARD, ">", X5-16*D5,Y5, 13, 13);
  new TStatic(this,0,"T",               X5-15*D5,Y5,15,15);
  new TButton(this, ID_2DTBACK, "<",    X5-14*D5,Y5, 13, 13);
  new TButton(this, ID_2DTFORWARD, ">", X5-13*D5,Y5, 13, 13);
  new TStatic(this,0,"0",               X5-12*D5,Y5,15,15);
  new TButton(this, ID_2DOBACK, "<",    X5-11*D5,Y5, 13, 13);
  new TButton(this, ID_2DOFORWARD, ">", X5-10*D5,Y5, 13, 13);
  new TStatic(this,0,"X",               X5-9*D5,Y5,15,15);
  new TButton(this, ID_2DSXBACK, "<",   X5-8*D5,Y5, 13, 13);
  new TButton(this, ID_2DSXFORWARD, ">",X5-7*D5,Y5, 13, 13);
  new TStatic(this,0,"Y",               X5-6*D5,Y5,15,15);
  new TButton(this, ID_2DSYBACK, "<",   X5-5*D5,Y5, 13, 13);
	new TButton(this, ID_2DSYFORWARD, ">",X5-4*D5,Y5, 13, 13);
  new TStatic(this,0,"Z",               X5-3*D5,Y5,15,15);
  new TButton(this, ID_2DSZBACK, "<",   X5-2*D5,Y5, 13, 13);
  new TButton(this, ID_2DSZFORWARD, ">",X5-1*D5,Y5, 13, 13);

  new TButton(this, ID_TNBACK, "<", X1+10, Y1+PH2-18, 13, 13);
  new TButton(this, ID_TNFORWARD, ">", X1+30, Y1+PH2-18, 13, 13);

  new TButton(this, ID_TNMESSFORWARD, "<", DX0+PBx+2*(PBx+D2)-3*13-3,Y1+PH2-15+3, 13, 13);
  new TButton(this, ID_TNMESSBACK, ">", DX0+PBx+2*(PBx+D2)-1*13-3,Y1+PH2-15+3, 13, 13);

  LogbookEdit=new TEdit(this,ID_LOGBOOKEDIT,"",X1,Y1+D2+PH2,PBx+2*(PBx+D2)+DX0-X1,PH3,0,true);
  #if defined(DetectLeaks)
     LeakDetect.New(LogbookEdit,5,50);
  #endif
  for (int i=0;i<NrPictureLists;i++) {
	  PictureListNamePointer[i]=new TStatic(this,0,PictureListName[i],DX0+3*(PBx+D2)+i*(PBlx+D2)+10,R,PBly+D2,15);
     #if defined(DetectLeaks)
       LeakDetect.New(PictureListNamePointer[i],5,51);
     #endif
	  PictureList[i]=new CPictureList(this,mb,ID_PICLIST0+i,DX0+3*(PBx+D2)+i*(PBlx+D2),R+TH,PBlx+D2,PBly+D2,PictureListHeight,PictureListColumns,abspicture,ColorTable,PictureList,NrPictureLists);
     #if defined(DetectLeaks)
       LeakDetect.New(PictureList[i],5,52);
     #endif
  }
  mb->SetText("System ready");
  if (RDSetup->HardwareAccess) SetTimer(ID_TESTSERIALTIMER,testserialtime);
  SerialTimerActive=true;
  InSerie=0;
  CheckDiskSpace();
  /*
  char Filename[300];
  char DataFilename[300];
  Name->GetSerieName(Filename);
  strcpy(DataFilename,Filename);
  strcat(DataFilename,"DataP-7.dat");
  LoadSerie(DataFilename);
  strcpy(DataFilename,Filename);
  strcat(DataFilename,"DataP-6.dat");
  TNmeasure->Load(DataFilename,1);
  */
}

bool TVisionDialog::CanClose() {
  SaveParams();
  bool canclose=true;
  #if defined(AltaCameraUsed)
    if (AltaFrameGrabber) canclose=canclose && AltaFrameGrabber->CanClose();
  #endif
  #if defined(ApogeeCameraUsed)
    if (ApogeeFrameGrabber) canclose=canclose && ApogeeFrameGrabber->CanClose();
  #endif
  return canclose;
}

bool TVisionDialog::Close() {
  if (SerialTimerActive) KillTimer(ID_TESTSERIALTIMER);
  SerialTimerActive=false;
  if ((MD) && (!MD->IsInList())) {
    #if defined(DetectLeaks)
       LeakDetect.Delete(MD);
    #endif
    delete MD;
  }
  if (gSocketManager) delete gSocketManager;
  #if defined(DetectLeaks)
    LeakDetect.Delete(gSocketManager);
  #endif
/*  if (NetAltaProbeLibrary) {
    NetAltaProbeLibrary->SaveProbeLibrary(ColorTable);
    #if defined(DetectLeaks)
      LeakDetect.Delete(NetAltaProbeLibrary);
    #endif
    delete NetAltaProbeLibrary;
  }*/
  #if defined(DetectLeaks)
    LeakDetect.Delete(ColorTable);
  #endif
  if (ColorTable) delete ColorTable;
  #if defined(DetectLeaks)
    LeakDetect.Delete(serial);
  #endif
  if (serial) delete serial;
  #if defined(DetectLeaks)
    LeakDetect.Delete(Name);
  #endif
  if (Name) delete Name;
  #if defined(DetectLeaks)
    LeakDetect.Delete(probenoisepicture);
  #endif
  if (probenoisepicture) delete	probenoisepicture;
  #if defined(DetectLeaks)
    LeakDetect.Delete(abspicture);
  #endif
  if (abspicture) delete abspicture;
  #if defined(DetectLeaks)
    LeakDetect.Delete(pictureprobe);
  #endif
  if (pictureprobe) delete pictureprobe;
  for (int i=0;i<MaxAdditionalProbe;i++) if (pictureAdditionalprobe[i]) delete pictureAdditionalprobe[i];
  #if defined(DetectLeaks)
    LeakDetect.Delete(picturenoiseabs);
  #endif
  if (picturenoiseabs) delete picturenoiseabs;
  #if defined(DetectLeaks)
    LeakDetect.Delete(picturenoiseprobe);
  #endif
  if (picturenoiseprobe) delete picturenoiseprobe;
  #if defined(DetectLeaks)
    LeakDetect.Delete(pictureabs);
  #endif
  if (pictureabs) delete pictureabs;
  #if defined(DetectLeaks)
    LeakDetect.Delete(opticalDensity);
  #endif
  if (opticalDensity) delete opticalDensity;
  //if (NetPrincetonFrameGrabber) delete NetPrincetonFrameGrabber;
  //if (NetAltaFrameGrabber) delete NetAltaFrameGrabber;
  for (int i=0; i<NrNetAndorCameras;i++) if (NetAndorFrameGrabber[i]) {
    #if defined(DetectLeaks)
      LeakDetect.Delete(NetAndorFrameGrabber[i]);
    #endif
    delete NetAndorFrameGrabber[i];
  }
  //if (SingleAtomDetectionFrameGrabber) delete SingleAtomDetectionFrameGrabber;
  //if (HamamatsuFrameGrabber) delete HamamatsuFrameGrabber;
  #if defined(AltaCameraUsed)
   #if defined(DetectLeaks)
      LeakDetect.Delete(AltaFrameGrabber);
    #endif
	 if (AltaFrameGrabber) delete AltaFrameGrabber;
  #endif
  //if (AndorFrameGrabber) delete AndorFrameGrabber;
  #if defined(ApogeeCameraUsed)
    #if defined(DetectLeaks)
      LeakDetect.Delete(ApogeeFrameGrabber);
    #endif
    if (ApogeeFrameGrabber) delete ApogeeFrameGrabber;
  #endif
  #if defined(DetectLeaks)
     LeakDetect.Delete(MatrixFileData);
  #endif
  if (MatrixFileData) delete MatrixFileData;
  #if defined(DetectLeaks)
     LeakDetect.Delete(BMPFileData);
  #endif
  if (BMPFileData) delete BMPFileData;
  #if defined(DetectLeaks)
     LeakDetect.Delete(TGAFileData);
  #endif
  if (TGAFileData) delete TGAFileData;
  #if defined(DetectLeaks)
     LeakDetect.Delete(SerieFileData);
  #endif
  if (SerieFileData) delete SerieFileData;
  #if defined(DetectLeaks)
     LeakDetect.Delete(MESFileData);
  #endif
  if (MESFileData) delete MESFileData;
  #if defined(DetectLeaks)
     LeakDetect.Delete(ROIFileData);
  #endif
  if (ROIFileData) delete ROIFileData;
  #if defined(DetectLeaks)
     LeakDetect.Delete(ProfilFileData);
  #endif
  if (ProfilFileData) delete ProfilFileData;
  #if defined(DetectLeaks)
     LeakDetect.Delete(RDSetup);
  #endif
  if (RDSetup) delete RDSetup;
  for (int i=0;i<NrPictureLists*MaxPictureLines;i++) if (MainPicture[i]) {
    #if defined(DetectLeaks)
      LeakDetect.Delete(MainPicture[i]);
    #endif
    delete MainPicture[i];
  }
  for (int i=0;i<NrDetailPictureButtons;i++) {
      #if defined(DetectLeaks)
        LeakDetect.Delete(DetailPictureButtonTitle[i]);
      #endif
      if (DetailPictureButtonTitle[i]) delete DetailPictureButtonTitle[i];
      #if defined(DetectLeaks)
        LeakDetect.Delete(DetailPictureButton[i]);
      #endif
	   if (DetailPictureButton[i]) delete DetailPictureButton[i];
  }
  for (int i=0;i<NrPictureLists;i++) {
     #if defined(DetectLeaks)
       LeakDetect.Delete(PictureListNamePointer[i]);
     #endif
     delete PictureListNamePointer[i];
     #if defined(DetectLeaks)
       LeakDetect.Delete(PictureList[i]);
     #endif
	  delete PictureList[i];
  }
  #if defined(DetectLeaks)
      LeakDetect.Delete(Fit2DTextBox);
  #endif
  delete Fit2DTextBox;
  #if defined(DetectLeaks)
      LeakDetect.Delete(GaussFitTextBox);
  #endif
  delete GaussFitTextBox;
  #if defined(DetectLeaks)
      LeakDetect.Delete(TNFitTextBox);
  #endif
  delete TNFitTextBox;
  #if defined(DetectLeaks)
      LeakDetect.Delete(MessDataTextBox);
  #endif
  delete MessDataTextBox;
  #if defined(DetectLeaks)
      LeakDetect.Delete(AxText);
  #endif
  delete AxText;
  #if defined(DetectLeaks)
      LeakDetect.Delete(CameraButton);
  #endif
  delete CameraButton;
  #if defined(DetectLeaks)
      LeakDetect.Delete(MeasureTNButton);
  #endif
  delete MeasureTNButton;
  #if defined(DetectLeaks)
      LeakDetect.Delete(TNmeasure);
  #endif
  delete TNmeasure;
  #if defined(DetectLeaks)
     LeakDetect.Delete(DetailButton);
  #endif
  delete DetailButton;
  #if defined(DetectLeaks)
     LeakDetect.Delete(profilH);
  #endif
  delete profilH;
  #if defined(DetectLeaks)
     LeakDetect.Delete(profilV);
  #endif
  delete profilV;
  #if defined(DetectLeaks)
     LeakDetect.Delete(PictureWin);
  #endif
  delete PictureWin;
  #if defined(DetectLeaks)
     LeakDetect.Delete(BkBrush);
  #endif
  delete BkBrush;
  #if defined(DetectLeaks)
     LeakDetect.Delete(CursorTextBox);
  #endif
  delete CursorTextBox;
  #if defined(DetectLeaks)
     LeakDetect.Delete(FinalTrapText);
  #endif
  delete FinalTrapText;
  #if defined(DetectLeaks)
     LeakDetect.Delete(LogbookEdit);
  #endif
  delete LogbookEdit;
  #if defined(DetectLeaks)
    LeakDetect.Delete(ParamList);
  #endif
  if (ParamList) delete ParamList;
/*  #if defined(DetectLeaks)
    LeakDetect.Delete(MetaPointer);
  #endif
  if (MetaPointer) delete MetaPointer;*/
  SigmaXReference=0;
  SigmaYReference=0;
}

TVisionDialog::~TVisionDialog() {
  Close();
}

void TVisionDialog::SaveParams() {
  double posx1o,posx2o,posy1o,posy2o;
  double posx1i,posx2i,posy1i,posy2i;
  profilH->GetPos(posx1o,posx2o,1);
  profilV->GetPos(posy1o,posy2o,1);
  profilH->GetPos(posx1i,posx2i,0);
  profilV->GetPos(posy1i,posy2i,0);
  int ColorTableNr=ColorTable->GetColorTableNr();
  double CPos1;
  double CPos2;
  ColorTable->GetPos(CPos1,CPos2);
  ofstream out(ParamFileName);
  out<<posx1o<<endl;
  out<<posx2o<<endl;
  out<<posy1o<<endl;
  out<<posy2o<<endl;
  out<<posx1i<<endl;
  out<<posx2i<<endl;
  out<<posy1i<<endl;
  out<<posy2i<<endl;
  for (int i=0;i<NrROIs*8;i++) out<<ROI[i]<<endl;
  for (int i=0;i<NrROIs;i++) out<<ROICam[i]<<endl;
  out<<((DoIntensityCompensation) ? 1 : 0)<<endl;
  out<<ColorTableNr<<endl;
  out<<CPos1<<endl;
  out<<CPos2<<endl;
  if (DetailButton) DetailButton->SaveCheckedToStream(out);
  if (CameraButton) CameraButton->SaveCheckedToStream(out);
  if (MeasureTNButton) MeasureTNButton->SaveCheckedToStream(out);
  out<<AktZoomMode<<endl;
  out<<((ProfilDesign) ? 1 : 0)<<endl;
  out<<((CursorFixed) ? 1 : 0)<<endl;
  out<<FixedX0<<endl;
  out<<FixedY0<<endl;
  out<<MainPictureMode<<endl;
  out<<SigmaXReference<<endl;
  out<<SigmaYReference<<endl;
  out.close();
}

void TVisionDialog::LoadParams() {
  struct ffblk ffblk;
  if (findfirst(ParamFileName,&ffblk,0)==0) {
	  double posx1o,posx2o,posy1o,posy2o;
	  double posx1i,posx2i,posy1i,posy2i;
	  ifstream in(ParamFileName);
	  in>>posx1o;
	  in>>posx2o;
	  in>>posy1o;
	  in>>posy2o;
	  in>>posx1i;
	  in>>posx2i;
	  in>>posy1i;
	  in>>posy2i;
	  for (int i=0;i<NrROIs*8;i++) in>>ROI[i];	  
	  for (int i=0;i<NrROIs;i++) in>>ROICam[i];
	  int help;
      in>>help;
	  DoIntensityCompensation=(help==1);
	  int ColorTableNr;
	  in>>ColorTableNr;
     double CPos1,CPos2;
     in>>CPos1;
     in>>CPos2;
     if (DetailButton) DetailButton->LoadCheckedFromStream(in);
     if (CameraButton) CameraButton->LoadCheckedFromStream(in);
     if (MeasureTNButton) MeasureTNButton->LoadCheckedFromStream(in);
     in>>AktZoomMode;
     in>>help;
     if (help==1) ToggleProfilDesign();
     in>>help;
     in>>CursorPosX;
     in>>CursorPosY;
     if (help==1) FixCursor();
	  profilH->SetPos(posx1o,posx2o,1);
	  profilV->SetPos(posy1o,posy2o,1);
	  profilH->SetPos(posx1i,posx2i,0);
	  profilV->SetPos(posy1i,posy2i,0);
     ColorTable->NewPos(CPos1,CPos2);
	  ColorTable->SetColorTableNr(ColorTableNr);
     in>>MainPictureMode;
	 in>>SigmaXReference;
	 in>>SigmaYReference;
     in.close();
  } else {
    char buff[400];
        sprintf(buff,"TVisionDialog::LoadParams: Parameter file not found (%s)\nA new one will be created",ParamFileName);
        MessageBox(buff,"Information",  MB_OK);
      
  }
}

void TVisionDialog::StoreReferenceSize() {
	SigmaXReference=MD->y("SigmaX");
	SigmaYReference=MD->y("SigmaY");
	mb->SetText("Size reference stored.");
}

void TVisionDialog::SetupWindow() {
  TWindow::SetupWindow();
  if (MeasureTNButton) MeasureTNButton->SetCheck(0,BF_CHECKED);
  if (CameraButton) {
    for (int i=0;i<NrCameraButtons;i++) CameraButton->SetCheck(i,BF_CHECKED);
  }
  if (DetailButton) {
    DetailButton->SetCheck(StartDetailButtonChecked,BF_CHECKED);
  }
  LoadParams();
  UpdateAll();
}

void TVisionDialog::CeZoomIn2(TCommandEnabler& ce)
{
  ce.SetCheck((AktZoomMode==2) ? TCommandEnabler::Checked : TCommandEnabler::Unchecked);
}

void TVisionDialog::CeZoomIn4(TCommandEnabler& ce)
{
  ce.SetCheck((AktZoomMode==4) ? TCommandEnabler::Checked : TCommandEnabler::Unchecked);
}

void TVisionDialog::ZoomIn2() {
  if (AktZoomMode==2) AktZoomMode=1;
  else AktZoomMode=2;
  UpdateAll();
  for (int i=0;i<NrPictureLists;i++) PictureList[i]->Update();
}

void TVisionDialog::ZoomIn4() {
  if (AktZoomMode==4) AktZoomMode=1;
  else AktZoomMode=4;
  UpdateAll();
  for (int i=0;i<NrPictureLists;i++) PictureList[i]->Update();
}

void TVisionDialog::GetZoomParameters(int &Zoom, double &ZoomCenterX, double &ZoomCenterY) {
  Zoom=AktZoomMode;
  ZoomCenterX=CursorPosX;
  ZoomCenterY=CursorPosY;
}

DWORD TVisionDialog::GetFreeMem() {
/*typedef struct _MEMORYSTATUS { // mst
    DWORD dwLength;        // sizeof(MEMORYSTATUS)
    DWORD dwMemoryLoad;    // percent of memory in use
    DWORD dwTotalPhys;     // bytes of physical memory
    DWORD dwAvailPhys;     // free physical memory bytes
    DWORD dwTotalPageFile; // bytes of paging file
    DWORD dwAvailPageFile; // free bytes of paging file
    DWORD dwTotalVirtual;  // user bytes of address space
    DWORD dwAvailVirtual;  // free user bytes
} MEMORYSTATUS, *LPMEMORYSTATUS; */
 LPMEMORYSTATUS  mem;
 mem=new MEMORYSTATUS;
 GlobalMemoryStatus(mem);
 DWORD FreeMem=mem->dwAvailPhys;
 delete mem;
 return FreeMem;
 }

 void TVisionDialog::About() {
  char buf[400];
  sprintf(buf,"\nVisionSrBEC 3.0\n\nFlorian  Schreck\n\nLKB, UT Austin, IQOQI\n\nfree Memory:      %u\nfree disk space:  %ld",GetFreeMem(),Name->GetFreeDiskSpace());
  MessageBox(buf,"About");

}

void TVisionDialog::CheckDiskSpace() {
  double FreeSpace=Name->GetFreeDiskSpace();
  if (FreeSpace<(50*1024*1024)) {
	 char buf[400];
	 sprintf(buf,"\n    Disk space low !\n    free disk space:  %f",FreeSpace);
	 MessageBox(buf,"Warning");
  }
}

void TVisionDialog::NetAndorPicture(int CameraUsed) {
  if (CameraUsed<0) return;
  if (CameraUsed>=NrNetAndorCameras) return;
	if (NetAndorFrameGrabber[CameraUsed]) {
	  picture=probenoisepicture;
	  mb->SetText("Taking Net Andor Picture");
	  double posx1,posx2,posy1,posy2;
	  profilH->GetPos(posx1,posx2);
	  profilV->GetPos(posy1,posy2);
     NetAndorFrameGrabber[CameraUsed]->SetImageParameters(
       /*CameraDirection*/"X",/*CameraUsed*/true,/*PictureNumber*/0,/*MainCamera*/1,/*hbin*/ 1,/*vbin*/ 1,/*xstart*/ 0,
       /*ystart*/0, /*width*/ TestCameraWidth,/*height*/ TestCameraHeight,/*ExposureTime*/ 500,/*Temperature*/ 20,
       /*ExternalTrigger*/ (TestCameraExternalTrigger) ? 1 : 0, /*vSpeed*/ 16, /*hSpeed*/ 1, /*FKSMode*/ 0,
       /*FKSHeight*/ 341,/*FKSDataImages*/ 1,/*FKSReferenceImages*/ 1,/*FKSvSpeed*/ 2,
       /*FKSExternalTriggerStart,*/ 1,/*FKSDirtyLines*/ 0, /*NumberPictures*/ 1,/* aPrePicture*/false,
       /*FlushWait*/ 100, /*DoInternalTriggerAfterExternalTrigger*/false,/*TriggerDelay*/ 1,/*AtomicMass*/ 6,
       /*AtomicWavelength*/461E-9, /*AtomicClebschGordon*/ 1, /*AtomicLinewidth*/6E6,
       /*AtomicMagneticMoment*/1,/*AtomicName*/"88Sr",
       /*ProbeDetuning*/0, /*ExpansionTime*/0,/*FluorescencePicture*/false,
       /*MaxFluorescence*/4000,/*Calibration*/1.0,
       /*FinalTrap*/ "MOT", /*EndState*/"MOT",/*EichX*/1,/*EichY*/1,0,0);
	  if (!NetAndorFrameGrabber[CameraUsed]->TakeTriggeredPicture()) {
   	 MessageBox("NetAndor: couldn't trigger!","Error");
	  };

   //NetAndorFrameGrabber[CameraUsed]->TakeTriggeredPicture();
   DWORD Start=GetTickCount();
  // while ((GetTickCount()-Start)<1000);
   bool CameraNetworkOk=NetAndorFrameGrabber[CameraUsed]->CheckReady(100000);
   CameraNetworkOk=CameraNetworkOk && NetAndorFrameGrabber[CameraUsed]->TestReady();
   NetAndorFrameGrabber[CameraUsed]->TransferPictureData(false);
   NetAndorFrameGrabber[CameraUsed]->TestReady();
   picture->ReadPicture(NetAndorFrameGrabber[CameraUsed],APposx1,APposx2,APposy1,APposy2,0);
	UpdateAll();

    /* NetAndorFrameGrabber[0]->CheckReady(100000);
     NetAndorFrameGrabber[0]->TransferPictureData(true);
	  picture->ReadPicture(NetAndorFrameGrabber[0],posx1,posx2,posy1,posy2,0);
	  mb->SetText("");
	  PictureWin->Update(MD->y("ProfilAverage"));
     UpdateAll();
	  MessDataTextBox->SetValue(0,2);    */
  }
}

void TVisionDialog::FilmCamera(int Nr) {
	StartFilming(Nr);
}

void TVisionDialog::PictureCamera(int Nr) {
  NetAndorPicture(Nr);
}

void TVisionDialog::PictureCamera0() {PictureCamera(0);}
void TVisionDialog::PictureCamera1() {PictureCamera(1);}
void TVisionDialog::PictureCamera2() {PictureCamera(2);}
void TVisionDialog::PictureCamera3() {PictureCamera(3);}
void TVisionDialog::PictureCamera4() {PictureCamera(4);}
void TVisionDialog::PictureCamera5() {PictureCamera(5);}
//void TVisionDialog::PictureCamera6() {PictureCamera(6);}
//void TVisionDialog::PictureCamera7() {PictureCamera(7);}

void TVisionDialog::FilmCamera0() {FilmCamera(0);}
void TVisionDialog::FilmCamera1() {FilmCamera(1);}
void TVisionDialog::FilmCamera2() {FilmCamera(2);}
void TVisionDialog::FilmCamera3() {FilmCamera(3);}
void TVisionDialog::FilmCamera4() {FilmCamera(4);}
void TVisionDialog::FilmCamera5() {FilmCamera(5);}
//void TVisionDialog::FilmCamera6() {FilmCamera(6);}
//void TVisionDialog::FilmCamera7() {FilmCamera(7);}

void TVisionDialog::NetAndorTest() {
    NetAndorFrameGrabber[0]->SetImageParameters(/*CameraDirection*/"X",/*CameraUsed*/true,/*PictureNumber*/0,/*MainCamera*/1,/*hbin*/ 1,/*vbin*/ 1,/*xstart*/ 0,
        /*ystart*/0, /*width*/ TestCameraWidth,/*height*/ TestCameraHeight,/*ExposureTime*/ 100,/*Temperature*/ 20,
        /*ExternalTrigger*/ 0, /*vSpeed*/ 16, /*hSpeed*/ 1, /*FKSMode*/ 0,
        /*FKSHeight*/ 341,/*FKSDataImages*/ 1,/*FKSReferenceImages*/ 1,/*FKSvSpeed*/ 2,
        /*FKSExternalTriggerStart,*/ (TestCameraExternalTrigger) ? 1 : 0,/*FKSDirtyLines*/ 0, /*NumberPictures*/ 3,/* aPrePicture*/false,
        /*FlushWait*/ 0, /*DoInternalTriggerAfterExternalTrigger*/false,/*TriggerDelay*/ 10,/*AtomicMass*/ 6,
        /*AtomicWavelength*/461E-9, /*AtomicClebschGordon*/ 1, /*AtomicLinewidth*/30.5E6,
        /*AtomicMagneticMoment*/1,/*AtomicName*/"88Sr",
        /*ProbeDetuning*/0, /*ExpansionTime*/0,/*FluorescencePicture*/false,
        /*MaxFluorescence*/4000,/*Calibration*/1.0,
        /*FinalTrap*/ "MOT", /*EndState*/"MOT",/*EichX*/1,/*EichY*/1, 0, 0);
    NetAndorFrameGrabber[1]->SetImageParameters(/*CameraDirection*/"X",/*CameraUsed*/true,/*PictureNumber*/0,/*MainCamera*/1,/*hbin*/ 1,/*vbin*/ 1,/*xstart*/ 0,
        /*ystart*/0, /*width*/ TestCameraWidth,/*height*/ TestCameraHeight,/*ExposureTime*/ 100,/*Temperature*/ 20,
        /*ExternalTrigger*/ 0, /*vSpeed*/ 16, /*hSpeed*/ 1, /*FKSMode*/ 0,
        /*FKSHeight*/ 341,/*FKSDataImages*/ 1,/*FKSReferenceImages*/ 1,/*FKSvSpeed*/ 2,
        /*FKSExternalTriggerStart,*/ (TestCameraExternalTrigger) ? 1 : 0,/*FKSDirtyLines*/ 0, /*NumberPictures*/ 3,/* aPrePicture*/false,
        /*FlushWait*/ 0, /*DoInternalTriggerAfterExternalTrigger*/false,/*TriggerDelay*/ 10,/*AtomicMass*/ 6,
        /*AtomicWavelength*/461E-9, /*AtomicClebschGordon*/ 1, /*AtomicLinewidth*/30.5E6,
        /*AtomicMagneticMoment*/1,/*AtomicName*/"88Sr",
        /*ProbeDetuning*/0, /*ExpansionTime*/0,/*FluorescencePicture*/false,
        /*MaxFluorescence*/4000,/*Calibration*/1.0,
        /*FinalTrap*/ "MOT", /*EndState*/"MOT",/*EichX*/1,/*EichY*/1, 0, 0);
    NetAndorFrameGrabber[2]->SetImageParameters(/*CameraDirection*/"X",/*CameraUsed*/true,/*PictureNumber*/0,/*MainCamera*/1,/*hbin*/ 1,/*vbin*/ 1,/*xstart*/ 0,
        /*ystart*/0, /*width*/ TestCameraWidth,/*height*/ TestCameraHeight,/*ExposureTime*/ 100,/*Temperature*/ 20,
        /*ExternalTrigger*/ 0, /*vSpeed*/ 16, /*hSpeed*/ 1, /*FKSMode*/ 0,
        /*FKSHeight*/ 341,/*FKSDataImages*/ 1,/*FKSReferenceImages*/ 1,/*FKSvSpeed*/ 2,
        /*FKSExternalTriggerStart,*/ (TestCameraExternalTrigger) ? 1 : 0,/*FKSDirtyLines*/ 0, /*NumberPictures*/ 3,/* aPrePicture*/false,
        /*FlushWait*/ 0, /*DoInternalTriggerAfterExternalTrigger*/false,/*TriggerDelay*/ 10,/*AtomicMass*/ 6,
        /*AtomicWavelength*/461E-9, /*AtomicClebschGordon*/ 1, /*AtomicLinewidth*/30.5E6,
        /*AtomicMagneticMoment*/1,/*AtomicName*/"88Sr",
        /*ProbeDetuning*/0, /*ExpansionTime*/0,/*FluorescencePicture*/false,
        /*MaxFluorescence*/4000,/*Calibration*/1.0,
        /*FinalTrap*/ "MOT", /*EndState*/"MOT",/*EichX*/1,/*EichY*/1, 0, 0);
    NetAndorFrameGrabber[3]->SetImageParameters(/*CameraDirection*/"X",/*CameraUsed*/true,/*PictureNumber*/0,/*MainCamera*/1,/*hbin*/ 1,/*vbin*/ 1,/*xstart*/ 0,
        /*ystart*/0, /*width*/ TestCameraWidth,/*height*/ TestCameraHeight,/*ExposureTime*/ 100,/*Temperature*/ 20,
        /*ExternalTrigger*/ 0, /*vSpeed*/ 16, /*hSpeed*/ 1, /*FKSMode*/ 0,
        /*FKSHeight*/ 341,/*FKSDataImages*/ 1,/*FKSReferenceImages*/ 1,/*FKSvSpeed*/ 2,
        /*FKSExternalTriggerStart,*/ (TestCameraExternalTrigger) ? 1 : 0,/*FKSDirtyLines*/ 0, /*NumberPictures*/ 3,/* aPrePicture*/false,
        /*FlushWait*/ 0, /*DoInternalTriggerAfterExternalTrigger*/false,/*TriggerDelay*/ 10,/*AtomicMass*/ 6,
        /*AtomicWavelength*/461E-9, /*AtomicClebschGordon*/ 1, /*AtomicLinewidth*/30.5E6,
        /*AtomicMagneticMoment*/1,/*AtomicName*/"88Sr",
        /*ProbeDetuning*/0, /*ExpansionTime*/0,/*FluorescencePicture*/false,
        /*MaxFluorescence*/4000,/*Calibration*/1.0,
        /*FinalTrap*/ "MOT", /*EndState*/"MOT",/*EichX*/1,/*EichY*/1, 0, 0);
    NetAndorFrameGrabber[4]->SetImageParameters(/*CameraDirection*/"X",/*CameraUsed*/true,/*PictureNumber*/0,/*MainCamera*/1,/*hbin*/ 1,/*vbin*/ 1,/*xstart*/ 0,
        /*ystart*/0, /*width*/ TestCameraWidth,/*height*/ TestCameraHeight,/*ExposureTime*/ 100,/*Temperature*/ 20,
        /*ExternalTrigger*/ 0, /*vSpeed*/ 16, /*hSpeed*/ 1, /*FKSMode*/ 0,
        /*FKSHeight*/ 341,/*FKSDataImages*/ 1,/*FKSReferenceImages*/ 1,/*FKSvSpeed*/ 2,
        /*FKSExternalTriggerStart,*/ (TestCameraExternalTrigger) ? 1 : 0,/*FKSDirtyLines*/ 0, /*NumberPictures*/ 3,/* aPrePicture*/false,
        /*FlushWait*/ 0, /*DoInternalTriggerAfterExternalTrigger*/false,/*TriggerDelay*/ 10,/*AtomicMass*/ 6,
        /*AtomicWavelength*/461E-9, /*AtomicClebschGordon*/ 1, /*AtomicLinewidth*/30.5E6,
        /*AtomicMagneticMoment*/1,/*AtomicName*/"88Sr",
        /*ProbeDetuning*/0, /*ExpansionTime*/0,/*FluorescencePicture*/false,
        /*MaxFluorescence*/4000,/*Calibration*/1.0,
        /*FinalTrap*/ "MOT", /*EndState*/"MOT",/*EichX*/1,/*EichY*/1, 0, 0);
    NetAndorFrameGrabber[5]->SetImageParameters(/*CameraDirection*/"X",/*CameraUsed*/true,/*PictureNumber*/0,/*MainCamera*/1,/*hbin*/ 1,/*vbin*/ 1,/*xstart*/ 0,
        /*ystart*/0, /*width*/ TestCameraWidth,/*height*/ TestCameraHeight,/*ExposureTime*/ 100,/*Temperature*/ 20,
        /*ExternalTrigger*/ 0, /*vSpeed*/ 16, /*hSpeed*/ 1, /*FKSMode*/ 0,
        /*FKSHeight*/ 341,/*FKSDataImages*/ 1,/*FKSReferenceImages*/ 1,/*FKSvSpeed*/ 2,
        /*FKSExternalTriggerStart,*/ (TestCameraExternalTrigger) ? 1 : 0,/*FKSDirtyLines*/ 0, /*NumberPictures*/ 3,/* aPrePicture*/false,
        /*FlushWait*/ 0, /*DoInternalTriggerAfterExternalTrigger*/false,/*TriggerDelay*/ 10,/*AtomicMass*/ 6,
        /*AtomicWavelength*/461E-9, /*AtomicClebschGordon*/ 1, /*AtomicLinewidth*/30.5E6,
        /*AtomicMagneticMoment*/1,/*AtomicName*/"88Sr",
        /*ProbeDetuning*/0, /*ExpansionTime*/0,/*FluorescencePicture*/false,
        /*MaxFluorescence*/4000,/*Calibration*/1.0,
        /*FinalTrap*/ "MOT", /*EndState*/"MOT",/*EichX*/1,/*EichY*/1, 0, 0);
    // NetAndorFrameGrabber[5]->SetImageParameters(/*CameraDirection*/"X",/*CameraUsed*/true,/*PictureNumber*/3,/*MainCamera*/1,/*hbin*/ 1,/*vbin*/ 1,/*xstart*/ 0,
   //    /*ystart*/0, /*width*/ 1004,/*height*/ 1002,/*ExposureTime*/ 100,/*Temperature*/ 10,
   //    /*ExternalTrigger*/ 0, /*vSpeed*/ 16, /*hSpeed*/ 1, /*FKSMode*/ 0,
   //    /*FKSHeight*/ 341,/*FKSDataImages*/ 1,/*FKSReferenceImages*/ 1,/*FKSvSpeed*/ 2,
   //    /*FKSExternalTriggerStart,*/ 1,/*FKSDirtyLines*/ 0, /*NumberPictures*/ 3,/* aPrePicture*/false,
   //    /*FlushWait*/ 100, /*DoInternalTriggerAfterExternalTrigger*/false,/*TriggerDelay*/ 10,/*AtomicMass*/ 6,
   //    /*AtomicWavelength*/671E-9, /*AtomicClebschGordon*/ 0.5, /*AtomicLinewidth*/6E6,
   //    /*AtomicMagneticMoment*/1,/*AtomicName*/"6Li",
   //    /*ProbeDetuning*/0, /*ExpansionTime*/0,/*FluorescencePicture*/false,
   //    /*MaxFluorescence*/1000,/*Calibration*/1.0,
   //    /*FinalTrap*/ "MOT", /*EndState*/"MOT",/*EichX*/1,/*EichY*/1, 0, 0);
   ////   NetAndorFrameGrabber[1]->SetImageParameters(/*CameraDirection*/"X",/*CameraUsed*/true,/*PictureNumber*/1,/*MainCamera*/0,/*hbin*/ 1,/*vbin*/ 1,/*xstart*/ 0,
//      /*ystart*/0, /*width*/ 512,/*height*/ 1024,/*ExposureTime*/ 10,/*Temperature*/ 0,
//      /*ExternalTrigger*/ 0, /*vSpeed*/ 16, /*hSpeed*/ 1, /*FKSMode*/ 1,
//       /*FKSHeight*/ 341,/*FKSDataImages*/ 1,/*FKSReferenceImages*/ 1,/*FKSvSpeed*/ 2,
//       /*FKSExternalTriggerStart,*/ 1,/*FKSDirtyLines*/ 0, /*NumberPictures*/ 3,/* aPrePicture*/false,
//       /*FlushWait*/ 100, /*DoInternalTriggerAfterExternalTrigger*/false,/*TriggerDelay*/ 10,/*AtomicMass*/ 6,
//       /*AtomicWavelength*/671E-9, /*AtomicClebschGordon*/ 0.5, /*AtomicLinewidth*/6E6,
//       /*AtomicMagneticMoment*/1,/*AtomicName*/"6Li",
//      /*ProbeDetuning*/0, /*ExpansionTime*/0,/*FluorescencePicture*/false,
//       /*MaxFluorescence*/1000,/*Calibration*/1.0,
//       /*FinalTrap*/ "MOT", /*EndState*/"MOT",/*EichX*/1,/*EichY*/1,0,0);
//   NetAndorFrameGrabber[2]->SetImageParameters(/*CameraDirection*/"X",/*CameraUsed*/true,/*PictureNumber*/2,/*MainCamera*/0,/*hbin*/ 1,/*vbin*/ 1,/*xstart*/ 0,
//       /*ystart*/0, /*width*/ 512,/*height*/ 1024,/*ExposureTime*/ 10,/*Temperature*/ 0,
//       /*ExternalTrigger*/ 0, /*vSpeed*/ 16, /*hSpeed*/ 1, /*FKSMode*/ 1,
//       /*FKSHeight*/ 341,/*FKSDataImages*/ 1,/*FKSReferenceImages*/ 1,/*FKSvSpeed*/ 2,
//       /*FKSExternalTriggerStart,*/ 1,/*FKSDirtyLines*/ 0, /*NumberPictures*/ 3,/* aPrePicture*/false,
//       /*FlushWait*/ 100,/*DoInternalTriggerAfterExternalTrigger*/false, /*TriggerDelay*/ 10,/*AtomicMass*/ 6,
//       /*AtomicWavelength*/671E-9, /*AtomicClebschGordon*/ 0.5, /*AtomicLinewidth*/6E6,
//       /*AtomicMagneticMoment*/1,/*AtomicName*/"6Li",
//       /*ProbeDetuning*/0, /*ExpansionTime*/0,/*FluorescencePicture*/false,
//       /*MaxFluorescence*/1000,/*Calibration*/1.0,
//       /*FinalTrap*/ "MOT", /*EndState*/"MOT",/*EichX*/1,/*EichY*/1,0,0);
//   NetAndorFrameGrabber[3]->SetImageParameters(/*CameraDirection*/"X",/*CameraUsed*/true,/*PictureNumber*/3,/*MainCamera*/0,/*hbin*/ 1,/*vbin*/ 1,/*xstart*/ 0,
//       /*ystart*/0, /*width*/ 512,/*height*/ 1024,/*ExposureTime*/ 10,/*Temperature*/ 0,
//       /*ExternalTrigger*/ 0, /*vSpeed*/ 16, /*hSpeed*/ 1, /*FKSMode*/ 1,
//       /*FKSHeight*/ 341,/*FKSDataImages*/ 1,/*FKSReferenceImages*/ 1,/*FKSvSpeed*/ 2,
//       /*FKSExternalTriggerStart,*/ 1,/*FKSDirtyLines*/ 0, /*NumberPictures*/ 3,/* aPrePicture*/false,
//       /*FlushWait*/ 100,/*DoInternalTriggerAfterExternalTrigger*/false, /*TriggerDelay*/ 10,/*AtomicMass*/ 6,
//       /*AtomicWavelength*/671E-9, /*AtomicClebschGordon*/ 0.5, /*AtomicLinewidth*/6E6,
//       /*AtomicMagneticMoment*/1,/*AtomicName*/"6Li",
//       /*ProbeDetuning*/0, /*ExpansionTime*/0,/*FluorescencePicture*/false,
//       /*MaxFluorescence*/1000,/*Calibration*/1.0,
//       /*FinalTrap*/ "MOT", /*EndState*/"MOT",/*EichX*/1,/*EichY*/1,0,0);

   int CameraUsed=0;
   NetAndorFrameGrabber[CameraUsed]->TakeTriggeredPicture();
   bool CameraNetworkOk=NetAndorFrameGrabber[CameraUsed]->CheckReady(20000);
   CameraNetworkOk=CameraNetworkOk && NetAndorFrameGrabber[CameraUsed]->TestReady();
   NetAndorFrameGrabber[CameraUsed]->TransferPictureData(!CameraNetworkOk);
   NetAndorFrameGrabber[CameraUsed]->TestReady();
   picture->ReadPicture(NetAndorFrameGrabber[CameraUsed],APposx1,APposx2,APposy1,APposy2,0);
	UpdateAll();
}

void TVisionDialog::ShowNetAndorCameraInformation() {
  for (int i=0;i>NrNetAndorCameras;i++) {
    if (NetAndorFrameGrabber[i]) {
      NetAndorFrameGrabber[i]->ShowCameraInformations();
    }
  }
}

void TVisionDialog::delay(unsigned int time) { //delay in milliseconds
  if (time<=0) return;
  unsigned long start;
  start=GetTickCount();
  do Sleep(10); while (((GetTickCount()-start)<time) && ((GetTickCount()-start)>0));
}

void TVisionDialog::AbsorptionPictureWithoutDelay() {
  AbsorptionPicture(130,0,false);
}

void TVisionDialog::GetPictureData() {
  if (serial) {
     char Command[200];
     Command[0]=0;
     DWORD StartTime=GetTickCount();
     bool ok=false;
     while ((!ok) && (abs(GetTickCount()-StartTime)<2000000)) ok=serial->GetCommand(Command);
     if ((strcmp(Command,"VisionSynchronizeParameters")!=0) || (!ok)) {
        char buf[200];
        sprintf(buf,"AbsorptionPicture: VisionSynchronizeParameters expected %s received",Command);
     		MessageBox(buf,"Error");
     } else {
	     SynchroniseParameters();
   	  serial->GetDouble(MD->y("MOTFluorescence"));
	     serial->GetDouble(MD->y("RecapFluo"));
   	  serial->GetDouble(MD->y("TrapType"));
	     serial->GetDouble(MD->y("BarCurrent"));
	     serial->GetDouble(MD->y("TransferCurrent"));
   	  serial->GetDouble(MD->y("MOTCurrent"));
	     serial->GetDouble(MD->y("GxOrOffset"));
   	  serial->GetDouble(MD->y("GyOrGradient"));
	     serial->GetDouble(MD->y("GzOrCurvature"));
	     serial->GetDouble(MD->y("LoadingTime"));
   	  serial->GetDouble(MD->y("TimingError"));
	     serial->GetDouble(MD->y("Li7f0"));
	     serial->GetDouble(MD->y("Li7fend"));
        int DataArrayNrPoints;
        serial->GetInt(DataArrayNrPoints);
        double* DataArray=NULL;
        try {
	        DataArray=new double[DataArrayNrPoints];
        }
		  catch (xalloc) {
  				MessageBox("TVisionDialog::GetPictureData : couldn't reserve memory","Error",  MB_OK);
		      exit(-1);
		  }
        for (int i=0;i<DataArrayNrPoints;i++) {
	        serial->GetDouble(DataArray[i]);
        }
        /*for (int i=0;i<DataArrayNrPoints;i++) {
 	        char buf[1000];
           int help=999;
			  serial->ReadChar(buf,help);
        }*/
        for (int i=0;i<MaxDataArray;i++) {
        		if (i<DataArrayNrPoints) {
               char buf[200];
               sprintf(buf,"Data%i",i);
        			MD->y(buf)=DataArray[i];
            }
        }
        delete[] DataArray;
	     SetTrapParameters();
     }
  } else {
    //create dummy data for debug
    ParamList=new CParamList(ParamList,"TestName","TestValue",'S');
    #if defined(DetectLeaks)
      LeakDetect.New(ParamList,5,101);
    #endif
  }
}

void TVisionDialog::SelectMainPicture() {
  switch (MainPictureMode) {
    case 0:picture=pictureabs;break;
    case 1:picture=pictureprobe;break;
    case 2:picture=picturenoiseabs;break;
	 case 3:picture=abspicture;break;
    case 4:picture=opticalDensity;break;
  }
}

void TVisionDialog::TreatPicture(int SaveData,int insert, bool NewLine) {
  //here we fit stuff to the finished picture and extract atomnumber etc.
  MD->y("2DFitFunction")=0;
  SelectMainPicture();
  if (SaveData) {
     PictureWin->Update(MD->y("ProfilAverage"));
     CheckNumber=0;
     Fit1DGaussToSum();
     //FitGaussWingsBothDirections();
     //Fit1DGaussToSum();
     IntegralAtomNumber();
	 CalculateFormulas();
     MakePictureCode();
     MD->SetEnabled(pictureabs->PictureGood());
     if (!InSerie) RegisterInformation(1,MD);
     else {
        MD->InList=true;
        RegisterInformation(0,MD);
     }
     int iPictureListNr=MD->y("PictureListPosition");
     if (iPictureListNr>=0) PictureList[iPictureListNr % NrPictureLists]->AddPicture(picture,MD->GetText("Filename"),MD->GetText("AtomicName"),MD->GetText("CameraDirection"),NewLine);
  } else {
     UpdateAll();
     Load2DFitParams();
     Fit1DGaussToSum();
    // FitGaussWingsBothDirections();
     //Fit1DGaussToSum();
     IntegralAtomNumber();
	 CalculateFormulas();
     DoAutomaticFit();
     if (insert) {
         int iPictureListNr=MD->y("PictureListPosition");
         if (iPictureListNr>=0) PictureList[iPictureListNr % NrPictureLists]->AddPicture(picture,MD->GetText("Filename"),MD->GetText("AtomicName"),MD->GetText("CameraDirection"), NewLine);
     }
     DisplayMessData();
     if (UpdateMeasurementAfterFit && MD->IsInList()) {
       TNmeasure->Draw();
     }
  }
}

void TVisionDialog::SavePictureAsBMPIcon() {
   if (picture) {
		char BMPName[300];
   	strcpy(BMPName,MD->GetText("Filename"));
	   *strchr(BMPName,'.')=0;
   	strcat(BMPName,".bmp");
	   picture->Save(BMPName,SAVE_PALETTEBMP,ColorTable);
   }
}

void TVisionDialog::SaveParameterList(CParamList *paramlist, ofstream &out, bool SaveAll, bool Update, bool Distance, TEdit* LogbookEdit, bool FirstLine, bool SkipUnused, bool Skipping) {

// added this function to save the parameter data in order to fix the vision crashing problem due to iterative calling of a function to write each line of the parameter files - shayne 25/3/2017

   char *Name;
   char *Value;
   char *OldValue;
   char Type;
   bool changed;
   CParamList *next;

	do {
   	Name=paramlist->GetName();
   	Value=paramlist->GetValue();
   	OldValue=paramlist->GetOldValue();
      Type=paramlist->GetType();
      changed=paramlist->Getchanged();

		if (SaveAll || changed) {
			if ((Type=='T') && (!FirstLine)) {
      		if (!Skipping) out<<endl;
      		Skipping=false;
    		}
    		if (Type=='N') {
        		if (!Skipping) out<<endl;
	    		Skipping=false;
			} else {
        		if ((Type=='F') && (SkipUnused)) {
		    		Skipping=(strcmp(Value,"FALSE")==0);
	    		}
			if (Distance) {
				if (!Skipping) out<<"  ";
            if (LogbookEdit) LogbookEdit->Insert("  ");
         }
			if (!Skipping) out<<Name;
			if (LogbookEdit) LogbookEdit->Insert(Name);
			if (Type!='T') {
				char lbuf[300];
				if ((changed) && (OldValue)) sprintf(lbuf,"=%s (%s)\r\n",Value,OldValue);
				else sprintf(lbuf,"=%s\r\n",Value);
				if (!Skipping) out<<lbuf;
				if (LogbookEdit) LogbookEdit->Insert(lbuf);
			} else {
				if (!Skipping) out<<endl;
				if (LogbookEdit) LogbookEdit->Insert("\r\n");
			}
     }
  }
  if (Update) changed=false;

  FirstLine=false;
  next=paramlist->GetNext();
  paramlist=next;
  } while (next!=NULL);
  //if (next!=NULL) next->Save(out, SaveAll, Update,Distance,LogbookEdit,false,SkipUnused, Skipping);

}


void TVisionDialog::UpdateParameterList(CParamList *paramlist) {

// added this function to save the parameter data in order to fix the vision crashing problem due to iterative calling of a function to write each line of the parameter files - shayne 25/3/2017

   CParamList *next;

	do {
 	  	paramlist->SingleUpdate();
  		next=paramlist->GetNext();
  		paramlist=next;
  	} while (next!=NULL);
  	//if (next!=NULL) next->Save(out, SaveAll, Update,Distance,LogbookEdit,false,SkipUnused, Skipping);

}


void TVisionDialog::SaveImage(bool FirstImage, unsigned int CameraNumber, unsigned int ImagePosition) {
    MD->y("OffsetShift")=InitialOffsetShift;
  	 char buf[200];
 	 char buf2[200];
    if (!CheckNumber && pictureabs->PictureGood()) {
    Name->GetName(buf,FirstImage,CameraNumber);
	 sprintf(buf2,buf,'A',ImagePosition);
    strcpy(LastName,buf);
	 int err1=pictureabs->Save(buf2,SAVE_AUTOMATIC,ColorTable);
    MD->SetText("Filename",buf2);
	 mb->SetText(buf2);
    sprintf(buf2,buf,'P',ImagePosition);
	 int err2=pictureprobe->Save(buf2,SAVE_AUTOMATIC,ColorTable);
	 sprintf(buf2,buf,'N',ImagePosition);
	 int err3=picturenoiseabs->Save(buf2,SAVE_AUTOMATIC,ColorTable);
    sprintf(buf2,buf,'O',ImagePosition);
	 int err4=picturenoiseprobe->Save(buf2,SAVE_AUTOMATIC,ColorTable);

	 int k=strlen(buf2);
	 for (int i=0;i<AktAdditionalProbe;i++) {
		 sprintf(buf2,buf,'P',ImagePosition);
		 buf2[k-5]='A'+i;
		 pictureAdditionalprobe[i]->Save(buf2,SAVE_AUTOMATIC,ColorTable);
	 }

    if (ParamList && FirstImage) {
      sprintf(buf2,buf,'_',ImagePosition);
      buf2[strlen(buf2)-7]=0;
      char buf3[300];
      strcpy(buf3,buf2);
      char buf4[300];
      strcpy(buf4,buf2);
      strcat(buf2,".txt");
      ofstream out(buf2);
      SaveParameterList(ParamList,out,true,false,false,NULL,true,false,false);
      // added this function to save the parameter data in order to fix the vision crashing problem due to iterative calling of a function to write each line of the parameter files - shayne 25/3/2017

    	//ParamList->Save(out,true,false,false,NULL,true,false,false);
      out.close();
      strcat(buf3,"s.txt");
    	ofstream out3(buf3);
      SaveParameterList(ParamList,out3,true,false,false,NULL,true,true,false);
      // added this function to save the parameter data in order to fix the vision crashing problem due to iterative calling of a function to write each line of the parameter files - shayne 25/3/2017

      //ParamList->Save(out3,true,false,false,NULL,true,true,false);
      out3.close();
      strcat(buf4,".roi");
      SaveROIs(buf4);
    }
    if (!InSerie) {
    	Name->GetImageLogbookName(buf);
    	ofstream out2(buf,ios::app);
      if (FirstImage) {
         char abuf[300];
         strcpy(abuf,MD->GetText("Filename"));
         while (strchr(abuf,'\\')>0) strcpy(abuf,strchr(abuf,'\\')+1);
         if ((strchr(abuf,'.')>0) && strlen(abuf)>6) {
           abuf[strlen(abuf)-6]=0;
         }
         char lbuf[300];
         sprintf(lbuf,"* %s   %s   %s\r\n",abuf,MD->GetText("Path"),MD->GetText("Endstate"));
         out2<<lbuf;//"* "<<abuf<<"   "<<MD->GetText("Path")<<"   "<<MD->GetText("Endstate")<<endl;
         if (LogbookEdit)  {
           LogbookEdit->Scroll(0,1000);
           LogbookEdit->Search(0,"�");
           LogbookEdit->Insert(lbuf);
         }
      }
	   if (ParamsJustLoaded) {
   	  ParamsJustLoaded=false;
        char lbuf[300];
        sprintf(lbuf,"  Read %s\r\n",buf2);
	     out2<<lbuf;//"  Read "<<buf2<<end;;
   	  if (LogbookEdit) {
        		LogbookEdit->Scroll(0,1000);
            LogbookEdit->Search(0,"�");
         	LogbookEdit->Insert(lbuf);
        }
        if (ParamList) UpdateParameterList(ParamList);
      } else if (ParamList && FirstImage) SaveParameterList(ParamList,out2,false,true,true,LogbookEdit,true,false,false); //ParamList->Save(out2,false,true,true,LogbookEdit,true,false,false);
      out2.close();
    }
	 if (err1 || err2 || err3 || err4) MessageBox("Insufficient disk space!","Error");
  }
}

void TVisionDialog::CopyAtomicDataFromFramegrabberToMessPoint(int FrameGrabberNr) {
   MD->y("Mass")=NetAndorFrameGrabber[FrameGrabberNr]->AtomicMass;
   MD->y("Wavelength")=NetAndorFrameGrabber[FrameGrabberNr]->AtomicWavelength;
   MD->y("Clebsch")=NetAndorFrameGrabber[FrameGrabberNr]->AtomicClebschGordon;
   MD->y("Linewidth")=NetAndorFrameGrabber[FrameGrabberNr]->AtomicLinewidth;
   MD->y("MagMoment")=NetAndorFrameGrabber[FrameGrabberNr]->AtomicMagneticMoment;
   MD->y("ProbeDetuning")=NetAndorFrameGrabber[FrameGrabberNr]->ProbeDetuning;
   MD->y("ExpansionTime")=NetAndorFrameGrabber[FrameGrabberNr]->ExpansionTime;  //in ms
   MD->y("Fluo")=(NetAndorFrameGrabber[FrameGrabberNr]->FluorescencePicture) ? 1 : 0;
   MD->y("MaxFluo")=NetAndorFrameGrabber[FrameGrabberNr]->MaxFluorescence;
   MD->y("Calibration")=NetAndorFrameGrabber[FrameGrabberNr]->Calibration;
   MD->y("FastKinetics")=NetAndorFrameGrabber[FrameGrabberNr]->FKSMode;
   MD->y("FKSHeight")=NetAndorFrameGrabber[FrameGrabberNr]->FKSHeight;
   MD->y("FKSDirtyLines")=NetAndorFrameGrabber[FrameGrabberNr]->FKSDirtyLines;
   MD->y("Camera")=FrameGrabberNr;
   //the following parameters are read indirectly because the image might be rotated or flipped
   int xh;
   int yh;
   int BiningX;
   int BiningY;
   double EichX;
   double EichY;
   int bitplanes;
   NetAndorFrameGrabber[FrameGrabberNr]->GetImageParameters(xh,yh,EichX,EichY,BiningX,BiningY,bitplanes);
   MD->y("EichX")=EichX;
   MD->y("EichY")=EichY;
   MD->y("BiningX")=BiningX;
   MD->y("BiningY")=BiningY;

   MD->SetText("Path",NetAndorFrameGrabber[FrameGrabberNr]->EndState);
   MD->SetText("Endstate",NetAndorFrameGrabber[FrameGrabberNr]->FinalTrap);

   MD->SetText("AtomicName",NetAndorFrameGrabber[FrameGrabberNr]->AtomicName);
   MD->SetText("CameraDirection",NetAndorFrameGrabber[FrameGrabberNr]->CameraDirection);
}

void TVisionDialog::AbsorptionPicture(double TriggerDelay, double TriggerDelay2, bool TimerTriggered) {
  if (SerialTimerActive) {
    KillTimer(ID_TESTSERIALTIMER);
    SerialTimerActive=false;
  }
  if (!TimerTriggered) {
    StoreTriggerDelay2=TriggerDelay2;
    FitButtonFunction=0;
    FirstFermiFit=1;
    profilV->SetGeneralProfil();
    profilH->SetGeneralProfil();
 //   if (!RDSetup->HardwareAccess) return;
    APNoiseMode=MD->y("NoisePictureMode");
    mb->SetText("Taking absorption Picture");
    profilH->GetPos(APposx1,APposx2);
    profilV->GetPos(APposy1,APposy2);
  } else {
		TriggerDelay2=StoreTriggerDelay2;
  }
  int timeout0,timeout1,timeout2;
//just for debugging
//  ofstream debugfile("DebugVision.txt",ios::app);
//  char db2buf[200];
//  sprintf(db2buf,"1: %X %i",this,(TimerTriggered) ? 1 : 0);
//  debugfile<<"---------"<<endl;
//  debugfile<<db2buf<<endl;
  switch (AktCameraTyp) {
      case 11:
		#if defined(NetAndorCameraUsed)
         bool CamerasExist=true;
         for (int i=0;i<NrNetAndorCameras;i++) CamerasExist=CamerasExist || (NetAndorFrameGrabber[i]!=NULL);
			if (!CamerasExist) {
           MessageBox("No NetAndor Framegrabber initialized","error");
           if (!SerialTimerActive) {
             SetTimer(ID_TESTSERIALTIMER,testserialtime);
			    SerialTimerActive=true;
			  }
           return;
         }
           //if the TriggerDelay is long enough, we just program the windows timer
           //to alert us 3 seconds before the images are taken and give control
           //back to the normal program. This enables the user to flip through
           //the images and data while the next picture is prepared
          if (!TimerTriggered) {
             if (serial) serial->SendCommand("*Ready#");
             gettime(&TakeAbsPictureStartTime);
             if ((TriggerDelay>4000) && ((TakeAbsPictureStartTime.ti_hour<23) || (TakeAbsPictureStartTime.ti_min<55))) {
                AbsPictureTimerSet=true;
                SetTimer(ID_ANDORCAMERATIMER,TriggerDelay-3000);
                AbsPictureTriggerDelay=TriggerDelay;
//  sprintf(db2buf,"22: %X %i",this,(TimerTriggered) ? 1 : 0);
//debugfile<<db2buf<<endl;
                return;
             } else delay(TriggerDelay);
          } else {
             //here control comes back triggered by the windows timer.
             //we have to wait the remainder of the TriggerDelay required
             struct time TakeAbsPictureStopTime;
             gettime(&TakeAbsPictureStopTime);
             long restDelay=AbsPictureTriggerDelay-((((TakeAbsPictureStopTime.ti_hour-TakeAbsPictureStartTime.ti_hour)*60+
               (TakeAbsPictureStopTime.ti_min-TakeAbsPictureStartTime.ti_min))*60+
               (TakeAbsPictureStopTime.ti_sec-TakeAbsPictureStartTime.ti_sec))*1000+
               (TakeAbsPictureStopTime.ti_hund-TakeAbsPictureStartTime.ti_hund)*10);
             if (restDelay>0) delay(restDelay);
             else MessageBox("Timer Event too slow","error");
//  sprintf(db2buf,"23: %X %i",this,(TimerTriggered) ? 1 : 0);
//debugfile<<db2buf<<endl;
          }

          //for every camera the control program did specify during the VisionSetCameraParameters
          //command if an image has to be taken and if yes, at which delay after the initial TriggerDelay
          //we sort through the cameras to find out how many pictures in total have to be taken
          unsigned long  AndorStart=GetTickCount();
          bool CameraDone[MaxNrNetAndorCameras];
          int NrPicturesToTake=0;
          for (int i=0;i<NrNetAndorCameras;i++) {
            CameraDone[i]=!NetAndorFrameGrabber[i]->CameraUsed;
            if (!CameraDone[i]) NrPicturesToTake++;
          }
          if (NrPicturesToTake==0) {
	         if (!SerialTimerActive) {
              SetTimer(ID_TESTSERIALTIMER,testserialtime);
				  SerialTimerActive=true;
				}
            return;
          }
          //if pictures have to be taken, we search for the yet unused camera
          //with the shortest TriggerDelay, wait this out, trigger the picturetaking
          //and continue. If the triggerdelay are equal (which will be the most
          //common situation) the following will just send the softtrigger commands
          //as fast as possible out to the cameraservers.
          //the final high precision timing is of course done by hardware trigger
          //signals to the cameras.
          for (int j=0;j<NrPicturesToTake;j++) {
            //find next camera to softtrigger:
            int NextCamera=-1;
            unsigned long NextTriggerDelay=1000000;
            for (int i=0;i<NrNetAndorCameras;i++) if (!CameraDone[i]) {
              if (NetAndorFrameGrabber[i]->TriggerDelay<NextTriggerDelay) {
                NextCamera=i;
                NextTriggerDelay=NetAndorFrameGrabber[i]->TriggerDelay;
              }
            }
            if (NextCamera==-1) {
              MessageBox("TVisionDialog::AbsorptionPicture case 11 : no camera has to take picture","error");
              if (!SerialTimerActive) {
                SetTimer(ID_TESTSERIALTIMER,testserialtime);
    			    SerialTimerActive=true;
	   		  }
              return;
            } else {
              CameraDone[NextCamera]=true;
              while (abs(GetTickCount()-AndorStart)<NextTriggerDelay) ;
              NetAndorFrameGrabber[NextCamera]->TakeTriggeredPicture();
            }
          }
//  sprintf(db2buf,"100: %X %i",this,(TimerTriggered) ? 1 : 0);
//debugfile<<db2buf<<endl;

          //now all the pictures have been recorded and are stored in the
          //framegrabber computer memory. We have to transfer them to Vision,
          //save them to disk and exploit them


		  //transfer the picture data over ethernet to the framegrabber class
          //FKS images have been cut up into normal images already by the camera computer
          //(which reduces the amount of data transferred.)
          //Probe and noise images which have not been taken to gain speed
          //are taken from hard disk by the camera class if possible
          //If new probe or noise images are available they are stored on hard disk
          mb->SetText("Waiting for camera");
          for (int i=0;i<NrNetAndorCameras;i++) {
            if (NetAndorFrameGrabber[i]->CameraUsed) {
              bool CameraNetworkOk=NetAndorFrameGrabber[i]->CheckReady(20000);
              CameraNetworkOk=CameraNetworkOk && NetAndorFrameGrabber[i]->TestReady();
              NetAndorFrameGrabber[i]->TransferPictureData(!CameraNetworkOk);
              NetAndorFrameGrabber[i]->TestReady();
            }
          }
          mb->SetText("Picture data transferred");

//  sprintf(db2buf,"101: %X %i",this,(TimerTriggered) ? 1 : 0);
//debugfile<<db2buf<<endl;

          //first we get the remaining data from Control
          if (serial) serial->SendCommand("*Ready#");
          mb->SetText("Waiting for picture data from Control");
          GetPictureData();
          //Get remaining commands from Control, e.g. ReceiveDataFile or RegisterInformation, so that control can at once start next run
          bool oldcheckingSerial=checkingSerial;
          checkingSerial=false;
          while (!RunFinished) CheckSerial();
          checkingSerial=oldcheckingSerial;

//  sprintf(db2buf,"102: %X %i",this,(TimerTriggered) ? 1 : 0);
//debugfile<<db2buf<<endl;



          //The control program has send the variable "DisplayPictureNumber" to each camera.
          //This variable decides where the picture will be displayed in the displaylists.
          //if this is greater than "NrPictureLists", a new line in the lists has to be started

          //First we determin the number of lines of pictures taken
          //in this run:
          //the order in the picturelists is the usual one:
          //0 1 2 ... NrPictureLists
          //NrPictureLists+1 ... 2*NrPictureLists
          //...
          int MaxPictureDisplayPosition=-1;
          for (int i=0;i<NrNetAndorCameras;i++) if (NetAndorFrameGrabber[i]->CameraUsed) {
            if (NetAndorFrameGrabber[i]->DisplayedPictureNumber>MaxPictureDisplayPosition)
              MaxPictureDisplayPosition=NetAndorFrameGrabber[i]->DisplayedPictureNumber;
          }

//  sprintf(db2buf,"103: %X %i",this,(TimerTriggered) ? 1 : 0);
//debugfile<<db2buf<<endl;
          if (MaxPictureDisplayPosition>(MaxPictureLines*NrPictureLists)) {
              MessageBox("TVisionDialog::AbsorptionPicture case 11 : picture position too high","error");
              if (!SerialTimerActive) {
                SetTimer(ID_TESTSERIALTIMER,testserialtime);
   			    SerialTimerActive=true;
	   		  }
              return;
          }
          int CameraForPictureNumber[MaxPictureLines*MaxNrPictureLists];
          for (int i=0;i<MaxPictureLines*NrPictureLists;i++) CameraForPictureNumber[i]=-1;
          bool PictureLineUsed[MaxPictureLines];
          for (int i=0;i<MaxPictureLines;i++) PictureLineUsed[i]=false;
          for (int i=0;i<NrNetAndorCameras;i++) if (NetAndorFrameGrabber[i]->CameraUsed) {
            int PictureLineNumber=NetAndorFrameGrabber[i]->DisplayedPictureNumber / NrPictureLists;
            PictureLineUsed[PictureLineNumber]=true;
            CameraForPictureNumber[NetAndorFrameGrabber[i]->DisplayedPictureNumber]=i;
          }

//  sprintf(db2buf,"104: %X %i",this,(TimerTriggered) ? 1 : 0);
//debugfile<<db2buf<<endl;
          //now we go over all possible picture positions in sequence and treat and save the corresponding data
          bool FirstImage=true;
          int ImageNr=0;
          int LastPicturePosition=-1;
          int MainCamera=-1;
          int LastCamera=-1;
		  for (int Nr=0;Nr<NrUsedROIs;Nr++) {
			char buf[300];
			sprintf(buf,"NROI%i",Nr);
			MD->y(buf)=0;
		  }
      mb->SetText("Analyzing pictures");
          CMessPoint *MainMD=NULL;
          for (int i=0;i<MaxThisRunMD;i++) ThisRunMD[i]=NULL;
          bool FirstPicture=true;
          for (int y=0;y<MaxPictureLines;y++) {
             for (int x=0;x<NrPictureLists;x++) {
                int PicturePos=y*NrPictureLists+x;
                if (CameraForPictureNumber[PicturePos]>=0) {
                  NewMDForAbsPicture();
                  if (ImageNr<MaxThisRunMD) {
                  	ThisRunMD[ImageNr]=MD;
	      	    		ThisRunMD[ImageNr]->SetAllX(MDXInformation);
	                  ImageNr++;
                  }
                  pictureabs->ReadPicture(NetAndorFrameGrabber[CameraForPictureNumber[PicturePos]],APposx1,APposx2,APposy1,APposy2,0);
                  pictureprobe->ReadPicture(NetAndorFrameGrabber[CameraForPictureNumber[PicturePos]],APposx1,APposx2,APposy1,APposy2,1);
                  picturenoiseabs->ReadPicture(NetAndorFrameGrabber[CameraForPictureNumber[PicturePos]],APposx1,APposx2,APposy1,APposy2,2);
                  picturenoiseprobe->ReadPicture(NetAndorFrameGrabber[CameraForPictureNumber[PicturePos]],APposx1,APposx2,APposy1,APposy2,3);

				AktAdditionalProbe=NetAndorFrameGrabber[CameraForPictureNumber[PicturePos]]->GetNrAdditionalProbePictures();
				for (int i=0;i<AktAdditionalProbe;i++) {
					if (!pictureAdditionalprobe[i]) pictureAdditionalprobe[i]=new CPicture(1,0);
					pictureAdditionalprobe[i]->ReadPicture(NetAndorFrameGrabber[CameraForPictureNumber[PicturePos]],APposx1,APposx2,APposy1,APposy2,4+i);
				}

//  sprintf(db2buf,"105: %X %i",this,(TimerTriggered) ? 1 : 0);
//debugfile<<db2buf<<endl;
                  //if this camera is the main camera then we make a security copy so that we can display this image at the end
                  if (NetAndorFrameGrabber[CameraForPictureNumber[PicturePos]]->MainCamera) {
                     MainCamera=CameraForPictureNumber[PicturePos];
                     MainMD=MD;
                  }
//  sprintf(db2buf,"105A: %X %i",this,(TimerTriggered) ? 1 : 0);
//debugfile<<db2buf<<endl;
                  MD->y("MainPicture")=MainPictureMode; //e.g. abs or opt dens, whatever is selected at the moment
                  MD->y("PictureListPosition")=PicturePos;
                  LastCamera=CameraForPictureNumber[PicturePos];
                  LastPicturePosition=PicturePos;
//  sprintf(db2buf,"105B: %X %i",this,(TimerTriggered) ? 1 : 0);
//debugfile<<db2buf<<endl;
                  MD->y("FreeMem")=GetFreeMem();
//  sprintf(db2buf,"105B1: %X %i",this,(TimerTriggered) ? 1 : 0);
//debugfile<<db2buf<<endl;
                  MD->y("SeveralPictures")=NrPicturesToTake>1;
//  sprintf(db2buf,"105B2: %X %i",this,(TimerTriggered) ? 1 : 0);
//debugfile<<db2buf<<endl;
                  CopyAtomicDataFromFramegrabberToMessPoint(CameraForPictureNumber[PicturePos]);
//  sprintf(db2buf,"105B3: %X %i",this,(TimerTriggered) ? 1 : 0);
//debugfile<<db2buf<<endl;
                  SaveImage(FirstImage,CameraForPictureNumber[PicturePos],PicturePos);
//  sprintf(db2buf,"105C: %X %i",this,(TimerTriggered) ? 1 : 0);
//debugfile<<db2buf<<endl;
                	GetProfilPos();
    					double posx1,posx2,posy1,posy2;
    					profilH->GetPos(posx1,posx2);
    					profilV->GetPos(posy1,posy2);
//  sprintf(db2buf,"105D: %X %i",this,(TimerTriggered) ? 1 : 0);
//debugfile<<db2buf<<endl;
    					LnNormalizeAbsorption(*pictureabs,*pictureprobe,picturenoiseabs,picturenoiseprobe,
      					*abspicture,*opticalDensity,MaxOptDensity,posx1,posx2,posy1,posy2,MD->y("Fluo"),MD->y("MaxFluo"),DoIntensityCompensation,&ROI[8*8],WV,WH,MD->y("FastKinetics"));
                  bool NewLine=(x==0) || FirstPicture;
                  FirstPicture=false;
                  mb->SetText("Saving pictures");
    					TreatPicture(/*SaveData*/true,/*insert*/true,NewLine);
//  sprintf(db2buf,"105E: %X %i",this,(TimerTriggered) ? 1 : 0);
//debugfile<<db2buf<<endl;
    					SavePictureAsBMPIcon();
                  CheckDiskSpace();
                  FirstImage=false;
//  sprintf(db2buf,"105F: %X %i",this,(TimerTriggered) ? 1 : 0);
//debugfile<<db2buf<<endl;
                  #if defined(DetectLeaks)
                     LeakDetect.Delete(MainPicture[PicturePos]);
                  #endif
//  sprintf(db2buf,"105G: %X %i",this,(TimerTriggered) ? 1 : 0);
//debugfile<<db2buf<<endl;
                  if (MainPicture[PicturePos]) delete MainPicture[PicturePos];
                  MainPicture[PicturePos]=new CPicture(*picture);
                  #if defined(DetectLeaks)
					       LeakDetect.New(MainPicture[PicturePos],5,53);
				      #endif
                } else {
                  #if defined(DetectLeaks)
                     LeakDetect.Delete(MainPicture[PicturePos]);
                  #endif
//  sprintf(db2buf,"105H: %X %i",this,(TimerTriggered) ? 1 : 0);
//debugfile<<db2buf<<endl;
                  if (MainPicture[PicturePos]) delete MainPicture[PicturePos];
                  MainPicture[PicturePos]=new CPicture(1,0);
                  #if defined(DetectLeaks)
					       LeakDetect.New(MainPicture[PicturePos],5,54);
				      #endif
                }
             }
          }

//  sprintf(db2buf,"106: %X %i",this,(TimerTriggered) ? 1 : 0);
//debugfile<<db2buf<<endl;

          if (LastPicturePosition>=0) PictureList[LastPicturePosition % NrPictureLists]->SynchronizePosition();
//  sprintf(db2buf,"107: %X %i",this,(TimerTriggered) ? 1 : 0);
//debugfile<<db2buf<<endl;


          if ((MainCamera>=0) && (LastCamera!=MainCamera) && (MainMD!=NULL)) {
            //here we should do more or less what happens when one clicks on a picture in the picturelist
			LastCamera=MainCamera;
//  sprintf(db2buf,"108: %X %i",this,(TimerTriggered) ? 1 : 0);
//debugfile<<db2buf<<endl;
			pictureabs->ReadPicture(NetAndorFrameGrabber[MainCamera],APposx1,APposx2,APposy1,APposy2,0);
            pictureprobe->ReadPicture(NetAndorFrameGrabber[MainCamera],APposx1,APposx2,APposy1,APposy2,1);
            picturenoiseabs->ReadPicture(NetAndorFrameGrabber[MainCamera],APposx1,APposx2,APposy1,APposy2,2);
            picturenoiseprobe->ReadPicture(NetAndorFrameGrabber[MainCamera],APposx1,APposx2,APposy1,APposy2,3);
//  sprintf(db2buf,"109: %X %i",this,(TimerTriggered) ? 1 : 0);
//debugfile<<db2buf<<endl;
      		MD->SetMetaPointer(NULL);
      		if (!MD->IsInList()) {
              #if defined(DetectLeaks)
                LeakDetect.Delete(MD);
              #endif
              delete MD;
            }
            MD=MainMD;
//  sprintf(db2buf,"110: %X %i",this,(TimerTriggered) ? 1 : 0);
//debugfile<<db2buf<<endl;
      		MD->SetMetaPointer(MetaPointer);
            GetProfilPos();
            double posx1,posx2,posy1,posy2;
            profilH->GetPos(posx1,posx2);
            profilV->GetPos(posy1,posy2);
//  sprintf(db2buf,"111: %X %i",this,(TimerTriggered) ? 1 : 0);
//debugfile<<db2buf<<endl;
            LnNormalizeAbsorption(*pictureabs,*pictureprobe,picturenoiseabs,picturenoiseprobe,
            	*abspicture,*opticalDensity,MaxOptDensity,
               posx1,posx2,posy1,posy2,MD->y("Fluo"),MD->y("MaxFluo"),DoIntensityCompensation,&ROI[8*8],WV,WH,MD->y("FastKinetics"));
            TreatPicture(/*SaveData*/false,/*insert*/false,/*NewLine*/ false);
          }
//  sprintf(db2buf,"112: %X %i",this,(TimerTriggered) ? 1 : 0);
//debugfile<<db2buf<<endl;
          DisplayMessData();
          UpdateAll();
          TNmeasure->Draw();
			#endif
		break;
	}
   if (!SerialTimerActive) {
     SetTimer(ID_TESTSERIALTIMER,testserialtime);
     SerialTimerActive=true;
   }
//sprintf(db2buf,"41: %X %i",this,(TimerTriggered) ? 1 : 0);
//debugfile<<db2buf<<endl;
//  char dbbuf[200];
//  sprintf(dbbuf,"Absorption picture: timeout0=%i timeout1=%i timeout2=%i",timeout0,timeout1,timeout2);
//    sprintf(dbbuf,"42: %X %i",this,(TimerTriggered) ? 1 : 0);
//  debugfile<<dbbuf<<endl;
//  debugfile.close();

}

void TVisionDialog::MakePictureCode() {
  char PictureCode[20];
  PictureCode[0]='P';
  PictureCode[1]=(char)('0'+MD->y("PictureListPosition"));
  PictureCode[2]=(char)('0'+MD->y("Camera"));
  PictureCode[3]=0;
  MD->SetCode(PictureCode);
}

void TVisionDialog::EvTimer(UINT timerid) {
  if (AbsPictureTimerSet) {
    if (timerid==ID_ANDORCAMERATIMER) {
      KillTimer(ID_ANDORCAMERATIMER);
      AbsPictureTimerSet=false;
      //called with this=NULL
      AbsorptionPicture(0,0,true);
    }
    return;
  }
  if (InTimer) return;
  InTimer=true;
  switch (timerid) {
	 case ID_TESTSERIALTIMER:
		CheckSerial();
      // test by Andi: kill this timer
	    KillTimer(ID_TESTSERIALTIMER);
       SerialTimerActive=false;
	 break;
	 case ID_FILMTIMER:
      if (!TakingPicture) {
        TakingPicture=1;
        NetAndorPicture(FilmCameraNr);
        TakingPicture=0;
      }
	 break;
  /*  case ID_LIVECONNECTIONTIMER:
      if (LiveStopSerie) StopSerie();
      if (LiveNewSerie) TNmeasure->Load(LiveSerieName,0);
      if (LiveNewPicture) LoadMeasurement(LivePicName);
      char Message[300];
	   int error=Name->CheckLivePicture(LivePicName,LiveSerieName,
         LiveNewPicture,LiveNewSerie,LiveStopSerie,Message);
      if (Message[0]!=0) {
        mb->SetText(Message);
        if (strncmp(strchr(Message,'|'),"| VisionCommand:",16)==0) {
          CheckNoArgCommands(strchr(Message,'|')+17);
        }
      }
      if (error!=0) {
        if (error==-1) ;//DisconnectLive();
      }
	 break;*/
  }
  InTimer=false;
}

void TVisionDialog::StartFilming(int Camera) {
  FilmCameraNr=Camera;
  if (!filming) {
	 mb->SetText("Continuously filming");
	 SetTimer(ID_FILMTIMER,filmtime);
	 filming=1;
  }
}

void TVisionDialog::StopFilming() {
  if (filming) {
	 mb->SetText("Stopt to film");
	 KillTimer(ID_FILMTIMER);
	 filming=0;
  }
}

void TVisionDialog::ToggleLiveConnection() {
  if (!LiveConnected) {
	 mb->SetText("Live connection");
	 SetTimer(ID_LIVECONNECTIONTIMER,RDSetup->LiveConnectTime);
	 LiveConnected=true;
  } else {
	 mb->SetText("Live connection ended");
	 KillTimer(ID_LIVECONNECTIONTIMER);
	 LiveConnected=false;
  }
}

void TVisionDialog::MainPictureMode0() {
  MainPictureMode=0;
}

void TVisionDialog::MainPictureMode1() {
  MainPictureMode=1;
}

void TVisionDialog::MainPictureMode2() {
  MainPictureMode=2;
}

void TVisionDialog::MainPictureMode3() {
  MainPictureMode=3;
}

void TVisionDialog::MainPictureMode4() {
  MainPictureMode=4;
}

void TVisionDialog::CeMainPictureMode0(TCommandEnabler& ce)
{
  ce.SetCheck(MainPictureMode==0 ? TCommandEnabler::Checked : TCommandEnabler::Unchecked);
}

void TVisionDialog::CeMainPictureMode1(TCommandEnabler& ce)
{
  ce.SetCheck(MainPictureMode==1 ? TCommandEnabler::Checked : TCommandEnabler::Unchecked);
}

void TVisionDialog::CeMainPictureMode2(TCommandEnabler& ce)
{
  ce.SetCheck(MainPictureMode==2 ? TCommandEnabler::Checked : TCommandEnabler::Unchecked);
}

void TVisionDialog::CeMainPictureMode3(TCommandEnabler& ce)
{
  ce.SetCheck(MainPictureMode==3 ? TCommandEnabler::Checked : TCommandEnabler::Unchecked);
}

void TVisionDialog::CeMainPictureMode4(TCommandEnabler& ce)
{
  ce.SetCheck(MainPictureMode==4 ? TCommandEnabler::Checked : TCommandEnabler::Unchecked);
}

void TVisionDialog::OptDensNormal() {
  OptDensMethod=0;
}

void TVisionDialog::OptDensAlice() {
  OptDensMethod=1;
}

void TVisionDialog::ChangeIntegralWithOffset() {
  IntegralWithOffset=!IntegralWithOffset;
}

void TVisionDialog::ChangeFermiFitHorizontal() {
  FitFermiHorizontal=!FitFermiHorizontal;
}

void TVisionDialog::ChangeFitFermiZeroTemp() {
  FitFermiZeroTemp=!FitFermiZeroTemp;
  if (FitFermiZeroTemp) {
    FitPartialFermiL=0;
    FitPartialFermiTemp=1;
    FitPartialFermiNumber=1;
  } else {
    FitPartialFermiL=1;
    FitPartialFermiTemp=0;
    FitPartialFermiNumber=0;
  }
}

void TVisionDialog::ChangeFitPartialFermiL() {
  FitPartialFermiL=!FitPartialFermiL;
}

void TVisionDialog::ChangeFitPartialFermiTemp() {
  FitPartialFermiTemp=!FitPartialFermiTemp;
}

void TVisionDialog::ChangeFitPartialFermiNumber() {
  FitPartialFermiNumber=!FitPartialFermiNumber;
}

void TVisionDialog::ChangeFitWithNumberRatio() {
  FitWithNumberRatio=!FitWithNumberRatio;
}

void TVisionDialog::ChangeFitGaussWithOffset() {
  FitGaussWithOffset=!FitGaussWithOffset;
}

void TVisionDialog::ChangeFitGaussWithSlope() {
  FitGaussWithSlope=!FitGaussWithSlope;
}

void TVisionDialog::Change1DFitWithAmplitude() {
  Fit1DWithAmplitude=!Fit1DWithAmplitude;
}

void TVisionDialog::Change1DFitWithWidth() {
  Fit1DWithWidth=!Fit1DWithWidth;
}

void TVisionDialog::ChangeFit2DWithOffset() {
  Fit2DWithOffset=!Fit2DWithOffset;
}

void TVisionDialog::ChangeFit2DWithSlope() {
  Fit2DWithSlope=!Fit2DWithSlope;
}

void TVisionDialog::ChangeFit2DWithAngle() {
  Fit2DWithAngle=!Fit2DWithAngle;
}

void TVisionDialog::ChangeFit2DWithLongitudinalFlightCorrection() {
  Fit2DWithLongitudinalFlightCorrection=!Fit2DWithLongitudinalFlightCorrection;
}

void TVisionDialog::SetInitialiseIoffeFitFromGaussFit() {
  InitialiseIoffeFit=0;
}

void TVisionDialog::SetInitialiseIoffeFitFromFile() {
  InitialiseIoffeFit=1;
}

void TVisionDialog::SetInitialiseIoffeFitFromLastFit() {
  InitialiseIoffeFit=2;
}

void TVisionDialog::ChangeGaussXAmpEqualYAmp() {
  FitGaussXAmpEqualYAmp=!FitGaussXAmpEqualYAmp;
}

void TVisionDialog::ChangeUpdateMeasurementAfterFit() {
  UpdateMeasurementAfterFit=!UpdateMeasurementAfterFit;
}

void TVisionDialog::ChangeDisplayMathErrors() {
  DisplayMathErrors=!DisplayMathErrors;
  SetMathErrorMessage(DisplayMathErrors);
}

void TVisionDialog::ChangeFit2DOnlyNotFitted() {
  Fit2DOnlyNotFitted=!Fit2DOnlyNotFitted;
}

void TVisionDialog::ChangeDisplay2DFitProfil() {
  Display2DFitProfil=!Display2DFitProfil;
}

void TVisionDialog::ChangeOptDensFromSample() {
  OptDensSource=0;
}

void TVisionDialog::ChangeOptDensFromXandYGaussFit() {
  OptDensSource=1;
}

void TVisionDialog::ChangeOptDensFromXGaussFit() {
  OptDensSource=2;
}

void TVisionDialog::ChangeOptDensFromYGaussFit() {
  OptDensSource=3;
}

void TVisionDialog::CeIntegralWithOffset(TCommandEnabler& ce) {
  ce.SetCheck(IntegralWithOffset ? TCommandEnabler::Checked : TCommandEnabler::Unchecked);
}

void TVisionDialog::CeFermiFitHorizontal(TCommandEnabler& ce) {
  ce.SetCheck(FitFermiHorizontal ? TCommandEnabler::Checked : TCommandEnabler::Unchecked);
}

void TVisionDialog::CeFermiZeroTemp(TCommandEnabler& ce) {
  ce.SetCheck(FitFermiZeroTemp ? TCommandEnabler::Checked : TCommandEnabler::Unchecked);
}

void TVisionDialog::CePartialFermiFitL(TCommandEnabler& ce) {
  ce.SetCheck(FitPartialFermiL ? TCommandEnabler::Checked : TCommandEnabler::Unchecked);
}

void TVisionDialog::CePartialFermiFitTemperature(TCommandEnabler& ce) {
  ce.SetCheck(FitPartialFermiTemp ? TCommandEnabler::Checked : TCommandEnabler::Unchecked);
}

void TVisionDialog::CePartialFermiFitNumber(TCommandEnabler& ce) {
  ce.SetCheck(FitPartialFermiNumber ? TCommandEnabler::Checked : TCommandEnabler::Unchecked);
}

void TVisionDialog::CeWithNumberRatio(TCommandEnabler& ce) {
  ce.SetCheck(FitWithNumberRatio ? TCommandEnabler::Checked : TCommandEnabler::Unchecked);
}

void TVisionDialog::CeGaussFitWithOffset(TCommandEnabler& ce) {
  ce.SetCheck(FitGaussWithOffset ? TCommandEnabler::Checked : TCommandEnabler::Unchecked);
}

void TVisionDialog::CeGaussFitWithSlope(TCommandEnabler& ce) {
  ce.SetCheck(FitGaussWithSlope ? TCommandEnabler::Checked : TCommandEnabler::Unchecked);
}

void TVisionDialog::Ce1DFitWithAmplitude(TCommandEnabler& Ce) {
  Ce.SetCheck(Fit1DWithAmplitude ? TCommandEnabler::Checked : TCommandEnabler::Unchecked);
}

void TVisionDialog::Ce1DFitWithWidth(TCommandEnabler& Ce) {
  Ce.SetCheck(Fit1DWithWidth ? TCommandEnabler::Checked : TCommandEnabler::Unchecked);
}

void TVisionDialog::Ce2DFitWithOffset(TCommandEnabler& Ce) {
  Ce.SetCheck(Fit2DWithOffset ? TCommandEnabler::Checked : TCommandEnabler::Unchecked);
}

void TVisionDialog::Ce2DFitWithSlope(TCommandEnabler& Ce) {
  Ce.SetCheck(Fit2DWithSlope ? TCommandEnabler::Checked : TCommandEnabler::Unchecked);
}

void TVisionDialog::Ce2DFitWithAngle(TCommandEnabler& Ce) {
  Ce.SetCheck(Fit2DWithAngle ? TCommandEnabler::Checked : TCommandEnabler::Unchecked);
}

void TVisionDialog::Ce2DFitWithLongitudinalFlightCorrection(TCommandEnabler& Ce) {
  Ce.SetCheck(Fit2DWithLongitudinalFlightCorrection ? TCommandEnabler::Checked : TCommandEnabler::Unchecked);
}

void TVisionDialog::CeGaussXAmpEqualYAmp(TCommandEnabler& ce) {
  ce.SetCheck(FitGaussXAmpEqualYAmp ? TCommandEnabler::Checked : TCommandEnabler::Unchecked);
}

void TVisionDialog::CeOptDensFromSample(TCommandEnabler& ce) {
  ce.SetCheck(OptDensSource==0 ? TCommandEnabler::Checked : TCommandEnabler::Unchecked);
}

void TVisionDialog::CeOptDensFromXandYGaussFit(TCommandEnabler& ce) {
  ce.SetCheck(OptDensSource==1 ? TCommandEnabler::Checked : TCommandEnabler::Unchecked);
}

void TVisionDialog::CeOptDensFromXGaussFit(TCommandEnabler& ce) {
  ce.SetCheck(OptDensSource==2 ? TCommandEnabler::Checked : TCommandEnabler::Unchecked);
}

void TVisionDialog::CeOptDensFromYGaussFit(TCommandEnabler& ce) {
  ce.SetCheck(OptDensSource==3 ? TCommandEnabler::Checked : TCommandEnabler::Unchecked);
}

void TVisionDialog::CeOptDensNormal(TCommandEnabler& ce) {
  ce.SetCheck(OptDensMethod==0 ? TCommandEnabler::Checked : TCommandEnabler::Unchecked);
}

void TVisionDialog::CeOptDensAlice(TCommandEnabler& ce)
{
  ce.SetCheck(OptDensMethod==1 ? TCommandEnabler::Checked : TCommandEnabler::Unchecked);
}

void TVisionDialog::CeLiveConnection(TCommandEnabler& ce)
{
  ce.SetCheck(LiveConnected ? TCommandEnabler::Checked : TCommandEnabler::Unchecked);
}

void TVisionDialog::CeGridOnOff(TCommandEnabler& ce)
{
  ce.SetCheck(IsGridOnOff ? TCommandEnabler::Checked : TCommandEnabler::Unchecked);
}

void TVisionDialog::CeToggleProfilDesign(TCommandEnabler& ce)
{
  ce.SetCheck(ProfilDesign? TCommandEnabler::Checked : TCommandEnabler::Unchecked);
}

void TVisionDialog::CeFixCursor(TCommandEnabler& ce)
{
  ce.SetCheck(CursorFixed==1? TCommandEnabler::Checked : TCommandEnabler::Unchecked);
}

void TVisionDialog::CeUpdateMeasurementAfterFit(TCommandEnabler& ce)
{
  ce.SetCheck(UpdateMeasurementAfterFit? TCommandEnabler::Checked : TCommandEnabler::Unchecked);
}

void TVisionDialog::CeDisplayMathErrors(TCommandEnabler& ce)
{
  ce.SetCheck(DisplayMathErrors? TCommandEnabler::Checked : TCommandEnabler::Unchecked);
}

void TVisionDialog::CeFit2DOnlyNotFitted(TCommandEnabler& ce)
{
  ce.SetCheck(Fit2DOnlyNotFitted? TCommandEnabler::Checked : TCommandEnabler::Unchecked);
}

void TVisionDialog::CeDisplay2DFitProfil(TCommandEnabler& ce)
{
  ce.SetCheck(Display2DFitProfil? TCommandEnabler::Checked : TCommandEnabler::Unchecked);
}

void TVisionDialog::CeInitialiseIoffeFitFromGaussFit(TCommandEnabler& ce)
{
  ce.SetCheck((InitialiseIoffeFit==0)? TCommandEnabler::Checked : TCommandEnabler::Unchecked);
}

void TVisionDialog::CeInitialiseIoffeFitFromFile(TCommandEnabler& ce)
{
  ce.SetCheck((InitialiseIoffeFit==1)? TCommandEnabler::Checked : TCommandEnabler::Unchecked);
}

void TVisionDialog::CeInitialiseIoffeFitFromLastFit(TCommandEnabler& ce)
{
  ce.SetCheck((InitialiseIoffeFit==2)? TCommandEnabler::Checked : TCommandEnabler::Unchecked);
}

void TVisionDialog::SetProbeLibraryROI() {
  double posx1,posx2,posy1,posy2;
  double posx1i,posx2i,posy1i,posy2i;
  profilH->GetPos(posx1,posx2,1);
  profilV->GetPos(posy1,posy2,1);
  profilH->GetPos(posx1i,posx2i);
  profilV->GetPos(posy1i,posy2i);
//  if (NetAltaProbeLibrary) NetAltaProbeLibrary->SetRegionOfInterest(posx1,posx2,posy1,posy2,posx1i,posx2i,posy1i,posy2i);
  picture->Line(posx1,posy1,posx1,posy2,62000000);//red outer
  picture->Line(posx1,posy2,posx2,posy2,62000000);
  picture->Line(posx2,posy2,posx2,posy1,62000000);
  picture->Line(posx2,posy1,posx1,posy1,62000000);
  picture->Line(posx1i,posy1i,posx1i,posy2i,32000000);//green  inner
  picture->Line(posx1i,posy2i,posx2i,posy2i,32000000);
  picture->Line(posx2i,posy2i,posx2i,posy1i,32000000);
  picture->Line(posx2i,posy1i,posx1i,posy1i,32000000);
  PictureWin->Update(MD->y("ProfilAverage"));
}

void TVisionDialog::CeEnableProbeLibrary(TCommandEnabler& ce)
{
  ce.SetCheck((MD->y("UseProbeLibrary"))? TCommandEnabler::Checked : TCommandEnabler::Unchecked);
}

void TVisionDialog::EnableProbeLibrary() {
  if (MD->y("UseProbeLibrary")>0) MD->y("UseProbeLibrary")=0; else MD->y("UseProbeLibrary")=1;
}

void TVisionDialog::MovePicturesToDetailPictureButtons(unsigned long c) {
   //c==0: details of single picture
   //c==1: set of pictures
   if (c==0) {
  		PictureOfDetailButton[0]=pictureabs;
  		PictureOfDetailButton[1]=pictureprobe;
  		PictureOfDetailButton[2]=abspicture;
  		PictureOfDetailButton[3]=picturenoiseabs;
  		PictureOfDetailButton[4]=picturenoiseprobe;
  		PictureOfDetailButton[5]=opticalDensity;
   } else {
      for (int i=0;i<NrDetailPictureButtons;i++) {
      	PictureOfDetailButton[i]=MainPicture[i];
      }
   }
}

void TVisionDialog::MovePicturesToDetailPictureButtonsAndRedraw() {
   unsigned long c=DetailButton->GetChecked();
   MovePicturesToDetailPictureButtons(c);
   for (int i=0;i<NrDetailPictureButtons;i++) {
   	DetailPictureButtonTitle[i]->SetText(DetailPictureButtonText[c][i]);
   }
  	for (int i=0;i<NrDetailPictureButtons;i++) DetailPictureButton[i]->Draw(1);
}

void TVisionDialog::CreateSubstractNoisePictures() {
  SubstractNoise(*pictureabs,*pictureprobe,*picturenoiseabs,*abspicture,*opticalDensity);
  MD->y("2DFitFunction")=0;
  MovePicturesToDetailPictureButtonsAndRedraw();
  DetailPictureButtonTitle[0]->SetText("abs-noise");
  DetailPictureButtonTitle[1]->SetText("probe-noise");
  ShowAbsorption();
}

void TVisionDialog::GridOnOff() {
  IsGridOnOff=!IsGridOnOff;
  PictureWin->GridOnOff();
  profilH->GridOnOff();
  profilV->GridOnOff();
};

void TVisionDialog::MandelBrot() {
  mb->SetText("Calculating...");
  picture->MandelPicture();
  mb->SetText("");
  UpdateAll();
};

void TVisionDialog::Gauss() {
  picture->GaussPicture();
  UpdateAll();
};

void TVisionDialog::SavePicture() {
  if (picture->bitplanes==8) SavePictureAsBMP();
  else SavePictureAsBWTGA();
};

void TVisionDialog::SavePictureAsBMPNoBinning() {
	SavePictureAsBMP(1);
}

void TVisionDialog::SavePictureAsBMP2Binning() {
	SavePictureAsBMP(2);
}

void TVisionDialog::SavePictureAsBMP3Binning() {
	SavePictureAsBMP(3);
}

void TVisionDialog::SavePictureAsBMP4Binning() {
	SavePictureAsBMP(4);
}

void TVisionDialog::SavePictureAsBMP2FloatingBinning() {
	SavePictureAsBMP(2,true);
}

void TVisionDialog::SavePictureAsBMP3FloatingBinning() {
	SavePictureAsBMP(3,true);
}

void TVisionDialog::SavePictureAsBMP4FloatingBinning() {
	SavePictureAsBMP(4,true);
}

void TVisionDialog::SavePictureAsBMP(int binning,bool floatingbinning) {
  if (BMPFileData->FileName[0]!=0) {
    BMPFileData->InitialDir=NULL;
  }
  if (TFileSaveDialog(this, *BMPFileData, 0,"Save BMP Picture").Execute()==IDOK) {
	 picture->Save(BMPFileData->FileName,SAVE_PALETTEBMP,ColorTable,binning,floatingbinning);
	 mb->SetText("Picture saved as palette BMP");
	 CheckDiskSpace();
  }
};

void TVisionDialog::SavePictureAsMatrix() {
  if (MatrixFileData->FileName[0]!=0) {
    MatrixFileData->InitialDir=NULL;
  }
  if (TFileSaveDialog(this, *MatrixFileData, 0,"Save Picture as Matrix").Execute()==IDOK) {
	 picture->Save(MatrixFileData->FileName,SAVE_MATRIX,ColorTable);
	 mb->SetText("Picture saved as Matrix");
	 CheckDiskSpace();
  }
};

void TVisionDialog::SaveLowByteAsBMP() {
  if (BMPFileData->FileName[0]!=0) {
    BMPFileData->InitialDir=NULL;
  }
  if (TFileSaveDialog(this, *BMPFileData, 0,"Save low byte as BMP Picture").Execute()==IDOK) {
	 picture->Save(BMPFileData->FileName,SAVE_LSBPALETTEBMP,ColorTable);
	 mb->SetText("Low byte of picture saved as palette BMP");
	 CheckDiskSpace();
  }
};

void TVisionDialog::SavePictureAsRGBTGA() {
  if (TGAFileData->FileName[0]!=0) {
    TGAFileData->InitialDir=NULL;
  }
  if (TFileSaveDialog(this, *TGAFileData, 0,"Save truecolor TGA Picture").Execute()==IDOK) {
	 picture->Save(TGAFileData->FileName,SAVE_TRUECOLORTGA,ColorTable);
	 mb->SetText("Picture saved as truecolor TGA");
	 CheckDiskSpace();
  }
};

void TVisionDialog::SavePictureAsBWTGA() {
  if (TGAFileData->FileName[0]!=0) {
    TGAFileData->InitialDir=NULL;
  }
  if (TFileSaveDialog(this, *TGAFileData, 0,"Save grayscale TGA Picture").Execute()==IDOK) {
	 picture->Save(TGAFileData->FileName,SAVE_GRAYSCALETGA,ColorTable);
	 mb->SetText("Picture saved as grayscale TGA");
	 CheckDiskSpace();
  }
};

void TVisionDialog::SavePictureAs(char *name,int aswhat) {
  int as;
  switch (aswhat) {
    case 0:as=SAVE_AUTOMATIC;break;
    case 1:as=SAVE_PALETTEBMP;break;
    case 2:as=SAVE_LSBPALETTEBMP;break;
    case 3:as=SAVE_GRAYSCALETGA;break;
    case 4:as=SAVE_TRUECOLORTGA;break;
    default: as=aswhat;
  }
  picture->Save(name,as,ColorTable);
};

void TVisionDialog::LoadBMPPicture() {
  if (BMPFileData->FileName[0]!=0) {
    BMPFileData->InitialDir=NULL;
  }
  if (TFileOpenDialog(this, *BMPFileData, 0,"Load BMP Picture").Execute()==IDOK) {
	 if (picture->Load(BMPFileData->FileName)==0) {
	   UpdateAll();
	   ColorTable->Update();
	   mb->SetText("BMP Picture loaded");
    } else MessageBox("Error loading BMP picture!","Error!");
  }
};

void TVisionDialog::LoadTGAPicture() {
  if (TGAFileData->FileName[0]!=0) {
    TGAFileData->InitialDir=NULL;
  }
  if (TFileOpenDialog(this, *TGAFileData, 0,"Load TGA Picture").Execute()==IDOK) {
	 if (picture->Load(TGAFileData->FileName)==0) {
	   UpdateAll();
	   ColorTable->Update();
	   mb->SetText("TGA Picture loaded");
    } else MessageBox("TGA picture not loaded!\nPossibly truecolor picture.","Error!");
  }
};

void TVisionDialog::LoadMeasurement() {
  if (MESFileData->FileName[0]!=0) {
    MESFileData->InitialDir=NULL;
  }
  if (TFileOpenDialog(this, *MESFileData, 0,"Load Measurement").Execute()==IDOK)
	 LoadMeasurement(MESFileData->FileName);
}

void TVisionDialog::SaveProfil() {
  if (TFileSaveDialog(this, *ProfilFileData, 0,"Save Profil").Execute()==IDOK)
	 profilH->Save(ProfilFileData->FileName,WV,WH);
    profilV->Save(ProfilFileData->FileName,WV,WH);
  CheckDiskSpace();
}

void TVisionDialog::LoadSerie() {
  if (SerieFileData->FileName[0]!=0) {
    SerieFileData->InitialDir=NULL;
  }
  if (TFileOpenDialog(this, *SerieFileData, 0,"Load Serie").Execute()==IDOK) {
    LoadSerie(SerieFileData->FileName);
  }
}

void TVisionDialog::AddSerieAllCameras() {
  LoadAddSerieAllCameras(1);
}

void TVisionDialog::LoadSerieAllCameras() {
  LoadAddSerieAllCameras(0);
}

void TVisionDialog::LoadAddSerieAllCameras(int LoadAdd) {  //0:Load 1: Add
  if (SerieFileData->FileName[0]!=0) {
    SerieFileData->InitialDir=NULL;
  }
  if (TFileOpenDialog(this, *SerieFileData, 0,(LoadAdd) ? "Add Serie All Cameras" : "Load Serie All Cameras").Execute()==IDOK) {
		char name[500];
      strcpy(name,SerieFileData->FileName);
      //Serie names have the format Datax.dat where x is A,B,C,D,...
      //we cycle through all those names and load the corresponding data
      int i=strlen(name)-5;
      if (i<0) return;
      name[i]='?';
      struct ffblk ffblk;
      int done = findfirst(name,&ffblk,FA_DIREC);
	  	while (!done) {
         int j=strlen(ffblk.ff_name)-5;
         if (j<0) return;
         name[i]=ffblk.ff_name[j];
         if (LoadAdd) TNmeasure->Load(name,1);
         else {
           LoadSerie(name);
           LoadAdd=1;  //after first serie has been loaded we add other cameras
         }
      	done = findnext(&ffblk);
      }
  }
}

void TVisionDialog::LoadSerie(char* FileName) {
  TNmeasure->Load(FileName,0);
  if (LogbookEdit) {
     int i=strlen(FileName);
     while ((i>0) && (FileName[i]!='\\')) i--;
     char buf[4096],buf2[200];
     strcpy(buf,FileName);
     buf[i]=0;
     sprintf(buf2,"%s\\%s",buf,"Logbook.txt");
     ifstream *in=new ifstream(buf2,ios::binary);
     if (!in->fail()) {
       LogbookEdit->Clear();
       while (!in->eof()) {
         in->read(buf,4095);
         buf[in->gcount()]=0;
         LogbookEdit->Insert(buf);
       }
     }
     delete in;
   }
}

void TVisionDialog::AddSerie() {
  if (SerieFileData->FileName[0]!=0) {
    SerieFileData->InitialDir=NULL;
  }
  if (TFileOpenDialog(this, *SerieFileData, 0,"Add Serie").Execute()==IDOK)
	 TNmeasure->Load(SerieFileData->FileName,1);
}

void TVisionDialog::NewSerie() {
  TNmeasure->New();
  TNmeasure->Draw();
}

void TVisionDialog::SaveSerie() {
  if (SerieFileData->FileName[0]!=0) {
    SerieFileData->InitialDir=NULL;
  }
  if (TFileSaveDialog(this, *SerieFileData, 0,"Save Serie").Execute()==IDOK)
	 TNmeasure->SaveSerie(SerieFileData->FileName);
  CheckDiskSpace();
}

void TVisionDialog::StartSerie() {
  InSerie=1;
  MD->SetMetaPointer(NULL);
  if (!MD->IsInList()) {
    #if defined(DetectLeaks)
      LeakDetect.Delete(MD);
    #endif
    delete MD;
  }
  MD=new CMessPoint();
  #if defined(DetectLeaks)
    LeakDetect.New(MD,5,54);
  #endif
  MD->SetMetaPointer(MetaPointer);
  MDSetCameraParameters();
  char **ParamNames=NULL;
  try {
	  ParamNames=new char* [1];
	  ParamNames[0]=new char[200];
  }
  catch (xalloc) {
  		MessageBox("TVisionDialog::StartSerie : couldn't reserve memory","Error",  MB_OK);
      exit(-1);
  }
  strcpy(ParamNames[0],"Parameter");
  MD->SetNewXNames(1,ParamNames);
  delete[] ParamNames[0];
  delete[] ParamNames;
  TNmeasure->StartSerie("free Serie",MD);
}

void TVisionDialog::StopSerie() {
  InSerie=0;
  TNmeasure->StopSerie();
}

void TVisionDialog::ContinueSerie() {
  InSerie=1;
  TNmeasure->ContinueSerie();
}


void TVisionDialog::SortSerie() {
  TNmeasure->SortSerie();
  mb->SetText("Serie sorted");
}

//standard call:   (e.g. used when load menu opened
//void LoadMeasurement(const char *aname,bool insert=true,bool DataAlreadyLoaded=false,
//    int LoadIsotop=0, bool OnlyOne=false, bool SpareMD=false);
//called from LoadMess if parameter passed is 1:
//LoadMeasurement(TNmeasure->GetMesName(),true,true,iso,true,false);
//when point pressed in TNmeasure:
//aname=path\\PxxxxCA.TGA
//insert=true
//DataAlreadyLoaded=true or false in dependance of updatefitaftermeasurement
//LoadIsotop=0
//OnlyOne=false
//SpareMD=true
//called when picture list clicked:
//LoadMeasurement(PictureList[newpicture-ID_PICLIST0]->GetName(l),false,false,newpicture-ID_PICLIST0,true);

void TVisionDialog::LoadMeasurement(const char *aname,bool insert,bool DataAlreadyLoaded,int LoadIsotop, bool OnlyOne, bool SpareMD) {
//insert decides if this newly loaded measurement should be inserted into the picture list
//Only one loads just this one datapoint. This is used if a button in the picturelist has been clicked
//DataAlreadyLoaded is true if a point from TNmeasure should be loaded
  profilV->SetGeneralProfil();
  profilH->SetGeneralProfil();
  FitButtonFunction=0;
  FirstFermiFit=1;
  char dataname[200];
  char path[200];
  char nrbuf[5];
  int i=strlen(aname);
  while ((i>0) && (aname[i]!='\\')) i--;
  if ((i>199) || (i==0)) return;
  char cam=aname[i+1];
 // char typ=aname[i+2];
  strncpy(path,aname,i+1);
  path[i+1]=0;
  strncpy(nrbuf,&aname[i+2],4);
  nrbuf[4]=0;
  int PicNr=atoi(nrbuf);
  sprintf(dataname,"%sdata%c.dat",path,aname[i+6]);
  ifstream *in=new ifstream(dataname);
  if (in->fail()) {
	 char buf[200];
	 sprintf(buf,"Could not open file %s",dataname);
	 MessageBox(buf,"Error");
    delete in;
	 return;
  }
  if (LogbookEdit) {
    /*int nr=atoi(nrbuf)+1;
    char nrbuf2[60];
    char h[50];
    sprintf(nrbuf2,"%04u\0",nr);
    //strcpy(nrbuf2,"0000");
    for (int i=0; i<strlen(h);i++) {
      if (((4-strlen(h)+i)>=0) || ((4-strlen(h)+i)<60))
		   nrbuf2[4-strlen(h)+i]=h[i];
      else return;
    }
    char buf[100],buf2[100];
    strncpy(buf,&aname[i+1],8);
    buf[8]=0;
    sprintf(buf2,"* %s",buf);
    strcpy(buf,buf2);
    //for (int i=0; i<4;i++) buf[4+i]=nrbuf2[i];
    LogbookEdit->Search(0,buf);
    LogbookEdit->Search(0,buf2); */
    char buf[200];
    sprintf(buf,"* P%04u",PicNr);
    LogbookEdit->Search(0,buf);
  }
  char buf[1000];
  in->getline(buf,1000);
  if (!DataAlreadyLoaded) {
    MD->SetMetaPointer(NULL);
    int err;
    if (!MD->IsInList()) {
      #if defined(DetectLeaks)
        LeakDetect.Delete(MD);
      #endif
      delete MD;
    }
    MD=new CMessPoint(in,err);
    #if defined(DetectLeaks)
      LeakDetect.New(MD,5,55);
    #endif
    MD->SetMetaPointer(MetaPointer);
    if (err!=0) {
      MD->SetMetaPointer(NULL);
      #if defined(DetectLeaks)
        LeakDetect.Delete(MD);
      #endif
      delete MD;
      MD=new CMessPoint();
      #if defined(DetectLeaks)
        LeakDetect.New(MD,5,56);
      #endif
      MD->SetMetaPointer(MetaPointer);
      MDSetCameraParameters();
      MessageBox("Not a Vision Serie!","Error");
      delete in;
      return;
    }
    if (MD->LoadPoint(in,PicNr)==0) {
      MD->AddDefaultValues(&CMessPoint());
      MD->OrderParameters(&CMessPoint());
      char path[300];
      strcpy(buf,MD->GetText("Filename"));
      buf[0]=aname[0];
      MD->SetText("Filename",buf);
    } else {
      MD->SetMetaPointer(NULL);
      #if defined(DetectLeaks)
        LeakDetect.Delete(MD);
      #endif
      delete MD;
      MD=new CMessPoint();
      #if defined(DetectLeaks)
        LeakDetect.New(MD,5,57);
      #endif
      MD->SetMetaPointer(MetaPointer);
      MDSetCameraParameters();
      MessageBox("Problem loading measurement data!","Error");
      delete in;
      return;
    }
  }
    for (int Nr=0;Nr<NrUsedROIs;Nr++) {
		char buf[300];
		sprintf(buf,"NROI%i",Nr);
		MD->y(buf)=0;
	 }
 	if ((!OnlyOne) && insert) {
      int LastPicturePosition=-1;
      bool FirstPicture=true;
      //we have to load all other pictures which belong to the same dataset than this picture and display it in the picturelist
  		for (int y=0;y<MaxPictureLines;y++) {
     		for (int x=0;x<NrPictureLists;x++) {
            int PicturePos=y*NrPictureLists+x;
            //we have to determin which other pictures have been taken together with this picture
            //for this we look on the hard disk which pictures with the same name exist
            //Format:
				//PnnnnCAp.TGA
            //nnnn:Number
            //C:  A,B,C... Camera
            //p: which picturelist

            //first we have to go to the correct subdirectory
            chdir(path);

            struct ffblk ffblk;
            char name[200];
            sprintf(name,"P%04u?A%u.TGA",PicNr,PicturePos);
            int done = findfirst(name,&ffblk,0);
            if (!done) {
               LastPicturePosition=PicturePos;
            	char buf[300];
               sprintf(buf,"%s%s",path,ffblk.ff_name);
  					i=strlen(buf)-4;
  					while ((i>0) && buf[i]!='A' && buf[i]!='a') i--;
  					int err1=pictureabs->Load(buf);
  					buf[i]='P';
  					int err2=pictureprobe->Load(buf);
  					buf[i]='N';
  					int err3=picturenoiseabs->Load(buf);
  					buf[i]='O';
  					int err4=picturenoiseprobe->Load(buf);
  					if (err1 || err2 || err3 || err4) MessageBox("TVisionDialog::LoadMeasurement : Problems loading measurement 1!","Error");
					GetProfilPos();
               double posx1,posx2,posy1,posy2;
               profilH->GetPos(posx1,posx2);
               profilV->GetPos(posy1,posy2);
               LnNormalizeAbsorption(*pictureabs,*pictureprobe,picturenoiseabs,picturenoiseprobe,
                  *abspicture,*opticalDensity,MaxOptDensity,posx1,posx2,posy1,posy2,MD->y("Fluo"),MD->y("MaxFluo"),DoIntensityCompensation,&ROI[8*8],WV,WH,MD->y("FastKinetics"));
               TreatPicture(/*SaveData*/false,/*insert*/false,/*NewLine*/ false);
               bool NewLine=(x==0) || FirstPicture;
               FirstPicture=false;
               SelectMainPicture();
               PictureList[x]->AddPicture(picture,buf,/*MD->GetText("AtomicName")*/"",/*MD->GetText("CameraDirection")*/"",NewLine);
               #if defined(DetectLeaks)
                 LeakDetect.Delete(MainPicture[PicturePos]);
               #endif
               if (MainPicture[PicturePos]) delete MainPicture[PicturePos];
               MainPicture[PicturePos]=new CPicture(*picture);
               #if defined(DetectLeaks)
                 LeakDetect.New(MainPicture[PicturePos],5,58);
               #endif
            } else {
               #if defined(DetectLeaks)
                 LeakDetect.Delete(MainPicture[PicturePos]);
               #endif
               if (MainPicture[PicturePos]) delete MainPicture[PicturePos];
               MainPicture[PicturePos]=new CPicture(1,0);
               #if defined(DetectLeaks)
                 LeakDetect.New(MainPicture[PicturePos],5,59);
               #endif
            }
         }
      }
      if ((LastPicturePosition>=0) && (PictureList[LastPicturePosition])) PictureList[LastPicturePosition]->SynchronizePosition();
  	}

  strcpy(buf,aname);
  i=strlen(aname)-6;
  //while ((i>0) && buf[i]!='A' && buf[i]!='a') i--;
  buf[i]='A';
  int err1=pictureabs->Load(buf);
  buf[i]='P';
  int err2=pictureprobe->Load(buf);
  buf[i]='N';
  int err3=picturenoiseabs->Load(buf);
  buf[i]='O';
  int err4=picturenoiseprobe->Load(buf);
  if (err1 || err2 || err3 || err4) MessageBox("TVisionDialog::LoadMeasurement : Problems loading measurement 2 !","Error");
  if (MD->y("BiningX")<1) MD->y("BiningX")=1;
  double EichX=MD->y("EichX")/MD->y("BiningX");
  double EichY=MD->y("EichY")/MD->y("BiningY");
  pictureabs->SetEich(EichX,EichY);
  pictureprobe->SetEich(EichX,EichY);
  picturenoiseabs->SetEich(EichX,EichY);
  picturenoiseprobe->SetEich(EichX,EichY);
  UpdateAll();
//  MovePicturesToDetailPictureButtonsAndRedraw();
  //Exploit Images:
  GetProfilPos();
  double posx1,posx2,posy1,posy2;
  profilH->GetPos(posx1,posx2);
  profilV->GetPos(posy1,posy2);
  LnNormalizeAbsorption(*pictureabs,*pictureprobe,picturenoiseabs,picturenoiseprobe,
  		*abspicture,*opticalDensity,MaxOptDensity,
      posx1,posx2,posy1,posy2,MD->y("Fluo"),MD->y("MaxFluo"),DoIntensityCompensation,&ROI[8*8],WV,WH,MD->y("FastKinetics"));
  TreatPicture(/*SaveData*/false,/*insert*/false,/*NewLine*/ false);
  DisplayMessData();
  TNmeasure->Draw();
  mb->SetText(aname);
  if (UpdateMeasurementAfterFit && MD->IsInList()) {
    TNmeasure->Draw();
  }
  delete in;
}

void TVisionDialog::ReduceFringes() {

	ofstream out("FringeReductionDebug.txt");
	out<<endl;
	out.close();

	//iterate over all pictures in measurement list
	char path[200];
	char cam;
	if (TNmeasure->points==NULL) return;
	CMessPoint *hpoint;
	hpoint=(CMessPoint *)(TNmeasure->points);
	while (hpoint!=NULL) {
		char MesName[200];
		sprintf(MesName,hpoint->GetText("Filename"),'A');
		MesName[0]=TNmeasure->disk;
		int MesIsotop=hpoint->y("Camera");
		if (hpoint->Enabled()) {

			int i=strlen(MesName);
			while ((i>0) && (MesName[i]!='\\')) i--;
			if ((i>199) || (i==0)) return;
			cam=MesName[i+6]; //P0001cAp.TGA
			char picpos=MesName[i+8];
			char nrbuf[10];
			strncpy(nrbuf,&MesName[i+2],4);
			nrbuf[4]=0;
			int PicNr=atoi(nrbuf);
			strncpy(path,MesName,i+1);
			path[i+1]=0;

			//load picture with atoms and its noise picture
			chdir(path);

			char buf[300];
			strcpy(buf,MesName);
			i=strlen(buf)-4;
			while ((i>0) && buf[i]!='A' && buf[i]!='a') i--;
			if (i==0) MessageBox("TVisionDialog::ReduceFringes : Problems loading measurement 1!","Error");
			int err1=pictureabs->Load(buf);
			buf[i]='N';
			int err2=picturenoiseabs->Load(buf);
			if (err1 || err2) MessageBox("TVisionDialog::ReduceFringes : Problems loading measurement 2!","Error");

			//iterate over all reference images of the same camera in the same folder as this measurement and then over all images in reference image folder

			//bool SameFolder=true;

			double MinNoise;
			char MinNoiseName[500];
         char MinPos;
         MinNoise=1E99;
			MinNoiseName[0]=0;
			char name[500];
			//Only the neighboring pictures might contain reference pictures fitting with this picture
			int AktPicNr=PicNr-5;
			if (AktPicNr<=0) AktPicNr=1;
			while (AktPicNr<=(PicNr+5)) {
				strcpy(name,MesName);
				//Format: C:\\Verzeichnis\\..\\P????XAY.TGA
				//X=A,B,C,D,... label camera and Y=1,2,3,... labels picture position
				i=strlen(name);
				if (i<12) return;
				char PicNrBuf[10];
				sprintf(PicNrBuf,"%04u",AktPicNr);
				for (int k=0;k<4;k++) name[i-11+k]=PicNrBuf[k];
            unsigned char OldPos=name[i-5];
				name[i-5]='?';
            name[i-6]='P';

				struct ffblk ffblk;
				int done = findfirst(name,&ffblk,0);
  				while (!done) {

					//load picture without atoms and its noise picture
					char buf[300];
					strcpy(buf,ffblk.ff_name);
					i=strlen(buf)-4;
					while ((i>0) && buf[i]!='P' && buf[i]!='p') i--;
					if (i==0) MessageBox("TVisionDialog::ReduceFringes : Problems loading measurement 3!","Error");
               int err2=pictureprobe->Load(buf);
					buf[i]='O';
              	buf[i+1]=OldPos;
					int err1=picturenoiseprobe->Load(buf);


					if (err1 || err2) MessageBox("TVisionDialog::ReduceFringes : Problems loading measurement 4!","Error");
					//calculate optical density picture

					double posx1,posx2,posy1,posy2;
					double posx1b,posx2b,posy1b,posy2b;
					profilV->GetPos(posy1,posy2,0);
					profilH->GetPos(posx1,posx2,0);
					profilV->GetPos(posy1b,posy2b,1);
					profilH->GetPos(posx1b,posx2b,1);

					LnNormalizeAbsorption(*pictureabs,*pictureprobe,picturenoiseabs,picturenoiseprobe,
											*abspicture,*opticalDensity,MaxOptDensity,
										 posx1,posx2,posy1,posy2,/*Flue*/0,/*MaxFluo*/1000,DoIntensityCompensation,&ROI[8*8],WV,WH,true);
					UpdateAll();

					//determine noise level
					double AktNoise;
					AktNoise=picture->RMSNoise(posx1,posy1,posx2,posy2,
												IntegralWithOffset,posx1b,posy1b,posx2b,posy2b,
												WV,WH);



					if (AktNoise<MinNoise) {
						MinNoise=AktNoise;
						strcpy(MinNoiseName,ffblk.ff_name);
                  MinPos=OldPos;

						/*ofstream out("FringeReductionDebug.txt",ios::app);
						out<<"AktPicture="<<ffblk.ff_name<<endl;
						out<<"AktNoise="<<AktNoise<<endl;
						out.close();*/

					}
  					done = findnext(&ffblk);

					/*if ((done) && (SameFolder)) {
						SameFolder=false;
						char ReferenceName[300];
						sprintf(ReferenceName,"%s\\ReferenceLibrary\\P????%cA?.TGA",MainDirectory,cam);
						done = findfirst(name,&ffblk,0);
					}*/
				}
				AktPicNr++;
			}
			//Now we know the best reference image.
			if (MinNoiseName[0]!=0) {


				ofstream out("FringeReductionDebug.txt",ios::app);
				out<<"ImageAnalyzed="<<nrbuf<<endl;
				out<<"ResultPicture="<<MinNoiseName<<endl;
				out<<"ResultNoise="<<MinNoise<<endl;
            out<<endl;
				out.close();


				//load good reference picture
				//load picture without atoms and its noise picture
				char buf[300];
				strcpy(buf,MinNoiseName);
				i=strlen(buf)-4;
				while ((i>0) && buf[i]!='P' && buf[i]!='p') i--;
				if (i==0) MessageBox("TVisionDialog::ReduceFringes : Problems loading measurement 7!","Error");
            int err2=pictureprobe->Load(buf);
				buf[i]='O';
            buf[i+1]=MinPos;
				int err1=picturenoiseprobe->Load(buf);
				if (err1 || err2) MessageBox("TVisionDialog::ReduceFringes : Problems loading measurement 8!","Error");

				//Store the absorption image together with the new reference image in the subfolder FR (Fringe Reduction)
				mkdir("FR");
				chdir("FR");
				char DestName[300];
				sprintf(DestName,"%s\\FR\\P%04u%cA%c.TGA",path,PicNr,cam,picpos);
				i=strlen(DestName)-4;
				while ((i>0) && DestName[i]!='A' && DestName[i]!='a') i--;
				if (i==0) MessageBox("TVisionDialog::ReduceFringes : Problems loading measurement 5!","Error");
				err1=pictureabs->Save(DestName,SAVE_AUTOMATIC,ColorTable);
				DestName[i]='N';
				err2=picturenoiseabs->Save(DestName,SAVE_AUTOMATIC,ColorTable);
				DestName[i]='O';
				int err3=picturenoiseprobe->Save(DestName,SAVE_AUTOMATIC,ColorTable);
				DestName[i]='P';
				int err4=pictureprobe->Save(DestName,SAVE_AUTOMATIC,ColorTable);
				if (err1 || err2 || err3 || err4) MessageBox("TVisionDialog::ReduceFringes : Problems loading measurement 6!","Error");
				DestName[i]='A';
				hpoint->SetText("Filename",DestName);
				chdir(path);
			}
		}
		hpoint=(CMessPoint *)(hpoint->next);
	}
	//copy the data file also into that folder and adapt the filename
	char buf[300];
	sprintf(buf,"%s\\FR\\Data%c.dat",path,cam);
	TNmeasure->SaveSerie(buf);	
}

void TVisionDialog::LoadPalette() {
  if (BMPFileData->FileName[0]!=0) {
    BMPFileData->InitialDir=NULL;
  }
  if (TFileOpenDialog(this, *BMPFileData, 0,"Load Palette").Execute()==IDOK)
	 ColorTable->LoadPalette(BMPFileData->FileName);
  UpdateAll();
  mb->SetText("Palette loaded");
};

void TVisionDialog::Fit1DHV(int horiz,double &Amp, double &X0, double &Sigma,
      double &Offset, double &Slope,double &RootMeanSquare, void
      (*funcs)(double, double [], double *, double [], int), double &err,
      double &chi) {
   double sigma,height,offset;
   double *x;
   x=NULL;
   double *y;
   y=NULL;
   int ndata=0;
   double x0;
   if (horiz) {
	   profilH->GetList(x,y,WV,WH,ndata,x0,sigma,height,offset);
   } else {
   	profilV->GetList(x,y,WV,WH,ndata,x0,sigma,height,offset);
   }
   if (sigma<0.01) sigma=0.01;
   double *sig=NULL;
   double *a=NULL;
   double *v=NULL;
   double *ah=NULL;
   double *vh=NULL;
   try {
      sig=new double[ndata];
      for (int i=0;i<ndata;i++) sig[i]=1;
      a=new double[5];
      v=new double[5];
      ah=new double[5];
      vh=new double[5];
   }
   catch (xalloc) {
	   MessageBox("TVisionDialog::Fit1DHV : couldn't reserve memory","Error",  MB_OK);
   	exit(-1);
   }

   double ChiMin=1E6;
   for (int k=0;k<5;k++) ah[k]=0;
   for (int k=0;k<5;k++) vh[k]=0;
   const int MaxSigmaModifier=5;
   double SigmaModifier[MaxSigmaModifier]={0.25,0.5,1.0,2.0,4.0};
   for (int i=0;i<MaxSigmaModifier;i++) {
      a[0]=height;
      a[1]=x0;
      a[2]=sigma*SigmaModifier[i];
      a[3]=(FitGaussWithOffset ? Offset : 0);
      a[4]=0;
      v[0]=(Fit1DWithAmplitude) ? 1 : 0;
      v[1]=1;
      v[2]=(Fit1DWithWidth) ?  1 : 0;
      v[3]=(FitGaussWithOffset ? 1 : 0);
      v[4]=0;
      double chisq;
      double *pchisq=&chisq;
      double change;
      int iter;
      if (fit(x,y,sig,ndata,a,v,5,pchisq,40,5,1E-7,1E-30,1E-7,iter,change,funcs)) {
	      mb->SetText("FitError 1 Fit1DHV");
   	   err=1;
      } else {
      	err=0;
	      chi=chisq;
      }
      if ((FitGaussWithSlope) && (err==0)) {
         v[4]=1;
         if (fit(x,y,sig,ndata,a,v,5,pchisq,40,5,1E-7,1E-30,1E-7,iter,change,funcs)) {
	         mb->SetText("FitError 2 Fit1DHV");
   	      err=1;
         } else {
      	   err=0;
         	chi=chisq;
         }
      }
      if ((chi<ChiMin) && (err==0)) {
	      ChiMin=chi;
   	   for (int k=0;k<5;k++) ah[k]=a[k];
      	for (int k=0;k<5;k++) vh[k]=v[k];
      }
   }
   for (int k=0;k<5;k++) a[k]=ah[k];
   for (int k=0;k<5;k++) v[k]=vh[k];

   Amp=a[0];
   X0=a[1];
   Sigma=a[2];
   Offset=a[3];
   Slope=a[4];
   double sq=0;
   double no=0;
   double help;
   for (int i=0;i<ndata;i++) {
	   help=(x[i]-X0);
   	sq+=help*help*(y[i]-Offset-Slope*x[i]);
	   no+=y[i];
   }
   if (no==0) no=1;
   RootMeanSquare=sq/no;
   RootMeanSquare=(RootMeanSquare>0) ? sqrt(RootMeanSquare) : 0;
   delete[] a;
   delete[] v;
   delete[] ah;
   delete[] vh;
   #if defined(DetectLeaks)
   LeakDetect.Delete(x);
   #endif
   delete[] x;
   #if defined(DetectLeaks)
   LeakDetect.Delete(y);
   #endif
   delete[] y;
   delete[] sig;
}

void TVisionDialog::Fit1DGaussToSum() {
  double pos1=0;
  double pos2=0;
  profilV->GetPos(pos1,pos2);
  profilH->GetSum(pos1,pos2,MD->y("ProfilAverage"));
  profilH->GetPos(pos1,pos2);
  profilV->GetSum(pos1,pos2,MD->y("ProfilAverage"));
  GetProfilPos();
  Fit1DGauss();
  PictureWin->SetCross(MD->y("X0"),MD->y("Y0"));
}

void TVisionDialog::FitPartialFermi() {
  double x0,sigma,height,offset;
  double *x;
  x=NULL;
  double *y;
  y=NULL;
  int ndata=0;
  if (FitFermiHorizontal) profilH->GetList(x,y,WV,WH,ndata,x0,sigma,height,offset);
  else profilV->GetList(x,y,WV,WH,ndata,x0,sigma,height,offset);

  //ofstream helpout("help.dat");
  //for (int i=0;i<ndata;i++) helpout<<i<<" "<<x[i]<<" "<<y[i]<<endl;

  if (sigma<0.01) sigma=0.01;
  double *sig=NULL;
  double *a=NULL;
  double *v=NULL;
  try {
	  sig=new double[ndata];
	  for (int i=0;i<ndata;i++) sig[i]=1;
	  a=new double[6];
	  v=new double[6];
  }
  catch (xalloc) {
  		MessageBox("TVisionDialog::FitPartialFermi : couldn't reserve memory","Error",  MB_OK);
      exit(-1);
  }
  if (FitFermiHorizontal) {
    a[0]=MD->y("AmpX");
    a[1]=MD->y("X0");
    a[2]=MD->y("SigmaX");
    a[3]=MD->y("GOffsetX");
    a[4]=MD->y("GSlopeX");
    a[5]=MD->y("FermiL");
  } else {
    a[0]=MD->y("AmpY");
    a[1]=MD->y("Y0");
    a[2]=MD->y("SigmaY");
    a[3]=MD->y("GOffsetY");
    a[4]=MD->y("GSlopeY");
    a[5]=MD->y("FermiL");
  }
  if ((a[5]<0.001) || (a[5]>=0.9)) a[5]=0.9;
  a[5]=tan((a[5]-0.5)*M_PI);
  v[0]=(FitPartialFermiNumber) ? 1 : 0;
  v[1]=0;
  v[2]=(FitPartialFermiTemp) ? 1 : 0;
  v[3]=0;
  v[4]=0;
  v[5]=(FitPartialFermiL) ? 1 : 0;

  if (FitFermiZeroTemp) {
    v[5]=0;
    SetFitFunc1D(FermiZeroTempFunc);
  } else SetFitFunc1D(PartialFermiFunc);

  double chisq;
  double *pchisq=&chisq;
  double change;
  int iter;
  if (fit(x,y,sig,ndata,a,v,6,pchisq,100,10,1E-30,1E-30,1E-5,iter,change,GeneralFitFunc1D)) {
    mb->SetText("FitError 1 FitPartialFermi");
  } else {
    char buf[200];
    sprintf(buf,"ChiSq=%.2e iter=%i change=%.2e",chisq,iter,change);
    MD->y("FitIter")=iter;
    MD->y("FitChange")=change;
    MD->y("FitChiSq")=chisq;
    mb->SetText(buf);
  }
  if (FitFermiHorizontal) {
    MD->y("AmpX")=a[0];
    MD->y("X0")=a[1];
    MD->y("SigmaX")=a[2];
    MD->y("GOffsetX")=a[3];
    MD->y("GSlopeX")=a[4];
  } else {
    MD->y("AmpY")=a[0];
    MD->y("Y0")=a[1];
    MD->y("SigmaY")=a[2];
    MD->y("GOffsetY")=a[3];
    MD->y("GSlopeY")=a[4];
  }
  //a[5]=tan((0.8-0.5)*M_PI);
  if (FitFermiZeroTemp) {
    if (FitFermiHorizontal) profilH->SetGeneralProfil(FermiZeroTempFunc,a,6,0,0);
    else profilV->SetGeneralProfil(FermiZeroTempFunc,a,6,0,0);
  } else {
    double help=atan(a[5])/M_PI+0.5;
    if (help>1) help=1;
    if (help<=0) help=1;
    MD->y("FermiL")=help;
    double width=a[2]*sqrt(5*(1-help)/help);
    if (FitFermiHorizontal) profilH->SetGeneralProfil(PartialFermiFunc,a,6,a[1]-width,a[1]+width);
    else profilV->SetGeneralProfil(PartialFermiFunc,a,6,a[1]-width,a[1]+width);
  }
  DisplayGaussFitResults();
  CalculateTOFTemperature();
  CalculateT_TF();
  Calculaten0rho0();
  delete[] a;
  delete[] v;
  #if defined(DetectLeaks)
     LeakDetect.Delete(x);
  #endif
  delete[] x;
  #if defined(DetectLeaks)
     LeakDetect.Delete(y);
  #endif
  delete[] y;
  delete[] sig;
  MD->y("1DFitFunction")=3;
}

void TVisionDialog::FitFermi() {
  FitFermiOrBose(true);
}

void TVisionDialog::FitBose() {
  FitFermiOrBose(false);
}

void TVisionDialog::FitFermiOrBose(bool FitFermi) {
  FitButtonFunction=1;
  double x0,sigma,height,offset;
  double *x;
  x=NULL;
  double *y;
  y=NULL;
  int ndata=0;
  if (FitFermiHorizontal) profilH->GetList(x,y,WV,WH,ndata,x0,sigma,height,offset);
  else profilV->GetList(x,y,WV,WH,ndata,x0,sigma,height,offset);
  if (sigma<0.01) sigma=0.01;
  double *sig=NULL;
  double *a=NULL;
  double *v=NULL;
  try {
    sig=new double[ndata];
	  	for (int i=0;i<ndata;i++) sig[i]=1;
	  	a=new double[3];
	  v=new double[3];
    }
    catch (xalloc) {
  		MessageBox("TVisionDialog::FitFermiOrBose : couldn't reserve memory","Error",  MB_OK);
      exit(-1);
  }
  a[0]=MD->y("NIntegral");
//  a[1]=MD->y("Y0");
  //if (FitFermiHorizontal) 
  a[1]=MD->y("TempTOFaxial");
  //else a[1]=MD->y("TempTOF");
//  a[3]=MD->y("GOffsetY");
//  a[2]=MD->y("GSlopeY");
  a[2]=1;//MD->y("NumberRatio");
//  if ((a[5]<0.001) || (a[5]>=1)) a[5]=1;
  v[0]=(Fit1DWithAmplitude) ? 1 : 0;
  v[1]=(Fit1DWithWidth) ? 1 : 0;
  v[2]=(FitWithNumberRatio) ? 1 : 0;
//  v[3]=0;
//  v[4]=0;
//  v[5]=0;
  double chisq;
  double *pchisq=&chisq;
  double change;
  int iter;
  SetFitFunc1D(FermiFunc);
  double frad,fax;
  CalculateTrapFrequencies(frad,fax);
  double masse=MD->y("Mass")*_mp;
  if (FitFermiHorizontal) SetFermiFuncParams(fax,frad,Sigma(MD->y("ProbeDetuning"),MD->y("Linewidth"),MD->y("Wavelength"),MD->y("Clebsch")),masse,MD->y("ExpansionTime"),MD->y("X0"),MD->y("GOffsetX"),MD->y("GSlopeX"),FitFermi,FitFermiHorizontal);
  else SetFermiFuncParams(fax,frad,Sigma(MD->y("ProbeDetuning"),MD->y("Linewidth"),MD->y("Wavelength"),MD->y("Clebsch")),masse,MD->y("ExpansionTime"),MD->y("Y0"),MD->y("GOffsetY"),MD->y("GSlopeY"),FitFermi,FitFermiHorizontal);
  if (!FirstFermiFit) {
    if (fit(x,y,sig,ndata,a,v,3,pchisq,4,2,1E-30,1E-30,1E-5,iter,change,GeneralFitFunc1D)) {
      mb->SetText("FitError 1 FitFermi");
    } else {
      char buf[200];
      sprintf(buf,"ChiSq=%.2e iter=%i change=%.2e",chisq,iter,change);
      MD->y("FitIter")=iter;
      MD->y("FitChange")=change;
      MD->y("FitChiSq")=chisq;
      mb->SetText(buf);
    }
  }
  FirstFermiFit=0;
  MD->y("NIntegral")=a[0];
//  MD->y("Y0")=a[1];
  if (FitFermiHorizontal) MD->y("Taxial")=a[1];
  else MD->y("TempTOF")=a[1];
//  MD->y("GOffsetY")=a[3];
//  MD->y("GSlopeY")=a[4];
  MD->y("NumberRatio")=a[2];
  if (FitFermi) MD->y("1DFitFunction")=5;
  else MD->y("1DFitFunction")=6;
  ShowFit();
  delete[] a;
  delete[] v;
  #if defined(DetectLeaks)
     LeakDetect.Delete(x);
  #endif
  delete[] x;
  #if defined(DetectLeaks)
     LeakDetect.Delete(y);
  #endif
  delete[] y;
  delete[] sig;
}

void TVisionDialog::ShowFit() {
  int help=MD->y("1DFitFunction");
  switch (help) {
    case 5:
    case 6:
      double *a=NULL;
      try {
	      a=new double[3];
       }
		  catch (xalloc) {
  			MessageBox("TVisionDialog::ShowFit : couldn't reserve memory","Error",  MB_OK);
	      exit(-1);
	  }
      a[0]=MD->y("NIntegral");
      a[1]= FitFermiHorizontal ? MD->y("Taxial") : MD->y("TempTOF");
      a[2]=MD->y("NumberRatio");
      if (FitFermiHorizontal) profilH->SetGeneralProfil(FermiFunc,a,3,0,0);
      else profilV->SetGeneralProfil(FermiFunc,a,3,0,0);
      DisplayFermiFitResults();
      CalculateT_TF();
      Calculaten0rho0();
      delete[] a;
    break;
  }
}

void TVisionDialog::DisplayFermiFitResults() {
  double T;
  if (FitFermiHorizontal) T=MD->y("Taxial");
  else T=MD->y("TempTOF");
  TNFitTextBox->SetValue(2,T);
  TNFitTextBox->SetValue(5,T);
  GaussFitTextBox->SetValue(5,MD->y("NIntegral"));
  GaussFitTextBox->SetValue(10,MD->y("NIntegral")*MD->y("NumberRatio"));
//  GaussFitTextBox->SetValue(13,MD->y("NumberRatio"));
  GaussFitTextBox->SetValue(6,MD->y("Y0"));
  GaussFitTextBox->SetValue(8,MD->y("GOffsetY"));
}

void TVisionDialog::FitGaussWingsBothDirections() {
  FitGaussWings(true);
  FitGaussWings(false);
}

void TVisionDialog::FitGaussWings(bool direction) {
  double x0,sigma,height,offset;
  double *hx;
  hx=NULL;
  double *hy;
  hy=NULL;
  int ndata=0;
  if (direction) profilH->GetList(hx,hy,WV,WH,ndata,x0,sigma,height,offset);
  else profilV->GetList(hx,hy,WV,WH,ndata,x0,sigma,height,offset);
  MD->y("FermiL")=0.5;
  MD->y("P1")=0;
  MD->y("P2")=1.5; //size of excluded area in units of sigma of the original gauss fit. 2*sigma=waist, 4*sigma=full size
  double help=5.0*(1.0/MD->y("FermiL")-1.0);
  if (help<0) help=0;
  double p2=MD->y("P2");
  double p1=MD->y("P1");
  if ((p2==0) && (p1==0)) p2=1;
  double width,start,end;
  if (direction) {
    width=MD->y("SigmaX")*(p2+p1*sqrt(help));
    start=MD->y("X0")-width;
    end=MD->y("X0")+width;
  } else {
    width=MD->y("SigmaY")*(p2+p1*sqrt(help));
    start=MD->y("Y0")-width;
    end=MD->y("Y0")+width;
  }
  double eich=(hx[ndata-1]-hx[0])/(ndata-1);
  int startn=(start-hx[0])/eich;
  int widthn=(2*width)/eich;
  if (startn<0 || (startn+width)>ndata || width<0) {
    #if defined(DetectLeaks)
      LeakDetect.Delete(hx);
    #endif
    delete[] hx;
    #if defined(DetectLeaks)
      LeakDetect.Delete(hy);
    #endif
    delete[] hy;
    mb->SetText("FitError 0 FitGaussWings");
    return;
  }
  ndata=ndata-widthn;
  double *x;
  double *y;
  x=new double[ndata];
  y=new double[ndata];
  for (int i=0;i<ndata;i++) {
    if (i<startn) {
      x[i]=hx[i];
      y[i]=hy[i];
    } else {
      x[i]=hx[i+widthn];
      y[i]=hy[i+widthn];
    }
  }
//  ofstream helpout("help.dat");
//  for (int i=0;i<ndata;i++) helpout<<i<<" "<<x[i]<<" "<<y[i]<<endl;
  #if defined(DetectLeaks)
    LeakDetect.Delete(hx);
  #endif
  delete[] hx;
  #if defined(DetectLeaks)
    LeakDetect.Delete(hy);
  #endif
  delete[] hy;
  if (sigma<0.01) sigma=0.01;
  double *sig=NULL;
  double *v=NULL;
  double *a=NULL;
  try {
	  sig=new double[ndata];
	  for (int i=0;i<ndata;i++) sig[i]=1;
	  a=new double[5];
	  v=new double[5];
  }
  catch (xalloc) {
  		MessageBox("TVisionDialog::DisplayFermiFitResults : couldn't reserve memory","Error",  MB_OK);
      exit(-1);
  }
  if (direction) {
    a[0]=MD->y("AmpX");
    a[1]=MD->y("X0");
    a[2]=MD->y("SigmaX");
    a[3]=MD->y("GOffsetX");
    a[4]=MD->y("GSlopeX");
  } else {
    a[0]=MD->y("AmpY");
    a[1]=MD->y("Y0");
    a[2]=MD->y("SigmaY");
    a[3]=MD->y("GOffsetY");
    a[4]=MD->y("GSlopeY");
  }
  v[0]=1;
  v[1]=0;
  v[2]=1;
  v[3]=FitGaussWithOffset ? 1 : 0;
  v[4]=0;
  double chisq;
  double *pchisq=&chisq;
  double change;
  int iter;
  SetFitFunc1D(GaussFunc);
  if (fit(x,y,sig,ndata,a,v,5,pchisq,100,10,1E-30,1E-30,1E-5,iter,change,GeneralFitFunc1D)) {
    mb->SetText("FitError 1 FitGaussWings");
  } else {
    char buf[200];
    sprintf(buf,"ChiSq=%.2e iter=%i change=%.2e",chisq,iter,change);
    MD->y("FitIter")=iter;
    MD->y("FitChange")=change;
    MD->y("FitChiSq")=chisq;
    mb->SetText(buf);
  }
  if (direction) {
    MD->y("AmpX")=a[0];
    MD->y("X0")=a[1];
    MD->y("SigmaX")=a[2];
    MD->y("GOffsetX")=a[3];
    MD->y("GSlopeX")=a[4];
  } else {
    MD->y("AmpY")=a[0];
    MD->y("Y0")=a[1];
    MD->y("SigmaY")=a[2];
    MD->y("GOffsetY")=a[3];
    MD->y("GSlopeY")=a[4];
  }
  if (direction) profilH->SetGeneralProfil(GaussFunc,a,/*Number of parameters*/5,start,end);
  else profilV->SetGeneralProfil(GaussFunc,a,5,start,end);
  DisplayGaussFitResults();
  CalculateTOFTemperature();
  CalculateT_TF();
  Calculaten0rho0();
  delete[] a;
  delete[] v;
  delete[] x;
  delete[] y;
  delete[] sig;
  MD->y("1DFitFunction")=4;
}

double Gauss2DOptDensWithNoise(double x, double y, double a[]) {
  return Gauss2D(x,y,a)+double(random(100))/2000;
}

double IoffeOptDensWithNoise(double x, double y, double a[]) {
  return IoffeOptDens(x,y,a)+double(random(100))/2000;
}

void TVisionDialog::Store2DFitParams() {
  MD->y("X02D")=Fit2DParameters[0]*1000;
  MD->y("Y02D")=Fit2DParameters[1]*1000;
  MD->y("Tradial")=Fit2DParameters[2]*1000;
  MD->y("Taxial")=Fit2DParameters[2]*1000/Fit2DParameters[8];
  MD->y("N2D")=Fit2DParameters[3];
  MD->y("Offset2D")=Fit2DParameters[4];
  MD->y("SlopeX2D")=Fit2DParameters[5];
  MD->y("SlopeY2D")=Fit2DParameters[6];
  MD->y("Angle2D")=Fit2DParameters[7]*360/(2*M_PI);
  MD->y("zCorr2D")=Fit2DParameters[8];
}

void TVisionDialog::Load2DFitParams() {
  Fit2DParameters[0]=MD->y("X02D")/1000;
  Fit2DParameters[1]=MD->y("Y02D")/1000;
  Fit2DParameters[2]=MD->y("Tradial")/1000;
  Fit2DParameters[3]=MD->y("N2D");
  Fit2DParameters[4]=MD->y("Offset2D");
  Fit2DParameters[5]=MD->y("SlopeX2D");
  Fit2DParameters[6]=MD->y("SlopeY2D");
  Fit2DParameters[7]=MD->y("Angle2D")/(360/(2*M_PI));
  Fit2DParameters[8]=MD->y("zCorr2D");
  int h=MD->y("2DFitFunction");
  switch (h) {
    default: akt2DFitFunc=IoffeOptDens;
  }
}

void TVisionDialog::Gauss2DPicture() {
  Fit2DParameters[0]=0/1000; // X0 [m]
  Fit2DParameters[1]=0/1000; // Y0 [m]
  Fit2DParameters[2]=3E-5;  // sx [m]
  Fit2DParameters[3]=3E-5;  // sy [m]
  Fit2DParameters[4]=0.01;     // Offset
  Fit2DParameters[5]=0;     // SlopeX
  Fit2DParameters[6]=0;     // SlopeY
  Fit2DParameters[7]=0.1;     // Amp
  double Params[10];
  for (int i=1;i<=9;i++) Params[i]=Fit2DParameters[i-1];
  MD->y("GxOrOffset")=2;
  MD->y("GyOrGradient")=2000;
  MD->y("GzOrCurvature")=560;
  MD->y("PictureListPosition")=1;
  MD->y("ProbeDetuning")=0;
  opticalDensity->DisplayFunction(Params,Gauss2D);//OptDensWithNoise);
//  aktPic2Dfited=0;
  MD->y("2DFitFunction")=0;
  picture=opticalDensity;
  Display2DFitResults();
  akt2DFitFunc=Gauss2D;
  UpdateAll();
  Display2DProfils();
}

void TVisionDialog::Fermi2DPicture() {
  Fit2DParameters[0]=0/1000; // X0 [m]
  Fit2DParameters[1]=0/1000; // Y0 [m]
  Fit2DParameters[2]=1E-4;  // sx [m]
  Fit2DParameters[3]=1E-4;  // sy [m]
  Fit2DParameters[4]=0.01;     // Offset
  Fit2DParameters[5]=0;     // SlopeX
  Fit2DParameters[6]=0;     // SlopeY
  Fit2DParameters[7]=0.1;     // Amp
  double Params[10];
  for (int i=1;i<=9;i++) Params[i]=Fit2DParameters[i-1];
  MD->y("GxOrOffset")=2;
  MD->y("GyOrGradient")=1268;
  MD->y("GzOrCurvature")=273;
  MD->y("PictureListPosition")=0;
  MD->y("ProbeDetuning")=0;
  opticalDensity->DisplayFunction(Params,Fermi2D);
//  aktPic2Dfited=0;
  MD->y("2DFitFunction")=0;
  picture=opticalDensity;
  Display2DFitResults();
  akt2DFitFunc=Gauss2D;
  UpdateAll();
  Display2DProfils();
  FirstFermiFit=1;
  FitButtonFunction=0;
  profilV->SetGeneralProfil();
  profilH->SetGeneralProfil();
}

void TVisionDialog::IoffePicture() {
  Fit2DParameters[0]=0.01/1000; // X0 [m]
  Fit2DParameters[1]=0.02/1000; // Y0 [m]
  Fit2DParameters[2]=2E-3;  // Temp [K]
  Fit2DParameters[3]=5E7;  // N [atoms]
  Fit2DParameters[4]=0.01;     // Offset
  Fit2DParameters[5]=2;     // SlopeX
  Fit2DParameters[6]=1;     // SlopeY
  Fit2DParameters[7]=CameraInclination/360.0*2*M_PI;     // Angle
  Fit2DParameters[8]=1;     // z Flight correction
  double Params[10];
  for (int i=1;i<=9;i++) Params[i]=Fit2DParameters[i-1];
  MD->y("GxOrOffset")=5;
  MD->y("GyOrGradient")=2000;
  MD->y("GzOrCurvature")=560;
  MD->y("PictureListPosition")=1;
  MD->y("ProbeDetuning")=0;
  SetIoffeTrapParameters(MD->y("GxOrOffset"), MD->y("GyOrGradient"),
    MD->y("GzOrCurvature"),fabs(MD->y("MagMoment")*_myb),Sigma(MD->y("ProbeDetuning"),MD->y("Linewidth"),MD->y("Wavelength"),MD->y("Clebsch")), MD->y("Mass")*_mp ,SigmaEl);
  opticalDensity->DisplayFunction(Params,IoffeOptDensWithNoise);
//  aktPic2Dfited=0;
  MD->y("2DFitFunction")=0;
  SetTrapParameters();
  picture=opticalDensity;
  Display2DFitResults();
  akt2DFitFunc=IoffeOptDens;
  UpdateAll();
  Display2DProfils();
}

void TVisionDialog::DisplayIoffeFitUpdateAll() {
  DisplayIoffeFit(true);
}

void TVisionDialog::Set2DFitTrapParameters() {
  int h=MD->y("2DFitFunction");
  switch (h) {
    case 1:
      SetIoffeTrapParameters(MD->y("GxOrOffset"), MD->y("GyOrGradient"),
        MD->y("GzOrCurvature"),fabs(MD->y("MagMoment")*(_myb)),
        Sigma(MD->y("ProbeDetuning"),MD->y("Linewidth"),MD->y("Wavelength"),MD->y("Clebsch")), MD->y("Mass")*_mp,SigmaEl);
      akt2DFitFunc=IoffeOptDens;
      break;
    case 2:
      akt2DFitFunc=Gauss2D;
      break;
    default: MD->y("2DFitFunction")=0;return;
  }
}

void TVisionDialog::DisplayIoffeFit(bool updateall) {
  double Params[10];
  for (int i=0;i<9;i++) Params[i+1]=Fit2DParameters[i];
  Set2DFitTrapParameters();
  if (updateall) {
    Copy(*opticalDensity,*pictureprobe);
    Copy(*opticalDensity,*pictureabs);
    pictureabs->DisplayFunction(Params,akt2DFitFunc);
    Substract(*pictureabs,*pictureprobe,*picturenoiseabs);
    picture=picturenoiseabs;
    UpdateAll();
  } else {
    Copy(*opticalDensity,*probenoisepicture);
    probenoisepicture->DisplayFunction(Params,akt2DFitFunc);
    Substract(*probenoisepicture,*opticalDensity,*picturenoiseabs);
    MovePicturesToDetailPictureButtonsAndRedraw();
    if (picture==picturenoiseabs) PictureWin->Update(MD->y("ProfilAverage"));
  }
}

void TVisionDialog::FitIoffeDistribution() {
  SetIoffeTrapParameters(MD->y("GxOrOffset"), MD->y("GyOrGradient"),
    MD->y("GzOrCurvature"),fabs(MD->y("MagMoment")*_myb),
    Sigma(MD->y("ProbeDetuning"),MD->y("Linewidth"),MD->y("Wavelength"),MD->y("Clebsch")), MD->y("Mass")*_mp,SigmaEl);
  switch (InitialiseIoffeFit) {
    case 0:
      Fit1DGaussToSum();
      IntegralAtomNumber();
	  CalculateFormulas();
      InitialiseIoffeFitParams();
      Update2DFitParams();
     break;
     case 1:
       Load2DFitParams();
     break;
  }
  Fit2DDistribution(1,FitIoffeOptDens,IoffeOptDens);
}

void TVisionDialog::Fit2DGaussDistribution() {
  Fit2DDistribution(2,FitGauss2D,Gauss2D);
}

void TVisionDialog::ResetFitParameters(int Function) {
  switch (Function) {
    case 1:
       Fit2DParameters[0]=0.0/1000; // X0 [m]
       Fit2DParameters[1]=0.0/1000; // Y0 [m]
       Fit2DParameters[2]=2E-3;  // Temp [K]
       Fit2DParameters[3]=0.8E8;  // N [atoms]
       Fit2DParameters[4]=0;     // Offset
       Fit2DParameters[5]=0;     // SlopeX
       Fit2DParameters[6]=0;     // SlopeY
       Fit2DParameters[7]=CameraInclination/360.0*2*M_PI;  // Angle
       Fit2DParameters[8]=1;     // z Flight correction
    break;
    case 2:
       Fit2DParameters[0]=0.0/1000; // X0 [m]
       Fit2DParameters[1]=0.0/1000; // Y0 [m]
       Fit2DParameters[2]=2E-3;  // Temp [K]
       Fit2DParameters[3]=2E-3;  // N [atoms]
       Fit2DParameters[4]=0;     // Offset
       Fit2DParameters[5]=0;     // SlopeX
       Fit2DParameters[6]=0;     // SlopeY
       Fit2DParameters[7]=CameraInclination/360.0*2*M_PI;  // Angle
       Fit2DParameters[8]=1;     // z Flight correction
    break;
  }
}

void TVisionDialog::Sz2DBackButtonClicked() {
  if (FitButtonFunction==0) {
    Fit2DParameters[8]=Fit2DParameters[8]*0.9-0.1;
    Update2DFitParams();
  } else {
    MD->y("NumberRatio")=MD->y("NumberRatio")*0.9-0.1;
    ShowFit();
  }
}

void TVisionDialog::Sz2DForwardButtonClicked() {
  if (FitButtonFunction==0) {
    Fit2DParameters[8]=Fit2DParameters[8]*1.1+0.01;
    Update2DFitParams();
  } else {
    MD->y("NumberRatio")=MD->y("NumberRatio")*1.1+0.01;
    ShowFit();
  }
}

void TVisionDialog::Sy2DBackButtonClicked() {
  Fit2DParameters[1]=Fit2DParameters[1]*0.9-0.00005;
  Update2DFitParams();
}

void TVisionDialog::Sy2DForwardButtonClicked() {
  Fit2DParameters[1]=Fit2DParameters[1]*1.1+0.00005;
  Update2DFitParams();
}

void TVisionDialog::Sx2DBackButtonClicked() {
  Fit2DParameters[0]=Fit2DParameters[0]*0.9-0.00005;
  Update2DFitParams();
}

void TVisionDialog::Sx2DForwardButtonClicked() {
  Fit2DParameters[0]=Fit2DParameters[0]*1.1+0.00005;
  Update2DFitParams();
}

void TVisionDialog::O2DBackButtonClicked() {
  Fit2DParameters[4]=Fit2DParameters[4]*0.9-0.01;
  Update2DFitParams();
}

void TVisionDialog::O2DForwardButtonClicked() {
  Fit2DParameters[4]=Fit2DParameters[4]*1.1+0.01;
  Update2DFitParams();
}

void TVisionDialog::T2DBackButtonClicked() {
  if (FitButtonFunction==0) {
    Fit2DParameters[2]*=0.9;
    Update2DFitParams();
  } else {
    if (FitFermiHorizontal) MD->y("Taxial")*=0.9;
    else MD->y("TempTOF")*=0.9;
    ShowFit();
  }
}

void TVisionDialog::T2DForwardButtonClicked() {
  if (FitButtonFunction==0) {
    Fit2DParameters[2]*=1.1;
    Update2DFitParams();
  } else {
    if (FitFermiHorizontal) MD->y("Taxial")*=1.1;
    else MD->y("TempTOF")*=1.1;
    ShowFit();
  }
}

void TVisionDialog::N2DBackButtonClicked() {
  if (FitButtonFunction==0) {
    Fit2DParameters[3]*=0.9;
    Update2DFitParams();
  } else {
    MD->y("NIntegral")*=0.9;
    ShowFit();
  }
}

void TVisionDialog::N2DForwardButtonClicked() {
  if (FitButtonFunction==0) {
    Fit2DParameters[3]*=1.1;
    Update2DFitParams();
  } else {
    MD->y("NIntegral")*=1.1;
    ShowFit();
  }
}

void TVisionDialog::InitialiseIoffeFitParams() {
  Fit2DParameters[3]=MD->y("NIntegral");
  Fit2DParameters[7]=CameraInclination/360.0*2*M_PI;  // Angle
  Fit2DParameters[0]=0.0/1000; // X0 [m]
  Fit2DParameters[1]=0.0/1000; // Y0 [m]
  Fit2DParameters[4]=MD->y("NIntOffset");     // Offset
  double my=fabs(MD->y("MagMoment")*_myb);
  double CRad=MD->y("GyOrGradient")*MD->y("GyOrGradient")/MD->y("GxOrOffset")-MD->y("GzOrCurvature")*0.5;
  double CAx=MD->y("GzOrCurvature");
  double Sx=MD->y("SigmaX");
  double Sy=MD->y("SigmaY");
  Fit2DParameters[8]=CRad*Sy*Sy/(CAx*Sx*Sx);     // z Flight correction
  Fit2DParameters[2]=my*CRad*Sy*Sy/_kB*1E-6;  // Temp [K]
  MD->y("2DFitFunction")=1;
}

void TVisionDialog::Update2DFitParams() {
  if (MD->y("2DFitFunction")==0) InitialiseIoffeFitParams();
  if (Fit2DParameters[2]<1E-6) Fit2DParameters[2]=1E-6;  // Temp [K]
  if (Fit2DParameters[3]<1E4) Fit2DParameters[3]=1E4;
  if (Fit2DParameters[8]<0.001) Fit2DParameters[8]=0.001;
  Store2DFitParams();
  Set2DFitTrapParameters();
  Ioffen0rho0(MD->y("N2D"),MD->y("Tradial")/1000.0,MD->y("zCorr2D"),MD->y("n0"),MD->y("rho0"));
  MD->y("n0")=MD->y("n0")*1E-6;
  MD->y("Gamma")=IoffeGamma(MD->y("N2D"),MD->y("Tradial")/1000.0,MD->y("zCorr2D"));
  MD->y("2DFitFunction")=1;
//  TCurrentEvent event=GetCurrentEvent();
  Update2DFitDrawing=true;
}

void TVisionDialog::Fit2DDistribution(int Function,
    void (*funcs)(double, double [], double *, double [], int),
    double (*FitFunc)(double, double,double [])) {
  akt2DFitFunc=FitFunc;
  picture=opticalDensity;
  GetProfilPos();
  double *x;
  double *y;
  x=NULL;
  y=NULL;
  int ndata=0;
  double sx,sy,dx,dy;
  int nx,ny;
  opticalDensity->GetPictureAsList(MD->y("Pos1Xs"),MD->y("Pos1Xe"),MD->y("Pos1Ys"),MD->y("Pos1Ye"),x,y,ndata,WH,WV,sx,sy,dx,dy,nx,ny);
  Set2DFitParameters(sx,sy, dx, dy, nx, ny,FitFunc);
/*  double dyda[10];
  double Params[10];
  for (int i=0;i<9;i++) Params[i+1]=Fit2DParameters[i];
  double h;
  ofstream out("List.dat");
  for (int i=0;i<ndata;i++) {
    out<<x[i]<<' '<<y[i]<<' ';
    FitIoffeOptDens(i, Params, &h, dyda, 7);
    out<<h<<endl;
  }*/
  double *sig=NULL;
  double *v=NULL;
  try {
	  sig=new double[ndata];
	  for (int i=0;i<ndata;i++) sig[i]=1;
	  v=new double[9];
  }
  catch (xalloc) {
  		MessageBox("TVisionDialog::Fit2DDistribution : couldn't reserve memory","Error",  MB_OK);
      exit(-1);
  }
  v[0]=1;
  v[1]=1;
  v[2]=1;
  v[3]=1;
  v[4]=(Fit2DWithOffset ? 1 : 0);
  v[5]=(Fit2DWithSlope ? 1 : 0);
  v[6]=(Fit2DWithSlope ? 1 : 0);
  v[7]=(Fit2DWithAngle ? 1 : 0);
  v[8]=(Fit2DWithLongitudinalFlightCorrection ? 1 : 0);
  double chisq;
  double *pchisq=&chisq;
  bool Fiterror=false;
  int nend=0;
  double changeend=0;
  if (fit(x,y,sig,ndata,Fit2DParameters,v,9,pchisq,50,1,1E-5,0.1,1E-5,nend,changeend,funcs)) {
    /*ResetFitParameters(Function);
    if (fit(x,y,sig,ndata,Fit2DParameters,v,9,pchisq,50,0.1,1E-5,nend,changeend,funcs)) {
      ResetFitParameters(Function);
      Fiterror=true;
    }*/
    Fiterror=true;
  }
  MD->y("FitIter")=nend;
  MD->y("FitChange")=changeend;
  MD->y("FitChiSq")=chisq;
  if (Fiterror) {
    MD->y("2DFitFunction")=-1;
    mb->SetText("2DFit error");
  } else {
    Store2DFitParams();
    char buf[200];
    sprintf(buf,"2D Distribution fitted: ChiSq=%.3f",chisq);
    mb->SetText(buf);
    MD->y("2DFitFunction")=Function;
    Ioffen0rho0(MD->y("N2D"),MD->y("Tradial")/1000.0,MD->y("zCorr2D"),MD->y("n0"),MD->y("rho0"));
    MD->y("n0")=MD->y("n0")*1E-6;
    MD->y("Gamma")=IoffeGamma(MD->y("N2D"),MD->y("Tradial")/1000.0,MD->y("zCorr2D"));
  }
  Display2DProfils();
  Display2DFitResults();
  DisplayIoffeFit(false);
//  aktPic2Dfited=Function;
  delete[] v;
  delete[] x;
  delete[] y;
  delete[] sig;
  if (UpdateMeasurementAfterFit && MD->IsInList()) {
    TNmeasure->Draw();
    TNmeasure->SaveSerie();
  }
}

void TVisionDialog::Display2DProfils() {
  double pos1y=0;
  double pos2y=0;
  double pos1x=0;
  double pos2x=0;
  double pos1=0;
  double pos2=0;
  int h=MD->y("2DFitFunction");
  switch (h) {
    case 1:
      SetIoffeTrapParameters(MD->y("GxOrOffset"), MD->y("GyOrGradient"),
        MD->y("GzOrCurvature"),fabs(MD->y("MagMoment")*_myb),
        Sigma(MD->y("ProbeDetuning"),MD->y("Linewidth"),MD->y("Wavelength"),MD->y("Clebsch")),MD->y("Mass")*_mp ,SigmaEl);
      akt2DFitFunc=IoffeOptDens;
      break;
    case 2:
      akt2DFitFunc=Gauss2D;
      break;
    default: MD->y("2DFitFunction")=0;return;
  }
  profilV->GetPos(pos1,pos2);
  profilH->GetSum(pos1,pos2,MD->y("ProfilAverage"));
  profilH->GetPos(pos1,pos2);
  profilV->GetSum(pos1,pos2,MD->y("ProfilAverage"));
  profilV->GetRealPos(pos1y,pos2y);
  profilH->Set2DDistribution(Fit2DParameters,pos1y,pos2y,profilV->GetEich(),akt2DFitFunc);
  profilH->GetRealPos(pos1x,pos2x);
  profilV->Set2DDistribution(Fit2DParameters,pos1x,pos2x,profilH->GetEich(),akt2DFitFunc);
}

void TVisionDialog::DisplayCutOf2DProfils() {
  double xpos,ypos;
  int h=MD->y("2DFitFunction");
  switch (h) {
    case 1:
      SetIoffeTrapParameters(MD->y("GxOrOffset"), MD->y("GyOrGradient"),
      MD->y("GzOrCurvature"),fabs(MD->y("MagMoment")*_myb),Sigma(MD->y("ProbeDetuning"),MD->y("Linewidth"),MD->y("Wavelength"),MD->y("Clebsch")),
      MD->y("Mass")*_mp ,SigmaEl);
      akt2DFitFunc=IoffeOptDens;
    break;
    case 2:
      akt2DFitFunc=Gauss2D;
    break;
    default: MD->y("2DFitFunction")=0;return;
  }
  PictureWin->GetRealPos(xpos,ypos);
  profilH->Set2DDistribution(Fit2DParameters,ypos,ypos,profilV->GetEich(),akt2DFitFunc);
  profilV->Set2DDistribution(Fit2DParameters,xpos,xpos,profilH->GetEich(),akt2DFitFunc);
}

void TVisionDialog::GetProfilPos() {
  profilV->GetPos(MD->y("Pos1Ys"),MD->y("Pos1Ye"),0);
  profilH->GetPos(MD->y("Pos1Xs"),MD->y("Pos1Xe"),0);
  profilV->GetPos(MD->y("Pos2Ys"),MD->y("Pos2Ye"),1);
  profilH->GetPos(MD->y("Pos2Xs"),MD->y("Pos2Xe"),1);
}

void TVisionDialog::SetProfilPos() {
  profilV->SetPos(MD->y("Pos1Ys"),MD->y("Pos1Ye"),0);
  profilH->SetPos(MD->y("Pos1Xs"),MD->y("Pos1Xe"),0);
  profilV->SetPos(MD->y("Pos2Ys"),MD->y("Pos2Ye"),1);
  profilH->SetPos(MD->y("Pos2Xs"),MD->y("Pos2Xe"),1);
}

void TVisionDialog::SetROIFromProfilPos(int Nr) {
  profilV->GetPos(ROI[8*Nr+0],ROI[8*Nr+1],0);
  profilH->GetPos(ROI[8*Nr+2],ROI[8*Nr+3],0);
  profilV->GetPos(ROI[8*Nr+4],ROI[8*Nr+5],1);
  profilH->GetPos(ROI[8*Nr+6],ROI[8*Nr+7],1);
  ROICam[Nr]=MD->y("Camera");
}

void TVisionDialog::GetProfilPosFromROI(int Nr) {
  profilV->SetPos(ROI[8*Nr+0],ROI[8*Nr+1],0);
  profilH->SetPos(ROI[8*Nr+2],ROI[8*Nr+3],0);
  profilV->SetPos(ROI[8*Nr+4],ROI[8*Nr+5],1);
  profilH->SetPos(ROI[8*Nr+6],ROI[8*Nr+7],1);
}

void TVisionDialog::LoadROI0() { LoadROINr(0); }
void TVisionDialog::LoadROI1() { LoadROINr(1); }
void TVisionDialog::LoadROI2() { LoadROINr(2); }
void TVisionDialog::LoadROI3() { LoadROINr(3); }
void TVisionDialog::LoadROI4() { LoadROINr(4); }
void TVisionDialog::LoadROI5() { LoadROINr(5); }
void TVisionDialog::LoadROI6() { LoadROINr(6); }
void TVisionDialog::LoadROI7() { LoadROINr(7); }

void TVisionDialog::LoadROIs() {
  LoadROINr(-1);
}

void TVisionDialog::LoadROINr(int Nr) {
  if (ROIFileData->FileName[0]!=0) {
    ROIFileData->InitialDir=NULL;
  }
  if (TFileOpenDialog(this, *ROIFileData, 0,"Load ROIs").Execute()==IDOK)
	 LoadROIs(ROIFileData->FileName,Nr);
}

void TVisionDialog::LoadROIs(const char *FileName, int Nr) {
  struct ffblk ffblk;
  if (findfirst(FileName,&ffblk,0)==0) {
     int NrROIsInFile;
     ifstream in(FileName);
     in>>NrROIsInFile;
     int SmallestNrROIs=(NrROIs<NrROIsInFile) ? NrROIs : NrROIsInFile;
     if (Nr==-1) {		 
		 for (int j=0;j<SmallestNrROIs;j++) {
			for (int k=0;k<8;k++) {
				in>>ROI[j*8+k];				
			}
			in>>ROICam[j];
		 }
     } else {
       if (Nr<SmallestNrROIs) {
          double dummy;
		  int dummyi;
		  for (int i=0;i<Nr;i++) {
			  for (int j=0;j<8;j++) in>>dummy;
			  in>>dummyi;
		  }
          for (int i=0;i<8;i++) in>>ROI[Nr*8+i];
		  in>>ROICam[Nr];
       }
     }
     in.close();
  }
}

void TVisionDialog::SaveROIs() {
  if (ROIFileData->FileName[0]!=0) {
    ROIFileData->InitialDir=NULL;
  }
  if (TFileSaveDialog(this, *ROIFileData, 0,"Save ROI").Execute()==IDOK)
	 SaveROIs(ROIFileData->FileName);
}

void TVisionDialog::SaveROIs(const char *FileName) {
  ofstream out(FileName);
  out<<NrROIs<<endl;
  for (int i=0;i<NrROIs;i++) {
	  for (int k=0;k<8;k++) {
		out<<ROI[i*8+k]<<endl;
	  }
	  out<<ROICam[i]<<endl;
  }
  out.close();
}

void TVisionDialog::SetProbeIntensityROI() { SetROIFromProfilPos(8);ShowROI(8);} 
void TVisionDialog::ShowProbeIntensityROI() { ShowROI(8);} 

void TVisionDialog::CeEnableIntensityCompensation(TCommandEnabler& ce)
{
  ce.SetCheck(DoIntensityCompensation? TCommandEnabler::Checked : TCommandEnabler::Unchecked);
}

void TVisionDialog::EnableIntensityCompensation() {
	DoIntensityCompensation=!DoIntensityCompensation;
}

void TVisionDialog::SetROI0() { SetROIFromProfilPos(0);ShowROI(0); }
void TVisionDialog::SetROI1() { SetROIFromProfilPos(1);ShowROI(1); }
void TVisionDialog::SetROI2() { SetROIFromProfilPos(2);ShowROI(2); }
void TVisionDialog::SetROI3() { SetROIFromProfilPos(3);ShowROI(3); }
void TVisionDialog::SetROI4() { SetROIFromProfilPos(4);ShowROI(4); }
void TVisionDialog::SetROI5() { SetROIFromProfilPos(5);ShowROI(5); }
void TVisionDialog::SetROI6() { SetROIFromProfilPos(6);ShowROI(6); }
void TVisionDialog::SetROI7() { SetROIFromProfilPos(7);ShowROI(7); }

void TVisionDialog::ShowROI0() { ShowROI(0); }
void TVisionDialog::ShowROI1() { ShowROI(1); }
void TVisionDialog::ShowROI2() { ShowROI(2); }
void TVisionDialog::ShowROI3() { ShowROI(3); }
void TVisionDialog::ShowROI4() { ShowROI(4); }
void TVisionDialog::ShowROI5() { ShowROI(5); }
void TVisionDialog::ShowROI6() { ShowROI(6); }
void TVisionDialog::ShowROI7() { ShowROI(7); }

void TVisionDialog::ShowROIs() {
  for (int Nr=0;Nr<NrUsedROIs;Nr++) ShowROI(Nr,false);
}

 void TVisionDialog::ShowROI(int Nr, bool DisplayError) {
	 if (ROICam[Nr]!=MD->y("Camera")) {
		if (DisplayError) {
			char buf[200];
			int help=MD->y("Camera");
			sprintf(buf,"ROI %i belongs to Camera %i and not Camera %i which is currently displayed.",Nr,ROICam[Nr],help);
			MessageBox(buf,"Error");
		}
		return;
	 }
   double posx1,posx2,posy1,posy2;
  double posx1i,posx2i,posy1i,posy2i;
    posy1i=ROI[8*Nr+0];
    posy2i=ROI[8*Nr+1];
    posx1i=ROI[8*Nr+2];
    posx2i=ROI[8*Nr+3];
    posy1=ROI[8*Nr+4];
    posy2=ROI[8*Nr+5];
    posx1=ROI[8*Nr+6];
    posx2=ROI[8*Nr+7];
    picture->Line(posx1,posy1,posx1,posy2,62000000,2);//red outer
    picture->Line(posx1,posy2,posx2,posy2,62000000,2);
    picture->Line(posx2,posy2,posx2,posy1,62000000,2);
    picture->Line(posx2,posy1,posx1,posy1,62000000,2);
    picture->Line(posx1i,posy1i,posx1i,posy2i,32000000,2);//green  inner
    picture->Line(posx1i,posy2i,posx2i,posy2i,32000000,2);
    picture->Line(posx2i,posy2i,posx2i,posy1i,32000000,2);
    picture->Line(posx2i,posy1i,posx1i,posy1i,32000000,2);
   PictureWin->Update(MD->y("ProfilAverage"));
}

void TVisionDialog::IntegralAtomNumber() {
  GetProfilPos();
  MD->y("NIntegral")=MD->y("Calibration")*/*opticalDensity*/picture->Integral(MD->y("Pos1Xs"),
      MD->y("Pos1Ys"),MD->y("Pos1Xe"),MD->y("Pos1Ye"),IntegralWithOffset,
      MD->y("Pos2Xs"),MD->y("Pos2Ys"),MD->y("Pos2Xe"),MD->y("Pos2Ye"),WV,WH,MD->y("NIntOffset"))*1E-6/
    Sigma(MD->y("ProbeDetuning"),MD->y("Linewidth"),MD->y("Wavelength"),MD->y("Clebsch"));
  GaussFitTextBox->SetValue(5,MD->y("NIntegral"));
  GaussFitTextBox->SetValue(13,MD->y("NIntOffset")*1E4);
/*  double h=pow(2*M_PI,1.5)*(MD->y("SigmaX")*1E-3*MD->y("SigmaY")*1E-3*
    sqrt(fabs(MD->y("SigmaX")*1E-3*MD->y("SigmaY")*1E-3)));
  if (h==0) h=100000;
  MD->y("n0Int")=1E-6*MD->y("NIntegral")/h;*/
  for (int Nr=0;Nr<NrUsedROIs;Nr++) {
	char buf[300];
	sprintf(buf,"NROI%i",Nr);
	if (ROICam[Nr]==MD->y("Camera")) {		
		char bufof[300];
		sprintf(bufof,"NOfROI%i",Nr);
		MD->y(buf)=MD->y("Calibration")*/*opticalDensity*/
			picture->Integral(
			ROI[8*Nr+2],
			ROI[8*Nr+0],
			ROI[8*Nr+3],
			ROI[8*Nr+1],
			IntegralWithOffset,
			ROI[8*Nr+6],
			ROI[8*Nr+4],
			ROI[8*Nr+7],
			ROI[8*Nr+5],	  
			WV,WH,MD->y(bufof)
		)*1E-6/Sigma(MD->y("ProbeDetuning"),MD->y("Linewidth"),MD->y("Wavelength"),MD->y("Clebsch"));		
	  }
	  if (Nr<4) GaussFitTextBox->SetValue(15+Nr,MD->y(buf));
	  else GaussFitTextBox->SetValue(16+Nr,MD->y(buf));
  }
}

void TVisionDialog::CalculateFormulas() {
	//Insert here your own formulas to calculate derived quantities using measured numbers, sizes etc.
   double help=(MD->y("NROI0")+MD->y("NROI1"));
   if (help<1E-50) help=1E-50;
	MD->y("F0")=(MD->y("NROI0"))/help;

   help=(MD->y("NROI2")+MD->y("NROI3"));
   if (help<1E-50) help=1E-50;
	MD->y("F1")=2*(MD->y("NROI1"))/help;

help=(MD->y("NROI1")+MD->y("NROI3"));
   if (help<1E-50) help=1E-50;
	MD->y("F2")=2*(MD->y("NROI2"))/help;

	help=(MD->y("NROI1")+MD->y("NROI2"));
   if (help<1E-50) help=1E-50;
	MD->y("F3")=2*(MD->y("NROI3"))/help;

	MD->y("F4")=MD->y("NROI2")*5+MD->y("NROI3");
	char buf[300];
	for (int i=0;i<5;i++) {
		sprintf(buf,"F%i",i);
		GaussFitTextBox->SetValue(i*5+4,MD->y(buf));
	}
}



void TVisionDialog::GetOpticalDensity() {
  double OptDens;
  switch (OptDensSource) {
    case 1:OptDens=(MD->y("AmpX")+MD->y("AmpY"))/2.0;break;
    case 2:OptDens=MD->y("AmpX");break;
    case 3:OptDens=MD->y("AmpY");break;
    default: OptDens=opticalDensity->GetAbsorption(MD->y("X0"),MD->y("Y0"));
  }
  MD->y("OpticalDensity")=OptDens;
  if (CursorFixed) {
	 MD->y("X0")=FixedX0;
	 MD->y("Y0")=FixedY0;
  }
}                                                                    

void TVisionDialog::Fit1DGauss() {
  double herr,hchisq,verr,vchisq;
  Fit1DHV(1,MD->y("AmpX"),MD->y("X0"),MD->y("SigmaX"),MD->y("GOffsetX"),MD->y("GSlopeX"),MD->y("XRMS"),gauss,herr,hchisq);
  Fit1DHV(0,MD->y("AmpY"),MD->y("Y0"),MD->y("SigmaY"),MD->y("GOffsetY"),MD->y("GSlopeY"),MD->y("YRMS"),gauss,verr,vchisq);
  char buf[200];
  sprintf(buf,"Horizontal (X): Err=%.0f Chisqu=%.2E   Vertical (Y): Err=%.0f Chisqu=%.2E ",herr,hchisq,verr,vchisq);
  mb->SetText(buf);
  MD->y("FitChiSq")=vchisq;
  profilH->SetGauss(MD->y("AmpX"),MD->y("X0"),MD->y("SigmaX"),MD->y("GOffsetX"),MD->y("GSlopeX"));
  profilV->SetGauss(MD->y("AmpY"),MD->y("Y0"),MD->y("SigmaY"),MD->y("GOffsetY"),MD->y("GSlopeY"));
  GetOpticalDensity();
//  GaussFitTextBox->SetValue(0,OptDens);
//  GaussFitTextBox->SetValue(1,X0);
//  GaussFitTextBox->SetValue(2,SigmaX);
//  GaussFitTextBox->SetValue(3,OffsetX);
  double sx=MD->y("SigmaX");
  double sy=MD->y("SigmaY");
  MD->y("NGauss")=MD->y("Calibration")*Number(sx*1E-3,sy*1E-3,
    MD->y("OpticalDensity"),MD->y("ProbeDetuning"),MD->y("Linewidth"),MD->y("Wavelength"),MD->y("Clebsch"));
    double h1=fabs(sx*1E-3*sy*1E-3);
  double h=15.74961/*pow(2*M_PI,1.5)*/*(sx*1E-3*sy*1E-3*sqrt(h1));
  if (h==0) h=100000;
  MD->y("n0Gauss")=1E-6*MD->y("NGauss")/h;
//  GaussFitTextBox->SetValue(8,N);
//  GaussFitTextBox->SetValue(5,Y0);
//  GaussFitTextBox->SetValue(6,SigmaY);
//  GaussFitTextBox->SetValue(7,OffsetY);
  DisplayGaussFitResults();
  CalculateTOFTemperature();
  CalculateT_TF();
  Calculaten0rho0();
  MD->y("1DFitFunction")=1;
}

void TVisionDialog::Fit1DExponential() {
  double herr,hchisq,verr,vchisq;
  Fit1DHV(1,MD->y("AmpX"),MD->y("X0"),MD->y("SigmaX"),MD->y("GOffsetX"),MD->y("GSlopeX"),MD->y("XRMS"),exponential,herr,hchisq);
  Fit1DHV(0,MD->y("AmpY"),MD->y("Y0"),MD->y("SigmaY"),MD->y("GOffsetY"),MD->y("GSlopeY"),MD->y("YRMS"),exponential,verr,vchisq);
  char buf[200];
  sprintf(buf,"Horizontal (X): Err=%.0f Chisqu=%.2e   Vertical (Y): Err=%.0f Chisqu=%.2e ",herr,hchisq,verr,vchisq);
  mb->SetText(buf);
  GetOpticalDensity();
//  GaussFitTextBox->SetValue(0,OptDens);
//  GaussFitTextBox->SetValue(1,X0);
//  GaussFitTextBox->SetValue(2,SigmaX);
//  GaussFitTextBox->SetValue(3,OffsetX);
  MD->y("NGauss")=MD->y("Calibration")*Number(MD->y("SigmaX")*1E-3,MD->y("SigmaY")*1E-3,
    MD->y("OpticalDensity"),MD->y("ProbeDetuning"),MD->y("Linewidth"),MD->y("Wavelength"),MD->y("Clebsch"));
//  GaussFitTextBox->SetValue(8,N);
//  GaussFitTextBox->SetValue(5,Y0);
//  GaussFitTextBox->SetValue(6,SigmaY);
//  GaussFitTextBox->SetValue(7,OffsetY);
  profilH->SetGauss(MD->y("AmpX"),MD->y("X0"),MD->y("SigmaX"),MD->y("GOffsetX"),MD->y("GSlopeX"));
  profilV->SetGauss(MD->y("AmpY"),MD->y("Y0"),MD->y("SigmaY"),MD->y("GOffsetY"),MD->y("GSlopeY"));
  DisplayGaussFitResults();
  CalculateTOFTemperature();
  CalculateT_TF();
  Calculaten0rho0();
  MD->y("1DFitFunction")=2;
}

void TVisionDialog::DoAutomaticFit() {
  if (autofit) {
    if ((MD->y("2DFitFunction")>0) && picture==opticalDensity && Display2DFitProfil) { Display2DProfils();DisplayIoffeFit(false);}
    else {
      Fit1DGaussToSum();
      IntegralAtomNumber();
	  CalculateFormulas();
    }
  }
}

void TVisionDialog::ShowFluorescence() {
  picture=probenoisepicture;
  PictureWin->Update(MD->y("ProfilAverage"));
  DoAutomaticFit();
}

void TVisionDialog::ShowAbsorption() {
  picture=abspicture;
  PictureWin->Update(MD->y("ProfilAverage"));
  DoAutomaticFit();
}

void TVisionDialog::ShowOpticalDensity() {
  picture=opticalDensity;
  PictureWin->Update(MD->y("ProfilAverage"));
  DoAutomaticFit();
}

void TVisionDialog::ShowAbsDirect() {
  picture=pictureabs;
  PictureWin->Update(MD->y("ProfilAverage"));
  DoAutomaticFit();
}

void TVisionDialog::ShowProbe() {
  picture=pictureprobe;
  PictureWin->Update(MD->y("ProfilAverage"));
  DoAutomaticFit();
}

void TVisionDialog::ShowNoise() {
  picture=picturenoiseabs;
  PictureWin->Update(MD->y("ProfilAverage"));
  DoAutomaticFit();
}

void TVisionDialog::ShowPicture(int picNr) {
  switch (picNr) {
	 case 0:picture=picturenoiseprobe;break;
	 case 1:picture=abspicture;break;
	 case 2:picture=pictureabs;break;
	 case 3:picture=pictureprobe;break;
	 case 4:picture=picturenoiseabs;break;
	 case 5:picture=opticalDensity;break;
  }
  PictureWin->Update(MD->y("ProfilAverage"));
  DoAutomaticFit();
}

void TVisionDialog::ToggleProfilDesign() {
  ProfilDesign=!ProfilDesign;
  profilH->ToggleDesign();
  profilV->ToggleDesign();
}

LRESULT TVisionDialog::PictureChanged(WPARAM newpicture,LPARAM l) {
  if ((newpicture>=ID_PICLIST0) && (newpicture<ID_PICLIST0+NrPictureLists)) {
      if (PictureList[newpicture-ID_PICLIST0]->GetName(l)) LoadMeasurement(PictureList[newpicture-ID_PICLIST0]->GetName(l),false,false,newpicture-ID_PICLIST0,true);
      return 0;
  }
  if ((newpicture>=ID_DETAILPICTUREBUTTON0) && (newpicture<ID_DETAILPICTUREBUTTON0+NrDetailPictureButtons)) {
  		picture=PictureOfDetailButton[newpicture-ID_DETAILPICTUREBUTTON0];
  }
  PictureWin->Update(MD->y("ProfilAverage"));
  DoAutomaticFit();
  return 0;
}

LRESULT TVisionDialog::PaletteChanged(WPARAM ,LPARAM) {
  UpdateAll();
  for (int i=0;i<NrPictureLists;i++) PictureList[i]->Update();
  return 0;
}

LRESULT TVisionDialog::LoadMess(WPARAM w,LPARAM) {
  int iso=TNmeasure->GetMesIsotop();
  switch (w) {
    case 0:
      MD->SetMetaPointer(NULL);
      if (!MD->IsInList()) {
        #if defined(DetectLeaks)
          LeakDetect.Delete(MD);
        #endif
        delete MD;
      }
      MD=TNmeasure->GetLoadPoint();
      MD->SetMetaPointer(MetaPointer);
      LoadMeasurement(TNmeasure->GetMesName(),true,true,iso,true,false);
    break;
    case 2:
      //called from CTNMeasure::ButtonAction
      MD->SetMetaPointer(NULL);
      if (!MD->IsInList()) {
        #if defined(DetectLeaks)
          LeakDetect.Delete(MD);
        #endif
        delete MD;
      }
      if (UpdateMeasurementAfterFit) {
        MD=TNmeasure->GetLoadPoint();
        LoadMeasurement(TNmeasure->GetMesName(),true,true,iso,false,true);
      } else {
        CMessPoint *HelpMD=MD;
        MD=new CMessPoint();
        #if defined(DetectLeaks)
          LeakDetect.New(MD,5,60);
        #endif
        MD->Copy(HelpMD);
        LoadMeasurement(TNmeasure->GetMesName(),true,false,iso,false,true);
      }
      MD->SetMetaPointer(MetaPointer);
    break;
  }
  return 0;
}

LRESULT TVisionDialog::IteratePicture(WPARAM w,LPARAM) {
  switch (IteratePictureOccupation) {
    case 0:
      char buf[200];
      Name->GetFilmPictureName(buf);
      picture->Save(buf,SAVE_PALETTEBMP,ColorTable);
    break;
    case 1:
      if ((MD->y("2DFitFunction")!=1) || (!Fit2DOnlyNotFitted)) FitIoffeDistribution();
    break;
    case 2:
      if ((MD->y("2DFitFunction")!=2) || (!Fit2DOnlyNotFitted)) Fit2DGaussDistribution();
    break;
    case 3:
      Averagepictureabs->Add(*pictureabs);
      Averagepictureprobe->Add(*pictureprobe);
      Averagepicturenoiseabs->Add(*picturenoiseabs);
      Averagepicturenoiseprobe->Add(*picturenoiseprobe);
      Averageprobenoisepicture->Add(*probenoisepicture);
      AveragePicNr++;
    break;
    case 4:
      Averagepictureabs->Add(*abspicture);
      Averagepictureprobe->Add(*opticalDensity);
      AveragePicNr++;
    break;
    case 5:
      FitPartialFermi();
    break;
    case 6:
      FitGaussWingsBothDirections();
    break;
    case 7:
      FitGaussWingsBothDirections();
      FitFermi();
    break;
    case 8:
      FitGaussWingsBothDirections();
      FitBose();
    break;
  }
  return 0;
}

void TVisionDialog::CreateFilm() {
  Name->NewFilm();
  IteratePictureOccupation=0;
  TNmeasure->Iterate();
  mb->SetText("Film created");
}

void TVisionDialog::AveragePicture() {
/*  MD->SetMetaPointer(NULL);
  if (!MD->IsInList()) delete MD;
  int err;
  MD=new CMessPoint();
  MD->SetMetaPointer(MetaPointer);*/

  AveragePicNr=0;
  Averagepictureabs=new CPicture(*pictureabs);
  #if defined(DetectLeaks)
     LeakDetect.New(Averagepictureabs,5,61);
  #endif
  Averagepictureprobe=new CPicture(*pictureprobe);
  #if defined(DetectLeaks)
     LeakDetect.New(Averagepictureprobe,5,62);
  #endif
  Averagepicturenoiseabs=new CPicture(*picturenoiseabs);
  #if defined(DetectLeaks)
     LeakDetect.New(Averagepicturenoiseabs,5,63);
  #endif
  Averagepicturenoiseprobe=new CPicture(*picturenoiseprobe);
  #if defined(DetectLeaks)
     LeakDetect.New(Averagepicturenoiseprobe,5,63);
  #endif
  Averageprobenoisepicture=new CPicture(*probenoisepicture);
  #if defined(DetectLeaks)
     LeakDetect.New(Averageprobenoisepicture,5,63);
  #endif
  Averagepictureabs->Clear();
  Averagepictureprobe->Clear();
  Averagepicturenoiseabs->Clear();
  Averagepicturenoiseprobe->Clear();
  Averageprobenoisepicture->Clear();
  IteratePictureOccupation=3;
  TNmeasure->Iterate();
  Averagepictureabs->Divide(AveragePicNr);
  Averagepictureprobe->Divide(AveragePicNr);
  Averagepicturenoiseabs->Divide(AveragePicNr);
  Averagepicturenoiseprobe->Divide(AveragePicNr);
  Averageprobenoisepicture->Divide(AveragePicNr);
  mb->SetText("Picture averaged and saved as AverageA.bmp or AverageA.tga");
  char buf[200];
  char buf2[200];
  Name->GetPath(buf2);
  sprintf(buf,"%s%s",buf2,"\Average%c.bmp");
  sprintf(buf2,buf,'A');
  strcpy(LastName,buf);
  int err1=pictureabs->Save(buf2,SAVE_AUTOMATIC,ColorTable);
  MD->SetText("Filename",buf2);
  mb->SetText(buf2);
  sprintf(buf2,buf,'P');
  int err2=pictureprobe->Save(buf2,SAVE_AUTOMATIC,ColorTable);
  sprintf(buf2,buf,'N');
  int err3=picturenoiseabs->Save(buf2,SAVE_AUTOMATIC,ColorTable);
  sprintf(buf2,buf,'O');
  int err4=picturenoiseprobe->Save(buf2,SAVE_AUTOMATIC,ColorTable);
  #if defined(DetectLeaks)
     LeakDetect.Delete(pictureabs);
  #endif
  delete pictureabs;
  #if defined(DetectLeaks)
     LeakDetect.Delete(pictureprobe);
  #endif
  delete pictureprobe;
  #if defined(DetectLeaks)
     LeakDetect.Delete(picturenoiseabs);
  #endif
  delete picturenoiseabs;
  #if defined(DetectLeaks)
     LeakDetect.Delete(picturenoiseprobe);
  #endif
  delete picturenoiseprobe;
  #if defined(DetectLeaks)
     LeakDetect.Delete(probenoisepicture);
  #endif
  delete probenoisepicture;
  pictureabs=Averagepictureabs;
  pictureprobe=Averagepictureprobe;
  picturenoiseabs=Averagepicturenoiseabs;
  picturenoiseprobe=Averagepicturenoiseprobe;
  probenoisepicture=Averageprobenoisepicture;

  GetProfilPos();
  double posx1,posx2,posy1,posy2;
  profilH->GetPos(posx1,posx2);
  profilV->GetPos(posy1,posy2);
  if (MD->y("FastKinetics")==1) {
    LnNormalizeAbsorption(*pictureabs,*pictureprobe,picturenoiseabs,picturenoiseprobe,
       *abspicture,*opticalDensity,MaxOptDensity,
       posx1,posx2,posy1,posy2,MD->y("Fluo"),MD->y("MaxFluo"),DoIntensityCompensation,&ROI[8*8],WV,WH,MD->y("FastKinetics"));
  } else {
    LnNormalizeAbsorption(*pictureabs,*pictureprobe,picturenoiseabs,picturenoiseprobe,*abspicture,
      *opticalDensity,MaxOptDensity,posx1,posx2,posy1,posy2,
      MD->y("Fluo"),MD->y("MaxFluo"),DoIntensityCompensation,&ROI[8*8],WV,WH,MD->y("FastKinetics"));
  }
  picture=opticalDensity;
  switch (MainPictureMode) {
    case 0:picture=pictureabs;break;
    case 1:picture=pictureprobe;break;
    case 2:picture=picturenoiseabs;break;
    case 3:picture=abspicture;break;
    case 4:picture=opticalDensity;break;
  }
  UpdateAll();
  Load2DFitParams();
  IntegralAtomNumber();  
  DoAutomaticFit();
  CalculateFormulas();
 /* if (insert) {
    PictureList6Li->AddPicture(picture,"noname");
  }*/
  DisplayMessData();
  if (UpdateMeasurementAfterFit && MD->IsInList()) {
    TNmeasure->Draw();
  }
}

void TVisionDialog::AverageOpticalDensity() {
  AveragePicNr=0;
  Averagepictureabs=new CPicture(*abspicture);
  #if defined(DetectLeaks)
     LeakDetect.New(Averagepictureabs,5,64);
  #endif
  Averagepictureprobe=new CPicture(*opticalDensity);
  #if defined(DetectLeaks)
     LeakDetect.New(Averagepictureprobe,5,65);
  #endif
  Averagepictureabs->Clear();
  Averagepictureprobe->Clear();
  IteratePictureOccupation=4;
  TNmeasure->Iterate();
  Averagepictureabs->Divide(AveragePicNr);
  Averagepictureprobe->Divide(AveragePicNr);
  mb->SetText("Optical Density averaged");
  char buf[200];
  #if defined(DetectLeaks)
     LeakDetect.Delete(abspicture);
  #endif
  delete abspicture;
  #if defined(DetectLeaks)
     LeakDetect.Delete(opticalDensity);
  #endif
  delete opticalDensity;
  abspicture=Averagepictureabs;
  opticalDensity=Averagepictureprobe;
  picture=opticalDensity;
  switch (MainPictureMode) {
    case 0:picture=pictureabs;break;
    case 1:picture=pictureprobe;break;
    case 2:picture=picturenoiseabs;break;
    case 3:picture=abspicture;break;
    case 4:picture=opticalDensity;break;
  }
  UpdateAll();
  Load2DFitParams();
  IntegralAtomNumber();
  DoAutomaticFit();
  CalculateFormulas();
 /* if (insert) {
    PictureList6Li->AddPicture(picture,"noname");
  }*/
  DisplayMessData();
  if (UpdateMeasurementAfterFit && MD->IsInList()) {
    TNmeasure->Draw();
  }
}

void TVisionDialog::ReCalcIoffeFit() {
  bool oldUpdateMeasurementAfterFit=UpdateMeasurementAfterFit;
  UpdateMeasurementAfterFit=true;
  IteratePictureOccupation=1;
  TNmeasure->Iterate();
  mb->SetText("Ioffe fits recalculated");
  UpdateMeasurementAfterFit=oldUpdateMeasurementAfterFit;
}

void TVisionDialog::ReCalc2DGauss() {
  bool oldUpdateMeasurementAfterFit=UpdateMeasurementAfterFit;
  UpdateMeasurementAfterFit=true;
  IteratePictureOccupation=2;
  TNmeasure->Iterate();
  mb->SetText("2D Gauss fits recalculated");
  UpdateMeasurementAfterFit=oldUpdateMeasurementAfterFit;
}

void TVisionDialog::ReCalcPartialFermi() {
  bool oldUpdateMeasurementAfterFit=UpdateMeasurementAfterFit;
  UpdateMeasurementAfterFit=true;
  IteratePictureOccupation=5;
  TNmeasure->Iterate();
  mb->SetText("Partial Fermi fits recalculated");
  UpdateMeasurementAfterFit=oldUpdateMeasurementAfterFit;
}

void TVisionDialog::ReCalcGaussWings() {
  bool oldUpdateMeasurementAfterFit=UpdateMeasurementAfterFit;
  UpdateMeasurementAfterFit=true;
  IteratePictureOccupation=6;
  TNmeasure->Iterate();
  mb->SetText("Gauss wings fits recalculated");
  UpdateMeasurementAfterFit=oldUpdateMeasurementAfterFit;
}

void TVisionDialog::ReCalcFermi() {
  bool oldUpdateMeasurementAfterFit=UpdateMeasurementAfterFit;
  UpdateMeasurementAfterFit=true;
  IteratePictureOccupation=7;
  TNmeasure->Iterate();
  mb->SetText("Fermi fits recalculated");
  UpdateMeasurementAfterFit=oldUpdateMeasurementAfterFit;
}

void TVisionDialog::ReCalcBose() {
  bool oldUpdateMeasurementAfterFit=UpdateMeasurementAfterFit;
  UpdateMeasurementAfterFit=true;
  IteratePictureOccupation=8;
  TNmeasure->Iterate();
  mb->SetText("Bose fits recalculated");
  UpdateMeasurementAfterFit=oldUpdateMeasurementAfterFit;
}

void TVisionDialog::DeleteMeasurement() {
  TNmeasure->DeleteAktPoint();
}

LRESULT TVisionDialog::Fit1DGauss(WPARAM ,LPARAM) {
  if (autofit) {
    if ((MD->y("2DFitFunction")>0) && picture==opticalDensity && Display2DFitProfil) DisplayCutOf2DProfils(); else Fit1DGauss();
  }
  return 0;
}

void TVisionDialog::FitTemperature() {
  TNmeasure->TemperatureFit(MD->y("Mass")*_mp);
}

void TVisionDialog::FitAtomNumber() {
//  TNmeasure->NumberFit();
}

void TVisionDialog::FitSimpleAtomNumber() {
  TNmeasure->SimpleNumberFit();
}

void TVisionDialog::FitChargingTime() {
  TNmeasure->ChargingTimeFit();
}

void TVisionDialog::FitLifeTime() {
  TNmeasure->LifetimeFit();
}

LRESULT TVisionDialog::CursorMoved(WPARAM ,LPARAM) {
  double Picxpos,Picypos,height,xpos,ypos;
  int w,h;
  PictureWin->GetPos(xpos,ypos);
  PictureWin->GetExtensions(w,h);
  Picxpos=picture->PicCoordsX(xpos,w,h);
  Picypos=picture->PicCoordsY(ypos,w,h);
  height=picture->GetEichPixel(Picxpos,Picypos);
  if (CursorFixed==1) {
    CursorPosX=FixedX0;
    CursorPosY=FixedY0;
  } else {
    CursorPosX=picture->GetEichPosX(Picxpos);
    CursorPosY=picture->GetEichPosY(Picypos);
  }
  CursorTextBox->SetValue(0,CursorPosX);
  CursorTextBox->SetValue(1,CursorPosY);
  CursorTextBox->SetValue(2,height);
  return 0;
}

LRESULT TVisionDialog::ProfilCursorMoved(WPARAM ,LPARAM) {
  double xpos1,ypos1,xpos2,ypos2;
  profilH->GetRealPos(xpos1,xpos2);
  profilV->GetRealPos(ypos1,ypos2);
  CursorTextBox->SetValue(3,xpos1);
  CursorTextBox->SetValue(4,xpos2-xpos1);
  CursorTextBox->SetValue(5,ypos1);
  CursorTextBox->SetValue(6,ypos2-ypos1);
  return 0;
}

void TVisionDialog::UpdateAll() {
  MovePicturesToDetailPictureButtonsAndRedraw();
  PictureWin->Update(MD->y("ProfilAverage"));
}

void TVisionDialog::SetStandardPalette() {
  ColorTable->SetStandard();
  UpdateAll();
}

void TVisionDialog::SetStandardPalette2() {
  ColorTable->SetStandard2();
  UpdateAll();
}

void TVisionDialog::SetBlackWhitePalette() {
  ColorTable->SetBlackWhite();
  UpdateAll();
}

void TVisionDialog::SetWhiteBlackPalette() {
  ColorTable->SetWhiteBlack();
  UpdateAll();
}

void TVisionDialog::SetLowBytePalette() {
  ColorTable->SetLowByte();
  UpdateAll();
}

void TVisionDialog::AddStripes() {
  ColorTable->AddStripes();
  UpdateAll();
}

void TVisionDialog::FixCursor() {
  if (CursorFixed==0) {
    CursorFixed=1;
    FixedX0=CursorPosX;
    FixedY0=CursorPosY;
    mb->SetText("Cursor fixed");
    PictureWin->SetCrossfixed(1,FixedX0,FixedY0);
  } else {
    CursorFixed=0;
    mb->SetText("Cursor unfixed");
    PictureWin->SetCrossfixed(0,FixedX0,FixedY0);
  }
}

void TVisionDialog::RegisterInformation(int NoSerie,CMessPoint *aMD) {
  MakePictureCode();
  TNmeasure->AddPoint(NoSerie,aMD,LogbookEdit);
}

bool TVisionDialog::NewMDForAbsPicture() {
  MD->SetMetaPointer(NULL);
  CMessPoint *help;
  if (InSerie) {
  		help=new CMessPoint(MD);
      #if defined(DetectLeaks)
        LeakDetect.New(help,5,66);
      #endif
      help->CopyData(MD);
	//	MD->next=help;
  } else {
    help=new CMessPoint();
    #if defined(DetectLeaks)
        LeakDetect.New(help,5,67);
    #endif
 	 help->CopyData(MD);
  }
//  if ((MD->y("MainPicture")) && TwoPictureCopy) help->Copy(MD);
  if (MD) {
    if (!MD->IsInList()) {
      #if defined(DetectLeaks)
        LeakDetect.Delete(MD);
      #endif
    	delete MD;
    }
  }
  MD=help;
  MD->SetMetaPointer(MetaPointer);
  //MDSetCameraParameters();
  return true;
}

bool TVisionDialog::NewMD() {
  MD->SetMetaPointer(NULL);
  CMessPoint *help;
  help=new CMessPoint(MD);
  #if defined(DetectLeaks)
        LeakDetect.New(help,5,68);
  #endif
  help->Copy(MD);
  MD=help;
  MD->SetMetaPointer(MetaPointer);
  return true;
}

bool TVisionDialog::CheckNoArgCommands(char *Command) {
	if (strcmp(Command,"VisionSendDataFile")==0) {
     ReceiveDataFile();
	} else if (strcmp(Command,"VisionTakeNetAndorPic")==0) {
      int Camera;
      serial->GetInt(Camera);
		NetAndorPicture(Camera);
	} else if (strcmp(Command,"VisionRunFinished")==0) {
		RunFinished=true;
	} else if (strcmp(Command,"VisionCCDFilmStart")==0) {
      StartFilming(9);
	} else if (strcmp(Command,"VisionCCDFilmStop")==0) {
      StopFilming();
	} else if (strcmp(Command,"VisionMainPictureShadow")==0) {
     MainPictureMode=0;
	} else if (strcmp(Command,"VisionMainPictureProbe")==0) {
     MainPictureMode=1;
	} else if (strcmp(Command,"VisionMainPictureNoise")==0) {
     MainPictureMode=2;
	} else if (strcmp(Command,"VisionMainPictureAbsorption")==0) {
     MainPictureMode=3;
	} else if (strcmp(Command,"VisionMainPictureOpticalDensity")==0) {
     MainPictureMode=4;
	} else if (strcmp(Command,"VisionStopSerie")==0) {
	  StopSerie();
  } else if (strcmp(Command,"VisionContinueSerie")==0) {
    ContinueSerie();
  } else if (strcmp(Command,"VisionCreateSubstractNoisePictures")==0) {
    CreateSubstractNoisePictures();
  } else if (strcmp(Command,"VisionCheckNumber")==0) {
    CheckNumber=1;
  } else if (strcmp(Command,"VisionTemperatureFit")==0) {
    TNmeasure->TemperatureFit(MD->y("Mass")*_mp);
  } else if (strcmp(Command,"VisionChargingTimeFit")==0) {
    TNmeasure->ChargingTimeFit();
	} else if (strcmp(Command,"VisionFixCursor")==0) {
    FixCursor();
  } else if (strcmp(Command,"VisionStopFilming")==0) {
    StopFilming();
  } else if (strcmp(Command,"VisionMandelbrot")==0) {
    MandelBrot();
  } else if (strcmp(Command,"VisionGauss")==0) {
    Gauss();
  } else if (strcmp(Command,"VisionGridOnOff")==0) {
    GridOnOff();
	} else if (strcmp(Command,"VisionCoolerOff")==0) {
			#if defined(ApogeeCameraUsed)
				if (ApogeeFrameGrabber) ApogeeFrameGrabber->SwitchCoolerOff();
			#endif
	} else if (strcmp(Command,"VisionProfilDesign")==0) {
		ToggleProfilDesign();
  } else if (strcmp(Command,"VisionLifetimeFit")==0) {
    TNmeasure->LifetimeFit();
	} else if (strcmp(Command,"VisionReady")==0) {
  } else if (strcmp(Command,"VisionNumberFit")==0) {
//    TNmeasure->NumberFit();
  } else if (strcmp(Command,"VisionSimpleNumberFit")==0) {
    TNmeasure->SimpleNumberFit();
  } else return false;
  return true;
}

void TVisionDialog::CheckSerial() {
  if (CloseVisionFlag) {
      CanClose();
      Close();
     	ExitProcess(1);
  }
  if (!checkingSerial && serial) {
	 checkingSerial=1;
	 char Command[100];
	 if (serial->GetCommand(Command)) {
		mb->SetText(Command);
			if (CheckNoArgCommands(Command)) {
				serial->SendCommand("*Ready#");
      } else if (strcmp(Command,"VisionPictureNumber")==0) {
        serial->SendCommand("*Ready#");
        char buf[200];
        sprintf(buf,"*%i#",Name->aktPictureNumber);
        serial->SendCommand(buf);
		sprintf(buf,"*%i#",Name->aktSerialNumber);
        serial->SendCommand(buf);
      } else if (strcmp(Command,"VisionGetIntegralAtomNumber")==0) {
        serial->SendCommand("*Ready#");
        char buf[200];
        sprintf(buf,"*%.3f#",MD->y("NIntegral"));
        serial->SendCommand(buf);
      } else if (strcmp(Command,"VisionGetBarPosition")==0) {
        serial->SendCommand("*Ready#");
        GetProfilPos();
        char buf[200];
        sprintf(buf,"*%.3f#",MD->y("Pos1Xs"));
        serial->SendCommand(buf);
        sprintf(buf,"*%.3f#",MD->y("Pos1Xe"));
        serial->SendCommand(buf);
        sprintf(buf,"*%.3f#",MD->y("Pos1Ys"));
        serial->SendCommand(buf);
        sprintf(buf,"*%.3f#",MD->y("Pos1Ye"));
        serial->SendCommand(buf);
        sprintf(buf,"*%.3f#",MD->y("Pos2Xs"));
        serial->SendCommand(buf);
        sprintf(buf,"*%.3f#",MD->y("Pos2Xe"));
        serial->SendCommand(buf);
        sprintf(buf,"*%.3f#",MD->y("Pos2Ys"));
        serial->SendCommand(buf);
        sprintf(buf,"*%.3f#",MD->y("Pos2Ye"));
        serial->SendCommand(buf);
      } else if (strcmp(Command,"VisionSetBarPosition")==0) {
		  serial->SendCommand("*Ready#");
        serial->GetDouble(MD->y("Pos1Xs"));
        serial->GetDouble(MD->y("Pos1Xe"));
        serial->GetDouble(MD->y("Pos1Ys"));
        serial->GetDouble(MD->y("Pos1Ye"));
        serial->GetDouble(MD->y("Pos2Xs"));
        serial->GetDouble(MD->y("Pos2Xe"));
        serial->GetDouble(MD->y("Pos2Ys"));
        serial->GetDouble(MD->y("Pos2Ye"));
        SetProfilPos();
		} else if (strcmp(Command,"VisionCameraReady")==0) {
		  serial->SendCommand("*Ready#");
        int Camera;
        serial->GetInt(Camera);
        switch (Camera) {
          case 11: //NetAndor
            #if defined(NetAndorCameraUsed)
            serial->SendCommand("*VisionCameraReady#");
            #endif
          break;
        }
      } else if (strcmp(Command,"VisionSetCameraParameters")==0) {
        FKBining=1;
		  serial->SendCommand("*Ready#");
        int Camera;
        serial->GetInt(Camera);
        double XCal;
        serial->GetDouble(XCal);
        double YCal;
        serial->GetDouble(YCal);
        int CamStartX;
        serial->GetInt(CamStartX);
        int CamStartY;
        serial->GetInt(CamStartY);
				int CamWidth;
        serial->GetInt(CamWidth);
        int CamHeight;
        serial->GetInt(CamHeight);
        serial->GetInt(BiningX);
        serial->GetInt(BiningY);
        switch (Camera) {
               case 11:
                  #if defined(NetAndorCameraUsed)
                  int CameraNr;
                  serial->GetInt(CameraNr);
                  char CameraDirection[300];
                  serial->GetString(CameraDirection);
                  int CameraUsed;
                  serial->GetInt(CameraUsed);
                  int DisplayedPictureNumber;
                  serial->GetInt(DisplayedPictureNumber);
                  int MainCamera;
                  serial->GetInt(MainCamera);
                  if (CameraNr<0) CameraNr=0;
				  //if (CameraNr>=NrNetAndorCameras) CameraNr=NrNetAndorCameras-1;
				  double ExposureTime3;
				  serial->GetDouble(ExposureTime3);
                  double AndorTemperature;
	              serial->GetDouble(AndorTemperature);
                  int ExternalTrigger;
                  serial->GetInt(ExternalTrigger);
                  int vSpeed;
                  serial->GetInt(vSpeed);
                  double hSpeed;
//                  serial->GetInt(hSpeed);
                  serial->GetDouble(hSpeed);
                  int FKSMode;
                  serial->GetInt(FKSMode);
                  int FKSHeight;
                  serial->GetInt(FKSHeight);
                  int FKSDataImages;
                  serial->GetInt(FKSDataImages);
				  int FKSReferenceImages;
                  serial->GetInt(FKSReferenceImages);
                  int FKSvSpeed;
                  serial->GetInt(FKSvSpeed);
                  int FKSExternalTriggerStart;
                  serial->GetInt(FKSExternalTriggerStart);
                  int FKSDirtyLines;
                  serial->GetInt(FKSDirtyLines);
                  int NumberPictures;
                  serial->GetInt(NumberPictures);
                  int PrePicture;				  
               	  serial->GetInt(PrePicture);
                  //Flushwait is the time a camera stays in the good cleaning
                  //mode after having taken a first picture before going again
                  //into the get ready to trigger mode
                  int FlushWait;
				  serial->GetInt(FlushWait);
				  //This is a trick to overcome bug of camera 1. 
				  int DoInternalTriggerAfterExternalTrigger;
				  serial->GetInt(DoInternalTriggerAfterExternalTrigger);
                  //after Vision has waited the TriggerDelay specified during
                  //the VisionTakeAbsorptionPicture command, Vision will wait
                  //an additional TriggerDelay which can be different for each
                  //camera.
 						int TriggerDelay;
                  serial->GetInt(TriggerDelay);
						double AtomicMass;
                  serial->GetDouble(AtomicMass);  //in units of mproton
                  double AtomicWavelength;
                  serial->GetDouble(AtomicWavelength);    //in meter
                  double AtomicClebschGordon;
                  serial->GetDouble(AtomicClebschGordon);   //no units
                  double AtomicLinewidth;
                  serial->GetDouble(AtomicLinewidth);    //in Hz
                  double AtomicMagneticMoment;
                  serial->GetDouble(AtomicMagneticMoment);   //in Bohr Magneton
                  char AtomicName[100];
                  serial->GetString(AtomicName);        //short: Li, K, Rb, Sr, Cs etc.
						double ProbeDetuning;
                  serial->GetDouble(ProbeDetuning);     //in Hz
                  double ExpansionTime;
                  serial->GetDouble(ExpansionTime);     //in seconds
                  int FluorescencePicture;
                  serial->GetInt(FluorescencePicture);
                  double MaxFluorescence;
                  serial->GetDouble(MaxFluorescence);
                  double Calibration;
                  serial->GetDouble(Calibration);
					int NumberOfAdditionalReferenceImages;
					serial->GetInt(NumberOfAdditionalReferenceImages);
					int DepthOfReferenceImageLibrary;
					serial->GetInt(DepthOfReferenceImageLibrary);
                  char FinalTrap[200];
          			serial->GetString(FinalTrap);
                  char EndState[200];
          			serial->GetString(EndState);
			         MD->SetText("Path",FinalTrap);
         			MD->SetText("Endstate",EndState);
                  FastKinetics=0;//this is just to tell the old parts of code that it should treat data as non FKS data since camera computer makes conversion from FKS to normal pictures
                  if (CameraNr<NrNetAndorCameras) {
                   if (NetAndorFrameGrabber[CameraNr]->TestReady()) {
							NetAndorFrameGrabber[CameraNr]->SetImageParameters(CameraDirection,
   	                 CameraUsed>0, DisplayedPictureNumber,MainCamera,
      	              BiningX,BiningY,CamStartX,CamStartY,CamWidth,CamHeight,
         	           ExposureTime3,AndorTemperature,ExternalTrigger,
            	        vSpeed,hSpeed,FKSMode,FKSHeight,FKSDataImages,FKSReferenceImages,FKSvSpeed,
               	     FKSExternalTriggerStart,FKSDirtyLines,NumberPictures,PrePicture==1,FlushWait,DoInternalTriggerAfterExternalTrigger,
                  	  TriggerDelay, AtomicMass, AtomicWavelength, AtomicClebschGordon,
	                    AtomicLinewidth, AtomicMagneticMoment, AtomicName,
   	                 ProbeDetuning, ExpansionTime,
      	              FluorescencePicture>0, MaxFluorescence, Calibration,
         	           FinalTrap, EndState, XCal, YCal,NumberOfAdditionalReferenceImages,DepthOfReferenceImageLibrary);
            	      NetAndorFrameGrabber[CameraNr]->TestReady();
                     }
                  }
                 #endif
					break;
        }
      } else if (strcmp(Command,"VisionStartFilming")==0) {
		  serial->SendCommand("*Ready#");
        int camera;
        serial->GetInt(camera);
		  StartFilming(camera);
		} else if (strcmp(Command,"VisionTakeAbsorptionPicture")==0) {
	     RunFinished=false;
        serial->SendCommand("*Ready#");
        serial->GetInt(AktCameraTyp);       //AktCameraTyp defines which cameras will be used
        double TriggerDelay,TriggerDelay2;
        switch (AktCameraTyp) {
        case 11:
          //this type of camera stores the picture parameters also in the
          //framegrabber class. This means the old style parameters are
          //not anymore used to store picture parameters and just initialized to
          //default values here. This is necessary to make it possible to take
          //up to four pictures of different atomic species in one run as required
          //for the SrBEC experiment
          AktCameraPosition=0;
          //if we have fast kinetics it is now the task of the camera computer to separate the pictures.
          //So Vision does not need to know about fast kinetics other than that it has two independent noise pictures
          FastKinetics=0;
//          MD->y("MaxFluo")=0;
          MD->y("PictureListPosition")=0;
//          MD->y("Fluo")=0;
		    MD->y("ProbeDetuning")=0;
          MD->y("ExpansionTime")=0;
		    MD->y("Calibration")=1;
          MD->y("MainPicture")=6;
          MD->y("NoisePictureMode")=0;
          MD->y("NrProbeImages")=1;
		    MD->y("UseProbeLibrary")=0;			
          //"TriggerDelay" contains the time from now to the moment at which
          //Vision should start sending the TakeAbsorptionpicture commands to
          //the cameras. This is the only parameter which is timing critical
          //and thus transfered all by itself during this command. All other
          //parameters are transfered during SetCameraParameters.
          serial->GetDouble(TriggerDelay);
          AbsorptionPicture(TriggerDelay,0,false);
        break;
        }
		} else if (strcmp(Command,"VisionStartSerie")==0) {
		  InSerie=1;
		  serial->SendCommand("*Ready#");
		  int numbParam;
		  serial->GetInt(numbParam);
        char **ParamNames=NULL;
        try {
			  ParamNames=new char* [numbParam];
			  for (int i=0; i<numbParam;i++) {
				 ParamNames[i]=new char[200];
				 serial->GetString(ParamNames[i]);
			  }
        }
        catch (xalloc) {
  				MessageBox("TVisionDialog::CheckSerial : couldn't reserve memory","Error",  MB_OK);
		      exit(-1);
		  }
		  char comentary[200];
		  serial->GetString(comentary);
        MD->SetMetaPointer(NULL);
        if (MD) if (!MD->IsInList()) {
          #if defined(DetectLeaks)
            LeakDetect.Delete(MD);
          #endif
          delete MD;
        }
        MD=new CMessPoint();
        #if defined(DetectLeaks)
          LeakDetect.New(MD,5,68);
        #endif
        MD->SetMetaPointer(MetaPointer);
        MDSetCameraParameters();
        if (InSerie) MD->SetNewXNames(numbParam,ParamNames);
		  for (int i=0; i<numbParam;i++) {
			 delete[] ParamNames[i];
		  }
        delete[] ParamNames;
		  TNmeasure->StartSerie(comentary,MD);
		} else if (strcmp(Command,"VisionRegisterInformation")==0) {
		  serial->SendCommand("*Ready#");
		  int hi=MD->GetNrX();  //the correct amount was transferred in "StartSerie"
		  for (int i=0;i<hi;i++) serial->GetDouble(MDXInformation[i]);
		} else if (strcmp(Command,"VisionAnalogIn")==0) {
		  serial->SendCommand("*Ready#");
        int ChannelNr;
        serial->GetInt(ChannelNr);
        /*if (NetPrincetonFrameGrabber) {
            double d;
        		NetPrincetonFrameGrabber->GetAnalogIn(ChannelNr,d);
            char buf[20];
            sprintf(buf,"*%.3f#",d);
  	         serial->SendCommand(buf);
        } else */serial->SendCommand("*0#");
		} else if (strcmp(Command,"VisionGoodbye")==0) {
		  //	serial->CloseConnection();
        //delete serial;
        //serial=new CSerialNet(this,ControlComputerPort);
		} else if (strcmp(Command,"VisionShowPicture")==0) {
		  serial->SendCommand("*Ready#");
        int pic;
        serial->GetInt(pic);
		  ShowPicture(pic);
		} else if (strcmp(Command,"VisionSavePicture")==0) {
		  serial->SendCommand("*Ready#");
        char name[200];
        serial->GetCommand(name);
        int aswhat;
        serial->GetInt(aswhat);
		  SavePictureAs(name,aswhat);
		} else if (strcmp(Command,"VisionMessage")==0) {
		  serial->SendCommand("*Ready#");
        char Message[300];
		  serial->GetString(Message);
        struct  time t;
        gettime(&t);
        char buf[300];
        sprintf(buf,"%2d:%02d:%02d | %s",
          t.ti_hour, t.ti_min, t.ti_sec,Message);
        char name[200];
		  Name->GetMessageName(name);
		  ofstream out(name);
        out<<buf<<endl;
        mb->SetText(buf);
		} else if (strcmp(Command,"VisionRegisterParameters")==0) {
		  serial->SendCommand("*Ready#");
		  char name[200];
		  Name->GetParamName(name);
		  ofstream out(name);
		  while (strcmp(name,"VisionRegisterParametersEnd")!=0) {
          out<<name<<endl;
			 serial->GetString(name);
		  }
        serial->SendCommand("*Ready#");
        if (ParamList) SaveParameterList(ParamList,out,true,false,false,NULL,true,false,false); //ParamList->Save(out,true,false,false,NULL,true,false,false);
		} else if (strcmp(Command,"VisionSynchronizeParameters")==0) SynchroniseParameters();
	 } else Sleep(10);
	 checkingSerial=0;
  }
}

void TVisionDialog::SynchroniseParameters() {
  serial->SendCommand("*Ready#");
  if (ParamsInitialised) serial->SendCommand("*SendChanged#");
  else serial->SendCommand("*SendAll#");
  ParamsInitialised=true;
  char name[200];
  char Type;
  char Value[200];
  serial->GetString(name);
  int NoNameNr=0;
  while (strcmp(name,"VisionSynchronizeParametersEnd")!=0) {

/*
//debug
  ofstream out("Visionde.dat", ios::app);
  out<<name<<endl;
   out.close();
//debug end
  */
    Type=name[0];
    strcpy(name,name+1);
    if (strchr(name,'=')!=NULL) {
      strcpy(Value,strchr(name,'=')+1);
      *strchr(name,'=')=0;
    } else {
      sprintf(name,"NoName%i",NoNameNr++);
      strcpy(Value,"Nix");
    }
    if (ParamList) {
      CParamList *help;
      help=ParamList->GetParam(name);
      if (help) help->SetValue(Value);
      else {
        ParamList=new CParamList(ParamList,name,Value,Type);
        #if defined(DetectLeaks)
          LeakDetect.New(ParamList,5,102);
        #endif
      }
    } else {
      ParamList=new CParamList(NULL,name,Value,Type);
      #if defined(DetectLeaks)
        LeakDetect.New(ParamList,5,103);
      #endif
    }

    serial->GetString(name);
  }
  /*
  //debug
  ofstream out("Visionde.dat", ios::app);
  out<<" finished" <<endl;
   out.close();
//debug end */
  serial->SendCommand("*Ready#");
}

void TVisionDialog::ReceiveDataFile() {
	serial->SendCommand("*Ready#");
   char Filename[1000];
   serial->GetString(Filename);
   char LocalName[1000];
   char ThisName[1000];
   Name->GetName(ThisName,true,0);
   sprintf(LocalName,ThisName,'_',0);
   LocalName[strlen(LocalName)-7]=0;
   strcat(LocalName,Filename);
   strcat(LocalName,".txt");
 	ofstream out(LocalName);
   char buf[10000];
   serial->GetString(buf);
   while (strcmp(buf,"VisionSendDataFileEnd")!=0) {
      out<<buf<<endl;
	    serial->GetString(buf);
  	}
   out.close();
}

/*void TVisionDialog::SetProbeDetuning(double aProbeDetuning) {
  ProbeDetuning=aProbeDetuning;
  MessDataTextBox->SetValue(1,ProbeDetuning);
} */

bool TVisionDialog::IdleAction(long idleCount) {
  Sleep(1);
  TWindow::IdleAction(idleCount);
  if (!RDSetup->HardwareAccess) return FALSE;
  CheckSerial();
  if (Update2DFitDrawing && (idleCount>0)) {
    if (picture==opticalDensity) Display2DProfils();
    TNmeasure->Draw();
    Display2DFitResults();
    DisplayIoffeFit(false);
    Update2DFitDrawing=false;
  }
/*if (AndorFrameGrabber) {
     int solltemp,isttemp;
     if (!AndorFrameGrabber->TemperatureStabilized(solltemp,isttemp)) {
       char buf[200];
       sprintf(buf,"Andor Temperature not stabilized : Actual Temperature %i �C   Goal Temperature %i �C",isttemp,solltemp);
       mb->SetText(buf);
       TempTextSet=true;
     } else if (TempTextSet) {
       TempTextSet=false;
       char buf[200];
       sprintf(buf,"Andor Temperature stabilized at %i �C",solltemp);
       mb->SetText(buf);
     }
  }*/

  #if defined(AltaCameraUsed)
  if (AltaFrameGrabber) {
     int solltemp,isttemp;
     if (!AltaFrameGrabber->TemperatureStabilized(solltemp,isttemp)) {
       char buf[200];
       sprintf(buf,"Alta Temperature not stabilized : Actual Temperature %i �C   Goal Temperature %i �C",isttemp,solltemp);
       mb->SetText(buf);
       TempTextSet=true;
     } else if (TempTextSet) {
       TempTextSet=false;
       char buf[200];
       sprintf(buf,"Alta Temperature stabilized at %i �C",solltemp);
       mb->SetText(buf);
     }
  }
  #endif

  #if defined(ApogeeCameraUsed)
  if (ApogeeFrameGrabber) {
     int solltemp,isttemp;
     if (!ApogeeFrameGrabber->TemperatureStabilized(solltemp,isttemp)) {
       char buf[200];
       sprintf(buf,"Apogee Temperature not stabilized : Actual Temperature %i �C   Goal Temperature %i �C",isttemp,solltemp);
       mb->SetText(buf);
       TempTextSet=true;
     } else if (TempTextSet) {
       TempTextSet=false;
       char buf[200];
       sprintf(buf,"Apogee Temperature stabilized at %i �C",solltemp);
       mb->SetText(buf);
     }
  }
  #endif
  return TRUE;
}

void TVisionDialog::MeasureTNButtonClicked() {
  TNmeasure->Draw();
}

void TVisionDialog::CameraButtonClicked() {
  TNmeasure->Draw();
}

void TVisionDialog::DetailButtonClicked() {
   MovePicturesToDetailPictureButtonsAndRedraw();
}

void TVisionDialog::LineUpButtonClicked() {
  MD->y("LineShift")--;
  LoadMeasurement(MD->GetText("Filename"),false,true,MD->y("PictureListPosition"),true);
}

void TVisionDialog::LineDownButtonClicked() {
  MD->y("LineShift")++;
  LoadMeasurement(MD->GetText("Filename"),false,true,MD->y("PictureListPosition"),true);
}

void TVisionDialog::OffsetUpButtonClicked() {
  MD->y("OffsetShift")/=1.0+OffsetButtonScale;
  LoadMeasurement(MD->GetText("Filename"),false,true,MD->y("PictureListPosition"),true);
  DoAutomaticFit();
}

void TVisionDialog::OffsetDownButtonClicked() {
  MD->y("OffsetShift")*=1.0+OffsetButtonScale;
  LoadMeasurement(MD->GetText("Filename"),false,true,MD->y("PictureListPosition"),true);
  DoAutomaticFit();
}

void TVisionDialog::CalcRMSNoise() {
  double xmin,xmax,ymin,ymax;
  profilV->GetPos(ymin,ymax);
  profilH->GetPos(xmin,xmax);
  double noise=picture->CalcRMSNoise(xmin,ymin,xmax,ymax);
  double min,max;
  picture->GetMinMax(xmin,ymin,xmax,ymax,min,max);
  char buf[200];
  sprintf(buf,"RMS noise=%.2f , Minimum=%.2f , Maximum=%.2f , Delta=%.2f",noise,min,max,max-min);
  mb->SetText(buf);
}

void TVisionDialog::ReCalc() {
  TNmeasure->ReCalc();
  mb->SetText("Re Calc done");
}

void TVisionDialog::ShiftTNmeasureParameters() {
  TNmeasure->ShiftParameters();
  mb->SetText("TN measure parameters shifted");
}

void TVisionDialog::SetTrapParameters() {
  MessDataTextBox->SetValue(4,MD->y("TrapType"));
  MessDataTextBox->SetValue(5,MD->y("GxOrOffset"));
  MessDataTextBox->SetValue(6,MD->y("GyOrGradient"));
  MessDataTextBox->SetValue(7,MD->y("GzOrCurvature"));
}

void TVisionDialog::TNBackButtonClicked() {
  MeasureTNButton->Last();
}

void TVisionDialog::TNForwardButtonClicked() {
  MeasureTNButton->Next();
}

void TVisionDialog::TNMessBackButtonClicked() {
  TNmeasure->ShowLast();
}

void TVisionDialog::TNMessForwardButtonClicked() {
  TNmeasure->ShowNext();
}

void TVisionDialog::DisplayMessData() {
  SetTrapParameters();
  MessDataTextBox->SetValue(0,MD->y("Camera"));
  MessDataTextBox->SetValue(1,MD->y("ProbeDetuning"));
  MessDataTextBox->SetValue(2,MD->y("ExpansionTime"));
  MessDataTextBox->SetValue(3,MD->y("PictureListPosition"));
  char buf[1000];
  strcpy(buf,MD->GetText("Path"));
  strcat(buf," ");
  strcat(buf,MD->GetText("Endstate"));
  FinalTrapText->SetText(buf);
  for (int i=0;i<MD->GetNrX() && i<4;i++) MessDataTextBox->SetValue(i+8,MD->GetXNr(i));
  DisplayGaussFitResults();
  CalculateTOFTemperature();
  CalculateT_TF();
  Calculaten0rho0();
  Display2DFitResults();
}

void TVisionDialog::CalculateTrapFrequencies(double &frad,double &fax) {
  /*double masse=MD->y("Mass");
  double mfgfmyb=MD->y("MagMoment")*_myb;
  double Cax=MD->y("GzOrCurvature");
  double Grad=MD->y("GyOrGradient");
  double Offset=MD->y("GxOrOffset");
  if ((masse>0) && (Offset>0) && (Cax>0)) {
    if (mfgfmyb*Cax/masse<0) fax=99.999;
    else fax=sqrt(mfgfmyb*Cax/masse)/(2*M_PI);
    double Crad=Grad*Grad/Offset-Cax/2;
    if (mfgfmyb*Crad/masse<0) frad=99.999;
    else frad=sqrt(mfgfmyb*Crad/masse)/(2*M_PI);    
  }*/
  fax=14;
  frad=800;
  TNFitTextBox->SetValue(9,frad);
  TNFitTextBox->SetValue(10,fax);
}

void TVisionDialog::CalculateT_TF() {
  double frad,fax;
  CalculateTrapFrequencies(frad,fax);
  double N=MD->y("NIntegral");
  if ((frad>1) && (N>0)) {
     double lambda=fax/frad;
     double fac=(MD->y("PictureListPosition")==0) ? 6 : 1.0/1.202;
     double ef = _h*frad*pow(fac*N*lambda,1./3);   //Fermi energy
     double tf=ef/_kB*1E6;
     MD->y("TFermi_TCritic")=tf;
     double T=MD->y("TBoson");
     if (tf>0) MD->y("T_TF")=T/tf;
//     GaussFitTextBox->SetValue(14,MD->y("TFermi_TCritic"));
     if (MD->y("PictureListPosition")==0) {
       double masse=MD->y("Mass")*_mp;
       double H0GndState=sqrt(_h/(2*M_PI*masse*frad*2*M_PI));
       double Rf=MD->y("RF")=pow(48*N*lambda,1.0/6.0)*H0GndState;
       double rm=MD->y("XRMS")/((Rf/lambda)*1000);      //unites: mm , axial rf
       MD->y("RM2_RF2")=rm*rm;
     }
  }
}

void TVisionDialog::Calculaten0rho0() {
  if (!MD) return;
  double masse=MD->y("Mass")*_mp;
  bool MagTrap=false;
  if (MagTrap) {
	double mfgfmyb=MD->y("MagMoment")*_myb;
	double Cax=MD->y("GzOrCurvature");
	double Grad=MD->y("GyOrGradient");
	if (MD->y("GxOrOffset")<0.00000001) MD->y("GxOrOffset")=0.00000001;
	double Offset=MD->y("GxOrOffset");
	double Crad=Grad*Grad/Offset-Cax/2;
	double N=MD->y("NIntegral");
	double T;
	if (FitFermiHorizontal) T=MD->y("Taxial")*1E-6;
	else T=MD->y("TempTOF")*1E-6;
	if ((masse>0) && (Offset>0) && (Cax>0) && (N>0)) {
		double lamndadb;
		if (2*M_PI*masse*_kB*T>0) lamndadb=_h/sqrt(2*M_PI*masse*_kB*T);
		else lamndadb=1E-6;
		double srad;
		if (_kB*T/(mfgfmyb*Crad)>0) srad=sqrt(_kB*T/(mfgfmyb*Crad));
		else srad=1E-6;
		double sax=sqrt(_kB*T/(mfgfmyb*Cax));
		double dens=N/(pow(2*M_PI,1.5)*srad*srad*sax);
		double psd=dens*lamndadb*lamndadb*lamndadb;
		double speed=sqrt(8*_kB*T/(M_PI*masse));
		double Gamma=0.5*dens*SigmaEl*speed;
		TNFitTextBox->SetValue(0,sax*1E3);
		TNFitTextBox->SetValue(1,speed);
		TNFitTextBox->SetValue(3,srad*1E3);
		TNFitTextBox->SetValue(4,Gamma);
		TNFitTextBox->SetValue(7,dens*1E-6);
		TNFitTextBox->SetValue(8,psd);
		double frad,fax;
		CalculateTrapFrequencies(frad,fax);
		TNFitTextBox->SetValue(11,T);
		MD->y("Gamma")=Gamma;
		MD->y("n0")=dens;
		MD->y("rho0")=psd;
	}
  } else {
	  //optical dipole trap
	  if ((SigmaXReference>0) && (SigmaYReference>0)) {
		double N=MD->y("NIntegral");
		double T=MD->y("TempTOFaxial")*1E-6; //�K to K
		double srad=SigmaXReference*0.001; //mm to m
		double sax=SigmaYReference*0.001;  //mm to m
		double dens=N/(pow(2*M_PI,1.5)*srad*srad*sax);
      double lamndadb=0;
		if (2*M_PI*masse*_kB*T>0) lamndadb=_h/sqrt(2*M_PI*masse*_kB*T);
		else lamndadb=1E-6;
		double psd=dens*lamndadb*lamndadb*lamndadb;
		double speed=sqrt(8*_kB*T/(M_PI*masse));
		double Gamma=0.5*dens*SigmaEl*speed;
		TNFitTextBox->SetValue(0,sax*1E3);
		TNFitTextBox->SetValue(1,speed);
		TNFitTextBox->SetValue(3,srad*1E3);
		TNFitTextBox->SetValue(4,Gamma);
		TNFitTextBox->SetValue(7,dens*1E-6);
		TNFitTextBox->SetValue(8,psd);
		TNFitTextBox->SetValue(11,T);
		MD->y("Gamma")=Gamma;
		MD->y("n0")=dens;
		MD->y("rho0")=psd;
	  }
  }
}

void TVisionDialog::DisplayGaussFitResults() {
  GaussFitTextBox->SetValue(0,MD->y("OpticalDensity"));
  GaussFitTextBox->SetValue(1,MD->y("X0"));
  GaussFitTextBox->SetValue(2,MD->y("SigmaX")*1000.0); //conversion mm to �m
  GaussFitTextBox->SetValue(3,MD->y("GOffsetX"));
  GaussFitTextBox->SetValue(5,MD->y("NIntegral"));
  GaussFitTextBox->SetValue(6,MD->y("Y0"));
  GaussFitTextBox->SetValue(7,MD->y("SigmaY")*1000.0);  //conversion mm to �m
  GaussFitTextBox->SetValue(8,MD->y("GOffsetY"));
  GaussFitTextBox->SetValue(10,MD->y("NGauss"));
  GaussFitTextBox->SetValue(11,MD->y("n0Gauss"));
  GaussFitTextBox->SetValue(12,MD->y("MOTFluorescence"));
  GaussFitTextBox->SetValue(13,MD->y("NIntOffset")*1E4);
//  GaussFitTextBox->SetValue(12,MD->y("RecapFluo"));
//  GaussFitTextBox->SetValue(13,MD->y("FermiL"));
//  GaussFitTextBox->SetValue(15,MD->y("XRMS"));
}

void TVisionDialog::CalculateTOFTemperature() {
  double Masse=MD->y("Mass")*_mp;
  if (MD->y("ExpansionTime")<0.0000001) MD->y("ExpansionTime")=0.000001;
  double TxTOF=Masse/_kB*pow(MD->y("SigmaX")/MD->y("ExpansionTime"),2)*1E6;
  if (TxTOF>10000) TxTOF=9999.99;

  double mfgf=MD->y("MagMoment");
  double Tx=pow(MD->y("SigmaX")*1E-3,2)*MD->y("MagMoment")*_myb*MD->y("GzOrCurvature")/_kB*1E6;
  if (Tx>10000) Tx=9999.99;

  double Ty=Masse/_kB*pow(MD->y("SigmaY")/MD->y("ExpansionTime"),2)*1E6;
  if (Ty>10000) Ty=9999.99;
  MD->y("TempTOF")=Ty;
  MD->y("TempTOFaxial")=TxTOF;
  MD->y("Tradial")=Ty;
  MD->y("Taxial")=Tx;
  TNFitTextBox->SetValue(2,TxTOF);
  TNFitTextBox->SetValue(5,Ty);

  MD->y("Ecut")=((MD->y("Li7fend")-MD->y("Li7f0"))*1E6*_h/(1.5*_kB))*1E6;
  double T;
  if (FitFermiHorizontal) T=MD->y("Taxial")*1E-6;
  else T=MD->y("TempTOF")*1E-6;
  if (T<0.0000001) T=0.0000001;
  MD->y("eta")=MD->y("Ecut")/T;
  TNFitTextBox->SetValue(6,MD->y("eta"));
  if (MD->y("PictureListPosition")!=0) TempBoson=MD->y("TBoson")=(FitFermiHorizontal) ? MD->y("Taxial") : MD->y("TempTOF");
  else MD->y("TBoson")=TempBoson;
}

void TVisionDialog::Display2DFitResults() {
  Fit2DTextBox->SetValue(0,MD->y("X02D"));
  Fit2DTextBox->SetValue(1,MD->y("Y02D"));
  Fit2DTextBox->SetValue(2,MD->y("Tradial"));
  Fit2DTextBox->SetValue(3,MD->y("N2D"));
  Fit2DTextBox->SetValue(4,MD->y("Offset2D"));
  Fit2DTextBox->SetValue(5,MD->y("SlopeX2D"));
  Fit2DTextBox->SetValue(6,MD->y("SlopeY2D"));
  Fit2DTextBox->SetValue(7,MD->y("Angle2D"));
  Fit2DTextBox->SetValue(8,MD->y("zCorr2D"));
  Fit2DTextBox->SetValue(9,MD->y("n0"));
  Fit2DTextBox->SetValue(10,MD->y("rho0"));
  Fit2DTextBox->SetValue(11,MD->y("Gamma"));
  Fit2DTextBox->SetValue(12,MD->y("Taxial"));
}

void TVisionDialog::MDSetCameraParameters(){
//this is outdated
/*
  MD->y("BiningX")=BiningX;
  MD->y("BiningY")=BiningY;
  MD->y("FKBining")=FKBining;
  MD->y("FastKinetics")=FastKinetics;
  MD->y("FKSDistance")=FKSDistance;
  MD->y("FKSHeight")=FKSHeight;
  MD->y("FKSOffset")=FKSOffset;
  */
}
