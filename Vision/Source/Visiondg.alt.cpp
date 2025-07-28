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

const UINT ID_FILMTIMER = 1;
const UINT ID_TESTSERIALTIMER = 2;
const filmtime=3000;
const testserialtime=500;
const MatroxWidth=192;
const MatroxHeight=192;
const AndorWidth=512;
const AndorHeight=512;
const double MaxOptDensity6Li=1.5;
const double MaxOptDensity7Li=3;
const unsigned char AbsorptionGain=255;
const unsigned char AbsorptionOffset=90;
const unsigned char FluorescenceGain=255;
const unsigned char FluorescenceOffset=120;
const double ClebschLi7=7.0/15.0;
const double ClebschLi6=1.0/2.0;
const double GammaLi7=122E6;
const double GammaLi6=94E6;
const double EichHeight=1/0.9;
const double EichOffset=0.05;

//*** TLaserDialog ***
DEFINE_RESPONSE_TABLE1(TVisionDialog, TWindow)
  EV_WM_ERASEBKGND,
  EV_WM_CTLCOLOR,
  EV_WM_SYSCOLORCHANGE,
  EV_BN_CLICKED(ID_MEASURETNBUTTON,MeasureTNButtonClicked),
  EV_BN_CLICKED(ID_BIGMEASUREBUTTON,BigMeasureButtonClicked),
  EV_COMMAND(CM_ABOUT, About),
  EV_COMMAND(CM_FLUORESCENCEPICTURE, FluorescencePicture),
  EV_COMMAND(CM_FILMFLUORESCENCE, FilmFluorescence),
  EV_COMMAND(CM_GRIDONOFF, GridOnOff),
  EV_COMMAND(CM_MANDELBROT, MandelBrot),
  EV_COMMAND(CM_GAUSS, Gauss),
  EV_COMMAND(CM_FIT1DGAUSS, Fit1DGauss),
  EV_COMMAND(CM_RECALC, ReCalc),
  EV_COMMAND(CM_FIT1DGAUSSTOSUM, Fit1DGaussToSum),
  EV_COMMAND(CM_FITTEMPERATURE, FitTemperature),
  EV_COMMAND(CM_FITATOMNUMBER, FitAtomNumber),
  EV_COMMAND(CM_INTEGRALATOMNUMBER, IntegralAtomNumber),
  EV_COMMAND(CM_FITSIMPLEATOMNUMBER, FitSimpleAtomNumber),
  EV_COMMAND(CM_FITCHARGINGTIME, FitChargingTime),
  EV_COMMAND(CM_FITLIFETIME, FitLifeTime),
  EV_COMMAND(CM_SAVEPICTURE, SavePicture),
  EV_COMMAND(CM_SAVEPICTUREASBMP, SavePictureAsBMP),
  EV_COMMAND(CM_SAVEPICTUREASRGBTGA, SavePictureAsRGBTGA),
  EV_COMMAND(CM_SAVEPICTUREASBWTGA, SavePictureAsBWTGA),
  EV_COMMAND(CM_LOADBMPPICTURE, LoadBMPPicture),
  EV_COMMAND(CM_LOADTGAPICTURE, LoadTGAPicture),
  EV_COMMAND(CM_LOADMEASUREMENT, LoadMeasurement),
  EV_COMMAND(CM_DELETEMEASUREMENT, DeleteMeasurement),
  EV_COMMAND(CM_LOADSERIE, LoadSerie),
  EV_COMMAND(CM_NEWSERIE, NewSerie),
  EV_COMMAND(CM_ADDSERIE, AddSerie),
  EV_COMMAND(CM_SAVESERIE, SaveSerie),
  EV_COMMAND(CM_STARTSERIE, StartSerie),
  EV_COMMAND(CM_STOPSERIE, StopSerie),
  EV_COMMAND(CM_SORTSERIE, SortSerie),
  EV_COMMAND(CM_LOADBIGSERIE, LoadBigSerie),
  EV_COMMAND(CM_NEWBIGSERIE, NewBigSerie),
  EV_COMMAND(CM_ADDBIGSERIE, AddBigSerie),
  EV_COMMAND(CM_SAVEBIGSERIE, SaveBigSerie),
  EV_COMMAND(CM_STARTBIGSERIE, StartBigSerie),
  EV_COMMAND(CM_STOPBIGSERIE, StopBigSerie),
  EV_COMMAND(CM_SORTBIGSERIE, SortBigSerie),
  EV_COMMAND(CM_LOADPALETTE, LoadPalette),
  EV_COMMAND(CM_ABSORPTIONPICTURE, AbsorptionPicture),
  EV_COMMAND(CM_SHOWFLUORESCENCE, ShowFluorescence),
  EV_COMMAND(CM_SHOWABSORPTION, ShowAbsorption),
  EV_COMMAND(CM_SHOWOPTICALDENSITY, ShowOpticalDensity),
  EV_COMMAND(CM_SHOWABSDIRECT, ShowAbsDirect),
  EV_COMMAND(CM_SHOWPROBE, ShowProbe),
  EV_COMMAND(CM_SHOWNOISE, ShowNoise),
  EV_COMMAND(CM_PROFILDESIGN, ToggleProfilDesign),
  EV_MESSAGE(IDM_PICTURECHANGED,PictureChanged),
  EV_MESSAGE(IDM_LOADPALETTE,PaletteChanged),
  EV_MESSAGE(IDM_AUTOFIT,Fit1DGauss),
  EV_MESSAGE(IDM_LOADMEASUREMENT,LoadMess),
  EV_MESSAGE(IDM_PROFILCURSORMOVED,ProfilCursorMoved),
  EV_MESSAGE(IDM_CURSORMOVED,CursorMoved),
  EV_COMMAND(CM_PALETTESTANDARD, SetStandardPalette),
  EV_COMMAND(CM_PALETTESTRIPES, AddStripes),
  EV_COMMAND(CM_FIXCURSOR, FixCursor),
  EV_COMMAND(CM_UNFIXCURSOR, UnFixCursor),
  EV_WM_TIMER,
END_RESPONSE_TABLE;


HBRUSH TVisionDialog::EvCtlColor(HDC hDC, HWND , UINT )
{
  TColor h=::GetSysColor(COLOR_BTNFACE);
  ::SetBkColor(hDC, ::GetSysColor(COLOR_BTNFACE));
  return *BkBrush;
}

void TVisionDialog::EvSysColorChange()
{
  delete BkBrush;
  BkBrush = new TBrush(::GetSysColor(COLOR_BTNFACE));
}

bool TVisionDialog::EvEraseBkgnd(HDC hDC)
{
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
  return TRUE;
}

