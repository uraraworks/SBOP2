/// @file InfoMapBase.cpp
/// @brief マップ情報基底クラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2006/10/08
/// @copyright Copyright(C)URARA-works 2006

#include "stdafx.h"
#include "LibInfoMapParts.h"
#include "LibInfoMapEvent.h"
#include "LibInfoMapObject.h"
#include "LibInfoMapObjectData.h"
#include "InfoMapEvent.h"
#include "InfoMapBase.h"

// ヘッダ情報
static LPCSTR s_aszName[] = {
	"sizeMap",	// マップサイズ
	"dwMapID",	// マップID
	"m_dwBGMID",	// BGMID
	"m_dwWeatherType",	// 天気種別
	"m_bEnableBattle",	// 戦闘許可
	"m_bRecovery",	// 気絶後回復する
	"m_byLevel",	// 暗さレベル
	"pwMap",	// マップ
	"pwMapPile",	// マップ重ね合わせ
	"pwMapShadow",	// マップ影
	"strMapName",	// マップ名
	"pLibInfoMapEvent",	// マップイベント情報
	"pLibInfoMapObjectData",	// マップオブジェクト配置データ
	NULL
};

CInfoMapBase::CInfoMapBase()
{
	m_sizeMap.cx = m_sizeMap.cy = 0;
	m_dwMapID	= 0;
	m_dwBGMID	= 0;
	m_dwWeatherType	= 0;
	m_bEnableBattle	= TRUE;	// 戦闘許可
	m_bRecovery	= TRUE;	// 気絶後回復する
	m_pbyMapEvent	= NULL;
	m_pbyHitTmp	= NULL;
	m_pwMap	= NULL;
	m_pwMapPile	= NULL;
	m_pwMapShadow	= NULL;
	m_byLevel	= 0;

	m_pLibInfoMapParts	= NULL;
	m_pLibInfoMapEvent	= NULL;
	m_pLibInfoMapObjectData	= NULL;
	m_pLibInfoMapObject	= NULL;

	for (m_nElementCount = 0; s_aszName[m_nElementCount] != NULL; m_nElementCount ++) {}
}

CInfoMapBase::~CInfoMapBase()
{
	SAFE_DELETE_ARRAY(m_pbyMapEvent);
	SAFE_DELETE_ARRAY(m_pbyHitTmp);
	SAFE_DELETE_ARRAY(m_pwMap);
	SAFE_DELETE_ARRAY(m_pwMapPile);
	SAFE_DELETE_ARRAY(m_pwMapShadow);
	SAFE_DELETE(m_pLibInfoMapEvent);
	SAFE_DELETE(m_pLibInfoMapObjectData);
}

void CInfoMapBase::Create(CLibInfoMapParts *pLibInfoMapParts)
{
	m_pLibInfoMapParts = pLibInfoMapParts;
	m_pLibInfoMapEvent = new CLibInfoMapEvent;
	m_pLibInfoMapEvent->Create();
	m_pLibInfoMapObjectData = new CLibInfoMapObjectData;
	m_pLibInfoMapObjectData->Create();
}

void CInfoMapBase::Init(
	int cx,	// [in] 横幅
	int cy,	// [in] 縦幅
	WORD wParts,	// [in] 塗りつぶすパーツID
	BOOL bDeleteMapEvent/*=TRUE*/)	// [in] マップイベントを初期化する
{
	int x, y;

	SAFE_DELETE_ARRAY(m_pwMap);
	SAFE_DELETE_ARRAY(m_pwMapPile);
	SAFE_DELETE_ARRAY(m_pwMapShadow);
	SAFE_DELETE_ARRAY(m_pbyHitTmp);
	if (bDeleteMapEvent) {
		SAFE_DELETE(m_pLibInfoMapEvent);
		SAFE_DELETE(m_pLibInfoMapObjectData);
	}
	m_sizeMap.cx = cx;
	m_sizeMap.cy = cy;

	if ((cx == 0) && (cy == 0)) {
		return;
	}

	m_pwMap	= new WORD[cx * cy];
	m_pwMapPile	= new WORD[cx * cy];
	m_pwMapShadow	= new WORD[cx * cy];
	m_pbyHitTmp	= new BYTE[cx * cy];
	if (bDeleteMapEvent) {
		m_pLibInfoMapEvent	= new CLibInfoMapEvent;
		m_pLibInfoMapEvent->Create();
		m_pLibInfoMapObjectData = new CLibInfoMapObjectData;
		m_pLibInfoMapObjectData->Create();
	}

	for (y = 0; y < cy; y ++) {
		for (x = 0; x < cx; x ++) {
			m_pwMap[cx * y + x]	= wParts;
			m_pwMapPile[cx * y + x]	= 0;
			m_pwMapShadow[cx * y + x]	= 0;
		}
	}

	RenewHitTmp();
}

