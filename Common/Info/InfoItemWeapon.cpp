/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:InfoItemWeapon.cpp											 */
/* 内容			:アイテム武器情報クラス 実装ファイル						 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/08/10													 */
/* ========================================================================= */

#include "stdafx.h"
#include "InfoItemWeapon.h"

/* ========================================================================= */
/* 定数定義																	 */
/* ========================================================================= */

/* ヘッダ情報 */
static LPCSTR s_aszName[] = {
	"m_dwWeaponInfoID",			/* 武器情報ID */
	"m_dwMotionType",			/* 使用可能な攻撃モーション */
	"m_dwMotionTypeStand",		/* 戦闘モード中の立ちモーション */
	"m_dwMotionTypeWalk",		/* 戦闘モード中のすり足モーション */
	"m_strName",				/* 武器種別名 */
	"m_adwEffectIDAtack",		/* 通常攻撃時のエフェクトID */
	"m_adwEffectIDCritical",	/* クリティカル時のエフェクトID */
	NULL
};


/* ========================================================================= */
/* 関数名	:CInfoItemWeapon::CInfoItemWeapon								 */
/* 内容		:コンストラクタ													 */
/* 日付		:2008/08/10														 */
/* ========================================================================= */

CInfoItemWeapon::CInfoItemWeapon()
{
	m_dwWeaponInfoID	= 0;
	m_dwMotionType		= 0;
	m_dwMotionTypeStand	= 0;
	m_dwMotionTypeWalk	= 0;

	for (m_nElementCount = 0; s_aszName[m_nElementCount] != NULL; m_nElementCount ++) {}
}


/* ========================================================================= */
/* 関数名	:CInfoItemWeapon::~CInfoItemWeapon								 */
/* 内容		:デストラクタ													 */
/* 日付		:2008/08/10														 */
/* ========================================================================= */

CInfoItemWeapon::~CInfoItemWeapon()
{
}


/* ========================================================================= */
/* 関数名	:CInfoItemWeapon::GetElementNo									 */
/* 内容		:要素番号を取得													 */
/* 日付		:2008/08/10														 */
/* ========================================================================= */

int CInfoItemWeapon::GetElementNo(LPCSTR pszName)
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
/* 関数名	:CInfoItemWeapon::GetDataSize									 */
/* 内容		:データサイズを取得												 */
/* 日付		:2008/08/10														 */
/* ========================================================================= */

DWORD CInfoItemWeapon::GetDataSize(void)
{
	DWORD dwRet;

	dwRet = 0;
	dwRet += sizeof (m_dwWeaponInfoID);									/* 武器情報ID */
	dwRet += sizeof (m_dwMotionType);									/* 使用可能な攻撃モーション */
	dwRet += sizeof (m_dwMotionTypeStand);								/* 戦闘モード中の立ちモーション */
	dwRet += sizeof (m_dwMotionTypeWalk);								/* 戦闘モード中のすり足モーション */
	dwRet += (m_strName.GetLength () + 1);								/* 武器種別名 */
	dwRet += ((m_adwEffectIDAtack.GetSize () + 1) * sizeof (DWORD));	/* 通常攻撃時のエフェクトID */
	dwRet += ((m_adwEffectIDCritical.GetSize () + 1) * sizeof (DWORD));	/* クリティカル時のエフェクトID */

	return dwRet;
}


/* ========================================================================= */
/* 関数名	:CInfoItemWeapon::GetDataSizeNo									 */
/* 内容		:指定要素のデータサイズを取得									 */
/* 日付		:2008/08/10														 */
/* ========================================================================= */

