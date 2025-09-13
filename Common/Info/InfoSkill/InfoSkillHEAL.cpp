/* Copyright(C)URARA-works 2009 */
/* ========================================================================= */
/* ファイル名	:InfoSkillHEAL.cpp											 */
/* 内容			:スキル情報(回復)クラス 実装ファイル						 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2009/01/22													 */
/* ========================================================================= */

#include "stdafx.h"
#include "InfoSkillHEAL.h"

/* ========================================================================= */
/* 定数定義																	 */
/* ========================================================================= */

/* ヘッダ情報 */
static LPCSTR s_aszName[] = {
	"m_dwArea",			/* 範囲 */
	"m_dwHealType",		/* 回復種別 */
	"m_dwHitEffectID",	/* ヒット時の表示エフェクト */
	"m_dwValue1",		/* 効果1 */
	"m_dwValue2",		/* 効果2 */
	"m_dwDistance",		/* 射程距離 */
	NULL
};


/* ========================================================================= */
/* 関数名	:CInfoSkillHEAL::CInfoSkillHEAL									 */
/* 内容		:コンストラクタ													 */
/* 日付		:2009/01/22														 */
/* ========================================================================= */

CInfoSkillHEAL::CInfoSkillHEAL()
{
	m_nClassType		= INFOSKILLTYPE_HEAL;
	m_dwArea			= 0;
	m_dwHealType		= 0;
	m_dwHitEffectID		= 0;
	m_dwValue1			= 0;
	m_dwValue2			= 0;
	m_dwDistance		= 0;

	for (m_nElementCountDerivation = 0; s_aszName[m_nElementCountDerivation] != NULL; m_nElementCountDerivation ++) {}
}


/* ========================================================================= */
/* 関数名	:CInfoSkillHEAL::~CInfoSkillHEAL								 */
/* 内容		:デストラクタ													 */
/* 日付		:2009/01/22														 */
/* ========================================================================= */

CInfoSkillHEAL::~CInfoSkillHEAL()
{
}


/* ========================================================================= */
/* 関数名	:CInfoSkillHEAL::GetElementNoTmp								 */
/* 内容		:要素番号を取得													 */
/* 日付		:2009/01/22														 */
/* ========================================================================= */

int CInfoSkillHEAL::GetElementNoTmp(LPCSTR pszName)
{
	int i, nRet;

	nRet = -1;
	for (i = 0; s_aszName[i] != NULL; i ++) {
		if (strcmp (s_aszName[i], pszName) == 0) {
			nRet = i;
			break;
		}
	}

	return nRet;
}


/* ========================================================================= */
/* 関数名	:CInfoSkillHEAL::GetDataSizeTmp									 */
/* 内容		:データサイズを取得												 */
/* 日付		:2009/01/22														 */
/* ========================================================================= */

DWORD CInfoSkillHEAL::GetDataSizeTmp(void)
{
	DWORD dwRet;

	dwRet = 0;
	dwRet += sizeof (m_dwArea);				/* 範囲 */
	dwRet += sizeof (m_dwHealType);			/* 回復種別 */
	dwRet += sizeof (m_dwHitEffectID);		/* ヒット時の表示エフェクト */
	dwRet += sizeof (m_dwValue1);			/* 効果1 */
	dwRet += sizeof (m_dwValue2);			/* 効果2 */
	dwRet += sizeof (m_dwDistance);			/* 射程距離 */

	return dwRet;
}


/* ========================================================================= */
/* 関数名	:CInfoSkillHEAL::GetDataSizeNoTmp								 */
/* 内容		:指定要素のデータサイズを取得									 */
/* 日付		:2009/01/22														 */
/* ========================================================================= */

DWORD CInfoSkillHEAL::GetDataSizeNoTmp(int nNo)
{
	DWORD dwRet;

	dwRet = 0;
	switch (nNo) {
	case 0:	dwRet = sizeof (m_dwArea);			break;		/* 範囲 */
	case 1:	dwRet = sizeof (m_dwHealType);		break;		/* 回復種別 */
	case 2:	dwRet = sizeof (m_dwHitEffectID);	break;		/* ヒット時の表示エフェクト */
	case 3:	dwRet = sizeof (m_dwValue1);		break;		/* 効果1 */
	case 4:	dwRet = sizeof (m_dwValue2);		break;		/* 効果2 */
	case 5:	dwRet = sizeof (m_dwDistance);		break;		/* 射程距離 */
	}

	return dwRet;
}


