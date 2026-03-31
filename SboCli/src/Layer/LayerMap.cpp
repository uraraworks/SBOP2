/// @file LayerMap.cpp
/// @brief レイヤー描画クラス(マップ) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2006/09/24
/// @copyright Copyright(C)URARA-works 2006

#include "stdafx.h"
#include "LibInfoMapParts.h"
#include "LibInfoMapShadow.h"
#include "LibInfoMapBase.h"
#include "LibInfoMapObject.h"
#include "LibInfoMapObjectData.h"
#include "LibInfoItem.h"
#include "InfoMapBase.h"
#include "InfoMapEventBase.h"
#include "LibInfoCharCli.h"
#include "InfoCharCli.h"
#include "Img32.h"
#include "MgrData.h"
#include "MgrGrpData.h"
#include "MgrDraw.h"
#include "MgrLayer.h"
#include "LayerCloud.h"
#include "LayerMisty.h"
#include "LayerSnow.h"
#include "LayerMap.h"
#include "myString.h"
#include <math.h>


CLayerMap::CLayerMap()
{
	m_nID = LAYERTYPE_MAP;

	m_bScroll = TRUE;
	m_byDirection = 0;
	m_nViewIcon = 0;
	m_nMoveWait = MOVEWAIT;
	m_nViewX = 0;
	m_nViewY = 0;
	m_nCameraSnapThreshold = 1;
	m_nMoveX = 0;
	m_nMoveY = 0;
	m_nSystemIconMode = 0;
	m_nSyatemIconOffset = 0;
	m_nLevelMapName = 0;
	m_dwLastTimeScroll = 0;
	m_dwLastTimeCameraUpdate = 0;
	m_dwLastTimeSystemIconMode = 0;
	m_dwLastTimeMapName = 0;
	m_dwMoveWaitOnce = 0;
	m_dCameraX = 0.0;
	m_dCameraY = 0.0;
	m_dCameraTargetX = 0.0;
	m_dCameraTargetY = 0.0;
	m_dCameraFollowSharpness = 8.0; // 停止後に約0.5秒で慣性っぽく収束

	m_pDibLevel = NULL;
	m_pDibLevelTmp = NULL;
	m_pDibMapName = NULL;
	m_pLibInfoItem = NULL;
	m_pLibInfoMapParts = NULL;
	m_pLibInfoMapShadow = NULL;

	m_pLayerCould = NULL;
	m_pLayerMisty = NULL;
	m_pLayerSnow = NULL;

        m_hFont32 = CreateFont(32, 0, 0, 0, FW_NORMAL,
                        TRUE, FALSE, FALSE, SHIFTJIS_CHARSET,
                        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                        DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T("ＭＳ Ｐゴシック"));
}


CLayerMap::~CLayerMap()
{
	SAFE_DELETE(m_pDibLevel);
	SAFE_DELETE(m_pDibLevelTmp);
	SAFE_DELETE(m_pDibMapName);
	SAFE_DELETE(m_pLayerCould);
	SAFE_DELETE(m_pLayerMisty);
	SAFE_DELETE(m_pLayerSnow);

	if (m_hFont32) {
		DeleteObject(m_hFont32);
		m_hFont32 = NULL;
	}
}


void CLayerMap::Create(
	CMgrData *pMgrData) // [in] データ管理
{
	CLayerBase::Create(pMgrData);

	m_pLibInfoItem = m_pMgrData->GetLibInfoItem();
	m_pLibInfoMapParts = m_pMgrData->GetLibInfoMapParts();
	m_pLibInfoMapShadow = m_pMgrData->GetLibInfoMapShadow();

	m_pDibLevel = new CImg32;
	m_pDibLevel->CreateWithoutGdi(SCRSIZEX + 64, SCRSIZEY + 64);
	m_pDibLevel->FillRect(0, 0, m_pDibLevel->Width(), m_pDibLevel->Height(), RGB(100, 0, 0));

	m_pDibLevelTmp = new CImg32;
	m_pDibLevelTmp->CreateWithoutGdi(SCRSIZEX + 64, SCRSIZEY + 64);

	m_pDibLevelTmp->CircleGradation(0, 0, 64, 48, RGB(50, 0, 0));

	m_pLayerCould = new CLayerCloud;
	m_pLayerMisty = new CLayerMisty;
	m_pLayerSnow  = new CLayerSnow;
	m_pLayerCould->Create(pMgrData);
	m_pLayerMisty->Create(pMgrData);
	m_pLayerSnow-> Create(pMgrData);

	m_pLayerCould->m_pLayerMap = this;
	m_pLayerSnow-> m_pLayerMap = this;
}


void CLayerMap::Draw(PCImg32 pDst)
{
	int y;
	PCInfoMapBase pMap;
	PCLayerBase pLayer;

	pMap = m_pMgrData->GetMap();
	if (pMap == NULL) {
		return;
	}

	if (pMap->m_byLevel != 0) {
		RenewLevel();
	}

	DrawPartsBase(pDst);
	DrawMapPile(pDst);
	DrawItem(pDst, 0);
	for (y = -1; y < DRAW_PARTS_Y + 2; y ++) {
		DrawMapObject(pDst, y);
		DrawChar(pDst, y);
		DrawPartsPile(pDst, y);
		DrawMapPile(pDst, y);
	}
	DrawPartsPile(pDst);
	DrawMapPile(pDst, -98);
	DrawShadow(pDst);

	pLayer = NULL;
	switch (pMap->m_dwWeatherType) {
	case WEATHERTYPE_CLOUD:  pLayer = m_pLayerCould; break;
	case WEATHERTYPE_MISTY:  pLayer = m_pLayerMisty; break;
	case WEATHERTYPE_SNOW:   pLayer = m_pLayerSnow;  break;
	}
	if (pLayer) {
		pLayer->Draw(pDst);
	}
	if (pMap->m_byLevel != 0) {
		pDst->SetLevel(m_pDibLevel);
	}
	DrawItem(pDst, 1);
	DrawCharText(pDst);
	DrawGauge(pDst);
	DrawSystemIcon(pDst);
	DrawMapName(pDst);
	if (m_pMgrData->GetMapEventEditMode()) {
		DrawMapEventDebug(pDst);
	}
	if (m_pMgrData->GetMapPartsEditMode()) {
		DrawMapPartsDebug(pDst);
	}
}


BOOL CLayerMap::TimerProc(void)
{
	BOOL bRet;
	PCInfoMapBase pMap;
	PCLayerBase pLayer;

	pMap = m_pMgrData->GetMap();
	if (pMap == NULL) {
		return FALSE;
	}

	bRet  = CLayerBase::TimerProc();
	bRet |= TimerProcScroll();
	bRet |= TimerProcSystemIcon();
	bRet |= TimerProcMapName();

	switch (pMap->m_dwWeatherType) {
	case WEATHERTYPE_CLOUD:
		bRet |= m_pLayerCould->TimerProc();
		break;
	}

	pLayer = NULL;
	switch (pMap->m_dwWeatherType) {
	case WEATHERTYPE_CLOUD:  pLayer = m_pLayerCould; break;
	case WEATHERTYPE_MISTY:  pLayer = m_pLayerMisty; break;
	case WEATHERTYPE_SNOW:   pLayer = m_pLayerSnow;  break;
	}
	if (pLayer) {
		bRet |= pLayer->TimerProc();
	}

	return bRet;
}


int CLayerMap::IsScrollPos(
	int x,          // [in] キャラ座標(横)
	int y,          // [in] キャラ座標(縦)
	int nDirection) // [in] 向き
{
	// Phase 3: カメラ追随のためスクロール判定不要
	return -1;
}


