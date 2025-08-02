#include "network.h"
#include "stdio.h"
#include "LeakDetect.h"

const int MAXDATABLOCKSIZE=4;

//*** CNetwork ***
DEFINE_RESPONSE_TABLE1(CNetwork, TWindow)
  EV_MESSAGE(MSG_SOCKET_NOTIFY, DoSocketNotification),
  EV_MESSAGE(MSG_HOST_INFO_NOTIFY, DoHostNotification),
END_RESPONSE_TABLE;

CNetwork::CNetwork(TWindow *aparent,char *aIPAdresse, ushort aport, bool aOpenAsServer, bool aAppendCRLFToCommand, bool aAppendSynchronization,TMessageBar *amb, bool *aCloseVisionFlag):TWindow(aparent,0,0) {
  CloseVisionFlag=aCloseVisionFlag;
  mb=amb;
  DebugFile=NULL;
  AppendCRLFToCommand=aAppendCRLFToCommand;
  AppendSynchronization=aAppendSynchronization;
  parent=aparent;
  OpenAsServer=aOpenAsServer;
  networkdata=false;
  strcpy(IPAdresse,aIPAdresse);
  port=aport;
  myStreamSocket=NULL;
  myListeningSocket=NULL;
  Open();
  SyncMode=false;

}

void CNetwork::DebugOn(char* DebugFilename, bool Append) {
  if (DebugFile) DebugOff();
	if (Append) DebugFile=new ofstream(DebugFilename, ios::app);
   else DebugFile=new ofstream(DebugFilename);
   #if defined(DetectLeaks)
     LeakDetect.New(DebugFile,1,1);
   #endif
}

void CNetwork::DebugOff() {
	if (DebugFile) {
     DebugFile->close();
     #if defined(DetectLeaks)
       LeakDetect.Delete(DebugFile);
     #endif
     delete DebugFile;
   }
   DebugFile=NULL;
}

void CNetwork::DebugWrite(char* String) {
  if (DebugFile) (*DebugFile)<<String<<endl;
}

int CNetwork::Sync() {
  SyncMode=true;
  int nError = myStreamSocket->CancelNotification();
  if (nError == WINSOCK_ERROR) {
    MessageBox(TSocketError(TWinSock::Dll()->WSAGetLastError()).GetReasonString(),"Error on CancelNotification() in ConnectWithThis().",  MB_OK);
    myPresentState=nIdle;
    return 0;
  }
  return 1;
}

int CNetwork::Async() {
  SyncMode=false;
  int nError = myStreamSocket->StartRegularNotification();
  if (nError == WINSOCK_ERROR) {
    MessageBox(TSocketError(TWinSock::Dll()->WSAGetLastError()).GetReasonString(),"Error on StartRegularNotification() in ConnectWithThis().",  MB_OK);
    myPresentState=nIdle;
    return 0;
  }
  return 1;
}

