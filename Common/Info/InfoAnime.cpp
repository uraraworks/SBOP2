/* Copyright(C)URARA-works 2005 */
/* ========================================================================= */
/* �t�@�C����	:InfoAnime.cpp												 */
/* ���e			:�A�j���[�V�������N���X �����t�@�C��						 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2005/04/29													 */
/* ========================================================================= */

#include "stdafx.h"
#include "InfoAnime.h"

/* ========================================================================= */
/* �萔��`																	 */
/* ========================================================================= */

/* �w�b�_��� */
static LPCSTR s_aszName[] = {
	"byWait",			/* �҂�����(�~�P�O�~���b) */
	"byLevel",			/* �����x */
	"wGrpIDBase",		/* �O���t�B�b�NID */
	"wGrpIDPile",		/* �d�ˍ��킹���̔w�iID */
	NULL
};


/* ========================================================================= */
/* �֐���	:CInfoAnime::CInfoAnime											 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2005/04/29														 */
/* ========================================================================= */

CInfoAnime::CInfoAnime()
{
	m_byWait	= 0;
	m_byLevel	= 0;
	m_wGrpIDBase	= 0;
	m_wGrpIDPile	= 0;

	for (m_nElementCount = 0; s_aszName[m_nElementCount] != NULL; m_nElementCount ++) {}
}


/* ========================================================================= */
/* �֐���	:CInfoAnime::~CInfoAnime										 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2005/04/29														 */
/* ========================================================================= */

CInfoAnime::~CInfoAnime()
{
}


/* ========================================================================= */
/* �֐���	:CInfoMapParts::GetElementCount									 */
/* ���e		:�v�f�����擾													 */
/* ���t		:2005/04/29														 */
/* ========================================================================= */

int CInfoAnime::GetElementCount(void)
{
	return m_nElementCount;
}


/* ========================================================================= */
/* �֐���	:CInfoAnime::GetElementNo										 */
/* ���e		:�v�f�ԍ����擾													 */
/* ���t		:2005/05/01														 */
/* ========================================================================= */

int CInfoAnime::GetElementNo(LPCSTR pszName)
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
/* �֐���	:CInfoAnime::GetDataSize										 */
/* ���e		:�f�[�^�T�C�Y���擾												 */
/* ���t		:2005/04/29														 */
/* ========================================================================= */

DWORD CInfoAnime::GetDataSize(void)
{
	DWORD dwRet;

	dwRet = sizeof (m_byWait)		+
			sizeof (m_byLevel)		+
			sizeof (m_wGrpIDBase)	+
			sizeof (m_wGrpIDPile);

	return dwRet;
}


/* ========================================================================= */
/* �֐���	:CInfoAnime::GetDataSizeNo										 */
/* ���e		:�w��v�f�̃f�[�^�T�C�Y���擾									 */
/* ���t		:2005/04/29														 */
/* ========================================================================= */

DWORD CInfoAnime::GetDataSizeNo(int nNo)
{
	DWORD dwRet;

	dwRet = 0;

	switch (nNo) {
	case 0:	dwRet = sizeof (m_byWait);		break;
	case 1:	dwRet = sizeof (m_byLevel);		break;
	case 2:	dwRet = sizeof (m_wGrpIDBase);	break;
	case 3:	dwRet = sizeof (m_wGrpIDPile);	break;
	}

	return dwRet;
}


/* ========================================================================= */
/* �֐���	:CInfoAnime::GetName											 */
/* ���e		:�v�f�����擾													 */
/* ���t		:2005/04/29														 */
/* ========================================================================= */

LPCSTR CInfoAnime::GetName(int nNo)
{
	return s_aszName[nNo];
}


/* ========================================================================= */
/* �֐���	:CInfoAnime::GetWriteData										 */
/* ���e		:�w��v�f�̕ۑ��p�f�[�^���擾									 */
/* ���t		:2005/04/30														 */
/* ========================================================================= */

