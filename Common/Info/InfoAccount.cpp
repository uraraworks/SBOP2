/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* �t�@�C����	:InfoAccount.cpp											 */
/* ���e			:�A�J�E���g�����N���X �����t�@�C��						 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2006/11/04													 */
/* ========================================================================= */

#include "stdafx.h"
#include "InfoAccount.h"

/* ========================================================================= */
/* �萔��`																	 */
/* ========================================================================= */

/* �w�b�_��� */
static LPCSTR s_aszName[] = {
	"m_bDisable",			/* ���O�C������ */
	"dwAccountID",			/* �A�J�E���gID */
	"dwCharID",				/* �L����ID */
	"dwTimeLastLogin",		/* �O��̃��O�C������ */
	"dwTimeMakeAccount",	/* �A�J�E���g�쐬���� */
	"dwLoginCount",			/* ���O�C���� */
	"adwCharID",			/* �L����ID�e�[�u�� */
	"nAdminLevel",			/* �Ǘ��҃��x�� */
	"strAccount",			/* �A�J�E���g */
	"strPassword",			/* �p�X���[�h */
	"m_strMacAddr",			/* �A�J�E���g�o�^MAC�A�h���X */
	NULL
};


/* ========================================================================= */
/* �֐���	:CInfoAccount::CInfoAccount										 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2006/11/04														 */
/* ========================================================================= */

CInfoAccount::CInfoAccount()
{
	m_bDisable			= FALSE;
	m_dwLastKeepalive	= 0;
	m_dwIP				= 0;
	m_dwAccountID		= 0;
	m_dwCharID			= 0;
	m_dwTimeLastLogin	= 0;
	m_dwTimeMakeAccount	= 0;
	m_dwLoginCount		= 0;

	m_nAdminLevel		= ADMINLEVEL_NONE;
	m_dwSessionID		= 0;

	for (m_nElementCount = 0; s_aszName[m_nElementCount] != NULL; m_nElementCount ++) {}
}


/* ========================================================================= */
/* �֐���	:CInfoAccount::~CInfoAccount									 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2006/11/04														 */
/* ========================================================================= */

CInfoAccount::~CInfoAccount()
{
}


/* ========================================================================= */
/* �֐���	:CInfoAccount::GetElementNo										 */
/* ���e		:�v�f�ԍ����擾													 */
/* ���t		:2007/04/30														 */
/* ========================================================================= */

int CInfoAccount::GetElementNo(LPCSTR pszName)
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
/* �֐���	:CInfoAccount::GetDataSize										 */
/* ���e		:�f�[�^�T�C�Y���擾												 */
/* ���t		:2007/04/30														 */
/* ========================================================================= */

DWORD CInfoAccount::GetDataSize(void)
{
	DWORD dwRet;

	dwRet = 0;
	dwRet += sizeof (m_bDisable);
	dwRet += sizeof (m_dwAccountID);
	dwRet += sizeof (m_dwCharID);
	dwRet += sizeof (m_dwTimeLastLogin);
	dwRet += sizeof (m_dwTimeMakeAccount);
	dwRet += sizeof (m_dwLoginCount);
	dwRet += ((m_adwCharID.GetSize () + 1) * sizeof (DWORD));
	dwRet += sizeof (m_nAdminLevel);
	dwRet += (m_strAccount.GetLength () + 1);
	dwRet += (m_strPassword.GetLength () + 1);
	dwRet += (m_strMacAddr.GetLength () + 1);

	return dwRet;
}


/* ========================================================================= */
/* �֐���	:CInfoAccount::GetDataSizeNo									 */
/* ���e		:�w��v�f�̃f�[�^�T�C�Y���擾									 */
/* ���t		:2007/04/30														 */
/* ========================================================================= */

DWORD CInfoAccount::GetDataSizeNo(int nNo)
{
	DWORD dwRet;

	dwRet = 0;

	switch (nNo) {
	case 0:	dwRet = sizeof (m_bDisable);								break;
	case 1:	dwRet = sizeof (m_dwAccountID);								break;
	case 2:	dwRet = sizeof (m_dwCharID);								break;
	case 3:	dwRet = sizeof (m_dwTimeLastLogin);							break;
	case 4:	dwRet = sizeof (m_dwTimeMakeAccount);						break;
	case 5:	dwRet = sizeof (m_dwLoginCount);							break;
	case 6:	dwRet = ((m_adwCharID.GetSize () + 1) * sizeof (DWORD));	break;
	case 7:	dwRet = sizeof (m_nAdminLevel);								break;
	case 8:	dwRet = (m_strAccount.GetLength () + 1);					break;
	case 9:	dwRet = (m_strPassword.GetLength () + 1);					break;
	case 10:dwRet = (m_strMacAddr.GetLength () + 1);					break;		/* �A�J�E���g�o�^MAC�A�h���X */
	}

	return dwRet;
}


