/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:LibInfoMapEvent.cpp										 */
/* ���e			:�}�b�v�C�x���g��񃉃C�u�����N���X �����t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/06/22													 */
/* ========================================================================= */

#include "stdafx.h"
#include "InfoMapEvent.h"
#include "LibInfoMapEvent.h"


/* ========================================================================= */
/* �֐���	:CLibInfoMapEvent::CLibInfoMapEvent								 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/06/22														 */
/* ========================================================================= */

CLibInfoMapEvent::CLibInfoMapEvent()
{
	m_dwNewIDTmp	= 0;
	m_paInfo		= NULL;
}


/* ========================================================================= */
/* �֐���	:CLibInfoMapEvent::~CLibInfoMapEvent							 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/06/22														 */
/* ========================================================================= */

CLibInfoMapEvent::~CLibInfoMapEvent()
{
	Destroy ();
}


/* ========================================================================= */
/* �֐���	:CLibInfoMapEvent::Create										 */
/* ���e		:�쐬															 */
/* ���t		:2008/06/22														 */
/* ========================================================================= */

void CLibInfoMapEvent::Create(void)
{
	m_paInfo = new ARRAYMAPEVENTBASEINFO;
}


/* ========================================================================= */
/* �֐���	:CLibInfoMapEvent::Destroy										 */
/* ���e		:�j��															 */
/* ���t		:2008/06/22														 */
/* ========================================================================= */

void CLibInfoMapEvent::Destroy(void)
{
	DeleteAll ();
	SAFE_DELETE (m_paInfo);
}


/* ========================================================================= */
/* �֐���	:CLibInfoMapEvent::GetNew										 */
/* ���e		:�V�K�f�[�^���擾												 */
/* ���t		:2008/06/22														 */
/* ========================================================================= */

PCInfoBase CLibInfoMapEvent::GetNew(int nType)
{
	PCInfoMapEventBase pInfo;

	pInfo = NULL;

	switch (nType) {
	case MAPEVENTTYPE_MOVE:				/* �}�b�v���ړ� */
		pInfo = new CInfoMapEventMOVE;
		break;
	case MAPEVENTTYPE_MAPMOVE:			/* �}�b�v�Ԉړ� */
		pInfo = new CInfoMapEventMAPMOVE;
		break;
	case MAPEVENTTYPE_INITSTATUS:		/* �X�e�[�^�X������ */
		pInfo = new CInfoMapEventINITSTATUS;
		break;
	case MAPEVENTTYPE_GRPIDTMP:			/* �ꎞ�摜�ݒ� */
		pInfo = new CInfoMapEventGRPIDTMP;
		break;
	default:
		pInfo = new CInfoMapEventBase;
		break;
	}
	pInfo->m_nType = nType;

	return pInfo;
}


/* ========================================================================= */
/* �֐���	:CLibInfoMapEvent::GetCount										 */
/* ���e		:�f�[�^�����擾													 */
/* ���t		:2008/06/22														 */
/* ========================================================================= */

int CLibInfoMapEvent::GetCount(void)
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
/* �֐���	:CLibInfoMapEvent::Add											 */
/* ���e		:�ǉ�															 */
/* ���t		:2008/06/22														 */
/* ========================================================================= */

void CLibInfoMapEvent::Add(PCInfoBase pInfo)
{
	PCInfoMapEventBase pInfoMapEvent;

	pInfoMapEvent = (PCInfoMapEventBase)pInfo;
	if (pInfoMapEvent->m_dwMapEventID == 0) {
		pInfoMapEvent->m_dwMapEventID = GetNewID ();
	}
	m_paInfo->Add (pInfoMapEvent);
}


/* ========================================================================= */
/* �֐���	:CLibInfoMapEvent::Delete										 */
/* ���e		:�폜															 */
/* ���t		:2008/06/22														 */
/* ========================================================================= */

void CLibInfoMapEvent::Delete(
	int nNo)		/* [in] �z��ԍ� */
{
	PCInfoMapEventBase pInfo;

	pInfo = m_paInfo->GetAt (nNo);
	SAFE_DELETE (pInfo);
	m_paInfo->RemoveAt (nNo);
}


/* ========================================================================= */
/* �֐���	:CLibInfoMapEvent::Delete										 */
/* ���e		:�폜															 */
/* ���t		:2008/06/22														 */
/* ========================================================================= */

