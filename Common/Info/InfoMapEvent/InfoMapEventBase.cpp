/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:InfoMapEventBase.cpp										 */
/* ���e			:�C�x���g�����N���X �����t�@�C��						 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/07/05													 */
/* ========================================================================= */

#include "stdafx.h"
#include "InfoMapEventBase.h"

/* ========================================================================= */
/* �萔��`																	 */
/* ========================================================================= */

/* �w�b�_��� */
static LPCSTR s_aszName[] = {
	"m_nType",			/* �C�x���g��� */
	"m_dwMapEventID",	/* �}�b�v�C�x���gID */
	"m_ptPos",			/* ���W */
	NULL
};


/* ========================================================================= */
/* �֐���	:CInfoMapEventBase::CInfoMapEventBase							 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/07/05														 */
/* ========================================================================= */

CInfoMapEventBase::CInfoMapEventBase()
{
	m_dwMapEventID	= 0;
	m_nType			= MAPEVENTTYPE_NONE;
	ZeroMemory (&m_ptPos, sizeof (m_ptPos));

	for (m_nElementCountBase = 0; s_aszName[m_nElementCountBase] != NULL; m_nElementCountBase ++) {}
	m_nElementCount = m_nElementCountBase;
}


/* ========================================================================= */
/* �֐���	:CInfoMapEventBase::~CInfoMapEventBase							 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/07/05														 */
/* ========================================================================= */

CInfoMapEventBase::~CInfoMapEventBase()
{
}


/* ========================================================================= */
/* �֐���	:CInfoMapEventBase::RenewSize									 */
/* ���e		:�T�C�Y�X�V														 */
/* ���t		:2008/09/14														 */
/* ========================================================================= */

void CInfoMapEventBase::RenewSize(int nDirection, int nSize)
{
	switch (nDirection) {
	case 0:
		m_ptPos.y += nSize;
		break;
	case 2:
		m_ptPos.x += nSize;
		break;
	}
}


/* ========================================================================= */
/* �֐���	:CInfoMapEventBase::GetElementNo								 */
/* ���e		:�v�f�ԍ����擾													 */
/* ���t		:2005/07/05														 */
/* ========================================================================= */

int CInfoMapEventBase::GetElementNo(LPCSTR pszName)
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
/* �֐���	:CInfoMapEventBase::GetDataSize									 */
/* ���e		:�f�[�^�T�C�Y���擾												 */
/* ���t		:2005/07/05														 */
/* ========================================================================= */

DWORD CInfoMapEventBase::GetDataSize(void)
{
	DWORD dwRet;

	dwRet = 0;
	dwRet += sizeof (m_nType);
	dwRet += sizeof (m_dwMapEventID);
	dwRet += sizeof (m_ptPos);

	return dwRet;
}


/* ========================================================================= */
/* �֐���	:CInfoMapEventBase::GetDataSizeNo								 */
/* ���e		:�w��v�f�̃f�[�^�T�C�Y���擾									 */
/* ���t		:2005/07/05														 */
/* ========================================================================= */

DWORD CInfoMapEventBase::GetDataSizeNo(int nNo)
{
	DWORD dwRet;

	dwRet = 0;

	switch (nNo) {
	case 0:	dwRet = sizeof (m_nType);			break;
	case 1:	dwRet = sizeof (m_dwMapEventID);	break;
	case 2:	dwRet = sizeof (m_ptPos);			break;
	}

	return dwRet;
}


/* ========================================================================= */
/* �֐���	:CInfoMapEventBase::GetName										 */
/* ���e		:�v�f�����擾													 */
/* ���t		:2005/07/05														 */
/* ========================================================================= */

LPCSTR CInfoMapEventBase::GetName(int nNo)
{
	return s_aszName[nNo];
}


/* ========================================================================= */
/* �֐���	:CInfoMapEventBase::GetWriteData								 */
/* ���e		:�w��v�f�̕ۑ��p�f�[�^���擾									 */
/* ���t		:2005/07/05														 */
/* ========================================================================= */

