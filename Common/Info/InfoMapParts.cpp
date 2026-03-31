/// @file InfoMapParts.cpp
/// @brief マップパーツクラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2005/04/25
/// @copyright Copyright(C)URARA-works 2005

#include "stdafx.h"
#include "InfoMapParts.h"

// ヘッダ情報
static LPCSTR s_aszName[] = {
	"byViewType",	// 表示種別
	"byAnimeType",	// アニメーション種別
	"byAnimeCount",	// アニメーションコマ数
	"byLevel",	// 透明度
	"byBlockDirection",	// 方向によるぶつかり判定
	"byMoveDirection",	// 乗ると移動する方向
	"wGrpIDBase",	// グラフィックID
	"wGrpIDPile",	// 重ね合わせ時の背景ID
	"dwPartsID",	// 編集画面での表示位置
	"dwPartsType",	// パーツの属性
	"ptViewPos",	// アニメーション情報
	NULL
};

CInfoMapParts::CInfoMapParts()
{
	m_byViewType	= 0;
	m_byAnimeType	= 0;
	m_byAnimeCount	= 0;
	m_byLevel	= 0;
	m_byBlockDirection	= 0;
	m_byMoveDirection	= 0;
	m_wGrpIDBase	= 0;
	m_wGrpIDPile	= 0;
	m_dwPartsID	= 0;
	m_dwPartsType	= 0;
	ZeroMemory(&m_ptViewPos, sizeof(m_ptViewPos));

	m_byAnimeNo	= 0;
	m_dwLastAnime	= 0;

	m_paAnimeInfo = new ARRAYANIMEINFO;

	for (m_nElementCount = 0; s_aszName[m_nElementCount] != NULL; m_nElementCount ++) {}
}

CInfoMapParts::~CInfoMapParts()
{
	if (m_paAnimeInfo) {
		DeleteAllAnime();
	}

	SAFE_DELETE(m_paAnimeInfo);
}

int CInfoMapParts::GetElementCount(void)
{
	int nRet;
	PCInfoAnime pAnimeTmp;

	pAnimeTmp = new CInfoAnime;

	nRet = m_nElementCount;
	nRet += pAnimeTmp->GetElementCount();

	SAFE_DELETE(pAnimeTmp);
	return nRet;
}

int CInfoMapParts::GetElementNo(LPCSTR pszName)
{
	int i, nRet;
	char szTmp[128];
	LPCSTR pszTmp;

	nRet = -1;

	for (i = 0; s_aszName[i] != NULL; i ++) {
		if (strcmp(s_aszName[i], pszName) == 0) {
			nRet = i;
			break;
		}
	}

	if (nRet < 0) {
		PCInfoAnime pAnimeTmp;

		pszTmp = strstr(pszName, PREFIX_INFOANIME);
		if (pszTmp == NULL) {
			goto Exit;
		}
		strcpy(szTmp, &pszName[strlen(PREFIX_INFOANIME)]);
		pAnimeTmp	= new CInfoAnime;
		nRet	= pAnimeTmp->GetElementNo(szTmp);
		SAFE_DELETE(pAnimeTmp);

		if (nRet < 0) {
			goto Exit;
		}
		nRet += i;
	}

Exit:
	return nRet;
}

DWORD CInfoMapParts::GetDataSize(void)
{
	int i, nCount;
	DWORD dwRet;

	dwRet = sizeof(m_byViewType)	+
			sizeof(m_byAnimeType)	+
			sizeof(m_byAnimeCount)	+
			sizeof(m_byLevel)	+
			sizeof(m_byBlockDirection)	+
			sizeof(m_byMoveDirection)	+
			sizeof(m_wGrpIDBase)	+
			sizeof(m_wGrpIDPile)	+
			sizeof(m_dwPartsID)	+
			sizeof(m_dwPartsType)	+
			sizeof(m_ptViewPos);

	if (m_byAnimeCount == 0) {
		goto Exit;
	}

	nCount = m_paAnimeInfo->size();
	for (i = 0; i < nCount; i ++) {
		PCInfoAnime pAnime;

		pAnime = m_paAnimeInfo->at(i);
		dwRet += pAnime->GetDataSize();
	}

Exit:
	return dwRet;
}

