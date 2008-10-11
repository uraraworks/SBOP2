/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketADMIN_ITEM_ADD.cpp									 */
/* ���e			:�R�}���h(�Ǘ��Ҍn:�A�C�e�����ǉ�) �����t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/08/16													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "InfoItem.h"
#include "Command.h"
#include "PacketADMIN_ITEM_ADD.h"

/* ========================================================================= */
/* �֐���	:CPacketADMIN_ITEM_ADD::CPacketADMIN_ITEM_ADD					 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/08/16														 */
/* ========================================================================= */

CPacketADMIN_ITEM_ADD::CPacketADMIN_ITEM_ADD()
{
	m_pInfoItem = NULL;
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_ITEM_ADD::~CPacketADMIN_ITEM_ADD					 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/08/16														 */
/* ========================================================================= */

CPacketADMIN_ITEM_ADD::~CPacketADMIN_ITEM_ADD()
{
	SAFE_DELETE (m_pInfoItem);
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_ITEM_ADD::Make									 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2007/08/16														 */
/* ========================================================================= */

void CPacketADMIN_ITEM_ADD::Make(
	CInfoItem *pInfo)		/* [in] �A�C�e����� */
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
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ADMIN_ITEM_ADD;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, pInfoDataTmp, dwSizeDataTmp, pDataTmp);	/* �A�C�e����� */

	RenewPacket (pData, dwSize);

	SAFE_DELETE_ARRAY (pInfoDataTmp);
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_ITEM_ADD::Set										 */
/* ���e		:�p�P�b�g��ݒ�													 */
/* ���t		:2007/08/16														 */
/* ========================================================================= */

PBYTE CPacketADMIN_ITEM_ADD::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	SAFE_DELETE (m_pInfoItem);
	m_pInfoItem	= new CInfoItem;
	pDataTmp	= m_pInfoItem->SetSendData (pDataTmp);

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2007 */
