/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:LibInfoItemType.cpp										 */
/* 内容			:アイテム種別情報ライブラリ基底クラス 実装ファイル			 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/09/27													 */
/* ========================================================================= */

#include "stdafx.h"
#include "LibInfoItemType.h"


/* ========================================================================= */
/* 関数名	:CLibInfoItemType::CLibInfoItemType								 */
/* 内容		:コンストラクタ													 */
/* 日付		:2007/09/27														 */
/* ========================================================================= */

CLibInfoItemType::CLibInfoItemType()
{
	m_dwNewIDTmp	= 0;
	m_paInfo		= NULL;
}


/* ========================================================================= */
/* 関数名	:CLibInfoItemType::~CLibInfoItemType							 */
/* 内容		:デストラクタ													 */
/* 日付		:2007/09/27														 */
/* ========================================================================= */

CLibInfoItemType::~CLibInfoItemType()
{
	Destroy ();
}


/* ========================================================================= */
/* 関数名	:CLibInfoItemType::Create										 */
/* 内容		:作成															 */
/* 日付		:2007/09/27														 */
/* ========================================================================= */

void CLibInfoItemType::Create(void)
{
	m_paInfo = new ARRAYITEMTYPEINFO;
}


/* ========================================================================= */
/* 関数名	:CLibInfoItemType::Destroy										 */
/* 内容		:破棄															 */
/* 日付		:2007/09/27														 */
/* ========================================================================= */

void CLibInfoItemType::Destroy(void)
{
	DeleteAll ();
	SAFE_DELETE (m_paInfo);
}


/* ========================================================================= */
/* 関数名	:CLibInfoItemType::GetNew										 */
/* 内容		:新規データを取得												 */
/* 日付		:2007/09/27														 */
/* ========================================================================= */

PCInfoBase CLibInfoItemType::GetNew(void)
{
	return (PCInfoBase)GetNew (ITEMTYPEID_NONE);
}


/* ========================================================================= */
/* 関数名	:CLibInfoItemType::GetNew										 */
/* 内容		:新規データを取得												 */
/* 日付		:2007/12/08														 */
/* ========================================================================= */

PCInfoBase CLibInfoItemType::GetNew(DWORD dwItemTypeID)
{
#if 0
	PCInfoBase pNew;

	switch (dwItemTypeID) {
	case ITEMTYPEID_ARMS:	pNew = (PCInfoBase)new CInfoItemTypeARMS;	break;	/* 持ち物 */
	default:				pNew = (PCInfoBase)new CInfoItemTypeBase;	break;
	}

	return pNew;
#endif
	PCInfoItemTypeBase pTmp;

	pTmp = new CInfoItemTypeBase;
	pTmp->m_dwItemTypeID = dwItemTypeID;

	return (PCInfoBase)pTmp;
}


/* ========================================================================= */
/* 関数名	:CLibInfoCharSvr::Revice										 */
/* 内容		:データの補正													 */
/* 日付		:2007/09/01														 */
/* ========================================================================= */

void CLibInfoItemType::Revice(void)
{
	int i, nCount;
	PCInfoItemTypeBase pInfoTmp, pInfo;

	nCount = GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = (PCInfoItemTypeBase)GetPtr (i);

		/* 移動種別に応じてクラスを更新 */
		pInfo = (PCInfoItemTypeBase)GetNew (pInfoTmp->m_dwItemTypeID);
		pInfo->Copy (pInfoTmp);
	}
}


/* ========================================================================= */
/* 関数名	:CLibInfoItemType::GetCount										 */
/* 内容		:データ数を取得													 */
/* 日付		:2007/09/27														 */
/* ========================================================================= */

int CLibInfoItemType::GetCount(void)
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
/* 関数名	:CLibInfoItemType::Add											 */
/* 内容		:追加															 */
/* 日付		:2007/09/27														 */
/* ========================================================================= */

