/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketITEM_REQ_ITEMINFO.cpp								 */
/* ���e			:�R�}���h(�A�C�e���n:�A�C�e�����v��) �����t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/08/16													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketITEM_REQ_ITEMINFO.h"

/* ========================================================================= */
/* �֐���	:CPacketITEM_REQ_ITEMINFO::CPacketITEM_REQ_ITEMINFO				 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/08/16														 */
/* ========================================================================= */

CPacketITEM_REQ_ITEMINFO::CPacketITEM_REQ_ITEMINFO()
{
	m_dwItemID = 0;
}


/* ========================================================================= */
/* �֐���	:CPacketITEM_REQ_ITEMINFO::~CPacketITEM_REQ_ITEMINFO			 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/08/16														 */
/* ========================================================================= */

CPacketITEM_REQ_ITEMINFO::~CPacketITEM_REQ_ITEMINFO()
{
}


/* ========================================================================= */
/* �֐���	:CPacketITEM_REQ_ITEMINFO::Make									 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2007/08/16														 */
/* ========================================================================= */

void CPacketITEM_REQ_ITEMINFO::Make(
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

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_ITEM;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ITEM_REQ_ITEMINFO;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwItemID, sizeof (dwItemID), pDataTmp);	/* �A�C�e��ID */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* �֐���	:CPacketITEM_REQ_ITEMINFO::Set									 */
/* ���e		:�p�P�b�g��ݒ�													 */
/* ���t		:2007/08/16														 */
/* ========================================================================= */

PBYTE CPacketITEM_REQ_ITEMINFO::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwItemID, pDataTmp, sizeof (m_dwItemID), pDataTmp);	/* �A�C�e��ID */

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2007 */