void CNetwork::Open() {
  int nError;
  HANDLE  hHostRequest;
  if (OpenAsServer) {
    TINetSocketAddress ourSocketAddress(0, INADDR_ANY);  //same as 'ourSocketAddress()'
    myPresentState = nIdle;
    myListeningSocket = new TStreamSocket(ourSocketAddress);
    #if defined(DetectLeaks)
      LeakDetect.New(myListeningSocket,1,2);
    #endif
    myListeningSocket->SetNotificationWindow(this); //redirect socket FD_XXX notifications to us.
    nError = myListeningSocket->CreateSocket(); //similar to 'socket()'
    if (nError == WINSOCK_ERROR){ //could also say 'if (nError){'
      MessageBox(TSocketError(myListeningSocket->GetLastError()).GetReasonString(),"Error on CreateSocket()",  MB_OK);
      return;
    }
    myListeningSocket->SetNotificationWindow(this); //redirect socket FD_XXX notifications to us.
    
    ushort nPort = TWinSock::Dll()->htons(port);
    nError = myListeningSocket->SetReuseAddressOption(true);
    if (nError == WINSOCK_ERROR){ //could also say 'if (nError){'
      MessageBox(TSocketError(myListeningSocket->GetLastError()).GetReasonString(),"Error on SetReuseAddressOption()",  MB_OK);
      return;
    }
    nError = myListeningSocket->BindSocket(TINetSocketAddress(nPort, INADDR_ANY)); //similar to 'bind()'
    if (nError == WINSOCK_ERROR){ //could also say 'if (nError){'
      MessageBox(TSocketError(myListeningSocket->GetLastError()).GetReasonString(),"Error on BindSocket()", MB_OK);
      return;
    }
    nError = myListeningSocket->Listen();
    if (nError == WINSOCK_ERROR){ //could also say 'if (nError){'
      MessageBox(TSocketError(myListeningSocket->GetLastError()).GetReasonString(),"Error on Listen()",  MB_OK);
      return;
    }
    nError = myListeningSocket->StartAcceptNotification();  //  must come last
    if (nError == WINSOCK_ERROR) {
      MessageBox(TSocketError(myListeningSocket->GetLastError()).GetReasonString(),"Error on StartAcceptNotification()",  MB_OK);
      return;
    }
    myPresentState=nListening;
    if (mb) mb->SetText("Listening");
    if (DebugFile) (*DebugFile) << "Listening socket setup done" << endl;
  } else {
    myPresentState = nIdle;
    TINetSocketAddress ourSocketAddress; //same as INetSocketAddress(0, INADDR_ANY);
    myStreamSocket = new TStreamSocket(ourSocketAddress);
    #if defined(DetectLeaks)
      LeakDetect.New(myStreamSocket,1,3);
    #endif
    myStreamSocket->SetNotificationWindow(this);
    ushort nPortEnteredByUser = TWinSock::Dll()->htons(port);  //need an 'atous()' function.
    sAddressToConnectTo.SetPort(nPortEnteredByUser);

    //Get actual address entered by user.
    short bAddressIsDottedDecimal = sAddressToConnectTo.IsAddressDottedDecimal(IPAdresse);
    if (bAddressIsDottedDecimal) {
      sAddressToConnectTo.SetNetworkAddress(sAddressToConnectTo.ConvertAddress(IPAdresse));
      //We can immediately move on to the 'nConnecting' state.
      myPresentState = nConnecting;
      Connect();
    }
     else{
      nError = myHostInfoManager.GetHostInfoAsync(*this, hHostRequest, IPAdresse);
      if (nError == WINSOCK_ERROR) {
        MessageBox(TSocketError(myHostInfoManager.GetLastError()).GetReasonString(),"Unable to convert the host name to an address.",  MB_OK);
      } else myPresentState = nWaitingForAddress;
    }
  }
}

CNetwork::~CNetwork() {
  if (DebugFile) DebugOff();
  if (myStreamSocket) {
  
  #if defined(DetectLeaks)
    LeakDetect.Delete(myStreamSocket);
  #endif

    myStreamSocket->CloseSocket();
    delete myStreamSocket;
    myStreamSocket = NULL;
  }

  if (myListeningSocket) {  
  #if defined(DetectLeaks)
    LeakDetect.Delete(myListeningSocket);
  #endif

    myListeningSocket->CloseSocket();
    delete myListeningSocket;
    myListeningSocket = NULL;
  }
}