DWORD CInfoMapParts::GetDataSizeNo(int nNo)
{
	int i, nCount;
	DWORD dwRet;
	PCInfoAnime pAnime;

	dwRet = 0;

	switch (nNo) {
	case 0:	dwRet = sizeof(m_byViewType);	break;
	case 1:	dwRet = sizeof(m_byAnimeType);	break;
	case 2:	dwRet = sizeof(m_byAnimeCount);	break;
	case 3:	dwRet = sizeof(m_byLevel);	break;
	case 4:	dwRet = sizeof(m_byBlockDirection);	break;
	case 5:	dwRet = sizeof(m_byMoveDirection);	break;
	case 6:	dwRet = sizeof(m_wGrpIDBase);	break;
	case 7:	dwRet = sizeof(m_wGrpIDPile);	break;
	case 8:	dwRet = sizeof(m_dwPartsID);	break;
	case 9:	dwRet = sizeof(m_dwPartsType);	break;
	case 10:	dwRet = sizeof(m_ptViewPos);	break;
	default:
		nCount = m_paAnimeInfo->size();
		for (i = 0; i < nCount; i ++) {
			pAnime	= m_paAnimeInfo->at(i);
			dwRet	+= pAnime->GetDataSizeNo(nNo - m_nElementCount);
		}
		break;
	}

	return dwRet;
}

LPCSTR CInfoMapParts::GetName(int nNo)
{
	return s_aszName[nNo];
}

PBYTE CInfoMapParts::GetWriteData(int nNo, PDWORD pdwSize)
{
	int i, nCount;
	PBYTE pRet, pSrc;
	DWORD dwSize;
	PCInfoAnime pAnime;

	pRet	= NULL;
	pSrc	= NULL;
	dwSize	= GetDataSizeNo(nNo);
	*pdwSize	= dwSize;

	if (dwSize == 0) {
		goto Exit;
	}
	pRet = new BYTE[dwSize];

	switch (nNo) {
	case 0:	pSrc = &m_byViewType;	break;
	case 1:	pSrc = &m_byAnimeType;	break;
	case 2:	pSrc = &m_byAnimeCount;	break;
	case 3:	pSrc = &m_byLevel;	break;
	case 4:	pSrc = &m_byBlockDirection;	break;
	case 5:	pSrc = &m_byMoveDirection;	break;
	case 6:	pSrc = (PBYTE)&m_wGrpIDBase;	break;
	case 7:	pSrc = (PBYTE)&m_wGrpIDPile;	break;
	case 8:	pSrc = (PBYTE)&m_dwPartsID;	break;
	case 9:	pSrc = (PBYTE)&m_dwPartsType;	break;
	case 10:	pSrc = (PBYTE)&m_ptViewPos;	break;
	default:
		{
			PBYTE pTmp;

			pTmp = pRet;
			nCount = m_paAnimeInfo->size();
			for (i = 0; i < nCount; i ++) {
				PBYTE pSrcTmp;
				DWORD dwSizeTmp;

				pAnime	= m_paAnimeInfo->at(i);
				pSrcTmp	= pAnime->GetWriteData(nNo - m_nElementCount, &dwSizeTmp);
				CopyMemoryRenew(pTmp, pSrcTmp, dwSizeTmp, pTmp);
				SAFE_DELETE_ARRAY(pSrcTmp);
			}
		}
		break;
	}

	if (pSrc) {
		CopyMemory(pRet, pSrc, dwSize);
	}

Exit:
	return pRet;
}

