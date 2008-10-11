/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* �t�@�C����	:PacketACCOUNT_REQ_ACCOUNTINFO.cpp							 */
/* ���e			:�R�}���h(�A�J�E���g�n:�A�J�E���g���v��) �����t�@�C��		 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2006/11/06													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketACCOUNT_REQ_ACCOUNTINFO.h"

/* ========================================================================= */
/* �֐���	:CPacketACCOUNT_REQ_ACCOUNTINFO::CPacketACCOUNT_REQ_ACCOUNTINFO	 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2006/11/06														 */
/* ========================================================================= */

CPacketACCOUNT_REQ_ACCOUNTINFO::CPacketACCOUNT_REQ_ACCOUNTINFO()
{
	m_dwAccountID = 0;
}


/* ========================================================================= */
/* �֐���	:CPacketACCOUNT_REQ_ACCOUNTINFO::~CPacketACCOUNT_REQ_ACCOUNTINFO */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2006/11/06														 */
/* ========================================================================= */

CPacketACCOUNT_REQ_ACCOUNTINFO::~CPacketACCOUNT_REQ_ACCOUNTINFO()
{
}


/* ========================================================================= */
/* �֐���	:CPacketACCOUNT_REQ_ACCOUNTINFO::Make							 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2006/11/06														 */
/* ========================================================================= */

void CPacketACCOUNT_REQ_ACCOUNTINFO::Make(
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

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_ACCOUNT;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ACCOUNT_REQ_ACCOUNTINFO;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwAccountID, sizeof (dwAccountID), pDataTmp);	/* �A�J�E���gID */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* �֐���	:CPacketACCOUNT_REQ_ACCOUNTINFO::Set							 */
/* ���e		:�p�P�b�g��ݒ�													 */
/* ���t		:2006/11/06														 */
/* ========================================================================= */

PBYTE CPacketACCOUNT_REQ_ACCOUNTINFO::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwAccountID, pDataTmp, sizeof (m_dwAccountID), pDataTmp);	/* �A�J�E���gID */

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2006 */