void CNetwork::Connect()
{
  //First create the socket.
  int nError = myStreamSocket->CreateSocket(); //similar to 'socket()'
  if (nError == WINSOCK_ERROR) { //could also say 'if (nError) {'
    MessageBox(TSocketError(myStreamSocket->GetLastError()).GetReasonString(),"Error on CreateSocket()",  MB_OK);
    myPresentState = nIdle;
    return;
  }

  //The StartRegularNotification() function converts socket into non-blocking socket.
  myStreamSocket->StartRegularNotification();

  //Bind is technically not really needed, as a connect() will bind for you:
  nError = myStreamSocket->BindSocket();
  if (nError == WINSOCK_ERROR) { //could also say 'if (nError) {'
    MessageBox(TSocketError(myStreamSocket->GetLastError()).GetReasonString(),"Error on BindSocket()",  MB_OK);
    myPresentState = nIdle;
    return;
  }

  nError = myStreamSocket->Connect(sAddressToConnectTo);
  if (nError == WINSOCK_ERROR) {
    if (myStreamSocket->GetLastError() != WSAEWOULDBLOCK) {
      MessageBox(TSocketError(myStreamSocket->GetLastError()).GetReasonString(),"Error on Connect()",  MB_OK);
      myPresentState = nIdle;
      return;
    }
    myPresentState = nConnected; //trick it out so that FD_CONNECT is not necessary
    //Else, we will soon get asynchronously notified of a completed connection with FD_CONNECT.
  }
}

