/* Copyright(C)URARA-works 2009 */
/* ========================================================================= */
/* ファイル名	:LibInfoDisable.cpp											 */
/* 内容			:拒否情報ライブラリ基底クラス 実装ファイル					 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2009/04/08													 */
/* ========================================================================= */

#include "stdafx.h"
#include "LibInfoDisable.h"


/* ========================================================================= */
/* 関数名	:CLibInfoDisable::CLibInfoDisable								 */
/* 内容		:コンストラクタ													 */
/* 日付		:2009/04/08														 */
/* ========================================================================= */

CLibInfoDisable::CLibInfoDisable()
{
	m_dwNewIDTmp		= 0;
	m_dwLastTimeProc	= 0;
	m_paInfo			= NULL;
	m_paInfoIPADdress	= NULL;
}


/* ========================================================================= */
/* 関数名	:CLibInfoDisable::~CLibInfoDisable								 */
/* 内容		:デストラクタ													 */
/* 日付		:2009/04/08														 */
/* ========================================================================= */

CLibInfoDisable::~CLibInfoDisable()
{
	Destroy ();
}


/* ========================================================================= */
/* 関数名	:CLibInfoDisable::Create										 */
/* 内容		:作成															 */
/* 日付		:2009/04/08														 */
/* ========================================================================= */

void CLibInfoDisable::Create(void)
{
	m_paInfo			= new ARRAYDISABLEINFO;
	m_paInfoIPADdress	= new ARRAYDISABLEIPADDRESS;
}


/* ========================================================================= */
/* 関数名	:CLibInfoDisable::Destroy										 */
/* 内容		:破棄															 */
/* 日付		:2009/04/08														 */
/* ========================================================================= */

void CLibInfoDisable::Destroy(void)
{
	DeleteAll ();
	DeleteAllIP ();
	SAFE_DELETE (m_paInfo);
	SAFE_DELETE (m_paInfoIPADdress);
}


/* ========================================================================= */
/* 関数名	:CLibInfoDisable::Proc											 */
/* 内容		:処理															 */
/* 戻り値	:TRUE:処理した FALS:処理していない								 */
/* 日付		:2009/04/11														 */
/* ========================================================================= */

