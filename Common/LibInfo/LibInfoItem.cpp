/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名	:LibInfoItem.cpp											 */
/* 内容			:アイテム情報ライブラリ基底クラス 実装ファイル				 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2007/05/05													 */
/* ========================================================================= */

#include "stdafx.h"
#include "InfoCharBase.h"
#include "LibInfoItemType.h"
#include "LibInfoItemWeapon.h"
#include "LibInfoMapParts.h"
#include "LibInfoItem.h"


/* ========================================================================= */
/* 関数名	:CLibInfoItem::CLibInfoItem										 */
/* 内容		:コンストラクタ													 */
/* 日付		:2007/05/05														 */
/* ========================================================================= */

CLibInfoItem::CLibInfoItem()
{
	m_dwNewIDTmp			= 0;
	m_paInfo				= NULL;
	m_pLibInfoItemType		= NULL;
	m_pLibInfoItemWeapon	= NULL;
}


/* ========================================================================= */
/* 関数名	:CLibInfoItem::~CLibInfoItem									 */
/* 内容		:デストラクタ													 */
/* 日付		:2007/05/05														 */
/* ========================================================================= */

CLibInfoItem::~CLibInfoItem()
{
	Destroy ();
}


/* ========================================================================= */
/* 関数名	:CLibInfoItem::Create											 */
/* 内容		:作成															 */
/* 日付		:2007/05/05														 */
/* ========================================================================= */

void CLibInfoItem::Create(void)
{
	m_paInfo = new ARRAYITEMINFO;
}


/* ========================================================================= */
/* 関数名	:CLibInfoItem::Destroy											 */
/* 内容		:破棄															 */
/* 日付		:2007/05/05														 */
/* ========================================================================= */

void CLibInfoItem::Destroy(void)
{
	DeleteAll ();
	SAFE_DELETE (m_paInfo);
}


/* ========================================================================= */
/* 関数名	:CLibInfoItem::SetTypeInfo										 */
/* 内容		:アイテム種別情報を設定											 */
/* 日付		:2007/10/06														 */
/* ========================================================================= */

void CLibInfoItem::SetTypeInfo(CLibInfoItemType *pLibInfoItemType)
{
	m_pLibInfoItemType = pLibInfoItemType;
}


/* ========================================================================= */
/* 関数名	:CLibInfoItem::SetWeaponInfo									 */
/* 内容		:武器情報を設定													 */
/* 日付		:2007/10/06														 */
/* ========================================================================= */

void CLibInfoItem::SetWeaponInfo(CLibInfoItemWeapon *pLibInfoItemWeapon)
{
	m_pLibInfoItemWeapon = pLibInfoItemWeapon;
}


/* ========================================================================= */
/* 関数名	:CLibInfoItem::RenewSize										 */
/* 内容		:マップサイズ更新												 */
/* 日付		:2007/08/25														 */
/* ========================================================================= */

void CLibInfoItem::RenewSize(DWORD dwMapID, int nDirection, int nSize)
{
	int i, nCount;
	PCInfoItem pInfoItem;

	/* マップサイズをキャラサイズに変更 */
	nSize *= 2;

	nCount = GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfoItem = (PCInfoItem)GetPtr (i);

		if (pInfoItem->m_dwMapID != dwMapID) {
			continue;
		}

		switch (nDirection) {
		case 0:		/* 上 */
			pInfoItem->m_ptPos.y += nSize;
			pInfoItem->m_ptPos.y = max (pInfoItem->m_ptPos.y, 0);
			break;
		case 1:		/* 下 */
			break;
		case 2:		/* 左 */
			pInfoItem->m_ptPos.x += nSize;
			pInfoItem->m_ptPos.x = max (pInfoItem->m_ptPos.x, 0);
			break;
		case 3:		/* 右 */
			break;
		}
	}
}


/* ========================================================================= */
/* 関数名	:CLibInfoItem::GetNew											 */
/* 内容		:新規データを取得												 */
/* 日付		:2007/05/05														 */
/* ========================================================================= */

PCInfoBase CLibInfoItem::GetNew(void)
{
	PCInfoItem pInfoItem;

	pInfoItem = new CInfoItem;

	return pInfoItem;
}


/* ========================================================================= */
/* 関数名	:CLibInfoItem::GetCount											 */
/* 内容		:データ数を取得													 */
/* 日付		:2007/05/05														 */
/* ========================================================================= */

