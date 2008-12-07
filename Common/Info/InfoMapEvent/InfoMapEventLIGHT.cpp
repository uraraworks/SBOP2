/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:InfoMapEventLIGHT.cpp										 */
/* ���e			:�C�x���g���(����)�N���X �����t�@�C��						 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/12/07													 */
/* ========================================================================= */

#include "stdafx.h"
#include "InfoMapEventLIGHT.h"

/* ========================================================================= */
/* �萔��`																	 */
/* ========================================================================= */

/* �w�b�_��� */
static LPCSTR s_aszName[] = {
	"m_bLightOn",		/* ��������� */
	"m_dwTime",			/* ��������*/
	NULL
};


/* ========================================================================= */
/* �֐���	:CInfoMapEventLIGHT::CInfoMapEventLIGHT							 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/12/07														 */
/* ========================================================================= */

CInfoMapEventLIGHT::CInfoMapEventLIGHT()
{
	m_bLightOn	= FALSE;
	m_dwTime	= 0;

	for (m_nElementCount = 0; s_aszName[m_nElementCount] != NULL; m_nElementCount ++) {}
	m_nElementCount += m_nElementCountBase;
}


/* ========================================================================= */
/* �֐���	:CInfoMapEventLIGHT::~CInfoMapEventLIGHT						 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/12/07														 */
/* ========================================================================= */

CInfoMapEventLIGHT::~CInfoMapEventLIGHT()
{
}


/* ========================================================================= */
/* �֐���	:CInfoMapEventLIGHT::GetElementNo								 */
/* ���e		:�v�f�ԍ����擾													 */
/* ���t		:2008/12/07														 */
/* ========================================================================= */

int CInfoMapEventLIGHT::GetElementNo(LPCSTR pszName)
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
/* �֐���	:CInfoMapEventLIGHT::GetDataSize								 */
/* ���e		:�f�[�^�T�C�Y���擾												 */
/* ���t		:2008/12/07														 */
/* ========================================================================= */

DWORD CInfoMapEventLIGHT::GetDataSize(void)
{
	DWORD dwRet;

	dwRet = CInfoMapEventBase::GetDataSize ();
	dwRet += sizeof (m_bLightOn);	/* ��������� */
	dwRet += sizeof (m_dwTime);		/* �������� */

	return dwRet;
}


/* ========================================================================= */
/* �֐���	:CInfoMapEventLIGHT::GetDataSizeNo								 */
/* ���e		:�w��v�f�̃f�[�^�T�C�Y���擾									 */
/* ���t		:2008/12/07														 */
/* ========================================================================= */

DWORD CInfoMapEventLIGHT::GetDataSizeNo(int nNo)
{
	DWORD dwRet;

	if (nNo < m_nElementCountBase) {
		dwRet = CInfoMapEventBase::GetDataSizeNo (nNo);
	} else {
		switch (nNo - m_nElementCountBase) {
		case 0:	dwRet = sizeof (m_bLightOn);	break;	/* ��������� */
		case 1:	dwRet = sizeof (m_dwTime);		break;	/* �������� */
		}
	}

	return dwRet;
}


/* ========================================================================= */
/* �֐���	:CInfoMapEventLIGHT::GetName									 */
/* ���e		:�v�f�����擾													 */
/* ���t		:2008/12/07														 */
/* ========================================================================= */

LPCSTR CInfoMapEventLIGHT::GetName(int nNo)
{
	if (nNo < m_nElementCountBase) {
		return CInfoMapEventBase::GetName (nNo);
	}

	return s_aszName[nNo - m_nElementCountBase];
}


/* ========================================================================= */
/* �֐���	:CInfoMapEventLIGHT::GetWriteData								 */
/* ���e		:�w��v�f�̕ۑ��p�f�[�^���擾									 */
/* ���t		:2008/12/07														 */
/* ========================================================================= */

PBYTE CInfoMapEventLIGHT::GetWriteData(int nNo, PDWORD pdwSize)
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
		case 0:	pSrc = (PBYTE)&m_bLightOn;		break;	/* ��������� */
		case 1:	pSrc = (PBYTE)&m_dwTime;		break;	/* �������� */
		}

		if (pSrc) {
			CopyMemory (pRet, pSrc, dwSize);
		}
	}

