// ExecuteMeasurementDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Control.h"
#include "ConnectedDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConnectedDlg dialog


CConnectedDlg::CConnectedDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CConnectedDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConnectedDlg)
	m_MyPort = _T("");
	m_VisionIP = _T("");
	m_VisionPort = _T("");
	//}}AFX_DATA_INIT
	ASSERT(pParent != NULL);

	m_pParent = pParent;
	m_nID = CConnectedDlg::IDD;
}


void CConnectedDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConnectedDlg)
	DDX_Text(pDX, IDC_MYPORT, m_MyPort);
	DDX_Text(pDX, IDC_VISIONIP, m_VisionIP);
	DDX_Text(pDX, IDC_VISIONPORT, m_VisionPort);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConnectedDlg, CDialog)
	//{{AFX_MSG_MAP(CConnectedDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConnectedDlg message handlers

void CConnectedDlg::PostNcDestroy() 
{
	delete this;
}

void CConnectedDlg::OnCancel() 
{
	Sequence.ExecuteMeasurementDlgDone(this);
	DestroyWindow();
}

BOOL CConnectedDlg::Create() 
{
	return CDialog::Create(m_nID, m_pParent);
}

void CConnectedDlg::SetData(unsigned int aMyPort, CString aVisionIP, unsigned int aVisionPort)
{
	m_MyPort.Format("%u",aMyPort);
	m_VisionIP = aVisionIP;
	m_VisionPort.Format("%u",aVisionPort);
	UpdateData(FALSE);
	PumpMessages();
}

void CConnectedDlg::PumpMessages() {
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