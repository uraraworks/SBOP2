/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:InfoMapEventMOVE.cpp										 */
/* ���e			:�C�x���g���(�}�b�v���ړ�)�N���X �����t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/07/05													 */
/* ========================================================================= */

#include "stdafx.h"
#include "InfoMapEventMOVE.h"

/* ========================================================================= */
/* �萔��`																	 */
/* ========================================================================= */

/* �w�b�_��� */
static LPCSTR s_aszName[] = {
	"ptDst",			/* �ړ��� */
	NULL
};


/* ========================================================================= */
/* �֐���	:CInfoMapEventMOVE::CInfoMapEventMOVE							 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/07/05														 */
/* ========================================================================= */

CInfoMapEventMOVE::CInfoMapEventMOVE()
{
	m_nType = MAPEVENTTYPE_MOVE;
	ZeroMemory (&m_ptDst, sizeof (m_ptDst));

	for (m_nElementCount = 0; s_aszName[m_nElementCount] != NULL; m_nElementCount ++) {}
	m_nElementCount += m_nElementCountBase;
}


/* ========================================================================= */
/* �֐���	:CInfoMapEventMOVE::~CInfoMapEventMOVE							 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/07/05														 */
/* ========================================================================= */

CInfoMapEventMOVE::~CInfoMapEventMOVE()
{
}


/* ========================================================================= */
/* �֐���	:CInfoMapEventMOVE::RenewSize									 */
/* ���e		:�T�C�Y�X�V														 */
/* ���t		:2008/09/14														 */
/* ========================================================================= */

void CInfoMapEventMOVE::RenewSize(int nDirection, int nSize)
{
	CInfoMapEventBase::RenewSize (nDirection, nSize);

	switch (nDirection) {
	case 0:
		m_ptDst.y += nSize;
		break;
	case 2:
		m_ptDst.x += nSize;
		break;
	}
}


/* ========================================================================= */
/* �֐���	:CInfoMapEventMOVE::GetElementNo								 */
/* ���e		:�v�f�ԍ����擾													 */
/* ���t		:2007/07/05														 */
/* ========================================================================= */

int CInfoMapEventMOVE::GetElementNo(LPCSTR pszName)
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
/* �֐���	:CInfoMapEventMOVE::GetDataSize									 */
/* ���e		:�f�[�^�T�C�Y���擾												 */
/* ���t		:2007/07/05														 */
/* ========================================================================= */

DWORD CInfoMapEventMOVE::GetDataSize(void)
{
	DWORD dwRet;

	dwRet = CInfoMapEventBase::GetDataSize ();
	dwRet += sizeof (m_ptDst);

	return dwRet;
}


/* ========================================================================= */
/* �֐���	:CInfoMapEventMOVE::GetDataSizeNo								 */
/* ���e		:�w��v�f�̃f�[�^�T�C�Y���擾									 */
/* ���t		:2007/07/05														 */
/* ========================================================================= */

DWORD CInfoMapEventMOVE::GetDataSizeNo(int nNo)
{
	DWORD dwRet;

	if (nNo < m_nElementCountBase) {
		dwRet = CInfoMapEventBase::GetDataSizeNo (nNo);
	} else {
		switch (nNo - m_nElementCountBase) {
		case 0:	dwRet = sizeof (m_ptDst);		break;
		}
	}

	return dwRet;
}


/* ========================================================================= */
/* �֐���	:CInfoMapEventMOVE::GetName										 */
/* ���e		:�v�f�����擾													 */
/* ���t		:2007/07/05														 */
/* ========================================================================= */

LPCSTR CInfoMapEventMOVE::GetName(int nNo)
{
	if (nNo < m_nElementCountBase) {
		return CInfoMapEventBase::GetName (nNo);
	}

	return s_aszName[nNo - m_nElementCountBase];
}


/* ========================================================================= */
/* �֐���	:CInfoMapEventMOVE::GetWriteData								 */
/* ���e		:�w��v�f�̕ۑ��p�f�[�^���擾									 */
/* ���t		:2007/07/05														 */
/* ========================================================================= */

