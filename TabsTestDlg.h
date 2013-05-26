// TabsTestDlg.h : header file
//

#if !defined(AFX_TABSTESTDLG_H__4DDC8D3B_4005_40BE_BCE5_4CAE4516FE51__INCLUDED_)
#define AFX_TABSTESTDLG_H__4DDC8D3B_4005_40BE_BCE5_4CAE4516FE51__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CTabsTestDlg dialog

class CTabsTestDlg : public CDialog
{
// Construction
public:
	void KillProperties();
	void CreateProperties();
	CTabsTestDlg(CWnd* pParent = NULL);	// standard constructor
// Dialog Data
	//{{AFX_DATA(CTabsTestDlg)
	enum { IDD = IDD_TABSTEST_DIALOG };
	CTabCtrl	m_IDC_TAB;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTabsTestDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CTabsTestDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnchkCREATE();
	afx_msg void OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangingTab(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABSTESTDLG_H__4DDC8D3B_4005_40BE_BCE5_4CAE4516FE51__INCLUDED_)
