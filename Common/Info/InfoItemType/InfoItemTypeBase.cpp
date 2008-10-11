/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:InfoItemTypeBase.cpp										 */
/* ���e			:�A�C�e����ʊ����N���X �����t�@�C��					 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/09/26													 */
/* ========================================================================= */

#include "stdafx.h"
#include "InfoMotion.h"
#include "InfoItemTypeBase.h"

/* ========================================================================= */
/* �萔��`																	 */
/* ========================================================================= */

/* �w�b�_��� */
static LPCSTR s_aszName[] = {
	"m_byDelAverage",		/* �g���ĂȂ��Ȃ�m�� */
	"m_byTarget",			/* �g�p�Ώ� */
	"m_byArea",				/* �g�p�͈� */
	"m_bPutOn",				/* ��ɒu���邩���� */
	"m_dwTypeID",			/* ���ID */
	"m_dwItemTypeID",		/* �A�C�e�����ID */
	"m_dwGrpID",			/* �n�ʂɂ��鎞�̉摜ID */
	"m_dwIconGrpID",		/* �o�b�N�p�b�N���̉摜ID */
	"m_strName",			/* �A�C�e���� */
	"m_dwDropSoundID",		/* �������Ƃ��̌��ʉ�ID */
	"m_wGrpIDMain",			/* �摜ID���C�� */
	"m_wGrpIDSub",			/* �摜ID�T�u */
	"m_dwUseEffectID",		/* �g�������ɍĐ�����G�t�F�N�gID */
	"m_dwUseSoundID",		/* �g�������ɍĐ�������ʉ�ID */
	"m_dwWeaponInfoID",		/* ������ID */
	"m_dwValue",			/* �U���� */
	"m_dwMoveWait",			/* ���x */
	"m_dwMoveCount",		/* �򋗗� */
	"m_dwValue2",			/* ����(�ő�) */
	NULL
};


/* ========================================================================= */
/* �֐���	:CInfoItemTypeBase::CInfoItemTypeBase							 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/09/26														 */
/* ========================================================================= */

CInfoItemTypeBase::CInfoItemTypeBase()
{
	m_byDelAverage	= 0;
	m_byTarget		= 0;				/* �g�p�Ώ� */
	m_byArea		= 0;				/* �g�p�͈� */
	m_bPutOn		= FALSE;
	m_wGrpIDMain	= 0;
	m_wGrpIDSub		= 0;
	m_dwTypeID		= 0;
	m_dwItemTypeID	= ITEMTYPEID_NONE;
	m_dwGrpID		= 0;
	m_dwIconGrpID	= 0;
	m_dwDropSoundID	= 0;
	m_dwUseEffectID	= 0;
	m_dwUseSoundID	= 0;

	m_dwWeaponInfoID	= 0;
	m_dwValue			= 0;	/* �U���� */
	m_dwMoveWait		= 0;	/* ���x */
	m_dwMoveCount		= 0;	/* �򋗗� */
	m_dwValue2			= 0;	/* ����(�ő�) */

	for (m_nElementCount = 0; s_aszName[m_nElementCount] != NULL; m_nElementCount ++) {}
}


/* ========================================================================= */
/* �֐���	:CInfoItemTypeBase::~CInfoItemTypeBase							 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/09/26														 */
/* ========================================================================= */

CInfoItemTypeBase::~CInfoItemTypeBase()
{
}


/* ========================================================================= */
/* �֐���	:CInfoItemTypeBase::GetElementNo								 */
/* ���e		:�v�f�ԍ����擾													 */
/* ���t		:2007/09/26														 */
/* ========================================================================= */

int CInfoItemTypeBase::GetElementNo(LPCSTR pszName)
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
/* �֐���	:CInfoItemTypeBase::GetDataSize									 */
/* ���e		:�f�[�^�T�C�Y���擾												 */
/* ���t		:2007/09/26														 */
/* ========================================================================= */