LRESULT CNetwork::DoSocketNotification(WPARAM, LPARAM lParam)
{
  int  nEvent = (int)WSAGETSELECTEVENT(lParam);
  int  nError = (int)WSAGETSELECTERROR(lParam);
  if (myPresentState==nListening) {
    if (nEvent == FD_ACCEPT) {
      if (mb) mb->SetText("Accepting connection");
      if (nError)
        MessageBox(TSocketError(nError).GetReasonString(),"Error on receipt of FD_ACCEPT.",  MB_OK);
      else {
        TStreamSocket newStreamSocket;  //construct an empty StreamSocket object.
        newStreamSocket.SetSaveSocketOnDelete();  //Don't close the socket on deletion, b/c we're giving it to someone else.

        int nError = myListeningSocket->Accept(newStreamSocket); //Accept() will completely fix up nStreamSocket.
        if (nError == WINSOCK_ERROR){ //could also say: 'if (nError){'
          MessageBox(TSocketError(myListeningSocket->GetLastError()).GetReasonString(),"Error on Accept().",  MB_OK);
          return 0;
        }
        myPresentState=nIdle;

        char  szPort[12];

        if (myStreamSocket) {
          myStreamSocket->CloseSocket();
          delete myStreamSocket;
        }
        myStreamSocket = new TStreamSocket();
        *myStreamSocket = newStreamSocket;
        myStreamSocket->SetSaveSocketOnDelete(FALSE); //Now we own the socket descriptor.
        sAddressToConnectTo = newStreamSocket.PeerSocketAddress;

        // The 'newConnectedSocket' may or may not be set up for Notification, so
        //  we specifically set it here to make sure it is the way we want it:
        //
        myStreamSocket->SetNotificationWindow(this);
        int nError2 = myStreamSocket->StartRegularNotification();
        if (nError2 == WINSOCK_ERROR) {
          MessageBox(TSocketError(TWinSock::Dll()->WSAGetLastError()).GetReasonString(),"Error on StartRegularNotification() in ConnectWithThis().",  MB_OK);
          myPresentState=nIdle;
          return 0;
        }

        // Equivalent Winsock code:
        //
        nError2 = TWinSock::Dll()->WSAAsyncSelect(*myStreamSocket, *this, MSG_SOCKET_NOTIFY, FD_READ | FD_WRITE | FD_CLOSE | FD_CONNECT | FD_OOB);
        if (nError2) {
          MessageBox(TSocketError(TWinSock::Dll()->WSAGetLastError()).GetReasonString(),"Error on WSASelect()",  MB_OK);
          myPresentState=nIdle;
          return 0;
        }

        char far* szIPAddress;
        szIPAddress = sAddressToConnectTo.ConvertAddress(sAddressToConnectTo.GetNetworkAddress());
        myPresentState = nConnected;
        //MessageBox("Connected", "Error", MB_OK);
        return 1;
      }
    } else {
      //MessageBox("Listening socket received a non-FD_ACCEPT FD_ notification", "Suspicious...", MB_OK);
    }
  } else {
    if (nEvent == FD_WRITE /*&& !bDataSent*/) {
      if (nError)
         MessageBox(TSocketError(nError).GetReasonString(),"Error on receipt of FD_WRITE.", MB_OK);
       else;
       //  SendData();
    } else if (nEvent == FD_READ) {
      if (nError)
        MessageBox(TSocketError(nError).GetReasonString(),"Error on receipt of FD_READ.", MB_OK);
      else {
        if (!SyncMode) parent->PostMessage(IDM_NETWORKREAD);
        networkdata=true;
      }
//        ReadData();
    } else if (nEvent == FD_CONNECT && myPresentState == nConnecting) {
      if (nError) {
        MessageBox(TSocketError(nError).GetReasonString(),"Error on receipt of FD_CONNECT.", MB_OK);
        myPresentState = nIdle;
      } else {
         myPresentState = nConnected;
         if (mb) mb->SetText("Connected to control");
         //MessageBox("Connected2", "Error", MB_OK);
      }
    } else if (nEvent == FD_CLOSE) {
      if ((nError==10054) || (nError==0)) {  //Connection reset by peer Or simply Connection closed
        if (OpenAsServer) {
          /*nError = myStreamSocket->ShutDownSocket();
          if (nError == WINSOCK_ERROR){
            MessageBox(TSocketError(myStreamSocket->GetLastError()).GetReasonString(),"Error on CloseSocket()",  MB_OK);
          } */
          //MessageBox("CNetwork::DoSocketNotification : Connection closed by client, closing Vision.", "Error", MB_OK);
          //if (CloseVisionFlag) *CloseVisionFlag=true;
          if (DebugFile) DebugOff();
			 myStreamSocket->CloseSocket();

          #if defined(DetectLeaks)
            LeakDetect.Delete(myStreamSocket);
          #endif
          delete myStreamSocket;
          myStreamSocket = NULL;
          networkdata=false;
          //Open();
          SyncMode=false;

          /*nError = myStreamSocket->Listen();
           if (nError == WINSOCK_ERROR){ //could also say 'if (nError){'
       	     MessageBox(TSocketError(myStreamSocket->GetLastError()).GetReasonString(),"Error on Listen()",  MB_OK);
           }
       	  nError = myStreamSocket->StartAcceptNotification();
       	  if (nError == WINSOCK_ERROR) {
       	    MessageBox(TSocketError(TWinSock::Dll()->WSAGetLastError()).GetReasonString(),"Error on StartAcceptNotification() in ConnectWithThis().",  MB_OK);
      	     myPresentState=nIdle;
     	       return 0;
       	  }
     		  myStreamSocket->SetNotificationWindow(this); //redirect socket FD_XXX notifications to us.
			  ushort nPort = TWinSock::Dll()->htons(port);
			  nError = myStreamSocket->SetReuseAddressOption(true);*/
          
          myPresentState=nListening;
          if (mb) mb->SetText("Connection lost. Listening...");
        } else {
          myPresentState = nConnectionLost;
        }
      } else if (nError) {
        MessageBox(TSocketError(nError).GetReasonString(),"FD_Close",MB_OK);
        myPresentState = nIdle;
      }
    }
  }
 return 1;
}

TResult CNetwork::DoHostNotification(TParam1, TParam2 param2)
{
  int  nError = WSAGETASYNCERROR(param2);

  if (myPresentState != nWaitingForAddress)
    return 1;

  if (nError) {
    MessageBox(TSocketError(nError).GetReasonString(),"Error on looking up address.",  MB_OK);
    myPresentState=nIdle;
    if (mb) mb->SetText("Idle");
  }
  else{
    sAddressToConnectTo.SetNetworkAddress(myHostInfoManager.HostEntry->GetINetAddress());
    myPresentState = nConnecting;

    Connect(); //Tries to connect to the given address.
    if (mb) mb->SetText("Connect");
  }
  return 1;
}

