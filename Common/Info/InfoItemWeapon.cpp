/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:InfoItemWeapon.cpp											 */
/* ���e			:�A�C�e��������N���X �����t�@�C��						 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/08/10													 */
/* ========================================================================= */

#include "stdafx.h"
#include "InfoItemWeapon.h"

/* ========================================================================= */
/* �萔��`																	 */
/* ========================================================================= */

/* �w�b�_��� */
static LPCSTR s_aszName[] = {
	"m_dwWeaponInfoID",			/* ������ID */
	"m_dwMotionType",			/* �g�p�\�ȍU�����[�V���� */
	"m_dwMotionTypeStand",		/* �퓬���[�h���̗������[�V���� */
	"m_dwMotionTypeWalk",		/* �퓬���[�h���̂��葫���[�V���� */
	"m_strName",				/* �����ʖ� */
	"m_adwEffectIDAtack",		/* �ʏ�U�����̃G�t�F�N�gID */
	"m_adwEffectIDCritical",	/* �N���e�B�J�����̃G�t�F�N�gID */
	NULL
};


/* ========================================================================= */
/* �֐���	:CInfoItemWeapon::CInfoItemWeapon								 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/08/10														 */
/* ========================================================================= */

CInfoItemWeapon::CInfoItemWeapon()
{
	m_dwWeaponInfoID	= 0;
	m_dwMotionType		= 0;
	m_dwMotionTypeStand	= 0;
	m_dwMotionTypeWalk	= 0;

	for (m_nElementCount = 0; s_aszName[m_nElementCount] != NULL; m_nElementCount ++) {}
}


/* ========================================================================= */
/* �֐���	:CInfoItemWeapon::~CInfoItemWeapon								 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/08/10														 */
/* ========================================================================= */

CInfoItemWeapon::~CInfoItemWeapon()
{
}


/* ========================================================================= */
/* �֐���	:CInfoItemWeapon::GetElementNo									 */
/* ���e		:�v�f�ԍ����擾													 */
/* ���t		:2008/08/10														 */
/* ========================================================================= */

int CInfoItemWeapon::GetElementNo(LPCSTR pszName)
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
/* �֐���	:CInfoItemWeapon::GetDataSize									 */
/* ���e		:�f�[�^�T�C�Y���擾												 */
/* ���t		:2008/08/10														 */
/* ========================================================================= */

DWORD CInfoItemWeapon::GetDataSize(void)
{
	DWORD dwRet;

	dwRet = 0;
	dwRet += sizeof (m_dwWeaponInfoID);									/* ������ID */
	dwRet += sizeof (m_dwMotionType);									/* �g�p�\�ȍU�����[�V���� */
	dwRet += sizeof (m_dwMotionTypeStand);								/* �퓬���[�h���̗������[�V���� */
	dwRet += sizeof (m_dwMotionTypeWalk);								/* �퓬���[�h���̂��葫���[�V���� */
	dwRet += (m_strName.GetLength () + 1);								/* �����ʖ� */
	dwRet += ((m_adwEffectIDAtack.GetSize () + 1) * sizeof (DWORD));	/* �ʏ�U�����̃G�t�F�N�gID */
	dwRet += ((m_adwEffectIDCritical.GetSize () + 1) * sizeof (DWORD));	/* �N���e�B�J�����̃G�t�F�N�gID */

	return dwRet;
}


/* ========================================================================= */
/* �֐���	:CInfoItemWeapon::GetDataSizeNo									 */
/* ���e		:�w��v�f�̃f�[�^�T�C�Y���擾									 */
/* ���t		:2008/08/10														 */
/* ========================================================================= */

DWORD CInfoItemWeapon::GetDataSizeNo(int nNo)
{
	DWORD dwRet;

	dwRet = 0;

	switch (nNo) {
	case 0:		dwRet = sizeof (m_dwWeaponInfoID);									break;	/* ������ID */
	case 1:		dwRet = sizeof (m_dwMotionType);									break;	/* �g�p�\�ȍU�����[�V���� */
	case 2:		dwRet = sizeof (m_dwMotionTypeStand);								break;	/* �퓬���[�h���̗������[�V���� */
	case 3:		dwRet = sizeof (m_dwMotionTypeWalk);								break;	/* �퓬���[�h���̂��葫���[�V���� */
	case 4:		dwRet = (m_strName.GetLength () + 1);								break;	/* �����ʖ� */
	case 5:		dwRet = ((m_adwEffectIDAtack.GetSize () + 1) * sizeof (DWORD));		break;	/* �ʏ�U�����̃G�t�F�N�gID�� */
	case 6:		dwRet = ((m_adwEffectIDCritical.GetSize () + 1) * sizeof (DWORD));	break;	/* �N���e�B�J�����̃G�t�F�N�gID�� */

	}

	return dwRet;
}