DWORD CInfoItemTypeBase::GetDataSize(void)
{
	DWORD dwRet;

	dwRet = 0;
	dwRet += sizeof (m_byDelAverage);		/* �g���ĂȂ��Ȃ�m�� */
	dwRet += sizeof (m_byTarget);			/* �g�p�Ώ� */
	dwRet += sizeof (m_byArea);				/* �g�p�͈� */
	dwRet += sizeof (m_bPutOn);				/* ��ɒu���邩���� */
	dwRet += sizeof (m_wGrpIDMain);			/* �摜ID���C�� */
	dwRet += sizeof (m_wGrpIDSub);			/* �摜ID�T�u */
	dwRet += sizeof (m_dwTypeID);			/* ���ID */
	dwRet += sizeof (m_dwItemTypeID);		/* �A�C�e�����ID */
	dwRet += sizeof (m_dwGrpID);			/* �n�ʂɂ��鎞�̉摜ID */
	dwRet += sizeof (m_dwIconGrpID);		/* �o�b�N�p�b�N���̉摜ID */
	dwRet += sizeof (m_dwDropSoundID);		/* �������Ƃ��̌��ʉ�ID */
	dwRet += (m_strName.GetLength () + 1);	/* �A�C�e���� */
	dwRet += sizeof (m_dwUseEffectID);		/* �g�������ɍĐ�����G�t�F�N�gID */
	dwRet += sizeof (m_dwUseSoundID);		/* �g�������ɍĐ�������ʉ�ID */
	dwRet += sizeof (m_dwWeaponInfoID);		/* ������ID */
	dwRet += sizeof (m_dwValue);			/* �U���� */
	dwRet += sizeof (m_dwMoveWait);			/* ���x */
	dwRet += sizeof (m_dwMoveCount);		/* �򋗗� */
	dwRet += sizeof (m_dwValue2);			/* ����(�ő�) */

	return dwRet;
}


/* ========================================================================= */
/* �֐���	:CInfoItemTypeBase::GetDataSizeNo								 */
/* ���e		:�w��v�f�̃f�[�^�T�C�Y���擾									 */
/* ���t		:2007/09/26														 */
/* ========================================================================= */

DWORD CInfoItemTypeBase::GetDataSizeNo(int nNo)
{
	DWORD dwRet;

	dwRet = 0;

	switch (nNo) {
	case 0:		dwRet = sizeof (m_byDelAverage);		break;	/* �g���ĂȂ��Ȃ�m�� */
	case 1:		dwRet = sizeof (m_byTarget);			break;	/* �g�p�Ώ� */
	case 2:		dwRet = sizeof (m_byArea);				break;	/* �g�p�͈� */
	case 3:		dwRet = sizeof (m_bPutOn);				break;	/* ��ɒu���邩���� */
	case 4:		dwRet = sizeof (m_dwTypeID);			break;	/* ���ID */
	case 5:		dwRet = sizeof (m_dwItemTypeID);		break;	/* �A�C�e�����ID */
	case 6:		dwRet = sizeof (m_dwGrpID);				break;	/* �n�ʂɂ��鎞�̉摜ID */
	case 7:		dwRet = sizeof (m_dwIconGrpID);			break;	/* �o�b�N�p�b�N���̉摜ID */
	case 8:		dwRet = (m_strName.GetLength () + 1);	break;	/* �A�C�e���� */
	case 9:		dwRet = sizeof (m_dwDropSoundID);		break;	/* �������Ƃ��̌��ʉ�ID */
	case 10:	dwRet = sizeof (m_wGrpIDMain);			break;	/* �摜ID���C�� */
	case 11:	dwRet = sizeof (m_wGrpIDSub);			break;	/* �摜ID�T�u */
	case 12:	dwRet = sizeof (m_dwUseEffectID);		break;	/* �g�������ɍĐ�����G�t�F�N�gID */
	case 13:	dwRet = sizeof (m_dwUseSoundID);		break;	/* �g�������ɍĐ�������ʉ�ID */
	case 14:	dwRet = sizeof (m_dwWeaponInfoID);		break;	/* ������ID */
	case 15:	dwRet = sizeof (m_dwValue);				break;	/* �U���� */
	case 16:	dwRet = sizeof (m_dwMoveWait);			break;	/* ���x */
	case 17:	dwRet = sizeof (m_dwMoveCount);			break;	/* �򋗗� */
	case 18:	dwRet = sizeof (m_dwValue2);			break;	/* ����(�ő�) */
	}

	return dwRet;
}


