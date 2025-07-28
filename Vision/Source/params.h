#if !defined(__params_h)
#define __params_h

#include <fstream.h>
#include <edit.h>

//*** CParamList ***
class CParamList {
  char *Name;
  char *Value;
  char *OldValue;
  char Type;
  bool changed;
  CParamList *next;
public:
  CParamList(CParamList *anext,const char *aName, const char *aValue, char aType);
  ~CParamList();
  CParamList *GetParam(const char *aName);
  void SetValue(const char* aValue);
  void Save(ofstream &out, bool SaveAll, bool Update, bool Distance,TEdit *LogbookEdit, bool FirstLine, bool SkipUnused, bool Skipping);
  void Update();

   // added following functions to try to fix vision crash bug shayne 25/3/17

  void SingleUpdate();
  char *GetName();
  char *GetValue();
  char *GetOldValue();
  char GetType();
  bool Getchanged();
  CParamList *GetNext();

};

#endif
