/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:InfoMapEventMAPMOVE.cpp									 */
/* 内容			:イベント情報(マップ間移動)クラス 実装ファイル				 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/07/27													 */
/* ========================================================================= */

#include "stdafx.h"
#include "InfoMapEventMAPMOVE.h"

/* ========================================================================= */
/* 定数定義																	 */
/* ========================================================================= */

/* ヘッダ情報 */
static LPCSTR s_aszName[] = {
	"m_dwMapID",		/* 移動先マップID */
	"ptDst",			/* 移動先 */
	"m_nDirection",		/* 移動後の向き */
	NULL
};


/* ========================================================================= */
/* 関数名	:CInfoMapEventMAPMOVE::CInfoMapEventMAPMOVE						 */
/* 内容		:コンストラクタ													 */
/* 日付		:2008/07/27														 */
/* ========================================================================= */

CInfoMapEventMAPMOVE::CInfoMapEventMAPMOVE()
{
	m_nType			= MAPEVENTTYPE_MOVE;
	m_nDirection	= -1;
	ZeroMemory (&m_ptDst, sizeof (m_ptDst));

	for (m_nElementCount = 0; s_aszName[m_nElementCount] != NULL; m_nElementCount ++) {}
	m_nElementCount += m_nElementCountBase;
}


/* ========================================================================= */
/* 関数名	:CInfoMapEventMAPMOVE::~CInfoMapEventMAPMOVE					 */
/* 内容		:デストラクタ													 */
/* 日付		:2008/07/27														 */
/* ========================================================================= */

CInfoMapEventMAPMOVE::~CInfoMapEventMAPMOVE()
{
}


/* ========================================================================= */
/* 関数名	:CInfoMapEventMAPMOVE::GetElementNo								 */
/* 内容		:要素番号を取得													 */
/* 日付		:2008/07/27														 */
/* ========================================================================= */

int CInfoMapEventMAPMOVE::GetElementNo(LPCSTR pszName)
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
/* 関数名	:CInfoMapEventMAPMOVE::GetDataSize								 */
/* 内容		:データサイズを取得												 */
/* 日付		:2008/07/27														 */
/* ========================================================================= */

DWORD CInfoMapEventMAPMOVE::GetDataSize(void)
{
	DWORD dwRet;

	dwRet = CInfoMapEventBase::GetDataSize ();
	dwRet += sizeof (m_dwMapID);		/* 移動先マップID */
	dwRet += sizeof (m_ptDst);			/* 移動先 */
	dwRet += sizeof (m_nDirection);		/* 移動後の向き */

	return dwRet;
}


/* ========================================================================= */
/* 関数名	:CInfoMapEventMAPMOVE::GetDataSizeNo							 */
/* 内容		:指定要素のデータサイズを取得									 */
/* 日付		:2008/07/27														 */
/* ========================================================================= */

DWORD CInfoMapEventMAPMOVE::GetDataSizeNo(int nNo)
{
	DWORD dwRet;

	if (nNo < m_nElementCountBase) {
		dwRet = CInfoMapEventBase::GetDataSizeNo (nNo);
	} else {
		switch (nNo - m_nElementCountBase) {
		case 0:	dwRet = sizeof (m_dwMapID);		break;		/* 移動先マップID */
		case 1:	dwRet = sizeof (m_ptDst);		break;		/* 移動先 */
		case 2:	dwRet = sizeof (m_nDirection);	break;		/* 移動後の向き */
		}
	}

	return dwRet;
}


/* ========================================================================= */
/* 関数名	:CInfoMapEventMAPMOVE::GetName									 */
/* 内容		:要素名を取得													 */
/* 日付		:2008/07/27														 */
/* ========================================================================= */

LPCSTR CInfoMapEventMAPMOVE::GetName(int nNo)
{
	if (nNo < m_nElementCountBase) {
		return CInfoMapEventBase::GetName (nNo);
	}

	return s_aszName[nNo - m_nElementCountBase];
}


/* ========================================================================= */
/* 関数名	:CInfoMapEventMAPMOVE::GetWriteData								 */
/* 内容		:指定要素の保存用データを取得									 */
/* 日付		:2008/07/27														 */
/* ========================================================================= */

PBYTE CInfoMapEventMAPMOVE::GetWriteData(int nNo, PDWORD pdwSize)
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
		case 0:	pSrc = (PBYTE)&m_dwMapID;		break;		/* 移動先マップID */
		case 1:	pSrc = (PBYTE)&m_ptDst;			break;		/* 移動先 */
		case 2:	pSrc = (PBYTE)&m_nDirection;	break;		/* 移動後の向き */
		}

		if (pSrc) {
			CopyMemory (pRet, pSrc, dwSize);
		}
	}