/* ========================================================================= */
/* �֐���	:CInfoItemTypeBase::GetName										 */
/* ���e		:�v�f�����擾													 */
/* ���t		:2007/09/26														 */
/* ========================================================================= */

LPCSTR CInfoItemTypeBase::GetName(int nNo)
{
	return s_aszName[nNo];
}


/* ========================================================================= */
/* �֐���	:CInfoItemTypeBase::GetWriteData								 */
/* ���e		:�w��v�f�̕ۑ��p�f�[�^���擾									 */
/* ���t		:2007/09/26														 */
/* ========================================================================= */

PBYTE CInfoItemTypeBase::GetWriteData(int nNo, PDWORD pdwSize)
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
	case 0:		pSrc = (PBYTE)&m_byDelAverage;		break;	/* �g���ĂȂ��Ȃ�m�� */
	case 1:		pSrc = (PBYTE)&m_byTarget;			break;	/* �g�p�Ώ� */
	case 2:		pSrc = (PBYTE)&m_byArea;			break;	/* �g�p�͈� */
	case 3:		pSrc = (PBYTE)&m_bPutOn;			break;	/* ��ɒu���邩���� */
	case 4:		pSrc = (PBYTE)&m_dwTypeID;			break;	/* ���ID */
	case 5:		pSrc = (PBYTE)&m_dwItemTypeID;		break;	/* �A�C�e�����ID */
	case 6:		pSrc = (PBYTE)&m_dwGrpID;			break;	/* �n�ʂɂ��鎞�̉摜ID */
	case 7:		pSrc = (PBYTE)&m_dwIconGrpID;		break;	/* �o�b�N�p�b�N���̉摜ID */
	case 8:		pSrc = (PBYTE)(LPCSTR)m_strName;	break;	/* �A�C�e���� */
	case 9:		pSrc = (PBYTE)&m_dwDropSoundID;		break;	/* �������Ƃ��̌��ʉ�ID */
	case 10:	pSrc = (PBYTE)&m_wGrpIDMain;		break;	/* �摜ID���C�� */
	case 11:	pSrc = (PBYTE)&m_wGrpIDSub;			break;	/* �摜ID�T�u */
	case 12:	pSrc = (PBYTE)&m_dwUseEffectID;		break;	/* �g�������ɍĐ�����G�t�F�N�gID */
	case 13:	pSrc = (PBYTE)&m_dwUseSoundID;		break;	/* �g�������ɍĐ�������ʉ�ID */
	case 14:	pSrc = (PBYTE)&m_dwWeaponInfoID;	break;	/* ������ID */
	case 15:	pSrc = (PBYTE)&m_dwValue;			break;	/* �U���� */
	case 16:	pSrc = (PBYTE)&m_dwMoveWait;		break;	/* ���x */
	case 17:	pSrc = (PBYTE)&m_dwMoveCount;		break;	/* �򋗗� */
	case 18:	pSrc = (PBYTE)&m_dwValue2;			break;	/* ����(�ő�) */
	}

	if (pSrc) {
		CopyMemory (pRet, pSrc, dwSize);
	}

Exit:
	return pRet;
}


/* ========================================================================= */
/* �֐���	:CInfoItemTypeBase::ReadElementData								 */
/* ���e		:�w��v�f�f�[�^��ǂݍ���										 */
/* ���t		:2007/09/26														 */
/* ========================================================================= */

