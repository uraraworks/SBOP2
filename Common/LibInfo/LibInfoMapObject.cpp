/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:LibInfoMapObject.cpp										 */
/* ���e			:�}�b�v�p�[�c��񃉃C�u�����N���X �����t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/11/01													 */
/* ========================================================================= */

#include "stdafx.h"
#include "LibInfoMapObject.h"


/* ========================================================================= */
/* �֐���	:CLibInfoMapObject::CLibInfoMapObject							 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/11/01														 */
/* ========================================================================= */

CLibInfoMapObject::CLibInfoMapObject()
{
	m_paInfo = NULL;
}


/* ========================================================================= */
/* �֐���	:CLibInfoMapObject::~CLibInfoMapObject							 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/11/01														 */
/* ========================================================================= */

CLibInfoMapObject::~CLibInfoMapObject()
{
	Destroy ();
}


/* ========================================================================= */
/* �֐���	:CLibInfoMapObject::Create										 */
/* ���e		:�쐬															 */
/* ���t		:2008/11/01														 */
/* ========================================================================= */

void CLibInfoMapObject::Create(void)
{
	m_paInfo = new ARRAYMAPOBJECT;
}


/* ========================================================================= */
/* �֐���	:CLibInfoMapObject::Destroy										 */
/* ���e		:�j��															 */
/* ���t		:2008/11/01														 */
/* ========================================================================= */

void CLibInfoMapObject::Destroy(void)
{
	DeleteAll ();
	SAFE_DELETE (m_paInfo);
}


/* ========================================================================= */
/* �֐���	:CLibInfoMapObject::GetNew										 */
/* ���e		:�V�K�f�[�^���擾												 */
/* ���t		:2008/11/01														 */
/* ========================================================================= */

PCInfoBase CLibInfoMapObject::GetNew(void)
{
	return new CInfoMapObject;
}


/* ========================================================================= */
/* �֐���	:CLibInfoMapObject::GetCount									 */
/* ���e		:�f�[�^�����擾													 */
/* ���t		:2008/11/01														 */
/* ========================================================================= */

int CLibInfoMapObject::GetCount(void)
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
/* �֐���	:CLibInfoMapObject::Add											 */
/* ���e		:�ǉ�															 */
/* ���t		:2008/11/01														 */
/* ========================================================================= */

void CLibInfoMapObject::Add(PCInfoBase pInfo)
{
	PCInfoMapObject pInfoMapObject;

	pInfoMapObject = (PCInfoMapObject)pInfo;
	if (pInfoMapObject->m_dwObjectID == 0) {
		pInfoMapObject->m_dwObjectID = GetNewID ();
	}

	m_paInfo->Add (pInfoMapObject);
}


/* ========================================================================= */
/* �֐���	:CLibInfoMapObject::Delete										 */
/* ���e		:�폜															 */
/* ���t		:2008/11/01														 */
/* ========================================================================= */

void CLibInfoMapObject::Delete(
	int nNo)		/* [in] �z��ԍ� */
{
	PCInfoMapObject pInfo;

	pInfo = m_paInfo->GetAt (nNo);
	SAFE_DELETE (pInfo);
	m_paInfo->RemoveAt (nNo);
}


/* ========================================================================= */
/* �֐���	:CLibInfoMapObject::Delete										 */
/* ���e		:�폜															 */
/* ���t		:2008/11/01														 */
/* ========================================================================= */

void CLibInfoMapObject::Delete(
	DWORD dwObjectID)		/* [in] �}�b�v�I�u�W�F�N�gID */
{
	int i, nCount, nNo;
	PCInfoMapObject pInfoTmp;

	nNo = -1;

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->GetAt (i);
		if (pInfoTmp->m_dwObjectID != dwObjectID) {
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
/* �֐���	:CLibInfoMapObject::DeleteAll									 */
/* ���e		:�S�č폜														 */
/* ���t		:2008/11/01														 */
/* ========================================================================= */

void CLibInfoMapObject::DeleteAll(void)
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
/* �֐���	:CLibInfoMapObject::Merge										 */
/* ���e		:��荞��														 */
/* ���t		:2008/11/01														 */
/* ========================================================================= */

void CLibInfoMapObject::Merge(CLibInfoMapObject *pSrc)
{
	int i, nCount;
	PCInfoMapObject pInfoTmp, pInfoSrc;

	nCount = pSrc->GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfoSrc = (PCInfoMapObject)pSrc->GetPtr (i);
		pInfoTmp = (PCInfoMapObject)GetPtr (pInfoSrc->m_dwObjectID);
		if (pInfoTmp == NULL) {
			pInfoTmp = (PCInfoMapObject)GetNew ();
			pInfoTmp->Copy (pInfoSrc);
			Add (pInfoTmp);
		}
		pInfoTmp->Copy (pInfoSrc);
	}
}


/* ========================================================================= */
/* �֐���	:CLibInfoMapObject::GetPtr										 */
/* ���e		:�����擾														 */
/* ���t		:2008/11/01														 */
/* ========================================================================= */

PCInfoBase CLibInfoMapObject::GetPtr(int nNo)
{
	return m_paInfo->GetAt (nNo);
}


/* ========================================================================= */
/* �֐���	:CLibInfoMapObject::GetPtr										 */
/* ���e		:�����擾														 */
/* ���t		:2008/11/01														 */
/* ========================================================================= */

PCInfoBase CLibInfoMapObject::GetPtr(
	DWORD dwObjectID)		/* [in] �}�b�v�I�u�W�F�N�gID */
{
	int i, nCount;
	PCInfoMapObject pRet, pInfoTmp;

	pRet = NULL;

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->GetAt (i);
		if (pInfoTmp->m_dwObjectID != dwObjectID) {
			continue;
		}
		pRet = pInfoTmp;
		break;
	}

	return pRet;
}


