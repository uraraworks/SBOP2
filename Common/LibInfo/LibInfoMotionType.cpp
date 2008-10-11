/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:LibInfoMotionType.cpp										 */
/* ���e			:���[�V������񃉃C�u�������N���X �����t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/05/26													 */
/* ========================================================================= */

#include "stdafx.h"
#include "LibInfoMotionType.h"


/* ========================================================================= */
/* �֐���	:CLibInfoMotionType::CLibInfoMotionType							 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/05/26														 */
/* ========================================================================= */

CLibInfoMotionType::CLibInfoMotionType()
{
	m_dwNewIDTmp	= 0;
	m_paInfo		= NULL;
}


/* ========================================================================= */
/* �֐���	:CLibInfoMotionType::~CLibInfoMotionType						 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/05/26														 */
/* ========================================================================= */

CLibInfoMotionType::~CLibInfoMotionType()
{
	Destroy ();
}


/* ========================================================================= */
/* �֐���	:CLibInfoMotionType::Create										 */
/* ���e		:�쐬															 */
/* ���t		:2008/05/26														 */
/* ========================================================================= */

void CLibInfoMotionType::Create(void)
{
	m_paInfo = new ARRAYMOTIONTYPEINFO;
}


/* ========================================================================= */
/* �֐���	:CLibInfoMotionType::Destroy									 */
/* ���e		:�j��															 */
/* ���t		:2008/05/26														 */
/* ========================================================================= */

void CLibInfoMotionType::Destroy(void)
{
	DeleteAll ();
	SAFE_DELETE (m_paInfo);
}


/* ========================================================================= */
/* �֐���	:CLibInfoMotionType::GetNew										 */
/* ���e		:�V�K�f�[�^���擾												 */
/* ���t		:2008/05/26														 */
/* ========================================================================= */

PCInfoBase CLibInfoMotionType::GetNew(void)
{
	PCInfoMotionType pInfoMotion;

	pInfoMotion = new CInfoMotionType;

	return pInfoMotion;
}


/* ========================================================================= */
/* �֐���	:CLibInfoMotionType::GetPtr										 */
/* ���e		:�����擾														 */
/* ���t		:2008/05/26														 */
/* ========================================================================= */

PCInfoBase CLibInfoMotionType::GetPtr(int nNo)
{
	PCInfoBase pRet;

	pRet = NULL;
	if (nNo < 0 || nNo >= GetCount ()) {
		goto Exit;
	}
	pRet = m_paInfo->GetAt (nNo);
Exit:
	return pRet;
}


/* ========================================================================= */
/* �֐���	:CLibInfoMotionType::GetPtr										 */
/* ���e		:�����擾														 */
/* ���t		:2008/05/27														 */
/* ========================================================================= */

PCInfoBase CLibInfoMotionType::GetPtr(DWORD dwMotionTypeID)
{
	int i, nCount;
	PCInfoMotionType pRet, pInfo;

	pRet = NULL;

	nCount = GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfo = m_paInfo->GetAt (i);
		if (pInfo->m_dwMotionTypeID != dwMotionTypeID) {
			continue;
		}
		pRet = pInfo;
		break;
	}

	return pRet;
}


/* ========================================================================= */
/* �֐���	:CLibInfoMotionType::GetCount									 */
/* ���e		:�f�[�^�����擾													 */
/* ���t		:2008/05/26														 */
/* ========================================================================= */

int CLibInfoMotionType::GetCount(void)
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
/* �֐���	:CLibInfoMotionType::Add										 */
/* ���e		:�ǉ�															 */
/* ���t		:2008/05/26														 */
/* ========================================================================= */

void CLibInfoMotionType::Add(PCInfoBase pInfo)
{
	PCInfoMotionType pMotionTypeInfo;

	pMotionTypeInfo = (PCInfoMotionType)pInfo;
	if (pMotionTypeInfo->m_dwMotionTypeID == 0) {
		pMotionTypeInfo->m_dwMotionTypeID = GetNewID ();
	}

	m_paInfo->Add (pMotionTypeInfo);
}


/* ========================================================================= */
/* �֐���	:CLibInfoMotionType::Delete										 */
/* ���e		:�폜															 */
/* ���t		:2008/05/26														 */
/* ========================================================================= */

void CLibInfoMotionType::Delete(
	int nNo)		/* [in] �z��ԍ� */
{
	PCInfoMotionType pInfo;

	pInfo = m_paInfo->GetAt (nNo);
	SAFE_DELETE (pInfo);
	m_paInfo->RemoveAt (nNo);
}


/* ========================================================================= */
/* �֐���	:CLibInfoMotionType::Delete										 */
/* ���e		:�폜															 */
/* ���t		:2008/05/26														 */
/* ========================================================================= */

