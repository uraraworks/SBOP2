/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:LibInfoMapParts.cpp										 */
/* ���e			:�}�b�v�p�[�c��񃉃C�u�����N���X �����t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/04/28													 */
/* ========================================================================= */

#include "stdafx.h"
#include "LibInfoMapParts.h"


/* ========================================================================= */
/* �֐���	:CLibInfoMapParts::CLibInfoMapParts								 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/04/28														 */
/* ========================================================================= */

CLibInfoMapParts::CLibInfoMapParts()
{
	m_dwNewIDTmp	= 0;
	m_paInfo		= NULL;
}


/* ========================================================================= */
/* �֐���	:CLibInfoMapParts::~CLibInfoMapParts							 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/04/28														 */
/* ========================================================================= */

CLibInfoMapParts::~CLibInfoMapParts()
{
	Destroy ();
}


/* ========================================================================= */
/* �֐���	:CLibInfoMapParts::Create										 */
/* ���e		:�쐬															 */
/* ���t		:2007/04/28														 */
/* ========================================================================= */

void CLibInfoMapParts::Create(void)
{
	m_paInfo = new ARRAYMAPPARTS;
}


/* ========================================================================= */
/* �֐���	:CLibInfoMapParts::Destroy										 */
/* ���e		:�j��															 */
/* ���t		:2007/04/28														 */
/* ========================================================================= */

void CLibInfoMapParts::Destroy(void)
{
	DeleteAll ();
	SAFE_DELETE (m_paInfo);
}


/* ========================================================================= */
/* �֐���	:CLibInfoMapParts::Proc											 */
/* ���e		:����															 */
/* �߂�l	:TRUE:�������� FALS:�������Ă��Ȃ�								 */
/* ���t		:2007/04/30														 */
/* ========================================================================= */

BOOL CLibInfoMapParts::Proc(void)
{
	BOOL bRet;
	int i, nCount;
	PCInfoMapParts pInfo;

	bRet = FALSE;

	nCount = GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfo = (PCInfoMapParts)GetPtr (i);
		bRet |= pInfo->TimerProc (timeGetTime ());
	}

	return bRet;
}


/* ========================================================================= */
/* �֐���	:CLibInfoMapParts::GetNew										 */
/* ���e		:�V�K�f�[�^���擾												 */
/* ���t		:2007/04/28														 */
/* ========================================================================= */

PCInfoBase CLibInfoMapParts::GetNew(void)
{
	return new CInfoMapParts;
}


/* ========================================================================= */
/* �֐���	:CLibInfoMapParts::GetCount										 */
/* ���e		:�f�[�^�����擾													 */
/* ���t		:2007/04/28														 */
/* ========================================================================= */

int CLibInfoMapParts::GetCount(void)
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
/* �֐���	:CLibInfoMapParts::Add											 */
/* ���e		:�ǉ�															 */
/* ���t		:2007/04/28														 */
/* ========================================================================= */

void CLibInfoMapParts::Add(PCInfoBase pInfo)
{
	PCInfoMapParts pMapPartsInfo;

	pMapPartsInfo = (PCInfoMapParts)pInfo;
	if (pMapPartsInfo->m_dwPartsID == 0) {
		pMapPartsInfo->m_dwPartsID = GetNewID ();
	}

	m_paInfo->Add (pMapPartsInfo);
}


/* ========================================================================= */
/* �֐���	:CLibInfoMapParts::Delete										 */
/* ���e		:�폜															 */
/* ���t		:2007/04/28														 */
/* ========================================================================= */

void CLibInfoMapParts::Delete(
	int nNo)		/* [in] �z��ԍ� */
{
	PCInfoMapParts pInfo;

	pInfo = m_paInfo->GetAt (nNo);
	SAFE_DELETE (pInfo);
	m_paInfo->RemoveAt (nNo);
}


/* ========================================================================= */
/* �֐���	:CLibInfoMapParts::Delete										 */
/* ���e		:�폜															 */
/* ���t		:2007/04/28														 */
/* ========================================================================= */

