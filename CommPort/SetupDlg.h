#if !defined(AFX_SETUPDLG_H__07252AC6_B1B5_4BA5_867C_49D7F2882F60__INCLUDED_)
#define AFX_SETUPDLG_H__07252AC6_B1B5_4BA5_867C_49D7F2882F60__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// SetupDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSetupDlg dialog
class CSetupDlg : public CDialog
{
	// Construction
public:
	CSetupDlg(CWnd* pParent = NULL);   // standard constructor
	int StopBits;
	int PortNo;
	int Parity;
	int DataBits;
	int BaudRate;

	// Dialog Data
	//{{AFX_DATA(CSetupDlg)
	enum { IDD = IDD_SETUPDLG };
	CComboBox m_StopBits;
	CComboBox m_PortNo;
	CComboBox m_Parity;
	CComboBox m_DataBits;
	CComboBox m_BaudRate;
	//}}AFX_DATA

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetupDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CSetupDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeBaud();
	afx_msg void OnSelchangeData();
	afx_msg void OnSelchangeParity();
	afx_msg void OnSelchangePort();
	afx_msg void OnSelchangeStop();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedOk();
};
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
#endif // !defined(AFX_SETUPDLG_H__07252AC6_B1B5_4BA5_867C_49D7F2882F60__INCLUDED_)