void CLibInfoMapEvent::Delete(DWORD dwMapEventID)
{
	int i, nCount;
	PCInfoMapEventBase pInfoTmp;

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->GetAt (i);
		if (pInfoTmp->m_dwMapEventID != dwMapEventID) {
			continue;
		}
		Delete (i);
		break;
	}
}


/* ========================================================================= */
/* �֐���	:CLibInfoMapEvent::DeleteAll									 */
/* ���e		:�S�č폜														 */
/* ���t		:2008/06/22														 */
/* ========================================================================= */

void CLibInfoMapEvent::DeleteAll(void)
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
/* �֐���	:CLibInfoMapEvent::Merge										 */
/* ���e		:��荞��														 */
/* ���t		:2008/06/25														 */
/* ========================================================================= */

void CLibInfoMapEvent::Merge(CLibInfoMapEvent *pSrc)
{
	int i, nCount;
	PCInfoMapEventBase pInfoTmp, pInfoSrc, pInfoDst;

	nCount = pSrc->GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfoSrc = (PCInfoMapEventBase)pSrc->GetPtr (i);
		pInfoTmp = (PCInfoMapEventBase)GetPtr (pInfoSrc->m_dwMapEventID);
		if (pInfoTmp == NULL) {
			pInfoTmp = (PCInfoMapEventBase)GetNew (pInfoSrc->m_nType);
			pInfoTmp->Copy (pInfoSrc);
			Add (pInfoTmp);
		} else {
			pInfoDst = (PCInfoMapEventBase)GetNew (pInfoSrc->m_nType);
			pInfoDst->Copy (pInfoSrc);
			Renew (pInfoSrc->m_dwMapEventID, pInfoDst);
		}
	}
}


/* ========================================================================= */
/* �֐���	:CLibInfoMapEvent::Renew										 */
/* ���e		:�X�V															 */
/* ���t		:2008/06/25														 */
/* ========================================================================= */

CInfoMapEventBase *CLibInfoMapEvent::Renew(DWORD dwMapEventID, CInfoMapEventBase *pSrc)
{
	int i, nCount;
	PCInfoMapEventBase pRet, pInfo, pInfoTmp;

	pRet = NULL;

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->GetAt (i);
		if (pInfoTmp->m_dwMapEventID != dwMapEventID) {
			continue;
		}
		pInfo = (PCInfoMapEventBase)GetNew (pSrc->m_nType);
		pInfo->Copy (pSrc);
		pInfo->m_dwMapEventID = pInfoTmp->m_dwMapEventID;

		SAFE_DELETE (pInfoTmp);
		m_paInfo->SetAt (i, pInfo);
		pRet = pInfo;
		break;
	}

	return pRet;
}


/* ========================================================================= */
/* �֐���	:CLibInfoMapEvent::GetPtr										 */
/* ���e		:�}�b�v�C�x���g�����擾										 */
/* ���t		:2008/06/22														 */
/* ========================================================================= */

PCInfoBase CLibInfoMapEvent::GetPtr(int nNo)
{
	return m_paInfo->GetAt (nNo);
}


/* ========================================================================= */
/* �֐���	:CLibInfoMapEvent::GetPtr										 */
/* ���e		:�}�b�v�C�x���g�����擾										 */
/* ���t		:2008/06/25														 */
/* ========================================================================= */

PCInfoBase CLibInfoMapEvent::GetPtr(DWORD dwMapEventID)
{
	int i, nCount;
	PCInfoMapEventBase pRet, pInfoTmp;

	pRet = NULL;

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->GetAt (i);
		if (pInfoTmp->m_dwMapEventID == dwMapEventID) {
			pRet = pInfoTmp;
			break;
		}
	}

	return pRet;
}


/* ========================================================================= */
/* �֐���	:CInfoMapEventMOVE::GetDataSize									 */
/* ���e		:�f�[�^�T�C�Y���擾												 */
/* ���t		:2008/06/22														 */
/* ========================================================================= */

