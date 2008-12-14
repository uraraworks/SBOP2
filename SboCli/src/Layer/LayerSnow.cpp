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
#include "LayerSnow.h"


/* ========================================================================= */
/* 関数名	:CLayerSnow::CLayerSnow											 */
/* 内容		:コンストラクタ													 */
/* 日付		:2008/12/14														 */
/* ========================================================================= */

CLayerSnow::CLayerSnow()
{
	m_nID = WEATHERTYPE_SNOW;

	m_dwLastProc = 0;
}


/* ========================================================================= */
/* 関数名	:CLayerSnow::~CLayerSnow										 */
/* 内容		:デストラクタ													 */
/* 日付		:2008/12/14														 */
/* ========================================================================= */

CLayerSnow::~CLayerSnow()
{
	DeleteSnowInfoAll ();
}


/* ========================================================================= */
/* 関数名	:CLayerSnow::Create												 */
/* 内容		:作成															 */
/* 日付		:2008/12/14														 */
/* ========================================================================= */

void CLayerSnow::Create(
	CMgrData	*pMgrData)		/* [in] データ管理 */
{
	CLayerBase::Create (pMgrData);

	RenewSnowInfo (200);
}


/* ========================================================================= */
/* 関数名	:CLayerSnow::Draw												 */
/* 内容		:描画															 */
/* 日付		:2008/12/14														 */
/* ========================================================================= */

void CLayerSnow::Draw(PCImg32 pDst)
{
	int i, nCount;
	PSTLAYERSNOW_SNOWINFO pInfo;

	nCount = m_aSnowInfo.GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfo = m_aSnowInfo[i];

		pDst->Circle (pInfo->x, pInfo->y, pInfo->nSize, RGB (255, 255, 255));
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

	if (dwTime - m_dwLastProc < 50) {
		goto Exit;
	}

	nCount = m_aSnowInfo.GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfo = m_aSnowInfo[i];
		if (pInfo->dwStartWait != 0) {
			if (dwTime - pInfo->dwLastProc < pInfo->dwStartWait) {
				continue;
			}
			pInfo->dwStartWait = 0;
		}
		if (dwTime - pInfo->dwLastProc < pInfo->dwWait) {
			continue;
		}
		pInfo->dwLastProc = dwTime;
		pInfo->y ++;
		if (pInfo->y >= pInfo->nEndY) {
			pInfo->y = -5;
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
		pInfo->nSize		= 3 - nTmp;
		pInfo->nEndY		= (SCRSIZEY - (genrand () % (SCRSIZEY / 3 * (nTmp + 1)))) + 32;
		pInfo->x			= (genrand () % SCRSIZEX) + 32;
		pInfo->y			= -5;
		pInfo->dwStartWait	= (genrand () % 30000);
		pInfo->dwWait		= 50 + nTmp * 25;
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