DWORD CInfoItemTypeBase::ReadElementData(
	PBYTE pSrc,		/* [in] �f�[�^�̓ǂݍ��݌� */
	int nNo)		/* [in] �v�f�ԍ� */
{
	PBYTE pDst;
	DWORD dwSize;

	pDst	= NULL;
	dwSize	= 0;

	switch (nNo) {
	case 0:		pDst = (PBYTE)&m_byDelAverage;		dwSize = sizeof (m_byDelAverage);	break;	/* �g���ĂȂ��Ȃ�m�� */
	case 1:		pDst = (PBYTE)&m_byTarget;			dwSize = sizeof (m_byTarget);		break;	/* �g�p�Ώ� */
	case 2:		pDst = (PBYTE)&m_byArea;			dwSize = sizeof (m_byArea);			break;	/* �g�p�͈� */
	case 3:		pDst = (PBYTE)&m_bPutOn;			dwSize = sizeof (m_bPutOn);			break;	/* ��ɒu���邩���� */
	case 4:		pDst = (PBYTE)&m_dwTypeID;			dwSize = sizeof (m_dwTypeID);		break;	/* ���ID */
	case 5:		pDst = (PBYTE)&m_dwItemTypeID;		dwSize = sizeof (m_dwItemTypeID);	break;	/* �A�C�e�����ID */
	case 6:		pDst = (PBYTE)&m_dwGrpID;			dwSize = sizeof (m_dwGrpID);		break;	/* �n�ʂɂ��鎞�̉摜ID */
	case 7:		pDst = (PBYTE)&m_dwIconGrpID;		dwSize = sizeof (m_dwIconGrpID);	break;	/* �o�b�N�p�b�N���̉摜ID */
	case 8:		/* �A�C�e���� */
		m_strName = (LPCSTR)pSrc;
		dwSize = m_strName.GetLength () + 1;
		break;
	case 9:		pDst = (PBYTE)&m_dwDropSoundID;		dwSize = sizeof (m_dwDropSoundID);		break;	/* �������Ƃ��̌��ʉ�ID */
	case 10:	pDst = (PBYTE)&m_wGrpIDMain;		dwSize = sizeof (m_wGrpIDMain);			break;	/* �摜ID���C�� */
	case 11:	pDst = (PBYTE)&m_wGrpIDSub;			dwSize = sizeof (m_wGrpIDSub);			break;	/* �摜ID�T�u */
	case 12:	pDst = (PBYTE)&m_dwUseEffectID;		dwSize = sizeof (m_dwUseEffectID);		break;	/* �g�������ɍĐ�����G�t�F�N�gID */
	case 13:	pDst = (PBYTE)&m_dwUseSoundID;		dwSize = sizeof (m_dwUseSoundID);		break;	/* �g�������ɍĐ�������ʉ�ID */
	case 14:	pDst = (PBYTE)&m_dwWeaponInfoID;	dwSize = sizeof (m_dwWeaponInfoID);		break;	/* ������ID */
	case 15:	pDst = (PBYTE)&m_dwValue;			dwSize = sizeof (m_dwValue);			break;	/* �U���� */
	case 16:	pDst = (PBYTE)&m_dwMoveWait;		dwSize = sizeof (m_dwMoveWait);			break;	/* ���x */
	case 17:	pDst = (PBYTE)&m_dwMoveCount;		dwSize = sizeof (m_dwMoveCount);		break;	/* �򋗗� */
	case 18:	pDst = (PBYTE)&m_dwValue2;			dwSize = sizeof (m_dwValue2);			break;	/* ����(�ő�) */
	}

	if (pDst) {
		CopyMemory (pDst, pSrc, dwSize);
	}

	return dwSize;
}


/* ========================================================================= */
/* �֐���	:CInfoItemTypeBase::GetSendDataSize								 */
/* ���e		:���M�f�[�^�T�C�Y���擾											 */
/* ���t		:2007/09/26														 */
/* ========================================================================= */