bool CNetwork::ReadData(char* data, int& charsToRead) {
  //the following three lines were excluded in Austin
 // if (myPresentState!=nConnected) {
//   	Open();
//  }

  if (!DataAvailable()) return false;
  if (myPresentState==nConnected) {
    char buf[2];
    int  nError;
    int l=1;
    int read=0;
    unsigned long StartTime=GetTickCount();
    do {
      l=1;
      nError = myStreamSocket->Read(buf, l);
      if (nError == WINSOCK_ERROR) {
        MessageBox(TSocketError(myStreamSocket->GetLastError()).GetReasonString(),"CNetwork::ReadData : Error trying to read data.", MB_OK);
        return false;
      }
      if ((l!=0) && (buf[0]!='*')) {
        if (DebugFile) {
              buf[l]=0;
          (*DebugFile)<<GetTickCount()<<" << BadData0:"<<buf<<endl;
            //DebugFile->flush();
              l=0;
        }
      }
      if (l==0) Sleep(10);
    } while ((l==0) && (buf[0]!='*') && (abs(GetTickCount()-StartTime)< READDATATIMEOUT));
    if (((GetTickCount()-StartTime)>= READDATATIMEOUT)) MessageBox("CNetwork::ReadData : Wait too long ReadData","Error",  MB_OK);
    if (buf[0]!='*') {
      MessageBox("CNetwork::ReadData : no *","Error trying to read data.", MB_OK);
      return false;
    }
    do {
      int  nError;
      int thisread=1;
      nError = myStreamSocket->Read(&data[read], thisread);
      if (nError == WINSOCK_ERROR) {
        MessageBox(TSocketError(myStreamSocket->GetLastError()).GetReasonString(),"CNetwork::ReadData : Error trying to read data.", MB_OK);
        return false;
      }
      read+=thisread;
    } while ((data[read-1]!='#') && (read<charsToRead) && (abs(GetTickCount()-StartTime)< READDATATIMEOUT));
    if (((GetTickCount()-StartTime)>= READDATATIMEOUT)) MessageBox("Wait too long ReadData2","Error",  MB_OK);
    if (data[read-1]!='#') {
      MessageBox("CNetwork::ReadData : no #","Error trying to read data.", MB_OK);
      return false;
    }
    char null=0;
    data[read-1]=null;
    read--;
    networkdata=false;
    charsToRead=read;
  } else {
    MessageBox("CNetwork::ReadData : not connected", "", MB_OK);
    return false;
  }
  if (DebugFile) {
    (*DebugFile)<<GetTickCount()<<" << "<<data<<endl;
    DebugFile->flush();
  }
  return true;
}

bool CNetwork::WriteData(char* data, int& charsToWrite, bool DisplayErrorMessage) {
  if (myPresentState!=nConnected) {
   	Open();
  }
  if (myPresentState==nConnected) {
    if (DebugFile) {
     (*DebugFile)<<GetTickCount()<<" >> "<<data<<endl;
     DebugFile->flush();
   }
  	 bool AddSynchronization=(data[0]!='*')/* && AddSynchronization)*/;
    char totalbuf[200];
    char totalbuf2[200];
    if (AddSynchronization) sprintf(totalbuf,"*%s#",data);
    else sprintf(totalbuf,"%s",data);
    if (AppendCRLFToCommand) sprintf(totalbuf2,"%s%c%c",totalbuf,char(13),char(10));
    else sprintf(totalbuf2,"%s",totalbuf);
    int nError;
    nError = myStreamSocket->Write(totalbuf2,strlen(totalbuf2));
    if (nError == WINSOCK_ERROR) {
      if (myStreamSocket->GetLastError() != WSAEWOULDBLOCK)
        //bDataSent = TRUE;
        if (DisplayErrorMessage) MessageBox(TSocketError(myStreamSocket->GetLastError()).GetReasonString(),"CNetwork::WriteData : Error trying to write data.", MB_OK);
      return false;
    }
//    bDataSent = TRUE;
  } else {
    if (DisplayErrorMessage) MessageBox("CNetwork::WriteData : not connected", "", MB_OK);
    return false;
  }
  return true;
}

