// StatusDlgContainer.cpp: implementation of the CStatusDlgContainer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StatusDlgContainer.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStatusDlgContainer::CStatusDlgContainer()
{

}

CStatusDlgContainer::~CStatusDlgContainer()
{

}

void CStatusDlgContainer::StatusDlgDone(CDialog *me) {
	AfxMessageBox("CStatusDlgContainer::StatusDlgDone : Abstract class called");
}