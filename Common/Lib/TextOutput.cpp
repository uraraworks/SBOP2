/* Copyright(C)URARA-works 2005 */
/* ========================================================================= */
/* �t�@�C�����F	TextOutput.cpp												 */
/* ���e�F		�e�L�X�g�o�̓N���X �����t�@�C��								 */
/* �쐬�F		�N����N���t�����(URARA-works)								 */
/* �쐬�J�n���F	2005/04/01													 */
/* ========================================================================= */

#include "stdafx.h"
#include <stdio.h>
#include "TextOutput.h"


/* ========================================================================= */
/* �֐����F	CTextOutput::CTextOutput										 */
/* ���e�F	�R���X�g���N�^													 */
/* ���t�F	2005/04/01														 */
/* ========================================================================= */

CTextOutput::CTextOutput()
{
	m_pszFileName	= NULL;
	m_bHeader		= FALSE;
	m_bReturn		= TRUE;

	/* �N���e�B�J���Z�N�V�����I�u�W�F�N�g�������� */
	InitializeCriticalSection (&m_csWrite);
}


/* ========================================================================= */
/* �֐����F	CTextOutput::~CTextOutput										 */
/* ���e�F	�f�X�g���N�^													 */
/* ���t�F	2005/04/01														 */
/* ========================================================================= */

CTextOutput::~CTextOutput()
{
	Destroy ();

	DeleteCriticalSection (&m_csWrite);
}


/* ========================================================================= */
/* �֐����F	CTextOutput::Create												 */
/* ���e�F	������															 */
/* ���t�F	2005/04/01														 */
/* ========================================================================= */

BOOL CTextOutput::Create(
	LPCSTR pszFileName,		/* [in] �o�͂���t�@�C���� */
	BOOL bHeader,			/* [in] �w�b�_���������ށH */
	BOOL bReturn)			/* [in] ���s�R�[�h���������ށH */
{
	BOOL bRet;

	bRet = FALSE;

	if (m_pszFileName) {
		goto Exit;
	}

	m_bHeader = bHeader;
	m_bReturn = bReturn;

	m_pszFileName = new char[strlen (pszFileName) + 1];
	strcpy (m_pszFileName, pszFileName);

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐����F	CTextOutput::Destroy											 */
/* ���e�F	�j��															 */
/* ���t�F	2005/04/01														 */
/* ========================================================================= */

void CTextOutput::Destroy(void)
{
	if (m_pszFileName) {
		delete [] m_pszFileName;
		m_pszFileName = NULL;
	}
}


/* ========================================================================= */
/* �֐����F	CTextOutput::Write												 */
/* ���e�F	��������														 */
/* ���t�F	2005/04/01														 */
/* ========================================================================= */

void CTextOutput::Write(
	LPCSTR format, ...)		/* [in] �o�͂���t�H�[�}�b�g�t�������� */
{
	int nLen;
	char szTmp[256];
	va_list ap;

	/* �����̎��o�� */
	va_start (ap, format);

	nLen = _vsnprintf (szTmp, sizeof (szTmp), format, ap);
	if (nLen <= 0) {
		goto Exit;
	}

	WriteProc (szTmp);

Exit:
	return;
}


/* ========================================================================= */
/* �֐����F	CTextOutput::WriteProc											 */
/* ���e�F	�������ݏ���													 */
/* ���t�F	2005/04/01														 */
/* ========================================================================= */

void CTextOutput::WriteProc(
	LPCSTR pszText)		/* [in] �o�͂���NULL�I�[������ */
{
	HANDLE hFile;
	DWORD dwBytes;
	char szTmp[128];
	SYSTEMTIME stSysTime;

	/* �r���J�n */
	EnterCriticalSection (&m_csWrite);

	if ((m_pszFileName == NULL) || (pszText == NULL)) {
		goto Exit;
	}

	/* �t�@�C�����J�� */
	hFile = CreateFile (
			m_pszFileName,
			GENERIC_WRITE | GENERIC_READ,
			0,
			0,
			OPEN_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			0);
	if (hFile == INVALID_HANDLE_VALUE) {
		goto Exit;
	}
	/* �t�@�C���|�C���^���I�[�Ɉړ� */
	SetFilePointer (hFile, 0, 0, FILE_END);

	/* �w�b�_���������ށH */
	if (m_bHeader) {
		GetLocalTime (&stSysTime);
		wsprintf (szTmp, "%04d/%02d/%02d %02d:%02d:%02d:%03d\t",
				stSysTime.wYear, stSysTime.wMonth,  stSysTime.wDay,
				stSysTime.wHour, stSysTime.wMinute, stSysTime.wSecond,
				stSysTime.wMilliseconds);
		WriteFile (hFile, szTmp, strlen (szTmp), &dwBytes, NULL);
	}

	/* �{������������ */
	WriteFile (hFile, pszText, strlen (pszText), &dwBytes, NULL);

	/* ���s�R�[�h���������ށH */
	if (m_bReturn) {
		strcpy (szTmp, "\r\n");
		WriteFile (hFile, szTmp, strlen (szTmp), &dwBytes, NULL);
	}

	CloseHandle (hFile);

Exit:
	/* �r���I�� */
	LeaveCriticalSection (&m_csWrite);
}

/* Copyright(C)URARA-works 2005 */
