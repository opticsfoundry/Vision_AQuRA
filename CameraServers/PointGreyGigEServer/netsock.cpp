#include "stdafx.h"
#include "netsock.h"
#include "network.h"

#include <stddef.h>

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CNetSocket, CSocket)

CNetSocket::CNetSocket(CNetwork* pDoc)
{
	m_pDoc = pDoc;
}

void CNetSocket::OnReceive(int nErrorCode)
{
	CSocket::OnReceive(nErrorCode);

	m_pDoc->ProcessPendingRead();
}