DWORD CInfoItemWeapon::GetDataSizeNo(int nNo)
{
	DWORD dwRet;

	dwRet = 0;

	switch (nNo) {
	case 0:		dwRet = sizeof (m_dwWeaponInfoID);									break;	/* 武器情報ID */
	case 1:		dwRet = sizeof (m_dwMotionType);									break;	/* 使用可能な攻撃モーション */
	case 2:		dwRet = sizeof (m_dwMotionTypeStand);								break;	/* 戦闘モード中の立ちモーション */
	case 3:		dwRet = sizeof (m_dwMotionTypeWalk);								break;	/* 戦闘モード中のすり足モーション */
	case 4:		dwRet = (m_strName.GetLength () + 1);								break;	/* 武器種別名 */
	case 5:		dwRet = ((m_adwEffectIDAtack.GetSize () + 1) * sizeof (DWORD));		break;	/* 通常攻撃時のエフェクトID数 */
	case 6:		dwRet = ((m_adwEffectIDCritical.GetSize () + 1) * sizeof (DWORD));	break;	/* クリティカル時のエフェクトID数 */

	}

	return dwRet;
}


/* ========================================================================= */
/* 関数名	:CInfoItemWeapon::GetName										 */
/* 内容		:要素名を取得													 */
/* 日付		:2008/08/10														 */
/* ========================================================================= */

LPCSTR CInfoItemWeapon::GetName(int nNo)
{
	return s_aszName[nNo];
}


/* ========================================================================= */
/* 関数名	:CInfoItemWeapon::GetWriteData									 */
/* 内容		:指定要素の保存用データを取得									 */
/* 日付		:2008/08/10														 */
/* ========================================================================= */

PBYTE CInfoItemWeapon::GetWriteData(int nNo, PDWORD pdwSize)
{
	int i, nCount;
	PBYTE pRet, pSrc, pTmp;
	DWORD dwSize, dwTmp;

	pRet		= NULL;
	pSrc		= NULL;
	dwSize		= GetDataSizeNo (nNo);
	*pdwSize	= dwSize;

	if (dwSize == 0) {
		goto Exit;
	}
	pRet = new BYTE[dwSize];

	switch (nNo) {
	case 0:		pSrc = (PBYTE)&m_dwWeaponInfoID;			break;	/* 武器情報ID */
	case 1:		pSrc = (PBYTE)&m_dwMotionType;				break;	/* 使用可能な攻撃モーション */
	case 2:		pSrc = (PBYTE)&m_dwMotionTypeStand;			break;	/* 戦闘モード中の立ちモーション */
	case 3:		pSrc = (PBYTE)&m_dwMotionTypeWalk;			break;	/* 戦闘モード中のすり足モーション */
	case 4:		pSrc = (PBYTE)(LPCSTR)m_strName;			break;	/* 武器種別名 */
	case 5:		/* 通常攻撃時のエフェクトID */
		pTmp	= pRet;
		nCount	= m_adwEffectIDAtack.GetSize ();

		for (i = 0; i < nCount; i ++) {
			dwTmp = m_adwEffectIDAtack[i];
			CopyMemoryRenew (pTmp, &dwTmp, sizeof (DWORD), pTmp);
		}
		/* 終端用 */
		dwTmp = 0;
		CopyMemoryRenew (pTmp, &dwTmp, sizeof (DWORD), pTmp);
		break;
	case 6:		/* クリティカル時のエフェクトID */
		pTmp	= pRet;
		nCount	= m_adwEffectIDCritical.GetSize ();

		for (i = 0; i < nCount; i ++) {
			dwTmp = m_adwEffectIDCritical[i];
			CopyMemoryRenew (pTmp, &dwTmp, sizeof (DWORD), pTmp);
		}
		/* 終端用 */
		dwTmp = 0;
		CopyMemoryRenew (pTmp, &dwTmp, sizeof (DWORD), pTmp);
		break;
	}

	if (pSrc) {
		CopyMemory (pRet, pSrc, dwSize);
	}

Exit:
	return pRet;
}


/* ========================================================================= */
/* 関数名	:CInfoItemWeapon::ReadElementData								 */
/* 内容		:指定要素データを読み込み										 */
/* 日付		:2008/08/10														 */
/* ========================================================================= */

