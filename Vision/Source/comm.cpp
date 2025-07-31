#include "comm.h"
#include <windows.h>

CComm::CComm() {
}

CComm::~CComm() {
}

int CComm::GetCommand(char *) {
	return 0;
}

void CComm::SendCommand(char *) {
}

void CComm::Clear() {
}

int CComm::GetDouble(double &d) {
  char Command[200];
  DWORD timeout=GetTickCount();
  while ((!GetCommand(Command)) && (GetTickCount()-timeout<30000)) Sleep(10);
  d=atof(Command);
  return (GetTickCount()-timeout>=30000);
}

int CComm::GetInt(int &i) {
  char Command[200];
  DWORD timeout=GetTickCount();
  while ((!GetCommand(Command)) && (GetTickCount()-timeout<30000)) Sleep(10);
  i=atoi(Command);
  return (GetTickCount()-timeout>=30000);
}

int CComm::GetChar(char &c) {
  char Command[200];
  DWORD timeout=GetTickCount();
  while ((!GetCommand(Command)) && (GetTickCount()-timeout<30000)) Sleep(10);
  c=Command[0];
  return (GetTickCount()-timeout>=30000);
}

int CComm::GetBool(bool &d) {
  char Command[200];
  DWORD timeout=GetTickCount();
  while ((!GetCommand(Command)) && (GetTickCount()-timeout<30000)) Sleep(10);
  d=(strcmp(Command,"TRUE")==0);
  return (GetTickCount()-timeout>=30000);
}

int CComm::GetString(char *Command) {
  //char Command[10000];
  DWORD timeout=GetTickCount();
  while ((!GetCommand(Command)) && (GetTickCount()-timeout<30000)) Sleep(10);
  return (GetTickCount()-timeout>=30000);
}

void CComm::CloseConnection() {
}

void CComm::DebugOn(char* DebugFilename, bool Append) {
}
