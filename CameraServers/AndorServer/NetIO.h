// NetIO.h: interface for the CNetIO class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NETIO_H__2352AEA4_3EAE_4F17_88B9_0A49C9685223__INCLUDED_)
#define AFX_NETIO_H__2352AEA4_3EAE_4F17_88B9_0A49C9685223__INCLUDED_

#include "network.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CNetIO : public CObject  
{
private:
	CNetwork *Network;
public:	
	CNetIO();
	virtual ~CNetIO();	
	bool ConnectSocket(LPCTSTR lpszAddress,UINT port);
	bool CheckForReady();
	bool ReadLine(CString command);
	bool ReadInt(long &Value);
	bool ReadDouble(double &Value);
	bool CheckReady();
	void PutLine(CString command);	
	void WriteDouble(double d);
	void WriteInteger(long i);
};

#endif // !defined(AFX_NETIO_H__2352AEA4_3EAE_4F17_88B9_0A49C9685223__INCLUDED_)
