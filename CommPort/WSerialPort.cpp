// PSerialPort.cpp: implementation of the CWSerialPort class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CommPort.h"
#include "WSerialPort.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CWSerialPort::CWSerialPort()
{
	m_hComm = INVALID_HANDLE_VALUE;
	m_hReadThread = NULL;
	m_bReceiving = FALSE;
	m_nBufferSize = 256; //�����С
}

CWSerialPort::~CWSerialPort()
{
	ClosePort();
}

DWORD WINAPI CWSerialPort::ReadPortThread(LPVOID lpParameter)
{
	CWSerialPort* m_pSerial;
	m_pSerial = (CWSerialPort*)lpParameter;
	BOOL fReadState;
	DWORD dwLength;
	char* buf = new char[m_pSerial->m_nBufferSize];
	while ((m_pSerial->m_hComm != INVALID_HANDLE_VALUE) && (m_pSerial->m_bReceiving))
	{
		fReadState = ReadFile(m_pSerial->m_hComm, buf, m_pSerial->m_nBufferSize, &dwLength, NULL);
		if (!fReadState)
		{
			//AfxMessageBox(_T("�޷��Ӵ��ڶ�ȡ���ݣ�"));
		}
		else
		{
			if (dwLength != 0)
			{
				//��������
				if (m_pSerial->m_lpDataArriveProc != NULL)
				{
					m_pSerial->m_lpDataArriveProc(buf, dwLength, m_pSerial->m_dwUserData);
				}
			}
		}
		Sleep(1);
	}
	delete[] buf;
	return 0;
}

BOOL CWSerialPort::OpenPort(LPCTSTR Port, int BaudRate, int DataBits, int StopBits, int Parity, LPDataArriveProc proc, DWORD userdata)
{
	m_lpDataArriveProc = proc;
	m_dwUserData = userdata;
	if (m_hComm == INVALID_HANDLE_VALUE)
	{
		m_hComm = CreateFile(Port, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, 0);
		if (m_hComm == INVALID_HANDLE_VALUE)
		{
			AfxMessageBox(_T("�޷��򿪶˿ڣ������Ƿ��ѱ�ռ�á�"));
			return FALSE;
		}
		GetCommState(m_hComm, &dcb);
		dcb.BaudRate = BaudRate;
		dcb.ByteSize = DataBits;
		dcb.Parity = Parity;
		dcb.StopBits = StopBits;
		dcb.fParity = FALSE;
		dcb.fBinary = TRUE;
		dcb.fDtrControl = 0;
		dcb.fRtsControl = 0;
		dcb.fOutX = dcb.fInX = dcb.fTXContinueOnXoff = 0;
		//����״̬����
		SetCommMask(m_hComm, EV_RXCHAR);
		SetupComm(m_hComm, 16384, 16384);
		if (!SetCommState(m_hComm, &dcb))
		{
			AfxMessageBox(_T("�޷�����ǰ�������ö˿ڣ����������"));
			PurgeComm(m_hComm, PURGE_TXCLEAR | PURGE_RXCLEAR);
			ClosePort();
			return FALSE;
		}
		//���ó�ʱ����
		GetCommTimeouts(m_hComm, &CommTimeOuts);
		CommTimeOuts.ReadIntervalTimeout = 100;
		CommTimeOuts.ReadTotalTimeoutMultiplier = 1;
		CommTimeOuts.ReadTotalTimeoutConstant = 100;
		CommTimeOuts.WriteTotalTimeoutMultiplier = 0;
		CommTimeOuts.WriteTotalTimeoutConstant = 0;
		if (!SetCommTimeouts(m_hComm, &CommTimeOuts))
		{
			AfxMessageBox(_T("�޷����ó�ʱ������"));
			PurgeComm(m_hComm, PURGE_TXCLEAR | PURGE_RXCLEAR);
			ClosePort();
			return FALSE;
		}
		PurgeComm(m_hComm, PURGE_TXCLEAR | PURGE_RXCLEAR);
		return TRUE;
	}
	return FALSE;
}