bool CNetwork::WriteCommand(char* command, bool DisplayErrorMessage) {
//  bool OldSyncMode=SyncMode;
//  Sync();
//  if (networkdata) MessageBox("unwanted network data","Error", MB_OK);
 return WriteData(command,strlen(command),DisplayErrorMessage);
/*  unsigned long StartTime=GetTickCount();
  while ((myStreamSocket->GetTotalWaitingSize()==0) && (abs(GetTickCount()-StartTime)<10000)) ;
  if (((GetTickCount()-StartTime)>=10000)) MessageBox("Wait too long WriteCommand","Error",  MB_OK);
  if ((myStreamSocket->GetTotalWaitingSize()==0)) return false; */
/*  char buf[10];
  int length=10;
  ReadData(buf,length);
  if (length!=5) return false;
  buf[5]=0;
  if (strcmp(buf,"Ready")!=0) return false;
  SyncMode=OldSyncMode;
  return true;*/
}

bool CNetwork::WriteReady() {
  int h=5;
  WriteData("Ready",h);
  return (h==5);
}

bool CNetwork::CheckReady(unsigned long timeout) {
  unsigned long StartTime=GetTickCount();
  while ((myStreamSocket->GetTotalWaitingSize()==0) && (abs(GetTickCount()-StartTime)<timeout)) Sleep(10);
  if ((GetTickCount()-StartTime)>=timeout) {
  //  WriteCommand("ErrorCheckReadyTimeout");
  //  MessageBox("Network : CheckReady : Timeout","Error",  MB_OK);
    return false;
  }
  char buf[10];
  int length=10;
  ReadData(buf,length);
  if (length!=5) {
    MessageBox("peer not ready1","Error",  MB_OK);
    return false;
  }
  buf[5]=0;
  if (strcmp(buf,"Ready")!=0) {
    MessageBox("peer not ready","Error",  MB_OK);
    return false;
  }
  return true;
}

bool CNetwork::WriteInt(int i) {
  char buf[200];
  sprintf(buf,"%04i",i);
  return WriteData(buf,strlen(buf));
}

bool CNetwork::WriteLong(long i) {
  char buf[200];
  sprintf(buf,"%i",i);
  return WriteData(buf,strlen(buf));
}

bool CNetwork::WriteDouble(double i) {
  char buf[200];
  sprintf(buf,"%e",i);
  return WriteData(buf,strlen(buf));
}

bool CNetwork::ReadChar(char *c,int &len) {
  unsigned long StartTime=GetTickCount();
  while ((myStreamSocket->GetTotalWaitingSize()==0) && (abs(GetTickCount()-StartTime)< READDATATIMEOUT)) Sleep(10);
  if (((GetTickCount()-StartTime)>= READDATATIMEOUT)) MessageBox("CNetwork::ReadChar : Wait too long ReadChar","Error",  MB_OK);
  if (myStreamSocket->GetTotalWaitingSize()==0) return false;
  return ReadData(c,len);
//  return WriteReady();
}

bool CNetwork::ReadDouble(double &d) {
  char buf[100];
  int help=99;
  if (ReadChar(buf,help)) {
    buf[help]=0;
    d=atof(buf);
    return true;
  } else return false;
}

bool CNetwork::ReadInt(int &i) {
  char buf[100];
  int help=99;
  if (ReadChar(buf,help)) {
    buf[help]=0;
    i=atoi(buf);
    return true;
  } else return false;
}

