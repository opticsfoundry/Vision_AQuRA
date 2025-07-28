// StatusDlgContainer.h: interface for the CStatusDlgContainer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_StatusDLGCONTAINER_H__E7A6C506_1CAC_4223_85A2_30428EB76A1F__INCLUDED_)
#define AFX_StatusDLGCONTAINER_H__E7A6C506_1CAC_4223_85A2_30428EB76A1F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CStatusDlg;

class CStatusDlgContainer {
public:
	CStatusDlgContainer();
	virtual ~CStatusDlgContainer();
	virtual void StatusDlgDone(CDialog *me);
};

#endif // !defined(AFX_StatusDLGCONTAINER_H__E7A6C506_1CAC_4223_85A2_30428EB76A1F__INCLUDED_)
