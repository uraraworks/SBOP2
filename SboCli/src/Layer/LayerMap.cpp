/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* ファイル名	:LayerMap.h													 */
/* 内容			:レイヤー描画クラス(マップ) 実装ファイル					 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2006/09/24													 */
/* ========================================================================= */

#include "stdafx.h"
#include "LibInfoMapParts.h"
#include "LibInfoMapBase.h"
#include "LibInfoMapObject.h"
#include "LibInfoMapObjectData.h"
#include "LibInfoItem.h"
#include "InfoMapBase.h"
#include "LibInfoCharCli.h"
#include "InfoCharCli.h"
#include "Img32.h"
#include "MgrData.h"
#include "MgrGrpData.h"
#include "MgrDraw.h"
#include "MgrLayer.h"
#include "LayerCloud.h"
#include "LayerMisty.h"
#include "LayerMap.h"


/* ========================================================================= */
/* 関数名	:CLayerMap::CLayerMap											 */
/* 内容		:コンストラクタ													 */
/* 日付		:2006/09/24														 */
/* ========================================================================= */

CLayerMap::CLayerMap()
{
	m_nID = LAYERTYPE_MAP;

	m_bScroll			= TRUE;
	m_byDirection		= 0;
	m_nViewIcon			= 0;
	m_nMoveWait			= MOVEWAIT;
	m_nViewX			= 0;
	m_nViewY			= 0;
	m_nMoveX			= 0;
	m_nMoveY			= 0;
	m_nSystemIconMode	= 0;
	m_nSyatemIconOffset	= 0;
	m_dwLastTimeScroll	= 0;
	m_dwLastTimeSystemIconMode	= 0;
	m_dwMoveWaitOnce	= 0;

	m_pDibLevel			= NULL;
	m_pDibLevelTmp		= NULL;
	m_pLibInfoItem		= NULL;
	m_pLibInfoMapParts	= NULL;

	m_pLayerCould		= NULL;
	m_pLayerMisty		= NULL;
}


/* ========================================================================= */
/* 関数名	:CLayerMap::~CLayerMap											 */
/* 内容		:デストラクタ													 */
/* 日付		:2006/09/24														 */
/* ========================================================================= */

CLayerMap::~CLayerMap()
{
	SAFE_DELETE (m_pDibLevel);
	SAFE_DELETE (m_pDibLevelTmp);
	SAFE_DELETE (m_pLayerCould);
	SAFE_DELETE (m_pLayerMisty);
}


/* ========================================================================= */
/* 関数名	:CLayerMap::Create												 */
/* 内容		:作成															 */
/* 日付		:2006/09/24														 */
/* ========================================================================= */

void CLayerMap::Create(
	CMgrData	*pMgrData)		/* [in] データ管理 */
{
	CLayerBase::Create (pMgrData);

	m_pLibInfoItem		= m_pMgrData->GetLibInfoItem ();
	m_pLibInfoMapParts	= m_pMgrData->GetLibInfoMapParts ();

	m_pDibLevel = new CImg32;
	m_pDibLevel->CreateWithoutGdi (SCRSIZEX + 64, SCRSIZEY + 64);
	m_pDibLevel->FillRect (0, 0, m_pDibLevel->Width (), m_pDibLevel->Height (), RGB (100, 0, 0));

	m_pDibLevelTmp = new CImg32;
	m_pDibLevelTmp->CreateWithoutGdi (SCRSIZEX + 64, SCRSIZEY + 64);

	m_pDibLevelTmp->CircleGradation (0, 0, 64, 48, RGB (50, 0, 0));

	m_pLayerCould = new CLayerCloud;
	m_pLayerMisty = new CLayerMisty;
	m_pLayerCould->Create (pMgrData);
	m_pLayerMisty->Create (pMgrData);

	m_pLayerCould->m_pLayerMap = this;
}


/* ========================================================================= */
/* 関数名	:CLayerMap::Draw												 */
/* 内容		:描画															 */
/* 日付		:2006/09/24														 */
/* ========================================================================= */

void CLayerMap::Draw(PCImg32 pDst)
{
	int y;
	PCInfoMapBase pMap;
	PCLayerBase pLayer;

	pMap = m_pMgrData->GetMap ();
	if (pMap == NULL) {
		return;
	}

	if (pMap->m_byLevel != 0) {
		RenewLevel ();
	}

	DrawPartsBase (pDst);
	DrawItem (pDst);
	for (y = -1; y < DRAW_PARTS_Y + 2; y ++) {
		DrawMapObject	(pDst, y);
		DrawChar		(pDst, y);
		DrawPartsPile	(pDst, y);
	}
	DrawPartsPile	(pDst);
	DrawShadow		(pDst);

	pLayer = NULL;
	switch (pMap->m_dwWeatherType) {
	case WEATHERTYPE_CLOUD:		pLayer = m_pLayerCould;		break;
	case WEATHERTYPE_MISTY:		pLayer = m_pLayerMisty;		break;
	}
	if (pLayer) {
		pLayer->Draw (pDst);
	}
	if (pMap->m_byLevel != 0) {
		pDst->SetLevel (m_pDibLevel);
	}
	DrawCharText		(pDst);
	DrawGauge			(pDst);
	DrawSystemIcon		(pDst);
}


/* ========================================================================= */
/* 関数名	:CLayerMap::TimerProc											 */
/* 内容		:時間処理														 */
/* 日付		:2006/09/24														 */
/* ========================================================================= */

BOOL CLayerMap::TimerProc(void)
{
	BOOL bRet;
	PCInfoMapBase pMap;
	PCLayerBase pLayer;

	pMap = m_pMgrData->GetMap ();
	if (pMap == NULL) {
		return FALSE;
	}

	bRet  = CLayerBase::TimerProc ();
	bRet |= TimerProcScroll ();
	bRet |= TimerProcSystemIcon ();

	switch (pMap->m_dwWeatherType) {
	case WEATHERTYPE_CLOUD:
		bRet |= m_pLayerCould->TimerProc ();
		break;
	}

	pLayer = NULL;
	switch (pMap->m_dwWeatherType) {
	case WEATHERTYPE_CLOUD:		pLayer = m_pLayerCould;		break;
	case WEATHERTYPE_MISTY:		pLayer = m_pLayerMisty;		break;
	}
	if (pLayer) {
		bRet |= pLayer->TimerProc ();
	}

	return bRet;
}


/* ========================================================================= */
/* 関数名	:CLayerMap::IsScrollPos											 */
/* 内容		:スクロールする場所か判定										 */
/* 日付		:2006/11/01														 */
/* 戻り値	:-1:スクロール不要 その他:スクロールする向き					 */
/* ========================================================================= */

