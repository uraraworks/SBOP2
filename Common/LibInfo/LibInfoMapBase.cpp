/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* �t�@�C����	:LibInfoMapBase.cpp											 */
/* ���e			:�}�b�v�����N���X �����t�@�C��							 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2006/10/15													 */
/* ========================================================================= */

#include "stdafx.h"
#include "LibInfoMapParts.h"
#include "LibInfoMapBase.h"


/* ========================================================================= */
/* �֐���	:CLibInfoMapBase::CLibInfoMapBase								 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2006/10/15														 */
/* ========================================================================= */

CLibInfoMapBase::CLibInfoMapBase()
{
	m_paInfo = NULL;
	m_pLibInfoMapParts = NULL;
}


/* ========================================================================= */
/* �֐���	:CLibInfoMapBase::~CLibInfoMapBase								 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2006/10/15														 */
/* ========================================================================= */

CLibInfoMapBase::~CLibInfoMapBase()
{
	Destroy ();
}


/* ========================================================================= */
/* �֐���	:CLibInfoMapBase::Create										 */
/* ���e		:�쐬															 */
/* ���t		:2006/10/15														 */
/* ========================================================================= */

void CLibInfoMapBase::Create(CLibInfoMapParts *pLibInfoMapParts)
{
	m_paInfo = new ARRAYINFOMAPBASE;

	m_pLibInfoMapParts = pLibInfoMapParts;
}


/* ========================================================================= */
/* �֐���	:CLibInfoMapBase::Destroy										 */
/* ���e		:�j��															 */
/* ���t		:2006/10/15														 */
/* ========================================================================= */

void CLibInfoMapBase::Destroy(void)
{
	DeleteAll ();
	SAFE_DELETE (m_paInfo);
}


/* ========================================================================= */
/* �֐���	:CLibInfoMapBase::GetNew										 */
/* ���e		:�V�K�f�[�^���擾												 */
/* ���t		:2006/10/15														 */
/* ========================================================================= */

PCInfoBase CLibInfoMapBase::GetNew(void)
{
	PCInfoMapBase pInfoMapBase;

	pInfoMapBase = new CInfoMapBase;
	pInfoMapBase->Create (m_pLibInfoMapParts);

	return pInfoMapBase;
}


/* ========================================================================= */
/* �֐���	:CLibInfoMapBase::RenewMapEvent									 */
/* ���e		:�}�b�v�C�x���g�X�V												 */
/* ���t		:2008/06/28														 */
/* ========================================================================= */

void CLibInfoMapBase::RenewMapEvent(void)
{
	int i, nCount;
	PCInfoMapBase pInfoMapBase;

	nCount = GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfoMapBase = (PCInfoMapBase)GetPtr (i);
		pInfoMapBase->RenewMapEvent ();
	}
}


/* ========================================================================= */
/* �֐���	:CLibInfoMapBase::RenewHitTmp									 */
/* ���e		:�}�b�v�p�[�c�ȊO�ł̓����蔻����X�V							 */
/* ���t		:2008/11/13														 */
/* ========================================================================= */

void CLibInfoMapBase::RenewHitTmp(void)
{
	int i, nCount;
	PCInfoMapBase pInfoMapBase;

	nCount = GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfoMapBase = (PCInfoMapBase)GetPtr (i);
		pInfoMapBase->RenewHitTmp ();
	}
}


/* ========================================================================= */
/* �֐���	:CLibInfoMapBase::SetMapObject									 */
/* ���e		:�}�b�v�I�u�W�F�N�g����ݒ�									 */
/* ���t		:2008/11/13														 */
/* ========================================================================= */

void CLibInfoMapBase::SetMapObject(CLibInfoMapObject *pLibInfo)
{
	int i, nCount;
	PCInfoMapBase pInfoMapBase;

	nCount = GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfoMapBase = (PCInfoMapBase)GetPtr (i);
		pInfoMapBase->m_pLibInfoMapObject = pLibInfo;
	}
}


/* ========================================================================= */
/* �֐���	:CLibInfoMapBase::GetCount										 */
/* ���e		:�f�[�^�����擾													 */
/* ���t		:2006/10/15														 */
/* ========================================================================= */

int CLibInfoMapBase::GetCount(void)
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
/* �֐���	:CLibInfoMapBase::Add											 */
/* ���e		:�ǉ�															 */
/* ���t		:2006/10/15														 */
/* ========================================================================= */

