#if !defined(__point_h)
#define __point_h

#include <fstream.h>
#include <dc.h>
#include <string.h>

//*** CMyPoint ***
class CMyPoint {
public:
	CMyPoint *hnext;
  int NrX;
  int NrY;
  int NrText;

  char **XName;
  char **YName;
  char **TextName;

  char **Text;
  double *x;
  double *yb;

  CMyPoint *next;
  bool enabled;
  bool active;
  char code[10];
  int PicNr;
  int mycolor;

  bool InList;
  CMyPoint **metapointer;

public:
  CMyPoint(int aNrX, int aNrY, int aNrText, char *aXName[], char *aYName[], char *aTextName[]);
  CMyPoint(ifstream *in,int &err);
  CMyPoint(CMyPoint *last);
  ~CMyPoint();
  void Init(int aNrX, int aNrY, int aNrText, char **aXName, char **aYName, char **aTextName);
  void Copy(CMyPoint *source);
  void CopyData(CMyPoint *source);
  double & operator[] (const char *);
  void SetNewXNames(int aNrX, char **aXName);
  int LoadPoint(ifstream *in,int Nr=-1);
  void GetData(char PicFileName);
  void SetColor(int acolor);
  void SetThisColor(int color);
  void SetPicNr(int aPicNr) {PicNr=aPicNr;}
  void SetCode(const char *aCode)  {strcpy(code,aCode);}
  void SetX(char *name, double value);
  void SetY(char *name, double value);
  void SetText(char *name,const char *text);
  double GetX(char *name);
  double GetXNr(int Nr);
  double GetY(char *name);
  const char *GetText(char *name);
  const char *GetXName(int Nr);
  int GetNrX() {return NrX;}
  void SaveHeader(ofstream &out);
  void SavePoints(ofstream &out);
  bool IsInList() {return InList;}
  CMyPoint* PutIntoList(CMyPoint* lastpoint);
  void Draw(TClientDC *dc,double xmin,double xmax,double ymin,double ymax,int W,int H,unsigned long what, int Selection);
  void GetMinMaxXY(double &xmin, double &ymin,double &xmax, double &ymax,unsigned long what, int Selection);
  void GetMax(double &value, int aktwhat, int Selection);
  void GetMin(double &value, int aktwhat, int Selection);
  int number(int Selection);
  void GetXY(double *x,double *y,int aktwhat, int pos,int Selection);
  int PointNr(unsigned long what);
  int What(unsigned long what,int nr);
  bool Displayed(int nr);
  void ToggleEnable();
  int Enabled() {return enabled;};
  void SetEnabled(bool aenabled) {enabled=aenabled;};
  void SetActive(int a);
  CMyPoint *GetNearestPoint(double xspan, double yspan, double ax, double ay,unsigned long what,int Selection,double &h);
  char *GetFilename(int nr=0);
  char *GetCode() {return code;}
  void Save(ofstream &out);
  void DeletePoint(CMyPoint *point);
  int operator<(const CMyPoint& b);
  friend void Sort(CMyPoint **liste);
  void ShiftParameters();
  void SetAllX(double *value);
  double y(int i);
  double &y(const char*);
  const char *GetTextNr(int i);
  void AddDefaultValues(CMyPoint *DefaultPoint);
  void OrderParameters(CMyPoint *DefaultPoint);
  CMyPoint *GetLastPoint(CMyPoint *FirstPoint);
  void SetMetaPointer(CMyPoint **metapointer);
  void GetMaxColor(int &color);    
private:
  int GetXNrFromName(char *name);
  int GetYNrFromName(char *name);
  int GetTextNrFromName(char *name);
  void AddXValue(char *name, double value);
  void AddYValue(char *name, double value);
  void AddTextValue(char *name, char *atext);
};

void Sort(CMyPoint *liste);

#endif
