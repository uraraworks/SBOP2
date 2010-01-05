/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* �t�@�C����	:myString.cpp												 */
/* ���e			:������N���X ��`�t�@�C��									 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2006/05/25													 */
/* ========================================================================= */

#include "StdAfx.h"
#include <tchar.h>
#include "myString.h"

/* ========================================================================= */
/* �֐���	:CmyString::CmyString											 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2006/05/25														 */
/* ========================================================================= */

CmyString::CmyString()
{
	m_nLength	= 0;
	m_pszString = NULL;
	Empty ();
}


/* ========================================================================= */
/* �֐���	:CmyString::CmyString											 */
/* ���e		:�R�s�[�R���X�g���N�^											 */
/* ���t		:2007/02/11														 */
/* ========================================================================= */

CmyString::CmyString(CmyString &strSrc)
{
	m_pszString = NULL;
	Renew ((LPCSTR)strSrc);
}
CmyString::CmyString(LPCTSTR szSrc)
{
	m_pszString = NULL;
	Renew (szSrc);
}


/* ========================================================================= */
/* �֐���	:CmyString::~CmyString											 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2006/05/25														 */
/* ========================================================================= */

CmyString::~CmyString()
{
	if (m_pszString) {
		delete [] m_pszString;
		m_pszString = NULL;
	}
}


/* ========================================================================= */
/* �֐���	:CmyString::Empty												 */
/* ���e		:���������ɂ���												 */
/* ���t		:2006/05/29														 */
/* ========================================================================= */

void CmyString::Empty(void)
{
	Renew ("");
}


/* ========================================================================= */
/* �֐���	:CmyString::IsEmpty												 */
/* ���e		:�����񂪋󂩔���												 */
/* ���t		:2006/05/29														 */
/* ========================================================================= */

BOOL CmyString::IsEmpty(void)
{
	BOOL bRet;

	bRet = TRUE;

	if (m_pszString == NULL) {
		goto Exit;
	}
	if (m_nLength <= 0) {
		goto Exit;
	}

	bRet = FALSE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CmyString::GetLength											 */
/* ���e		:�����񒷂��擾													 */
/* ���t		:2006/05/30														 */
/* ========================================================================= */

int CmyString::GetLength(void)
{
	return m_nLength;
}


/* ========================================================================= */
/* �֐���	:CmyString::Format												 */
/* ���e		:����������ŏ�����												 */
/* ���t		:2006/06/02														 */
/* �����F	�o�b�t�@��1024�o�C�g�Ȃ̂ł���ȏ�ɂ͂Ȃ�Ȃ��悤�ɒ���		 */
/* ========================================================================= */

void CmyString::Format(LPCSTR lpFormat, ...)
{
	LPSTR pszTmp;

	pszTmp = new char[1024];

	/* ������������� */
	va_list argList;
	va_start (argList, lpFormat);
	wvsprintf (pszTmp, lpFormat, argList);
	va_end (argList);

	Renew (pszTmp);
	delete [] pszTmp;
}


/* ========================================================================= */
/* �֐���	:CmyString::operator =											 */
/* ���e		:�������������													 */
/* ���t		:2006/05/25														 */
/* ========================================================================= */

void CmyString::operator =(CmyString &strSrc)
{
	Renew ((LPCSTR)strSrc);
}


/* ========================================================================= */
/* �֐���	:CmyString::operator =											 */
/* ���e		:�������������													 */
/* ���t		:2006/05/25														 */
/* ========================================================================= */

void CmyString::operator =(LPCSTR pszSrc)
{
	Renew (pszSrc);
}


/* ========================================================================= */
/* �֐���	:CmyString::operator +=											 */
/* ���e		:�������ǉ�													 */
/* ���t		:2006/05/25														 */
/* ========================================================================= */

void CmyString::operator +=(LPCSTR pszSrc)
{
	int nLen;
	LPSTR pszTmp;

	nLen	= 0;
	pszTmp	= NULL;

	if (m_pszString)	{	nLen += (strlen (m_pszString) + 1);	}
	if (pszSrc)			{	nLen += (strlen (pszSrc) + 1);		}

	if (nLen == 0) {
		goto Exit;
	}

	pszTmp = new char[nLen];
	ZeroMemory (pszTmp, nLen);

	if (m_pszString)	{	strcpy (pszTmp, m_pszString);	}
	if (pszSrc)			{	strcat (pszTmp, pszSrc);		}

	Renew (pszTmp);

Exit:
	if (pszTmp) {
		delete [] pszTmp;
	}
}


/* ========================================================================= */
/* �֐���	:CmyString::operator ==											 */
/* ���e		:��������r													 */
/* ���t		:2006/06/01														 */
/* ========================================================================= */

BOOL CmyString::operator ==(LPCSTR pszSrc)
{
	int nResult;
	BOOL bRet;

	bRet = FALSE;

	if (m_pszString == NULL) {
		goto Exit;
	}

	nResult = strcmp (m_pszString, pszSrc);
	if (nResult == 0) {
		bRet = TRUE;
	}

Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CmyString::operator !=											 */
/* ���e		:��������r													 */
/* ���t		:2006/11/05														 */
/* ========================================================================= */

BOOL CmyString::operator !=(LPCSTR pszSrc)
{
	int nResult;
	BOOL bRet;

	bRet = FALSE;

	if (m_pszString == NULL) {
		goto Exit;
	}

	nResult = strcmp (m_pszString, pszSrc);
	if (nResult != 0) {
		bRet = TRUE;
	}

Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CmyString::operator LPCTSTR()									 */
/* ���e		:�������������													 */
/* ���t		:2006/05/25														 */
/* ========================================================================= */

CmyString::operator LPCTSTR()
{
	return m_pszString;
}


/* ========================================================================= */
/* �֐���	:CmyString::CompareNoCase										 */
/* ���e		:��������r(�啶����������ʖ���)								 */
/* ���t		:2007/02/14														 */
/* ========================================================================= */

int CmyString::CompareNoCase(LPCSTR pszSrc)
{
	int nRet;

	nRet = -1;

	if (m_pszString == NULL) {
		goto Exit;
	}

	nRet = _tcsicmp (m_pszString, pszSrc);
Exit:
	return nRet;
}


/* ========================================================================= */
/* �֐���	:CmyString::Renew												 */
/* ���e		:��������X�V													 */
/* ���t		:2006/05/25														 */
/* ========================================================================= */

void CmyString::Renew(LPCSTR pszSrc)
{
	if (m_pszString == pszSrc) {
		return;
	}
	if (m_pszString) {
		delete [] m_pszString;
		m_pszString = NULL;
	}

	if (pszSrc == NULL) {
		return;
	}

	m_nLength = strlen (pszSrc);
	m_pszString = new char[m_nLength + 1];
	strcpy (m_pszString, pszSrc);
}

/* Copyright(C)URARA-works 2006 */
