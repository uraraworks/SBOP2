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
		m_nPosXBack	= pChar->m_nMapX;
		m_nPosYBack	= pChar->m_nMapY;
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
	int i, x, y, xx, yy, cx, cy, nTmp, nMoveX, nMoveY, nPosX, nPosY;
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

	nPosX = m_nCount + (m_nPosXBack - nPosX) * SCROLLSIZE;
	nPosY = m_nCount + (m_nPosYBack - nPosY) * SCROLLSIZE;

	/* 雲を表示 */
	for (i = 0; ; i ++) {
		if (anPos[i * 4 + 2] == 0) {
			break;
		}

		nTmp = SCRSIZEX + (anPos[i * 4 + 2] * 3);
		x = ((nPosX + (anPos[i * 4 + 0] * 2)) % nTmp) - (anPos[i * 4 + 2] * 2) + nMoveX;
		cx = anPos[i * 4 + 2];
		xx = anPos[i * 4 + 0];

		nTmp = SCRSIZEY + (anPos[i * 4 + 3] * 3);
		y = ((nPosY + (anPos[i * 4 + 1] * 2)) % nTmp) - (anPos[i * 4 + 3] * 2) + nMoveY;
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