void CLibInfoMotionType::Delete(
	DWORD dwMotionTypeID)		/* [in] ���[�V�������ID */
{
	int i, nCount, nNo;
	PCInfoMotionType pInfoTmp;

	nNo = -1;

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->GetAt (i);
		if (pInfoTmp->m_dwMotionTypeID != dwMotionTypeID) {
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
/* �֐���	:CLibInfoMotionType::DeleteAll									 */
/* ���e		:�S�č폜														 */
/* ���t		:2008/05/26														 */
/* ========================================================================= */

void CLibInfoMotionType::DeleteAll(void)
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
/* �֐���	:CLibInfoMotionType::GetSendDataSize							 */
/* ���e		:���M�f�[�^�T�C�Y���擾											 */
/* ���t		:2008/05/26														 */
/* ========================================================================= */

DWORD CLibInfoMotionType::GetSendDataSize(DWORD dwMotionTypeID)
{
	DWORD dwRet;
	int i, nCount;
	PCInfoMotionType pInfo;

	dwRet = 0;

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfo = m_paInfo->GetAt (i);
		if (dwMotionTypeID != 0) {
			if (pInfo->m_dwMotionTypeID != dwMotionTypeID) {
				continue;
			}
		}
		dwRet += pInfo->GetSendDataSize ();
	}

	dwRet += sizeof (DWORD);	/* �I�[�� */

	return dwRet;
}


/* ========================================================================= */
/* �֐���	:CLibInfoMotionType::GetSendData								 */
/* ���e		:���M�f�[�^���擾												 */
/* ���t		:2008/05/26														 */
/* ========================================================================= */

PBYTE CLibInfoMotionType::GetSendData(DWORD dwMotionTypeID)
{
	PBYTE pRet, pDataTmp, pPos;
	DWORD dwSize, dwSizeTmp;
	int i, nCount;
	PCInfoMotionType pInfo;

	pRet = NULL;

	dwSize = GetSendDataSize (dwMotionTypeID);
	if (dwSize == 0) {
		goto Exit;
	}
	pRet = ZeroNew (dwSize);
	pPos = pRet;

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfo = m_paInfo->GetAt (i);
		if (dwMotionTypeID != 0) {
			if (pInfo->m_dwMotionTypeID != dwMotionTypeID) {
				continue;
			}
		}
		dwSizeTmp	= pInfo->GetSendDataSize ();
		pDataTmp	= pInfo->GetSendData ();
		CopyMemoryRenew (pPos, pDataTmp, dwSizeTmp, pPos);
		SAFE_DELETE_ARRAY (pDataTmp);
	}

Exit:
	return pRet;
}


/* ========================================================================= */
/* �֐���	:CLibInfoMotionType::SetSendData								 */
/* ���e		:���M�f�[�^�����荞��											 */
/* ���t		:2008/05/26														 */
/* ========================================================================= */

PBYTE CLibInfoMotionType::SetSendData(PBYTE pSrc)
{
	PBYTE pRet, pDataTmp;
	DWORD dwMotionTypeID;
	PCInfoMotionType pInfoMotion;

	pRet = pSrc;

	pDataTmp = pSrc;
	while (1) {
		CopyMemory (&dwMotionTypeID, pDataTmp, sizeof (dwMotionTypeID));
		if (dwMotionTypeID == 0) {
			pDataTmp += sizeof (DWORD);
			break;
		}
		pInfoMotion = (PCInfoMotionType)GetPtr (dwMotionTypeID);
		if (pInfoMotion == NULL) {
			pInfoMotion = new CInfoMotionType;
			pDataTmp = pInfoMotion->SetSendData (pDataTmp);
			Add (pInfoMotion);
		} else {
			pDataTmp = pInfoMotion->SetSendData (pDataTmp);
		}
	}
	pRet = pDataTmp;

	return pRet;
}


/* ========================================================================= */
/* �֐���	:CLibInfoMotionType::GetNewID									 */
/* ���e		:�V�������[�V����ID���擾										 */
/* ���t		:2008/05/26														 */
/* ========================================================================= */

DWORD CLibInfoMotionType::GetNewID(void)
{
	DWORD dwRet;
	int i, nCount;
	PCInfoMotionType pInfoTmp;

	dwRet = m_dwNewIDTmp + 1;
	if (dwRet == 0) {
		dwRet = 1;
	}

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->GetAt (i);
		if (pInfoTmp->m_dwMotionTypeID == dwRet) {
			dwRet ++;
			i = -1;
			continue;
		}
	}
	m_dwNewIDTmp = dwRet;

	return dwRet;
}

/* Copyright(C)URARA-works 2008 */
