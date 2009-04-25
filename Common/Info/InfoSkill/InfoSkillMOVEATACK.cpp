/* Copyright(C)URARA-works 2009 */
/* ========================================================================= */
/* �t�@�C����	:InfoSkillMOVEATACK.cpp										 */
/* ���e			:�X�L�����(�ړ����čU��)�N���X �����t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2009/01/08													 */
/* ========================================================================= */

#include "stdafx.h"
#include "InfoSkillMOVEATACK.h"

/* ========================================================================= */
/* �萔��`																	 */
/* ========================================================================= */

/* �w�b�_��� */
static LPCSTR s_aszName[] = {
	"m_dwTartgetType",		/* �U���Ώ� */
	"m_dwHitEffectID",		/* �q�b�g���̕\���G�t�F�N�g */
	"m_dwEffectID",			/* �����ɂ��\���G�t�F�N�g */
	"m_dwPutType",			/* ���ˎ�� */
	"m_dwAliveTime",		/* �ϋv���� */
	"m_dwWaitTime",			/* �ړ����x */
	"m_dwValue1",			/* ����1 */
	"m_dwValue2",			/* ����2 */
	"m_dwDistance",			/* �˒����� */
	"m_bHitQuit",			/* �q�b�g����Ə��� */
	"m_bDistanceDelete",	/* �˒������܂ōs���Ə����� */
	NULL
};


/* ========================================================================= */
/* �֐���	:CInfoSkillMOVEATACK::CInfoSkillMOVEATACK						 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2009/01/08														 */
/* ========================================================================= */

CInfoSkillMOVEATACK::CInfoSkillMOVEATACK()
{
	m_nClassType		= INFOSKILLTYPE_MOVEATACK;
	m_dwHitEffectID		= 0;
	m_dwTartgetType		= 0;
	m_dwPutType			= 0;
	m_dwAliveTime		= 0;
	m_dwWaitTime		= 0;
	m_dwValue1			= 0;
	m_dwValue2			= 0;
	m_dwDistance		= 0;
	m_bHitQuit			= FALSE;
	m_bDistanceDelete	= FALSE;

	ZeroMemory (m_adwEffectID, sizeof (m_adwEffectID));

	for (m_nElementCountDerivation = 0; s_aszName[m_nElementCountDerivation] != NULL; m_nElementCountDerivation ++) {}
}


/* ========================================================================= */
/* �֐���	:CInfoSkillMOVEATACK::~CInfoSkillMOVEATACK						 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2009/01/08														 */
/* ========================================================================= */

CInfoSkillMOVEATACK::~CInfoSkillMOVEATACK()
{
}


/* ========================================================================= */
/* �֐���	:CInfoSkillMOVEATACK::GetElementNoTmp							 */
/* ���e		:�v�f�ԍ����擾													 */
/* ���t		:2009/01/08														 */
/* ========================================================================= */

int CInfoSkillMOVEATACK::GetElementNoTmp(LPCSTR pszName)
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
/* �֐���	:CInfoSkillMOVEATACK::GetDataSizeTmp							 */
/* ���e		:�f�[�^�T�C�Y���擾												 */
/* ���t		:2009/01/08														 */
/* ========================================================================= */

DWORD CInfoSkillMOVEATACK::GetDataSizeTmp(void)
{
	DWORD dwRet;

	dwRet = 0;
	dwRet += sizeof (m_dwTartgetType);		/* �U���Ώ� */
	dwRet += sizeof (m_dwHitEffectID);		/* �q�b�g���̕\���G�t�F�N�g */
	dwRet += sizeof (m_adwEffectID);		/* �����ɂ��\���G�t�F�N�g */
	dwRet += sizeof (m_dwPutType);			/* ���ˎ�� */
	dwRet += sizeof (m_dwAliveTime);		/* �ϋv���� */
	dwRet += sizeof (m_dwWaitTime);			/* �ړ����x */
	dwRet += sizeof (m_dwValue1);			/* ����1 */
	dwRet += sizeof (m_dwValue2);			/* ����2 */
	dwRet += sizeof (m_dwDistance);			/* �˒����� */
	dwRet += sizeof (m_bHitQuit);			/* �q�b�g����Ə��� */
	dwRet += sizeof (m_bDistanceDelete);	/* �˒������܂ōs���Ə����� */

	return dwRet;
}