Exit:
	return pRet;
}


/* ========================================================================= */
/* 関数名	:CInfoMapEventMAPMOVE::ReadElementData							 */
/* 内容		:指定要素データを読み込み										 */
/* 日付		:2008/07/27														 */
/* ========================================================================= */

DWORD CInfoMapEventMAPMOVE::ReadElementData(
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
		case 0: pDst = (PBYTE)&m_dwMapID;		dwSize = sizeof (m_dwMapID);	break;		/* 移動先マップID */
		case 1: pDst = (PBYTE)&m_ptDst;			dwSize = sizeof (m_ptDst);		break;		/* 移動先 */
		case 2: pDst = (PBYTE)&m_nDirection;	dwSize = sizeof (m_nDirection);	break;		/* 移動後の向き */
		}

		if (pDst) {
			CopyMemory (pDst, pSrc, dwSize);
		}
	}

	return dwSize;
}


/* ========================================================================= */
/* 関数名	:CInfoMapEventMAPMOVE::GetSendDataSize							 */
/* 内容		:送信データサイズを取得											 */
/* 日付		:2008/06/24														 */
/* ========================================================================= */

DWORD CInfoMapEventMAPMOVE::GetSendDataSize(void)
{
	DWORD dwRet;

	dwRet = CInfoMapEventBase::GetSendDataSize ();
	dwRet += sizeof (m_dwMapID);
	dwRet += sizeof (m_ptDst);
	dwRet += sizeof (m_nDirection);

	return dwRet;
}


/* ========================================================================= */
/* 関数名	:CInfoMapEventMAPMOVE::GetSendData								 */
/* 内容		:送信データを取得												 */
/* 日付		:2008/06/24														 */
/* ========================================================================= */

PBYTE CInfoMapEventMAPMOVE::GetSendData(void)
{
	PBYTE pData, pDataTmp, pDataBase;
	DWORD dwSize, dwSizeBase;

	pDataBase  = CInfoMapEventBase::GetSendData ();
	dwSizeBase = CInfoMapEventBase::GetSendDataSize ();

	dwSize	= GetSendDataSize ();
	pData	= ZeroNew (dwSize);

	pDataTmp = pData;

	CopyMemoryRenew (pDataTmp, pDataBase, dwSizeBase, pDataTmp);

	CopyMemoryRenew (pDataTmp, &m_dwMapID,		sizeof (m_dwMapID),		pDataTmp);	/* 移動先マップID */
	CopyMemoryRenew (pDataTmp, &m_ptDst,		sizeof (m_ptDst),		pDataTmp);	/* 移動先 */
	CopyMemoryRenew (pDataTmp, &m_nDirection,	sizeof (m_nDirection),	pDataTmp);	/* 移動後の向き */

	SAFE_DELETE_ARRAY (pDataBase);

	return pData;
}


/* ========================================================================= */
/* 関数名	:CInfoMapEventMAPMOVE::SetSendData								 */
/* 内容		:送信データから取り込み											 */
/* 日付		:2008/06/24														 */
/* ========================================================================= */

PBYTE CInfoMapEventMAPMOVE::SetSendData(PBYTE pSrc)
{
	PBYTE pRet, pDataTmp;

	pRet = pSrc;

	pDataTmp = pSrc;
	pDataTmp = CInfoMapEventBase::SetSendData (pSrc);

	CopyMemoryRenew (&m_dwMapID,	pDataTmp, sizeof (m_dwMapID),		pDataTmp);	/* 移動先マップID */
	CopyMemoryRenew (&m_ptDst,		pDataTmp, sizeof (m_ptDst),			pDataTmp);	/* 移動先 */
	CopyMemoryRenew (&m_nDirection,	pDataTmp, sizeof (m_nDirection),	pDataTmp);	/* 移動後の向き */

	pRet = pDataTmp;
	return pRet;
}


/* ========================================================================= */
/* 関数名	:CInfoMapEventMAPMOVE::Copy										 */
/* 内容		:コピー															 */
/* 日付		:2008/06/25														 */
/* 備考		:種別が違う場合はおかしなことになるので注意						 */
/* ========================================================================= */

void CInfoMapEventMAPMOVE::Copy(CInfoMapEventBase *pSrc)
{
	PCInfoMapEventMAPMOVE pSrcTmp = (PCInfoMapEventMAPMOVE)pSrc;

	if (pSrcTmp == NULL) {
		return;
	}
	CInfoMapEventBase::Copy (pSrc);

	m_dwMapID		= pSrcTmp->m_dwMapID;
	m_ptDst			= pSrcTmp->m_ptDst;
	m_nDirection	= pSrcTmp->m_nDirection;
}

/* Copyright(C)URARA-works 2008 */
