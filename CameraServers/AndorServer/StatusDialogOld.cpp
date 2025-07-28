// StatusDialog.cpp : implementation file
//

#include "stdafx.h"
#include "ApogeeServer.h"
#include "StatusDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStatusDialog dialog


CStatusDialog::CStatusDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CStatusDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStatusDialog)
	m_Static = _T("");
	//}}AFX_DATA_INIT
}


void CStatusDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStatusDialog)
	DDX_Control(pDX, IDC_PROGRESS_BAR, m_Progress);
	DDX_Text(pDX, IDC_PROGRESS_STATIC, m_Static);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStatusDialog, CDialog)
	//{{AFX_MSG_MAP(CStatusDialog)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStatusDialog message handlers
