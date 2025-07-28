#if !defined(AFX_NETWORK_H__2CA3109B_CC55_4698_9482_0A24FB76F059__INCLUDED_)
#define AFX_NETWORK_H__2CA3109B_CC55_4698_9482_0A24FB76F059__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "netsock.h"

class CNetwork : public CObject
{
// Attributes
public:
	CNetSocket* m_pSocket;
	CSocketFile* m_pFile;
	CArchive* m_pArchiveIn;
	CArchive* m_pArchiveOut;
// Operations
public:
	bool ConnectSocket(LPCTSTR lpszAddress, UINT nPort);
	void ProcessPendingRead();
	void SendMsg(CString& strText);
	virtual void ReceiveMsg();
// Implementation
public:
	void Flush();
	void StoreLastMessage(CString Message);
	bool GetMessage(CString &Message, unsigned int timeout);
	CString LastMessage;
	CNetwork();
	virtual ~CNetwork();
	void DeleteContents();
};

/////////////////////////////////////////////////////////////////////////////
#endif