BOOL CLayerMap::IsScrollArea(
	int x,           // [in] キャラ座標(横)
	int y,           // [in] キャラ座標(縦)
	int &nDirection) // [in] 向き
{
	BOOL bRet, bResult;
	int nDirectionTmp;

	bRet = FALSE;

	switch (nDirection) {
	case 0:
		// Phase 3: m_nViewY/x,y はpx単位。旧スケール*2→MAPPARTSSIZE に変換
		if (y - m_nViewY >= (DRAW_PARTS_Y - 1) * MAPPARTSSIZE) {
			goto Exit;
		}
		break;
	case 1:
		if (y - m_nViewY <= 0) {
			goto Exit;
		}
		break;
	case 2:
		if (x - m_nViewX >= (DRAW_PARTS_X - 1) * MAPPARTSSIZE) {
			goto Exit;
		}
		break;
	case 3:
		if (x - m_nViewX <= 0) {
			goto Exit;
		}
		break;
	case 4:
		nDirectionTmp = 0;
		bResult = IsScrollArea(x, y, nDirectionTmp);
		nDirectionTmp = 3;
		if (bResult) {
			bResult = IsScrollArea(x, y, nDirectionTmp);
			if (bResult) {
				nDirectionTmp = nDirection;
			} else {
				nDirectionTmp = 0;
			}
			bResult = TRUE;
		} else {
			bResult = IsScrollArea(x, y, nDirectionTmp);
		}
		bRet = bResult;
		if (bRet) {
			nDirection = nDirectionTmp;
		}
		goto Exit;
	case 5:
		nDirectionTmp = 1;
		bResult = IsScrollArea(x, y, nDirectionTmp);
		nDirectionTmp = 3;
		if (bResult) {
			bResult = IsScrollArea(x, y, nDirectionTmp);
			if (bResult) {
				nDirectionTmp = nDirection;
			} else {
				nDirectionTmp = 1;
			}
			bResult = TRUE;
		} else {
			bResult = IsScrollArea(x, y, nDirectionTmp);
		}
		bRet = bResult;
		if (bRet) {
			nDirection = nDirectionTmp;
		}
		goto Exit;
	case 6:
		nDirectionTmp = 1;
		bResult = IsScrollArea(x, y, nDirectionTmp);
		nDirectionTmp = 2;
		if (bResult) {
			bResult = IsScrollArea(x, y, nDirectionTmp);
			if (bResult) {
				nDirectionTmp = nDirection;
			} else {
				nDirectionTmp = 1;
			}
			bResult = TRUE;
		} else {
			bResult = IsScrollArea(x, y, nDirectionTmp);
		}
		bRet = bResult;
		if (bRet) {
			nDirection = nDirectionTmp;
		}
		goto Exit;
	case 7:
		nDirectionTmp = 0;
		bResult = IsScrollArea(x, y, nDirectionTmp);
		nDirectionTmp = 2;
		if (bResult) {
			bResult = IsScrollArea(x, y, nDirectionTmp);
			if (bResult) {
				nDirectionTmp = nDirection;
			} else {
				nDirectionTmp = 0;
			}
			bResult = TRUE;
		} else {
			bResult = IsScrollArea(x, y, nDirectionTmp);
		}
		bRet = bResult;
		if (bRet) {
			nDirection = nDirectionTmp;
		}
		goto Exit;
	}

	bRet = TRUE;
Exit:
	return bRet;
}


BOOL CLayerMap::IsScroll(void)
{
	return (m_dwLastTimeScroll) ? TRUE : FALSE;
}


BOOL CLayerMap::IsInScreen(
	int x, // [in] キャラ座標(横)
	int y) // [in] キャラ座標(縦)
{
	int nMarginX;
	int nMarginY;
	int nLeft;
	int nTop;
	int nRight;
	int nBottom;

	nMarginX = MAPPARTSSIZE;
	nMarginY = MAPPARTSSIZE;
	nLeft = m_nViewX - nMarginX;
	nTop = m_nViewY - HALF_TILE - nMarginY;
	nRight = m_nViewX + SCRSIZEX + nMarginX;
	nBottom = m_nViewY + SCRSIZEY + nMarginY;

	if ((x < nLeft) || (x > nRight) || (y < nTop) || (y > nBottom)) {
		return FALSE;
	}
	return TRUE;
}


BYTE CLayerMap::Scroll(BYTE byDirection, BOOL bNoCheck)
{
	BYTE byRet;
	int x, y, nMapX, nMapY, nMoveWait, anPosChangeX[] = {0, 0, -1, 1, 1, 1, -1, -1}, anPosChangeY[] = {-1, 1, 0, 0, -1, 1, 1, -1};
	PCInfoMapBase pMap;

	byRet = 0;

	// スクロールしないモード？
	if (m_bScroll == FALSE) {
		goto Exit;
	}

	if (bNoCheck) {
		m_nMoveX = m_nMoveY = 0;

	} else {
		if (IsScroll()) {
			goto Exit;
		}
	}
	pMap = m_pMgrData->GetMap();
	if (pMap == NULL) {
		goto Exit;
	}

	x = (int)m_nViewX + anPosChangeX[byDirection];
	y = (int)m_nViewY + anPosChangeY[byDirection];
	nMapX = (x % 2) ? x / 2 + 1 : x / 2;
	nMapY = (y % 2) ? y / 2 + 1 : y / 2;

	switch (byDirection) {
	case 0:
		if (y < 0) {
			goto Exit;
		}
		byRet = byDirection + 1;
		break;
	case 1:
		if (nMapY + DRAW_PARTS_Y > pMap->m_sizeMap.cy) {
			goto Exit;
		}
		byRet = byDirection + 1;
		break;
	case 2:
		if (x < 0) {
			goto Exit;
		}
		byRet = byDirection + 1;
		break;
	case 3:
		if (nMapX + DRAW_PARTS_X > pMap->m_sizeMap.cx) {
			goto Exit;
		}
		byRet = byDirection + 1;
		break;
	case 4:
		if (nMapX + DRAW_PARTS_X > pMap->m_sizeMap.cx) {
			if (y >= 0) {
				byRet = 0 + 1;
				break;
			}
			goto Exit;
		}
		if (y < 0) {
			goto Exit;
		}
		byRet = byDirection + 1;
		break;
	case 5:
		if (nMapX + DRAW_PARTS_X > pMap->m_sizeMap.cx) {
			if (nMapY + DRAW_PARTS_Y <= pMap->m_sizeMap.cy) {
				byRet = 1 + 1;
				break;
			}
			goto Exit;
		}
		if (nMapY + DRAW_PARTS_Y > pMap->m_sizeMap.cy) {
			goto Exit;
		}
		byRet = byDirection + 1;
		break;
	case 6:
		if (x < 0) {
			if (nMapY + DRAW_PARTS_Y <= pMap->m_sizeMap.cy) {
				byRet = 1 + 1;
				break;
			}
			goto Exit;
		}
		if (nMapY + DRAW_PARTS_Y > pMap->m_sizeMap.cy) {
			goto Exit;
		}
		byRet = byDirection + 1;
		break;
	case 7:
		if (x < 0) {
			if (y >= 0) {
				byRet = 0 + 1;
				break;
			}
			goto Exit;
		}
		if (y < 0) {
			goto Exit;
		}
		byRet = byDirection + 1;
		break;
	}

	m_byDirection = byRet - 1;
	x = (int)m_nViewX + anPosChangeX[m_byDirection];
	y = (int)m_nViewY + anPosChangeY[m_byDirection];
	nMoveWait = m_nMoveWait;
	if (m_dwMoveWaitOnce != 0) {
		nMoveWait = m_dwMoveWaitOnce;
	}
	m_dwLastTimeScroll = timeGetTime() - nMoveWait;
	m_nViewX = (WORD)x;
	m_nViewY = (WORD)y;
	TimerProc();

Exit:
	return byRet;
}


