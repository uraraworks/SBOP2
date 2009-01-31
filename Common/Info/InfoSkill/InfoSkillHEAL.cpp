/* Copyright(C)URARA-works 2009 */
/* ========================================================================= */
/* �t�@�C����	:InfoSkillHEAL.cpp											 */
/* ���e			:�X�L�����(��)�N���X �����t�@�C��						 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2009/01/22													 */
/* ========================================================================= */

#include "stdafx.h"
#include "InfoSkillHEAL.h"

/* ========================================================================= */
/* �萔��`																	 */
/* ========================================================================= */

/* �w�b�_��� */
static LPCSTR s_aszName[] = {
	"m_dwArea",			/* �͈� */
	"m_dwHealType",		/* �񕜎�� */
	"m_dwHitEffectID",	/* �q�b�g���̕\���G�t�F�N�g */
	"m_dwValue1",		/* ����1 */
	"m_dwValue2",		/* ����2 */
	"m_dwDistance",		/* �˒����� */
	NULL
};


/* ========================================================================= */
/* �֐���	:CInfoSkillHEAL::CInfoSkillHEAL									 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2009/01/22														 */
/* ========================================================================= */

CInfoSkillHEAL::CInfoSkillHEAL()
{
	m_nClassType		= INFOSKILLTYPE_HEAL;
	m_dwArea			= 0;
	m_dwHealType		= 0;
	m_dwHitEffectID		= 0;
	m_dwValue1			= 0;
	m_dwValue2			= 0;
	m_dwDistance		= 0;

	for (m_nElementCountDerivation = 0; s_aszName[m_nElementCountDerivation] != NULL; m_nElementCountDerivation ++) {}
}


/* ========================================================================= */
/* �֐���	:CInfoSkillHEAL::~CInfoSkillHEAL								 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2009/01/22														 */
/* ========================================================================= */

CInfoSkillHEAL::~CInfoSkillHEAL()
{
}


/* ========================================================================= */
/* �֐���	:CInfoSkillHEAL::GetElementNoTmp								 */
/* ���e		:�v�f�ԍ����擾													 */
/* ���t		:2009/01/22														 */
/* ========================================================================= */

int CInfoSkillHEAL::GetElementNoTmp(LPCSTR pszName)
{
	int i, nRet;

	nRet = -1;
	for (i = 0; s_aszName[i] != NULL; i ++) {
		if (strcmp (s_aszName[i], pszName) == 0) {
			nRet = i;
			break;
		}
	}

	return nRet;
}


/* ========================================================================= */
/* �֐���	:CInfoSkillHEAL::GetDataSizeTmp									 */
/* ���e		:�f�[�^�T�C�Y���擾												 */
/* ���t		:2009/01/22														 */
/* ========================================================================= */

DWORD CInfoSkillHEAL::GetDataSizeTmp(void)
{
	DWORD dwRet;

	dwRet = 0;
	dwRet += sizeof (m_dwArea);				/* �͈� */
	dwRet += sizeof (m_dwHealType);			/* �񕜎�� */
	dwRet += sizeof (m_dwHitEffectID);		/* �q�b�g���̕\���G�t�F�N�g */
	dwRet += sizeof (m_dwValue1);			/* ����1 */
	dwRet += sizeof (m_dwValue2);			/* ����2 */
	dwRet += sizeof (m_dwDistance);			/* �˒����� */

	return dwRet;
}


/* ========================================================================= */
/* �֐���	:CInfoSkillHEAL::GetDataSizeNoTmp								 */
/* ���e		:�w��v�f�̃f�[�^�T�C�Y���擾									 */
/* ���t		:2009/01/22														 */
/* ========================================================================= */

DWORD CInfoSkillHEAL::GetDataSizeNoTmp(int nNo)
{
	DWORD dwRet;

	dwRet = 0;
	switch (nNo) {
	case 0:	dwRet = sizeof (m_dwArea);			break;		/* �͈� */
	case 1:	dwRet = sizeof (m_dwHealType);		break;		/* �񕜎�� */
	case 2:	dwRet = sizeof (m_dwHitEffectID);	break;		/* �q�b�g���̕\���G�t�F�N�g */
	case 3:	dwRet = sizeof (m_dwValue1);		break;		/* ����1 */
	case 4:	dwRet = sizeof (m_dwValue2);		break;		/* ����2 */
	case 5:	dwRet = sizeof (m_dwDistance);		break;		/* �˒����� */
	}

	return dwRet;
}


