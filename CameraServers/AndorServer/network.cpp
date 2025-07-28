#include "stdafx.h"
#include "netsock.h"
#include "network.h"
#include "ApogeeServer.h"

#ifdef _WIN32
#ifndef _UNICODE
#include <strstrea.h>
#endif
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNetwork construction/destruction

CNetwork::CNetwork()
{	
	m_pSocket = NULL;
	m_pFile = NULL;
	m_pArchiveIn = NULL;
	m_pArchiveOut = NULL;
	LastMessage="";
}

CNetwork::~CNetwork()
{
	DeleteContents();
}

void CNetwork::DeleteContents()
{
	if ((m_pSocket != NULL) && (m_pFile != NULL) && (m_pArchiveOut != NULL))
	{
		
	}

	delete m_pArchiveOut;
	m_pArchiveOut = NULL;
	delete m_pArchiveIn;
	m_pArchiveIn = NULL;
	delete m_pFile;
	m_pFile = NULL;

	if (m_pSocket != NULL)
	{
		//BYTE Buffer[50];
		m_pSocket->ShutDown();

	//	while(m_pSocket->Receive(Buffer,50) > 0);
	}

	delete m_pSocket;
	m_pSocket = NULL;
}

/////////////////////////////////////////////////////////////////////////////
// CNetwork Operations

bool CNetwork::ConnectSocket(LPCTSTR lpszAddress, UINT nPort)
{

	m_pSocket = new CNetSocket(this);

	if (!m_pSocket->Create())
	{
		int error=GetLastError();
		delete m_pSocket;
		m_pSocket = NULL;
		AfxMessageBox(IDS_CREATEFAILED);
		return FALSE;
	}

	while (!m_pSocket->Connect(lpszAddress, nPort ))
	{
		if (AfxMessageBox(IDS_RETRYCONNECT,MB_YESNO) == IDNO)
		{
			delete m_pSocket;
			m_pSocket = NULL;
			return FALSE;
		}
	}

	m_pFile = new CSocketFile(m_pSocket);
	m_pArchiveIn = new CArchive(m_pFile,CArchive::load);
	m_pArchiveOut = new CArchive(m_pFile,CArchive::store);

	return TRUE;
}

void CNetwork::ProcessPendingRead()
{
	do
	{
		ReceiveMsg();
		if (m_pSocket == NULL)
			return;
	}
	while(!m_pArchiveIn->IsBufferEmpty());
}

void CNetwork::SendMsg(CString& strText)
{
	StoreLastMessage(">>"+strText);
	for (int i=0;i<strText.GetLength();i++) {
		char c=strText[i];
		(*m_pArchiveOut)<<c;
	}
	m_pArchiveOut->Flush();
}

void CNetwork::ReceiveMsg()
{
//	if (m_pArchiveIn->IsBufferEmpty()!=0) return;
//	if (LastMessage!="") AfxMessageBox("CNetwork::ReceiveMsg :: Last message not read : "+LastMessage);	
	LastMessage="";
	char in;
//	if (!m_pArchiveIn->IsBufferEmpty()) {
//		AfxMessageBox("CNetwork::ReceiveMsg :: Buffer empty");	
//		return;
//	}
	(*m_pArchiveIn)>>in;
	if (in!='*') AfxMessageBox("CNetwork::ReceiveMsg :: * expected");		
	in='@';
	DWORD StartTime=GetTickCount();
	while (/*(m_pArchiveIn->IsBufferEmpty()) &&*/ (in!='#') && ((GetTickCount()-StartTime)<5000))  {
		(*m_pArchiveIn)>>in;
		if (in!='#') LastMessage+=in;
	}
	if (in!='#') AfxMessageBox("CNetwork::ReceiveMsg :: # expected");			
	StoreLastMessage("<<"+LastMessage);
}

bool CNetwork::GetMessage(CString &Message, unsigned int timeout)
{
	if (LastMessage=="") {
		DWORD StartTime=GetTickCount();
		while ((GetTickCount()-StartTime<timeout) && (LastMessage=="")) ReceiveMsg();
	}
	Message=LastMessage;
	LastMessage="";
	return Message!="";
}

void CNetwork::StoreLastMessage(CString Message)
{
	for (int i=(LastMessageNr-1);i>0;i--) 
		(*(LastMessageList[i]))=(*(LastMessageList[i-1]));
	(*(LastMessageList[0]))=Message;
}

void CNetwork::Flush()
{	
	if (LastMessage!="") AfxMessageBox("CNetwork::Flush : Message "+LastMessage+" flushed");	
	LastMessage="";
	m_pArchiveOut->Flush();
}
