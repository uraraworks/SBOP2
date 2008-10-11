/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:InfoItem.cpp												 */
/* ���e			:�A�C�e�����N���X �����t�@�C��							 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/05/05													 */
/* ========================================================================= */

#include "stdafx.h"
#include "InfoItem.h"

/* ========================================================================= */
/* �萔��`																	 */
/* ========================================================================= */

/* �w�b�_��� */
static LPCSTR s_aszName[] = {
	"m_bPutOn",			/* ��ɒu���邩���� */
	"m_dwItemID",		/* �A�C�e��ID */
	"m_dwItemTypeID",	/* �A�C�e�����ID */
	"m_dwGrpID",		/* �摜ID */
	"m_dwIconGrpID",	/* �o�b�N�p�b�N���̉摜ID */
	"m_dwMapID",		/* �����Ă���}�b�vID */
	"m_dwCharID",		/* ���L�҃L����ID */
	"m_nPosZ",			/* �����Ă��鍂��(0���n��) */
	"m_ptPos",			/* �����Ă�����W */
	"m_ptBackPack",		/* �o�b�N�p�b�N���̈ʒu */
	"m_strName",		/* �A�C�e���� */
	"m_dwDropSoundID",	/* �������Ƃ��̌��ʉ�ID */
	NULL
};


/* ========================================================================= */
/* �֐���	:CInfoItem::CInfoItem											 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/05/05														 */
/* ========================================================================= */

CInfoItem::CInfoItem()
{
	m_bPutOn		= FALSE;
	m_dwItemID		= 0;
	m_dwItemTypeID	= 0;
	m_dwGrpID		= 0;
	m_dwIconGrpID	= 0;
	m_dwMapID		= 0;
	m_dwCharID		= 0;
	m_dwDropSoundID	= 0;
	m_nPosZ			= 0;
	m_ptPos.x = m_ptPos.y = 0;
	m_ptBackPack.x = m_ptBackPack.y = 0;

	for (m_nElementCount = 0; s_aszName[m_nElementCount] != NULL; m_nElementCount ++) {}
}


/* ========================================================================= */
/* �֐���	:CInfoItem::~CInfoItem											 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/05/05														 */
/* ========================================================================= */

CInfoItem::~CInfoItem()
{
}


/* ========================================================================= */
/* �֐���	:CInfoItem::GetElementNo										 */
/* ���e		:�v�f�ԍ����擾													 */
/* ���t		:2007/04/30														 */
/* ========================================================================= */

int CInfoItem::GetElementNo(LPCSTR pszName)
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
/* �֐���	:CInfoItem::GetDataSize											 */
/* ���e		:�f�[�^�T�C�Y���擾												 */
/* ���t		:2007/08/19														 */
/* ========================================================================= */

DWORD CInfoItem::GetDataSize(void)
{
	DWORD dwRet;

	dwRet = 0;
	dwRet += sizeof (m_bPutOn);				/* ��ɒu���邩���� */
	dwRet += sizeof (m_dwItemID);			/* �A�C�e��ID */
	dwRet += sizeof (m_dwItemTypeID);		/* �A�C�e�����ID */
	dwRet += sizeof (m_dwGrpID);			/* �摜ID */
	dwRet += sizeof (m_dwIconGrpID);		/* �o�b�N�p�b�N���̉摜ID */
	dwRet += sizeof (m_dwMapID);			/* �����Ă���}�b�vID */
	dwRet += sizeof (m_dwCharID);			/* ���L�҃L����ID */
	dwRet += sizeof (m_dwDropSoundID);		/* �������Ƃ��̌��ʉ�ID */
	dwRet += sizeof (m_nPosZ);				/* �����Ă��鍂��(0���n��) */
	dwRet += sizeof (m_ptPos);				/* �����Ă�����W */
	dwRet += sizeof (m_ptBackPack);			/* �o�b�N�p�b�N���̈ʒu */
	dwRet += (m_strName.GetLength () + 1);	/* �A�C�e���� */

	return dwRet;
}


/* ========================================================================= */
/* �֐���	:CInfoItem::GetDataSizeNo										 */
/* ���e		:�w��v�f�̃f�[�^�T�C�Y���擾									 */
/* ���t		:2007/08/19														 */
/* ========================================================================= */

