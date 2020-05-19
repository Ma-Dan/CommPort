// CommPortDlg.cpp : implementation file
//
#include "stdafx.h"
#include "CommPort.h"
#include "CommPortDlg.h"
#include "SetupDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define WM_NEW_DATA_ARRIVE WM_USER+1

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();
	// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	// Implementation

protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCommPortDlg dialog
CCommPortDlg::CCommPortDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCommPortDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCommPortDlg)
	m_strReceive = _T("");
	m_strTransmit = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCommPortDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCommPortDlg)
	DDX_Control(pDX, IDC_RECEIVE, m_CtrlReceive);
	DDX_Text(pDX, IDC_RECEIVE, m_strReceive);
	DDX_Text(pDX, IDC_TRANSMIT, m_strTransmit);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCommPortDlg, CDialog)
	//{{AFX_MSG_MAP(CCommPortDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_SETUP, OnSetup)
	ON_BN_CLICKED(IDC_OPEN, OnOpen)
	ON_BN_CLICKED(IDC_CLEAR_RECE, OnClearRece)
	ON_BN_CLICKED(IDC_CLEAR_SEND, OnClearSend)
	ON_BN_CLICKED(IDC_RECE, OnRece)
	ON_BN_CLICKED(IDC_SEND, OnSend)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_CHECK_HEX, OnCheckHex)
	ON_BN_CLICKED(IDC_SEND_FILE, OnSendFile)
	ON_BN_CLICKED(IDC_SAVE_RECEIVED, OnSaveReceived)
	ON_BN_CLICKED(IDC_CHECK_HEX_SEND, OnCheckHexSend)
	ON_MESSAGE(WM_NEW_DATA_ARRIVE, OnDataArrivedMsg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCommPortDlg message handlers
BOOL CCommPortDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	// Add "About..." menu item to system menu.
	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE); // Set big icon
	SetIcon(m_hIcon, FALSE); // Set small icon
	//初始化变量
	BaudRates[0] = 1200;
	BaudRates[1] = 2400;
	BaudRates[2] = 4800;
	BaudRates[3] = 9600;
	BaudRates[4] = 19200;
	BaudRates[5] = 38400;
	BaudRates[6] = 57600;
	BaudRates[7] = 115200;
	PortIDs[0] = "COM1:";
	PortIDs[1] = "COM2:";
	PortIDs[2] = "COM3:";
	PortIDs[3] = "COM4:";
	PortIDs[4] = "COM5:";
	PortIDs[5] = "COM6:";
	PortIDs[6] = "COM7:";
	PortIDs[7] = "COM8:";
	PortIDs[8] = "COM9:";
	PortNo = 1;
	BaudRate = 3;
	DataBits = 8;
	StopBits = ONESTOPBIT;
	Parity = NOPARITY;
	Open = FALSE;
	Receive = FALSE;
	HexDisplay = FALSE;
	HexSend = FALSE;
	m_strDataReceived = _T("");
	m_pSerial = new CWSerialPort();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCommPortDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.
void CCommPortDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting
		SendMessage(WM_ICONERASEBKGND, (WPARAM)dc.GetSafeHdc(), 0);
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
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CCommPortDlg::OnQueryDragIcon()
{
	return (HCURSOR)m_hIcon;
}

void CCommPortDlg::OnDataArrive(char *data, int length, DWORD userdata)
{
	CCommPortDlg *pWnd = (CCommPortDlg *)userdata;
	::SendMessage(pWnd->m_hWnd, WM_NEW_DATA_ARRIVE, (WPARAM)data, LPARAM(length));
}

