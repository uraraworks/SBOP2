/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:LibInfoTalkEvent.cpp										 */
/* ���e			:��b�C�x���g��񃉃C�u�����N���X �����t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/12/27													 */
/* ========================================================================= */

#include "stdafx.h"
#include "LibInfoTalkEvent.h"


/* ========================================================================= */
/* �֐���	:CLibInfoTalkEvent::CLibInfoTalkEvent							 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/12/27														 */
/* ========================================================================= */

CLibInfoTalkEvent::CLibInfoTalkEvent()
{
	m_paInfo = NULL;
}


/* ========================================================================= */
/* �֐���	:CLibInfoTalkEvent::~CLibInfoTalkEvent							 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/12/27														 */
/* ========================================================================= */

CLibInfoTalkEvent::~CLibInfoTalkEvent()
{
	Destroy ();
}


/* ========================================================================= */
/* �֐���	:CLibInfoTalkEvent::Create										 */
/* ���e		:�쐬															 */
/* ���t		:2008/12/27														 */
/* ========================================================================= */

void CLibInfoTalkEvent::Create(void)
{
	m_paInfo = new ARRAYTALKEVENTINFO;
}


/* ========================================================================= */
/* �֐���	:CLibInfoTalkEvent::Destroy										 */
/* ���e		:�j��															 */
/* ���t		:2008/12/27														 */
/* ========================================================================= */

void CLibInfoTalkEvent::Destroy(void)
{
	DeleteAll ();
	SAFE_DELETE (m_paInfo);
}


/* ========================================================================= */
/* �֐���	:CLibInfoTalkEvent::GetNew										 */
/* ���e		:�V�K�f�[�^���擾												 */
/* ���t		:2008/12/07														 */
/* ========================================================================= */

PCInfoBase CLibInfoTalkEvent::GetNew(void)
{
	return (PCInfoBase)new CInfoTalkEvent;
}


/* ========================================================================= */
/* �֐���	:CLibInfoTalkEvent::GetCount									 */
/* ���e		:�f�[�^�����擾													 */
/* ���t		:2008/12/27														 */
/* ========================================================================= */

int CLibInfoTalkEvent::GetCount(void)
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
/* �֐���	:CLibInfoTalkEvent::Add											 */
/* ���e		:�ǉ�															 */
/* ���t		:2008/12/27														 */
/* ========================================================================= */

void CLibInfoTalkEvent::Add(PCInfoBase pInfo)
{
	PCInfoTalkEvent pInfoTalkEvent;

	pInfoTalkEvent = (PCInfoTalkEvent)pInfo;
	if (pInfoTalkEvent->m_dwTalkEventID == 0) {
		pInfoTalkEvent->m_dwTalkEventID = GetNewID ();
	}

	m_paInfo->Add (pInfoTalkEvent);
}


/* ========================================================================= */
/* �֐���	:CLibInfoTalkEvent::Delete										 */
/* ���e		:�폜															 */
/* ���t		:2008/12/27														 */
/* ========================================================================= */

void CLibInfoTalkEvent::Delete(
	int nNo)		/* [in] �z��ԍ� */
{
	PCInfoTalkEvent pInfo;

	pInfo = m_paInfo->GetAt (nNo);
	SAFE_DELETE (pInfo);
	m_paInfo->RemoveAt (nNo);
}


/* ========================================================================= */
/* �֐���	:CLibInfoTalkEvent::Delete										 */
/* ���e		:�폜															 */
/* ���t		:2008/12/27														 */
/* ========================================================================= */

void CLibInfoTalkEvent::Delete(
	DWORD dwTalkEventID)		/* [in] ��b�C�x���gID */
{
	int i, nCount, nNo;
	PCInfoTalkEvent pInfoTmp;

	nNo = -1;

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->GetAt (i);
		if (pInfoTmp->m_dwTalkEventID != dwTalkEventID) {
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
/* �֐���	:CLibInfoTalkEvent::DeleteAll									 */
/* ���e		:�S�č폜														 */
/* ���t		:2008/12/27														 */
/* ========================================================================= */

void CLibInfoTalkEvent::DeleteAll(void)
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
/* �֐���	:CLibInfoTalkEvent::Merge										 */
/* ���e		:��荞��														 */
/* ���t		:2008/12/27														 */
/* ========================================================================= */

void CLibInfoTalkEvent::Merge(CLibInfoTalkEvent *pSrc)
{
	int i, nCount;
	PCInfoTalkEvent pInfoTmp, pInfoSrc;

	nCount = pSrc->GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfoSrc = (PCInfoTalkEvent)pSrc->GetPtr (i);
		pInfoTmp = (PCInfoTalkEvent)GetPtr (pInfoSrc->m_dwTalkEventID);
		if (pInfoTmp == NULL) {
			pInfoTmp = (PCInfoTalkEvent)GetNew ();
			pInfoTmp->Copy (pInfoSrc);
			Add (pInfoTmp);
		}
		pInfoTmp->Copy (pInfoSrc);
	}
}


/* ========================================================================= */
/* �֐���	:CLibInfoTalkEvent::Renew										 */
/* ���e		:�X�V															 */
/* ���t		:2008/12/27														 */
/* ========================================================================= */

CInfoTalkEvent *CLibInfoTalkEvent::Renew(CInfoTalkEvent *pSrc)
{
	int i, nCount;
	PCInfoTalkEvent pRet, pInfo, pInfoTmp;

	pRet = NULL;

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->GetAt (i);
		if (pInfoTmp->m_dwTalkEventID != pSrc->m_dwTalkEventID) {
			continue;
		}
		pInfo = (PCInfoTalkEvent)GetNew ();
		pInfo->Copy (pSrc);
		pInfo->m_dwTalkEventID = pInfoTmp->m_dwTalkEventID;

		SAFE_DELETE (pInfoTmp);
		m_paInfo->SetAt (i, pInfo);
		pRet = pInfo;
		break;
	}

	return pRet;
}


