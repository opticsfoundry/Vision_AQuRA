#include "serial.h"
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

CSerial::CSerial() {
  struct _COMMTIMEOUTS ComTimeouts={10,10,100,10,100};
  ComHandle = CreateFile("COM1", GENERIC_READ | GENERIC_WRITE, 0, NULL,
    OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
  int err1 = BuildCommDCB("baud=115200 parity=N data=8 stop=1", &dcb); //115200   9600
  dcb.fDtrControl=DTR_CONTROL_ENABLE;
  dcb.fRtsControl=RTS_CONTROL_ENABLE;
  int err2 = SetCommState(ComHandle,&dcb);
  int err3 = SetCommTimeouts(ComHandle,&ComTimeouts);
  if ((ComHandle ==INVALID_HANDLE_VALUE) || (err1!=1) || (err2!=1) || (err3!=1))
    MessageBox(GetActiveWindow(),"Error initialising COM1","Error",MB_OK);
  SendCommand("*VisionReady#");
}

CSerial::~CSerial() {
  int err= CloseHandle(ComHandle);
  if (err < 0) MessageBox(GetActiveWindow(),"Error closing COM1","Error",MB_OK);
}

int CSerial::ReadComm(char *ein) {
  DWORD in;
  ReadFile(ComHandle, ein, 1,&in,NULL);
  if (in==0) Sleep(10);
  return in==1;
}

int CSerial::GetCommand(char *string) {
  *string=0;
  char ein='@';
  while (ReadComm(&ein) && (ein!='*'));
  if (ein!='*') return 0;
  char *p=string;
  DWORD timeout=GetTickCount();
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
  bool ret=WriteFile(ComHandle, string, strlen(string),&out,NULL);
  if (!ret) {
    DWORD error=GetLastError();
    char aBuffer[100];
    sprintf(aBuffer,"Serial Write Error %u",error);
    MessageBox(GetActiveWindow(),aBuffer,"Error!",MB_OK);
  }
}

void CSerial::CloseConnection() {
}