DWORD CInfoItem::GetDataSizeNo(int nNo)
{
	DWORD dwRet;

	dwRet = 0;

	switch (nNo) {
	case 0:		dwRet = sizeof (m_bPutOn);				break;	/* ��ɒu���邩���� */
	case 1:		dwRet = sizeof (m_dwItemID);			break;	/* �A�C�e��ID */
	case 2:		dwRet = sizeof (m_dwItemTypeID);		break;	/* �A�C�e�����ID */
	case 3:		dwRet = sizeof (m_dwGrpID);				break;	/* �摜ID */
	case 4:		dwRet = sizeof (m_dwIconGrpID);			break;	/* �o�b�N�p�b�N���̉摜ID */
	case 5:		dwRet = sizeof (m_dwMapID);				break;	/* �����Ă���}�b�vID */
	case 6:		dwRet = sizeof (m_dwCharID);			break;	/* ���L�҃L����ID */
	case 7:		dwRet = sizeof (m_nPosZ);				break;	/* �����Ă��鍂��(0���n��) */
	case 8:		dwRet = sizeof (m_ptPos);				break;	/* �����Ă�����W */
	case 9:		dwRet = sizeof (m_ptBackPack);			break;	/* �o�b�N�p�b�N���̈ʒu */
	case 10:	dwRet = (m_strName.GetLength () + 1);	break;	/* �A�C�e���� */
	case 11:	dwRet = sizeof (m_dwDropSoundID);		break;	/* �������Ƃ��̌��ʉ�ID */
	}

	return dwRet;
}


/* ========================================================================= */
/* �֐���	:CInfoItem::GetName												 */
/* ���e		:�v�f�����擾													 */
/* ���t		:2007/04/30														 */
/* ========================================================================= */

LPCSTR CInfoItem::GetName(int nNo)
{
	return s_aszName[nNo];
}


/* ========================================================================= */
/* �֐���	:CInfoItem::GetWriteData										 */
/* ���e		:�w��v�f�̕ۑ��p�f�[�^���擾									 */
/* ���t		:2007/08/19														 */
/* ========================================================================= */

PBYTE CInfoItem::GetWriteData(int nNo, PDWORD pdwSize)
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
	case 0:		pSrc = (PBYTE)&m_bPutOn;			break;	/* ��ɒu���邩���� */
	case 1:		pSrc = (PBYTE)&m_dwItemID;			break;	/* �A�C�e��ID */
	case 2:		pSrc = (PBYTE)&m_dwItemTypeID;		break;	/* �A�C�e�����ID */
	case 3:		pSrc = (PBYTE)&m_dwGrpID;			break;	/* �摜ID */
	case 4:		pSrc = (PBYTE)&m_dwIconGrpID;		break;	/* �o�b�N�p�b�N���̉摜ID */
	case 5:		pSrc = (PBYTE)&m_dwMapID;			break;	/* �����Ă���}�b�vID */
	case 6:		pSrc = (PBYTE)&m_dwCharID;			break;	/* ���L�҃L����ID */
	case 7:		pSrc = (PBYTE)&m_nPosZ;				break;	/* �����Ă��鍂��(0���n��) */
	case 8:		pSrc = (PBYTE)&m_ptPos;				break;	/* �����Ă�����W */
	case 9:		pSrc = (PBYTE)&m_ptBackPack;		break;	/* �o�b�N�p�b�N���̈ʒu */
	case 10:	pSrc = (PBYTE)(LPCSTR)m_strName;	break;	/* �A�C�e���� */
	case 11:	pSrc = (PBYTE)&m_dwDropSoundID;		break;	/* �������Ƃ��̌��ʉ�ID */
	}

	if (pSrc) {
		CopyMemory (pRet, pSrc, dwSize);
	}

Exit:
	return pRet;
}


/* ========================================================================= */
/* �֐���	:CInfoItem::ReadElementData										 */
/* ���e		:�w��v�f�f�[�^��ǂݍ���										 */
/* ���t		:2007/08/19														 */
/* ========================================================================= */