void CLayerMap::SetCenterPos(
	int x, // [in] キャラ座標(横)
	int y  // [in] キャラ座標(縦)
)
{
	SnapCameraToCenterPos(x, y);
}


void CLayerMap::SetCameraTargetCenterPos(
	int x, // [in] キャラ座標(横)
	int y  // [in] キャラ座標(縦)
)
{
	double dCamX, dCamY;

	CalcCameraPosFromCenter(x, y, dCamX, dCamY);
	m_dCameraTargetX = dCamX;
	m_dCameraTargetY = dCamY;
}


void CLayerMap::SnapCameraToCenterPos(
	int x, // [in] キャラ座標(横)
	int y  // [in] キャラ座標(縦)
)
{
	double dCamX, dCamY;

	CalcCameraPosFromCenter(x, y, dCamX, dCamY);
	m_dCameraX = dCamX;
	m_dCameraY = dCamY;
	m_dCameraTargetX = dCamX;
	m_dCameraTargetY = dCamY;
	m_nViewX = (int)(dCamX + 0.5);
	m_nViewY = (int)(dCamY + 0.5);
	m_dwLastTimeCameraUpdate = timeGetTime();
}


void CLayerMap::SetScrollWait(int nMoveWait)
{
	m_nMoveWait = nMoveWait;
}


void CLayerMap::SetScrollMode(BOOL bScroll, int nViewIcon)
{
	m_bScroll = bScroll;
	if (nViewIcon != -1) {
		m_nViewIcon = nViewIcon;
	}
}


void CLayerMap::SetSystemIconMode(int nMode)
{
	if (m_nSystemIconMode == nMode) {
		return;
	}

	m_nSystemIconMode = nMode;
	m_dwLastTimeSystemIconMode = timeGetTime();
}


void CLayerMap::GetViewMapPos(int &nDstX, int &nDstY)
{
	// Phase 3: m_nViewX/Y はpx単位 → タイル座標
	nDstX = m_nViewX / MAPPARTSSIZE;
	nDstY = m_nViewY / MAPPARTSSIZE;
}


void CLayerMap::RenewLevel(void)
{
	int i, nCount, x, y, cx, r, nTmp;
	WORD wGrpIDMainBase, wGrpIdNPC;
	PCInfoCharCli pChar;
	POINT ptTmp;
	PCInfoMotion pInfoMotion;
	PCInfoMapBase pMap;

	pMap = m_pMgrData->GetMap();
	if (pMap == NULL) {
		return;
	}
	if (pMap->m_byLevel == 0) {
		return;
	}

	m_pDibLevel->FillRect(0, 0, m_pDibLevel->Width(), m_pDibLevel->Height(), RGB(100 - pMap->m_byLevel, 0, 0));
	nCount = m_pLibInfoChar->GetCount();
	for (i = 0; i < nCount; i ++) {
		pChar = (PCInfoCharCli)m_pLibInfoChar->GetPtr(i);
		if (pChar->m_nLightLevel == 0) {
			continue;
		}

		pInfoMotion = pChar->GetMotionInfo();
		wGrpIDMainBase = pInfoMotion->m_wGrpIDMainBase;
		wGrpIdNPC = pChar->m_wGrpIDNPC;
		if (wGrpIdNPC != 0) {
			if (wGrpIdNPC >= 50000) {
				wGrpIdNPC -= 50000;
				if (wGrpIDMainBase == GRPIDMAIN_2X2_CHAR) {
					wGrpIDMainBase = GRPIDMAIN_2X2_NPC;
				} else {
					wGrpIDMainBase = GRPIDMAIN_NPC;
				}
			} else {
				wGrpIDMainBase = GRPIDMAIN_2X2_NPC;
			}
		}

		x = y = 32;
		GetDrawPos(pChar, x, y);
		pChar->GetViewCharPos(ptTmp);
		y -= (ptTmp.y / 2);
		cx = m_pMgrGrpData->GetGrpSize(wGrpIDMainBase);
		r = cx * 2;
		nTmp = (r - cx / 2);
//		m_pDibLevel->CircleGradation(x - nTmp, y - nTmp, r, cx + cx / 2, RGB(50, 0, 0));
		m_pDibLevel->BltPlus(x - nTmp, y - nTmp, r * 2, r * 2, m_pDibLevelTmp, 0, 0, 100, TRUE);
//		m_pDibLevel->Circle(x - nTmp, y - nTmp, r, RGB(100, 0, 0));
	}
}


void CLayerMap::RenewMapName(LPCTSTR pszMapName)
{
	HFONT hFontOld;
	HDC hDCTmp;
	CString strMapName;

	m_nLevelMapName = 0;
	m_dwLastTimeMapName = 0;
	SAFE_DELETE(m_pDibMapName);

	if ((pszMapName == NULL) || (*pszMapName == 0)) {
		return;
	}

	strMapName = pszMapName;
	int nLen = strMapName.GetLength();
	if (nLen <= 0) {
		return;
	}

	// 仮のDCとフォントでテキスト幅を取得
	HDC hScreenDC = GetDC(NULL);
	HFONT hOldFont = (HFONT)SelectObject(hScreenDC, m_hFont32);
	SIZE sizeText = {0, 0};
#ifdef UNICODE
	GetTextExtentPoint32W(hScreenDC, strMapName, nLen, &sizeText);
#else
	GetTextExtentPoint32A(hScreenDC, strMapName, nLen, &sizeText);
#endif
	SelectObject(hScreenDC, hOldFont);
	ReleaseDC(NULL, hScreenDC);

	int nWidth = sizeText.cx + 8; // 余白
	int nHeight = 36;

	m_pDibMapName = new CImg32;
	m_pDibMapName->Create(nWidth, nHeight);

	hDCTmp = m_pDibMapName->Lock();
	hFontOld = (HFONT)SelectObject(hDCTmp, m_hFont32);
	SetBkMode(hDCTmp, TRANSPARENT);

	this->TextOut3(hDCTmp, 1, 2, strMapName, RGB(255, 255, 255));

	SelectObject(hDCTmp, hFontOld);
	m_pDibMapName->Unlock();

	m_dwLastTimeMapName = timeGetTime();
}


BOOL CLayerMap::TimerProcScroll(void)
{
	BOOL bRet;
	DWORD dwNow, dwElapsed;
	double dDt, dAlpha, dNextX, dNextY, dDiffX, dDiffY, dThreshold;
	int nViewX, nViewY;

	bRet = FALSE;
	dwNow = timeGetTime();
	if (m_dwLastTimeCameraUpdate == 0) {
		m_dwLastTimeCameraUpdate = dwNow;
		return FALSE;
	}
	dwElapsed = dwNow - m_dwLastTimeCameraUpdate;
	m_dwLastTimeCameraUpdate = dwNow;

	if (m_bScroll == FALSE) {
		return FALSE;
	}
	if (dwElapsed == 0) {
		return FALSE;
	}

	dDt = (double)dwElapsed / 1000.0;
	if (dDt > 0.10) {
		dDt = 0.10;
	}
	dAlpha = 1.0 - exp(-m_dCameraFollowSharpness * dDt);
	if (dAlpha < 0.0) {
		dAlpha = 0.0;
	} else if (dAlpha > 1.0) {
		dAlpha = 1.0;
	}

	dNextX = m_dCameraX + (m_dCameraTargetX - m_dCameraX) * dAlpha;
	dNextY = m_dCameraY + (m_dCameraTargetY - m_dCameraY) * dAlpha;

	dThreshold = (double)m_nCameraSnapThreshold;
	dDiffX = fabs(m_dCameraTargetX - dNextX);
	dDiffY = fabs(m_dCameraTargetY - dNextY);
	if ((dDiffX <= dThreshold) && (dDiffY <= dThreshold)) {
		dNextX = m_dCameraTargetX;
		dNextY = m_dCameraTargetY;
	}

	m_dCameraX = dNextX;
	m_dCameraY = dNextY;

	nViewX = (int)(m_dCameraX + 0.5);
	nViewY = (int)(m_dCameraY + 0.5);
	if ((nViewX != m_nViewX) || (nViewY != m_nViewY)) {
		m_nViewX = nViewX;
		m_nViewY = nViewY;
		bRet = TRUE;
	}
	return bRet;
}