PBYTE CInfoMapEventBase::GetWriteData(int nNo, PDWORD pdwSize)
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
	case 0:	pSrc = (PBYTE)&m_nType;			break;
	case 1:	pSrc = (PBYTE)&m_dwMapEventID;	break;
	case 2:	pSrc = (PBYTE)&m_ptPos;			break;
	}

	if (pSrc) {
		CopyMemory (pRet, pSrc, dwSize);
	}

Exit:
	return pRet;
}


/* ========================================================================= */
/* �֐���	:CInfoMapEventBase::ReadElementData								 */
/* ���e		:�w��v�f�f�[�^��ǂݍ���										 */
/* ���t		:2005/07/05														 */
/* ========================================================================= */

DWORD CInfoMapEventBase::ReadElementData(
	PBYTE pSrc,		/* [in] �f�[�^�̓ǂݍ��݌� */
	int nNo)		/* [in] �v�f�ԍ� */
{
	PBYTE pDst;
	DWORD dwSize;

	pDst	= NULL;
	dwSize	= 0;

	switch (nNo) {
	case 0: pDst = (PBYTE)&m_nType;			dwSize = sizeof (m_nType);			break;
	case 1: pDst = (PBYTE)&m_dwMapEventID;	dwSize = sizeof (m_dwMapEventID);	break;
	case 2:	pDst = (PBYTE)&m_ptPos;			dwSize = sizeof (m_ptPos);			break;
	}

	if (pDst) {
		CopyMemory (pDst, pSrc, dwSize);
	}

	return dwSize;
}


/* ========================================================================= */
/* �֐���	:CInfoMapEventBase::GetSendDataSize								 */
/* ���e		:���M�f�[�^�T�C�Y���擾											 */
/* ���t		:2008/06/24														 */
/* ========================================================================= */

DWORD CInfoMapEventBase::GetSendDataSize(void)
{
	DWORD dwRet;

	dwRet = sizeof (m_nType)		+
			sizeof (m_dwMapEventID)	+
			sizeof (m_ptPos);

	return dwRet;
}


/* ========================================================================= */
/* �֐���	:CInfoMapEventBase::GetSendData									 */
/* ���e		:���M�f�[�^���擾												 */
/* ���t		:2008/06/24														 */
/* ========================================================================= */

PBYTE CInfoMapEventBase::GetSendData(void)
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;

	dwSize	= GetSendDataSize ();
	pData	= ZeroNew (dwSize);

	pDataTmp = pData;

	CopyMemoryRenew (pDataTmp, &m_nType,		sizeof (m_nType), 			pDataTmp);
	CopyMemoryRenew (pDataTmp, &m_dwMapEventID,	sizeof (m_dwMapEventID), 	pDataTmp);
	CopyMemoryRenew (pDataTmp, &m_ptPos,		sizeof (m_ptPos),			pDataTmp);

	return pData;
}


/* ========================================================================= */
/* �֐���	:CInfoMapEventBase::SetSendData									 */
/* ���e		:���M�f�[�^�����荞��											 */
/* ���t		:2008/06/24														 */
/* ========================================================================= */

PBYTE CInfoMapEventBase::SetSendData(PBYTE pSrc)
{
	PBYTE pRet, pDataTmp;

	pRet = pSrc;

	pDataTmp = pSrc;
	CopyMemoryRenew (&m_nType,			pDataTmp, sizeof (m_nType), 		pDataTmp);
	CopyMemoryRenew (&m_dwMapEventID,	pDataTmp, sizeof (m_dwMapEventID), 	pDataTmp);
	CopyMemoryRenew (&m_ptPos,			pDataTmp, sizeof (m_ptPos),			pDataTmp);

	pRet = pDataTmp;
	return pRet;
}


/* ========================================================================= */
/* �֐���	:CInfoMapEventBase::Copy										 */
/* ���e		:�R�s�[															 */
/* ���t		:2008/06/25														 */
/* ========================================================================= */

void CInfoMapEventBase::Copy(CInfoMapEventBase *pSrc)
{
	if (pSrc == NULL) {
		return;
	}
	m_nType			= pSrc->m_nType;
	m_dwMapEventID	= pSrc->m_dwMapEventID;
	m_ptPos			= pSrc->m_ptPos;
}

/* Copyright(C)URARA-works 2007 */