void CInfoMapBase::RenewSize(int nDirection, int nSize)
{
	int i, nCount, x, y, nSrcStartX, nSrcEndX, nDstStartX, nDstEndX, nSrcStartY, nSrcEndY, nDstStartY, nDstEndY,
		anPosX[] = {0, 0, 1, 0}, anPosY[] = {1, 0, 0, 0};
	PWORD pwMapBack, pwMapPileBack, pwMapShadow;
	SIZE sizeBack;
	PCInfoMapEventBase pInfoMapEvent;
	PCInfoMapObjectData pInfoMapObjectData;

	if ((nDirection < 0) || (nDirection >= 4)) {
		return;
	}

	sizeBack	= m_sizeMap;
	pwMapBack	= m_pwMap;
	pwMapPileBack	= m_pwMapPile;
	pwMapShadow	= m_pwMapShadow;
	m_pwMap	= NULL;
	m_pwMapPile	= NULL;
	m_pwMapShadow	= NULL;

	nSrcStartX	= 0;
	nSrcStartY	= 0;
	nSrcEndX	= sizeBack.cx;
	nSrcEndY	= sizeBack.cy;
	nDstStartX	= 0;
	nDstStartY	= 0;
	nDstEndX	= sizeBack.cx;
	nDstEndY	= sizeBack.cy;

	switch (nDirection) {
	case 0:	// 上
		Init(sizeBack.cx, sizeBack.cy + nSize, 0, FALSE);
		if (nSize < 0) {
			nSrcStartY	= nSize * -1;
			nSrcEndY	+= nSize;
		} else {
			nDstStartY	= nSize;
		}
		break;
	case 1:	// 下
		Init(sizeBack.cx, sizeBack.cy + nSize, 0, FALSE);
		if (nSize < 0) {
			nSrcEndY = sizeBack.cy + nSize;
		}
		break;
	case 2:	// 左
		Init(sizeBack.cx + nSize, sizeBack.cy, 0, FALSE);
		if (nSize < 0) {
			nSrcStartX	= nSize * -1;
			nSrcEndX	+= nSize;
		} else {
			nDstStartX	= nSize;
		}
		break;
	case 3:	// 右
		Init(sizeBack.cx + nSize, sizeBack.cy, 0, FALSE);
		if (nSize < 0) {
			nSrcEndX = sizeBack.cx + nSize;
		}
		break;
	}
	// ずらす位置を考慮しながら新しいマップへコピー
	for (y = 0; y < nSrcEndY; y ++) {
		for (x = 0; x < nSrcEndX; x ++) {
			m_pwMap[(m_sizeMap.cx * (nDstStartY + y)) + x + nDstStartX] =
					pwMapBack[(sizeBack.cx * (nSrcStartY + y)) + x + nSrcStartX];
			m_pwMapPile[(m_sizeMap.cx * (nDstStartY + y)) + x + nDstStartX] =
					pwMapPileBack[(sizeBack.cx * (nSrcStartY + y)) + x + nSrcStartX];
			m_pwMapShadow[(m_sizeMap.cx * (nDstStartY + y)) + x + nDstStartX] =
					pwMapShadow[(sizeBack.cx * (nSrcStartY + y)) + x + nSrcStartX];
		}
	}
	SAFE_DELETE_ARRAY(pwMapBack);
	SAFE_DELETE_ARRAY(pwMapPileBack);
	SAFE_DELETE_ARRAY(pwMapShadow);

	nCount = m_pLibInfoMapEvent->GetCount();
	for (i = 0; i < nCount; i ++) {
		pInfoMapEvent = (PCInfoMapEventBase)m_pLibInfoMapEvent->GetPtr(i);
		pInfoMapEvent->RenewSize(nDirection, nSize, &m_sizeMap);
	}
	nCount = m_pLibInfoMapObjectData->GetCount();
	for (i = 0; i < nCount; i ++) {
		pInfoMapObjectData = (PCInfoMapObjectData)m_pLibInfoMapObjectData->GetPtr(i);
		pInfoMapObjectData->RenewSize(nDirection, nSize, &m_sizeMap);
	}

	RenewMapEvent();
	RenewHitTmp();
}

void CInfoMapBase::FillShadow(int x, int y, int cx, int cy, WORD wShadowID)
{
	int xx, yy;

	for (yy = 0; yy < cy; yy ++) {
		for (xx = 0; xx < cx; xx ++) {
			if (x + xx >= m_sizeMap.cx) {
				continue;
			}
			if (y + yy >= m_sizeMap.cy) {
				continue;
			}
			m_pwMapShadow[cx * (y + yy) + x + xx] = wShadowID;
		}
	}
}

int CInfoMapBase::GetElementNo(LPCSTR pszName)
{
	int i, nRet;

	nRet = -1;

	for (i = 0; s_aszName[i] != NULL; i ++) {
		if (strcmp(s_aszName[i], pszName) == 0) {
			nRet = i;
			break;
		}
	}

	return nRet;
}

DWORD CInfoMapBase::GetDataSize(void)
{
	DWORD dwRet;

	dwRet = 0;
	dwRet += sizeof(m_sizeMap);
	dwRet += sizeof(m_dwMapID);
	dwRet += sizeof(m_dwBGMID);	// BGMID
	dwRet += sizeof(m_dwWeatherType);	// 天気種別
	dwRet += sizeof(m_bEnableBattle);	// 戦闘許可
	dwRet += sizeof(m_bRecovery);	// 気絶後回復する
	dwRet += sizeof(m_byLevel);	// 暗さレベル
	dwRet += ((m_sizeMap.cx * sizeof(WORD)) * m_sizeMap.cy);
	dwRet += ((m_sizeMap.cx * sizeof(WORD)) * m_sizeMap.cy);
	dwRet += ((m_sizeMap.cx * sizeof(WORD)) * m_sizeMap.cy);
	dwRet += (m_strMapName.GetLegacyStoreLength() + 1);
	if (m_pLibInfoMapEvent) {
		dwRet += m_pLibInfoMapEvent->GetDataSize();
	}
	if (m_pLibInfoMapObjectData) {
		dwRet += m_pLibInfoMapObjectData->GetDataSize();
	}

	return dwRet;
}

