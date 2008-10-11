/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:LibInfoEffect.cpp											 */
/* ���e			:�G�t�F�N�g��񃉃C�u�����N���X �����t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/07/26													 */
/* ========================================================================= */

#include "stdafx.h"
#include "LibInfoEffect.h"


/* ========================================================================= */
/* �֐���	:CLibInfoEffect::CLibInfoEffect									 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/07/26														 */
/* ========================================================================= */

CLibInfoEffect::CLibInfoEffect()
{
	m_paInfo = NULL;
}


/* ========================================================================= */
/* �֐���	:CLibInfoEffect::~CLibInfoEffect								 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/07/26														 */
/* ========================================================================= */

CLibInfoEffect::~CLibInfoEffect()
{
	Destroy ();
}


/* ========================================================================= */
/* �֐���	:CLibInfoEffect::Create											 */
/* ���e		:�쐬															 */
/* ���t		:2007/07/26														 */
/* ========================================================================= */

void CLibInfoEffect::Create(void)
{
	m_paInfo = new ARRAYEFFECT;
}


/* ========================================================================= */
/* �֐���	:CLibInfoEffect::Destroy										 */
/* ���e		:�j��															 */
/* ���t		:2007/07/26														 */
/* ========================================================================= */

void CLibInfoEffect::Destroy(void)
{
	DeleteAll ();
	SAFE_DELETE (m_paInfo);
}


/* ========================================================================= */
/* �֐���	:CLibInfoEffect::Proc											 */
/* ���e		:����															 */
/* �߂�l	:TRUE:�������� FALS:�������Ă��Ȃ�								 */
/* ���t		:2007/04/30														 */
/* ========================================================================= */

BOOL CLibInfoEffect::Proc(void)
{
	BOOL bRet;
	int i, nCount;
	PCInfoEffect pInfo;

	bRet = FALSE;

	nCount = GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfo = (PCInfoEffect)GetPtr (i);
		bRet |= pInfo->TimerProc (timeGetTime ());
	}

	return bRet;
}


/* ========================================================================= */
/* �֐���	:CLibInfoEffect::GetNew											 */
/* ���e		:�V�K�f�[�^���擾												 */
/* ���t		:2007/07/26														 */
/* ========================================================================= */

PCInfoBase CLibInfoEffect::GetNew(void)
{
	return new CInfoEffect;
}


/* ========================================================================= */
/* �֐���	:CLibInfoEffect::GetCount										 */
/* ���e		:�f�[�^�����擾													 */
/* ���t		:2007/07/26														 */
/* ========================================================================= */

int CLibInfoEffect::GetCount(void)
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
/* �֐���	:CLibInfoEffect::Add											 */
/* ���e		:�ǉ�															 */
/* ���t		:2007/07/26														 */
/* ========================================================================= */

void CLibInfoEffect::Add(PCInfoBase pInfo)
{
	PCInfoEffect pEffectInfo;

	pEffectInfo = (PCInfoEffect)pInfo;
	if (pEffectInfo->m_dwEffectID == 0) {
		pEffectInfo->m_dwEffectID = GetNewID ();
	}

	m_paInfo->Add (pEffectInfo);
}


/* ========================================================================= */
/* �֐���	:CLibInfoEffect::Delete											 */
/* ���e		:�폜															 */
/* ���t		:2007/07/26														 */
/* ========================================================================= */

void CLibInfoEffect::Delete(
	int nNo)		/* [in] �z��ԍ� */
{
	PCInfoEffect pInfo;

	pInfo = m_paInfo->GetAt (nNo);
	SAFE_DELETE (pInfo);
	m_paInfo->RemoveAt (nNo);
}


/* ========================================================================= */
/* �֐���	:CLibInfoEffect::Delete											 */
/* ���e		:�폜															 */
/* ���t		:2007/07/26														 */
/* ========================================================================= */