int CLayerMap::IsScrollPos(
	int x,				/* [in] キャラ座標(横) */
	int y,				/* [in] キャラ座標(縦) */
	int nDirection)		/* [in] 向き */
{
	int yy, nRet, nMaxX, nMaxY;

	nRet = -1;

	yy = y;
	x = (x % 2) ? x / 2 + 1 : x / 2;
	y = (y % 2) ? y / 2 + 1 : y / 2;
	nMaxX = m_nViewX / 2;
	nMaxY = m_nViewY / 2;

	switch (nDirection) {
	case 0:
		if ((m_nViewY > 0) && (y - nMaxY <= DRAW_PARTS_Y / 2)) {
			nRet = nDirection;
		}
		break;
	case 1:
		if ((nMaxY + DRAW_PARTS_Y) - y <= DRAW_PARTS_Y / 2) {
			nRet = nDirection;
		}
		break;
	case 2:
		if ((m_nViewX > 0) && (x - nMaxX <= DRAW_PARTS_X / 2)) {
			nRet = nDirection;
		}
		break;
	case 3:
		if ((nMaxX + DRAW_PARTS_X) - x <= DRAW_PARTS_X / 2) {
			nRet = nDirection;
		}
		break;
	case 4:
		if ((m_nViewY > 0) && (y - nMaxY <= DRAW_PARTS_Y / 2)) {
			nRet = 0;
			if ((nMaxX + DRAW_PARTS_X) - x <= DRAW_PARTS_X / 2) {
				nRet = nDirection;
			}
		} else {
			if ((nMaxX + DRAW_PARTS_X) - x <= DRAW_PARTS_X / 2) {
				nRet = 3;
			}
		}
		break;
	case 5:
		if ((nMaxY + DRAW_PARTS_Y) - y <= DRAW_PARTS_Y / 2) {
			nRet = 1;
			if ((nMaxX + DRAW_PARTS_X) - x <= DRAW_PARTS_X / 2) {
				nRet = nDirection;
			}
		} else {
			if ((nMaxX + DRAW_PARTS_X) - x <= DRAW_PARTS_X / 2) {
				nRet = 3;
			}
		}
		break;
	case 6:
		if ((nMaxY + DRAW_PARTS_Y) - y <= DRAW_PARTS_Y / 2) {
			nRet = 1;
			if ((m_nViewX > 0) && (x - nMaxX <= DRAW_PARTS_X / 2)) {
				nRet = nDirection;
			}
		} else {
			if ((m_nViewX > 0) && (x - nMaxX <= DRAW_PARTS_X / 2)) {
				nRet = 2;
			}
		}
		break;
	case 7:
		if ((m_nViewY > 0) && (y - nMaxY <= DRAW_PARTS_Y / 2)) {
			nRet = 0;
			if ((m_nViewX > 0) && (x - nMaxX <= DRAW_PARTS_X / 2)) {
				nRet = nDirection;
			}
		} else {
			if ((m_nViewX > 0) && (x - nMaxX <= DRAW_PARTS_X / 2)) {
				nRet = 2;
			}
		}
		break;
	}

	return nRet;
}


/* ========================================================================= */
/* 関数名	:CLayerMap::IsScrollArea										 */
/* 内容		:スクロールできるか判定											 */
/* 日付		:2006/11/01														 */
/* ========================================================================= */

