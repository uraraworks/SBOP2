/* Copyright(C)URARA-works 2005 */
/* ========================================================================= */
/* ファイル名	:LayerCloud.cpp												 */
/* 内容			:レイヤー描画クラス(雲) 実装ファイル						 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2005/09/08													 */
/* ========================================================================= */

#include "stdafx.h"
#include "InfoCharCLI.h"
#include "MgrLayer.h"
#include "MgrData.h"
#include "MgrGrpData.h"
#include "Img32.h"
#include "LayerMap.h"
#include "LayerCloud.h"


/* ========================================================================= */
/* 関数名	:CLayerCloud::CLayerCloud										 */
/* 内容		:コンストラクタ													 */
/* 日付		:2005/09/08														 */
/* ========================================================================= */

CLayerCloud::CLayerCloud()
{
	m_nID			= LAYERTYPE_CLOUD;
	m_nCount		= GetTickCount ();
	m_dwLastProc	= 0;
	m_nPosX			= 0;
	m_nPosY			= 0;
	m_nPosXBack		= 0;
	m_nPosYBack		= 0;
	m_pDibCloud		= NULL;
	m_pLayerMap		= NULL;
}


/* ========================================================================= */
/* 関数名	:CLayerCloud::~CLayerCloud										 */
/* 内容		:デストラクタ													 */
/* 日付		:2005/09/08														 */
/* ========================================================================= */

CLayerCloud::~CLayerCloud()
{
}


/* ========================================================================= */
/* 関数名	:CLayerCloud::Create											 */
/* 内容		:作成															 */
/* 日付		:2005/09/08														 */
/* ========================================================================= */

void CLayerCloud::Create(
	CMgrData	*pMgrData)		/* [in] データ管理 */
{
	PCMgrLayer pMgrLayer;
	PCInfoCharCli pChar;

	CLayerBase::Create (pMgrData);

	m_pDibCloud = m_pMgrGrpData->GetDibShadow ();
	pChar		= pMgrData->GetPlayerChar ();

	if (pChar) {
		/*
		   雲レイヤーは旧来「HALF_TILE単位」を前提に視差計算しているため、
		   自由移動(px)座標をそのまま使うと 16 倍速相当で流れてしまう。
		*/
		m_nPosXBack	= pChar->m_nMapX / HALF_TILE;
		m_nPosYBack	= pChar->m_nMapY / HALF_TILE;
	}

	pMgrLayer	= pMgrData->GetMgrLayer ();
	m_pLayerMap = (PCLayerMap)pMgrLayer->Get (LAYERTYPE_MAP);
}


/* ========================================================================= */
/* 関数名	:CLayerCloud::Draw												 */
/* 内容		:描画															 */
/* 日付		:2005/09/08														 */
/* ========================================================================= */

void CLayerCloud::Draw(PCImg32 pDst)
{
	int i, x, y, xx, yy, cx, cy, nMoveX, nMoveY;
	double dTmp, dPosX, dPosY, dX, dY;
	int anPos[] = {
			5, 10, 42, 26,
			2, 40, 78, 46,
			1, 89, 60, 37,
			0, 0, 0, 0
		};
	int aMoveX[] = {1, 1, 1, -1, -1, -1, 1, 1}, aMoveY[] = {1, -1, 1, 1, 1, -1, -1, 1},
		aPosX[] = {0, 0, 1, -1, -1, -1, 1, 1}, aPosY[] = {1, -1, 0, 0, 1, -1, -1, 1};
	HDC hDCBmp, hDCBmp2;

	nMoveX	= 0;
	nMoveY	= 0;
	dPosX	= 0.0;
	dPosY	= 0.0;

	if (m_pLayerMap) {
		nMoveX	= m_pLayerMap->m_nMoveX;
		nMoveY	= m_pLayerMap->m_nMoveY;
		/* m_nViewX/Y はpx単位。HALF_TILE正規化を実数で行い段階移動を防ぐ */
		dPosX	= (double)m_pLayerMap->m_nViewX / (double)HALF_TILE;
		dPosY	= (double)m_pLayerMap->m_nViewY / (double)HALF_TILE;

		if (nMoveX || nMoveY) {
			/* 移動中の座標を補正 */
			nMoveX	*= aMoveX[m_pLayerMap->m_byDirection];
			nMoveY	*= aMoveY[m_pLayerMap->m_byDirection];
			dPosX	+= aPosX[m_pLayerMap->m_byDirection];
			dPosY	+= aPosY[m_pLayerMap->m_byDirection];
		}
	}

	dPosX = (double)m_nCount + ((double)m_nPosXBack - dPosX) * (double)SCROLLSIZE;
	dPosY = (double)m_nCount + ((double)m_nPosYBack - dPosY) * (double)SCROLLSIZE;

	/* 雲を表示 */
	for (i = 0; ; i ++) {
		if (anPos[i * 4 + 2] == 0) {
			break;
		}

		dTmp = (double)(SCRSIZEX + (anPos[i * 4 + 2] * 3));
		dX = fmod (dPosX + (double)(anPos[i * 4 + 0] * 2), dTmp);
		if (dX < 0) {
			dX += dTmp;
		}
		x = (int)dX - (anPos[i * 4 + 2] * 2) + nMoveX;
		cx = anPos[i * 4 + 2];
		xx = anPos[i * 4 + 0];

		dTmp = (double)(SCRSIZEY + (anPos[i * 4 + 3] * 3));
		dY = fmod (dPosY + (double)(anPos[i * 4 + 1] * 2), dTmp);
		if (dY < 0) {
			dY += dTmp;
		}
		y = (int)dY - (anPos[i * 4 + 3] * 2) + nMoveY;
		cy = anPos[i * 4 + 3];
		yy = anPos[i * 4 + 1];

		m_pDibBase->BltFrom256 (0, 0, cx, cy, m_pDibCloud, xx, yy);

		hDCBmp2	= m_pDibBase->Lock ();
		hDCBmp	= pDst->Lock ();

		StretchBlt (hDCBmp2, 0, cy * 2, cx * 2, cy * 2, hDCBmp2, 0, 0, cx, cy, SRCCOPY);

		m_pDibBase->Unlock ();
		pDst->Unlock ();
		pDst->BltAlpha (32 + x, 32 + y, cx * 2, cy * 2, m_pDibBase, 0, cy * 2, 75, TRUE);
	}
}


/* ========================================================================= */
/* 関数名	:CLayerCloud::TimerProc											 */
/* 内容		:時間処理														 */
/* 日付		:2005/09/08														 */
/* ========================================================================= */

BOOL CLayerCloud::TimerProc(void)
{
	BOOL bRet;
	DWORD dwTime;

	bRet	= FALSE;
	dwTime	= timeGetTime ();

	if (dwTime - m_dwLastProc < 100) {
		goto Exit;
	}

	m_dwLastProc = dwTime;
	m_nCount ++;

	bRet = TRUE;
Exit:
	return bRet;
}

/* Copyright(C)URARA-works 2005 */
