// NetIO.cpp: implementation of the CNetIO class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Control.h"
#include "NetIO.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNetIO::CNetIO(bool aHardwareAccess)
{	
	Network=NULL;
}

CNetIO::~CNetIO()
{
	if (Network) delete Network;
}

bool CNetIO::ConnectSocket(LPCTSTR lpszAddress,UINT port) {
	Network=new CNetwork();
	bool Connected=Network->ConnectSocket(lpszAddress,port);
	if (!Connected) {
		delete Network;
		Network=NULL;		
	}
	return Connected;
}

