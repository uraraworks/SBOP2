/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:InfoSkillBase.cpp											 */
/* ���e			:�X�L�������N���X �����t�@�C��							 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/12/03													 */
/* ========================================================================= */

#include "stdafx.h"
#include "InfoSkillBase.h"

/* ========================================================================= */
/* �萔��`																	 */
/* ========================================================================= */

/* �w�b�_��� */
static LPCSTR s_aszName[] = {
	"m_dwSkillID",		/* �X�L��ID */
	"m_dwSP",			/* ����SP */
	"m_dwIconID",		/* �A�C�R���摜ID */
	"m_nTypeMain",		/* �X�L�����(���C��) */
	"m_nTypeSub",		/* �X�L�����(�T�u) */
	"m_nUse",			/* �g�p���� */
	"m_strName",		/* �X�L���� */
	"derivation",		/* �h���f�[�^ */
	NULL
};


/* ========================================================================= */
/* �֐���	:CInfoSkillBase::CInfoSkillBase									 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/12/03														 */
/* ========================================================================= */

CInfoSkillBase::CInfoSkillBase()
{
	m_nClassType	= INFOSKILLTYPE_BASE;
	m_dwSkillID		= 0;
	m_dwSP			= 0;
	m_dwIconID		= 0;
	m_nTypeMain		= 0;
	m_nTypeSub		= 0;
	m_nUse			= SKILLUSE_ANY;

	for (m_nElementCountBase = 0; s_aszName[m_nElementCountBase] != NULL; m_nElementCountBase ++) {}
	m_nElementCount = m_nElementCountBase;
}


/* ========================================================================= */
/* �֐���	:CInfoSkillBase::~CInfoSkillBase								 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/12/03														 */
/* ========================================================================= */

CInfoSkillBase::~CInfoSkillBase()
{
}


/* ========================================================================= */
/* �֐���	:CInfoSkillBase::GetElementNo									 */
/* ���e		:�v�f�ԍ����擾													 */
/* ���t		:2008/12/03														 */
/* ========================================================================= */

int CInfoSkillBase::GetElementNo(LPCSTR pszName)
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
/* �֐���	:CInfoSkillBase::GetDataSize									 */
/* ���e		:�f�[�^�T�C�Y���擾												 */
/* ���t		:2008/12/03														 */
/* ========================================================================= */

DWORD CInfoSkillBase::GetDataSize(void)
{
	DWORD dwRet;

	dwRet = 0;
	dwRet += sizeof (m_dwSkillID);			/* �X�L��ID */
	dwRet += sizeof (m_dwSP);				/* ����SP */
	dwRet += sizeof (m_dwIconID);			/* �A�C�R���摜ID */
	dwRet += sizeof (m_nTypeMain);			/* �X�L�����(���C��) */
	dwRet += sizeof (m_nTypeSub);			/* �X�L�����(�T�u) */
	dwRet += sizeof (m_nUse);				/* �g�p���� */
	dwRet += (m_strName.GetLength () + 1);	/* �X�L���� */
	dwRet += GetDerivationSize ();			/* �h���f�[�^ */

	return dwRet;
}


/* ========================================================================= */
/* �֐���	:CInfoSkillBase::GetDataSizeNo									 */
/* ���e		:�w��v�f�̃f�[�^�T�C�Y���擾									 */
/* ���t		:2008/12/03														 */
/* ========================================================================= */

DWORD CInfoSkillBase::GetDataSizeNo(int nNo)
{
	DWORD dwRet;

	dwRet = 0;

	switch (nNo) {
	case 0:	dwRet = sizeof (m_dwSkillID);			break;	/* �X�L��ID */
	case 1:	dwRet = sizeof (m_dwSP);				break;	/* ����SP */
	case 2:	dwRet = sizeof (m_dwIconID);			break;	/* �A�C�R���摜ID */
	case 3:	dwRet = sizeof (m_nTypeMain);			break;	/* �X�L�����(���C��) */
	case 4:	dwRet = sizeof (m_nTypeSub);			break;	/* �X�L�����(�T�u) */
	case 5:	dwRet = sizeof (m_nUse);				break;	/* �g�p���� */
	case 6:	dwRet = (m_strName.GetLength () + 1);	break;	/* �X�L���� */
	case 7:	dwRet = GetDerivationSize ();			break;	/* �h���f�[�^ */
	}

	return dwRet;
}


/* ========================================================================= */
/* �֐���	:CInfoSkillBase::GetName										 */
/* ���e		:�v�f�����擾													 */
/* ���t		:2008/12/03														 */
/* ========================================================================= */

LPCSTR CInfoSkillBase::GetName(int nNo)
{
	return s_aszName[nNo];
}


