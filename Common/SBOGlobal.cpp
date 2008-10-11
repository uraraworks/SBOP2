/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* �t�@�C����	:SBOGlobal.cpp												 */
/* ���e			:�v���W�F�N�g�ˑ��ł͂Ȃ����ʏ��� �����t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2006/11/06													 */
/* ========================================================================= */

#include "StdAfx.h"

/* ========================================================================= */
/* �֐��v���g�^�C�v�錾														 */
/* ========================================================================= */

PBYTE	ZeroNew			(DWORD dwSize);												/* �w��T�C�Y�̃��������m�ۂ���0�N���A */
void	CopyMemoryRenew	(PVOID pDst, CONST PVOID pSrc, DWORD dwSize, PBYTE &pPos);	/* �������R�s�[���ă|�C���^��i�߂� */
void	strcpyRenew		(LPSTR pszDst, LPCSTR pszSrc, PBYTE &pPos);					/* ������R�s�[���ă|�C���^��i�߂� */
void	StoreRenew		(CmyString &strDst, LPCSTR pszSrc, PBYTE &pPos);			/* ������R�s�[���ă|�C���^��i�߂� */

/* ========================================================================= */
/* �֐���	:ZeroNew														 */
/* ���e		:�w��T�C�Y�̃��������m�ۂ���0�N���A							 */
/* ���t		:2006/11/06														 */
/* ========================================================================= */

PBYTE ZeroNew(DWORD dwSize)
{
	PBYTE pRet;

	pRet = new BYTE[dwSize];
	ZeroMemory (pRet, dwSize);

	return pRet;
}


/* ========================================================================= */
/* �֐���	:CopyMemoryRenew												 */
/* ���e		:�������R�s�[���ă|�C���^��i�߂�								 */
/* ���t		:2006/06/08														 */
/* ========================================================================= */

void CopyMemoryRenew(PVOID pDst, CONST PVOID pSrc, DWORD dwSize, PBYTE &pPos)
{
	CopyMemory (pDst, pSrc, dwSize);
	pPos += dwSize;
}


/* ========================================================================= */
/* �֐���	:strcpyRenew													 */
/* ���e		:������R�s�[���ă|�C���^��i�߂�								 */
/* ���t		:2006/06/08														 */
/* ========================================================================= */

void strcpyRenew(LPSTR pszDst, LPCSTR pszSrc, PBYTE &pPos)
{
	if ((pszSrc == NULL) || (strlen (pszSrc) <= 0)) {
		pPos ++;
		return;
	}
	strcpy (pszDst, pszSrc);
	pPos += (strlen (pszSrc) + 1);
}


/* ========================================================================= */
/* �֐���	:StoreRenew														 */
/* ���e		:������R�s�[���ă|�C���^��i�߂�								 */
/* ���t		:2006/11/05														 */
/* ========================================================================= */

void StoreRenew(CmyString &strDst, LPCSTR pszSrc, PBYTE &pPos)
{
	if ((pszSrc == NULL) || (strlen (pszSrc) <= 0)) {
		pPos ++;
		return;
	}
	strDst = pszSrc;
	pPos += (strDst.GetLength () + 1);
}


/* ========================================================================= */
/* �֐���	:GetModuleFilePath												 */
/* ���e		:���W���[���p�X���擾											 */
/* ���t		:2007/05/03														 */
/* ========================================================================= */

void GetModuleFilePath(
	LPSTR pszDst,		/* [ou] �p�X�i�[��o�b�t�@ */
	DWORD dwSize)		/* [in] �p�X�i�[��o�b�t�@�T�C�Y */
{
	ZeroMemory (pszDst, dwSize);
	GetModuleFileName (NULL, pszDst, dwSize);	/* ���W���[�������擾 */
	PathRemoveFileSpec (pszDst);				/* �t�@�C�������������� */
	PathAddBackslash (pszDst);					/* �u\�v��ǉ� */
}


/* ========================================================================= */
/* �֐���	:AllCreateDirectory												 */
/* ���e		:�w�肳�ꂽ�p�X�̃t�H���_���쐬									 */
/* ���t		:2008/03/09														 */
/* ========================================================================= */

