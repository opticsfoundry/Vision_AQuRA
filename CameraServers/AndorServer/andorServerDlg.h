// AndorServerDlg.h : header file
//

#if !defined(AFX_AndorSERVERDLG_H__5413E9A8_2152_4C49_BFC7_F8F780E1043C__INCLUDED_)
#define AFX_AndorSERVERDLG_H__5413E9A8_2152_4C49_BFC7_F8F780E1043C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CNetwork;
class CAndorServerDlgAutoProxy;
class CAndor;
class CTCPIPServer;
#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CAndorServerDlg dialog

enum Enum_State {
	State_Connecting = 0,
	State_Idle,
	State_Connected
};

class CAndorServerDlg : public CDialog
{
	DECLARE_DYNAMIC(CAndorServerDlg);
	friend class CAndorServerDlgAutoProxy;

// Construction
public:
	void WakeUp();
	void SetTCPIPServer(CTCPIPServer* aTCPIPServer);
	CTCPIPServer* TCPIPServer;
	void SetCamera(CAndor *aAndor);
	void UpdateStatus();
	Enum_State State;
	CNetwork* Network;
	CAndor* Andor;
	CAndorServerDlg(CAndor* aAndor,CWnd* pParent = NULL);	// standard constructor
	virtual ~CAndorServerDlg();
	void PumpMessages();

// Dialog Data
	//{{AFX_DATA(CAndorServerDlg)
	enum { IDD = IDD_AndorSERVER_DIALOG };
	CString	m_Status;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAndorServerDlg)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CAndorServerDlgAutoProxy* m_pAutoProxy;
	HICON m_hIcon;

	BOOL CanExit();

	// Generated message map functions
	//{{AFX_MSG(CAndorServerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnClose();	
	virtual void OnCancel();
	afx_msg void OnTemp();
	afx_msg void OnExpose();
	afx_msg void OnLedDialog();
	afx_msg void OnIoSetup();
	afx_msg void OnDiscover();
	afx_msg void OnTestTriggerLine();
	afx_msg void OnExposeExternal();
	afx_msg void OnDebugStart();
	afx_msg void OnDebugStop();
	//}}AFX_MSG
	afx_msg LRESULT OnKickIdle(WPARAM, LPARAM);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AndorSERVERDLG_H__5413E9A8_2152_4C49_BFC7_F8F780E1043C__INCLUDED_)
