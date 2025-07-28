#if !defined(__serial_h)
#define __serial_h

#include <windows.h>
#include "Comm.h"

//*** CSerial ***
class CSerial : public CComm {
private:
  DCB dcb;
  HANDLE ComHandle;
public:
  CSerial();
  ~CSerial();
  int GetCommand(char *string);
  void SendCommand(char *string);
  virtual void CloseConnection();
private:
  int ReadComm(char *ein);
};

#endif
