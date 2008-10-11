/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:PacketITEM_RES_ITEMWEAPONINFO.cpp							 */
/* ���e			:�R�}���h(�A�C�e���n:�����񉞓�) �����t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/08/11													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "InfoItemWeapon.h"
#include "Command.h"
#include "PacketITEM_RES_ITEMWEAPONINFO.h"

/* ========================================================================= */
/* �֐���	:CPacketITEM_RES_ITEMWEAPONINFO::CPacketITEM_RES_ITEMWEAPONINFO	 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/08/11														 */
/* ========================================================================= */

CPacketITEM_RES_ITEMWEAPONINFO::CPacketITEM_RES_ITEMWEAPONINFO()
{
	m_pInfo = NULL;
}


/* ========================================================================= */
/* �֐���	:CPacketITEM_RES_ITEMWEAPONINFO::~CPacketITEM_RES_ITEMWEAPONINFO */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/08/11														 */
/* ========================================================================= */

CPacketITEM_RES_ITEMWEAPONINFO::~CPacketITEM_RES_ITEMWEAPONINFO()
{
	SAFE_DELETE (m_pInfo);
}


/* ========================================================================= */
/* �֐���	:CPacketITEM_RES_ITEMWEAPONINFO::Make							 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2008/08/11														 */
/* ========================================================================= */

void CPacketITEM_RES_ITEMWEAPONINFO::Make(
	CInfoItemWeapon *pInfo)		/* [in] ������ */
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
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ITEM_RES_ITEMWEAPONINFO;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, pInfoDataTmp, dwSizeDataTmp, pDataTmp);	/* ������ */

	RenewPacket (pData, dwSize);

	SAFE_DELETE_ARRAY (pInfoDataTmp);
}


/* ========================================================================= */
/* �֐���	:CPacketITEM_RES_ITEMWEAPONINFO::Set							 */
/* ���e		:�p�P�b�g��ݒ�													 */
/* ���t		:2008/08/11														 */
/* ========================================================================= */

PBYTE CPacketITEM_RES_ITEMWEAPONINFO::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	SAFE_DELETE (m_pInfo);
	m_pInfo		= new CInfoItemWeapon;
	pDataTmp	= m_pInfo->SetSendData (pDataTmp);

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2008 */