DWORD CInfoMapBase::GetDataSizeNo(int nNo)
{
	DWORD dwRet;

	dwRet = 0;

	switch (nNo) {
	case 0:	dwRet = sizeof(m_sizeMap);	break;
	case 1:	dwRet = sizeof(m_dwMapID);	break;
	case 2:	dwRet = sizeof(m_dwBGMID);	break;	// BGMID
	case 3:	dwRet = sizeof(m_dwWeatherType);	break;	// 天気種別
	case 4:	dwRet = sizeof(m_bEnableBattle);	break;	// 戦闘許可
	case 5:	dwRet = sizeof(m_bRecovery);	break;	// 気絶後回復する
	case 6:	dwRet = sizeof(m_byLevel);	break;	// 暗さレベル
	case 7:	dwRet = (m_sizeMap.cx * sizeof(WORD)) * m_sizeMap.cy;	break;
	case 8:	dwRet = (m_sizeMap.cx * sizeof(WORD)) * m_sizeMap.cy;	break;
	case 9:	dwRet = (m_sizeMap.cx * sizeof(WORD)) * m_sizeMap.cy;	break;
	case 10:dwRet = m_strMapName.GetLegacyStoreLength() + 1;	break;
	case 11:
		if (m_pLibInfoMapEvent) {
			dwRet = m_pLibInfoMapEvent->GetDataSize();
		}
		break;
	case 12:
		if (m_pLibInfoMapObjectData) {
			dwRet = m_pLibInfoMapObjectData->GetDataSize();
		}
		break;
	}

	return dwRet;
}

LPCSTR CInfoMapBase::GetName(int nNo)
{
	return s_aszName[nNo];
}

PBYTE CInfoMapBase::GetWriteData(int nNo, PDWORD pdwSize)
{
	PBYTE pRet, pSrc;
	DWORD dwSize;

	pRet	= NULL;
	pSrc	= NULL;
	dwSize	= GetDataSizeNo(nNo);
	*pdwSize	= dwSize;

	if (dwSize == 0) {
		goto Exit;
	}
	pRet = new BYTE[dwSize];

	switch (nNo) {
	case 0:	pSrc = (PBYTE)&m_sizeMap;	break;
	case 1:	pSrc = (PBYTE)&m_dwMapID;	break;
	case 2:	pSrc = (PBYTE)&m_dwBGMID;	break;	// BGMID
	case 3:	pSrc = (PBYTE)&m_dwWeatherType;	break;	// 天気種別
	case 4:	pSrc = (PBYTE)&m_bEnableBattle;	break;	// 戦闘許可
	case 5:	pSrc = (PBYTE)&m_bRecovery;	break;	// 気絶後回復する
	case 6:	pSrc = (PBYTE)&m_byLevel;	break;	// 暗さレベル
	case 7:	pSrc = (PBYTE)m_pwMap;	break;
	case 8:	pSrc = (PBYTE)m_pwMapPile;	break;
	case 9:	pSrc = (PBYTE)m_pwMapShadow;	break;
	case 10:pSrc = (PBYTE)(LPCSTR)m_strMapName;	break;
	case 11:
		if (m_pLibInfoMapEvent) {
			pSrc = m_pLibInfoMapEvent->GetWriteData(pdwSize);
			CopyMemory(pRet, pSrc, dwSize);
			SAFE_DELETE_ARRAY(pSrc);
			goto Exit;
		}
		break;
	case 12:
		if (m_pLibInfoMapObjectData) {
			pSrc = m_pLibInfoMapObjectData->GetWriteData(pdwSize);
			CopyMemory(pRet, pSrc, dwSize);
			SAFE_DELETE_ARRAY(pSrc);
			goto Exit;
		}
		break;
	}

	if (pSrc) {
		CopyMemory(pRet, pSrc, dwSize);
	}

Exit:
	return pRet;
}

