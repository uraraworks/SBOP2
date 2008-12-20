/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:InfoTalkEventBase.cpp										 */
/* ���e			:��b�C�x���g�����N���X �����t�@�C��					 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/12/18													 */
/* ========================================================================= */

#include "stdafx.h"
#include "InfoTalkEventBase.h"

/* ========================================================================= */
/* �萔��`																	 */
/* ========================================================================= */

/* �w�b�_��� */
static LPCSTR s_aszName[] = {
	"m_nEventType",					/* ��b�C�x���g��� */
	"m_nPage",						/* �����y�[�W�ԍ� */
	"m_nPageChgCondition",			/* �y�[�W�؂�ւ����� */
	"m_dwData",						/* �o�C�i���f�[�^ */
	"m_strText",					/* ������f�[�^ */
	NULL
};


/* ========================================================================= */
/* �֐���	:CInfoTalkEventBase::CInfoTalkEventBase							 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/12/18														 */
/* ========================================================================= */

CInfoTalkEventBase::CInfoTalkEventBase()
{
	m_nEventType		= TALKEVENTTYPE_NONE;
	m_nPage				= 0;
	m_nPageChgCondition	= CHGPAGECONDITION_NONE;
	m_dwData			= 0;

	for (m_nElementCount = 0; s_aszName[m_nElementCount] != NULL; m_nElementCount ++) {}
}


/* ========================================================================= */
/* �֐���	:CInfoTalkEventBase::~CInfoTalkEventBase						 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/12/18														 */
/* ========================================================================= */

CInfoTalkEventBase::~CInfoTalkEventBase()
{
}


/* ========================================================================= */
/* �֐���	:CInfoTalkEventBase::GetElementNo								 */
/* ���e		:�v�f�ԍ����擾													 */
/* ���t		:2005/07/05														 */
/* ========================================================================= */

int CInfoTalkEventBase::GetElementNo(LPCSTR pszName)
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
/* �֐���	:CInfoTalkEventBase::GetDataSize								 */
/* ���e		:�f�[�^�T�C�Y���擾												 */
/* ���t		:2008/12/18														 */
/* ========================================================================= */

DWORD CInfoTalkEventBase::GetDataSize(void)
{
	DWORD dwRet;

	dwRet = 0;
	dwRet += sizeof (m_nEventType);					/* ��b�C�x���g��� */
	dwRet += sizeof (m_nPage);						/* �����y�[�W�ԍ� */
	dwRet += sizeof (m_nPageChgCondition);			/* �y�[�W�؂�ւ����� */
	dwRet += sizeof (m_dwData);						/* �o�C�i���f�[�^ */
	dwRet += (m_strText.GetLength () + 1);			/* ������f�[�^ */

	return dwRet;
}


/* ========================================================================= */
/* �֐���	:CInfoTalkEventBase::GetDataSizeNo								 */
/* ���e		:�w��v�f�̃f�[�^�T�C�Y���擾									 */
/* ���t		:2008/12/18														 */
/* ========================================================================= */

DWORD CInfoTalkEventBase::GetDataSizeNo(int nNo)
{
	DWORD dwRet;

	dwRet = 0;

	switch (nNo) {
	case 0:	dwRet = sizeof (m_nEventType);			break;	/* ��b�C�x���g��� */
	case 1:	dwRet = sizeof (m_nPage);				break;	/* �����y�[�W�ԍ� */
	case 2:	dwRet = sizeof (m_nPageChgCondition);	break;	/* �y�[�W�؂�ւ����� */
	case 3:	dwRet = sizeof (m_dwData);				break;	/* �o�C�i���f�[�^ */
	case 4:	dwRet = m_strText.GetLength () + 1;		break;	/* ������f�[�^ */
	}

	return dwRet;
}


/* ========================================================================= */
/* �֐���	:CInfoTalkEventBase::GetName									 */
/* ���e		:�v�f�����擾													 */
/* ���t		:2005/07/05														 */
/* ========================================================================= */

LPCSTR CInfoTalkEventBase::GetName(int nNo)
{
	return s_aszName[nNo];
}


/* ========================================================================= */
/* �֐���	:CInfoTalkEventBase::GetWriteData								 */
/* ���e		:�w��v�f�̕ۑ��p�f�[�^���擾									 */
/* ���t		:2008/12/18														 */
/* ========================================================================= */

PBYTE CInfoTalkEventBase::GetWriteData(int nNo, PDWORD pdwSize)
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
	case 0:	pSrc = (PBYTE)&m_nEventType;			break;		/* ��b�C�x���g��� */
	case 1:	pSrc = (PBYTE)&m_nPage;					break;		/* �����y�[�W�ԍ� */
	case 2:	pSrc = (PBYTE)&m_nPageChgCondition;		break;		/* �y�[�W�؂�ւ����� */
	case 3:	pSrc = (PBYTE)&m_dwData;				break;		/* �o�C�i���f�[�^ */
	case 4:	pSrc = (PBYTE)(LPCSTR)m_strText;		break;		/* ������f�[�^ */
	}

	if (pSrc) {
		CopyMemory (pRet, pSrc, dwSize);
	}

Exit:
	return pRet;
}


/* ========================================================================= */
/* �֐���	:CInfoTalkEventBase::ReadElementData							 */
/* ���e		:�w��v�f�f�[�^��ǂݍ���										 */
/* ���t		:2008/12/18														 */
/* ========================================================================= */

