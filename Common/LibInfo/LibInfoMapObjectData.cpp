/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:LibInfoMapObjectData.cpp									 */
/* ���e			:�}�b�v�I�u�W�F�N�g�z�u�f�[�^���C�u�����N���X �����t�@�C��	 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/11/03													 */
/* ========================================================================= */

#include "stdafx.h"
#include "InfoMapObjectData.h"
#include "LibInfoMapObjectData.h"


/* ========================================================================= */
/* �֐���	:CLibInfoMapObjectData::CLibInfoMapObjectData					 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/11/03														 */
/* ========================================================================= */

CLibInfoMapObjectData::CLibInfoMapObjectData()
{
	m_dwNewIDTmp	= 0;
	m_paInfo		= NULL;
}


/* ========================================================================= */
/* �֐���	:CLibInfoMapObjectData::~CLibInfoMapObjectData					 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/11/03														 */
/* ========================================================================= */

CLibInfoMapObjectData::~CLibInfoMapObjectData()
{
	Destroy ();
}


/* ========================================================================= */
/* �֐���	:CLibInfoMapObjectData::Create									 */
/* ���e		:�쐬															 */
/* ���t		:2008/11/03														 */
/* ========================================================================= */

void CLibInfoMapObjectData::Create(void)
{
	m_paInfo = new ARRAYMAPOBJECTDATAINFO;
}


/* ========================================================================= */
/* �֐���	:CLibInfoMapObjectData::Destroy									 */
/* ���e		:�j��															 */
/* ���t		:2008/11/03														 */
/* ========================================================================= */

void CLibInfoMapObjectData::Destroy(void)
{
	DeleteAll ();
	SAFE_DELETE (m_paInfo);
}


/* ========================================================================= */
/* �֐���	:CLibInfoMapObjectData::GetNew									 */
/* ���e		:�V�K�f�[�^���擾												 */
/* ���t		:2008/11/03														 */
/* ========================================================================= */

PCInfoBase CLibInfoMapObjectData::GetNew(void)
{
	PCInfoMapObjectData pInfo;

	pInfo = new CInfoMapObjectData;

	return pInfo;
}


/* ========================================================================= */
/* �֐���	:CLibInfoMapObjectData::GetCount								 */
/* ���e		:�f�[�^�����擾													 */
/* ���t		:2008/11/03														 */
/* ========================================================================= */

int CLibInfoMapObjectData::GetCount(void)
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
/* �֐���	:CLibInfoMapObjectData::Add										 */
/* ���e		:�ǉ�															 */
/* ���t		:2008/11/03														 */
/* ========================================================================= */

void CLibInfoMapObjectData::Add(PCInfoBase pInfo)
{
	PCInfoMapObjectData pInfoMapObjectData;

	pInfoMapObjectData = (PCInfoMapObjectData)pInfo;
	if (pInfoMapObjectData->m_dwDataID == 0) {
		pInfoMapObjectData->m_dwDataID = GetNewID ();
	}
	m_paInfo->Add (pInfoMapObjectData);
}


/* ========================================================================= */
/* �֐���	:CLibInfoMapObjectData::Delete									 */
/* ���e		:�폜															 */
/* ���t		:2008/11/03														 */
/* ========================================================================= */

void CLibInfoMapObjectData::Delete(
	int nNo)		/* [in] �z��ԍ� */
{
	PCInfoMapObjectData pInfo;

	pInfo = m_paInfo->GetAt (nNo);
	SAFE_DELETE (pInfo);
	m_paInfo->RemoveAt (nNo);
}


/* ========================================================================= */
/* �֐���	:CLibInfoMapObjectData::Delete									 */
/* ���e		:�폜															 */
/* ���t		:2008/11/03														 */
/* ========================================================================= */

void CLibInfoMapObjectData::Delete(DWORD dwDataID)
{
	int i, nCount;
	PCInfoMapObjectData pInfoTmp;

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->GetAt (i);
		if (pInfoTmp->m_dwDataID != dwDataID) {
			continue;
		}
		Delete (i);
		break;
	}
}


/* ========================================================================= */
/* �֐���	:CLibInfoMapObjectData::DeleteAll								 */
/* ���e		:�S�č폜														 */
/* ���t		:2008/11/03														 */
/* ========================================================================= */

void CLibInfoMapObjectData::DeleteAll(void)
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
/* �֐���	:CLibInfoMapObjectData::Merge									 */
/* ���e		:��荞��														 */
/* ���t		:2008/06/25														 */
/* ========================================================================= */