/* ========================================================================= */
/* 関数名	:CInfoSkillHEAL::GetNameTmp										 */
/* 内容		:要素名を取得													 */
/* 日付		:2009/01/22														 */
/* ========================================================================= */

LPCSTR CInfoSkillHEAL::GetNameTmp(int nNo)
{
	return s_aszName[nNo];
}


/* ========================================================================= */
/* 関数名	:CInfoSkillHEAL::GetWriteDataTmp								 */
/* 内容		:指定要素の保存用データを取得									 */
/* 日付		:2009/01/22														 */
/* ========================================================================= */

PBYTE CInfoSkillHEAL::GetWriteDataTmp(int nNo, PDWORD pdwSize)
{
	PBYTE pRet, pSrc;
	DWORD dwSize;

	pRet		= NULL;
	pSrc		= NULL;
	dwSize		= GetDataSizeNoTmp (nNo);
	*pdwSize	= dwSize;

	if (dwSize == 0) {
		goto Exit;
	}
	pRet = new BYTE[dwSize];

	switch (nNo) {
	case 0:	pSrc = (PBYTE)&m_dwArea;			break;		/* 範囲 */
	case 1:	pSrc = (PBYTE)&m_dwHealType;		break;		/* 回復種別 */
	case 2:	pSrc = (PBYTE)&m_dwHitEffectID;		break;		/* ヒット時の表示エフェクト */
	case 3:	pSrc = (PBYTE)&m_dwValue1;			break;		/* 効果1 */
	case 4:	pSrc = (PBYTE)&m_dwValue2;			break;		/* 効果2 */
	case 5:	pSrc = (PBYTE)&m_dwDistance;		break;		/* 射程距離 */
	}

	if (pSrc) {
		CopyMemory (pRet, pSrc, dwSize);
	}

Exit:
	return pRet;
}


/* ========================================================================= */
/* 関数名	:CInfoSkillHEAL::ReadElementDataTmp								 */
/* 内容		:指定要素データを読み込み										 */
/* 日付		:2009/01/22														 */
/* ========================================================================= */

DWORD CInfoSkillHEAL::ReadElementDataTmp(
	PBYTE pSrc,		/* [in] データの読み込み元 */
	int nNo)		/* [in] 要素番号 */
{
	PBYTE pDst;
	DWORD dwSize;

	pDst	= NULL;
	dwSize	= 0;

	switch (nNo) {
	case 0:	pDst = (PBYTE)&m_dwArea;			dwSize = sizeof (m_dwArea);			break;	/* 範囲 */
	case 1:	pDst = (PBYTE)&m_dwHealType;		dwSize = sizeof (m_dwHealType);		break;	/* 回復種別 */
	case 2:	pDst = (PBYTE)&m_dwHitEffectID;		dwSize = sizeof (m_dwHitEffectID);	break;	/* ヒット時の表示エフェクト */
	case 3:	pDst = (PBYTE)&m_dwValue1;			dwSize = sizeof (m_dwValue1);		break;	/* 効果1 */
	case 4:	pDst = (PBYTE)&m_dwValue2;			dwSize = sizeof (m_dwValue2);		break;	/* 効果2 */
	case 5:	pDst = (PBYTE)&m_dwDistance;		dwSize = sizeof (m_dwDistance);		break;	/* 射程距離 */
	}

	if (pDst) {
		CopyMemory (pDst, pSrc, dwSize);
	}

	return dwSize;
}


/* ========================================================================= */
/* 関数名	:CInfoSkillHEAL::GetDerivationSize								 */
/* 内容		:派生データサイズを取得											 */
/* 日付		:2009/01/10														 */
/* ========================================================================= */

