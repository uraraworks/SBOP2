/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:LibInfoSkill.cpp											 */
/* ���e			:�X�L����񃉃C�u�����N���X �����t�@�C��					 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/12/04													 */
/* ========================================================================= */

#include "stdafx.h"
#include "InfoSkillMOVEATACK.h"
#include "InfoSkillHEAL.h"
#include "LibInfoSkill.h"


/* ========================================================================= */
/* �֐���	:CLibInfoSkill::CLibInfoSkill									 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/12/04														 */
/* ========================================================================= */

CLibInfoSkill::CLibInfoSkill()
{
	m_paInfo = NULL;
}


/* ========================================================================= */
/* �֐���	:CLibInfoSkill::~CLibInfoSkill									 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/12/04														 */
/* ========================================================================= */

CLibInfoSkill::~CLibInfoSkill()
{
	Destroy ();
}


/* ========================================================================= */
/* �֐���	:CLibInfoSkill::Create											 */
/* ���e		:�쐬															 */
/* ���t		:2008/12/04														 */
/* ========================================================================= */

void CLibInfoSkill::Create(void)
{
	m_paInfo = new ARRAYSKILLBASEINFO;
}


/* ========================================================================= */
/* �֐���	:CLibInfoSkill::Destroy											 */
/* ���e		:�j��															 */
/* ���t		:2008/12/04														 */
/* ========================================================================= */

void CLibInfoSkill::Destroy(void)
{
	DeleteAll ();
	SAFE_DELETE (m_paInfo);
}


/* ========================================================================= */
/* �֐���	:CLibInfoSkill::GetNew											 */
/* ���e		:�V�K�f�[�^���擾												 */
/* ���t		:2008/12/07														 */
/* ========================================================================= */

PCInfoBase CLibInfoSkill::GetNew(void)
{
	return (PCInfoBase)GetNew (0, 0);
}


/* ========================================================================= */
/* �֐���	:CLibInfoSkill::GetNew											 */
/* ���e		:�V�K�f�[�^���擾												 */
/* ���t		:2008/12/04														 */
/* ========================================================================= */

PCInfoBase CLibInfoSkill::GetNew(int nTypeMain, int nTypeSub)
{
	PCInfoSkillBase pInfo;

	pInfo = NULL;

	switch (nTypeMain) {
	case SKILLTYPEMAIN_NONE:			/* �\�� */
		break;
	case SKILLTYPEMAIN_BATTLE:			/* �퓬 */
		switch (nTypeSub) {
		case SKILLTYPESUB_BATTLE_MOVEATACK:		/* �ړ����čU�� */
			pInfo = new CInfoSkillMOVEATACK;
			break;
		case SKILLTYPESUB_BATTLE_HEAL:			/* �� */
			pInfo = new CInfoSkillHEAL;
			break;
		}
		break;
	case SKILLTYPEMAIN_LIFE:			/* ���� */
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
/* �֐���	:CLibInfoSkill::GetCount										 */
/* ���e		:�f�[�^�����擾													 */
/* ���t		:2008/12/04														 */
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
/* �֐���	:CLibInfoSkill::Add												 */
/* ���e		:�ǉ�															 */
/* ���t		:2008/12/04														 */
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
/* �֐���	:CLibInfoSkill::Delete											 */
/* ���e		:�폜															 */
/* ���t		:2008/12/04														 */
/* ========================================================================= */

void CLibInfoSkill::Delete(
	int nNo)		/* [in] �z��ԍ� */
{
	PCInfoSkillBase pInfo;

	pInfo = m_paInfo->GetAt (nNo);
	SAFE_DELETE (pInfo);
	m_paInfo->RemoveAt (nNo);
}


/* ========================================================================= */
/* �֐���	:CLibInfoSkill::Delete											 */
/* ���e		:�폜															 */
/* ���t		:2008/12/04														 */
/* ========================================================================= */

void CLibInfoSkill::Delete(
	DWORD dwSkillID)		/* [in] �X�L��ID */
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
/* �֐���	:CLibInfoSkill::DeleteAll										 */
/* ���e		:�S�č폜														 */
/* ���t		:2008/12/04														 */
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
/* �֐���	:CLibInfoSkill::Merge											 */
/* ���e		:��荞��														 */
/* ���t		:2008/12/04														 */
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
/* �֐���	:CLibInfoSkill::Renew											 */
/* ���e		:�X�V															 */
/* ���t		:2008/12/08														 */
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
/* �֐���	:CLibInfoSkill::GetPtr											 */
/* ���e		:�����擾														 */
/* ���t		:2008/12/04														 */
/* ========================================================================= */

PCInfoBase CLibInfoSkill::GetPtr(int nNo)
{
	return m_paInfo->GetAt (nNo);
}


/* ========================================================================= */
/* �֐���	:CLibInfoSkill::GetPtr											 */
/* ���e		:�����擾														 */
/* ���t		:2008/12/04														 */
/* ========================================================================= */

PCInfoBase CLibInfoSkill::GetPtr(
	DWORD dwSkillID)		/* [in] �X�L��ID */
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
/* �֐���	:CLibInfoSkill::RenewInfo										 */
/* ���e		:���e�ɉ����čX�V												 */
/* ���t		:2009/01/11														 */
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
/* �֐���	:CLibInfoSkill::GetSendDataSize									 */
/* ���e		:���M�f�[�^�T�C�Y���擾											 */
/* ���t		:2008/12/04														 */
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
	/* �I�[�p */
	dwSize += sizeof (DWORD);

	return dwSize;
}


/* ========================================================================= */
/* �֐���	:CLibInfoSkill::GetSendData										 */
/* ���e		:���M�f�[�^���擾												 */
/* ���t		:2008/12/04														 */
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
/* �֐���	:CLibInfoSkill::SetSendData										 */
/* ���e		:���M�f�[�^�����荞��											 */
/* ���t		:2008/12/04														 */
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
/* �֐���	:CLibInfoSkill::GetNewID										 */
/* ���e		:�V����ID���擾													 */
/* ���t		:2008/12/04														 */
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
