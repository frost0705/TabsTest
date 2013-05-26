// Page2.cpp : implementation file
//

#include "stdafx.h"
#include "tabstest.h"

#include "MyTabDialog.h"
#include "Page2.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPage2 dialog


CPage2::CPage2(CWnd* pParent /*=NULL*/)
	: CMyTabDialog(CPage2::IDD, pParent)
	, EncryptionKeyPath(_T("C:\\Users\\Frost0705\\Desktop\\RSA_Files\\encryption_key.txt"))
	, OpenFilePath(_T("C:\\Users\\Frost0705\\Desktop\\RSA_Files\\open_file.txt"))
	, EncrypteFilePath(_T("C:\\Users\\Frost0705\\Desktop\RSA_Files\\encrypted_file.txt"))
{
	//{{AFX_DATA_INIT(CPage2)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CPage2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPage2)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT1, EncryptionKeyPath);
	DDX_Text(pDX, IDC_EDIT2, OpenFilePath);
	DDX_Text(pDX, IDC_EDIT3, EncrypteFilePath);
}


BEGIN_MESSAGE_MAP(CPage2, CDialog)
	//{{AFX_MSG_MAP(CPage2)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CPage2::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CPage2::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CPage2::OnBnClickedButton3)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPage2 message handlers


void CPage2::OnBnClickedButton1()
{
	CFileDialog dlg(TRUE);

	if (dlg.DoModal() == IDOK)
	{
		EncryptionKeyPath = dlg.m_ofn.lpstrFile;
		UpdateData(FALSE);
	}

}


void CPage2::OnBnClickedButton2()
{
	CFileDialog dlg(TRUE);

	if (dlg.DoModal() == IDOK)
	{
		OpenFilePath = dlg.m_ofn.lpstrFile;
		UpdateData(FALSE);
	}
	

}


void CPage2::OnBnClickedButton3()
{
	CFileDialog dlg(TRUE);

	if (dlg.DoModal() == IDOK)
	{
		EncrypteFilePath = dlg.m_ofn.lpstrFile;
		UpdateData(FALSE);
	}
	
}
