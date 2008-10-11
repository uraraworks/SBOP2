/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:InfoMapEventMOVE.cpp										 */
/* 内容			:イベント情報(マップ内移動)クラス 実装ファイル				 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/07/05													 */
/* ========================================================================= */

#include "stdafx.h"
#include "InfoMapEventMOVE.h"

/* ========================================================================= */
/* 定数定義																	 */
/* ========================================================================= */

/* ヘッダ情報 */
static LPCSTR s_aszName[] = {
	"ptDst",			/* 移動先 */
	NULL
};


/* ========================================================================= */
/* 関数名	:CInfoMapEventMOVE::CInfoMapEventMOVE							 */
/* 内容		:コンストラクタ													 */
/* 日付		:2007/07/05														 */
/* ========================================================================= */

CInfoMapEventMOVE::CInfoMapEventMOVE()
{
	m_nType = MAPEVENTTYPE_MOVE;
	ZeroMemory (&m_ptDst, sizeof (m_ptDst));

	for (m_nElementCount = 0; s_aszName[m_nElementCount] != NULL; m_nElementCount ++) {}
	m_nElementCount += m_nElementCountBase;
}


/* ========================================================================= */
/* 関数名	:CInfoMapEventMOVE::~CInfoMapEventMOVE							 */
/* 内容		:デストラクタ													 */
/* 日付		:2007/07/05														 */
/* ========================================================================= */

CInfoMapEventMOVE::~CInfoMapEventMOVE()
{
}


/* ========================================================================= */
/* 関数名	:CInfoMapEventMOVE::RenewSize									 */
/* 内容		:サイズ更新														 */
/* 日付		:2008/09/14														 */
/* ========================================================================= */

void CInfoMapEventMOVE::RenewSize(int nDirection, int nSize)
{
	CInfoMapEventBase::RenewSize (nDirection, nSize);

	switch (nDirection) {
	case 0:
		m_ptDst.y += nSize;
		break;
	case 2:
		m_ptDst.x += nSize;
		break;
	}
}


/* ========================================================================= */
/* 関数名	:CInfoMapEventMOVE::GetElementNo								 */
/* 内容		:要素番号を取得													 */
/* 日付		:2007/07/05														 */
/* ========================================================================= */

int CInfoMapEventMOVE::GetElementNo(LPCSTR pszName)
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
/* 関数名	:CInfoMapEventMOVE::GetDataSize									 */
/* 内容		:データサイズを取得												 */
/* 日付		:2007/07/05														 */
/* ========================================================================= */

DWORD CInfoMapEventMOVE::GetDataSize(void)
{
	DWORD dwRet;

	dwRet = CInfoMapEventBase::GetDataSize ();
	dwRet += sizeof (m_ptDst);

	return dwRet;
}


/* ========================================================================= */
/* 関数名	:CInfoMapEventMOVE::GetDataSizeNo								 */
/* 内容		:指定要素のデータサイズを取得									 */
/* 日付		:2007/07/05														 */
/* ========================================================================= */

DWORD CInfoMapEventMOVE::GetDataSizeNo(int nNo)
{
	DWORD dwRet;

	if (nNo < m_nElementCountBase) {
		dwRet = CInfoMapEventBase::GetDataSizeNo (nNo);
	} else {
		switch (nNo - m_nElementCountBase) {
		case 0:	dwRet = sizeof (m_ptDst);		break;
		}
	}

	return dwRet;
}


/* ========================================================================= */
/* 関数名	:CInfoMapEventMOVE::GetName										 */
/* 内容		:要素名を取得													 */
/* 日付		:2007/07/05														 */
/* ========================================================================= */

LPCSTR CInfoMapEventMOVE::GetName(int nNo)
{
	if (nNo < m_nElementCountBase) {
		return CInfoMapEventBase::GetName (nNo);
	}

	return s_aszName[nNo - m_nElementCountBase];
}


