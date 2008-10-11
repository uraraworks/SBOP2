/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketITEM_DELETEITEMINFO.cpp								 */
/* ���e			:�R�}���h(�A�C�e���n:�A�C�e�����폜) �����t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/05/05													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketITEM_DELETEITEMINFO.h"


/* ========================================================================= */
/* �֐���	:CPacketITEM_DELETEITEMINFO::CPacketITEM_DELETEITEMINFO			 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/05/05														 */
/* ========================================================================= */

CPacketITEM_DELETEITEMINFO::CPacketITEM_DELETEITEMINFO()
{
	m_dwItemID = 0;
}


/* ========================================================================= */
/* �֐���	:CPacketITEM_DELETEITEMINFO::~CPacketITEM_DELETEITEMINFO		 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/05/05														 */
/* ========================================================================= */

CPacketITEM_DELETEITEMINFO::~CPacketITEM_DELETEITEMINFO()
{
}


/* ========================================================================= */
/* �֐���	:CPacketITEM_DELETEITEMINFO::Make								 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2007/05/05														 */
/* ========================================================================= */

void CPacketITEM_DELETEITEMINFO::Make(
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
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ITEM_DELETEITEMINFO;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwItemID, sizeof (dwItemID), pDataTmp);	/* �A�C�e��ID */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* �֐���	:CPacketITEM_DELETEITEMINFO::Set								 */
/* ���e		:�p�P�b�g��ݒ�													 */
/* ���t		:2007/05/05														 */
/* ========================================================================= */

PBYTE CPacketITEM_DELETEITEMINFO::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwItemID,	pDataTmp, sizeof (m_dwItemID), pDataTmp);	/* �A�C�e��ID */

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2007 */
