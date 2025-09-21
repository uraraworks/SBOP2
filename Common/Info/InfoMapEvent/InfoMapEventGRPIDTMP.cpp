/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:InfoMapEventGRPIDTMP.cpp									 */
/* 内容			:イベント情報(一時画像設定)クラス 実装ファイル				 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/10/12													 */
/* ========================================================================= */

#include "stdafx.h"
#include "InfoMapEventGRPIDTMP.h"

/* ========================================================================= */
/* 定数定義																	 */
/* ========================================================================= */

/* ヘッダ情報 */
static LPCSTR s_aszName[] = {
	"m_nSetType",		/* 設定種別 */
	"m_dwIDMain",		/* 画像IDメイン */
	"m_dwIDSub",		/* 画像IDサブ */
	NULL
};


/* ========================================================================= */
/* 関数名	:CInfoMapEventGRPIDTMP::CInfoMapEventGRPIDTMP					 */
/* 内容		:コンストラクタ													 */
/* 日付		:2008/10/12														 */
/* ========================================================================= */

CInfoMapEventGRPIDTMP::CInfoMapEventGRPIDTMP()
{
	m_nSetType = MAPEVENTTYPE_GRPIDTMP;

	m_nSetType	= GRPIDTMPTYPE_OFF;
	m_dwIDMain	= 0;
	m_dwIDSub	= 0;

	for (m_nElementCount = 0; s_aszName[m_nElementCount] != NULL; m_nElementCount ++) {}
	m_nElementCount += m_nElementCountBase;
}


/* ========================================================================= */
/* 関数名	:CInfoMapEventGRPIDTMP::~CInfoMapEventGRPIDTMP					 */
/* 内容		:デストラクタ													 */
/* 日付		:2008/10/12														 */
/* ========================================================================= */

CInfoMapEventGRPIDTMP::~CInfoMapEventGRPIDTMP()
{
}


/* ========================================================================= */
/* 関数名	:CInfoMapEventGRPIDTMP::GetElementNo							 */
/* 内容		:要素番号を取得													 */
/* 日付		:2008/10/12														 */
/* ========================================================================= */

int CInfoMapEventGRPIDTMP::GetElementNo(LPCSTR pszName)
{
	int i, nRet;

	nRet = CInfoMapEventBase::GetElementNo (pszName);
	if (nRet >= 0) {
		goto Exit;
	}

	for (i = 0; s_aszName[i] != NULL; i ++) {
		if (strcmp (s_aszName[i], pszName) == 0) {
			nRet = i;
			break;
		}
	}
	if (nRet >= 0) {
		nRet += m_nElementCountBase;
	}

Exit:
	return nRet;
}


/* ========================================================================= */
/* 関数名	:CInfoMapEventGRPIDTMP::GetDataSize								 */
/* 内容		:データサイズを取得												 */
/* 日付		:2008/10/12														 */
/* ========================================================================= */

DWORD CInfoMapEventGRPIDTMP::GetDataSize(void)
{
	DWORD dwRet;

	dwRet = CInfoMapEventBase::GetDataSize ();
	dwRet += sizeof (m_nSetType);
	dwRet += sizeof (m_dwIDMain);
	dwRet += sizeof (m_dwIDSub);

	return dwRet;
}


/* ========================================================================= */
/* 関数名	:CInfoMapEventGRPIDTMP::GetDataSizeNo							 */
/* 内容		:指定要素のデータサイズを取得									 */
/* 日付		:2008/10/12														 */
/* ========================================================================= */

DWORD CInfoMapEventGRPIDTMP::GetDataSizeNo(int nNo)
{
	DWORD dwRet;

	if (nNo < m_nElementCountBase) {
		dwRet = CInfoMapEventBase::GetDataSizeNo (nNo);
	} else {
		switch (nNo - m_nElementCountBase) {
		case 0:	dwRet = sizeof (m_nSetType);	break;	/* 設定種別 */
		case 1:	dwRet = sizeof (m_dwIDMain);	break;	/* 画像IDメイン */
		case 2:	dwRet = sizeof (m_dwIDSub);		break;	/* 画像IDサブ */
		}
	}

	return dwRet;
}


/* ========================================================================= */
/* 関数名	:CInfoMapEventGRPIDTMP::GetName									 */
/* 内容		:要素名を取得													 */
/* 日付		:2008/10/12														 */
/* ========================================================================= */

LPCSTR CInfoMapEventGRPIDTMP::GetName(int nNo)
{
	if (nNo < m_nElementCountBase) {
		return CInfoMapEventBase::GetName (nNo);
	}

	return s_aszName[nNo - m_nElementCountBase];
}


/* ========================================================================= */
/* 関数名	:CInfoMapEventGRPIDTMP::GetWriteData							 */
/* 内容		:指定要素の保存用データを取得									 */
/* 日付		:2008/10/12														 */
/* ========================================================================= */

PBYTE CInfoMapEventGRPIDTMP::GetWriteData(int nNo, PDWORD pdwSize)
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
	if (nNo < m_nElementCountBase) {
		pRet = CInfoMapEventBase::GetWriteData (nNo, pdwSize);
	} else {
		pRet = new BYTE[dwSize];

		switch (nNo - m_nElementCountBase) {
		case 0:	pSrc = (PBYTE)&m_nSetType;		break;	/* 設定種別 */
		case 1:	pSrc = (PBYTE)&m_dwIDMain;		break;	/* 画像IDメイン */
		case 2:	pSrc = (PBYTE)&m_dwIDSub;		break;	/* 画像IDサブ */
		}

		if (pSrc) {
			CopyMemory (pRet, pSrc, dwSize);
		}
	}

