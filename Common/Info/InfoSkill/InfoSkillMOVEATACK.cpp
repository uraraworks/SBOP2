/* Copyright(C)URARA-works 2009 */
/* ========================================================================= */
/* ファイル名	:InfoSkillMOVEATACK.cpp										 */
/* 内容			:スキル情報(移動して攻撃)クラス 実装ファイル				 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2009/01/08													 */
/* ========================================================================= */

#include "stdafx.h"
#include "InfoSkillMOVEATACK.h"

/* ========================================================================= */
/* 定数定義																	 */
/* ========================================================================= */

/* ヘッダ情報 */
static LPCSTR s_aszName[] = {
	"m_dwTartgetType",		/* 攻撃対象 */
	"m_dwHitEffectID",		/* ヒット時の表示エフェクト */
	"m_dwEffectID",			/* 向きによる表示エフェクト */
	"m_dwPutType",			/* 発射種別 */
	"m_dwAliveTime",		/* 耐久時間 */
	"m_dwWaitTime",			/* 移動速度 */
	"m_dwValue1",			/* 効果1 */
	"m_dwValue2",			/* 効果2 */
	"m_dwDistance",			/* 射程距離 */
	"m_bHitQuit",			/* ヒットすると消滅 */
	"m_bDistanceDelete",	/* 射程距離まで行くと消える */
	NULL
};


/* ========================================================================= */
/* 関数名	:CInfoSkillMOVEATACK::CInfoSkillMOVEATACK						 */
/* 内容		:コンストラクタ													 */
/* 日付		:2009/01/08														 */
/* ========================================================================= */

CInfoSkillMOVEATACK::CInfoSkillMOVEATACK()
{
	m_nClassType		= INFOSKILLTYPE_MOVEATACK;
	m_dwHitEffectID		= 0;
	m_dwTartgetType		= 0;
	m_dwPutType			= 0;
	m_dwAliveTime		= 0;
	m_dwWaitTime		= 0;
	m_dwValue1			= 0;
	m_dwValue2			= 0;
	m_dwDistance		= 0;
	m_bHitQuit			= FALSE;
	m_bDistanceDelete	= FALSE;

	ZeroMemory (m_adwEffectID, sizeof (m_adwEffectID));

	for (m_nElementCountDerivation = 0; s_aszName[m_nElementCountDerivation] != NULL; m_nElementCountDerivation ++) {}
}


/* ========================================================================= */
/* 関数名	:CInfoSkillMOVEATACK::~CInfoSkillMOVEATACK						 */
/* 内容		:デストラクタ													 */
/* 日付		:2009/01/08														 */
/* ========================================================================= */

CInfoSkillMOVEATACK::~CInfoSkillMOVEATACK()
{
}


/* ========================================================================= */
/* 関数名	:CInfoSkillMOVEATACK::GetElementNoTmp							 */
/* 内容		:要素番号を取得													 */
/* 日付		:2009/01/08														 */
/* ========================================================================= */

int CInfoSkillMOVEATACK::GetElementNoTmp(LPCSTR pszName)
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
/* 関数名	:CInfoSkillMOVEATACK::GetDataSizeTmp							 */
/* 内容		:データサイズを取得												 */
/* 日付		:2009/01/08														 */
/* ========================================================================= */

DWORD CInfoSkillMOVEATACK::GetDataSizeTmp(void)
{
	DWORD dwRet;

	dwRet = 0;
	dwRet += sizeof (m_dwTartgetType);		/* 攻撃対象 */
	dwRet += sizeof (m_dwHitEffectID);		/* ヒット時の表示エフェクト */
	dwRet += sizeof (m_adwEffectID);		/* 向きによる表示エフェクト */
	dwRet += sizeof (m_dwPutType);			/* 発射種別 */
	dwRet += sizeof (m_dwAliveTime);		/* 耐久時間 */
	dwRet += sizeof (m_dwWaitTime);			/* 移動速度 */
	dwRet += sizeof (m_dwValue1);			/* 効果1 */
	dwRet += sizeof (m_dwValue2);			/* 効果2 */
	dwRet += sizeof (m_dwDistance);			/* 射程距離 */
	dwRet += sizeof (m_bHitQuit);			/* ヒットすると消滅 */
	dwRet += sizeof (m_bDistanceDelete);	/* 射程距離まで行くと消える */

	return dwRet;
}


/* ========================================================================= */
/* 関数名	:CInfoSkillMOVEATACK::GetDataSizeNoTmp							 */
/* 内容		:指定要素のデータサイズを取得									 */
/* 日付		:2009/01/08														 */
/* ========================================================================= */

