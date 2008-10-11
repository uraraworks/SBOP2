/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:LibInfoItemWeapon.cpp										 */
/* ���e			:�A�C�e�������񃉃C�u�����N���X �����t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/08/10													 */
/* ========================================================================= */

#include "stdafx.h"
#include "LibInfoItemWeapon.h"


/* ========================================================================= */
/* �֐���	:CLibInfoItemWeapon::CLibInfoItemWeapon							 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/08/10														 */
/* ========================================================================= */

CLibInfoItemWeapon::CLibInfoItemWeapon()
{
	m_dwNewIDTmp	= 0;
	m_paInfo		= NULL;
}


/* ========================================================================= */
/* �֐���	:CLibInfoItemWeapon::~CLibInfoItemWeapon						 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/08/10														 */
/* ========================================================================= */

CLibInfoItemWeapon::~CLibInfoItemWeapon()
{
	Destroy ();
}


/* ========================================================================= */
/* �֐���	:CLibInfoItemWeapon::Create										 */
/* ���e		:�쐬															 */
/* ���t		:2008/08/10														 */
/* ========================================================================= */

void CLibInfoItemWeapon::Create(void)
{
	m_paInfo = new ARRAYWEAPONINFO;
}


/* ========================================================================= */
/* �֐���	:CLibInfoItemWeapon::Destroy									 */
/* ���e		:�j��															 */
/* ���t		:2008/08/10														 */
/* ========================================================================= */

void CLibInfoItemWeapon::Destroy(void)
{
	DeleteAll ();
	SAFE_DELETE (m_paInfo);
}


/* ========================================================================= */
/* �֐���	:CLibInfoItemWeapon::GetNew										 */
/* ���e		:�V�K�f�[�^���擾												 */
/* ���t		:2008/08/10														 */
/* ========================================================================= */

PCInfoBase CLibInfoItemWeapon::GetNew(void)
{
	PCInfoItemWeapon pTmp;

	pTmp = new CInfoItemWeapon;

	return (PCInfoBase)pTmp;
}


/* ========================================================================= */
/* �֐���	:CLibInfoItemWeapon::GetCount									 */
/* ���e		:�f�[�^�����擾													 */
/* ���t		:2008/08/10														 */
/* ========================================================================= */

int CLibInfoItemWeapon::GetCount(void)
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
/* �֐���	:CLibInfoItemWeapon::Add										 */
/* ���e		:�ǉ�															 */
/* ���t		:2008/08/10														 */
/* ========================================================================= */

void CLibInfoItemWeapon::Add(PCInfoBase pInfo)
{
	PCInfoItemWeapon pItemInfo;

	pItemInfo = (PCInfoItemWeapon)pInfo;
	if (pItemInfo->m_dwWeaponInfoID == 0) {
		pItemInfo->m_dwWeaponInfoID = GetNewID ();
	}

	m_paInfo->Add (pItemInfo);
}


/* ========================================================================= */
/* �֐���	:CLibInfoItemWeapon::Delete										 */
/* ���e		:�폜															 */
/* ���t		:2008/08/10														 */
/* ========================================================================= */

void CLibInfoItemWeapon::Delete(
	int nNo)		/* [in] �z��ԍ� */
{
	PCInfoItemWeapon pInfo;

	pInfo = m_paInfo->GetAt (nNo);
	SAFE_DELETE (pInfo);
	m_paInfo->RemoveAt (nNo);
}


/* ========================================================================= */
/* �֐���	:CLibInfoItemWeapon::Delete										 */
/* ���e		:�폜															 */
/* ���t		:2008/08/10														 */
/* ========================================================================= */

