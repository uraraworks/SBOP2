/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketADMIN_ITEM_COPY.cpp									 */
/* ���e			:�R�}���h(�Ǘ��Ҍn:�A�C�e�����R�s�[) �����t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/09/22													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketADMIN_ITEM_COPY.h"


/* ========================================================================= */
/* �֐���	:CPacketADMIN_ITEM_COPY::CPacketADMIN_ITEM_COPY					 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/09/22														 */
/* ========================================================================= */

CPacketADMIN_ITEM_COPY::CPacketADMIN_ITEM_COPY()
{
	m_dwItemID = 0;
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_ITEM_COPY::~CPacketADMIN_ITEM_COPY				 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/09/22														 */
/* ========================================================================= */

CPacketADMIN_ITEM_COPY::~CPacketADMIN_ITEM_COPY()
{
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_ITEM_COPY::Make									 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2007/09/22														 */
/* ========================================================================= */

void CPacketADMIN_ITEM_COPY::Make(
	DWORD dwItemID)		/* [in] �R�s�[���A�C�e��ID */
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE) +
			 sizeof (dwItemID);

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_ADMIN;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ADMIN_ITEM_COPY;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwItemID, sizeof (dwItemID), pDataTmp);	/* �A�C�e��ID */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_ITEM_COPY::Set									 */
/* ���e		:�p�P�b�g��ݒ�													 */
/* ���t		:2007/09/22														 */
/* ========================================================================= */

PBYTE CPacketADMIN_ITEM_COPY::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwItemID, pDataTmp, sizeof (m_dwItemID), pDataTmp);	/* �A�C�e��ID */

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2007 */
