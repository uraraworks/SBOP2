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
	"m_nHitType",		/* �����蔻���� */
	"m_nHitDirection",	/* ������� */
	"m_dwMapEventID",	/* �}�b�v�C�x���gID */
	"m_ptPos",			/* ���W1 */
	"m_ptPos2",			/* ���W2 */
	"m_dwSoundID",		/* ���s���̌��ʉ�ID */
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
	m_nHitType		= MAPEVENTHITTYPE_MAPPOS;
	m_nHitDirection	= -1;
	m_dwSoundID		= 0;
	ZeroMemory (&m_ptPos,  sizeof (m_ptPos));
	ZeroMemory (&m_ptPos2, sizeof (m_ptPos2));

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

void CInfoMapEventBase::RenewSize(int nDirection, int nSize, SIZE *pSize)
{
	switch (nDirection) {
	case 0:
		m_ptPos. y += nSize;
		m_ptPos2.y += nSize;
		break;
	case 2:
		m_ptPos. x += nSize;
		m_ptPos2.x += nSize;
		break;
	}

	/* ���W�̒��� */
	m_ptPos.x = max (0, m_ptPos.x);
	m_ptPos.x = min (m_ptPos.x, pSize->cx - 1);
	m_ptPos.y = max (0, m_ptPos.y);
	m_ptPos.y = min (m_ptPos.y, pSize->cy - 1);
	m_ptPos2.x = max (0, m_ptPos2.x);
	m_ptPos2.x = min (m_ptPos2.x, pSize->cx - 1);
	m_ptPos2.y = max (0, m_ptPos2.y);
	m_ptPos2.y = min (m_ptPos2.y, pSize->cy - 1);
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
	dwRet += sizeof (m_nType);			/* �C�x���g��� */
	dwRet += sizeof (m_nHitType);		/* �����蔻���� */
	dwRet += sizeof (m_nHitDirection);	/* ������� */
	dwRet += sizeof (m_dwMapEventID);	/* �}�b�v�C�x���gID */
	dwRet += sizeof (m_ptPos);			/* ���W1 */
	dwRet += sizeof (m_ptPos2);			/* ���W2 */
	dwRet += sizeof (m_dwSoundID);		/* ���s���̌��ʉ�ID */

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
	case 0:	dwRet = sizeof (m_nType);			break;	/* �C�x���g��� */
	case 1:	dwRet = sizeof (m_nHitType);		break;	/* �����蔻���� */
	case 2:	dwRet = sizeof (m_nHitDirection);	break;	/* ������� */
	case 3:	dwRet = sizeof (m_dwMapEventID);	break;	/* �}�b�v�C�x���gID */
	case 4:	dwRet = sizeof (m_ptPos);			break;	/* ���W1 */
	case 5:	dwRet = sizeof (m_ptPos2);			break;	/* ���W2 */
	case 6:	dwRet = sizeof (m_dwSoundID);		break;	/* ���s���̌��ʉ�ID */
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
	case 0:	pSrc = (PBYTE)&m_nType;			break;	/* �C�x���g��� */
	case 1:	pSrc = (PBYTE)&m_nHitType;		break;	/* �����蔻���� */
	case 2:	pSrc = (PBYTE)&m_nHitDirection;	break;	/* ������� */
	case 3:	pSrc = (PBYTE)&m_dwMapEventID;	break;	/* �}�b�v�C�x���gID */
	case 4:	pSrc = (PBYTE)&m_ptPos;			break;	/* ���W1 */
	case 5:	pSrc = (PBYTE)&m_ptPos2;		break;	/* ���W2 */
	case 6:	pSrc = (PBYTE)&m_dwSoundID;		break;	/* ���s���̌��ʉ�ID */
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
	case 0: pDst = (PBYTE)&m_nType;			dwSize = sizeof (m_nType);			break;	/* �C�x���g��� */
	case 1: pDst = (PBYTE)&m_nHitType;		dwSize = sizeof (m_nHitType);		break;	/* �����蔻���� */
	case 2: pDst = (PBYTE)&m_nHitDirection;	dwSize = sizeof (m_nHitDirection);	break;	/* ������� */
	case 3: pDst = (PBYTE)&m_dwMapEventID;	dwSize = sizeof (m_dwMapEventID);	break;	/* �}�b�v�C�x���gID */
	case 4:	pDst = (PBYTE)&m_ptPos;			dwSize = sizeof (m_ptPos);			break;	/* ���W1 */
	case 5:	pDst = (PBYTE)&m_ptPos2;		dwSize = sizeof (m_ptPos2);			break;	/* ���W2 */
	case 6:	pDst = (PBYTE)&m_dwSoundID;		dwSize = sizeof (m_dwSoundID);		break;	/* ���s���̌��ʉ�ID */
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

	dwRet = sizeof (m_nType)		+	/* �C�x���g��� */
			sizeof (m_nHitType)		+	/* �����蔻���� */
			sizeof (m_nHitDirection)+	/* ������� */
			sizeof (m_dwMapEventID)	+	/* �}�b�v�C�x���gID */
			sizeof (m_ptPos)		+	/* ���W1 */
			sizeof (m_ptPos2)		+	/* ���W2 */
			sizeof (m_dwSoundID);		/* ���s���̌��ʉ�ID */

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

	CopyMemoryRenew (pDataTmp, &m_nType,		sizeof (m_nType), 			pDataTmp);	/* �C�x���g��� */
	CopyMemoryRenew (pDataTmp, &m_nHitType,		sizeof (m_nHitType), 		pDataTmp);	/* �����蔻���� */
	CopyMemoryRenew (pDataTmp, &m_nHitDirection,sizeof (m_nHitDirection), 	pDataTmp);	/* ������� */
	CopyMemoryRenew (pDataTmp, &m_dwMapEventID,	sizeof (m_dwMapEventID), 	pDataTmp);	/* �}�b�v�C�x���gID */
	CopyMemoryRenew (pDataTmp, &m_ptPos,		sizeof (m_ptPos),			pDataTmp);	/* ���W1 */
	CopyMemoryRenew (pDataTmp, &m_ptPos2,		sizeof (m_ptPos2),			pDataTmp);	/* ���W2 */
	CopyMemoryRenew (pDataTmp, &m_dwSoundID,	sizeof (m_dwSoundID),		pDataTmp);	/* ���s���̌��ʉ�ID */

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
	CopyMemoryRenew (&m_nType,			pDataTmp, sizeof (m_nType), 		pDataTmp);	/* �C�x���g��� */
	CopyMemoryRenew (&m_nHitType,		pDataTmp, sizeof (m_nHitType), 		pDataTmp);	/* �����蔻���� */
	CopyMemoryRenew (&m_nHitDirection,	pDataTmp, sizeof (m_nHitDirection), pDataTmp);	/* ������� */
	CopyMemoryRenew (&m_dwMapEventID,	pDataTmp, sizeof (m_dwMapEventID), 	pDataTmp);	/* �}�b�v�C�x���gID */
	CopyMemoryRenew (&m_ptPos,			pDataTmp, sizeof (m_ptPos),			pDataTmp);	/* ���W1 */
	CopyMemoryRenew (&m_ptPos2,			pDataTmp, sizeof (m_ptPos2),		pDataTmp);	/* ���W2 */
	CopyMemoryRenew (&m_dwSoundID,		pDataTmp, sizeof (m_dwSoundID),		pDataTmp);	/* ���s���̌��ʉ�ID */

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
	m_nType			= pSrc->m_nType;		/* �C�x���g��� */
	m_nHitType		= pSrc->m_nHitType;		/* �����蔻���� */
	m_nHitDirection	= pSrc->m_nHitDirection;/* ������� */
	m_dwMapEventID	= pSrc->m_dwMapEventID;	/* �}�b�v�C�x���gID */
	m_ptPos			= pSrc->m_ptPos;		/* ���W1 */
	m_ptPos2		= pSrc->m_ptPos2;		/* ���W2 */
	m_dwSoundID		= pSrc->m_dwSoundID;	/* ���s���̌��ʉ�ID */
}

/* Copyright(C)URARA-works 2007 */
