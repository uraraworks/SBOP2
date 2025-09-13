/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* ファイル名	:LibInfoSkill.cpp											 */
/* 内容			:スキル情報ライブラリクラス 実装ファイル					 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2008/12/04													 */
/* ========================================================================= */

#include "stdafx.h"
#include "InfoSkillMOVEATACK.h"
#include "InfoSkillHEAL.h"
#include "LibInfoSkill.h"


/* ========================================================================= */
/* 関数名	:CLibInfoSkill::CLibInfoSkill									 */
/* 内容		:コンストラクタ													 */
/* 日付		:2008/12/04														 */
/* ========================================================================= */

CLibInfoSkill::CLibInfoSkill()
{
	m_paInfo = NULL;
}


/* ========================================================================= */
/* 関数名	:CLibInfoSkill::~CLibInfoSkill									 */
/* 内容		:デストラクタ													 */
/* 日付		:2008/12/04														 */
/* ========================================================================= */

CLibInfoSkill::~CLibInfoSkill()
{
	Destroy ();
}


/* ========================================================================= */
/* 関数名	:CLibInfoSkill::Create											 */
/* 内容		:作成															 */
/* 日付		:2008/12/04														 */
/* ========================================================================= */

void CLibInfoSkill::Create(void)
{
	m_paInfo = new ARRAYSKILLBASEINFO;
}


/* ========================================================================= */
/* 関数名	:CLibInfoSkill::Destroy											 */
/* 内容		:破棄															 */
/* 日付		:2008/12/04														 */
/* ========================================================================= */

void CLibInfoSkill::Destroy(void)
{
	DeleteAll ();
	SAFE_DELETE (m_paInfo);
}


/* ========================================================================= */
/* 関数名	:CLibInfoSkill::GetNew											 */
/* 内容		:新規データを取得												 */
/* 日付		:2008/12/07														 */
/* ========================================================================= */

PCInfoBase CLibInfoSkill::GetNew(void)
{
	return (PCInfoBase)GetNew (0, 0);
}


/* ========================================================================= */
/* 関数名	:CLibInfoSkill::GetNew											 */
/* 内容		:新規データを取得												 */
/* 日付		:2008/12/04														 */
/* ========================================================================= */

PCInfoBase CLibInfoSkill::GetNew(int nTypeMain, int nTypeSub)
{
	PCInfoSkillBase pInfo;

	pInfo = NULL;

	switch (nTypeMain) {
	case SKILLTYPEMAIN_NONE:			/* 能力 */
		break;
	case SKILLTYPEMAIN_BATTLE:			/* 戦闘 */
		switch (nTypeSub) {
		case SKILLTYPESUB_BATTLE_MOVEATACK:		/* 移動して攻撃 */
			pInfo = new CInfoSkillMOVEATACK;
			break;
		case SKILLTYPESUB_BATTLE_HEAL:			/* 回復 */
			pInfo = new CInfoSkillHEAL;
			break;
		}
		break;
	case SKILLTYPEMAIN_LIFE:			/* 生活 */
		break;
	}

	if (pInfo == NULL) {
		pInfo = new CInfoSkillBase;
	}
	pInfo->m_nTypeMain = nTypeMain;
	pInfo->m_nTypeSub  = nTypeSub;

	return pInfo;
}


/* ========================================================================= */
/* 関数名	:CLibInfoSkill::GetCount										 */
/* 内容		:データ数を取得													 */
/* 日付		:2008/12/04														 */
/* ========================================================================= */

int CLibInfoSkill::GetCount(void)
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
/* 関数名	:CLibInfoSkill::Add												 */
/* 内容		:追加															 */
/* 日付		:2008/12/04														 */
/* ========================================================================= */

void CLibInfoSkill::Add(PCInfoBase pInfo)
{
	PCInfoSkillBase pInfoSkillBase;

	pInfoSkillBase = (PCInfoSkillBase)pInfo;
	if (pInfoSkillBase->m_dwSkillID == 0) {
		pInfoSkillBase->m_dwSkillID = GetNewID ();
	}

	m_paInfo->Add (pInfoSkillBase);
}


/* ========================================================================= */
/* 関数名	:CLibInfoSkill::Delete											 */
/* 内容		:削除															 */
/* 日付		:2008/12/04														 */
/* ========================================================================= */

