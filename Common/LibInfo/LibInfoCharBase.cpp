/// @file LibInfoCharBase.cpp
/// @brief キャラ情報基底クラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2006/10/01
/// @copyright Copyright(C)URARA-works 2006

#include "StdAfx.h"
#ifdef _WIN32
#include <tchar.h>
#endif
#include <math.h>
#include <string.h>
#include "LibInfoCharBase.h"

CLibInfoCharBase::CLibInfoCharBase()
{
	m_dwNewIDTmp	= 0;
	m_paInfo	= NULL;
}

CLibInfoCharBase::~CLibInfoCharBase()
{
	Destroy();
}

void CLibInfoCharBase::Create(void)
{
	m_paInfo = new ARRAYINFOCHARBASE;
}

void CLibInfoCharBase::Destroy(void)
{
	DeleteAll();
	SAFE_DELETE(m_paInfo);
}

BOOL CLibInfoCharBase::Proc(void)
{
	return FALSE;
}

void CLibInfoCharBase::RenewSize(DWORD dwMapID, int nDirection, int nSize)
{
	int i, nCount;
	PCInfoCharBase pInfoCharBase;

	// マップサイズをキャラサイズに変更
	nSize *= 2;

	nCount = GetCount();
	for (i = 0; i < nCount; i ++) {
		pInfoCharBase = (PCInfoCharBase)GetPtr(i);

		if (pInfoCharBase->m_dwMapID != dwMapID) {
			continue;
	}

		switch (nDirection) {
		case 0:	// 上
			pInfoCharBase->m_nMapY += nSize;
			pInfoCharBase->m_nMapY = max(pInfoCharBase->m_nMapY, 0);
			break;
		case 1:	// 下
			break;
		case 2:	// 左
			pInfoCharBase->m_nMapX += nSize;
			pInfoCharBase->m_nMapX = max(pInfoCharBase->m_nMapX, 0);
			break;
		case 3:	// 右
			break;
	}
	}
}

PCInfoBase CLibInfoCharBase::GetNew(void)
{
	return (PCInfoBase)GetNew(CHARMOVETYPE_PC);
}

PCInfoBase CLibInfoCharBase::GetNew(int nType)
{
	return (PCInfoBase)new CInfoCharBase;
}

void CLibInfoCharBase::GetSaveNo(ARRAYINT &anDst)
{
	int i, nCount;
	PCInfoCharBase pChar;

	anDst.clear();

	nCount = GetCount();
	for (i = 0; i < nCount; i ++) {
		pChar = (PCInfoCharBase)GetPtr(i);
		if (pChar->IsLogoutDelete()) {
			continue;
	}

		anDst.push_back(i);
	}
}

int CLibInfoCharBase::GetCount(void)
{
	int nRet;

	nRet = 0;

	if (m_paInfo == NULL) {
		goto Exit;
	}

	nRet = m_paInfo->size();
Exit:
	return nRet;
}

int CLibInfoCharBase::GetCountScreen(PCInfoCharBase pCharBase)
{
	BOOL bResult;
	int nRet, i, nCount;
	PCInfoCharBase pChar;

	nRet = 0;

	nCount = GetCount();
	for (i = 0; i < nCount; i ++) {
		pChar = (PCInfoCharBase)GetPtr(i);

		bResult = IsScreenInside(pCharBase, pChar);
		if (bResult == FALSE) {
			continue;
	}
		nRet ++;
	}

	return nRet;
}

void CLibInfoCharBase::Add(PCInfoBase pInfo)
{
	Add((PCInfoCharBase)pInfo);
}

DWORD CLibInfoCharBase::Add(PCInfoCharBase pChar)
{
	if (pChar->m_dwCharID == 0) {
		pChar->m_dwCharID = GetNewID();
	}

	m_paInfo->Add(pChar);

	return pChar->m_dwCharID;
}

void CLibInfoCharBase::CopyAdd(PCInfoCharBase pChar)
{
	PCInfoCharBase pCharTmp;

	pCharTmp = (PCInfoCharBase)GetNew(pChar->m_nMoveType);
	pCharTmp->Copy(pChar);

	m_paInfo->Add(pCharTmp);
}

void CLibInfoCharBase::Delete(
	int nNo)	// [in] 配列番号
{
	PCInfoCharBase pInfo;

	pInfo = m_paInfo->at(nNo);
	SAFE_DELETE(pInfo);
	if ((nNo >= 0) && (nNo < static_cast<int>(m_paInfo->size()))) {
		m_paInfo->erase(m_paInfo->begin() + nNo);
	}
}

