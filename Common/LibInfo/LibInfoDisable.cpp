/* Copyright(C)URARA-works 2009 */
/* ========================================================================= */
/* �t�@�C����	:LibInfoDisable.cpp											 */
/* ���e			:���ۏ�񃉃C�u�������N���X �����t�@�C��					 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2009/04/08													 */
/* ========================================================================= */

#include "stdafx.h"
#include "LibInfoDisable.h"


/* ========================================================================= */
/* �֐���	:CLibInfoDisable::CLibInfoDisable								 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2009/04/08														 */
/* ========================================================================= */

CLibInfoDisable::CLibInfoDisable()
{
	m_dwNewIDTmp		= 0;
	m_dwLastTimeProc	= 0;
	m_paInfo			= NULL;
	m_paInfoIPADdress	= NULL;
}


/* ========================================================================= */
/* �֐���	:CLibInfoDisable::~CLibInfoDisable								 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2009/04/08														 */
/* ========================================================================= */

CLibInfoDisable::~CLibInfoDisable()
{
	Destroy ();
}


/* ========================================================================= */
/* �֐���	:CLibInfoDisable::Create										 */
/* ���e		:�쐬															 */
/* ���t		:2009/04/08														 */
/* ========================================================================= */

void CLibInfoDisable::Create(void)
{
	m_paInfo			= new ARRAYDISABLEINFO;
	m_paInfoIPADdress	= new ARRAYDISABLEIPADDRESS;
}


/* ========================================================================= */
/* �֐���	:CLibInfoDisable::Destroy										 */
/* ���e		:�j��															 */
/* ���t		:2009/04/08														 */
/* ========================================================================= */

void CLibInfoDisable::Destroy(void)
{
	DeleteAll ();
	DeleteAllIP ();
	SAFE_DELETE (m_paInfo);
	SAFE_DELETE (m_paInfoIPADdress);
}


/* ========================================================================= */
/* �֐���	:CLibInfoDisable::Proc											 */
/* ���e		:����															 */
/* �߂�l	:TRUE:�������� FALS:�������Ă��Ȃ�								 */
/* ���t		:2009/04/11														 */
/* ========================================================================= */

BOOL CLibInfoDisable::Proc(void)
{
	int i, nCount;
	BOOL bRet;
	DWORD dwTime;
	PSTDISABLEIPADDRESS pInfo;

	bRet = FALSE;

	if (m_paInfoIPADdress == NULL) {
		goto Exit;
	}
	nCount = m_paInfoIPADdress->GetSize ();
	if (nCount <= 0) {
		goto Exit;
	}

	dwTime = timeGetTime ();
	if (dwTime - m_dwLastTimeProc < 1000 * 10) {
		/* 10�b�ȓ��Ȃ珈�����Ȃ� */
		goto Exit;
	}
	m_dwLastTimeProc = dwTime;

	for (i = nCount - 1; i >= 0; i --) {
		pInfo = m_paInfoIPADdress->GetAt (i);
		/* 1���Ԉȏ�o�߂��Ă���H */
		if (dwTime - pInfo->dwLastTime >= 1000 * 60 * 60) {
			/* ���ۉ��� */
			DeleteIP (i);
		}
	}

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CLibInfoDisable::GetNew										 */
/* ���e		:�V�K�f�[�^���擾												 */
/* ���t		:2009/04/08														 */
/* ========================================================================= */

PCInfoBase CLibInfoDisable::GetNew(void)
{
	PCInfoDisable pInfoDisable;

	pInfoDisable = new CInfoDisable;

	return pInfoDisable;
}


/* ========================================================================= */
/* �֐���	:CLibInfoDisable::GetCount										 */
/* ���e		:�f�[�^�����擾													 */
/* ���t		:2009/04/08														 */
/* ========================================================================= */

int CLibInfoDisable::GetCount(void)
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
/* �֐���	:CLibInfoDisable::Add											 */
/* ���e		:�ǉ�															 */
/* ���t		:2009/04/08														 */
/* ========================================================================= */

void CLibInfoDisable::Add(PCInfoBase pInfo)
{
	PCInfoDisable pDisableInfo;

	pDisableInfo = (PCInfoDisable)pInfo;
	if (pDisableInfo->m_dwDisableID == 0) {
		pDisableInfo->m_dwDisableID = GetNewID ();
	}

	m_paInfo->Add (pDisableInfo);
}


/* ========================================================================= */
/* �֐���	:CLibInfoDisable::Delete										 */
/* ���e		:�폜															 */
/* ���t		:2009/04/08														 */
/* ========================================================================= */

void CLibInfoDisable::Delete(
	int nNo)		/* [in] �z��ԍ� */
{
	PCInfoDisable pInfo;

	pInfo = m_paInfo->GetAt (nNo);
	SAFE_DELETE (pInfo);
	m_paInfo->RemoveAt (nNo);
}


/* ========================================================================= */
/* �֐���	:CLibInfoDisable::Delete										 */
/* ���e		:�폜															 */
/* ���t		:2009/04/08														 */
/* ========================================================================= */

void CLibInfoDisable::Delete(
	DWORD dwDisableID)		/* [in] ���ۏ��ID */
{
	int i, nCount, nNo;
	PCInfoDisable pInfoTmp;

	Enter ();

	nNo = -1;

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->GetAt (i);
		if (pInfoTmp->m_dwDisableID != dwDisableID) {
			continue;
		}
		nNo = i;
		break;
	}

	if (nNo >= 0) {
		Delete (nNo);
	}

	Leave ();
}