DWORD CInfoMapBase::ReadElementData(
	PBYTE pSrc,	// [in] データの読み込み元
	int nNo)	// [in] 要素番号
{
	PBYTE pDst;
	DWORD dwSize;

	pDst	= NULL;
	dwSize	= 0;

	switch (nNo) {
	case 0: pDst = (PBYTE)&m_sizeMap;	dwSize = sizeof(m_sizeMap);	break;
	case 1:	pDst = (PBYTE)&m_dwMapID;	dwSize = sizeof(m_dwMapID);	break;
	case 2:	pDst = (PBYTE)&m_dwBGMID;	dwSize = sizeof(m_dwBGMID);	break;	// BGMID
	case 3:	pDst = (PBYTE)&m_dwWeatherType;	dwSize = sizeof(m_dwWeatherType);	break;	// 天気種別
	case 4:	pDst = (PBYTE)&m_bEnableBattle;	dwSize = sizeof(m_bEnableBattle);	break;	// 戦闘許可
	case 5:	pDst = (PBYTE)&m_bRecovery;	dwSize = sizeof(m_bRecovery);	break;	// 気絶後回復する
	case 6:	pDst = (PBYTE)&m_byLevel;	dwSize = sizeof(m_byLevel);	break;	// 暗さレベル
	case 7:
		Init(m_sizeMap.cx, m_sizeMap.cy, 0, FALSE);
		pDst	= (PBYTE)m_pwMap;
		dwSize	= (m_sizeMap.cx * sizeof(WORD)) * m_sizeMap.cy;
		break;
	case 8:
		pDst	= (PBYTE)m_pwMapPile;
		dwSize	= (m_sizeMap.cx * sizeof(WORD)) * m_sizeMap.cy;
		break;
	case 9:
		SAFE_DELETE_ARRAY(m_pwMapShadow);
		m_pwMapShadow	= new WORD[m_sizeMap.cx * m_sizeMap.cy];
		pDst	= (PBYTE)m_pwMapShadow;
		dwSize	= (m_sizeMap.cx * sizeof(WORD)) * m_sizeMap.cy;
		break;
        case 10:
                m_strMapName = (LPCTSTR)LegacyAnsiToTString((LPCSTR)pSrc);
                dwSize = (DWORD)(strlen((LPCSTR)pSrc) + 1);
                break;
	case 11:
		if (m_pLibInfoMapEvent) {
			dwSize = m_pLibInfoMapEvent->ReadElementData(pSrc);
		}
		break;
	case 12:
		if (m_pLibInfoMapObjectData) {
			dwSize = m_pLibInfoMapObjectData->ReadElementData(pSrc);
		}
		break;
	}

	if (pDst) {
		CopyMemory(pDst, pSrc, dwSize);
	}

	return dwSize;
}

void CInfoMapBase::TimerProc(DWORD dwTime)
{
}

BOOL CInfoMapBase::IsMapEvent(int x, int y)
{
	BOOL bRet;

	bRet = FALSE;
	if (m_pbyMapEvent == NULL) {
		goto Exit;
	}
	if (x >= m_sizeMap.cx) {
		goto Exit;
	}
	if (y >= m_sizeMap.cy) {
		goto Exit;
	}
	if ((x < 0) || (y < 0)) {
		goto Exit;
	}
	if (m_pbyMapEvent[y * m_sizeMap.cx + x] == 0) {
		goto Exit;
	}

	bRet = TRUE;
Exit:
	return bRet;
}

BOOL CInfoMapBase::IsHitMapEvent(RECT *pPos)
{
	BOOL bRet, bResult;
	int x, y;

	bRet = FALSE;
	if (m_pbyMapEvent == NULL) {
		goto Exit;
	}
	for (y = pPos->top; y <= pPos->bottom; y ++) {
		for (x = pPos->left; x <= pPos->right; x ++) {
			bResult = IsMapEvent(x, y);
			if (bResult) {
				bRet = TRUE;
				break;
			}
		}
	}

Exit:
	return bRet;
}

int CInfoMapBase::GetMapEventType(int x, int y)
{
	int nRet;
	PCInfoMapEventBase pInfoMapEventBase;

	nRet = 0;

	pInfoMapEventBase = GetEvent(x, y);
	if (pInfoMapEventBase == NULL) {
		goto Exit;
	}

	nRet = pInfoMapEventBase->m_nType;
Exit:
	return nRet;
}

WORD CInfoMapBase::GetParts(int x, int y)
{
	WORD wRet;

	wRet = 0;

	if ((x < 0) || (y < 0) || (x >= m_sizeMap.cx) || (y >= m_sizeMap.cy)) {
		goto Exit;
	}

	wRet = m_pwMap[m_sizeMap.cx * y + x];
Exit:
	return wRet;
}

void CInfoMapBase::SetParts(int x, int y, DWORD dwPartsID)
{
	if ((x < 0) || (y < 0) || (x >= m_sizeMap.cx) || (y >= m_sizeMap.cy)) {
		return;
	}

	m_pwMap[m_sizeMap.cx * y + x] = (WORD)dwPartsID;
}

WORD CInfoMapBase::GetPartsPile(int x, int y)
{
	WORD wRet;

	wRet = 0;

	if ((x < 0) || (y < 0) || (x >= m_sizeMap.cx) || (y >= m_sizeMap.cy)) {
		goto Exit;
	}

	wRet = m_pwMapPile[m_sizeMap.cx * y + x];
Exit:
	return wRet;
}

void CInfoMapBase::SetPartsPile(int x, int y, DWORD dwPartsID)
{
	if ((x < 0) || (y < 0) || (x >= m_sizeMap.cx) || (y >= m_sizeMap.cy)) {
		return;
	}

	m_pwMapPile[m_sizeMap.cx * y + x] = (WORD)dwPartsID;
}

