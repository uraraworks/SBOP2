/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:InfoTalkEventPAGE.cpp										 */
/* ���e			:��b�C�x���g���(�y�[�W�؂�ւ�)�N���X �����t�@�C��		 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/12/29													 */
/* ========================================================================= */

#include "stdafx.h"
#include "InfoTalkEventPAGE.h"

/* ========================================================================= */
/* �萔��`																	 */
/* ========================================================================= */

/* �w�b�_��� */
static LPCSTR s_aszName[] = {
	"m_nPageChgCondition",	/* �y�[�W�؂�ւ����� */
	"m_nPageJump",			/* �y�[�W�؂�ւ��� */
	NULL
};


/* ========================================================================= */
/* �֐���	:CInfoTalkEventPAGE::CInfoTalkEventPAGE							 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/12/29														 */
/* ========================================================================= */

CInfoTalkEventPAGE::CInfoTalkEventPAGE()
{
	m_nEventType = TALKEVENTTYPE_PAGE;

	m_nElementCountBase = m_nElementCount;
	for (m_nElementCount = 0; s_aszName[m_nElementCount] != NULL; m_nElementCount ++) {}
}


/* ========================================================================= */
/* �֐���	:CInfoTalkEventPAGE::~CInfoTalkEventPAGE						 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/12/29														 */
/* ========================================================================= */

CInfoTalkEventPAGE::~CInfoTalkEventPAGE()
{
}


/* ========================================================================= */
/* �֐���	:CInfoTalkEventPAGE::GetElementCount							 */
/* ���e		:�v�f�����擾													 */
/* ���t		:2008/12/28														 */
/* ========================================================================= */

int CInfoTalkEventPAGE::GetElementCount(void)
{
	return m_nElementCountBase + m_nElementCount;
}


/* ========================================================================= */
/* �֐���	:CInfoTalkEventPAGE::GetElementNo								 */
/* ���e		:�v�f�ԍ����擾													 */
/* ���t		:2005/07/05														 */
/* ========================================================================= */

int CInfoTalkEventPAGE::GetElementNo(LPCSTR pszName)
{
	int i, nRet;

	nRet = CInfoTalkEventBase::GetElementNo (pszName);
	if (nRet >= 0) {
		goto Exit;
	}

	for (i = 0; s_aszName[i] != NULL; i ++) {
		if (strcmp (s_aszName[i], pszName) == 0) {
			nRet = m_nElementCountBase + i;
			break;
		}
	}

Exit:
	return nRet;
}


/* ========================================================================= */
/* �֐���	:CInfoTalkEventPAGE::GetDataSize								 */
/* ���e		:�f�[�^�T�C�Y���擾												 */
/* ���t		:2008/12/29														 */
/* ========================================================================= */

DWORD CInfoTalkEventPAGE::GetDataSize(void)
{
	DWORD dwRet;

	dwRet = CInfoTalkEventBase::GetDataSize();
	dwRet += sizeof (m_nPageChgCondition);		/* �y�[�W�؂�ւ����� */
	dwRet += sizeof (m_nPageJump);				/* �y�[�W�؂�ւ��� */

	return dwRet;
}


/* ========================================================================= */
/* �֐���	:CInfoTalkEventPAGE::GetDataSizeNo								 */
/* ���e		:�w��v�f�̃f�[�^�T�C�Y���擾									 */
/* ���t		:2008/12/29														 */
/* ========================================================================= */

DWORD CInfoTalkEventPAGE::GetDataSizeNo(int nNo)
{
	DWORD dwRet;

	dwRet = 0;
	if (nNo < m_nElementCountBase) {
		return CInfoTalkEventBase::GetDataSizeNo (nNo);
	}

	switch (nNo - m_nElementCountBase) {
	case 0:	dwRet = sizeof (m_nPageChgCondition);	break;		/* �y�[�W�؂�ւ����� */
	case 1:	dwRet = sizeof (m_nPageJump);			break;		/* �y�[�W�؂�ւ��� */
	}

	return dwRet;
}


/* ========================================================================= */
/* �֐���	:CInfoTalkEventPAGE::GetName									 */
/* ���e		:�v�f�����擾													 */
/* ���t		:2005/07/05														 */
/* ========================================================================= */

LPCSTR CInfoTalkEventPAGE::GetName(int nNo)
{
	if (nNo < m_nElementCountBase) {
		return CInfoTalkEventBase::GetName (nNo);
	}
	return s_aszName[nNo - m_nElementCountBase];
}


/* ========================================================================= */
/* �֐���	:CInfoTalkEventPAGE::GetWriteData								 */
/* ���e		:�w��v�f�̕ۑ��p�f�[�^���擾									 */
/* ���t		:2008/12/29														 */
/* ========================================================================= */

PBYTE CInfoTalkEventPAGE::GetWriteData(int nNo, PDWORD pdwSize)
{
	DWORD dwSize;
	PBYTE pRet, pSrc;

	if (nNo < m_nElementCountBase) {
		return CInfoTalkEventBase::GetWriteData (nNo, pdwSize);
	}

	pRet		= NULL;
	pSrc		= NULL;
	dwSize		= GetDataSizeNo (nNo);
	*pdwSize	= dwSize;

	if (dwSize == 0) {
		goto Exit;
	}
	pRet = ZeroNew (dwSize);

	nNo -= m_nElementCountBase;
	switch (nNo) {
	case 0:	pSrc = (PBYTE)&m_nPageChgCondition;		break;		/* �y�[�W�؂�ւ����� */
	case 1:	pSrc = (PBYTE)&m_nPageJump;				break;		/* �y�[�W�؂�ւ��� */
	}

	if (pSrc) {
		CopyMemory (pRet, pSrc, dwSize);
	}

Exit:
	return pRet;
}


