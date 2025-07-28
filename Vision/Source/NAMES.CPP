#include <messageb.h>
#include <date.h>
#include <dos.h>
#include <dir.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <fstream.h>
#include "names.h"
#include "setup.h"
#include "rdsetup.h"

CName::CName(CRDSetup *aRDSetup) {
  RDSetup=aRDSetup;
  Init();
  aktname[0]=0;
  aktPictureNumber=0;
  YoungestFileTime=0;
  LiveSerie=0;
}

void CName::Init() {
  struct dosdate_t datep;
  _dos_getdate(&datep);
  TDate *date=new TDate(datep.day,datep.month,datep.year);
  InSerie=0;
  strcpy(SerialName,"");
  strcpy(BigSerialName,"");
  strcpy(pfad,DataDirectory);
  setdisk(RDSetup->DataDisk);
  char buf[100];
  strncpy(buf,DataDirectory,4);
  buf[3]=0;
  chdir(buf);
  strcpy(buf,DataDirectory+3);
  char *help=strchr(buf,'\\');
  if (help) *help=0;
  mkdir(buf);
  chdir(buf);
  strcpy(buf,DataDirectory+4+strlen(buf));
  mkdir(buf);
  chdir(DataDirectory);
  char monthnumber[4];
  char month[16];
  char day[16];
  char year[16];

  //sprintf(monthnumber,"%u\0",datep.month);
  //if (strlen(monthnumber)==1) {
	 //monthnumber[1]=monthnumber[0];
	 //monthnumber[0]='0';
	 //monthnumber[2]=0;
  //}
  //strncpy(month,date->NameOfMonth(),3);
  //month[3]=0;


 sprintf(month,"%u\0",datep.month);
  if (strlen(month)==1) {
	 month[1]=month[0];
	 month[0]='0';
	 month[2]=0;
  }



  sprintf(year,"%u\0",date->Year());
  year[0]=year[2];
  year[1]=year[3];
  year[2]=0;
  char pfadbuf[200];
  pfadbuf[0]=0;
  sprintf(day,"%u\0",date->DayOfMonth());
  if (strlen(day)==1) {
	 day[1]=day[0];
	 day[0]='0';
	 day[2]=0;
  }
/*  sprintf(dirname,"%s%s\0",year,month);
  mkdir(dirname);
  sprintf(pfadbuf,"%s\\%s\0",pfad,dirname);
  strcpy(pfad,pfadbuf);
  chdir(pfad);*/
  sprintf(dirname,"%s%s%s\0",year,month,day);
  mkdir(dirname);
  sprintf(pfadbuf,"%s\\%s\0",pfad,dirname);
  strcpy(pfad,pfadbuf);
  strcpy(daypfad,pfad);
  chdir(pfad);

  struct ffblk ffblk;
  int done = findfirst("*.",&ffblk,FA_DIREC);
  int max=0;
  while (!done) {
	 char buf[20];
	 if (ffblk.ff_name[0]=='S') {
      int i;
		for (i=1;(i<7) && (ffblk.ff_name[i]!='.');i++) buf[i-1]=ffblk.ff_name[i];
		buf[i]=0;
		int n=atoi(buf);
		if (n>max) max=n;
	 }
	 done = findnext(&ffblk);
  }
  aktSerialNumber=max+1;

  done = findfirst("*.",&ffblk,FA_DIREC);
  max=0;
  while (!done) {
	 char buf[20];
	 if (ffblk.ff_name[0]=='F') {
      int i;
		for (i=1;(i<7) && (ffblk.ff_name[i]!='.');i++) buf[i-1]=ffblk.ff_name[i];
		buf[i]=0;
		int n=atoi(buf);
		if (n>max) max=n;
	 }
	 done = findnext(&ffblk);
  }
  aktFilmNumber=max+1;

  delete date;
}

double CName::GetFreeDiskSpace() {
  int olddisk=getdisk();
  setdisk(RDSetup->DataDisk);
  chdir(DataDirectory);
  struct diskfree_t free;
  if (_dos_getdiskfree(0, &free)!=0) return 0;
  setdisk(olddisk);
  double AvailClusters=free.avail_clusters;
  double BytesPerSector=free.bytes_per_sector;
  double SectorsPerCluster=free.sectors_per_cluster;
  return AvailClusters*BytesPerSector*SectorsPerCluster;
}