BOOL CInfoMapBase::IsMove(int x, int y, int nDirection)
{
	BOOL bRet, bResult;
	DWORD dwPartsID;
	PCInfoMapParts pInfoMapParts;

	bRet = FALSE;

	if ((x < 0) || (y < 0) || (x >= m_sizeMap.cx) || (y >= m_sizeMap.cy)) {
		goto Exit;
	}
	if (m_pLibInfoMapParts == NULL) {
		goto Exit;
	}

	dwPartsID	= GetParts(x, y);
	pInfoMapParts	= (PCInfoMapParts)m_pLibInfoMapParts->GetPtr(dwPartsID);
	if (pInfoMapParts == NULL) {
		goto Exit;
	}
	bResult = pInfoMapParts->IsBlock(nDirection);
	if (bResult == FALSE) {
		goto Exit;
	}
	dwPartsID	= GetPartsPile(x, y);
	pInfoMapParts	= (PCInfoMapParts)m_pLibInfoMapParts->GetPtr(dwPartsID);
	if (pInfoMapParts) {
		bResult = pInfoMapParts->IsBlock(nDirection);
		if (bResult == FALSE) {
			goto Exit;
		}
	}
	if (m_pbyHitTmp) {
		if (m_pbyHitTmp[m_sizeMap.cx * y + x] != 0) {
			goto Exit;
		}
	}

	bRet = TRUE;
Exit:
	return bRet;
}

BOOL CInfoMapBase::IsMoveOut(int x, int y, int nDirection)
{
	int anDirection[] = {1, 0, 3, 2, 6, 7, 4, 5};
	BOOL bRet, bResult;
	DWORD dwPartsID;
	PCInfoMapParts pInfoMapParts;

	bRet = FALSE;

	if ((x < 0) || (y < 0) || (x >= m_sizeMap.cx) || (y >= m_sizeMap.cy)) {
		goto Exit;
	}
	if (m_pLibInfoMapParts == NULL) {
		goto Exit;
	}

	dwPartsID	= GetParts(x, y);
	pInfoMapParts	= (PCInfoMapParts)m_pLibInfoMapParts->GetPtr(dwPartsID);
	if (pInfoMapParts == NULL) {
		goto Exit;
	}
	bResult = pInfoMapParts->IsBlock(anDirection[nDirection]);
	if (bResult == FALSE) {
		goto Exit;
	}
	dwPartsID	= GetPartsPile(x, y);
	pInfoMapParts	= (PCInfoMapParts)m_pLibInfoMapParts->GetPtr(dwPartsID);
	if (pInfoMapParts) {
		bResult = pInfoMapParts->IsBlock(anDirection[nDirection]);
		if (bResult == FALSE) {
			goto Exit;
		}
	}

	bRet = TRUE;
Exit:
	return bRet;
}

void CInfoMapBase::DeleteParts(DWORD dwPartsID)
{
	int x, y;
	WORD wPartsID;

	for (y = 0; y < m_sizeMap.cy; y ++) {
		for (x = 0; x < m_sizeMap.cx; x ++) {
			wPartsID = m_pwMap[m_sizeMap.cx * y + x];
			if (wPartsID == dwPartsID) {
				m_pwMap[m_sizeMap.cx * y + x] = 0;
			}
			wPartsID = m_pwMapPile[m_sizeMap.cx * y + x];
			if (wPartsID == dwPartsID) {
				m_pwMapPile[m_sizeMap.cx * y + x] = 0;
			}
		}
	}
}

WORD CInfoMapBase::GetShadow(int x, int y)
{
	WORD wRet;

	wRet = 0;

	if ((x < 0) || (y < 0) || (x >= m_sizeMap.cx) || (y >= m_sizeMap.cy)) {
		goto Exit;
	}

	wRet = m_pwMapShadow[m_sizeMap.cx * y + x];
Exit:
	return wRet;
}

void CInfoMapBase::SetShadow(int x, int y, DWORD dwShadowID)
{
	if ((x < 0) || (y < 0) || (x >= m_sizeMap.cx) || (y >= m_sizeMap.cy)) {
		return;
	}

	m_pwMapShadow[m_sizeMap.cx * y + x] = (WORD)dwShadowID;
}

void CInfoMapBase::DeleteShadow(DWORD dwShadowID)
{
	int x, y;
	WORD wShadowID;

	for (y = 0; y < m_sizeMap.cy; y ++) {
		for (x = 0; x < m_sizeMap.cx; x ++) {
			wShadowID = m_pwMapShadow[m_sizeMap.cx * y + x];
			if (wShadowID == dwShadowID) {
				m_pwMapShadow[m_sizeMap.cx * y + x] = 0;
			}
		}
	}
}

DWORD CInfoMapBase::GetMapObject(int x, int y)
{
	int i, nCount;
	DWORD dwRet;
	PCInfoMapObjectData pInfo;

	dwRet = 0;

	if ((x < 0) || (y < 0) || (x >= m_sizeMap.cx) || (y >= m_sizeMap.cy)) {
		goto Exit;
	}

	nCount = m_pLibInfoMapObjectData->GetCount();
	for (i = 0; i < nCount; i ++) {
		pInfo = (PCInfoMapObjectData)m_pLibInfoMapObjectData->GetPtr(i);
		if (!((pInfo->m_ptPos.x == x) && (pInfo->m_ptPos.y == y))) {
			continue;
		}
		dwRet = pInfo->m_dwObjectID;
		break;
	}

Exit:
	return dwRet;
}

