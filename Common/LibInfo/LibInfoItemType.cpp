/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:LibInfoItemType.cpp										 */
/* ���e			:�A�C�e����ʏ�񃉃C�u�������N���X �����t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/09/27													 */
/* ========================================================================= */

#include "stdafx.h"
#include "LibInfoItemType.h"


/* ========================================================================= */
/* �֐���	:CLibInfoItemType::CLibInfoItemType								 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/09/27														 */
/* ========================================================================= */

CLibInfoItemType::CLibInfoItemType()
{
	m_dwNewIDTmp	= 0;
	m_paInfo		= NULL;
}


/* ========================================================================= */
/* �֐���	:CLibInfoItemType::~CLibInfoItemType							 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/09/27														 */
/* ========================================================================= */

CLibInfoItemType::~CLibInfoItemType()
{
	Destroy ();
}


/* ========================================================================= */
/* �֐���	:CLibInfoItemType::Create										 */
/* ���e		:�쐬															 */
/* ���t		:2007/09/27														 */
/* ========================================================================= */

void CLibInfoItemType::Create(void)
{
	m_paInfo = new ARRAYITEMTYPEINFO;
}


/* ========================================================================= */
/* �֐���	:CLibInfoItemType::Destroy										 */
/* ���e		:�j��															 */
/* ���t		:2007/09/27														 */
/* ========================================================================= */

void CLibInfoItemType::Destroy(void)
{
	DeleteAll ();
	SAFE_DELETE (m_paInfo);
}


/* ========================================================================= */
/* �֐���	:CLibInfoItemType::GetNew										 */
/* ���e		:�V�K�f�[�^���擾												 */
/* ���t		:2007/09/27														 */
/* ========================================================================= */

PCInfoBase CLibInfoItemType::GetNew(void)
{
	return (PCInfoBase)GetNew (ITEMTYPEID_NONE);
}


/* ========================================================================= */
/* �֐���	:CLibInfoItemType::GetNew										 */
/* ���e		:�V�K�f�[�^���擾												 */
/* ���t		:2007/12/08														 */
/* ========================================================================= */

