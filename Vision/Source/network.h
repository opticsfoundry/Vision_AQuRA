#if !defined(NETWORK_H)
#define NETWORK_H

#include <window.h>
#include <winsock.h>
#include <wskaddr.h>
#include <wskhostm.h>
#include <MESSAGEB.H>

#define IDM_NETWORKREAD 211
#define READHUGEDATATIMEOUT 100000
#define READDATATIMEOUT 10000

class CNetwork : public TWindow
{
 public:
   TMessageBar *mb;
   ofstream *DebugFile;
   TINetSocketAddress sAddressToConnectTo;
   THostInfoManager   myHostInfoManager;
   bool               OpenAsServer;
   bool SyncMode;
   bool networkdata;
   char IPAdresse[200];
   ushort port;
   TWindow *parent;

    enum          nState{nIdle, nListening, nWaitingForAddress, nConnecting, nConnected,nConnectionLost} myPresentState;
    TStreamSocket* myStreamSocket;
    bool IsServer;
    bool AppendCRLFToCommand;
    bool AppendSynchronization;
    bool *CloseVisionFlag;
 public:
    CNetwork(TWindow *parent,char *IPAdresse, ushort aport, bool OpenAsListener, bool aAppendCRLFToCommand=false, bool aAppendSynchronization=true,  TMessageBar *amb=NULL, bool *aCloseVisionFlag=NULL);
   ~CNetwork();

   void DebugOn(char* DebugFilename, bool Append);
   void DebugOff();
   void DebugWrite(char* String);
   int Sync();
   int Async();

   bool IsConnected() {return myPresentState==nConnected;}
   bool NetworkData() { return networkdata; }
   bool DataAvailable();
  bool ReadData(char* data, int& charsToRead);
  bool WriteData(char* data, int& charsToWrite, bool DisplayErrorMessage=true);
  bool WriteCommand(char* command, bool DisplayErrorMessage=true);
  bool WriteReady();
  bool CheckReady(unsigned long timeout=10000);
  bool WriteInt(int i);
  bool WriteLong(long i);
  bool WriteDouble(double i);
  bool ReadChar(char *c,int &len);
  bool ReadDouble(double &d);
  bool ReadInt(int &i);
  bool ReadLong(long &l);
  bool WriteHugeData(char *data, long length, long timeout= READHUGEDATATIMEOUT);
  bool ReadHugeData(char *data, long &alength,long timeout= READHUGEDATATIMEOUT);
   TResult DoHostNotification(TParam1, TParam2 param2);
   TResult DoSocketNotification(TParam1, TParam2 param2);
   void Flush(long time);
   virtual void CloseConnection();
private:
  void Open();
  void Connect();

  DECLARE_RESPONSE_TABLE(CNetwork);
};

#endif  // DLGLSTN_H