DWORD CLibInfoMapEvent::GetDataSize(void)
{
	int i, j, nCount, nCount2;
	DWORD dwRet;
	PCInfoMapEventBase pInfo;

	dwRet = 0;

	dwRet += sizeof (int);									/* �f�[�^�� */
	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfo = m_paInfo->GetAt (i);
		dwRet += sizeof (int);								/* �v�f�� */
		nCount2 = pInfo->GetElementCount ();
		for (j = 0; j < nCount2; j ++) {
			dwRet += (strlen (pInfo->GetName (j)) + 1);		/* �v�f�� */
			dwRet += sizeof (DWORD);						/* �f�[�^�T�C�Y */
			dwRet += pInfo->GetDataSizeNo (j);				/* �f�[�^ */
		}
	}

	return dwRet;
}


/* ========================================================================= */
/* �֐���	:CLibInfoMapEvent::GetWriteData									 */
/* ���e		:�w��v�f�̕ۑ��p�f�[�^���擾									 */
/* ���t		:2008/06/22														 */
/* ========================================================================= */

PBYTE CLibInfoMapEvent::GetWriteData(PDWORD pdwSize)
{
	int i, j, nCount, nCount2;
	PBYTE pRet, pRetTmp, pTmp;
	DWORD dwSize, dwTmp;
	PCInfoMapEventBase pInfo;

	pRet		= NULL;
	dwSize		= GetDataSize ();
	*pdwSize	= dwSize;

	if (dwSize == 0) {
		goto Exit;
	}

	pRet = ZeroNew (dwSize);
	pRetTmp = pRet;

	nCount = m_paInfo->GetSize ();
	CopyMemoryRenew (pRetTmp, &nCount, sizeof (nCount), pRetTmp);		/* �f�[�^�� */
	for (i = 0; i < nCount; i ++) {
		pInfo = m_paInfo->GetAt (i);
		nCount2 = pInfo->GetElementCount ();
		CopyMemoryRenew (pRetTmp, &nCount2, sizeof (nCount2), pRetTmp);	/* �v�f�� */

		for (j = 0; j < nCount2; j ++) {
			strcpyRenew ((LPSTR)pRetTmp, pInfo->GetName (j), pRetTmp); 	/* �v�f�� */
			dwTmp = pInfo->GetDataSizeNo (j);
			CopyMemoryRenew (pRetTmp, &dwTmp, sizeof (dwTmp), pRetTmp);	/* �f�[�^�T�C�Y */
			pTmp = pInfo->GetWriteData (j, &dwTmp);
			CopyMemoryRenew (pRetTmp, pTmp, dwTmp, pRetTmp);			/* �f�[�^ */
			SAFE_DELETE_ARRAY (pTmp);
		}
	}

Exit:
	return pRet;
}


/* ========================================================================= */
/* �֐���	:CLibInfoMapEvent::ReadElementData								 */
/* ���e		:�w��v�f�f�[�^��ǂݍ���										 */
/* ���t		:2005/07/05														 */
/* ========================================================================= */

DWORD CLibInfoMapEvent::ReadElementData(PBYTE pSrc)
{
	int i, j, nNo, nCount, nCount2, nType;
	PBYTE pDst, pSrcTmp;
	DWORD dwSize, dwSizeTmp;
	PCInfoMapEventBase pInfo;
	CmyString strTmp;

	pDst	= NULL;
	dwSize	= 0;

	DeleteAll ();

	pSrcTmp = pSrc;
	CopyMemoryRenew (&nCount, pSrcTmp, sizeof (nCount), pSrcTmp);				/* �f�[�^�� */
	for (i = 0; i < nCount; i ++) {
		CopyMemoryRenew (&nCount2, pSrcTmp, sizeof (nCount2), pSrcTmp);			/* �v�f�� */

		for (j = 0; j < nCount2; j ++) {
			StoreRenew (strTmp, (LPCSTR)pSrcTmp, pSrcTmp);						/* �v�f�� */
			CopyMemoryRenew (&dwSizeTmp, pSrcTmp, sizeof (dwSizeTmp), pSrcTmp);	/* �f�[�^�T�C�Y */
			if (j == 0) {
				/* �ŏ��̓C�x���g��� */
				CopyMemoryRenew (&nType, pSrcTmp, sizeof (nType), pSrcTmp);		/* �C�x���g��� */
				pInfo = (PCInfoMapEventBase)GetNew (nType);
			} else {
				nNo = pInfo->GetElementNo ((LPCSTR)strTmp);
				if (nNo >= 0) {
					dwSizeTmp = pInfo->ReadElementData (pSrcTmp, nNo);
				}
				pSrcTmp += dwSizeTmp;
			}
		}
		Add (pInfo);
	}

	return (DWORD)(pSrcTmp - pSrc);
}