char *CursorText[7]={{"X=%.2f mm"},{"Y=%.2f mm"},{"H=%.2f"},{"XP=%.2f"},{"DX=%.2f"},{"YP=%.2f"},{"DY=%.2f"}};
char *MessDataText[4]={{"Camera=%.0f"},{"Det=%.2f MHz"},{"TOF=%.2f ms"},{"Isotop=%.0f"}};
char *GaussFitText[10]={{"Dens=%.2f"},{"X0=%.2f"},{"SX=%.2f"},{"OX=%.2f"},
							  {"N=%.2E "},{"Y0=%.2f"},{"SY=%.2f"},{"OY=%.2f"},
							  {"NG=%.2E"},{"n0=%.2e cm-3"}};
char *TNFitText[12]={{"Sigma0X=%.2f mm"},{"SpeedX=%.2f m/s"},{"TempX=%.0f microK"},
  {"Sigma0Y=%.2f mm"},{"SpeedY=%.2f m/s"},{"TempY=%.0f microK"},{"N=%.2E"},{"n0=%.2E"},{"r0=%.2E"},
  {"SX=%.2f"},{"SY=%.2f"},{"NT=%.2E"}};
char *MeasureTNButtonText[7]={{"SX"},{"SY"},{"Ab"},{"OD"},{"X0"},{"Y0"},{"N"}};
char *Li6Li7ButtonText[3]={{"6 Li"},{"7 Li"},{"6Li/(6Li+7Li)"}};
char *BigMeasureButtonText[7]={{"TX"},{"TY"},{"N"},{"SX"},{"SY"},{"n0"},{"r0"}};

TVisionDialog::TVisionDialog(TMessageBar *amb)
				 :TWindow(0,0,0),
				 BMPFileData(OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT |
								  OFN_NOCHANGEDIR | OFN_EXTENSIONDIFFERENT,
								  "Picture (*.BMP) | *.BMP",0,"c:\\Rubidium\\Data"),
             TGAFileData(OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT |
								  OFN_NOCHANGEDIR | OFN_EXTENSIONDIFFERENT,
								  "Picture (*.TGA) | *.TGA",0,"c:\\Rubidium\\Data"),
				 SerieFileData(OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT |
								  OFN_NOCHANGEDIR | OFN_EXTENSIONDIFFERENT,
								  "Serie (*.DAT) | *.DAT",0,"c:\\Rubidium\\Data"),
				 BigSerieFileData(OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT |
								  OFN_NOCHANGEDIR | OFN_EXTENSIONDIFFERENT,
								  "Big Serie (*.DAT) | *.DAT",0,"c:\\Rubidium\\Data"),
				 MESFileData(OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT |
								  OFN_NOCHANGEDIR | OFN_EXTENSIONDIFFERENT,
								  "Measurement | ??????A?.*",0,"c:\\Rubidium\\Data")

				 {
  Name =new CName();
  /*Name->GetPath(BMPFileData.InitialDir);
  Name->GetPath(SerieFileData.InitialDir);
  Name->GetPath(BigSerieFileData.InitialDir);
  Name->GetPath(MESFileData.InitialDir);*/
  SetClebsch(ClebschLi7);
  SetGamma(GammaLi7);
  MaxOptDensity=MaxOptDensity7Li;
  TimeOfFlight=0;
  Calibration=1;
  BigTempPictureNumb=0;
  EichH=0.01;
  EichV=0.01;
  CamStartX=0;
  CamStartY=0;
  CamWidth=AndorWidth;
  CamHeight=AndorHeight;
  PicEichX=1;
  PicEichY=1;
  BiningX=1;
  BiningY=1;
  aktcamera='M';
  PictureTyp='E';
  CheckNumber=0;
  Isotop=7;
  BigSigmaX=0;
  BigSigmaY=0;
  BigNumber=0;
  strcpy(BigCode,"----");
  ProbeDetuning=0;
  DetectLithium7=1;
  DetectLithium6=0;
  TrapLithium6=1;
  TrapLithium6=0;
  SlowLithium6=1;
  SlowLithium6=0;
  NGauss=0;
  X0=0;
  SigmaX=0;
  OffsetX=0;
  Abs=0;
  Y0=0;
  SigmaY=0;
  OffsetY=0;
  CursorFixed=0;
  FixedX0=0;
  FixedY0=0;
  serial=new CSerial();
  checkingSerial=0;
  autofit=1;
  const R=10;
  const PH=128;
  const D=5;
  const D2=10;
  const WH=190;
  const WV=190;
  const BB=8;
  const Slash=6;
  const Text=10;
  const SB=25;
  const SH=25;

  mb=amb;
  filming=0;
  ColorTable=new CColorTable(this,R+PH+D+SB+D2+WH+D2,R,BB,WV);
  MatroxFrameGrabber=new CMatroxFrameGrabber();
  AndorFrameGrabber=new CAndorFrameGrabber("c:\\Rubidium\\Vision");
  fluopicture=new CPicture(1,0);
  abspicture=new CPicture(EichHeight,EichOffset);
  pictureprobe=new CPicture(1,0);
  picturenoise=new CPicture(1,0);
  pictureabs=new CPicture(1,0);
  opticalDensity=new CPicture(EichHeight,EichOffset);
  picture=fluopicture;
  new CScale(this,R+PH+D,R,SB,WV,Slash,Text,0,1,-0.5,2,1,-EichV,-WV*EichV/2.0,1.5);
  new CScale(this,R+PH+D+SB+D2,R+WV+D,WH,SH,Slash,Text,1,1,0.5,2,-1,EichH,-WH*EichH/2.0,-1.5);
  new CScale(this,R,R+WV+D,PH,SH,Slash,Text,1,1,-50,2,-1,-2,255,250);
  new CScale(this,R+PH+D,R+WV+D+SH+D2,SB,PH,Slash,Text,0,0,-50,2,-1,-2,255,250);
  new CScale(this,R+PH+D+SB+D2+WH+D2+BB+D,R,SB,WV,Slash,Text,0,1,-50,2,-1,-256.0/WV,255,250);
  profilH=new CProfil(this,R+PH+D+SB+D2,R+WV+D+SH+D2,WH,PH,1,WH,WV,&picture,ColorTable);
  profilV=new CProfil(this,R,R,PH,WV,0,WH,WV,&picture,ColorTable);
  PictureWin=new CPictureProfilWindow(this,mb,R+PH+D+SB+D2,R,WH,WV,&picture,profilH,profilV,ColorTable);
  BkBrush =new TBrush(::GetSysColor(COLOR_BTNFACE));
  const BoxWide=60;
  const X0=R+PH+D+SB+D2+WH+D2+BoxWide+D2;
  const TH=16;
  const D3=5;
  const PB=(WV-2*(TH+D3))/2;
  new TStatic(this,ID_FLUO,"Fluo",X0+0*(PB+D2),R+0*(PB+D3+TH),PB,TH);
  FluoButton=new CPictureButton(this,mb,X0+0*(PB+D2),R+TH+0*(PB+D3+TH),PB,PB,&fluopicture,ColorTable,ID_FLUO);
  new TStatic(this,ID_ABS,"Absorp",X0+1*(PB+D2),R+0*(PB+D3+TH),PB,TH);
  AbsButton=new CPictureButton(this,mb,X0+1*(PB+D2),R+TH+0*(PB+D3+TH),PB,PB,&abspicture,ColorTable,ID_ABS);
  new TStatic(this,ID_OPTDENS,"OptDens",X0+2*(PB+D2),R+0*(PB+D3+TH),PB,TH);
  OptDensButton=new CPictureButton(this,mb,X0+2*(PB+D2),R+TH+0*(PB+D3+TH),PB,PB,&opticalDensity,ColorTable,ID_OPTDENS);
  new TStatic(this,ID_ABSDIRECT,"Raw Abs",X0+0*(PB+D2),R+1*(PB+D3+TH),PB,TH);
  RawAbsButton=new CPictureButton(this,mb,X0+0*(PB+D2),R+TH+1*(PB+D3+TH),PB,PB,&pictureabs,ColorTable,ID_ABSDIRECT);
  new TStatic(this,ID_PROBE,"Probe",X0+1*(PB+D2),R+1*(PB+D3+TH),PB,TH);
  ProbeButton=new CPictureButton(this,mb,X0+1*(PB+D2),R+TH+1*(PB+D3+TH),PB,PB,&pictureprobe,ColorTable,ID_PROBE);
  new TStatic(this,ID_NOISE,"Noise",X0+2*(PB+D2),R+1*(PB+D3+TH),PB,TH);
  NoiseButton=new CPictureButton(this,mb,X0+2*(PB+D2),R+TH+1*(PB+D3+TH),PB,PB,&picturenoise,ColorTable,ID_NOISE);
  const TBW=PH+D+SB+D2+WH;
  const Y1=R+WV+D+SH+D2;
  const Y2=R+WV+D+SH+D2+PH+D2;
  const MTextBoxH=35;
  const GTextBoxH=65;
  const TTextBoxH=80;
  CursorTextBox=new CTextBox(this,R,R+WV+D+SH+D2,PH,PH,80,"Cursor Position",1,7,7,CursorText);
  MessDataTextBox=new CTextBox(this,R,Y2,TBW,MTextBoxH,90,"Measurement Parameters",4,1,4,MessDataText);
  GaussFitTextBox=new CTextBox(this,R,Y2+MTextBoxH+D2,TBW,GTextBoxH,90,"Gauss Fit Result",4,3,10,GaussFitText);
  TNFitTextBox=new CTextBox(this,R,Y2+GTextBoxH+D2+MTextBoxH+D2,TBW,TTextBoxH,120,"Temperature and Atom Number Fit Result",3,4,12,TNFitText);
  const X1=R+PH+D+SB+D2+WH+D2;
  MeasureTNButton =new CButtonBox(this,ID_MEASURETNBUTTON,X1,Y1,BoxWide,PH,7,MeasureTNButtonText);
  Li6Li7Button =new CButtonBox(this,ID_LI6LI7BUTTON,X0,R+WV+D,PB+2*(PB+D2),SH,3,Li6Li7ButtonText);
  TStatic* AxText=new TStatic(this,0,"",X0,Y1+PH-15,PB+2*(PB+D2),15);
  TNmeasure=new CTNMeasure(this,Name,mb,MeasureTNButton,AxText,TNFitTextBox,X0,Y1,PB+2*(PB+D2),PH-15);
  BigMeasureButton =new CButtonBox(this,ID_BIGMEASUREBUTTON,X1,Y1+D2+PH,BoxWide,PH,7,BigMeasureButtonText);
  AxText=new TStatic(this,0,"",X0,Y1+D2+PH+PH-15,PB+2*(PB+D2),15);
  Bigmeasure=new CBigMeasure(this,TNmeasure,Name,mb,BigMeasureButton,AxText,TNFitTextBox,X0,Y1+D2+PH,PB+2*(PB+D2),PH-15);
  new TStatic(this,0,"7Li",X0+3*(PB+D2)+10,R,PB+D2,15);
  new TStatic(this,0,"6Li",X0+4*(PB+D2)+20,R,PB+D2,15);
  PictureList7Li=new CPictureList(this,mb,ID_PICLIST,X0+3*(PB+D2),R+TH,PB+D2,510,abspicture,ColorTable);
  PictureList6Li=new CPictureList(this,mb,ID_PICLIST,X0+4*(PB+D2)+10,R+TH,PB+D2,510,abspicture,ColorTable);
  TwoPictures=0;
  mb->SetText("System ready");
  SetTimer(ID_TESTSERIALTIMER,testserialtime);
  InSerie=0;
  CheckDiskSpace(10000000);
}

