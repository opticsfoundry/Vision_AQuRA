#if !defined(AFX_STATUSDIALOG_H__980AA166_89FC_45C8_A71F_9BD69072E6A0__INCLUDED_)
#define AFX_STATUSDIALOG_H__980AA166_89FC_45C8_A71F_9BD69072E6A0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StatusDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStatusDialog dialog

class CStatusDialog : public CDialog
{
// Construction
public:
	CStatusDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CStatusDialog)
	enum { IDD = IDD_STATUS_DIALOG };
	CProgressCtrl	m_Progress;
	CString	m_Static;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStatusDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CStatusDialog)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STATUSDIALOG_H__980AA166_89FC_45C8_A71F_9BD69072E6A0__INCLUDED_)
