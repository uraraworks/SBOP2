/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:InfoMapEventLIGHT.cpp										 */
/* 内容			:イベント情報(灯り)クラス 実装ファイル						 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/12/07													 */
/* ========================================================================= */

#include "stdafx.h"
#include "InfoMapEventLIGHT.h"

/* ========================================================================= */
/* 定数定義																	 */
/* ========================================================================= */

/* ヘッダ情報 */
static LPCSTR s_aszName[] = {
	"m_bLightOn",		/* 灯りをつける */
	"m_dwTime",			/* 持続時間*/
	NULL
};


/* ========================================================================= */
/* 関数名	:CInfoMapEventLIGHT::CInfoMapEventLIGHT							 */
/* 内容		:コンストラクタ													 */
/* 日付		:2008/12/07														 */
/* ========================================================================= */

CInfoMapEventLIGHT::CInfoMapEventLIGHT()
{
	m_bLightOn	= FALSE;
	m_dwTime	= 0;

	for (m_nElementCount = 0; s_aszName[m_nElementCount] != NULL; m_nElementCount ++) {}
	m_nElementCount += m_nElementCountBase;
}


/* ========================================================================= */
/* 関数名	:CInfoMapEventLIGHT::~CInfoMapEventLIGHT						 */
/* 内容		:デストラクタ													 */
/* 日付		:2008/12/07														 */
/* ========================================================================= */

CInfoMapEventLIGHT::~CInfoMapEventLIGHT()
{
}


/* ========================================================================= */
/* 関数名	:CInfoMapEventLIGHT::GetElementNo								 */
/* 内容		:要素番号を取得													 */
/* 日付		:2008/12/07														 */
/* ========================================================================= */

int CInfoMapEventLIGHT::GetElementNo(LPCSTR pszName)
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
/* 関数名	:CInfoMapEventLIGHT::GetDataSize								 */
/* 内容		:データサイズを取得												 */
/* 日付		:2008/12/07														 */
/* ========================================================================= */

DWORD CInfoMapEventLIGHT::GetDataSize(void)
{
	DWORD dwRet;

	dwRet = CInfoMapEventBase::GetDataSize ();
	dwRet += sizeof (m_bLightOn);	/* 灯りをつける */
	dwRet += sizeof (m_dwTime);		/* 持続時間 */

	return dwRet;
}


/* ========================================================================= */
/* 関数名	:CInfoMapEventLIGHT::GetDataSizeNo								 */
/* 内容		:指定要素のデータサイズを取得									 */
/* 日付		:2008/12/07														 */
/* ========================================================================= */

DWORD CInfoMapEventLIGHT::GetDataSizeNo(int nNo)
{
	DWORD dwRet;

	if (nNo < m_nElementCountBase) {
		dwRet = CInfoMapEventBase::GetDataSizeNo (nNo);
	} else {
		switch (nNo - m_nElementCountBase) {
		case 0:	dwRet = sizeof (m_bLightOn);	break;	/* 灯りをつける */
		case 1:	dwRet = sizeof (m_dwTime);		break;	/* 持続時間 */
		}
	}

	return dwRet;
}


/* ========================================================================= */
/* 関数名	:CInfoMapEventLIGHT::GetName									 */
/* 内容		:要素名を取得													 */
/* 日付		:2008/12/07														 */
/* ========================================================================= */

LPCSTR CInfoMapEventLIGHT::GetName(int nNo)
{
	if (nNo < m_nElementCountBase) {
		return CInfoMapEventBase::GetName (nNo);
	}

	return s_aszName[nNo - m_nElementCountBase];
}


/* ========================================================================= */
/* 関数名	:CInfoMapEventLIGHT::GetWriteData								 */
/* 内容		:指定要素の保存用データを取得									 */
/* 日付		:2008/12/07														 */
/* ========================================================================= */

PBYTE CInfoMapEventLIGHT::GetWriteData(int nNo, PDWORD pdwSize)
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
		case 0:	pSrc = (PBYTE)&m_bLightOn;		break;	/* 灯りをつける */
		case 1:	pSrc = (PBYTE)&m_dwTime;		break;	/* 持続時間 */
		}

		if (pSrc) {
			CopyMemory (pRet, pSrc, dwSize);
		}
	}