DWORD CInfoItemTypeBase::GetSendDataSize(void)
{
	DWORD dwRet;

	dwRet = sizeof (m_dwTypeID)				+
			sizeof (m_dwItemTypeID)			+
			sizeof (m_dwGrpID)				+
			sizeof (m_dwIconGrpID)			+
			sizeof (m_dwDropSoundID)		+
			sizeof (m_byDelAverage)			+	/* �g���ĂȂ��Ȃ�m�� */
			sizeof (m_byTarget)				+	/* �g�p�Ώ� */
			sizeof (m_byArea)				+	/* �g�p�͈� */
			sizeof (m_bPutOn)				+
			sizeof (m_wGrpIDMain)			+
			sizeof (m_wGrpIDSub)			+
			sizeof (m_dwUseEffectID)		+	/* �g�������ɍĐ�����G�t�F�N�gID */
			sizeof (m_dwUseSoundID)			+	/* �g�������ɍĐ�������ʉ�ID */
			(m_strName.GetLength () + 1)	+
			sizeof (m_dwWeaponInfoID)		+	/* ������ID */
			sizeof (m_dwValue)				+	/* �U���� */
			sizeof (m_dwMoveWait)			+	/* ���x */
			sizeof (m_dwMoveCount)			+	/* �򋗗� */
			sizeof (m_dwValue2);				/* ����(�ő�) */

	return dwRet;
}


/* ========================================================================= */
/* �֐���	:CInfoItemTypeBase::GetSendData									 */
/* ���e		:���M�f�[�^���擾												 */
/* ���t		:2007/09/26														 */
/* ========================================================================= */

PBYTE CInfoItemTypeBase::GetSendData(void)
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;

	dwSize	= GetSendDataSize ();
	pData	= ZeroNew (dwSize);

	pDataTmp = pData;

	CopyMemoryRenew (pDataTmp, &m_dwItemTypeID,		sizeof (m_dwItemTypeID),	pDataTmp);
	CopyMemoryRenew (pDataTmp, &m_dwTypeID,			sizeof (m_dwTypeID),		pDataTmp);
	CopyMemoryRenew (pDataTmp, &m_dwGrpID,			sizeof (m_dwGrpID),			pDataTmp);
	CopyMemoryRenew (pDataTmp, &m_dwIconGrpID,		sizeof (m_dwIconGrpID),		pDataTmp);
	CopyMemoryRenew (pDataTmp, &m_dwDropSoundID,	sizeof (m_dwDropSoundID),	pDataTmp);
	CopyMemoryRenew (pDataTmp, &m_byDelAverage,		sizeof (m_byDelAverage),	pDataTmp);				/* �g���ĂȂ��Ȃ�m�� */
	CopyMemoryRenew (pDataTmp, &m_byTarget,			sizeof (m_byTarget),		pDataTmp);				/* �g�p�Ώ� */
	CopyMemoryRenew (pDataTmp, &m_byArea,			sizeof (m_byArea),			pDataTmp);				/* �g�p�͈� */
	CopyMemoryRenew (pDataTmp, &m_bPutOn,			sizeof (m_bPutOn),			pDataTmp);
	CopyMemoryRenew (pDataTmp, &m_wGrpIDMain,		sizeof (m_wGrpIDMain),		pDataTmp);
	CopyMemoryRenew (pDataTmp, &m_wGrpIDSub,		sizeof (m_wGrpIDSub),		pDataTmp);
	CopyMemoryRenew (pDataTmp, &m_dwUseEffectID,	sizeof (m_dwUseEffectID),	pDataTmp);				/* �g�������ɍĐ�����G�t�F�N�gID */
	CopyMemoryRenew (pDataTmp, &m_dwUseSoundID,		sizeof (m_dwUseSoundID),	pDataTmp);				/* �g�������ɍĐ�������ʉ�ID */
	CopyMemoryRenew (pDataTmp, &m_dwValue,			sizeof (m_dwValue),			pDataTmp);				/* �U���� */
	CopyMemoryRenew (pDataTmp, &m_dwMoveWait,		sizeof (m_dwMoveWait),		pDataTmp);				/* ���x */
	CopyMemoryRenew (pDataTmp, &m_dwMoveCount,		sizeof (m_dwMoveCount),		pDataTmp);				/* �򋗗� */
	strcpyRenew ((LPSTR)pDataTmp, m_strName, pDataTmp);
	CopyMemoryRenew (pDataTmp, &m_dwWeaponInfoID,		sizeof (m_dwWeaponInfoID),		pDataTmp);		/* ������ID */
	CopyMemoryRenew (pDataTmp, &m_dwValue2,				sizeof (m_dwValue2),			pDataTmp);		/* ����(�ő�) */

	return pData;
}


