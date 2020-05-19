// SetupDlg.cpp : implementation file
//
#include "stdafx.h"
#include "CommPort.h"
#include "SetupDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetupDlg dialog
CSetupDlg::CSetupDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetupDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetupDlg)
	//}}AFX_DATA_INIT
}

void CSetupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetupDlg)
	DDX_Control(pDX, IDC_STOP, m_StopBits);
	DDX_Control(pDX, IDC_PORT, m_PortNo);
	DDX_Control(pDX, IDC_PARITY, m_Parity);
	DDX_Control(pDX, IDC_DATA, m_DataBits);
	DDX_Control(pDX, IDC_BAUD, m_BaudRate);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSetupDlg, CDialog)
	//{{AFX_MSG_MAP(CSetupDlg)
	ON_CBN_SELCHANGE(IDC_BAUD, OnSelchangeBaud)
	ON_CBN_SELCHANGE(IDC_DATA, OnSelchangeData)
	ON_CBN_SELCHANGE(IDC_PARITY, OnSelchangeParity)
	ON_CBN_SELCHANGE(IDC_PORT, OnSelchangePort)
	ON_CBN_SELCHANGE(IDC_STOP, OnSelchangeStop)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetupDlg message handlers
BOOL CSetupDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_StopBits.SetCurSel(StopBits);
	m_PortNo.SetCurSel(PortNo);
	m_Parity.SetCurSel(Parity);
	m_BaudRate.SetCurSel(BaudRate);
	m_DataBits.SetCurSel(DataBits);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void CSetupDlg::OnSelchangePort()
{
	if (m_PortNo.GetCurSel() != CB_ERR)
	{
		PortNo = m_PortNo.GetCurSel();
	}
}

void CSetupDlg::OnSelchangeBaud()
{
	if (m_BaudRate.GetCurSel() != CB_ERR)
	{
		BaudRate = m_BaudRate.GetCurSel();
	}
}

void CSetupDlg::OnSelchangeData()
{
	if (m_DataBits.GetCurSel() != CB_ERR)
	{
		DataBits = m_DataBits.GetCurSel();
	}
}

void CSetupDlg::OnSelchangeParity()
{
	if (m_Parity.GetCurSel() != CB_ERR)
	{
		Parity = m_Parity.GetCurSel();
	}
}

void CSetupDlg::OnSelchangeStop()
{
	if (m_StopBits.GetCurSel() != CB_ERR)
	{
		StopBits = m_StopBits.GetCurSel();
	}
}