int CLibInfoItem::GetCount(void)
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
/* 関数名	:CLibInfoItem::Add												 */
/* 内容		:追加															 */
/* 日付		:2007/05/05														 */
/* ========================================================================= */

void CLibInfoItem::Add(PCInfoBase pInfo)
{
	PCInfoItem pItemInfo;

	pItemInfo = (PCInfoItem)pInfo;
	if (pItemInfo->m_dwItemID == 0) {
		pItemInfo->m_dwItemID = GetNewID ();
	}

	m_paInfo->Add (pItemInfo);
}


/* ========================================================================= */
/* 関数名	:CLibInfoItem::Delete											 */
/* 内容		:削除															 */
/* 日付		:2007/05/05														 */
/* ========================================================================= */

void CLibInfoItem::Delete(
	int nNo)		/* [in] 配列番号 */
{
	int i, nCount;
	PCInfoItem pInfo;

	pInfo = m_paInfo->GetAt (nNo);

	nCount = m_adwAreaID.GetSize ();
	for (i = 0; i < nCount; i ++) {
		if (m_adwAreaID[i] == pInfo->m_dwItemID) {
			m_adwAreaID.RemoveAt (i);
			break;
		}
	}

	SAFE_DELETE (pInfo);
	m_paInfo->RemoveAt (nNo);
}


/* ========================================================================= */
/* 関数名	:CLibInfoItem::Delete											 */
/* 内容		:削除															 */
/* 日付		:2007/05/05														 */
/* ========================================================================= */

