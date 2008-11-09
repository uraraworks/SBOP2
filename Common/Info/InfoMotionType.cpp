/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:InfoMotionType.cpp											 */
/* ���e			:���[�V������ʏ��N���X �����t�@�C��						 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/05/26													 */
/* ========================================================================= */

#include "stdafx.h"
#include "InfoMotionType.h"

/* ========================================================================= */
/* �萔��`																	 */
/* ========================================================================= */

/* �w�b�_��� */
static LPCSTR s_aszName[] = {
	"m_dwMotionTypeID",		/* ���[�V�������ID */
	"m_wGrpIDSub",			/* �v���r���[�p�O���t�B�b�NID�T�u */
	"m_strName",			/* ���[�V������ʖ� */
	NULL
};


/* ========================================================================= */
/* �֐���	:CInfoMotionType::CInfoMotionType								 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/05/26														 */
/* ========================================================================= */

CInfoMotionType::CInfoMotionType()
{
	m_wGrpIDSub		 = 0;
	m_dwMotionTypeID = 0;

	for (m_nElementCount = 0; s_aszName[m_nElementCount] != NULL; m_nElementCount ++) {}
}


/* ========================================================================= */
/* �֐���	:CInfoMotionType::~CInfoMotionType								 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/05/26														 */
/* ========================================================================= */

CInfoMotionType::~CInfoMotionType()
{
}


/* ========================================================================= */
/* �֐���	:CInfoMotionType::GetElementNo									 */
/* ���e		:�v�f�ԍ����擾													 */
/* ���t		:2005/05/01														 */
/* ========================================================================= */

int CInfoMotionType::GetElementNo(LPCSTR pszName)
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
/* �֐���	:CInfoMotionType::GetDataSize									 */
/* ���e		:�f�[�^�T�C�Y���擾												 */
/* ���t		:2008/05/26														 */
/* ========================================================================= */

DWORD CInfoMotionType::GetDataSize(void)
{
	DWORD dwRet;

	dwRet = sizeof (m_dwMotionTypeID)	+
			sizeof (m_wGrpIDSub)		+
			(m_strName.GetLength () + 1);

	return dwRet;
}


/* ========================================================================= */
/* �֐���	:CInfoMotionType::GetDataSizeNo									 */
/* ���e		:�w��v�f�̃f�[�^�T�C�Y���擾									 */
/* ���t		:2008/05/26														 */
/* ========================================================================= */

DWORD CInfoMotionType::GetDataSizeNo(int nNo)
{
	DWORD dwRet;

	dwRet = 0;

	switch (nNo) {
	case 0:		dwRet = sizeof (m_dwMotionTypeID);		break;		/* ���[�V�������ID */
	case 1:		dwRet = sizeof (m_wGrpIDSub);			break;		/* �v���r���[�p�O���t�B�b�NID�T�u */
	case 2:		dwRet = (m_strName.GetLength () + 1);	break;		/* ���[�V������ʖ� */
	}

	return dwRet;
}


/* ========================================================================= */
/* �֐���	:CInfoMotionType::GetName										 */
/* ���e		:�v�f�����擾													 */
/* ���t		:2008/05/26														 */
/* ========================================================================= */

LPCSTR CInfoMotionType::GetName(int nNo)
{
	return s_aszName[nNo];
}


/* ========================================================================= */
/* �֐���	:CInfoMotionType::GetWriteData									 */
/* ���e		:�w��v�f�̕ۑ��p�f�[�^���擾									 */
/* ���t		:2008/05/26														 */
/* ========================================================================= */

PBYTE CInfoMotionType::GetWriteData(int nNo, PDWORD pdwSize)
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
	case 0:		pSrc = (PBYTE)&m_dwMotionTypeID;	break;		/* ���[�V�������ID */
	case 1:		pSrc = (PBYTE)&m_wGrpIDSub;			break;		/* �v���r���[�p�O���t�B�b�NID�T�u */
	case 2:		pSrc = (PBYTE)(LPCSTR)m_strName;	break;		/* ���[�V������ʖ� */
	}

	if (pSrc) {
		CopyMemory (pRet, pSrc, dwSize);
	}

