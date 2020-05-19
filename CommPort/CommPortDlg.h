// CommPortDlg.h : header file
//
#if !defined(AFX_COMMPORTDLG_H__9CE2A6A5_0689_49CC_8E3E_85EA26286437__INCLUDED_)
#define AFX_COMMPORTDLG_H__9CE2A6A5_0689_49CC_8E3E_85EA26286437__INCLUDED_
#if _MSC_VER > 1000
#pragma once

#endif // _MSC_VER > 1000
#include "WSerialPort.h"

/////////////////////////////////////////////////////////////////////////////
// CCommPortDlg dialog
class CCommPortDlg : public CDialog
{
	// Construction
public:
	CCommPortDlg(CWnd* pParent = NULL); // standard constructor
	BOOL SaveReceivedToFile(LPCTSTR FileName);

private:
	//变量
	BOOL Open;
	BOOL Receive;
	int PortNo, BaudRate, DataBits, StopBits, Parity;
	CString PortID, PortIDs[9];
	int BaudRates[8];
	BOOL HexDisplay, HexSend;

	//串口类变量
	CWSerialPort* m_pSerial;
	CString m_strDataReceived;
	static void OnDataArrive(char *data, int length, DWORD userdata);

	// Dialog Data
	//{{AFX_DATA(CCommPortDlg)
	enum { IDD = IDD_COMMPORT_DIALOG };
	CEdit m_CtrlReceive;
	CString m_strReceive;
	CString m_strTransmit;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCommPortDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:
	HICON m_hIcon;
	// Generated message map functions
	//{{AFX_MSG(CCommPortDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSetup();
	afx_msg void OnOpen();
	afx_msg void OnClearRece();
	afx_msg void OnClearSend();
	afx_msg void OnRece();
	afx_msg void OnSend();
	afx_msg void OnDestroy();
	afx_msg void OnCheckHex();
	afx_msg void OnSendFile();
	afx_msg void OnSaveReceived();
	afx_msg void OnCheckHexSend();
	afx_msg LRESULT OnDataArrivedMsg(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
#endif // !defined(AFX_COMMPORTDLG_H__9CE2A6A5_0689_49CC_8E3E_85EA26286437__INCLUDED_)