/* ========================================================================= */
/* �֐���	:CInfoAccount::GetName											 */
/* ���e		:�v�f�����擾													 */
/* ���t		:2007/04/30														 */
/* ========================================================================= */

LPCSTR CInfoAccount::GetName(int nNo)
{
	return s_aszName[nNo];
}


/* ========================================================================= */
/* �֐���	:CInfoAccount::GetWriteData										 */
/* ���e		:�w��v�f�̕ۑ��p�f�[�^���擾									 */
/* ���t		:2007/04/30														 */
/* ========================================================================= */

PBYTE CInfoAccount::GetWriteData(int nNo, PDWORD pdwSize)
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
	case 0:	pSrc = (PBYTE)&m_bDisable;			break;
	case 1:	pSrc = (PBYTE)&m_dwAccountID;		break;
	case 2:	pSrc = (PBYTE)&m_dwCharID;			break;
	case 3:	pSrc = (PBYTE)&m_dwTimeLastLogin;	break;
	case 4:	pSrc = (PBYTE)&m_dwTimeMakeAccount;	break;
	case 5:	pSrc = (PBYTE)&m_dwLoginCount;		break;
	case 6:
		{
			int i, nCount;
			DWORD dwTmp;
			PBYTE pDataTmp;

			pDataTmp = pRet;
			ZeroMemory (pRet, dwSize);
			nCount = m_adwCharID.GetSize ();
			for (i = 0; i < nCount; i ++) {
				dwTmp = m_adwCharID[i];
				CopyMemoryRenew (pDataTmp, &dwTmp, sizeof (dwTmp), pDataTmp);
			}
		}
		break;
	case 7:	pSrc = (PBYTE)&m_nAdminLevel;			break;
	case 8:	pSrc = (PBYTE)(LPCSTR)m_strAccount;		break;
	case 9:	pSrc = (PBYTE)(LPCSTR)m_strPassword;	break;
	case 10:pSrc = (PBYTE)(LPCSTR)m_strMacAddr;		break;
	}

	if (pSrc) {
		CopyMemory (pRet, pSrc, dwSize);
	}

Exit:
	return pRet;
}


/* ========================================================================= */
/* �֐���	:CInfoAccount::ReadElementData									 */
/* ���e		:�w��v�f�f�[�^��ǂݍ���										 */
/* ���t		:2007/04/30														 */
/* ========================================================================= */

DWORD CInfoAccount::ReadElementData(
	PBYTE pSrc,		/* [in] �f�[�^�̓ǂݍ��݌� */
	int nNo)		/* [in] �v�f�ԍ� */
{
	PBYTE pDst;
	DWORD dwSize;

	pDst	= NULL;
	dwSize	= 0;

	switch (nNo) {
	case 0:	pDst = (PBYTE)&m_bDisable;			dwSize = sizeof (m_bDisable);			break;
	case 1:	pDst = (PBYTE)&m_dwAccountID;		dwSize = sizeof (m_dwAccountID);		break;
	case 2:	pDst = (PBYTE)&m_dwCharID;			dwSize = sizeof (m_dwCharID);			break;
	case 3:	pDst = (PBYTE)&m_dwTimeLastLogin;	dwSize = sizeof (m_dwTimeLastLogin);	break;
	case 4:	pDst = (PBYTE)&m_dwTimeMakeAccount;	dwSize = sizeof (m_dwTimeMakeAccount);	break;
	case 5:	pDst = (PBYTE)&m_dwLoginCount;		dwSize = sizeof (m_dwLoginCount);		break;
	case 6:
		{
			DWORD dwTmp;
			PBYTE pDataTmp;

			pDataTmp = pSrc;
			m_adwCharID.RemoveAll ();
			while (1) {
				dwSize += sizeof (DWORD);
				CopyMemoryRenew (&dwTmp, pDataTmp, sizeof (dwTmp), pDataTmp);
				if (dwTmp == 0) {
					break;
				}
				m_adwCharID.Add (dwTmp);
			}
		}
		break;
	case 7:	pDst = (PBYTE)&m_nAdminLevel;		dwSize = sizeof (m_nAdminLevel);		break;
	case 8:
		m_strAccount = (LPCSTR)pSrc;
		dwSize = m_strAccount.GetLength () + 1;
		break;
	case 9:
		m_strPassword = (LPCSTR)pSrc;
		dwSize = m_strPassword.GetLength () + 1;
		break;
	case 10:
		m_strMacAddr = (LPCSTR)pSrc;
		dwSize = m_strMacAddr.GetLength () + 1;
		break;
	}

	if (pDst) {
		CopyMemory (pDst, pSrc, dwSize);
	}

	return dwSize;
}