/* ========================================================================= */
/* �֐���	:CInfoTalkEventPAGE::ReadElementData							 */
/* ���e		:�w��v�f�f�[�^��ǂݍ���										 */
/* ���t		:2008/12/29														 */
/* ========================================================================= */

DWORD CInfoTalkEventPAGE::ReadElementData(
	PBYTE pSrc,		/* [in] �f�[�^�̓ǂݍ��݌� */
	int nNo)		/* [in] �v�f�ԍ� */
{
	DWORD dwSize;
	PBYTE pDst;

	pDst	= NULL;
	dwSize	= 0;

	if (nNo < m_nElementCountBase) {
		return CInfoTalkEventBase::ReadElementData (pSrc, nNo);
	}

	nNo -= m_nElementCountBase;
	switch (nNo) {
	case 0:	pDst = (PBYTE)&m_nPageChgCondition;	dwSize = sizeof (m_nPageChgCondition);	break;	/* �y�[�W�؂�ւ����� */
	case 1:	pDst = (PBYTE)&m_nPageJump;			dwSize = sizeof (m_nPageJump);			break;	/* �y�[�W�؂�ւ��� */
	}

	if (pDst) {
		CopyMemory (pDst, pSrc, dwSize);
	}

	return dwSize;
}


/* ========================================================================= */
/* �֐���	:CInfoTalkEventPAGE::GetSendDataSize							 */
/* ���e		:���M�f�[�^�T�C�Y���擾											 */
/* ���t		:2008/12/29														 */
/* ========================================================================= */

DWORD CInfoTalkEventPAGE::GetSendDataSize(void)
{
	DWORD dwRet;

	dwRet = CInfoTalkEventBase::GetSendDataSize ();
	dwRet += GetDataSizeNo (m_nElementCountBase + 0);	/* �y�[�W�؂�ւ����� */
	dwRet += GetDataSizeNo (m_nElementCountBase + 1);	/* �y�[�W�؂�ւ��� */

	return dwRet;
}


/* ========================================================================= */
/* �֐���	:CInfoTalkEventPAGE::GetSendData								 */
/* ���e		:���M�f�[�^���擾												 */
/* ���t		:2008/12/29														 */
/* ========================================================================= */

PBYTE CInfoTalkEventPAGE::GetSendData(void)
{
	PBYTE pData, pDataTmp, pDataBase;
	DWORD dwSize, dwSizeBase;

	dwSize	 = GetSendDataSize ();
	pData	 = ZeroNew (dwSize);
	pDataTmp = pData;

	pDataBase  = CInfoTalkEventBase::GetSendData ();
	dwSizeBase = CInfoTalkEventBase::GetSendDataSize ();
	CopyMemoryRenew (pDataTmp, pDataBase, dwSizeBase, pDataTmp);
	SAFE_DELETE_ARRAY (pDataBase);

	CopyMemoryRenew (pDataTmp, &m_nPageChgCondition,	sizeof (m_nPageChgCondition),	pDataTmp);	/* �y�[�W�؂�ւ����� */
	CopyMemoryRenew (pDataTmp, &m_nPageJump,			sizeof (m_nPageJump),			pDataTmp);	/* �y�[�W�؂�ւ��� */

	return pData;
}


/* ========================================================================= */
/* �֐���	:CInfoTalkEventPAGE::SetSendData								 */
/* ���e		:���M�f�[�^�����荞��											 */
/* ���t		:2008/12/29														 */
/* ========================================================================= */

PBYTE CInfoTalkEventPAGE::SetSendData(PBYTE pSrc)
{
	PBYTE pRet, pDataTmp;

	pRet = pSrc;

	pDataTmp = CInfoTalkEventBase::SetSendData (pSrc);
	CopyMemoryRenew (&m_nPageChgCondition,	pDataTmp, sizeof (m_nPageChgCondition),	pDataTmp);	/* �y�[�W�؂�ւ����� */
	CopyMemoryRenew (&m_nPageJump,			pDataTmp, sizeof (m_nPageJump),			pDataTmp);	/* �y�[�W�؂�ւ��� */

	pRet = pDataTmp;
	return pRet;
}


/* ========================================================================= */
/* �֐���	:CInfoTalkEventPAGE::Copy										 */
/* ���e		:�R�s�[															 */
/* ���t		:2008/12/29														 */
/* ========================================================================= */

void CInfoTalkEventPAGE::Copy(CInfoTalkEventBase *pSrc)
{
	PCInfoTalkEventPAGE pSrcTmp;

	if (pSrc == NULL) {
		return;
	}
	pSrcTmp = (PCInfoTalkEventPAGE)pSrc;

	CInfoTalkEventBase::Copy (pSrcTmp);
	m_nPageChgCondition	= pSrcTmp->m_nPageChgCondition;			/* �y�[�W�؂�ւ����� */
	m_nPageJump			= pSrcTmp->m_nPageJump;					/* �y�[�W�؂�ւ��� */
}

/* Copyright(C)URARA-works 2008 */
