/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:LibInfoMotion.cpp											 */
/* ���e			:���[�V������񃉃C�u�������N���X �����t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/10/29													 */
/* ========================================================================= */

#include "stdafx.h"
#include "InfoMotion.h"
#include "LibInfoMotion.h"


/* ========================================================================= */
/* �֐���	:CLibInfoMotion::CLibInfoMotion									 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/10/29														 */
/* ========================================================================= */

CLibInfoMotion::CLibInfoMotion()
{
	m_dwNewIDTmp	= 0;
	m_paInfo		= NULL;
}


/* ========================================================================= */
/* �֐���	:CLibInfoMotion::~CLibInfoMotion								 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/10/29														 */
/* ========================================================================= */

CLibInfoMotion::~CLibInfoMotion()
{
	Destroy ();
}


/* ========================================================================= */
/* �֐���	:CLibInfoMotion::Create											 */
/* ���e		:�쐬															 */
/* ���t		:2007/10/29														 */
/* ========================================================================= */

void CLibInfoMotion::Create(void)
{
	m_paInfo = new ARRAYMOTIONINFO;
}


/* ========================================================================= */
/* �֐���	:CLibInfoMotion::Destroy										 */
/* ���e		:�j��															 */
/* ���t		:2007/10/29														 */
/* ========================================================================= */

void CLibInfoMotion::Destroy(void)
{
	DeleteAll ();
	SAFE_DELETE (m_paInfo);
}


/* ========================================================================= */
/* �֐���	:CLibInfoMotion::GetNew											 */
/* ���e		:�V�K�f�[�^���擾												 */
/* ���t		:2007/10/29														 */
/* ========================================================================= */

PCInfoBase CLibInfoMotion::GetNew(void)
{
	PCInfoMotion pInfoMotion;

	pInfoMotion = new CInfoMotion;

	return pInfoMotion;
}


/* ========================================================================= */
/* �֐���	:CLibInfoMotion::GetPtr											 */
/* ���e		:�����擾														 */
/* ���t		:2007/11/14														 */
/* ========================================================================= */

PCInfoBase CLibInfoMotion::GetPtr(int nNo)
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
/* �֐���	:CLibInfoMotion::Revice											 */
/* ���e		:�f�[�^�̕␳													 */
/* ���t		:2008/07/02														 */
/* ========================================================================= */

void CLibInfoMotion::Revice(void)
{
	int i, j, nCount;
	PCInfoMotion pInfoTmp;

	nCount = GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = (PCInfoMotion)GetPtr (i);

		if (pInfoTmp->m_anDrawList.GetSize () > 0) {
			continue;
		}
		for (j = 0; j < 4; j ++) {
			pInfoTmp->m_anDrawList.Add (j);
		}
	}
}


/* ========================================================================= */
/* �֐���	:CLibInfoMotion::GetCount										 */
/* ���e		:�f�[�^�����擾													 */
/* ���t		:2007/10/29														 */
/* ========================================================================= */

int CLibInfoMotion::GetCount(void)
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
/* �֐���	:CLibInfoMotion::Add											 */
/* ���e		:�ǉ�															 */
/* ���t		:2007/10/29														 */
/* ========================================================================= */

void CLibInfoMotion::Add(PCInfoBase pInfo)
{
	PCInfoMotion pItemInfo;

	pItemInfo = (PCInfoMotion)pInfo;
	if (pItemInfo->m_dwMotionID == 0) {
		pItemInfo->m_dwMotionID = GetNewID ();
	}

	m_paInfo->Add (pItemInfo);
}


/* ========================================================================= */
/* �֐���	:CLibInfoMotion::Add											 */
/* ���e		:�ǉ�															 */
/* ���t		:2008/07/15														 */
/* ========================================================================= */

void CLibInfoMotion::Add(PCInfoBase pInfo, int nNo)
{
	PCInfoMotion pItemInfo;

	pItemInfo = (PCInfoMotion)pInfo;
	if (pItemInfo->m_dwMotionID == 0) {
		pItemInfo->m_dwMotionID = GetNewID ();
	}

	m_paInfo->InsertAt (nNo, pItemInfo);
}


/* ========================================================================= */
/* �֐���	:CLibInfoMotion::AddType										 */
/* ���e		:�w���ʂňꗗ�ǉ�												 */
/* ���t		:2008/06/09														 */
/* ========================================================================= */

void CLibInfoMotion::AddType(DWORD dwMotionTypeID)
{
	int i;
	PCInfoMotion pInfoTmp;

	for (i = CHARMOTIONLISTID_NONE + 1; i < CHARMOTIONLISTID_MAX; i ++) {
		pInfoTmp = (PCInfoMotion)GetNew ();
		pInfoTmp->m_dwMotionListID	= i;
		pInfoTmp->m_dwMotionTypeID	= dwMotionTypeID;
		Add (pInfoTmp);
	}
}


