#if !defined(AFX_Andor_H__96FDE3A1_DEB3_46A7_83DB_BC321770525A__INCLUDED_)
#define AFX_Andor_H__96FDE3A1_DEB3_46A7_83DB_BC321770525A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Andor.h : header file
//
#include "FlyCapture2.h"
using namespace FlyCapture2;

/////////////////////////////////////////////////////////////////////////////
// CAndor window

class CAndor : public CWnd
{
// Construction
public:
	CAndor(bool aHardwareAccess);
// Operations
public:
	unsigned int CameraSerialNumber;
	int CameraNumber;
	int MyIPPort;
	bool Use16Bit;
	int NumberOfAdditionalReferenceImages;
	bool HardwareAccess;
	long hBin,vBin,xMax,xMin,yMin,yMax;
	int CameraGain;
	float LastTemperature;
	unsigned long FlushWait;
	bool ExternalTrigger;
	int MinTemp,MaxTemp;
	bool CoolerOn;
	DWORD StartTime,StartAcquisitionTime;
	bool initialized;
	int CameraBrightness;
	int vSpeed;
	bool FKSMode;
	int FKSHeight;
	int FKSImages;
	int FKSDataImages;
	int FKSReferenceImages;
	bool FKSExternalTriggerStart;
	int FKSDirtyLines;
	int ReadoutDelay;
	int NrADChannels;
	int NrAmp;
	int NrPreAmpGain;
	float PixelSizeY;
	float PixelSizeX;
	float ExposureTime;
	int NumberPictures;
	bool PrePicture;	
	TriggerMode triggerMode;

	PixelFormat pixFormat;
	Format7ImageSettings fmt7ImageSettings;
	GigECamera cam;
	FC2Version fc2Version;
	CameraInfo camInfo;

//	class CAndorServerApp *CAndorServerApp;

	bool Debug;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAndor)
	//}}AFX_VIRTUAL

// Implementation
public:	
	void ShutDown();
	void DebugStart() {Debug=true;}
	void DebugStop() {Debug=false;}
	void TransferParameters();
	int Width;
	int Height;
	void TestTriggerLine();
	double GetCCDTemperature();
	bool InitCamera(char* directory="",unsigned int CameraSerialNumber=0,int CameraNumber=0,int MyIPPort=0);
	bool TakePicture(unsigned short* Image[]);
	void SetParameters(int ahBin, int avBin, int axMin,int axMax,int ayMin,int ayMax,
		float aExposureTime,int aCameraGain, bool aExternalTrigger,
		int avSpeed, int aCameraBrightness, 
		bool aFKSMode, int aFKSHeight, int aFKSImages, int aFKSDataImages, int aFKSReferenceImages, int aReadoutDelay, bool aFKSExternalTriggerStart,int aFKSDirtyLines,
		unsigned int aFlushWait, int aNumberPictures, bool aPrePicture, bool aUse16Bit,int aNumberOfAdditionalReferenceImages);
	void GetParameters(long &axMin,long &axMax,long &ayMin,long &ayMax,long &ahBin, long &avBin,int &aNrImages,bool &aPrePicture, bool &aFKSMode,
		long &aFKSHeight,long &aFKSDirtyLines,int &aFKSImages,int &aFKSDataImages,int &aFKSReferenceImages);
	virtual ~CAndor();
private:
	bool PollForTriggerReady( GigECamera* pCam );
	bool CheckSoftwareTriggerPresence( GigECamera* pCam );
	bool FireSoftwareTrigger( GigECamera* pCam );

	// Generated message map functions
protected:
	//{{AFX_MSG(CAndor)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	
public:
	CString GetStatusString();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_Andor_H__96FDE3A1_DEB3_46A7_83DB_BC321770525A__INCLUDED_)
