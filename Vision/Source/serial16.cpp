#include "serial.h"
#include <string.h>
#include <math.h>
#include <stdlib.h>

CSerial::CSerial() {
  idComDev = OpenComm("COM1", 1024, 128);
  int err1 = BuildCommDCB("COM1:9600,n,8,1", &dcb);
  int err2 = SetCommState(&dcb);
//  if ((idComDev < 0) || (err1 < 0) || (err2<0)) MessageBox("Error initialising COM1","Error");
  WriteComm(idComDev, "*VisionReady#", 13);
}

CSerial::~CSerial() {
  int err= CloseComm(idComDev);
//  if (err < 0) MessageBox("Error closing COM1","Error");
}

int CSerial::GetCommand(char *string) {
  *string=0;
  char ein='@';
  while ((ReadComm(idComDev, &ein, 1)==1) && (ein!='*'));
  if (ein!='*') return 0;
  char *p=string;
  int timeout=0;
  while ((ein!='#') && (timeout<30000)) {
	 if (ReadComm(idComDev, &ein, 1)==1) {
		*p=ein;
		p++;
		timeout=0;
	 } else {
		timeout++;
		for (int j=0;j<30000;j++);
	 }
  }
  if (p>string) p--;
  *p=0;
  if (timeout<30000) return 1; else return 0;
}

void CSerial::SendCommand(char *string) {
  WriteComm(idComDev, string, strlen(string));
}

void CSerial::Clear() {
  FlushComm(idComDev,1);
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
