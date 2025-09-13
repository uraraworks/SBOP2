/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:LayerSnow.cpp												 */
/* 内容			:レイヤー描画クラス(雪) 実装ファイル						 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/12/14													 */
/* ========================================================================= */

#include "stdafx.h"
#include "MgrLayer.h"
#include "MgrData.h"
#include "MgrGrpData.h"
#include "Img32.h"
#include "InfoMapBase.h"
#include "LayerMap.h"
#include "LayerSnow.h"


/* ========================================================================= */
/* 関数名	:CLayerSnow::CLayerSnow											 */
/* 内容		:コンストラクタ													 */
/* 日付		:2008/12/14														 */
/* ========================================================================= */

CLayerSnow::CLayerSnow()
{
	m_nID = WEATHERTYPE_SNOW;

	m_dwLastProc	= 0;
	m_pLayerMap		= NULL;
	m_pImgTmp		= new CImg32;
}


/* ========================================================================= */
/* 関数名	:CLayerSnow::~CLayerSnow										 */
/* 内容		:デストラクタ													 */
/* 日付		:2008/12/14														 */
/* ========================================================================= */

CLayerSnow::~CLayerSnow()
{
	DeleteSnowInfoAll ();
	SAFE_DELETE (m_pImgTmp);
}


/* ========================================================================= */
/* 関数名	:CLayerSnow::Create												 */
/* 内容		:作成															 */
/* 日付		:2008/12/14														 */
/* ========================================================================= */

void CLayerSnow::Create(
	CMgrData	*pMgrData)		/* [in] データ管理 */
{
	PCMgrLayer pMgrLayer;

	CLayerBase::Create (pMgrData);

	pMgrLayer	= pMgrData->GetMgrLayer ();
	m_pLayerMap = (PCLayerMap)pMgrLayer->Get (LAYERTYPE_MAP);

	m_pImgTmp->CreateWithoutGdi (100, 100);

	RenewSnowInfo (200);
}


/* ========================================================================= */
/* 関数名	:CLayerSnow::Draw												 */
/* 内容		:描画															 */
/* 日付		:2008/12/14														 */
/* ========================================================================= */

void CLayerSnow::Draw(PCImg32 pDst)
{
	PSTLAYERSNOW_SNOWINFO pInfo;
	int i, x, y, xx, yy, nMoveX, nMoveY, nPosX, nPosY, nCount;
	int aMoveX[] = {1, 1, 1, -1, -1, -1, 1, 1}, aMoveY[] = {1, -1, 1, 1, 1, -1, -1, 1},
		aPosX[] = {0, 0, 1, -1, -1, -1, 1, 1}, aPosY[] = {1, -1, 0, 0, 1, -1, -1, 1};

	nMoveX	= 0;
	nMoveY	= 0;
	nPosX	= 0;
	nPosY	= 0;

	if (m_pLayerMap) {
		nMoveX	= m_pLayerMap->m_nMoveX;
		nMoveY	= m_pLayerMap->m_nMoveY;
		nPosX	= m_pLayerMap->m_nViewX;
		nPosY	= m_pLayerMap->m_nViewY;

		if (nMoveX || nMoveY) {
			/* 移動中の座標を補正 */
			nMoveX	*= aMoveX[m_pLayerMap->m_byDirection];
			nMoveY	*= aMoveY[m_pLayerMap->m_byDirection];
			nPosX	+= aPosX[m_pLayerMap->m_byDirection];
			nPosY	+= aPosY[m_pLayerMap->m_byDirection];
		}
	}
	x = nPosX % (DRAW_PARTS_X * 2);
	y = nPosY % (DRAW_PARTS_Y * 2);

	nCount = m_aSnowInfo.GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfo = m_aSnowInfo[i];

		xx = (pInfo->x + (SCRSIZEX - x * 16)) % SCRSIZEX;
		yy = (pInfo->y + (SCRSIZEY - y * 16)) % SCRSIZEY;
		m_pImgTmp->FillRect (0, 0, pInfo->nSize * 2, pInfo->nSize * 2, RGB (0, 0, 0));
		m_pImgTmp->Circle (0, 0, pInfo->nSize, RGB (255, 255, 255));
		pDst->BltAlpha (32 + xx + nMoveX, 32 + yy + nMoveY, pInfo->nSize * 2, pInfo->nSize * 2, m_pImgTmp, 0, 0, pInfo->nLevel, TRUE);
	}
}


