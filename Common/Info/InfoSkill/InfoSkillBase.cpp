/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:InfoSkillBase.cpp											 */
/* 内容			:スキル情報基底クラス 実装ファイル							 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/12/03													 */
/* ========================================================================= */

#include "stdafx.h"
#include "InfoSkillBase.h"

/* ========================================================================= */
/* 定数定義																	 */
/* ========================================================================= */

/* ヘッダ情報 */
static LPCSTR s_aszName[] = {
	"m_dwSkillID",		/* スキルID */
	"m_dwSP",			/* 消費SP */
	"m_dwIconID",		/* アイコン画像ID */
	"m_nTypeMain",		/* スキル種別(メイン) */
	"m_nTypeSub",		/* スキル種別(サブ) */
	"m_nUse",			/* 使用制限 */
	"m_strName",		/* スキル名 */
	"derivation",		/* 派生データ */
	NULL
};


/* ========================================================================= */
/* 関数名	:CInfoSkillBase::CInfoSkillBase									 */
/* 内容		:コンストラクタ													 */
/* 日付		:2008/12/03														 */
/* ========================================================================= */

CInfoSkillBase::CInfoSkillBase()
{
	m_nClassType	= INFOSKILLTYPE_BASE;
	m_dwSkillID		= 0;
	m_dwSP			= 0;
	m_dwIconID		= 0;
	m_nTypeMain		= 0;
	m_nTypeSub		= 0;
	m_nUse			= SKILLUSE_ANY;

	for (m_nElementCountBase = 0; s_aszName[m_nElementCountBase] != NULL; m_nElementCountBase ++) {}
	m_nElementCount = m_nElementCountBase;
}


/* ========================================================================= */
/* 関数名	:CInfoSkillBase::~CInfoSkillBase								 */
/* 内容		:デストラクタ													 */
/* 日付		:2008/12/03														 */
/* ========================================================================= */

CInfoSkillBase::~CInfoSkillBase()
{
}


/* ========================================================================= */
/* 関数名	:CInfoSkillBase::GetElementNo									 */
/* 内容		:要素番号を取得													 */
/* 日付		:2008/12/03														 */
/* ========================================================================= */

int CInfoSkillBase::GetElementNo(LPCSTR pszName)
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
/* 関数名	:CInfoSkillBase::GetDataSize									 */
/* 内容		:データサイズを取得												 */
/* 日付		:2008/12/03														 */
/* ========================================================================= */

DWORD CInfoSkillBase::GetDataSize(void)
{
	DWORD dwRet;

	dwRet = 0;
	dwRet += sizeof (m_dwSkillID);			/* スキルID */
	dwRet += sizeof (m_dwSP);				/* 消費SP */
	dwRet += sizeof (m_dwIconID);			/* アイコン画像ID */
	dwRet += sizeof (m_nTypeMain);			/* スキル種別(メイン) */
	dwRet += sizeof (m_nTypeSub);			/* スキル種別(サブ) */
	dwRet += sizeof (m_nUse);				/* 使用制限 */
	dwRet += (m_strName.GetLength () + 1);	/* スキル名 */
	dwRet += GetDerivationSize ();			/* 派生データ */

	return dwRet;
}


/* ========================================================================= */
/* 関数名	:CInfoSkillBase::GetDataSizeNo									 */
/* 内容		:指定要素のデータサイズを取得									 */
/* 日付		:2008/12/03														 */
/* ========================================================================= */

DWORD CInfoSkillBase::GetDataSizeNo(int nNo)
{
	DWORD dwRet;

	dwRet = 0;

	switch (nNo) {
	case 0:	dwRet = sizeof (m_dwSkillID);			break;	/* スキルID */
	case 1:	dwRet = sizeof (m_dwSP);				break;	/* 消費SP */
	case 2:	dwRet = sizeof (m_dwIconID);			break;	/* アイコン画像ID */
	case 3:	dwRet = sizeof (m_nTypeMain);			break;	/* スキル種別(メイン) */
	case 4:	dwRet = sizeof (m_nTypeSub);			break;	/* スキル種別(サブ) */
	case 5:	dwRet = sizeof (m_nUse);				break;	/* 使用制限 */
	case 6:	dwRet = (m_strName.GetLength () + 1);	break;	/* スキル名 */
	case 7:	dwRet = GetDerivationSize ();			break;	/* 派生データ */
	}

	return dwRet;
}


/* ========================================================================= */
/* 関数名	:CInfoSkillBase::GetName										 */
/* 内容		:要素名を取得													 */
/* 日付		:2008/12/03														 */
/* ========================================================================= */

LPCSTR CInfoSkillBase::GetName(int nNo)
{
	return s_aszName[nNo];
}


