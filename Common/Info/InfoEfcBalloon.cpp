/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:InfoEfcBalloon.cpp											 */
/* ���e			:���o�����N���X �����t�@�C��								 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/12/24													 */
/* ========================================================================= */

#include "stdafx.h"
#include "InfoEfcBalloon.h"

/* ========================================================================= */
/* �萔��`																	 */
/* ========================================================================= */

/* �w�b�_��� */
static LPCSTR s_aszName[] = {
	"m_byWait",				/* �҂�����(�~�P�O�~���b) */
	"m_bLoop",				/* ���[�v�Đ� */
	"m_dwEfcBalloonID",		/* ���o��ID */
	"m_dwListID",			/* ���o�����ID */
	"m_dwAnimeID",			/* �R�}�ԍ� */
	"m_dwSoundID",			/* ���ʉ�ID */
	"m_dwGrpID",			/* �摜ID*/
	"m_strName",			/* ���o���� */
	NULL
};


/* ========================================================================= */
/* �֐���	:CInfoEfcBalloon::CInfoEfcBalloon								 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/12/24														 */
/* ========================================================================= */

CInfoEfcBalloon::CInfoEfcBalloon()
{
	m_byWait			= 0;
	m_bLoop				= 0;
	m_dwEfcBalloonID	= 0;
	m_dwListID			= 0;
	m_dwAnimeID			= 0;
	m_dwSoundID			= 0;
	m_dwGrpID			= 0;

	for (m_nElementCount = 0; s_aszName[m_nElementCount] != NULL; m_nElementCount ++) {}
}


/* ========================================================================= */
/* �֐���	:CInfoEfcBalloon::~CInfoEfcBalloon								 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/12/24														 */
/* ========================================================================= */

CInfoEfcBalloon::~CInfoEfcBalloon()
{
}


/* ========================================================================= */
/* �֐���	:CInfoEfcBalloon::GetElementNo									 */
/* ���e		:�v�f�ԍ����擾													 */
/* ���t		:2005/05/01														 */
/* ========================================================================= */

int CInfoEfcBalloon::GetElementNo(LPCSTR pszName)
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
/* �֐���	:CInfoEfcBalloon::GetDataSize									 */
/* ���e		:�f�[�^�T�C�Y���擾												 */
/* ���t		:2007/12/24														 */
/* ========================================================================= */

DWORD CInfoEfcBalloon::GetDataSize(void)
{
	DWORD dwRet;

	dwRet = sizeof (m_byWait)				+	/* �҂�����(�~�P�O�~���b) */
			sizeof (m_bLoop)				+	/* ���[�v�Đ� */
			sizeof (m_dwEfcBalloonID)		+	/* ���o��ID */
			sizeof (m_dwListID)				+	/* ���o�����ID */
			sizeof (m_dwAnimeID)			+	/* �R�}�ԍ� */
			sizeof (m_dwSoundID)			+	/* ���ʉ�ID */
			(m_strName.GetLength () + 1)	+	/* �摜ID */
			sizeof (m_dwGrpID); 			 	/* ���o���� */

	return dwRet;
}


/* ========================================================================= */
/* �֐���	:CInfoEfcBalloon::GetDataSizeNo									 */
/* ���e		:�w��v�f�̃f�[�^�T�C�Y���擾									 */
/* ���t		:2007/12/24														 */
/* ========================================================================= */

DWORD CInfoEfcBalloon::GetDataSizeNo(int nNo)
{
	DWORD dwRet;

	dwRet = 0;

	switch (nNo) {
	case 0:		dwRet = sizeof (m_byWait);				break;	/* �҂�����(�~�P�O�~���b) */
	case 1:		dwRet = sizeof (m_bLoop);				break;	/* ���[�v�Đ� */
	case 2:		dwRet = sizeof (m_dwEfcBalloonID);		break;	/* ���o��ID */
	case 3:		dwRet = sizeof (m_dwListID);			break;	/* ���o�����ID */
	case 4:		dwRet = sizeof (m_dwAnimeID);			break;	/* �R�}�ԍ� */
	case 5:		dwRet = sizeof (m_dwSoundID);			break;	/* ���ʉ�ID */
	case 6:		dwRet = sizeof (m_dwGrpID);				break;	/* �摜ID */
	case 7:		dwRet = (m_strName.GetLength () + 1);	break;	/* ���o���� */
	}

	return dwRet;
}


