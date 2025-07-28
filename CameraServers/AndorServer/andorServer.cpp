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
	LastActiveTickCount=GetTickCount();
	LoadString(m_hInstance, IDC_MUTEX, szMutexName, MAX_LOADSTRING);
	Mutex=CreateOneAppMutex(szMutexName);
	if (Mutex==NULL) {
		//another copy of Control is already running
		//->disable hardware access
		AfxMessageBox("CAndorServerApp::InitInstance: Another instance of Andor server is running. Disabling hardware access. ");		
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
	ImagePtr=&Image[0];
	FKSImagePtr=&FKSImage[0];

	Andor=new CAndor(/*HardwareAccess*/ true,ImagePtr,FKSImagePtr);
	if (!Andor->InitCamera()) {
		delete Andor;
		AfxMessageBox("CAndorServerApp::InitInstance : camera not initialized");
		return FALSE;
	}

	TCPIPServer=new CTCPIPServer(ImagePtr,FKSImagePtr);
	dlg=new CAndorServerDlg(Andor);	
	m_pMainWnd = dlg;
	dlg->SetCamera(Andor);
	dlg->SetTCPIPServer(TCPIPServer);
	TCPIPServer->SetCamera(Andor);
	TCPIPServer->SetAndorServerDialog(dlg);
	//TCPIPServer->StartNetworkServer(705); // SrCAL Horizontal imaging andor camera
	TCPIPServer->StartNetworkServer(704); // SrCAL Horizontal imaging andor camera
	//TCPIPServer->StartNetworkServer(703); // SrCAL Vertical imaging andor camera
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
	//if no picture has been taken for five minutes (or after midnight) talk to camera to avoid it going to sleep
	if (((GetTickCount()-LastActiveTickCount)>(1000*60*5)) || ((GetTickCount()-LastActiveTickCount)<0)) {
		
	/*	Andor->SetParameters(1,1,0,500,0,500,100,-20,false,0);
		const NrImages=1;
		unsigned short* Image[NrImages];	
		for (int i=0;i<NrImages;i++) Image[i]=new unsigned short[(1024*1024)/(1*1)];
		Andor->TakePicture(NrImages,Image);	
		for (int i=0;i<NrImages;i++) delete[] Image[i];		*/
		LastActiveTickCount=GetTickCount();
	}
	Sleep(1);
	CWinApp::OnIdle(lCount);
	TCPIPServer->CheckIfMessageArrived();
	return true;
}

void CAndorServerApp::WakeUp()
{
	LastActiveTickCount=GetTickCount();
}
