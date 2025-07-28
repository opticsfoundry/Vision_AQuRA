#ifndef __NETSOCK_H__
#define __NETSOCK_H__

#include <afxsock.h>

class CNetwork;

class CNetSocket : public CSocket
{
	DECLARE_DYNAMIC(CNetSocket);

// Construction
public:
	CNetSocket(CNetwork* pDoc);

// Operations
public:
	CNetwork* m_pDoc;

// Implementation
protected:
	virtual void OnReceive(int nErrorCode);
};

#endif // __NETSOCK_H__
