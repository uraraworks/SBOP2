/* Copyright(C)URARA-works 2005 */
/* ========================================================================= */
/* �t�@�C����	:GetMD5File.cpp												 */
/* ���e			:MD5�v�Z�N���X �����t�@�C��									 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/08/28													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "GetMD5File.h"

/* ========================================================================= */
/* �֐���	:CGetMD5File::CGetMD5File										 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/08/28														 */
/* ========================================================================= */

CGetMD5File::CGetMD5File()
{
}


/* ========================================================================= */
/* �֐���	:CGetMD5File::~CGetMD5File										 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/08/28														 */
/* ========================================================================= */

CGetMD5File::~CGetMD5File()
{
}


/* ========================================================================= */
/* �֐���	:CGetMD5File::Update											 */
/* ���e		:�X�V															 */
/* ���t		:2007/08/28														 */
/* ========================================================================= */

void CGetMD5File::Update(LPCSTR pszFileName)
{
	PBYTE pTmp;
	DWORD dwFileSize, dwBytes;
	HANDLE hFile;

	pTmp = NULL;

	hFile = CreateFile (pszFileName, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (hFile == INVALID_HANDLE_VALUE) {
		goto Exit;
	}
	dwFileSize = GetFileSize (hFile, NULL);
	pTmp = new BYTE[dwFileSize];

	/* �t�@�C����ǂݍ��� */
	ReadFile (hFile, pTmp, dwFileSize, &dwBytes, NULL);
	CloseHandle (hFile);

	CGetMD5::Update (pTmp, dwFileSize);

Exit:
	SAFE_DELETE_ARRAY (pTmp);
}

/* Copyright(C)URARA-works 2007 */
