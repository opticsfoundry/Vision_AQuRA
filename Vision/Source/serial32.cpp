#include "serial.h"
#include <string.h>
#include <math.h>
#include <stdlib.h>

CSerial::CSerial() {
  struct _SECURITY_ATTRIBUTES Security_Attributes={8,NULL,FALSE};
  struct _COMMTIMEOUTS ComTimeouts={10,10,100,10,100};
  ComHandle = CreateFile("COM1", GENERIC_ALL, 0, &Security_Attributes, OPEN_EXISTING,
    FILE_ATTRIBUTE_NORMAL,   NULL);
  int err1 = BuildCommDCB("baud=9600 parity=N data=8 stop=1", &dcb);
  int err2 = SetCommState(ComHandle,&dcb);
  int err3 = SetCommTimeouts(ComHandle,&ComTimeouts);
  if ((ComHandle ==INVALID_HANDLE_VALUE) || (err1 < 0) || (err2<0) || (err3<0)) MessageBox(GetActiveWindow(),"Error initialising COM1","Error",MB_OK);

  SendCommand("*VisionReady#");
}

CSerial::~CSerial() {
  int err= CloseHandle(ComHandle);
  if (err < 0) MessageBox(GetActiveWindow(),"Error closing COM1","Error",MB_OK);
}

int CSerial::ReadComm(char *ein) {
  DWORD in;
  ReadFile(ComHandle, ein, 1,&in,NULL);
  return in==1;
}

int CSerial::GetCommand(char *string) {
  *string=0;
  char ein='@';
  while (ReadComm(&ein) && (ein!='*'));
  if (ein!='*') return 0;
  char *p=string;
  int timeout=GetTickCount();
  while ((ein!='#') && (GetTickCount()-timeout<30000)) {
	 if (ReadComm(&ein)) {
		*p=ein;
		p++;
		timeout=GetTickCount();
	 }
  }
  if (p>string) p--;
  *p=0;
  if (GetTickCount()-timeout<30000) return 1; else return 0;
}

void CSerial::SendCommand(char *string) {
  DWORD out;
  WriteFile(ComHandle, string, strlen(string),&out,NULL);
}

void CSerial::Clear() {
//  FlushComm(idComDev,1);
}

int CSerial::GetDouble(double &d) {
  char Command[200];
  int timeout=0;
  while ((!GetCommand(Command)) && (timeout<30000)) timeout++;
  d=atof(Command);
  return (timeout>=30000);
}

int CSerial::GetInt(int &i) {
  char Command[200];
  int timeout=0;
  while ((!GetCommand(Command)) && (timeout<30000)) timeout++;
  i=atoi(Command);
  return (timeout>=30000);
}

int CSerial::GetChar(char &c) {
  char Command[200];
  int timeout=0;
  while ((!GetCommand(Command)) && (timeout<30000)) timeout++;
  c=Command[0];
  return (timeout>=30000);
}

int CSerial::GetString(char *c) {
  char Command[200];
  int timeout=0;
  while ((!GetCommand(Command)) && (timeout<30000)) timeout++;
  strcpy(c,Command);
  return (timeout>=30000);
}
