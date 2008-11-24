/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:InfoMapEventMAPMOVE.cpp									 */
/* ���e			:�C�x���g���(�}�b�v�Ԉړ�)�N���X �����t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/07/27													 */
/* ========================================================================= */

#include "stdafx.h"
#include "InfoMapEventMAPMOVE.h"

/* ========================================================================= */
/* �萔��`																	 */
/* ========================================================================= */

/* �w�b�_��� */
static LPCSTR s_aszName[] = {
	"m_dwMapID",		/* �ړ���}�b�vID */
	"ptDst",			/* �ړ��� */
	"m_nDirection",		/* �ړ���̌��� */
	NULL
};


/* ========================================================================= */
/* �֐���	:CInfoMapEventMAPMOVE::CInfoMapEventMAPMOVE						 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/07/27														 */
/* ========================================================================= */

CInfoMapEventMAPMOVE::CInfoMapEventMAPMOVE()
{
	m_nType			= MAPEVENTTYPE_MOVE;
	m_nDirection	= -1;
	ZeroMemory (&m_ptDst, sizeof (m_ptDst));

	for (m_nElementCount = 0; s_aszName[m_nElementCount] != NULL; m_nElementCount ++) {}
	m_nElementCount += m_nElementCountBase;
}


/* ========================================================================= */
/* �֐���	:CInfoMapEventMAPMOVE::~CInfoMapEventMAPMOVE					 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/07/27														 */
/* ========================================================================= */

CInfoMapEventMAPMOVE::~CInfoMapEventMAPMOVE()
{
}


/* ========================================================================= */
/* �֐���	:CInfoMapEventMAPMOVE::GetElementNo								 */
/* ���e		:�v�f�ԍ����擾													 */
/* ���t		:2008/07/27														 */
/* ========================================================================= */

int CInfoMapEventMAPMOVE::GetElementNo(LPCSTR pszName)
{
	int i, nRet;

	nRet = CInfoMapEventBase::GetElementNo (pszName);
	if (nRet >= 0) {
		goto Exit;
	}

	for (i = 0; s_aszName[i] != NULL; i ++) {
		if (strcmp (s_aszName[i], pszName) == 0) {
			nRet = i;
			break;
		}
	}
	if (nRet >= 0) {
		nRet += m_nElementCountBase;
	}

Exit:
	return nRet;
}


/* ========================================================================= */
/* �֐���	:CInfoMapEventMAPMOVE::GetDataSize								 */
/* ���e		:�f�[�^�T�C�Y���擾												 */
/* ���t		:2008/07/27														 */
/* ========================================================================= */

DWORD CInfoMapEventMAPMOVE::GetDataSize(void)
{
	DWORD dwRet;

	dwRet = CInfoMapEventBase::GetDataSize ();
	dwRet += sizeof (m_dwMapID);		/* �ړ���}�b�vID */
	dwRet += sizeof (m_ptDst);			/* �ړ��� */
	dwRet += sizeof (m_nDirection);		/* �ړ���̌��� */

	return dwRet;
}


/* ========================================================================= */
/* �֐���	:CInfoMapEventMAPMOVE::GetDataSizeNo							 */
/* ���e		:�w��v�f�̃f�[�^�T�C�Y���擾									 */
/* ���t		:2008/07/27														 */
/* ========================================================================= */

DWORD CInfoMapEventMAPMOVE::GetDataSizeNo(int nNo)
{
	DWORD dwRet;

	if (nNo < m_nElementCountBase) {
		dwRet = CInfoMapEventBase::GetDataSizeNo (nNo);
	} else {
		switch (nNo - m_nElementCountBase) {
		case 0:	dwRet = sizeof (m_dwMapID);		break;		/* �ړ���}�b�vID */
		case 1:	dwRet = sizeof (m_ptDst);		break;		/* �ړ��� */
		case 2:	dwRet = sizeof (m_nDirection);	break;		/* �ړ���̌��� */
		}
	}

	return dwRet;
}


/* ========================================================================= */
/* �֐���	:CInfoMapEventMAPMOVE::GetName									 */
/* ���e		:�v�f�����擾													 */
/* ���t		:2008/07/27														 */
/* ========================================================================= */

LPCSTR CInfoMapEventMAPMOVE::GetName(int nNo)
{
	if (nNo < m_nElementCountBase) {
		return CInfoMapEventBase::GetName (nNo);
	}

	return s_aszName[nNo - m_nElementCountBase];
}


/* ========================================================================= */
/* �֐���	:CInfoMapEventMAPMOVE::GetWriteData								 */
/* ���e		:�w��v�f�̕ۑ��p�f�[�^���擾									 */
/* ���t		:2008/07/27														 */
/* ========================================================================= */

PBYTE CInfoMapEventMAPMOVE::GetWriteData(int nNo, PDWORD pdwSize)
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
	if (nNo < m_nElementCountBase) {
		pRet = CInfoMapEventBase::GetWriteData (nNo, pdwSize);
	} else {
		pRet = new BYTE[dwSize];

		switch (nNo - m_nElementCountBase) {
		case 0:	pSrc = (PBYTE)&m_dwMapID;		break;		/* �ړ���}�b�vID */
		case 1:	pSrc = (PBYTE)&m_ptDst;			break;		/* �ړ��� */
		case 2:	pSrc = (PBYTE)&m_nDirection;	break;		/* �ړ���̌��� */
		}

		if (pSrc) {
			CopyMemory (pRet, pSrc, dwSize);
		}
	}