LRESULT CCommPortDlg::OnDataArrivedMsg(WPARAM wParam, LPARAM lParam)
{
	//处理收到的数据
	int length = (int)lParam;
	char *data = (char*)wParam;
	CString THex(_T(""));
	unsigned char cdata;
	int i;
	if (length != 0)
	{
		if (HexDisplay)
		{
			for (i = 0; i < length; i++)
			{
				cdata = data[i];
				if (data[i] < 0)
					cdata = 256 + data[i];
				if (cdata < 16)
				{
					THex.Format(_T("%02X "), cdata);
				}
				else
				{
					THex.Format(_T("%02X "), cdata);
				}
				m_strDataReceived += THex;
			}
		}
		else
		{
			m_strDataReceived = CString(data).Left(length);
		}
		m_strReceive += m_strDataReceived;
		m_strDataReceived = _T("");
		SetDlgItemText(IDC_RECEIVE, m_strReceive);
		m_CtrlReceive.SetSel(m_strReceive.GetLength(), m_strReceive.GetLength(), FALSE);
	}
	return 0;
}

BOOL CCommPortDlg::SaveReceivedToFile(LPCTSTR FileName)
{
	int i;
	CFile cf;
	DWORD dwBytesToSave = m_strReceive.GetLength();
	if (!cf.Open(FileName, CFile::modeWrite | CFile::modeCreate))
	{
		AfxMessageBox(_T("无法打开Text文件！"));
	}
	cf.Seek(0, CFile::begin);
	char * buf = new char[dwBytesToSave];
	for (i = 0; i < dwBytesToSave; i++)
	{
		buf[i] = m_strReceive.GetAt(i);
	}
	cf.Write(buf, dwBytesToSave);
	delete[] buf;
	cf.Close();
	return TRUE;
}

void CCommPortDlg::OnSetup()
{
	CSetupDlg* Dialog1;
	Dialog1 = new CSetupDlg;
	Dialog1->BaudRate = BaudRate;
	Dialog1->PortNo = PortNo - 1;
	Dialog1->DataBits = DataBits - 7;
	Dialog1->StopBits = StopBits;
	Dialog1->Parity = Parity;
	if (Dialog1->DoModal() == IDOK)
	{
		BaudRate = Dialog1->BaudRate;
		PortNo = Dialog1->PortNo + 1;
		DataBits = Dialog1->DataBits + 7;
		StopBits = Dialog1->StopBits;
		Parity = Dialog1->Parity;
	}
	delete Dialog1;
}

void CCommPortDlg::OnOpen()
{
	if (!Open)
	{
		PortID = PortIDs[PortNo - 1];
		Open = m_pSerial->OpenPort(PortID, BaudRates[BaudRate], DataBits, StopBits, Parity, OnDataArrive, (DWORD)this);
		if (Open)
		{
			SetDlgItemText(IDC_OPEN, _T("关闭端口"));
		}
	}
	else
	{
		m_pSerial->ClosePort();
		Open = FALSE;
		Receive = FALSE;
		SetDlgItemText(IDC_RECE, _T("接收"));
		SetDlgItemText(IDC_OPEN, _T("打开端口"));
	}
}

void CCommPortDlg::OnClearRece()
{
	m_strReceive = _T("");
	GetDlgItemText(IDC_TRANSMIT, m_strTransmit);
	UpdateData(FALSE);
}

void CCommPortDlg::OnClearSend()
{
	m_strTransmit = _T("");
	UpdateData(FALSE);
}

void CCommPortDlg::OnRece()
{
	if (!Receive)
	{
		Receive = m_pSerial->Activate();
		if (Receive)
		{
			SetDlgItemText(IDC_RECE, _T("不接收"));
		}
	}
	else
	{
		m_pSerial->Deactivate();
		Receive = FALSE;
		SetDlgItemText(IDC_RECE, _T("接收"));
	}
}