void CLibInfoCharBase::Delete(
	DWORD dwCharID)	// [in] キャラID
{
	int i, nCount, nNo;
	PCInfoCharBase pInfoTmp;

	nNo = -1;

	nCount = m_paInfo->size();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->at(i);
		if (pInfoTmp->m_dwCharID != dwCharID) {
			continue;
	}
		nNo = i;
		break;
	}

	if (nNo >= 0) {
		Delete(nNo);
	}
}

void CLibInfoCharBase::DeleteAll(void)
{
	int i, nCount;

	if (m_paInfo == NULL) {
		return;
	}

	nCount = m_paInfo->size();
	for (i = nCount - 1; i >= 0; i --) {
		Delete(i);
	}
	m_dwNewIDTmp = 0;
}

void CLibInfoCharBase::SortY(void)
{
	int i, j, nCount, nCountTmp, nNo;
	PCInfoCharBase pInfo, pInfoTmp;
	ARRAYINFOCHARBASE aTmp;
	ARRAYINT anNo;

	nCount = m_paInfo->size();
	for (i = 0; i < nCount; i ++) {
		pInfo = m_paInfo->at(i);
		nCountTmp = anNo.size();
		nNo = 0;
		if (nCountTmp > 0) {
			for (j = 0; j < nCountTmp; j ++) {
				pInfoTmp = m_paInfo->at(anNo[j]);
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
		anNo.insert(anNo.begin() + nNo, i);
		aTmp.insert(aTmp.begin() + nNo, pInfo);
	}

	m_paInfo->Copy(&aTmp);
}

BOOL CLibInfoCharBase::IsBlockChar(
	PCInfoCharBase pChar,	// [in] 判定元のキャラ情報
	int nDirection,	// [in] 判定方向
	BOOL bNoBlockFlg/*TRUE*/,	// [in] TRUE:キャラのぶつかり判定を使用する
	BOOL bHitCheck/*FALSE*/)	// [in] TRUE:重なっている場合は判定しない
{
	BOOL bRet, bResult;
	int i, nCount, nDirectionBack;
	PCInfoCharBase pInfoCharTmp;
	POINT ptBack, ptFront;
	RECT rcSrc, rcFront, rcTmp;

	bRet = FALSE;

	ptBack.x = pChar->m_nMapX;
	ptBack.y = pChar->m_nMapY;
	nDirectionBack = pChar->m_nDirection;
	pChar->m_nDirection = nDirection;
	pChar->GetFrontPos(ptFront, nDirection, TRUE);
	pChar->GetCollisionRect(rcSrc);
	pChar->m_nMapX = ptFront.x;
	pChar->m_nMapY = ptFront.y;
	pChar->GetCollisionRectOnce(rcFront);
	pChar->m_nMapX = ptBack.x;
	pChar->m_nMapY = ptBack.y;

	nCount = m_paInfo->size();
	for (i = 0; i < nCount; i ++) {
		pInfoCharTmp = m_paInfo->at(i);
		bResult = pInfoCharTmp->IsLogin();
		if (bResult == FALSE) {
			continue;
	}
		if (pChar == pInfoCharTmp) {
			continue;
	}
		if (pChar->m_dwMapID != pInfoCharTmp->m_dwMapID) {
			continue;
	}
		pInfoCharTmp->GetCollisionRect(rcTmp);
		if (!((rcFront.left <= rcTmp.right) && (rcTmp.left <= rcFront.right) &&
			(rcFront.top <= rcTmp.bottom) && (rcTmp.top <= rcFront.bottom))) {
			continue;
		}
		if (bNoBlockFlg && (pInfoCharTmp->m_bBlock == FALSE)) {
			continue;
	}
		if (bHitCheck) {
			if ((rcSrc.left <= rcTmp.right) && (rcTmp.left <= rcSrc.right) &&
				(rcSrc.top <= rcTmp.bottom) && (rcTmp.top <= rcSrc.bottom)) {
				// 重なる場合は対象外
				continue;
		}
	}
		bRet = TRUE;
		break;
	}
	pChar->m_nDirection = nDirectionBack;

	return bRet;
}

BOOL CLibInfoCharBase::IsUseName(LPCSTR pszName)
{
	BOOL bRet;
	int i, nCount;
	PCInfoCharBase pInfo;

	bRet = FALSE;

	nCount = m_paInfo->size();
	for (i = 0; i < nCount; i ++) {
		pInfo = m_paInfo->at(i);
		if (pInfo->m_strCharName == pszName) {
			bRet = TRUE;
			break;
	}
	}

	return bRet;
}

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
	nLen = strlen(pszName);
	if (nLen <= 0) {
		goto Exit;
	}
        pszTmp = strstr(pszName, " ");
        if (pszTmp) {
                goto Exit;
        }
        pszTmp = strstr(pszName, "　");
        if (pszTmp) {
                goto Exit;
        }
        pszTmp = strstr(pszName, "\t");
	if (pszTmp) {
		goto Exit;
	}

	for (i = 0; i < nLen; i ++) {
		byTmp = (BYTE)pszName[i];
		// 制御文字 / DEL は不可
		if ((byTmp < 0x20) || (byTmp == 0x7F)) {
			goto Exit;
		}
		// ASCII 印字可文字
		if (byTmp < 0x80) {
			continue;
		}
		// SJIS の DBCS 先頭バイトなら次の 1 バイトをスキップ
		// （ブラウザ版は IsDBCSLeadByte がスタブで常に FALSE のため、
		//  SJIS 先頭バイト範囲を明示的にも判定する）
		if (IsDBCSLeadByte(byTmp) ||
			((byTmp >= 0x81) && (byTmp <= 0x9F)) ||
			((byTmp >= 0xE0) && (byTmp <= 0xFC))) {
			i ++;
			continue;
		}
		// SJIS 半角カナ (単独バイト)
		if ((byTmp >= 0xA1) && (byTmp <= 0xDF)) {
			continue;
		}
		// UTF-8 マルチバイトシーケンス (2/3/4 バイト) を許容
		if ((byTmp & 0xE0) == 0xC0) {
			i += 1;
			continue;
		}
		if ((byTmp & 0xF0) == 0xE0) {
			i += 2;
			continue;
		}
		if ((byTmp & 0xF8) == 0xF0) {
			i += 3;
			continue;
		}
		// それ以外の 0x80 以上は無効
		goto Exit;
	}

	bRet = TRUE;
Exit:
	return bRet;
}

int CLibInfoCharBase::GetTurnDirection(int nDirection)
{
	int anTurnDirection[] = {1, 0, 3, 2, 6, 7, 4, 5};

	return anTurnDirection[nDirection];
}

void CLibInfoCharBase::GetDistance(SIZE &sizeDst, PCInfoCharBase pInfoCharSrc, PCInfoCharBase pInfoCharDst, BOOL bFrontPos/*FALSE*/)
{
	RECT rcSrc, rcDst;

	sizeDst.cx = sizeDst.cy = -1;
	if (pInfoCharSrc->m_dwMapID != pInfoCharDst->m_dwMapID) {
		return;
	}
	// 比較元の座標矩形を取得
	pInfoCharSrc->GetPosRect(rcSrc, bFrontPos);
	// 比較先の座標矩形を取得
	pInfoCharDst->GetPosRect(rcDst);

	sizeDst.cx = rcSrc.left - rcDst.right;
	if (pInfoCharSrc->m_nMapX < pInfoCharDst->m_nMapX) {
		sizeDst.cx = rcDst.left - rcSrc.right;

	} else if (pInfoCharSrc->m_nMapX == pInfoCharDst->m_nMapX) {
		sizeDst.cx = 0;
	}
	sizeDst.cx = max(sizeDst.cx, 0);

	sizeDst.cy = rcSrc.top - rcDst.bottom;
	if (pInfoCharSrc->m_nMapY < pInfoCharDst->m_nMapY) {
		sizeDst.cy = rcDst.top - rcSrc.bottom;

	} else if (pInfoCharSrc->m_nMapY == pInfoCharDst->m_nMapY) {
		sizeDst.cy = 0;
	}
	sizeDst.cy = max(sizeDst.cy, 0);
}

BOOL CLibInfoCharBase::IsScreenInside(
	PCInfoCharBase pCharBase,	// [in] 基準となるキャラ
	PCInfoCharBase pCharTarget)	// [in] チェック対象のキャラ
{
	BOOL bRet, bResult;

	bRet = FALSE;

	// ログインしていない？
	bResult = pCharTarget->IsLogin();
	if (bResult == FALSE) {
		goto Exit;
	}
	// 別のマップにいる？
	if (pCharBase->m_dwMapID != pCharTarget->m_dwMapID) {
		goto Exit;
	}
	// Phase 8: m_nMapX/Y は px 単位。旧判定 (DRAW_PARTS_* * 2) は旧座標系向けで
	// 現状ほぼ自キャラしか含まれず、ゲーム開始時の周辺キャラ情報送信に NPC が乗らない。
	// GetScreenCharID と同じ px ベース判定に揃える。
	if (!(
		(abs(pCharBase->m_nMapX - pCharTarget->m_nMapX) < DRAW_PARTS_X * MAPPARTSSIZE) &&
		(abs(pCharBase->m_nMapY - pCharTarget->m_nMapY) < DRAW_PARTS_Y * MAPPARTSSIZE))) {
		goto Exit;
	}

	bRet = TRUE;
Exit:
	return bRet;
}

DWORD CLibInfoCharBase::GetFrontCharID(DWORD dwCharID, int nDirection)
{
	int i, nCount;
	DWORD dwRet;
	PCInfoCharBase pInfoCharSrc, pInfoCharTmp;
	POINT ptFront;
	SIZE size;

	dwRet = 0;

	pInfoCharSrc = (PCInfoCharBase)GetPtr(dwCharID);
	if (pInfoCharSrc == NULL) {
		goto Exit;
	}
	pInfoCharSrc->GetFrontPos(ptFront, nDirection, TRUE);
	pInfoCharSrc->GetCharSize(size);

	nCount = m_paInfo->size();
	for (i = 0; i < nCount; i ++) {
		pInfoCharTmp = m_paInfo->at(i);
		if (pInfoCharTmp == pInfoCharSrc) {
			continue;
	}
		if (pInfoCharSrc->m_dwMapID != pInfoCharTmp->m_dwMapID) {
			continue;
	}
		if (pInfoCharTmp->IsHitCharPos(ptFront.x, ptFront.y, &size) == FALSE) {
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

BOOL CLibInfoCharBase::IsPushBlockChar(PCInfoCharBase pChar, int nDirection)
{
	return GetPushBlockCharID(pChar, nDirection) != 0;
}

DWORD CLibInfoCharBase::GetPushBlockCharID(PCInfoCharBase pChar, int nDirection)
{
	// S1: 押せる物(m_bPush)の作り直し第1段階。押す動作はまだ実装せず、
	// 押せる物を「Block=0 でも固い物」として扱い、壊れた押し送信(REQ_PUSH)を止める。
	// S3b: 押し予測(MoveProc)が「どの押せる物に接しているか」を知る必要があるため、
	// IsPushBlockChar から判定本体をこちらへ切り出し、相手のCharIDを返すようにした。
	// GetCollisionRectOnce は GetDrawDirection に依存するため使わず、
	// 呼び出し元から渡された向きへ1pxだけずらした矩形で判定する。
	static const int anPosX[] = {0, 0, -1, 1, 1, 1, -1, -1};
	static const int anPosY[] = {-1, 1, 0, 0, -1, 1, 1, -1};
	int i, nCount;
	DWORD dwRet;
	BOOL bResult;
	PCInfoCharBase pInfoCharTmp;
	int nMapXBack, nMapYBack;
	RECT rcSrc, rcFront, rcTmp;

	dwRet = 0;

	if ((nDirection < 0) || (nDirection > 7)) {
		goto Exit;
	}

	// 現在位置の当たり矩形（既に重なっている相手を除外するために使う）
	pChar->GetCollisionRect(rcSrc);

	nMapXBack = pChar->m_nMapX;
	nMapYBack = pChar->m_nMapY;
	pChar->m_nMapX = nMapXBack + anPosX[nDirection];
	pChar->m_nMapY = nMapYBack + anPosY[nDirection];
	pChar->GetCollisionRect(rcFront);
	pChar->m_nMapX = nMapXBack;
	pChar->m_nMapY = nMapYBack;

	nCount = m_paInfo->size();
	for (i = 0; i < nCount; i ++) {
		pInfoCharTmp = m_paInfo->at(i);
		if (pChar == pInfoCharTmp) {
			continue;
	}
		bResult = pInfoCharTmp->IsLogin();
		if (bResult == FALSE) {
			continue;
	}
		if (pChar->m_dwMapID != pInfoCharTmp->m_dwMapID) {
			continue;
	}
		if (pInfoCharTmp->m_bPush == FALSE) {
			continue;
	}
		pInfoCharTmp->GetCollisionRect(rcTmp);
		if ((rcSrc.left <= rcTmp.right) && (rcTmp.left <= rcSrc.right) &&
			(rcSrc.top <= rcTmp.bottom) && (rcTmp.top <= rcSrc.bottom)) {
			// 現在位置で既に重なっている相手は、そこから抜け出せるように対象外にする
			continue;
	}
		if (!((rcFront.left <= rcTmp.right) && (rcTmp.left <= rcFront.right) &&
			(rcFront.top <= rcTmp.bottom) && (rcTmp.top <= rcFront.bottom))) {
			continue;
		}
		dwRet = pInfoCharTmp->m_dwCharID;
		break;
	}

Exit:
	return dwRet;
}

BOOL CLibInfoCharBase::IsPushAreaFree(PCInfoCharBase pExclude1, PCInfoCharBase pExclude2, DWORD dwMapID, const RECT &rcMoveTo)
{
	// S3b: 押せる物が1px先へ進めるか、本人・押せる物自身を除く全キャラと当たるかを見る。
	// SboSvr/src/MainFrame/MainFrameRecvProcCHAR.cpp の static IsPushCharAreaFree と
	// 同じ判定(m_bBlockは見ない)。サーバーと押し予測(クライアント)で共用するため
	// Common へ置く。クライアントは m_paInfo に画面内・既知のキャラしか持たないため、
	// サーバーより判定対象が狭くなる(=予測がサーバーよりゆるく通ることがある)点は
	// RES_PUSH による補正で吸収する想定。
	int i, nCount;
	PCInfoCharBase pInfoCharTmp;
	RECT rcTmp;

	nCount = m_paInfo->size();
	for (i = 0; i < nCount; i ++) {
		pInfoCharTmp = m_paInfo->at(i);
		if (pInfoCharTmp == NULL) {
			continue;
		}
		if ((pInfoCharTmp == pExclude1) || (pInfoCharTmp == pExclude2)) {
			continue;
		}
		if (pInfoCharTmp->IsLogin() == FALSE) {
			continue;
		}
		if (pInfoCharTmp->m_dwMapID != dwMapID) {
			continue;
		}
		pInfoCharTmp->GetCollisionRect(rcTmp);
		if ((rcMoveTo.left <= rcTmp.right) && (rcTmp.left <= rcMoveTo.right) &&
			(rcMoveTo.top <= rcTmp.bottom) && (rcTmp.top <= rcMoveTo.bottom)) {
			return FALSE;
		}
	}
	return TRUE;
}

void CLibInfoCharBase::GetMoveCheckMapRect(
	PCInfoCharBase pInfoChar,
	RECT &rcDst,
	int nDirection,
	int nLookAheadPixel)
{
	// SboCli/LibInfoCharCli.cpp の GetMoveCheckMapRect (自キャラ移動判定) を
	// S3 でサーバーと共用するため Common へ移設したもの。ロジックは変更していない。
	int anPosX[] = {0, 0, -1, 1, 1, 1, -1, -1}, anPosY[] = {-1, 1, 0, 0, -1, 1, 1, -1};
	int nMapXBack, nMapYBack;
	int nMovePixel;

	nMapXBack = pInfoChar->m_nMapX;
	nMapYBack = pInfoChar->m_nMapY;
	nMovePixel = nLookAheadPixel;
	if (nMovePixel < 0) {
		// 負値のみガード。0 は「現在位置のリーディングエッジ」を意味するのでそのまま許す
		nMovePixel = 0;
	}

	pInfoChar->m_nMapX = nMapXBack + anPosX[nDirection] * nMovePixel;
	pInfoChar->m_nMapY = nMapYBack + anPosY[nDirection] * nMovePixel;
	pInfoChar->GetCollisionRect(rcDst);
	pInfoChar->m_nMapX = nMapXBack;
	pInfoChar->m_nMapY = nMapYBack;

	// 直進時は移動方向のリーディングエッジ(先頭辺)だけを調べ、壁沿い移動時の引っ掛かりを減らす
	// (nLookAheadPixel=0 なら現在位置、正値なら移動先のリーディングエッジタイルになる)
	switch (nDirection) {
	case 0:
		rcDst.bottom = rcDst.top;
		break;
	case 1:
		rcDst.top = rcDst.bottom;
		break;
	case 2:
		rcDst.right = rcDst.left;
		break;
	case 3:
		rcDst.left = rcDst.right;
		break;
	}

	rcDst.left	/= MAPPARTSSIZE;
	rcDst.right	/= MAPPARTSSIZE;
	rcDst.top	/= MAPPARTSSIZE;
	rcDst.bottom	/= MAPPARTSSIZE;

	if ((rcDst.left < 0) || (rcDst.top < 0)) {
		SetRect(&rcDst, -1, -1, -1, -1);
	}
}

BOOL CLibInfoCharBase::CanMoveDirection(
	PCInfoMapBase pInfoMap,
	PCInfoCharBase pInfoChar,
	int nDirection)
{
	// SboCli/LibInfoCharCli.cpp の CanMoveDirection (自キャラ移動判定) を
	// S3 でサーバーと共用するため Common へ移設したもの。
	// クライアントの当たり判定先読み(CHAR_MOVE_COLLISION_LOOKAHEAD)は
	// CHAR_MOVE_SPEED_MAX と同じく常に1pxのため、ここでは1px固定で扱う
	// (押せる物の判定も1pxずつ呼ばれるため、この固定値でクライアントと一致する)。
	static const int nLookAheadPixel = 1;
	int x, y;
	int nDirTmp;
	BOOL bEscape;
	RECT rcMapNow, rcMapDst;

	if (pInfoMap == NULL) {
		return FALSE;
	}

	// 方向ブロックビットはタイルの辺属性なので、タイル境界を跨ぐ瞬間だけ判定する
	GetMoveCheckMapRect(pInfoChar, rcMapNow, nDirection, 0);
	GetMoveCheckMapRect(pInfoChar, rcMapDst, nDirection, nLookAheadPixel);

	// 移動先がマップ外(無効矩形)なら移動不可
	if ((rcMapDst.left == -1) && (rcMapDst.top == -1) &&
	    (rcMapDst.right == -1) && (rcMapDst.bottom == -1)) {
		return FALSE;
	}

	// タイル境界を跨がない移動は辺ビット判定不要
	if ((rcMapNow.left == rcMapDst.left) && (rcMapNow.top == rcMapDst.top) &&
	    (rcMapNow.right == rcMapDst.right) && (rcMapNow.bottom == rcMapDst.bottom)) {
		return TRUE;
	}

	// 出口チェック: 今いるタイルの辺から出られるか
	for (y = rcMapNow.top; y <= rcMapNow.bottom; y ++) {
		for (x = rcMapNow.left; x <= rcMapNow.right; x ++) {
			if (!pInfoMap->IsMoveOut(x, y, nDirection)) {
				// どの方向からも進入できないタイル（全方向ブロック等）に
				// めり込んでいる場合だけ、脱出用に出口チェックを免除する。
				// 一部方向からのみ進入できるタイル（例: 右からだけ入れるイス）は
				// 正規に重なれるため、辺ビット通りに出口をブロックする
				bEscape = TRUE;
				for (nDirTmp = 0; nDirTmp < 4; nDirTmp ++) {
					if (pInfoMap->IsMove(x, y, nDirTmp)) {
						bEscape = FALSE;
						break;
					}
				}
				if (bEscape == FALSE) {
					return FALSE;
				}
			}
		}
	}

	// 入口チェック: 移動先タイルの辺から入れるか
	for (y = rcMapDst.top; y <= rcMapDst.bottom; y ++) {
		for (x = rcMapDst.left; x <= rcMapDst.right; x ++) {
			if (!pInfoMap->IsMove(x, y, nDirection)) {
				return FALSE;
			}
		}
	}

	return TRUE;
}

DWORD CLibInfoCharBase::GetHitCharID(DWORD dwCharIDBase, int x, int y)
{
	int i, nCount;
	DWORD dwRet;
	PCInfoCharBase pInfoCharBase, pInfoCharTmp;
	SIZE size;

	dwRet = 0;

	pInfoCharBase = (PCInfoCharBase)GetPtr(dwCharIDBase);
	if (pInfoCharBase == NULL) {
		goto Exit;
	}
	pInfoCharBase->GetCharSize(size);

	nCount = m_paInfo->size();
	for (i = 0; i < nCount; i ++) {
		pInfoCharTmp = m_paInfo->at(i);
		if (pInfoCharBase == pInfoCharTmp) {
			continue;
	}
		if (pInfoCharTmp->m_nMoveType == CHARMOVETYPE_PUTNPC) {
			continue;
	}
		if (pInfoCharBase->m_dwMapID != pInfoCharTmp->m_dwMapID) {
			continue;
	}
		if (pInfoCharTmp->IsHitCharPos(x, y, &size) == FALSE) {
			continue;
	}
		dwRet = pInfoCharTmp->m_dwCharID;
		break;
	}

Exit:
	return dwRet;
}

void CLibInfoCharBase::SetPtr(DWORD dwCharID, PCInfoCharBase pChar)
{
	int i, nCount;
	PCInfoCharBase pTmp;

	nCount = GetCount();
	for (i = 0; i < nCount; i ++) {
		pTmp = (PCInfoCharBase)m_paInfo->at(i);
                if (pTmp->m_dwCharID == dwCharID) {
                        SAFE_DELETE(pTmp);
                        (*m_paInfo)[i] = pChar;
                        break;
                }
	}
}

PCInfoBase CLibInfoCharBase::GetPtr(int nNo)
{
	return (PCInfoBase)m_paInfo->at(nNo);
}

PCInfoBase CLibInfoCharBase::GetPtr(
	DWORD dwCharID)	// [in] キャラID
{
	int i, nCount;
	PCInfoCharBase pRet, pInfoTmp;

	pRet = NULL;

	if (dwCharID == 0) {
		return NULL;
	}

	nCount = m_paInfo->size();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->at(i);
		if (pInfoTmp->m_dwCharID != dwCharID) {
			continue;
	}
		pRet = pInfoTmp;
		break;
	}

	return (PCInfoBase)pRet;
}

PCInfoBase CLibInfoCharBase::GetPtrAccountID(
	DWORD dwAccountID)	// [in] アカウントID
{
	int i, nCount;
	PCInfoCharBase pRet, pInfoTmp;

	pRet = NULL;

	if (dwAccountID == 0) {
		return NULL;
	}

	nCount = m_paInfo->size();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->at(i);
		if (pInfoTmp->m_dwAccountID != dwAccountID) {
			continue;
	}
		pRet = pInfoTmp;
		break;
	}

	return (PCInfoBase)pRet;
}

PCInfoBase CLibInfoCharBase::GetPtrFront(
	PCInfoCharBase pChar,	// [in] 基準となるキャラ情報
	int nDirection)	// [in] 向き
{
	BOOL bResult;
	int i, j, nCount, nCount2;
	PCInfoCharBase pRet, pInfoCharTmp;
	std::vector<POINT> aptFront;

	pRet = NULL;

	pChar->GetFrontPos(aptFront, nDirection);
	nCount2 = aptFront.size();

	nCount = m_paInfo->size();
	for (i = 0; i < nCount; i ++) {
		pInfoCharTmp = m_paInfo->at(i);
		bResult = pInfoCharTmp->IsLogin();
		if (bResult == FALSE) {
			continue;
	}
		if (pChar->m_dwMapID != pInfoCharTmp->m_dwMapID) {
			continue;
	}
		for (j = 0; j < nCount2; j ++) {
			if (pInfoCharTmp->IsHitCharPos(aptFront[j].x, aptFront[j].y)) {
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

DWORD CLibInfoCharBase::GetSendDataSize(void)
{
	int i, nCount;
	DWORD dwRet, dwSize;
	PCInfoCharBase pChar;

	dwRet = dwSize = 0;

	// データ数分のサイズ
	dwSize += sizeof(DWORD);

	nCount = GetCount();
	for (i = 0; i < nCount; i ++) {
		pChar = (PCInfoCharBase)GetPtr(i);

		dwSize += pChar->GetSendDataSize();
	}

	dwRet = dwSize;
	return dwRet;
}

PBYTE CLibInfoCharBase::GetSendData(void)
{
	int i, nCount;
	PBYTE pRet, pData, pDataTmp;
	DWORD dwCount, dwSize, dwSizeTmp, dwOffset;
	PCInfoCharBase pChar;

	pRet = NULL;

	dwSize	= GetSendDataSize();
	pData	= ZeroNew(dwSize);
	dwOffset	= 0;

	// データ数を書き込み
	dwCount = (DWORD)GetCount();
	CopyMemory(pData, &dwCount, sizeof(dwCount));
	dwOffset += sizeof(dwCount);

	// キャラ情報を書き込み
	nCount = GetCount();
	for (i = 0; i < nCount; i ++) {
		pChar = (PCInfoCharBase)GetPtr(i);

		dwSizeTmp	= pChar->GetSendDataSize();
		pDataTmp	= pChar->GetSendData();
		CopyMemory(&pData[dwOffset], pDataTmp, dwSizeTmp);
		dwOffset += dwSizeTmp;

		SAFE_DELETE_ARRAY(pDataTmp);
	}

	pRet = pData;
	return pRet;
}

PBYTE CLibInfoCharBase::SetSendData(PBYTE pSrc)
{
	int i, nCount;
	DWORD dwCount;
	PBYTE pRet, pDataTmp;
	CInfoCharBase *pChar;

	pRet	= pSrc;
	pDataTmp	= pSrc;

	DeleteAll();

	// データ数を読み込み
	CopyMemory(&dwCount, pDataTmp, sizeof(dwCount));
	nCount	= (int)dwCount;
	pDataTmp	+= sizeof(dwCount);

	for (i = 0; i < nCount; i ++) {
		CInfoCharBase CharTmp;

		pDataTmp	= CharTmp.SetSendData(pDataTmp);
		pChar	= (PCInfoCharBase)GetNew(CharTmp.m_nMoveType);
		pChar->Copy(&CharTmp);
		Add(pChar);
	}

	pRet = pDataTmp;
	return pRet;
}

DWORD CLibInfoCharBase::GetDataSizeScreen(PCInfoCharBase pCharBase)
{
	BOOL bResult;
	int i, nCount;
	DWORD dwRet, dwSize;
	PCInfoCharBase pChar;

	dwRet = dwSize = 0;

	// データ数分のサイズ
	dwSize += sizeof(DWORD);

	nCount = GetCount();
	for (i = 0; i < nCount; i ++) {
		pChar = (PCInfoCharBase)GetPtr(i);

		// 画面外？
		bResult = IsScreenInside(pCharBase, pChar);
		if (bResult == FALSE) {
			continue;
	}
		dwSize += pChar->GetSendDataSize();
	}

	dwRet = dwSize;
	return dwRet;
}

PBYTE CLibInfoCharBase::GetDataScreen(PCInfoCharBase pCharBase)
{
	BOOL bResult;
	int i, nCount;
	PBYTE pRet, pData, pDataTmp;
	DWORD dwCount, dwSize, dwSizeTmp, dwOffset;
	PCInfoCharBase pChar;

	pRet = NULL;

	dwSize	= GetDataSizeScreen(pCharBase);
	pData	= ZeroNew(dwSize);
	dwOffset	= 0;

	// データ数を書き込み
	dwCount = (DWORD)GetCountScreen(pCharBase);
	CopyMemory(pData, &dwCount, sizeof(dwCount));
	dwOffset += sizeof(dwCount);

	// キャラ情報を書き込み
	nCount = GetCount();
	for (i = 0; i < nCount; i ++) {
		pChar = (PCInfoCharBase)GetPtr(i);

		if (pCharBase->m_dwMapID != pChar->m_dwMapID) {
			continue;
	}
		// 画面外？
		bResult = IsScreenInside(pCharBase, pChar);
		if (bResult == FALSE) {
			continue;
	}
		dwSizeTmp	= pChar->GetSendDataSize();
		pDataTmp	= pChar->GetSendData();
		CopyMemory(&pData[dwOffset], pDataTmp, dwSizeTmp);
		dwOffset += dwSizeTmp;

		SAFE_DELETE_ARRAY(pDataTmp);
	}

	pRet = pData;
	return pRet;
}

DWORD CLibInfoCharBase::GetNewID(void)
{
	DWORD dwRet;
	int i, nCount;
	PCInfoCharBase pInfoTmp;

	dwRet = m_dwNewIDTmp + 1;
	if (dwRet == 0) {
		dwRet = 1;
	}

	nCount = m_paInfo->size();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->at(i);
		if (pInfoTmp->m_dwCharID == dwRet) {
			dwRet ++;
			i = -1;
			continue;
	}
	}
	m_dwNewIDTmp = dwRet;

	return dwRet;
}