/* ========================================================================= */
/* �֐���	:CInfoItemWeapon::GetName										 */
/* ���e		:�v�f�����擾													 */
/* ���t		:2008/08/10														 */
/* ========================================================================= */

LPCSTR CInfoItemWeapon::GetName(int nNo)
{
	return s_aszName[nNo];
}


/* ========================================================================= */
/* �֐���	:CInfoItemWeapon::GetWriteData									 */
/* ���e		:�w��v�f�̕ۑ��p�f�[�^���擾									 */
/* ���t		:2008/08/10														 */
/* ========================================================================= */

PBYTE CInfoItemWeapon::GetWriteData(int nNo, PDWORD pdwSize)
{
	int i, nCount;
	PBYTE pRet, pSrc, pTmp;
	DWORD dwSize, dwTmp;

	pRet		= NULL;
	pSrc		= NULL;
	dwSize		= GetDataSizeNo (nNo);
	*pdwSize	= dwSize;

	if (dwSize == 0) {
		goto Exit;
	}
	pRet = new BYTE[dwSize];

	switch (nNo) {
	case 0:		pSrc = (PBYTE)&m_dwWeaponInfoID;			break;	/* ������ID */
	case 1:		pSrc = (PBYTE)&m_dwMotionType;				break;	/* �g�p�\�ȍU�����[�V���� */
	case 2:		pSrc = (PBYTE)&m_dwMotionTypeStand;			break;	/* �퓬���[�h���̗������[�V���� */
	case 3:		pSrc = (PBYTE)&m_dwMotionTypeWalk;			break;	/* �퓬���[�h���̂��葫���[�V���� */
	case 4:		pSrc = (PBYTE)(LPCSTR)m_strName;			break;	/* �����ʖ� */
	case 5:		/* �ʏ�U�����̃G�t�F�N�gID */
		pTmp	= pRet;
		nCount	= m_adwEffectIDAtack.GetSize ();

		for (i = 0; i < nCount; i ++) {
			dwTmp = m_adwEffectIDAtack[i];
			CopyMemoryRenew (pTmp, &dwTmp, sizeof (DWORD), pTmp);
		}
		/* �I�[�p */
		dwTmp = 0;
		CopyMemoryRenew (pTmp, &dwTmp, sizeof (DWORD), pTmp);
		break;
	case 6:		/* �N���e�B�J�����̃G�t�F�N�gID */
		pTmp	= pRet;
		nCount	= m_adwEffectIDCritical.GetSize ();

		for (i = 0; i < nCount; i ++) {
			dwTmp = m_adwEffectIDCritical[i];
			CopyMemoryRenew (pTmp, &dwTmp, sizeof (DWORD), pTmp);
		}
		/* �I�[�p */
		dwTmp = 0;
		CopyMemoryRenew (pTmp, &dwTmp, sizeof (DWORD), pTmp);
		break;
	}

	if (pSrc) {
		CopyMemory (pRet, pSrc, dwSize);
	}

Exit:
	return pRet;
}


/* ========================================================================= */
/* �֐���	:CInfoItemWeapon::ReadElementData								 */
/* ���e		:�w��v�f�f�[�^��ǂݍ���										 */
/* ���t		:2008/08/10														 */
/* ========================================================================= */

