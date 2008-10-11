/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:PacketADMIN_CHAR_REQ_ACCOUNT.cpp							 */
/* ���e			:�R�}���h(�Ǘ��Ҍn:�A�J�E���g���v��) �����t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/06/07													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketADMIN_CHAR_REQ_ACCOUNT.h"

/* ========================================================================= */
/* �֐���	:CPacketADMIN_CHAR_REQ_ACCOUNT::CPacketADMIN_CHAR_REQ_ACCOUNT	 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/06/07														 */
/* ========================================================================= */

CPacketADMIN_CHAR_REQ_ACCOUNT::CPacketADMIN_CHAR_REQ_ACCOUNT()
{
	m_dwAccountID = 0;
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_CHAR_REQ_ACCOUNT::~CPacketADMIN_CHAR_REQ_ACCOUNT	 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/06/07														 */
/* ========================================================================= */

CPacketADMIN_CHAR_REQ_ACCOUNT::~CPacketADMIN_CHAR_REQ_ACCOUNT()
{
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_CHAR_REQ_ACCOUNT::Make							 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2008/06/07														 */
/* ========================================================================= */

void CPacketADMIN_CHAR_REQ_ACCOUNT::Make(
	DWORD dwAccountID)		/* [in] �A�J�E���gID */
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE) +
			 sizeof (dwAccountID);

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_ADMIN;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ADMIN_CHAR_REQ_ACCOUNT;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwAccountID, sizeof (dwAccountID), pDataTmp);	/* �A�J�E���gID */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_CHAR_REQ_ACCOUNT::Set								 */
/* ���e		:�p�P�b�g��ݒ�													 */
/* ���t		:2008/06/07														 */
/* ========================================================================= */

PBYTE CPacketADMIN_CHAR_REQ_ACCOUNT::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwAccountID, pDataTmp, sizeof (m_dwAccountID), pDataTmp);	/* �A�J�E���gID */

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2008 */
