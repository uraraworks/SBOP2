/// @file SaveLoadInfoBase.cpp
/// @brief データ保存・読み込み基底クラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2005/04/26
/// @copyright Copyright(C)URARA-works 2005

#include "stdafx.h"
#include "SaveLoadInfoBase.h"

CSaveLoadInfoBase::CSaveLoadInfoBase()
{
	m_pData	= NULL;
	m_dwDataSize	= 0;
	m_dwHeaderSize	= 0;
	m_pLibInfoBase	= NULL;

	m_pHeaderInfo = new ARRAYHEADERINFO;
}

CSaveLoadInfoBase::~CSaveLoadInfoBase()
{
	DelAllHeaderInfo();

	SAFE_DELETE(m_pHeaderInfo);
	SAFE_DELETE_ARRAY(m_pData);
}

void CSaveLoadInfoBase::WriteData(void)
{
	HANDLE hFile;
	DWORD dwBytes;

	hFile = CreateFile(m_strFileName, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if (hFile == INVALID_HANDLE_VALUE) {
		return;
	}

	dwBytes = 0;
	WriteFile(hFile, m_pData, m_dwHeaderSize + m_dwDataSize + 8, &dwBytes, NULL);
	CloseHandle(hFile);
}

BOOL CSaveLoadInfoBase::ReadData(void)
{
	HANDLE hFile;
	DWORD dwBytes;
	BOOL bRet;

	bRet = FALSE;
	hFile = CreateFile(m_strFileName, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (hFile == INVALID_HANDLE_VALUE) {
		goto Exit;
	}

	// ファイルサイズ取得
	m_dwDataSize = GetFileSize(hFile, NULL);
	if ((int)m_dwDataSize == -1) {
		goto Exit;
	}

	m_pData = new BYTE[m_dwDataSize];
	// ファイル内容を全部読み込む
	ReadFile(hFile, m_pData, m_dwDataSize, &dwBytes, NULL);

	bRet = TRUE;
Exit:
	if (hFile != INVALID_HANDLE_VALUE) {
		CloseHandle(hFile);
	}

	return bRet;
}

void CSaveLoadInfoBase::Save(PCLibInfoBase pSrc)
{
	PCInfoBase pInfo;

	pInfo = NULL;
	if (pSrc == NULL) {
		goto Exit;
	}

	m_pLibInfoBase = pSrc;
	pInfo = m_pLibInfoBase->GetNew();

	SetHeaderInfo(pInfo);
	MakeWriteData();
	WriteData();

Exit:
	SAFE_DELETE(pInfo);
}

void CSaveLoadInfoBase::Load(PCLibInfoBase pDst)
{
	BOOL bResult;
	PCInfoBase pInfo;

	pInfo = NULL;
	if (pDst == NULL) {
		goto Exit;
	}

	m_pLibInfoBase = pDst;
	pInfo = m_pLibInfoBase->GetNew();

	SetHeaderInfo(pInfo);
	bResult = ReadData();
	if (bResult == FALSE) {
		goto Exit;
	}
	ReadHeader();

Exit:
	SAFE_DELETE(pInfo);
}

void CSaveLoadInfoBase::SetHeaderInfo(PCInfoBase pInfo)
{
	int i;
	LPCSTR pszName;

	for (i = 0; ; i ++) {
		pszName = pInfo->GetName(i);
		if (pszName == NULL) {
			break;
		}
		AddHeaderInfo(pszName);
	}
}

void CSaveLoadInfoBase::MakeWriteData(void)
{
	int i, j, nCount, nCountElement, nCountData;
	PBYTE pData, pTmp;
	DWORD dwSize, dwColumnCount, dwDataCount;
	PHEADERINFO pInfo, pInfoElement;
	PCInfoBase pInfoData;
	ARRAYINT anSaveNo;

	if (m_pLibInfoBase == NULL) {
		return;
	}

	m_pLibInfoBase->GetSaveNo(anSaveNo);

	// **************************
	// 各要素のサイズを設定
	// *************************

	m_dwDataSize	= 0;
	nCountElement	= m_pHeaderInfo->size();
	nCountData	= anSaveNo.size();

	for (i = 0; i < nCountElement; i ++) {
		pInfoElement = m_pHeaderInfo->at(i);
		pInfoElement->dwOffset	= 0;
		pInfoElement->dwDataSize	= 0;

		if (i > 0) {
			pInfoElement->dwOffset = m_dwDataSize;
		}
		pInfoElement->dwOffset += (m_dwHeaderSize + 8);

		for (j = 0; j < nCountData; j ++) {
			pInfoData = m_pLibInfoBase->GetPtr(anSaveNo[j]);
			pInfoElement->dwDataSize += pInfoData->GetDataSizeNo(i);
		}
		m_dwDataSize += pInfoElement->dwDataSize;
	}

	// ************************
	// 保存用データを確保
	// ***********************

	m_pData = new BYTE[m_dwHeaderSize + m_dwDataSize + 8];
	ZeroMemory(m_pData, m_dwHeaderSize + m_dwDataSize + 8);
	pData = m_pData;

	dwColumnCount	= m_pHeaderInfo->size();
	dwDataCount	= anSaveNo.size();

	// カラム数を書き込み
	CopyMemoryDataDst(pData, (PBYTE)&dwColumnCount, sizeof (dwColumnCount));
	// データ数を書き込み
	CopyMemoryDataDst(pData, (PBYTE)&dwDataCount, sizeof (dwDataCount));

	// ヘッダ部分を書き込み
	nCount = (int)dwColumnCount;
	for (i = 0; i < nCount; i ++) {
		pInfo = m_pHeaderInfo->at(i);

		strcpy((LPSTR)pData, pInfo->szName);
		pData += (strlen(pInfo->szName) + 1);

		CopyMemoryDataDst(pData, (PBYTE)&pInfo->dwOffset, sizeof (pInfo->dwOffset));
	}

	// データ部分を作成
	for (i = 0; i < nCountElement; i ++) {
		for (j = 0; j < (int)dwDataCount; j ++) {
			dwSize	= 0;
			pInfoData	= m_pLibInfoBase->GetPtr(anSaveNo[j]);
			pTmp	= pInfoData->GetWriteData(i, &dwSize);
			CopyMemoryDataDst(pData, pTmp, dwSize);
			SAFE_DELETE_ARRAY(pTmp);
		}
	}
}

void CSaveLoadInfoBase::ReadHeader(void)
{
	int nNo;
	PBYTE pData, pDataBack;
	DWORD i, j, dwCountElement, dwDataCount, dwSize;
	PHEADERINFO pInfoElement, pInfoTmp;
	PCInfoBase pInfo;

	pData	= m_pData;
	pInfoTmp	= new HEADERINFO;
	pInfoElement	= NULL;

	// カラム数を読み込み
	CopyMemoryDataSrc((PBYTE)&dwCountElement, pData, sizeof (dwCountElement));
	// データ数を読み込み
	CopyMemoryDataSrc((PBYTE)&dwDataCount, pData, sizeof (dwDataCount));
	// ヘッダ情報を読み込み
	for (i = 0; i < dwCountElement; i ++) {
		ZeroMemory(pInfoTmp, sizeof (HEADERINFO));
		strcpy(pInfoTmp->szName, (LPCSTR)pData);
		pData += (strlen(pInfoTmp->szName) + 1);

		CopyMemoryDataSrc((PBYTE)&pInfoTmp->dwOffset, pData, sizeof (pInfoTmp->dwOffset));
		pInfoElement = GetHeaderInfo(pInfoTmp->szName);
		if (pInfoElement == NULL) {
			continue;
		}
		pInfoElement->dwOffset = pInfoTmp->dwOffset;
	}

	// 空のデータを作成
	for (i = 0; i < dwDataCount; i ++) {
		pInfo = m_pLibInfoBase->GetNew();
		m_pLibInfoBase->Add(pInfo);
	}

	// データ先頭部分のアドレスを保存
	pDataBack = m_pData;

	// **********************
	// データを読み込み
	// *********************

	dwCountElement = m_pHeaderInfo->size();
	for (i = 0; i < dwCountElement; i ++) {
		pInfoElement = m_pHeaderInfo->at(i);
		if (pInfoElement->dwOffset == 0) {
			continue;
		}
		pData = pDataBack + pInfoElement->dwOffset;

		for (j = 0; j < dwDataCount; j ++) {
			pInfo	= m_pLibInfoBase->RenewInfo(j);
			nNo	= pInfo->GetElementNo(pInfoElement->szName);
			dwSize	= pInfo->ReadElementData(pData, nNo);
			pData	+= dwSize;
			if ((DWORD)(pData - m_pData) >= m_dwDataSize) {
				break;
			}
		}
		if ((DWORD)(pData - m_pData) >= m_dwDataSize) {
			break;
		}
	}
	m_pLibInfoBase->RenewIDPtr();

	SAFE_DELETE(pInfoTmp);
}

void CSaveLoadInfoBase::SetFileName(LPCSTR pszName)
{
	char szName[MAX_PATH];
	LPSTR pszTmp;

	// ファイル名の作成
	GetModuleFileNameA(NULL, szName, MAX_PATH);
	pszTmp = strrchr(szName, '\\');
	pszTmp[1] = 0;
	strcat(szName, pszName);

	m_strFileName = szName;
}

void CSaveLoadInfoBase::AddHeaderInfo(LPCSTR pszName)
{
	PHEADERINFO pInfo;

	pInfo = new HEADERINFO;
	ZeroMemory(pInfo, sizeof (HEADERINFO));

	strcpy(pInfo->szName, pszName);
	m_dwHeaderSize += sizeof (DWORD);
	m_dwHeaderSize += (strlen(pszName) + 1);

	m_pHeaderInfo->push_back(pInfo);
}

PHEADERINFO CSaveLoadInfoBase::GetHeaderInfo(LPCSTR pszName)
{
	int i, nCount;
	PHEADERINFO pRet, pInfo;

	pRet	= NULL;
	nCount	= m_pHeaderInfo->size();

	for (i = 0; i < nCount; i ++) {
		pInfo = m_pHeaderInfo->at(i);
		if (strcmp(pInfo->szName, pszName) == 0) {
			pRet = pInfo;
			break;
		}
	}

	return pRet;
}

void CSaveLoadInfoBase::DelAllHeaderInfo(void)
{
	int i, nCount;

	if (m_pHeaderInfo) {
		nCount = m_pHeaderInfo->size();
		for (i = 0; i < nCount; i ++) {
			PHEADERINFO pInfo;

			pInfo = m_pHeaderInfo->at(i);
			SAFE_DELETE(pInfo);
		}
		m_pHeaderInfo->clear();
		m_dwHeaderSize = 0;
	}
}

void CSaveLoadInfoBase::CopyMemoryDataDst(PBYTE &pDst, PBYTE pSrc, DWORD dwSize)
{
	CopyMemory(pDst, pSrc, dwSize);
	pDst += dwSize;
}

void CSaveLoadInfoBase::CopyMemoryDataSrc(PBYTE pDst, PBYTE &pSrc, DWORD dwSize)
{
	CopyMemory(pDst, pSrc, dwSize);
	pSrc += dwSize;
}