DWORD CInfoMapParts::ReadElementData(
	PBYTE pSrc,	// [in] データの読み込み元
	int nNo)	// [in] 要素番号
{
	int i;
	PBYTE pDst, pSrcTmp;
	DWORD dwSize, dwSizeTmp;
	PCInfoAnime pAnime;

	pDst	= NULL;
	dwSize	= 0;

	switch (nNo) {
	case 0: pDst = &m_byViewType;	dwSize = sizeof(m_byViewType);	break;
	case 1:	pDst = &m_byAnimeType;	dwSize = sizeof(m_byAnimeType);	break;
	case 2:
		dwSize = sizeof(BYTE);
		CopyMemory(&m_byAnimeCount, pSrc, dwSize);
		for (i = 0; i < m_byAnimeCount; i ++) {
			pAnime = new CInfoAnime;
			m_paAnimeInfo->Add(pAnime);
		}
		break;
	case 3:	pDst = &m_byLevel;	dwSize = sizeof(m_byLevel);	break;
	case 4:	pDst = &m_byBlockDirection;	dwSize = sizeof(m_byBlockDirection);	break;
	case 5:	pDst = &m_byMoveDirection;	dwSize = sizeof(m_byMoveDirection);	break;
	case 6:	pDst = (PBYTE)&m_wGrpIDBase;	dwSize = sizeof(m_wGrpIDBase);	break;
	case 7:	pDst = (PBYTE)&m_wGrpIDPile;	dwSize = sizeof(m_wGrpIDPile);	break;
	case 8:	pDst = (PBYTE)&m_dwPartsID;	dwSize = sizeof(m_dwPartsID);	break;
	case 9:	pDst = (PBYTE)&m_dwPartsType;	dwSize = sizeof(m_dwPartsType);	break;
	case 10:	pDst = (PBYTE)&m_ptViewPos;	dwSize = sizeof(m_ptViewPos);	break;
	default:
		pSrcTmp	= pSrc;
		for (i = 0; i < m_byAnimeCount; i ++) {
			pAnime	= m_paAnimeInfo->at(i);
			dwSizeTmp	= pAnime->ReadElementData(pSrcTmp, nNo - m_nElementCount);
			dwSize	+= dwSizeTmp;
			pSrcTmp	+= dwSizeTmp;
		}
		break;
	}

	if (pDst) {
		CopyMemory(pDst, pSrc, dwSize);
	}

	return dwSize;
}

void CInfoMapParts::Copy(CInfoMapParts *pSrc)
{
	PBYTE pTmp;

	pTmp = pSrc->GetSendData();
	SetSendData(pTmp);
	SAFE_DELETE_ARRAY(pTmp);
}

DWORD CInfoMapParts::GetSendDataSize(void)
{
	int i, nCount;
	DWORD dwRet;
	PCInfoAnime pAnime;

	dwRet = sizeof(m_dwPartsID)	+
			sizeof(m_byViewType)	+
			sizeof(m_byAnimeType)	+
			sizeof(m_byAnimeCount)	+
			sizeof(m_byLevel)	+
			sizeof(m_byBlockDirection)	+
			sizeof(m_byMoveDirection)	+
			sizeof(m_wGrpIDBase)	+
			sizeof(m_wGrpIDPile)	+
			sizeof(m_dwPartsType)	+
			sizeof(m_ptViewPos);

	if (m_byAnimeCount == 0) {
		goto Exit;
	}

	nCount = m_paAnimeInfo->size();
	for (i = 0; i < nCount; i ++) {
		pAnime = m_paAnimeInfo->at(i);
		dwRet += pAnime->GetSendDataSize();
	}

Exit:
	return dwRet;
}

