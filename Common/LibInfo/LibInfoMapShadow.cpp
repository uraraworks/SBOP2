/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:LibInfoMapShadow.cpp										 */
/* ���e			:�}�b�v�e��񃉃C�u�����N���X �����t�@�C��					 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/06/05													 */
/* ========================================================================= */

#include "stdafx.h"
#include "LibInfoMapShadow.h"


/* ========================================================================= */
/* �֐���	:CLibInfoMapShadow::CLibInfoMapShadow							 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/06/05														 */
/* ========================================================================= */

CLibInfoMapShadow::CLibInfoMapShadow()
{
	m_paInfo = NULL;
}


/* ========================================================================= */
/* �֐���	:CLibInfoMapShadow::~CLibInfoMapShadow							 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/06/05														 */
/* ========================================================================= */

CLibInfoMapShadow::~CLibInfoMapShadow()
{
	Destroy ();
}


/* ========================================================================= */
/* �֐���	:CLibInfoMapShadow::Create										 */
/* ���e		:�쐬															 */
/* ���t		:2007/06/05														 */
/* ========================================================================= */

void CLibInfoMapShadow::Create(void)
{
	m_paInfo = new ARRAYMAPSHADOW;
}


/* ========================================================================= */
/* �֐���	:CLibInfoMapShadow::Destroy										 */
/* ���e		:�j��															 */
/* ���t		:2007/06/05														 */
/* ========================================================================= */

void CLibInfoMapShadow::Destroy(void)
{
	DeleteAll ();
	SAFE_DELETE (m_paInfo);
}


/* ========================================================================= */
/* �֐���	:CLibInfoMapShadow::Proc										 */
/* ���e		:����															 */
/* �߂�l	:TRUE:�������� FALS:�������Ă��Ȃ�								 */
/* ���t		:2007/04/30														 */
/* ========================================================================= */

BOOL CLibInfoMapShadow::Proc(void)
{
	BOOL bRet;
	int i, nCount;
	PCInfoMapShadow pInfo;

	bRet = FALSE;

	nCount = GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfo = (PCInfoMapShadow)GetPtr (i);
		bRet |= pInfo->TimerProc (timeGetTime ());
	}

	return bRet;
}


/* ========================================================================= */
/* �֐���	:CLibInfoMapShadow::GetNew										 */
/* ���e		:�V�K�f�[�^���擾												 */
/* ���t		:2007/06/05														 */
/* ========================================================================= */

PCInfoBase CLibInfoMapShadow::GetNew(void)
{
	return new CInfoMapShadow;
}


/* ========================================================================= */
/* �֐���	:CLibInfoMapShadow::GetCount									 */
/* ���e		:�f�[�^�����擾													 */
/* ���t		:2007/06/05														 */
/* ========================================================================= */

int CLibInfoMapShadow::GetCount(void)
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
/* �֐���	:CLibInfoMapShadow::Add											 */
/* ���e		:�ǉ�															 */
/* ���t		:2007/06/05														 */
/* ========================================================================= */

void CLibInfoMapShadow::Add(PCInfoBase pInfo)
{
	PCInfoMapShadow pMapShadowInfo;

	pMapShadowInfo = (PCInfoMapShadow)pInfo;
	if (pMapShadowInfo->m_dwShadowID == 0) {
		pMapShadowInfo->m_dwShadowID = GetNewID ();
	}

	m_paInfo->Add (pMapShadowInfo);
}


/* ========================================================================= */
/* �֐���	:CLibInfoMapShadow::Delete										 */
/* ���e		:�폜															 */
/* ���t		:2007/06/05														 */
/* ========================================================================= */

void CLibInfoMapShadow::Delete(
	int nNo)		/* [in] �z��ԍ� */
{
	PCInfoMapShadow pInfo;

	pInfo = m_paInfo->GetAt (nNo);
	SAFE_DELETE (pInfo);
	m_paInfo->RemoveAt (nNo);
}


/* ========================================================================= */
/* �֐���	:CLibInfoMapShadow::Delete										 */
/* ���e		:�폜															 */
/* ���t		:2007/06/05														 */
/* ========================================================================= */

void CLibInfoMapShadow::Delete(
	DWORD dwShadowID)		/* [in] �}�b�v�eID */
{
	int i, nCount, nNo;
	PCInfoMapShadow pInfoTmp;

	nNo = -1;

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->GetAt (i);
		if (pInfoTmp->m_dwShadowID != dwShadowID) {
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
/* �֐���	:CLibInfoMapShadow::DeleteAll									 */
/* ���e		:�S�č폜														 */
/* ���t		:2007/06/05														 */
/* ========================================================================= */

void CLibInfoMapShadow::DeleteAll(void)
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
/* �֐���	:CLibInfoMapShadow::Merge										 */
/* ���e		:��荞��														 */
/* ���t		:2007/04/30														 */
/* ========================================================================= */

void CLibInfoMapShadow::Merge(CLibInfoMapShadow *pSrc)
{
	int i, nCount;
	PCInfoMapShadow pInfoTmp, pInfoSrc;

	nCount = pSrc->GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfoSrc = (PCInfoMapShadow)pSrc->GetPtr (i);
		pInfoTmp = (PCInfoMapShadow)GetPtr (pInfoSrc->m_dwShadowID);
		if (pInfoTmp == NULL) {
			pInfoTmp = (PCInfoMapShadow)GetNew ();
			Add (pInfoTmp);
		}
		pInfoTmp->Copy (pInfoSrc);
	}
}


/* ========================================================================= */
/* �֐���	:CLibInfoMapShadow::ResetAnime									 */
/* ���e		:�A�j���[�V������Ԃ�������										 */
/* ���t		:2007/05/01														 */
/* ========================================================================= */

void CLibInfoMapShadow::ResetAnime(void)
{
	int i, nCount;
	PCInfoMapShadow pInfo;

	nCount = GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfo = (PCInfoMapShadow)GetPtr (i);
		pInfo->m_byAnimeNo		= 0;
		pInfo->m_dwLastAnime	= timeGetTime ();
	}
}