DWORD CInfoItem::ReadElementData(
	PBYTE pSrc,		/* [in] �f�[�^�̓ǂݍ��݌� */
	int nNo)		/* [in] �v�f�ԍ� */
{
	PBYTE pDst;
	DWORD dwSize;

	pDst	= NULL;
	dwSize	= 0;

	switch (nNo) {
	case 0:		pDst = (PBYTE)&m_bPutOn;			dwSize = sizeof (m_bPutOn);			break;	/* ��ɒu���邩���� */
	case 1:		pDst = (PBYTE)&m_dwItemID;			dwSize = sizeof (m_dwItemID);		break;	/* �A�C�e��ID */
	case 2:		pDst = (PBYTE)&m_dwItemTypeID;		dwSize = sizeof (m_dwItemTypeID);	break;	/* �A�C�e�����ID */
	case 3:		pDst = (PBYTE)&m_dwGrpID;			dwSize = sizeof (m_dwGrpID);		break;	/* �摜ID */
	case 4:		pDst = (PBYTE)&m_dwIconGrpID;		dwSize = sizeof (m_dwIconGrpID);	break;	/* �o�b�N�p�b�N���̉摜ID */
	case 5:		pDst = (PBYTE)&m_dwMapID;			dwSize = sizeof (m_dwMapID);		break;	/* �����Ă���}�b�vID */
	case 6:		pDst = (PBYTE)&m_dwCharID;			dwSize = sizeof (m_dwCharID);		break;	/* ���L�҃L����ID */
	case 7:		pDst = (PBYTE)&m_nPosZ;				dwSize = sizeof (m_nPosZ);			break;	/* �����Ă��鍂��(0���n��) */
	case 8:		pDst = (PBYTE)&m_ptPos;				dwSize = sizeof (m_ptPos);			break;	/* �����Ă�����W */
	case 9:		pDst = (PBYTE)&m_ptBackPack;		dwSize = sizeof (m_ptBackPack);		break;	/* �o�b�N�p�b�N���̈ʒu */
	case 10:	/* �A�C�e���� */
		m_strName = (LPCSTR)pSrc;
		dwSize = m_strName.GetLength () + 1;
		break;
	case 11:	pDst = (PBYTE)&m_dwDropSoundID;		dwSize = sizeof (m_dwDropSoundID);	break;	/* �������Ƃ��̌��ʉ�ID */
	}

	if (pDst) {
		CopyMemory (pDst, pSrc, dwSize);
	}

	return dwSize;
}


/* ========================================================================= */
/* �֐���	:CInfoItem::GetSendDataSize										 */
/* ���e		:���M�f�[�^�T�C�Y���擾											 */
/* ���t		:2007/05/05														 */
/* ========================================================================= */

DWORD CInfoItem::GetSendDataSize(void)
{
	DWORD dwRet;

	dwRet = sizeof (m_dwItemID)			+
			sizeof (m_dwItemTypeID)		+
			sizeof (m_dwGrpID)			+
			sizeof (m_dwIconGrpID)		+
			sizeof (m_dwMapID)			+
			sizeof (m_dwCharID)			+
			sizeof (m_dwDropSoundID)	+
			sizeof (m_bPutOn)			+
			sizeof (m_nPosZ)			+
			sizeof (m_ptPos)			+
			sizeof (m_ptBackPack)		+
			(m_strName.GetLength () + 1);

	return dwRet;
}


/* ========================================================================= */
/* �֐���	:CInfoItem::GetSendData											 */
/* ���e		:���M�f�[�^���擾												 */
/* ���t		:2007/05/05														 */
/* ========================================================================= */

