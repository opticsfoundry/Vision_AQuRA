#include "rdsetup.h"
#include "setup.h"
#include "stdio.h"
#include "string.h"
#include "fstream.h"
#include <messageb.h>

/*
const RDSetupXNameNr=0;
const RDSetupYNameNr=4;
const RDSetupTextNameNr=2;
char *RDSetupXName[RDSetupXNameNr]={};
char *RDSetupYName[RDSetupYNameNr]={{"DataDisk"},{"HardwareAccess"},{"LiveDirectory"},{"LiveDisk"},{"LiveConnectTime"}};
char *RDSetupTextName[RDSetupTextNameNr]={{"DataDirectory"},{"MainDirectory"}};
*/

bool GetCommand(ifstream *in,char *command, char *data) {
  if (in->eof()) return false;
  char buf[string_length];
  in->getline(buf, string_length);
  if (buf[0]=='/') return false; // skip comment lines
  char *sep=strchr(buf,'=');
  if (!sep) return false;
  strncpy(command,buf,sep-buf);
  command[sep-buf]=0;
  sep++;
  strcpy(data,sep);
  sep=strchr(data,'/');
  if (sep) data[sep-data]=0; // remove trailing comment if present
  sep=strchr(data,' ');
  if (sep) data[sep-data]=0; // remove trailing space if present
  return true;
}