PBYTE CInfoMapEventMOVE::GetWriteData(int nNo, PDWORD pdwSize)
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
		case 0:	pSrc = (PBYTE)&m_ptDst;		break;
		}

		if (pSrc) {
			CopyMemory (pRet, pSrc, dwSize);
		}
	}

Exit:
	return pRet;
}


/* ========================================================================= */
/* �֐���	:CInfoMapEventMOVE::ReadElementData								 */
/* ���e		:�w��v�f�f�[�^��ǂݍ���										 */
/* ���t		:2007/07/05														 */
/* ========================================================================= */

DWORD CInfoMapEventMOVE::ReadElementData(
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
		case 0: pDst = (PBYTE)&m_ptDst;	dwSize = sizeof (m_ptDst);	break;
		}

		if (pDst) {
			CopyMemory (pDst, pSrc, dwSize);
		}
	}

	return dwSize;
}


/* ========================================================================= */
/* �֐���	:CInfoMapEventMOVE::GetSendDataSize								 */
/* ���e		:���M�f�[�^�T�C�Y���擾											 */
/* ���t		:2008/06/24														 */
/* ========================================================================= */

DWORD CInfoMapEventMOVE::GetSendDataSize(void)
{
	DWORD dwRet;

	dwRet = CInfoMapEventBase::GetSendDataSize ();
	dwRet += sizeof (m_ptDst);

	return dwRet;
}


/* ========================================================================= */
/* �֐���	:CInfoMapEventMOVE::GetSendData									 */
/* ���e		:���M�f�[�^���擾												 */
/* ���t		:2008/06/24														 */
/* ========================================================================= */

PBYTE CInfoMapEventMOVE::GetSendData(void)
{
	PBYTE pData, pDataTmp, pDataBase;
	DWORD dwSize, dwSizeBase;

	pDataBase  = CInfoMapEventBase::GetSendData ();
	dwSizeBase = CInfoMapEventBase::GetSendDataSize ();

	dwSize	= GetSendDataSize ();
	pData	= ZeroNew (dwSize);

	pDataTmp = pData;

	CopyMemoryRenew (pDataTmp, pDataBase, dwSizeBase, pDataTmp);

	CopyMemoryRenew (pDataTmp, &m_ptDst, sizeof (m_ptDst), pDataTmp);	/* �ړ��� */

	SAFE_DELETE_ARRAY (pDataBase);

	return pData;
}


/* ========================================================================= */
/* �֐���	:CInfoMapEventMOVE::SetSendData									 */
/* ���e		:���M�f�[�^�����荞��											 */
/* ���t		:2008/06/24														 */
/* ========================================================================= */

PBYTE CInfoMapEventMOVE::SetSendData(PBYTE pSrc)
{
	PBYTE pRet, pDataTmp;

	pRet = pSrc;

	pDataTmp = pSrc;
	pDataTmp = CInfoMapEventBase::SetSendData (pSrc);

	CopyMemoryRenew (&m_ptDst, pDataTmp, sizeof (m_ptDst), pDataTmp);	/* �ړ��� */

	pRet = pDataTmp;
	return pRet;
}


/* ========================================================================= */
/* �֐���	:CInfoMapEventMOVE::Copy										 */
/* ���e		:�R�s�[															 */
/* ���t		:2008/06/25														 */
/* ���l		:��ʂ��Ⴄ�ꍇ�͂������Ȃ��ƂɂȂ�̂Œ���						 */
/* ========================================================================= */

void CInfoMapEventMOVE::Copy(CInfoMapEventBase *pSrc)
{
	PCInfoMapEventMOVE pSrcTmp = (PCInfoMapEventMOVE)pSrc;

	if (pSrcTmp == NULL) {
		return;
	}
	CInfoMapEventBase::Copy (pSrc);

	m_ptDst = pSrcTmp->m_ptDst;
}

/* Copyright(C)URARA-works 2007 */