void CLibInfoMapObjectData::Merge(CLibInfoMapObjectData *pSrc)
{
	int i, nCount;
	PCInfoMapObjectData pInfoTmp, pInfoSrc, pInfoDst;

	nCount = pSrc->GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfoSrc = (PCInfoMapObjectData)pSrc->GetPtr (i);
		pInfoTmp = (PCInfoMapObjectData)GetPtr (pInfoSrc->m_dwDataID);
		if (pInfoTmp == NULL) {
			pInfoTmp = (PCInfoMapObjectData)GetNew ();
			pInfoTmp->Copy (pInfoSrc);
			Add (pInfoTmp);
		} else {
			pInfoDst = (PCInfoMapObjectData)GetNew ();
			pInfoDst->Copy (pInfoSrc);
			Renew (pInfoSrc->m_dwDataID, pInfoDst);
		}
	}
}


/* ========================================================================= */
/* �֐���	:CLibInfoMapObjectData::Renew									 */
/* ���e		:�X�V															 */
/* ���t		:2008/06/25														 */
/* ========================================================================= */

CInfoMapObjectData *CLibInfoMapObjectData::Renew(DWORD dwDataID, CInfoMapObjectData *pSrc)
{
	int i, nCount;
	PCInfoMapObjectData pRet, pInfo, pInfoTmp;

	pRet = NULL;

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->GetAt (i);
		if (pInfoTmp->m_dwDataID != dwDataID) {
			continue;
		}
		pInfo = (PCInfoMapObjectData)GetNew ();
		pInfo->Copy (pSrc);
		pInfo->m_dwDataID = pInfoTmp->m_dwDataID;

		SAFE_DELETE (pInfoTmp);
		m_paInfo->SetAt (i, pInfo);
		pRet = pInfo;
		break;
	}

	return pRet;
}


/* ========================================================================= */
/* �֐���	:CLibInfoMapObjectData::GetPtr									 */
/* ���e		:�z�u�f�[�^���擾												 */
/* ���t		:2008/11/03														 */
/* ========================================================================= */

PCInfoBase CLibInfoMapObjectData::GetPtr(int nNo)
{
	return m_paInfo->GetAt (nNo);
}


/* ========================================================================= */
/* �֐���	:CLibInfoMapObjectData::GetPtr									 */
/* ���e		:�z�u�f�[�^���擾												 */
/* ���t		:2008/11/03														 */
/* ========================================================================= */

PCInfoBase CLibInfoMapObjectData::GetPtr(DWORD dwDataID)
{
	int i, nCount;
	PCInfoMapObjectData pRet, pInfoTmp;

	pRet = NULL;

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->GetAt (i);
		if (pInfoTmp->m_dwDataID == dwDataID) {
			pRet = pInfoTmp;
			break;
		}
	}

	return pRet;
}


/* ========================================================================= */
/* �֐���	:CInfoMapObjectDataMOVE::GetDataSize							 */
/* ���e		:�f�[�^�T�C�Y���擾												 */
/* ���t		:2008/11/03														 */
/* ========================================================================= */

