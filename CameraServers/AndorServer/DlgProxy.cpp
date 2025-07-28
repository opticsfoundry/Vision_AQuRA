// DlgProxy.cpp : implementation file
//

#include "stdafx.h"
#include "ApogeeServer.h"
#include "DlgProxy.h"
#include "ApogeeServerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CApogeeServerDlgAutoProxy

IMPLEMENT_DYNCREATE(CApogeeServerDlgAutoProxy, CCmdTarget)

CApogeeServerDlgAutoProxy::CApogeeServerDlgAutoProxy()
{
	EnableAutomation();
	
	// To keep the application running as long as an automation 
	//	object is active, the constructor calls AfxOleLockApp.
	AfxOleLockApp();

	ASSERT (AfxGetApp()->m_pMainWnd != NULL);
	ASSERT_VALID (AfxGetApp()->m_pMainWnd);
	ASSERT_KINDOF(CApogeeServerDlg, AfxGetApp()->m_pMainWnd);
	m_pDialog = (CApogeeServerDlg*) AfxGetApp()->m_pMainWnd;
	m_pDialog->m_pAutoProxy = this;
}

CApogeeServerDlgAutoProxy::~CApogeeServerDlgAutoProxy()
{
	if (m_pDialog != NULL)
		m_pDialog->m_pAutoProxy = NULL;
	AfxOleUnlockApp();
}

void CApogeeServerDlgAutoProxy::OnFinalRelease()
{
	CCmdTarget::OnFinalRelease();
}

BEGIN_MESSAGE_MAP(CApogeeServerDlgAutoProxy, CCmdTarget)
	//{{AFX_MSG_MAP(CApogeeServerDlgAutoProxy)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CApogeeServerDlgAutoProxy, CCmdTarget)
	//{{AFX_DISPATCH_MAP(CApogeeServerDlgAutoProxy)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IApogeeServer to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {D6B99834-4887-4AEA-9472-0742D75EB31A}
static const IID IID_IApogeeServer =
{ 0xd6b99834, 0x4887, 0x4aea, { 0x94, 0x72, 0x7, 0x42, 0xd7, 0x5e, 0xb3, 0x1a } };

BEGIN_INTERFACE_MAP(CApogeeServerDlgAutoProxy, CCmdTarget)
	INTERFACE_PART(CApogeeServerDlgAutoProxy, IID_IApogeeServer, Dispatch)
END_INTERFACE_MAP()

// {F1AF7842-3514-4B1A-A06B-FEF361988ADB}
IMPLEMENT_OLECREATE2(CApogeeServerDlgAutoProxy, "ApogeeServer.Application", 0xf1af7842, 0x3514, 0x4b1a, 0xa0, 0x6b, 0xfe, 0xf3, 0x61, 0x98, 0x8a, 0xdb)

/////////////////////////////////////////////////////////////////////////////
// CApogeeServerDlgAutoProxy message handlers