Exit:
	return pRet;
}


/* ========================================================================= */
/* 関数名	:CInfoMapEventGRPIDTMP::ReadElementData							 */
/* 内容		:指定要素データを読み込み										 */
/* 日付		:2008/10/12														 */
/* ========================================================================= */

DWORD CInfoMapEventGRPIDTMP::ReadElementData(
	PBYTE pSrc,		/* [in] データの読み込み元 */
	int nNo)		/* [in] 要素番号 */
{
	PBYTE pDst;
	DWORD dwSize;

	pDst	= NULL;
	dwSize	= 0;

	if (nNo < m_nElementCountBase) {
		dwSize = CInfoMapEventBase::ReadElementData (pSrc, nNo);
	} else {
		switch (nNo - m_nElementCountBase) {
		case 0:	pDst = (PBYTE)&m_nSetType;	dwSize = sizeof (m_nSetType);	break;	/* 設定種別 */
		case 1:	pDst = (PBYTE)&m_dwIDMain;	dwSize = sizeof (m_dwIDMain);	break;	/* 画像IDメイン */
		case 2:	pDst = (PBYTE)&m_dwIDSub;	dwSize = sizeof (m_dwIDSub);	break;	/* 画像IDサブ */
		}

		if (pDst) {
			CopyMemory (pDst, pSrc, dwSize);
		}
	}

	return dwSize;
}


/* ========================================================================= */
/* 関数名	:CInfoMapEventGRPIDTMP::GetSendDataSize							 */
/* 内容		:送信データサイズを取得											 */
/* 日付		:2008/10/12														 */
/* ========================================================================= */

DWORD CInfoMapEventGRPIDTMP::GetSendDataSize(void)
{
	DWORD dwRet;

	dwRet = CInfoMapEventBase::GetSendDataSize ();
	dwRet += sizeof (m_nSetType);
	dwRet += sizeof (m_dwIDMain);
	dwRet += sizeof (m_dwIDSub);

	return dwRet;
}


/* ========================================================================= */
/* 関数名	:CInfoMapEventGRPIDTMP::GetSendData								 */
/* 内容		:送信データを取得												 */
/* 日付		:2008/10/12														 */
/* ========================================================================= */

PBYTE CInfoMapEventGRPIDTMP::GetSendData(void)
{
	PBYTE pData, pDataTmp, pDataBase;
	DWORD dwSize, dwSizeBase;

	pDataBase  = CInfoMapEventBase::GetSendData ();
	dwSizeBase = CInfoMapEventBase::GetSendDataSize ();

	dwSize	= GetSendDataSize ();
	pData	= ZeroNew (dwSize);

	pDataTmp = pData;

	CopyMemoryRenew (pDataTmp, pDataBase, dwSizeBase, pDataTmp);

	CopyMemoryRenew (pDataTmp, &m_nSetType,	sizeof (m_nSetType), pDataTmp);		/* 設定種別 */
	CopyMemoryRenew (pDataTmp, &m_dwIDMain,	sizeof (m_dwIDMain), pDataTmp);		/* 画像IDメイン */
	CopyMemoryRenew (pDataTmp, &m_dwIDSub,	sizeof (m_dwIDSub),	 pDataTmp);		/* 画像IDサブ */

	SAFE_DELETE_ARRAY (pDataBase);

	return pData;
}


/* ========================================================================= */
/* 関数名	:CInfoMapEventGRPIDTMP::SetSendData								 */
/* 内容		:送信データから取り込み											 */
/* 日付		:2008/10/12														 */
/* ========================================================================= */

PBYTE CInfoMapEventGRPIDTMP::SetSendData(PBYTE pSrc)
{
	PBYTE pRet, pDataTmp;

	pRet = pSrc;

	pDataTmp = pSrc;
	pDataTmp = CInfoMapEventBase::SetSendData (pSrc);

	CopyMemoryRenew (&m_nSetType,	pDataTmp, sizeof (m_nSetType),	pDataTmp);		/* 設定種別 */
	CopyMemoryRenew (&m_dwIDMain,	pDataTmp, sizeof (m_dwIDMain),	pDataTmp);		/* 画像IDメイン */
	CopyMemoryRenew (&m_dwIDSub,	pDataTmp, sizeof (m_dwIDSub),	pDataTmp);		/* 画像IDサブ */

	pRet = pDataTmp;
	return pRet;
}


/* ========================================================================= */
/* 関数名	:CInfoMapEventGRPIDTMP::Copy									 */
/* 内容		:コピー															 */
/* 日付		:2008/10/12														 */
/* 備考		:種別が違う場合はおかしなことになるので注意						 */
/* ========================================================================= */

void CInfoMapEventGRPIDTMP::Copy(CInfoMapEventBase *pSrc)
{
	PCInfoMapEventGRPIDTMP pSrcTmp = (PCInfoMapEventGRPIDTMP)pSrc;

	if (pSrcTmp == NULL) {
		return;
	}
	CInfoMapEventBase::Copy (pSrc);

	m_nSetType	= pSrcTmp->m_nSetType;	/* 設定種別 */
	m_dwIDMain	= pSrcTmp->m_dwIDMain;	/* 画像IDメイン */
	m_dwIDSub	= pSrcTmp->m_dwIDSub;	/* 画像IDサブ */
}

/* Copyright(C)URARA-works 2008 */
