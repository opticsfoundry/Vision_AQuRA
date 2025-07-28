#include "SerialNet.h"
#include <stdio.h>

CSerialNet::CSerialNet(TWindow *parent,ushort port,TMessageBar *amb,bool *CloseVisionFlag) {
	Network=new CNetwork(parent,"",port,true,false,true,amb,CloseVisionFlag);
}

CSerialNet::~CSerialNet() {
	delete Network;
}

int CSerialNet::GetCommand(char *string) {
	int charsToRead=10000;
   return ((Network->ReadData(string,charsToRead)) && (string[0]!=0));
}

void CSerialNet::SendCommand(char *string) {
  int WriteCount=strlen(string);
  Network->WriteData(string,WriteCount);
  if (WriteCount!=strlen(string)) {
    DWORD error=GetLastError();
    char aBuffer[100];
    sprintf(aBuffer,"Serial Write Error %u",error);
    MessageBox(GetActiveWindow(),aBuffer,"Error!",MB_OK);
  }
}

void CSerialNet::DebugOn(char* DebugFilename, bool Append) {
	Network->DebugOn(DebugFilename, Append);
}
