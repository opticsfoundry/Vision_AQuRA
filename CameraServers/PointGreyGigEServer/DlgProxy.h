// DlgProxy.h : header file
//

#if !defined(AFX_DLGPROXY_H__FF6D8BBE_7815_4A83_9601_4F03E0064216__INCLUDED_)
#define AFX_DLGPROXY_H__FF6D8BBE_7815_4A83_9601_4F03E0064216__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CApogeeServerDlg;

/////////////////////////////////////////////////////////////////////////////
// CApogeeServerDlgAutoProxy command target

class CApogeeServerDlgAutoProxy : public CCmdTarget
{
	DECLARE_DYNCREATE(CApogeeServerDlgAutoProxy)

	CApogeeServerDlgAutoProxy();           // protected constructor used by dynamic creation

// Attributes
public:
	CApogeeServerDlg* m_pDialog;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CApogeeServerDlgAutoProxy)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CApogeeServerDlgAutoProxy();

	// Generated message map functions
	//{{AFX_MSG(CApogeeServerDlgAutoProxy)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	DECLARE_OLECREATE(CApogeeServerDlgAutoProxy)

	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CApogeeServerDlgAutoProxy)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPROXY_H__FF6D8BBE_7815_4A83_9601_4F03E0064216__INCLUDED_)
