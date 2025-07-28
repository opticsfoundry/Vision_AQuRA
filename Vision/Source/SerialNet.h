#if !defined(__SerialNet_h)
#define __SerialNet_h

#include "Network.h"
#include "Comm.h"

//*** CSerialNet ***
class CSerialNet : public CComm {
private:
CNetwork* Network;
public:
  CSerialNet(TWindow *parent,ushort port,  TMessageBar *amb=NULL,bool *CloseVisionFlag=NULL);
  ~CSerialNet();
  virtual int GetCommand(char *string);
  virtual void SendCommand(char *string);
  virtual void DebugOn(char* DebugFilename, bool Append);
};

#endif