BOOL CLibInfoDisable::Proc(void)
{
	int i, nCount;
	BOOL bRet;
	DWORD dwTime;
	PSTDISABLEIPADDRESS pInfo;

	bRet = FALSE;

	if (m_paInfoIPADdress == NULL) {
		goto Exit;
	}
	nCount = m_paInfoIPADdress->GetSize ();
	if (nCount <= 0) {
		goto Exit;
	}

	dwTime = timeGetTime ();
	if (dwTime - m_dwLastTimeProc < 1000 * 10) {
		/* 10秒以内なら処理しない */
		goto Exit;
	}
	m_dwLastTimeProc = dwTime;

	for (i = nCount - 1; i >= 0; i --) {
		pInfo = m_paInfoIPADdress->GetAt (i);
		/* 1時間以上経過している？ */
		if (dwTime - pInfo->dwLastTime >= 1000 * 60 * 60) {
			/* 拒否解除 */
			DeleteIP (i);
		}
	}

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoDisable::GetNew										 */
/* 内容		:新規データを取得												 */
/* 日付		:2009/04/08														 */
/* ========================================================================= */

PCInfoBase CLibInfoDisable::GetNew(void)
{
	PCInfoDisable pInfoDisable;

	pInfoDisable = new CInfoDisable;

	return pInfoDisable;
}


/* ========================================================================= */
/* 関数名	:CLibInfoDisable::GetCount										 */
/* 内容		:データ数を取得													 */
/* 日付		:2009/04/08														 */
/* ========================================================================= */

int CLibInfoDisable::GetCount(void)
{
	int nRet;

	nRet = 0;

	if (m_paInfo == NULL) {
		goto Exit;
	}

	nRet = m_paInfo->GetSize ();
Exit:
	return nRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoDisable::Add											 */
/* 内容		:追加															 */
/* 日付		:2009/04/08														 */
/* ========================================================================= */

void CLibInfoDisable::Add(PCInfoBase pInfo)
{
	PCInfoDisable pDisableInfo;

	pDisableInfo = (PCInfoDisable)pInfo;
	if (pDisableInfo->m_dwDisableID == 0) {
		pDisableInfo->m_dwDisableID = GetNewID ();
	}

	m_paInfo->Add (pDisableInfo);
}


/* ========================================================================= */
/* 関数名	:CLibInfoDisable::Delete										 */
/* 内容		:削除															 */
/* 日付		:2009/04/08														 */
/* ========================================================================= */

void CLibInfoDisable::Delete(
	int nNo)		/* [in] 配列番号 */
{
	PCInfoDisable pInfo;

	pInfo = m_paInfo->GetAt (nNo);
	SAFE_DELETE (pInfo);
	m_paInfo->RemoveAt (nNo);
}


/* ========================================================================= */
/* 関数名	:CLibInfoDisable::Delete										 */
/* 内容		:削除															 */
/* 日付		:2009/04/08														 */
/* ========================================================================= */

void CLibInfoDisable::Delete(
	DWORD dwDisableID)		/* [in] 拒否情報ID */
{
	int i, nCount, nNo;
	PCInfoDisable pInfoTmp;

	Enter ();

	nNo = -1;

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->GetAt (i);
		if (pInfoTmp->m_dwDisableID != dwDisableID) {
			continue;
		}
		nNo = i;
		break;
	}

	if (nNo >= 0) {
		Delete (nNo);
	}

	Leave ();
}


/* ========================================================================= */
/* 関数名	:CLibInfoDisable::DeleteAll										 */
/* 内容		:全て削除														 */
/* 日付		:2009/04/08														 */
/* ========================================================================= */

void CLibInfoDisable::DeleteAll(void)
{
	int i, nCount;

	if (m_paInfo == NULL) {
		return;
	}

	nCount = m_paInfo->GetSize ();
	for (i = nCount - 1; i >= 0; i --) {
		Delete (i);
	}
}


/* ========================================================================= */
/* 関数名	:CLibInfoDisable::CopyAll										 */
/* 内容		:全てコピー														 */
/* 日付		:2009/04/11														 */
/* ========================================================================= */

void CLibInfoDisable::CopyAll(CLibInfoDisable *pSrc)
{
	int i, nCount;
	PCInfoDisable pInfoTmp, pInfoNew;

	DeleteAll ();

	nCount = pSrc->GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = (PCInfoDisable)pSrc->GetPtr (i);
		pInfoNew = (PCInfoDisable)GetNew ();
		pInfoNew->Copy (pInfoTmp);
		Add (pInfoNew);
	}

}


/* ========================================================================= */
/* 関数名	:CLibInfoDisable::IsDisable										 */
/* 内容		:拒否情報が登録されているか判定									 */
/* 日付		:2009/04/11														 */
/* ========================================================================= */

BOOL CLibInfoDisable::IsDisable(LPCSTR pszMacAddress)
{
	BOOL bRet;
	PCInfoBase pInfo;

	bRet = FALSE;

	pInfo = GetPtr (pszMacAddress);
	if (pInfo) {
		bRet = TRUE;
	}

	return bRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoDisable::Merge											 */
/* 内容		:取り込み														 */
/* 日付		:2009/04/11														 */
/* ========================================================================= */

void CLibInfoDisable::Merge(CLibInfoDisable *pSrc)
{
	int i, nCount;
	PCInfoDisable pInfoTmp, pInfoSrc;

	nCount = pSrc->GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfoSrc = (PCInfoDisable)pSrc->GetPtr (i);
		pInfoTmp = (PCInfoDisable)GetPtr (pInfoSrc->m_dwDisableID);
		if (pInfoTmp == NULL) {
			pInfoTmp = (PCInfoDisable)GetNew ();
			pInfoTmp->Copy (pInfoSrc);
			Add (pInfoTmp);
		}
		pInfoTmp->Copy (pInfoSrc);
	}
}


