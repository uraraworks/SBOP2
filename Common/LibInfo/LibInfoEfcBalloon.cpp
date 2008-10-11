/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:LibInfoEfcBalloon.cpp										 */
/* ���e			:���o����񃉃C�u�������N���X �����t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/12/24													 */
/* ========================================================================= */

#include "stdafx.h"
#include "InfoEfcBalloon.h"
#include "LibInfoEfcBalloon.h"


/* ========================================================================= */
/* �֐���	:CLibInfoEfcBalloon::CLibInfoEfcBalloon							 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/12/24														 */
/* ========================================================================= */

CLibInfoEfcBalloon::CLibInfoEfcBalloon()
{
	m_dwNewIDTmp	= 0;
	m_paInfo		= NULL;
}


/* ========================================================================= */
/* �֐���	:CLibInfoEfcBalloon::~CLibInfoEfcBalloon						 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/12/24														 */
/* ========================================================================= */

CLibInfoEfcBalloon::~CLibInfoEfcBalloon()
{
	Destroy ();
}


/* ========================================================================= */
/* �֐���	:CLibInfoEfcBalloon::Create										 */
/* ���e		:�쐬															 */
/* ���t		:2007/12/24														 */
/* ========================================================================= */

void CLibInfoEfcBalloon::Create(void)
{
	m_paInfo = new ARRAYEFCBALLOON;
}


/* ========================================================================= */
/* �֐���	:CLibInfoEfcBalloon::Destroy									 */
/* ���e		:�j��															 */
/* ���t		:2007/12/24														 */
/* ========================================================================= */

void CLibInfoEfcBalloon::Destroy(void)
{
	DeleteAll ();
	SAFE_DELETE (m_paInfo);
}


/* ========================================================================= */
/* �֐���	:CLibInfoEfcBalloon::GetNew										 */
/* ���e		:�V�K�f�[�^���擾												 */
/* ���t		:2007/12/24														 */
/* ========================================================================= */

PCInfoBase CLibInfoEfcBalloon::GetNew(void)
{
	PCInfoEfcBalloon pInfoEfcBalloon;

	pInfoEfcBalloon = new CInfoEfcBalloon;

	return pInfoEfcBalloon;
}


/* ========================================================================= */
/* �֐���	:CLibInfoEfcBalloon::GetPtr										 */
/* ���e		:�����擾														 */
/* ���t		:2007/12/24														 */
/* ========================================================================= */

PCInfoBase CLibInfoEfcBalloon::GetPtr(int nNo)
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
/* �֐���	:CLibInfoEfcBalloon::GetCount									 */
/* ���e		:�f�[�^�����擾													 */
/* ���t		:2007/12/24														 */
/* ========================================================================= */

int CLibInfoEfcBalloon::GetCount(void)
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
/* �֐���	:CLibInfoEfcBalloon::Add										 */
/* ���e		:�ǉ�															 */
/* ���t		:2007/12/24														 */
/* ========================================================================= */

void CLibInfoEfcBalloon::Add(PCInfoBase pInfo)
{
	PCInfoEfcBalloon pInfoEfcBalloon;

	pInfoEfcBalloon = (PCInfoEfcBalloon)pInfo;
	if (pInfoEfcBalloon->m_dwEfcBalloonID == 0) {
		pInfoEfcBalloon->m_dwEfcBalloonID = GetNewID ();
	}

	m_paInfo->Add (pInfoEfcBalloon);
}


/* ========================================================================= */
/* �֐���	:CLibInfoEfcBalloon::Delete										 */
/* ���e		:�폜															 */
/* ���t		:2007/12/24														 */
/* ========================================================================= */

void CLibInfoEfcBalloon::Delete(
	int nNo)		/* [in] �z��ԍ� */
{
	PCInfoEfcBalloon pInfo;

	pInfo = m_paInfo->GetAt (nNo);
	SAFE_DELETE (pInfo);
	m_paInfo->RemoveAt (nNo);
}


/* ========================================================================= */
/* �֐���	:CLibInfoEfcBalloon::Delete										 */
/* ���e		:�폜															 */
/* ���t		:2007/12/24														 */
/* ========================================================================= */