void CLibInfoEffect::Delete(
	DWORD dwEffectID)		/* [in] �G�t�F�N�gID */
{
	int i, nCount, nNo;
	PCInfoEffect pInfoTmp;

	nNo = -1;

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->GetAt (i);
		if (pInfoTmp->m_dwEffectID != dwEffectID) {
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
/* �֐���	:CLibInfoEffect::DeleteAll										 */
/* ���e		:�S�č폜														 */
/* ���t		:2007/07/26														 */
/* ========================================================================= */

void CLibInfoEffect::DeleteAll(void)
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
/* �֐���	:CLibInfoEffect::GetName										 */
/* ���e		:�G�t�F�N�g�����擾												 */
/* ���t		:2008/07/06														 */
/* ========================================================================= */

void CLibInfoEffect::GetName(
	DWORD dwEffectID,		/* [in]  �G�t�F�N�gID */
	CmyString &strDst)		/* [out] �G�t�F�N�g�� */
{
	int i, nCount;
	PCInfoEffect pInfo;

	strDst.Empty ();

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfo = m_paInfo->GetAt (i);
		if (pInfo->m_dwEffectID != dwEffectID) {
			continue;
		}
		strDst = pInfo->m_strName;
		break;
	}
}


/* ========================================================================= */
/* �֐���	:CLibInfoEffect::GetPtr											 */
/* ���e		:�����擾														 */
/* ���t		:2007/07/26														 */
/* ========================================================================= */

PCInfoBase CLibInfoEffect::GetPtr(int nNo)
{
	PCInfoBase pRet;

	pRet = NULL;
	if (nNo >= m_paInfo->GetSize ()) {
		goto Exit;
	}

	pRet = m_paInfo->GetAt (nNo);
Exit:
	return pRet;
}


/* ========================================================================= */
/* �֐���	:CLibInfoEffect::GetPtr											 */
/* ���e		:�����擾														 */
/* ���t		:2007/07/26														 */
/* ========================================================================= */

PCInfoBase CLibInfoEffect::GetPtr(
	DWORD dwEffectID)		/* [in] �G�t�F�N�gID */
{
	int i, nCount;
	PCInfoEffect pRet, pInfoTmp;

	pRet = NULL;

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->GetAt (i);
		if (pInfoTmp->m_dwEffectID != dwEffectID) {
			continue;
		}
		pRet = pInfoTmp;
		break;
	}

	return pRet;
}


/* ========================================================================= */
/* �֐���	:CLibInfoEffect::GetSendDataSize								 */
/* ���e		:���M�f�[�^�T�C�Y���擾											 */
/* ���t		:2007/07/26														 */
/* ========================================================================= */

DWORD CLibInfoEffect::GetSendDataSize(void)
{
	int i, nCount;
	DWORD dwSize;
	PCInfoEffect pInfoEffect;

	dwSize = 0;

	nCount = GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfoEffect = (PCInfoEffect)GetPtr (i);
		dwSize += pInfoEffect->GetSendDataSize ();
	}
	/* �I�[�p */
	dwSize += sizeof (DWORD);

	return dwSize;
}


/* ========================================================================= */
/* �֐���	:CLibInfoEffect::GetSendData									 */
/* ���e		:���M�f�[�^���擾												 */
/* ���t		:2007/07/26														 */
/* ========================================================================= */

PBYTE CLibInfoEffect::GetSendData(void)
{
	int i, nCount;
	PBYTE pData, pDataPos, pDataTmp;
	DWORD dwSize, dwSizeTmp;
	PCInfoEffect pInfoEffect;

	dwSize	= GetSendDataSize ();
	pData	= new BYTE[dwSize];
	ZeroMemory (pData, dwSize);

	pDataPos = pData;

	nCount = GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfoEffect = (PCInfoEffect)GetPtr (i);

		dwSizeTmp	= pInfoEffect->GetSendDataSize ();
		pDataTmp	= pInfoEffect->GetSendData ();
		CopyMemoryRenew (pDataPos, pDataTmp, dwSizeTmp, pDataPos);
		SAFE_DELETE_ARRAY (pDataTmp);
	}

	return pData;
}


/* ========================================================================= */
/* �֐���	:CLibInfoEffect::SetSendData									 */
/* ���e		:���M�f�[�^�����荞��											 */
/* ���t		:2007/07/26														 */
/* ========================================================================= */

PBYTE CLibInfoEffect::SetSendData(PBYTE pSrc)
{
	PBYTE pDataTmp;
	DWORD dwTmp;
	PCInfoEffect pInfoEffect, pInfoEffectTmp;

	pDataTmp = pSrc;

	while (1) {
		CopyMemory (&dwTmp, pDataTmp, sizeof (DWORD));
		if (dwTmp == 0) {
			pDataTmp += sizeof (DWORD);
			break;
		}
		pInfoEffectTmp = (PCInfoEffect)GetNew ();
		pDataTmp = pInfoEffectTmp->SetSendData (pDataTmp);

		pInfoEffect = (PCInfoEffect)GetPtr (pInfoEffectTmp->m_dwEffectID);
		if (pInfoEffect) {
			pInfoEffect->Copy (pInfoEffectTmp);
			SAFE_DELETE (pInfoEffectTmp);
		} else {
			Add (pInfoEffectTmp);
		}
	}

	return pDataTmp;
}


/* ========================================================================= */
/* �֐���	:CLibInfoEffect::GetNewID										 */
/* ���e		:�V����ID���擾													 */
/* ���t		:2007/07/26														 */
/* ========================================================================= */

DWORD CLibInfoEffect::GetNewID(void)
{
	DWORD dwRet;
	int i, nCount;
	PCInfoEffect pInfoTmp;

	dwRet = 1;

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->GetAt (i);
		if (pInfoTmp->m_dwEffectID == dwRet) {
			dwRet ++;
			i = -1;
			continue;
		}
	}

	return dwRet;
}

/* Copyright(C)URARA-works 2007 */
