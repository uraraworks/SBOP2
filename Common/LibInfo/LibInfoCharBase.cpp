/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* ファイル名	:LibInfoCharBase.cpp										 */
/* 内容			:キャラ情報基底クラス 実装ファイル							 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2006/10/01													 */
/* ========================================================================= */

#include "stdafx.h"
#include <tchar.h>
#include <math.h>
#include "LibInfoCharBase.h"


/* ========================================================================= */
/* 関数名	:CLibInfoCharBase::CLibInfoCharBase								 */
/* 内容		:コンストラクタ													 */
/* 日付		:2006/10/01														 */
/* ========================================================================= */

CLibInfoCharBase::CLibInfoCharBase()
{
	m_dwNewIDTmp	= 0;
	m_paInfo		= NULL;
}


/* ========================================================================= */
/* 関数名	:CLibInfoCharBase::~CLibInfoCharBase							 */
/* 内容		:デストラクタ													 */
/* 日付		:2006/10/01														 */
/* ========================================================================= */

CLibInfoCharBase::~CLibInfoCharBase()
{
	Destroy ();
}


/* ========================================================================= */
/* 関数名	:CLibInfoCharBase::Create										 */
/* 内容		:作成															 */
/* 日付		:2006/10/01														 */
/* ========================================================================= */

void CLibInfoCharBase::Create(void)
{
	m_paInfo = new ARRAYINFOCHARBASE;
}


/* ========================================================================= */
/* 関数名	:CLibInfoCharBase::Destroy										 */
/* 内容		:破棄															 */
/* 日付		:2006/10/01														 */
/* ========================================================================= */

void CLibInfoCharBase::Destroy(void)
{
	DeleteAll ();
	SAFE_DELETE (m_paInfo);
}


/* ========================================================================= */
/* 関数名	:CLibInfoCharBase::Proc											 */
/* 内容		:処理															 */
/* 戻り値	:TRUE:処理した FALS:処理していない								 */
/* 日付		:2007/01/21														 */
/* ========================================================================= */

BOOL CLibInfoCharBase::Proc(void)
{
	return FALSE;
}


/* ========================================================================= */
/* 関数名	:CLibInfoCharBase::RenewSize									 */
/* 内容		:マップサイズ更新												 */
/* 日付		:2007/05/01														 */
/* ========================================================================= */

void CLibInfoCharBase::RenewSize(DWORD dwMapID, int nDirection, int nSize)
{
	int i, nCount;
	PCInfoCharBase pInfoCharBase;

	/* マップサイズをキャラサイズに変更 */
	nSize *= 2;

	nCount = GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfoCharBase = (PCInfoCharBase)GetPtr (i);

		if (pInfoCharBase->m_dwMapID != dwMapID) {
			continue;
		}

		switch (nDirection) {
		case 0:		/* 上 */
			pInfoCharBase->m_nMapY += nSize;
			pInfoCharBase->m_nMapY = max (pInfoCharBase->m_nMapY, 0);
			break;
		case 1:		/* 下 */
			break;
		case 2:		/* 左 */
			pInfoCharBase->m_nMapX += nSize;
			pInfoCharBase->m_nMapX = max (pInfoCharBase->m_nMapX, 0);
			break;
		case 3:		/* 右 */
			break;
		}
	}
}


/* ========================================================================= */
/* 関数名	:CLibInfoCharBase::GetNew										 */
/* 内容		:新規データを取得												 */
/* 日付		:2007/01/21														 */
/* ========================================================================= */

PCInfoBase CLibInfoCharBase::GetNew(void)
{
	return (PCInfoBase)GetNew (CHARMOVETYPE_PC);
}


/* ========================================================================= */
/* 関数名	:CLibInfoCharBase::GetNew										 */
/* 内容		:新規データを取得												 */
/* 日付		:2006/10/01														 */
/* ========================================================================= */

PCInfoBase CLibInfoCharBase::GetNew(int nType)
{
	return (PCInfoBase)new CInfoCharBase;
}