DWORD CInfoItemWeapon::ReadElementData(
	PBYTE pSrc,		/* [in] �f�[�^�̓ǂݍ��݌� */
	int nNo)		/* [in] �v�f�ԍ� */
{
	PBYTE pDst, pTmp;
	DWORD dwSize, dwTmp;

	pDst	= NULL;
	dwSize	= GetDataSizeNo (nNo);

	switch (nNo) {
	case 0:		pDst = (PBYTE)&m_dwWeaponInfoID;			break;	/* ������ID */
	case 1:		pDst = (PBYTE)&m_dwMotionType;				break;	/* �g�p�\�ȍU�����[�V���� */
	case 2:		pDst = (PBYTE)&m_dwMotionTypeStand;			break;	/* �퓬���[�h���̗������[�V���� */
	case 3:		pDst = (PBYTE)&m_dwMotionTypeWalk;			break;	/* �퓬���[�h���̂��葫���[�V���� */
	case 4:		/* �����ʖ� */
		m_strName = (LPCSTR)pSrc;
		dwSize = m_strName.GetLength () + 1;
		break;
	case 5:		/* �ʏ�U�����̃G�t�F�N�gID */
		pTmp	= pSrc;
		dwTmp	= 0;

		while (1) {
			CopyMemoryRenew (&dwTmp, pTmp, sizeof (DWORD), pTmp);
			if (dwTmp == 0) {
				break;
			}
			m_adwEffectIDAtack.Add (dwTmp);
		}
		dwSize = (m_adwEffectIDAtack.GetSize () + 1) * sizeof (DWORD);
		break;
	case 6:		/* �N���e�B�J�����̃G�t�F�N�gID */
		pTmp	= pSrc;
		dwTmp	= 0;

		while (1) {
			CopyMemoryRenew (&dwTmp, pTmp, sizeof (DWORD), pTmp);
			if (dwTmp == 0) {
				break;
			}
			m_adwEffectIDCritical.Add (dwTmp);
		}
		dwSize = (m_adwEffectIDCritical.GetSize () + 1) * sizeof (DWORD);
		break;
	}

	if (pDst) {
		CopyMemory (pDst, pSrc, dwSize);
	}

	return dwSize;
}


/* ========================================================================= */
/* �֐���	:CInfoItemWeapon::GetSendDataSize								 */
/* ���e		:���M�f�[�^�T�C�Y���擾											 */
/* ���t		:2008/08/10														 */
/* ========================================================================= */

DWORD CInfoItemWeapon::GetSendDataSize(void)
{
	return GetDataSize ();
}


/* ========================================================================= */
/* �֐���	:CInfoItemWeapon::GetSendData									 */
/* ���e		:���M�f�[�^���擾												 */
/* ���t		:2008/08/10														 */
/* ========================================================================= */

PBYTE CInfoItemWeapon::GetSendData(void)
{
	int i, nCount;
	PBYTE pData, pDataTmp;
	DWORD dwSize, dwTmp;

	dwSize	= GetSendDataSize ();
	pData	= ZeroNew (dwSize);

	pDataTmp = pData;

	CopyMemoryRenew (pDataTmp, &m_dwWeaponInfoID,		sizeof (m_dwWeaponInfoID),		pDataTmp);	/* ������ID */
	CopyMemoryRenew (pDataTmp, &m_dwMotionType,			sizeof (m_dwMotionType),		pDataTmp);	/* �g�p�\�ȍU�����[�V���� */
	CopyMemoryRenew (pDataTmp, &m_dwMotionTypeStand,	sizeof (m_dwMotionTypeStand),	pDataTmp);	/* �퓬���[�h���̗������[�V���� */
	CopyMemoryRenew (pDataTmp, &m_dwMotionTypeWalk,		sizeof (m_dwMotionTypeWalk),	pDataTmp);	/* �퓬���[�h���̂��葫���[�V���� */
	strcpyRenew ((LPSTR)pDataTmp, (LPCSTR)m_strName, pDataTmp);										/* �����ʖ� */

	/* �ʏ�U�����̃G�t�F�N�gID */
	nCount = m_adwEffectIDAtack.GetSize ();
	for (i = 0; i < nCount; i ++) {
		dwTmp = m_adwEffectIDAtack[i];
		CopyMemoryRenew (pDataTmp, &dwTmp, sizeof (dwTmp), pDataTmp);
	}
	/* �I�[�p */
	dwTmp = 0;
	CopyMemoryRenew (pDataTmp, &dwTmp, sizeof (dwTmp), pDataTmp);

	/* �N���e�B�J�����̃G�t�F�N�gID */
	nCount = m_adwEffectIDCritical.GetSize ();
	for (i = 0; i < nCount; i ++) {
		dwTmp = m_adwEffectIDCritical[i];
		CopyMemoryRenew (pDataTmp, &dwTmp, sizeof (dwTmp), pDataTmp);
	}
	/* �I�[�p */
	dwTmp = 0;
	CopyMemoryRenew (pDataTmp, &dwTmp, sizeof (dwTmp), pDataTmp);

	return pData;
}