/* ========================================================================= */
/* �֐���	:CInfoSkillMOVEATACK::GetDataSizeNoTmp							 */
/* ���e		:�w��v�f�̃f�[�^�T�C�Y���擾									 */
/* ���t		:2009/01/08														 */
/* ========================================================================= */

DWORD CInfoSkillMOVEATACK::GetDataSizeNoTmp(int nNo)
{
	DWORD dwRet;

	dwRet = 0;
	switch (nNo) {
	case 0:		dwRet = sizeof (m_dwTartgetType);	break;		/* �U���Ώ� */
	case 1:		dwRet = sizeof (m_dwHitEffectID);	break;		/* �q�b�g���̕\���G�t�F�N�g */
	case 2:		dwRet = sizeof (m_adwEffectID);		break;		/* �����ɂ��\���G�t�F�N�g */
	case 3:		dwRet = sizeof (m_dwPutType);		break;		/* ���ˎ�� */
	case 4:		dwRet = sizeof (m_dwAliveTime);		break;		/* �ϋv���� */
	case 5:		dwRet = sizeof (m_dwWaitTime);		break;		/* �ړ����x */
	case 6:		dwRet = sizeof (m_dwValue1);		break;		/* ����1 */
	case 7:		dwRet = sizeof (m_dwValue2);		break;		/* ����2 */
	case 8:		dwRet = sizeof (m_dwDistance);		break;		/* �˒����� */
	case 9:		dwRet = sizeof (m_bHitQuit);		break;		/* �q�b�g����Ə��� */
	case 10:	dwRet = sizeof (m_bDistanceDelete);	break;		/* �˒������܂ōs���Ə����� */
	}

	return dwRet;
}


/* ========================================================================= */
/* �֐���	:CInfoSkillMOVEATACK::GetNameTmp								 */
/* ���e		:�v�f�����擾													 */
/* ���t		:2009/01/08														 */
/* ========================================================================= */

LPCSTR CInfoSkillMOVEATACK::GetNameTmp(int nNo)
{
	return s_aszName[nNo];
}


/* ========================================================================= */
/* �֐���	:CInfoSkillMOVEATACK::GetWriteDataTmp							 */
/* ���e		:�w��v�f�̕ۑ��p�f�[�^���擾									 */
/* ���t		:2009/01/08														 */
/* ========================================================================= */

PBYTE CInfoSkillMOVEATACK::GetWriteDataTmp(int nNo, PDWORD pdwSize)
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
	case 0:		pSrc = (PBYTE)&m_dwTartgetType;		break;		/* �U���Ώ� */
	case 1:		pSrc = (PBYTE)&m_dwHitEffectID;		break;		/* �q�b�g���̕\���G�t�F�N�g */
	case 2:		pSrc = (PBYTE)&m_adwEffectID;		break;		/* �����ɂ��\���G�t�F�N�g */
	case 3:		pSrc = (PBYTE)&m_dwPutType;			break;		/* ���ˎ�� */
	case 4:		pSrc = (PBYTE)&m_dwAliveTime;		break;		/* �ϋv���� */
	case 5:		pSrc = (PBYTE)&m_dwWaitTime;		break;		/* �ړ����x */
	case 6:		pSrc = (PBYTE)&m_dwValue1;			break;		/* ����1 */
	case 7:		pSrc = (PBYTE)&m_dwValue2;			break;		/* ����2 */
	case 8:		pSrc = (PBYTE)&m_dwDistance;		break;		/* �˒����� */
	case 9:		pSrc = (PBYTE)&m_bHitQuit;			break;		/* �q�b�g����Ə��� */
	case 10:	pSrc = (PBYTE)&m_bDistanceDelete;	break;		/* �˒������܂ōs���Ə����� */
	}

	if (pSrc) {
		CopyMemory (pRet, pSrc, dwSize);
	}