/* ========================================================================= */
/* 関数名	:CInfoSkillBase::GetWriteData									 */
/* 内容		:指定要素の保存用データを取得									 */
/* 日付		:2008/12/03														 */
/* ========================================================================= */

PBYTE CInfoSkillBase::GetWriteData(int nNo, PDWORD pdwSize)
{
	PBYTE pRet, pSrc;
	DWORD dwSize;

	pRet		= NULL;
	pSrc		= NULL;
	dwSize		= GetDataSizeNo (nNo);
	*pdwSize	= dwSize;

	if (dwSize == 0) {
		goto Exit;
	}
	pRet = new BYTE[dwSize];

	switch (nNo) {
	case 0:	pSrc = (PBYTE)&m_dwSkillID;			break;	/* スキルID */
	case 1:	pSrc = (PBYTE)&m_dwSP;				break;	/* 消費SP */
	case 2:	pSrc = (PBYTE)&m_dwIconID;			break;	/* アイコン画像ID */
	case 3:	pSrc = (PBYTE)&m_nTypeMain;			break;	/* スキル種別(メイン) */
	case 4:	pSrc = (PBYTE)&m_nTypeSub;			break;	/* スキル種別(サブ) */
	case 5:	pSrc = (PBYTE)&m_nUse;				break;	/* 使用制限 */
	case 6:	pSrc = (PBYTE)(LPCSTR)m_strName;	break;	/* スキル名 */
	case 7:		/* 派生データ */
		pSrc = GetDerivationWriteData (&dwSize);
		CopyMemory (pRet, pSrc, dwSize);
		SAFE_DELETE_ARRAY (pSrc);
		break;
	}

	if (pSrc) {
		CopyMemory (pRet, pSrc, dwSize);
	}

Exit:
	return pRet;
}


/* ========================================================================= */
/* 関数名	:CInfoSkillBase::ReadElementData								 */
/* 内容		:指定要素データを読み込み										 */
/* 日付		:2008/12/03														 */
/* ========================================================================= */

DWORD CInfoSkillBase::ReadElementData(
	PBYTE pSrc,		/* [in] データの読み込み元 */
	int nNo)		/* [in] 要素番号 */
{
	PBYTE pDst;
	DWORD dwSize;

	pDst	= NULL;
	dwSize	= 0;

	switch (nNo) {
	case 0: pDst = (PBYTE)&m_dwSkillID;	dwSize = sizeof (m_dwSkillID);	break;	/* スキルID */
	case 1: pDst = (PBYTE)&m_dwSP;		dwSize = sizeof (m_dwSP);		break;	/* 消費SP */
	case 2: pDst = (PBYTE)&m_dwIconID;	dwSize = sizeof (m_dwIconID);	break;	/* アイコン画像ID */
	case 3: pDst = (PBYTE)&m_nTypeMain;	dwSize = sizeof (m_nTypeMain);	break;	/* スキル種別(メイン) */
	case 4: pDst = (PBYTE)&m_nTypeSub;	dwSize = sizeof (m_nTypeSub);	break;	/* スキル種別(サブ) */
	case 5: pDst = (PBYTE)&m_nUse;		dwSize = sizeof (m_nUse);		break;	/* 使用制限 */
	case 6:		/* スキル名 */
		m_strName = (LPCSTR)pSrc;
		dwSize = m_strName.GetLength () + 1;
		break;
	case 7:		/* 派生データ */
		dwSize = ReadDerivationData (pSrc);
		break;
	}

	if (pDst) {
		CopyMemory (pDst, pSrc, dwSize);
	}

	return dwSize;
}


/* ========================================================================= */
/* 関数名	:CInfoSkillBase::GetDerivationSize								 */
/* 内容		:派生データサイズを取得											 */
/* 日付		:2009/01/10														 */
/* ========================================================================= */

DWORD CInfoSkillBase::GetDerivationSize(void)
{
	return 1;
}


/* ========================================================================= */
/* 関数名	:CInfoSkillBase::GetDerivationWriteData							 */
/* 内容		:派生データの保存用データを取得									 */
/* 日付		:2009/01/10														 */
/* ========================================================================= */

PBYTE CInfoSkillBase::GetDerivationWriteData(PDWORD pdwSize)
{
	PBYTE pRet;
	DWORD dwSize;

	dwSize	= GetDerivationSize ();
	pRet	= ZeroNew (dwSize);

	return pRet;
}


/* ========================================================================= */
/* 関数名	:CInfoSkillBase::ReadDerivationData								 */
/* 内容		:派生データを読み込み											 */
/* 日付		:2009/01/10														 */
/* ========================================================================= */

DWORD CInfoSkillBase::ReadDerivationData(PBYTE pSrc)
{
	return GetDerivationSize ();
}


/* ========================================================================= */
/* 関数名	:CInfoSkillBase::GetSendDataSize								 */
/* 内容		:送信データサイズを取得											 */
/* 日付		:2008/12/03														 */
/* ========================================================================= */

