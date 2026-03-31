/// @file InfoFileList.cpp
/// @brief ファイルリスト情報クラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/03/02
/// @copyright Copyright(C)URARA-works 2008

#include "StdAfx.h"
#include "InfoFileList.h"

CInfoFileList::CInfoFileList()
{
}

CInfoFileList::~CInfoFileList()
{
	DeleteAll();
}

void CInfoFileList::Add(LPCSTR pszHash, LPCSTR pszFileName, DWORD dwFileSize)
{
	PFILELISTINFO pInfo;

	pInfo = GetNew();
	pInfo->strMD5	= pszHash;
	pInfo->strFileName	= pszFileName;
	pInfo->dwFileSize	= dwFileSize;
	m_aFileListInfo.push_back(pInfo);
}

int CInfoFileList::GetCount(void)
{
	return m_aFileListInfo.size();
}

PFILELISTINFO CInfoFileList::GetPtr(int nNo)
{
	PFILELISTINFO pRet;

	pRet = NULL;
	if ((nNo < 0) || (nNo >= static_cast<int>(m_aFileListInfo.size()))) {
		goto Exit;
	}

	pRet = m_aFileListInfo[nNo];
Exit:
	return pRet;
}

void CInfoFileList::DeleteAll(void)
{
	int i, nCount;
	PFILELISTINFO pInfo;

	nCount = m_aFileListInfo.size();
	for (i = 0; i < nCount; i ++) {
		pInfo = m_aFileListInfo[i];
		SAFE_DELETE(pInfo);
	}
	m_aFileListInfo.clear();
}

PFILELISTINFO CInfoFileList::GetNew(void)
{
	PFILELISTINFO pRet;

	pRet = new FILELISTINFO;
	pRet->dwFileSize = 0;

	return pRet;
}

void CInfoFileList::Copy(CInfoFileList *pSrc)
{
	int i, nCount;
	PFILELISTINFO pInfo;

	DeleteAll();

	nCount = pSrc->GetCount();
	for (i = 0; i < nCount; i ++) {
		pInfo = pSrc->GetPtr(i);
		Add(pInfo->strMD5, pInfo->strFileName, pInfo->dwFileSize);
	}
}

DWORD CInfoFileList::GetSendDataSize(void)
{
	int i, nCount;
	DWORD dwRet;
	PFILELISTINFO pInfo;

	dwRet = 0;

	nCount = GetCount();
	for (i = 0; i < nCount; i ++) {
		pInfo = GetPtr(i);
		dwRet += sizeof(pInfo->dwFileSize);	// ファイルサイズ
		dwRet += (pInfo->strMD5.GetLegacyStoreLength() + 1);	// MD5ハッシュ
		dwRet += (pInfo->strFileName.GetLegacyStoreLength() + 1);	// ファイル名
	}
	dwRet += sizeof(DWORD);

	return dwRet;
}

PBYTE CInfoFileList::GetSendData(void)
{
	PBYTE pRet, pData, pDataTmp;
	int i, nCount;
	DWORD dwSize, dwTmp;
	PFILELISTINFO pInfo;

	pRet = NULL;

	dwSize	= GetSendDataSize();
	pData	= ZeroNew(dwSize);

	pDataTmp = pData;
	nCount   = GetCount();
	for (i = 0; i < nCount; i ++) {
		pInfo = GetPtr(i);
		CopyMemoryRenew(pDataTmp, &pInfo->dwFileSize, sizeof(pInfo->dwFileSize), pDataTmp);	// ファイルサイズ
		strcpyRenew((LPSTR)pDataTmp, pInfo->strMD5,	pDataTmp);	// MD5ハッシュ
		strcpyRenew((LPSTR)pDataTmp, pInfo->strFileName,	pDataTmp);	// ファイル名
	}
	dwTmp = 0;
	CopyMemoryRenew(pDataTmp, &dwTmp, sizeof(dwTmp), pDataTmp);	// 終端

	pRet = pData;
	return pRet;
}

PBYTE CInfoFileList::SetSendData(PBYTE pSrc)
{
	PBYTE pRet, pDataTmp;
	DWORD dwSize;
	CmyString strHash, strFileName;

	pRet = pSrc;
	DeleteAll();

	pDataTmp = pSrc;
	while (1) {
		CopyMemoryRenew(&dwSize, pDataTmp, sizeof(dwSize), pDataTmp);	// ファイルサイズ
		if (dwSize == 0) {
			break;
		}
		StoreRenew(strHash,	 (LPCSTR)pDataTmp, pDataTmp);	// MD5ハッシュ
		StoreRenew(strFileName, (LPCSTR)pDataTmp, pDataTmp);	// ファイル名
		Add(strHash, strFileName, dwSize);
	}

	pRet = pDataTmp;
	return pRet;
}