/* ========================================================================= */
/* �֐���	:CInfoEfcBalloon::GetName										 */
/* ���e		:�v�f�����擾													 */
/* ���t		:2007/12/24														 */
/* ========================================================================= */

LPCSTR CInfoEfcBalloon::GetName(int nNo)
{
	return s_aszName[nNo];
}


/* ========================================================================= */
/* �֐���	:CInfoEfcBalloon::GetWriteData									 */
/* ���e		:�w��v�f�̕ۑ��p�f�[�^���擾									 */
/* ���t		:2007/12/24														 */
/* ========================================================================= */

PBYTE CInfoEfcBalloon::GetWriteData(int nNo, PDWORD pdwSize)
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
	case 0:		pSrc = &m_byWait;					break;		/* �҂�����(�~�P�O�~���b) */
	case 1:		pSrc = (PBYTE)&m_bLoop;				break;		/* ���[�v�Đ� */
	case 2:		pSrc = (PBYTE)&m_dwEfcBalloonID;	break;		/* ���o��ID */
	case 3:		pSrc = (PBYTE)&m_dwListID;			break;		/* ���o�����ID */
	case 4:		pSrc = (PBYTE)&m_dwAnimeID;			break;		/* �R�}�ԍ� */
	case 5:		pSrc = (PBYTE)&m_dwSoundID;			break;		/* ���ʉ�ID */
	case 6:		pSrc = (PBYTE)&m_dwGrpID;			break;		/* �摜ID */
	case 7:		pSrc = (PBYTE)(LPCSTR)m_strName;	break;		/* ���o���� */
	}

	if (pSrc) {
		CopyMemory (pRet, pSrc, dwSize);
	}

Exit:
	return pRet;
}


/* ========================================================================= */
/* �֐���	:CInfoEfcBalloon::ReadElementData								 */
/* ���e		:�w��v�f�f�[�^��ǂݍ���										 */
/* ���t		:2007/12/24														 */
/* ========================================================================= */

DWORD CInfoEfcBalloon::ReadElementData(
	PBYTE pSrc,		/* [in] �f�[�^�̓ǂݍ��݌� */
	int nNo)		/* [in] �v�f�ԍ� */
{
	PBYTE pDst;
	DWORD dwSize;

	pDst	= NULL;
	dwSize	= 0;

	switch (nNo) {
	case 0:		pDst = &m_byWait;					dwSize = sizeof (m_byWait);			break;	/* �҂�����(�~�P�O�~���b) */
	case 1:		pDst = (PBYTE)&m_bLoop;				dwSize = sizeof (m_bLoop);			break;	/* ���[�v�Đ� */
	case 2:		pDst = (PBYTE)&m_dwEfcBalloonID;	dwSize = sizeof (m_dwEfcBalloonID);	break;	/* ���o��ID */
	case 3:		pDst = (PBYTE)&m_dwListID;			dwSize = sizeof (m_dwListID);		break;	/* ���o�����ID */
	case 4:		pDst = (PBYTE)&m_dwAnimeID;			dwSize = sizeof (m_dwAnimeID);		break;	/* �R�}�ԍ� */
	case 5:		pDst = (PBYTE)&m_dwSoundID;			dwSize = sizeof (m_dwSoundID);		break;	/* ���ʉ�ID */
	case 6:		pDst = (PBYTE)&m_dwGrpID;			dwSize = sizeof (m_dwGrpID);		break;	/* �摜ID */
	case 7: 																				  	/* ���o���� */
		m_strName = (LPCSTR)pSrc;
		dwSize = m_strName.GetLength () + 1;
		break;
	}

	if (pDst) {
		CopyMemory (pDst, pSrc, dwSize);
	}

	return dwSize;
}


/* ========================================================================= */
/* �֐���	:CInfoEfcBalloon::GetSendDataSize								 */
/* ���e		:���M�f�[�^�T�C�Y���擾											 */
/* ���t		:2007/12/24														 */
/* ========================================================================= */

DWORD CInfoEfcBalloon::GetSendDataSize(void)
{
	return GetDataSize ();
}


/* ========================================================================= */
/* �֐���	:CInfoEfcBalloon::GetSendData									 */
/* ���e		:���M�f�[�^���擾												 */
/* ���t		:2007/12/24														 */
/* ========================================================================= */

