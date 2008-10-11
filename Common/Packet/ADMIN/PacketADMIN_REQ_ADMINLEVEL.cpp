/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketADMIN_REQ_ADMINLEVEL.cpp								 */
/* ���e			:�R�}���h(�Ǘ��Ҍn:�Ǘ��Ҍ������x���v��) �����t�@�C��		 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/07/05													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketADMIN_REQ_ADMINLEVEL.h"

/* ========================================================================= */
/* �֐���	:CPacketADMIN_REQ_ADMINLEVEL::CPacketADMIN_REQ_ADMINLEVEL		 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/07/05														 */
/* ========================================================================= */

CPacketADMIN_REQ_ADMINLEVEL::CPacketADMIN_REQ_ADMINLEVEL()
{
	m_dwAccountID = 0;
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_REQ_ADMINLEVEL::~CPacketADMIN_REQ_ADMINLEVEL		 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/07/05														 */
/* ========================================================================= */

CPacketADMIN_REQ_ADMINLEVEL::~CPacketADMIN_REQ_ADMINLEVEL()
{
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_REQ_ADMINLEVEL::Make								 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2007/07/05														 */
/* ========================================================================= */

void CPacketADMIN_REQ_ADMINLEVEL::Make(
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
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ADMIN_REQ_ADMINLEVEL;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwAccountID, sizeof (dwAccountID), pDataTmp);	/* �A�J�E���gID */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_REQ_ADMINLEVEL::Set								 */
/* ���e		:�p�P�b�g��ݒ�													 */
/* ���t		:2007/07/05														 */
/* ========================================================================= */

PBYTE CPacketADMIN_REQ_ADMINLEVEL::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwAccountID, pDataTmp, sizeof (m_dwAccountID), pDataTmp);	/* �A�J�E���gID */

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2007 */
