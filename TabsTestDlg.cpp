// TabsTestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TabsTest.h"
#include "TabsTestDlg.h"

#include "MyTabDialog.h"
#include "Page1.h"
#include "Page2.h"
#include "Page3.h"
#include "Page4.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTabsTestDlg dialog

CTabsTestDlg::CTabsTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTabsTestDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTabsTestDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTabsTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTabsTestDlg)
	DDX_Control(pDX, IDC_TAB, m_IDC_TAB);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTabsTestDlg, CDialog)
	//{{AFX_MSG_MAP(CTabsTestDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_chkCREATE, OnchkCREATE)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, OnSelchangeTab)
	ON_NOTIFY(TCN_SELCHANGING, IDC_TAB, OnSelchangingTab)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabsTestDlg message handlers

BOOL CTabsTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTabsTestDlg::OnPaint() 
{
	
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
	CreateProperties();
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTabsTestDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CTabsTestDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

void CTabsTestDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

//НЕ ВИЗАРДОВСКИЙ КОД

//щёлкнули по флажку "Создать"
void CTabsTestDlg::OnchkCREATE() 
{
	CButton* pBtn=0;
	pBtn=(CButton*)GetDlgItem(IDC_chkCREATE);
	if(!pBtn)return;


	if(pBtn->GetCheck())
	{
		CreateProperties();
	}
	else
	{
		KillProperties();
	}

	
}

//создаём и добавляем все вкладки
void CTabsTestDlg::CreateProperties()
{
	enum//смещение страницы от верхнего левого угла на Tab Control
	{
		def_DlgTabLef0=5,
		def_DlgTabTop0=25
	};

	TCITEM item={TCIF_TEXT|TCIF_PARAM,0,0,"???",0,-1,0};
	item.lParam =(LPARAM)0;
	int nCurrIndx=0;

	//макрос - добавление страницы
	#define def_create_page(clss,caption) \
		{\
		clss* pPage=new clss(&m_IDC_TAB);\
		item.pszText =(char*)caption;\
		item.lParam =(LPARAM)pPage;\
		pPage->Create(pPage->IDD,&m_IDC_TAB);\
		pPage->SetWindowPos(0, def_DlgTabLef0,def_DlgTabTop0,\
				0,0,SWP_NOSIZE|SWP_NOZORDER|SWP_HIDEWINDOW);\
		m_IDC_TAB.InsertItem(nCurrIndx, &item);\
		nCurrIndx++;\
		}


	//вставка страниц, в таком порядке они и будут 
	def_create_page(CPage1,"Содержимое файлов");
	def_create_page(CPage2,"Шифрование");
	def_create_page(CPage3,"Дешифрование");
	def_create_page(CPage4,"Создание ключа");

	#undef def_create_page

	//показываем первую вкладку
	item.mask=TCIF_PARAM;
	m_IDC_TAB.GetItem(0,&item);
	if(item.lParam)
	{
		((CWnd*)item.lParam)->ShowWindow(SW_SHOW);
	}
}

//удаляем все вкладки
void CTabsTestDlg::KillProperties()
{
	TC_ITEM item;
	item.mask = TCIF_PARAM;
	CWnd *pWnd;

	//достаём по очереди указатели всё вкладки и удаляем
	for(;m_IDC_TAB.GetItemCount();)
	{
		m_IDC_TAB.GetItem(0,&item);

		pWnd=(CWnd*)(item.lParam);
		if(pWnd && pWnd->m_hWnd)
		{
			pWnd->DestroyWindow();
		}
		
		m_IDC_TAB.DeleteItem(0);
	}
}

//страница, на которую пришли. Показываем её
void CTabsTestDlg::OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TCITEM item={TCIF_PARAM};
	int nTab = m_IDC_TAB.GetCurSel();
	m_IDC_TAB.GetItem(nTab,&item);
	if(item.lParam)
	{
		((CWnd*)item.lParam)->ShowWindow(SW_SHOW);
	}
	*pResult = 0;
}

//страница, с которой ушли. Скрываем её
void CTabsTestDlg::OnSelchangingTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TCITEM item={TCIF_PARAM};
	int nTab = m_IDC_TAB.GetCurSel();
	m_IDC_TAB.GetItem(nTab,&item);
	if(item.lParam)
	{
		((CWnd*)item.lParam)->ShowWindow(SW_HIDE);
	}
	*pResult = 0;
}