bool TVisionDialog::CanClose() {
  if (AndorFrameGrabber) delete AndorFrameGrabber;
  AndorFrameGrabber=NULL;
  return true;
}

TVisionDialog::~TVisionDialog() {
  KillTimer(ID_TESTSERIALTIMER);
  if (serial) delete serial;
  if (Name) delete Name;
  if (fluopicture) delete	fluopicture;
  if (abspicture) delete abspicture;
  if (pictureprobe) delete pictureprobe;
  if (picturenoise) delete picturenoise;
  if (pictureabs) delete pictureabs;
  if (opticalDensity) delete opticalDensity;
  if (MatroxFrameGrabber) delete MatroxFrameGrabber;
  if (AndorFrameGrabber) delete AndorFrameGrabber;
}

void TVisionDialog::SetupWindow() {
  TWindow::SetupWindow();
  if (MeasureTNButton) MeasureTNButton->SetCheck(1,BF_CHECKED);
  if (BigMeasureButton) BigMeasureButton->SetCheck(1,BF_CHECKED);
}

void TVisionDialog::About() {
  char buf[400];
  sprintf(buf,"\n       Vision 2.0\n\n    Florian  Schreck\n\n          LKB\n\n    free Memory:      %u\n    free disk space:  %ld",GetFreeSpace(0),Name->GetFreeDiskSpace());
  MessageBox(buf,"About");
}

void TVisionDialog::CheckDiskSpace(long needed) {
  if (Name->GetFreeDiskSpace()<needed) {
	 char buf[400];
	 sprintf(buf,"\n    Disk space low !\n    free disk space:  %ld",Name->GetFreeDiskSpace());
	 MessageBox(buf,"Warning");
  }
}