/* ========================================================================= */
/* �֐���	:CLibInfoDisable::DeleteAll										 */
/* ���e		:�S�č폜														 */
/* ���t		:2009/04/08														 */
/* ========================================================================= */

void CLibInfoDisable::DeleteAll(void)
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
/* �֐���	:CLibInfoDisable::CopyAll										 */
/* ���e		:�S�ăR�s�[														 */
/* ���t		:2009/04/11														 */
/* ========================================================================= */

void CLibInfoDisable::CopyAll(CLibInfoDisable *pSrc)
{
	int i, nCount;
	PCInfoDisable pInfoTmp, pInfoNew;

	DeleteAll ();

	nCount = pSrc->GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = (PCInfoDisable)pSrc->GetPtr (i);
		pInfoNew = (PCInfoDisable)GetNew ();
		pInfoNew->Copy (pInfoTmp);
		Add (pInfoNew);
	}

}


/* ========================================================================= */
/* �֐���	:CLibInfoDisable::IsDisable										 */
/* ���e		:���ۏ�񂪓o�^����Ă��邩����									 */
/* ���t		:2009/04/11														 */
/* ========================================================================= */

BOOL CLibInfoDisable::IsDisable(LPCSTR pszMacAddress)
{
	BOOL bRet;
	PCInfoBase pInfo;

	bRet = FALSE;

	pInfo = GetPtr (pszMacAddress);
	if (pInfo) {
		bRet = TRUE;
	}

	return bRet;
}


/* ========================================================================= */
/* �֐���	:CLibInfoDisable::Merge											 */
/* ���e		:��荞��														 */
/* ���t		:2009/04/11														 */
/* ========================================================================= */

void CLibInfoDisable::Merge(CLibInfoDisable *pSrc)
{
	int i, nCount;
	PCInfoDisable pInfoTmp, pInfoSrc;

	nCount = pSrc->GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfoSrc = (PCInfoDisable)pSrc->GetPtr (i);
		pInfoTmp = (PCInfoDisable)GetPtr (pInfoSrc->m_dwDisableID);
		if (pInfoTmp == NULL) {
			pInfoTmp = (PCInfoDisable)GetNew ();
			pInfoTmp->Copy (pInfoSrc);
			Add (pInfoTmp);
		}
		pInfoTmp->Copy (pInfoSrc);
	}
}


/* ========================================================================= */
/* �֐���	:CLibInfoDisable::GetPtr										 */
/* ���e		:���ۏ����擾													 */
/* ���t		:2009/04/08														 */
/* ========================================================================= */

