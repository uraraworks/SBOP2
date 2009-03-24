/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:PacketADMIN_CHAR_RENEW_ACCOUNT.cpp							 */
/* ���e			:�R�}���h(�Ǘ��Ҍn:�A�J�E���g���X�V) �����t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/06/07													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "InfoAccount.h"
#include "PacketADMIN_CHAR_RENEW_ACCOUNT.h"

/* ========================================================================= */
/* �֐���	:CPacketADMIN_CHAR_RENEW_ACCOUNT::CPacketADMIN_CHAR_RENEW_ACCOUNT */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/06/07														 */
/* ========================================================================= */

CPacketADMIN_CHAR_RENEW_ACCOUNT::CPacketADMIN_CHAR_RENEW_ACCOUNT()
{
	m_bDisable = FALSE;
	m_dwAccountID = 0;
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_CHAR_RENEW_ACCOUNT::~CPacketADMIN_CHAR_RENEW_ACCOUNT */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/06/07														 */
/* ========================================================================= */

CPacketADMIN_CHAR_RENEW_ACCOUNT::~CPacketADMIN_CHAR_RENEW_ACCOUNT()
{
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_CHAR_RENEW_ACCOUNT::Make							 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2008/06/07														 */
/* ========================================================================= */

void CPacketADMIN_CHAR_RENEW_ACCOUNT::Make(
	CInfoAccount *pInfoAccount,		/* [in] �A�J�E���g��� */
	BOOL bDisable,					/* [in] ���O�C������ */
	LPCSTR pszMacAddress)			/* [in] ����MAC�A�h���X */
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE) +
			 sizeof (bDisable) +
			 sizeof (pInfoAccount->m_dwAccountID) +
			 		(pInfoAccount->m_strAccount.GetLength () + 1) +
			 		(pInfoAccount->m_strPassword.GetLength () + 1);
	if (pszMacAddress) {
		dwSize += strlen (pszMacAddress);
	}
	dwSize ++;

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_ADMIN;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ADMIN_CHAR_RENEW_ACCOUNT;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &pInfoAccount->m_dwAccountID, sizeof (pInfoAccount->m_dwAccountID), pDataTmp);	/* �A�J�E���gID */
	CopyMemoryRenew (pDataTmp, &bDisable, sizeof (bDisable), pDataTmp);	/* ���O�C������ */
	strcpyRenew ((LPSTR)pDataTmp, pInfoAccount->m_strAccount,  pDataTmp);										/* �A�J�E���g */
	strcpyRenew ((LPSTR)pDataTmp, pInfoAccount->m_strPassword, pDataTmp);										/* �p�X���[�h */
	if (pszMacAddress) {
		strcpyRenew ((LPSTR)pDataTmp, pszMacAddress, pDataTmp);													/* ����MAC�A�h���X */
	}

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_CHAR_RENEW_ACCOUNT::Set							 */
/* ���e		:�p�P�b�g��ݒ�													 */
/* ���t		:2008/06/07														 */
/* ========================================================================= */

PBYTE CPacketADMIN_CHAR_RENEW_ACCOUNT::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwAccountID, pDataTmp, sizeof (m_dwAccountID), pDataTmp);	/* �A�J�E���gID */
	CopyMemoryRenew (&m_bDisable, pDataTmp, sizeof (m_bDisable), pDataTmp);			/* ���O�C������ */
	StoreRenew (m_strAccount,	(LPCSTR)pDataTmp, pDataTmp);						/* �A�J�E���g */
	StoreRenew (m_strPassword,	(LPCSTR)pDataTmp, pDataTmp);						/* �p�X���[�h */
	StoreRenew (m_strMacAddress,	(LPCSTR)pDataTmp, pDataTmp);					/* ����MAC�A�h���X */

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2008 */