/* ========================================================================= */
/* �֐���	:CInfoSkillBase::GetWriteData									 */
/* ���e		:�w��v�f�̕ۑ��p�f�[�^���擾									 */
/* ���t		:2008/12/03														 */
/* ========================================================================= */

PBYTE CInfoSkillBase::GetWriteData(int nNo, PDWORD pdwSize)
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
	case 0:	pSrc = (PBYTE)&m_dwSkillID;			break;	/* �X�L��ID */
	case 1:	pSrc = (PBYTE)&m_dwSP;				break;	/* ����SP */
	case 2:	pSrc = (PBYTE)&m_dwIconID;			break;	/* �A�C�R���摜ID */
	case 3:	pSrc = (PBYTE)&m_nTypeMain;			break;	/* �X�L�����(���C��) */
	case 4:	pSrc = (PBYTE)&m_nTypeSub;			break;	/* �X�L�����(�T�u) */
	case 5:	pSrc = (PBYTE)&m_nUse;				break;	/* �g�p���� */
	case 6:	pSrc = (PBYTE)(LPCSTR)m_strName;	break;	/* �X�L���� */
	case 7:		/* �h���f�[�^ */
		pSrc = GetDerivationWriteData (&dwSize);
		CopyMemory (pRet, pSrc, dwSize);
		SAFE_DELETE_ARRAY (pSrc);
		break;
	}

	if (pSrc) {
		CopyMemory (pRet, pSrc, dwSize);
	}

Exit:
	return pRet;
}


/* ========================================================================= */
/* �֐���	:CInfoSkillBase::ReadElementData								 */
/* ���e		:�w��v�f�f�[�^��ǂݍ���										 */
/* ���t		:2008/12/03														 */
/* ========================================================================= */

DWORD CInfoSkillBase::ReadElementData(
	PBYTE pSrc,		/* [in] �f�[�^�̓ǂݍ��݌� */
	int nNo)		/* [in] �v�f�ԍ� */
{
	PBYTE pDst;
	DWORD dwSize;

	pDst	= NULL;
	dwSize	= 0;

	switch (nNo) {
	case 0: pDst = (PBYTE)&m_dwSkillID;	dwSize = sizeof (m_dwSkillID);	break;	/* �X�L��ID */
	case 1: pDst = (PBYTE)&m_dwSP;		dwSize = sizeof (m_dwSP);		break;	/* ����SP */
	case 2: pDst = (PBYTE)&m_dwIconID;	dwSize = sizeof (m_dwIconID);	break;	/* �A�C�R���摜ID */
	case 3: pDst = (PBYTE)&m_nTypeMain;	dwSize = sizeof (m_nTypeMain);	break;	/* �X�L�����(���C��) */
	case 4: pDst = (PBYTE)&m_nTypeSub;	dwSize = sizeof (m_nTypeSub);	break;	/* �X�L�����(�T�u) */
	case 5: pDst = (PBYTE)&m_nUse;		dwSize = sizeof (m_nUse);		break;	/* �g�p���� */
	case 6:		/* �X�L���� */
		m_strName = (LPCSTR)pSrc;
		dwSize = m_strName.GetLength () + 1;
		break;
	case 7:		/* �h���f�[�^ */
		dwSize = ReadDerivationData (pSrc);
		break;
	}

	if (pDst) {
		CopyMemory (pDst, pSrc, dwSize);
	}

	return dwSize;
}


/* ========================================================================= */
/* �֐���	:CInfoSkillBase::GetDerivationSize								 */
/* ���e		:�h���f�[�^�T�C�Y���擾											 */
/* ���t		:2009/01/10														 */
/* ========================================================================= */

DWORD CInfoSkillBase::GetDerivationSize(void)
{
	return 1;
}


/* ========================================================================= */
/* �֐���	:CInfoSkillBase::GetDerivationWriteData							 */
/* ���e		:�h���f�[�^�̕ۑ��p�f�[�^���擾									 */
/* ���t		:2009/01/10														 */
/* ========================================================================= */

PBYTE CInfoSkillBase::GetDerivationWriteData(PDWORD pdwSize)
{
	PBYTE pRet;
	DWORD dwSize;

	dwSize	= GetDerivationSize ();
	pRet	= ZeroNew (dwSize);

	return pRet;
}


/* ========================================================================= */
/* �֐���	:CInfoSkillBase::ReadDerivationData								 */
/* ���e		:�h���f�[�^��ǂݍ���											 */
/* ���t		:2009/01/10														 */
/* ========================================================================= */

DWORD CInfoSkillBase::ReadDerivationData(PBYTE pSrc)
{
	return GetDerivationSize ();
}


/* ========================================================================= */
/* �֐���	:CInfoSkillBase::GetSendDataSize								 */
/* ���e		:���M�f�[�^�T�C�Y���擾											 */
/* ���t		:2008/12/03														 */
/* ========================================================================= */

