// clntsock.cpp : implementation of the CClientSocket class
//
// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1998 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#include "stdafx.h"
#include "clntsock.h"
#include "ClientSocketUser.h"

/////////////////////////////////////////////////////////////////////////////
// CClientSocket Construction

CClientSocket::CClientSocket(CClientSocketUser* pDoc)
{
	m_pDoc = pDoc;
	m_nMsgCount = 0;
	m_pFile = NULL;
	m_pArchiveIn = NULL;
	m_pArchiveOut = NULL;
}

/////////////////////////////////////////////////////////////////////////////
// CClientSocket Operations

void CClientSocket::Init()
{
	m_pFile = new CSocketFile(this);
	m_pArchiveIn = new CArchive(m_pFile,CArchive::load);
	m_pArchiveOut = new CArchive(m_pFile,CArchive::store);
}

void CClientSocket::Abort()
{
	if (m_pArchiveOut != NULL)
	{
		m_pArchiveOut->Abort();
		delete m_pArchiveOut;
		m_pArchiveOut = NULL;
	}
}

void CClientSocket::SendString(CString string)
{
	if (m_pArchiveOut != NULL)
	{
		(*m_pArchiveOut)<<'*';
		for (int i=0;i<string.GetLength();i++) {
			char c=string[i];
			(*m_pArchiveOut)<<c;
		}		
		(*m_pArchiveOut)<<'#';
		m_pArchiveOut->Flush();
	}
}

void CClientSocket::SendData(unsigned char* Data, unsigned long length)
{
	if (m_pArchiveOut != NULL)
	{		
		for (unsigned long i=0;i<length;i++) {			
			unsigned char help=128;
			(*m_pArchiveOut)<<Data[i];
			
		}
		//m_pArchiveOut->Write(Data,length);
		m_pArchiveOut->Flush();
	}
}

bool CClientSocket::ReceiveString(CString &string)
{
	string="";
	if (m_pArchiveIn == NULL) return false;
	char in;
//	if (!m_pArchiveIn->IsBufferEmpty()) {
//		AfxMessageBox("CNetwork::ReceiveMsg :: Buffer empty");	
//		return;
//	}
	(*m_pArchiveIn)>>in;
	if (m_pDoc->DebugFile) (*(m_pDoc->DebugFile))<<"CClientSocket::ReceiveString "<<in;
	if (in!='*') {
		AfxMessageBox("CNetwork::ReceiveMsg :: * expected");		
		return false;
	}
	in='@';
	long StartTime=(long)(GetTickCount());
	while (/*(m_pArchiveIn->IsBufferEmpty()) &&*/ (in!='#') && (abs(((long)(GetTickCount())-StartTime))<5000))  {
		(*m_pArchiveIn)>>in;
		if (m_pDoc->DebugFile) (*(m_pDoc->DebugFile))<<in;
		if (in!='#') string+=in;
	}
	if (in!='#') {
		AfxMessageBox("CClientSocket::ReceiveString :: # expected");			
		return false;
	}
	if (m_pDoc->DebugFile) (*(m_pDoc->DebugFile))<<endl;
	return true;
}

/////////////////////////////////////////////////////////////////////////////
// CClientSocket Overridable callbacks

void CClientSocket::OnReceive(int nErrorCode)
{
	CSocket::OnReceive(nErrorCode);
	if (m_pDoc->DebugFile) (*(m_pDoc->DebugFile))<<"CClientSocket::OnReceive "<<nErrorCode<<endl;
	m_pDoc->ProcessPendingRead(this);
}

/////////////////////////////////////////////////////////////////////////////
// CSocket Implementation

CClientSocket::~CClientSocket()
{
	if (m_pArchiveOut != NULL)
		delete m_pArchiveOut;

	if (m_pArchiveIn != NULL)
		delete m_pArchiveIn;

	if (m_pFile != NULL)
		delete m_pFile;
}

#ifdef _DEBUG
void CClientSocket::AssertValid() const
{
	CSocket::AssertValid();
}

void CClientSocket::Dump(CDumpContext& dc) const
{
	CSocket::Dump(dc);
}
#endif //_DEBUG

IMPLEMENT_DYNAMIC(CClientSocket, CSocket)