/* ========================================================================= */
/* �֐���	:CInfoSkillHEAL::GetNameTmp										 */
/* ���e		:�v�f�����擾													 */
/* ���t		:2009/01/22														 */
/* ========================================================================= */

LPCSTR CInfoSkillHEAL::GetNameTmp(int nNo)
{
	return s_aszName[nNo];
}


/* ========================================================================= */
/* �֐���	:CInfoSkillHEAL::GetWriteDataTmp								 */
/* ���e		:�w��v�f�̕ۑ��p�f�[�^���擾									 */
/* ���t		:2009/01/22														 */
/* ========================================================================= */

PBYTE CInfoSkillHEAL::GetWriteDataTmp(int nNo, PDWORD pdwSize)
{
	PBYTE pRet, pSrc;
	DWORD dwSize;

	pRet		= NULL;
	pSrc		= NULL;
	dwSize		= GetDataSizeNoTmp (nNo);
	*pdwSize	= dwSize;

	if (dwSize == 0) {
		goto Exit;
	}
	pRet = new BYTE[dwSize];

	switch (nNo) {
	case 0:	pSrc = (PBYTE)&m_dwArea;			break;		/* �͈� */
	case 1:	pSrc = (PBYTE)&m_dwHealType;		break;		/* �񕜎�� */
	case 2:	pSrc = (PBYTE)&m_dwHitEffectID;		break;		/* �q�b�g���̕\���G�t�F�N�g */
	case 3:	pSrc = (PBYTE)&m_dwValue1;			break;		/* ����1 */
	case 4:	pSrc = (PBYTE)&m_dwValue2;			break;		/* ����2 */
	case 5:	pSrc = (PBYTE)&m_dwDistance;		break;		/* �˒����� */
	}

	if (pSrc) {
		CopyMemory (pRet, pSrc, dwSize);
	}

Exit:
	return pRet;
}


/* ========================================================================= */
/* �֐���	:CInfoSkillHEAL::ReadElementDataTmp								 */
/* ���e		:�w��v�f�f�[�^��ǂݍ���										 */
/* ���t		:2009/01/22														 */
/* ========================================================================= */

DWORD CInfoSkillHEAL::ReadElementDataTmp(
	PBYTE pSrc,		/* [in] �f�[�^�̓ǂݍ��݌� */
	int nNo)		/* [in] �v�f�ԍ� */
{
	PBYTE pDst;
	DWORD dwSize;

	pDst	= NULL;
	dwSize	= 0;

	switch (nNo) {
	case 0:	pDst = (PBYTE)&m_dwArea;			dwSize = sizeof (m_dwArea);			break;	/* �͈� */
	case 1:	pDst = (PBYTE)&m_dwHealType;		dwSize = sizeof (m_dwHealType);		break;	/* �񕜎�� */
	case 2:	pDst = (PBYTE)&m_dwHitEffectID;		dwSize = sizeof (m_dwHitEffectID);	break;	/* �q�b�g���̕\���G�t�F�N�g */
	case 3:	pDst = (PBYTE)&m_dwValue1;			dwSize = sizeof (m_dwValue1);		break;	/* ����1 */
	case 4:	pDst = (PBYTE)&m_dwValue2;			dwSize = sizeof (m_dwValue2);		break;	/* ����2 */
	case 5:	pDst = (PBYTE)&m_dwDistance;		dwSize = sizeof (m_dwDistance);		break;	/* �˒����� */
	}

	if (pDst) {
		CopyMemory (pDst, pSrc, dwSize);
	}

	return dwSize;
}


/* ========================================================================= */
/* �֐���	:CInfoSkillHEAL::GetDerivationSize								 */
/* ���e		:�h���f�[�^�T�C�Y���擾											 */
/* ���t		:2009/01/10														 */
/* ========================================================================= */

DWORD CInfoSkillHEAL::GetDerivationSize(void)
{
	int i, nCount;
	DWORD dwRet;

	dwRet = sizeof (int);		/* �v�f�� */
	nCount = m_nElementCountDerivation;
	for (i = 0; i < nCount; i ++) {
		dwRet += (strlen (GetNameTmp (i)) + 1);		/* �v�f�� */
		dwRet += sizeof (DWORD);					/* �f�[�^�T�C�Y */
		dwRet += GetDataSizeNoTmp (i);				/* �f�[�^ */
	}

	return dwRet;
}