DWORD CInfoSkillBase::GetSendDataSize(void)
{
	DWORD dwRet;

	dwRet = 0;
	dwRet += sizeof (m_dwSkillID);			/* �X�L��ID */
	dwRet += sizeof (m_dwSP);				/* ����SP */
	dwRet += sizeof (m_dwIconID);			/* �A�C�R���摜ID */
	dwRet += sizeof (m_nTypeMain);			/* �X�L�����(���C��) */
	dwRet += sizeof (m_nTypeSub);			/* �X�L�����(�T�u) */
	dwRet += sizeof (m_nUse);				/* �g�p���� */
	dwRet += (m_strName.GetLength () + 1);	/* �X�L���� */

	return dwRet;
}


/* ========================================================================= */
/* �֐���	:CInfoSkillBase::GetSendData									 */
/* ���e		:���M�f�[�^���擾												 */
/* ���t		:2008/12/03														 */
/* ========================================================================= */

PBYTE CInfoSkillBase::GetSendData(void)
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;

	dwSize	= GetSendDataSize ();
	pData	= ZeroNew (dwSize);

	pDataTmp = pData;

	CopyMemoryRenew (pDataTmp, &m_dwSkillID,	sizeof (m_dwSkillID), 	pDataTmp);	/* �X�L��ID */
	CopyMemoryRenew (pDataTmp, &m_dwSP,			sizeof (m_dwSP), 		pDataTmp);	/* ����SP */
	CopyMemoryRenew (pDataTmp, &m_dwIconID,		sizeof (m_dwIconID), 	pDataTmp);	/* �A�C�R���摜ID */
	CopyMemoryRenew (pDataTmp, &m_nTypeMain,	sizeof (m_nTypeMain),	pDataTmp);	/* �X�L�����(���C��) */
	CopyMemoryRenew (pDataTmp, &m_nTypeSub,		sizeof (m_nTypeSub),	pDataTmp);	/* �X�L�����(�T�u) */
	CopyMemoryRenew (pDataTmp, &m_nUse,			sizeof (m_nUse),	 	pDataTmp);	/* �g�p���� */
	strcpyRenew ((LPSTR)pDataTmp, m_strName, pDataTmp);								/* �X�L���� */

	return pData;
}


/* ========================================================================= */
/* �֐���	:CInfoSkillBase::SetSendData									 */
/* ���e		:���M�f�[�^�����荞��											 */
/* ���t		:2008/12/03														 */
/* ========================================================================= */

PBYTE CInfoSkillBase::SetSendData(PBYTE pSrc)
{
	PBYTE pRet, pDataTmp;

	pRet = pSrc;

	pDataTmp = pSrc;
	CopyMemoryRenew (&m_dwSkillID,	pDataTmp, sizeof (m_dwSkillID), pDataTmp);	/* �X�L��ID */
	CopyMemoryRenew (&m_dwSP,		pDataTmp, sizeof (m_dwSP), 		pDataTmp);	/* ����SP */
	CopyMemoryRenew (&m_dwIconID,	pDataTmp, sizeof (m_dwIconID), 	pDataTmp);	/* �A�C�R���摜ID */
	CopyMemoryRenew (&m_nTypeMain,	pDataTmp, sizeof (m_nTypeMain),	pDataTmp);	/* �X�L�����(���C��) */
	CopyMemoryRenew (&m_nTypeSub,	pDataTmp, sizeof (m_nTypeSub),	pDataTmp);	/* �X�L�����(�T�u) */
	CopyMemoryRenew (&m_nUse,		pDataTmp, sizeof (m_nUse),	 	pDataTmp);	/* �g�p���� */
	StoreRenew (m_strName, (LPCSTR)pDataTmp, pDataTmp);							/* �X�L���� */

	pRet = pDataTmp;
	return pRet;
}


/* ========================================================================= */
/* �֐���	:CInfoSkillBase::Copy											 */
/* ���e		:�R�s�[															 */
/* ���t		:2008/12/03														 */
/* ========================================================================= */

void CInfoSkillBase::Copy(CInfoSkillBase *pSrc)
{
	if (pSrc == NULL) {
		return;
	}
	m_dwSkillID	= pSrc->m_dwSkillID;	/* �X�L��ID */
	m_dwSP		= pSrc->m_dwSP;			/* ����SP */
	m_dwIconID	= pSrc->m_dwIconID;		/* �A�C�R���摜ID */
	m_nTypeMain	= pSrc->m_nTypeMain;	/* �X�L�����(���C��) */
	m_nTypeSub	= pSrc->m_nTypeSub;		/* �X�L�����(�T�u) */
	m_nUse		= pSrc->m_nUse;			/* �g�p���� */
	m_strName	= pSrc->m_strName;		/* �X�L���� */
}

/* Copyright(C)URARA-works 2008 */
