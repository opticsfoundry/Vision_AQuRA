#include "LeakDetect.h"
#include "stddef.h"
#include <window.h>
#include <stdio.h>

/*
Leak detect file codes:
1: network.cpp
2: picture.cpp
3: profil.cpp
4: picturew.cpp
5: visiondg.cpp
6: measure.cpp
7: point.cpp
8: netandor.cpp
9: params.cpp
10: tga.cpp
11: fitmath.cpp

files which might still contain leaks and would require a complete
implementation of LeakDetect.New and LeakDetect.Delete:
visiondg.cpp
piclist.cpp
colortab.cpp

everything else has been checked by eye
*/

CLeakDetect LeakDetect;

CLeakDetect::CLeakDetect() {
	for (int i=0;i<MaxNew;i++) {
     LeakLine[i]=0;
     LeakFile[i]=0;
     LeakP[i]=NULL;
     LeakInfo[i]=0;
   }
   Min=0;
   Stored=0;
  ofstream DebugFile("LeakDebug.dat");
  DebugFile<<"Start"<<endl;
  StartMem=GetFreeMem();
  DebugFile<<"Start Memory "<<StartMem<<endl;
  DebugFile.close();
}

CLeakDetect::~CLeakDetect() {
}

void CLeakDetect::New(void* p,int file,int line,long Info) {
   if (p==NULL) return;

   Stored++;
//    ofstream DebugFile("LeakDebug.dat", ios::app);
//  DebugFile<<"New "<<p<<" "<<file<<" "<<line<<" "<<Min<<" "<<Stored<<endl;
//  DebugFile.close();


	LeakLine[Min]=line;
   LeakFile[Min]=file;
   LeakP[Min]=p;
   LeakInfo[Min]=Info;
   Min++;
   while ((Min<MaxNew) && (LeakP[Min]!=NULL)) Min++;
   if (Min>=MaxNew) {
	   MessageBox(GetActiveWindow(),"LeakDetect::New out of memory","Error",MB_OK);
   }
}

void CLeakDetect::Delete(void* p) {
  if (p==NULL) return;
  int Max=0;
  while ((Max<MaxNew) && (LeakP[Max]!=p)) Max++;
  if (LeakP[Max]==p) {
  	 LeakP[Max]=NULL;
    if (Max<Min) Min=Max;
    Stored--;
  } else {
      MessageBox(GetActiveWindow(),"LeakDetect::Delete : pointer not found","Error",MB_OK);
  }
/*  ofstream DebugFile("LeakDebug.dat", ios::app);
  DebugFile<<"Del "<<p<<" "<<LeakFile[Max]<<" "<<LeakLine[Max]<<" "<<Max<<" "<<Stored<<endl;
  DebugFile.close();*/
}

void CLeakDetect::LeakDump(char* Filename) {
  ofstream DebugFile(Filename);
  DebugFile<<"Leakdetect"<<endl;
  DebugFile<<"Start Memory "<<StartMem<<endl;
  DebugFile<<"Stop  Memory "<<GetFreeMem()<<endl;
  for (int i=0;i<MaxNew;i++) {
    if (LeakP[i]!=NULL) {
		  DebugFile<<LeakP[i]<<" "<<LeakFile[i]<<" "<<LeakLine[i]<<" "<<LeakInfo[i]<<" "<<i<<endl;
    }
  }
  DebugFile.close();
}

unsigned long CLeakDetect::GetFreeMem() {
/*typedef struct _MEMORYSTATUS { // mst
    DWORD dwLength;        // sizeof(MEMORYSTATUS)
    DWORD dwMemoryLoad;    // percent of memory in use
    DWORD dwTotalPhys;     // bytes of physical memory
    DWORD dwAvailPhys;     // free physical memory bytes
    DWORD dwTotalPageFile; // bytes of paging file
    DWORD dwAvailPageFile; // free bytes of paging file
    DWORD dwTotalVirtual;  // user bytes of address space
    DWORD dwAvailVirtual;  // free user bytes
} MEMORYSTATUS, *LPMEMORYSTATUS; */
 LPMEMORYSTATUS  mem;
 mem=new MEMORYSTATUS;
 GlobalMemoryStatus(mem);
 unsigned long FreeMem=mem->dwAvailPhys;
 delete mem;
 return FreeMem;
 }