/* ========================================================================= */
/* �֐���	:CInfoSkillHEAL::GetDerivationWriteData							 */
/* ���e		:�h���f�[�^�̕ۑ��p�f�[�^���擾									 */
/* ���t		:2009/01/10														 */
/* ========================================================================= */

PBYTE CInfoSkillHEAL::GetDerivationWriteData(PDWORD pdwSize)
{
	int i, nCount;
	PBYTE pRet, pRetTmp, pTmp;
	DWORD dwSize, dwTmp;

	dwSize	= GetDerivationSize ();
	pRet	= ZeroNew (dwSize);
	pRetTmp	= pRet;

	nCount = m_nElementCountDerivation;
	CopyMemoryRenew (pRetTmp, &nCount, sizeof (nCount), pRetTmp);	/* �v�f�� */
	for (i = 0; i < nCount; i ++) {
		strcpyRenew ((LPSTR)pRetTmp, GetNameTmp (i), pRetTmp); 		/* �v�f�� */
		dwTmp = GetDataSizeNoTmp (i);
		CopyMemoryRenew (pRetTmp, &dwTmp, sizeof (dwTmp), pRetTmp);	/* �f�[�^�T�C�Y */
		pTmp = GetWriteDataTmp (i, &dwTmp);
		CopyMemoryRenew (pRetTmp, pTmp, dwTmp, pRetTmp);			/* �f�[�^ */
		SAFE_DELETE_ARRAY (pTmp);
	}

	return pRet;
}


/* ========================================================================= */
/* �֐���	:CInfoSkillHEAL::ReadDerivationData								 */
/* ���e		:�h���f�[�^��ǂݍ���											 */
/* ���t		:2009/01/10														 */
/* ========================================================================= */

DWORD CInfoSkillHEAL::ReadDerivationData(PBYTE pSrc)
{
	int i, nCount, nNoTmp;
	PBYTE pSrcTmp;
	DWORD dwSizeTmp;
	CmyString strTmp;

	pSrcTmp = pSrc;

	CopyMemoryRenew (&nCount, pSrcTmp, sizeof (nCount), pSrcTmp);	/* �v�f�� */
	for (i = 0; i < nCount; i ++) {
		StoreRenew (strTmp, (LPCSTR)pSrcTmp, pSrcTmp);						/* �v�f�� */
		CopyMemoryRenew (&dwSizeTmp, pSrcTmp, sizeof (dwSizeTmp), pSrcTmp);	/* �f�[�^�T�C�Y */
		nNoTmp = GetElementNoTmp ((LPCSTR)strTmp);
		if (nNoTmp >= 0) {
			dwSizeTmp = ReadElementDataTmp (pSrcTmp, nNoTmp);
		}
		pSrcTmp += dwSizeTmp;
	}

	return (DWORD)(pSrcTmp - pSrc);
}


/* ========================================================================= */
/* �֐���	:CInfoSkillHEAL::GetSendDataSize								 */
/* ���e		:���M�f�[�^�T�C�Y���擾											 */
/* ���t		:2009/01/22														 */
/* ========================================================================= */

DWORD CInfoSkillHEAL::GetSendDataSize(void)
{
	DWORD dwRet;

	dwRet = CInfoSkillBase::GetSendDataSize ();
	dwRet += sizeof (m_dwArea);				/* �͈� */
	dwRet += sizeof (m_dwHealType);			/* �񕜎�� */
	dwRet += sizeof (m_dwHitEffectID);		/* �q�b�g���̕\���G�t�F�N�g */
	dwRet += sizeof (m_dwValue1);			/* ����1 */
	dwRet += sizeof (m_dwValue2);			/* ����2 */
	dwRet += sizeof (m_dwDistance);			/* �˒����� */

	return dwRet;
}


/* ========================================================================= */
/* �֐���	:CInfoSkillHEAL::GetSendData									 */
/* ���e		:���M�f�[�^���擾												 */
/* ���t		:2009/01/22														 */
/* ========================================================================= */

