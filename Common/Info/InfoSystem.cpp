/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:InfoSystem.cpp												 */
/* ���e			:�V�X�e�����N���X �����t�@�C��							 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/09/25													 */
/* ========================================================================= */

#include "stdafx.h"
#include "InfoSystem.h"

/* ========================================================================= */
/* �萔��`																	 */
/* ========================================================================= */

/* �w�b�_��� */
static LPCSTR s_aszName[] = {
	"wStamina",					/* �X�^�~�i */
	"wPower",					/* �r�� */
	"wStrength",				/* �̗� */
	"wMagic",					/* ���� */
	"wSkillful",				/* ��p */
	"wAbillityAT",				/* �U���Z�\ */
	"wAbillityDF",				/* �h��Z�\ */
	"wPAtack",					/* �U���� */
	"wPDefense",				/* �h��� */
	"wPMagic",					/* ���@�� */
	"wPMagicDefense",			/* ���@�h��� */
	"wPHitAverage",				/* ������ */
	"wPAvoidAverage",			/* ��� */
	"wPCriticalAverage",		/* �N���e�B�J���� */
	"wAttrFire",				/* ����[��] */
	"wAttrWind",				/* ����[��] */
	"wAttrWater",				/* ����[��] */
	"wAttrEarth",				/* ����[�y] */
	"wAttrLight",				/* ����[��] */
	"wAttrDark",				/* ����[��] */
	"dwMaxHP",					/* �ő�HP */
	"dwMaxSP",					/* �ő�SP */
	"dwInitPosMapID",			/* �����ʒu�}�b�vID */
	"ptInitPos",				/* �����ʒu���W */
	NULL
};

/* ========================================================================= */
/* �֐���	:CInfoSystem::CInfoSystem										 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/09/25														 */
/* ========================================================================= */

CInfoSystem::CInfoSystem()
{
	m_pInitCharStatus = new STSYSTEM_INITCHARSTATUS;

	ZeroMemory (m_pInitCharStatus, sizeof (STSYSTEM_INITCHARSTATUS));
	for (m_nElementCount = 0; s_aszName[m_nElementCount] != NULL; m_nElementCount ++) {}
}


/* ========================================================================= */
/* �֐���	:CInfoSystem::~CInfoSystem										 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/09/25														 */
/* ========================================================================= */

CInfoSystem::~CInfoSystem()
{
	SAFE_DELETE (m_pInitCharStatus);
}


/* ========================================================================= */
/* �֐���	:CInfoSystem::GetElementNo										 */
/* ���e		:�v�f�ԍ����擾													 */
/* ���t		:2008/09/25														 */
/* ========================================================================= */

int CInfoSystem::GetElementNo(LPCSTR pszName)
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
/* �֐���	:CInfoSystem::GetDataSize										 */
/* ���e		:�f�[�^�T�C�Y���擾												 */
/* ���t		:2008/09/25														 */
/* ========================================================================= */

DWORD CInfoSystem::GetDataSize(void)
{
	DWORD dwRet;

	dwRet = 0;
	dwRet += sizeof (m_pInitCharStatus->wStamina);				/* �X�^�~�i */
	dwRet += sizeof (m_pInitCharStatus->wPower);				/* �r�� */
	dwRet += sizeof (m_pInitCharStatus->wStrength);				/* �̗� */
	dwRet += sizeof (m_pInitCharStatus->wMagic);				/* ���� */
	dwRet += sizeof (m_pInitCharStatus->wSkillful);				/* ��p */
	dwRet += sizeof (m_pInitCharStatus->wAbillityAT);			/* �U���Z�\ */
	dwRet += sizeof (m_pInitCharStatus->wAbillityDF);			/* �h��Z�\ */
	dwRet += sizeof (m_pInitCharStatus->wPAtack);				/* �U���� */
	dwRet += sizeof (m_pInitCharStatus->wPDefense);				/* �h��� */
	dwRet += sizeof (m_pInitCharStatus->wPMagic);				/* ���@�� */
	dwRet += sizeof (m_pInitCharStatus->wPMagicDefense);		/* ���@�h��� */
	dwRet += sizeof (m_pInitCharStatus->wPHitAverage);			/* ������ */
	dwRet += sizeof (m_pInitCharStatus->wPAvoidAverage);		/* ��� */
	dwRet += sizeof (m_pInitCharStatus->wPCriticalAverage);		/* �N���e�B�J���� */
	dwRet += sizeof (m_pInitCharStatus->wAttrFire);				/* ����[��] */
	dwRet += sizeof (m_pInitCharStatus->wAttrWind);				/* ����[��] */
	dwRet += sizeof (m_pInitCharStatus->wAttrWater);			/* ����[��] */
	dwRet += sizeof (m_pInitCharStatus->wAttrEarth);			/* ����[�y] */
	dwRet += sizeof (m_pInitCharStatus->wAttrLight);			/* ����[��] */
	dwRet += sizeof (m_pInitCharStatus->wAttrDark);				/* ����[��] */
	dwRet += sizeof (m_pInitCharStatus->dwMaxHP);				/* �ő�HP */
	dwRet += sizeof (m_pInitCharStatus->dwMaxSP);				/* �ő�SP */
	dwRet += sizeof (m_pInitCharStatus->dwInitPosMapID);		/* �����ʒu�}�b�vID */
	dwRet += sizeof (m_pInitCharStatus->ptInitPos);				/* �����ʒu���W */

	return dwRet;
}