PCInfoBase CLibInfoDisable::GetPtr(int nNo)
{
	return m_paInfo->GetAt (nNo);
}


/* ========================================================================= */
/* �֐���	:CLibInfoDisable::GetPtr										 */
/* ���e		:���ۏ����擾													 */
/* ���t		:2009/04/08														 */
/* ========================================================================= */

PCInfoBase CLibInfoDisable::GetPtr(
	DWORD dwDisableID)		/* [in] ���ۏ��ID */
{
	int i, nCount;
	PCInfoDisable pRet, pInfoTmp;

	pRet = NULL;

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->GetAt (i);
		if (pInfoTmp->m_dwDisableID != dwDisableID) {
			continue;
		}
		pRet = pInfoTmp;
		break;
	}

	return pRet;
}


/* ========================================================================= */
/* �֐���	:CLibInfoDisable::GetPtr										 */
/* ���e		:���ۏ����擾													 */
/* ���t		:2009/04/11														 */
/* ========================================================================= */

PCInfoBase CLibInfoDisable::GetPtr(
	LPCSTR pszMacAddress)		/* [in] MAC�A�h���X */
{
	int i, nCount;
	PCInfoDisable pRet, pInfoTmp;

	pRet = NULL;

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->GetAt (i);
		if (pInfoTmp->m_strMacAddress != pszMacAddress) {
			continue;
		}
		pRet = pInfoTmp;
		break;
	}

	return pRet;
}


/* ========================================================================= */
/* �֐���	:CLibInfoDisable::GetSendDataSize								 */
/* ���e		:���M�f�[�^�T�C�Y���擾											 */
/* ���t		:2009/04/08														 */
/* ========================================================================= */

DWORD CLibInfoDisable::GetSendDataSize(void)
{
	int i, nCount;
	DWORD dwRet, dwSize;
	PCInfoDisable pDisable;

	dwRet = dwSize = 0;

	/* �f�[�^�����̃T�C�Y */
	dwSize += sizeof (DWORD);

	nCount = GetCount ();
	for (i = 0; i < nCount; i ++) {
		pDisable = (PCInfoDisable)GetPtr (i);

		dwSize += pDisable->GetSendDataSize ();
	}

	dwRet = dwSize;
	return dwRet;
}


/* ========================================================================= */
/* �֐���	:CLibInfoDisable::GetSendData									 */
/* ���e		:���M�f�[�^���擾												 */
/* ���t		:2009/04/08														 */
/* ========================================================================= */

PBYTE CLibInfoDisable::GetSendData(void)
{
	int i, nCount;
	PBYTE pRet, pData, pDataTmp;
	DWORD dwCount, dwSize, dwSizeTmp, dwOffset;
	PCInfoDisable pDisable;

	pRet = NULL;

	dwSize		= GetSendDataSize ();
	pData		= ZeroNew (dwSize);
	dwOffset	= 0;

	/* �f�[�^������������ */
	dwCount = (DWORD)GetCount ();
	CopyMemory (pData, &dwCount, sizeof (dwCount));
	dwOffset += sizeof (dwCount);

	/* ���ۏ����������� */
	nCount = GetCount ();
	for (i = 0; i < nCount; i ++) {
		pDisable = (PCInfoDisable)GetPtr (i);

		dwSizeTmp	= pDisable->GetSendDataSize ();
		pDataTmp	= pDisable->GetSendData ();
		CopyMemory (&pData[dwOffset], pDataTmp, dwSizeTmp);
		dwOffset += dwSizeTmp;

		SAFE_DELETE_ARRAY (pDataTmp);
	}

	pRet = pData;
	return pRet;
}


/* ========================================================================= */
/* �֐���	:CLibInfoDisable::SetSendData									 */
/* ���e		:���M�f�[�^�����荞��											 */
/* ���t		:2009/04/08														 */
/* �߂�l	:����������̃A�h���X											 */
/* ========================================================================= */