/* ========================================================================= */
/* �֐���	:CInfoItemWeapon::SetSendData									 */
/* ���e		:���M�f�[�^�����荞��											 */
/* ���t		:2008/08/10														 */
/* ========================================================================= */

PBYTE CInfoItemWeapon::SetSendData(PBYTE pSrc)
{
	PBYTE pRet, pDataTmp;
	DWORD dwTmp;

	pRet = pSrc;

	pDataTmp = pSrc;

	m_adwEffectIDAtack.RemoveAll ();
	m_adwEffectIDCritical.RemoveAll ();

	CopyMemoryRenew (&m_dwWeaponInfoID,		pDataTmp, sizeof (m_dwWeaponInfoID),	pDataTmp);	/* ������ID */
	CopyMemoryRenew (&m_dwMotionType,		pDataTmp, sizeof (m_dwMotionType),		pDataTmp);	/* �g�p�\�ȍU�����[�V���� */
	CopyMemoryRenew (&m_dwMotionTypeStand,	pDataTmp, sizeof (m_dwMotionTypeStand),	pDataTmp);	/* �퓬���[�h���̗������[�V���� */
	CopyMemoryRenew (&m_dwMotionTypeWalk,	pDataTmp, sizeof (m_dwMotionTypeWalk),	pDataTmp);	/* �퓬���[�h���̂��葫���[�V���� */
	StoreRenew (m_strName, (LPCSTR)pDataTmp, pDataTmp);											/* �����ʖ� */

	/* �ʏ�U�����̃G�t�F�N�gID */
	while (1) {
		CopyMemoryRenew (&dwTmp, pDataTmp, sizeof (dwTmp), pDataTmp);
		if (dwTmp == 0) {
			break;
		}
		m_adwEffectIDAtack.Add (dwTmp);
	}

	/* �N���e�B�J�����̃G�t�F�N�gID */
	while (1) {
		CopyMemoryRenew (&dwTmp, pDataTmp, sizeof (dwTmp), pDataTmp);
		if (dwTmp == 0) {
			break;
		}
		m_adwEffectIDCritical.Add (dwTmp);
	}

	pRet = pDataTmp;
	return pRet;
}


/* ========================================================================= */
/* �֐���	:CInfoItemWeapon::Copy											 */
/* ���e		:�R�s�[															 */
/* ���t		:2008/08/10														 */
/* ========================================================================= */

void CInfoItemWeapon::Copy(CInfoItemWeapon *pSrc)
{
	int i, nCount;

	if (pSrc == NULL) {
		return;
	}
	m_dwWeaponInfoID		= pSrc->m_dwWeaponInfoID;		/* ������ID */
	m_dwMotionType			= pSrc->m_dwMotionType;			/* �g�p�\�ȍU�����[�V���� */
	m_dwMotionTypeStand		= pSrc->m_dwMotionTypeStand;	/* �퓬���[�h���̗������[�V���� */
	m_dwMotionTypeWalk		= pSrc->m_dwMotionTypeWalk;		/* �퓬���[�h���̂��葫���[�V���� */
	m_strName				= pSrc->m_strName;				/* �����ʖ� */

	/* �ʏ�U�����̃G�t�F�N�gID */
	m_adwEffectIDAtack.RemoveAll ();
	nCount = pSrc->m_adwEffectIDAtack.GetSize ();
	for (i = 0; i < nCount; i ++) {
		m_adwEffectIDAtack.Add (pSrc->m_adwEffectIDAtack[i]);
	}

	/* �N���e�B�J�����̃G�t�F�N�gID */
	m_adwEffectIDCritical.RemoveAll ();
	nCount = pSrc->m_adwEffectIDCritical.GetSize ();
	for (i = 0; i < nCount; i ++) {
		m_adwEffectIDCritical.Add (pSrc->m_adwEffectIDCritical[i]);
	}
}

/* Copyright(C)URARA-works 2008 */
