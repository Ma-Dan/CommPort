// CommPort.h : main header file for the COMMPORT application
//
#if !defined(AFX_COMMPORT_H__6049CF36_3F18_4C7F_A003_EECF23658480__INCLUDED_)
#define AFX_COMMPORT_H__6049CF36_3F18_4C7F_A003_EECF23658480__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h" // main symbols

/////////////////////////////////////////////////////////////////////////////
// CCommPortApp:
// See CommPort.cpp for the implementation of this class
//
class CCommPortApp : public CWinApp
{
public:
	CCommPortApp();
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCommPortApp)

public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL
	// Implementation
	//{{AFX_MSG(CCommPortApp)
	// NOTE - the ClassWizard will add and remove member functions here.
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
#endif // !defined(AFX_COMMPORT_H__6049CF36_3F18_4C7F_A003_EECF23658480__INCLUDED_)