PBYTE CLibInfoDisable::SetSendData(PBYTE pSrc)
{
	int i, nCount;
	DWORD dwCount;
	PBYTE pRet, pDataTmp;
	PCInfoDisable pDisable;

	pRet		= pSrc;
	pDataTmp	= pSrc;

	DeleteAll ();

	/* �f�[�^����ǂݍ��� */
	CopyMemory (&dwCount, pDataTmp, sizeof (dwCount));
	nCount		= (int)dwCount;
	pDataTmp	+= sizeof (dwCount);

	for (i = 0; i < nCount; i ++) {
		pDisable = (PCInfoDisable)GetNew ();

		pDataTmp = pDisable->SetSendData (pDataTmp);
		Add (pDisable);
	}

	pRet = pDataTmp;
	return pRet;
}


/* ========================================================================= */
/* �֐���	:CLibInfoDisable::AddIP											 */
/* ���e		:IP�A�h���X�֘A(�ǉ�)											 */
/* ���t		:2009/04/11														 */
/* ========================================================================= */

void CLibInfoDisable::AddIP(ULONG ulIPAddress)
{
	BOOL bResult;
	PSTDISABLEIPADDRESS pInfo;

	bResult = IsDisableIP (ulIPAddress);
	if (bResult) {
		/* �o�^�ς� */
		return;
	}

	pInfo = new STDISABLEIPADDRESS;
	pInfo->ulIPAddress	= ulIPAddress;
	pInfo->dwLastTime	= timeGetTime ();
	m_paInfoIPADdress->Add (pInfo);
}


/* ========================================================================= */
/* �֐���	:CLibInfoDisable::DeleteIP										 */
/* ���e		:IP�A�h���X�֘A(�폜)											 */
/* ���t		:2009/04/11														 */
/* ========================================================================= */

void CLibInfoDisable::DeleteIP(int nNo)
{
	PSTDISABLEIPADDRESS pInfo;

	pInfo = m_paInfoIPADdress->GetAt (nNo);
	SAFE_DELETE (pInfo);
	m_paInfoIPADdress->RemoveAt (nNo);
}


/* ========================================================================= */
/* �֐���	:CLibInfoDisable::DeleteAllIP									 */
/* ���e		:IP�A�h���X�֘A(�S�č폜)										 */
/* ���t		:2009/04/11														 */
/* ========================================================================= */

void CLibInfoDisable::DeleteAllIP(void)
{
	int i, nCount;

	if (m_paInfoIPADdress == NULL) {
		return;
	}
	nCount = m_paInfoIPADdress->GetSize ();
	for (i = nCount - 1; i >= 0; nCount --) {
		DeleteIP (i);
	}
}


/* ========================================================================= */
/* �֐���	:CLibInfoDisable::IsDisableIP									 */
/* ���e		:IP�A�h���X�֘A(�֎~����Ă���IP�A�h���X������)					 */
/* ���t		:2009/04/11														 */
/* ========================================================================= */

BOOL CLibInfoDisable::IsDisableIP(ULONG ulIPAddress)
{
	BOOL bRet;
	int i, nCount;
	PSTDISABLEIPADDRESS pInfo;

	bRet = FALSE;

	nCount = m_paInfoIPADdress->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfo = m_paInfoIPADdress->GetAt (i);
		if (pInfo->ulIPAddress != ulIPAddress) {
			continue;
		}
		bRet = TRUE;
		break;
	}

	return bRet;
}


/* ========================================================================= */
/* �֐���	:CLibInfoDisable::GetNewID										 */
/* ���e		:�V�������ۏ��ID���擾											 */
/* ���t		:2009/04/08														 */
/* ========================================================================= */

DWORD CLibInfoDisable::GetNewID(void)
{
	DWORD dwRet;
	int i, nCount;
	PCInfoDisable pInfoTmp;

	dwRet = m_dwNewIDTmp + 1;
	if (dwRet == 0) {
		dwRet = 1;
	}

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->GetAt (i);
		if (pInfoTmp->m_dwDisableID == dwRet) {
			dwRet ++;
			i = -1;
			continue;
		}
	}
	m_dwNewIDTmp = dwRet;

	return dwRet;
}

/* Copyright(C)URARA-works 2009 */