/* ========================================================================= */
/* 関数名	:CLibInfoBase::GetSaveNo										 */
/* 内容		:保存する情報のインデックスを取得								 */
/* 日付		:2008/07/12														 */
/* ========================================================================= */

void CLibInfoCharBase::GetSaveNo(ARRAYINT &anDst)
{
	int i, nCount;
	PCInfoCharBase pChar;

	anDst.RemoveAll ();

	nCount = GetCount ();
	for (i = 0; i < nCount; i ++) {
		pChar = (PCInfoCharBase)GetPtr (i);
		if (pChar->IsLogoutDelete ()) {
			continue;
		}

		anDst.Add (i);
	}
}


/* ========================================================================= */
/* 関数名	:CLibInfoCharBase::GetCount										 */
/* 内容		:データ数を取得													 */
/* 日付		:2006/10/01														 */
/* ========================================================================= */

int CLibInfoCharBase::GetCount(void)
{
	int nRet;

	nRet = 0;

	if (m_paInfo == NULL) {
		goto Exit;
	}

	nRet = m_paInfo->GetSize ();
Exit:
	return nRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoCharBase::GetCountScreen								 */
/* 内容		:データ数を取得(画面内のキャラ)									 */
/* 日付		:2007/01/27														 */
/* ========================================================================= */

int CLibInfoCharBase::GetCountScreen(PCInfoCharBase pCharBase)
{
	BOOL bResult;
	int nRet, i, nCount;
	PCInfoCharBase pChar;

	nRet = 0;

	nCount = GetCount ();
	for (i = 0; i < nCount; i ++) {
		pChar = (PCInfoCharBase)GetPtr (i);

		bResult = IsScreenInside (pCharBase, pChar);
		if (bResult == FALSE) {
			continue;
		}
		nRet ++;
	}

	return nRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoCharBase::Add											 */
/* 内容		:追加															 */
/* 日付		:2007/04/30														 */
/* ========================================================================= */

void CLibInfoCharBase::Add(PCInfoBase pInfo)
{
	Add ((PCInfoCharBase)pInfo);
}


/* ========================================================================= */
/* 関数名	:CLibInfoCharBase::Add											 */
/* 内容		:追加															 */
/* 日付		:2006/10/01														 */
/* ========================================================================= */

DWORD CLibInfoCharBase::Add(PCInfoCharBase pChar)
{
	if (pChar->m_dwCharID == 0) {
		pChar->m_dwCharID = GetNewID ();
	}

	m_paInfo->Add (pChar);

	return pChar->m_dwCharID;
}


/* ========================================================================= */
/* 関数名	:CLibInfoCharBase::CopyAdd										 */
/* 内容		:コピーして追加													 */
/* 日付		:2007/01/27														 */
/* ========================================================================= */

void CLibInfoCharBase::CopyAdd(PCInfoCharBase pChar)
{
	PCInfoCharBase pCharTmp;

	pCharTmp = (PCInfoCharBase)GetNew (pChar->m_nMoveType);
	pCharTmp->Copy (pChar);

	m_paInfo->Add (pCharTmp);
}


/* ========================================================================= */
/* 関数名	:CLibInfoCharBase::Delete										 */
/* 内容		:削除															 */
/* 日付		:2006/10/01														 */
/* ========================================================================= */

void CLibInfoCharBase::Delete(
	int nNo)		/* [in] 配列番号 */
{
	PCInfoCharBase pInfo;

	pInfo = m_paInfo->GetAt (nNo);
	SAFE_DELETE (pInfo);
	m_paInfo->RemoveAt (nNo);
}


/* ========================================================================= */
/* 関数名	:CLibInfoCharBase::Delete										 */
/* 内容		:削除															 */
/* 日付		:2006/10/01														 */
/* ========================================================================= */

void CLibInfoCharBase::Delete(
	DWORD dwCharID)		/* [in] キャラID */
{
	int i, nCount, nNo;
	PCInfoCharBase pInfoTmp;

	nNo = -1;

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->GetAt (i);
		if (pInfoTmp->m_dwCharID != dwCharID) {
			continue;
		}
		nNo = i;
		break;
	}

	if (nNo >= 0) {
		Delete (nNo);
	}
}


/* ========================================================================= */
/* 関数名	:CLibInfoCharBase::DeleteAll									 */
/* 内容		:全て削除														 */
/* 日付		:2006/10/01														 */
/* ========================================================================= */

void CLibInfoCharBase::DeleteAll(void)
{
	int i, nCount;

	if (m_paInfo == NULL) {
		return;
	}

	nCount = m_paInfo->GetSize ();
	for (i = nCount - 1; i >= 0; i --) {
		Delete (i);
	}
	m_dwNewIDTmp = 0;
}


/* ========================================================================= */
/* 関数名	:CLibInfoCharBase::SortY										 */
/* 内容		:Y座標順にソート												 */
/* 日付		:2006/10/08														 */
/* ========================================================================= */

void CLibInfoCharBase::SortY(void)
{
	int i, j, nCount, nCountTmp, nNo;
	PCInfoCharBase pInfo, pInfoTmp;
	ARRAYINFOCHARBASE aTmp;
	ARRAYINT anNo;

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfo = m_paInfo->GetAt (i);
		nCountTmp = anNo.GetSize ();
		nNo = 0;
		if (nCountTmp > 0) {
			for (j = 0; j < nCountTmp; j ++) {
				pInfoTmp = m_paInfo->GetAt (anNo[j]);
				if (pInfo->m_dwMapID < pInfoTmp->m_dwMapID) {
					break;
				}
				if (pInfo->m_dwMapID == pInfoTmp->m_dwMapID) {
					if (pInfo->m_nMapY < pInfoTmp->m_nMapY) {
						break;
					}
				}
				if (pInfo->m_dwMapID == pInfoTmp->m_dwMapID) {
					if (pInfo->m_nMapY == pInfoTmp->m_nMapY) {
						if (pInfo->m_nMapX < pInfoTmp->m_nMapX) {
							break;
						}
					}
				}
				if (pInfo->m_dwMapID == pInfoTmp->m_dwMapID) {
					if (pInfo->m_nMapY == pInfoTmp->m_nMapY) {
						if (pInfo->m_nMapX == pInfoTmp->m_nMapX) {
							if (pInfo->m_dwCharID < pInfoTmp->m_dwCharID) {
								break;
							}
						}
					}
				}
			}
			nNo = j;
		}
		anNo.InsertAt (nNo, i);
		aTmp.InsertAt (nNo, pInfo);
	}

	m_paInfo->Copy (&aTmp);
}