DWORD CLibInfoMapObjectData::GetDataSize(void)
{
	int i, j, nCount, nCount2;
	DWORD dwRet;
	PCInfoMapObjectData pInfo;

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
/* �֐���	:CLibInfoMapObjectData::GetWriteData							 */
/* ���e		:�w��v�f�̕ۑ��p�f�[�^���擾									 */
/* ���t		:2008/11/03														 */
/* ========================================================================= */

PBYTE CLibInfoMapObjectData::GetWriteData(PDWORD pdwSize)
{
	int i, j, nCount, nCount2;
	PBYTE pRet, pRetTmp, pTmp;
	DWORD dwSize, dwTmp;
	PCInfoMapObjectData pInfo;

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
/* �֐���	:CLibInfoMapObjectData::ReadElementData							 */
/* ���e		:�w��v�f�f�[�^��ǂݍ���										 */
/* ���t		:2008/11/03														 */
/* ========================================================================= */

DWORD CLibInfoMapObjectData::ReadElementData(PBYTE pSrc)
{
	int i, j, nNo, nCount, nCount2;
	PBYTE pDst, pSrcTmp;
	DWORD dwSize, dwSizeTmp;
	PCInfoMapObjectData pInfo;
	CmyString strTmp;

	pDst	= NULL;
	dwSize	= 0;

	DeleteAll ();

	pSrcTmp = pSrc;
	CopyMemoryRenew (&nCount, pSrcTmp, sizeof (nCount), pSrcTmp);				/* �f�[�^�� */
	for (i = 0; i < nCount; i ++) {
		CopyMemoryRenew (&nCount2, pSrcTmp, sizeof (nCount2), pSrcTmp);			/* �v�f�� */

		pInfo = (PCInfoMapObjectData)GetNew ();
		for (j = 0; j < nCount2; j ++) {
			StoreRenew (strTmp, (LPCSTR)pSrcTmp, pSrcTmp);						/* �v�f�� */
			CopyMemoryRenew (&dwSizeTmp, pSrcTmp, sizeof (dwSizeTmp), pSrcTmp);	/* �f�[�^�T�C�Y */
			nNo = pInfo->GetElementNo ((LPCSTR)strTmp);
			if (nNo >= 0) {
				dwSizeTmp = pInfo->ReadElementData (pSrcTmp, nNo);
			}
			pSrcTmp += dwSizeTmp;
		}
		Add (pInfo);
	}

	return (DWORD)(pSrcTmp - pSrc);
}


/* ========================================================================= */
/* �֐���	:CLibInfoMapObjectData::GetSendDataSize							 */
/* ���e		:���M�f�[�^�T�C�Y���擾											 */
/* ���t		:2008/11/03														 */
/* ========================================================================= */

DWORD CLibInfoMapObjectData::GetSendDataSize(void)
{
	int i, nCount;
	DWORD dwRet, dwSize;
	PCInfoMapObjectData pInfo;

	dwRet = dwSize = 0;

	/* �f�[�^�����̃T�C�Y */
	dwSize += sizeof (DWORD);

	nCount = GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfo = (PCInfoMapObjectData)GetPtr (i);

		dwSize += pInfo->GetSendDataSize ();
	}

	dwRet = dwSize;
	return dwRet;
}


/* ========================================================================= */
/* �֐���	:CLibInfoMapObjectData::GetSendData								 */
/* ���e		:���M�f�[�^���擾												 */
/* ���t		:2008/11/03														 */
/* ========================================================================= */

PBYTE CLibInfoMapObjectData::GetSendData(void)
{
	int i, nCount;
	PBYTE pRet, pData, pDataTmp;
	DWORD dwCount, dwSize, dwSizeTmp, dwOffset;
	PCInfoMapObjectData pInfo;

	pRet = NULL;

	dwSize		= GetSendDataSize ();
	pData		= ZeroNew (dwSize);
	dwOffset	= 0;

	/* �f�[�^������������ */
	dwCount = (DWORD)GetCount ();
	CopyMemory (pData, &dwCount, sizeof (dwCount));
	dwOffset += sizeof (dwCount);

	/* �z�u�f�[�^���������� */
	nCount = (int)dwCount;
	for (i = 0; i < nCount; i ++) {
		pInfo = (PCInfoMapObjectData)GetPtr (i);

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
/* �֐���	:CLibInfoMapObjectData::SetSendData								 */
/* ���e		:���M�f�[�^�����荞��											 */
/* ���t		:2008/11/03														 */
/* �߂�l	:����������̃A�h���X											 */
/* ========================================================================= */

PBYTE CLibInfoMapObjectData::SetSendData(PBYTE pSrc)
{
	int i, nCount;
	DWORD dwCount;
	PBYTE pRet, pDataTmp, pDataTmpBack;
	CInfoMapObjectData InfoTmp, *pInfo;

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
		pInfo = (PCInfoMapObjectData)GetNew ();
		pDataTmp = pDataTmpBack;
		pDataTmp = pInfo->SetSendData (pDataTmp);
		Add (pInfo);
	}

	pRet = pDataTmp;
	return pRet;
}


/* ========================================================================= */
/* �֐���	:CLibInfoMapObjectData::GetNewID								 */
/* ���e		:�V����ID���擾													 */
/* ���t		:2008/11/03														 */
/* ========================================================================= */

DWORD CLibInfoMapObjectData::GetNewID(void)
{
	DWORD dwRet;
	int i, nCount;
	PCInfoMapObjectData pInfoTmp;

	dwRet = m_dwNewIDTmp + 1;
	if (dwRet == 0) {
		dwRet = 1;
	}

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->GetAt (i);
		if (pInfoTmp->m_dwDataID == dwRet) {
			dwRet ++;
			i = -1;
			continue;
		}
	}
	m_dwNewIDTmp = dwRet;

	return dwRet;
}

/* Copyright(C)URARA-works 2008 */