void CLayerMap::CalcCameraPosFromCenter(
	int x,
	int y,
	double &dCamX,
	double &dCamY)
{
	PCInfoMapBase pMap;
	int nMaxCamX, nMaxCamY;

	dCamX = 0.0;
	dCamY = 0.0;

	pMap = m_pMgrData->GetMap();
	if (pMap == NULL) {
		return;
	}

	nMaxCamX = pMap->m_sizeMap.cx * MAPPARTSSIZE - SCRSIZEX;
	nMaxCamY = pMap->m_sizeMap.cy * MAPPARTSSIZE - SCRSIZEY;
	if (nMaxCamX < 0) {
		nMaxCamX = 0;
	}
	if (nMaxCamY < 0) {
		nMaxCamY = 0;
	}

	dCamX = (double)(x - (SCRSIZEX / 2));
	dCamY = (double)(y - (SCRSIZEY / 2));
	if (dCamX < 0.0) {
		dCamX = 0.0;
	} else if (dCamX > (double)nMaxCamX) {
		dCamX = (double)nMaxCamX;
	}
	if (dCamY < 0.0) {
		dCamY = 0.0;
	} else if (dCamY > (double)nMaxCamY) {
		dCamY = (double)nMaxCamY;
	}
}


BOOL CLayerMap::TimerProcSystemIcon(void)
{
	BOOL bRet;
	int nTmp, nMax, nMove;

	bRet = FALSE;
	nMax = 120;
	nMove = 4;

	switch (m_nSystemIconMode) {
	case 0: // 上に登場
		if (m_nSyatemIconOffset < 0) {
			goto Exit;
		}
		break;
	case 1: // 下へ退場
		if (m_nSyatemIconOffset >= nMax) {
			goto Exit;
		}
		break;
	}

	nTmp = timeGetTime() - m_dwLastTimeSystemIconMode;
	if (nTmp < 20) {
		goto Exit;
	}

	m_dwLastTimeSystemIconMode = timeGetTime();

	switch (m_nSystemIconMode) {
	case 0: // 上に登場
		m_nSyatemIconOffset = max(m_nSyatemIconOffset - nMove, 0);
		break;
	case 1: // 下へ退場
		m_nSyatemIconOffset = min(m_nSyatemIconOffset + nMove, nMax);
		break;
	}

	if (m_nSyatemIconOffset != 0) {
		bRet = TRUE;
	}
Exit:
	return bRet;
}


BOOL CLayerMap::TimerProcMapName(void)
{
	BOOL bRet;
	DWORD dwTimeTmp;

	bRet = FALSE;

	if (m_dwLastTimeMapName == 0) {
		goto Exit;
	}
	dwTimeTmp = (timeGetTime() - m_dwLastTimeMapName) / 20;
	if (dwTimeTmp == 0) {
		goto Exit;
	}
	if (dwTimeTmp < 100) {
		m_nLevelMapName = dwTimeTmp;
	} else if (dwTimeTmp < 200) {
	} else {
		m_nLevelMapName = 100 - (dwTimeTmp - 200);
	}
	m_nLevelMapName = min(m_nLevelMapName, 100);
	m_nLevelMapName = max(m_nLevelMapName, 0);

	// 終了？
	if (m_nLevelMapName == 0) {
		RenewMapName(NULL);
	}

	bRet = TRUE;
Exit:
	return bRet;
}


void CLayerMap::DrawPartsBase(PCImg32 pDst, int nDrawY/*-1*/)
{
	BOOL bPile;
	BYTE byLevel;
	DWORD dwPartsID, dwPartsIDBack;
	int x, y, xx, yy, nMoveX, nMoveY, nPosX, nPosY;
	POINT ptMove, ptPos;
	PCInfoMapBase pMap;
	PCInfoMapParts pInfoMapParts, pInfoMapPartsBack;

	pMap = m_pMgrData->GetMap();
	if (pMap == NULL) {
		return;
	}
	if (nDrawY != -99) {
		if (nDrawY % 2) {
			return;
		}
	}
	byLevel = 0;
	bPile = m_pMgrData->GetEditMapPile();
	if (bPile) {
		byLevel = 50;
	}

	GetDrawMovePos(ptMove, ptPos);
	nMoveX = ptMove.x;
	nMoveY = ptMove.y;
	nPosX = ptPos.x;
	nPosY = ptPos.y;

	dwPartsIDBack = -1;
	pInfoMapPartsBack = NULL;
	m_pMgrDraw->LockDibTmp();
	y = -1;
	if (nDrawY != -99) {
		y = nDrawY / 2;
	}
	for (; y < DRAW_PARTS_Y + 2; y ++) {
		for (x = -1; x < DRAW_PARTS_X + 2; x ++) {
			xx = nPosX + x;
			yy = nPosY + y;
			dwPartsID = pMap->GetParts(xx, yy);
			if (dwPartsID == dwPartsIDBack) {
				pInfoMapParts = pInfoMapPartsBack;
			} else {
				pInfoMapParts = (PCInfoMapParts)m_pLibInfoMapParts->GetPtr(dwPartsID);
			}
			if (bPile && pInfoMapParts) {
				if ((pInfoMapParts->m_dwPartsType & (BIT_PARTSHIT_PILE | BIT_PARTSHIT_PILEBACK)) == 0) {
					pDst->FillRect(
						32 + x * 32 + nMoveX,
						32 + y * 32 + nMoveY,
						32, 32, RGB(0, 0, 0));
				}
			}
			m_pMgrDraw->DrawMapParts(
					pDst,
					32 + x * 32 + nMoveX,
					32 + y * 32 + nMoveY,
					pInfoMapParts,
					2,
					FALSE,
					TRUE,
					FALSE,
					byLevel);
			pInfoMapPartsBack = pInfoMapParts;
			dwPartsIDBack = dwPartsID;
		}
		if (nDrawY != -99) {
			break;
		}
	}
	m_pMgrDraw->UnLockDibTmp();
}