BOOL AllCreateDirectory(
	LPCTSTR					lpszDir,			/* [in] �f�B���N�g����					*/
	LPSECURITY_ATTRIBUTES	pSAtt,				/* [in] (��{�FNULL)�Z�L�����e�B���ʎq	*/
	BOOL					bResult				/* [in] (��{�FTRUE)�����p�X�����݂��鎞*/
												/*		TRUE FALSE �ǂ����Ԃ���		*/
	)
{
	UINT	nStrLen;
	TCHAR	szTempDir[MAX_PATH] = _T("");
	LPTSTR	posRoot;
	TCHAR	ch = _T('\0');


	/*****************************************
		�����`�F�b�N
	*****************************************/

	if (lpszDir == NULL) {
		return FALSE;
	}
	nStrLen = _tcslen (lpszDir);
	if ((nStrLen == 0) || (nStrLen >= MAX_PATH)){
		return FALSE;
	}


	/*****************************************
		�Ƃ肠�����쐬
	*****************************************/

	if (CreateDirectory (lpszDir, pSAtt)) {
		return TRUE;
	}

	if (GetLastError () == ERROR_ALREADY_EXISTS) {
		return bResult;
	}


	/*****************************************
		 �쐬�J�n�ʒu�����o
	*****************************************/

	_tcscpy (szTempDir, lpszDir);
	szTempDir[MAX_PATH -1] = _T('\0');

	/* �l�b�g���[�N�t�H���_���w�肳��Ă���H */
	if (memcmp (szTempDir, _T("\\\\"), 2) == 0) {
		posRoot = &szTempDir[1];
		posRoot = _tcschr (posRoot + 1, _T('\\'));
		if (posRoot == NULL) {
			return FALSE;
		}
		posRoot = _tcschr (posRoot + 1, _T('\\'));
		if (posRoot == NULL) {
			return FALSE;
		}

	/* ���[�J���t�H���_���w�肳��Ă��� */
	} else {
		posRoot = _tcschr (szTempDir, _T('\\'));
		if (posRoot == NULL) {
			return FALSE;
		}
	}


	/********************************************
		�f�B���N�g����擪���珇�ɍ쐬���Ă���
	********************************************/

	while (posRoot) {
		posRoot = _tcschr (posRoot + 1, _T('\\'));
		if (posRoot) {
			ch = *posRoot;
			/* ���������U�؂�i���ƂŌ��ɖ߂��j�B	*/
			*posRoot = NULL;
		}

		if  (!CreateDirectory(szTempDir, pSAtt)) {
			if (ERROR_ALREADY_EXISTS != GetLastError()) {
				return FALSE;
			}
		}
		if (posRoot) {
			/* ��؂����ʒu�����ɖ߂� */
			*posRoot = ch;
		}
	}

	return TRUE;
}


/* ========================================================================= */
/* �֐���	:TrimViewString													 */
/* ���e		:�\���ł��Ȃ��������폜											 */
/* ���t		:2008/06/07														 */
/* ========================================================================= */

void TrimViewString(CmyString &strDst, LPCSTR pszSrc)
{
	int i, nLen;
	BYTE byTmp;
	char szTmp[3];

	strDst.Empty ();
	if (pszSrc == NULL) {
		return;
	}
	nLen = strlen (pszSrc);
	if (nLen <= 0) {
		return;
	}
	ZeroMemory (szTmp, sizeof (szTmp));

	for (i = 0; i < nLen; i ++) {
		byTmp = (BYTE)pszSrc[i];
		if (IsDBCSLeadByte (byTmp)) {
			szTmp[0] = byTmp;
			szTmp[1] = pszSrc[i + 1];
			strDst += szTmp;
			i ++;
			continue;
		}
		if ((byTmp < 0x20) || ((byTmp >= 0x7F) && !((byTmp >= 0xA1) && (byTmp <= 0xDF)))) {
			continue;
		}
		szTmp[0] = byTmp;
		szTmp[1] = 0;
		strDst += szTmp;
	}
}

/* Copyright(C)URARA-works 2006 */