void CLibInfoSkill::Delete(
	int nNo)		/* [in] 配列番号 */
{
	PCInfoSkillBase pInfo;

	pInfo = m_paInfo->GetAt (nNo);
	SAFE_DELETE (pInfo);
	m_paInfo->RemoveAt (nNo);
}


/* ========================================================================= */
/* 関数名	:CLibInfoSkill::Delete											 */
/* 内容		:削除															 */
/* 日付		:2008/12/04														 */
/* ========================================================================= */

void CLibInfoSkill::Delete(
	DWORD dwSkillID)		/* [in] スキルID */
{
	int i, nCount, nNo;
	PCInfoSkillBase pInfoTmp;

	nNo = -1;

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->GetAt (i);
		if (pInfoTmp->m_dwSkillID != dwSkillID) {
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
/* 関数名	:CLibInfoSkill::DeleteAll										 */
/* 内容		:全て削除														 */
/* 日付		:2008/12/04														 */
/* ========================================================================= */

void CLibInfoSkill::DeleteAll(void)
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
/* 関数名	:CLibInfoSkill::Merge											 */
/* 内容		:取り込み														 */
/* 日付		:2008/12/04														 */
/* ========================================================================= */

void CLibInfoSkill::Merge(CLibInfoSkill *pSrc)
{
	int i, nCount;
	PCInfoSkillBase pInfoTmp, pInfoSrc;

	nCount = pSrc->GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfoSrc = (PCInfoSkillBase)pSrc->GetPtr (i);
		pInfoTmp = (PCInfoSkillBase)GetPtr (pInfoSrc->m_dwSkillID);
		if (pInfoTmp == NULL) {
			pInfoTmp = (PCInfoSkillBase)GetNew (pInfoSrc->m_nTypeMain, pInfoSrc->m_nTypeSub);
			pInfoTmp->Copy (pInfoSrc);
			Add (pInfoTmp);
		}
		pInfoTmp->Copy (pInfoSrc);
	}
}


/* ========================================================================= */
/* 関数名	:CLibInfoSkill::Renew											 */
/* 内容		:更新															 */
/* 日付		:2008/12/08														 */
/* ========================================================================= */

CInfoSkillBase *CLibInfoSkill::Renew(CInfoSkillBase *pSrc)
{
	int i, nCount;
	PCInfoSkillBase pRet, pInfo, pInfoTmp;

	pRet = NULL;

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->GetAt (i);
		if (pInfoTmp->m_dwSkillID != pSrc->m_dwSkillID) {
			continue;
		}
		pInfo = (PCInfoSkillBase)GetNew (pSrc->m_nTypeMain, pSrc->m_nTypeSub);
		pInfo->Copy (pSrc);
		pInfo->m_dwSkillID = pInfoTmp->m_dwSkillID;

		SAFE_DELETE (pInfoTmp);
		m_paInfo->SetAt (i, pInfo);
		pRet = pInfo;
		break;
	}

	return pRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoSkill::GetPtr											 */
/* 内容		:情報を取得														 */
/* 日付		:2008/12/04														 */
/* ========================================================================= */

PCInfoBase CLibInfoSkill::GetPtr(int nNo)
{
	return m_paInfo->GetAt (nNo);
}


/* ========================================================================= */
/* 関数名	:CLibInfoSkill::GetPtr											 */
/* 内容		:情報を取得														 */
/* 日付		:2008/12/04														 */
/* ========================================================================= */

PCInfoBase CLibInfoSkill::GetPtr(
	DWORD dwSkillID)		/* [in] スキルID */
{
	int i, nCount;
	PCInfoSkillBase pRet, pInfoTmp;

	pRet = NULL;

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->GetAt (i);
		if (pInfoTmp->m_dwSkillID != dwSkillID) {
			continue;
		}
		pRet = pInfoTmp;
		break;
	}

	return pRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoSkill::RenewInfo										 */
/* 内容		:内容に応じて更新												 */
/* 日付		:2009/01/11														 */
/* ========================================================================= */

PCInfoBase CLibInfoSkill::RenewInfo(int nNo)
{
	PCInfoSkillBase pInfoSkillBase, pInfoTmp;

	pInfoSkillBase = (PCInfoSkillBase)GetPtr (nNo);

	pInfoTmp = (PCInfoSkillBase)GetNew (pInfoSkillBase->m_nTypeMain, pInfoSkillBase->m_nTypeSub);
	pInfoTmp->Copy (pInfoSkillBase);
	m_paInfo->SetAt (nNo, pInfoTmp);
	SAFE_DELETE (pInfoSkillBase);

	return pInfoTmp;
}


/* ========================================================================= */
/* 関数名	:CLibInfoSkill::GetSendDataSize									 */
/* 内容		:送信データサイズを取得											 */
/* 日付		:2008/12/04														 */
/* ========================================================================= */

DWORD CLibInfoSkill::GetSendDataSize(void)
{
	int i, nCount;
	DWORD dwSize;
	PCInfoSkillBase pInfoSkillBase;

	dwSize = 0;

	nCount = GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfoSkillBase = (PCInfoSkillBase)GetPtr (i);
		dwSize += pInfoSkillBase->GetSendDataSize ();
	}
	/* 終端用 */
	dwSize += sizeof (DWORD);

	return dwSize;
}