Exit:
	return pRet;
}


/* ========================================================================= */
/* �֐���	:CInfoSkillMOVEATACK::ReadElementDataTmp						 */
/* ���e		:�w��v�f�f�[�^��ǂݍ���										 */
/* ���t		:2009/01/08														 */
/* ========================================================================= */

DWORD CInfoSkillMOVEATACK::ReadElementDataTmp(
	PBYTE pSrc,		/* [in] �f�[�^�̓ǂݍ��݌� */
	int nNo)		/* [in] �v�f�ԍ� */
{
	PBYTE pDst;
	DWORD dwSize;

	pDst	= NULL;
	dwSize	= 0;

	switch (nNo) {
	case 0:		pDst = (PBYTE)&m_dwTartgetType;		dwSize = sizeof (m_dwTartgetType);		break;		/* �U���Ώ� */
	case 1:		pDst = (PBYTE)&m_dwHitEffectID;		dwSize = sizeof (m_dwHitEffectID);		break;		/* �q�b�g���̕\���G�t�F�N�g */
	case 2:		pDst = (PBYTE)&m_adwEffectID;		dwSize = sizeof (m_adwEffectID);		break;		/* �����ɂ��\���G�t�F�N�g */
	case 3:		pDst = (PBYTE)&m_dwPutType;			dwSize = sizeof (m_dwPutType);			break;		/* ���ˎ�� */
	case 4:		pDst = (PBYTE)&m_dwAliveTime;		dwSize = sizeof (m_dwAliveTime);		break;		/* �ϋv���� */
	case 5:		pDst = (PBYTE)&m_dwWaitTime;		dwSize = sizeof (m_dwWaitTime);			break;		/* �ړ����x */
	case 6:		pDst = (PBYTE)&m_dwValue1;			dwSize = sizeof (m_dwValue1);			break;		/* ����1 */
	case 7:		pDst = (PBYTE)&m_dwValue2;			dwSize = sizeof (m_dwValue2);			break;		/* ����2 */
	case 8:		pDst = (PBYTE)&m_dwDistance;		dwSize = sizeof (m_dwDistance);			break;		/* �˒����� */
	case 9:		pDst = (PBYTE)&m_bHitQuit;			dwSize = sizeof (m_bHitQuit);			break;		/* �q�b�g����Ə��� */
	case 10:	pDst = (PBYTE)&m_bDistanceDelete;	dwSize = sizeof (m_bDistanceDelete);	break;		/* �˒������܂ōs���Ə����� */
	}

	if (pDst) {
		CopyMemory (pDst, pSrc, dwSize);
	}

	return dwSize;
}


/* ========================================================================= */
/* �֐���	:CInfoSkillMOVEATACK::GetDerivationSize							 */
/* ���e		:�h���f�[�^�T�C�Y���擾											 */
/* ���t		:2009/01/10														 */
/* ========================================================================= */

DWORD CInfoSkillMOVEATACK::GetDerivationSize(void)
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
/* �֐���	:CInfoSkillMOVEATACK::GetDerivationWriteData					 */
/* ���e		:�h���f�[�^�̕ۑ��p�f�[�^���擾									 */
/* ���t		:2009/01/10														 */
/* ========================================================================= */

PBYTE CInfoSkillMOVEATACK::GetDerivationWriteData(PDWORD pdwSize)
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
/* �֐���	:CInfoSkillMOVEATACK::ReadDerivationData						 */
/* ���e		:�h���f�[�^��ǂݍ���											 */
/* ���t		:2009/01/10														 */
/* ========================================================================= */

