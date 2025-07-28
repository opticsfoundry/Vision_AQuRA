// NetworkServer.cpp: implementation of the CNetworkServer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NetworkServer.h"
#include "clntsock.h"
#include "lstnsock.h"

CString* LastMessageList[LastMessageNr];
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNetworkServer::CNetworkServer()
{	
	DebugFile=NULL;
	ClientSocket=NULL;
	m_pSocket=NULL;	
	Connected=false;
	for (int i=0;i<LastMessageNr;i++) LastMessageList[i]=new CString();
}

CNetworkServer::~CNetworkServer()
{
	for (int i=0;i<LastMessageNr;i++) if (LastMessageList[i]) delete LastMessageList[i];
	if (m_pSocket) delete m_pSocket;
	if (ClientSocket) delete ClientSocket;	
	DebugStop();
}

void CNetworkServer::StartNetworkServer(unsigned int aServerIPPort)
{
	ServerIPPort=aServerIPPort;	
	bool Running=true;	
	while (Running) {
		if (m_pSocket) return;		
		m_pSocket = new CListeningSocket(this);
		if (m_pSocket->Create(ServerIPPort)) {
			if (m_pSocket->Listen()) {
				return;
			} else { 
				AfxMessageBox("CNetworkServer::RunNetworkServer error listening");
				Running=false;
			}				
		} else {
			AfxMessageBox("CNetworkServer::RunNetworkServer error creating listen socket");
			Running=false;
		}		 
	}	
	if (m_pSocket) delete m_pSocket;
	m_pSocket=NULL;	
}

void CNetworkServer::StopNetworkServer()
{		
	if (ClientSocket) {
		CString buf="Goodbye";
		SendMsg(buf);
		if (!ClientSocket->IsAborted()) {
			ClientSocket->ShutDown();
//	BYTE Buffer[50];
//	while (ClientSocket->Receive(Buffer,50) > 0);
			delete ClientSocket;
			ClientSocket=NULL;
		}
	}
	if (m_pSocket) delete m_pSocket;
	m_pSocket=NULL;	
}

void CNetworkServer::ProcessPendingAccept() {
	if (ClientSocket) DeleteClientSocket();
	ClientSocket = new CClientSocket(this);
	if (m_pSocket->Accept(*ClientSocket)) {
		ClientSocket->Init();				
		Connected=true;
		UpdateStatus();
	} else {
		DeleteClientSocket();	
		AfxMessageBox("CNetworkServer::ProcessPendingAccept : connection refused");
	}
}

void CNetworkServer::ProcessPendingRead(CClientSocket* pSocket)
{
	if (pSocket!=ClientSocket) {
		AfxMessageBox("CNetworkServer::ProcessPendingRead : unknown client");
		return;
	}
	CString Message;
	do {if (ReceiveMsg(Message)) {
			if (Message=="GoodBye") {
				ClientSocket->Close();
				DeleteClientSocket();
			} else ProcessMessage(Message);			
		} else {
			/*AfxMessageBox("CNetworkServer::ProcessPendingRead : error reading socket");
			if (ClientSocket) {
				ClientSocket->Abort();
				DeleteClientSocket();
			}*/
		}
	} while ((ClientSocket) && (ClientSocket->m_pArchiveIn->IsBufferEmpty()==0));	
}

bool CNetworkServer::ReceiveMsg(CString &Message)
{
	if (!ClientSocket) return false;
	bool ok;
	TRY
	{
		ok=ClientSocket->ReceiveString(Message);	
	}
	CATCH(CFileException, e)
	{	
		/*AfxMessageBox("CNetworkServer::ReceiveMsg : error reading socket");		
		if (ClientSocket) {
			ClientSocket->Abort();
			DeleteClientSocket();	
		}*/
		return false;
	}
	END_CATCH
	StoreLastMessage("<< "+Message);
	return ok;
}

bool CNetworkServer::SendMsg(CString& strText)
{
	if (!ClientSocket) return false;
	TRY
	{
		if (strText.GetLength()<100) StoreLastMessage(">> "+strText);
		ClientSocket->SendString(strText);		
		//ClientSocket->m_pArchiveOut->Flush();
	}
	CATCH(CFileException, e)
	{
		ClientSocket->Abort();
		AfxMessageBox("CNetworkServer::SendMsg : error sending to socket");
		DeleteClientSocket();	
		return false;
	}
	END_CATCH
	return true;
}

bool CNetworkServer::SendData(unsigned char* Data, unsigned long Size)
{
	if (!ClientSocket) return false;
	TRY
	{
		CString Message;
		Message.Format(">> SendData %u",Size);
		StoreLastMessage(Message);
		ClientSocket->SendData(Data,Size);		
		//ClientSocket->m_pArchiveOut->Flush();
	}
	CATCH(CFileException, e)
	{
		ClientSocket->Abort();
		AfxMessageBox("CNetworkServer::SendData : error sending to socket");
		DeleteClientSocket();	
		return false;
	}
	END_CATCH
	return true;
}

void CNetworkServer::StoreLastMessage(CString Message)
{
	if (DebugFile) {
		(*DebugFile)<<(long)(GetTickCount())<<" "<<Message<<endl;
		DebugFile->flush();
	}
	for (int i=(LastMessageNr-1);i>0;i--) 
		(*(LastMessageList[i]))=(*(LastMessageList[i-1]));
	(*(LastMessageList[0]))=Message;
}

void CNetworkServer::DeleteClientSocket()
{
	if (ClientSocket) {
		if (!ClientSocket->IsAborted()) ClientSocket->ShutDown();		
		delete ClientSocket;
	}
	ClientSocket=NULL;	
	Connected=false;
	UpdateStatus();
}

void CNetworkServer::ProcessMessage(CString Message)
{
	AfxMessageBox("CNetworkServer::ProcessMessage : abstract class called");
}

bool CNetworkServer::ReadDouble(double &Value) {
	CString Message;
	if (ReceiveMsg(Message)) {
		Value=atof(Message);
		return true;
	} else return false;
}

bool CNetworkServer::ReadInt(long &Value) {
	CString Message;
	if (ReceiveMsg(Message)) {
		Value=atoi(Message);
		return true;
	} else return false;
}

bool CNetworkServer::ReadBool(bool &Value) {
	CString Message;
	if (ReceiveMsg(Message)) {
		Value=Message=="TRUE";
		return true;
	} else return false;
}

bool CNetworkServer::SendReady() {
	CString buf="Ready";
	return SendMsg(buf);	
}

bool CNetworkServer::CheckReady() {
	CString Message;
	if (ReceiveMsg(Message)) {
		return Message=="Ready";
	} else return false;	
}

CArchive* CNetworkServer::GetInArchive()
{
	if (ClientSocket) return ClientSocket->m_pArchiveIn;
	else return NULL;
}

void CNetworkServer::WriteDouble(double d) {
	CString buf;
	buf.Format("%8.5f",d);
	SendMsg(buf);
}

void CNetworkServer::WriteInteger(long i) {
	CString buf;
	buf.Format("%4i",i);
	SendMsg(buf);
}

void CNetworkServer::UpdateStatus() {	

}

void CNetworkServer::DebugStart() {
	if (DebugFile) {
		DebugFile->close();
		delete DebugFile;
		DebugFile=NULL;
	}
	DebugFile=new ofstream("c:\\SrCAL\\Debug.dat",ios::out);	
}

void CNetworkServer::DebugStop() {
	if (DebugFile) {
		DebugFile->close();
		delete DebugFile;
		DebugFile=NULL;
	}
}