void CLibInfoMapParts::Delete(
	DWORD dwPartsID)		/* [in] �}�b�v�p�[�cID */
{
	int i, nCount, nNo;
	PCInfoMapParts pInfoTmp;

	nNo = -1;

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->GetAt (i);
		if (pInfoTmp->m_dwPartsID != dwPartsID) {
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
/* �֐���	:CLibInfoMapParts::DeleteAll									 */
/* ���e		:�S�č폜														 */
/* ���t		:2007/04/28														 */
/* ========================================================================= */

void CLibInfoMapParts::DeleteAll(void)
{
	int i, nCount;

	if (m_paInfo == NULL) {
		return;
	}

	nCount = m_paInfo->GetSize ();
	for (i = nCount - 1; i >= 0; i --) {
		Delete (i);
	}
	m_dwNewIDTmp = 0;
}


/* ========================================================================= */
/* �֐���	:CLibInfoMapParts::Merge										 */
/* ���e		:��荞��														 */
/* ���t		:2007/04/30														 */
/* ========================================================================= */

void CLibInfoMapParts::Merge(CLibInfoMapParts *pSrc)
{
	int i, nCount;
	PCInfoMapParts pInfoTmp, pInfoSrc;

	nCount = pSrc->GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfoSrc = (PCInfoMapParts)pSrc->GetPtr (i);
		pInfoTmp = (PCInfoMapParts)GetPtr (pInfoSrc->m_dwPartsID);
		if (pInfoTmp == NULL) {
			pInfoTmp = (PCInfoMapParts)GetNew ();
			pInfoTmp->Copy (pInfoSrc);
			Add (pInfoTmp);
		}
		pInfoTmp->Copy (pInfoSrc);
	}
}


/* ========================================================================= */
/* �֐���	:CLibInfoMapParts::ResetAnime									 */
/* ���e		:�A�j���[�V������Ԃ�������										 */
/* ���t		:2007/05/01														 */
/* ========================================================================= */

void CLibInfoMapParts::ResetAnime(void)
{
	int i, nCount;
	PCInfoMapParts pInfo;

	nCount = GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfo = (PCInfoMapParts)GetPtr (i);
		pInfo->m_byAnimeNo		= 0;
		pInfo->m_dwLastAnime	= timeGetTime ();
	}
}


/* ========================================================================= */
/* �֐���	:CLibInfoMapParts::GetPtr										 */
/* ���e		:�����擾														 */
/* ���t		:2007/04/28														 */
/* ========================================================================= */

PCInfoBase CLibInfoMapParts::GetPtr(int nNo)
{
	return m_paInfo->GetAt (nNo);
}


/* ========================================================================= */
/* �֐���	:CLibInfoMapParts::GetPtr										 */
/* ���e		:�����擾														 */
/* ���t		:2007/04/28														 */
/* ========================================================================= */

PCInfoBase CLibInfoMapParts::GetPtr(
	DWORD dwPartsID)		/* [in] �}�b�v�p�[�cID */
{
	int i, nCount;
	PCInfoMapParts pRet, pInfoTmp;

	pRet = NULL;
	if (dwPartsID == 0) {
		return pRet;
	}

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->GetAt (i);
		if (pInfoTmp->m_dwPartsID != dwPartsID) {
			continue;
		}
		pRet = pInfoTmp;
		break;
	}

	return pRet;
}


/* ========================================================================= */
/* �֐���	:CLibInfoMapParts::GetPtr										 */
/* ���e		:�����擾														 */
/* ���t		:2007/04/28														 */
/* ========================================================================= */

PCInfoBase CLibInfoMapParts::GetPtr(
	POINT *pPos)		/* [in] �\���ʒu */
{
	int i, nCount;
	PCInfoMapParts pRet, pInfoTmp;

	pRet = NULL;

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->GetAt (i);
		if (!((pInfoTmp->m_ptViewPos.x == pPos->x) &&
			(pInfoTmp->m_ptViewPos.y == pPos->y))) {
			continue;
		}
		pRet = pInfoTmp;
		break;
	}

	return pRet;
}