DWORD CInfoItemWeapon::ReadElementData(
	PBYTE pSrc,		/* [in] データの読み込み元 */
	int nNo)		/* [in] 要素番号 */
{
	PBYTE pDst, pTmp;
	DWORD dwSize, dwTmp;

	pDst	= NULL;
	dwSize	= GetDataSizeNo (nNo);

	switch (nNo) {
	case 0:		pDst = (PBYTE)&m_dwWeaponInfoID;			break;	/* 武器情報ID */
	case 1:		pDst = (PBYTE)&m_dwMotionType;				break;	/* 使用可能な攻撃モーション */
	case 2:		pDst = (PBYTE)&m_dwMotionTypeStand;			break;	/* 戦闘モード中の立ちモーション */
	case 3:		pDst = (PBYTE)&m_dwMotionTypeWalk;			break;	/* 戦闘モード中のすり足モーション */
	case 4:		/* 武器種別名 */
		m_strName = (LPCSTR)pSrc;
		dwSize = m_strName.GetLength () + 1;
		break;
	case 5:		/* 通常攻撃時のエフェクトID */
		pTmp	= pSrc;
		dwTmp	= 0;

		while (1) {
			CopyMemoryRenew (&dwTmp, pTmp, sizeof (DWORD), pTmp);
			if (dwTmp == 0) {
				break;
			}
			m_adwEffectIDAtack.Add (dwTmp);
		}
		dwSize = (m_adwEffectIDAtack.GetSize () + 1) * sizeof (DWORD);
		break;
	case 6:		/* クリティカル時のエフェクトID */
		pTmp	= pSrc;
		dwTmp	= 0;

		while (1) {
			CopyMemoryRenew (&dwTmp, pTmp, sizeof (DWORD), pTmp);
			if (dwTmp == 0) {
				break;
			}
			m_adwEffectIDCritical.Add (dwTmp);
		}
		dwSize = (m_adwEffectIDCritical.GetSize () + 1) * sizeof (DWORD);
		break;
	}

	if (pDst) {
		CopyMemory (pDst, pSrc, dwSize);
	}

	return dwSize;
}


/* ========================================================================= */
/* 関数名	:CInfoItemWeapon::GetSendDataSize								 */
/* 内容		:送信データサイズを取得											 */
/* 日付		:2008/08/10														 */
/* ========================================================================= */

DWORD CInfoItemWeapon::GetSendDataSize(void)
{
	return GetDataSize ();
}


/* ========================================================================= */
/* 関数名	:CInfoItemWeapon::GetSendData									 */
/* 内容		:送信データを取得												 */
/* 日付		:2008/08/10														 */
/* ========================================================================= */

PBYTE CInfoItemWeapon::GetSendData(void)
{
	int i, nCount;
	PBYTE pData, pDataTmp;
	DWORD dwSize, dwTmp;

	dwSize	= GetSendDataSize ();
	pData	= ZeroNew (dwSize);

	pDataTmp = pData;

	CopyMemoryRenew (pDataTmp, &m_dwWeaponInfoID,		sizeof (m_dwWeaponInfoID),		pDataTmp);	/* 武器情報ID */
	CopyMemoryRenew (pDataTmp, &m_dwMotionType,			sizeof (m_dwMotionType),		pDataTmp);	/* 使用可能な攻撃モーション */
	CopyMemoryRenew (pDataTmp, &m_dwMotionTypeStand,	sizeof (m_dwMotionTypeStand),	pDataTmp);	/* 戦闘モード中の立ちモーション */
	CopyMemoryRenew (pDataTmp, &m_dwMotionTypeWalk,		sizeof (m_dwMotionTypeWalk),	pDataTmp);	/* 戦闘モード中のすり足モーション */
	strcpyRenew ((LPSTR)pDataTmp, (LPCSTR)m_strName, pDataTmp);										/* 武器種別名 */

	/* 通常攻撃時のエフェクトID */
	nCount = m_adwEffectIDAtack.GetSize ();
	for (i = 0; i < nCount; i ++) {
		dwTmp = m_adwEffectIDAtack[i];
		CopyMemoryRenew (pDataTmp, &dwTmp, sizeof (dwTmp), pDataTmp);
	}
	/* 終端用 */
	dwTmp = 0;
	CopyMemoryRenew (pDataTmp, &dwTmp, sizeof (dwTmp), pDataTmp);

	/* クリティカル時のエフェクトID */
	nCount = m_adwEffectIDCritical.GetSize ();
	for (i = 0; i < nCount; i ++) {
		dwTmp = m_adwEffectIDCritical[i];
		CopyMemoryRenew (pDataTmp, &dwTmp, sizeof (dwTmp), pDataTmp);
	}
	/* 終端用 */
	dwTmp = 0;
	CopyMemoryRenew (pDataTmp, &dwTmp, sizeof (dwTmp), pDataTmp);

	return pData;
}


