// NetworkClient.h: interface for the CNetworkClient class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NetworkClient_H__D3C2A5C8_2D51_4BFB_8061_4C5802EF5B3F__INCLUDED_)
#define AFX_NetworkClient_H__D3C2A5C8_2D51_4BFB_8061_4C5802EF5B3F__INCLUDED_

#include "network.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CNetworkClient : public CObject
{
private:
	int mode;
	CNetwork *Network;
public:
	void StopFastWrite();
	void StartFastWrite();
	bool FastWrite;
	void Flush();
	CArchive* GetInArchive();
	CArchive* GetOutArchive();
	bool Ready();
	bool ConnectSocket(LPCTSTR lpszAddress,UINT port);
	bool ReadDouble(double &Value);
	bool SendCommand(CString comand);
	void WriteDouble(double d);
	void WriteInteger(long i);
	void WriteBoolean(bool b);
	void WriteString(CString s);
	void WriteChar(char c);
	bool Command(CString comand);
	bool GetCommand(CString &Command);	
	CNetworkClient(int amode);
	virtual ~CNetworkClient();
};

#endif // !defined(AFX_NetworkClient_H__D3C2A5C8_2D51_4BFB_8061_4C5802EF5B3F__INCLUDED_)
