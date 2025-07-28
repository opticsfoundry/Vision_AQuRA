// AndorServerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AndorServer.h"
#include "AndorServerDlg.h"
#include "Network.h"
#include "TCPIPServer.h"
#include "afxpriv.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAndorServerDlg dialog

IMPLEMENT_DYNAMIC(CAndorServerDlg, CDialog);

CAndorServerDlg::CAndorServerDlg(CAndor* aAndor,CWnd* pParent /*=NULL*/)
	: CDialog(CAndorServerDlg::IDD, pParent)
{
	Andor=aAndor;
	//{{AFX_DATA_INIT(CAndorServerDlg)	
	m_Status = _T("");
	//}}AFX_DATA_INIT
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pAutoProxy = NULL;
	Network=NULL;
	Andor=NULL;
}

CAndorServerDlg::~CAndorServerDlg()
{
	if (Network) delete Network;
	Network=NULL;
}

BOOL CAndorServerDlg::CanExit() {
	return true;
}
void CAndorServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAndorServerDlg)
	DDX_Text(pDX, IDC_STATUS, m_Status);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAndorServerDlg, CDialog)
	//{{AFX_MSG_MAP(CAndorServerDlg)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_EXPOSE, OnExpose)
	ON_BN_CLICKED(IDC_TEST_TRIGGER_LINE, OnTestTriggerLine)
	ON_BN_CLICKED(IDC_EXPOSE_EXTERNAL, OnExposeExternal)
	ON_BN_CLICKED(IDC_DEBUG_START, OnDebugStart)
	ON_BN_CLICKED(IDC_DEBUG_STOP, OnDebugStop)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_KICKIDLE, OnKickIdle)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAndorServerDlg message handlers

BOOL CAndorServerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	UpdateStatus();
	//just for debug
	//OnDebugStart();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CAndorServerDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

void CAndorServerDlg::OnCancel() 
{
	WakeUp();
	if (CanExit())
		CDialog::OnCancel();
}

void CAndorServerDlg::PumpMessages() {
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

void CAndorServerDlg::OnExpose() 
{	
	WakeUp();
	const int NrImages=1;
	Andor->SetParameters(/*hBin*/ 1, /*vBin*/ 1, /*xMin*/ 1,/*xMax*/ 1920,/*yMin*/ 1,/*yMax*/ 1200,
		/*ExposureTime*/ 0.1,/*Temperature*/ 0,/*ExternalTrigger*/ false,
		/*vSpeed*/ 12.5, /*CameraBrightness*/ 1, /*FKSMode*/ false,/*FKSHeight*/ 1200,/*FKSImages*/ 2,/*FKSDataImages*/ 1,/*FKSReferenceImages*/ 1,/*ReadoutDelay*/ 2,/*FKSExternalTriggerStart,*/ true,
		/*FKSDirtyLines*/0,/*FlushWait*/ 100, /*NumberPictures*/ NrImages,/*PrePicture*/false,/*Use16Bit*/ false,/*NumberOfAdditionalReferenceImages*/0);
	unsigned short* Image[NrImages];	
	for (int i=0;i<NrImages;i++) Image[i]=new unsigned short[(1920*1200)/(1*1)];
	Andor->TakePicture(Image);	
	for (int i=0;i<NrImages;i++) {
		delete[] Image[i];
		Image[i]=NULL;
	}
	UpdateStatus();
}

void CAndorServerDlg::OnExposeExternal() 
{
	WakeUp();
	const int NrImages=1;
	Andor->SetParameters(/*hBin*/ 1, /*vBin*/ 1, /*xMin*/ 1,/*xMax*/ 1920,/*yMin*/ 1,/*yMax*/ 1200,
		/*ExposureTime*/ 0.1,/*Temperature*/ 0,/*ExternalTrigger*/ true,
		/*vSpeed*/ 16, /*CameraBrightness*/ 1, /*FKSMode*/ false,/*FKSHeight*/ 1200,/*FKSImages*/ 2,/*FKSDataImages*/ 1,/*FKSReferenceImages*/ 1,/*ReadoutDelay*/ 2,/*FKSExternalTriggerStart,*/ true,
		/*FKSDirtyLines*/0,/*FlushWait*/ 100, /*NumberPictures*/ NrImages,/*PrePicture*/false,/*Use16Bit*/ false,/*NumberOfAdditionalReferenceImages*/0);
	unsigned short* Image[NrImages];	
	for (int i=0;i<NrImages;i++) Image[i]=new unsigned short[(1920*1200)/(1*1)];
	Andor->TakePicture(Image);
	for (int i=0;i<NrImages;i++) {
		delete[] Image[i];
		Image[i]=NULL;
	}
	UpdateStatus();
}

void CAndorServerDlg::SetCamera(CAndor *aAndor)
{
	Andor=aAndor;
}

void CAndorServerDlg::OnTestTriggerLine() 
{
	WakeUp();
	Andor->TestTriggerLine();	
}

void CAndorServerDlg::OnDebugStart() 
{
	WakeUp();
	 TCPIPServer->DebugStart();	
	 Andor->DebugStart();
	 UpdateStatus();
}

void CAndorServerDlg::OnDebugStop() 
{
	WakeUp();
	TCPIPServer->DebugStop();	
	Andor->DebugStop();
	UpdateStatus();
}

void CAndorServerDlg::SetTCPIPServer(CTCPIPServer *aTCPIPServer)
{
	TCPIPServer=aTCPIPServer;
}

 LRESULT CAndorServerDlg::OnKickIdle(WPARAM, LPARAM lCount) {
	 AfxGetApp()->OnIdle(lCount);
	 return true;
}

void CAndorServerDlg::WakeUp()
{
	((CAndorServerApp*)AfxGetApp())->WakeUp();
}

void CAndorServerDlg::UpdateStatus() {	
	CString Status;
	Status.Format("Camera status:\n%s\n\nServer status:\n%s",Andor->GetStatusString(),TCPIPServer->GetStatus());
	
	#define DIV 1048576
	// #define DIV 1

	char *divisor = "M";
	// char *divisor = "";

	// Handle the width of the field in which to print numbers this way to
	// make changes easier. The asterisk in the print format specifier
	// "%*ld" takes an int from the argument list, and uses it to pad and
	// right-justify the number being formatted.
	#define WIDTH 7

	CString MemoryStatus;
	MEMORYSTATUS stat;
	GlobalMemoryStatus (&stat);
	MemoryStatus.Format ("%ld percent of memory is in use\n%*ld %sB physical memory.\n%*ld free %sB physical memory\n",
			stat.dwMemoryLoad,WIDTH, stat.dwTotalPhys/DIV, divisor,WIDTH, stat.dwAvailPhys/DIV, divisor);
	CString Status2;  
    Status2.Format("%s\n%s",Status,MemoryStatus);

	m_Status=Status2;
	UpdateData(FALSE);
}