/* ========================================================================= */
/* 関数名	:CInfoItemWeapon::SetSendData									 */
/* 内容		:送信データから取り込み											 */
/* 日付		:2008/08/10														 */
/* ========================================================================= */

PBYTE CInfoItemWeapon::SetSendData(PBYTE pSrc)
{
	PBYTE pRet, pDataTmp;
	DWORD dwTmp;

	pRet = pSrc;

	pDataTmp = pSrc;

	m_adwEffectIDAtack.RemoveAll ();
	m_adwEffectIDCritical.RemoveAll ();

	CopyMemoryRenew (&m_dwWeaponInfoID,		pDataTmp, sizeof (m_dwWeaponInfoID),	pDataTmp);	/* 武器情報ID */
	CopyMemoryRenew (&m_dwMotionType,		pDataTmp, sizeof (m_dwMotionType),		pDataTmp);	/* 使用可能な攻撃モーション */
	CopyMemoryRenew (&m_dwMotionTypeStand,	pDataTmp, sizeof (m_dwMotionTypeStand),	pDataTmp);	/* 戦闘モード中の立ちモーション */
	CopyMemoryRenew (&m_dwMotionTypeWalk,	pDataTmp, sizeof (m_dwMotionTypeWalk),	pDataTmp);	/* 戦闘モード中のすり足モーション */
	StoreRenew (m_strName, (LPCSTR)pDataTmp, pDataTmp);											/* 武器種別名 */

	/* 通常攻撃時のエフェクトID */
	while (1) {
		CopyMemoryRenew (&dwTmp, pDataTmp, sizeof (dwTmp), pDataTmp);
		if (dwTmp == 0) {
			break;
		}
		m_adwEffectIDAtack.Add (dwTmp);
	}

	/* クリティカル時のエフェクトID */
	while (1) {
		CopyMemoryRenew (&dwTmp, pDataTmp, sizeof (dwTmp), pDataTmp);
		if (dwTmp == 0) {
			break;
		}
		m_adwEffectIDCritical.Add (dwTmp);
	}

	pRet = pDataTmp;
	return pRet;
}


/* ========================================================================= */
/* 関数名	:CInfoItemWeapon::Copy											 */
/* 内容		:コピー															 */
/* 日付		:2008/08/10														 */
/* ========================================================================= */

void CInfoItemWeapon::Copy(CInfoItemWeapon *pSrc)
{
	int i, nCount;

	if (pSrc == NULL) {
		return;
	}
	m_dwWeaponInfoID		= pSrc->m_dwWeaponInfoID;		/* 武器情報ID */
	m_dwMotionType			= pSrc->m_dwMotionType;			/* 使用可能な攻撃モーション */
	m_dwMotionTypeStand		= pSrc->m_dwMotionTypeStand;	/* 戦闘モード中の立ちモーション */
	m_dwMotionTypeWalk		= pSrc->m_dwMotionTypeWalk;		/* 戦闘モード中のすり足モーション */
	m_strName				= pSrc->m_strName;				/* 武器種別名 */

	/* 通常攻撃時のエフェクトID */
	m_adwEffectIDAtack.RemoveAll ();
	nCount = pSrc->m_adwEffectIDAtack.GetSize ();
	for (i = 0; i < nCount; i ++) {
		m_adwEffectIDAtack.Add (pSrc->m_adwEffectIDAtack[i]);
	}

	/* クリティカル時のエフェクトID */
	m_adwEffectIDCritical.RemoveAll ();
	nCount = pSrc->m_adwEffectIDCritical.GetSize ();
	for (i = 0; i < nCount; i ++) {
		m_adwEffectIDCritical.Add (pSrc->m_adwEffectIDCritical[i]);
	}
}

/* Copyright(C)URARA-works 2008 */