/* ========================================================================= */
/* 関数名	:CLibInfoCharBase::IsBlockChar									 */
/* 内容		:一歩前でぶつかるかチェック										 */
/* 戻り値	:TRUE:ぶつかる													 */
/* 日付		:2007/09/01														 */
/* ========================================================================= */

BOOL CLibInfoCharBase::IsBlockChar(PCInfoCharBase pChar, int nDirection)
{
	BOOL bRet, bResult;
	int i, nCount, nDirectionBack;
	PCInfoCharBase pInfoCharTmp;
	POINT ptFront;
	SIZE size, sizeTmp;

	bRet = FALSE;

	nDirectionBack = pChar->m_nDirection;
	pChar->m_nDirection = nDirection;
	pChar->GetFrontPos (ptFront, nDirection, TRUE);
	pChar->GetCharSize (size);

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoCharTmp = m_paInfo->GetAt (i);
		bResult = pInfoCharTmp->IsLogin ();
		if (bResult == FALSE) {
			continue;
		}
		if (pChar == pInfoCharTmp) {
			continue;
		}
		if (pChar->m_dwMapID != pInfoCharTmp->m_dwMapID) {
			continue;
		}
		GetDistance (sizeTmp, pChar, pInfoCharTmp, TRUE);
		if ((sizeTmp.cx < 0) || (sizeTmp.cx + sizeTmp.cy >= 1)) {
			continue;
		}
		if (pInfoCharTmp->m_bBlock == FALSE) {
			continue;
		}
		bRet = TRUE;
		break;
	}
	pChar->m_nDirection = nDirectionBack;

	return bRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoCharBase::IsUseName									 */