/*void copy(char *&dest,const char *source) {
  if (dest) delete dest;
  dest=new char[strlen(source)+1];
  strcpy(dest,source);
} */
/*
int NrNetAndorCameras=6;
unsigned short NetAndorCameraPort[NrNetAndorCameras]={713,714,715,716,717,718};
char* NetAndorCameraIPAddress[NrNetAndorCameras] = { "192.168.88.101","192.168.88.101","192.168.88.101","192.168.88.101","192.168.88.101","192.168.88.101" };

*/
CRDSetup::CRDSetup() {
  //strcpy(MainDirectory,InitMainDirectory);
  //strcpy(DataDirectory,InitDataDirectory);
  HardwareAccess=InitHardwareAccess;
  LiveDirectory=InitLiveDirectory;
  LiveDisk=InitLiveDisk;
  DataDisk=InitDataDisk;
  LiveConnectTime=InitLiveConnectTime;

  ifstream *in=new ifstream(ConfigFileName);
  if (in->fail()) {
    delete in;
	 char buf[string_length];
   sprintf(buf,"..\\Source\\%s",ConfigFileName);
    in = new ifstream(buf);
    if (in->fail()) {
      sprintf(buf,"Could not open Configuration File %s or ..\\Source\\%s.",ConfigFileName,ConfigFileName);
        MessageBox(GetActiveWindow(),buf,"Error!",MB_OK);
        delete in;
      return;
      }
}
  char command[string_length];
  char data[string_length];
  while (!in->eof()) {
    if (GetCommand(in,command,data)) {
       if (strcmp(command,"DataDirectory")==0) {
        data[string_length-1]=0; // ensure null termination
        strcpy(DataDirectory,data);
       } else if (strcmp(command,"MainDirectory")==0) {
        data[string_length-1]=0; // ensure null termination
        strcpy(MainDirectory,data);
       }else if (strcmp(command,"ParamFileName")==0) {
        data[string_length-1]=0; // ensure null termination
        strcpy(ParamFileName,data);
       }
       else if (strcmp(command,"HardwareAccess")==0) HardwareAccess=(strcmp(data,"true")==0);
       else if (strcmp(command,"LiveDirectory")==0) LiveDirectory=data[0];
       else if (strcmp(command,"LiveDisk")==0) LiveDisk=atoi(data);
       else if (strcmp(command,"DataDisk")==0) DataDisk=atoi(data);
       else if (strcmp(command,"LiveConnectTime")==0) LiveConnectTime=atoi(data);
       else if (strcmp(command,"NrCameras")==0) NrNetAndorCameras=atoi(data);
       else if (strcmp(command,"CameraIPPort0")==0) NetAndorCameraPort[0]=atoi(data);
       else if (strcmp(command,"CameraIPPort1")==0) NetAndorCameraPort[1]=atoi(data);
       else if (strcmp(command,"CameraIPPort2")==0) NetAndorCameraPort[2]=atoi(data);
       else if (strcmp(command,"CameraIPPort3")==0) NetAndorCameraPort[3]=atoi(data);
       else if (strcmp(command,"CameraIPPort4")==0) NetAndorCameraPort[4]=atoi(data);
       else if (strcmp(command,"CameraIPPort5")==0) NetAndorCameraPort[5]=atoi(data);
       else if (strcmp(command,"CameraIPAddress0")==0) {
       	data[15] = 0; // ensure null termination
       	strcpy(NetAndorCameraIPAddress[0],data);
       } else if (strcmp(command,"CameraIPAddress1")==0) {
       	data[15] = 0;
       	strcpy(NetAndorCameraIPAddress[1],data);
       } else if (strcmp(command,"CameraIPAddress2")==0) {
       	data[15] = 0;
       	strcpy(NetAndorCameraIPAddress[2],data);
       } else if (strcmp(command,"CameraIPAddress3")==0) {
       	data[15] = 0;
       	strcpy(NetAndorCameraIPAddress[3],data);
       } else if (strcmp(command,"CameraIPAddress4")==0) {
       	data[15] = 0;
       	strcpy(NetAndorCameraIPAddress[4],data);
       } else if (strcmp(command,"CameraIPAddress5")==0) {
       	data[15] = 0;
       	strcpy(NetAndorCameraIPAddress[5],data);
       }
       else if (strcmp(command,"NrPictureLists")==0) NrPictureLists=atoi(data);
       else if (strcmp(command,"NrCameraButtons")==0) NrCameraButtons=atoi(data);
       else if (strcmp(command,"filmtime")==0) filmtime=atoi(data);
       else if (strcmp(command,"testserialtime")==0) testserialtime=atoi(data);
       else if (strcmp(command,"twopicturelists")==0) twopicturelists=atoi(data);
       else if (strcmp(command,"MaxOptDensity6Li")==0) MaxOptDensity6Li=atof(data);
       else if (strcmp(command,"MaxOptDensity7Li")==0) MaxOptDensity7Li=atof(data);
       else if (strcmp(command,"ClebschLi7")==0) ClebschLi7=atof(data);
       else if (strcmp(command,"ClebschLi6")==0) ClebschLi6=atof(data);
       else if (strcmp(command,"GammaLi7")==0) GammaLi7=atof(data);
       else if (strcmp(command,"GammaLi6")==0) GammaLi6=atof(data);
       else if (strcmp(command,"EichHeight")==0) EichHeight=atof(data);
       else if (strcmp(command,"EichOffset")==0) EichOffset=atof(data);
       else if (strcmp(command,"SigmaEl")==0) SigmaEl=atof(data);
       else if (strcmp(command,"CameraInclination")==0) CameraInclination=atof(data);
       else if (strcmp(command,"OffsetButtonScale")==0) OffsetButtonScale=atof(data);
       else if (strcmp(command,"InitialOffsetShift")==0) InitialOffsetShift=atof(data);
       else if (strcmp(command,"WindowSize")==0) WindowSize=atoi(data);
       else if (strcmp(command,"WindowHeight")==0) WindowHeight=atoi(data);
       else if (strcmp(command,"WindowOriginX")==0) WindowOriginX=atoi(data);
       else if (strcmp(command,"WindowOriginY")==0) WindowOriginY=atoi(data);
       else if (strcmp(command,"R")==0) R=atoi(data);
       else if (strcmp(command,"PH")==0) PH=atoi(data);
       else if (strcmp(command,"PH2")==0) PH2=atoi(data);
       else if (strcmp(command,"PH3")==0) PH3=atoi(data);
       else if (strcmp(command,"D")==0) D=atoi(data);
       else if (strcmp(command,"D2")==0) D2=atoi(data);
       else if (strcmp(command,"WH")==0) WH=atoi(data);
       else if (strcmp(command,"WV")==0) WV=atoi(data);
       else if (strcmp(command,"BB")==0) BB=atoi(data);
       else if (strcmp(command,"Slash")==0) Slash=atoi(data);
       else if (strcmp(command,"Text")==0) Text=atoi(data);
       else if (strcmp(command,"SB")==0) SB=atoi(data);
       else if (strcmp(command,"SH")==0) SH=atoi(data);
       else if (strcmp(command,"PictureListHeight")==0) PictureListHeight=atoi(data);
       else if (strcmp(command,"BoxWide")==0) BoxWide=atoi(data);
       else if (strcmp(command,"TH")==0) TH=atoi(data);
       else if (strcmp(command,"D3")==0) D3=atoi(data);
       else if (strcmp(command,"MTextBoxH")==0) MTextBoxH=atoi(data);
       else if (strcmp(command,"GTextBoxH")==0) GTextBoxH=atoi(data);
       else if (strcmp(command,"TTextBoxH")==0) TTextBoxH=atoi(data);
       else if (strcmp(command,"TextBoxHeight")==0) TextBoxHeight=atoi(data);
       else if (strcmp(command,"TextBox3Width")==0) TextBox3Width=atoi(data);
       else if (strcmp(command,"TextBox4Width")==0) TextBox4Width=atoi(data);
       else if (strcmp(command,"TextBox4WidthSmall")==0) TextBox4WidthSmall=atoi(data);
       else if (strcmp(command,"InitialOptDensMethod")==0) InitialOptDensMethod=atoi(data);
       else if (strcmp(command,"PictureListColumns")==0) PictureListColumns=atoi(data);
      else if (strcmp(command,"TestCameraWidth")==0) TestCameraWidth=atoi(data);
      else if (strcmp(command,"TestCameraHeight")==0) TestCameraHeight=atoi(data);
      else if (strcmp(command,"TestCameraExternalTrigger")==0) TestCameraExternalTrigger=(strcmp(data,"true")==0);
      else if (strcmp(command,"DebugControlComm")==0) DebugControlComm=(strcmp(data,"true")==0);
      else if (strcmp(command,"DebugCameraComm")==0) DebugCameraComm=(strcmp(data,"true")==0);

       //else if (strcmp(command,"NrCameras")==0) NrNetAndorCameras=atoi(data);
       else {
         char buf[200];
         sprintf(buf,"Unkown parameter %s = %s",command,data);
         MessageBox(GetActiveWindow(),buf,"Error!",MB_OK);
       }
    }
  }
  delete in;
  //calc interface layout parameters
  DX0=R+PH+D+SB+D2+WH+D2+BoxWide+D2;
  PBy=(WV-2*(TH+D3))/2.1;
  PBx=PBy*WH/WV;
  PBly=(WV-2*(TH+D3))/3.1;
  PBlx=PBly*WH/WV;
  TBW=PH+D+SB+D2+WH;
  Y1=R+WV+D+SH+D2;
  Y2=R+WV+D+SH+D2+PH+D2;
  X1=R+PH+D+SB+D2+WH+D2;
  if (NrCameraButtons>MaxNrCameraButtons) {
    char buf[200];
    sprintf(buf,"NrCameraButtons %d exceeds MaxNrCameraButtons %d",NrCameraButtons,MaxNrCameraButtons);
    MessageBox(GetActiveWindow(),buf,"Error!",MB_OK);
    NrCameraButtons=MaxNrCameraButtons;
  }
  if (NrPictureLists>MaxNrPictureLists) {
    char buf[200];
    sprintf(buf,"NrPictureLists %d exceeds MaxNrPictureLists %d",NrPictureLists,MaxNrPictureLists);
    MessageBox(GetActiveWindow(),buf,"Error!",MB_OK);
    NrPictureLists=MaxNrPictureLists;
  }
  if (NrNetAndorCameras>MaxNrNetAndorCameras) {
    char buf[200];
    sprintf(buf,"NrNetAndorCameras %d exceeds MaxNrNetAndorCameras %d",NrNetAndorCameras,MaxNrNetAndorCameras);
    MessageBox(GetActiveWindow(),buf,"Error!",MB_OK);
    NrNetAndorCameras=MaxNrNetAndorCameras;
  }
}

CRDSetup::~CRDSetup() {}