/* ========================================================================= */
/* 関数名	:CLayerSnow::TimerProc											 */
/* 内容		:時間処理														 */
/* 日付		:2008/12/14														 */
/* ========================================================================= */

BOOL CLayerSnow::TimerProc(void)
{
	BOOL bRet;
	int i, nCount;
	DWORD dwTime;
	PSTLAYERSNOW_SNOWINFO pInfo;

	bRet	= FALSE;
	dwTime	= timeGetTime ();

	if (dwTime - m_dwLastProc < 25) {
		goto Exit;
	}

	nCount = m_aSnowInfo.GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfo = m_aSnowInfo[i];
		if (dwTime - pInfo->dwLastProc < pInfo->dwWait) {
			continue;
		}

		switch (pInfo->nState) {
		case 0:
			if (dwTime - pInfo->dwLastProc < pInfo->dwStartWait) {
				continue;
			}
			pInfo->dwLastProc = dwTime;
			pInfo->nState = 1;
			pInfo->nLevel = 0;
			pInfo->y = pInfo->nStartY;
			break;
		case 1:
			pInfo->dwLastProc = dwTime;
			pInfo->y ++;
			if (pInfo->y >= pInfo->nEndY) {
				pInfo->nState = 2;
			}
			break;
		case 2:
			if (dwTime - pInfo->dwLastProc > 1000) {
				pInfo->nState = 0;
				break;
			}
			pInfo->nLevel = (dwTime - pInfo->dwLastProc) / 10;
			pInfo->nLevel = min (pInfo->nLevel, 100);
			pInfo->nLevel = max (pInfo->nLevel, 1);
			break;
		}
		bRet = TRUE;
	}
	m_dwLastProc = dwTime;

Exit:
	return bRet;
}


/* ========================================================================= */
/* 関数名	:CLayerSnow::RenewSnowInfo										 */
/* 内容		:雪情報を更新													 */
/* 日付		:2008/12/14														 */
/* ========================================================================= */
void CLayerSnow::RenewSnowInfo(int nCount)
{
	int i, nTmp;
	PSTLAYERSNOW_SNOWINFO pInfo;

	DeleteSnowInfoAll ();

	for (i = 0; i < nCount; i ++) {
		pInfo = new STLAYERSNOW_SNOWINFO;

		nTmp = (genrand () % 3);
		pInfo->nState		= 0;
		pInfo->nLevel		= 100;
		pInfo->nSize		= 3 - nTmp;
		pInfo->nStartY		= genrand () % SCRSIZEY;
		pInfo->nEndY		= pInfo->nStartY + (SCRSIZEY - (genrand () % (SCRSIZEY / 3 * (nTmp + 1)))) + 32;
		pInfo->x			= (genrand () % SCRSIZEX) + 32;
		pInfo->y			= pInfo->nStartY;
		pInfo->dwStartWait	= genrand () % 3000;
		pInfo->dwWait		= 50 + nTmp * 50;
		pInfo->dwLastProc	= timeGetTime ();

		m_aSnowInfo.Add (pInfo);
	}
}


/* ========================================================================= */
/* 関数名	:CLayerSnow::DeleteSnowInfoAll									 */
/* 内容		:雪情報を全て削除												 */
/* 日付		:2008/12/14														 */
/* ========================================================================= */
void CLayerSnow::DeleteSnowInfoAll(void)
{
	int i, nCount;
	PSTLAYERSNOW_SNOWINFO pInfo;

	nCount = m_aSnowInfo.GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfo = m_aSnowInfo[i];
		SAFE_DELETE (pInfo);
	}
	m_aSnowInfo.RemoveAll ();
}

/* Copyright(C)URARA-works 2008 */
