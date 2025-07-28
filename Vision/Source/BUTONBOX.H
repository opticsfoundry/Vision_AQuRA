#if !defined(__butonbox_h)
#define __butonbox_h

#include <groupbox.h>
#include <checkbox.h>
#include <radiobut.h>
#include <static.h>

//*** CButtonBox ***
class CButtonBox : public TGroupBox {
  int nrblocks;
  int listlength;
  int length;
  TRadioButton **RButton;
  TCheckBox **CButton;
  bool radio;
  char **text;
  int *nrlist;
  bool *checked;
  int aktblock;
  TStatic **Texte;
public:
  CButtonBox::CButtonBox(TWindow* aparent,int ID,int x, int y, int w, int h,int alength,
    int anrblocks, int alistlength,char **atext,int *anrlist, bool aradio);
  ~CButtonBox();
  void SetCheck(int anr, bool value);
  unsigned long GetChecked();
  void SetChecked(unsigned long Checked);
  void Next();
  void Last();
  void UpdateChecked();
  void GetCheckedAsString(char* buf);
  void SetCheckedFromString(char* buf);
  void SaveCheckedToStream(ofstream& out);
  void LoadCheckedFromStream(ifstream& in);
};

#endif