Exit:
	return pRet;
}


/* ========================================================================= */
/* �֐���	:CInfoMapEventMAPMOVE::ReadElementData							 */
/* ���e		:�w��v�f�f�[�^��ǂݍ���										 */
/* ���t		:2008/07/27														 */
/* ========================================================================= */

DWORD CInfoMapEventMAPMOVE::ReadElementData(
	PBYTE pSrc,		/* [in] �f�[�^�̓ǂݍ��݌� */
	int nNo)		/* [in] �v�f�ԍ� */
{
	PBYTE pDst;
	DWORD dwSize;

	pDst	= NULL;
	dwSize	= 0;

	if (nNo < m_nElementCountBase) {
		dwSize = CInfoMapEventBase::ReadElementData (pSrc, nNo);
	} else {
		switch (nNo - m_nElementCountBase) {
		case 0: pDst = (PBYTE)&m_dwMapID;		dwSize = sizeof (m_dwMapID);	break;		/* �ړ���}�b�vID */
		case 1: pDst = (PBYTE)&m_ptDst;			dwSize = sizeof (m_ptDst);		break;		/* �ړ��� */
		case 2: pDst = (PBYTE)&m_nDirection;	dwSize = sizeof (m_nDirection);	break;		/* �ړ���̌��� */
		}

		if (pDst) {
			CopyMemory (pDst, pSrc, dwSize);
		}
	}

	return dwSize;
}


/* ========================================================================= */
/* �֐���	:CInfoMapEventMAPMOVE::GetSendDataSize							 */
/* ���e		:���M�f�[�^�T�C�Y���擾											 */
/* ���t		:2008/06/24														 */
/* ========================================================================= */

DWORD CInfoMapEventMAPMOVE::GetSendDataSize(void)
{
	DWORD dwRet;

	dwRet = CInfoMapEventBase::GetSendDataSize ();
	dwRet += sizeof (m_dwMapID);
	dwRet += sizeof (m_ptDst);
	dwRet += sizeof (m_nDirection);

	return dwRet;
}


/* ========================================================================= */
/* �֐���	:CInfoMapEventMAPMOVE::GetSendData								 */
/* ���e		:���M�f�[�^���擾												 */
/* ���t		:2008/06/24														 */
/* ========================================================================= */

PBYTE CInfoMapEventMAPMOVE::GetSendData(void)
{
	PBYTE pData, pDataTmp, pDataBase;
	DWORD dwSize, dwSizeBase;

	pDataBase  = CInfoMapEventBase::GetSendData ();
	dwSizeBase = CInfoMapEventBase::GetSendDataSize ();

	dwSize	= GetSendDataSize ();
	pData	= ZeroNew (dwSize);

	pDataTmp = pData;

	CopyMemoryRenew (pDataTmp, pDataBase, dwSizeBase, pDataTmp);

	CopyMemoryRenew (pDataTmp, &m_dwMapID,		sizeof (m_dwMapID),		pDataTmp);	/* �ړ���}�b�vID */
	CopyMemoryRenew (pDataTmp, &m_ptDst,		sizeof (m_ptDst),		pDataTmp);	/* �ړ��� */
	CopyMemoryRenew (pDataTmp, &m_nDirection,	sizeof (m_nDirection),	pDataTmp);	/* �ړ���̌��� */

	SAFE_DELETE_ARRAY (pDataBase);

	return pData;
}


/* ========================================================================= */
/* �֐���	:CInfoMapEventMAPMOVE::SetSendData								 */
/* ���e		:���M�f�[�^�����荞��											 */
/* ���t		:2008/06/24														 */
/* ========================================================================= */

PBYTE CInfoMapEventMAPMOVE::SetSendData(PBYTE pSrc)
{
	PBYTE pRet, pDataTmp;

	pRet = pSrc;

	pDataTmp = pSrc;
	pDataTmp = CInfoMapEventBase::SetSendData (pSrc);

	CopyMemoryRenew (&m_dwMapID,	pDataTmp, sizeof (m_dwMapID),		pDataTmp);	/* �ړ���}�b�vID */
	CopyMemoryRenew (&m_ptDst,		pDataTmp, sizeof (m_ptDst),			pDataTmp);	/* �ړ��� */
	CopyMemoryRenew (&m_nDirection,	pDataTmp, sizeof (m_nDirection),	pDataTmp);	/* �ړ���̌��� */

	pRet = pDataTmp;
	return pRet;
}


/* ========================================================================= */
/* �֐���	:CInfoMapEventMAPMOVE::Copy										 */
/* ���e		:�R�s�[															 */
/* ���t		:2008/06/25														 */
/* ���l		:��ʂ��Ⴄ�ꍇ�͂������Ȃ��ƂɂȂ�̂Œ���						 */
/* ========================================================================= */

void CInfoMapEventMAPMOVE::Copy(CInfoMapEventBase *pSrc)
{
	PCInfoMapEventMAPMOVE pSrcTmp = (PCInfoMapEventMAPMOVE)pSrc;

	if (pSrcTmp == NULL) {
		return;
	}
	CInfoMapEventBase::Copy (pSrc);

	m_dwMapID		= pSrcTmp->m_dwMapID;
	m_ptDst			= pSrcTmp->m_ptDst;
	m_nDirection	= pSrcTmp->m_nDirection;
}

/* Copyright(C)URARA-works 2008 */