void TVisionDialog::FluorescencePicture() {
  picture=fluopicture;
  mb->SetText("Taking Picture");
  MatroxFrameGrabber->SetGain(FluorescenceGain);
  MatroxFrameGrabber->SetOffset(FluorescenceOffset);
  MatroxFrameGrabber->SetTV(0);
  BiningX=1;
  BiningY=1;
  CamStartX=(512-MatroxWidth)/2;
  CamStartY=(512-MatroxHeight*4/3)/2;
  CamWidth=MatroxWidth;
  CamHeight=MatroxHeight*4/3;
  EichH=0.046;
  EichV=0.03333;
  MatroxFrameGrabber->SetImageParameters(BiningX,BiningY,CamStartX,CamStartY,CamWidth,CamHeight,EichH,EichV,false);
  PicEichX=EichH*BiningX;
  PicEichY=EichV*BiningY;
  double posx1,posx2,posy1,posy2;
  profilH->GetPos(posx1,posx2);
  profilV->GetPos(posy1,posy2);
  picture->ReadPicture(MatroxFrameGrabber,posx1,posx2,posy1,posy2,0);
  mb->SetText("");
  PictureWin->Update();
  FluoButton->Draw(1);
}

void TVisionDialog::AbsorptionPicture() {
  mb->SetText("Taking absorption Picture");
  double posx1,posx2,posy1,posy2;
  profilH->GetPos(posx1,posx2);
  profilV->GetPos(posy1,posy2);
  int timeout0,timeout1,timeout2;
  switch (aktcamera) {
    case 'M','T': {
      if (CamWidth+CamStartX>MatroxWidth) {
        CamStartX=0;
        if (CamWidth+CamStartX>MatroxWidth) CamWidth=MatroxWidth;
      }
      if (CamHeight+CamStartX>MatroxHeight) {
        CamStartX=0;
        if (CamHeight+CamStartX>MatroxHeight) CamHeight=MatroxHeight;
      }
      MatroxFrameGrabber->SetImageParameters(BiningX,BiningY,CamStartX,CamStartY,CamWidth,CamHeight,EichH,EichV,true);
      PicEichX=EichH*BiningX;
      PicEichY=EichV*BiningY*2;//onlyoddlines==> factor 2
      MatroxFrameGrabber->SetGain(AbsorptionGain);
      MatroxFrameGrabber->SetOffset(AbsorptionOffset);
      MatroxFrameGrabber->TakeTriggeredPicture(0);
      MatroxFrameGrabber->TakeTriggeredPicture(1);
      MatroxFrameGrabber->TakeTriggeredPicture(2);
      timeout1=pictureprobe->ReadPicture(MatroxFrameGrabber,posx1,posx2,posy1,posy2,1);
      timeout2=picturenoise->ReadPicture(MatroxFrameGrabber,posx1,posx2,posy1,posy2,2);
      timeout0=pictureabs->ReadPicture(MatroxFrameGrabber,posx1,posx2,posy1,posy2,0);
    }
    case 'S': {
      if (CamWidth+CamStartX>AndorWidth) {
        CamStartX=0;
        if (CamWidth+CamStartX>AndorWidth) CamWidth=AndorWidth;
      }
      if (CamHeight+CamStartX>AndorHeight) {
        CamStartX=0;
        if (CamHeight+CamStartX>AndorHeight) CamHeight=AndorHeight;
      }
      AndorFrameGrabber->SetImageParameters(BiningX,BiningY,CamStartX,CamStartY,CamWidth,CamHeight,EichH,EichV,false);
      PicEichX=EichH*BiningX;
      PicEichY=EichV*BiningY;
      AndorFrameGrabber->TakeTriggeredPicture();
      timeout0=pictureabs->ReadPicture(AndorFrameGrabber,posx1,posx2,posy1,posy2);
      AndorFrameGrabber->TakeTriggeredPicture();
      timeout1=pictureprobe->ReadPicture(AndorFrameGrabber,posx1,posx2,posy1,posy2);
      AndorFrameGrabber->TakeTriggeredPicture();
      timeout2=picturenoise->ReadPicture(AndorFrameGrabber,posx1,posx2,posy1,posy2);
    }
  }
  pictureprobe->SetEich(PicEichX,PicEichY);
  picturenoise->SetEich(PicEichX,PicEichY);
  pictureabs->SetEich(PicEichX,PicEichY);
  if (pictureabs->PictureGood()) serial->SendCommand("*VisionPictureOk#");
  else serial->SendCommand("*VisionPictureBad#");
  LnNormalizeAbsorption(*pictureabs,*pictureprobe,*picturenoise,*abspicture,*opticalDensity,MaxOptDensity,ProbeDetuning*1E6,posx1,posx2,posy1,posy2);
  picture=opticalDensity;
  char buf[200];
  char buf2[200];
  sprintf(buf,"Absorption picture: timeout0=%i timeout1=%i timeout2=%i",timeout0,timeout1,timeout2);
  mb->SetText(buf);
  PictureWin->Update();
  AbsButton->Draw(1);
  OptDensButton->Draw(1);
  RawAbsButton->Draw(1);
  NoiseButton->Draw(1);
  ProbeButton->Draw(1);
  if (!CheckNumber && pictureabs->PictureGood()) {
	 Name->GetName(buf,aktcamera,PictureTyp,TwoPictures,Isotop);
	 sprintf(buf2,buf,'A');
	 int err1=pictureabs->Save(buf2,SAVE_AUTOMATIC,ColorTable);
	 mb->SetText(buf2);
	 sprintf(buf2,buf,'P');
	 int err2=pictureprobe->Save(buf2,SAVE_AUTOMATIC,ColorTable);
	 sprintf(buf2,buf,'N');
	 int err3=picturenoise->Save(buf2,SAVE_AUTOMATIC,ColorTable);
	 if (err1 || err2 || err3) MessageBox("Insufficient disk space!","Error");
  }
  CheckNumber=0;
  Fit1DGaussToSum();
  IntegralAtomNumber();       //Exclure si fit avec gaussienne pour # atoms est necessaire
  PictureCode[0]=aktcamera;
  PictureCode[1]=PictureTyp;
  if (Isotop==6) PictureCode[2]='6'; else PictureCode[2]='7';
  if (TwoPictures) PictureCode[3]='+'; else PictureCode[3]='-';
  PictureCode[4]=0;
  if (!InSerie) RegisterInformation(NULL,1,pictureabs->PictureGood());
  if (Isotop==7) {
	 if (PictureList7Li!=NULL) PictureList7Li->AddPicture(picture);
  } else {
	 if (PictureList6Li!=NULL) PictureList6Li->AddPicture(picture);
  }
  CheckDiskSpace(1000000);
}

void TVisionDialog::ProbePicture() {
  mb->SetText("Taking probe picture");
  MatroxFrameGrabber->SetGain(AbsorptionGain);
  MatroxFrameGrabber->SetOffset(AbsorptionOffset);
  MatroxFrameGrabber->TakeTriggeredPicture(0);
  double posx1,posx2,posy1,posy2;
  profilH->GetPos(posx1,posx2);
  profilV->GetPos(posy1,posy2);
  pictureprobe->ReadPicture(MatroxFrameGrabber,posx1,posx2,posy1,posy2,0);
  picture=pictureprobe;
  PictureWin->Update();
  AbsButton->Draw(1);
  if (PictureList7Li!=NULL) PictureList7Li->AddPicture(picture);
  if (PictureList6Li!=NULL) PictureList6Li->AddPicture(picture);
}

