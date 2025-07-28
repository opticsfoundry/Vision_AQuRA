// ConnectedDialog.cpp : implementation file
//

#include "stdafx.h"
#include "ApogeeServer.h"
#include "ConnectedDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConnectedDialog dialog


CConnectedDialog::CConnectedDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CConnectedDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConnectedDialog)
	m_MyPort = _T("");
	m_VisionIP = _T("");
	m_VisionPort = _T("");
	//}}AFX_DATA_INIT
}


void CConnectedDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConnectedDialog)
	DDX_Text(pDX, IDC_MYPORT, m_MyPort);
	DDX_Text(pDX, IDC_VISIONIP, m_VisionIP);
	DDX_Text(pDX, IDC_VISIONPORT, m_VisionPort);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConnectedDialog, CDialog)
	//{{AFX_MSG_MAP(CConnectedDialog)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConnectedDialog message handlers