DWORD CInfoSkillMOVEATACK::ReadDerivationData(PBYTE pSrc)
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
/* �֐���	:CInfoSkillMOVEATACK::GetSendDataSize							 */
/* ���e		:���M�f�[�^�T�C�Y���擾											 */
/* ���t		:2009/01/08														 */
/* ========================================================================= */

DWORD CInfoSkillMOVEATACK::GetSendDataSize(void)
{
	DWORD dwRet;

	dwRet = CInfoSkillBase::GetSendDataSize ();
	dwRet += sizeof (m_dwTartgetType);		/* �U���Ώ� */
	dwRet += sizeof (m_dwHitEffectID);		/* �q�b�g���̕\���G�t�F�N�g */
	dwRet += sizeof (m_adwEffectID);		/* �����ɂ��\���G�t�F�N�g */
	dwRet += sizeof (m_dwPutType);			/* ���ˎ�� */
	dwRet += sizeof (m_dwAliveTime);		/* �ϋv���� */
	dwRet += sizeof (m_dwWaitTime);			/* �ړ����x */
	dwRet += sizeof (m_dwValue1);			/* ����1 */
	dwRet += sizeof (m_dwValue2);			/* ����2 */
	dwRet += sizeof (m_dwDistance);			/* �˒����� */
	dwRet += sizeof (m_bHitQuit);			/* �q�b�g����Ə��� */
	dwRet += sizeof (m_bDistanceDelete);	/* �˒������܂ōs���Ə����� */

	return dwRet;
}


/* ========================================================================= */
/* �֐���	:CInfoSkillMOVEATACK::GetSendData								 */
/* ���e		:���M�f�[�^���擾												 */
/* ���t		:2009/01/08														 */
/* ========================================================================= */

PBYTE CInfoSkillMOVEATACK::GetSendData(void)
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

	CopyMemoryRenew (pDataTmp, &m_dwTartgetType,	sizeof (m_dwTartgetType),	pDataTmp);	/* �U���Ώ� */
	CopyMemoryRenew (pDataTmp, &m_dwHitEffectID,	sizeof (m_dwHitEffectID),	pDataTmp);	/* �q�b�g���̕\���G�t�F�N�g */
	CopyMemoryRenew (pDataTmp, &m_adwEffectID,		sizeof (m_adwEffectID),		pDataTmp);	/* �����ɂ��\���G�t�F�N�g */
	CopyMemoryRenew (pDataTmp, &m_dwPutType,		sizeof (m_dwPutType),		pDataTmp);	/* ���ˎ�� */
	CopyMemoryRenew (pDataTmp, &m_dwAliveTime,		sizeof (m_dwAliveTime),		pDataTmp);	/* �ϋv���� */
	CopyMemoryRenew (pDataTmp, &m_dwWaitTime,		sizeof (m_dwWaitTime),		pDataTmp);	/* �ړ����x */
	CopyMemoryRenew (pDataTmp, &m_dwValue1,			sizeof (m_dwValue1),		pDataTmp);	/* ����1 */
	CopyMemoryRenew (pDataTmp, &m_dwValue2,			sizeof (m_dwValue2),		pDataTmp);	/* ����2 */
	CopyMemoryRenew (pDataTmp, &m_dwDistance,		sizeof (m_dwDistance),		pDataTmp);	/* �˒����� */
	CopyMemoryRenew (pDataTmp, &m_bHitQuit,			sizeof (m_bHitQuit),		pDataTmp);	/* �q�b�g����Ə��� */
	CopyMemoryRenew (pDataTmp, &m_bDistanceDelete,	sizeof (m_bDistanceDelete),	pDataTmp);	/* �˒������܂ōs���Ə����� */

	return pData;
}


/* ========================================================================= */
/* �֐���	:CInfoSkillMOVEATACK::SetSendData								 */
/* ���e		:���M�f�[�^�����荞��											 */
/* ���t		:2009/01/08														 */
/* ========================================================================= */