DWORD CInfoSkillHEAL::GetDerivationSize(void)
{
	int i, nCount;
	DWORD dwRet;

	dwRet = sizeof (int);		/* 要素数 */
	nCount = m_nElementCountDerivation;
	for (i = 0; i < nCount; i ++) {
		dwRet += (strlen (GetNameTmp (i)) + 1);		/* 要素名 */
		dwRet += sizeof (DWORD);					/* データサイズ */
		dwRet += GetDataSizeNoTmp (i);				/* データ */
	}

	return dwRet;
}


/* ========================================================================= */
/* 関数名	:CInfoSkillHEAL::GetDerivationWriteData							 */
/* 内容		:派生データの保存用データを取得									 */
/* 日付		:2009/01/10														 */
/* ========================================================================= */

PBYTE CInfoSkillHEAL::GetDerivationWriteData(PDWORD pdwSize)
{
	int i, nCount;
	PBYTE pRet, pRetTmp, pTmp;
	DWORD dwSize, dwTmp;

	dwSize	= GetDerivationSize ();
	pRet	= ZeroNew (dwSize);
	pRetTmp	= pRet;

	nCount = m_nElementCountDerivation;
	CopyMemoryRenew (pRetTmp, &nCount, sizeof (nCount), pRetTmp);	/* 要素数 */
	for (i = 0; i < nCount; i ++) {
		strcpyRenew ((LPSTR)pRetTmp, GetNameTmp (i), pRetTmp); 		/* 要素名 */
		dwTmp = GetDataSizeNoTmp (i);
		CopyMemoryRenew (pRetTmp, &dwTmp, sizeof (dwTmp), pRetTmp);	/* データサイズ */
		pTmp = GetWriteDataTmp (i, &dwTmp);
		CopyMemoryRenew (pRetTmp, pTmp, dwTmp, pRetTmp);			/* データ */
		SAFE_DELETE_ARRAY (pTmp);
	}

	return pRet;
}


/* ========================================================================= */
/* 関数名	:CInfoSkillHEAL::ReadDerivationData								 */
/* 内容		:派生データを読み込み											 */
/* 日付		:2009/01/10														 */
/* ========================================================================= */

DWORD CInfoSkillHEAL::ReadDerivationData(PBYTE pSrc)
{
	int i, nCount, nNoTmp;
	PBYTE pSrcTmp;
	DWORD dwSizeTmp;
	CmyString strTmp;

	pSrcTmp = pSrc;

	CopyMemoryRenew (&nCount, pSrcTmp, sizeof (nCount), pSrcTmp);	/* 要素数 */
	for (i = 0; i < nCount; i ++) {
		StoreRenew (strTmp, (LPCSTR)pSrcTmp, pSrcTmp);						/* 要素名 */
		CopyMemoryRenew (&dwSizeTmp, pSrcTmp, sizeof (dwSizeTmp), pSrcTmp);	/* データサイズ */
		nNoTmp = GetElementNoTmp ((LPCSTR)strTmp);
		if (nNoTmp >= 0) {
			dwSizeTmp = ReadElementDataTmp (pSrcTmp, nNoTmp);
		}
		pSrcTmp += dwSizeTmp;
	}

	return (DWORD)(pSrcTmp - pSrc);
}


/* ========================================================================= */
/* 関数名	:CInfoSkillHEAL::GetSendDataSize								 */
/* 内容		:送信データサイズを取得											 */
/* 日付		:2009/01/22														 */
/* ========================================================================= */

DWORD CInfoSkillHEAL::GetSendDataSize(void)
{
	DWORD dwRet;

	dwRet = CInfoSkillBase::GetSendDataSize ();
	dwRet += sizeof (m_dwArea);				/* 範囲 */
	dwRet += sizeof (m_dwHealType);			/* 回復種別 */
	dwRet += sizeof (m_dwHitEffectID);		/* ヒット時の表示エフェクト */
	dwRet += sizeof (m_dwValue1);			/* 効果1 */
	dwRet += sizeof (m_dwValue2);			/* 効果2 */
	dwRet += sizeof (m_dwDistance);			/* 射程距離 */

	return dwRet;
}


/* ========================================================================= */
/* 関数名	:CInfoSkillHEAL::GetSendData									 */
/* 内容		:送信データを取得												 */
/* 日付		:2009/01/22														 */
/* ========================================================================= */