/* ========================================================================= */
/* �֐���	:CInfoItemTypeBase::SetSendData									 */
/* ���e		:���M�f�[�^�����荞��											 */
/* ���t		:2007/09/26														 */
/* ========================================================================= */

PBYTE CInfoItemTypeBase::SetSendData(PBYTE pSrc)
{
	PBYTE pRet, pDataTmp;

	pRet = pSrc;

	pDataTmp = pSrc;
	CopyMemoryRenew (&m_dwItemTypeID,	pDataTmp, sizeof (m_dwItemTypeID),	pDataTmp);
	CopyMemoryRenew (&m_dwTypeID,		pDataTmp, sizeof (m_dwTypeID),		pDataTmp);
	CopyMemoryRenew (&m_dwGrpID,		pDataTmp, sizeof (m_dwGrpID),		pDataTmp);
	CopyMemoryRenew (&m_dwIconGrpID,	pDataTmp, sizeof (m_dwIconGrpID),	pDataTmp);
	CopyMemoryRenew (&m_dwDropSoundID,	pDataTmp, sizeof (m_dwDropSoundID),	pDataTmp);
	CopyMemoryRenew (&m_byDelAverage,	pDataTmp, sizeof (m_byDelAverage),	pDataTmp);			/* �g���ĂȂ��Ȃ�m�� */
	CopyMemoryRenew (&m_byTarget,		pDataTmp, sizeof (m_byTarget),		pDataTmp);			/* �g�p�Ώ� */
	CopyMemoryRenew (&m_byArea,			pDataTmp, sizeof (m_byArea),		pDataTmp);			/* �g�p�͈� */
	CopyMemoryRenew (&m_bPutOn,			pDataTmp, sizeof (m_bPutOn),		pDataTmp);
	CopyMemoryRenew (&m_wGrpIDMain,		pDataTmp, sizeof (m_wGrpIDMain),	pDataTmp);
	CopyMemoryRenew (&m_wGrpIDSub,		pDataTmp, sizeof (m_wGrpIDSub),		pDataTmp);
	CopyMemoryRenew (&m_dwUseEffectID,	pDataTmp, sizeof (m_dwUseEffectID),	pDataTmp);			/* �g�������ɍĐ�����G�t�F�N�gID */
	CopyMemoryRenew (&m_dwUseSoundID,	pDataTmp, sizeof (m_dwUseSoundID),	pDataTmp);			/* �g�������ɍĐ�������ʉ�ID */
	CopyMemoryRenew (&m_dwValue,		pDataTmp, sizeof (m_dwValue),		pDataTmp);			/* �U���� */
	CopyMemoryRenew (&m_dwMoveWait,		pDataTmp, sizeof (m_dwMoveWait),	pDataTmp);			/* ���x */
	CopyMemoryRenew (&m_dwMoveCount,	pDataTmp, sizeof (m_dwMoveCount),	pDataTmp);			/* �򋗗� */
	StoreRenew (m_strName, (LPCSTR)pDataTmp, pDataTmp);
	CopyMemoryRenew (&m_dwWeaponInfoID,	pDataTmp, sizeof (m_dwWeaponInfoID),	pDataTmp);		/* ������ID */
	CopyMemoryRenew (&m_dwValue2,		pDataTmp, sizeof (m_dwValue2),			pDataTmp);		/* ����(�ő�) */

	pRet = pDataTmp;
	return pRet;
}


