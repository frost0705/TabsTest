// Page1.cpp : implementation file
//

#include "stdafx.h"
#include "tabstest.h"

#include "MyTabDialog.h"
#include "Page1.h"




#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPage1 dialog


CPage1::CPage1(CWnd* pParent /*=NULL*/)
	: CMyTabDialog(CPage1::IDD, pParent)
	, OpenFilePath(_T("C:\\Users\\Frost0705\\Desktop\\RSA_Files\\open_fiile.txt"))
	, DecryptedFilePath(_T("C:\\Users\\Frost0705\\Desktop\\RSA_Files\\decrypted_file.txt"))
	, OpenFileText(_T("Текст открытого файла"))
	, DecryptedFileText(_T("Текст расшифрованного файла"))
{
	//{{AFX_DATA_INIT(CPage1)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CPage1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPage1)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT1, OpenFilePath);
	DDX_Text(pDX, IDC_EDIT2, DecryptedFilePath);
	DDX_Text(pDX, IDC_EDIT3, OpenFileText);
	DDX_Text(pDX, IDC_EDIT4, DecryptedFileText);
}


BEGIN_MESSAGE_MAP(CPage1, CDialog)
	//{{AFX_MSG_MAP(CPage1)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CPage1::OnBnClickedOpenFileButton)
	ON_BN_CLICKED(IDC_BUTTON2, &CPage1::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON4, &CPage1::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON3, &CPage1::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON5, &CPage1::OnBnClickedButton5)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPage1 message handlers



void CPage1::OnBnClickedOpenFileButton()
{
	CFileDialog dlg(TRUE);

	if (dlg.DoModal() == IDOK)
	{
		OpenFilePath = dlg.m_ofn.lpstrFile;
		UpdateData(FALSE);
	}
	
}

void CPage1::OnBnClickedButton2()
{
	CFileDialog dlg(TRUE);

	if (dlg.DoModal() == IDOK)
	{
		DecryptedFilePath = dlg.m_ofn.lpstrFile;
		UpdateData(FALSE);
	}
	
}

void CPage1::OnBnClickedButton3()
{
	UpdateData(TRUE);
	FILE *openFile;
	
	fopen_s(&openFile,OpenFilePath,"w");
	int size;
	size = OpenFileText.GetLength();
	OpenFileText.OemToCharA();
	wchar_t *h = new wchar_t[size];
	for (int i = 0; i < size;i++)
	{
		h[i] = OpenFileText[i];
	}
	fwrite(&size,sizeof(int),1,openFile);
	fwrite(h,size*sizeof(wchar_t),1,openFile);
	delete [] h;
	fclose(openFile);
	
}

void CPage1::OnBnClickedButton4()//Прочитать открытый файл
{
	UpdateData(TRUE);

	FILE *openFile;
	fopen_s(&openFile,OpenFilePath,"r");
	int size;
	
	fread(&size,sizeof(int),1,openFile);
	wchar_t *h = new wchar_t[size];
	fread(h,size*sizeof(wchar_t),1,openFile);
	CString message(h,size);
	OpenFileText = message;
	fclose(openFile);
	delete [] h;
	UpdateData(FALSE);
}

void CPage1::OnBnClickedButton5()//Прочитать дешифрованный файл
{
	UpdateData(TRUE);

	FILE *decryptedFile;
	fopen_s(&decryptedFile,DecryptedFilePath,"r");
	int size;
	
	fread(&size,sizeof(int),1,decryptedFile);
	wchar_t *h = new wchar_t[size];
	fread(h,size*sizeof(wchar_t),1,decryptedFile);
	CString message(h,size);
	
	DecryptedFileText = message;
	fclose(decryptedFile);
	delete [] h;
	UpdateData(FALSE);
	
}