/* ========================================================================= */
/* 関数名	:CLibInfoDisable::GetPtr										 */
/* 内容		:拒否情報を取得													 */
/* 日付		:2009/04/08														 */
/* ========================================================================= */

PCInfoBase CLibInfoDisable::GetPtr(int nNo)
{
	return m_paInfo->GetAt (nNo);
}


/* ========================================================================= */
/* 関数名	:CLibInfoDisable::GetPtr										 */
/* 内容		:拒否情報を取得													 */
/* 日付		:2009/04/08														 */
/* ========================================================================= */

PCInfoBase CLibInfoDisable::GetPtr(
	DWORD dwDisableID)		/* [in] 拒否情報ID */
{
	int i, nCount;
	PCInfoDisable pRet, pInfoTmp;

	pRet = NULL;

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->GetAt (i);
		if (pInfoTmp->m_dwDisableID != dwDisableID) {
			continue;
		}
		pRet = pInfoTmp;
		break;
	}

	return pRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoDisable::GetPtr										 */
/* 内容		:拒否情報を取得													 */
/* 日付		:2009/04/11														 */
/* ========================================================================= */

PCInfoBase CLibInfoDisable::GetPtr(
	LPCSTR pszMacAddress)		/* [in] MACアドレス */
{
	int i, nCount;
	PCInfoDisable pRet, pInfoTmp;

	pRet = NULL;

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->GetAt (i);
		if (pInfoTmp->m_strMacAddress != pszMacAddress) {
			continue;
		}
		pRet = pInfoTmp;
		break;
	}

	return pRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoDisable::GetSendDataSize								 */
/* 内容		:送信データサイズを取得											 */
/* 日付		:2009/04/08														 */
/* ========================================================================= */