DWORD CInfoSkillMOVEATACK::GetDataSizeNoTmp(int nNo)
{
	DWORD dwRet;

	dwRet = 0;
	switch (nNo) {
	case 0:		dwRet = sizeof (m_dwTartgetType);	break;		/* 攻撃対象 */
	case 1:		dwRet = sizeof (m_dwHitEffectID);	break;		/* ヒット時の表示エフェクト */
	case 2:		dwRet = sizeof (m_adwEffectID);		break;		/* 向きによる表示エフェクト */
	case 3:		dwRet = sizeof (m_dwPutType);		break;		/* 発射種別 */
	case 4:		dwRet = sizeof (m_dwAliveTime);		break;		/* 耐久時間 */
	case 5:		dwRet = sizeof (m_dwWaitTime);		break;		/* 移動速度 */
	case 6:		dwRet = sizeof (m_dwValue1);		break;		/* 効果1 */
	case 7:		dwRet = sizeof (m_dwValue2);		break;		/* 効果2 */
	case 8:		dwRet = sizeof (m_dwDistance);		break;		/* 射程距離 */
	case 9:		dwRet = sizeof (m_bHitQuit);		break;		/* ヒットすると消滅 */
	case 10:	dwRet = sizeof (m_bDistanceDelete);	break;		/* 射程距離まで行くと消える */
	}

	return dwRet;
}


/* ========================================================================= */
/* 関数名	:CInfoSkillMOVEATACK::GetNameTmp								 */
/* 内容		:要素名を取得													 */
/* 日付		:2009/01/08														 */
/* ========================================================================= */

LPCSTR CInfoSkillMOVEATACK::GetNameTmp(int nNo)
{
	return s_aszName[nNo];
}


/* ========================================================================= */
/* 関数名	:CInfoSkillMOVEATACK::GetWriteDataTmp							 */
/* 内容		:指定要素の保存用データを取得									 */
/* 日付		:2009/01/08														 */
/* ========================================================================= */

PBYTE CInfoSkillMOVEATACK::GetWriteDataTmp(int nNo, PDWORD pdwSize)
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
	case 0:		pSrc = (PBYTE)&m_dwTartgetType;		break;		/* 攻撃対象 */
	case 1:		pSrc = (PBYTE)&m_dwHitEffectID;		break;		/* ヒット時の表示エフェクト */
	case 2:		pSrc = (PBYTE)&m_adwEffectID;		break;		/* 向きによる表示エフェクト */
	case 3:		pSrc = (PBYTE)&m_dwPutType;			break;		/* 発射種別 */
	case 4:		pSrc = (PBYTE)&m_dwAliveTime;		break;		/* 耐久時間 */
	case 5:		pSrc = (PBYTE)&m_dwWaitTime;		break;		/* 移動速度 */
	case 6:		pSrc = (PBYTE)&m_dwValue1;			break;		/* 効果1 */
	case 7:		pSrc = (PBYTE)&m_dwValue2;			break;		/* 効果2 */
	case 8:		pSrc = (PBYTE)&m_dwDistance;		break;		/* 射程距離 */
	case 9:		pSrc = (PBYTE)&m_bHitQuit;			break;		/* ヒットすると消滅 */
	case 10:	pSrc = (PBYTE)&m_bDistanceDelete;	break;		/* 射程距離まで行くと消える */
	}

	if (pSrc) {
		CopyMemory (pRet, pSrc, dwSize);
	}

Exit:
	return pRet;
}


/* ========================================================================= */
/* 関数名	:CInfoSkillMOVEATACK::ReadElementDataTmp						 */
/* 内容		:指定要素データを読み込み										 */
/* 日付		:2009/01/08														 */
/* ========================================================================= */

DWORD CInfoSkillMOVEATACK::ReadElementDataTmp(
	PBYTE pSrc,		/* [in] データの読み込み元 */
	int nNo)		/* [in] 要素番号 */
{
	PBYTE pDst;
	DWORD dwSize;

	pDst	= NULL;
	dwSize	= 0;

	switch (nNo) {
	case 0:		pDst = (PBYTE)&m_dwTartgetType;		dwSize = sizeof (m_dwTartgetType);		break;		/* 攻撃対象 */
	case 1:		pDst = (PBYTE)&m_dwHitEffectID;		dwSize = sizeof (m_dwHitEffectID);		break;		/* ヒット時の表示エフェクト */
	case 2:		pDst = (PBYTE)&m_adwEffectID;		dwSize = sizeof (m_adwEffectID);		break;		/* 向きによる表示エフェクト */
	case 3:		pDst = (PBYTE)&m_dwPutType;			dwSize = sizeof (m_dwPutType);			break;		/* 発射種別 */
	case 4:		pDst = (PBYTE)&m_dwAliveTime;		dwSize = sizeof (m_dwAliveTime);		break;		/* 耐久時間 */
	case 5:		pDst = (PBYTE)&m_dwWaitTime;		dwSize = sizeof (m_dwWaitTime);			break;		/* 移動速度 */
	case 6:		pDst = (PBYTE)&m_dwValue1;			dwSize = sizeof (m_dwValue1);			break;		/* 効果1 */
	case 7:		pDst = (PBYTE)&m_dwValue2;			dwSize = sizeof (m_dwValue2);			break;		/* 効果2 */
	case 8:		pDst = (PBYTE)&m_dwDistance;		dwSize = sizeof (m_dwDistance);			break;		/* 射程距離 */
	case 9:		pDst = (PBYTE)&m_bHitQuit;			dwSize = sizeof (m_bHitQuit);			break;		/* ヒットすると消滅 */
	case 10:	pDst = (PBYTE)&m_bDistanceDelete;	dwSize = sizeof (m_bDistanceDelete);	break;		/* 射程距離まで行くと消える */
	}

	if (pDst) {
		CopyMemory (pDst, pSrc, dwSize);
	}

	return dwSize;
}


