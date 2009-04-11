/* Copyright(C)URARA-works 2009 */
/* ========================================================================= */
/* �t�@�C����	:InfoDisable.cpp											 */
/* ���e			:���ۏ��N���X �����t�@�C��								 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2009/04/08													 */
/* ========================================================================= */

#include "stdafx.h"
#include "InfoDisable.h"

/* ========================================================================= */
/* �萔��`																	 */
/* ========================================================================= */

/* �w�b�_��� */
static LPCSTR s_aszName[] = {
	"m_dwDisableID",		/* ���ۏ��ID */
	"m_strMacAddress",		/* MAC�A�h���X */
	NULL
};


/* ========================================================================= */
/* �֐���	:CInfoDisable::CInfoDisable										 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2009/04/08														 */
/* ========================================================================= */

CInfoDisable::CInfoDisable()
{
	m_dwDisableID = 0;

	for (m_nElementCount = 0; s_aszName[m_nElementCount] != NULL; m_nElementCount ++) {}
}


/* ========================================================================= */
/* �֐���	:CInfoDisable::~CInfoDisable									 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2009/04/08														 */
/* ========================================================================= */

CInfoDisable::~CInfoDisable()
{
}


/* ========================================================================= */
/* �֐���	:CInfoDisable::GetElementNo										 */
/* ���e		:�v�f�ԍ����擾													 */
/* ���t		:2009/04/08														 */
/* ========================================================================= */

int CInfoDisable::GetElementNo(LPCSTR pszName)
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
/* �֐���	:CInfoDisable::GetDataSize										 */
/* ���e		:�f�[�^�T�C�Y���擾												 */
/* ���t		:2009/04/08														 */
/* ========================================================================= */

DWORD CInfoDisable::GetDataSize(void)
{
	DWORD dwRet;

	dwRet = 0;
	dwRet += sizeof (m_dwDisableID);				/* ���ۏ��ID */
	dwRet += (m_strMacAddress.GetLength () + 1);	/* MAC�A�h���X */

	return dwRet;
}


/* ========================================================================= */
/* �֐���	:CInfoDisable::GetDataSizeNo									 */
/* ���e		:�w��v�f�̃f�[�^�T�C�Y���擾									 */
/* ���t		:2009/04/08														 */
/* ========================================================================= */

DWORD CInfoDisable::GetDataSizeNo(int nNo)
{
	DWORD dwRet;

	dwRet = 0;

	switch (nNo) {
	case 0:		dwRet = sizeof (m_dwDisableID);				break;	/* ���ۏ��ID */
	case 1:		dwRet = (m_strMacAddress.GetLength () + 1);	break;	/* MAC�A�h���X */
	}

	return dwRet;
}


/* ========================================================================= */
/* �֐���	:CInfoDisable::GetName											 */
/* ���e		:�v�f�����擾													 */
/* ���t		:2009/04/08														 */
/* ========================================================================= */

LPCSTR CInfoDisable::GetName(int nNo)
{
	return s_aszName[nNo];
}


/* ========================================================================= */
/* �֐���	:CInfoDisable::GetWriteData										 */
/* ���e		:�w��v�f�̕ۑ��p�f�[�^���擾									 */
/* ���t		:2009/04/08														 */
/* ========================================================================= */

PBYTE CInfoDisable::GetWriteData(int nNo, PDWORD pdwSize)
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
	case 0:		pSrc = (PBYTE)&m_dwDisableID;			break;	/* ���ۏ��ID */
	case 1:		pSrc = (PBYTE)(LPCSTR)m_strMacAddress;	break;	/* MAC�A�h���X */
	}

	if (pSrc) {
		CopyMemory (pRet, pSrc, dwSize);
	}

Exit:
	return pRet;
}


/* ========================================================================= */
/* �֐���	:CInfoDisable::ReadElementData									 */
/* ���e		:�w��v�f�f�[�^��ǂݍ���										 */
/* ���t		:2009/04/08														 */
/* ========================================================================= */

DWORD CInfoDisable::ReadElementData(
	PBYTE pSrc,		/* [in] �f�[�^�̓ǂݍ��݌� */
	int nNo)		/* [in] �v�f�ԍ� */
{
	PBYTE pDst;
	DWORD dwSize;

	pDst	= NULL;
	dwSize	= 0;

	switch (nNo) {
	case 0:		pDst = (PBYTE)&m_dwDisableID;	dwSize = sizeof (m_dwDisableID);	break;	/* ���ۏ��ID */
	case 1:		/* MAC�A�h���X */
		m_strMacAddress = (LPCSTR)pSrc;
		dwSize = m_strMacAddress.GetLength () + 1;
		break;
	}

	if (pDst) {
		CopyMemory (pDst, pSrc, dwSize);
	}

	return dwSize;
}


/* ========================================================================= */
/* �֐���	:CInfoDisable::GetSendDataSize									 */
/* ���e		:���M�f�[�^�T�C�Y���擾											 */
/* ���t		:2009/04/08														 */
/* ========================================================================= */

DWORD CInfoDisable::GetSendDataSize(void)
{
	DWORD dwRet;

	dwRet = sizeof (m_dwDisableID)	+
			(m_strMacAddress.GetLength () + 1);

	return dwRet;
}


/* ========================================================================= */
/* �֐���	:CInfoDisable::GetSendData										 */
/* ���e		:���M�f�[�^���擾												 */
/* ���t		:2009/04/08														 */
/* ========================================================================= */

PBYTE CInfoDisable::GetSendData(void)
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;

	dwSize	= GetSendDataSize ();
	pData	= ZeroNew (dwSize);

	pDataTmp = pData;

	CopyMemoryRenew (pDataTmp, &m_dwDisableID, sizeof (m_dwDisableID), pDataTmp);
	strcpyRenew ((LPSTR)pDataTmp, m_strMacAddress, pDataTmp);

	return pData;
}


/* ========================================================================= */
/* �֐���	:CInfoDisable::SetSendData										 */
/* ���e		:���M�f�[�^�����荞��											 */
/* ���t		:2009/04/08														 */
/* ========================================================================= */

PBYTE CInfoDisable::SetSendData(PBYTE pSrc)
{
	PBYTE pRet, pDataTmp;

	pRet = pSrc;

	pDataTmp = pSrc;
	CopyMemoryRenew (&m_dwDisableID, pDataTmp, sizeof (m_dwDisableID), pDataTmp);
	StoreRenew (m_strMacAddress, (LPCSTR)pDataTmp, pDataTmp);

	pRet = pDataTmp;
	return pRet;
}


/* ========================================================================= */
/* �֐���	:CInfoDisable::Copy												 */
/* ���e		:�R�s�[															 */
/* ���t		:2009/04/08														 */
/* ========================================================================= */

void CInfoDisable::Copy(CInfoDisable *pSrc)
{
	m_dwDisableID		= pSrc->m_dwDisableID;
	m_strMacAddress		= pSrc->m_strMacAddress;
}

/* Copyright(C)URARA-works 2009 */