/* ========================================================================= */
/* �֐���	:CLibInfoMotion::Delete											 */
/* ���e		:�폜															 */
/* ���t		:2007/10/29														 */
/* ========================================================================= */

void CLibInfoMotion::Delete(
	int nNo)		/* [in] �z��ԍ� */
{
	PCInfoMotion pInfo;

	pInfo = m_paInfo->GetAt (nNo);
	SAFE_DELETE (pInfo);
	m_paInfo->RemoveAt (nNo);
}


/* ========================================================================= */
/* �֐���	:CLibInfoMotion::Delete											 */
/* ���e		:�폜															 */
/* ���t		:2007/10/29														 */
/* ========================================================================= */

void CLibInfoMotion::Delete(
	DWORD dwMotionID)		/* [in] ���[�V����ID */
{
	int i, nCount, nNo;
	PCInfoMotion pInfoTmp;

	nNo = -1;

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->GetAt (i);
		if (pInfoTmp->m_dwMotionID != dwMotionID) {
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
/* �֐���	:CLibInfoMotion::DeleteAll										 */
/* ���e		:�S�č폜														 */
/* ���t		:2007/10/29														 */
/* ========================================================================= */

void CLibInfoMotion::DeleteAll(void)
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
/* �֐���	:CLibInfoMotion::SetList										 */
/* ���e		:�w��ID�̃��X�g��ݒ�											 */
/* ���t		:2007/11/23														 */
/* ========================================================================= */

void CLibInfoMotion::SetList(DWORD dwMotionTypeID, DWORD dwMotionListID, CLibInfoMotion *pSrc)
{
	int i, nCount;
	PCInfoMotion pInfo, pInfoTmp;

	/* �܂��͑ΏۂƂȂ郊�X�gID�̏����폜 */
	nCount = m_paInfo->GetSize ();
	for (i = nCount - 1; i >= 0; i --) {
		pInfo = m_paInfo->GetAt (i);
		if (dwMotionTypeID != 0) {
			if (pInfo->m_dwMotionTypeID != dwMotionTypeID) {
				continue;
			}
		}
		if (dwMotionListID != 0) {
			if (pInfo->m_dwMotionListID != dwMotionListID) {
				continue;
			}
		}
		Delete (i);
	}

	nCount = pSrc->GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfo = (PCInfoMotion)pSrc->GetPtr (i);
		if (dwMotionTypeID != 0) {
			if (pInfo->m_dwMotionTypeID != dwMotionTypeID) {
				continue;
			}
		}
		if (dwMotionListID != 0) {
			if (pInfo->m_dwMotionListID != dwMotionListID) {
				continue;
			}
		}
		pInfoTmp = new CInfoMotion;
		pInfoTmp->Copy (pInfo);
		Add (pInfoTmp);
	}
}


/* ========================================================================= */
/* �֐���	:CLibInfoMotion::IsDirection									 */
/* ���e		:�������ɐݒ肪���郂�[�V����������								 */
/* ���t		:2008/01/03														 */
/* ========================================================================= */

BOOL CLibInfoMotion::IsDirection(DWORD dwMotionListID)
{
	BOOL bRet;

	bRet = TRUE;
	switch (dwMotionListID) {
	case CHARMOTIONLISTID_STAND:			/* ���� */
	case CHARMOTIONLISTID_STAND_SIMPLE:		/* ����(�㉺���E1�R�}����) */
	case CHARMOTIONLISTID_WALK:				/* ����(��������) */
	case CHARMOTIONLISTID_DESTROY:			/* �j�� */
	case CHARMOTIONLISTID_SIMPLE:			/* 16�R�}�A�� */
		bRet = FALSE;
		break;
	}

	return bRet;
}


/* ========================================================================= */
/* �֐���	:CLibInfoMotion::GetWaitTime									 */
/* ���e		:�S�R�}�̕\�����Ԃ��擾											 */
/* ���t		:2008/07/24														 */
/* ========================================================================= */

DWORD CLibInfoMotion::GetWaitTime(DWORD dwMotionTypeID, DWORD dwMotionListID)
{
	int i, nCount;
	DWORD dwRet;
	ARRAYMOTIONINFO apMotionInfo;

	dwRet = 0;
	GetMotionInfo (dwMotionTypeID, dwMotionListID, apMotionInfo);

	nCount = apMotionInfo.GetSize ();
	for (i = 0; i < nCount; i ++) {
		dwRet += apMotionInfo[i]->m_byWait;
	}
	dwRet *= 10;

	return dwRet;
}


/* ========================================================================= */
/* �֐���	:CLibInfoMotion::GetMotionInfo									 */
/* ���e		:�w��ID�̃��[�V�������ꗗ���擾								 */
/* ���t		:2007/11/24														 */
/* ========================================================================= */

void CLibInfoMotion::GetMotionInfo(DWORD dwMotionTypeID, DWORD dwMotionListID, ARRAYMOTIONINFO &aDst)
{
	int i, nCount;
	PCInfoMotion pInfo;

	if (dwMotionListID == 0) {
		return;
	}
	aDst.RemoveAll ();

	nCount = GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfo = (PCInfoMotion)GetPtr (i);
		if (dwMotionTypeID != 0) {
			if (pInfo->m_dwMotionTypeID != dwMotionTypeID) {
				continue;
			}
		}
		if (dwMotionListID != 0) {
			if (pInfo->m_dwMotionListID != dwMotionListID) {
				continue;
			}
		}
		aDst.Add (pInfo);
	}
}