/* ========================================================================= */
/* �֐���	:CInfoSystem::GetDataSizeNo										 */
/* ���e		:�w��v�f�̃f�[�^�T�C�Y���擾									 */
/* ���t		:2008/09/25														 */
/* ========================================================================= */

DWORD CInfoSystem::GetDataSizeNo(int nNo)
{
	DWORD dwRet;

	dwRet = 0;

	switch (nNo) {
	case 0:		dwRet = sizeof (m_pInitCharStatus->wStamina);			break;	/* �X�^�~�i */
	case 1:		dwRet = sizeof (m_pInitCharStatus->wPower);				break;	/* �r�� */
	case 2:		dwRet = sizeof (m_pInitCharStatus->wStrength);			break;	/* �̗� */
	case 3:		dwRet = sizeof (m_pInitCharStatus->wMagic);				break;	/* ���� */
	case 4:		dwRet = sizeof (m_pInitCharStatus->wSkillful);			break;	/* ��p */
	case 5:		dwRet = sizeof (m_pInitCharStatus->wAbillityAT);		break;	/* �U���Z�\ */
	case 6:		dwRet = sizeof (m_pInitCharStatus->wAbillityDF);		break;	/* �h��Z�\ */
	case 7:		dwRet = sizeof (m_pInitCharStatus->wPAtack);			break;	/* �U���� */
	case 8:		dwRet = sizeof (m_pInitCharStatus->wPDefense);			break;	/* �h��� */
	case 9:		dwRet = sizeof (m_pInitCharStatus->wPMagic);			break;	/* ���@�� */
	case 10:	dwRet = sizeof (m_pInitCharStatus->wPMagicDefense);		break;	/* ���@�h��� */
	case 11:	dwRet = sizeof (m_pInitCharStatus->wPHitAverage);		break;	/* ������ */
	case 12:	dwRet = sizeof (m_pInitCharStatus->wPAvoidAverage);		break;	/* ��� */
	case 13:	dwRet = sizeof (m_pInitCharStatus->wPCriticalAverage);	break;	/* �N���e�B�J���� */
	case 14:	dwRet = sizeof (m_pInitCharStatus->wAttrFire);			break;	/* ����[��] */
	case 15:	dwRet = sizeof (m_pInitCharStatus->wAttrWind);			break;	/* ����[��] */
	case 16:	dwRet = sizeof (m_pInitCharStatus->wAttrWater);			break;	/* ����[��] */
	case 17:	dwRet = sizeof (m_pInitCharStatus->wAttrEarth);			break;	/* ����[�y] */
	case 18:	dwRet = sizeof (m_pInitCharStatus->wAttrLight);			break;	/* ����[��] */
	case 19:	dwRet = sizeof (m_pInitCharStatus->wAttrDark);			break;	/* ����[��] */
	case 20:	dwRet = sizeof (m_pInitCharStatus->dwMaxHP);			break;	/* �ő�HP */
	case 21:	dwRet = sizeof (m_pInitCharStatus->dwMaxSP);			break;	/* �ő�SP */
	case 22:	dwRet = sizeof (m_pInitCharStatus->dwInitPosMapID);		break;	/* �����ʒu�}�b�vID */
	case 23:	dwRet = sizeof (m_pInitCharStatus->ptInitPos);			break;	/* �����ʒu���W */
	}

	return dwRet;
}


