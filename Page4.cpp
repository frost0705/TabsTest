// Page4.cpp : implementation file
//

#include "stdafx.h"
#include "tabstest.h"

#include "MyTabDialog.h"
#include "Page4.h"




//#include "RSA\RSA_frost0705.h"
//#include <iostream>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPage4 dialog


CPage4::CPage4(CWnd* pParent /*=NULL*/)
	: CMyTabDialog(CPage4::IDD, pParent)
	, EncryptionKeyPath(_T("C:\\Users\\Frost0705\\Desktop\\RSA_Files\\encryption_key.txt"))
	, DecryptionKeyPath(_T("C:\\Users\\Frost0705\\Desktop\\RSA_Files\\decryption_key.txt"))
{
	//{{AFX_DATA_INIT(CPage4)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CPage4::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPage4)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT1, EncryptionKeyPath);
	DDX_Text(pDX, IDC_EDIT2, DecryptionKeyPath);
}


BEGIN_MESSAGE_MAP(CPage4, CDialog)
	//{{AFX_MSG_MAP(CPage4)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CPage4::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CPage4::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CPage4::OnBnClickedButton3)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPage4 message handlers


void CPage4::OnBnClickedButton1()
{
	CFileDialog dlg(TRUE);

	if (dlg.DoModal() == IDOK)
	{
		EncryptionKeyPath = dlg.m_ofn.lpstrFile;
		UpdateData(FALSE);
	}
}


void CPage4::OnBnClickedButton2()
{
	CFileDialog dlg(TRUE);

	if (dlg.DoModal() == IDOK)
	{
		DecryptionKeyPath = dlg.m_ofn.lpstrFile;
		UpdateData(FALSE);
	}
}


void CPage4::OnBnClickedButton3()
{
	/*
	cBigNumber d,e,q,p,n;
	//подготовка ключей. На данный момент используютя слабые ключи(маленькие простые числа). Сделать модуль, который просчитает большие ключи и запишет их в файл
	preparation(&e,&d,&p,&q,&n,30);
	FILE *eFile,*dFile;
	fopen_s(&eFile,EncryptionKeyPath,"w");
	fopen_s(&dFile,DecryptionKeyPath,"w");
	int eSize = sizeof(e),dSize = sizeof(d);
	fwrite(&eSize,sizeof(int),
	fwrite(&e,sizeof(e),1,eFile);
	fwrite(&d,sizeof(d),1,dFile);*/
}
