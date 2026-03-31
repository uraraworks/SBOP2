/// @file InfoTextEffect.cpp
/// @brief 文字エフェクトクラス 実装ファイル
/// @author 年がら年中春うらら(URARA-works)
/// @date 2008/07/13
/// @copyright Copyright(C)URARA-works 2008

#include "stdafx.h"
#include "InfoTextEffect.h"

#define ANIMECOUNT	(30)	// アニメーションコマ数

CInfoTextEffect::CInfoTextEffect()
{
	m_nType	= TEXTEFFECTTYPE_NORMAL;	// 表示種別
	m_nColor	= 0;	// 色
	m_nSize	= TEXTEFFECTNUMSIZE_SMALL;	// サイズ
	m_bAnimeEnd	= FALSE;	// アニメーション終了？
	m_byAnimeNo	= 0;	// アニメーションコマ番号
	m_byAnimeCount	= 0;	// アニメーションコマ数
	m_dwLastAnime	= 0;	// 最後にアニメーションした時間
	m_dwData	= -1;	// データ
}

CInfoTextEffect::~CInfoTextEffect()
{
}

BOOL CInfoTextEffect::TimerProc(DWORD dwTime)
{
	BOOL bRet;
	DWORD dwTmp, dwWait;

	bRet = FALSE;
	if (m_bAnimeEnd) {
		goto Exit;
	}
	dwWait = 50;
	dwTmp  = dwTime - m_dwLastAnime;
	if (m_byAnimeNo >= 5) {
		dwWait  = 100;
	}
	if (dwTmp < dwWait) {
		goto Exit;
	}
	m_dwLastAnime = dwTime;
	m_byAnimeNo ++;
	if (m_byAnimeNo == 5) {
		if (m_nSize >= TEXTEFFECTNUMSIZE_MEDIUM) {
			m_nSize --;
		}
	}

	// 終了？
	if (m_byAnimeNo >= m_byAnimeCount) {
		m_bAnimeEnd = TRUE;
	}

	bRet = TRUE;
Exit:
	return FALSE;
}

void CInfoTextEffect::Set(DWORD dwData, int nType, int nColor, int nSize)
{
        if (nColor == 5) {
                m_strData = _T("0123");
	} else {
		m_strData.Format(_T("%d"), dwData);
	}
	m_nType	= nType;
	m_nColor	= nColor;
	m_nSize	= nSize;
	m_bAnimeEnd	= FALSE;
	m_byAnimeNo	= 0;
//	m_byAnimeCount	= ANIMECOUNT + ((m_strData.GetLength() - 1) * 2) + 20;
	m_byAnimeCount	= ANIMECOUNT;
	m_dwLastAnime	= timeGetTime();
	m_dwData	= dwData;
}

void CInfoTextEffect::Set(LPCSTR pszData, int nType, int nColor, int nSize)
{
	m_strData	= pszData;
	m_nType	= nType;
	m_nColor	= nColor;
	m_nSize	= nSize;
	m_bAnimeEnd	= FALSE;
	m_byAnimeNo	= 0;
	m_byAnimeCount	= ANIMECOUNT + 10;
	m_dwLastAnime	= timeGetTime();
	m_dwData	= -1;
}

int CInfoTextEffect::GetGrpCount(void)
{
	int nRet;

	nRet = 0;
	if (m_bAnimeEnd) {
		goto Exit;
	}
	if (m_dwData == -1) {
		nRet = 1;
	} else {
		nRet = m_strData.GetLength();
	}

Exit:
	return nRet;
}

int CInfoTextEffect::GetGrpNo(int nNo)
{
	char szTmp[2];

	ZeroMemory(szTmp, sizeof(szTmp));
	szTmp[0] = ((LPCSTR)m_strData)[nNo];

	return atoi(szTmp);
}

BOOL CInfoTextEffect::GetDrawPos(int nNo, POINT &ptDst, BYTE &byLevel)
{
	BOOL bRet;
	int nTmp, anPos[ANIMECOUNT] = {0, -2, -3, -4, -5, -4, -5, -6, -7, -8, -9, -10, -11, -12, -13, -14, -15, -16, -17, -18, -19, -20, -21, -22, -23, -24, -25, -26, -27, -28};

	bRet	= FALSE;
	byLevel	= 0;
	ptDst.x = ptDst.y = 0;

	nTmp = m_byAnimeNo;
	nTmp = min(nTmp, ANIMECOUNT - 1);
	ptDst.y = anPos[nTmp];
	if (nTmp >= 9) {
		byLevel = (nTmp - 9) * 5;
	}
	switch (m_nSize) {
	case TEXTEFFECTNUMSIZE_MEDIUM:
		ptDst.y -= 5;
		break;
	case TEXTEFFECTNUMSIZE_LARGE:
		ptDst.y -= 12;
		break;
	}

	bRet = TRUE;
	return bRet;
}