PBYTE CInfoMapParts::GetSendData(void)
{
	int i, nCount;
	PBYTE pData, pDataTmp, pDataAnimeTmp;
	DWORD dwSize, dwSizeAnime;
	PCInfoAnime pAnime;

	dwSize	= GetSendDataSize();
	pData	= new BYTE[dwSize];
	ZeroMemory(pData, dwSize);

	pDataTmp = pData;

	CopyMemoryRenew(pDataTmp, &m_dwPartsID,	sizeof(m_dwPartsID),	pDataTmp);
	CopyMemoryRenew(pDataTmp, &m_byViewType,	sizeof(m_byViewType),	pDataTmp);
	CopyMemoryRenew(pDataTmp, &m_byAnimeType,	sizeof(m_byAnimeType),	pDataTmp);
	CopyMemoryRenew(pDataTmp, &m_byAnimeCount,	sizeof(m_byAnimeCount),	pDataTmp);
	CopyMemoryRenew(pDataTmp, &m_byLevel,	sizeof(m_byLevel),	pDataTmp);
	CopyMemoryRenew(pDataTmp, &m_byBlockDirection,	sizeof(m_byBlockDirection),	pDataTmp);
	CopyMemoryRenew(pDataTmp, &m_byMoveDirection,	sizeof(m_byMoveDirection),	pDataTmp);
	CopyMemoryRenew(pDataTmp, &m_wGrpIDBase,	sizeof(m_wGrpIDBase),	pDataTmp);
	CopyMemoryRenew(pDataTmp, &m_wGrpIDPile,	sizeof(m_wGrpIDPile),	pDataTmp);
	CopyMemoryRenew(pDataTmp, &m_dwPartsType,	sizeof(m_dwPartsType),	pDataTmp);
	CopyMemoryRenew(pDataTmp, &m_ptViewPos,	sizeof(m_ptViewPos),	pDataTmp);

	nCount = m_paAnimeInfo->size();
	for (i = 0; i < nCount; i ++) {
		pAnime = m_paAnimeInfo->at(i);
		pDataAnimeTmp	= pAnime->GetSendData();
		dwSizeAnime	= pAnime->GetSendDataSize();
		CopyMemoryRenew(pDataTmp, pDataAnimeTmp, dwSizeAnime, pDataTmp);
		SAFE_DELETE_ARRAY(pDataAnimeTmp);
	}

	return pData;
}

PBYTE CInfoMapParts::SetSendData(PBYTE pSrc)
{
	int i, nCount;
	PBYTE pDataTmp;
	PCInfoAnime pAnime;

	DeleteAllAnime();
	pDataTmp = pSrc;

	CopyMemoryRenew(&m_dwPartsID,	pDataTmp, sizeof(m_dwPartsID),	pDataTmp);
	CopyMemoryRenew(&m_byViewType,	pDataTmp, sizeof(m_byViewType),	pDataTmp);
	CopyMemoryRenew(&m_byAnimeType,	pDataTmp, sizeof(m_byAnimeType),	pDataTmp);
	CopyMemoryRenew(&m_byAnimeCount,	pDataTmp, sizeof(m_byAnimeCount),	pDataTmp);
	CopyMemoryRenew(&m_byLevel,	pDataTmp, sizeof(m_byLevel),	pDataTmp);
	CopyMemoryRenew(&m_byBlockDirection,	pDataTmp, sizeof(m_byBlockDirection),	pDataTmp);
	CopyMemoryRenew(&m_byMoveDirection,	pDataTmp, sizeof(m_byMoveDirection),	pDataTmp);
	CopyMemoryRenew(&m_wGrpIDBase,	pDataTmp, sizeof(m_wGrpIDBase),	pDataTmp);
	CopyMemoryRenew(&m_wGrpIDPile,	pDataTmp, sizeof(m_wGrpIDPile),	pDataTmp);
	CopyMemoryRenew(&m_dwPartsType,	pDataTmp, sizeof(m_dwPartsType),	pDataTmp);
	CopyMemoryRenew(&m_ptViewPos,	pDataTmp, sizeof(m_ptViewPos),	pDataTmp);

	nCount = m_byAnimeCount;
	for (i = 0; i < nCount; i ++) {
		pAnime = new CInfoAnime;
		pAnime->SetSendData(pDataTmp);
		pDataTmp += pAnime->GetSendDataSize();
		m_paAnimeInfo->Add(pAnime);
	}

	return pDataTmp;
}

