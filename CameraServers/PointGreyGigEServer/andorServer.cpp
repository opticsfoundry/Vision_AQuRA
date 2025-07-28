// AndorServer.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "AndorServer.h"
#include "AndorServerDlg.h"
#include "TCPIPServer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAndorServerApp

BEGIN_MESSAGE_MAP(CAndorServerApp, CWinApp)
	//{{AFX_MSG_MAP(CAndorServerApp)
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAndorServerApp construction

CAndorServerApp::CAndorServerApp()
{
	dlg=NULL;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CAndorServerApp object

CAndorServerApp AndorServerApp;
CTCPIPServer *TCPIPServer;


#define MAX_LOADSTRING 100
static TCHAR szMutexName[MAX_LOADSTRING];              // Mutex name

/////////////////////////////////////////////////////////////////////////////
// Creates a mutex only for one application
//    This function tries to create a mutex, but if specified mutex exits
//    this function returns NULL.
HANDLE CreateOneAppMutex(LPCTSTR lpName)
{
    HANDLE hMutex;
    hMutex = CreateMutex(NULL, TRUE, lpName);   // Create mutex
    switch(GetLastError())
    {
    case ERROR_SUCCESS:
        // Mutex created successfully. There is no instances running
        break;
    case ERROR_ALREADY_EXISTS:
        // Mutex already exists so there is a running instance of our app.
        hMutex = NULL;
        break;
    default:
        // Failed to create mutex by unknown reason
        break;
    }
    return hMutex;
}


/////////////////////////////////////////////////////////////////////////////
// CAndorServerApp initialization

BOOL CAndorServerApp::InitInstance()
{
//	unsigned int CameraSerialNumber;
//	int MyIPPort;
//	unsigned int CameraNumber;

	//CCommandLineInfo& rCmdInfo;
	//ParseCommandLine(rCmdInfo );
	CString help=m_lpCmdLine;
//	CameraNumber=0;CameraSerialNumber=15394769;MyIPPort=704;
	CameraNumber=0;CameraSerialNumber=15394761;MyIPPort=704;
	if (help=="2") {	
		CameraNumber=2;CameraSerialNumber=15394754;MyIPPort=705;
	} else if (help=="3") {	
		CameraNumber=3;CameraSerialNumber=15394770;MyIPPort=706;
	}

	LastActiveTickCount=(long)(GetTickCount());
	if (CameraNumber==0) LoadString(m_hInstance, IDC_MUTEX_CAM0, szMutexName, MAX_LOADSTRING);
	if (CameraNumber==1) LoadString(m_hInstance, IDC_MUTEX_CAM1, szMutexName, MAX_LOADSTRING);
	if (CameraNumber==2) LoadString(m_hInstance, IDC_MUTEX_CAM2, szMutexName, MAX_LOADSTRING);
	if (CameraNumber==3) LoadString(m_hInstance, IDC_MUTEX_CAM3, szMutexName, MAX_LOADSTRING);
	if (CameraNumber==4) LoadString(m_hInstance, IDC_MUTEX_CAM4, szMutexName, MAX_LOADSTRING);
	if (CameraNumber>4) Mutex=NULL; 
	else Mutex=CreateOneAppMutex(szMutexName);
	if (Mutex==NULL) {
		//another copy of Control is already running
		//->disable hardware access
		AfxMessageBox("CAndorServerApp::InitInstance: Another instance of Andor server is running (or unknown mutex). Disabling hardware access. ");		
		HardwareAccess=false;
		return FALSE;
	} else HardwareAccess=true;		

	if (CoInitialize(NULL)!=S_OK) {
		AfxMessageBox("CAndorServerApp::InitInstance : COM initialization failed");
	};
	/*if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}*/

	// Initialize OLE libraries
	/*if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
*/
	AfxEnableControlContainer();

	// Standard initialization

	if (!AfxSocketInit()) {
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}
/*	if (RunEmbedded() || RunAutomated())
	{
		COleTemplateServer::RegisterAll();
	}
	else
	{
		COleObjectFactory::UpdateRegistryAll();
	}*/
	

	Andor=new CAndor(/*HardwareAccess*/ true);
	if (!Andor->InitCamera("",CameraSerialNumber,CameraNumber,MyIPPort)) {
		delete Andor;
		AfxMessageBox("CAndorServerApp::InitInstance : camera not initialized");
		return FALSE;
	}

	TCPIPServer=new CTCPIPServer();
	dlg=new CAndorServerDlg(Andor);	
	m_pMainWnd = dlg;
	dlg->SetCamera(Andor);
	dlg->SetTCPIPServer(TCPIPServer);
	TCPIPServer->SetCamera(Andor);
	TCPIPServer->SetAndorServerDialog(dlg);
	TCPIPServer->StartNetworkServer(MyIPPort);//703);
	int nResponse;			
	do {
		nResponse=dlg->DoModal();
	} while (nResponse != IDCANCEL);
	TCPIPServer->StopNetworkServer();	
	delete TCPIPServer;
	delete dlg;
	dlg=NULL;

	if (Andor) delete Andor;
	CoUninitialize();
	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

BOOL CAndorServerApp::OnIdle(LONG lCount) 
{	
	Sleep(1);
	CWinApp::OnIdle(lCount);
	return true;
}

void CAndorServerApp::WakeUp()
{
	LastActiveTickCount=(long)(GetTickCount());
}