/* ========================================================================= */
/* �֐���	:CInfoSystem::GetName											 */
/* ���e		:�v�f�����擾													 */
/* ���t		:2008/09/25														 */
/* ========================================================================= */

LPCSTR CInfoSystem::GetName(int nNo)
{
	return s_aszName[nNo];
}


/* ========================================================================= */
/* �֐���	:CInfoSystem::GetWriteData										 */
/* ���e		:�w��v�f�̕ۑ��p�f�[�^���擾									 */
/* ���t		:2008/09/25														 */
/* ========================================================================= */

PBYTE CInfoSystem::GetWriteData(int nNo, PDWORD pdwSize)
{
	PBYTE pRet, pSrc;
	DWORD dwSize;

	pRet		= NULL;
	pSrc		= NULL;
	dwSize		= GetDataSizeNo (nNo);
	*pdwSize	= dwSize;

	if (dwSize == 0) {
		goto Exit;
	}
	pRet = new BYTE[dwSize];

	switch (nNo) {
	case 0:		pSrc = (PBYTE)&m_pInitCharStatus->wStamina;				break;	/* �X�^�~�i */
	case 1:		pSrc = (PBYTE)&m_pInitCharStatus->wPower;				break;	/* �r�� */
	case 2:		pSrc = (PBYTE)&m_pInitCharStatus->wStrength;			break;	/* �̗� */
	case 3:		pSrc = (PBYTE)&m_pInitCharStatus->wMagic;				break;	/* ���� */
	case 4:		pSrc = (PBYTE)&m_pInitCharStatus->wSkillful;			break;	/* ��p */
	case 5:		pSrc = (PBYTE)&m_pInitCharStatus->wAbillityAT;			break;	/* �U���Z�\ */
	case 6:		pSrc = (PBYTE)&m_pInitCharStatus->wAbillityDF;			break;	/* �h��Z�\ */
	case 7:		pSrc = (PBYTE)&m_pInitCharStatus->wPAtack;				break;	/* �U���� */
	case 8:		pSrc = (PBYTE)&m_pInitCharStatus->wPDefense;			break;	/* �h��� */
	case 9:		pSrc = (PBYTE)&m_pInitCharStatus->wPMagic;				break;	/* ���@�� */
	case 10:	pSrc = (PBYTE)&m_pInitCharStatus->wPMagicDefense;		break;	/* ���@�h��� */
	case 11:	pSrc = (PBYTE)&m_pInitCharStatus->wPHitAverage;			break;	/* ������ */
	case 12:	pSrc = (PBYTE)&m_pInitCharStatus->wPAvoidAverage;		break;	/* ��� */
	case 13:	pSrc = (PBYTE)&m_pInitCharStatus->wPCriticalAverage;	break;	/* �N���e�B�J���� */
	case 14:	pSrc = (PBYTE)&m_pInitCharStatus->wAttrFire;			break;	/* ����[��] */
	case 15:	pSrc = (PBYTE)&m_pInitCharStatus->wAttrWind;			break;	/* ����[��] */
	case 16:	pSrc = (PBYTE)&m_pInitCharStatus->wAttrWater;			break;	/* ����[��] */
	case 17:	pSrc = (PBYTE)&m_pInitCharStatus->wAttrEarth;			break;	/* ����[�y] */
	case 18:	pSrc = (PBYTE)&m_pInitCharStatus->wAttrLight;			break;	/* ����[��] */
	case 19:	pSrc = (PBYTE)&m_pInitCharStatus->wAttrDark;			break;	/* ����[��] */
	case 20:	pSrc = (PBYTE)&m_pInitCharStatus->dwMaxHP;				break;	/* �ő�HP */
	case 21:	pSrc = (PBYTE)&m_pInitCharStatus->dwMaxSP;				break;	/* �ő�SP */
	case 22:	pSrc = (PBYTE)&m_pInitCharStatus->dwInitPosMapID;		break;	/* �����ʒu�}�b�vID */
	case 23:	pSrc = (PBYTE)&m_pInitCharStatus->ptInitPos;			break;	/* �����ʒu���W */
	}

	if (pSrc) {
		CopyMemory (pRet, pSrc, dwSize);
	}

Exit:
	return pRet;
}