BOOL CInfoMapBase::IsFlg(int x, int y, DWORD dwFlg)
{
	BOOL bRet;
	DWORD dwPartsID;
	PCInfoMapParts pInfoMapParts;

	bRet = FALSE;
	if ((x < 0) || (y < 0) || (x >= m_sizeMap.cx) || (y >= m_sizeMap.cy)) {
		goto Exit;
	}
	if (m_pLibInfoMapParts == NULL) {
		goto Exit;
	}
	dwPartsID	= GetParts(x, y);
	pInfoMapParts	= (PCInfoMapParts)m_pLibInfoMapParts->GetPtr(dwPartsID);
	if (pInfoMapParts == NULL) {
		goto Exit;
	}
	if ((pInfoMapParts->m_dwPartsType & dwFlg) == 0) {
		goto Exit;
	}

	bRet = TRUE;
Exit:
	return bRet;
}

BOOL CInfoMapBase::IsEnableBattle(void)
{
	BOOL bRet;

	bRet = TRUE;

	if (m_bEnableBattle == FALSE) {
		bRet = FALSE;
	}

	return bRet;
}

DWORD CInfoMapBase::GetSendDataSize(void)
{
	DWORD dwRet;

	dwRet = 0;
	dwRet += sizeof(m_sizeMap);
	dwRet += sizeof(m_dwMapID);
	dwRet += sizeof(m_dwBGMID);	// BGMID
	dwRet += sizeof(m_dwWeatherType);	// 天気種別
	dwRet += sizeof(m_bEnableBattle);	// 戦闘許可
	dwRet += sizeof(m_bRecovery);	// 気絶後回復する
	dwRet += sizeof(m_byLevel);	// 暗さレベル
	dwRet += ((m_sizeMap.cx * sizeof(WORD)) * m_sizeMap.cy);
	dwRet += ((m_sizeMap.cx * sizeof(WORD)) * m_sizeMap.cy);
	dwRet += ((m_sizeMap.cx * sizeof(WORD)) * m_sizeMap.cy);
	dwRet += (m_strMapName.GetLegacyStoreLength() + 1);
	if (m_pLibInfoMapEvent) {
		dwRet += m_pLibInfoMapEvent->GetSendDataSize();
	}
	if (m_pLibInfoMapObjectData) {
		dwRet += m_pLibInfoMapObjectData->GetSendDataSize();
	}

	return dwRet;
}

PBYTE CInfoMapBase::GetSendData(void)
{
	PBYTE pRet, pData, pDataTmp, pDataLibInfoTmp;
	DWORD dwSize, dwSizeLibInfo;

	pRet = NULL;

	dwSize	= GetSendDataSize();
	pData	= ZeroNew(dwSize);
	pDataTmp	= pData;

	CopyMemoryRenew(pDataTmp, &m_sizeMap,	sizeof(m_sizeMap),	pDataTmp);	// マップサイズ
	CopyMemoryRenew(pDataTmp, &m_dwMapID,	sizeof(m_dwMapID),	pDataTmp);	// マップID
	CopyMemoryRenew(pDataTmp, &m_dwBGMID,	sizeof(m_dwBGMID),	pDataTmp);	// BGMID
	CopyMemoryRenew(pDataTmp, &m_dwWeatherType,	sizeof(m_dwWeatherType),	pDataTmp);	// 天気種別
	CopyMemoryRenew(pDataTmp, &m_bEnableBattle,	sizeof(m_bEnableBattle),	pDataTmp);	// 戦闘許可
	CopyMemoryRenew(pDataTmp, &m_bRecovery,	sizeof(m_bRecovery),	pDataTmp);	// 気絶後回復する
	CopyMemoryRenew(pDataTmp, &m_byLevel,	sizeof(m_byLevel),	pDataTmp);	// 暗さレベル
	strcpyRenew((LPSTR)pDataTmp, m_strMapName, pDataTmp);	// マップ名
	if (m_pwMap) {
		CopyMemoryRenew(pDataTmp, (PBYTE)m_pwMap, (m_sizeMap.cx * sizeof(WORD)) * m_sizeMap.cy, pDataTmp);	// マップ
	}
	if (m_pwMapPile) {
		CopyMemoryRenew(pDataTmp, (PBYTE)m_pwMapPile, (m_sizeMap.cx * sizeof(WORD)) * m_sizeMap.cy, pDataTmp);	// マップ重ね合わせ
	}
	if (m_pwMapShadow) {
		CopyMemoryRenew(pDataTmp, (PBYTE)m_pwMapShadow, (m_sizeMap.cx * sizeof(WORD)) * m_sizeMap.cy, pDataTmp);	// マップ影
	}
	if (m_pLibInfoMapEvent) {
		pDataLibInfoTmp	= m_pLibInfoMapEvent->GetSendData();
		dwSizeLibInfo	= m_pLibInfoMapEvent->GetSendDataSize();
		CopyMemoryRenew(pDataTmp, pDataLibInfoTmp, dwSizeLibInfo, pDataTmp);	// マップイベント情報
		SAFE_DELETE_ARRAY(pDataLibInfoTmp);
	}
	if (m_pLibInfoMapObjectData) {
		pDataLibInfoTmp	= m_pLibInfoMapObjectData->GetSendData();
		dwSizeLibInfo	= m_pLibInfoMapObjectData->GetSendDataSize();
		CopyMemoryRenew(pDataTmp, pDataLibInfoTmp, dwSizeLibInfo, pDataTmp);	// マップオブジェクト配置データ
		SAFE_DELETE_ARRAY(pDataLibInfoTmp);
	}

	pRet = pData;
	return pRet;
}