DWORD CInfoSkillBase::GetSendDataSize(void)
{
	DWORD dwRet;

	dwRet = 0;
	dwRet += sizeof (m_dwSkillID);			/* スキルID */
	dwRet += sizeof (m_dwSP);				/* 消費SP */
	dwRet += sizeof (m_dwIconID);			/* アイコン画像ID */
	dwRet += sizeof (m_nTypeMain);			/* スキル種別(メイン) */
	dwRet += sizeof (m_nTypeSub);			/* スキル種別(サブ) */
	dwRet += sizeof (m_nUse);				/* 使用制限 */
	dwRet += (m_strName.GetLength () + 1);	/* スキル名 */

	return dwRet;
}


/* ========================================================================= */
/* 関数名	:CInfoSkillBase::GetSendData									 */
/* 内容		:送信データを取得												 */
/* 日付		:2008/12/03														 */
/* ========================================================================= */

PBYTE CInfoSkillBase::GetSendData(void)
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;

	dwSize	= GetSendDataSize ();
	pData	= ZeroNew (dwSize);

	pDataTmp = pData;

	CopyMemoryRenew (pDataTmp, &m_dwSkillID,	sizeof (m_dwSkillID), 	pDataTmp);	/* スキルID */
	CopyMemoryRenew (pDataTmp, &m_dwSP,			sizeof (m_dwSP), 		pDataTmp);	/* 消費SP */
	CopyMemoryRenew (pDataTmp, &m_dwIconID,		sizeof (m_dwIconID), 	pDataTmp);	/* アイコン画像ID */
	CopyMemoryRenew (pDataTmp, &m_nTypeMain,	sizeof (m_nTypeMain),	pDataTmp);	/* スキル種別(メイン) */
	CopyMemoryRenew (pDataTmp, &m_nTypeSub,		sizeof (m_nTypeSub),	pDataTmp);	/* スキル種別(サブ) */
	CopyMemoryRenew (pDataTmp, &m_nUse,			sizeof (m_nUse),	 	pDataTmp);	/* 使用制限 */
	strcpyRenew ((LPSTR)pDataTmp, m_strName, pDataTmp);								/* スキル名 */

	return pData;
}


/* ========================================================================= */
/* 関数名	:CInfoSkillBase::SetSendData									 */
/* 内容		:送信データから取り込み											 */
/* 日付		:2008/12/03														 */
/* ========================================================================= */

PBYTE CInfoSkillBase::SetSendData(PBYTE pSrc)
{
	PBYTE pRet, pDataTmp;

	pRet = pSrc;

	pDataTmp = pSrc;
	CopyMemoryRenew (&m_dwSkillID,	pDataTmp, sizeof (m_dwSkillID), pDataTmp);	/* スキルID */
	CopyMemoryRenew (&m_dwSP,		pDataTmp, sizeof (m_dwSP), 		pDataTmp);	/* 消費SP */
	CopyMemoryRenew (&m_dwIconID,	pDataTmp, sizeof (m_dwIconID), 	pDataTmp);	/* アイコン画像ID */
	CopyMemoryRenew (&m_nTypeMain,	pDataTmp, sizeof (m_nTypeMain),	pDataTmp);	/* スキル種別(メイン) */
	CopyMemoryRenew (&m_nTypeSub,	pDataTmp, sizeof (m_nTypeSub),	pDataTmp);	/* スキル種別(サブ) */
	CopyMemoryRenew (&m_nUse,		pDataTmp, sizeof (m_nUse),	 	pDataTmp);	/* 使用制限 */
	StoreRenew (m_strName, (LPCSTR)pDataTmp, pDataTmp);							/* スキル名 */

	pRet = pDataTmp;
	return pRet;
}


/* ========================================================================= */
/* 関数名	:CInfoSkillBase::Copy											 */
/* 内容		:コピー															 */
/* 日付		:2008/12/03														 */
/* ========================================================================= */

void CInfoSkillBase::Copy(CInfoSkillBase *pSrc)
{
	if (pSrc == NULL) {
		return;
	}
	m_dwSkillID	= pSrc->m_dwSkillID;	/* スキルID */
	m_dwSP		= pSrc->m_dwSP;			/* 消費SP */
	m_dwIconID	= pSrc->m_dwIconID;		/* アイコン画像ID */
	m_nTypeMain	= pSrc->m_nTypeMain;	/* スキル種別(メイン) */
	m_nTypeSub	= pSrc->m_nTypeSub;		/* スキル種別(サブ) */
	m_nUse		= pSrc->m_nUse;			/* 使用制限 */
	m_strName	= pSrc->m_strName;		/* スキル名 */
}

/* Copyright(C)URARA-works 2008 */