BOOL CWSerialPort::ClosePort()
{
	Deactivate();
	if (m_hComm != INVALID_HANDLE_VALUE)
	{
		SetCommMask(m_hComm, 0);
		PurgeComm(m_hComm, PURGE_TXCLEAR | PURGE_RXCLEAR);
		CloseHandle(m_hComm);
		m_hComm = INVALID_HANDLE_VALUE;
		return TRUE;
	}
	return TRUE;
}

BOOL CWSerialPort::Activate()
{
	if (m_hComm == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}
	if (!m_bReceiving)
	{
		//��ʼ�����߳�
		PurgeComm(m_hComm, PURGE_TXCLEAR | PURGE_RXCLEAR);
		m_bReceiving = TRUE;
		m_hReadThread = CreateThread(NULL, 0, ReadPortThread, this, 0, NULL);
	}
	if (m_hReadThread != NULL)
	{
		return TRUE;
	}
	else
	{
		m_bReceiving = FALSE;
		return FALSE;
	}
	return FALSE;
}

BOOL CWSerialPort::Deactivate()
{
	if (m_hComm == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}
	//ֹͣ�����߳�
	if (m_bReceiving)
	{
		m_bReceiving = FALSE;
		WaitForSingleObject(m_hReadThread, 500);
		CloseHandle(m_hReadThread);
		m_hReadThread = NULL;
		return TRUE;
	}
	return FALSE;
}

BOOL CWSerialPort::IsActive()
{
	return m_bReceiving;
}

DWORD CWSerialPort::WritePort(char *data, int length)
{
	if (m_hComm == INVALID_HANDLE_VALUE)
	{
		return 0;
	}
	BOOL fWriteState;
	DWORD dwBytesWritten = 0;
	fWriteState = WriteFile(m_hComm, data, length * sizeof(char), &dwBytesWritten, NULL);
	return dwBytesWritten;
}

DWORD CWSerialPort::ReadPort(char *data, int length)
{
	BOOL fReadState;
	DWORD dwLength, dwBytesRead;
	int TimeOutCount;
	dwBytesRead = 0;
	TimeOutCount = 0;
	while (m_hComm != INVALID_HANDLE_VALUE)
	{
		char* buf = new char[length];
		fReadState = ReadFile(m_hComm, data, length, &dwLength, NULL);
		if (!fReadState)
		{
			break;
		}
		else
		{
			dwBytesRead += dwLength;
			data += dwLength;
		}
		if (dwBytesRead == length)
		{
			break;
		}
		if (dwLength != 0)
		{
			TimeOutCount = 0;
		}
		else
		{
			TimeOutCount++;
			Sleep(5);
		}
		if (TimeOutCount == 5)
		{
			break;
		}
	}
	return dwBytesRead;
}

DWORD CWSerialPort::WriteFileToPort(LPCTSTR FileName)
{
	if (m_hComm == INVALID_HANDLE_VALUE)
	{
		return 0;
	}
	CFile cf;
	BOOL fWriteState;
	DWORD dwBytesWritten;
	DWORD dwCharToWrite;
	dwCharToWrite = 0;
	if (!cf.Open(FileName, CFile::modeRead))
	{
		//AfxMessageBox(_T("�޷���Hex�ļ���"));
		return 0;
	}
	dwCharToWrite = (DWORD)cf.GetLength();
	cf.Seek(0, CFile::begin);
	dwBytesWritten = 0;
	if (m_hComm != INVALID_HANDLE_VALUE && dwCharToWrite != 0)
	{
		char* buf = new char[dwCharToWrite];
		cf.Read(buf, dwCharToWrite);
		fWriteState = WriteFile(m_hComm, buf, dwCharToWrite * sizeof(char), &dwBytesWritten, NULL);
		if (!fWriteState)
		{
			//AfxMessageBox(_T("�޷���˿�д�����ݣ�"));
		}
		delete[] buf;
	}
	cf.Close();
	return dwBytesWritten;
}