DWORD CLibInfoDisable::GetSendDataSize(void)
{
	int i, nCount;
	DWORD dwRet, dwSize;
	PCInfoDisable pDisable;

	dwRet = dwSize = 0;

	/* データ数分のサイズ */
	dwSize += sizeof (DWORD);

	nCount = GetCount ();
	for (i = 0; i < nCount; i ++) {
		pDisable = (PCInfoDisable)GetPtr (i);

		dwSize += pDisable->GetSendDataSize ();
	}

	dwRet = dwSize;
	return dwRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoDisable::GetSendData									 */
/* 内容		:送信データを取得												 */
/* 日付		:2009/04/08														 */
/* ========================================================================= */

PBYTE CLibInfoDisable::GetSendData(void)
{
	int i, nCount;
	PBYTE pRet, pData, pDataTmp;
	DWORD dwCount, dwSize, dwSizeTmp, dwOffset;
	PCInfoDisable pDisable;

	pRet = NULL;

	dwSize		= GetSendDataSize ();
	pData		= ZeroNew (dwSize);
	dwOffset	= 0;

	/* データ数を書き込み */
	dwCount = (DWORD)GetCount ();
	CopyMemory (pData, &dwCount, sizeof (dwCount));
	dwOffset += sizeof (dwCount);

	/* 拒否情報を書き込み */
	nCount = GetCount ();
	for (i = 0; i < nCount; i ++) {
		pDisable = (PCInfoDisable)GetPtr (i);

		dwSizeTmp	= pDisable->GetSendDataSize ();
		pDataTmp	= pDisable->GetSendData ();
		CopyMemory (&pData[dwOffset], pDataTmp, dwSizeTmp);
		dwOffset += dwSizeTmp;

		SAFE_DELETE_ARRAY (pDataTmp);
	}

	pRet = pData;
	return pRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoDisable::SetSendData									 */
/* 内容		:送信データから取り込み											 */
/* 日付		:2009/04/08														 */
/* 戻り値	:処理した後のアドレス											 */
/* ========================================================================= */

PBYTE CLibInfoDisable::SetSendData(PBYTE pSrc)
{
	int i, nCount;
	DWORD dwCount;
	PBYTE pRet, pDataTmp;
	PCInfoDisable pDisable;

	pRet		= pSrc;
	pDataTmp	= pSrc;

	DeleteAll ();

	/* データ数を読み込み */
	CopyMemory (&dwCount, pDataTmp, sizeof (dwCount));
	nCount		= (int)dwCount;
	pDataTmp	+= sizeof (dwCount);

	for (i = 0; i < nCount; i ++) {
		pDisable = (PCInfoDisable)GetNew ();

		pDataTmp = pDisable->SetSendData (pDataTmp);
		Add (pDisable);
	}

	pRet = pDataTmp;
	return pRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoDisable::AddIP											 */
/* 内容		:IPアドレス関連(追加)											 */
/* 日付		:2009/04/11														 */
/* ========================================================================= */

void CLibInfoDisable::AddIP(ULONG ulIPAddress)
{
	BOOL bResult;
	PSTDISABLEIPADDRESS pInfo;

	bResult = IsDisableIP (ulIPAddress);
	if (bResult) {
		/* 登録済み */
		return;
	}

	pInfo = new STDISABLEIPADDRESS;
	pInfo->ulIPAddress	= ulIPAddress;
	pInfo->dwLastTime	= timeGetTime ();
	m_paInfoIPADdress->Add (pInfo);
}


/* ========================================================================= */
/* 関数名	:CLibInfoDisable::DeleteIP										 */
/* 内容		:IPアドレス関連(削除)											 */
/* 日付		:2009/04/11														 */
/* ========================================================================= */

void CLibInfoDisable::DeleteIP(int nNo)
{
	PSTDISABLEIPADDRESS pInfo;

	pInfo = m_paInfoIPADdress->GetAt (nNo);
	SAFE_DELETE (pInfo);
	m_paInfoIPADdress->RemoveAt (nNo);
}


/* ========================================================================= */
/* 関数名	:CLibInfoDisable::DeleteAllIP									 */
/* 内容		:IPアドレス関連(全て削除)										 */
/* 日付		:2009/04/11														 */
/* ========================================================================= */

void CLibInfoDisable::DeleteAllIP(void)
{
	int i, nCount;

	if (m_paInfoIPADdress == NULL) {
		return;
	}
	nCount = m_paInfoIPADdress->GetSize ();
	for (i = nCount - 1; i >= 0; nCount --) {
		DeleteIP (i);
	}
}


/* ========================================================================= */
/* 関数名	:CLibInfoDisable::IsDisableIP									 */
/* 内容		:IPアドレス関連(禁止されているIPアドレスか判定)					 */
/* 日付		:2009/04/11														 */
/* ========================================================================= */

BOOL CLibInfoDisable::IsDisableIP(ULONG ulIPAddress)
{
	BOOL bRet;
	int i, nCount;
	PSTDISABLEIPADDRESS pInfo;

	bRet = FALSE;

	nCount = m_paInfoIPADdress->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfo = m_paInfoIPADdress->GetAt (i);
		if (pInfo->ulIPAddress != ulIPAddress) {
			continue;
		}
		bRet = TRUE;
		break;
	}

	return bRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoDisable::GetNewID										 */
/* 内容		:新しい拒否情報IDを取得											 */
/* 日付		:2009/04/08														 */
/* ========================================================================= */

DWORD CLibInfoDisable::GetNewID(void)
{
	DWORD dwRet;
	int i, nCount;
	PCInfoDisable pInfoTmp;

	dwRet = m_dwNewIDTmp + 1;
	if (dwRet == 0) {
		dwRet = 1;
	}

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->GetAt (i);
		if (pInfoTmp->m_dwDisableID == dwRet) {
			dwRet ++;
			i = -1;
			continue;
		}
	}
	m_dwNewIDTmp = dwRet;

	return dwRet;
}

/* Copyright(C)URARA-works 2009 */
