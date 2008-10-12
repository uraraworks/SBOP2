/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:InfoMapEventGRPIDTMP.cpp									 */
/* ���e			:�C�x���g���(�ꎞ�摜�ݒ�)�N���X �����t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/10/12													 */
/* ========================================================================= */

#include "stdafx.h"
#include "InfoMapEventGRPIDTMP.h"

/* ========================================================================= */
/* �萔��`																	 */
/* ========================================================================= */

/* �w�b�_��� */
static LPCSTR s_aszName[] = {
	"m_nSetType",		/* �ݒ��� */
	"m_dwIDMain",		/* �摜ID���C�� */
	"m_dwIDSub",		/* �摜ID�T�u */
	NULL
};


/* ========================================================================= */
/* �֐���	:CInfoMapEventGRPIDTMP::CInfoMapEventGRPIDTMP					 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/10/12														 */
/* ========================================================================= */

CInfoMapEventGRPIDTMP::CInfoMapEventGRPIDTMP()
{
	m_nSetType = MAPEVENTTYPE_GRPIDTMP;

	m_nSetType	= GRPIDTMPTYPE_OFF;
	m_dwIDMain	= 0;
	m_dwIDSub	= 0;

	for (m_nElementCount = 0; s_aszName[m_nElementCount] != NULL; m_nElementCount ++) {}
	m_nElementCount += m_nElementCountBase;
}


/* ========================================================================= */
/* �֐���	:CInfoMapEventGRPIDTMP::~CInfoMapEventGRPIDTMP					 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/10/12														 */
/* ========================================================================= */

CInfoMapEventGRPIDTMP::~CInfoMapEventGRPIDTMP()
{
}


/* ========================================================================= */
/* �֐���	:CInfoMapEventGRPIDTMP::GetElementNo							 */
/* ���e		:�v�f�ԍ����擾													 */
/* ���t		:2008/10/12														 */
/* ========================================================================= */

int CInfoMapEventGRPIDTMP::GetElementNo(LPCSTR pszName)
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
/* �֐���	:CInfoMapEventGRPIDTMP::GetDataSize								 */
/* ���e		:�f�[�^�T�C�Y���擾												 */
/* ���t		:2008/10/12														 */
/* ========================================================================= */

DWORD CInfoMapEventGRPIDTMP::GetDataSize(void)
{
	DWORD dwRet;

	dwRet = CInfoMapEventBase::GetDataSize ();
	dwRet += sizeof (m_nSetType);
	dwRet += sizeof (m_dwIDMain);
	dwRet += sizeof (m_dwIDSub);

	return dwRet;
}


/* ========================================================================= */
/* �֐���	:CInfoMapEventGRPIDTMP::GetDataSizeNo							 */
/* ���e		:�w��v�f�̃f�[�^�T�C�Y���擾									 */
/* ���t		:2008/10/12														 */
/* ========================================================================= */

DWORD CInfoMapEventGRPIDTMP::GetDataSizeNo(int nNo)
{
	DWORD dwRet;

	if (nNo < m_nElementCountBase) {
		dwRet = CInfoMapEventBase::GetDataSizeNo (nNo);
	} else {
		switch (nNo - m_nElementCountBase) {
		case 0:	dwRet = sizeof (m_nSetType);	break;	/* �ݒ��� */
		case 1:	dwRet = sizeof (m_dwIDMain);	break;	/* �摜ID���C�� */
		case 2:	dwRet = sizeof (m_dwIDSub);		break;	/* �摜ID�T�u */
		}
	}

	return dwRet;
}


/* ========================================================================= */
/* �֐���	:CInfoMapEventGRPIDTMP::GetName									 */
/* ���e		:�v�f�����擾													 */
/* ���t		:2008/10/12														 */
/* ========================================================================= */

LPCSTR CInfoMapEventGRPIDTMP::GetName(int nNo)
{
	if (nNo < m_nElementCountBase) {
		return CInfoMapEventBase::GetName (nNo);
	}

	return s_aszName[nNo - m_nElementCountBase];
}


/* ========================================================================= */
/* �֐���	:CInfoMapEventGRPIDTMP::GetWriteData							 */
/* ���e		:�w��v�f�̕ۑ��p�f�[�^���擾									 */
/* ���t		:2008/10/12														 */
/* ========================================================================= */

PBYTE CInfoMapEventGRPIDTMP::GetWriteData(int nNo, PDWORD pdwSize)
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
		case 0:	pSrc = (PBYTE)&m_nSetType;		break;	/* �ݒ��� */
		case 1:	pSrc = (PBYTE)&m_dwIDMain;		break;	/* �摜ID���C�� */
		case 2:	pSrc = (PBYTE)&m_dwIDSub;		break;	/* �摜ID�T�u */
		}

		if (pSrc) {
			CopyMemory (pRet, pSrc, dwSize);
		}
	}

Exit:
	return pRet;
}


