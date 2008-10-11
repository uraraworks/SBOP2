/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:PacketITEM_REQ_ITEMWEAPONINFO.cpp							 */
/* ���e			:�R�}���h(�A�C�e���n:������v��) �����t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/08/11													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketITEM_REQ_ITEMWEAPONINFO.h"

/* ========================================================================= */
/* �֐���	:CPacketITEM_REQ_ITEMWEAPONINFO::CPacketITEM_REQ_ITEMWEAPONINFO	 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/08/11														 */
/* ========================================================================= */

CPacketITEM_REQ_ITEMWEAPONINFO::CPacketITEM_REQ_ITEMWEAPONINFO()
{
	m_dwWeaponInfoID = 0;
}


/* ========================================================================= */
/* �֐���	:CPacketITEM_REQ_ITEMWEAPONINFO::~CPacketITEM_REQ_ITEMWEAPONINFO */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/08/11														 */
/* ========================================================================= */

CPacketITEM_REQ_ITEMWEAPONINFO::~CPacketITEM_REQ_ITEMWEAPONINFO()
{
}


/* ========================================================================= */
/* �֐���	:CPacketITEM_REQ_ITEMWEAPONINFO::Make							 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2008/08/11														 */
/* ========================================================================= */

void CPacketITEM_REQ_ITEMWEAPONINFO::Make(
	DWORD dwWeaponInfoID)		/* [in] ������ID */
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE) +
			 sizeof (dwWeaponInfoID);

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_ITEM;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ITEM_REQ_ITEMWEAPONINFO;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwWeaponInfoID, sizeof (dwWeaponInfoID), pDataTmp);	/* ������ID */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* �֐���	:CPacketITEM_REQ_ITEMWEAPONINFO::Set							 */
/* ���e		:�p�P�b�g��ݒ�													 */
/* ���t		:2008/08/11														 */
/* ========================================================================= */

PBYTE CPacketITEM_REQ_ITEMWEAPONINFO::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwWeaponInfoID, pDataTmp, sizeof (m_dwWeaponInfoID), pDataTmp);	/* ������ID */

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2008 */