bool CNetwork::ReadLong(long &l) {
  char buf[100];
  int help=99;
  if (ReadChar(buf,help)) {
    buf[help]=0;
    if (DebugFile) {
     (*DebugFile)<<GetTickCount()<<" >> ReadLong "<<buf<<endl;
     DebugFile->flush();
   }
    l=atol(buf);
    return true;
  } else return false;
}

bool CNetwork::WriteHugeData(char *data, long length,long timeout) {
  if (DebugFile) {
     (*DebugFile)<<GetTickCount()<<" >> WriteHugeData "<<length<<" Bytes"<<endl;
     DebugFile->flush();
   }
  if (WriteLong(length)) {
    long datawritten=0;
    unsigned long StartTime=GetTickCount();
    while ((datawritten<length)  && (abs(GetTickCount()-StartTime)<timeout)) {
      int writelength;
      if ((length-datawritten)>16384) writelength=16384; else writelength=length-datawritten;
      int nError=myStreamSocket->Write(&data[datawritten],writelength);
      if (nError == WINSOCK_ERROR) {
        MessageBox(TSocketError(myStreamSocket->GetLastError()).GetReasonString(),"Error trying to write data.", MB_OK);
        return false;
      }
      datawritten+=writelength;
    }
    if (datawritten<length) return false;
    else return true;
  } return false;
/*  int n;
    int writelength;
    for (n=0;n<(length | MAXDATABLOCKSIZE);n++) {
      writelength=MAXDATABLOCKSIZE;
      int nError=myStreamSocket->Write(&data[n*MAXDATABLOCKSIZE],writelength);
      if (nError == WINSOCK_ERROR) {
        MessageBox(TSocketError(myStreamSocket->GetLastError()).GetReasonString(),"Error trying to write data.", MB_OK);
        return false;
      }
      if (writelength!=MAXDATABLOCKSIZE) {
        MessageBox("WriteHugeData could not write","Error",  MB_OK);
        return false;
      }
    }
    int rest=(length-(length | MAXDATABLOCKSIZE)*MAXDATABLOCKSIZE);
    if (rest>0) {
      int writelength=rest;
      int nError=myStreamSocket->Write(&data[n*MAXDATABLOCKSIZE],writelength);
      if (nError == WINSOCK_ERROR) {
        MessageBox(TSocketError(myStreamSocket->GetLastError()).GetReasonString(),"Error trying to write data.", MB_OK);
        return false;
      }
      if (writelength!=rest) {
        MessageBox("WriteHugeData could not write 2","Error",  MB_OK);
        return false;
      }
    }
    return true;
  }
  return false; */
}

