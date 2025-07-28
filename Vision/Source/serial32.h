#if !defined(__serial_h)
#define __serial_h

#include <windows.h>

//*** CSerial ***
class CSerial {
private:
  DCB dcb;
  HANDLE ComHandle;
public:
  CSerial();
  ~CSerial();
  int GetCommand(char *string);
  void SendCommand(char *string);
  void Clear();
  int GetDouble(double &d);
  int GetInt(int &i);
  int GetString(char *c);
  int GetChar(char &c);
private:
  int ReadComm(char *ein);
};

#endif
