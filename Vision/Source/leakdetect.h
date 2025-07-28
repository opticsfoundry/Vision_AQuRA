#if !defined(__LeakDetect_h)
#define __LeakDetect_h

//#define DetectLeaks

const MaxNew=65536;

class CLeakDetect {
public:
	int AktNew;
	int LeakLine[MaxNew];
	int LeakFile[MaxNew];
   long LeakInfo[MaxNew];
	void* LeakP[MaxNew];
   int Min;
   int Stored;
   unsigned long StartMem;
public:
	CLeakDetect();
   ~CLeakDetect();
   void New(void* p,int file,int line,long Info=0);
   void Delete(void* p);
   void LeakDump(char* Filename);
   unsigned long GetFreeMem();
};

extern CLeakDetect LeakDetect;

#endif

