#if !defined(AFX_MYTABDIALOG_H__1ECB2FB7_6C7C_4D1F_9C87_BE6C155757B7__INCLUDED_)
#define AFX_MYTABDIALOG_H__1ECB2FB7_6C7C_4D1F_9C87_BE6C155757B7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyTabDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyTabDialog dialog

class CMyTabDialog : public CDialog
{
public:
	CMyTabDialog(UINT dwdIDD,CWnd* pParent);

	//{{AFX_DATA(CMyTabDialog)
	enum { IDD = 0 };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CMyTabDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

protected:

	//{{AFX_MSG(CMyTabDialog)
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // !defined(AFX_MYTABDIALOG_H__1ECB2FB7_6C7C_4D1F_9C87_BE6C155757B7__INCLUDED_)
