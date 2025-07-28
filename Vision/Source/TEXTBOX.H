#if !defined(__textbox_h)
#define __textbox_h

#include <window.h>
#include <static.h>

//*** CTextBox ***
class CTextBox : public TWindow {
  int nx,ny,ntot;
  char **text;
  TStatic **Texte;
  bool visible;
public:
  CTextBox(TWindow* aparent,int x, int y, int w, int h,int dx,int dy,const char *title,int anx, int any,
	int antot,char **atext,bool avisible=true);
  ~CTextBox();
  void SetText(int nr, char* atext);
  void SetValue(int nr, double value);
  void Paint(TDC& adc, bool a, TRect& arect);
  void Visible() {visible=true;}
  void UnVisible() {visible=false;}
};

#endif