void CName::NewSerie() {
  Init();
  chdir(pfad);
  sprintf(SerialName,"S%i",aktSerialNumber);
  mkdir(SerialName);
  aktSerialNumber++;
  InSerie=1;
}

void CName::StopSerie() {
  InSerie=0;
}

void CName::ContinueSerie() {
  InSerie=1;
}

void CName::GetName(char *name, bool FirstImage, int CameraNumber) {
 char buf[200];
 if (InSerie) {
   sprintf(buf,"%s\\%s",pfad,SerialName);
 } else {
   Init();
   strcpy(buf,pfad);
 }
 chdir(buf);
 char buf2[20];
 if (FirstImage) {
   struct ffblk ffblk;
   int done = findfirst("P????.txt",&ffblk,0);
   int max=0;
   while (!done) {
     for (int i=1;i<=5;i++) buf2[i-1]=ffblk.ff_name[i];
     buf2[4]=0;
     int n=atoi(buf2);
     if (n>max) max=n;
     done = findnext(&ffblk);
   }
   aktPictureNumber=max+1;
 }
 char aktPictureString[5];
 char h[5];
 sprintf(h,"%u\0",aktPictureNumber);
 strcpy(aktPictureString,"0000X");
 for (int i=0; i<strlen(h);i++) aktPictureString[4-strlen(h)+i]=h[i];
 if ((CameraNumber>=0) && (CameraNumber<10)) {
   aktPictureString[4]=(char)('A'+CameraNumber);
 } else {
   MessageBox(GetActiveWindow(),"CName::GetName : CameraNumber out of range [0..9]","Error!",MB_OK);
 }
 sprintf(aktname,"%s\\P%s%%c%%u.bmp",buf,aktPictureString);
 strcpy(name,aktname);
}

void CName::GetSerieName(char *name) {
  char buf[200];
  sprintf(buf,"%s\\%s",pfad,SerialName);
  strcpy(name,buf);
}

void CName::GetParamName(char *name) {
  char buf[200];
  if (InSerie) {
	 sprintf(buf,"%s\\%s",pfad,SerialName);
	 sprintf(name,"%s\\Params.txt",buf);
  } else {
    sprintf(name,"%s\\Params%i.txt",pfad,aktPictureNumber);
  }
}

void CName::GetMessageName(char *name) {
  sprintf(name,"%s\\Message.txt",DataDirectory);
}

void CName::GetRegisterName(char *name,int aktcamera) {
  if (aktcamera>=10) aktcamera=9;
  if (aktcamera<0) aktcamera=0;
  char buf[200];
  char buf2[200];
  strcpy(buf,pfad);
  if (InSerie) {
	 sprintf(buf2,"%s\\%s",buf,SerialName);
  } else {
	 strcpy(buf2,buf);
  }
  sprintf(name,"%s\\Data%c.dat",buf2,(char)('A'+aktcamera));
}

void CName::GetSerieRegisterName(char *name, int aktcamera) {
  char buf[200];
  char buf2[200];
  strcpy(buf,pfad);
  sprintf(buf2,"%s\\%s",buf,SerialName);
  sprintf(name,"%s\\Data%c.dat",buf2,(char)('A'+aktcamera));
}

void CName::GetLogbookName(char *name) {
  sprintf(name,"%s\\Logbook.dat",DataDirectory);
}

void CName::GetImageLogbookName(char *name) {
  sprintf(name,"%s\\Logbook.txt",daypfad);
}

void CName::GetPath(char *path) {
  strcpy(path,pfad);
}

void CName::NewFilm() {
  Init();
  chdir(pfad);
  char FilmName[200];
  sprintf(FilmName,"F%i",aktFilmNumber);
  mkdir(FilmName);
  sprintf(FilmPfad,"%s\\%s\0",pfad,FilmName);
  aktFilmNumber++;
  aktFilmPictureNumber=9999;
}

void CName::GetFilmPictureName(char *name) {
  char buf[200];
  char aktPictureString[5];
  char h[5];
  sprintf(h,"%u\0",aktFilmPictureNumber);
  strcpy(aktPictureString,"0000");
  for (int i=0; i<strlen(h);i++) aktPictureString[4-strlen(h)+i]=h[i];
  sprintf(buf,"%s\\P%s.BMP",FilmPfad,aktPictureString);
  aktFilmPictureNumber--;
  strcpy(name,buf);
}

