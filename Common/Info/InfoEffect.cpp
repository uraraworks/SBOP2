/// @file InfoEffect.cpp
/// @brief エフェクトクラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/07/26
/// @copyright Copyright(C)URARA-works 2007

#include "stdafx.h"
#include "InfoEffect.h"

// ヘッダ情報
static LPCSTR s_aszName[] = {
	"byAnimeCount",	// アニメーションコマ数
	"dwEffectID",	// エフェクトID
	"dwSoundID",	// 効果音ID
	"bLoop",	// ループ判定
	"m_bLoopSound",	// ループ時に効果音を再生する
	"m_dwGrpIDMain",	// 画像メインID
	"m_strName",	// エフェクト名
	NULL
};

CInfoEffect::CInfoEffect()
{
	m_bLoop	= FALSE;
	m_bLoopSound	= FALSE;
	m_byAnimeCount	= 0;
	m_dwEffectID	= 0;
	m_dwSoundID	= 0;
	m_dwGrpIDMain	= GRPIDMAIN_EFFECT32;

	m_bAnimeEnd	= FALSE;
	m_byAnimeNo	= 0;
	m_dwLastAnime	= 0;

	m_paAnimeInfo = new ARRAYANIMEINFO;

	for (m_nElementCount = 0; s_aszName[m_nElementCount] != NULL; m_nElementCount ++) {}
}

CInfoEffect::~CInfoEffect()
{
	if (m_paAnimeInfo) {
		DeleteAllAnime();
	}

	SAFE_DELETE(m_paAnimeInfo);
}

int CInfoEffect::GetElementCount(void)
{
	int nRet;
	PCInfoAnime pAnimeTmp;

	pAnimeTmp = new CInfoAnime;

	nRet = m_nElementCount;
	nRet += pAnimeTmp->GetElementCount();

	SAFE_DELETE(pAnimeTmp);
	return nRet;
}

int CInfoEffect::GetElementNo(LPCSTR pszName)
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