PBYTE CInfoSkillHEAL::GetSendData(void)
{
	PBYTE pData, pDataTmp, pDataBase;
	DWORD dwSize, dwSizeBase;

	dwSize	 = GetSendDataSize ();
	pData	 = ZeroNew (dwSize);
	pDataTmp = pData;

	pDataBase  = CInfoSkillBase::GetSendData ();
	dwSizeBase = CInfoSkillBase::GetSendDataSize ();
	CopyMemoryRenew (pDataTmp, pDataBase, dwSizeBase, pDataTmp);
	SAFE_DELETE_ARRAY (pDataBase);

	CopyMemoryRenew (pDataTmp, &m_dwArea,			sizeof (m_dwArea),			pDataTmp);	/* �͈� */
	CopyMemoryRenew (pDataTmp, &m_dwHealType,		sizeof (m_dwHealType),		pDataTmp);	/* �񕜎�� */
	CopyMemoryRenew (pDataTmp, &m_dwHitEffectID,	sizeof (m_dwHitEffectID),	pDataTmp);	/* �q�b�g���̕\���G�t�F�N�g */
	CopyMemoryRenew (pDataTmp, &m_dwValue1,			sizeof (m_dwValue1),		pDataTmp);	/* ����1 */
	CopyMemoryRenew (pDataTmp, &m_dwValue2,			sizeof (m_dwValue2),		pDataTmp);	/* ����2 */
	CopyMemoryRenew (pDataTmp, &m_dwDistance,		sizeof (m_dwDistance),		pDataTmp);	/* �˒����� */

	return pData;
}


/* ========================================================================= */
/* �֐���	:CInfoSkillHEAL::SetSendData									 */
/* ���e		:���M�f�[�^�����荞��											 */
/* ���t		:2009/01/22														 */
/* ========================================================================= */

PBYTE CInfoSkillHEAL::SetSendData(PBYTE pSrc)
{
	PBYTE pRet, pDataTmp;

	pRet = pSrc;

	pDataTmp = CInfoSkillBase::SetSendData (pSrc);
	CopyMemoryRenew (&m_dwArea,			pDataTmp, sizeof (m_dwArea),		pDataTmp);	/* �͈� */
	CopyMemoryRenew (&m_dwHealType,		pDataTmp, sizeof (m_dwHealType),	pDataTmp);	/* �񕜎�� */
	CopyMemoryRenew (&m_dwHitEffectID,	pDataTmp, sizeof (m_dwHitEffectID),	pDataTmp);	/* �q�b�g���̕\���G�t�F�N�g */
	CopyMemoryRenew (&m_dwValue1,		pDataTmp, sizeof (m_dwValue1),		pDataTmp);	/* ����1 */
	CopyMemoryRenew (&m_dwValue2,		pDataTmp, sizeof (m_dwValue2),		pDataTmp);	/* ����2 */
	CopyMemoryRenew (&m_dwDistance,		pDataTmp, sizeof (m_dwDistance),	pDataTmp);	/* �˒����� */

	pRet = pDataTmp;
	return pRet;
}


/* ========================================================================= */
/* �֐���	:CInfoSkillHEAL::Copy											 */
/* ���e		:�R�s�[															 */
/* ���t		:2009/01/22														 */
/* ========================================================================= */

void CInfoSkillHEAL::Copy(CInfoSkillBase *pSrc)
{
	PCInfoSkillHEAL pSrcTmp;

	if (pSrc == NULL) {
		return;
	}
	pSrcTmp = (PCInfoSkillHEAL)pSrc;

	CInfoSkillBase::Copy (pSrc);
	if (pSrc->m_nClassType != m_nClassType) {
		return;
	}

	m_dwArea		= pSrcTmp->m_dwArea;			/* �͈� */
	m_dwHealType	= pSrcTmp->m_dwHealType;		/* �񕜎�� */
	m_dwHitEffectID	= pSrcTmp->m_dwHitEffectID;		/* �q�b�g���̕\���G�t�F�N�g */
	m_dwValue1		= pSrcTmp->m_dwValue1;			/* ����1 */
	m_dwValue2		= pSrcTmp->m_dwValue2;			/* ����2 */
	m_dwDistance	= pSrcTmp->m_dwDistance;		/* �˒����� */

}

/* Copyright(C)URARA-works 2009 */