void CLibInfoEfcBalloon::Delete(
	DWORD dwEfcBalloonID)		/* [in] ���[�V����ID */
{
	int i, nCount, nNo;
	PCInfoEfcBalloon pInfoTmp;

	nNo = -1;

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->GetAt (i);
		if (pInfoTmp->m_dwEfcBalloonID != dwEfcBalloonID) {
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
/* �֐���	:CLibInfoEfcBalloon::DeleteAll									 */
/* ���e		:�S�č폜														 */
/* ���t		:2007/12/24														 */
/* ========================================================================= */

void CLibInfoEfcBalloon::DeleteAll(void)
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
/* �֐���	:CLibInfoEfcBalloon::SetList									 */
/* ���e		:�w��ID�̃��X�g��ݒ�											 */
/* ���t		:2007/12/24														 */
/* ========================================================================= */

void CLibInfoEfcBalloon::SetList(DWORD dwListID, CLibInfoEfcBalloon *pSrc)
{
	int i, nCount;
	PCInfoEfcBalloon pInfo, pInfoTmp;

	/* �܂��͑ΏۂƂȂ郊�X�gID�̏����폜 */
	nCount = m_paInfo->GetSize ();
	for (i = nCount - 1; i >= 0; i --) {
		pInfo = m_paInfo->GetAt (i);
		if (dwListID != 0) {
			if (pInfo->m_dwListID != dwListID) {
				continue;
			}
		}
		Delete (i);
	}

	nCount = pSrc->GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfo = (PCInfoEfcBalloon)pSrc->GetPtr (i);
		if (dwListID != 0) {
			if (pInfo->m_dwListID != dwListID) {
				continue;
			}
		}
		pInfoTmp = new CInfoEfcBalloon;
		pInfoTmp->Copy (pInfo);
		Add (pInfoTmp);
	}

	Sort ();
}


/* ========================================================================= */
/* �֐���	:CLibInfoEfcBalloon::GetListID									 */
/* ���e		:���ID�ꗗ���擾												 */
/* ���t		:2007/12/29														 */
/* ========================================================================= */

void CLibInfoEfcBalloon::GetListID(ARRAYDWORD &aDst)
{
	int i, j, nCount, nCountTmp;
	PCInfoEfcBalloon pInfo;

	aDst.RemoveAll ();

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfo = m_paInfo->GetAt (i);

		nCountTmp = aDst.GetSize ();
		for (j = 0; j < nCountTmp; j ++) {
			if (pInfo->m_dwListID == aDst[j]) {
				break;
			}
		}
		if (j < nCountTmp) {
			continue;
		}
		aDst.Add (pInfo->m_dwListID);
	}
}


/* ========================================================================= */
/* �֐���	:CLibInfoEfcBalloon::GetName									 */
/* ���e		:���o�������擾													 */
/* ���t		:2007/12/29														 */
/* ========================================================================= */

void CLibInfoEfcBalloon::GetName(
	DWORD dwListID,			/* [in] ���ID */
	CmyString &strDst)		/* [ou] ���o���� */
{
	int i, nCount;
	PCInfoEfcBalloon pInfo;

	strDst.Empty ();

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfo = m_paInfo->GetAt (i);
		if (pInfo->m_dwListID != dwListID) {
			continue;
		}
		strDst = pInfo->m_strName;
		break;
	}
}


/* ========================================================================= */
/* �֐���	:CLibInfoEfcBalloon::Sort										 */
/* ���e		:���ID�Ń\�[�g													 */
/* ���t		:2008/01/01														 */
/* ========================================================================= */

void CLibInfoEfcBalloon::Sort(void)
{
	int i, nCount;
	DWORD dwListID;
	PCInfoEfcBalloon pInfo;
	ARRAYEFCBALLOON apInfo;

	apInfo.Copy (m_paInfo);
	m_paInfo->RemoveAll ();

	while (1) {
		nCount = apInfo.GetSize ();
		if (nCount <= 0) {
			break;
		}
		dwListID = -1;
		/* �ŏ��̎��ID��T�� */
		for (i = 0; i < nCount; i ++) {
			pInfo = apInfo[i];
			if (pInfo->m_dwListID > dwListID) {
				continue;
			}
			dwListID = pInfo->m_dwListID;
		}

		for (i = 0; i < nCount; i ++) {
			pInfo = apInfo[i];
			if (pInfo->m_dwListID != dwListID) {
				continue;
			}
			m_paInfo->Add (pInfo);
			apInfo.RemoveAt (i);
			i --;
			nCount = apInfo.GetSize ();
		}
	}
}


/* ========================================================================= */
/* �֐���	:CLibInfoEfcBalloon::GetNextAnimeID								 */
/* ���e		:���̃R�}�ԍ����擾												 */
/* ���t		:2007/12/31														 */
/* ========================================================================= */

DWORD CLibInfoEfcBalloon::GetNextAnimeID(
	DWORD dwListID,			/* [in] ���ID */
	DWORD dwAnimeID)		/* [in] �R�}�ԍ� */
{
	int i, nCount;
	DWORD dwRet;
	ARRAYEFCBALLOON apBalloon;
	PCInfoEfcBalloon pInfo;

	dwRet = -1;

	GetEfcBalloonInfo (dwListID, apBalloon);
	nCount = apBalloon.GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfo = apBalloon[i];
		if (dwAnimeID + 1 != pInfo->m_dwAnimeID) {
			continue;
		}
		break;
	}
	/* ����������H */
	if (i < nCount) {
		dwRet = dwAnimeID + 1;
	}

	return dwRet;
}


/* ========================================================================= */
/* �֐���	:CLibInfoEfcBalloon::GetPtr										 */
/* ���e		:���o�������擾												 */
/* ���t		:2007/12/31														 */
/* ========================================================================= */

