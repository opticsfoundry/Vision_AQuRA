#if !defined(__names_h)
#define __names_h
#include <date.h>
#include "rdsetup.h"

//*** CName ***
class CName {
public:
  CRDSetup *RDSetup;
  char dirname[16];
  char pfad[200];
  char daypfad[200];
  char FilmPfad[200];
  char SerialName[8];
  char BigSerialName[8];
  char aktname[200];
  int InSerie;
  int aktSerialNumber;
  int aktFilmNumber;
  int aktFilmPictureNumber;
  int aktPictureNumber;
  int SerialPictureNumber;
  unsigned short YoungestFileTime;
  int LiveSerie;
public:
  CName(CRDSetup *aRDSetup);
  ~CName() {};
  void Init();
  double GetFreeDiskSpace();
  void StopSerie();
  void ContinueSerie();
  void NewFilm();
  void GetFilmPictureName(char *name);
  void GetRegisterName(char *name,int aktcamera);
  int PictureNumber() {return aktPictureNumber;};
  int Serie() {return InSerie;};
  void NewSerie();
  void GetName(char *name, bool FirstImage, int CameraNumber);
  char *GetAktName() {return aktname;};
  char *GetDirName() {return dirname;};
  int GetSerialNumber() {return aktSerialNumber;};
  void GetParamName(char *name);
  void GetMessageName(char *name);
  void GetLogbookName(char *name);
  void GetImageLogbookName(char *name);
  void GetSerieName(char *name);
  void GetSerieRegisterName(char *name, int aktcamera);
  void GetPath(char *path);
 /* int CheckLivePicture(char *name, char *serialname,bool &NewPicture, bool &NewSerie,
    bool &StopSerie,char *Message); */
};

#endif