/* ========================================================================= */
/* �֐���	:CInfoAccount::GetSendDataSize									 */
/* ���e		:���M�f�[�^�T�C�Y���擾											 */
/* ���t		:2006/11/06														 */
/* ========================================================================= */

DWORD CInfoAccount::GetSendDataSize(void)
{
	DWORD dwRet;

	dwRet = 0;
	dwRet += sizeof (m_bDisable);
	dwRet += sizeof (m_dwAccountID);
	dwRet += sizeof (m_dwCharID);
	dwRet += sizeof (m_dwTimeLastLogin);
	dwRet += sizeof (m_dwTimeMakeAccount);
	dwRet += sizeof (m_dwLoginCount);
	dwRet += (m_adwCharID.GetSize () * sizeof (DWORD));
	dwRet += sizeof (DWORD);

	return dwRet;
}


/* ========================================================================= */
/* �֐���	:CInfoAccount::GetSendData										 */
/* ���e		:���M�f�[�^���擾												 */
/* ���t		:2006/11/06														 */
/* ========================================================================= */

PBYTE CInfoAccount::GetSendData(void)
{
	int i, nCount;
	PBYTE pRet, pData, pDataTmp;
	DWORD dwSize, dwTmp;

	pRet = NULL;

	dwSize		= GetSendDataSize ();
	pData		= ZeroNew (dwSize);
	pDataTmp	= pData;

	CopyMemoryRenew (pDataTmp, &m_dwAccountID,			sizeof (m_dwAccountID),			pDataTmp);	/* �A�J�E���gID */
	CopyMemoryRenew (pDataTmp, &m_dwCharID,				sizeof (m_dwCharID),			pDataTmp);	/* �g�p���̃L����ID */
	CopyMemoryRenew (pDataTmp, &m_dwTimeLastLogin,		sizeof (m_dwTimeLastLogin),		pDataTmp);	/* �O��̃��O�C������ */
	CopyMemoryRenew (pDataTmp, &m_dwTimeMakeAccount,	sizeof (m_dwTimeMakeAccount),	pDataTmp);	/* �A�J�E���g�쐬���� */
	CopyMemoryRenew (pDataTmp, &m_dwLoginCount,			sizeof (m_dwLoginCount),		pDataTmp);	/* ���O�C���� */
	nCount = m_adwCharID.GetSize ();
	for (i = 0; i < nCount; i ++) {
		dwTmp = m_adwCharID[i];
		CopyMemoryRenew (pDataTmp, &dwTmp, sizeof (dwTmp), pDataTmp);	/* �L����ID */
	}

	pRet = pData;
	return pRet;
}


/* ========================================================================= */
/* �֐���	:CInfoAccount::SetSendData										 */
/* ���e		:���M�f�[�^��ݒ�												 */
/* ���t		:2006/11/06														 */
/* �߂�l	:����������̃A�h���X											 */
/* ========================================================================= */

PBYTE CInfoAccount::SetSendData(PBYTE pSrc)
{
	PBYTE pRet, pDataTmp;
	DWORD dwTmp;

	pRet = pSrc;

	m_adwCharID.RemoveAll ();

	pDataTmp = pSrc;
	CopyMemoryRenew (&m_dwAccountID,		pDataTmp, sizeof (m_dwAccountID), 		pDataTmp);	/* �A�J�E���gID */
	CopyMemoryRenew (&m_dwCharID,			pDataTmp, sizeof (m_dwCharID),			pDataTmp);	/* �g�p���̃L����ID */
	CopyMemoryRenew (&m_dwTimeLastLogin,	pDataTmp, sizeof (m_dwTimeLastLogin),	pDataTmp);	/* �O��̃��O�C������ */
	CopyMemoryRenew (&m_dwTimeMakeAccount,	pDataTmp, sizeof (m_dwTimeMakeAccount),	pDataTmp);	/* �A�J�E���g�쐬���� */
	CopyMemoryRenew (&m_dwLoginCount,		pDataTmp, sizeof (m_dwLoginCount),		pDataTmp);	/* ���O�C���� */

	while (1) {
		CopyMemoryRenew (&dwTmp, pDataTmp, sizeof (dwTmp), pDataTmp);	/* �L����ID */
		if (dwTmp == 0) {
			break;
		}
		m_adwCharID.Add (dwTmp);
	}

	pRet = pDataTmp;
	return pRet;
}