Exit:
	return pRet;
}


/* ========================================================================= */
/* �֐���	:CInfoMapEventLIGHT::ReadElementData							 */
/* ���e		:�w��v�f�f�[�^��ǂݍ���										 */
/* ���t		:2008/12/07														 */
/* ========================================================================= */

DWORD CInfoMapEventLIGHT::ReadElementData(
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
		case 0:	pDst = (PBYTE)&m_bLightOn;	dwSize = sizeof (m_bLightOn);	break;	/* ��������� */
		case 1:	pDst = (PBYTE)&m_dwTime;	dwSize = sizeof (m_dwTime);		break;	/* �������� */
		}

		if (pDst) {
			CopyMemory (pDst, pSrc, dwSize);
		}
	}

	return dwSize;
}


/* ========================================================================= */
/* �֐���	:CInfoMapEventLIGHT::GetSendDataSize							 */
/* ���e		:���M�f�[�^�T�C�Y���擾											 */
/* ���t		:2008/12/07														 */
/* ========================================================================= */

DWORD CInfoMapEventLIGHT::GetSendDataSize(void)
{
	DWORD dwRet;

	dwRet = CInfoMapEventBase::GetSendDataSize ();
	dwRet += sizeof (m_bLightOn);
	dwRet += sizeof (m_dwTime);

	return dwRet;
}


/* ========================================================================= */
/* �֐���	:CInfoMapEventLIGHT::GetSendData								 */
/* ���e		:���M�f�[�^���擾												 */
/* ���t		:2008/12/07														 */
/* ========================================================================= */

PBYTE CInfoMapEventLIGHT::GetSendData(void)
{
	PBYTE pData, pDataTmp, pDataBase;
	DWORD dwSize, dwSizeBase;

	pDataBase  = CInfoMapEventBase::GetSendData ();
	dwSizeBase = CInfoMapEventBase::GetSendDataSize ();

	dwSize	= GetSendDataSize ();
	pData	= ZeroNew (dwSize);

	pDataTmp = pData;

	CopyMemoryRenew (pDataTmp, pDataBase, dwSizeBase, pDataTmp);

	CopyMemoryRenew (pDataTmp, &m_bLightOn,	sizeof (m_bLightOn), pDataTmp);		/* ��������� */
	CopyMemoryRenew (pDataTmp, &m_dwTime,	sizeof (m_dwTime), pDataTmp);		/* �������� */

	SAFE_DELETE_ARRAY (pDataBase);

	return pData;
}


/* ========================================================================= */
/* �֐���	:CInfoMapEventLIGHT::SetSendData								 */
/* ���e		:���M�f�[�^�����荞��											 */
/* ���t		:2008/12/07														 */
/* ========================================================================= */

PBYTE CInfoMapEventLIGHT::SetSendData(PBYTE pSrc)
{
	PBYTE pRet, pDataTmp;

	pRet = pSrc;

	pDataTmp = pSrc;
	pDataTmp = CInfoMapEventBase::SetSendData (pSrc);

	CopyMemoryRenew (&m_bLightOn,	pDataTmp, sizeof (m_bLightOn),	pDataTmp);		/* ��������� */
	CopyMemoryRenew (&m_dwTime,		pDataTmp, sizeof (m_dwTime),	pDataTmp);		/* �������� */

	pRet = pDataTmp;
	return pRet;
}


/* ========================================================================= */
/* �֐���	:CInfoMapEventLIGHT::Copy										 */
/* ���e		:�R�s�[															 */
/* ���t		:2008/12/07														 */
/* ���l		:��ʂ��Ⴄ�ꍇ�͂������Ȃ��ƂɂȂ�̂Œ���						 */
/* ========================================================================= */

void CInfoMapEventLIGHT::Copy(CInfoMapEventBase *pSrc)
{
	PCInfoMapEventLIGHT pSrcTmp = (PCInfoMapEventLIGHT)pSrc;

	if (pSrcTmp == NULL) {
		return;
	}
	CInfoMapEventBase::Copy (pSrc);

	m_bLightOn	= pSrcTmp->m_bLightOn;	/* ��������� */
	m_dwTime	= pSrcTmp->m_dwTime;	/* �������� */
}

/* Copyright(C)URARA-works 2008 */