Exit:
	return pRet;
}


/* ========================================================================= */
/* 関数名	:CInfoMapEventLIGHT::ReadElementData							 */
/* 内容		:指定要素データを読み込み										 */
/* 日付		:2008/12/07														 */
/* ========================================================================= */

DWORD CInfoMapEventLIGHT::ReadElementData(
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
		case 0:	pDst = (PBYTE)&m_bLightOn;	dwSize = sizeof (m_bLightOn);	break;	/* 灯りをつける */
		case 1:	pDst = (PBYTE)&m_dwTime;	dwSize = sizeof (m_dwTime);		break;	/* 持続時間 */
		}

		if (pDst) {
			CopyMemory (pDst, pSrc, dwSize);
		}
	}

	return dwSize;
}


/* ========================================================================= */
/* 関数名	:CInfoMapEventLIGHT::GetSendDataSize							 */
/* 内容		:送信データサイズを取得											 */
/* 日付		:2008/12/07														 */
/* ========================================================================= */

DWORD CInfoMapEventLIGHT::GetSendDataSize(void)
{
	DWORD dwRet;

	dwRet = CInfoMapEventBase::GetSendDataSize ();
	dwRet += sizeof (m_bLightOn);
	dwRet += sizeof (m_dwTime);

	return dwRet;
}


/* ========================================================================= */
/* 関数名	:CInfoMapEventLIGHT::GetSendData								 */
/* 内容		:送信データを取得												 */
/* 日付		:2008/12/07														 */
/* ========================================================================= */

PBYTE CInfoMapEventLIGHT::GetSendData(void)
{
	PBYTE pData, pDataTmp, pDataBase;
	DWORD dwSize, dwSizeBase;

	pDataBase  = CInfoMapEventBase::GetSendData ();
	dwSizeBase = CInfoMapEventBase::GetSendDataSize ();

	dwSize	= GetSendDataSize ();
	pData	= ZeroNew (dwSize);

	pDataTmp = pData;

	CopyMemoryRenew (pDataTmp, pDataBase, dwSizeBase, pDataTmp);

	CopyMemoryRenew (pDataTmp, &m_bLightOn,	sizeof (m_bLightOn), pDataTmp);		/* 灯りをつける */
	CopyMemoryRenew (pDataTmp, &m_dwTime,	sizeof (m_dwTime), pDataTmp);		/* 持続時間 */

	SAFE_DELETE_ARRAY (pDataBase);

	return pData;
}


/* ========================================================================= */
/* 関数名	:CInfoMapEventLIGHT::SetSendData								 */
/* 内容		:送信データから取り込み											 */
/* 日付		:2008/12/07														 */
/* ========================================================================= */

PBYTE CInfoMapEventLIGHT::SetSendData(PBYTE pSrc)
{
	PBYTE pRet, pDataTmp;

	pRet = pSrc;

	pDataTmp = pSrc;
	pDataTmp = CInfoMapEventBase::SetSendData (pSrc);

	CopyMemoryRenew (&m_bLightOn,	pDataTmp, sizeof (m_bLightOn),	pDataTmp);		/* 灯りをつける */
	CopyMemoryRenew (&m_dwTime,		pDataTmp, sizeof (m_dwTime),	pDataTmp);		/* 持続時間 */

	pRet = pDataTmp;
	return pRet;
}


/* ========================================================================= */
/* 関数名	:CInfoMapEventLIGHT::Copy										 */
/* 内容		:コピー															 */
/* 日付		:2008/12/07														 */
/* 備考		:種別が違う場合はおかしなことになるので注意						 */
/* ========================================================================= */

void CInfoMapEventLIGHT::Copy(CInfoMapEventBase *pSrc)
{
	PCInfoMapEventLIGHT pSrcTmp = (PCInfoMapEventLIGHT)pSrc;

	if (pSrcTmp == NULL) {
		return;
	}
	CInfoMapEventBase::Copy (pSrc);

	m_bLightOn	= pSrcTmp->m_bLightOn;	/* 灯りをつける */
	m_dwTime	= pSrcTmp->m_dwTime;	/* 持続時間 */
}

/* Copyright(C)URARA-works 2008 */
