#if !defined(AFX_PAGE1_H__F5A24CAD_4727_41D3_AD54_F824E73318D0__INCLUDED_)
#define AFX_PAGE1_H__F5A24CAD_4727_41D3_AD54_F824E73318D0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Page1.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPage1 dialog

class CPage1 : public CMyTabDialog
{
// Construction
public:
	CPage1(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPage1)
	enum { IDD = IDD_page1 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPage1)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPage1)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedOpenFileButton();
	CString OpenFilePath;
	CString DecryptedFilePath;
	CString OpenFileText;
	CString DecryptedFileText;
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGE1_H__F5A24CAD_4727_41D3_AD54_F824E73318D0__INCLUDED_)
