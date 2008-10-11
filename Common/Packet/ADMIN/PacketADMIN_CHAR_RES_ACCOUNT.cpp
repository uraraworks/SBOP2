/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:PacketADMIN_CHAR_RES_ACCOUNT.cpp							 */
/* ���e			:�R�}���h(�Ǘ��Ҍn:�A�J�E���g��񉞓�) �����t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/06/07													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "InfoAccount.h"
#include "PacketADMIN_CHAR_RES_ACCOUNT.h"

/* ========================================================================= */
/* �֐���	:CPacketADMIN_CHAR_RES_ACCOUNT::CPacketADMIN_CHAR_RES_ACCOUNT	 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/06/07														 */
/* ========================================================================= */

CPacketADMIN_CHAR_RES_ACCOUNT::CPacketADMIN_CHAR_RES_ACCOUNT()
{
	m_dwAccountID = 0;
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_CHAR_RES_ACCOUNT::~CPacketADMIN_CHAR_RES_ACCOUNT	 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/06/07														 */
/* ========================================================================= */

CPacketADMIN_CHAR_RES_ACCOUNT::~CPacketADMIN_CHAR_RES_ACCOUNT()
{
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_CHAR_RES_ACCOUNT::Make							 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2008/06/07														 */
/* ========================================================================= */

void CPacketADMIN_CHAR_RES_ACCOUNT::Make(
	CInfoAccount *pInfoAccount)		/* [in] �A�J�E���g��� */
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE) +
			 sizeof (pInfoAccount->m_dwAccountID) +
			 		(pInfoAccount->m_strAccount.GetLength () + 1) +
			 		(pInfoAccount->m_strPassword.GetLength () + 1);

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_ADMIN;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ADMIN_CHAR_RES_ACCOUNT;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &pInfoAccount->m_dwAccountID, sizeof (pInfoAccount->m_dwAccountID), pDataTmp);	/* �A�J�E���gID */
	strcpyRenew ((LPSTR)pDataTmp, pInfoAccount->m_strAccount,  pDataTmp);										/* �A�J�E���g */
	strcpyRenew ((LPSTR)pDataTmp, pInfoAccount->m_strPassword, pDataTmp);										/* �p�X���[�h */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_CHAR_RES_ACCOUNT::Set								 */
/* ���e		:�p�P�b�g��ݒ�													 */
/* ���t		:2008/06/07														 */
/* ========================================================================= */

PBYTE CPacketADMIN_CHAR_RES_ACCOUNT::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwAccountID, pDataTmp, sizeof (m_dwAccountID), pDataTmp);	/* �A�J�E���gID */
	StoreRenew (m_strAccount,	(LPCSTR)pDataTmp, pDataTmp);						/* �A�J�E���g */
	StoreRenew (m_strPassword,	(LPCSTR)pDataTmp, pDataTmp);						/* �p�X���[�h */

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2008 */
