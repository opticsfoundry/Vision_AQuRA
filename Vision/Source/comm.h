#if !defined(__comm_h)
#define __comm_h

//*** CComm ***
class CComm {
public:
  CComm();
  ~CComm();
  virtual int GetCommand(char *string);
  virtual void SendCommand(char *string);
  virtual void Clear();
  virtual int GetDouble(double &d);
  virtual int GetBool(bool &d);
  virtual int GetInt(int &i);
  virtual int GetString(char *c);
  virtual int GetChar(char &c);
  virtual void CloseConnection();
  virtual void DebugOn(char* DebugFilename, bool Append);
};

#endif