/* ========================================================================= */
/* �֐���	:CInfoSystem::ReadElementData									 */
/* ���e		:�w��v�f�f�[�^��ǂݍ���										 */
/* ���t		:2008/09/25														 */
/* ========================================================================= */

DWORD CInfoSystem::ReadElementData(
	PBYTE pSrc,		/* [in] �f�[�^�̓ǂݍ��݌� */
	int nNo)		/* [in] �v�f�ԍ� */
{
	PBYTE pDst;
	DWORD dwSize;

	pDst	= NULL;
	dwSize	= 0;

	switch (nNo) {
	case 0:		pDst = (PBYTE)&m_pInitCharStatus->wStamina;				dwSize = sizeof (m_pInitCharStatus->wStamina);			break;	/* �X�^�~�i */
	case 1:		pDst = (PBYTE)&m_pInitCharStatus->wPower;				dwSize = sizeof (m_pInitCharStatus->wPower);			break;	/* �r�� */
	case 2:		pDst = (PBYTE)&m_pInitCharStatus->wStrength;			dwSize = sizeof (m_pInitCharStatus->wStrength);			break;	/* �̗� */
	case 3:		pDst = (PBYTE)&m_pInitCharStatus->wMagic;				dwSize = sizeof (m_pInitCharStatus->wMagic);			break;	/* ���� */
	case 4:		pDst = (PBYTE)&m_pInitCharStatus->wSkillful;			dwSize = sizeof (m_pInitCharStatus->wSkillful);			break;	/* ��p */
	case 5:		pDst = (PBYTE)&m_pInitCharStatus->wAbillityAT;			dwSize = sizeof (m_pInitCharStatus->wAbillityAT);		break;	/* �U���Z�\ */
	case 6:		pDst = (PBYTE)&m_pInitCharStatus->wAbillityDF;			dwSize = sizeof (m_pInitCharStatus->wAbillityDF);		break;	/* �h��Z�\ */
	case 7:		pDst = (PBYTE)&m_pInitCharStatus->wPAtack;				dwSize = sizeof (m_pInitCharStatus->wPAtack);			break;	/* �U���� */
	case 8:		pDst = (PBYTE)&m_pInitCharStatus->wPDefense;			dwSize = sizeof (m_pInitCharStatus->wPDefense);			break;	/* �h��� */
	case 9:		pDst = (PBYTE)&m_pInitCharStatus->wPMagic;				dwSize = sizeof (m_pInitCharStatus->wPMagic);			break;	/* ���@�� */
	case 10:	pDst = (PBYTE)&m_pInitCharStatus->wPMagicDefense;		dwSize = sizeof (m_pInitCharStatus->wPMagicDefense);	break;	/* ���@�h��� */
	case 11:	pDst = (PBYTE)&m_pInitCharStatus->wPHitAverage;			dwSize = sizeof (m_pInitCharStatus->wPHitAverage);		break;	/* ������ */
	case 12:	pDst = (PBYTE)&m_pInitCharStatus->wPAvoidAverage;		dwSize = sizeof (m_pInitCharStatus->wPAvoidAverage);	break;	/* ��� */
	case 13:	pDst = (PBYTE)&m_pInitCharStatus->wPCriticalAverage;	dwSize = sizeof (m_pInitCharStatus->wPCriticalAverage);	break;	/* �N���e�B�J���� */
	case 14:	pDst = (PBYTE)&m_pInitCharStatus->wAttrFire;			dwSize = sizeof (m_pInitCharStatus->wAttrFire);			break;	/* ����[��] */
	case 15:	pDst = (PBYTE)&m_pInitCharStatus->wAttrWind;			dwSize = sizeof (m_pInitCharStatus->wAttrWind);			break;	/* ����[��] */
	case 16:	pDst = (PBYTE)&m_pInitCharStatus->wAttrWater;			dwSize = sizeof (m_pInitCharStatus->wAttrWater);		break;	/* ����[��] */
	case 17:	pDst = (PBYTE)&m_pInitCharStatus->wAttrEarth;			dwSize = sizeof (m_pInitCharStatus->wAttrEarth);		break;	/* ����[�y] */
	case 18:	pDst = (PBYTE)&m_pInitCharStatus->wAttrLight;			dwSize = sizeof (m_pInitCharStatus->wAttrLight);		break;	/* ����[��] */
	case 19:	pDst = (PBYTE)&m_pInitCharStatus->wAttrDark;			dwSize = sizeof (m_pInitCharStatus->wAttrDark);			break;	/* ����[��] */
	case 20:	pDst = (PBYTE)&m_pInitCharStatus->dwMaxHP;				dwSize = sizeof (m_pInitCharStatus->dwMaxHP);			break;	/* �ő�HP */
	case 21:	pDst = (PBYTE)&m_pInitCharStatus->dwMaxSP;				dwSize = sizeof (m_pInitCharStatus->dwMaxSP);			break;	/* �ő�SP */
	case 22:	pDst = (PBYTE)&m_pInitCharStatus->dwInitPosMapID;		dwSize = sizeof (m_pInitCharStatus->dwInitPosMapID);	break;	/* �����ʒu�}�b�vID */
	case 23:	pDst = (PBYTE)&m_pInitCharStatus->ptInitPos;			dwSize = sizeof (m_pInitCharStatus->ptInitPos);			break;	/* �����ʒu���W */
	}

	if (pDst) {
		CopyMemory (pDst, pSrc, dwSize);
	}

	return dwSize;
}