void CLayerMap::DrawMapPile(PCImg32 pDst, int nDrawY/*-1*/)
{
	DWORD dwPartsID, dwPartsIDBack;
	int x, y, xx, yy, nMoveX, nMoveY, nPosX, nPosY, nCount;
	POINT ptMove, ptPos;
	PCInfoMapBase pMap;
	PCInfoMapParts pInfoMapParts, pInfoMapPartsBack;

	pMap = m_pMgrData->GetMap();
	if (pMap == NULL) {
		return;
	}

	GetDrawMovePos(ptMove, ptPos);
	nMoveX = ptMove.x;
	nMoveY = ptMove.y;
	nPosX = ptPos.x;
	nPosY = ptPos.y;

	dwPartsIDBack = -1;
	pInfoMapPartsBack = NULL;
	m_pMgrDraw->LockDibTmp();
	y = -1;
	nCount = DRAW_PARTS_Y + 2;
	if (nDrawY >= -1) {
		y = nDrawY;
		nCount = nDrawY + 2;
	}
	for (; y < nCount; y ++) {
		for (x = -1; x < DRAW_PARTS_X + 2; x ++) {
			xx = nPosX + x;
			yy = nPosY + y;
			dwPartsID = pMap->GetPartsPile(xx, yy);
			if (dwPartsID == 0) {
				continue;
			}
			if (dwPartsID == dwPartsIDBack) {
				pInfoMapParts = pInfoMapPartsBack;
			} else {
				pInfoMapParts = (PCInfoMapParts)m_pLibInfoMapParts->GetPtr(dwPartsID);
			}
			if (nDrawY >= -1) {
				if ((pInfoMapParts->m_dwPartsType & (BIT_PARTSHIT_PILE | BIT_PARTSHIT_PILEBACK)) == 0) {
					continue;
				}
			} else if (nDrawY == -98) {
				if ((pInfoMapParts->m_dwPartsType & BIT_PARTSHIT_DRAWLAST) == 0) {
					continue;
				}
			} else {
				if ((pInfoMapParts->m_dwPartsType & (BIT_PARTSHIT_PILE | BIT_PARTSHIT_PILEBACK)) != 0) {
					continue;
				}
			}
			m_pMgrDraw->DrawMapParts(
					pDst,
					32 + x * 32 + nMoveX,
					32 + y * 32 + nMoveY,
					pInfoMapParts,
					1,
					FALSE,
					TRUE,
					FALSE);
			pInfoMapPartsBack = pInfoMapParts;
			dwPartsIDBack = dwPartsID;
		}
		if (nDrawY >= -1) {
			break;
		}
	}
	m_pMgrDraw->UnLockDibTmp();
}


void CLayerMap::GetDrawMapPos(POINT *ptPos, int &nDstX, int &nDstY)
{
	// Phase 3: ptPos はタイル座標、m_nViewX/Y はpx単位カメラ左上。
	// 旧スクロールアニメーション(m_nMoveX/Y)は廃止済み。
	// 描画オフセット = タイル座標×MAPPARTSSIZE - カメラpx
	nDstX += ptPos->x * MAPPARTSSIZE - m_nViewX;
	nDstY += ptPos->y * MAPPARTSSIZE - m_nViewY;
}


void CLayerMap::GetDrawMovePos(POINT &ptMove, POINT &ptPos)
{
	// Phase 3: カメラ追随方式。m_nViewX/Y はピクセル単位のカメラ左上座標
	ptPos.x  = m_nViewX / MAPPARTSSIZE;      // タイル開始インデックス(横)
	ptPos.y  = m_nViewY / MAPPARTSSIZE;      // タイル開始インデックス(縦)
	ptMove.x = -(m_nViewX % MAPPARTSSIZE);   // ピクセルオフセット(横, 0～-31)
	ptMove.y = -(m_nViewY % MAPPARTSSIZE);   // ピクセルオフセット(縦, 0～-31)
}


void CLayerMap::DrawPartsPile(PCImg32 pDst, int nDrawY/*-99*/)
{
	BOOL bDraw, bPile;
	BYTE byViewGrid, byLevel;
	DWORD dwPartsID, dwPartsIDBack;
	int x, y, xx, yy, nMoveX, nMoveY, nPosX, nPosY, nCount;
	POINT ptMove, ptPos;
	PCInfoMapBase pMap;
	PCInfoMapParts pInfoMapParts, pInfoMapPartsBack;

	pMap = m_pMgrData->GetMap();
	if (pMap == NULL) {
		return;
	}
	byLevel = 0;
	bPile = m_pMgrData->GetEditMapPile();
	if (bPile) {
		byLevel = 50;
	}

	GetDrawMovePos(ptMove, ptPos);
	nMoveX = ptMove.x;
	nMoveY = ptMove.y;
	nPosX = ptPos.x;
	nPosY = ptPos.y;
	byViewGrid = m_pMgrData->GetViewGrid();

	dwPartsIDBack = -1;
	pInfoMapPartsBack = NULL;
	m_pMgrDraw->LockDibTmp();
	y = -1;
	nCount = DRAW_PARTS_Y + 2;
	if (nDrawY != -99) {
		y = nDrawY;
		nCount = nDrawY + 1;
	}
	for (; y < nCount; y ++) {
		for (x = -1; x < DRAW_PARTS_X + 2; x ++) {
			xx = nPosX + x;
			yy = nPosY + y;
			dwPartsID = pMap->GetParts(xx, yy);
			if (dwPartsID == dwPartsIDBack) {
				pInfoMapParts = pInfoMapPartsBack;
			} else {
				pInfoMapParts = (PCInfoMapParts)m_pLibInfoMapParts->GetPtr(dwPartsID);
			}
			bDraw = TRUE;
			pInfoMapPartsBack = pInfoMapParts;
			dwPartsIDBack = dwPartsID;
			if (pInfoMapParts == NULL) {
				bDraw = FALSE;
				continue;
			}
			if ((pInfoMapParts->m_dwPartsType & (BIT_PARTSHIT_PILE | BIT_PARTSHIT_PILEBACK)) == 0) {
				bDraw = FALSE;
			}
			if (nDrawY != -99) {
				if (pInfoMapParts->m_dwPartsType & BIT_PARTSHIT_DRAWLAST) {
					bDraw = FALSE;
				}
			} else {
				if (nMoveY <= 0) {
					if ((pInfoMapParts->m_dwPartsType & BIT_PARTSHIT_DRAWLAST) == 0) {
						bDraw = FALSE;
					}
				}
			}
			if (bDraw) {
				m_pMgrDraw->DrawMapParts(
						pDst,
						32 + x * 32 + nMoveX,
						32 + y * 32 + nMoveY,
						pInfoMapParts,
						3,
						FALSE,
						TRUE,
						FALSE);
				if (bPile) {
					if (nDrawY != -99) {
						pDst->ChgLevel(
							32 + x * 32 + nMoveX,
							32 + y * 32 + nMoveY,
							32, 32, 50);
					}
				}
			}
			if (byViewGrid && nDrawY == -99) {
				if (byViewGrid == 1) {
					pDst->Rectangle(32 + x * 32 + nMoveX, 32 + y * 32 + nMoveY, 32, 32, -1);
				} else {
					pDst->Rectangle(32 + x * 32 + nMoveX,      32 + y * 32 + nMoveY,      16, 16, -1);
					pDst->Rectangle(32 + x * 32 + nMoveX + 16, 32 + y * 32 + nMoveY,      16, 16, -1);
					pDst->Rectangle(32 + x * 32 + nMoveX,      32 + y * 32 + nMoveY + 16, 16, 16, -1);
					pDst->Rectangle(32 + x * 32 + nMoveX + 16, 32 + y * 32 + nMoveY + 16, 16, 16, -1);
				}
			}
		}
		if (nDrawY != -99) {
			break;
		}
	}
	m_pMgrDraw->UnLockDibTmp();
}


