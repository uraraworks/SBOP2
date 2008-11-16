/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:LibInfoItem.cpp											 */
/* ���e			:�A�C�e����񃉃C�u�������N���X �����t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/05/05													 */
/* ========================================================================= */

#include "stdafx.h"
#include "InfoCharBase.h"
#include "LibInfoItemType.h"
#include "LibInfoItemWeapon.h"
#include "LibInfoMapParts.h"
#include "LibInfoItem.h"


/* ========================================================================= */
/* �֐���	:CLibInfoItem::CLibInfoItem										 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/05/05														 */
/* ========================================================================= */

CLibInfoItem::CLibInfoItem()
{
	m_dwNewIDTmp			= 0;
	m_paInfo				= NULL;
	m_pLibInfoItemType		= NULL;
	m_pLibInfoItemWeapon	= NULL;
}


/* ========================================================================= */
/* �֐���	:CLibInfoItem::~CLibInfoItem									 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/05/05														 */
/* ========================================================================= */

CLibInfoItem::~CLibInfoItem()
{
	Destroy ();
}


/* ========================================================================= */
/* �֐���	:CLibInfoItem::Create											 */
/* ���e		:�쐬															 */
/* ���t		:2007/05/05														 */
/* ========================================================================= */

void CLibInfoItem::Create(void)
{
	m_paInfo = new ARRAYITEMINFO;
}


/* ========================================================================= */
/* �֐���	:CLibInfoItem::Destroy											 */
/* ���e		:�j��															 */
/* ���t		:2007/05/05														 */
/* ========================================================================= */

void CLibInfoItem::Destroy(void)
{
	DeleteAll ();
	SAFE_DELETE (m_paInfo);
}


/* ========================================================================= */
/* �֐���	:CLibInfoItem::SetTypeInfo										 */
/* ���e		:�A�C�e����ʏ���ݒ�											 */
/* ���t		:2007/10/06														 */
/* ========================================================================= */

void CLibInfoItem::SetTypeInfo(CLibInfoItemType *pLibInfoItemType)
{
	m_pLibInfoItemType = pLibInfoItemType;
}


/* ========================================================================= */
/* �֐���	:CLibInfoItem::SetWeaponInfo									 */
/* ���e		:�������ݒ�													 */
/* ���t		:2007/10/06														 */
/* ========================================================================= */

void CLibInfoItem::SetWeaponInfo(CLibInfoItemWeapon *pLibInfoItemWeapon)
{
	m_pLibInfoItemWeapon = pLibInfoItemWeapon;
}


/* ========================================================================= */
/* �֐���	:CLibInfoItem::RenewSize										 */
/* ���e		:�}�b�v�T�C�Y�X�V												 */
/* ���t		:2007/08/25														 */
/* ========================================================================= */

void CLibInfoItem::RenewSize(DWORD dwMapID, int nDirection, int nSize)
{
	int i, nCount;
	PCInfoItem pInfoItem;

	/* �}�b�v�T�C�Y���L�����T�C�Y�ɕύX */
	nSize *= 2;

	nCount = GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfoItem = (PCInfoItem)GetPtr (i);

		if (pInfoItem->m_dwMapID != dwMapID) {
			continue;
		}

		switch (nDirection) {
		case 0:		/* �� */
			pInfoItem->m_ptPos.y += nSize;
			pInfoItem->m_ptPos.y = max (pInfoItem->m_ptPos.y, 0);
			break;
		case 1:		/* �� */
			break;
		case 2:		/* �� */
			pInfoItem->m_ptPos.x += nSize;
			pInfoItem->m_ptPos.x = max (pInfoItem->m_ptPos.x, 0);
			break;
		case 3:		/* �E */
			break;
		}
	}
}


/* ========================================================================= */
/* �֐���	:CLibInfoItem::GetNew											 */
/* ���e		:�V�K�f�[�^���擾												 */
/* ���t		:2007/05/05														 */
/* ========================================================================= */

PCInfoBase CLibInfoItem::GetNew(void)
{
	PCInfoItem pInfoItem;

	pInfoItem = new CInfoItem;

	return pInfoItem;
}


/* ========================================================================= */
/* �֐���	:CLibInfoItem::GetCount											 */
/* ���e		:�f�[�^�����擾													 */
/* ���t		:2007/05/05														 */
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
/* �֐���	:CLibInfoItem::Add												 */
/* ���e		:�ǉ�															 */
/* ���t		:2007/05/05														 */
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
/* �֐���	:CLibInfoItem::Delete											 */
/* ���e		:�폜															 */
/* ���t		:2007/05/05														 */
/* ========================================================================= */