PBYTE CInfoMapBase::SetSendData(PBYTE pSrc)
{
	PBYTE pRet, pDataTmp;

	pRet = pSrc;

	pDataTmp = pSrc;
	CopyMemoryRenew(&m_sizeMap,	pDataTmp, sizeof(m_sizeMap),	pDataTmp);	// マップサイズ
	CopyMemoryRenew(&m_dwMapID,	pDataTmp, sizeof(m_dwMapID),	pDataTmp);	// マップID
	CopyMemoryRenew(&m_dwBGMID,	pDataTmp, sizeof(m_dwBGMID),	pDataTmp);	// BGMID
	CopyMemoryRenew(&m_dwWeatherType,	pDataTmp, sizeof(m_dwWeatherType),	pDataTmp);	// 天気種別
	CopyMemoryRenew(&m_bEnableBattle,	pDataTmp, sizeof(m_bEnableBattle),	pDataTmp);	// 戦闘許可
	CopyMemoryRenew(&m_bRecovery,	pDataTmp, sizeof(m_bRecovery),	pDataTmp);	// 気絶後回復する
	CopyMemoryRenew(&m_byLevel,	pDataTmp, sizeof(m_byLevel),	pDataTmp);	// 暗さレベル
	StoreRenew(m_strMapName, (LPCSTR)pDataTmp, pDataTmp);	// マップ名
	Init(m_sizeMap.cx, m_sizeMap.cy, 0);
	if ((m_sizeMap.cx != 0) || (m_sizeMap.cy != 0)) {
		CopyMemoryRenew((PBYTE)m_pwMap,	pDataTmp, (m_sizeMap.cx * sizeof(WORD)) * m_sizeMap.cy, pDataTmp);	// マップ
		CopyMemoryRenew((PBYTE)m_pwMapPile,	pDataTmp, (m_sizeMap.cx * sizeof(WORD)) * m_sizeMap.cy, pDataTmp);	// マップ重ね合わせ
		CopyMemoryRenew((PBYTE)m_pwMapShadow,	pDataTmp, (m_sizeMap.cx * sizeof(WORD)) * m_sizeMap.cy, pDataTmp);	// マップ影
	}
	if (m_pLibInfoMapEvent) {
		pDataTmp = m_pLibInfoMapEvent->SetSendData(pDataTmp);
	}
	if (m_pLibInfoMapObjectData) {
		pDataTmp = m_pLibInfoMapObjectData->SetSendData(pDataTmp);
	}

	pRet = pDataTmp;
	return pRet;
}

void CInfoMapBase::Copy(CInfoMapBase *pSrc)
{
	PBYTE pTmp;

	CopyMemory(&m_sizeMap, &pSrc->m_sizeMap, sizeof(m_sizeMap));
	m_dwMapID	= pSrc->m_dwMapID;	// マップID
	m_dwBGMID	= pSrc->m_dwBGMID;	// BGMID
	m_dwWeatherType	= pSrc->m_dwWeatherType;	// 天気種別
	m_bEnableBattle	= pSrc->m_bEnableBattle;	// 戦闘許可
	m_bRecovery	= pSrc->m_bRecovery;	// 気絶後回復する
	m_byLevel	= pSrc->m_byLevel;	// 暗さレベル
	m_strMapName	= pSrc->m_strMapName;
	Init(pSrc->m_sizeMap.cx, pSrc->m_sizeMap.cy, 0);
	if ((m_sizeMap.cx != 0) || (m_sizeMap.cy != 0)) {
		CopyMemory((PBYTE)m_pwMap,	pSrc->m_pwMap,	(m_sizeMap.cx * sizeof(WORD)) * m_sizeMap.cy);
		CopyMemory((PBYTE)m_pwMapPile,	pSrc->m_pwMapPile,	(m_sizeMap.cx * sizeof(WORD)) * m_sizeMap.cy);
		CopyMemory((PBYTE)m_pwMapShadow,	pSrc->m_pwMapShadow, (m_sizeMap.cx * sizeof(WORD)) * m_sizeMap.cy);
	}
	pTmp = pSrc->m_pLibInfoMapEvent->GetSendData();
	m_pLibInfoMapEvent->SetSendData(pTmp);
	SAFE_DELETE_ARRAY(pTmp);
	pTmp = pSrc->m_pLibInfoMapObjectData->GetSendData();
	m_pLibInfoMapObjectData->SetSendData(pTmp);
	SAFE_DELETE_ARRAY(pTmp);
}

void CInfoMapBase::RenewHitTmp(void)
{
	int i, nCount, x, y, xx, yy;
	PCInfoMapObjectData pInfoMapObjectData;
	PCInfoMapObject pInfoMapObject;

	if (m_pbyHitTmp == NULL) {
		return;
	}
	ZeroMemory(m_pbyHitTmp, m_sizeMap.cx * m_sizeMap.cy);

	if (m_pLibInfoMapObject == NULL) {
		return;
	}

	nCount = GetMapObjectDataCount();
	for (i = 0; i < nCount; i ++) {
		pInfoMapObjectData	= GetObjectData(i);
		pInfoMapObject	= (PCInfoMapObject)m_pLibInfoMapObject->GetPtr(pInfoMapObjectData->m_dwObjectID);
		if (pInfoMapObject == NULL) {
			continue;
		}
		if ((pInfoMapObject->m_bHit == FALSE) || (pInfoMapObject->m_pHit == NULL)) {
			continue;
		}
		xx = pInfoMapObjectData->m_ptPos.x;
		yy = pInfoMapObjectData->m_ptPos.y - pInfoMapObject->m_sizeGrp.cy + 1;
		for (y = 0; y < pInfoMapObject->m_sizeGrp.cy; y ++) {
			for (x = 0; x < pInfoMapObject->m_sizeGrp.cx; x ++) {
				if (pInfoMapObject->m_pHit[pInfoMapObject->m_sizeGrp.cx * y + x] == 0) {
					continue;
				}
				m_pbyHitTmp[m_sizeMap.cx * (yy + y) + (xx + x)] |= 1;
			}
		}
	}
}

