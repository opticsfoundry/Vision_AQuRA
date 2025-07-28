// AndorServer.h : main header file for the AndorSERVER application
//

#if !defined(AFX_AndorSERVER_H__63750E11_82F2_474D_BCB1_47C8887B90F8__INCLUDED_)
#define AFX_AndorSERVER_H__63750E11_82F2_474D_BCB1_47C8887B90F8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "Andor.h"	// Added by ClassView

/////////////////////////////////////////////////////////////////////////////
// CAndorServerApp:
// See AndorServer.cpp for the implementation of this class
//
class CAndorServerDlg;

class CAndorServerApp : public CWinApp
{
private:
	HANDLE Mutex;	
public:
	CAndorServerDlg* dlg;
	DWORD LastActiveTickCount;
	void WakeUp();
	bool HardwareAccess;
	CAndor *Andor;
	CAndorServerApp();

	unsigned short Image[(10*1004*1002)];
	unsigned short FKSImage[(10*1004*1002)];
	unsigned short* ImagePtr;
	unsigned short* FKSImagePtr;
//	unsigned short Image[5][(1024*1024)];	

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAndorServerApp)
	public:
	virtual BOOL InitInstance();
	virtual BOOL OnIdle(LONG lCount);
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CAndorServerApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

extern CAndorServerApp AndorServerApp;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AndorSERVER_H__63750E11_82F2_474D_BCB1_47C8887B90F8__INCLUDED_)