void TVisionDialog::EvTimer(UINT timerid) {
  switch (timerid) {
	 case ID_FILMTIMER:
		FluorescencePicture();
	 break;
	 case ID_TESTSERIALTIMER:
		CheckSerial();
	 break;
  }
}

void TVisionDialog::FilmFluorescence() {
  if (filming) {
	 mb->SetText("Stopt to film");
	 KillTimer(ID_FILMTIMER);
	 filming=0;
  } else {
	 mb->SetText("Continuously filming");
	 SetTimer(ID_FILMTIMER,filmtime);
	 filming=1;
  }
};

void TVisionDialog::GridOnOff() {
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

void TVisionDialog::SavePictureAsBMP() {
  if (TFileSaveDialog(this, BMPFileData, 0,"Save BMP Picture").Execute()==IDOK) {
	 picture->Save(BMPFileData.FileName,SAVE_PALETTEBMP,ColorTable);
	 mb->SetText("Picture saved as palette BMP");
	 CheckDiskSpace(1000000);
  }
};

void TVisionDialog::SavePictureAsRGBTGA() {
  if (TFileSaveDialog(this, TGAFileData, 0,"Save truecolor TGA Picture").Execute()==IDOK) {
	 picture->Save(TGAFileData.FileName,SAVE_TRUECOLORTGA,ColorTable);
	 mb->SetText("Picture saved as truecolor TGA");
	 CheckDiskSpace(1000000);
  }
};

void TVisionDialog::SavePictureAsBWTGA() {
  if (TFileSaveDialog(this, TGAFileData, 0,"Save grayscale TGA Picture").Execute()==IDOK) {
	 picture->Save(TGAFileData.FileName,SAVE_GRAYSCALETGA,ColorTable);
	 mb->SetText("Picture saved as grayscale TGA");
	 CheckDiskSpace(1000000);
  }
};

void TVisionDialog::LoadBMPPicture() {
  if (TFileOpenDialog(this, BMPFileData, 0,"Load BMP Picture").Execute()==IDOK) {
	 if (picture->Load(BMPFileData.FileName,true)==0) {
	   UpdateAll();
	   ColorTable->Update();
	   mb->SetText("BMP Picture loaded");
    } else MessageBox("Error loading BMP picture!","Error!");
  }
};

void TVisionDialog::LoadTGAPicture() {
  if (TFileOpenDialog(this, TGAFileData, 0,"Load TGA Picture").Execute()==IDOK) {
	 if (picture->Load(TGAFileData.FileName,false)==0) {
	   UpdateAll();
	   ColorTable->Update();
	   mb->SetText("TGA Picture loaded");
    } else MessageBox("TGA picture not loaded!\nPossibly truecolor picture.","Error!");
  }
};

void TVisionDialog::LoadMeasurement() {
  if (TFileOpenDialog(this, MESFileData, 0,"Load Measurement").Execute()==IDOK)
	 LoadMeasurement(MESFileData.FileName);
}

void TVisionDialog::LoadSerie() {
  if (TFileOpenDialog(this, SerieFileData, 0,"Load Serie").Execute()==IDOK)
	 TNmeasure->Load(SerieFileData.FileName,0);
}

void TVisionDialog::NewSerie() {
  TNmeasure->New();
  TNmeasure->Draw();
}

void TVisionDialog::AddSerie() {
  if (TFileOpenDialog(this, SerieFileData, 0,"Add Serie").Execute()==IDOK)
	 TNmeasure->Load(SerieFileData.FileName,1);
}

void TVisionDialog::SaveSerie() {
  if (TFileSaveDialog(this, SerieFileData, 0,"Save Serie").Execute()==IDOK)
	 TNmeasure->SaveSerie(SerieFileData.FileName);
  CheckDiskSpace(1000000);
}

void TVisionDialog::StartSerie() {
  InSerie=1;
  char **ParamNames=new char* [1];
  ParamNames[0]=new char[200];
  strcpy(ParamNames[0],"Parameter");
  TNmeasure->StartSerie("free Serie",1,ParamNames);
}

void TVisionDialog::StopSerie() {
  InSerie=0;
  TNmeasure->StopSerie();
}

void TVisionDialog::SortSerie() {
  TNmeasure->SortSerie();
  mb->SetText("Serie sorted");
}

void TVisionDialog::LoadBigSerie() {
  if (TFileOpenDialog(this, BigSerieFileData, 0,"Load BigSerie").Execute()==IDOK)
	 Bigmeasure->Load(BigSerieFileData.FileName,0);
}

void TVisionDialog::NewBigSerie() {
  Bigmeasure->New();
  Bigmeasure->Draw();
}

void TVisionDialog::AddBigSerie() {
  if (TFileOpenDialog(this, BigSerieFileData, 0,"Add BigSerie").Execute()==IDOK)
	 Bigmeasure->Load(BigSerieFileData.FileName,1);
}

void TVisionDialog::SaveBigSerie() {
  if (TFileSaveDialog(this, BigSerieFileData, 0,"Save BigSerie").Execute()==IDOK)
	 Bigmeasure->SaveSerie(BigSerieFileData.FileName);
  CheckDiskSpace(1000000);
}

void TVisionDialog::StartBigSerie() {
  char **ParamNames=new char* [1];
  ParamNames[0]=new char[200];
  strcpy(ParamNames[0],"Parameter");
  Bigmeasure->StartBigSerie("free BigSerie",1,ParamNames);
}

void TVisionDialog::StopBigSerie() {
  Bigmeasure->StopBigSerie();
}

void TVisionDialog::SortBigSerie() {
  Bigmeasure->SortSerie();
  mb->SetText("Big Serie sorted");
}

void TVisionDialog::LoadPictureData(const char *aname) {
  char dataname[200];
  char path[200];
  char nrbuf[5];
  int i=strlen(aname);
  while ((i>0) && (aname[i]!='\\')) i--;
  char cam=aname[i+1];
  char typ=aname[i+2];
  char iso=aname[i+8];
  if (iso=='8') iso='6';
  if (iso=='9') iso='7';
  strncpy(path,aname,i+1);
  path[i+1]=0;
  strncpy(nrbuf,&aname[i+3],4);
  nrbuf[4]=0;
  int PicNr=atoi(nrbuf);
  sprintf(dataname,"%sdata%c%c%c.dat",path,cam,typ,iso);
  ifstream *in=new ifstream(dataname);
  if (!in) {
	 char buf[200];
	 sprintf(buf,"Could not open file %s",dataname);
	 MessageBox(buf,"Error");
	 return;
  }
  char buf[300];
  in->getline(buf,300);
  in->getline(buf,300);
  if (strncmp(buf,"PicNr Ok Code",13)==0) {
	 char buf2[200];
	 char buf3[200];
	 int ParamNr=0;
	 strcpy(buf2,&buf[13]);
	 do {
		while (buf2[0]==' ') strcpy(buf2,&buf2[1]);
		int h=strchr(buf2,' ')-buf2;
		strncpy(buf3,buf2,h);
		buf3[h]=0;
		if (strcmp(buf3,"SigmaX")!=0) ParamNr++;
		strcpy(buf2,strchr(buf2,' ')+1);
	 } while (ParamNr<10 && (strcmp(buf3,"SigmaX")!=0));
	 while (!in->eof()) {
		in->getline(buf,300,' ');
		if (buf[0]!=0) {
		  int nr=atoi(buf);
		  if (nr==PicNr) {
			 in->getline(buf,300,' ');
			 int Ok=atoi(buf);
			 char code[300];
			 in->getline(code,300,' ');
			 for (i=0;i<(ParamNr+9);i++) in->getline(buf,300,' ');
			 double Detuning=atof(buf);
			 SetProbeDetuning(Detuning);
			 if (code[2]=='7') SetIsotop(7); else SetIsotop(6);
			 in->getline(buf,300,' ');
			 TimeOfFlight=atof(buf);
			 in->getline(buf,300,' ');
			 Calibration=atof(buf);
          in->getline(buf,300,' ');
			 PicEichX=atof(buf);
          in->getline(buf,300,' ');
			 PicEichY=atof(buf);
          in->getline(buf,300,' ');
			 BiningX=atoi(buf);
          in->getline(buf,300,' ');
			 BiningY=atoi(buf);
			 MessDataTextBox->SetValue(2,TimeOfFlight);
			 delete in;
			 return;
		  } else in->getline(buf,300);
		}
	 }
  };
  delete in;
  buf[200];
  sprintf(buf,"Problem reading file %s",dataname);
  MessageBox(buf,"Error");
}

void TVisionDialog::LoadMeasurement(const char *aname) {
  char buf[200];
  LoadPictureData(aname);
  strcpy(buf,aname);
  int i=strlen(aname)-4;
  while ((i>0) && buf[i]!='A' && buf[i]!='a') i--;
  int err1=pictureabs->Load(buf,true);
  buf[i]='P';
  int err2=pictureprobe->Load(buf,true);
  buf[i]='N';
  int err3=picturenoise->Load(buf,true);
  if (err1 || err2 || err3) MessageBox("Problems loading measurement!","Error");
  pictureabs->SetEich(PicEichX,PicEichY);
  pictureprobe->SetEich(PicEichX,PicEichY);
  picturenoise->SetEich(PicEichX,PicEichY);
  double posx1,posx2,posy1,posy2;
  profilH->GetPos(posx1,posx2);
  profilV->GetPos(posy1,posy2);
  LnNormalizeAbsorption(*pictureabs,*pictureprobe,*picturenoise,*abspicture,*opticalDensity,MaxOptDensity,ProbeDetuning*1E6,posx1,posx2,posy1,posy2);
  picture=opticalDensity;
  UpdateAll();
  Fit1DGaussToSum();
  IntegralAtomNumber();
  if (PictureList7Li!=NULL && Isotop==7) PictureList7Li->AddPicture(picture);
  if (PictureList6Li!=NULL && Isotop==6) PictureList6Li->AddPicture(picture);
  mb->SetText(aname);
}

void TVisionDialog::LoadPalette() {
  if (TFileOpenDialog(this, BMPFileData, 0,"Load Palette").Execute()==IDOK)
	 ColorTable->LoadPalette(BMPFileData.FileName);
  UpdateAll();
  mb->SetText("Palette loaded");
};

void TVisionDialog::Fit1DGaussHV(int horiz,double &Amp, double &X0, double &Sigma, double &Offset) {
  double sigma,height,offset;
  double *x;
  x=NULL;
  double *y;
  x=NULL;
  int ndata=0;
  double x0;
  if (horiz) profilH->GetList(x,y,ndata,x0,sigma,height,offset);
  else profilV->GetList(x,y,ndata,x0,sigma,height,offset);
  double *sig=new double[ndata];
  for (int i=0;i<ndata;i++) sig[i]=1;
  double *a=new double[4];
  a[0]=height;
  a[1]=x0;
  a[2]=sigma;
  a[3]=offset;
  double chisq;
  double *pchisq=&chisq;
  fit(x,y,sig,ndata,a,4,pchisq,gauss);
  Amp=a[0];
  X0=a[1];
  Sigma=a[2];
  Offset=a[3];
  delete a;
  delete x;
  delete y;
  delete sig;
}

void TVisionDialog::Fit1DGaussToSum() {
  double pos1=0;
  double pos2=0;
  profilV->GetPos(pos1,pos2);
  profilH->GetSum(pos1,pos2);
  profilH->GetPos(pos1,pos2);
  profilV->GetSum(pos1,pos2);
  Fit1DGauss();
  if (CursorFixed) {
	 X0=FixedX0;
	 Y0=FixedY0;
  }
  OptDens=opticalDensity->GetAbsorption(X0,Y0);
  Abs=abspicture->GetAbsorption(X0,Y0);
  GaussFitTextBox->SetValue(0,OptDens);
  NGauss=Calibration*Number(SigmaX*1E-3, SigmaY*1E-3, OptDens, ProbeDetuning*1E6);
  GaussFitTextBox->SetValue(8,NGauss);
  double h=(pow(2*M_PI,1.5)*SigmaX*1E-3*SigmaY*1E-3*sqrt(fabs(SigmaX*1E-3*SigmaY*1E-3)));
  if (h==0) h=100000;
  double n0=1E-6*NGauss/h;
  GaussFitTextBox->SetValue(9,n0);
  double aX0=X0;
  double aY0=Y0;
  picture->CalcNormalizedX0Y0(aX0,aY0);
  PictureWin->SetCross(aX0,aY0);
}

void TVisionDialog::IntegralAtomNumber() {
  double xmin,xmax,ymin,ymax;
  profilV->GetPos(ymin,ymax);
  profilH->GetPos(xmin,xmax);
  N=Calibration*opticalDensity->Integral(xmin,ymin,xmax,ymax)*1E-6/SigmaRest(ProbeDetuning*1E6);
  GaussFitTextBox->SetValue(4,N);
  double h=(pow(2*M_PI,1.5)*SigmaX*1E-3*SigmaY*1E-3*sqrt(fabs(SigmaX*1E-3*SigmaY*1E-3)));
  if (h==0) h=100000;
  double n0=1E-6*N/h;
  GaussFitTextBox->SetValue(9,n0);
}

void TVisionDialog::Fit1DGauss() {
  double OptDensX,OptDensY;

  Fit1DGaussHV(1,OptDensX,X0,SigmaX,OffsetX);
  Fit1DGaussHV(0,OptDensY,Y0,SigmaY,OffsetY);
  OptDens=(OptDensX+OptDensY)/2.0;
  GaussFitTextBox->SetValue(0,OptDens);
  GaussFitTextBox->SetValue(1,X0);
  GaussFitTextBox->SetValue(2,SigmaX);
  GaussFitTextBox->SetValue(3,OffsetX);
  N=Calibration*Number(SigmaX*1E-3, SigmaY*1E-3, OptDens,ProbeDetuning*1E6);
  GaussFitTextBox->SetValue(4,N);
  GaussFitTextBox->SetValue(5,Y0);
  GaussFitTextBox->SetValue(6,SigmaY);
  GaussFitTextBox->SetValue(7,OffsetY);
  profilH->SetGauss(OptDensX,X0,SigmaX,OffsetX);
  profilV->SetGauss(OptDensY,Y0,SigmaY,OffsetY);
}

void TVisionDialog::ShowFluorescence() {
  picture=fluopicture;
  PictureWin->Update();
  if (autofit) Fit1DGauss();
}

void TVisionDialog::ShowAbsorption() {
  picture=abspicture;
  PictureWin->Update();
  if (autofit) Fit1DGauss();
}

void TVisionDialog::ShowOpticalDensity() {
  picture=opticalDensity;
  PictureWin->Update();
  if (autofit) Fit1DGauss();
}

void TVisionDialog::ShowAbsDirect() {
  picture=pictureabs;
  PictureWin->Update();
  if (autofit) Fit1DGauss();
}

void TVisionDialog::ShowProbe() {
  picture=pictureprobe;
  PictureWin->Update();
  if (autofit) Fit1DGauss();
}

void TVisionDialog::ShowNoise() {
  picture=picturenoise;
  PictureWin->Update();
  if (autofit) Fit1DGauss();
}

void TVisionDialog::ToggleProfilDesign() {
  profilH->ToggleDesign();
  profilV->ToggleDesign();
}

LRESULT TVisionDialog::PictureChanged(WPARAM newpicture,LPARAM) {
  switch (newpicture) {
	 case ID_FLUO:picture=fluopicture;break;
	 case ID_ABS:picture=abspicture;break;
	 case ID_ABSDIRECT:picture=pictureabs;break;
	 case ID_PROBE:picture=pictureprobe;break;
	 case ID_NOISE:picture=picturenoise;break;
	 case ID_OPTDENS:picture=opticalDensity;break;
  }
  PictureWin->Update();
  if (autofit) Fit1DGauss();
  return 0;
}

LRESULT TVisionDialog::PaletteChanged(WPARAM ,LPARAM) {
  UpdateAll();
  return 0;
}

LRESULT TVisionDialog::LoadMess(WPARAM ,LPARAM) {
  SetProbeDetuning(TNmeasure->GetMesProbeDetuning());
  LoadMeasurement(TNmeasure->GetMesName());
  TNmeasure->SetData(X0,SigmaX,Y0,SigmaY,Abs,OptDens,N,NGauss);
  return 0;
}

void TVisionDialog::DeleteMeasurement() {
  TNmeasure->DeleteAktPoint();
}

LRESULT TVisionDialog::Fit1DGauss(WPARAM ,LPARAM) {
  if (autofit) Fit1DGauss();
  return 0;
}

void TVisionDialog::FitTemperature() {
  TNmeasure->TemperatureFit();
}

void TVisionDialog::FitAtomNumber() {
  TNmeasure->NumberFit();
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
  double xpos,ypos,height;
  PictureWin->GetPos(xpos,ypos);
  height=picture->GetEichPixel(xpos,ypos);
  xpos=picture->GetEichPosX(xpos);
  ypos=picture->GetEichPosY(ypos);
  CursorTextBox->SetValue(0,xpos);
  CursorTextBox->SetValue(1,ypos);
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
  PictureWin->Update();
  FluoButton->Draw(1);
  AbsButton->Draw(1);
  OptDensButton->Draw(1);
  RawAbsButton->Draw(1);
  NoiseButton->Draw(1);
  ProbeButton->Draw(1);
}

void TVisionDialog::SetStandardPalette() {
  ColorTable->SetStandard();
  UpdateAll();
}

void TVisionDialog::AddStripes() {
  ColorTable->AddStripes();
  UpdateAll();
}

void TVisionDialog::FixCursor() {
  CursorFixed=1;
  FixedX0=X0;
  FixedY0=Y0;
  mb->SetText("Cursor fixed");
  PictureWin->SetCrossfixed(1);
}

void TVisionDialog::UnFixCursor() {
  CursorFixed=0;
  mb->SetText("Cursor unfixed");
  PictureWin->SetCrossfixed(0);
}

void TVisionDialog::RegisterInformation(double *ax,int NoSerie,int Ok) {
  TNmeasure->AddPoint(ax,X0,SigmaX,Y0,SigmaY,Abs,OptDens,ProbeDetuning,
		 TimeOfFlight,Calibration,PicEichX,PicEichY,BiningX,BiningY,N,NGauss,NoSerie,Ok,PictureCode);
}

void TVisionDialog::SetIsotop(int aIsotop) {
  Isotop=aIsotop;
  MessDataTextBox->SetValue(3,Isotop);
  if (Isotop==7) {
	 TNmeasure->SetIsotop(7);
	 mb->SetText("Lithium 7");
	 SetClebsch(ClebschLi7);
	 SetGamma(GammaLi7);
	 MaxOptDensity=MaxOptDensity7Li;
  } else {
	 TNmeasure->SetIsotop(6);
	 mb->SetText("Lithium 6");
	 SetClebsch(ClebschLi6);
	 SetGamma(GammaLi6);
	 MaxOptDensity=MaxOptDensity6Li;
  }
}

void TVisionDialog::CheckSerial() {
  if (!checkingSerial && serial) {
	 checkingSerial=1;
	 char Command[100];
	 if (serial->GetCommand(Command)) {
		mb->SetText(Command);
		if (strcmp(Command,"VisionTakeAbsorptionPicture")==0) {
		  serial->SendCommand("*Ready#");
		  double Detuning;
		  serial->GetChar(aktcamera);
        serial->GetDouble(EichH);
        serial->GetDouble(EichV);
        serial->GetInt(BiningX);
        serial->GetInt(BiningY);
        serial->GetInt(CamStartX);
        serial->GetInt(CamStartY);
        serial->GetInt(CamWidth);
        serial->GetInt(CamHeight);
	     switch (aktcamera)  {
		    case 'M': MessDataTextBox->SetValue(0,0);
		    case 'T': MessDataTextBox->SetValue(0,1);
          case 'S': MessDataTextBox->SetValue(0,2);
	     }
		  serial->GetChar(PictureTyp);
		  serial->GetInt(Isotop);
		  serial->GetDouble(Detuning);
		  serial->GetDouble(TimeOfFlight);
		  serial->GetDouble(Calibration);
		  serial->GetInt(TwoPictures);
		  MessDataTextBox->SetValue(2,TimeOfFlight);
		  SetProbeDetuning(Detuning);
		  SetIsotop(Isotop);
		  if (TwoPictures && (Isotop==7)) PictureList6Li->SetPosition(PictureList7Li->GetPosition());
		  AbsorptionPicture();
		}else if (strcmp(Command,"VisionTakeProbePicture")==0) {
		  serial->SendCommand("*Ready#");
		  ProbePicture();
		} else if (strcmp(Command,"VisionStartSerie")==0) {
		  InSerie=1;
		  serial->SendCommand("*Ready#");
		  int numbParam;
		  serial->GetInt(numbParam);
		  char **ParamNames=new char* [numbParam];
		  for (int i=0; i<numbParam;i++) {
			 ParamNames[i]=new char[200];
			 serial->GetString(ParamNames[i]);
		  }
		  char comentary[200];
		  serial->GetString(comentary);
		  TNmeasure->StartSerie(comentary,numbParam,ParamNames);
		} else if (strcmp(Command,"VisionStopSerie")==0) {
		  InSerie=0;
		  serial->SendCommand("*Ready#");
		  TNmeasure->StopSerie();
		} else if (strcmp(Command,"VisionRegisterInformation")==0) {
		  serial->SendCommand("*Ready#");
		  int hi=TNmeasure->GetParamNr();
		  double *h= new double[hi];
		  for (int i=0;i<hi;i++) serial->GetDouble(h[i]);
		  RegisterInformation(h,0,pictureabs->PictureGood());
		} else if (strcmp(Command,"VisionStartBigSerie")==0) {
		  strcpy(BigCode,"----");
		  serial->SendCommand("*Ready#");
		  int numbParam;
		  serial->GetInt(numbParam);
		  char **ParamNames=new char* [numbParam];
		  for (int i=0; i<numbParam;i++) {
			 ParamNames[i]=new char[200];
			 serial->GetString(ParamNames[i]);
		  }
		  char comentary[200];
		  serial->GetString(comentary);
		  if (Bigmeasure!=NULL) Bigmeasure->StartBigSerie(comentary,numbParam,ParamNames);
		} else if (strcmp(Command,"VisionStopBigSerie")==0) {
		  serial->SendCommand("*Ready#");
		  if (Bigmeasure!=NULL) Bigmeasure->StopBigSerie();
		} else if (strcmp(Command,"VisionRegisterBigInformation")==0) {
		  if (Bigmeasure!=NULL) {
			 serial->SendCommand("*Ready#");
			 int hi=Bigmeasure->GetParamNr();
			 double *h= new double[hi];
			 for (int i=0;i<hi;i++) serial->GetDouble(h[i]);
			 double TempX=TNmeasure->GetTempX();
			 double TempY=TNmeasure->GetTempY();
			 if (BigNumber==0) {
				if (BigTempNumber!=0) BigNumber=BigTempNumber; else BigNumber=TNmeasure->GetN();
			 }
			 if (BigSigmaX==0) BigSigmaX=TNmeasure->GetSigma0X();
			 if (BigSigmaY==0) BigSigmaY=TNmeasure->GetSigma0Y();
			 Bigmeasure->AddPoint(Isotop,h,TempX,TempY,BigSigmaX,BigSigmaY,BigNumber,
				TNmeasure->GetSigma0X(),TNmeasure->GetSigma0Y(),BigTempNumber,BigCode);
			 BigNumber=0;
			 BigTempNumber=0;
			 BigSigmaX=0;
			 BigSigmaY=0;
		  }
		} else if (strcmp(Command,"VisionRegisterBigSigma")==0) {
		  serial->SendCommand("*Ready#");
		  BigSigmaX=SigmaX;
		  BigSigmaY=SigmaY;
		  BigCode[0]='S';
		} else if (strcmp(Command,"VisionRegisterBigNumber")==0) {
		  serial->SendCommand("*Ready#");
		  BigNumber=NGauss;
		  BigCode[2]='N';
		  BigCode[3]='G';
		} else if (strcmp(Command,"VisionCheckNumber")==0) {
		  serial->SendCommand("*Ready#");
		  CheckNumber=1;
		} else if (strcmp(Command,"VisionTemperatureFit")==0) {
		  serial->SendCommand("*Ready#");
		  TNmeasure->TemperatureFit();
		  BigCode[1]='T';
		} else if (strcmp(Command,"VisionChargingTimeFit")==0) {
		  serial->SendCommand("*Ready#");
		  TNmeasure->ChargingTimeFit();
		} else if (strcmp(Command,"VisionLifetimeFit")==0) {
		  serial->SendCommand("*Ready#");
		  TNmeasure->LifetimeFit();
		} else if (strcmp(Command,"VisionNumberFit")==0) {
		  serial->SendCommand("*Ready#");
		  TNmeasure->NumberFit();
		} else if (strcmp(Command,"VisionSimpleNumberFit")==0) {
		  serial->SendCommand("*Ready#");
		  TNmeasure->SimpleNumberFit();
		} else if (strcmp(Command,"VisionRegisterBigTemperatureNumber")==0) {
		  serial->SendCommand("*Ready#");
		  serial->GetInt(BigTempPictureNumb);
		  BigTempNumber=TNmeasure->GetNumber(BigTempPictureNumb);
		  BigCode[2]='N';
		  char buf[10];
		  sprintf(buf,"%i",BigTempPictureNumb);
		  BigCode[3]=buf[0];
		} else if (strcmp(Command,"VisionRegisterParameters")==0) {
		  serial->SendCommand("*Ready#");
		  int pnr;
		  serial->GetInt(pnr);
		  char name[200];
		  Name->GetParamName(name);
		  ofstream out(name);
		  for (int i=0;i<pnr;i++) {
			 serial->GetString(name);
			 out<<name<<endl;
		  }
		}
	 }
	 checkingSerial=0;
  }
}

void TVisionDialog::SetProbeDetuning(double aProbeDetuning) {
  ProbeDetuning=aProbeDetuning;
  MessDataTextBox->SetValue(1,ProbeDetuning);
}

bool TVisionDialog::IdleAction(long idleCount) {
  TWindow::IdleAction(idleCount);
  CheckSerial();
  return TRUE;
}

void TVisionDialog::MeasureTNButtonClicked() {
  TNmeasure->Draw();
}

void TVisionDialog::BigMeasureButtonClicked() {
  if (Bigmeasure!=NULL) Bigmeasure->Draw();
}

void TVisionDialog::ReCalc() {
  TNmeasure->ReCalc();
}
