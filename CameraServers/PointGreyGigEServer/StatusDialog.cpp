// StatusDialog.cpp : implementation file
//

#include "stdafx.h"
#include "StatusDialog.h"
#include "StatusDlgContainer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStatusDialog dialog


CStatusDialog::CStatusDialog(CWnd* pParent /*=NULL*/,CStatusDlgContainer *aContainer)
	: CDialog(CStatusDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStatusDialog)
	m_Static = _T("");
	//}}AFX_DATA_INIT
	ASSERT(pParent != NULL);

	m_pParent = pParent;
	m_nID = CStatusDialog::IDD;
	Container=aContainer;
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
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStatusDialog message handlers

void CStatusDialog::PostNcDestroy() 
{
	delete this;
}

void CStatusDialog::OnCancel() 
{
	if (Container) Container->StatusDlgDone(this);
	DestroyWindow();
}

BOOL CStatusDialog::Create() 
{
	return CDialog::Create(m_nID, m_pParent);
}

void CStatusDialog::SetData(CString text, int ProgressPosition,int ProgressMax, bool DoPumpMessages)
{
	m_Static=text;
	m_Progress.SetRange32( 0,ProgressMax);
	m_Progress.SetPos(ProgressPosition);	
	UpdateData(FALSE);
	if (DoPumpMessages) PumpMessages();
}

void CStatusDialog::PumpMessages() {
	MSG msg;
	while ( ::PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ) ) 
	{ 
		if ( !AfxGetApp ()->PumpMessage( ) ) 
		{ 	       
	       ::PostQuitMessage( 0); 
		    break; 
		} 
	} 
	// let MFC do its idle processing
	LONG lIdle = 0;
	while ( AfxGetApp()->OnIdle(lIdle++ ) )	;  
	// Perform some background processing here 
	// using another call to OnIdle	
}