void CLayerMap::DrawShadow(PCImg32 pDst, int nDrawY/*-99*/)
{
	WORD wShadowID;
	int x, y, xx, yy, nMoveX, nMoveY, nPosX, nPosY, r, nTmp;
	POINT ptMove, ptPos;
	PCInfoMapBase pMap;
	PCInfoMapShadow pInfoMapShadow;

	pMap = m_pMgrData->GetMap();
	if (pMap == NULL) {
		return;
	}

	GetDrawMovePos(ptMove, ptPos);
	nMoveX = ptMove.x;
	nMoveY = ptMove.y;
	nPosX = ptPos.x;
	nPosY = ptPos.y;

	m_pMgrDraw->LockDibTmp();
	y = -1;
	if (nDrawY != -99) {
		y = nDrawY;
	}
	for (; y < DRAW_PARTS_Y + 2; y ++) {
		for (x = -1; x < DRAW_PARTS_X + 2; x ++) {
			xx = nPosX + x;
			yy = nPosY + y;
			wShadowID = pMap->GetShadow(xx, yy);
			if (wShadowID == 0) {
				continue;
			}
			xx = 32 + x * 32 + nMoveX;
			yy = 32 + y * 32 + nMoveY;
			pInfoMapShadow = (PCInfoMapShadow)m_pLibInfoMapShadow->GetPtr((DWORD)wShadowID);
			if (pInfoMapShadow == NULL) {
				continue;
			}
			if (pInfoMapShadow->m_bLight) {
				if (pMap->m_byLevel != 0) {
					r = 32 * 2;
					nTmp = (r - 32 / 2);
					m_pDibLevel->BltPlus(xx - nTmp, yy - nTmp, r * 2, r * 2, m_pDibLevelTmp, 0, 0, 100, TRUE);
				}
			} else {
				m_pMgrDraw->DrawMapShadow(
						pDst,
						xx,
						yy,
						wShadowID,
						FALSE,
						TRUE,
						FALSE);
			}
		}
		if (nDrawY != -99) {
			break;
		}
	}
	m_pMgrDraw->UnLockDibTmp();
}


void CLayerMap::DrawMapObject(PCImg32 pDst, int nDrawY/*-99*/)
{
	DWORD dwObjectID;
	int i, nCount, x, y, yy, yTmp, nMoveX, nMoveY, nPosX, nPosY;
	POINT ptMove, ptPos;
	PCInfoMapBase pMap;
	PCLibInfoMapObjectData pLibInfoMapObjectData;
	PCLibInfoMapObject pLibInfoMapObject;
	PCInfoMapObjectData pInfoData;
	PCInfoMapObject pInfo;

	pMap = m_pMgrData->GetMap();
	if (pMap == NULL) {
		return;
	}
	pLibInfoMapObjectData = pMap->m_pLibInfoMapObjectData;
	pLibInfoMapObject = m_pMgrData->GetLibInfoMapObject();

	GetDrawMovePos(ptMove, ptPos);
	nMoveX = ptMove.x;
	nMoveY = ptMove.y;
	nPosX = ptPos.x;
	nPosY = ptPos.y;

	m_pMgrDraw->LockDibTmp();
	y = -1;
	if (nDrawY != -99) {
		y = nDrawY;
	}

	nCount = pLibInfoMapObjectData->GetCount();
	yy = nPosY + y;
	for (i = 0; i < nCount; i ++) {
		pInfoData = (PCInfoMapObjectData)pLibInfoMapObjectData->GetPtr(i);
		pInfo = (PCInfoMapObject)pLibInfoMapObject->GetPtr(pInfoData->m_dwObjectID);
		if (pInfo == NULL) {
			continue;
		}
		yTmp = pInfoData->m_ptPos.y;
		if (pInfo->m_nHideY != 0) {
			yTmp -= (pInfo->m_sizeGrp.cy - pInfo->m_nHideY);
		}
		if (nDrawY != -99) {
			if (yy != yTmp) {
				if (y == DRAW_PARTS_Y + 1) {
					if (!((pInfoData->m_ptPos.y >= yy) && (pInfoData->m_ptPos.y - pInfo->m_sizeGrp.cy - 1 < yy))) {
						continue;
					}
				} else {
					continue;
				}
			}
		}
		x = pInfoData->m_ptPos.x - nPosX;
		if (pInfoData->m_ptPos.x > nPosX + DRAW_PARTS_X + 2) {
			continue;
		}
		if (pInfoData->m_ptPos.x + pInfo->m_sizeGrp.cx < nPosX) {
			continue;
		}
		x = pInfoData->m_ptPos.x - nPosX;
		y = pInfoData->m_ptPos.y - nPosY;
		dwObjectID = pInfoData->m_dwObjectID;
		m_pMgrDraw->DrawMapObject(
				pDst,
				32 + x * 32 + nMoveX,
				32 + y * 32 + nMoveY,
				dwObjectID,
				TRUE,
				FALSE);
	}
	m_pMgrDraw->UnLockDibTmp();
}


void CLayerMap::DrawItem(PCImg32 pDst, int nType, int nDrawY/*-99*/)
{
	int i, nCount, x, y, xx, yy,
		aMoveX[] = {1, 1, 1, -1, -1, -1, 1, 1}, aMoveY[] = {1, -1, 1, 1, 1, -1, -1, 1},
		aPosX[] = {0, 0, -1, 1, 1, 1, -1, -1}, aPosY[] = {-1, 1, 0, 0, -1, 1, 1, -1};
	PCInfoItem pInfoItem;
	PCInfoCharCli pPlayerChar;
	HDC hDC;
	HFONT hFontOld;

	pPlayerChar = m_pMgrData->GetPlayerChar();
	if (pPlayerChar == NULL) {
		return;
	}
	if (m_pMgrData->GetOptionViewItem() == FALSE) {
		return;
	}
	if (nType != 0) {
		if (m_pMgrData->GetOptionViewItemName() == FALSE) {
			return;
		}
	}

	// Phase 3: スクロールアニメーション廃止（xx/yy は常に 0）
	xx = yy = 0;

	nCount = m_pLibInfoItem->GetAreaCount();
	m_pMgrDraw->LockDibTmp();
	for (i = 0; i < nCount; i ++) {
		pInfoItem = (PCInfoItem)m_pLibInfoItem->GetPtrArea(i);
		if (pInfoItem->m_dwMapID != pPlayerChar->m_dwMapID) {
			continue;
		}
		if (nDrawY != -99) {
			if (m_nViewY / MAPPARTSSIZE + nDrawY != pInfoItem->m_ptPos.y / MAPPARTSSIZE) {
				continue;
			}
		}

		x = 32 + (pInfoItem->m_ptPos.x - m_nViewX) + xx;
		y = 32 + (pInfoItem->m_ptPos.y - m_nViewY) + yy;
		y -= 16;
		if (nType == 0) {
			m_pMgrDraw->DrawItem(
					pDst,
					x,
					y,
					pInfoItem,
					0,
					FALSE);
		} else {
			hDC = pDst->Lock();
			hFontOld = (HFONT)SelectObject(hDC, m_hFont);
			SetBkMode(hDC, TRANSPARENT);
			x += 16;
			x -= (pInfoItem->m_strName.GetLength() * 6 / 2);
			y += 32;
			TextOut2(hDC, x, y, (LPCTSTR)pInfoItem->m_strName, RGB(255, 255, 255));
			SelectObject(hDC, hFontOld);
			pDst->Unlock();
		}
	}
	m_pMgrDraw->UnLockDibTmp();
}


void CLayerMap::GetDrawPos(CInfoCharCli *pChar, int &nDstX, int &nDstY)
{
	POINT ptDrawMapPos;

	pChar->GetDrawMapPos(ptDrawMapPos);
	// Phase 3: m_nMapX/Y・m_nViewX/Y ともにピクセル単位。スクロールアニメーション補正廃止
	nDstX += (ptDrawMapPos.x - m_nViewX);
	// Phase 8: m_nMapY は足元基準のため、描画は半キャラ(16px)上へ補正する
	nDstY += (ptDrawMapPos.y - m_nViewY - HALF_TILE);
}


