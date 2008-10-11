/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketADMIN_ITEM_DELETE.cpp								 */
/* ���e			:�R�}���h(�Ǘ��Ҍn:�A�C�e�����폜) �����t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/08/19													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketADMIN_ITEM_DELETE.h"


/* ========================================================================= */
/* �֐���	:CPacketADMIN_ITEM_DELETE::CPacketADMIN_ITEM_DELETE				 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/08/19														 */
/* ========================================================================= */

CPacketADMIN_ITEM_DELETE::CPacketADMIN_ITEM_DELETE()
{
	m_dwItemID = 0;
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_ITEM_DELETE::~CPacketADMIN_ITEM_DELETE			 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/08/19														 */
/* ========================================================================= */

CPacketADMIN_ITEM_DELETE::~CPacketADMIN_ITEM_DELETE()
{
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_ITEM_DELETE::Make									 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2007/08/19														 */
/* ========================================================================= */

void CPacketADMIN_ITEM_DELETE::Make(
	DWORD dwItemID)		/* [in] �A�C�e��ID */
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
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ADMIN_ITEM_DELETE;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwItemID, sizeof (dwItemID), pDataTmp);	/* �A�C�e��ID */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_ITEM_DELETE::Set									 */
/* ���e		:�p�P�b�g��ݒ�													 */
/* ���t		:2007/08/19														 */
/* ========================================================================= */

PBYTE CPacketADMIN_ITEM_DELETE::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwItemID, pDataTmp, sizeof (m_dwItemID), pDataTmp);	/* �A�C�e��ID */

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2007 */
