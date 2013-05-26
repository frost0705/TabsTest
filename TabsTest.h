// TabsTest.h : main header file for the TABSTEST application
//

#if !defined(AFX_TABSTEST_H__D0E7BE3A_5EFE_4AAB_9F74_0203665310BA__INCLUDED_)
#define AFX_TABSTEST_H__D0E7BE3A_5EFE_4AAB_9F74_0203665310BA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols


/////////////////////////////////////////////////////////////////////////////
// CTabsTestApp:
// See TabsTest.cpp for the implementation of this class
//

class CTabsTestApp : public CWinApp
{
public:
	CTabsTestApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTabsTestApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CTabsTestApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABSTEST_H__D0E7BE3A_5EFE_4AAB_9F74_0203665310BA__INCLUDED_)
