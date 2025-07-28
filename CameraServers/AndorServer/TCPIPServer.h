// TCPIPServer.h: interface for the CTCPIPServer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TCPIPSERVER_H__909129ED_C7BF_4DBC_8C8E_14ECA60531E3__INCLUDED_)
#define AFX_TCPIPSERVER_H__909129ED_C7BF_4DBC_8C8E_14ECA60531E3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MessageReceiver.h"
#include "NetworkServer.h"
#include "AndorServerDlg.h"

class CSequenceListPoint;

class CTCPIPServer : public CNetworkServer 
{
public:	
	CAndorServerDlg *AndorServerDlg;
	void SendReady();
	void TakeImage();
	void TransferImage();
	double LastTime;		
	bool HardwareAccess;
	CAndor *Andor;
	int TakingImageStatus;
	unsigned short* ImagePtr;
	unsigned short* FKSImagePtr;
	long ImageSize;
	long FKSImageSize;
private:
	//unsigned short* Image[30];	
public:
	//void PrepareTransferImage();
	CString LastCommand;
	void SetAndorServerDialog(CAndorServerDlg* aAndorServerDlg);
	virtual void UpdateStatus();
	long ExternalTrigger;
	void SetParameters();
	void SetCamera(CAndor *aAndor);
	void Disable();	
	void StopServer();
	void ProcessMessage(CString Message);
	CTCPIPServer(unsigned short* ImagePtrVal,unsigned short* FKSImagePtrVal);
	virtual ~CTCPIPServer();	
	CString GetStatus();
};

#endif // !defined(AFX_TCPIPSERVER_H__909129ED_C7BF_4DBC_8C8E_14ECA60531E3__INCLUDED_)
