// Page3.cpp : implementation file
//

#include "stdafx.h"
#include "tabstest.h"

#include "MyTabDialog.h"
#include "Page3.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPage3 dialog


CPage3::CPage3(CWnd* pParent /*=NULL*/)
	: CMyTabDialog(CPage3::IDD, pParent)
	, DecryptionKeyPath(_T("C:\\Users\\Frost0705\\Desktop\\RSA_Files\\decryption_key.txt"))
	, EncryptedFilePath(_T("C:\\Users\\Frost0705\\Desktop\\RSA_Files\\encrypted_file.txt"))
	, DecryptedFilePath(_T("C:\\Users\\Frost0705\\Desktop\\RSA_Files\\decrypted_file.txt"))
{
	//{{AFX_DATA_INIT(CPage3)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CPage3::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPage3)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT1, DecryptionKeyPath);
	DDX_Text(pDX, IDC_EDIT2, EncryptedFilePath);
	DDX_Text(pDX, IDC_EDIT3, DecryptedFilePath);
}


BEGIN_MESSAGE_MAP(CPage3, CDialog)
	//{{AFX_MSG_MAP(CPage3)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CPage3::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CPage3::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CPage3::OnBnClickedButton3)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPage3 message handlers


void CPage3::OnBnClickedButton1()
{
	CFileDialog dlg(TRUE);

	if (dlg.DoModal() == IDOK)
	{
		DecryptionKeyPath = dlg.m_ofn.lpstrFile;
		UpdateData(FALSE);
	}
}


void CPage3::OnBnClickedButton2()
{
	CFileDialog dlg(TRUE);

	if (dlg.DoModal() == IDOK)
	{
		EncryptedFilePath = dlg.m_ofn.lpstrFile;
		UpdateData(FALSE);
	}
}


void CPage3::OnBnClickedButton3()
{
	CFileDialog dlg(TRUE);
	
	if (dlg.DoModal() == IDOK)
	{
		DecryptedFilePath = dlg.m_ofn.lpstrFile;
		UpdateData(FALSE);
	}
}