/* ========================================================================= */
/* 関数名	:CInfoSkillMOVEATACK::GetDerivationSize							 */
/* 内容		:派生データサイズを取得											 */
/* 日付		:2009/01/10														 */
/* ========================================================================= */

DWORD CInfoSkillMOVEATACK::GetDerivationSize(void)
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
/* 関数名	:CInfoSkillMOVEATACK::GetDerivationWriteData					 */
/* 内容		:派生データの保存用データを取得									 */
/* 日付		:2009/01/10														 */
/* ========================================================================= */

PBYTE CInfoSkillMOVEATACK::GetDerivationWriteData(PDWORD pdwSize)
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
/* 関数名	:CInfoSkillMOVEATACK::ReadDerivationData						 */
/* 内容		:派生データを読み込み											 */
/* 日付		:2009/01/10														 */
/* ========================================================================= */

DWORD CInfoSkillMOVEATACK::ReadDerivationData(PBYTE pSrc)
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
/* 関数名	:CInfoSkillMOVEATACK::GetSendDataSize							 */
/* 内容		:送信データサイズを取得											 */
/* 日付		:2009/01/08														 */
/* ========================================================================= */

DWORD CInfoSkillMOVEATACK::GetSendDataSize(void)
{
	DWORD dwRet;

	dwRet = CInfoSkillBase::GetSendDataSize ();
	dwRet += sizeof (m_dwTartgetType);		/* 攻撃対象 */
	dwRet += sizeof (m_dwHitEffectID);		/* ヒット時の表示エフェクト */
	dwRet += sizeof (m_adwEffectID);		/* 向きによる表示エフェクト */
	dwRet += sizeof (m_dwPutType);			/* 発射種別 */
	dwRet += sizeof (m_dwAliveTime);		/* 耐久時間 */
	dwRet += sizeof (m_dwWaitTime);			/* 移動速度 */
	dwRet += sizeof (m_dwValue1);			/* 効果1 */
	dwRet += sizeof (m_dwValue2);			/* 効果2 */
	dwRet += sizeof (m_dwDistance);			/* 射程距離 */
	dwRet += sizeof (m_bHitQuit);			/* ヒットすると消滅 */
	dwRet += sizeof (m_bDistanceDelete);	/* 射程距離まで行くと消える */

	return dwRet;
}


/* ========================================================================= */
/* 関数名	:CInfoSkillMOVEATACK::GetSendData								 */
/* 内容		:送信データを取得												 */
/* 日付		:2009/01/08														 */
/* ========================================================================= */

PBYTE CInfoSkillMOVEATACK::GetSendData(void)
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

	CopyMemoryRenew (pDataTmp, &m_dwTartgetType,	sizeof (m_dwTartgetType),	pDataTmp);	/* 攻撃対象 */
	CopyMemoryRenew (pDataTmp, &m_dwHitEffectID,	sizeof (m_dwHitEffectID),	pDataTmp);	/* ヒット時の表示エフェクト */
	CopyMemoryRenew (pDataTmp, &m_adwEffectID,		sizeof (m_adwEffectID),		pDataTmp);	/* 向きによる表示エフェクト */
	CopyMemoryRenew (pDataTmp, &m_dwPutType,		sizeof (m_dwPutType),		pDataTmp);	/* 発射種別 */
	CopyMemoryRenew (pDataTmp, &m_dwAliveTime,		sizeof (m_dwAliveTime),		pDataTmp);	/* 耐久時間 */
	CopyMemoryRenew (pDataTmp, &m_dwWaitTime,		sizeof (m_dwWaitTime),		pDataTmp);	/* 移動速度 */
	CopyMemoryRenew (pDataTmp, &m_dwValue1,			sizeof (m_dwValue1),		pDataTmp);	/* 効果1 */
	CopyMemoryRenew (pDataTmp, &m_dwValue2,			sizeof (m_dwValue2),		pDataTmp);	/* 効果2 */
	CopyMemoryRenew (pDataTmp, &m_dwDistance,		sizeof (m_dwDistance),		pDataTmp);	/* 射程距離 */
	CopyMemoryRenew (pDataTmp, &m_bHitQuit,			sizeof (m_bHitQuit),		pDataTmp);	/* ヒットすると消滅 */
	CopyMemoryRenew (pDataTmp, &m_bDistanceDelete,	sizeof (m_bDistanceDelete),	pDataTmp);	/* 射程距離まで行くと消える */

	return pData;
}