PBYTE CInfoSkillMOVEATACK::SetSendData(PBYTE pSrc)
{
	PBYTE pRet, pDataTmp;

	pRet = pSrc;

	pDataTmp = CInfoSkillBase::SetSendData (pSrc);
	CopyMemoryRenew (&m_dwTartgetType,		pDataTmp, sizeof (m_dwTartgetType),		pDataTmp);	/* �U���Ώ� */
	CopyMemoryRenew (&m_dwHitEffectID,		pDataTmp, sizeof (m_dwHitEffectID),		pDataTmp);	/* �q�b�g���̕\���G�t�F�N�g */
	CopyMemoryRenew (&m_adwEffectID,		pDataTmp, sizeof (m_adwEffectID),		pDataTmp);	/* �����ɂ��\���G�t�F�N�g */
	CopyMemoryRenew (&m_dwPutType,			pDataTmp, sizeof (m_dwPutType),			pDataTmp);	/* ���ˎ�� */
	CopyMemoryRenew (&m_dwAliveTime,		pDataTmp, sizeof (m_dwAliveTime),		pDataTmp);	/* �ϋv���� */
	CopyMemoryRenew (&m_dwWaitTime,			pDataTmp, sizeof (m_dwWaitTime),		pDataTmp);	/* �ړ����x */
	CopyMemoryRenew (&m_dwValue1,			pDataTmp, sizeof (m_dwValue1),			pDataTmp);	/* ����1 */
	CopyMemoryRenew (&m_dwValue2,			pDataTmp, sizeof (m_dwValue2),			pDataTmp);	/* ����2 */
	CopyMemoryRenew (&m_dwDistance,			pDataTmp, sizeof (m_dwDistance),		pDataTmp);	/* �˒����� */
	CopyMemoryRenew (&m_bHitQuit,			pDataTmp, sizeof (m_bHitQuit),			pDataTmp);	/* �q�b�g����Ə��� */
	CopyMemoryRenew (&m_bDistanceDelete,	pDataTmp, sizeof (m_bDistanceDelete),	pDataTmp);	/* �˒������܂ōs���Ə����� */

	pRet = pDataTmp;
	return pRet;
}


/* ========================================================================= */
/* �֐���	:CInfoSkillMOVEATACK::Copy										 */
/* ���e		:�R�s�[															 */
/* ���t		:2009/01/08														 */
/* ========================================================================= */

void CInfoSkillMOVEATACK::Copy(CInfoSkillBase *pSrc)
{
	PCInfoSkillMOVEATACK pSrcTmp;

	if (pSrc == NULL) {
		return;
	}
	pSrcTmp = (PCInfoSkillMOVEATACK)pSrc;

	CInfoSkillBase::Copy (pSrc);
	if (pSrc->m_nClassType != m_nClassType) {
		return;
	}

	m_dwHitEffectID		= pSrcTmp->m_dwHitEffectID;		/* �q�b�g���̕\���G�t�F�N�g */
	m_dwTartgetType		= pSrcTmp->m_dwTartgetType;		/* �U���Ώ� */
	m_dwPutType			= pSrcTmp->m_dwPutType;			/* ���ˎ�� */
	m_dwAliveTime		= pSrcTmp->m_dwAliveTime;		/* �ϋv���� */
	m_dwWaitTime		= pSrcTmp->m_dwWaitTime;		/* �ړ����x */
	m_dwValue1			= pSrcTmp->m_dwValue1;			/* ����1 */
	m_dwValue2			= pSrcTmp->m_dwValue2;			/* ����2 */
	m_dwDistance		= pSrcTmp->m_dwDistance;		/* �˒����� */
	m_bHitQuit			= pSrcTmp->m_bHitQuit;			/* �q�b�g����Ə��� */
	m_bDistanceDelete	= pSrcTmp->m_bDistanceDelete;	/* �˒������܂ōs���Ə����� */

	/* �����ɂ��\���G�t�F�N�g */
	CopyMemory (m_adwEffectID, pSrcTmp->m_adwEffectID, sizeof (m_adwEffectID));
}

/* Copyright(C)URARA-works 2009 */