void CLibInfoItemType::Add(PCInfoBase pInfo)
{
	PCInfoItemTypeBase pItemInfo;

	pItemInfo = (PCInfoItemTypeBase)pInfo;
	if (pItemInfo->m_dwTypeID == 0) {
		pItemInfo->m_dwTypeID = GetNewID ();
	}

	m_paInfo->Add (pItemInfo);
}


/* ========================================================================= */
/* 関数名	:CLibInfoItemType::Delete										 */
/* 内容		:削除															 */
/* 日付		:2007/09/27														 */
/* ========================================================================= */

void CLibInfoItemType::Delete(
	int nNo)		/* [in] 配列番号 */
{
	PCInfoItemTypeBase pInfo;

	pInfo = m_paInfo->GetAt (nNo);
	SAFE_DELETE (pInfo);
	m_paInfo->RemoveAt (nNo);
}


/* ========================================================================= */
/* 関数名	:CLibInfoItemType::Delete										 */
/* 内容		:削除															 */
/* 日付		:2007/09/27														 */
/* ========================================================================= */

void CLibInfoItemType::Delete(
	DWORD m_dwTypeID)		/* [in] 種別ID */
{
	int i, nCount, nNo;
	PCInfoItemTypeBase pInfoTmp;

	nNo = -1;

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->GetAt (i);
		if (pInfoTmp->m_dwTypeID != m_dwTypeID) {
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
/* 関数名	:CLibInfoItemType::DeleteAll									 */
/* 内容		:全て削除														 */
/* 日付		:2007/09/27														 */
/* ========================================================================= */

void CLibInfoItemType::DeleteAll(void)
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
/* 関数名	:CLibInfoItemType::GetGrpIDMainCount							 */
/* 内容		:アイテム種別の画像IDメイン数を取得								 */
/* 日付		:2007/10/07														 */
/* ========================================================================= */

int CLibInfoItemType::GetGrpIDMainCount(DWORD dwTypeID)
{
	int nRet;

	nRet = 1;

	switch (dwTypeID) {
	case ITEMTYPEID_CLOTH:		nRet = ITEMGRPIDMAIN_CLOTH_MAX;		break;	/* 服 */
	case ITEMTYPEID_ARMS:		nRet = ITEMGRPIDMAIN_ARMS_MAX;		break;	/* 持ち物 */
	}

	return nRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoItemType::GetTypeName									 */
/* 内容		:アイテム種別名を取得											 */
/* 日付		:2007/09/23														 */
/* ========================================================================= */

LPCSTR CLibInfoItemType::GetTypeName(DWORD dwItemTypeID)
{
	LPCSTR pszRet;

	pszRet = "";

	switch (dwItemTypeID) {
	case ITEMTYPEID_NONE:		pszRet = "効果無し";		break;	/* 効果無し */
	case ITEMTYPEID_CLOTH:		pszRet = "服";				break;	/* 服 */
	case ITEMTYPEID_ACCE:		pszRet = "アクセサリ";		break;	/* アクセサリ */
	case ITEMTYPEID_ARMS:		pszRet = "持ち物";			break;	/* 持ち物 */
	case ITEMTYPEID_SHIELD:		pszRet = "盾";				break;	/* 盾 */
	case ITEMTYPEID_HP:			pszRet = "HP増減";			break;	/* HP増減 */
	case ITEMTYPEID_LIGHT:		pszRet = "灯り";			break;	/* 灯り */
	}

	return pszRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoItemType::GetGrpIDMainName								 */
/* 内容		:画像IDメイン種別名を取得										 */
/* 日付		:2007/10/07														 */
/* ========================================================================= */

LPCSTR CLibInfoItemType::GetGrpIDMainName(
	DWORD dwTypeID,			/* [in] アイテム種別 */ 
	WORD wGrpIDMain)		/* [in] 画像IDメイン */
{
	LPCSTR pszRet;

	pszRet = "種別の基本画像";

	switch (dwTypeID) {
	case ITEMTYPEID_CLOTH:	/* 服 */
		switch (wGrpIDMain) {
		case ITEMGRPIDMAIN_CLOTH_NONE:	pszRet = "種別の基本画像";	break;	/* 未設定 */
		case ITEMGRPIDMAIN_CLOTH_SP:	pszRet = "特殊服";			break;	/* 特殊服 */
		}
		break;
	case ITEMTYPEID_ARMS:	/* 持ち物 */
		switch (wGrpIDMain) {
		case ITEMGRPIDMAIN_ARMS_NONE:		pszRet = "種別の基本画像";	break;	/* 未設定 */
		case ITEMGRPIDMAIN_ARMS_BOW:		pszRet = "弓";				break;	/* 弓 */
		case ITEMGRPIDMAIN_WEAPON_GLOVE:	pszRet = "武器(打撃)";		break;	/* 武器(打撃) */
		case ITEMGRPIDMAIN_WEAPON_ETC:		pszRet = "武器(その他)";	break;	/* 武器(その他) */
		case ITEMGRPIDMAIN_2X2_ARMS:		pszRet = "持ち物(2x2)";		break;	/* 持ち物(2x2) */
		case ITEMGRPIDMAIN_2X2_SHIELD:		pszRet = "盾(2x2)";			break;	/* 盾(2x2) */
		case ITEMGRPIDMAIN_2X2_ARMSSP:		pszRet = "特殊持ち物(2x2)";	break;	/* 特殊持ち物(2x2) */
		case ITEMGRPIDMAIN_2X2_BOW:			pszRet = "弓(2x2)";			break;	/* 弓(2x2) */
		}
		break;
	}

	return pszRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoItemType::GetGrpIDMain									 */
/* 内容		:画像IDメインを取得												 */
/* 日付		:2007/12/23														 */
/* ========================================================================= */

DWORD CLibInfoItemType::GetGrpIDMain(
	DWORD dwTypeID,			/* [in] アイテム種別 */ 
	WORD wGrpIDMain)		/* [in] 画像IDメイン */
{
	return 0;
}


/* ========================================================================= */
/* 関数名	:CLibInfoItemType::GetTypeNameTypeID							 */
/* 内容		:アイテム種別名を取得											 */
/* 日付		:2007/10/06														 */
/* ========================================================================= */

LPCSTR CLibInfoItemType::GetTypeNameTypeID(
	DWORD dwTypeID)		/* [in] 種別ID */
{
	LPCSTR pRet;
	PCInfoItemTypeBase pInfoTmp;

	pRet = NULL;

	pInfoTmp = (PCInfoItemTypeBase)GetPtr (dwTypeID);
	if (pInfoTmp == NULL) {
		goto Exit;
	}

	pRet = GetTypeName (pInfoTmp->m_dwItemTypeID);
Exit:
	return pRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoItemType::GetPtr										 */
/* 内容		:アイテム情報を取得												 */
/* 日付		:2007/09/27														 */
/* ========================================================================= */

PCInfoBase CLibInfoItemType::GetPtr(int nNo)
{
	return m_paInfo->GetAt (nNo);
}


/* ========================================================================= */
/* 関数名	:CLibInfoItemType::GetPtr										 */
/* 内容		:アイテム情報を取得												 */
/* 日付		:2007/09/27														 */
/* ========================================================================= */

PCInfoBase CLibInfoItemType::GetPtr(
	DWORD dwTypeID)		/* [in] 種別ID */
{
	int i, nCount;
	PCInfoItemTypeBase pRet, pInfoTmp;

	pRet = NULL;

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->GetAt (i);
		if (pInfoTmp->m_dwTypeID != dwTypeID) {
			continue;
		}
		pRet = pInfoTmp;
		break;
	}

	return pRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoItemType::GetSendDataSize								 */
/* 内容		:送信データサイズを取得											 */
/* 日付		:2007/09/27														 */
/* ========================================================================= */

DWORD CLibInfoItemType::GetSendDataSize(void)
{
	int i, nCount;
	DWORD dwRet, dwSize;
	PCInfoItemTypeBase pItem;

	dwRet = dwSize = 0;

	/* データ数分のサイズ */
	dwSize += sizeof (DWORD);

	nCount = GetCount ();
	for (i = 0; i < nCount; i ++) {
		pItem = (PCInfoItemTypeBase)GetPtr (i);

		dwSize += pItem->GetSendDataSize ();
	}

	dwRet = dwSize;
	return dwRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoItemType::GetSendData									 */
/* 内容		:送信データを取得												 */
/* 日付		:2007/09/27														 */
/* ========================================================================= */

PBYTE CLibInfoItemType::GetSendData(void)
{
	int i, nCount;
	PBYTE pRet, pData, pDataTmp;
	DWORD dwCount, dwSize, dwSizeTmp, dwOffset;
	PCInfoItemTypeBase pItem;

	pRet = NULL;

	dwSize		= GetSendDataSize ();
	pData		= ZeroNew (dwSize);
	dwOffset	= 0;

	/* データ数を書き込み */
	dwCount = (DWORD)GetCount ();
	CopyMemory (pData, &dwCount, sizeof (dwCount));
	dwOffset += sizeof (dwCount);

	/* キャラ情報を書き込み */
	nCount = GetCount ();
	for (i = 0; i < nCount; i ++) {
		pItem = (PCInfoItemTypeBase)GetPtr (i);

		dwSizeTmp	= pItem->GetSendDataSize ();
		pDataTmp	= pItem->GetSendData ();
		CopyMemory (&pData[dwOffset], pDataTmp, dwSizeTmp);
		dwOffset += dwSizeTmp;

		SAFE_DELETE_ARRAY (pDataTmp);
	}

	pRet = pData;
	return pRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoItemType::SetSendData									 */
/* 内容		:送信データから取り込み											 */
/* 日付		:2007/09/27														 */
/* 戻り値	:処理した後のアドレス											 */
/* ========================================================================= */

PBYTE CLibInfoItemType::SetSendData(PBYTE pSrc)
{
	int i, nCount;
	DWORD dwCount;
	PBYTE pRet, pDataTmp;
	CInfoItemTypeBase ItemTmp, *pItem;

	pRet		= pSrc;
	pDataTmp	= pSrc;

	DeleteAll ();

	/* データ数を読み込み */
	CopyMemoryRenew (&dwCount, pDataTmp, sizeof (dwCount), pDataTmp);
	nCount = (int)dwCount;

	for (i = 0; i < nCount; i ++) {
		/* まずは基底クラスへ取り込み */
		ItemTmp.SetSendData (pDataTmp);
		pItem = (PCInfoItemTypeBase)GetNew (ItemTmp.m_dwItemTypeID);

		pDataTmp = pItem->SetSendData (pDataTmp);
		Add (pItem);
	}

	pRet = pDataTmp;
	return pRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoItemType::GetNewSendData								 */
/* 内容		:送信データから新規データを取得									 */
/* 日付		:2007/12/08														 */
/* 戻り値	:処理した後のアドレス											 */
/* ========================================================================= */

PBYTE CLibInfoItemType::GetNewSendData(PBYTE pSrc, PCInfoItemTypeBase &pDst)
{
	PBYTE pRet;
	CInfoItemTypeBase InfoTmp;

	SAFE_DELETE (pDst);

	/* まずは基底クラスへ取り込み */
	InfoTmp.SetSendData (pSrc);
	pDst = (PCInfoItemTypeBase)GetNew (InfoTmp.m_dwItemTypeID);
	pRet = pDst->SetSendData (pSrc);

	return pRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoItemType::GetNewID										 */
/* 内容		:新しいアイテム種別IDを取得										 */
/* 日付		:2007/09/27														 */
/* ========================================================================= */

DWORD CLibInfoItemType::GetNewID(void)
{
	DWORD dwRet;
	int i, nCount;
	PCInfoItemTypeBase pInfoTmp;

	dwRet = m_dwNewIDTmp + 1;
	if (dwRet == 0) {
		dwRet = 1;
	}

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->GetAt (i);
		if (pInfoTmp->m_dwTypeID == dwRet) {
			dwRet ++;
			i = -1;
			continue;
		}
	}
	m_dwNewIDTmp = dwRet;

	return dwRet;
}

/* Copyright(C)URARA-works 2007 */