void CLibInfoMapBase::Add(PCInfoBase pInfo)
{
	PCInfoMapBase pMapInfo;

	pMapInfo = (PCInfoMapBase)pInfo;
	if (pMapInfo->m_dwMapID == 0) {
		pMapInfo->m_dwMapID = GetNewID ();
	}

	m_paInfo->Add (pMapInfo);
}


/* ========================================================================= */
/* �֐���	:CLibInfoMapBase::Delete										 */
/* ���e		:�폜															 */
/* ���t		:2006/10/15														 */
/* ========================================================================= */

void CLibInfoMapBase::Delete(
	int nNo)		/* [in] �z��ԍ� */
{
	PCInfoMapBase pInfo;

	pInfo = m_paInfo->GetAt (nNo);
	SAFE_DELETE (pInfo);
	m_paInfo->RemoveAt (nNo);
}


/* ========================================================================= */
/* �֐���	:CLibInfoMapBase::Delete										 */
/* ���e		:�폜															 */
/* ���t		:2006/10/15														 */
/* ========================================================================= */

void CLibInfoMapBase::Delete(
	DWORD dwMapID)		/* [in] �}�b�vID */
{
	int i, nCount, nNo;
	PCInfoMapBase pInfoTmp;

	nNo = -1;

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->GetAt (i);
		if (pInfoTmp->m_dwMapID != dwMapID) {
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
/* �֐���	:CLibInfoMapBase::DeleteAll										 */
/* ���e		:�S�č폜														 */
/* ���t		:2006/10/15														 */
/* ========================================================================= */

void CLibInfoMapBase::DeleteAll(void)
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
/* �֐���	:CLibInfoMapBase::DeleteParts									 */
/* ���e		:�w��p�[�c���폜												 */
/* ���t		:2007/05/04														 */
/* ========================================================================= */

void CLibInfoMapBase::DeleteParts(DWORD dwPartsID)
{
	int i, nCount;
	PCInfoMapBase pInfoTmp;

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->GetAt (i);
		pInfoTmp->DeleteParts (dwPartsID);
	}
}


/* ========================================================================= */
/* �֐���	:CLibInfoMapBase::DeleteShadow									 */
/* ���e		:�w��}�b�v�e���폜												 */
/* ���t		:2007/05/04														 */
/* ========================================================================= */

void CLibInfoMapBase::DeleteShadow(DWORD dwShadowID)
{
	int i, nCount;
	PCInfoMapBase pInfoTmp;

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->GetAt (i);
		pInfoTmp->DeleteShadow (dwShadowID);
	}
}


/* ========================================================================= */
/* �֐���	:CLibInfoMapBase::GetPtr										 */
/* ���e		:�}�b�v�����擾												 */
/* ���t		:2006/10/15														 */
/* ========================================================================= */

PCInfoBase CLibInfoMapBase::GetPtr(int nNo)
{
	return m_paInfo->GetAt (nNo);
}


/* ========================================================================= */
/* �֐���	:CLibInfoMapBase::GetPtr										 */
/* ���e		:�}�b�v�����擾												 */
/* ���t		:2006/10/15														 */
/* ========================================================================= */

PCInfoBase CLibInfoMapBase::GetPtr(
	DWORD dwMapID)		/* [in] �}�b�vID */
{
	int i, nCount;
	PCInfoMapBase pRet, pInfoTmp;

	pRet = NULL;

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->GetAt (i);
		if (pInfoTmp->m_dwMapID != dwMapID) {
			continue;
		}
		pRet = pInfoTmp;
		break;
	}

	return pRet;
}


/* ========================================================================= */
/* �֐���	:CLibInfoMapBase::GetNewID										 */
/* ���e		:�V�����}�b�vID���擾											 */
/* ���t		:2006/10/15														 */
/* ========================================================================= */

DWORD CLibInfoMapBase::GetNewID(void)
{
	DWORD dwRet;
	int i, nCount;
	PCInfoMapBase pInfoTmp;

	dwRet = 1;

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->GetAt (i);
		if (pInfoTmp->m_dwMapID == dwRet) {
			dwRet ++;
			i = -1;
			continue;
		}
	}

	return dwRet;
}

/* Copyright(C)URARA-works 2006 */