void CLibInfoItem::Delete(
	int nNo)		/* [in] �z��ԍ� */
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
/* �֐���	:CLibInfoItem::Delete											 */
/* ���e		:�폜															 */
/* ���t		:2007/05/05														 */
/* ========================================================================= */

void CLibInfoItem::Delete(
	DWORD dwItemID)		/* [in] �A�C�e��ID */
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
/* �֐���	:CLibInfoItem::DeleteAll										 */
/* ���e		:�S�č폜														 */
/* ���t		:2007/05/05														 */
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
/* �֐���	:CLibInfoItem::Sort												 */
/* ���e		:�\���ʒu�ɍ��킹�ă\�[�g										 */
/* ���t		:2008/06/03														 */
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
/* �֐���	:CLibInfoItem::GetTypeName										 */
/* ���e		:�A�C�e����ʖ����擾											 */
/* ���t		:2007/09/23														 */
/* ========================================================================= */

LPCSTR CLibInfoItem::GetTypeName(DWORD dwTypeID)
{
	LPCSTR pszRet;

	pszRet = "";

	return pszRet;
}


/* ========================================================================= */
/* �֐���	:CLibInfoItem::GetItemType										 */
/* ���e		:�A�C�e����ʂ��擾												 */
/* ���t		:2007/10/07														 */
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
/* �֐���	:CLibInfoItem::GetItemTypeID									 */
/* ���e		:�A�C�e�����ID���擾											 */
/* ���t		:2008/08/08														 */
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
/* �֐���	:CLibInfoItem::GetMotionIDAtack									 */
/* ���e		:�g�p�\�ȍU�����[�V����ID���擾								 */
/* ���t		:2007/12/09														 */
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
/* �֐���	:CLibInfoItem::GetMotionIDBattleStand							 */
/* ���e		:�퓬���[�h���̗������[�V����ID���擾							 */
/* ���t		:2007/12/09														 */
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
/* �֐���	:CLibInfoItem::GetMotionIDBattleWalk							 */
/* ���e		:�퓬���[�h���̂��葫���[�V����ID���擾							 */
/* ���t		:2007/12/09														 */
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
/* �֐���	:CLibInfoItem::GetPtr											 */
/* ���e		:�A�C�e�������擾												 */
/* ���t		:2007/05/05														 */
/* ========================================================================= */

PCInfoBase CLibInfoItem::GetPtr(int nNo)
{
	return m_paInfo->GetAt (nNo);
}


/* ========================================================================= */
/* �֐���	:CLibInfoItem::GetPtr											 */
/* ���e		:�A�C�e�������擾												 */
/* ���t		:2007/05/05														 */
/* ========================================================================= */

PCInfoBase CLibInfoItem::GetPtr(
	DWORD dwItemID)		/* [in] �A�C�e��ID */
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
/* �֐���	:CLibInfoItem::GetPtr											 */
/* ���e		:�A�C�e�������擾												 */
/* ���t		:2007/05/05														 */
/* ========================================================================= */

PCInfoBase CLibInfoItem::GetPtr(
	DWORD dwMapID,			/* [in] �����Ă���}�b�vID */
	POINT *pptPos,			/* [in] �����Ă�����W */
	BOOL bPoint)/*=TRUE*/	/* [in] TRUE:���S��v�̈�_�̂ݑΏۂɂ��� */
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
/* �֐���	:CLibInfoItem::GetItemTypePtr									 */
/* ���e		:�A�C�e����ʏ����擾											 */
/* ���t		:2007/12/23														 */
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
/* �֐���	:CLibInfoItem::GetSendDataSize									 */
/* ���e		:���M�f�[�^�T�C�Y���擾											 */
/* ���t		:2007/05/05														 */
/* ========================================================================= */