PCInfoBase CLibInfoItemType::GetNew(DWORD dwItemTypeID)
{
#if 0
	PCInfoBase pNew;

	switch (dwItemTypeID) {
	case ITEMTYPEID_ARMS:	pNew = (PCInfoBase)new CInfoItemTypeARMS;	break;	/* ������ */
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
/* �֐���	:CLibInfoCharSvr::Revice										 */
/* ���e		:�f�[�^�̕␳													 */
/* ���t		:2007/09/01														 */
/* ========================================================================= */

void CLibInfoItemType::Revice(void)
{
	int i, nCount;
	PCInfoItemTypeBase pInfoTmp, pInfo;

	nCount = GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = (PCInfoItemTypeBase)GetPtr (i);

		/* �ړ���ʂɉ����ăN���X���X�V */
		pInfo = (PCInfoItemTypeBase)GetNew (pInfoTmp->m_dwItemTypeID);
		pInfo->Copy (pInfoTmp);
	}
}


/* ========================================================================= */
/* �֐���	:CLibInfoItemType::GetCount										 */
/* ���e		:�f�[�^�����擾													 */
/* ���t		:2007/09/27														 */
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
/* �֐���	:CLibInfoItemType::Add											 */
/* ���e		:�ǉ�															 */
/* ���t		:2007/09/27														 */
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
/* �֐���	:CLibInfoItemType::Delete										 */
/* ���e		:�폜															 */
/* ���t		:2007/09/27														 */
/* ========================================================================= */

void CLibInfoItemType::Delete(
	int nNo)		/* [in] �z��ԍ� */
{
	PCInfoItemTypeBase pInfo;

	pInfo = m_paInfo->GetAt (nNo);
	SAFE_DELETE (pInfo);
	m_paInfo->RemoveAt (nNo);
}


/* ========================================================================= */
/* �֐���	:CLibInfoItemType::Delete										 */
/* ���e		:�폜															 */
/* ���t		:2007/09/27														 */
/* ========================================================================= */

void CLibInfoItemType::Delete(
	DWORD m_dwTypeID)		/* [in] ���ID */
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
/* �֐���	:CLibInfoItemType::DeleteAll									 */
/* ���e		:�S�č폜														 */
/* ���t		:2007/09/27														 */
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
/* �֐���	:CLibInfoItemType::GetGrpIDMainCount							 */
/* ���e		:�A�C�e����ʂ̉摜ID���C�������擾								 */
/* ���t		:2007/10/07														 */
/* ========================================================================= */

int CLibInfoItemType::GetGrpIDMainCount(DWORD dwTypeID)
{
	int nRet;

	nRet = 1;

	switch (dwTypeID) {
	case ITEMTYPEID_CLOTH:		nRet = ITEMGRPIDMAIN_CLOTH_MAX;		break;	/* �� */
	case ITEMTYPEID_ARMS:		nRet = ITEMGRPIDMAIN_ARMS_MAX;		break;	/* ������ */
	}

	return nRet;
}


/* ========================================================================= */
/* �֐���	:CLibInfoItemType::GetTypeName									 */
/* ���e		:�A�C�e����ʖ����擾											 */
/* ���t		:2007/09/23														 */
/* ========================================================================= */

LPCSTR CLibInfoItemType::GetTypeName(DWORD dwItemTypeID)
{
	LPCSTR pszRet;

	pszRet = "";

	switch (dwItemTypeID) {
	case ITEMTYPEID_NONE:		pszRet = "���ʖ���";		break;	/* ���ʖ��� */
	case ITEMTYPEID_CLOTH:		pszRet = "��";				break;	/* �� */
	case ITEMTYPEID_ACCE:		pszRet = "�A�N�Z�T��";		break;	/* �A�N�Z�T�� */
	case ITEMTYPEID_ARMS:		pszRet = "������";			break;	/* ������ */
	case ITEMTYPEID_SHIELD:		pszRet = "��";				break;	/* �� */
	case ITEMTYPEID_HP:			pszRet = "HP����";			break;	/* HP���� */
	case ITEMTYPEID_LIGHT:		pszRet = "����";			break;	/* ���� */
	}

	return pszRet;
}


/* ========================================================================= */
/* �֐���	:CLibInfoItemType::GetGrpIDMainName								 */
/* ���e		:�摜ID���C����ʖ����擾										 */
/* ���t		:2007/10/07														 */
/* ========================================================================= */

LPCSTR CLibInfoItemType::GetGrpIDMainName(
	DWORD dwTypeID,			/* [in] �A�C�e����� */ 
	WORD wGrpIDMain)		/* [in] �摜ID���C�� */
{
	LPCSTR pszRet;

	pszRet = "��ʂ̊�{�摜";

	switch (dwTypeID) {
	case ITEMTYPEID_CLOTH:	/* �� */
		switch (wGrpIDMain) {
		case ITEMGRPIDMAIN_CLOTH_NONE:	pszRet = "��ʂ̊�{�摜";	break;	/* ���ݒ� */
		case ITEMGRPIDMAIN_CLOTH_SP:	pszRet = "���ꕞ";			break;	/* ���ꕞ */
		}
		break;
	case ITEMTYPEID_ARMS:	/* ������ */
		switch (wGrpIDMain) {
		case ITEMGRPIDMAIN_ARMS_NONE:		pszRet = "��ʂ̊�{�摜";	break;	/* ���ݒ� */
		case ITEMGRPIDMAIN_ARMS_BOW:		pszRet = "�|";				break;	/* �| */
		case ITEMGRPIDMAIN_WEAPON_GLOVE:	pszRet = "����(�Ō�)";		break;	/* ����(�Ō�) */
		case ITEMGRPIDMAIN_WEAPON_ETC:		pszRet = "����(���̑�)";	break;	/* ����(���̑�) */
		case ITEMGRPIDMAIN_2X2_ARMS:		pszRet = "������(2x2)";		break;	/* ������(2x2) */
		case ITEMGRPIDMAIN_2X2_SHIELD:		pszRet = "��(2x2)";			break;	/* ��(2x2) */
		case ITEMGRPIDMAIN_2X2_ARMSSP:		pszRet = "���ꎝ����(2x2)";	break;	/* ���ꎝ����(2x2) */
		case ITEMGRPIDMAIN_2X2_BOW:			pszRet = "�|(2x2)";			break;	/* �|(2x2) */
		}
		break;
	}

	return pszRet;
}


/* ========================================================================= */
/* �֐���	:CLibInfoItemType::GetGrpIDMain									 */
/* ���e		:�摜ID���C�����擾												 */
/* ���t		:2007/12/23														 */
/* ========================================================================= */

DWORD CLibInfoItemType::GetGrpIDMain(
	DWORD dwTypeID,			/* [in] �A�C�e����� */ 
	WORD wGrpIDMain)		/* [in] �摜ID���C�� */
{
	return 0;
}


/* ========================================================================= */
/* �֐���	:CLibInfoItemType::GetTypeNameTypeID							 */
/* ���e		:�A�C�e����ʖ����擾											 */
/* ���t		:2007/10/06														 */
/* ========================================================================= */

LPCSTR CLibInfoItemType::GetTypeNameTypeID(
	DWORD dwTypeID)		/* [in] ���ID */
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
/* �֐���	:CLibInfoItemType::GetPtr										 */
/* ���e		:�A�C�e�������擾												 */
/* ���t		:2007/09/27														 */
/* ========================================================================= */

PCInfoBase CLibInfoItemType::GetPtr(int nNo)
{
	return m_paInfo->GetAt (nNo);
}


/* ========================================================================= */
/* �֐���	:CLibInfoItemType::GetPtr										 */
/* ���e		:�A�C�e�������擾												 */
/* ���t		:2007/09/27														 */
/* ========================================================================= */

PCInfoBase CLibInfoItemType::GetPtr(
	DWORD dwTypeID)		/* [in] ���ID */
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
/* �֐���	:CLibInfoItemType::GetSendDataSize								 */
/* ���e		:���M�f�[�^�T�C�Y���擾											 */
/* ���t		:2007/09/27														 */
/* ========================================================================= */

DWORD CLibInfoItemType::GetSendDataSize(void)
{
	int i, nCount;
	DWORD dwRet, dwSize;
	PCInfoItemTypeBase pItem;

	dwRet = dwSize = 0;

	/* �f�[�^�����̃T�C�Y */
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
/* �֐���	:CLibInfoItemType::GetSendData									 */
/* ���e		:���M�f�[�^���擾												 */
/* ���t		:2007/09/27														 */
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

	/* �f�[�^������������ */
	dwCount = (DWORD)GetCount ();
	CopyMemory (pData, &dwCount, sizeof (dwCount));
	dwOffset += sizeof (dwCount);

	/* �L���������������� */
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
/* �֐���	:CLibInfoItemType::SetSendData									 */
/* ���e		:���M�f�[�^�����荞��											 */
/* ���t		:2007/09/27														 */
/* �߂�l	:����������̃A�h���X											 */
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

	/* �f�[�^����ǂݍ��� */
	CopyMemoryRenew (&dwCount, pDataTmp, sizeof (dwCount), pDataTmp);
	nCount = (int)dwCount;

	for (i = 0; i < nCount; i ++) {
		/* �܂��͊��N���X�֎�荞�� */
		ItemTmp.SetSendData (pDataTmp);
		pItem = (PCInfoItemTypeBase)GetNew (ItemTmp.m_dwItemTypeID);

		pDataTmp = pItem->SetSendData (pDataTmp);
		Add (pItem);
	}

	pRet = pDataTmp;
	return pRet;
}


/* ========================================================================= */
/* �֐���	:CLibInfoItemType::GetNewSendData								 */
/* ���e		:���M�f�[�^����V�K�f�[�^���擾									 */
/* ���t		:2007/12/08														 */
/* �߂�l	:����������̃A�h���X											 */
/* ========================================================================= */

PBYTE CLibInfoItemType::GetNewSendData(PBYTE pSrc, PCInfoItemTypeBase &pDst)
{
	PBYTE pRet;
	CInfoItemTypeBase InfoTmp;

	SAFE_DELETE (pDst);

	/* �܂��͊��N���X�֎�荞�� */
	InfoTmp.SetSendData (pSrc);
	pDst = (PCInfoItemTypeBase)GetNew (InfoTmp.m_dwItemTypeID);
	pRet = pDst->SetSendData (pSrc);

	return pRet;
}


/* ========================================================================= */
/* �֐���	:CLibInfoItemType::GetNewID										 */
/* ���e		:�V�����A�C�e�����ID���擾										 */
/* ���t		:2007/09/27														 */
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