PBYTE CInfoItem::GetSendData(void)
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;

	dwSize	= GetSendDataSize ();
	pData	= ZeroNew (dwSize);

	pDataTmp = pData;

	CopyMemoryRenew (pDataTmp, &m_dwItemID,			sizeof (m_dwItemID),		pDataTmp);
	CopyMemoryRenew (pDataTmp, &m_dwItemTypeID,		sizeof (m_dwItemTypeID),	pDataTmp);
	CopyMemoryRenew (pDataTmp, &m_dwGrpID,			sizeof (m_dwGrpID),			pDataTmp);
	CopyMemoryRenew (pDataTmp, &m_dwIconGrpID,		sizeof (m_dwIconGrpID),		pDataTmp);
	CopyMemoryRenew (pDataTmp, &m_dwMapID,			sizeof (m_dwMapID),			pDataTmp);
	CopyMemoryRenew (pDataTmp, &m_dwCharID,			sizeof (m_dwCharID),		pDataTmp);
	CopyMemoryRenew (pDataTmp, &m_dwDropSoundID,	sizeof (m_dwDropSoundID),	pDataTmp);
	CopyMemoryRenew (pDataTmp, &m_bPutOn,			sizeof (m_bPutOn),			pDataTmp);
	CopyMemoryRenew (pDataTmp, &m_nPosZ,			sizeof (m_nPosZ),			pDataTmp);
	CopyMemoryRenew (pDataTmp, &m_ptPos,			sizeof (m_ptPos),			pDataTmp);
	CopyMemoryRenew (pDataTmp, &m_ptBackPack,		sizeof (m_ptBackPack),		pDataTmp);
	strcpyRenew ((LPSTR)pDataTmp, m_strName, pDataTmp);

	return pData;
}


/* ========================================================================= */
/* �֐���	:CInfoItem::SetSendData											 */
/* ���e		:���M�f�[�^�����荞��											 */
/* ���t		:2007/05/05														 */
/* ========================================================================= */

PBYTE CInfoItem::SetSendData(PBYTE pSrc)
{
	PBYTE pRet, pDataTmp;

	pRet = pSrc;

	pDataTmp = pSrc;
	CopyMemoryRenew (&m_dwItemID,		pDataTmp, sizeof (m_dwItemID),		pDataTmp);
	CopyMemoryRenew (&m_dwItemTypeID,	pDataTmp, sizeof (m_dwItemTypeID),	pDataTmp);
	CopyMemoryRenew (&m_dwGrpID,		pDataTmp, sizeof (m_dwGrpID),		pDataTmp);
	CopyMemoryRenew (&m_dwIconGrpID,	pDataTmp, sizeof (m_dwIconGrpID),	pDataTmp);
	CopyMemoryRenew (&m_dwMapID,		pDataTmp, sizeof (m_dwMapID),		pDataTmp);
	CopyMemoryRenew (&m_dwCharID,		pDataTmp, sizeof (m_dwCharID),		pDataTmp);
	CopyMemoryRenew (&m_dwDropSoundID,	pDataTmp, sizeof (m_dwDropSoundID),	pDataTmp);
	CopyMemoryRenew (&m_bPutOn,			pDataTmp, sizeof (m_bPutOn),		pDataTmp);
	CopyMemoryRenew (&m_nPosZ,			pDataTmp, sizeof (m_nPosZ),			pDataTmp);
	CopyMemoryRenew (&m_ptPos,			pDataTmp, sizeof (m_ptPos),			pDataTmp);
	CopyMemoryRenew (&m_ptBackPack,		pDataTmp, sizeof (m_ptBackPack),	pDataTmp);
	StoreRenew (m_strName, (LPCSTR)pDataTmp, pDataTmp);

	pRet = pDataTmp;
	return pRet;
}


/* ========================================================================= */
/* �֐���	:CInfoItem::Copy												 */
/* ���e		:�R�s�[															 */
/* ���t		:2007/05/05														 */
/* ========================================================================= */

void CInfoItem::Copy(CInfoItem *pSrc)
{
	m_bPutOn		= pSrc->m_bPutOn;
	m_dwItemID		= pSrc->m_dwItemID;
	m_dwItemTypeID	= pSrc->m_dwItemTypeID;
	m_dwGrpID		= pSrc->m_dwGrpID;
	m_dwIconGrpID	= pSrc->m_dwIconGrpID;
	m_dwMapID		= pSrc->m_dwMapID;
	m_dwCharID		= pSrc->m_dwCharID;
	m_dwDropSoundID	= pSrc->m_dwDropSoundID;
	m_nPosZ			= pSrc->m_nPosZ;
	m_ptPos			= pSrc->m_ptPos;
	m_ptBackPack	= pSrc->m_ptBackPack;
	m_strName		= pSrc->m_strName;
}

/* Copyright(C)URARA-works 2007 */