/* ========================================================================= */
/* �֐���	:CInfoSystem::GetSendDataSize									 */
/* ���e		:���M�f�[�^�T�C�Y���擾											 */
/* ���t		:2008/09/25														 */
/* ========================================================================= */

DWORD CInfoSystem::GetSendDataSize(void)
{
	DWORD dwRet;

	dwRet = 0;
	dwRet += sizeof (STSYSTEM_INITCHARSTATUS);

	return dwRet;
}


/* ========================================================================= */
/* �֐���	:CInfoSystem::GetSendData										 */
/* ���e		:���M�f�[�^���擾												 */
/* ���t		:2008/09/25														 */
/* ========================================================================= */

PBYTE CInfoSystem::GetSendData(void)
{
	PBYTE pRet, pData, pDataTmp;
	DWORD dwSize;

	pRet = NULL;

	dwSize		= GetSendDataSize ();
	pData		= ZeroNew (dwSize);
	pDataTmp	= pData;

	CopyMemoryRenew (pDataTmp, m_pInitCharStatus, sizeof (STSYSTEM_INITCHARSTATUS), pDataTmp);	/* �L�����X�e�[�^�X�����l */

	pRet = pData;
	return pRet;
}


/* ========================================================================= */
/* �֐���	:CInfoSystem::SetSendData										 */
/* ���e		:���M�f�[�^��ݒ�												 */
/* ���t		:2008/09/25														 */
/* �߂�l	:����������̃A�h���X											 */
/* ========================================================================= */

PBYTE CInfoSystem::SetSendData(PBYTE pSrc)
{
	PBYTE pRet, pDataTmp;

	pRet = pSrc;

	pDataTmp = pSrc;
	CopyMemoryRenew (m_pInitCharStatus, pDataTmp, sizeof (STSYSTEM_INITCHARSTATUS), pDataTmp);	/* �L�����X�e�[�^�X�����l */

	pRet = pDataTmp;
	return pRet;
}


/* ========================================================================= */
/* �֐���	:CInfoSystem::Copy												 */
/* ���e		:�R�s�[															 */
/* ���t		:2008/09/25														 */
/* ========================================================================= */

void CInfoSystem::Copy(CInfoSystem *pSrc)
{
	CopyMemory (m_pInitCharStatus, pSrc->m_pInitCharStatus, sizeof (STSYSTEM_INITCHARSTATUS));	/* �L�����X�e�[�^�X�����l */
}

/* Copyright(C)URARA-works 2008 */
