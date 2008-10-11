/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketITEM_DELETEITEMTYPEINFO.cpp							 */
/* ���e			:�R�}���h(�A�C�e���n:�A�C�e����ʏ��폜) �����t�@�C��		 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/10/03													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketITEM_DELETEITEMTYPEINFO.h"


/* ========================================================================= */
/* �֐���	:CPacketITEM_DELETEITEMTYPEINFO::CPacketITEM_DELETEITEMTYPEINFO	 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/10/03														 */
/* ========================================================================= */

CPacketITEM_DELETEITEMTYPEINFO::CPacketITEM_DELETEITEMTYPEINFO()
{
	m_dwTypeID = 0;
}


/* ========================================================================= */
/* �֐���	:CPacketITEM_DELETEITEMTYPEINFO::~CPacketITEM_DELETEITEMTYPEINFO */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/10/03														 */
/* ========================================================================= */

CPacketITEM_DELETEITEMTYPEINFO::~CPacketITEM_DELETEITEMTYPEINFO()
{
}


/* ========================================================================= */
/* �֐���	:CPacketITEM_DELETEITEMTYPEINFO::Make							 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2007/10/03														 */
/* ========================================================================= */

void CPacketITEM_DELETEITEMTYPEINFO::Make(
	DWORD dwTypeID)		/* [in] ���ID */
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE) +
			 sizeof (dwTypeID);

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_ITEM;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ITEM_DELETEITEMTYPEINFO;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwTypeID, sizeof (dwTypeID), pDataTmp);	/* ���ID */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* �֐���	:CPacketITEM_DELETEITEMTYPEINFO::Set							 */
/* ���e		:�p�P�b�g��ݒ�													 */
/* ���t		:2007/10/03														 */
/* ========================================================================= */

PBYTE CPacketITEM_DELETEITEMTYPEINFO::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwTypeID,	pDataTmp, sizeof (m_dwTypeID), pDataTmp);	/* ���ID */

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2007 */