DWORD CLibInfoItem::GetSendDataSize(void)
{
	int i, nCount;
	DWORD dwRet, dwSize;
	PCInfoItem pItem;

	dwRet = dwSize = 0;

	/* �f�[�^�����̃T�C�Y */
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
/* �֐���	:CLibInfoItem::GetSendData										 */
/* ���e		:���M�f�[�^���擾												 */
/* ���t		:2007/05/05														 */
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

	/* �f�[�^������������ */
	dwCount = (DWORD)GetCount ();
	CopyMemory (pData, &dwCount, sizeof (dwCount));
	dwOffset += sizeof (dwCount);

	/* �A�C�e�������������� */
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
/* �֐���	:CLibInfoItem::SetSendData										 */
/* ���e		:���M�f�[�^�����荞��											 */
/* ���t		:2007/05/05														 */
/* �߂�l	:����������̃A�h���X											 */
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

	/* �f�[�^����ǂݍ��� */
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
/* �֐���	:CLibInfoItem::AddItem											 */
/* ���e		:�����A�C�e����ǉ�												 */
/* ���t		:2007/08/11														 */
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

	/* �󂢂Ă���ꏊ�����ォ��T�� */
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
/* �֐���	:CLibInfoItem::DeleteItem										 */
/* ���e		:�����A�C�e�����폜												 */
/* ���t		:2007/08/11														 */
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
	/* ������Ȃ������H */
	if (i >= nCount) {
		return;
	}

	pInfo->m_dwCharID = 0;
	ZeroMemory (&pInfo->m_ptBackPack, sizeof (pInfo->m_ptBackPack));

	/* �z�u���Ȃ��H */
	if (bNoPos) {
		pInfo->m_dwMapID = 0;
		pInfo->m_ptPos.x = pInfo->m_ptPos.y = 0;

	} else {
		pInfoChar->GetFrontPos (ptTmp);
		pInfoTmp = (PCInfoItem)GetPtr (pInfoChar->m_dwMapID, &ptTmp);

		pInfo->m_dwMapID	= pInfoChar->m_dwMapID;
		pInfo->m_ptPos		= ptTmp;
		/* �A�C�e�����u����Ă���H */
		if (pInfoTmp) {
			pInfo->m_nPosZ = pInfoTmp->m_nPosZ + 1;
		}
	}
	Sort ();
}


/* ========================================================================= */
/* �֐���	:CLibInfoItem::Equip											 */
/* ���e		:�A�C�e���ʒu�𑕔��p�ɓ���ւ�									 */
/* ���t		:2007/10/08														 */
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
		/* ����ւ��H */
		if (pInfoNew) {
			pInfoOld->m_ptBackPack = ptNew;
		} else {
			GetFreePos (pInfoOld->m_ptBackPack, &pInfoChar->m_adwItemID);
		}
		pInfoChar->m_adwItemID.Add (dwItemIDOld);
	}
}


/* ========================================================================= */
/* �֐���	:CLibInfoItem::MakeItem											 */
/* ���e		:�w��ꏊ�ɃA�C�e�����쐬										 */
/* ���t		:2007/10/20														 */
/* ========================================================================= */

DWORD CLibInfoItem::MakeItem(
	DWORD dwMapID,			/* [in] �}�b�vID */
	POINT *pptPos,			/* [in] ���W */
	DWORD dwItemTypeID)		/* [in] �A�C�e�����ID */
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
/* �֐���	:CLibInfoItem::GetFreePos										 */
/* ���e		:�A�C�e���̋󂫏ꏊ���擾										 */
/* ���t		:2007/10/08														 */
/* ========================================================================= */

BOOL CLibInfoItem::GetFreePos(POINT &ptDst, ARRAYDWORD *padwItemID)
{
	BOOL bRet, bResult;
	int x, y;
	POINT ptTmp;

	bRet = FALSE;

	/* �󂢂Ă���ꏊ�����ォ��T�� */
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
	/* �󂫂��Ȃ������H */
	if (bResult) {
		goto Exit;
	}
	ptDst = ptTmp;

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CLibInfoItem::IsUseItem										 */
/* ���e		:�g�p�ł���A�C�e��������										 */
/* ���t		:2008/07/05														 */
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
	case ITEMTYPEID_HP:		/* HP���� */
	case ITEMTYPEID_LIGHT:	/* ����*/
		break;
	default:
		goto Exit;
	}

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CLibInfoItem::SetArea											 */
/* ���e		:�͈͎w��ꗗ�쐬												 */
/* ���t		:2008/06/14														 */
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
/* �֐���	:CLibInfoItem::GetAreaCount										 */
/* ���e		:�͈͎w��ꗗ���擾												 */
/* ���t		:2008/06/14														 */
/* ========================================================================= */

int CLibInfoItem::GetAreaCount(void)
{
	return m_adwAreaID.GetSize ();
}


/* ========================================================================= */
/* �֐���	:CLibInfoItem::GetPtrArea										 */
/* ���e		:�͈͎w��ꗗ����A�C�e�������擾								 */
/* ���t		:2008/06/14														 */
/* ========================================================================= */

PCInfoBase CLibInfoItem::GetPtrArea(int nNo)
{
	return GetPtr (m_adwAreaID[nNo]);
}


/* ========================================================================= */
/* �֐���	:CLibInfoItem::GetNewID											 */
/* ���e		:�V�����A�C�e��ID���擾											 */
/* ���t		:2007/05/05														 */
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
/* �֐���	:CLibInfoItem::IsItemPos										 */
/* ���e		:�w��ꏊ���g�p�ς݂��`�F�b�N									 */
/* �߂�l	:TRUE:�g�p�ς�													 */
/* ���t		:2007/08/12														 */
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