bool CNetwork::ReadHugeData(char *data, long& alength,long timeout) {
  //WriteReady();
  unsigned long StartTime=GetTickCount();
  while ((myStreamSocket->GetTotalWaitingSize()==0) && (abs(GetTickCount()-StartTime)< READHUGEDATATIMEOUT)) Sleep(10);
  if (((GetTickCount()-StartTime)>= READHUGEDATATIMEOUT)) MessageBox("Wait too long ReadHugeData","Error",  MB_OK);
  long length=alength;
  long LengthToRead;
  if (!ReadLong(LengthToRead)) {
	  	MessageBox("CNetwork::ReadHugeData : block size not send","Error",  MB_OK);
      return false;
  }
 // char buf[200];
 // sprintf(buf,"expected=%i, to read=%i",length,LengthToRead);
 // MessageBox(buf,"Error",  MB_OK);
 //just for filming:
           //LengthToRead=length;
    if (LengthToRead!=length) {
      char buf[200];
      sprintf(buf,"CNetwork::ReadHugeData : too little or too much data (%i instead of %i)",LengthToRead,length);
      MessageBox(buf,"Error",  MB_OK);
      for (long i=0;i<alength;i++) data[i]=0;
      return false;
    }
           //   LengthToRead=length;
    if (DebugFile) {
     (*DebugFile)<<GetTickCount()<<" << ReadHugeData "<<alength<<"/"<<LengthToRead<<" Bytes "<<endl;
     DebugFile->flush();
   }
//    unsigned long StartTime=GetTickCount();
    long dataread=0;
    while ((dataread<LengthToRead)/* && (abs(GetTickCount()-StartTime)<timeout) */) {
      while ((myStreamSocket->GetTotalWaitingSize()==0)/* && (abs(GetTickCount()-StartTime)<timeout)*/) Sleep(10);
  //    if ((GetTickCount()-StartTime)>=timeout) {
   //     MessageBox("Network : CheckReady : Timeout","Error",  MB_OK);
    //    return false;
    //  }
      int toread=LengthToRead-dataread;
      int nError=myStreamSocket->Read(&data[dataread],toread);
      if (nError == WINSOCK_ERROR) {
        MessageBox(TSocketError(myStreamSocket->GetLastError()).GetReasonString(),"Error trying to read data.", MB_OK);
        return false;
      }
      dataread+=toread;
    }
 //   sprintf(buf,"expected=%i, to read=%i, read=%i",length,LengthToRead,dataread);
 //   MessageBox(buf,"Error",  MB_OK);
    alength=dataread;
//    Flush(100);
    /*int n;
    for (n=0;n<(length | MAXDATABLOCKSIZE);n++) {
      unsigned long StartTime=GetTickCount();
      while (!networkdata && (abs(GetTickCount()-StartTime)<10000)) ;
      if (((GetTickCount()-StartTime)>=10000)) MessageBox("Wait too long ReadHugeData","Error",  MB_OK);
      if (!networkdata) return false;
      int len=MAXDATABLOCKSIZE;
      int readlength=len*4;
      myStreamSocket->Read((char*)(data[n*MAXDATABLOCKSIZE]),readlength);
      if (readlength!=len*4) {
        MessageBox("ReadHugeData not enough data","Error",  MB_OK);
        return false;
      }
    }
    int rest=(length-(length | MAXDATABLOCKSIZE)*MAXDATABLOCKSIZE);
    unsigned long StartTime=GetTickCount();
    while (!networkdata && (abs(GetTickCount()-StartTime)<10000)) ;
    if (((GetTickCount()-StartTime)>=10000)) MessageBox("Wait too long ReadHugeData2","Error",  MB_OK);
    if (!networkdata) return false;
    int len=rest;
    int readlength=len*4;
    if (rest>0) myStreamSocket->Read((char*)(data[n*MAXDATABLOCKSIZE]),readlength);
    if (readlength!=len*4) {
      MessageBox("ReadHugeData not enough data 2","Error",  MB_OK);
      return false;
    } */
//  }
}

bool CNetwork::DataAvailable() {
if (myStreamSocket) return myStreamSocket->GetTotalWaitingSize()>0;
else return 0;
}

void CNetwork::Flush(long time) {
   char buf[200];
   while (DataAvailable()) {
     int l=100;
      myStreamSocket->Read(buf,l);
      if ((l!=0) && (DebugFile)) {
         buf[l]=0;
    	  	(*DebugFile)<<GetTickCount()<<" << BadData Flush:"<<buf<<endl;
         DebugFile->flush();
      }
   }
   if (time==0) return;
   unsigned long StartTime=GetTickCount();
   while (abs(GetTickCount()-StartTime)<time) {
	   if (DataAvailable()) {
      	StartTime=GetTickCount();
         int l=100;
         myStreamSocket->Read(buf,l);
         if ((l!=0) && (DebugFile)) {
           buf[l]=0;
    	  	  (*DebugFile)<<GetTickCount()<<" << BadData Flush2:"<<buf<<endl;
           DebugFile->flush();
        }
        if (l==0) {
          Sleep(10);
        }
      }
   }
}

void CNetwork::CloseConnection() {

}
