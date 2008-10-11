/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* �t�@�C����	:LibInfoAccount.cpp											 */
/* ���e			:�A�J�E���g�����N���X �����t�@�C��						 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2006/11/05													 */
/* ========================================================================= */

#include "stdafx.h"
#include "LibInfoAccount.h"


/* ========================================================================= */
/* �֐���	:CLibInfoAccount::CLibInfoAccount								 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2006/11/05														 */
/* ========================================================================= */

CLibInfoAccount::CLibInfoAccount()
{
	m_paInfo = NULL;
}


/* ========================================================================= */
/* �֐���	:CLibInfoAccount::~CLibInfoAccount								 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2006/11/05														 */
/* ========================================================================= */

CLibInfoAccount::~CLibInfoAccount()
{
	Destroy ();
}


/* ========================================================================= */
/* �֐���	:CLibInfoAccount::Create										 */
/* ���e		:�쐬															 */
/* ���t		:2006/11/05														 */
/* ========================================================================= */

void CLibInfoAccount::Create(void)
{
	m_paInfo = new ARRAYINFOACCOUNT;
}


/* ========================================================================= */
/* �֐���	:CLibInfoAccount::Destroy										 */
/* ���e		:�j��															 */
/* ���t		:2006/11/05														 */
/* ========================================================================= */

void CLibInfoAccount::Destroy(void)
{
	DeleteAll ();
	SAFE_DELETE (m_paInfo);
}


/* ========================================================================= */
/* �֐���	:CLibInfoAccount::CheckPassword									 */
/* ���e		:�p�X���[�h�̃`�F�b�N											 */
/* ���t		:2006/11/05														 */
/* ========================================================================= */

BOOL CLibInfoAccount::CheckPassword(LPCSTR pszAccount, LPCSTR pszPassword)
{
	BOOL bRet;
	PCInfoAccount pInfoAccount;

	bRet = FALSE;

	pInfoAccount = GetPtr (pszAccount);
	if (pInfoAccount == NULL) {
		goto Exit;
	}

	if (pInfoAccount->m_strPassword != pszPassword) {
		goto Exit;
	}

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CLibInfoAccount::IsUseMacAddr									 */
/* ���e		:MAC�A�h���X���g�p�ς݂��`�F�b�N								 */
/* �߂�l	:TRUE:�g�p�ς�													 */
/* ���t		:2008/07/20														 */
/* ========================================================================= */

BOOL CLibInfoAccount::IsUseMacAddr(LPCSTR pszMacAddr)
{
	BOOL bRet;
	int i, nCount;
	PCInfoAccount pInfoAccount;

	bRet = FALSE;

	if (strcmp (pszMacAddr, "00-00-00-00-00-00") == 0) {
		return FALSE;
	}

	nCount = GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfoAccount = (PCInfoAccount)GetPtr (i);
		if (pInfoAccount->m_strMacAddr == pszMacAddr) {
			break;
		}
	}
	if (i >= nCount) {
		goto Exit;
	}

	bRet = TRUE;
Exit:
	return bRet;
}


/* ========================================================================= */
/* �֐���	:CLibInfoAccount::GetNew										 */
/* ���e		:�V�K�f�[�^���擾												 */
/* ���t		:2006/11/05														 */
/* ========================================================================= */

PCInfoBase CLibInfoAccount::GetNew(void)
{
	return (PCInfoBase)new CInfoAccount;
}


/* ========================================================================= */
/* �֐���	:CLibInfoAccount::GetCount										 */
/* ���e		:�f�[�^�����擾													 */
/* ���t		:2006/11/05														 */
/* ========================================================================= */

int CLibInfoAccount::GetCount(void)
{
	int nRet;

	nRet = 0;

	if (m_paInfo == NULL) {
		goto Exit;
	}

	nRet = m_paInfo->GetSize ();
Exit:
	return nRet;
}


/* ========================================================================= */
/* �֐���	:CLibInfoAccount::Add											 */
/* ���e		:�ǉ�															 */
/* ���t		:2006/11/05														 */
/* ========================================================================= */

void CLibInfoAccount::Add(PCInfoBase pInfo)
{
	PCInfoAccount pAccountInfo;

	pAccountInfo = (PCInfoAccount)pInfo;
	if (pAccountInfo->m_dwAccountID == 0) {
		pAccountInfo->m_dwAccountID = GetNewID ();
	}

	m_paInfo->Add (pAccountInfo);
}


/* ========================================================================= */
/* �֐���	:CLibInfoAccount::Delete										 */
/* ���e		:�폜															 */
/* ���t		:2006/11/05														 */
/* ========================================================================= */

void CLibInfoAccount::Delete(
	int nNo)		/* [in] �z��ԍ� */
{
	PCInfoAccount pInfo;

	pInfo = m_paInfo->GetAt (nNo);
	SAFE_DELETE (pInfo);
	m_paInfo->RemoveAt (nNo);
}