/* ========================================================================= */
/* �֐���	:CInfoAccount::GetCharCount										 */
/* ���e		:�L���������擾													 */
/* ���t		:2007/03/21														 */
/* ========================================================================= */

int CInfoAccount::GetCharCount(void)
{
	return m_adwCharID.GetSize ();
}


/* ========================================================================= */
/* �֐���	:CInfoAccount::Copy												 */
/* ���e		:�R�s�[															 */
/* ���t		:2006/11/08														 */
/* ========================================================================= */

void CInfoAccount::Copy(CInfoAccount *pSrc)
{
	m_dwAccountID		= pSrc->m_dwAccountID;
	m_dwCharID			= pSrc->m_dwCharID;
	m_dwTimeLastLogin	= pSrc->m_dwTimeLastLogin;
	m_dwTimeMakeAccount	= pSrc->m_dwTimeMakeAccount;
	m_dwLoginCount		= pSrc->m_dwLoginCount;
	m_adwCharID.Copy (&pSrc->m_adwCharID);

	m_dwSessionID		= pSrc->m_dwSessionID;
	m_strAccount		= pSrc->m_strAccount;
	m_strPassword		= pSrc->m_strPassword;
	m_strMacAddr		= pSrc->m_strMacAddr;
}


/* ========================================================================= */
/* �֐���	:CInfoAccount::GetTmpData										 */
/* ���e		:�f�[�^�L���b�V���p�Ɏ擾										 */
/* ���t		:2008/06/07														 */
/* ========================================================================= */

PBYTE CInfoAccount::GetTmpData(DWORD &dwDataSize)
{
	PBYTE pRet, pDataTmp;

	pRet = NULL;
	dwDataSize = 0;

	/* �T�C�Y���v�Z */
	dwDataSize += sizeof (m_dwAccountID);
	dwDataSize += sizeof (m_dwIP);
	dwDataSize += (m_strAccount.GetLength () + 1);
	dwDataSize += (m_strPassword.GetLength () + 1);
	dwDataSize += (m_strLastMacAddr.GetLength () + 1);

	pRet = ZeroNew (dwDataSize);
	pDataTmp = pRet;
	CopyMemoryRenew (pDataTmp, &m_dwAccountID, sizeof (m_dwAccountID), pDataTmp);	/* �A�J�E���gID */
	CopyMemoryRenew (pDataTmp, &m_dwIP, sizeof (m_dwIP), pDataTmp);					/* IP�A�h���X */
	strcpyRenew ((LPSTR)pDataTmp, m_strAccount,  pDataTmp);							/* �A�J�E���g */
	strcpyRenew ((LPSTR)pDataTmp, m_strPassword, pDataTmp);							/* �p�X���[�h */
	strcpyRenew ((LPSTR)pDataTmp, m_strLastMacAddr, pDataTmp);						/* ���O�C������MAC�A�h���X */

	return pRet;
}


/* ========================================================================= */
/* �֐���	:CInfoAccount::SetTmpData										 */
/* ���e		:�f�[�^�L���b�V������ݒ�										 */
/* ���t		:2008/06/07														 */
/* ========================================================================= */

void CInfoAccount::SetTmpData(PBYTE pSrc)
{
	PBYTE pDataTmp;

	pDataTmp = pSrc;
	CopyMemoryRenew (&m_dwAccountID, pDataTmp, sizeof (m_dwAccountID), pDataTmp);	/* �A�J�E���gID */
	CopyMemoryRenew (&m_dwIP, pDataTmp, sizeof (m_dwIP), pDataTmp);					/* IP�A�h���X */
	StoreRenew (m_strAccount,	(LPCSTR)pDataTmp, pDataTmp);						/* �A�J�E���g */
	StoreRenew (m_strPassword,	(LPCSTR)pDataTmp, pDataTmp);						/* �p�X���[�h */
	StoreRenew (m_strLastMacAddr,	(LPCSTR)pDataTmp, pDataTmp);					/* ���O�C������MAC�A�h���X */
}

/* Copyright(C)URARA-works 2006 */
