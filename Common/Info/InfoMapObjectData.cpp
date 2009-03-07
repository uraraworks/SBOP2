/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:InfoMapObjectData.cpp										 */
/* ���e			:�}�b�v�I�u�W�F�N�g�z�u�f�[�^�N���X �����t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/11/03													 */
/* ========================================================================= */

#include "stdafx.h"
#include "InfoMapObjectData.h"

/* ========================================================================= */
/* �萔��`																	 */
/* ========================================================================= */

/* �w�b�_��� */
static LPCSTR s_aszName[] = {
	"m_dwDataID",		/* �z�u�f�[�^ID */
	"m_dwObjectID",		/* �I�u�W�F�N�gID */
	"m_ptPos",			/* ���W */
	NULL
};


/* ========================================================================= */
/* �֐���	:CInfoMapObjectData::CInfoMapObjectData							 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/11/03														 */
/* ========================================================================= */

CInfoMapObjectData::CInfoMapObjectData()
{
	m_dwDataID		= 0;
	m_dwObjectID	= 0;
	ZeroMemory (&m_ptPos,  sizeof (m_ptPos));

	for (m_nElementCount = 0; s_aszName[m_nElementCount] != NULL; m_nElementCount ++) {}
}


/* ========================================================================= */
/* �֐���	:CInfoMapObjectData::~CInfoMapObjectData						 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/11/03														 */
/* ========================================================================= */

CInfoMapObjectData::~CInfoMapObjectData()
{
}


/* ========================================================================= */
/* �֐���	:CInfoMapObjectData::RenewSize									 */
/* ���e		:�T�C�Y�X�V														 */
/* ���t		:2008/11/03														 */
/* ========================================================================= */

void CInfoMapObjectData::RenewSize(int nDirection, int nSize, SIZE *pSize)
{
	switch (nDirection) {
	case 0:
		m_ptPos. y += nSize;
		break;
	case 2:
		m_ptPos. x += nSize;
		break;
	}

	/* ���W�̒��� */
	m_ptPos.x = max (0, m_ptPos.x);
	m_ptPos.x = min (m_ptPos.x, pSize->cx - 1);
	m_ptPos.y = max (0, m_ptPos.y);
	m_ptPos.y = min (m_ptPos.y, pSize->cy - 1);
}


/* ========================================================================= */
/* �֐���	:CInfoMapObjectData::GetElementNo								 */
/* ���e		:�v�f�ԍ����擾													 */
/* ���t		:2008/11/03														 */
/* ========================================================================= */

int CInfoMapObjectData::GetElementNo(LPCSTR pszName)
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
/* �֐���	:CInfoMapObjectData::GetDataSize								 */
/* ���e		:�f�[�^�T�C�Y���擾												 */
/* ���t		:2008/11/03														 */
/* ========================================================================= */

DWORD CInfoMapObjectData::GetDataSize(void)
{
	DWORD dwRet;

	dwRet = 0;
	dwRet += sizeof (m_dwDataID);		/* �z�u�f�[�^ID */
	dwRet += sizeof (m_dwObjectID);		/* �I�u�W�F�N�gID */
	dwRet += sizeof (m_ptPos);			/* ���W */

	return dwRet;
}


/* ========================================================================= */
/* �֐���	:CInfoMapObjectData::GetDataSizeNo								 */
/* ���e		:�w��v�f�̃f�[�^�T�C�Y���擾									 */
/* ���t		:2008/11/03														 */
/* ========================================================================= */

DWORD CInfoMapObjectData::GetDataSizeNo(int nNo)
{
	DWORD dwRet;

	dwRet = 0;

	switch (nNo) {
	case 0:	dwRet = sizeof (m_dwDataID);		break;	/* �z�u�f�[�^ID */
	case 1:	dwRet = sizeof (m_dwObjectID);		break;	/* �I�u�W�F�N�gID */
	case 2:	dwRet = sizeof (m_ptPos);			break;	/* ���W */
	}

	return dwRet;
}


/* ========================================================================= */
/* �֐���	:CInfoMapObjectData::GetName									 */
/* ���e		:�v�f�����擾													 */
/* ���t		:2008/11/03														 */
/* ========================================================================= */

LPCSTR CInfoMapObjectData::GetName(int nNo)
{
	return s_aszName[nNo];
}


/* ========================================================================= */
/* �֐���	:CInfoMapObjectData::GetWriteData								 */
/* ���e		:�w��v�f�̕ۑ��p�f�[�^���擾									 */
/* ���t		:2008/11/03														 */
/* ========================================================================= */

