/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:LibInfoEffect.cpp											 */
/* 内容			:エフェクト情報ライブラリクラス 実装ファイル				 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/07/26													 */
/* ========================================================================= */

#include "stdafx.h"
#include "LibInfoEffect.h"


/* ========================================================================= */
/* 関数名	:CLibInfoEffect::CLibInfoEffect									 */
/* 内容		:コンストラクタ													 */
/* 日付		:2007/07/26														 */
/* ========================================================================= */

CLibInfoEffect::CLibInfoEffect()
{
	m_paInfo = NULL;
}


/* ========================================================================= */
/* 関数名	:CLibInfoEffect::~CLibInfoEffect								 */
/* 内容		:デストラクタ													 */
/* 日付		:2007/07/26														 */
/* ========================================================================= */

CLibInfoEffect::~CLibInfoEffect()
{
	Destroy ();
}


/* ========================================================================= */
/* 関数名	:CLibInfoEffect::Create											 */
/* 内容		:作成															 */
/* 日付		:2007/07/26														 */
/* ========================================================================= */

void CLibInfoEffect::Create(void)
{
	m_paInfo = new ARRAYEFFECT;
}


/* ========================================================================= */
/* 関数名	:CLibInfoEffect::Destroy										 */
/* 内容		:破棄															 */
/* 日付		:2007/07/26														 */
/* ========================================================================= */

void CLibInfoEffect::Destroy(void)
{
	DeleteAll ();
	SAFE_DELETE (m_paInfo);
}


/* ========================================================================= */
/* 関数名	:CLibInfoEffect::Proc											 */
/* 内容		:処理															 */
/* 戻り値	:TRUE:処理した FALS:処理していない								 */
/* 日付		:2007/04/30														 */
/* ========================================================================= */

BOOL CLibInfoEffect::Proc(void)
{
	BOOL bRet;
	int i, nCount;
	PCInfoEffect pInfo;

	bRet = FALSE;

	nCount = GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfo = (PCInfoEffect)GetPtr (i);
		bRet |= pInfo->TimerProc (timeGetTime ());
	}

	return bRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoEffect::GetNew											 */
/* 内容		:新規データを取得												 */
/* 日付		:2007/07/26														 */
/* ========================================================================= */

PCInfoBase CLibInfoEffect::GetNew(void)
{
	return new CInfoEffect;
}


/* ========================================================================= */
/* 関数名	:CLibInfoEffect::GetCount										 */
/* 内容		:データ数を取得													 */
/* 日付		:2007/07/26														 */
/* ========================================================================= */

int CLibInfoEffect::GetCount(void)
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
/* 関数名	:CLibInfoEffect::Add											 */
/* 内容		:追加															 */
/* 日付		:2007/07/26														 */
/* ========================================================================= */

void CLibInfoEffect::Add(PCInfoBase pInfo)
{
	PCInfoEffect pEffectInfo;

	pEffectInfo = (PCInfoEffect)pInfo;
	if (pEffectInfo->m_dwEffectID == 0) {
		pEffectInfo->m_dwEffectID = GetNewID ();
	}

	m_paInfo->Add (pEffectInfo);
}


/* ========================================================================= */
/* 関数名	:CLibInfoEffect::Delete											 */
/* 内容		:削除															 */
/* 日付		:2007/07/26														 */
/* ========================================================================= */

void CLibInfoEffect::Delete(
	int nNo)		/* [in] 配列番号 */
{
	PCInfoEffect pInfo;

	pInfo = m_paInfo->GetAt (nNo);
	SAFE_DELETE (pInfo);
	m_paInfo->RemoveAt (nNo);
}


/* ========================================================================= */
/* 関数名	:CLibInfoEffect::Delete											 */
/* 内容		:削除															 */
/* 日付		:2007/07/26														 */
/* ========================================================================= */

void CLibInfoEffect::Delete(
	DWORD dwEffectID)		/* [in] エフェクトID */
{
	int i, nCount, nNo;
	PCInfoEffect pInfoTmp;

	nNo = -1;

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->GetAt (i);
		if (pInfoTmp->m_dwEffectID != dwEffectID) {
			continue;
		}
		nNo = i;
		break;
	}

	if (nNo >= 0) {
		Delete (nNo);
	}
}


/* ========================================================================= */
/* 関数名	:CLibInfoEffect::DeleteAll										 */
/* 内容		:全て削除														 */
/* 日付		:2007/07/26														 */
/* ========================================================================= */

void CLibInfoEffect::DeleteAll(void)
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
/* 関数名	:CLibInfoEffect::GetName										 */
/* 内容		:エフェクト名を取得												 */
/* 日付		:2008/07/06														 */
/* ========================================================================= */

