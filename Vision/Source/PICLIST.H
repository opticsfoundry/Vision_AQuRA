#if !defined(__piclist_h)
#define __piclist_h

#include "colortab.h"
#include "picture.h"
#include "picturew.h"
#include "static.h"

//*** CPictureList ***
class CPictureList {
  CPictureButton** PictureWin;
  CPicture** Pictures;
  TStatic** TextList;
  TStatic** TitleList;
  char** NameList;
  int PicNrX,PicNrY,PicNr,AktPicNr,OldAkt;
  CPictureList **SyncList;
  int NrPictureLists;
public:
  CPictureList(TWindow* aparent,TMessageBar *amb,int StartID,
    int x, int y, int w, int h,int th,int nrX,CPicture *prototyp,
    CColorTable *colortable,CPictureList **aPictureList,int aNrPictureLists);
  ~CPictureList();
  void AddPicture(CPicture *picture, const char *name,
		const char *AtomName, const char *CameraDirection, bool NewLine);
  void SynchronizePosition();
  int GetPosition();
  void SetPosition(double aPosition);
  void SetMarker(double aPosition);
  const char* GetName(LPARAM nr);
  void Update();
  void ClearAktPos();
};

#endif