BOOL CLayerMap::IsScrollArea(
	int x,				/* [in] キャラ座標(横) */
	int y,				/* [in] キャラ座標(縦) */
	int &nDirection)	/* [in] 向き */
{
	BOOL bRet, bResult;
	int nDirectionTmp;

	bRet = FALSE;

	switch (nDirection) {
	case 0:
		if (y - m_nViewY >= (DRAW_PARTS_Y - 1) * 2) {
			goto Exit;
		}
		break;
	case 1:
		if (y - m_nViewY <= 0) {
			goto Exit;
		}
		break;
	case 2:
		if (x - m_nViewX >= (DRAW_PARTS_X - 1) * 2) {
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
		bResult = IsScrollArea (x, y, nDirectionTmp);
		nDirectionTmp = 3;
		if (bResult) {
			bResult = IsScrollArea (x, y, nDirectionTmp);
			if (bResult) {
				nDirectionTmp = nDirection;
			} else {
				nDirectionTmp = 0;
			}
			bResult = TRUE;
		} else {
			bResult = IsScrollArea (x, y, nDirectionTmp);
		}
		bRet = bResult;
		if (bRet) {
			nDirection = nDirectionTmp;
		}
		goto Exit;
	case 5:
		nDirectionTmp = 1;
		bResult = IsScrollArea (x, y, nDirectionTmp);
		nDirectionTmp = 3;
		if (bResult) {
			bResult = IsScrollArea (x, y, nDirectionTmp);
			if (bResult) {
				nDirectionTmp = nDirection;
			} else {
				nDirectionTmp = 1;
			}
			bResult = TRUE;
		} else {
			bResult = IsScrollArea (x, y, nDirectionTmp);
		}
		bRet = bResult;
		if (bRet) {
			nDirection = nDirectionTmp;
		}
		goto Exit;
	case 6:
		nDirectionTmp = 1;
		bResult = IsScrollArea (x, y, nDirectionTmp);
		nDirectionTmp = 2;
		if (bResult) {
			bResult = IsScrollArea (x, y, nDirectionTmp);
			if (bResult) {
				nDirectionTmp = nDirection;
			} else {
				nDirectionTmp = 1;
			}
			bResult = TRUE;
		} else {
			bResult = IsScrollArea (x, y, nDirectionTmp);
		}
		bRet = bResult;
		if (bRet) {
			nDirection = nDirectionTmp;
		}
		goto Exit;
	case 7:
		nDirectionTmp = 0;
		bResult = IsScrollArea (x, y, nDirectionTmp);
		nDirectionTmp = 2;
		if (bResult) {
			bResult = IsScrollArea (x, y, nDirectionTmp);
			if (bResult) {
				nDirectionTmp = nDirection;
			} else {
				nDirectionTmp = 0;
			}
			bResult = TRUE;
		} else {
			bResult = IsScrollArea (x, y, nDirectionTmp);
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


/* ========================================================================= */
/* 関数名	:CLayerMap::IsScroll											 */
/* 内容		:スクロール中か判定												 */
/* 日付		:2006/09/24														 */
/* ========================================================================= */

BOOL CLayerMap::IsScroll(void)
{
	return (m_dwLastTimeScroll) ? TRUE : FALSE;
}


/* ========================================================================= */
/* 関数名	:CLayerMap::IsInScreen											 */
/* 内容		:指定座標が画面内にいるかチェック								 */
/* 戻り値	:TRUE:画面内													 */
/* 日付		:2007/02/21														 */
/* ========================================================================= */

BOOL CLayerMap::IsInScreen(
	int x,		/* [in] キャラ座標(横) */
	int y)		/* [in] キャラ座標(縦) */
{
	BOOL bRet;
	int nMapX, nMapY;

	bRet = FALSE;

	x = (x % 2) ? x / 2 + 1 : x / 2;
	y = (y % 2) ? y / 2 + 1 : y / 2;
	nMapX = (m_nViewX % 2) ? m_nViewX / 2 + 1 : m_nViewX / 2;
	nMapY = (m_nViewY % 2) ? m_nViewY / 2 + 1 : m_nViewY / 2;

	if (x < nMapX - 1) {
		goto Exit;
	}
	if (x > nMapX + DRAW_PARTS_X) {
		goto Exit;
	}
	if (y < nMapY - 1) {
		goto Exit;
	}
	if (y > nMapY + DRAW_PARTS_Y) {
		goto Exit;
	}

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* 関数名	:CLayerMap::Scroll												 */
/* 内容		:スクロール														 */
/* 日付		:2006/09/24														 */
/* ========================================================================= */

BYTE CLayerMap::Scroll(BYTE byDirection, BOOL bNoCheck)
{
	BYTE byRet;
	int x, y, nMapX, nMapY, nMoveWait, anPosChangeX[] = {0, 0, -1, 1, 1, 1, -1, -1}, anPosChangeY[] = {-1, 1, 0, 0, -1, 1, 1, -1};
	PCInfoMapBase pMap;

	byRet = 0;

	/* スクロールしないモード？ */
	if (m_bScroll == FALSE) {
		goto Exit;
	}

	if (bNoCheck) {
		m_nMoveX = m_nMoveY = 0;

	} else {
		if (IsScroll ()) {
			goto Exit;
		}
	}
	pMap = m_pMgrData->GetMap ();
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
	m_dwLastTimeScroll = timeGetTime () - nMoveWait;
	m_nViewX = (WORD)x;
	m_nViewY = (WORD)y;
	TimerProc ();

Exit:
	return byRet;
}


/* ========================================================================= */
/* 関数名	:CLayerMap::SetCenterPos										 */
/* 内容		:指定座標が画面中央になるように設定								 */
/* 日付		:2007/02/15														 */
/* ========================================================================= */

void CLayerMap::SetCenterPos(
	int x,		/* [in] キャラ座標(横) */
	int y)		/* [in] キャラ座標(縦) */
{
	int xx, yy, nMapX, nMapY;
	PCInfoMapBase pMap;

	pMap = m_pMgrData->GetMap ();
	if (pMap == NULL) {
		return;
	}

	nMapX = (x % 2) ? x / 2 + 1 : x / 2;
	nMapY = (y % 2) ? y / 2 + 1 : y / 2;
	xx = yy = 0;

	xx = nMapX - (DRAW_PARTS_X / 2);
	yy = nMapY - (DRAW_PARTS_Y / 2);

	xx = max (0, xx);
	xx = min (pMap->m_sizeMap.cx - DRAW_PARTS_X, xx);
	yy = max (0, yy);
	yy = min (pMap->m_sizeMap.cy - DRAW_PARTS_Y, yy);

	m_nViewX = (x % 2) ? (WORD)xx * 2 - 1 : (WORD)xx * 2;
	m_nViewY = (y % 2) ? (WORD)yy * 2 - 1 : (WORD)yy * 2;
	m_nViewX = max (0, m_nViewX);
	m_nViewY = max (0, m_nViewY);
}


/* ========================================================================= */
/* 関数名	:CLayerMap::SetScrollWait										 */
/* 内容		:スクロール移動待ち時間を設定									 */
/* 日付		:2007/02/15														 */
/* ========================================================================= */

void CLayerMap::SetScrollWait(int nMoveWait)
{
	m_nMoveWait = nMoveWait;
}


/* ========================================================================= */
/* 関数名	:CLayerMap::SetScrollMode										 */
/* 内容		:スクロールするか設定											 */
/* 日付		:2007/09/09														 */
/* ========================================================================= */

void CLayerMap::SetScrollMode(BOOL bScroll, int nViewIcon)
{
	m_bScroll = bScroll;
	if (nViewIcon != -1) {
		m_nViewIcon = nViewIcon;
	}
}


/* ========================================================================= */
/* 関数名	:CLayerMap::SetSystemIconMode									 */
/* 内容		:システムアイコンモードを設定									 */
/* 日付		:2007/09/16														 */
/* ========================================================================= */

void CLayerMap::SetSystemIconMode(int nMode)
{
	if (m_nSystemIconMode == nMode) {
		return;
	}

	m_nSystemIconMode = nMode;
	m_dwLastTimeSystemIconMode = timeGetTime ();
}


/* ========================================================================= */
/* 関数名	:CLayerMap::GetViewMapPos										 */
/* 内容		:視点をマップ座標で取得											 */
/* 日付		:2008/04/19														 */
/* ========================================================================= */

void CLayerMap::GetViewMapPos(int &nDstX, int &nDstY)
{
	nDstX = m_nViewX / 2;
	nDstY = m_nViewY / 2;
}


/* ========================================================================= */
/* 関数名	:CLayerMap::RenewLevel											 */
/* 内容		:明度レベル画像を更新											 */
/* 日付		:2008/09/20														 */
/* ========================================================================= */

void CLayerMap::RenewLevel(void)
{
	int i, nCount, x, y, cx, r, nTmp;
	WORD wGrpIDMainBase, wGrpIdNPC;
	PCInfoCharCli pChar;
	POINT ptTmp;
	PCInfoMotion pInfoMotion;
	PCInfoMapBase pMap;

	pMap = m_pMgrData->GetMap ();
	if (pMap == NULL) {
		return;
	}
	if (pMap->m_byLevel == 0) {
		return;
	}

	m_pDibLevel->FillRect (0, 0, m_pDibLevel->Width (), m_pDibLevel->Height (), RGB (100 - pMap->m_byLevel, 0, 0));
	nCount = m_pLibInfoChar->GetCount ();
	for (i = 0; i < nCount; i ++) {
		pChar = (PCInfoCharCli)m_pLibInfoChar->GetPtr (i);
		if (pChar->m_nLightLevel == 0) {
			continue;
		}

		pInfoMotion = pChar->GetMotionInfo ();
		wGrpIDMainBase	= pInfoMotion->m_wGrpIDMainBase;
		wGrpIdNPC		= pChar->m_wGrpIDNPC;
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
		GetDrawPos (pChar, x, y);
		x += pChar->m_ptMove.x;
		y += pChar->m_ptMove.y;
		pChar->GetViewCharPos (ptTmp);
		y -= (ptTmp.y / 2);
		cx = m_pMgrGrpData->GetGrpSize (wGrpIDMainBase);
		r = cx * 2;
		nTmp = (r - cx / 2);
//		m_pDibLevel->CircleGradation (x - nTmp, y - nTmp, r, cx + cx / 2, RGB (50, 0, 0));
		m_pDibLevel->BltPlus (x - nTmp, y - nTmp, r * 2, r * 2, m_pDibLevelTmp, 0, 0, 100, TRUE);
//		m_pDibLevel->Circle (x - nTmp, y - nTmp, r, RGB (100, 0, 0));
	}
}


/* ========================================================================= */
/* 関数名	:CLayerMap::TimerProcScroll										 */
/* 内容		:時間処理(スクロール)											 */
/* 日付		:2007/09/16														 */
/* ========================================================================= */

BOOL CLayerMap::TimerProcScroll(void)
{
	BOOL bRet;
	int nTmp, nMoveWait, anPosChangeX[] = {0, 0, 1, 1, 1, 1, 1, 1}, anPosChangeY[] = {1, 1, 0, 0, 1, 1, 1, 1, };

	bRet = FALSE;

	if (m_dwLastTimeScroll == 0) {
		goto Exit;
	}

	nTmp = timeGetTime () - m_dwLastTimeScroll;
	nMoveWait = m_nMoveWait;
	if (m_dwMoveWaitOnce != 0) {
		nMoveWait = m_dwMoveWaitOnce;
	}
	nMoveWait = max (nMoveWait, 1);
	if (nTmp < nMoveWait) {
		goto Exit;
	}

	nTmp = nTmp / nMoveWait;
	nTmp = max (nTmp, 1);
	m_dwLastTimeScroll = timeGetTime ();
	m_nMoveX += (anPosChangeX[m_byDirection] * nTmp);
	m_nMoveY += (anPosChangeY[m_byDirection] * nTmp);

	if (m_nMoveX >= SCROLLSIZE) {
		m_nMoveX = 0;
	}
	if (m_nMoveY >= SCROLLSIZE) {
		m_nMoveY = 0;
	}

	if ((m_nMoveX <= 0) && (m_nMoveY <= 0)) {
		m_dwLastTimeScroll = 0;
		m_dwMoveWaitOnce = 0;
	}

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* 関数名	:CLayerMap::TimerProcSystemIcon									 */
/* 内容		:時間処理(システムアイコン)										 */
/* 日付		:2007/09/16														 */
/* ========================================================================= */

BOOL CLayerMap::TimerProcSystemIcon(void)
{
	BOOL bRet;
	int nTmp, nMax, nMove;

	bRet	= FALSE;
	nMax	= 120;
	nMove	= 4;

	switch (m_nSystemIconMode) {
	case 0:		/* 上に登場 */
		if (m_nSyatemIconOffset < 0) {
			goto Exit;
		}
		break;
	case 1:		/* 下へ退場 */
		if (m_nSyatemIconOffset >= nMax) {
			goto Exit;
		}
		break;
	}

	nTmp = timeGetTime () - m_dwLastTimeSystemIconMode;
	if (nTmp < 20) {
		goto Exit;
	}

	m_dwLastTimeSystemIconMode = timeGetTime ();

	switch (m_nSystemIconMode) {
	case 0:		/* 上に登場 */
		m_nSyatemIconOffset = max (m_nSyatemIconOffset - nMove, 0);
		break;
	case 1:		/* 下へ退場 */
		m_nSyatemIconOffset = min (m_nSyatemIconOffset + nMove, nMax);
		break;
	}

	if (m_nSyatemIconOffset != 0) {
		bRet = TRUE;
	}
Exit:
	return bRet;
}


/* ========================================================================= */
/* 関数名	:CLayerMap::DrawPartsBase										 */
/* 内容		:描画(土台)														 */
/* 日付		:2006/09/24														 */
/* ========================================================================= */

void CLayerMap::DrawPartsBase(PCImg32 pDst, int nDrawY/*-1*/)
{
	DWORD dwPartsID, dwPartsIDBack;
	int x, y, xx, yy, nMoveX, nMoveY, nPosX, nPosY, cx, cy;
	int aMoveX[] = {1, 1, 1, -1, -1, -1, 1, 1}, aMoveY[] = {1, -1, 1, 1, 1, -1, -1, 1},
		aScrollX[] = {-16, -16, -16, 16, 16, 16, -16, -16}, aScrollY[] = {-16, 16, -16, -16, -16, 16, 16, -16},
		aPosX[] = {0, 0, 1, 0, 0, 0, 1, 1}, aPosY[] = {1, 0, 0, 0, 1, 0, 0, 1};
	PCInfoMapBase pMap;
	PCInfoMapParts pInfoMapParts, pInfoMapPartsBack;

	pMap = m_pMgrData->GetMap ();
	if (pMap == NULL) {
		return;
	}
	if (nDrawY != -99) {
		if (nDrawY % 2) {
			return;
		}
	}

	cx		= pMap->m_sizeMap.cx;
	cy		= pMap->m_sizeMap.cy;
	nMoveX	= m_nMoveX;
	nMoveY	= m_nMoveY;
	nPosX	= m_nViewX;
	nPosY	= m_nViewY;

	if (nMoveX > 0 || nMoveY > 0) {
		/* 移動中の座標を補正 */
		nMoveX *= aMoveX[m_byDirection];
		nMoveY *= aMoveY[m_byDirection];
		switch (m_byDirection) {
		case 0:
		case 1:
			if (m_nViewX % 2) {
				nMoveX += aScrollX[m_byDirection];
			}
			if (m_nViewY % 2 == 0) {
				nMoveY += aScrollY[m_byDirection];
			} else {
				nPosY += aPosY[m_byDirection];
			}
			break;
		case 2:
		case 3:
			if (m_nViewX % 2 == 0) {
				nMoveX += aScrollX[m_byDirection];
			} else {
				nPosX += aPosX[m_byDirection];
			}
			if (m_nViewY % 2) {
				nMoveY += aScrollY[m_byDirection];
			}
			break;
		case 4:
		case 5:
		case 6:
		case 7:
			if (m_nViewX % 2 == 0) {
				nMoveX += aScrollX[m_byDirection];
			} else {
				nPosX += aPosX[m_byDirection];
			}
			if (m_nViewY % 2 == 0) {
				nMoveY += aScrollY[m_byDirection];
			} else {
				nPosY += aPosY[m_byDirection];
			}
			break;
		}
	} else {
		if (m_nViewX % 2) {
			nMoveX -= 16;
		}
		if (m_nViewY % 2) {
			nMoveY -= 16;
		}
	}
	nPosX /= 2;
	nPosY /= 2;

	dwPartsIDBack = -1;
	pInfoMapPartsBack = NULL;
	m_pMgrDraw->LockDibTmp ();
	y = -1;
	if (nDrawY != -99) {
		y = nDrawY / 2;
	}
	for (; y < DRAW_PARTS_Y + 2; y ++) {
		for (x = -1; x < DRAW_PARTS_X + 2; x ++) {
			xx = nPosX + x;
			yy = nPosY + y;
			dwPartsID = pMap->GetParts (xx, yy);
			if (dwPartsID == dwPartsIDBack) {
				pInfoMapParts = pInfoMapPartsBack;
			} else {
				pInfoMapParts = (PCInfoMapParts)m_pLibInfoMapParts->GetPtr (dwPartsID);
			}
			m_pMgrDraw->DrawMapParts (
					pDst,
					32 + x * 32 + nMoveX,
					32 + y * 32 + nMoveY,
					pInfoMapParts,
					2,
					FALSE,
					TRUE,
					FALSE);
			pInfoMapPartsBack	= pInfoMapParts;
			dwPartsIDBack		= dwPartsID;
		}
		if (nDrawY != -99) {
			break;
		}
	}
	m_pMgrDraw->UnLockDibTmp ();
}


/* ========================================================================= */
/* 関数名	:CLayerMap::GetDrawMapPos										 */
/* 内容		:マップ座標の描画位置を所得										 */
/* 日付		:2008/04/13														 */
/* ========================================================================= */

void CLayerMap::GetDrawMapPos(POINT *ptPos, int &nDstX, int &nDstY)
{
	int aMoveX[] = {1, 1, 1, -1, -1, -1, 1, 1}, aMoveY[] = {1, -1, 1, 1, 1, -1, -1, 1},
		aPosX[] = {0, 0, -1, 1, 1, 1, -1, -1}, aPosY[] = {-1, 1, 0, 0, -1, 1, 1, -1};

	/* スクロール中？ */
	if (m_nMoveX > 0 || m_nMoveY > 0) {
		nDstX += (int)m_nMoveX * aMoveX[m_byDirection];
		nDstY += (int)m_nMoveY * aMoveY[m_byDirection];
		nDstX += (aPosX[m_byDirection] * MAPPARTSSIZE);
		nDstY += (aPosY[m_byDirection] * MAPPARTSSIZE);
	}
	nDstX += ((ptPos->x - m_nViewX) * MAPPARTSSIZE);
	nDstY += ((ptPos->y - m_nViewY) * MAPPARTSSIZE);
}


/* ========================================================================= */
/* 関数名	:CLayerMap::DrawPartsPile										 */
/* 内容		:描画(重ね合わせ)												 */
/* 日付		:2007/05/26														 */
/* ========================================================================= */

void CLayerMap::DrawPartsPile(PCImg32 pDst, int nDrawY/*-99*/)
{
	BYTE byViewGrid;
	DWORD dwPartsID, dwPartsIDBack;
	int x, y, xx, yy, nMoveX, nMoveY, nPosX, nPosY, cx, cy;
	int aMoveX[] = {1, 1, 1, -1, -1, -1, 1, 1}, aMoveY[] = {1, -1, 1, 1, 1, -1, -1, 1},
		aScrollX[] = {-16, -16, -16, 16, 16, 16, -16, -16}, aScrollY[] = {-16, 16, -16, -16, -16, 16, 16, -16},
		aPosX[] = {0, 0, 1, 0, 0, 0, 1, 1}, aPosY[] = {1, 0, 0, 0, 1, 0, 0, 1};
	PCInfoMapBase pMap;
	PCInfoMapParts pInfoMapParts, pInfoMapPartsBack;

	pMap = m_pMgrData->GetMap ();
	if (pMap == NULL) {
		return;
	}

	cx		= pMap->m_sizeMap.cx;
	cy		= pMap->m_sizeMap.cy;
	nMoveX	= m_nMoveX;
	nMoveY	= m_nMoveY;
	nPosX	= m_nViewX;
	nPosY	= m_nViewY;
	byViewGrid = m_pMgrData->GetViewGrid ();

	if (nMoveX > 0 || nMoveY > 0) {
		/* 移動中の座標を補正 */
		nMoveX *= aMoveX[m_byDirection];
		nMoveY *= aMoveY[m_byDirection];
		switch (m_byDirection) {
		case 0:
		case 1:
			if (m_nViewX % 2) {
				nMoveX += aScrollX[m_byDirection];
			}
			if (m_nViewY % 2 == 0) {
				nMoveY += aScrollY[m_byDirection];
			} else {
				nPosY += aPosY[m_byDirection];
			}
			break;
		case 2:
		case 3:
			if (m_nViewX % 2 == 0) {
				nMoveX += aScrollX[m_byDirection];
			} else {
				nPosX += aPosX[m_byDirection];
			}
			if (m_nViewY % 2) {
				nMoveY += aScrollY[m_byDirection];
			}
			break;
		case 4:
		case 5:
		case 6:
		case 7:
			if (m_nViewX % 2 == 0) {
				nMoveX += aScrollX[m_byDirection];
			} else {
				nPosX += aPosX[m_byDirection];
			}
			if (m_nViewY % 2 == 0) {
				nMoveY += aScrollY[m_byDirection];
			} else {
				nPosY += aPosY[m_byDirection];
			}
			break;
		}
	} else {
		if (m_nViewX % 2) {
			nMoveX -= 16;
		}
		if (m_nViewY % 2) {
			nMoveY -= 16;
		}
	}
	nPosX /= 2;
	nPosY /= 2;

	dwPartsIDBack = -1;
	pInfoMapPartsBack = NULL;
	m_pMgrDraw->LockDibTmp ();
	y = -1;
	if (nDrawY != -99) {
		y = nDrawY;
	}
	for (; y < DRAW_PARTS_Y + 2; y ++) {
		for (x = -1; x < DRAW_PARTS_X + 2; x ++) {
			xx = nPosX + x;
			yy = nPosY + y;
			dwPartsID = pMap->GetParts (xx, yy);
			if (dwPartsID == dwPartsIDBack) {
				pInfoMapParts = pInfoMapPartsBack;
			} else {
				pInfoMapParts = (PCInfoMapParts)m_pLibInfoMapParts->GetPtr (dwPartsID);
			}
			pInfoMapPartsBack	= pInfoMapParts;
			dwPartsIDBack		= dwPartsID;
			if (pInfoMapParts == NULL) {
				continue;
			}
			if ((pInfoMapParts->m_dwPartsType & (BIT_PARTSHIT_PILE | BIT_PARTSHIT_PILEBACK)) == 0) {
				continue;
			}
			if (nDrawY != -99) {
				if (pInfoMapParts->m_dwPartsType & BIT_PARTSHIT_DRAWLAST) {
					continue;
				}
			} else {
				if ((pInfoMapParts->m_dwPartsType & BIT_PARTSHIT_DRAWLAST) == 0) {
					continue;
				}
			}
			m_pMgrDraw->DrawMapParts (
					pDst,
					32 + x * 32 + nMoveX,
					32 + y * 32 + nMoveY,
					pInfoMapParts,
					3,
					FALSE,
					TRUE,
					FALSE);
			if (byViewGrid) {
				if (byViewGrid == 1) {
					pDst->Rectangle (32 + x * 32 + nMoveX, 32 + y * 32 + nMoveY, 32, 32, -1);
				} else {
					pDst->Rectangle (32 + x * 32 + nMoveX,		32 + y * 32 + nMoveY,		16, 16, -1);
					pDst->Rectangle (32 + x * 32 + nMoveX + 16,	32 + y * 32 + nMoveY,		16, 16, -1);
					pDst->Rectangle (32 + x * 32 + nMoveX,		32 + y * 32 + nMoveY + 16,	16, 16, -1);
					pDst->Rectangle (32 + x * 32 + nMoveX + 16,	32 + y * 32 + nMoveY + 16,	16, 16, -1);
				}
			}
		}
		if (nDrawY != -99) {
			break;
		}
	}
	m_pMgrDraw->UnLockDibTmp ();
}


/* ========================================================================= */
/* 関数名	:CLayerMap::DrawShadow											 */
/* 内容		:描画(マップ影)													 */
/* 日付		:2007/06/06														 */
/* ========================================================================= */

void CLayerMap::DrawShadow(PCImg32 pDst, int nDrawY/*-99*/)
{
	WORD wShadowID;
	int x, y, xx, yy, nMoveX, nMoveY, nPosX, nPosY, cx, cy;
	int aMoveX[] = {1, 1, 1, -1, -1, -1, 1, 1}, aMoveY[] = {1, -1, 1, 1, 1, -1, -1, 1},
		aScrollX[] = {-16, -16, -16, 16, 16, 16, -16, -16}, aScrollY[] = {-16, 16, -16, -16, -16, 16, 16, -16},
		aPosX[] = {0, 0, 1, 0, 0, 0, 1, 1}, aPosY[] = {1, 0, 0, 0, 1, 0, 0, 1};
	PCInfoMapBase pMap;

	pMap = m_pMgrData->GetMap ();
	if (pMap == NULL) {
		return;
	}

	cx		= pMap->m_sizeMap.cx;
	cy		= pMap->m_sizeMap.cy;
	nMoveX	= m_nMoveX;
	nMoveY	= m_nMoveY;
	nPosX	= m_nViewX;
	nPosY	= m_nViewY;

	if (nMoveX > 0 || nMoveY > 0) {
		/* 移動中の座標を補正 */
		nMoveX *= aMoveX[m_byDirection];
		nMoveY *= aMoveY[m_byDirection];
		switch (m_byDirection) {
		case 0:
		case 1:
			if (m_nViewX % 2) {
				nMoveX += aScrollX[m_byDirection];
			}
			if (m_nViewY % 2 == 0) {
				nMoveY += aScrollY[m_byDirection];
			} else {
				nPosY += aPosY[m_byDirection];
			}
			break;
		case 2:
		case 3:
			if (m_nViewX % 2 == 0) {
				nMoveX += aScrollX[m_byDirection];
			} else {
				nPosX += aPosX[m_byDirection];
			}
			if (m_nViewY % 2) {
				nMoveY += aScrollY[m_byDirection];
			}
			break;
		case 4:
		case 5:
		case 6:
		case 7:
			if (m_nViewX % 2 == 0) {
				nMoveX += aScrollX[m_byDirection];
			} else {
				nPosX += aPosX[m_byDirection];
			}
			if (m_nViewY % 2 == 0) {
				nMoveY += aScrollY[m_byDirection];
			} else {
				nPosY += aPosY[m_byDirection];
			}
			break;
		}
	} else {
		if (m_nViewX % 2) {
			nMoveX -= 16;
		}
		if (m_nViewY % 2) {
			nMoveY -= 16;
		}
	}
	nPosX /= 2;
	nPosY /= 2;

	m_pMgrDraw->LockDibTmp ();
	y = -1;
	if (nDrawY != -99) {
		y = nDrawY;
	}
	for (; y < DRAW_PARTS_Y + 2; y ++) {
		for (x = -1; x < DRAW_PARTS_X + 2; x ++) {
			xx = nPosX + x;
			yy = nPosY + y;
			wShadowID = pMap->GetShadow (xx, yy);
			if (wShadowID == 0) {
				continue;
			}
			m_pMgrDraw->DrawMapShadow (
					pDst,
					32 + x * 32 + nMoveX,
					32 + y * 32 + nMoveY,
					wShadowID,
					FALSE,
					TRUE,
					FALSE);
		}
		if (nDrawY != -99) {
			break;
		}
	}
	m_pMgrDraw->UnLockDibTmp ();
}


/* ========================================================================= */
/* 関数名	:CLayerMap::DrawMapObject										 */
/* 内容		:描画(マップオブジェクト)										 */
/* 日付		:2008/11/03														 */
/* ========================================================================= */

void CLayerMap::DrawMapObject(PCImg32 pDst, int nDrawY/*-99*/)
{
	DWORD dwObjectID;
	int i, nCount, x, y, yy, nMoveX, nMoveY, nPosX, nPosY, cx, cy;
	int aMoveX[] = {1, 1, 1, -1, -1, -1, 1, 1}, aMoveY[] = {1, -1, 1, 1, 1, -1, -1, 1},
		aScrollX[] = {-16, -16, -16, 16, 16, 16, -16, -16}, aScrollY[] = {-16, 16, -16, -16, -16, 16, 16, -16},
		aPosX[] = {0, 0, 1, 0, 0, 0, 1, 1}, aPosY[] = {1, 0, 0, 0, 1, 0, 0, 1};
	PCInfoMapBase pMap;
	PCLibInfoMapObjectData pLibInfoMapObjectData;
	PCLibInfoMapObject pLibInfoMapObject;
	PCInfoMapObjectData pInfoData;
	PCInfoMapObject pInfo;

	pMap = m_pMgrData->GetMap ();
	if (pMap == NULL) {
		return;
	}
	pLibInfoMapObjectData = pMap->m_pLibInfoMapObjectData;
	pLibInfoMapObject = m_pMgrData->GetLibInfoMapObject ();

	cx		= pMap->m_sizeMap.cx;
	cy		= pMap->m_sizeMap.cy;
	nMoveX	= m_nMoveX;
	nMoveY	= m_nMoveY;
	nPosX	= m_nViewX;
	nPosY	= m_nViewY;

	if (nMoveX > 0 || nMoveY > 0) {
		/* 移動中の座標を補正 */
		nMoveX *= aMoveX[m_byDirection];
		nMoveY *= aMoveY[m_byDirection];
		switch (m_byDirection) {
		case 0:
		case 1:
			if (m_nViewX % 2) {
				nMoveX += aScrollX[m_byDirection];
			}
			if (m_nViewY % 2 == 0) {
				nMoveY += aScrollY[m_byDirection];
			} else {
				nPosY += aPosY[m_byDirection];
			}
			break;
		case 2:
		case 3:
			if (m_nViewX % 2 == 0) {
				nMoveX += aScrollX[m_byDirection];
			} else {
				nPosX += aPosX[m_byDirection];
			}
			if (m_nViewY % 2) {
				nMoveY += aScrollY[m_byDirection];
			}
			break;
		case 4:
		case 5:
		case 6:
		case 7:
			if (m_nViewX % 2 == 0) {
				nMoveX += aScrollX[m_byDirection];
			} else {
				nPosX += aPosX[m_byDirection];
			}
			if (m_nViewY % 2 == 0) {
				nMoveY += aScrollY[m_byDirection];
			} else {
				nPosY += aPosY[m_byDirection];
			}
			break;
		}
	} else {
		if (m_nViewX % 2) {
			nMoveX -= 16;
		}
		if (m_nViewY % 2) {
			nMoveY -= 16;
		}
	}
	nPosX /= 2;
	nPosY /= 2;

	m_pMgrDraw->LockDibTmp ();
	y = -1;
	if (nDrawY != -99) {
		y = nDrawY;
	}

	nCount = pLibInfoMapObjectData->GetCount ();
	yy = nPosY + y;
	dwObjectID = 0;
	for (i = 0; i < nCount; i ++) {
		pInfoData = (PCInfoMapObjectData)pLibInfoMapObjectData->GetPtr (i);
		pInfo = (PCInfoMapObject)pLibInfoMapObject->GetPtr (pInfoData->m_dwObjectID);
		if (pInfo == NULL) {
			continue;
		}
		if (nDrawY != -99) {
			if (yy != pInfoData->m_ptPos.y) {
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
		break;
	}
	if (dwObjectID != 0) {
		m_pMgrDraw->DrawMapObject (
				pDst,
				32 + x * 32 + nMoveX,
				32 + y * 32 + nMoveY,
				dwObjectID,
				TRUE,
				FALSE);
	}
	m_pMgrDraw->UnLockDibTmp ();
}


/* ========================================================================= */
/* 関数名	:CLayerMap::DrawItem											 */
/* 内容		:描画(アイテム)													 */
/* 日付		:2007/05/05														 */
/* ========================================================================= */

void CLayerMap::DrawItem(PCImg32 pDst, int nDrawY/*-99*/)
{
	int i, nCount, x, y, xx, yy,
		aMoveX[] = {1, 1, 1, -1, -1, -1, 1, 1}, aMoveY[] = {1, -1, 1, 1, 1, -1, -1, 1},
		aPosX[] = {0, 0, -1, 1, 1, 1, -1, -1}, aPosY[] = {-1, 1, 0, 0, -1, 1, 1, -1};
	PCInfoItem pInfoItem;
	PCInfoCharCli pPlayerChar;

	pPlayerChar = m_pMgrData->GetPlayerChar ();
	if (pPlayerChar == NULL) {
		return;
	}

	xx = yy = 0;
	/* スクロール中？ */
	if (m_nMoveX > 0 || m_nMoveY > 0) {
		xx += (int)m_nMoveX * aMoveX[m_byDirection];
		yy += (int)m_nMoveY * aMoveY[m_byDirection];
		xx += (aPosX[m_byDirection] * SCROLLSIZE);
		yy += (aPosY[m_byDirection] * SCROLLSIZE);
	}

	nCount = m_pLibInfoItem->GetAreaCount ();
	m_pMgrDraw->LockDibTmp ();
	for (i = 0; i < nCount; i ++) {
		pInfoItem = (PCInfoItem)m_pLibInfoItem->GetPtrArea (i);
		if (pInfoItem->m_dwMapID != pPlayerChar->m_dwMapID) {
			continue;
		}
		if (nDrawY != -99) {
			if (nDrawY + m_nViewY != pInfoItem->m_ptPos.y) {
				continue;
			}
		}

		x = pInfoItem->m_ptPos.x - m_nViewX;
		y = pInfoItem->m_ptPos.y - m_nViewY;
		m_pMgrDraw->DrawItem (
				pDst,
				32 + x * SCROLLSIZE + xx,
				32 + y * SCROLLSIZE + yy,
				pInfoItem,
				FALSE);
	}
	m_pMgrDraw->UnLockDibTmp ();
}


/* ========================================================================= */
/* 関数名	:CLayerMap::GetDrawPos											 */
/* 内容		:描画位置を取得													 */
/* 日付		:2006/09/24														 */
/* ========================================================================= */

void CLayerMap::GetDrawPos(CInfoCharCli *pChar, int &nDstX, int &nDstY)
{
	int aMoveX[] = {1, 1, 1, -1, -1, -1, 1, 1}, aMoveY[] = {1, -1, 1, 1, 1, -1, -1, 1},
		aPosX[] = {0, 0, -1, 1, 1, 1, -1, -1}, aPosY[] = {-1, 1, 0, 0, -1, 1, 1, -1};

	/* スクロール中？ */
	if (m_nMoveX > 0 || m_nMoveY > 0) {
		nDstX += (int)m_nMoveX * aMoveX[m_byDirection];
		nDstY += (int)m_nMoveY * aMoveY[m_byDirection];
		nDstX += (aPosX[m_byDirection] * SCROLLSIZE);
		nDstY += (aPosY[m_byDirection] * SCROLLSIZE);
	}
	nDstX += ((pChar->m_nMapX - m_nViewX) * SCROLLSIZE);
	nDstY += ((pChar->m_nMapY - m_nViewY) * SCROLLSIZE);
}


/* ========================================================================= */
/* 関数名	:CLayerMap::DrawChar											 */
/* 内容		:描画(キャラ)													 */
/* 日付		:2006/09/24														 */
/* ========================================================================= */

void CLayerMap::DrawChar(PCImg32 pDst, int nDrawY/*-99*/)
{
	int i, nCount, x, y;
	PCInfoCharCli pChar;
	POINT ptTmp;

	nCount = m_pLibInfoChar->GetCount ();
	m_pMgrDraw->LockDibTmp ();
	for (i = 0; i < nCount; i ++) {
		pChar = (PCInfoCharCli)m_pLibInfoChar->GetPtr (i);
		if (nDrawY != -99) {
			if ((m_nViewY + (nDrawY * 2) != pChar->m_nMapY) &&
				(m_nViewY + (nDrawY * 2) + 1 != pChar->m_nMapY)) {
				continue;
			}
		}
		x = y = 32;

		pChar->GetViewCharPos (ptTmp);
		x -= ptTmp.x;
		y -= ptTmp.y;

		GetDrawPos (pChar, x, y);
		m_pMgrDraw->DrawChar (pDst, x, y, pChar, FALSE);
	}
	m_pMgrDraw->UnLockDibTmp ();
}


/* ========================================================================= */
/* 関数名	:CLayerMap::DrawCharText										 */
/* 内容		:描画(キャラ関連のテキスト)										 */
/* 日付		:2006/09/24														 */
/* ========================================================================= */

void CLayerMap::DrawCharText(PCImg32 pDst, int nDrawY/*-1*/)
{
	BOOL bResult, bDraw, bDrawChat;
	int i, j, nCount, nCount2, x, y, xx, yy, cy, nMaxX, nMaxY, nWidth, nHeight, nDrawMode;
	POINT ptViewCharPos;
	PCInfoCharCli pChar;
	PCInfoTextEffect pInfoTextEffect;

	bDraw		= TRUE;
	nMaxX		= SCRSIZEX + 32;
	nMaxY		= SCRSIZEY + 32;
	nDrawMode	= m_pMgrData->GetDrawMode ();
	if (nDrawMode == 0) {
		bDraw = FALSE;
	}
	bDrawChat = m_pMgrData->GetOptionViewChat ();

	nCount = m_pLibInfoChar->GetCount ();
	for (i = nCount - 1; i >= 0; i --) {
		pChar = (PCInfoCharCli)m_pLibInfoChar->GetPtr (i);

		if (pChar->m_nMoveType == CHARMOVETYPE_PUTNPC) {
			if (m_pMgrData->GetAdminLevel () == ADMINLEVEL_NONE) {
				continue;
			}
		}
		bResult = IsInScreen (pChar->m_nMapX, pChar->m_nMapY);
		if (bResult == FALSE) {
			continue;
		}
		x = y = 32;
		GetDrawPos (pChar, x, y);
		x += pChar->m_ptMove.x;
		y += pChar->m_ptMove.y;
		pChar->GetViewCharPos (ptViewCharPos);

		/* 文字エフェクト */
		nCount2 = pChar->GetTextEffectCount ();
		for (j = 0; j < nCount2; j ++) {
			pInfoTextEffect = pChar->GetTextEffect (j);
			m_pMgrDraw->DrawTextEffect (pDst, x + pChar->m_nGrpSize, y - 20 - ptViewCharPos.y, pInfoTextEffect);
		}

		/* 噴出し */
		cy = m_pMgrGrpData->GetGrpSize (GRPIDMAIN_EFCBALLOON);
		m_pMgrDraw->DrawBalloon (pDst, x, y - (cy * 2) - ptViewCharPos.y, pChar->GetBalloonGrpID ());

		/* キャラ名の表示 */
		if (bDraw) {
			nWidth	= pChar->m_pDibName->Width ();
			nHeight	= pChar->m_pDibName->Height ();
			xx = x + pChar->m_nGrpSize - (nWidth / 2);
			xx = (xx < 32) ? 32 : xx;
			xx = (xx + nWidth >= nMaxX) ? nMaxX - nWidth : xx;
			yy = y + (pChar->m_nGrpSize * 2);
			yy = (yy < 32) ? 32 : yy;
			yy = (yy + nHeight >= nMaxY) ? nMaxY - nHeight : yy;
			pDst->Blt (xx, yy, nWidth, nHeight, pChar->m_pDibName, 0, 0, TRUE);
		}

		/* 発言内容の表示 */
		if (bDrawChat && pChar->m_strSpeak.GetLength ()) {
			nWidth	= pChar->m_pDibSpeak->Width ();
			nHeight	= pChar->m_pDibSpeak->Height ();

			xx = x + pChar->m_nGrpSize - (nWidth / 2) + 3;
			xx = (xx < 32) ? 32: xx;
			xx = (xx + nWidth >= nMaxX) ? nMaxX - nWidth: xx;
			yy = y - nHeight;
			yy = (yy < 32) ? 32 : yy;
			yy = (yy + nHeight >= nMaxY) ? nMaxY - nHeight : yy;
			yy -= ptViewCharPos.y;
			pDst->Blt (xx, yy, nWidth, nHeight, pChar->m_pDibSpeak, 0, 0, TRUE);
		}
	}
}


/* ========================================================================= */
/* 関数名	:CLayerMap::DrawSystemIcon										 */
/* 内容		:描画(システムアイコン)											 */
/* 日付		:2007/09/16														 */
/* ========================================================================= */

void CLayerMap::DrawSystemIcon(PCImg32 pDst)
{
	int nOffset;

	if (m_pMgrData->GetOptionViewHelpIcon () == FALSE) {
		return;
	}

	nOffset = m_nSyatemIconOffset;

//	pDst->BltFrom256 (32,			nOffset + SCRSIZEY - 8,	32, 40, m_pDibSystem, 688, 0, TRUE);			/* チャット */
//	pDst->BltFrom256 (SCRSIZEX - 64,nOffset + SCRSIZEY - 8,	32, 40, m_pDibSystem, 688 + 32 * 1, 0, TRUE);	/* アイテム */
//	pDst->BltFrom256 (SCRSIZEX - 32,nOffset + SCRSIZEY - 8,	32, 40, m_pDibSystem, 688 + 32 * 7, 0, TRUE);	/* 休憩 */
//	pDst->BltFrom256 (SCRSIZEX,		nOffset + SCRSIZEY - 8,	32, 40, m_pDibSystem, 688 + 32 * 6, 0, TRUE);	/* システム */
//	pDst->BltFrom256 (64,			nOffset + SCRSIZEY - 8,	32, 32, m_pDibSystem, 688 + 64, 48, TRUE);		/* 視点 */
	pDst->BltFrom256 (32,			nOffset + SCRSIZEY - 40,112, 32, m_pDibSystem, 688, 112, TRUE);			/* 説明 */
	pDst->BltFrom256 (32,			nOffset + SCRSIZEY - 8,128, 36, m_pDibSystem, 688, 208, TRUE);			/* 説明 */

	if (m_nViewIcon > 0) {
		/* 視点モード */
		pDst->BltFrom256 (32, nOffset + SCRSIZEY - 80, 32, 32, m_pDibSystem, 688 + (m_nViewIcon - 1) * 32, 48, TRUE);
	}
}


/* ========================================================================= */
/* 関数名	:CLayerMap::DrawGauge											 */
/* 内容		:描画(ゲージ類)													 */
/* 日付		:2008/06/29														 */
/* ========================================================================= */

void CLayerMap::DrawGauge(PCImg32 pDst)
{
	int x, y, nTmp;
	float fAverage;
	PCInfoCharCli pPlayerChar;

	pPlayerChar = m_pMgrData->GetPlayerChar ();
	if (pPlayerChar == NULL) {
		return;
	}
	if (pPlayerChar->IsStateBattle () == FALSE) {
		return;
	}

	/* HP */
	fAverage = (float)pPlayerChar->m_dwHP * 100 / (float)pPlayerChar->m_dwMaxHP;
	nTmp	 = (int)(70.0f * fAverage * 0.01f) + 1;
	x = 32 + 16;
	y = 32 + SCRSIZEY - 80;
	pDst->BltFrom256 (x, y, 80, 80, m_pDibSystem, 144, 48, TRUE);
	if (pPlayerChar->m_dwHP > 0) {
		pDst->BltFrom256 (x + 3, y + 3 + 70 - nTmp, 70, nTmp, m_pDibSystem, 144, 128 + 70 - nTmp, TRUE);
	}

	/* SP */
	fAverage = (float)pPlayerChar->m_dwSP * 100 / (float)pPlayerChar->m_dwMaxSP;
	nTmp	 = (int)(70.0f * fAverage * 0.01f) + 1;
	x = 32 + SCRSIZEX - 96;
	y = 32 + SCRSIZEY - 80;
	pDst->BltFrom256 (x, y, 80, 80, m_pDibSystem, 224, 48, TRUE);
	if (pPlayerChar->m_dwSP > 0) {
		pDst->BltFrom256 (x + 3, y + 3 + 70 - nTmp, 70, nTmp, m_pDibSystem, 224, 128 + 70 - nTmp, TRUE);
	}

	/* アタックゲージ */
	x = 32 + 32 * 6;
	y = 32 + SCRSIZEY - 66;
	fAverage = (float)pPlayerChar->m_wAtackGauge * 100 / MAX_ATACKGAUGE;
	nTmp	 = (int)(98 * fAverage * 0.01f) + 1;
	pDst->BltFrom256 (x,		y,		104, 34, m_pDibSystem, 504, 52, TRUE);	/* ゲージ部分 */
	pDst->BltFrom256 (x + 104,	y + 8,  28, 30, m_pDibSystem, 608, 60, TRUE);	/* アイコン部分 */
	if (pPlayerChar->m_wAtackGauge > 0) {
		pDst->BltFrom256 (x + 3, y + 15, nTmp, 16, m_pDibSystem, 448, 128);
	}

	if (pPlayerChar->m_nMoveState == CHARMOVESTATE_BATTLE_DEFENSE) {
		/* ガードゲージ */
		x = 32 + 32 * 6;
		y = 32 + SCRSIZEY - 30;
		fAverage = (float)pPlayerChar->m_wDefenseGauge * 100 / MAX_DEFENSEGAUGE;
		nTmp	 = (int)(98 * fAverage * 0.01f) + 1;
		pDst->BltFrom256 (x, y, 104, 28, m_pDibSystem, 488, 88, TRUE);
		if (pPlayerChar->m_wDefenseGauge > 0) {
			pDst->BltFrom256 (x + 3, y + 3,	nTmp, 10, m_pDibSystem, 448, 147, TRUE);
		}
	}
}

/* Copyright(C)URARA-works 2006 */