/* ========================================================================= */
/* 関数名	:CLibInfoSkill::GetSendData										 */
/* 内容		:送信データを取得												 */
/* 日付		:2008/12/04														 */
/* ========================================================================= */

PBYTE CLibInfoSkill::GetSendData(void)
{
	int i, nCount;
	PBYTE pData, pDataPos, pDataTmp;
	DWORD dwSize, dwSizeTmp;
	PCInfoSkillBase pInfoSkillBase;

	dwSize	= GetSendDataSize ();
	pData	= new BYTE[dwSize];
	ZeroMemory (pData, dwSize);

	pDataPos = pData;

	nCount = GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfoSkillBase = (PCInfoSkillBase)GetPtr (i);

		dwSizeTmp	= pInfoSkillBase->GetSendDataSize ();
		pDataTmp	= pInfoSkillBase->GetSendData ();
		CopyMemoryRenew (pDataPos, pDataTmp, dwSizeTmp, pDataPos);
		SAFE_DELETE_ARRAY (pDataTmp);
	}

	return pData;
}


/* ========================================================================= */
/* 関数名	:CLibInfoSkill::SetSendData										 */
/* 内容		:送信データから取り込み											 */
/* 日付		:2008/12/04														 */
/* ========================================================================= */

PBYTE CLibInfoSkill::SetSendData(PBYTE pSrc)
{
	PBYTE pDataTmp, pDataTmpBack;
	DWORD dwTmp;
	CInfoSkillBase InfoTmp, *pInfoSkillBase, *pInfoSkillBaseTmp;

	pDataTmp = pSrc;

	while (1) {
		CopyMemory (&dwTmp, pDataTmp, sizeof (DWORD));
		if (dwTmp == 0) {
			pDataTmp += sizeof (DWORD);
			break;
		}
		pDataTmpBack = pDataTmp;
		InfoTmp.SetSendData (pDataTmp);
		pInfoSkillBaseTmp = (PCInfoSkillBase)GetNew (InfoTmp.m_nTypeMain, InfoTmp.m_nTypeSub);
		pDataTmp = pInfoSkillBaseTmp->SetSendData (pDataTmpBack);

		pInfoSkillBase = (PCInfoSkillBase)GetPtr (pInfoSkillBaseTmp->m_dwSkillID);
		if (pInfoSkillBase) {
			pInfoSkillBase->Copy (pInfoSkillBaseTmp);
			SAFE_DELETE (pInfoSkillBaseTmp);
		} else {
			Add (pInfoSkillBaseTmp);
		}
	}

	return pDataTmp;
}


/* ========================================================================= */
/* 関数名	:CLibInfoSkill::GetNewID										 */
/* 内容		:新しいIDを取得													 */
/* 日付		:2008/12/04														 */
/* ========================================================================= */

DWORD CLibInfoSkill::GetNewID(void)
{
	DWORD dwRet;
	int i, nCount;
	PCInfoSkillBase pInfoTmp;

	dwRet = 1;

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->GetAt (i);
		if (pInfoTmp->m_dwSkillID == dwRet) {
			dwRet ++;
			i = -1;
			continue;
		}
	}

	return dwRet;
}

/* Copyright(C)URARA-works 2008 */
