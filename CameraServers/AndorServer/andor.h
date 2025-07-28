#if !defined(AFX_Andor_H__96FDE3A1_DEB3_46A7_83DB_BC321770525A__INCLUDED_)
#define AFX_Andor_H__96FDE3A1_DEB3_46A7_83DB_BC321770525A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Andor.h : header file
//


#define FKSDIRTYLINESTOCUT 4

/////////////////////////////////////////////////////////////////////////////
// CAndor window

class CAndor : public CWnd
{
// Construction
public:
	CAndor(bool aHardwareAccess,unsigned short* Imageptr,unsigned short* FKSImageptr);
// Operations
public:
	bool DoInternalTriggerAfterExternalTrigger;
	int NumberOfAdditionalReferenceImages;
	bool HardwareAccess;
	long hBin,vBin,xMax,xMin,yMin,yMax;
	int GoalTemperature;
	float LastTemperature;
	unsigned long FlushWait;
	bool ExternalTrigger;
	int MinTemp,MaxTemp;
	bool CoolerOn;
	DWORD StartTime,StartAcquisitionTime;
	bool initialized;
	int NrHorizontalSpeeds;
	int NrVerticalSpeeds;
	int NrFKVerticalSpeeds;
	float* HorizontalSpeed;
	int* VerticalSpeed;
	int* FKSVerticalSpeed;	
	int hSpeed;
	int vSpeed;
	bool FKSMode;
	int FKSHeight;
	int FKSImages;
	int FKSDataImages;
	int FKSReferenceImages;
	bool FKSExternalTriggerStart;
	int FKSDirtyLines;
	int FKSvSpeed;
	int NrADChannels;
	int NrAmp;
	int NrPreAmpGain;
	float PixelSizeY;
	float PixelSizeX;
	float ExposureTime;
	int NumberPictures;
	bool PrePicture;
	unsigned short* ImagePtr;
	long ImageSize;
	unsigned short* FKSImagePtr;
	long FKSImageSize;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAndor)
	//}}AFX_VIRTUAL

// Implementation
public:	
	void TransferParameters();
	int Width;
	int Height;
	void TestTriggerLine();
	double GetCCDTemperature();
	bool InitCamera(char* directory="");
	bool TakePicture(void);
//	bool TakePicture(unsigned short* Image[][(1024*1024)]);
	void SetParameters(int ahBin, int avBin, int axMin,int axMax,int ayMin,int ayMax,
		float aExposureTime,int aTemperature, bool aExternalTrigger,
		int avSpeed, int ahSpeed, 
		bool aFKSMode, int aFKSHeight, int aFKSImages, int aFKSDataImages, int aFKSReferenceImages, int aFKSvSpeed, bool aFKSExternalTriggerStart,int aFKSDirtyLines,
		unsigned int aFlushWait, int aNumberPictures, bool aPrePicture, bool aDoInternalTriggerAfterExternalTrigger,int aNumberOfAdditionalReferenceImages);
	void GetParameters(long &axMin,long &axMax,long &ayMin,long &ayMax,long &ahBin, long &avBin,int &aNrImages,bool &aPrePicture, bool &aFKSMode,
		long &aFKSHeight,long &aFKSDirtyLines,int &aFKSImages,int &aFKSDataImages,int &aFKSReferenceImages);
	virtual ~CAndor();

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