void CLibInfoItemWeapon::Delete(
	DWORD dwWeaponInfoID)		/* [in] ������ID */
{
	int i, nCount, nNo;
	PCInfoItemWeapon pInfoTmp;

	nNo = -1;

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->GetAt (i);
		if (pInfoTmp->m_dwWeaponInfoID != dwWeaponInfoID) {
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
/* �֐���	:CLibInfoItemWeapon::DeleteAll									 */
/* ���e		:�S�č폜														 */
/* ���t		:2008/08/10														 */
/* ========================================================================= */

void CLibInfoItemWeapon::DeleteAll(void)
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
/* �֐���	:CLibInfoItemWeapon::GetPtr										 */
/* ���e		:�A�C�e�������擾												 */
/* ���t		:2008/08/10														 */
/* ========================================================================= */

PCInfoBase CLibInfoItemWeapon::GetPtr(int nNo)
{
	return m_paInfo->GetAt (nNo);
}


/* ========================================================================= */
/* �֐���	:CLibInfoItemWeapon::GetPtr										 */
/* ���e		:�A�C�e�������擾												 */
/* ���t		:2008/08/10														 */
/* ========================================================================= */

PCInfoBase CLibInfoItemWeapon::GetPtr(
	DWORD dwWeaponInfoID)		/* [in] ������ID */
{
	int i, nCount;
	PCInfoItemWeapon pRet, pInfoTmp;

	pRet = NULL;

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->GetAt (i);
		if (pInfoTmp->m_dwWeaponInfoID != dwWeaponInfoID) {
			continue;
		}
		pRet = pInfoTmp;
		break;
	}

	return pRet;
}


/* ========================================================================= */
/* �֐���	:CLibInfoItemWeapon::GetSendDataSize							 */
/* ���e		:���M�f�[�^�T�C�Y���擾											 */
/* ���t		:2008/08/10														 */
/* ========================================================================= */

DWORD CLibInfoItemWeapon::GetSendDataSize(void)
{
	int i, nCount;
	DWORD dwRet, dwSize;
	PCInfoItemWeapon pItem;

	dwRet = dwSize = 0;

	/* �f�[�^�����̃T�C�Y */
	dwSize += sizeof (DWORD);

	nCount = GetCount ();
	for (i = 0; i < nCount; i ++) {
		pItem = (PCInfoItemWeapon)GetPtr (i);

		dwSize += pItem->GetSendDataSize ();
	}

	dwRet = dwSize;
	return dwRet;
}


/* ========================================================================= */
/* �֐���	:CLibInfoItemWeapon::GetSendData								 */
/* ���e		:���M�f�[�^���擾												 */
/* ���t		:2008/08/10														 */
/* ========================================================================= */

PBYTE CLibInfoItemWeapon::GetSendData(void)
{
	int i, nCount;
	PBYTE pRet, pData, pDataTmp;
	DWORD dwCount, dwSize, dwSizeTmp, dwOffset;
	PCInfoItemWeapon pItem;

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
		pItem = (PCInfoItemWeapon)GetPtr (i);

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
/* �֐���	:CLibInfoItemWeapon::SetSendData								 */
/* ���e		:���M�f�[�^�����荞��											 */
/* ���t		:2008/08/10														 */
/* �߂�l	:����������̃A�h���X											 */
/* ========================================================================= */

PBYTE CLibInfoItemWeapon::SetSendData(PBYTE pSrc)
{
	int i, nCount;
	DWORD dwCount;
	PBYTE pRet, pDataTmp;
	CInfoItemWeapon *pItem;

	pRet		= pSrc;
	pDataTmp	= pSrc;

	DeleteAll ();

	/* �f�[�^����ǂݍ��� */
	CopyMemoryRenew (&dwCount, pDataTmp, sizeof (dwCount), pDataTmp);
	nCount = (int)dwCount;

	for (i = 0; i < nCount; i ++) {
		pItem = (PCInfoItemWeapon)GetNew ();

		pDataTmp = pItem->SetSendData (pDataTmp);
		Add (pItem);
	}

	pRet = pDataTmp;
	return pRet;
}


/* ========================================================================= */
/* �֐���	:CLibInfoItemWeapon::GetNewSendData								 */
/* ���e		:���M�f�[�^����V�K�f�[�^���擾									 */
/* ���t		:2008/08/11														 */
/* �߂�l	:����������̃A�h���X											 */
/* ========================================================================= */

PBYTE CLibInfoItemWeapon::GetNewSendData(PBYTE pSrc, PCInfoItemWeapon &pDst)
{
	PBYTE pRet;
	CInfoItemWeapon InfoTmp;

	SAFE_DELETE (pDst);

	/* �܂��͊��N���X�֎�荞�� */
	InfoTmp.SetSendData (pSrc);
	pDst = (PCInfoItemWeapon)GetNew ();
	pRet = pDst->SetSendData (pSrc);

	return pRet;
}


/* ========================================================================= */
/* �֐���	:CLibInfoItemWeapon::GetNewID									 */
/* ���e		:�V�����A�C�e�����ID���擾										 */
/* ���t		:2008/08/10														 */
/* ========================================================================= */

DWORD CLibInfoItemWeapon::GetNewID(void)
{
	DWORD dwRet;
	int i, nCount;
	PCInfoItemWeapon pInfoTmp;

	dwRet = m_dwNewIDTmp + 1;
	if (dwRet == 0) {
		dwRet = 1;
	}

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->GetAt (i);
		if (pInfoTmp->m_dwWeaponInfoID == dwRet) {
			dwRet ++;
			i = -1;
			continue;
		}
	}
	m_dwNewIDTmp = dwRet;

	return dwRet;
}

/* Copyright(C)URARA-works 2008 */
