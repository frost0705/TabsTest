// MyTabDialog.cpp : implementation file
//

#include "stdafx.h"



#include "TabsTest.h"
#include "MyTabDialog.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyTabDialog dialog


CMyTabDialog::CMyTabDialog(UINT dwdIDD, CWnd* pParent):CDialog(dwdIDD, pParent)
{
	//{{AFX_DATA_INIT(CMyTabDialog)
	//}}AFX_DATA_INIT
}


void CMyTabDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMyTabDialog)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMyTabDialog, CDialog)
	//{{AFX_MSG_MAP(CMyTabDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyTabDialog message handlers

void CMyTabDialog::OnOK(){}
void CMyTabDialog::OnCancel(){}
void CMyTabDialog::PostNcDestroy() 
{
	CDialog::PostNcDestroy();
	delete this;
	
}