/* ========================================================================= */
/* �֐���	:CLibInfoMapObject::GetSendDataSize								 */
/* ���e		:���M�f�[�^�T�C�Y���擾											 */
/* ���t		:2008/11/01														 */
/* ========================================================================= */

DWORD CLibInfoMapObject::GetSendDataSize(void)
{
	int i, nCount;
	DWORD dwSize;
	PCInfoMapObject pInfoMapObject;

	dwSize = 0;

	nCount = GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfoMapObject = (PCInfoMapObject)GetPtr (i);
		dwSize += pInfoMapObject->GetSendDataSize ();
	}
	/* �I�[�p */
	dwSize += sizeof (DWORD);

	return dwSize;
}


/* ========================================================================= */
/* �֐���	:CLibInfoMapObject::GetSendData									 */
/* ���e		:���M�f�[�^���擾												 */
/* ���t		:2008/11/01														 */
/* ========================================================================= */

PBYTE CLibInfoMapObject::GetSendData(void)
{
	int i, nCount;
	PBYTE pData, pDataPos, pDataTmp;
	DWORD dwSize, dwSizeTmp;
	PCInfoMapObject pInfoMapObject;

	dwSize	= GetSendDataSize ();
	pData	= new BYTE[dwSize];
	ZeroMemory (pData, dwSize);

	pDataPos = pData;

	nCount = GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfoMapObject = (PCInfoMapObject)GetPtr (i);

		dwSizeTmp	= pInfoMapObject->GetSendDataSize ();
		pDataTmp	= pInfoMapObject->GetSendData ();
		CopyMemoryRenew (pDataPos, pDataTmp, dwSizeTmp, pDataPos);
		SAFE_DELETE_ARRAY (pDataTmp);
	}

	return pData;
}


/* ========================================================================= */
/* �֐���	:CLibInfoMapObject::SetSendData									 */
/* ���e		:���M�f�[�^�����荞��											 */
/* ���t		:2007/04/29														 */
/* ========================================================================= */

PBYTE CLibInfoMapObject::SetSendData(PBYTE pSrc)
{
	PBYTE pDataTmp;
	DWORD dwTmp;
	PCInfoMapObject pInfoMapObject, pInfoMapObjectTmp;

	pDataTmp = pSrc;

	while (1) {
		CopyMemory (&dwTmp, pDataTmp, sizeof (DWORD));
		if (dwTmp == 0) {
			pDataTmp += sizeof (DWORD);
			break;
		}
		pInfoMapObjectTmp = (PCInfoMapObject)GetNew ();
		pDataTmp = pInfoMapObjectTmp->SetSendData (pDataTmp);

		pInfoMapObject = (PCInfoMapObject)GetPtr (pInfoMapObjectTmp->m_dwObjectID);
		if (pInfoMapObject) {
			pInfoMapObject->Copy (pInfoMapObjectTmp);
			SAFE_DELETE (pInfoMapObjectTmp);
		} else {
			Add (pInfoMapObjectTmp);
		}
	}

	return pDataTmp;
}


/* ========================================================================= */
/* �֐���	:CLibInfoMapObject::GetNewID									 */
/* ���e		:�V����ID���擾													 */
/* ���t		:2008/11/01														 */
/* ========================================================================= */

DWORD CLibInfoMapObject::GetNewID(void)
{
	DWORD dwRet;
	int i, nCount;
	PCInfoMapObject pInfoTmp;

	dwRet = 1;

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->GetAt (i);
		if (pInfoTmp->m_dwObjectID == dwRet) {
			dwRet ++;
			i = -1;
			continue;
		}
	}

	return dwRet;
}

/* Copyright(C)URARA-works 2008 */