/* ========================================================================= */
/* �֐���	:CLibInfoMapShadow::GetPtr										 */
/* ���e		:�����擾														 */
/* ���t		:2007/06/05														 */
/* ========================================================================= */

PCInfoBase CLibInfoMapShadow::GetPtr(int nNo)
{
	return m_paInfo->GetAt (nNo);
}


/* ========================================================================= */
/* �֐���	:CLibInfoMapShadow::GetPtr										 */
/* ���e		:�����擾														 */
/* ���t		:2007/06/05														 */
/* ========================================================================= */

PCInfoBase CLibInfoMapShadow::GetPtr(
	DWORD dwShadowID)		/* [in] �}�b�v�eID */
{
	int i, nCount;
	PCInfoMapShadow pRet, pInfoTmp;

	pRet = NULL;

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->GetAt (i);
		if (pInfoTmp->m_dwShadowID != dwShadowID) {
			continue;
		}
		pRet = pInfoTmp;
		break;
	}

	return pRet;
}


/* ========================================================================= */
/* �֐���	:CLibInfoMapShadow::GetPtr										 */
/* ���e		:�����擾														 */
/* ���t		:2007/06/05														 */
/* ========================================================================= */

PCInfoBase CLibInfoMapShadow::GetPtr(
	POINT *pPos)		/* [in] �\���ʒu */
{
	int i, nCount;
	PCInfoMapShadow pRet, pInfoTmp;

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
/* �֐���	:CLibInfoMapShadow::GetSendDataSize								 */
/* ���e		:���M�f�[�^�T�C�Y���擾											 */
/* ���t		:2007/04/29														 */
/* ========================================================================= */

DWORD CLibInfoMapShadow::GetSendDataSize(void)
{
	int i, nCount;
	DWORD dwSize;
	PCInfoMapShadow pInfoMapShadow;

	dwSize = 0;

	nCount = GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfoMapShadow = (PCInfoMapShadow)GetPtr (i);
		dwSize += pInfoMapShadow->GetSendDataSize ();
	}
	/* �I�[�p */
	dwSize += sizeof (DWORD);

	return dwSize;
}


/* ========================================================================= */
/* �֐���	:CLibInfoMapShadow::GetSendData									 */
/* ���e		:���M�f�[�^���擾												 */
/* ���t		:2007/04/29														 */
/* ========================================================================= */

PBYTE CLibInfoMapShadow::GetSendData(void)
{
	int i, nCount;
	PBYTE pData, pDataPos, pDataTmp;
	DWORD dwSize, dwSizeTmp;
	PCInfoMapShadow pInfoMapShadow;

	dwSize	= GetSendDataSize ();
	pData	= new BYTE[dwSize];
	ZeroMemory (pData, dwSize);

	pDataPos = pData;

	nCount = GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfoMapShadow = (PCInfoMapShadow)GetPtr (i);

		dwSizeTmp	= pInfoMapShadow->GetSendDataSize ();
		pDataTmp	= pInfoMapShadow->GetSendData ();
		CopyMemoryRenew (pDataPos, pDataTmp, dwSizeTmp, pDataPos);
		SAFE_DELETE_ARRAY (pDataTmp);
	}

	return pData;
}


/* ========================================================================= */
/* �֐���	:CLibInfoMapShadow::SetSendData									 */
/* ���e		:���M�f�[�^�����荞��											 */
/* ���t		:2007/04/29														 */
/* ========================================================================= */

PBYTE CLibInfoMapShadow::SetSendData(PBYTE pSrc)
{
	PBYTE pDataTmp;
	DWORD dwTmp;
	PCInfoMapShadow pInfoMapShadow, pInfoMapShadowTmp;

	pDataTmp = pSrc;

	while (1) {
		CopyMemory (&dwTmp, pDataTmp, sizeof (DWORD));
		if (dwTmp == 0) {
			pDataTmp += sizeof (DWORD);
			break;
		}
		pInfoMapShadowTmp = (PCInfoMapShadow)GetNew ();
		pDataTmp = pInfoMapShadowTmp->SetSendData (pDataTmp);

		pInfoMapShadow = (PCInfoMapShadow)GetPtr (pInfoMapShadowTmp->m_dwShadowID);
		if (pInfoMapShadow) {
			pInfoMapShadow->Copy (pInfoMapShadowTmp);
			SAFE_DELETE (pInfoMapShadowTmp);
		} else {
			Add (pInfoMapShadowTmp);
		}
	}

	return pDataTmp;
}


/* ========================================================================= */
/* �֐���	:CLibInfoMapShadow::GetNewID									 */
/* ���e		:�V����ID���擾													 */
/* ���t		:2007/06/05														 */
/* ========================================================================= */

DWORD CLibInfoMapShadow::GetNewID(void)
{
	DWORD dwRet;
	int i, nCount;
	PCInfoMapShadow pInfoTmp;

	dwRet = 1;

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->GetAt (i);
		if (pInfoTmp->m_dwShadowID == dwRet) {
			dwRet ++;
			i = -1;
			continue;
		}
	}

	return dwRet;
}

/* Copyright(C)URARA-works 2007 */