DWORD CInfoEffect::GetDataSize(void)
{
	int i, nCount;
	DWORD dwRet;

	dwRet = sizeof(m_bLoop)	+
			sizeof(m_bLoopSound)	+
			sizeof(m_byAnimeCount)	+
			sizeof(m_dwEffectID)	+
			sizeof(m_dwSoundID)	+
			sizeof(m_dwGrpIDMain)	+
			(m_strName.GetLegacyStoreLength() + 1);

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

DWORD CInfoEffect::GetDataSizeNo(int nNo)
{
	int i, nCount;
	DWORD dwRet;
	PCInfoAnime pAnime;

	dwRet = 0;

	switch (nNo) {
	case 0:	dwRet = sizeof(m_byAnimeCount);	break;	// アニメーションコマ数
	case 1:	dwRet = sizeof(m_dwEffectID);	break;	// エフェクトID
	case 2:	dwRet = sizeof(m_dwSoundID);	break;	// 効果音ID
	case 3:	dwRet = sizeof(m_bLoop);	break;	// ループ判定
	case 4:	dwRet = sizeof(m_bLoopSound);	break;	// ループ時に効果音を再生する
	case 5:	dwRet = sizeof(m_dwGrpIDMain);	break;	// 画像メインID
	case 6:	dwRet = (m_strName.GetLegacyStoreLength() + 1);	break;	// エフェクト名
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

LPCSTR CInfoEffect::GetName(int nNo)
{
	return s_aszName[nNo];
}

PBYTE CInfoEffect::GetWriteData(int nNo, PDWORD pdwSize)
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
	case 0:	pSrc = &m_byAnimeCount;	break;	// アニメーションコマ数
	case 1:	pSrc = (PBYTE)&m_dwEffectID;	break;	// エフェクトID
	case 2:	pSrc = (PBYTE)&m_dwSoundID;	break;	// 効果音ID
	case 3:	pSrc = (PBYTE)&m_bLoop;	break;	// ループ判定
	case 4:	pSrc = (PBYTE)&m_bLoopSound;	break;	// ループ時に効果音を再生する
	case 5:	pSrc = (PBYTE)&m_dwGrpIDMain;	break;	// 画像メインID
	case 6:	pSrc = (PBYTE)(LPCSTR)m_strName;	break;	// エフェクト名
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

DWORD CInfoEffect::ReadElementData(
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
	case 0:
		dwSize = sizeof(BYTE);
		CopyMemory(&m_byAnimeCount, pSrc, dwSize);
		for (i = 0; i < m_byAnimeCount; i ++) {
			pAnime = new CInfoAnime;
			m_paAnimeInfo->Add(pAnime);
		}
		break;
	case 1:	pDst = (PBYTE)&m_dwEffectID;	dwSize = sizeof(m_dwEffectID);	break;	// エフェクトID
	case 2:	pDst = (PBYTE)&m_dwSoundID;	dwSize = sizeof(m_dwSoundID);	break;	// 効果音ID
	case 3:	pDst = (PBYTE)&m_bLoop;	dwSize = sizeof(m_bLoop);	break;	// ループ判定
	case 4:	pDst = (PBYTE)&m_bLoopSound;	dwSize = sizeof(m_bLoopSound);	break;	// ループ時に効果音を再生する
	case 5:	pDst = (PBYTE)&m_dwGrpIDMain;	dwSize = sizeof(m_dwGrpIDMain);	break;	// 画像メインID
	case 6:	// エフェクト名
		m_strName = (LPCTSTR)LegacyAnsiToTString((LPCSTR)pSrc);
		dwSize = (DWORD)(strlen((LPCSTR)pSrc) + 1);
		break;
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

void CInfoEffect::Copy(CInfoEffect *pSrc)
{
	PBYTE pTmp;

	pTmp = pSrc->GetSendData();
	SetSendData(pTmp);
	SAFE_DELETE_ARRAY(pTmp);
}

DWORD CInfoEffect::GetSendDataSize(void)
{
	int i, nCount;
	DWORD dwRet;
	PCInfoAnime pAnime;

	dwRet = sizeof(m_bLoop)	+
			sizeof(m_bLoopSound)	+
			sizeof(m_dwEffectID)	+
			sizeof(m_dwSoundID)	+
			sizeof(m_dwGrpIDMain)	+
			sizeof(m_byAnimeCount)	+
			(m_strName.GetLegacyStoreLength() + 1);

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

PBYTE CInfoEffect::GetSendData(void)
{
	int i, nCount;
	PBYTE pData, pDataTmp, pDataAnimeTmp;
	DWORD dwSize, dwSizeAnime;
	PCInfoAnime pAnime;

	dwSize	= GetSendDataSize();
	pData	= new BYTE[dwSize];
	ZeroMemory(pData, dwSize);

	pDataTmp = pData;

	CopyMemoryRenew(pDataTmp, &m_dwEffectID,	sizeof(m_dwEffectID),	pDataTmp);	// エフェクトID
	CopyMemoryRenew(pDataTmp, &m_dwSoundID,	sizeof(m_dwSoundID),	pDataTmp);	// 効果音ID
	CopyMemoryRenew(pDataTmp, &m_dwGrpIDMain,	sizeof(m_dwGrpIDMain),	pDataTmp);	// 画像メインID
	CopyMemoryRenew(pDataTmp, &m_byAnimeCount,	sizeof(m_byAnimeCount),	pDataTmp);	// アニメーションコマ数
	CopyMemoryRenew(pDataTmp, &m_bLoop,	sizeof(m_bLoop),	pDataTmp);	// ループ判定
	CopyMemoryRenew(pDataTmp, &m_bLoopSound,	sizeof(m_bLoopSound),	pDataTmp);	// ループ時に効果音を再生する
	strcpyRenew((LPSTR)pDataTmp, m_strName, pDataTmp);	// エフェクト名

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

PBYTE CInfoEffect::SetSendData(PBYTE pSrc)
{
	int i, nCount;
	PBYTE pDataTmp;
	PCInfoAnime pAnime;

	DeleteAllAnime();
	pDataTmp = pSrc;

	CopyMemoryRenew(&m_dwEffectID,	pDataTmp, sizeof(m_dwEffectID),	pDataTmp);	// エフェクトID
	CopyMemoryRenew(&m_dwSoundID,	pDataTmp, sizeof(m_dwSoundID),	pDataTmp);	// 効果音ID
	CopyMemoryRenew(&m_dwGrpIDMain,	pDataTmp, sizeof(m_dwGrpIDMain),	pDataTmp);	// 画像メインID
	CopyMemoryRenew(&m_byAnimeCount,	pDataTmp, sizeof(m_byAnimeCount),	pDataTmp);	// アニメーションコマ数
	CopyMemoryRenew(&m_bLoop,	pDataTmp, sizeof(m_bLoop),	pDataTmp);	// ループ判定
	CopyMemoryRenew(&m_bLoopSound,	pDataTmp, sizeof(m_bLoopSound),	pDataTmp);	// ループ時に効果音を再生する
	StoreRenew(m_strName, (LPCSTR)pDataTmp, pDataTmp);	// エフェクト名

	nCount = m_byAnimeCount;
	for (i = 0; i < nCount; i ++) {
		pAnime = new CInfoAnime;
		pAnime->SetSendData(pDataTmp);
		pDataTmp += pAnime->GetSendDataSize();
		m_paAnimeInfo->Add(pAnime);
	}

	return pDataTmp;
}

BOOL CInfoEffect::TimerProc(DWORD dwTime)
{
	BOOL bRet;
	BYTE byAnimeNoBack;
	DWORD dwTmp;
	PCInfoAnime pAnime;

	bRet = FALSE;
	byAnimeNoBack = m_byAnimeNo;

	if (m_byAnimeCount == 0) {
		goto Exit;
	}

	pAnime = m_paAnimeInfo->at(m_byAnimeNo);
	dwTmp  = (DWORD)pAnime->m_byWait * 10;
	if (dwTime - m_dwLastAnime >= dwTmp) {
		if (m_dwLastAnime > 0) {
			m_byAnimeNo ++;
		}
		m_dwLastAnime = dwTime;
	}
	if (m_byAnimeNo >= m_byAnimeCount) {
		if (m_bLoop) {
			m_byAnimeNo = 0;
		} else {
			m_byAnimeNo = m_byAnimeCount - 1;
			m_bAnimeEnd = TRUE;
		}
	}

Exit:
	if (byAnimeNoBack != m_byAnimeNo) {
		bRet = TRUE;
	}

	return bRet;
}

int CInfoEffect::GetAnimeCount(void)
{
	return m_paAnimeInfo->size();
}

void CInfoEffect::AddAnime(void)
{
	PCInfoAnime pInfo;

	pInfo = new CInfoAnime;
	m_paAnimeInfo->Add(pInfo);
	m_byAnimeCount = static_cast<BYTE>(m_paAnimeInfo->size());
}

void CInfoEffect::DeleteAnime(int nNo)
{
	PCInfoAnime pInfo;

	pInfo = m_paAnimeInfo->at(nNo);
	if ((nNo >= 0) && (nNo < static_cast<int>(m_paAnimeInfo->size()))) { m_paAnimeInfo->erase(m_paAnimeInfo->begin() + nNo); }
	SAFE_DELETE(pInfo);
	m_byAnimeCount = static_cast<BYTE>(m_paAnimeInfo->size());
}

void CInfoEffect::DeleteAllAnime(void)
{
	int i, nCount;

	nCount = m_paAnimeInfo->size();
	for (i = 0; i < nCount; i ++) {
		DeleteAnime(0);
	}
}

PCInfoAnime CInfoEffect::GetAnimePtr(int nNo)
{
	int nCount;
	PCInfoAnime pRet;

	pRet = NULL;

	nCount = m_paAnimeInfo->size();
	if ((nNo < 0) || (nNo >= nCount)) {
		goto Exit;
	}

	pRet = m_paAnimeInfo->at(nNo);
Exit:
	return pRet;
}

WORD CInfoEffect::GetGrpID(void)
{
	WORD wRet;
	PCInfoAnime pAnime;

	wRet = 0;

	if (m_byAnimeCount == 0) {
		goto Exit;
	}

	pAnime = m_paAnimeInfo->at(m_byAnimeNo);
	if (pAnime == NULL) {
		goto Exit;
	}

	wRet = pAnime->m_wGrpIDBase;
Exit:
	return wRet;
}

BYTE CInfoEffect::GetLevel(void)
{
	BYTE byRet;
	PCInfoAnime pAnime;

	byRet = 0;

	if (m_byAnimeCount == 0) {
		goto Exit;
	}

	pAnime = m_paAnimeInfo->at(m_byAnimeNo);
	if (pAnime == NULL) {
		goto Exit;
	}

	byRet = pAnime->m_byLevel;
Exit:
	return byRet;
}

BOOL CInfoEffect::IsAnimeEnd(void)
{
	return m_bAnimeEnd;
}