/* ========================================================================= */
/* �֐���	:CLibInfoAccount::Delete										 */
/* ���e		:�폜															 */
/* ���t		:2006/11/05														 */
/* ========================================================================= */

void CLibInfoAccount::Delete(
	DWORD dwAccountID)		/* [in] �A�J�E���gID */
{
	int i, nCount, nNo;
	PCInfoAccount pInfoTmp;

	nNo = -1;

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->GetAt (i);
		if (pInfoTmp->m_dwAccountID != dwAccountID) {
			continue;
		}
		nNo = i;
		break;
	}

	if (nNo >= 0) {
		Delete (nNo);
	}
}


/* ========================================================================= */
/* �֐���	:CLibInfoAccount::DeleteAll										 */
/* ���e		:�S�č폜														 */
/* ���t		:2006/11/05														 */
/* ========================================================================= */

void CLibInfoAccount::DeleteAll(void)
{
	int i, nCount;

	if (m_paInfo == NULL) {
		return;
	}

	nCount = m_paInfo->GetSize ();
	for (i = nCount - 1; i >= 0; i --) {
		Delete (i);
	}
}


/* ========================================================================= */
/* �֐���	:CLibInfoAccount::GetAccountID									 */
/* ���e		:�L����ID����A�J�E���gID���擾									 */
/* ���t		:2007/07/05														 */
/* ========================================================================= */

DWORD CLibInfoAccount::GetAccountID(DWORD dwCharID)
{
	int i, nCount;
	DWORD dwRet;
	PCInfoAccount pInfo;

	dwRet = 0;

	nCount = GetCount ();
	for (i = 0; i < nCount; i ++) {
		pInfo = m_paInfo->GetAt (i);
		if (pInfo->m_dwCharID == dwCharID) {
			dwRet = pInfo->m_dwAccountID;
			break;
		}
	}

	return dwRet;
}


/* ========================================================================= */
/* �֐���	:CLibInfoAccount::GetPtr										 */
/* ���e		:�A�J�E���g�����擾											 */
/* ���t		:2006/11/05														 */
/* ========================================================================= */

PCInfoBase CLibInfoAccount::GetPtr(int nNo)
{
	return (PCInfoBase)m_paInfo->GetAt (nNo);
}


/* ========================================================================= */
/* �֐���	:CLibInfoAccount::GetPtr										 */
/* ���e		:�A�J�E���g�����擾											 */
/* ���t		:2006/11/05														 */
/* ========================================================================= */

PCInfoAccount CLibInfoAccount::GetPtr(
	DWORD dwAccountID)		/* [in] �A�J�E���gID */
{
	int i, nCount;
	PCInfoAccount pRet, pInfoTmp;

	pRet = NULL;

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->GetAt (i);
		if (pInfoTmp->m_dwAccountID != dwAccountID) {
			continue;
		}
		pRet = pInfoTmp;
		break;
	}

	return pRet;
}


/* ========================================================================= */
/* �֐���	:CLibInfoAccount::GetPtr										 */
/* ���e		:�A�J�E���g�����擾											 */
/* ���t		:2006/11/05														 */
/* ========================================================================= */

PCInfoAccount CLibInfoAccount::GetPtr(
	LPCSTR pszAccount)		/* [in] �A�J�E���g */
{
	int i, nCount;
	PCInfoAccount pRet, pInfoTmp;

	pRet = NULL;

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->GetAt (i);
		if (pInfoTmp->m_strAccount != pszAccount) {
			continue;
		}
		pRet = pInfoTmp;
		break;
	}

	return pRet;
}


/* ========================================================================= */
/* �֐���	:CLibInfoAccount::GetPtrSessionID								 */
/* ���e		:�A�J�E���g�����擾											 */
/* ���t		:2007/02/15														 */
/* ========================================================================= */

PCInfoAccount CLibInfoAccount::GetPtrSessionID(
	DWORD dwSessionID)		/* [in] �Z�b�V����ID */
{
	int i, nCount;
	PCInfoAccount pRet, pInfoTmp;

	pRet = NULL;

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->GetAt (i);
		if (pInfoTmp->m_dwSessionID != dwSessionID) {
			continue;
		}
		pRet = pInfoTmp;
		break;
	}

	return pRet;
}


/* ========================================================================= */
/* �֐���	:CLibInfoAccount::GetNewID										 */
/* ���e		:�V�����A�J�E���gID���擾										 */
/* ���t		:2006/11/05														 */
/* ========================================================================= */

DWORD CLibInfoAccount::GetNewID(void)
{
	DWORD dwRet;
	int i, nCount;
	PCInfoAccount pInfoTmp;

	dwRet = 1;

	nCount = m_paInfo->GetSize ();
	for (i = 0; i < nCount; i ++) {
		pInfoTmp = m_paInfo->GetAt (i);
		if (pInfoTmp->m_dwAccountID == dwRet) {
			dwRet ++;
			i = -1;
			continue;
		}
	}

	return dwRet;
}

/* Copyright(C)URARA-works 2006 */
