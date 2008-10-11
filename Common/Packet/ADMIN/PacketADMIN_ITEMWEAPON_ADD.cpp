/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:PacketADMIN_ITEMWEAPON_ADD.cpp								 */
/* ���e			:�R�}���h(�Ǘ��Ҍn:������ǉ�) �����t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/08/11													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "LibInfoItemWeapon.h"
#include "Command.h"
#include "PacketADMIN_ITEMWEAPON_ADD.h"

/* ========================================================================= */
/* �֐���	:CPacketADMIN_ITEMWEAPON_ADD::CPacketADMIN_ITEMWEAPON_ADD		 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/08/11														 */
/* ========================================================================= */

CPacketADMIN_ITEMWEAPON_ADD::CPacketADMIN_ITEMWEAPON_ADD()
{
	m_pInfo = NULL;
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_ITEMWEAPON_ADD::~CPacketADMIN_ITEMWEAPON_ADD		 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/08/11														 */
/* ========================================================================= */

CPacketADMIN_ITEMWEAPON_ADD::~CPacketADMIN_ITEMWEAPON_ADD()
{
	SAFE_DELETE (m_pInfo);
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_ITEMWEAPON_ADD::Make								 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2008/08/11														 */
/* ========================================================================= */

void CPacketADMIN_ITEMWEAPON_ADD::Make(
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

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_ADMIN;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ADMIN_ITEMWEAPON_ADD;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, pInfoDataTmp, dwSizeDataTmp, pDataTmp);	/* ������ */

	RenewPacket (pData, dwSize);

	SAFE_DELETE_ARRAY (pInfoDataTmp);
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_ITEMWEAPON_ADD::Set								 */
/* ���e		:�p�P�b�g��ݒ�													 */
/* ���t		:2008/08/11														 */
/* ========================================================================= */

PBYTE CPacketADMIN_ITEMWEAPON_ADD::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;
	CLibInfoItemWeapon LibInfo;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);
	pDataTmp	= LibInfo.GetNewSendData (pDataTmp, m_pInfo);

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2008 */
