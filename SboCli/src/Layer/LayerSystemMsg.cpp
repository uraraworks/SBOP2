/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:LayerSystemMsg.cpp											 */
/* 内容			:レイヤー描画クラス(システムメッセージ) 実装ファイル		 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/07/30													 */
/* ========================================================================= */

#include "stdafx.h"
#include "MgrData.h"
#include "MgrGrpData.h"
#include "Img32.h"
#include "LayerSystemMsg.h"


/* ========================================================================= */
/* 関数名	:CLayerSystemMsg::CLayerSystemMsg								 */
/* 内容		:コンストラクタ													 */
/* 日付		:2007/07/30														 */
/* ========================================================================= */

CLayerSystemMsg::CLayerSystemMsg()
{
	m_nID				= LAYERTYPE_SYSTEMMSG;
	m_dwLastTimeProc	= 0;
}


/* ========================================================================= */
/* 関数名	:CLayerSystemMsg::~CLayerSystemMsg								 */
/* 内容		:デストラクタ													 */
/* 日付		:2007/07/30														 */
/* ========================================================================= */

CLayerSystemMsg::~CLayerSystemMsg()
{
	DeleteAllMsg ();
}


/* ========================================================================= */
/* 関数名	:CLayerSystemMsg::Draw											 */
/* 内容		:描画															 */
/* 日付		:2007/07/30														 */
/* ========================================================================= */

void CLayerSystemMsg::Draw(PCImg32 pDst)
{
	int i, nCount;
	PSYSTEMMSGINFO pInfo;

	nCount = m_aSystemMsgInfo.GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfo = m_aSystemMsgInfo[i];
		pDst->Blt (
				38, pInfo->nPosY,
				pInfo->pImg->Width (), pInfo->pImg->Height (),
				pInfo->pImg,
				0, 0, TRUE);
	}
}


/* ========================================================================= */
/* 関数名	:CLayerSystemMsg::TimerProc										 */
/* 内容		:時間処理														 */
/* 日付		:2007/07/30														 */
/* ========================================================================= */

BOOL CLayerSystemMsg::TimerProc(void)
{
	BOOL bRet;
	int i, nCount;
	DWORD dwTmp;
	PSYSTEMMSGINFO pInfo;

	bRet = FALSE;

	dwTmp = timeGetTime () - m_dwLastTimeProc;
	if (dwTmp < 50) {
		goto Exit;
	}
	m_dwLastTimeProc = timeGetTime ();

	nCount = m_aSystemMsgInfo.GetSize ();
	for (i = nCount - 1; i >= 0; i --) {
		pInfo = m_aSystemMsgInfo[i];
		pInfo->nPosY --;
		if (pInfo->nPosY > SCRSIZEY - (SCRSIZEY / 3)) {
			continue;
		}
		DeleteMsg (i);
	}

Exit:
	return bRet;
}


/* ========================================================================= */
/* 関数名	:CLayerSystemMsg::AddMsg										 */
/* 内容		:描画															 */
/* 日付		:2007/07/30														 */
/* ========================================================================= */

void CLayerSystemMsg::AddMsg(LPCSTR pszMsg, COLORREF cl)
{
	int i, nLen, nCount;
	HDC hDCTmp;
	HFONT hFontOld;
	PSYSTEMMSGINFO pInfo, pInfoTmp;

	nCount = m_aSystemMsgInfo.GetSize ();

	pInfo = new SYSTEMMSGINFO;
	pInfo->nPosY	= SCRSIZEY;
	pInfo->pImg		= new CImg32;

	if (nCount > 0) {
		pInfoTmp = m_aSystemMsgInfo[nCount - 1];
		/* 追加すると既存のメッセージに重なる？ */
		if (pInfo->nPosY <= pInfoTmp->nPosY + 14) {
			for (i = 0; i < nCount; i ++) {
				pInfoTmp = m_aSystemMsgInfo[i];
				pInfoTmp->nPosY -= 14;
			}
		}
	}

	nLen = strlen (pszMsg);
	pInfo->pImg->Create (nLen * 14 + 1, 14);

	hDCTmp		= pInfo->pImg->Lock ();
	hFontOld	= (HFONT)SelectObject (hDCTmp, m_hFont);
	SetBkMode (hDCTmp, TRANSPARENT);
	TextOut2 (hDCTmp, 1, 1, pszMsg, cl);

	SelectObject (hDCTmp, hFontOld);
	pInfo->pImg->Unlock ();

	m_aSystemMsgInfo.Add (pInfo);
}


/* ========================================================================= */
/* 関数名	:CLayerSystemMsg::DeleteMsg										 */
/* 内容		:メッセージを削除												 */
/* 日付		:2007/07/30														 */
/* ========================================================================= */

void CLayerSystemMsg::DeleteMsg(int nNo)
{
	PSYSTEMMSGINFO pInfo;

	pInfo = m_aSystemMsgInfo[nNo];
	SAFE_DELETE (pInfo->pImg);
	SAFE_DELETE (pInfo);
	m_aSystemMsgInfo.RemoveAt (nNo);
}


/* ========================================================================= */
/* 関数名	:CLayerSystemMsg::DeleteAllMsg									 */
/* 内容		:全てのメッセージを削除											 */
/* 日付		:2007/07/30														 */
/* ========================================================================= */

void CLayerSystemMsg::DeleteAllMsg(void)
{
	int i, nCount;

	nCount = m_aSystemMsgInfo.GetSize ();
	for (i = nCount - 1; i >= 0; i --) {
		DeleteMsg (i);
	}
}

/* Copyright(C)URARA-works 2007 */