void CLibInfoEffect::GetName(
	DWORD dwEffectID,		/* [in]  エフェクトID */
	CmyString &strDst)		/* [out] エフェクト名 */
{
	int i, nCount;
	PCInfoEffect pInfo;

	strDst.Empty ();

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfo = m_paInfo->GetAt (i);
		if (pInfo->m_dwEffectID != dwEffectID) {
			continue;
		}
		strDst = pInfo->m_strName;
		break;
	}
}


/* ========================================================================= */
/* 関数名	:CLibInfoEffect::GetPtr											 */
/* 内容		:情報を取得														 */
/* 日付		:2007/07/26														 */
/* ========================================================================= */

PCInfoBase CLibInfoEffect::GetPtr(int nNo)
{
	PCInfoBase pRet;

	pRet = NULL;
	if (nNo >= m_paInfo->GetSize ()) {
		goto Exit;
	}

	pRet = m_paInfo->GetAt (nNo);
Exit:
	return pRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoEffect::GetPtr											 */
/* 内容		:情報を取得														 */
/* 日付		:2007/07/26														 */
/* ========================================================================= */

PCInfoBase CLibInfoEffect::GetPtr(
	DWORD dwEffectID)		/* [in] エフェクトID */
{
	int i, nCount;
	PCInfoEffect pRet, pInfoTmp;

	pRet = NULL;

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->GetAt (i);
		if (pInfoTmp->m_dwEffectID != dwEffectID) {
			continue;
		}
		pRet = pInfoTmp;
		break;
	}

	return pRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoEffect::GetSendDataSize								 */
/* 内容		:送信データサイズを取得											 */
/* 日付		:2007/07/26														 */
/* ========================================================================= */

DWORD CLibInfoEffect::GetSendDataSize(void)
{
	int i, nCount;
	DWORD dwSize;
	PCInfoEffect pInfoEffect;

	dwSize = 0;

	nCount = GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfoEffect = (PCInfoEffect)GetPtr (i);
		dwSize += pInfoEffect->GetSendDataSize ();
	}
	/* 終端用 */
	dwSize += sizeof (DWORD);

	return dwSize;
}


/* ========================================================================= */
/* 関数名	:CLibInfoEffect::GetSendData									 */
/* 内容		:送信データを取得												 */
/* 日付		:2007/07/26														 */
/* ========================================================================= */

PBYTE CLibInfoEffect::GetSendData(void)
{
	int i, nCount;
	PBYTE pData, pDataPos, pDataTmp;
	DWORD dwSize, dwSizeTmp;
	PCInfoEffect pInfoEffect;

	dwSize	= GetSendDataSize ();
	pData	= new BYTE[dwSize];
	ZeroMemory (pData, dwSize);

	pDataPos = pData;

	nCount = GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfoEffect = (PCInfoEffect)GetPtr (i);

		dwSizeTmp	= pInfoEffect->GetSendDataSize ();
		pDataTmp	= pInfoEffect->GetSendData ();
		CopyMemoryRenew (pDataPos, pDataTmp, dwSizeTmp, pDataPos);
		SAFE_DELETE_ARRAY (pDataTmp);
	}

	return pData;
}


/* ========================================================================= */
/* 関数名	:CLibInfoEffect::SetSendData									 */
/* 内容		:送信データから取り込み											 */
/* 日付		:2007/07/26														 */
/* ========================================================================= */

PBYTE CLibInfoEffect::SetSendData(PBYTE pSrc)
{
	PBYTE pDataTmp;
	DWORD dwTmp;
	PCInfoEffect pInfoEffect, pInfoEffectTmp;

	pDataTmp = pSrc;

	while (1) {
		CopyMemory (&dwTmp, pDataTmp, sizeof (DWORD));
		if (dwTmp == 0) {
			pDataTmp += sizeof (DWORD);
			break;
		}
		pInfoEffectTmp = (PCInfoEffect)GetNew ();
		pDataTmp = pInfoEffectTmp->SetSendData (pDataTmp);

		pInfoEffect = (PCInfoEffect)GetPtr (pInfoEffectTmp->m_dwEffectID);
		if (pInfoEffect) {
			pInfoEffect->Copy (pInfoEffectTmp);
			SAFE_DELETE (pInfoEffectTmp);
		} else {
			Add (pInfoEffectTmp);
		}
	}

	return pDataTmp;
}


/* ========================================================================= */
/* 関数名	:CLibInfoEffect::GetNewID										 */
/* 内容		:新しいIDを取得													 */
/* 日付		:2007/07/26														 */
/* ========================================================================= */

DWORD CLibInfoEffect::GetNewID(void)
{
	DWORD dwRet;
	int i, nCount;
	PCInfoEffect pInfoTmp;

	dwRet = 1;

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->GetAt (i);
		if (pInfoTmp->m_dwEffectID == dwRet) {
			dwRet ++;
			i = -1;
			continue;
		}
	}

	return dwRet;
}

/* Copyright(C)URARA-works 2007 */