/* 内容		:名前が使用されているかチェック									 */
/* 日付		:2006/12/03														 */
/* ========================================================================= */

BOOL CLibInfoCharBase::IsUseName(LPCSTR pszName)
{
	BOOL bRet;
	int i, nCount;
	PCInfoCharBase pInfo;

	bRet = FALSE;

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfo = m_paInfo->GetAt (i);
		if (pInfo->m_strCharName == pszName) {
			bRet = TRUE;
			break;
		}
	}

	return bRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoCharBase::NameCheck									 */
/* 内容		:名前に使用できない名前があるかチェック							 */
/* 戻り値	:TRUE:問題無し													 */
/* 日付		:2007/02/22														 */
/* ========================================================================= */

BOOL CLibInfoCharBase::NameCheck(LPCSTR pszName)
{
	int i, nLen;
	BYTE byTmp;
	BOOL bRet;
	LPCSTR pszTmp;

	bRet = FALSE;

	if (pszName == NULL) {
		goto Exit;
	}
	nLen = strlen (pszName);
	if (nLen <= 0) {
		goto Exit;
	}
	pszTmp = _tcsstr (pszName, " ");
	if (pszTmp) {
		goto Exit;
	}
	pszTmp = _tcsstr (pszName, "　");
	if (pszTmp) {
		goto Exit;
	}
	pszTmp = _tcsstr (pszName, "\t");
	if (pszTmp) {
		goto Exit;
	}

	for (i = 0; i < nLen; i ++) {
		byTmp = (BYTE)pszName[i];
		if (IsDBCSLeadByte (byTmp)) {
			i ++;
			continue;
		}
		if ((byTmp < 0x20) || ((byTmp >= 0x7F) && !((byTmp >= 0xA1) && (byTmp <= 0xDF)))) {
			goto Exit;
		}
	}

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoCharBase::GetTurnDirection								 */
/* 内容		:逆向きを取得													 */
/* 日付		:2007/09/02														 */
/* ========================================================================= */

int CLibInfoCharBase::GetTurnDirection(int nDirection)
{
	int anTurnDirection[] = {1, 0, 3, 2, 6, 7, 4, 5};

	return anTurnDirection[nDirection];
}


/* ========================================================================= */
/* 関数名	:CLibInfoCharBase::GetDistance									 */
/* 内容		:キャラ座標で距離を取得											 */
/* 日付		:2009/01/17														 */
/* ========================================================================= */

void CLibInfoCharBase::GetDistance(SIZE &sizeDst, PCInfoCharBase pInfoCharSrc, PCInfoCharBase pInfoCharDst, BOOL bFrontPos/*FALSE*/)
{
	RECT rcSrc, rcDst;

	sizeDst.cx = sizeDst.cy = -1;
	if (pInfoCharSrc->m_dwMapID != pInfoCharDst->m_dwMapID) {
		return;
	}
	/* 比較元の座標矩形を取得 */
	pInfoCharSrc->GetPosRect (rcSrc, bFrontPos);
	/* 比較先の座標矩形を取得 */
	pInfoCharDst->GetPosRect (rcDst);

	sizeDst.cx = rcSrc.left - rcDst.right;
	if (pInfoCharSrc->m_nMapX < pInfoCharDst->m_nMapX) {
		sizeDst.cx = rcDst.left - rcSrc.right;

	} else if (pInfoCharSrc->m_nMapX == pInfoCharDst->m_nMapX) {
		sizeDst.cx = 0;
	}
	sizeDst.cx = max (sizeDst.cx, 0);

	sizeDst.cy = rcSrc.top - rcDst.bottom;
	if (pInfoCharSrc->m_nMapY < pInfoCharDst->m_nMapY) {
		sizeDst.cy = rcDst.top - rcSrc.bottom;

	} else if (pInfoCharSrc->m_nMapY == pInfoCharDst->m_nMapY) {
		sizeDst.cy = 0;
	}
	sizeDst.cy = max (sizeDst.cy, 0);
}


/* ========================================================================= */
/* 関数名	:CLibInfoCharBase::IsScreenInside								 */
/* 内容		:画面内にいるかチェック											 */
/* 戻り値	:TRUE:画面内 FALSE:画面外										 */
/* 日付		:2007/01/27														 */
/* ========================================================================= */

BOOL CLibInfoCharBase::IsScreenInside(
	PCInfoCharBase pCharBase,		/* [in] 基準となるキャラ */
	PCInfoCharBase pCharTarget)		/* [in] チェック対象のキャラ */
{
	BOOL bRet, bResult;

	bRet = FALSE;

	/* ログインしていない？ */
	bResult = pCharTarget->IsLogin ();
	if (bResult == FALSE) {
		goto Exit;
	}
	/* 別のマップにいる？ */
	if (pCharBase->m_dwMapID != pCharTarget->m_dwMapID) {
		goto Exit;
	}
	if (!(
		(abs (pCharBase->m_nMapX - pCharTarget->m_nMapX) < DRAW_PARTS_X * 2) &&
		(abs (pCharBase->m_nMapY - pCharTarget->m_nMapY) < DRAW_PARTS_Y * 2))) {
		goto Exit;
	}

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoCharBase::GetFrontCharID								 */
/* 内容		:一歩前のキャラIDを取得											 */
/* 日付		:2007/07/26														 */
/* ========================================================================= */

DWORD CLibInfoCharBase::GetFrontCharID(DWORD dwCharID, int nDirection)
{
	int i, nCount;
	DWORD dwRet;
	PCInfoCharBase pInfoCharSrc, pInfoCharTmp;
	POINT ptFront;
	SIZE size;

	dwRet = 0;

	pInfoCharSrc = (PCInfoCharBase)GetPtr (dwCharID);
	if (pInfoCharSrc == NULL) {
		goto Exit;
	}
	pInfoCharSrc->GetFrontPos (ptFront, nDirection, TRUE);
	pInfoCharSrc->GetCharSize (size);

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoCharTmp = m_paInfo->GetAt (i);
		if (pInfoCharTmp == pInfoCharSrc) {
			continue;
		}
		if (pInfoCharSrc->m_dwMapID != pInfoCharTmp->m_dwMapID) {
			continue;
		}
		if (pInfoCharTmp->IsHitCharPos (ptFront.x, ptFront.y, &size) == FALSE) {
			continue;
		}
		if ((pInfoCharSrc->m_nMapX == pInfoCharTmp->m_nMapX) && (pInfoCharSrc->m_nMapY == pInfoCharTmp->m_nMapY)) {
			continue;
		}
		dwRet = pInfoCharTmp->m_dwCharID;
		break;
	}

Exit:
	return dwRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoCharBase::GetFrontCharIDPush							 */
/* 内容		:一歩前の押せるキャラIDを取得									 */
/* 日付		:2007/09/09														 */
/* ========================================================================= */

DWORD CLibInfoCharBase::GetFrontCharIDPush(DWORD dwCharID, int nDirection)
{
	int i, nCount;
	DWORD dwRet;
	PCInfoCharBase pInfoCharSrc, pInfoCharTmp;
	POINT ptFront;
	SIZE size;

	dwRet = 0;

	pInfoCharSrc = (PCInfoCharBase)GetPtr (dwCharID);
	if (pInfoCharSrc == NULL) {
		goto Exit;
	}
	pInfoCharSrc->GetFrontPos (ptFront, nDirection, TRUE);
	pInfoCharSrc->GetCharSize (size);

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoCharTmp = m_paInfo->GetAt (i);
		if (pInfoCharSrc == pInfoCharTmp) {
			continue;
		}
		if (pInfoCharTmp->m_bPush == FALSE) {
			continue;
		}
		if (pInfoCharTmp->m_nMoveType == CHARMOVETYPE_PUTNPC) {
			continue;
		}
		if (pInfoCharSrc->m_dwMapID != pInfoCharTmp->m_dwMapID) {
			continue;
		}
		if ((pInfoCharSrc->m_nMapX == pInfoCharTmp->m_nMapX) && (pInfoCharSrc->m_nMapY == pInfoCharTmp->m_nMapY)) {
			continue;
		}
		if (pInfoCharTmp->IsHitCharPos (ptFront.x, ptFront.y, &size) == FALSE) {
			continue;
		}
		dwRet = pInfoCharTmp->m_dwCharID;
		break;
	}

Exit:
	return dwRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoCharBase::GetHitCharID									 */
/* 内容		:指定座標に当たるキャラIDを取得									 */
/* 日付		:2008/12/29														 */
/* ========================================================================= */

DWORD CLibInfoCharBase::GetHitCharID(DWORD dwCharIDBase, int x, int y)
{
	int i, nCount;
	DWORD dwRet;
	PCInfoCharBase pInfoCharBase, pInfoCharTmp;
	SIZE size;

	dwRet = 0;

	pInfoCharBase = (PCInfoCharBase)GetPtr (dwCharIDBase);
	if (pInfoCharBase == NULL) {
		goto Exit;
	}
	pInfoCharBase->GetCharSize (size);

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoCharTmp = m_paInfo->GetAt (i);
		if (pInfoCharBase == pInfoCharTmp) {
			continue;
		}
		if (pInfoCharTmp->m_nMoveType == CHARMOVETYPE_PUTNPC) {
			continue;
		}
		if (pInfoCharBase->m_dwMapID != pInfoCharTmp->m_dwMapID) {
			continue;
		}
		if (pInfoCharTmp->IsHitCharPos (x, y, &size) == FALSE) {
			continue;
		}
		dwRet = pInfoCharTmp->m_dwCharID;
		break;
	}

Exit:
	return dwRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoCharBase::SetPtr										 */
/* 内容		:キャラ情報を更新												 */
/* 日付		:2007/09/02														 */
/* ========================================================================= */

void CLibInfoCharBase::SetPtr(DWORD dwCharID, PCInfoCharBase pChar)
{
	int i, nCount;
	PCInfoCharBase pTmp;

	nCount = GetCount ();
	for (i = 0; i < nCount; i ++) {
		pTmp = (PCInfoCharBase)m_paInfo->GetAt (i);
		if (pTmp->m_dwCharID == dwCharID) {
			SAFE_DELETE (pTmp);
			m_paInfo->SetAt (i, pChar);
			break;
		}
	}
}


/* ========================================================================= */
/* 関数名	:CLibInfoCharBase::GetPtr										 */
/* 内容		:キャラ情報を取得												 */
/* 日付		:2006/10/01														 */
/* ========================================================================= */

PCInfoBase CLibInfoCharBase::GetPtr(int nNo)
{
	return (PCInfoBase)m_paInfo->GetAt (nNo);
}


/* ========================================================================= */
/* 関数名	:CLibInfoCharBase::GetPtr										 */
/* 内容		:キャラ情報を取得												 */
/* 日付		:2006/10/01														 */
/* ========================================================================= */

PCInfoBase CLibInfoCharBase::GetPtr(
	DWORD dwCharID)		/* [in] キャラID */
{
	int i, nCount;
	PCInfoCharBase pRet, pInfoTmp;

	pRet = NULL;

	if (dwCharID == 0) {
		return NULL;
	}

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->GetAt (i);
		if (pInfoTmp->m_dwCharID != dwCharID) {
			continue;
		}
		pRet = pInfoTmp;
		break;
	}

	return (PCInfoBase)pRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoCharBase::GetPtrAccountID								 */
/* 内容		:キャラ情報を取得												 */
/* 日付		:2006/10/01														 */
/* ========================================================================= */

PCInfoBase CLibInfoCharBase::GetPtrAccountID(
	DWORD dwAccountID)		/* [in] アカウントID */
{
	int i, nCount;
	PCInfoCharBase pRet, pInfoTmp;

	pRet = NULL;

	if (dwAccountID == 0) {
		return NULL;
	}

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->GetAt (i);
		if (pInfoTmp->m_dwAccountID != dwAccountID) {
			continue;
		}
		pRet = pInfoTmp;
		break;
	}

	return (PCInfoBase)pRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoCharBase::GetPtrFront									 */
/* 内容		:キャラ情報を取得												 */
/* 日付		:2007/09/01														 */
/* ========================================================================= */

PCInfoBase CLibInfoCharBase::GetPtrFront(
	PCInfoCharBase pChar,		/* [in] 基準となるキャラ情報 */
	int nDirection)				/* [in] 向き */
{
	BOOL bResult;
	int i, j, nCount, nCount2;
	PCInfoCharBase pRet, pInfoCharTmp;
	CmyArray<POINT, POINT> aptFront;

	pRet = NULL;

	pChar->GetFrontPos (aptFront, nDirection);
	nCount2 = aptFront.GetSize ();

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoCharTmp = m_paInfo->GetAt (i);
		bResult = pInfoCharTmp->IsLogin ();
		if (bResult == FALSE) {
			continue;
		}
		if (pChar->m_dwMapID != pInfoCharTmp->m_dwMapID) {
			continue;
		}
		for (j = 0; j < nCount2; j ++) {
			if (pInfoCharTmp->IsHitCharPos (aptFront[j].x, aptFront[j].y)) {
				break;
			}
		}
		if (j >= nCount2) {
			continue;
		}
		pRet = pInfoCharTmp;
		break;
	}

	return (PCInfoBase)pRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoCharBase::GetSendDataSize								 */
/* 内容		:送信データサイズを取得											 */
/* 日付		:2007/01/27														 */
/* ========================================================================= */

DWORD CLibInfoCharBase::GetSendDataSize(void)
{
	int i, nCount;
	DWORD dwRet, dwSize;
	PCInfoCharBase pChar;

	dwRet = dwSize = 0;

	/* データ数分のサイズ */
	dwSize += sizeof (DWORD);

	nCount = GetCount ();
	for (i = 0; i < nCount; i ++) {
		pChar = (PCInfoCharBase)GetPtr (i);

		dwSize += pChar->GetSendDataSize ();
	}

	dwRet = dwSize;
	return dwRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoCharBase::GetSendData									 */
/* 内容		:送信データを取得												 */
/* 日付		:2007/01/27														 */
/* ========================================================================= */

PBYTE CLibInfoCharBase::GetSendData(void)
{
	int i, nCount;
	PBYTE pRet, pData, pDataTmp;
	DWORD dwCount, dwSize, dwSizeTmp, dwOffset;
	PCInfoCharBase pChar;

	pRet = NULL;

	dwSize		= GetSendDataSize ();
	pData		= ZeroNew (dwSize);
	dwOffset	= 0;

	/* データ数を書き込み */
	dwCount = (DWORD)GetCount ();
	CopyMemory (pData, &dwCount, sizeof (dwCount));
	dwOffset += sizeof (dwCount);

	/* キャラ情報を書き込み */
	nCount = GetCount ();
	for (i = 0; i < nCount; i ++) {
		pChar = (PCInfoCharBase)GetPtr (i);

		dwSizeTmp	= pChar->GetSendDataSize ();
		pDataTmp	= pChar->GetSendData ();
		CopyMemory (&pData[dwOffset], pDataTmp, dwSizeTmp);
		dwOffset += dwSizeTmp;

		SAFE_DELETE_ARRAY (pDataTmp);
	}

	pRet = pData;
	return pRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoCharBase::SetSendData									 */
/* 内容		:送信データから取り込み											 */
/* 日付		:2007/01/27														 */
/* 戻り値	:処理した後のアドレス											 */
/* ========================================================================= */

PBYTE CLibInfoCharBase::SetSendData(PBYTE pSrc)
{
	int i, nCount;
	DWORD dwCount;
	PBYTE pRet, pDataTmp;
	CInfoCharBase *pChar;

	pRet		= pSrc;
	pDataTmp	= pSrc;

	DeleteAll ();

	/* データ数を読み込み */
	CopyMemory (&dwCount, pDataTmp, sizeof (dwCount));
	nCount		= (int)dwCount;
	pDataTmp	+= sizeof (dwCount);

	for (i = 0; i < nCount; i ++) {
		CInfoCharBase CharTmp;

		pDataTmp	= CharTmp.SetSendData (pDataTmp);
		pChar		= (PCInfoCharBase)GetNew (CharTmp.m_nMoveType);
		pChar->Copy (&CharTmp);
		Add (pChar);
	}

	pRet = pDataTmp;
	return pRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoCharBase::GetDataSizeScreen							 */
/* 内容		:データサイズを取得(画面内のキャラ)								 */
/* 日付		:2007/01/27														 */
/* ========================================================================= */

DWORD CLibInfoCharBase::GetDataSizeScreen(PCInfoCharBase pCharBase)
{
	BOOL bResult;
	int i, nCount;
	DWORD dwRet, dwSize;
	PCInfoCharBase pChar;

	dwRet = dwSize = 0;

	/* データ数分のサイズ */
	dwSize += sizeof (DWORD);

	nCount = GetCount ();
	for (i = 0; i < nCount; i ++) {
		pChar = (PCInfoCharBase)GetPtr (i);

		/* 画面外？ */
		bResult = IsScreenInside (pCharBase, pChar);
		if (bResult == FALSE) {
			continue;
		}
		dwSize += pChar->GetSendDataSize ();
	}

	dwRet = dwSize;
	return dwRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoCharBase::GetDataScreen								 */
/* 内容		:データを取得(画面内のキャラ)									 */
/* 日付		:2007/01/27														 */
/* ========================================================================= */

PBYTE CLibInfoCharBase::GetDataScreen(PCInfoCharBase pCharBase)
{
	BOOL bResult;
	int i, nCount;
	PBYTE pRet, pData, pDataTmp;
	DWORD dwCount, dwSize, dwSizeTmp, dwOffset;
	PCInfoCharBase pChar;

	pRet = NULL;

	dwSize		= GetDataSizeScreen (pCharBase);
	pData		= ZeroNew (dwSize);
	dwOffset	= 0;

	/* データ数を書き込み */
	dwCount = (DWORD)GetCountScreen (pCharBase);
	CopyMemory (pData, &dwCount, sizeof (dwCount));
	dwOffset += sizeof (dwCount);

	/* キャラ情報を書き込み */
	nCount = GetCount ();
	for (i = 0; i < nCount; i ++) {
		pChar = (PCInfoCharBase)GetPtr (i);

		if (pCharBase->m_dwMapID != pChar->m_dwMapID) {
			continue;
		}
		/* 画面外？ */
		bResult = IsScreenInside (pCharBase, pChar);
		if (bResult == FALSE) {
			continue;
		}
		dwSizeTmp	= pChar->GetSendDataSize ();
		pDataTmp	= pChar->GetSendData ();
		CopyMemory (&pData[dwOffset], pDataTmp, dwSizeTmp);
		dwOffset += dwSizeTmp;

		SAFE_DELETE_ARRAY (pDataTmp);
	}

	pRet = pData;
	return pRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoCharBase::GetNewID										 */
/* 内容		:新しいキャラIDを取得											 */
/* 日付		:2006/10/01														 */
/* ========================================================================= */

DWORD CLibInfoCharBase::GetNewID(void)
{
	DWORD dwRet;
	int i, nCount;
	PCInfoCharBase pInfoTmp;

	dwRet = m_dwNewIDTmp + 1;
	if (dwRet == 0) {
		dwRet = 1;
	}

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->GetAt (i);
		if (pInfoTmp->m_dwCharID == dwRet) {
			dwRet ++;
			i = -1;
			continue;
		}
	}
	m_dwNewIDTmp = dwRet;

	return dwRet;
}

/* Copyright(C)URARA-works 2006 */