/* ========================================================================= */
/* �֐���	:CLibInfoMotion::GetSendDataSize								 */
/* ���e		:���M�f�[�^�T�C�Y���擾											 */
/* ���t		:2007/11/23														 */
/* ========================================================================= */

DWORD CLibInfoMotion::GetSendDataSize(DWORD dwMotionTypeID, DWORD dwMotionListID)
{
	DWORD dwRet;
	int i, nCount;
	PCInfoMotion pInfo;

	dwRet = 0;

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfo = m_paInfo->GetAt (i);
		if (dwMotionTypeID != 0) {
			if (pInfo->m_dwMotionTypeID != dwMotionTypeID) {
				continue;
			}
		}
		if (dwMotionListID != 0) {
			if (pInfo->m_dwMotionListID != dwMotionListID) {
				continue;
			}
		}
		dwRet += pInfo->GetSendDataSize ();
	}

	dwRet += sizeof (DWORD);	/* �I�[�� */

	return dwRet;
}


/* ========================================================================= */
/* �֐���	:CLibInfoMotion::GetSendData									 */
/* ���e		:���M�f�[�^���擾												 */
/* ���t		:2007/11/23														 */
/* ========================================================================= */

PBYTE CLibInfoMotion::GetSendData(DWORD dwMotionTypeID, DWORD dwMotionListID)
{
	PBYTE pRet, pDataTmp, pPos;
	DWORD dwSize, dwSizeTmp;
	int i, nCount;
	PCInfoMotion pInfo;

	pRet = NULL;

	dwSize = GetSendDataSize (dwMotionTypeID, dwMotionListID);
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
		if (dwMotionListID != 0) {
			if (pInfo->m_dwMotionListID != dwMotionListID) {
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
/* �֐���	:CLibInfoMotion::SetSendData									 */
/* ���e		:���M�f�[�^�����荞��											 */
/* ���t		:2007/11/23														 */
/* ========================================================================= */

PBYTE CLibInfoMotion::SetSendData(PBYTE pSrc)
{
	PBYTE pRet, pDataTmp;
	DWORD dwMotionID;
	PCInfoMotion pInfoMotion;

	pRet = pSrc;

	pDataTmp = pSrc;
	while (1) {
		CopyMemory (&dwMotionID, pDataTmp, sizeof (dwMotionID));
		if (dwMotionID == 0) {
			pDataTmp += sizeof (DWORD);
			break;
		}
		pInfoMotion = new CInfoMotion;
		pDataTmp = pInfoMotion->SetSendData (pDataTmp);
		Add (pInfoMotion);
	}
	pRet = pDataTmp;

	return pRet;
}


/* ========================================================================= */
/* �֐���	:CLibInfoMotion::GetSendDataSizeType							 */
/* ���e		:�w����ID�̑��M�f�[�^�T�C�Y���擾								 */
/* ���t		:2008/06/09														 */
/* ========================================================================= */

DWORD CLibInfoMotion::GetSendDataSizeType(DWORD dwMotionTypeID)
{
	return 0;
}


/* ========================================================================= */
/* �֐���	:CLibInfoMotion::GetSendDataType								 */
/* ���e		:�w����ID�̑��M�f�[�^���擾									 */
/* ���t		:2008/06/09														 */
/* ========================================================================= */

PBYTE CLibInfoMotion::GetSendDataType(DWORD dwMotionTypeID)
{
	return NULL;
}


/* ========================================================================= */
/* �֐���	:CLibInfoMotion::SetSendDataType								 */
/* ���e		:���M�f�[�^�����荞��											 */
/* ���t		:2008/06/09														 */
/* ========================================================================= */

PBYTE CLibInfoMotion::SetSendDataType(PBYTE pSrc)
{
	return NULL;
}


/* ========================================================================= */
/* �֐���	:CLibInfoMotion::GetNewID										 */
/* ���e		:�V�������[�V����ID���擾										 */
/* ���t		:2007/10/29														 */
/* ========================================================================= */

DWORD CLibInfoMotion::GetNewID(void)
{
	DWORD dwRet;
	int i, nCount;
	PCInfoMotion pInfoTmp;

	dwRet = m_dwNewIDTmp + 1;
	if (dwRet == 0) {
		dwRet = 1;
	}

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->GetAt (i);
		if (pInfoTmp->m_dwMotionID == dwRet) {
			dwRet ++;
			i = -1;
			continue;
		}
	}
	m_dwNewIDTmp = dwRet;

	return dwRet;
}

/* Copyright(C)URARA-works 2007 */