/* ========================================================================= */
/* �֐���	:CInfoMapEventGRPIDTMP::ReadElementData							 */
/* ���e		:�w��v�f�f�[�^��ǂݍ���										 */
/* ���t		:2008/10/12														 */
/* ========================================================================= */

DWORD CInfoMapEventGRPIDTMP::ReadElementData(
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
		case 0:	pDst = (PBYTE)&m_nSetType;	dwSize = sizeof (m_nSetType);	break;	/* �ݒ��� */
		case 1:	pDst = (PBYTE)&m_dwIDMain;	dwSize = sizeof (m_dwIDMain);	break;	/* �摜ID���C�� */
		case 2:	pDst = (PBYTE)&m_dwIDSub;	dwSize = sizeof (m_dwIDSub);	break;	/* �摜ID�T�u */
		}

		if (pDst) {
			CopyMemory (pDst, pSrc, dwSize);
		}
	}

	return dwSize;
}


/* ========================================================================= */
/* �֐���	:CInfoMapEventGRPIDTMP::GetSendDataSize							 */
/* ���e		:���M�f�[�^�T�C�Y���擾											 */
/* ���t		:2008/10/12														 */
/* ========================================================================= */

DWORD CInfoMapEventGRPIDTMP::GetSendDataSize(void)
{
	DWORD dwRet;

	dwRet = CInfoMapEventBase::GetSendDataSize ();
	dwRet += sizeof (m_nSetType);
	dwRet += sizeof (m_dwIDMain);
	dwRet += sizeof (m_dwIDSub);

	return dwRet;
}


/* ========================================================================= */
/* �֐���	:CInfoMapEventGRPIDTMP::GetSendData								 */
/* ���e		:���M�f�[�^���擾												 */
/* ���t		:2008/10/12														 */
/* ========================================================================= */

PBYTE CInfoMapEventGRPIDTMP::GetSendData(void)
{
	PBYTE pData, pDataTmp, pDataBase;
	DWORD dwSize, dwSizeBase;

	pDataBase  = CInfoMapEventBase::GetSendData ();
	dwSizeBase = CInfoMapEventBase::GetSendDataSize ();

	dwSize	= GetSendDataSize ();
	pData	= ZeroNew (dwSize);

	pDataTmp = pData;

	CopyMemoryRenew (pDataTmp, pDataBase, dwSizeBase, pDataTmp);

	CopyMemoryRenew (pDataTmp, &m_nSetType,	sizeof (m_nSetType), pDataTmp);		/* �ݒ��� */
	CopyMemoryRenew (pDataTmp, &m_dwIDMain,	sizeof (m_dwIDMain), pDataTmp);		/* �摜ID���C�� */
	CopyMemoryRenew (pDataTmp, &m_dwIDSub,	sizeof (m_dwIDSub),	 pDataTmp);		/* �摜ID�T�u */

	SAFE_DELETE_ARRAY (pDataBase);

	return pData;
}


/* ========================================================================= */
/* �֐���	:CInfoMapEventGRPIDTMP::SetSendData								 */
/* ���e		:���M�f�[�^�����荞��											 */
/* ���t		:2008/10/12														 */
/* ========================================================================= */

PBYTE CInfoMapEventGRPIDTMP::SetSendData(PBYTE pSrc)
{
	PBYTE pRet, pDataTmp;

	pRet = pSrc;

	pDataTmp = pSrc;
	pDataTmp = CInfoMapEventBase::SetSendData (pSrc);

	CopyMemoryRenew (&m_nSetType,	pDataTmp, sizeof (m_nSetType),	pDataTmp);		/* �ݒ��� */
	CopyMemoryRenew (&m_dwIDMain,	pDataTmp, sizeof (m_dwIDMain),	pDataTmp);		/* �摜ID���C�� */
	CopyMemoryRenew (&m_dwIDSub,	pDataTmp, sizeof (m_dwIDSub),	pDataTmp);		/* �摜ID�T�u */

	pRet = pDataTmp;
	return pRet;
}


/* ========================================================================= */
/* �֐���	:CInfoMapEventGRPIDTMP::Copy									 */
/* ���e		:�R�s�[															 */
/* ���t		:2008/10/12														 */
/* ���l		:��ʂ��Ⴄ�ꍇ�͂������Ȃ��ƂɂȂ�̂Œ���						 */
/* ========================================================================= */

void CInfoMapEventGRPIDTMP::Copy(CInfoMapEventBase *pSrc)
{
	PCInfoMapEventGRPIDTMP pSrcTmp = (PCInfoMapEventGRPIDTMP)pSrc;

	if (pSrcTmp == NULL) {
		return;
	}
	CInfoMapEventBase::Copy (pSrc);

	m_nSetType	= pSrcTmp->m_nSetType;	/* �ݒ��� */
	m_dwIDMain	= pSrcTmp->m_dwIDMain;	/* �摜ID���C�� */
	m_dwIDSub	= pSrcTmp->m_dwIDSub;	/* �摜ID�T�u */
}

/* Copyright(C)URARA-works 2008 */