PBYTE CInfoMapObjectData::GetWriteData(int nNo, PDWORD pdwSize)
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
	case 0:	pSrc = (PBYTE)&m_dwDataID;		break;	/* �z�u�f�[�^ID */
	case 1:	pSrc = (PBYTE)&m_dwObjectID;	break;	/* �I�u�W�F�N�gID */
	case 2:	pSrc = (PBYTE)&m_ptPos;			break;	/* ���W */
	}

	if (pSrc) {
		CopyMemory (pRet, pSrc, dwSize);
	}

Exit:
	return pRet;
}


/* ========================================================================= */
/* �֐���	:CInfoMapObjectData::ReadElementData							 */
/* ���e		:�w��v�f�f�[�^��ǂݍ���										 */
/* ���t		:2008/11/03														 */
/* ========================================================================= */

DWORD CInfoMapObjectData::ReadElementData(
	PBYTE pSrc,		/* [in] �f�[�^�̓ǂݍ��݌� */
	int nNo)		/* [in] �v�f�ԍ� */
{
	PBYTE pDst;
	DWORD dwSize;

	pDst	= NULL;
	dwSize	= 0;

	switch (nNo) {
	case 0:	pDst = (PBYTE)&m_dwDataID;		dwSize = sizeof (m_dwDataID);	break;	/* �z�u�f�[�^ID */
	case 1:	pDst = (PBYTE)&m_dwObjectID;	dwSize = sizeof (m_dwObjectID);	break;	/* �I�u�W�F�N�gID */
	case 2:	pDst = (PBYTE)&m_ptPos;			dwSize = sizeof (m_ptPos);		break;	/* ���W */
	}

	if (pDst) {
		CopyMemory (pDst, pSrc, dwSize);
	}

	return dwSize;
}


/* ========================================================================= */
/* �֐���	:CInfoMapObjectData::GetSendDataSize							 */
/* ���e		:���M�f�[�^�T�C�Y���擾											 */
/* ���t		:2008/11/03														 */
/* ========================================================================= */

DWORD CInfoMapObjectData::GetSendDataSize(void)
{
	DWORD dwRet;

	dwRet = sizeof (m_dwDataID)		+	/* �z�u�f�[�^ID */
			sizeof (m_dwObjectID)	+	/* �I�u�W�F�N�gID */
			sizeof (m_ptPos);			/* ���W */

	return dwRet;
}


/* ========================================================================= */
/* �֐���	:CInfoMapObjectData::GetSendData								 */
/* ���e		:���M�f�[�^���擾												 */
/* ���t		:2008/11/03														 */
/* ========================================================================= */

PBYTE CInfoMapObjectData::GetSendData(void)
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;

	dwSize	= GetSendDataSize ();
	pData	= ZeroNew (dwSize);

	pDataTmp = pData;

	CopyMemoryRenew (pDataTmp, &m_dwDataID,		sizeof (m_dwDataID),	pDataTmp);	/* �z�u�f�[�^ID */
	CopyMemoryRenew (pDataTmp, &m_dwObjectID,	sizeof (m_dwObjectID),	pDataTmp);	/* �I�u�W�F�N�gID */
	CopyMemoryRenew (pDataTmp, &m_ptPos,		sizeof (m_ptPos),		pDataTmp);	/* ���W */

	return pData;
}


/* ========================================================================= */
/* �֐���	:CInfoMapObjectData::SetSendData								 */
/* ���e		:���M�f�[�^�����荞��											 */
/* ���t		:2008/11/03														 */
/* ========================================================================= */

PBYTE CInfoMapObjectData::SetSendData(PBYTE pSrc)
{
	PBYTE pRet, pDataTmp;

	pRet = pSrc;

	pDataTmp = pSrc;
	CopyMemoryRenew (&m_dwDataID,	pDataTmp, sizeof (m_dwDataID),		pDataTmp);	/* �z�u�f�[�^ID */
	CopyMemoryRenew (&m_dwObjectID,	pDataTmp, sizeof (m_dwObjectID),	pDataTmp);	/* �I�u�W�F�N�gID */
	CopyMemoryRenew (&m_ptPos,		pDataTmp, sizeof (m_ptPos),			pDataTmp);	/* ���W */

	pRet = pDataTmp;
	return pRet;
}


/* ========================================================================= */
/* �֐���	:CInfoMapObjectData::Copy										 */
/* ���e		:�R�s�[															 */
/* ���t		:2008/11/03														 */
/* ========================================================================= */

void CInfoMapObjectData::Copy(CInfoMapObjectData *pSrc)
{
	if (pSrc == NULL) {
		return;
	}
	m_dwDataID		= pSrc->m_dwDataID;			/* �z�u�f�[�^ID */
	m_dwObjectID	= pSrc->m_dwObjectID;		/* �I�u�W�F�N�gID */
	m_ptPos			= pSrc->m_ptPos;			/* ���W */
}

/* Copyright(C)URARA-works 2008 */