/* ========================================================================= */
/* 関数名	:CInfoMapEventMOVE::GetWriteData								 */
/* 内容		:指定要素の保存用データを取得									 */
/* 日付		:2007/07/05														 */
/* ========================================================================= */

PBYTE CInfoMapEventMOVE::GetWriteData(int nNo, PDWORD pdwSize)
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
		case 0:	pSrc = (PBYTE)&m_ptDst;		break;
		}

		if (pSrc) {
			CopyMemory (pRet, pSrc, dwSize);
		}
	}

Exit:
	return pRet;
}


/* ========================================================================= */
/* 関数名	:CInfoMapEventMOVE::ReadElementData								 */
/* 内容		:指定要素データを読み込み										 */
/* 日付		:2007/07/05														 */
/* ========================================================================= */

DWORD CInfoMapEventMOVE::ReadElementData(
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
		case 0: pDst = (PBYTE)&m_ptDst;	dwSize = sizeof (m_ptDst);	break;
		}

		if (pDst) {
			CopyMemory (pDst, pSrc, dwSize);
		}
	}

	return dwSize;
}


/* ========================================================================= */
/* 関数名	:CInfoMapEventMOVE::GetSendDataSize								 */
/* 内容		:送信データサイズを取得											 */
/* 日付		:2008/06/24														 */
/* ========================================================================= */

DWORD CInfoMapEventMOVE::GetSendDataSize(void)
{
	DWORD dwRet;

	dwRet = CInfoMapEventBase::GetSendDataSize ();
	dwRet += sizeof (m_ptDst);

	return dwRet;
}


/* ========================================================================= */
/* 関数名	:CInfoMapEventMOVE::GetSendData									 */
/* 内容		:送信データを取得												 */
/* 日付		:2008/06/24														 */
/* ========================================================================= */

PBYTE CInfoMapEventMOVE::GetSendData(void)
{
	PBYTE pData, pDataTmp, pDataBase;
	DWORD dwSize, dwSizeBase;

	pDataBase  = CInfoMapEventBase::GetSendData ();
	dwSizeBase = CInfoMapEventBase::GetSendDataSize ();

	dwSize	= GetSendDataSize ();
	pData	= ZeroNew (dwSize);

	pDataTmp = pData;

	CopyMemoryRenew (pDataTmp, pDataBase, dwSizeBase, pDataTmp);

	CopyMemoryRenew (pDataTmp, &m_ptDst, sizeof (m_ptDst), pDataTmp);	/* 移動先 */

	SAFE_DELETE_ARRAY (pDataBase);

	return pData;
}


/* ========================================================================= */
/* 関数名	:CInfoMapEventMOVE::SetSendData									 */
/* 内容		:送信データから取り込み											 */
/* 日付		:2008/06/24														 */
/* ========================================================================= */

PBYTE CInfoMapEventMOVE::SetSendData(PBYTE pSrc)
{
	PBYTE pRet, pDataTmp;

	pRet = pSrc;

	pDataTmp = pSrc;
	pDataTmp = CInfoMapEventBase::SetSendData (pSrc);

	CopyMemoryRenew (&m_ptDst, pDataTmp, sizeof (m_ptDst), pDataTmp);	/* 移動先 */

	pRet = pDataTmp;
	return pRet;
}


/* ========================================================================= */
/* 関数名	:CInfoMapEventMOVE::Copy										 */
/* 内容		:コピー															 */
/* 日付		:2008/06/25														 */
/* 備考		:種別が違う場合はおかしなことになるので注意						 */
/* ========================================================================= */

void CInfoMapEventMOVE::Copy(CInfoMapEventBase *pSrc)
{
	PCInfoMapEventMOVE pSrcTmp = (PCInfoMapEventMOVE)pSrc;

	if (pSrcTmp == NULL) {
		return;
	}
	CInfoMapEventBase::Copy (pSrc);

	m_ptDst = pSrcTmp->m_ptDst;
}

/* Copyright(C)URARA-works 2007 */