/* ========================================================================= */
/* �֐���	:CLibInfoMapParts::GetSendDataSize								 */
/* ���e		:���M�f�[�^�T�C�Y���擾											 */
/* ���t		:2007/04/29														 */
/* ========================================================================= */

DWORD CLibInfoMapParts::GetSendDataSize(void)
{
	int i, nCount;
	DWORD dwSize;
	PCInfoMapParts pInfoMapParts;

	dwSize = 0;

	nCount = GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfoMapParts = (PCInfoMapParts)GetPtr (i);
		dwSize += pInfoMapParts->GetSendDataSize ();
	}
	/* �I�[�p */
	dwSize += sizeof (DWORD);

	return dwSize;
}


/* ========================================================================= */
/* �֐���	:CLibInfoMapParts::GetSendData									 */
/* ���e		:���M�f�[�^���擾												 */
/* ���t		:2007/04/29														 */
/* ========================================================================= */

PBYTE CLibInfoMapParts::GetSendData(void)
{
	int i, nCount;
	PBYTE pData, pDataPos, pDataTmp;
	DWORD dwSize, dwSizeTmp;
	PCInfoMapParts pInfoMapParts;

	dwSize	= GetSendDataSize ();
	pData	= new BYTE[dwSize];
	ZeroMemory (pData, dwSize);

	pDataPos = pData;

	nCount = GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfoMapParts = (PCInfoMapParts)GetPtr (i);

		dwSizeTmp	= pInfoMapParts->GetSendDataSize ();
		pDataTmp	= pInfoMapParts->GetSendData ();
		CopyMemoryRenew (pDataPos, pDataTmp, dwSizeTmp, pDataPos);
		SAFE_DELETE_ARRAY (pDataTmp);
	}

	return pData;
}


/* ========================================================================= */
/* �֐���	:CLibInfoMapParts::SetSendData									 */
/* ���e		:���M�f�[�^�����荞��											 */
/* ���t		:2007/04/29														 */
/* ========================================================================= */

PBYTE CLibInfoMapParts::SetSendData(PBYTE pSrc)
{
	PBYTE pDataTmp;
	DWORD dwTmp;
	PCInfoMapParts pInfoMapParts, pInfoMapPartsTmp;

	pDataTmp = pSrc;

	while (1) {
		CopyMemory (&dwTmp, pDataTmp, sizeof (DWORD));
		if (dwTmp == 0) {
			pDataTmp += sizeof (DWORD);
			break;
		}
		pInfoMapPartsTmp = (PCInfoMapParts)GetNew ();
		pDataTmp = pInfoMapPartsTmp->SetSendData (pDataTmp);

		pInfoMapParts = (PCInfoMapParts)GetPtr (pInfoMapPartsTmp->m_dwPartsID);
		if (pInfoMapParts) {
			pInfoMapParts->Copy (pInfoMapPartsTmp);
			SAFE_DELETE (pInfoMapPartsTmp);
		} else {
			Add (pInfoMapPartsTmp);
		}
	}

	return pDataTmp;
}


/* ========================================================================= */
/* �֐���	:CLibInfoMapParts::GetNewID										 */
/* ���e		:�V����ID���擾													 */
/* ���t		:2007/04/28														 */
/* ========================================================================= */

DWORD CLibInfoMapParts::GetNewID(void)
{
	DWORD dwRet;
	int i, nCount;
	PCInfoMapParts pInfoTmp;

	dwRet = m_dwNewIDTmp + 1;
	if (dwRet == 0) {
		dwRet = 1;
	}

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->GetAt (i);
		if (pInfoTmp->m_dwPartsID == dwRet) {
			dwRet ++;
			i = -1;
			continue;
		}
	}
	m_dwNewIDTmp = dwRet;

	return dwRet;
}

/* Copyright(C)URARA-works 2007 */