void CInfoMapBase::RenewMapEvent(void)
{
	int i, x, y, nCount, nCountX, nCountY;
	PCInfoMapEventBase pInfo;

	SAFE_DELETE_ARRAY(m_pbyMapEvent);

	m_pbyMapEvent = ZeroNew(m_sizeMap.cx * m_sizeMap.cy);

	if (m_pLibInfoMapEvent) {
		nCount = m_pLibInfoMapEvent->GetCount();
		for (i = 0; i < nCount; i ++) {
			pInfo = (PCInfoMapEventBase)m_pLibInfoMapEvent->GetPtr(i);

			// 範囲で判定？
			if (pInfo->m_nHitType == MAPEVENTHITTYPE_AREA) {
				nCountX = pInfo->m_ptPos2.x - pInfo->m_ptPos.x + 1;
				nCountY = pInfo->m_ptPos2.y - pInfo->m_ptPos.y + 1;
				for (y = 0; y < nCountY; y ++) {
					for (x = 0; x < nCountX; x ++) {
						m_pbyMapEvent[(pInfo->m_ptPos.y + y) * m_sizeMap.cx + (pInfo->m_ptPos.x + x)] = pInfo->m_nType;
					}
				}

			} else {
				m_pbyMapEvent[pInfo->m_ptPos.y * m_sizeMap.cx + pInfo->m_ptPos.x] = pInfo->m_nType;
			}
		}
	}
}

int CInfoMapBase::GetEventCount(void)
{
	int nRet;

	nRet = 0;
	if (m_pLibInfoMapEvent == NULL) {
		goto Exit;
	}

	nRet = m_pLibInfoMapEvent->GetCount();
Exit:
	return nRet;
}

CInfoMapEventBase *CInfoMapBase::GetEvent(int nNo)
{
	PCInfoMapEventBase pRet;

	pRet = NULL;
	if (m_pLibInfoMapEvent == NULL) {
		goto Exit;
	}

	pRet = (PCInfoMapEventBase)m_pLibInfoMapEvent->GetPtr(nNo);
Exit:
	return pRet;
}

CInfoMapEventBase *CInfoMapBase::GetEvent(
	int x,	// [in] マップ座標X
	int y)	// [in] マップ座標Y
{
	int i, nCount;
	PCInfoMapEventBase pRet, pInfo;

	pRet = NULL;
	if (m_pLibInfoMapEvent == NULL) {
		goto Exit;
	}

	nCount = m_pLibInfoMapEvent->GetCount();
	for (i = 0; i < nCount; i ++) {
		pInfo = (PCInfoMapEventBase)m_pLibInfoMapEvent->GetPtr(i);
		switch (pInfo->m_nHitType) {
		case MAPEVENTHITTYPE_AREA:	// 範囲で判定
			if ((x >= pInfo->m_ptPos.x) && (x <= pInfo->m_ptPos2.x)) {
				if ((y >= pInfo->m_ptPos.y) && (y <= pInfo->m_ptPos2.y)) {
					pRet = pInfo;
				}
			}
			break;
		default:
			if ((pInfo->m_ptPos.x == x) && (pInfo->m_ptPos.y == y)) {
				pRet = pInfo;
			}
			break;
		}
	}

Exit:
	return pRet;
}

int CInfoMapBase::GetMapObjectDataCount(void)
{
	int nRet;

	nRet = 0;
	if (m_pLibInfoMapObjectData == NULL) {
		goto Exit;
	}

	nRet = m_pLibInfoMapObjectData->GetCount();
Exit:
	return nRet;
}

CInfoMapObjectData *CInfoMapBase::GetObjectData(int nNo)
{
	PCInfoMapObjectData pRet;

	pRet = NULL;
	if (m_pLibInfoMapObjectData == NULL) {
		goto Exit;
	}

	pRet = (PCInfoMapObjectData)m_pLibInfoMapObjectData->GetPtr(nNo);
Exit:
	return pRet;
}

CInfoMapObjectData *CInfoMapBase::GetObjectData(
	int x,	// [in] マップ座標X
	int y)	// [in] マップ座標Y
{
	int i, nCount;
	PCInfoMapObjectData pRet, pInfo;

	pRet = NULL;
	if (m_pLibInfoMapObjectData == NULL) {
		goto Exit;
	}

	nCount = m_pLibInfoMapObjectData->GetCount();
	for (i = 0; i < nCount; i ++) {
		pInfo = (PCInfoMapObjectData)m_pLibInfoMapObjectData->GetPtr(i);
		if ((pInfo->m_ptPos.x == x) && (pInfo->m_ptPos.y == y)) {
			pRet = pInfo;
		}
	}

Exit:
	return pRet;
}