PBYTE CInfoAnime::GetWriteData(int nNo, PDWORD pdwSize)
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
	case 0:	pSrc = &m_byWait;				break;
	case 1:	pSrc = &m_byLevel;				break;
	case 2:	pSrc = (PBYTE)&m_wGrpIDBase;	break;
	case 3:	pSrc = (PBYTE)&m_wGrpIDPile;	break;
	}

	if (pSrc) {
		CopyMemory (pRet, pSrc, dwSize);
	}

Exit:
	return pRet;
}


/* ========================================================================= */
/* �֐���	:CInfoAnime::ReadElementData									 */
/* ���e		:�w��v�f�f�[�^��ǂݍ���										 */
/* ���t		:2005/05/01														 */
/* ========================================================================= */

DWORD CInfoAnime::ReadElementData(
	PBYTE pSrc,		/* [in] �f�[�^�̓ǂݍ��݌� */
	int nNo)		/* [in] �v�f�ԍ� */
{
	PBYTE pDst;
	DWORD dwSize;

	pDst	= NULL;
	dwSize	= 0;

	switch (nNo) {
	case 0: pDst = &m_byWait;				dwSize = sizeof (BYTE);		break;
	case 1:	pDst = &m_byLevel;				dwSize = sizeof (BYTE);		break;
	case 2:	pDst = (PBYTE)&m_wGrpIDBase;	dwSize = sizeof (WORD);		break;
	case 3:	pDst = (PBYTE)&m_wGrpIDPile;	dwSize = sizeof (WORD);		break;
	}

	if (pDst) {
		CopyMemory (pDst, pSrc, dwSize);
	}

	return dwSize;
}


/* ========================================================================= */
/* �֐���	:CInfoAnime::GetSendDataSize									 */
/* ���e		:���M�f�[�^�T�C�Y���擾											 */
/* ���t		:2005/10/21														 */
/* ========================================================================= */

DWORD CInfoAnime::GetSendDataSize(void)
{
	DWORD dwRet;

	dwRet = sizeof (m_byWait)		+
			sizeof (m_byLevel)		+
			sizeof (m_wGrpIDBase)	+
			sizeof (m_wGrpIDPile);

	return dwRet;
}


/* ========================================================================= */
/* �֐���	:CInfoAnime::GetSendData										 */
/* ���e		:���M�f�[�^���擾												 */
/* ���t		:2005/10/21														 */
/* ========================================================================= */

PBYTE CInfoAnime::GetSendData(void)
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;

	dwSize	= GetSendDataSize ();
	pData	= ZeroNew (dwSize);

	pDataTmp = pData;

	CopyMemoryRenew (pDataTmp, &m_byWait,		sizeof (m_byWait),		pDataTmp);
	CopyMemoryRenew (pDataTmp, &m_byLevel,		sizeof (m_byLevel),		pDataTmp);
	CopyMemoryRenew (pDataTmp, &m_wGrpIDBase,	sizeof (m_wGrpIDBase),	pDataTmp);
	CopyMemoryRenew (pDataTmp, &m_wGrpIDPile,	sizeof (m_wGrpIDPile),	pDataTmp);

	return pData;
}


/* ========================================================================= */
/* �֐���	:CInfoAnime::SetSendData										 */
/* ���e		:���M�f�[�^�����荞��											 */
/* ���t		:2005/10/18														 */
/* ========================================================================= */

void CInfoAnime::SetSendData(PBYTE pSrc)
{
	PBYTE pDataTmp;

	pDataTmp = pSrc;

	CopyMemoryRenew (&m_byWait,		pDataTmp, sizeof (m_byWait),		pDataTmp);
	CopyMemoryRenew (&m_byLevel,	pDataTmp, sizeof (m_byLevel),		pDataTmp);
	CopyMemoryRenew (&m_wGrpIDBase,	pDataTmp, sizeof (m_wGrpIDBase),	pDataTmp);
	CopyMemoryRenew (&m_wGrpIDPile,	pDataTmp, sizeof (m_wGrpIDPile),	pDataTmp);
}

/* Copyright(C)URARA-works 2005 */