/* ========================================================================= */
/* �֐���	:CLibInfoMapEvent::GetSendDataSize								 */
/* ���e		:���M�f�[�^�T�C�Y���擾											 */
/* ���t		:2007/05/05														 */
/* ========================================================================= */

DWORD CLibInfoMapEvent::GetSendDataSize(void)
{
	int i, nCount;
	DWORD dwRet, dwSize;
	PCInfoMapEventBase pInfo;

	dwRet = dwSize = 0;

	/* �f�[�^�����̃T�C�Y */
	dwSize += sizeof (DWORD);

	nCount = GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfo = (PCInfoMapEventBase)GetPtr (i);

		dwSize += pInfo->GetSendDataSize ();
	}

	dwRet = dwSize;
	return dwRet;
}


/* ========================================================================= */
/* �֐���	:CLibInfoMapEvent::GetSendData									 */
/* ���e		:���M�f�[�^���擾												 */
/* ���t		:2007/05/05														 */
/* ========================================================================= */

PBYTE CLibInfoMapEvent::GetSendData(void)
{
	int i, nCount;
	PBYTE pRet, pData, pDataTmp;
	DWORD dwCount, dwSize, dwSizeTmp, dwOffset;
	PCInfoMapEventBase pInfo;

	pRet = NULL;

	dwSize		= GetSendDataSize ();
	pData		= ZeroNew (dwSize);
	dwOffset	= 0;

	/* �f�[�^������������ */
	dwCount = (DWORD)GetCount ();
	CopyMemory (pData, &dwCount, sizeof (dwCount));
	dwOffset += sizeof (dwCount);

	/* �C�x���g������������ */
	nCount = (int)dwCount;
	for (i = 0; i < nCount; i ++) {
		pInfo = (PCInfoMapEventBase)GetPtr (i);

		dwSizeTmp	= pInfo->GetSendDataSize ();
		pDataTmp	= pInfo->GetSendData ();
		CopyMemory (&pData[dwOffset], pDataTmp, dwSizeTmp);
		dwOffset += dwSizeTmp;

		SAFE_DELETE_ARRAY (pDataTmp);
	}

	pRet = pData;
	return pRet;
}


/* ========================================================================= */
/* �֐���	:CLibInfoMapEvent::SetSendData									 */
/* ���e		:���M�f�[�^�����荞��											 */
/* ���t		:2007/05/05														 */
/* �߂�l	:����������̃A�h���X											 */
/* ========================================================================= */

PBYTE CLibInfoMapEvent::SetSendData(PBYTE pSrc)
{
	int i, nCount;
	DWORD dwCount;
	PBYTE pRet, pDataTmp, pDataTmpBack;
	CInfoMapEventBase InfoTmp, *pInfo;

	pRet		= pSrc;
	pDataTmp	= pSrc;

	DeleteAll ();

	/* �f�[�^����ǂݍ��� */
	CopyMemory (&dwCount, pDataTmp, sizeof (dwCount));
	nCount		= (int)dwCount;
	pDataTmp	+= sizeof (dwCount);

	for (i = 0; i < nCount; i ++) {
		pDataTmpBack = pDataTmp;
		InfoTmp.SetSendData (pDataTmp);
		pInfo = (PCInfoMapEventBase)GetNew (InfoTmp.m_nType);
		pDataTmp = pDataTmpBack;
		pDataTmp = pInfo->SetSendData (pDataTmp);
		Add (pInfo);
	}

	pRet = pDataTmp;
	return pRet;
}


/* ========================================================================= */
/* �֐���	:CLibInfoMapEvent::GetNewID										 */
/* ���e		:�V�����A�C�e��ID���擾											 */
/* ���t		:2007/05/05														 */
/* ========================================================================= */

DWORD CLibInfoMapEvent::GetNewID(void)
{
	DWORD dwRet;
	int i, nCount;
	PCInfoMapEventBase pInfoTmp;

	dwRet = m_dwNewIDTmp + 1;
	if (dwRet == 0) {
		dwRet = 1;
	}

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->GetAt (i);
		if (pInfoTmp->m_dwMapEventID == dwRet) {
			dwRet ++;
			i = -1;
			continue;
		}
	}
	m_dwNewIDTmp = dwRet;

	return dwRet;
}

/* Copyright(C)URARA-works 2008 */
