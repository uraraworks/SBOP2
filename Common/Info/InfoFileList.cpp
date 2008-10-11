/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:InfoFileList.cpp											 */
/* ���e			:�t�@�C�����X�g���N���X �����t�@�C��						 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/03/02													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "InfoFileList.h"

/* ========================================================================= */
/* �֐���	:CInfoFileList::CInfoFileList									 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/03/02														 */
/* ========================================================================= */

CInfoFileList::CInfoFileList()
{
}


/* ========================================================================= */
/* �֐���	:CInfoFileList::~CInfoFileList									 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/03/02														 */
/* ========================================================================= */

CInfoFileList::~CInfoFileList()
{
	DeleteAll ();
}


/* ========================================================================= */
/* �֐���	:CInfoFileList::Add												 */
/* ���e		:�ǉ�															 */
/* ���t		:2008/03/02														 */
/* ========================================================================= */

void CInfoFileList::Add(LPCSTR pszHash, LPCSTR pszFileName, DWORD dwFileSize)
{
	PFILELISTINFO pInfo;

	pInfo = GetNew ();
	pInfo->strMD5		= pszHash;
	pInfo->strFileName	= pszFileName;
	pInfo->dwFileSize	= dwFileSize;
	m_aFileListInfo.Add (pInfo);
}


/* ========================================================================= */
/* �֐���	:CInfoFileList::GetCount										 */
/* ���e		:�f�[�^�����擾													 */
/* ���t		:2008/03/02														 */
/* ========================================================================= */

int CInfoFileList::GetCount(void)
{
	return m_aFileListInfo.GetSize ();
}


/* ========================================================================= */
/* �֐���	:CInfoFileList::GetPtr											 */
/* ���e		:�t�@�C�����X�g�����擾										 */
/* ���t		:2008/03/02														 */
/* ========================================================================= */

PFILELISTINFO CInfoFileList::GetPtr(int nNo)
{
	PFILELISTINFO pRet;

	pRet = NULL;
	if (nNo >= m_aFileListInfo.GetSize ()) {
		goto Exit;
	}

	pRet = m_aFileListInfo[nNo];
Exit:
	return pRet;
}


/* ========================================================================= */
/* �֐���	:CInfoFileList::DeleteAll										 */
/* ���e		:�S�č폜														 */
/* ���t		:2008/03/02														 */
/* ========================================================================= */

void CInfoFileList::DeleteAll(void)
{
	int i, nCount;
	PFILELISTINFO pInfo;

	nCount = m_aFileListInfo.GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfo = m_aFileListInfo[i];
		SAFE_DELETE (pInfo);
	}
	m_aFileListInfo.RemoveAll ();
}


/* ========================================================================= */
/* �֐���	:CInfoFileList::GetNew											 */
/* ���e		:�V�K�f�[�^�擾													 */
/* ���t		:2008/03/02														 */
/* ========================================================================= */

PFILELISTINFO CInfoFileList::GetNew(void)
{
	PFILELISTINFO pRet;

	pRet = new FILELISTINFO;
	pRet->dwFileSize = 0;

	return pRet;
}


/* ========================================================================= */
/* �֐���	:CInfoFileList::Copy											 */
/* ���e		:�R�s�[															 */
/* ���t		:2008/03/08														 */
/* ========================================================================= */

void CInfoFileList::Copy(CInfoFileList *pSrc)
{
	int i, nCount;
	PFILELISTINFO pInfo;

	DeleteAll ();

	nCount = pSrc->GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfo = pSrc->GetPtr (i);
		Add (pInfo->strMD5, pInfo->strFileName, pInfo->dwFileSize);
	}
}


/* ========================================================================= */
/* �֐���	:CInfoFileList::GetSendDataSize									 */
/* ���e		:���M�f�[�^�T�C�Y���擾											 */
/* ���t		:2008/03/08														 */
/* ========================================================================= */

DWORD CInfoFileList::GetSendDataSize(void)
{
	int i, nCount;
	DWORD dwRet;
	PFILELISTINFO pInfo;

	dwRet = 0;

	nCount = GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfo = GetPtr (i);
		dwRet += sizeof (pInfo->dwFileSize);				/* �t�@�C���T�C�Y */
		dwRet += (pInfo->strMD5.GetLength () + 1);			/* MD5�n�b�V�� */
		dwRet += (pInfo->strFileName.GetLength () + 1);		/* �t�@�C���� */
	}
	dwRet += sizeof (DWORD);

	return dwRet;
}


/* ========================================================================= */
/* �֐���	:CInfoFileList::GetSendData										 */
/* ���e		:���M�f�[�^���擾												 */
/* ���t		:2008/03/08														 */
/* ========================================================================= */

PBYTE CInfoFileList::GetSendData(void)
{
	PBYTE pRet, pData, pDataTmp;
	int i, nCount;
	DWORD dwSize, dwTmp;
	PFILELISTINFO pInfo;

	pRet = NULL;

	dwSize	= GetSendDataSize ();
	pData	= ZeroNew (dwSize);

	pDataTmp = pData;
	nCount   = GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfo = GetPtr (i);
		CopyMemoryRenew (pDataTmp, &pInfo->dwFileSize, sizeof (pInfo->dwFileSize), pDataTmp);	/* �t�@�C���T�C�Y */
		strcpyRenew ((LPSTR)pDataTmp, pInfo->strMD5,		pDataTmp);	/* MD5�n�b�V�� */
		strcpyRenew ((LPSTR)pDataTmp, pInfo->strFileName,	pDataTmp);	/* �t�@�C���� */
	}
	dwTmp = 0;
	CopyMemoryRenew (pDataTmp, &dwTmp, sizeof (dwTmp), pDataTmp);		/* �I�[ */

	pRet = pData;
	return pRet;
}


/* ========================================================================= */
/* �֐���	:CInfoFileList::SetSendData										 */
/* ���e		:���M�f�[�^���擾												 */
/* ���t		:2008/03/08														 */
/* ========================================================================= */

PBYTE CInfoFileList::SetSendData(PBYTE pSrc)
{
	PBYTE pRet, pDataTmp;
	DWORD dwSize;
	CmyString strHash, strFileName;

	pRet = pSrc;
	DeleteAll ();

	pDataTmp = pSrc;
	while (1) {
		CopyMemoryRenew (&dwSize, pDataTmp, sizeof (dwSize), pDataTmp);		/* �t�@�C���T�C�Y */
		if (dwSize == 0) {
			break;
		}
		StoreRenew (strHash,	 (LPCSTR)pDataTmp, pDataTmp);	/* MD5�n�b�V�� */
		StoreRenew (strFileName, (LPCSTR)pDataTmp, pDataTmp);	/* �t�@�C���� */
		Add (strHash, strFileName, dwSize);
	}

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2008 */