PBYTE CInfoEfcBalloon::GetSendData(void)
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;

	dwSize	= GetSendDataSize ();
	pData	= ZeroNew (dwSize);

	pDataTmp = pData;

	CopyMemoryRenew (pDataTmp, &m_dwListID,			sizeof (m_dwListID),		pDataTmp);	/* ���o�����ID */
	CopyMemoryRenew (pDataTmp, &m_byWait,			sizeof (m_byWait),			pDataTmp);	/* �҂�����(�~�P�O�~���b) */
	CopyMemoryRenew (pDataTmp, &m_bLoop,			sizeof (m_bLoop),			pDataTmp);	/* ���[�v�Đ� */
	CopyMemoryRenew (pDataTmp, &m_dwEfcBalloonID,	sizeof (m_dwEfcBalloonID),	pDataTmp);	/* ���o��ID */
	CopyMemoryRenew (pDataTmp, &m_dwAnimeID,		sizeof (m_dwAnimeID),		pDataTmp);	/* �R�}�ԍ� */
	CopyMemoryRenew (pDataTmp, &m_dwSoundID,		sizeof (m_dwSoundID),		pDataTmp);	/* ���ʉ�ID */
	CopyMemoryRenew (pDataTmp, &m_dwGrpID,			sizeof (m_dwGrpID),			pDataTmp);	/* �摜ID */
	strcpyRenew ((LPSTR)pDataTmp, m_strName, pDataTmp); 								  	/* ���o���� */

	return pData;
}


/* ========================================================================= */
/* �֐���	:CInfoEfcBalloon::SetSendData									 */
/* ���e		:���M�f�[�^�����荞��											 */
/* ���t		:2007/12/24														 */
/* ========================================================================= */

PBYTE CInfoEfcBalloon::SetSendData(PBYTE pSrc)
{
	PBYTE pDataTmp;

	pDataTmp = pSrc;

	CopyMemoryRenew (&m_dwListID,		pDataTmp, sizeof (m_dwListID),			pDataTmp);	/* ���o�����ID */
	CopyMemoryRenew (&m_byWait,			pDataTmp, sizeof (m_byWait),			pDataTmp);	/* �҂�����(�~�P�O�~���b) */
	CopyMemoryRenew (&m_bLoop,			pDataTmp, sizeof (m_bLoop),				pDataTmp);	/* ���[�v�Đ� */
	CopyMemoryRenew (&m_dwEfcBalloonID,	pDataTmp, sizeof (m_dwEfcBalloonID),	pDataTmp);	/* ���o��ID */
	CopyMemoryRenew (&m_dwAnimeID,		pDataTmp, sizeof (m_dwAnimeID),			pDataTmp);	/* �R�}�ԍ� */
	CopyMemoryRenew (&m_dwSoundID,		pDataTmp, sizeof (m_dwSoundID),			pDataTmp);	/* ���ʉ�ID */
	CopyMemoryRenew (&m_dwGrpID,		pDataTmp, sizeof (m_dwGrpID),			pDataTmp);	/* �摜ID */
	StoreRenew (m_strName, (LPCSTR)pDataTmp, pDataTmp); 								  	/* ���o���� */

	return pDataTmp;
}


/* ========================================================================= */
/* �֐���	:CInfoEfcBalloon::Copy											 */
/* ���e		:�R�s�[															 */
/* ���t		:2007/12/24														 */
/* ========================================================================= */

void CInfoEfcBalloon::Copy(CInfoEfcBalloon *pSrc)
{
	m_byWait			= pSrc->m_byWait;			/* �҂�����(�~�P�O�~���b) */
	m_bLoop				= pSrc->m_bLoop;			/* ���[�v�Đ� */
	m_dwEfcBalloonID	= pSrc->m_dwEfcBalloonID;	/* ���o��ID */
	m_dwListID			= pSrc->m_dwListID;			/* ���o�����ID */
	m_dwAnimeID			= pSrc->m_dwAnimeID;		/* �R�}�ԍ� */
	m_dwSoundID			= pSrc->m_dwSoundID;		/* ���ʉ�ID */
	m_dwGrpID			= pSrc->m_dwGrpID;			/* �摜ID */
	m_strName			= pSrc->m_strName;			/* ���o���� */
}

/* Copyright(C)URARA-works 2007 */