BOOL CInfoMapParts::IsBlock(
	int nDirection)	// [in] ぶつかってくる方向
{
	BOOL bRet;

	bRet = FALSE;

	switch (nDirection) {
	case 0:
		if (m_byBlockDirection & BIT_PARTSBLOCK_DOWN) {
			goto Exit;
		}
		break;
	case 1:
		if (m_byBlockDirection & BIT_PARTSBLOCK_UP) {
			goto Exit;
		}
		break;
	case 2:
		if (m_byBlockDirection & BIT_PARTSBLOCK_RIGHT) {
			goto Exit;
		}
		break;
	case 3:
		if (m_byBlockDirection & BIT_PARTSBLOCK_LEFT) {
			goto Exit;
		}
		break;
	case 4:
		if (m_byBlockDirection & BIT_PARTSBLOCK_DOWN) {
			goto Exit;
		}
		if (m_byBlockDirection & BIT_PARTSBLOCK_LEFT) {
			goto Exit;
		}
		break;
	case 5:
		if (m_byBlockDirection & BIT_PARTSBLOCK_UP) {
			goto Exit;
		}
		if (m_byBlockDirection & BIT_PARTSBLOCK_LEFT) {
			goto Exit;
		}
		break;
	case 6:
		if (m_byBlockDirection & BIT_PARTSBLOCK_UP) {
			goto Exit;
		}
		if (m_byBlockDirection & BIT_PARTSBLOCK_RIGHT) {
			goto Exit;
		}
		break;
	case 7:
		if (m_byBlockDirection & BIT_PARTSBLOCK_DOWN) {
			goto Exit;
		}
		if (m_byBlockDirection & BIT_PARTSBLOCK_RIGHT) {
			goto Exit;
		}
		break;
	}

	if (m_dwPartsType & BIT_PARTSHIT_BLOCK) {
		goto Exit;
	}

	bRet = TRUE;
Exit:
	return bRet;
}

BOOL CInfoMapParts::TimerProc(DWORD dwTime)
{
	BOOL bRet;
	BYTE byAnimeNoBack;
	PCInfoAnime pAnime;

	bRet = FALSE;
	byAnimeNoBack = m_byAnimeNo;

	if (m_byAnimeCount == 0) {
		goto Exit;
	}

	pAnime = m_paAnimeInfo->at(m_byAnimeNo);
	if (pAnime->m_byWait == 0) {
		goto Exit;
	}
	if (dwTime - m_dwLastAnime >= (DWORD)(pAnime->m_byWait * 10)) {
		if (m_dwLastAnime > 0) {
			m_byAnimeNo ++;
		}
		m_dwLastAnime = dwTime;
	}
	m_byAnimeNo = (m_byAnimeNo >= m_byAnimeCount) ? 0 : m_byAnimeNo;

Exit:
	if (byAnimeNoBack != m_byAnimeNo) {
		bRet = TRUE;
	}

	return bRet;
}

int CInfoMapParts::GetMoveDirection(void)
{
	int nRet;

	nRet = -1;

	if (m_byMoveDirection & BIT_PARTSMOVE_UP) {
		nRet = 0;
	} else if (m_byMoveDirection & BIT_PARTSMOVE_DOWN) {
		nRet = 1;
	} else if (m_byMoveDirection & BIT_PARTSMOVE_LEFT) {
		nRet = 2;
	} else if (m_byMoveDirection & BIT_PARTSMOVE_RIGHT) {
		nRet = 3;
	}

	return nRet;
}

int CInfoMapParts::GetAnimeCount(void)
{
	return m_paAnimeInfo->size();
}

void CInfoMapParts::AddAnime(void)
{
	PCInfoAnime pInfo;

	pInfo = new CInfoAnime;
	m_paAnimeInfo->Add(pInfo);
	m_byAnimeCount = static_cast<BYTE>(m_paAnimeInfo->size());
}

void CInfoMapParts::DeleteAnime(int nNo)
{
	PCInfoAnime pInfo;

	pInfo = m_paAnimeInfo->at(nNo);
	if ((nNo >= 0) && (nNo < static_cast<int>(m_paAnimeInfo->size()))) { m_paAnimeInfo->erase(m_paAnimeInfo->begin() + nNo); }
	SAFE_DELETE(pInfo);
	m_byAnimeCount = static_cast<BYTE>(m_paAnimeInfo->size());
}

void CInfoMapParts::DeleteAllAnime(void)
{
	int i, nCount;

	nCount = m_paAnimeInfo->size();
	for (i = 0; i < nCount; i ++) {
		DeleteAnime(0);
	}
}

PCInfoAnime CInfoMapParts::GetAnimePtr(int nNo)
{
	int nCount;
	PCInfoAnime pRet;

	pRet = NULL;

	nCount = m_paAnimeInfo->size();
	if (nNo >= nCount) {
		goto Exit;
	}

	pRet = m_paAnimeInfo->at(nNo);
Exit:
	return pRet;
}

