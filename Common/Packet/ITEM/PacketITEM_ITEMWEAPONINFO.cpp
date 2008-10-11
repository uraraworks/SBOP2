/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:PacketITEM_ITEMWEAPONINFO.cpp								 */
/* ���e			:�R�}���h(�A�C�e���n:������ʒm) �����t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/08/11													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketITEM_ITEMWEAPONINFO.h"

/* ========================================================================= */
/* �֐���	:CPacketITEM_ITEMWEAPONINFO::CPacketITEM_ITEMWEAPONINFO			 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/08/11														 */
/* ========================================================================= */

CPacketITEM_ITEMWEAPONINFO::CPacketITEM_ITEMWEAPONINFO()
{
	m_pLibInfo = NULL;
}


/* ========================================================================= */
/* �֐���	:CPacketITEM_ITEMWEAPONINFO::~CPacketITEM_ITEMWEAPONINFO		 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/08/11														 */
/* ========================================================================= */

CPacketITEM_ITEMWEAPONINFO::~CPacketITEM_ITEMWEAPONINFO()
{
	SAFE_DELETE (m_pLibInfo);
}


/* ========================================================================= */
/* �֐���	:CPacketITEM_ITEMWEAPONINFO::Make								 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2008/08/11														 */
/* ========================================================================= */

void CPacketITEM_ITEMWEAPONINFO::Make(
	PCLibInfoItemWeapon pInfo)		/* [in] ������ */
{
	PBYTE pData, pDataTmp, pInfoDataTmp;
	DWORD dwSize, dwSizeDataTmp;
	PPACKETBASE pPacketBase;

	dwSizeDataTmp	= pInfo->GetSendDataSize ();
	pInfoDataTmp	= pInfo->GetSendData ();

	dwSize = sizeof (PACKETBASE) +
			 dwSizeDataTmp;

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_ITEM;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ITEM_ITEMWEAPONINFO;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, pInfoDataTmp, dwSizeDataTmp, pDataTmp);	/* ������ */

	RenewPacket (pData, dwSize);

	SAFE_DELETE_ARRAY (pInfoDataTmp);
}


/* ========================================================================= */
/* �֐���	:CPacketITEM_ITEMWEAPONINFO::Set								 */
/* ���e		:�p�P�b�g��ݒ�													 */
/* ���t		:2008/08/11														 */
/* ========================================================================= */

PBYTE CPacketITEM_ITEMWEAPONINFO::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	SAFE_DELETE (m_pLibInfo);
	m_pLibInfo	= new CLibInfoItemWeapon;
	m_pLibInfo->Create ();
	pDataTmp	= m_pLibInfo->SetSendData (pDataTmp);

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2008 */