PBYTE CInfoSkillHEAL::GetSendData(void)
{
	PBYTE pData, pDataTmp, pDataBase;
	DWORD dwSize, dwSizeBase;

	dwSize	 = GetSendDataSize ();
	pData	 = ZeroNew (dwSize);
	pDataTmp = pData;

	pDataBase  = CInfoSkillBase::GetSendData ();
	dwSizeBase = CInfoSkillBase::GetSendDataSize ();
	CopyMemoryRenew (pDataTmp, pDataBase, dwSizeBase, pDataTmp);
	SAFE_DELETE_ARRAY (pDataBase);

	CopyMemoryRenew (pDataTmp, &m_dwArea,			sizeof (m_dwArea),			pDataTmp);	/* 範囲 */
	CopyMemoryRenew (pDataTmp, &m_dwHealType,		sizeof (m_dwHealType),		pDataTmp);	/* 回復種別 */
	CopyMemoryRenew (pDataTmp, &m_dwHitEffectID,	sizeof (m_dwHitEffectID),	pDataTmp);	/* ヒット時の表示エフェクト */
	CopyMemoryRenew (pDataTmp, &m_dwValue1,			sizeof (m_dwValue1),		pDataTmp);	/* 効果1 */
	CopyMemoryRenew (pDataTmp, &m_dwValue2,			sizeof (m_dwValue2),		pDataTmp);	/* 効果2 */
	CopyMemoryRenew (pDataTmp, &m_dwDistance,		sizeof (m_dwDistance),		pDataTmp);	/* 射程距離 */

	return pData;
}


/* ========================================================================= */
/* 関数名	:CInfoSkillHEAL::SetSendData									 */
/* 内容		:送信データから取り込み											 */
/* 日付		:2009/01/22														 */
/* ========================================================================= */

PBYTE CInfoSkillHEAL::SetSendData(PBYTE pSrc)
{
	PBYTE pRet, pDataTmp;

	pRet = pSrc;

	pDataTmp = CInfoSkillBase::SetSendData (pSrc);
	CopyMemoryRenew (&m_dwArea,			pDataTmp, sizeof (m_dwArea),		pDataTmp);	/* 範囲 */
	CopyMemoryRenew (&m_dwHealType,		pDataTmp, sizeof (m_dwHealType),	pDataTmp);	/* 回復種別 */
	CopyMemoryRenew (&m_dwHitEffectID,	pDataTmp, sizeof (m_dwHitEffectID),	pDataTmp);	/* ヒット時の表示エフェクト */
	CopyMemoryRenew (&m_dwValue1,		pDataTmp, sizeof (m_dwValue1),		pDataTmp);	/* 効果1 */
	CopyMemoryRenew (&m_dwValue2,		pDataTmp, sizeof (m_dwValue2),		pDataTmp);	/* 効果2 */
	CopyMemoryRenew (&m_dwDistance,		pDataTmp, sizeof (m_dwDistance),	pDataTmp);	/* 射程距離 */

	pRet = pDataTmp;
	return pRet;
}


/* ========================================================================= */
/* 関数名	:CInfoSkillHEAL::Copy											 */
/* 内容		:コピー															 */
/* 日付		:2009/01/22														 */
/* ========================================================================= */

void CInfoSkillHEAL::Copy(CInfoSkillBase *pSrc)
{
	PCInfoSkillHEAL pSrcTmp;

	if (pSrc == NULL) {
		return;
	}
	pSrcTmp = (PCInfoSkillHEAL)pSrc;

	CInfoSkillBase::Copy (pSrc);
	if (pSrc->m_nClassType != m_nClassType) {
		return;
	}

	m_dwArea		= pSrcTmp->m_dwArea;			/* 範囲 */
	m_dwHealType	= pSrcTmp->m_dwHealType;		/* 回復種別 */
	m_dwHitEffectID	= pSrcTmp->m_dwHitEffectID;		/* ヒット時の表示エフェクト */
	m_dwValue1		= pSrcTmp->m_dwValue1;			/* 効果1 */
	m_dwValue2		= pSrcTmp->m_dwValue2;			/* 効果2 */
	m_dwDistance	= pSrcTmp->m_dwDistance;		/* 射程距離 */

}

/* Copyright(C)URARA-works 2009 */