/* ========================================================================= */
/* �֐���	:CInfoItemTypeBase::Copy										 */
/* ���e		:�R�s�[															 */
/* ���t		:2007/09/26														 */
/* ========================================================================= */

void CInfoItemTypeBase::Copy(CInfoItemTypeBase *pSrc)
{
	if (pSrc == NULL) {
		return;
	}
	m_byDelAverage		= pSrc->m_byDelAverage;			/* �g���ĂȂ��Ȃ�m�� */
	m_byTarget			= pSrc->m_byTarget;				/* �g�p�Ώ� */
	m_byArea			= pSrc->m_byArea;				/* �g�p�͈� */
	m_bPutOn			= pSrc->m_bPutOn;
	m_wGrpIDMain		= pSrc->m_wGrpIDMain;
	m_wGrpIDSub			= pSrc->m_wGrpIDSub;
	m_dwTypeID			= pSrc->m_dwTypeID;
	m_dwItemTypeID		= pSrc->m_dwItemTypeID;
	m_dwGrpID			= pSrc->m_dwGrpID;
	m_dwIconGrpID		= pSrc->m_dwIconGrpID;
	m_dwDropSoundID		= pSrc->m_dwDropSoundID;
	m_dwUseEffectID		= pSrc->m_dwUseEffectID;		/* �g�������ɍĐ�����G�t�F�N�gID */
	m_dwUseSoundID		= pSrc->m_dwUseSoundID;			/* �g�������ɍĐ�������ʉ�ID */
	m_strName			= pSrc->m_strName;
	m_dwWeaponInfoID	= pSrc->m_dwWeaponInfoID;		/* ������ID */
	m_dwValue			= pSrc->m_dwValue;				/* �U���� */
	m_dwMoveWait		= pSrc->m_dwMoveWait;			/* ���x */
	m_dwMoveCount		= pSrc->m_dwMoveCount;			/* �򋗗� */
	m_dwValue2			= pSrc->m_dwValue2;				/* ����(�ő�) */
}


/* ========================================================================= */
/* �֐���	:CInfoItemTypeBase::SetGrpID									 */
/* ���e		:�摜ID��ݒ�													 */
/* ���t		:2007/10/08														 */
/* ========================================================================= */

void CInfoItemTypeBase::SetGrpID(WORD *pwDst1, WORD *pwDst2)
{
	if (pwDst1) {
		*pwDst1 = 0;
	}
	if (pwDst2) {
		*pwDst2 = 0;
	}

	switch (m_dwItemTypeID) {
	case ITEMTYPEID_NONE:			/* ���ʖ��� */
		break;
	case ITEMTYPEID_CLOTH:			/* �� */
		switch (m_wGrpIDMain) {
		case ITEMGRPIDMAIN_CLOTH_NONE:		/* ���ݒ� */
			if (pwDst1) {
				*pwDst1 = m_wGrpIDSub;
			}
			break;
		case ITEMGRPIDMAIN_CLOTH_SP:		/* ���ꕞ */
			if (pwDst2) {
				*pwDst2 = m_wGrpIDSub;
			}
			break;
		}
		break;
	case ITEMTYPEID_ACCE:			/* �A�N�Z�T�� */
		*pwDst1 = m_wGrpIDSub;
		break;
	case ITEMTYPEID_ARMS:			/* ������ */
	case ITEMTYPEID_SHIELD:			/* �� */
	case ITEMTYPEID_HP:				/* HP���� */
		*pwDst1 = m_wGrpIDMain;
		*pwDst2 = m_wGrpIDSub;
		break;
	}
}

/* Copyright(C)URARA-works 2007 */
