/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:InfoMapEventINITSTATUS.cpp									 */
/* ���e			:�C�x���g���(�X�e�[�^�X������)�N���X �����t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/10/05													 */
/* ========================================================================= */

#include "stdafx.h"
#include "InfoMapEventINITSTATUS.h"

/* ========================================================================= */
/* �萔��`																	 */
/* ========================================================================= */

/* �w�b�_��� */
static LPCSTR s_aszName[] = {
	"m_dwEffectID",		/* �G�t�F�N�gID */
	NULL
};


/* ========================================================================= */
/* �֐���	:CInfoMapEventINITSTATUS::CInfoMapEventINITSTATUS				 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/10/05														 */
/* ========================================================================= */

CInfoMapEventINITSTATUS::CInfoMapEventINITSTATUS()
{
	m_nType = MAPEVENTTYPE_INITSTATUS;

	m_dwEffectID = 0;

	for (m_nElementCount = 0; s_aszName[m_nElementCount] != NULL; m_nElementCount ++) {}
	m_nElementCount += m_nElementCountBase;
}


/* ========================================================================= */
/* �֐���	:CInfoMapEventINITSTATUS::~CInfoMapEventINITSTATUS				 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/10/05														 */
/* ========================================================================= */

CInfoMapEventINITSTATUS::~CInfoMapEventINITSTATUS()
{
}


/* ========================================================================= */
/* �֐���	:CInfoMapEventINITSTATUS::GetElementNo							 */
/* ���e		:�v�f�ԍ����擾													 */
/* ���t		:2008/10/05														 */
/* ========================================================================= */

int CInfoMapEventINITSTATUS::GetElementNo(LPCSTR pszName)
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
/* �֐���	:CInfoMapEventINITSTATUS::GetDataSize							 */
/* ���e		:�f�[�^�T�C�Y���擾												 */
/* ���t		:2008/10/05														 */
/* ========================================================================= */

DWORD CInfoMapEventINITSTATUS::GetDataSize(void)
{
	DWORD dwRet;

	dwRet = CInfoMapEventBase::GetDataSize ();
	dwRet += sizeof (m_dwEffectID);

	return dwRet;
}


/* ========================================================================= */
/* �֐���	:CInfoMapEventINITSTATUS::GetDataSizeNo							 */
/* ���e		:�w��v�f�̃f�[�^�T�C�Y���擾									 */
/* ���t		:2008/10/05														 */
/* ========================================================================= */

DWORD CInfoMapEventINITSTATUS::GetDataSizeNo(int nNo)
{
	DWORD dwRet;

	if (nNo < m_nElementCountBase) {
		dwRet = CInfoMapEventBase::GetDataSizeNo (nNo);
	} else {
		switch (nNo - m_nElementCountBase) {
		case 0:	dwRet = sizeof (m_dwEffectID);		break;
		}
	}

	return dwRet;
}


/* ========================================================================= */
/* �֐���	:CInfoMapEventINITSTATUS::GetName								 */
/* ���e		:�v�f�����擾													 */
/* ���t		:2008/10/05														 */
/* ========================================================================= */

LPCSTR CInfoMapEventINITSTATUS::GetName(int nNo)
{
	if (nNo < m_nElementCountBase) {
		return CInfoMapEventBase::GetName (nNo);
	}

	return s_aszName[nNo - m_nElementCountBase];
}


/* ========================================================================= */
/* �֐���	:CInfoMapEventINITSTATUS::GetWriteData							 */
/* ���e		:�w��v�f�̕ۑ��p�f�[�^���擾									 */
/* ���t		:2008/10/05														 */
/* ========================================================================= */

PBYTE CInfoMapEventINITSTATUS::GetWriteData(int nNo, PDWORD pdwSize)
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
		case 0:	pSrc = (PBYTE)&m_dwEffectID;		break;
		}

		if (pSrc) {
			CopyMemory (pRet, pSrc, dwSize);
		}
	}

Exit:
	return pRet;
}


/* ========================================================================= */
/* �֐���	:CInfoMapEventINITSTATUS::ReadElementData						 */
/* ���e		:�w��v�f�f�[�^��ǂݍ���										 */
/* ���t		:2008/10/05														 */
/* ========================================================================= */

DWORD CInfoMapEventINITSTATUS::ReadElementData(
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
		case 0: pDst = (PBYTE)&m_dwEffectID;	dwSize = sizeof (m_dwEffectID);	break;
		}

		if (pDst) {
			CopyMemory (pDst, pSrc, dwSize);
		}
	}

	return dwSize;
}


/* ========================================================================= */
/* �֐���	:CInfoMapEventINITSTATUS::GetSendDataSize						 */
/* ���e		:���M�f�[�^�T�C�Y���擾											 */
/* ���t		:2008/10/05														 */
/* ========================================================================= */

DWORD CInfoMapEventINITSTATUS::GetSendDataSize(void)
{
	DWORD dwRet;

	dwRet = CInfoMapEventBase::GetSendDataSize ();
	dwRet += sizeof (m_dwEffectID);

	return dwRet;
}


/* ========================================================================= */
/* �֐���	:CInfoMapEventINITSTATUS::GetSendData							 */
/* ���e		:���M�f�[�^���擾												 */
/* ���t		:2008/10/05														 */
/* ========================================================================= */

PBYTE CInfoMapEventINITSTATUS::GetSendData(void)
{
	PBYTE pData, pDataTmp, pDataBase;
	DWORD dwSize, dwSizeBase;

	pDataBase  = CInfoMapEventBase::GetSendData ();
	dwSizeBase = CInfoMapEventBase::GetSendDataSize ();

	dwSize	= GetSendDataSize ();
	pData	= ZeroNew (dwSize);

	pDataTmp = pData;

	CopyMemoryRenew (pDataTmp, pDataBase, dwSizeBase, pDataTmp);

	CopyMemoryRenew (pDataTmp, &m_dwEffectID, sizeof (m_dwEffectID), pDataTmp);		/* �G�t�F�N�gID */

	SAFE_DELETE_ARRAY (pDataBase);

	return pData;
}


/* ========================================================================= */
/* �֐���	:CInfoMapEventINITSTATUS::SetSendData							 */
/* ���e		:���M�f�[�^�����荞��											 */
/* ���t		:2008/10/05														 */
/* ========================================================================= */

PBYTE CInfoMapEventINITSTATUS::SetSendData(PBYTE pSrc)
{
	PBYTE pRet, pDataTmp;

	pRet = pSrc;

	pDataTmp = pSrc;
	pDataTmp = CInfoMapEventBase::SetSendData (pSrc);

	CopyMemoryRenew (&m_dwEffectID, pDataTmp, sizeof (m_dwEffectID), pDataTmp);		/* �G�t�F�N�gID */

	pRet = pDataTmp;
	return pRet;
}


/* ========================================================================= */
/* �֐���	:CInfoMapEventINITSTATUS::Copy									 */
/* ���e		:�R�s�[															 */
/* ���t		:2008/10/05														 */
/* ���l		:��ʂ��Ⴄ�ꍇ�͂������Ȃ��ƂɂȂ�̂Œ���						 */
/* ========================================================================= */

void CInfoMapEventINITSTATUS::Copy(CInfoMapEventBase *pSrc)
{
	PCInfoMapEventINITSTATUS pSrcTmp = (PCInfoMapEventINITSTATUS)pSrc;

	if (pSrcTmp == NULL) {
		return;
	}
	CInfoMapEventBase::Copy (pSrc);

	m_dwEffectID = pSrcTmp->m_dwEffectID;
}

/* Copyright(C)URARA-works 2008 */