DWORD CInfoTalkEventBase::ReadElementData(
	PBYTE pSrc,		/* [in] �f�[�^�̓ǂݍ��݌� */
	int nNo)		/* [in] �v�f�ԍ� */
{
	PBYTE pDst;
	DWORD dwSize;

	pDst	= NULL;
	dwSize	= 0;

	switch (nNo) {
	case 0: pDst = (PBYTE)&m_nEventType;		dwSize = sizeof (m_nEventType);			break;		/* ��b�C�x���g��� */
	case 1: pDst = (PBYTE)&m_nPage;				dwSize = sizeof (m_nPage);				break;		/* �����y�[�W�ԍ� */
	case 2: pDst = (PBYTE)&m_nPageChgCondition;	dwSize = sizeof (m_nPageChgCondition);	break;		/* �y�[�W�؂�ւ����� */
	case 3: pDst = (PBYTE)&m_dwData;			dwSize = sizeof (m_dwData);				break;		/* �o�C�i���f�[�^ */
	case 4:				/* ������f�[�^ */
		m_strText = (LPCSTR)pSrc;
		dwSize = m_strText.GetLength () + 1;
		break;
	}

	if (pDst) {
		CopyMemory (pDst, pSrc, dwSize);
	}

	return dwSize;
}


/* ========================================================================= */
/* �֐���	:CInfoTalkEventBase::GetSendDataSize							 */
/* ���e		:���M�f�[�^�T�C�Y���擾											 */
/* ���t		:2008/12/18														 */
/* ========================================================================= */

DWORD CInfoTalkEventBase::GetSendDataSize(void)
{
	DWORD dwRet;

	dwRet = 0;
	dwRet += sizeof (m_nEventType);					/* ��b�C�x���g��� */
	dwRet += sizeof (m_nPage);						/* �����y�[�W�ԍ� */
	dwRet += sizeof (m_nPageChgCondition);			/* �y�[�W�؂�ւ����� */
	dwRet += sizeof (m_dwData);						/* �o�C�i���f�[�^ */
	dwRet += (m_strText.GetLength () + 1);			/* ������f�[�^ */

	return dwRet;
}


/* ========================================================================= */
/* �֐���	:CInfoTalkEventBase::GetSendData								 */
/* ���e		:���M�f�[�^���擾												 */
/* ���t		:2008/12/18														 */
/* ========================================================================= */

PBYTE CInfoTalkEventBase::GetSendData(void)
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;

	dwSize	= GetSendDataSize ();
	pData	= ZeroNew (dwSize);

	pDataTmp = pData;

	CopyMemoryRenew (pDataTmp, &m_nEventType,			sizeof (m_nEventType),			pDataTmp);		/* ��b�C�x���g��� */
	CopyMemoryRenew (pDataTmp, &m_nPage,				sizeof (m_nPage),				pDataTmp);		/* �����y�[�W�ԍ� */
	CopyMemoryRenew (pDataTmp, &m_nPageChgCondition,	sizeof (m_nPageChgCondition),	pDataTmp);		/* �y�[�W�؂�ւ����� */
	CopyMemoryRenew (pDataTmp, &m_dwData,				sizeof (m_dwData),				pDataTmp);		/* �o�C�i���f�[�^ */
	strcpyRenew ((LPSTR)pDataTmp, m_strText, pDataTmp);					/* ������f�[�^ */

	return pData;
}


/* ========================================================================= */
/* �֐���	:CInfoTalkEventBase::SetSendData								 */
/* ���e		:���M�f�[�^�����荞��											 */
/* ���t		:2008/12/18														 */
/* ========================================================================= */

PBYTE CInfoTalkEventBase::SetSendData(PBYTE pSrc)
{
	PBYTE pRet, pDataTmp;

	pRet = pSrc;

	pDataTmp = pSrc;
	CopyMemoryRenew (&m_nEventType,			pDataTmp, sizeof (m_nEventType),		pDataTmp);		/* ��b�C�x���g��� */
	CopyMemoryRenew (&m_nPage,				pDataTmp, sizeof (m_nPage),				pDataTmp);		/* �����y�[�W�ԍ� */
	CopyMemoryRenew (&m_nPageChgCondition,	pDataTmp, sizeof (m_nPageChgCondition),	pDataTmp);		/* �y�[�W�؂�ւ����� */
	CopyMemoryRenew (&m_dwData,				pDataTmp, sizeof (m_dwData),			pDataTmp);		/* �o�C�i���f�[�^ */
	StoreRenew (m_strText, (LPCSTR)pDataTmp, pDataTmp);					/* ������f�[�^ */

	pRet = pDataTmp;
	return pRet;
}


/* ========================================================================= */
/* �֐���	:CInfoTalkEventBase::Copy										 */
/* ���e		:�R�s�[															 */
/* ���t		:2008/12/18														 */
/* ========================================================================= */

void CInfoTalkEventBase::Copy(CInfoTalkEventBase *pSrc)
{
	if (pSrc == NULL) {
		return;
	}

	m_nEventType		= pSrc->m_nEventType;			/* ��b�C�x���g��� */
	m_nPage				= pSrc->m_nPage;				/* �����y�[�W�ԍ� */
	m_nPageChgCondition	= pSrc->m_nPageChgCondition;	/* �y�[�W�؂�ւ����� */
	m_dwData			= pSrc->m_dwData;				/* �o�C�i���f�[�^ */
	m_strText			= pSrc->m_strText;				/* ������f�[�^ */
}

/* Copyright(C)URARA-works 2008 */