void CLayerMap::DrawChar(PCImg32 pDst, int nDrawY/*-99*/)
{
	int i, nCount, x, y;
	PCInfoCharCli pChar;
	POINT ptTmp;

	nCount = m_pLibInfoChar->GetCount();
	m_pMgrDraw->LockDibTmp();
	for (i = 0; i < nCount; i ++) {
		pChar = (PCInfoCharCli)m_pLibInfoChar->GetPtr(i);
		if (nDrawY != -99) {
			// Phase 3: m_nViewY/MAPPARTSSIZE でタイル行に変換して比較
			if (m_nViewY / MAPPARTSSIZE + nDrawY != pChar->m_nMapY / MAPPARTSSIZE) {
				continue;
			}
		}
		x = y = 32;

		pChar->GetViewCharPos(ptTmp);
		x -= ptTmp.x;
		y -= ptTmp.y;

		GetDrawPos(pChar, x, y);
		m_pMgrDraw->DrawChar(pDst, x, y, pChar, FALSE);
	}
	m_pMgrDraw->UnLockDibTmp();
}

void CLayerMap::DrawCharText(PCImg32 pDst, int nDrawY/*-1*/)
{
	BOOL bResult, bDraw, bDrawChat;
	int i, j, nCount, nCount2, x, y, xx, yy, cy, nMaxX, nMaxY, nWidth, nHeight, nDrawMode;
	DWORD dwBalloonID;
	POINT ptViewCharPos, ptDrawMapPos;
	PCInfoCharCli pChar;
	PCInfoTextEffect pInfoTextEffect;

	bDraw = TRUE;
	nMaxX = SCRSIZEX + 32;
	nMaxY = SCRSIZEY + 32;
	nDrawMode = m_pMgrData->GetDrawMode();
	if (nDrawMode == 0) {
		bDraw = FALSE;
	}
	bDrawChat = m_pMgrData->GetOptionViewChat();

	nCount = m_pLibInfoChar->GetCount();
	for (i = nCount - 1; i >= 0; i --) {
		pChar = (PCInfoCharCli)m_pLibInfoChar->GetPtr(i);
		if (pChar->m_nMoveType == CHARMOVETYPE_PUTNPC) {
			if (m_pMgrData->GetAdminLevel() == ADMINLEVEL_NONE) {
				continue;
			}
		}
		pChar->GetDrawMapPos(ptDrawMapPos);
		bResult = IsInScreen(ptDrawMapPos.x, ptDrawMapPos.y);
		if (bResult == FALSE) {
			continue;
		}
		x = y = 32;
		GetDrawPos(pChar, x, y);
		pChar->GetViewCharPos(ptViewCharPos);

		nCount2 = pChar->GetTextEffectCount();
		for (j = 0; j < nCount2; j ++) {
			pInfoTextEffect = pChar->GetTextEffect(j);
			m_pMgrDraw->DrawTextEffect(pDst, x + pChar->m_nGrpSize, y - 20 - ptViewCharPos.y, pInfoTextEffect);
		}

		cy = m_pMgrGrpData->GetGrpSize(GRPIDMAIN_EFCBALLOON);
		dwBalloonID = pChar->GetBalloonGrpID();
		m_pMgrDraw->DrawBalloon(pDst, x, y - (cy * 2) - ptViewCharPos.y, dwBalloonID);

		if (bDraw) {
			nWidth = pChar->m_pDibName->Width();
			nHeight = pChar->m_pDibName->Height();
			xx = x + pChar->m_nGrpSize - (nWidth / 2);
			if (xx < 32) {
				xx = 32;
			}
			if (xx + nWidth >= nMaxX) {
				xx = nMaxX - nWidth;
			}
			yy = y + nHeight;
			if (yy < 32) {
				yy = 32;
			}
			if (yy + nHeight >= nMaxY) {
				yy = nMaxY - nHeight;
			}
			pDst->Blt(xx, yy, nWidth, nHeight, pChar->m_pDibName, 0, 0, TRUE);
		}

		if (bDrawChat && pChar->m_strSpeak.GetLength() > 0) {
			nWidth = pChar->m_pDibSpeak->Width();
			nHeight = pChar->m_pDibSpeak->Height();
			xx = x + pChar->m_nGrpSize - (nWidth / 2) + 3;
			if (xx < 32) {
				xx = 32;
			}
			if (xx + nWidth >= nMaxX) {
				xx = nMaxX - nWidth;
			}
			yy = y - nHeight;
			yy -= ptViewCharPos.y;
			if (yy < 32) {
				yy = 32;
			}
			if (yy + nHeight >= nMaxY) {
				yy = nMaxY - nHeight;
			}
			pDst->Blt(xx, yy, nWidth, nHeight, pChar->m_pDibSpeak, 0, 0, TRUE);
		}
	}
}


void CLayerMap::DrawSystemIcon(PCImg32 pDst)
{
	int nOffset;

	if (m_pMgrData->GetOptionViewHelpIcon() == FALSE) {
		return;
	}

	nOffset = m_nSyatemIconOffset;

//	pDst->BltFrom256(32,           nOffset + SCRSIZEY - 8, 32, 40, m_pDibSystem, 688,          0, TRUE); // チャット
	pDst->BltFrom256(SCRSIZEX - 64, nOffset + SCRSIZEY - 8, 32, 40, m_pDibSystem, 688 + 32 * 1, 0, TRUE); // アイテム
	pDst->BltFrom256(SCRSIZEX - 32, nOffset + SCRSIZEY - 8, 32, 40, m_pDibSystem, 688 + 32 * 7, 0, TRUE); // 休憩
	pDst->BltFrom256(SCRSIZEX,      nOffset + SCRSIZEY - 8, 32, 40, m_pDibSystem, 688 + 32 * 6, 0, TRUE); // システム
	pDst->BltFrom256(64,            nOffset + SCRSIZEY - 8, 32, 32, m_pDibSystem, 688 + 64,    48, TRUE); // 視点
	pDst->BltFrom256(32,            nOffset + SCRSIZEY - 40, 112, 32, m_pDibSystem, 688,        112, TRUE); // 説明
	pDst->BltFrom256(32,            nOffset + SCRSIZEY - 8, 128, 36, m_pDibSystem, 688,         208, TRUE); // 説明

	if (m_nViewIcon > 0) {
		// 視点モード
		pDst->BltFrom256(32, nOffset + SCRSIZEY - 80, 32, 32, m_pDibSystem, 688 + (m_nViewIcon - 1) * 32, 48, TRUE);
	}
}