/* ========================================================================= */
/* 関数名	:CInfoSkillMOVEATACK::SetSendData								 */
/* 内容		:送信データから取り込み											 */
/* 日付		:2009/01/08														 */
/* ========================================================================= */

PBYTE CInfoSkillMOVEATACK::SetSendData(PBYTE pSrc)
{
	PBYTE pRet, pDataTmp;

	pRet = pSrc;

	pDataTmp = CInfoSkillBase::SetSendData (pSrc);
	CopyMemoryRenew (&m_dwTartgetType,		pDataTmp, sizeof (m_dwTartgetType),		pDataTmp);	/* 攻撃対象 */
	CopyMemoryRenew (&m_dwHitEffectID,		pDataTmp, sizeof (m_dwHitEffectID),		pDataTmp);	/* ヒット時の表示エフェクト */
	CopyMemoryRenew (&m_adwEffectID,		pDataTmp, sizeof (m_adwEffectID),		pDataTmp);	/* 向きによる表示エフェクト */
	CopyMemoryRenew (&m_dwPutType,			pDataTmp, sizeof (m_dwPutType),			pDataTmp);	/* 発射種別 */
	CopyMemoryRenew (&m_dwAliveTime,		pDataTmp, sizeof (m_dwAliveTime),		pDataTmp);	/* 耐久時間 */
	CopyMemoryRenew (&m_dwWaitTime,			pDataTmp, sizeof (m_dwWaitTime),		pDataTmp);	/* 移動速度 */
	CopyMemoryRenew (&m_dwValue1,			pDataTmp, sizeof (m_dwValue1),			pDataTmp);	/* 効果1 */
	CopyMemoryRenew (&m_dwValue2,			pDataTmp, sizeof (m_dwValue2),			pDataTmp);	/* 効果2 */
	CopyMemoryRenew (&m_dwDistance,			pDataTmp, sizeof (m_dwDistance),		pDataTmp);	/* 射程距離 */
	CopyMemoryRenew (&m_bHitQuit,			pDataTmp, sizeof (m_bHitQuit),			pDataTmp);	/* ヒットすると消滅 */
	CopyMemoryRenew (&m_bDistanceDelete,	pDataTmp, sizeof (m_bDistanceDelete),	pDataTmp);	/* 射程距離まで行くと消える */

	pRet = pDataTmp;
	return pRet;
}


/* ========================================================================= */
/* 関数名	:CInfoSkillMOVEATACK::Copy										 */
/* 内容		:コピー															 */
/* 日付		:2009/01/08														 */
/* ========================================================================= */

void CInfoSkillMOVEATACK::Copy(CInfoSkillBase *pSrc)
{
	PCInfoSkillMOVEATACK pSrcTmp;

	if (pSrc == NULL) {
		return;
	}
	pSrcTmp = (PCInfoSkillMOVEATACK)pSrc;

	CInfoSkillBase::Copy (pSrc);
	if (pSrc->m_nClassType != m_nClassType) {
		return;
	}

	m_dwHitEffectID		= pSrcTmp->m_dwHitEffectID;		/* ヒット時の表示エフェクト */
	m_dwTartgetType		= pSrcTmp->m_dwTartgetType;		/* 攻撃対象 */
	m_dwPutType			= pSrcTmp->m_dwPutType;			/* 発射種別 */
	m_dwAliveTime		= pSrcTmp->m_dwAliveTime;		/* 耐久時間 */
	m_dwWaitTime		= pSrcTmp->m_dwWaitTime;		/* 移動速度 */
	m_dwValue1			= pSrcTmp->m_dwValue1;			/* 効果1 */
	m_dwValue2			= pSrcTmp->m_dwValue2;			/* 効果2 */
	m_dwDistance		= pSrcTmp->m_dwDistance;		/* 射程距離 */
	m_bHitQuit			= pSrcTmp->m_bHitQuit;			/* ヒットすると消滅 */
	m_bDistanceDelete	= pSrcTmp->m_bDistanceDelete;	/* 射程距離まで行くと消える */

	/* 向きによる表示エフェクト */
	CopyMemory (m_adwEffectID, pSrcTmp->m_adwEffectID, sizeof (m_adwEffectID));
}

/* Copyright(C)URARA-works 2009 */