void CLibInfoItem::Delete(
	DWORD dwItemID)		/* [in] アイテムID */
{
	int i, nCount, nNo;
	PCInfoItem pInfoTmp;

	nNo = -1;

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->GetAt (i);
		if (pInfoTmp->m_dwItemID != dwItemID) {
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
/* 関数名	:CLibInfoItem::DeleteAll										 */
/* 内容		:全て削除														 */
/* 日付		:2007/05/05														 */
/* ========================================================================= */

void CLibInfoItem::DeleteAll(void)
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
/* 関数名	:CLibInfoItem::Sort												 */
/* 内容		:表示位置に合わせてソート										 */
/* 日付		:2008/06/03														 */
/* ========================================================================= */

void CLibInfoItem::Sort(void)
{
	int i, j, nCount, nCountTmp, nNo;
	PCInfoItem pInfoItem, pInfoItemTmp;
	ARRAYITEMINFO aTmp;
	ARRAYINT anNo;

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoItem = m_paInfo->GetAt (i);
		nCountTmp = anNo.GetSize ();
		nNo = 0;
		if (nCountTmp > 0) {
			for (j = 0; j < nCountTmp; j ++) {
				pInfoItemTmp = m_paInfo->GetAt (anNo[j]);
				if (pInfoItem->m_ptPos.y < pInfoItemTmp->m_ptPos.y) {
					break;
				}
				if (pInfoItem->m_ptPos.y == pInfoItemTmp->m_ptPos.y) {
					if (pInfoItem->m_ptPos.x < pInfoItemTmp->m_ptPos.x) {
						break;
					}
				}
				if (pInfoItem->m_ptPos.y == pInfoItemTmp->m_ptPos.y) {
					if (pInfoItem->m_ptPos.x == pInfoItemTmp->m_ptPos.x) {
						if (pInfoItem->m_nPosZ < pInfoItemTmp->m_nPosZ) {
							break;
						}
					}
				}
				if (pInfoItem->m_ptPos.y == pInfoItemTmp->m_ptPos.y) {
					if (pInfoItem->m_ptPos.x == pInfoItemTmp->m_ptPos.x) {
						if (pInfoItem->m_nPosZ == pInfoItemTmp->m_nPosZ) {
							if (pInfoItem->m_dwItemID < pInfoItemTmp->m_dwItemID) {
								break;
							}
						}
					}
				}
			}
			nNo = j;
		}
		anNo.InsertAt (nNo, i);
		aTmp.InsertAt (nNo, pInfoItem);
	}

	m_paInfo->Copy (&aTmp);
}


/* ========================================================================= */
/* 関数名	:CLibInfoItem::GetTypeName										 */
/* 内容		:アイテム種別名を取得											 */
/* 日付		:2007/09/23														 */
/* ========================================================================= */

LPCSTR CLibInfoItem::GetTypeName(DWORD dwTypeID)
{
	LPCSTR pszRet;

	pszRet = "";

	return pszRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoItem::GetItemType										 */
/* 内容		:アイテム種別を取得												 */
/* 日付		:2007/10/07														 */
/* ========================================================================= */

DWORD CLibInfoItem::GetItemType(DWORD dwItemID)
{
	DWORD dwRet;
	PCInfoItemTypeBase pInfoItemType;

	dwRet = 0;

	if (dwItemID == 0) {
		goto Exit;
	}
	pInfoItemType = (PCInfoItemTypeBase)GetItemTypePtr (dwItemID);
	if (pInfoItemType == NULL) {
		goto Exit;
	}

	dwRet = pInfoItemType->m_dwItemTypeID;
Exit:
	return dwRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoItem::GetItemTypeID									 */
/* 内容		:アイテム種別IDを取得											 */
/* 日付		:2008/08/08														 */
/* ========================================================================= */

DWORD CLibInfoItem::GetItemTypeID(DWORD dwItemID)
{
	DWORD dwRet;
	PCInfoItemTypeBase pInfoItemType;

	dwRet = 0;

	if (dwItemID == 0) {
		goto Exit;
	}
	pInfoItemType = (PCInfoItemTypeBase)GetItemTypePtr (dwItemID);
	if (pInfoItemType == NULL) {
		goto Exit;
	}

	dwRet = pInfoItemType->m_dwTypeID;
Exit:
	return dwRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoItem::GetMotionIDAtack									 */
/* 内容		:使用可能な攻撃モーションIDを取得								 */
/* 日付		:2007/12/09														 */
/* ========================================================================= */

DWORD CLibInfoItem::GetMotionIDAtack(DWORD dwItemID)
{
	DWORD dwRet;
	PCInfoItemTypeBase pInfoItemType;
	PCInfoItemWeapon pInfoItemWeapon;

	dwRet = 0;

	if (dwItemID == 0) {
		goto Exit;
	}
	if (m_pLibInfoItemType == NULL) {
		goto Exit;
	}
	pInfoItemType = (PCInfoItemTypeBase)GetItemTypePtr (dwItemID);
	if (pInfoItemType == NULL) {
		goto Exit;
	}
	pInfoItemWeapon = (PCInfoItemWeapon)m_pLibInfoItemWeapon->GetPtr (pInfoItemType->m_dwWeaponInfoID);
	if (pInfoItemWeapon == NULL) {
		goto Exit;
	}

	dwRet = pInfoItemWeapon->m_dwMotionType;
Exit:
	return dwRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoItem::GetMotionIDBattleStand							 */
/* 内容		:戦闘モード中の立ちモーションIDを取得							 */
/* 日付		:2007/12/09														 */
/* ========================================================================= */

DWORD CLibInfoItem::GetMotionIDBattleStand(DWORD dwItemID)
{
	DWORD dwRet;
	PCInfoItemTypeBase pInfoItemType;
	PCInfoItemWeapon pInfoItemWeapon;

	dwRet = 0;

	if (dwItemID == 0) {
		goto Exit;
	}
	if (m_pLibInfoItemType == NULL) {
		goto Exit;
	}
	pInfoItemType = (PCInfoItemTypeBase)GetItemTypePtr (dwItemID);
	if (pInfoItemType == NULL) {
		goto Exit;
	}
	pInfoItemWeapon = (PCInfoItemWeapon)m_pLibInfoItemWeapon->GetPtr (pInfoItemType->m_dwWeaponInfoID);
	if (pInfoItemWeapon == NULL) {
		goto Exit;
	}

	dwRet = pInfoItemWeapon->m_dwMotionTypeStand;
Exit:
	return dwRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoItem::GetMotionIDBattleWalk							 */
/* 内容		:戦闘モード中のすり足モーションIDを取得							 */
/* 日付		:2007/12/09														 */
/* ========================================================================= */

DWORD CLibInfoItem::GetMotionIDBattleWalk(DWORD dwItemID)
{
	DWORD dwRet;
	PCInfoItemTypeBase pInfoItemType;
	PCInfoItemWeapon pInfoItemWeapon;

	dwRet = 0;

	if (dwItemID == 0) {
		goto Exit;
	}
	if (m_pLibInfoItemType == NULL) {
		goto Exit;
	}
	pInfoItemType = (PCInfoItemTypeBase)GetItemTypePtr (dwItemID);
	if (pInfoItemType == NULL) {
		goto Exit;
	}
	pInfoItemWeapon = (PCInfoItemWeapon)m_pLibInfoItemWeapon->GetPtr (pInfoItemType->m_dwWeaponInfoID);
	if (pInfoItemWeapon == NULL) {
		goto Exit;
	}

	dwRet = pInfoItemWeapon->m_dwMotionTypeWalk;
Exit:
	return dwRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoItem::GetPtr											 */
/* 内容		:アイテム情報を取得												 */
/* 日付		:2007/05/05														 */
/* ========================================================================= */

PCInfoBase CLibInfoItem::GetPtr(int nNo)
{
	return m_paInfo->GetAt (nNo);
}


/* ========================================================================= */
/* 関数名	:CLibInfoItem::GetPtr											 */
/* 内容		:アイテム情報を取得												 */
/* 日付		:2007/05/05														 */
/* ========================================================================= */

PCInfoBase CLibInfoItem::GetPtr(
	DWORD dwItemID)		/* [in] アイテムID */
{
	int i, nCount;
	PCInfoItem pRet, pInfoTmp;

	pRet = NULL;

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->GetAt (i);
		if (pInfoTmp->m_dwItemID != dwItemID) {
			continue;
		}
		pRet = pInfoTmp;
		break;
	}

	return pRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoItem::GetPtr											 */
/* 内容		:アイテム情報を取得												 */
/* 日付		:2007/05/05														 */
/* ========================================================================= */

PCInfoBase CLibInfoItem::GetPtr(
	DWORD dwMapID,			/* [in] 落ちているマップID */
	POINT *pptPos,			/* [in] 落ちている座標 */
	BOOL bPoint)/*=TRUE*/	/* [in] TRUE:完全一致の一点のみ対象にする */
{
	int i, nCount;
	PCInfoItem pRet, pInfoTmp;

	pRet = NULL;

	nCount = m_paInfo->GetSize ();
	for (i = nCount - 1; i >= 0 ; i --) {
		pInfoTmp = m_paInfo->GetAt (i);
		if (pInfoTmp->m_dwMapID != dwMapID) {
			continue;
		}
		if (bPoint) {
			if (!((pInfoTmp->m_ptPos.x == pptPos->x) && (pInfoTmp->m_ptPos.y == pptPos->y))) {
				continue;
			}
		} else {
			if (!((pInfoTmp->m_ptPos.x == pptPos->x) && (pInfoTmp->m_ptPos.y == pptPos->y))) {
				if (!((pInfoTmp->m_ptPos.x == pptPos->x) && (pInfoTmp->m_ptPos.y + 1 == pptPos->y))) {
					continue;
				}
			}
		}
		pRet = pInfoTmp;
		break;
	}

	return pRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoItem::GetItemTypePtr									 */
/* 内容		:アイテム種別情報を取得											 */
/* 日付		:2007/12/23														 */
/* ========================================================================= */

PCInfoBase CLibInfoItem::GetItemTypePtr(DWORD dwItemID)
{
	PCInfoItemTypeBase pRet;
	PCInfoItem pInfoItem;

	pRet = NULL;

	if (m_pLibInfoItemType == NULL) {
		goto Exit;
	}
	if (dwItemID == 0) {
		goto Exit;
	}
	pInfoItem = (PCInfoItem)GetPtr (dwItemID);
	if (pInfoItem == NULL) {
		goto Exit;
	}

	pRet = (PCInfoItemTypeBase)m_pLibInfoItemType->GetPtr (pInfoItem->m_dwItemTypeID);
Exit:
	return pRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoItem::GetSendDataSize									 */
/* 内容		:送信データサイズを取得											 */
/* 日付		:2007/05/05														 */
/* ========================================================================= */

DWORD CLibInfoItem::GetSendDataSize(void)
{
	int i, nCount;
	DWORD dwRet, dwSize;
	PCInfoItem pItem;

	dwRet = dwSize = 0;

	/* データ数分のサイズ */
	dwSize += sizeof (DWORD);

	nCount = GetCount ();
	for (i = 0; i < nCount; i ++) {
		pItem = (PCInfoItem)GetPtr (i);

		dwSize += pItem->GetSendDataSize ();
	}

	dwRet = dwSize;
	return dwRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoItem::GetSendData										 */
/* 内容		:送信データを取得												 */
/* 日付		:2007/05/05														 */
/* ========================================================================= */

PBYTE CLibInfoItem::GetSendData(void)
{
	int i, nCount;
	PBYTE pRet, pData, pDataTmp;
	DWORD dwCount, dwSize, dwSizeTmp, dwOffset;
	PCInfoItem pItem;

	pRet = NULL;

	dwSize		= GetSendDataSize ();
	pData		= ZeroNew (dwSize);
	dwOffset	= 0;

	/* データ数を書き込み */
	dwCount = (DWORD)GetCount ();
	CopyMemory (pData, &dwCount, sizeof (dwCount));
	dwOffset += sizeof (dwCount);

	/* アイテム情報を書き込み */
	nCount = GetCount ();
	for (i = 0; i < nCount; i ++) {
		pItem = (PCInfoItem)GetPtr (i);

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
/* 関数名	:CLibInfoItem::SetSendData										 */
/* 内容		:送信データから取り込み											 */
/* 日付		:2007/05/05														 */
/* 戻り値	:処理した後のアドレス											 */
/* ========================================================================= */

PBYTE CLibInfoItem::SetSendData(PBYTE pSrc)
{
	int i, nCount;
	DWORD dwCount;
	PBYTE pRet, pDataTmp;
	PCInfoItem pItem;

	pRet		= pSrc;
	pDataTmp	= pSrc;

	DeleteAll ();

	/* データ数を読み込み */
	CopyMemory (&dwCount, pDataTmp, sizeof (dwCount));
	nCount		= (int)dwCount;
	pDataTmp	+= sizeof (dwCount);

	for (i = 0; i < nCount; i ++) {
		pItem = (PCInfoItem)GetNew ();

		pDataTmp = pItem->SetSendData (pDataTmp);
		Add (pItem);
	}
	Sort ();

	pRet = pDataTmp;
	return pRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoItem::AddItem											 */
/* 内容		:所持アイテムを追加												 */
/* 日付		:2007/08/11														 */
/* ========================================================================= */

void CLibInfoItem::AddItem(DWORD dwCharID, DWORD dwItemID, ARRAYDWORD *padwItemID)
{
	BOOL bResult;
	POINT ptTmp;
	PCInfoItem pInfo;

	pInfo = (PCInfoItem)GetPtr (dwItemID);
	if (pInfo == NULL) {
		return;
	}

	/* 空いている場所を左上から探す */
	bResult = GetFreePos (ptTmp, padwItemID);
	if (bResult == FALSE) {
		return;
	}

	pInfo->m_dwCharID	= dwCharID;
	pInfo->m_ptBackPack	= ptTmp;
	pInfo->m_dwMapID	= 0;
	pInfo->m_ptPos.x	= 0;
	pInfo->m_ptPos.y	= 0;
	padwItemID->Add (dwItemID);
}


/* ========================================================================= */
/* 関数名	:CLibInfoItem::DeleteItem										 */
/* 内容		:所持アイテムを削除												 */
/* 日付		:2007/08/11														 */
/* ========================================================================= */

void CLibInfoItem::DeleteItem(DWORD dwItemID, CInfoCharBase *pInfoChar, BOOL bNoPos)
{
	int i, nCount;
	DWORD dwItemIDTmp;
	POINT ptTmp;
	PCInfoItem pInfo, pInfoTmp;

	pInfo = (PCInfoItem)GetPtr (dwItemID);
	if (pInfo == NULL) {
		return;
	}

	nCount = pInfoChar->m_adwItemID.GetSize ();
	for (i = 0; i < nCount; i ++) {
		dwItemIDTmp = pInfoChar->m_adwItemID[i];
		if (dwItemIDTmp == dwItemID) {
			pInfoChar->m_adwItemID.RemoveAt (i);
			break;
		}
	}
	/* 見つからなかった？ */
	if (i >= nCount) {
		return;
	}

	pInfo->m_dwCharID = 0;
	ZeroMemory (&pInfo->m_ptBackPack, sizeof (pInfo->m_ptBackPack));

	/* 配置しない？ */
	if (bNoPos) {
		pInfo->m_dwMapID = 0;
		pInfo->m_ptPos.x = pInfo->m_ptPos.y = 0;

	} else {
		pInfoChar->GetFrontPos (ptTmp);
		pInfoTmp = (PCInfoItem)GetPtr (pInfoChar->m_dwMapID, &ptTmp);

		pInfo->m_dwMapID	= pInfoChar->m_dwMapID;
		pInfo->m_ptPos		= ptTmp;
		/* アイテムが置かれている？ */
		if (pInfoTmp) {
			pInfo->m_nPosZ = pInfoTmp->m_nPosZ + 1;
		}
	}
	Sort ();
}


/* ========================================================================= */
/* 関数名	:CLibInfoItem::Equip											 */
/* 内容		:アイテム位置を装備用に入れ替え									 */
/* 日付		:2007/10/08														 */
/* ========================================================================= */

void CLibInfoItem::Equip(CInfoCharBase *pInfoChar, DWORD dwItemIDOld, DWORD dwItemIDNew)
{
	int i, nCount;
	DWORD dwItemIDTmp;
	PCInfoItem pInfoOld, pInfoNew;
	POINT ptOld, ptNew;

	pInfoOld = (PCInfoItem)GetPtr (dwItemIDOld);
	pInfoNew = (PCInfoItem)GetPtr (dwItemIDNew);

	ptOld.x = ptOld.y = 0;
	ptNew.x = ptNew.y = 0;

	if (pInfoOld) {
		ptOld = pInfoOld->m_ptBackPack;
	}
	if (pInfoNew) {
		ptNew = pInfoNew->m_ptBackPack;
	}

	if (pInfoNew) {
		nCount = pInfoChar->m_adwItemID.GetSize ();
		for (i = 0; i < nCount; i ++) {
			dwItemIDTmp = pInfoChar->m_adwItemID[i];
			if (dwItemIDTmp == dwItemIDNew) {
				pInfoChar->m_adwItemID.RemoveAt (i);
				break;
			}
		}
		pInfoNew->m_ptBackPack.x = pInfoNew->m_ptBackPack.y = 0;
	}
	if (pInfoOld) {
		/* 入れ替え？ */
		if (pInfoNew) {
			pInfoOld->m_ptBackPack = ptNew;
		} else {
			GetFreePos (pInfoOld->m_ptBackPack, &pInfoChar->m_adwItemID);
		}
		pInfoChar->m_adwItemID.Add (dwItemIDOld);
	}
}


/* ========================================================================= */
/* 関数名	:CLibInfoItem::MakeItem											 */
/* 内容		:指定場所にアイテムを作成										 */
/* 日付		:2007/10/20														 */
/* ========================================================================= */

DWORD CLibInfoItem::MakeItem(
	DWORD dwMapID,			/* [in] マップID */
	POINT *pptPos,			/* [in] 座標 */
	DWORD dwItemTypeID)		/* [in] アイテム種別ID */
{
	DWORD dwRet;
	PCInfoItemTypeBase pInfoItemType;
	PCInfoItem pInfoItem;

	dwRet = 0;

	pInfoItemType = (PCInfoItemTypeBase)m_pLibInfoItemType->GetPtr (dwItemTypeID);
	if (pInfoItemType == NULL) {
		goto Exit;
	}

	pInfoItem = (PCInfoItem)GetNew ();
	if (dwMapID > 0) {
		pInfoItem->m_dwMapID	= dwMapID;
		pInfoItem->m_ptPos		= *pptPos;
	}
	pInfoItem->m_dwItemTypeID	= dwItemTypeID;
	pInfoItem->m_bPutOn			= pInfoItemType->m_bPutOn;
	pInfoItem->m_strName		= pInfoItemType->m_strName;
	pInfoItem->m_dwGrpID		= pInfoItemType->m_dwGrpID;
	pInfoItem->m_dwIconGrpID	= pInfoItemType->m_dwIconGrpID;
	pInfoItem->m_dwDropSoundID	= pInfoItemType->m_dwDropSoundID;

	Add (pInfoItem);
	dwRet = pInfoItem->m_dwItemID;
Exit:
	return dwRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoItem::GetFreePos										 */
/* 内容		:アイテムの空き場所を取得										 */
/* 日付		:2007/10/08														 */
/* ========================================================================= */

BOOL CLibInfoItem::GetFreePos(POINT &ptDst, ARRAYDWORD *padwItemID)
{
	BOOL bRet, bResult;
	int x, y;
	POINT ptTmp;

	bRet = FALSE;

	/* 空いている場所を左上から探す */
	for (y = 0; y < 5; y ++) {
		for (x = 0; x < 5; x ++) {
			ptTmp.x = x;
			ptTmp.y = y;
			bResult = IsItemPos (&ptTmp, padwItemID);
			if (bResult == FALSE) {
				x = y = 4;
				break;
			}
		}
	}
	/* 空きがなかった？ */
	if (bResult) {
		goto Exit;
	}
	ptDst = ptTmp;

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoItem::IsUseItem										 */
/* 内容		:使用できるアイテムか判定										 */
/* 日付		:2008/07/05														 */
/* ========================================================================= */

BOOL CLibInfoItem::IsUseItem(DWORD dwItemID)
{
	BOOL bRet;
	PCInfoItem pInfoItem;
	PCInfoItemTypeBase pInfoItemType;

	bRet = FALSE;

	pInfoItem = (PCInfoItem)GetPtr (dwItemID);
	if (pInfoItem == NULL) {
		goto Exit;
	}
	pInfoItemType = (PCInfoItemTypeBase)GetItemTypePtr (dwItemID);
	if (pInfoItemType == NULL) {
		goto Exit;
	}

	switch (pInfoItemType->m_dwItemTypeID) {
	case ITEMTYPEID_HP:		/* HP増減 */
	case ITEMTYPEID_LIGHT:	/* 灯り*/
		break;
	default:
		goto Exit;
	}

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoItem::SetArea											 */
/* 内容		:範囲指定一覧作成												 */
/* 日付		:2008/06/14														 */
/* ========================================================================= */

void CLibInfoItem::SetArea(DWORD dwMapID, RECT *prcArea)
{
	int i, nCount;
	PCInfoItem pInfoTmp;

	m_adwAreaID.RemoveAll ();

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->GetAt (i);
		if (pInfoTmp->m_dwMapID != dwMapID) {
			continue;
		}
		if (!((pInfoTmp->m_ptPos.x >= prcArea->left) && (pInfoTmp->m_ptPos.x <= prcArea->right) &&
			(pInfoTmp->m_ptPos.y >= prcArea->top) && (pInfoTmp->m_ptPos.y <= prcArea->bottom))) {
			continue;
		}
		m_adwAreaID.Add (pInfoTmp->m_dwItemID);
	}
}


/* ========================================================================= */
/* 関数名	:CLibInfoItem::GetAreaCount										 */
/* 内容		:範囲指定一覧数取得												 */
/* 日付		:2008/06/14														 */
/* ========================================================================= */

int CLibInfoItem::GetAreaCount(void)
{
	return m_adwAreaID.GetSize ();
}


/* ========================================================================= */
/* 関数名	:CLibInfoItem::GetPtrArea										 */
/* 内容		:範囲指定一覧からアイテム情報を取得								 */
/* 日付		:2008/06/14														 */
/* ========================================================================= */

PCInfoBase CLibInfoItem::GetPtrArea(int nNo)
{
	return GetPtr (m_adwAreaID[nNo]);
}


/* ========================================================================= */
/* 関数名	:CLibInfoItem::GetNewID											 */
/* 内容		:新しいアイテムIDを取得											 */
/* 日付		:2007/05/05														 */
/* ========================================================================= */

DWORD CLibInfoItem::GetNewID(void)
{
	DWORD dwRet;
	int i, nCount;
	PCInfoItem pInfoTmp;

	dwRet = m_dwNewIDTmp + 1;
	if (dwRet == 0) {
		dwRet = 1;
	}

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->GetAt (i);
		if (pInfoTmp->m_dwItemID == dwRet) {
			dwRet ++;
			i = -1;
			continue;
		}
	}
	m_dwNewIDTmp = dwRet;

	return dwRet;
}


/* ========================================================================= */
/* 関数名	:CLibInfoItem::IsItemPos										 */
/* 内容		:指定場所が使用済みかチェック									 */
/* 戻り値	:TRUE:使用済み													 */
/* 日付		:2007/08/12														 */
/* ========================================================================= */

BOOL CLibInfoItem::IsItemPos(POINT *ptItem, ARRAYDWORD *padwItemID)
{
	BOOL bRet;
	int i, nCount;
	PCInfoItem pInfoTmp;

	bRet = TRUE;

	nCount = padwItemID->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = (PCInfoItem)GetPtr (padwItemID->GetAt (i));
		if (pInfoTmp == NULL) {
			continue;
		}
		if ((pInfoTmp->m_ptBackPack.x == ptItem->x) &&
			(pInfoTmp->m_ptBackPack.y == ptItem->y)) {
			goto Exit;
		}
	}

	bRet = FALSE;
Exit:
	return bRet;
}

/* Copyright(C)URARA-works 2007 */