CInfoEfcBalloon *CLibInfoEfcBalloon::GetPtr(
	DWORD dwListID,			/* [in] ���ID */
	DWORD dwAnimeID)		/* [in] �R�}�ԍ� */
{
	int i, nCount;
	PCInfoEfcBalloon pRet, pInfo;
	ARRAYEFCBALLOON apBalloon;

	pRet = NULL;
	if (dwListID == 0) {
		goto Exit;
	}
	GetEfcBalloonInfo (dwListID, apBalloon);

	nCount = apBalloon.GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfo = apBalloon[i];
		if (dwAnimeID != pInfo->m_dwAnimeID) {
			continue;
		}
		pRet = pInfo;
		break;
	}

Exit:
	return pRet;
}


/* ========================================================================= */
/* �֐���	:CLibInfoEfcBalloon::GetEfcBalloonInfo							 */
/* ���e		:�w����ID�̏���ݒ�											 */
/* ���t		:2007/12/24														 */
/* ========================================================================= */

void CLibInfoEfcBalloon::GetEfcBalloonInfo(DWORD dwListID, ARRAYEFCBALLOON &aDst)
{
	int i, nCount;
	PCInfoEfcBalloon pInfo;

	if (dwListID == 0) {
		return;
	}
	aDst.RemoveAll ();

	nCount = GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfo = (PCInfoEfcBalloon)GetPtr (i);
		if (dwListID != 0) {
			if (pInfo->m_dwListID != dwListID) {
				continue;
			}
		}
		aDst.Add (pInfo);
	}
}


/* ========================================================================= */
/* �֐���	:CLibInfoEfcBalloon::GetSendDataSize							 */
/* ���e		:���M�f�[�^�T�C�Y���擾											 */
/* ���t		:2007/12/24														 */
/* ========================================================================= */

DWORD CLibInfoEfcBalloon::GetSendDataSize(DWORD dwListID)
{
	DWORD dwRet;
	int i, nCount;
	PCInfoEfcBalloon pInfo;

	dwRet = 0;

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfo = m_paInfo->GetAt (i);
		if (dwListID > 0) {
			if (pInfo->m_dwListID != dwListID) {
				continue;
			}
		}
		dwRet += pInfo->GetSendDataSize ();
	}

	dwRet += sizeof (DWORD);	/* �I�[�� */

	return dwRet;
}


/* ========================================================================= */
/* �֐���	:CLibInfoEfcBalloon::GetSendData								 */
/* ���e		:���M�f�[�^���擾												 */
/* ���t		:2007/12/24														 */
/* ========================================================================= */

PBYTE CLibInfoEfcBalloon::GetSendData(DWORD dwListID)
{
	PBYTE pRet, pDataTmp, pPos;
	DWORD dwSize, dwSizeTmp;
	int i, nCount;
	PCInfoEfcBalloon pInfo;

	pRet = NULL;

	dwSize = GetSendDataSize (dwListID);
	if (dwSize == 0) {
		goto Exit;
	}
	pRet = ZeroNew (dwSize);
	pPos = pRet;

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfo = m_paInfo->GetAt (i);
		if (dwListID > 0) {
			if (pInfo->m_dwListID != dwListID) {
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
/* �֐���	:CLibInfoEfcBalloon::SetSendData								 */
/* ���e		:���M�f�[�^�����荞��											 */
/* ���t		:2007/12/24														 */
/* ========================================================================= */

PBYTE CLibInfoEfcBalloon::SetSendData(PBYTE pSrc)
{
	PBYTE pRet, pDataTmp;
	DWORD dwListID;
	PCInfoEfcBalloon pInfoEfcBalloon;

	pRet = pSrc;

	pDataTmp = pSrc;
	while (1) {
		CopyMemory (&dwListID, pDataTmp, sizeof (dwListID));
		if (dwListID == 0) {
			pDataTmp += sizeof (DWORD);
			break;
		}
		pInfoEfcBalloon = new CInfoEfcBalloon;
		pDataTmp = pInfoEfcBalloon->SetSendData (pDataTmp);
		Add (pInfoEfcBalloon);
	}
	pRet = pDataTmp;

	return pRet;
}


/* ========================================================================= */
/* �֐���	:CLibInfoEfcBalloon::GetNewID									 */
/* ���e		:�V����ID���擾													 */
/* ���t		:2007/12/24														 */
/* ========================================================================= */

DWORD CLibInfoEfcBalloon::GetNewID(void)
{
	DWORD dwRet;
	int i, nCount;
	PCInfoEfcBalloon pInfoTmp;

	dwRet = m_dwNewIDTmp + 1;
	if (dwRet == 0) {
		dwRet = 1;
	}

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->GetAt (i);
		if (pInfoTmp->m_dwEfcBalloonID == dwRet) {
			dwRet ++;
			i = -1;
			continue;
		}
	}
	m_dwNewIDTmp = dwRet;

	return dwRet;
}

/* Copyright(C)URARA-works 2007 */