Exit:
	return pRet;
}


/* ========================================================================= */
/* �֐���	:CInfoMotionType::ReadElementData								 */
/* ���e		:�w��v�f�f�[�^��ǂݍ���										 */
/* ���t		:2008/05/26														 */
/* ========================================================================= */

DWORD CInfoMotionType::ReadElementData(
	PBYTE pSrc,		/* [in] �f�[�^�̓ǂݍ��݌� */
	int nNo)		/* [in] �v�f�ԍ� */
{
	PBYTE pDst;
	DWORD dwSize;

	pDst	= NULL;
	dwSize	= 0;

	switch (nNo) {
	case 0:		pDst = (PBYTE)&m_dwMotionTypeID;	dwSize = sizeof (m_dwMotionTypeID);	break;		/* ���[�V�������ID */
	case 1:		pDst = (PBYTE)&m_wGrpIDSub;			dwSize = sizeof (m_wGrpIDSub);		break;		/* �v���r���[�p�O���t�B�b�NID�T�u */
	case 2:		/* ���[�V������ʖ� */
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
/* �֐���	:CInfoMotionType::GetSendDataSize								 */
/* ���e		:���M�f�[�^�T�C�Y���擾											 */
/* ���t		:2008/05/26														 */
/* ========================================================================= */

DWORD CInfoMotionType::GetSendDataSize(void)
{
	return GetDataSize ();
}


/* ========================================================================= */
/* �֐���	:CInfoMotionType::GetSendData									 */
/* ���e		:���M�f�[�^���擾												 */
/* ���t		:2008/05/26														 */
/* ========================================================================= */

PBYTE CInfoMotionType::GetSendData(void)
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;

	dwSize	= GetSendDataSize ();
	pData	= ZeroNew (dwSize);

	pDataTmp = pData;

	CopyMemoryRenew (pDataTmp, &m_dwMotionTypeID,	sizeof (m_dwMotionTypeID),	pDataTmp);	/* ���[�V�������ID */
	CopyMemoryRenew (pDataTmp, &m_wGrpIDSub,		sizeof (m_wGrpIDSub),		pDataTmp);	/* �v���r���[�p�O���t�B�b�NID�T�u */
	strcpyRenew ((LPSTR)pDataTmp, m_strName, pDataTmp);										/* ���[�V������ʖ� */

	return pData;
}


/* ========================================================================= */
/* �֐���	:CInfoMotionType::SetSendData									 */
/* ���e		:���M�f�[�^�����荞��											 */
/* ���t		:2008/05/26														 */
/* ========================================================================= */

PBYTE CInfoMotionType::SetSendData(PBYTE pSrc)
{
	PBYTE pDataTmp;

	pDataTmp = pSrc;

	CopyMemoryRenew (&m_dwMotionTypeID, pDataTmp, sizeof (m_dwMotionTypeID), pDataTmp);	/* ���[�V�������ID */
	CopyMemoryRenew (&m_wGrpIDSub,		pDataTmp, sizeof (m_wGrpIDSub),		 pDataTmp);	/* �v���r���[�p�O���t�B�b�NID�T�u */
	StoreRenew (m_strName, (LPCSTR)pDataTmp, pDataTmp);									/* ���[�V������ʖ� */

	return pDataTmp;
}


/* ========================================================================= */
/* �֐���	:CInfoMotionType::Copy											 */
/* ���e		:�R�s�[															 */
/* ���t		:2007/11/23														 */
/* ========================================================================= */

void CInfoMotionType::Copy(CInfoMotionType *pSrc)
{
	m_dwMotionTypeID	= pSrc->m_dwMotionTypeID;	/* ���[�V�������ID */
	m_wGrpIDSub			= pSrc->m_wGrpIDSub;		/* �v���r���[�p�O���t�B�b�NID�T�u */
	m_strName			= pSrc->m_strName;			/* ���[�V������ʖ� */
}

/* Copyright(C)URARA-works 2008 */