void CCommPortDlg::OnSend()
{
	DWORD dwCharToWrite = 0;
	DWORD dwBytesWritten = 0;
	int i, j;
	GetDlgItemText(IDC_TRANSMIT, m_strTransmit);
	if (!HexSend)
	{
		dwCharToWrite = (DWORD)m_strTransmit.GetLength();
	}
	else
	{
		for (i = 0; i < m_strTransmit.GetLength() / 2; i++)
		{
			if (((m_strTransmit.GetAt(i * 2) >= '0'&&m_strTransmit.GetAt(i * 2) <= '9') || (m_strTransmit.GetAt(i * 2) >= 'A'&&m_strTransmit.GetAt(i * 2) <= 'F')) && ((m_strTransmit.GetAt(i * 2 + 1) >= '0'&&m_strTransmit.GetAt(i * 2 + 1) <= '9') || (m_strTransmit.GetAt(i * 2 + 1) >= 'A'&&m_strTransmit.GetAt(i * 2 + 1) <= 'F')))
			{
				dwCharToWrite++;
			}
		}
	}
	dwBytesWritten = 0;
	if (Open&&dwCharToWrite != 0)
	{
		char* buf = new char[dwCharToWrite];
		if (!HexSend)
		{
			for (i = 0; i < (int)dwCharToWrite; i++)
			{
				buf[i] = (char)m_strTransmit.GetAt(i);
			}
		}
		else
		{
			j = 0;
			for (i = 0; i < m_strTransmit.GetLength() / 2; i++)
			{
				if (((m_strTransmit.GetAt(i * 2) >= '0'&&m_strTransmit.GetAt(i * 2) <= '9') || (m_strTransmit.GetAt(i * 2) >= 'A'&&m_strTransmit.GetAt(i * 2) <= 'F')) && ((m_strTransmit.GetAt(i * 2 + 1) >= '0'&&m_strTransmit.GetAt(i * 2 + 1) <= '9') || (m_strTransmit.GetAt(i * 2 + 1) >= 'A'&&m_strTransmit.GetAt(i * 2 + 1) <= 'F')))
				{
					if (m_strTransmit.GetAt(i * 2 + 1) >= '0'&&m_strTransmit.GetAt(i * 2 + 1) <= '9')
					{
						buf[j] = m_strTransmit.GetAt(i * 2 + 1) - 48;
					}
					else
					{
						buf[j] = m_strTransmit.GetAt(i * 2 + 1) - 55;
					}
					if (m_strTransmit.GetAt(i * 2) >= '0'&&m_strTransmit.GetAt(i * 2) <= '9')
					{
						buf[j] += (m_strTransmit.GetAt(i * 2) - 48) * 16;
					}
					else
					{
						buf[j] += (m_strTransmit.GetAt(i * 2) - 55) * 16;
					}
					j++;
				}
			}
		}
		dwBytesWritten = m_pSerial->WritePort(buf, dwCharToWrite);
		if (dwBytesWritten == 0)
		{
			AfxMessageBox(_T("无法向端口写入数据！"));
		}
		delete[] buf;
	}
}

void CCommPortDlg::OnDestroy()
{
	CDialog::OnDestroy();
	m_pSerial->ClosePort();
	delete m_pSerial;
}

void CCommPortDlg::OnCheckHex()
{
	HexDisplay = !HexDisplay;
}

void CCommPortDlg::OnSendFile()
{
	if (!Open)
	{
		AfxMessageBox(_T("请先打开串口!"));
		return;
	}
	CString FileName(_T(""));
	CFileDialog FileDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, _T("All Files(*.*)|*.*||"));
	if (FileDlg.DoModal() == IDOK)
	{
		FileName = FileDlg.GetPathName();
	}
	if (FileName != _T(""))
	{
		m_pSerial->WriteFileToPort(FileName);
	}
}

void CCommPortDlg::OnSaveReceived()
{
	CString FileName(_T(""));
	CFileDialog FileDlg(FALSE, NULL, NULL, OFN_HIDEREADONLY, TEXT("Text Files(*.txt)|*.txt||"));
	if (FileDlg.DoModal() == IDOK)
	{
		FileName = FileDlg.GetPathName();
	}
	if (FileName != _T(""))
	{
		SaveReceivedToFile(FileName);
	}
}

void CCommPortDlg::OnCheckHexSend()
{
	HexSend = !HexSend;
}