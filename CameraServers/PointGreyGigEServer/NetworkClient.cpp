// CNetworkClient.cpp: implementation of the CNetworkClient class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Control.h"
#include "NetworkClient.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNetworkClient::CNetworkClient(int amode)
{
	mode=amode;
	Network=NULL;
	FastWrite=false;
}

CNetworkClient::~CNetworkClient()
{
	if (Network) {
		if (mode==1) {
			CString help="Goodbye";
			SendCommand(help);
		}
		delete Network;
	}
}

bool CNetworkClient::ConnectSocket(LPCTSTR lpszAddress,UINT port) {
	Network=new CNetwork();
	bool Connected=Network->ConnectSocket(lpszAddress,port);
	if (!Connected) {
		delete Network;
		Network=NULL;
	}
	return Connected;
}

bool CNetworkClient::SendCommand(CString command) {
	if (Network) {
		Network->SendMsg("*"+command+"#");
	}
	return true;
}

void CNetworkClient::WriteDouble(double d) {
	CString buf;
	buf.Format("%8.5f",d);
	SendCommand(buf);
}

void CNetworkClient::WriteInteger(long i) {
	CString buf;
	buf.Format("%8i",i);
	SendCommand(buf);
}

void CNetworkClient::WriteBoolean(bool b) {
  if (b) SendCommand("TRUE");
  else SendCommand("FALSE");
}

void CNetworkClient::WriteString(CString s) {
  SendCommand(s);
}

void CNetworkClient::WriteChar(char c) {
	CString buf=c;
	SendCommand(buf);
}

bool CNetworkClient::ReadDouble(double &Value)
{
	if (!Network) return false;
	CString buf;
	bool ok=GetCommand(buf);
	Value=atof(buf);
	return ok;
}

bool CNetworkClient::Command(CString comand) {  
	if (Network) Network->Flush();
	SendCommand(comand);
	if (FastWrite) return true;
	if (!Ready()) {
		AfxMessageBox("CNetworkClient not Ready!\n(Command: "+comand+")");
		return false;
	} else return true;
}

bool CNetworkClient::GetCommand(CString &Command)
{
	if (!Network) return false;
	return Network->GetMessage(Command,1000);
}

bool CNetworkClient::Ready() {
	CString buf;
	if (GetCommand(buf)) return buf=="Ready";
	else return false;
}

CArchive* CNetworkClient::GetOutArchive()
{
	if (Network) return Network->m_pArchiveOut;
	else return NULL;
}

CArchive* CNetworkClient::GetInArchive()
{
	if (Network) return Network->m_pArchiveIn;
	else return NULL;
}

void CNetworkClient::Flush()
{
	if (Network) Network->Flush();
}

void CNetworkClient::StartFastWrite()
{
	FastWrite=true;
}

void CNetworkClient::StopFastWrite()
{
	FastWrite=false;
	Flush();
}
