/// @file LayerSystemMsg.cpp
/// @brief レイヤー描画クラス(システムメッセージ) 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2007/07/30
/// @copyright Copyright(C)URARA-works 2007

#include "StdAfx.h"
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include "MgrData.h"
#include "MgrGrpData.h"
#include "Img32.h"
#include "LayerSystemMsg.h"
#include "myString.h"
#include "../Platform/SdlFont.h"
#include "../../../Common/Platform/SjisConvert.h"


// 1 行の高さ（小さい文字 16px ＋縁取り上下 2px ずつ）
#define SYSTEMMSG_LINE_H	(20)

CLayerSystemMsg::CLayerSystemMsg()
{
	m_nID = LAYERTYPE_SYSTEMMSG;
	m_dwLastTimeProc = 0;
}


CLayerSystemMsg::~CLayerSystemMsg()
{
	DeleteAllMsg();
}


void CLayerSystemMsg::Draw(PCImg32 pDst)
{
	int i, nCount;
	PSYSTEMMSGINFO pInfo;

	nCount = m_aSystemMsgInfo.size();
	for (i = 0; i < nCount; i ++) {
		pInfo = m_aSystemMsgInfo[i];
		pDst->Blt(
				38, pInfo->nPosY,
				pInfo->pImg->Width(), pInfo->pImg->Height(),
				pInfo->pImg,
				0, 0, TRUE);
	}
}


BOOL CLayerSystemMsg::TimerProc(void)
{
	BOOL bRet;
	int i, nCount;
	DWORD dwTmp;
	PSYSTEMMSGINFO pInfo;

	bRet = FALSE;

	dwTmp = SDL_GetTicks() - m_dwLastTimeProc;
	// 文字のドットに合わせて 2px ずつ動かす（1px ずつだと格子からずれてにじんで見える）
	if (dwTmp < 100) {
		goto Exit;
	}
	m_dwLastTimeProc = SDL_GetTicks();

	nCount = m_aSystemMsgInfo.size();
	for (i = nCount - 1; i >= 0; i --) {
		pInfo = m_aSystemMsgInfo[i];
		pInfo->nPosY -= SdlFontPixelScale();
		if (pInfo->nPosY > SCRSIZEY - (SCRSIZEY / 3)) {
			continue;
		}
		DeleteMsg(i);
	}

Exit:
	return bRet;
}


void CLayerSystemMsg::AddMsg(LPCSTR pszMsg, COLORREF cl)
{
	int i, nLen, nCount, nShift, nTextW, nTextH;
	HDC hDCTmp;
	PSYSTEMMSGINFO pInfo, pInfoTmp;

	nCount = m_aSystemMsgInfo.size();

	pInfo = new SYSTEMMSGINFO;
	pInfo->nPosY = SCRSIZEY;
	pInfo->pImg = new CImg32;

	if (nCount > 0) {
		pInfoTmp = m_aSystemMsgInfo[nCount - 1];
		// 追加すると既存のメッセージに重なる？
		if (pInfo->nPosY <= pInfoTmp->nPosY + SYSTEMMSG_LINE_H) {
			nShift = SYSTEMMSG_LINE_H - (pInfo->nPosY - pInfoTmp->nPosY);
			for (i = 0; i < nCount; i ++) {
				pInfoTmp = m_aSystemMsgInfo[i];
				pInfoTmp->nPosY -= nShift;
			}
		}
	}

        // pszMsg は CmyString::operator LPCSTR() 経由で UTF-8 として渡る
        CString strMsg = Utf8ToTString(pszMsg);
        nLen = strMsg.GetLength();
        // 縁取り（上下左右 1 ドット＝2px）が入る大きさで作る
        nTextW = nTextH = 0;
        SdlFontGetTextExtent((void *)m_hFont, (LPCTSTR)strMsg, nLen, &nTextW, &nTextH);
        if (nTextW <= 0) {
                nTextW = nLen * 16;
        }
        pInfo->pImg->Create(nTextW + 4, SYSTEMMSG_LINE_H);

	hDCTmp = pInfo->pImg->Lock();
        TextOut2(hDCTmp, m_hFont, 2, 2, strMsg, cl);

	pInfo->pImg->Unlock();

	m_aSystemMsgInfo.push_back(pInfo);
}


void CLayerSystemMsg::DeleteMsg(int nNo)
{
	PSYSTEMMSGINFO pInfo;

        pInfo = m_aSystemMsgInfo[nNo];
        SAFE_DELETE(pInfo->pImg);
        SAFE_DELETE(pInfo);
        if ((nNo >= 0) && (nNo < static_cast<int>(m_aSystemMsgInfo.size()))) {
                m_aSystemMsgInfo.erase(m_aSystemMsgInfo.begin() + nNo);
        }
}


void CLayerSystemMsg::DeleteAllMsg(void)
{
	int i, nCount;

	nCount = m_aSystemMsgInfo.size();
	for (i = nCount - 1; i >= 0; i --) {
		DeleteMsg(i);
	}
}