/* ========================================================================= */
/* �֐���	:CLibInfoTalkEvent::GetPtr										 */
/* ���e		:�����擾														 */
/* ���t		:2008/12/27														 */
/* ========================================================================= */

PCInfoBase CLibInfoTalkEvent::GetPtr(int nNo)
{
	return m_paInfo->GetAt (nNo);
}


/* ========================================================================= */
/* �֐���	:CLibInfoTalkEvent::GetPtr										 */
/* ���e		:�����擾														 */
/* ���t		:2008/12/27														 */
/* ========================================================================= */

PCInfoBase CLibInfoTalkEvent::GetPtr(
	DWORD dwTalkEventID)		/* [in] ��b�C�x���gID */
{
	int i, nCount;
	PCInfoTalkEvent pRet, pInfoTmp;

	pRet = NULL;

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->GetAt (i);
		if (pInfoTmp->m_dwTalkEventID != dwTalkEventID) {
			continue;
		}
		pRet = pInfoTmp;
		break;
	}

	return pRet;
}


/* ========================================================================= */
/* �֐���	:CLibInfoTalkEvent::GetSendDataSize								 */
/* ���e		:���M�f�[�^�T�C�Y���擾											 */
/* ���t		:2008/12/27														 */
/* ========================================================================= */

DWORD CLibInfoTalkEvent::GetSendDataSize(void)
{
	int i, nCount;
	DWORD dwSize;
	PCInfoTalkEvent pInfoTalkEvent;

	dwSize = 0;

	nCount = GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfoTalkEvent = (PCInfoTalkEvent)GetPtr (i);
		dwSize += pInfoTalkEvent->GetSendDataSize ();
	}
	/* �I�[�p */
	dwSize += sizeof (DWORD);

	return dwSize;
}


/* ========================================================================= */
/* �֐���	:CLibInfoTalkEvent::GetSendData									 */
/* ���e		:���M�f�[�^���擾												 */
/* ���t		:2008/12/27														 */
/* ========================================================================= */

PBYTE CLibInfoTalkEvent::GetSendData(void)
{
	int i, nCount;
	PBYTE pData, pDataPos, pDataTmp;
	DWORD dwSize, dwSizeTmp;
	PCInfoTalkEvent pInfoTalkEvent;

	dwSize	= GetSendDataSize ();
	pData	= ZeroNew (dwSize);

	pDataPos = pData;

	nCount = GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfoTalkEvent = (PCInfoTalkEvent)GetPtr (i);

		dwSizeTmp	= pInfoTalkEvent->GetSendDataSize ();
		pDataTmp	= pInfoTalkEvent->GetSendData ();
		CopyMemoryRenew (pDataPos, pDataTmp, dwSizeTmp, pDataPos);
		SAFE_DELETE_ARRAY (pDataTmp);
	}

	return pData;
}


/* ========================================================================= */
/* �֐���	:CLibInfoTalkEvent::SetSendData									 */
/* ���e		:���M�f�[�^�����荞��											 */
/* ���t		:2008/12/27														 */
/* ========================================================================= */

PBYTE CLibInfoTalkEvent::SetSendData(PBYTE pSrc)
{
	PBYTE pDataTmp, pDataTmpBack;
	DWORD dwTmp;
	CInfoTalkEvent InfoTmp, *pInfoTalkEvent, *pInfoTalkEventTmp;

	pDataTmp = pSrc;

	while (1) {
		CopyMemory (&dwTmp, pDataTmp, sizeof (DWORD));
		if (dwTmp == 0) {
			pDataTmp += sizeof (DWORD);
			break;
		}
		pDataTmpBack = pDataTmp;
		InfoTmp.SetSendData (pDataTmp);
		pInfoTalkEventTmp = (PCInfoTalkEvent)GetNew ();
		pDataTmp = pInfoTalkEventTmp->SetSendData (pDataTmpBack);

		pInfoTalkEvent = (PCInfoTalkEvent)GetPtr (pInfoTalkEventTmp->m_dwTalkEventID);
		if (pInfoTalkEvent) {
			pInfoTalkEvent->Copy (pInfoTalkEventTmp);
			SAFE_DELETE (pInfoTalkEventTmp);
		} else {
			Add (pInfoTalkEventTmp);
		}
	}

	return pDataTmp;
}


/* ========================================================================= */
/* �֐���	:CLibInfoTalkEvent::GetNewID									 */
/* ���e		:�V����ID���擾													 */
/* ���t		:2008/12/27														 */
/* ========================================================================= */

DWORD CLibInfoTalkEvent::GetNewID(void)
{
	DWORD dwRet;
	int i, nCount;
	PCInfoTalkEvent pInfoTmp;

	dwRet = 1;

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->GetAt (i);
		if (pInfoTmp->m_dwTalkEventID == dwRet) {
			dwRet ++;
			i = -1;
			continue;
		}
	}

	return dwRet;
}

/* Copyright(C)URARA-works 2008 */