void CLayerMap::DrawGauge(PCImg32 pDst)
{
	int x, y, nTmp;
	float fAverage;
	PCInfoCharCli pPlayerChar;

	pPlayerChar = m_pMgrData->GetPlayerChar();
	if (pPlayerChar == NULL) {
		return;
	}
	if (pPlayerChar->IsStateBattle() == FALSE) {
		return;
	}

	// HP
	fAverage = (float)pPlayerChar->m_dwHP * 100 / (float)pPlayerChar->m_dwMaxHP;
	nTmp = (int)(70.0f * fAverage * 0.01f) + 1;
	x = 32 + 16;
	y = 32 + SCRSIZEY - 80;
	pDst->BltFrom256(x, y, 80, 80, m_pDibSystem, 144, 48, TRUE);
	if (pPlayerChar->m_dwHP > 0) {
		pDst->BltFrom256(x + 3, y + 3 + 70 - nTmp, 70, nTmp, m_pDibSystem, 144, 128 + 70 - nTmp, TRUE);
	}

	// SP
	fAverage = (float)pPlayerChar->m_dwSP * 100 / (float)pPlayerChar->m_dwMaxSP;
	nTmp = (int)(70.0f * fAverage * 0.01f) + 1;
	x = 32 + SCRSIZEX - 96;
	y = 32 + SCRSIZEY - 80;
	pDst->BltFrom256(x, y, 80, 80, m_pDibSystem, 224, 48, TRUE);
	if (pPlayerChar->m_dwSP > 0) {
		pDst->BltFrom256(x + 3, y + 3 + 70 - nTmp, 70, nTmp, m_pDibSystem, 224, 128 + 70 - nTmp, TRUE);
	}

	// アタックゲージ
	x = 32 + 32 * 6;
	y = 32 + SCRSIZEY - 66;
	fAverage = (float)pPlayerChar->m_wAtackGauge * 100 / MAX_ATACKGAUGE;
	nTmp = (int)(98 * fAverage * 0.01f) + 1;
	pDst->BltFrom256(x,       y,      104, 34, m_pDibSystem, 504, 52, TRUE); // ゲージ部分
	pDst->BltFrom256(x + 104, y + 8,   28, 30, m_pDibSystem, 608, 60, TRUE); // アイコン部分
	if (pPlayerChar->m_wAtackGauge > 0) {
		pDst->BltFrom256(x + 3, y + 15, nTmp, 16, m_pDibSystem, 448, 128);
	}

	if (pPlayerChar->m_nMoveState == CHARMOVESTATE_BATTLE_DEFENSE) {
		// ガードゲージ
		x = 32 + 32 * 6;
		y = 32 + SCRSIZEY - 30;
		fAverage = (float)pPlayerChar->m_wDefenseGauge * 100 / MAX_DEFENSEGAUGE;
		nTmp = (int)(98 * fAverage * 0.01f) + 1;
		pDst->BltFrom256(x, y, 104, 28, m_pDibSystem, 488, 88, TRUE);
		if (pPlayerChar->m_wDefenseGauge > 0) {
			pDst->BltFrom256(x + 3, y + 3, nTmp, 10, m_pDibSystem, 448, 147, TRUE);
		}
	}
}


void CLayerMap::DrawMapName(PCImg32 pDst)
{
	int x, y, cx, cy;

	if (m_pDibMapName == NULL) {
		return;
	}

	cx = m_pDibMapName->Width();
	cy = m_pDibMapName->Height();

	x = pDst->Width() / 2;
	x -= (cx / 2);
	y = pDst->Height() / 2 - 72;
	y -= (cy / 2);

	pDst->BltAlphaFrom256(x, y, 50, 58, m_pDibSystem, 544, 320, 100 - m_nLevelMapName, TRUE);
	pDst->BltAlpha(x + 32, y + 16, cx, cy, m_pDibMapName, 0, 0, 100 - m_nLevelMapName, TRUE);
}


void CLayerMap::DrawMapEventDebug(CImg32 *pDst)
{
	int i, nCount, nScrX, nScrY;
	PCInfoMapBase pMap;
	PCInfoMapEventBase pEvent;
	PCInfoCharCli pPlayer;

	pMap = m_pMgrData->GetMap();
	if (pMap == NULL) {
		return;
	}

	// 全イベントタイルに黄色の矩形を描画
	nCount = pMap->GetEventCount();
	for (i = 0; i < nCount; i ++) {
		pEvent = pMap->GetEvent(i);
		if (pEvent == NULL) {
			continue;
		}
		nScrX = 32 + pEvent->m_ptPos.x * MAPPARTSSIZE - m_nViewX;
		nScrY = 32 + pEvent->m_ptPos.y * MAPPARTSSIZE - m_nViewY;
		if (pEvent->m_nHitType == MAPEVENTHITTYPE_AREA) {
			int nWidth, nHeight;

			nWidth = (pEvent->m_ptPos2.x - pEvent->m_ptPos.x + 1) * MAPPARTSSIZE;
			nHeight = (pEvent->m_ptPos2.y - pEvent->m_ptPos.y + 1) * MAPPARTSSIZE;
			pDst->Rectangle(nScrX, nScrY, nWidth, nHeight, RGB(255, 200, 0));
		} else {
			pDst->Rectangle(nScrX, nScrY, MAPPARTSSIZE, MAPPARTSSIZE, RGB(255, 255, 0));
		}
	}

	// プレイヤーの衝突判定矩形を緑色で描画
	pPlayer = m_pMgrData->GetPlayerChar();
	if (pPlayer) {
		RECT rcHit;
		pPlayer->GetCollisionRect(rcHit);
		nScrX = 32 + rcHit.left  - m_nViewX;
		nScrY = 32 + rcHit.top   - m_nViewY;
		pDst->Rectangle(nScrX, nScrY, rcHit.right - rcHit.left + 1, rcHit.bottom - rcHit.top + 1, RGB(0, 255, 0));
	}
}


void CLayerMap::DrawMapPartsDebug(CImg32 *pDst)
{
	int x, y, xx, yy, nMoveX, nMoveY, nPosX, nPosY;
	DWORD dwPartsID;
	POINT ptMove, ptPos;
	RECT rcHit;
	PCInfoCharCli pPlayer;
	PCInfoMapBase pMap;
	PCInfoMapParts pInfoMapParts;

	pMap = m_pMgrData->GetMap();
	if (pMap == NULL) {
		return;
	}

	GetDrawMovePos(ptMove, ptPos);
	nMoveX = ptMove.x;
	nMoveY = ptMove.y;
	nPosX = ptPos.x;
	nPosY = ptPos.y;

	for (y = -1; y < DRAW_PARTS_Y + 2; y ++) {
		for (x = -1; x < DRAW_PARTS_X + 2; x ++) {
			BOOL bHit;

			xx = nPosX + x;
			yy = nPosY + y;
			bHit = FALSE;

			dwPartsID = pMap->GetParts(xx, yy);
			pInfoMapParts = (PCInfoMapParts)m_pLibInfoMapParts->GetPtr(dwPartsID);
			if (pInfoMapParts) {
				if ((pInfoMapParts->m_dwPartsType & BIT_PARTSHIT_BLOCK) ||
					(pInfoMapParts->m_byBlockDirection != 0)) {
					bHit = TRUE;
				}
			}

			if (bHit == FALSE) {
				dwPartsID = pMap->GetPartsPile(xx, yy);
				pInfoMapParts = (PCInfoMapParts)m_pLibInfoMapParts->GetPtr(dwPartsID);
				if (pInfoMapParts) {
					if ((pInfoMapParts->m_dwPartsType & BIT_PARTSHIT_BLOCK) ||
						(pInfoMapParts->m_byBlockDirection != 0)) {
						bHit = TRUE;
					}
				}
			}

			if (bHit) {
				pDst->Rectangle(
					32 + x * MAPPARTSSIZE + nMoveX,
					32 + y * MAPPARTSSIZE + nMoveY,
					MAPPARTSSIZE,
					MAPPARTSSIZE,
					RGB(0, 255, 255));
			}
		}
	}

	pPlayer = m_pMgrData->GetPlayerChar();
	if (pPlayer) {
		pPlayer->GetCollisionRect(rcHit);
		pDst->Rectangle(
			32 + rcHit.left - m_nViewX,
			32 + rcHit.top - m_nViewY,
			rcHit.right - rcHit.left + 1,
			rcHit.bottom - rcHit.top + 1,
			RGB(0, 255, 0));
	}
}
