/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketADMIN_ITEMTYPE_ADD.cpp								 */
/* ���e			:�R�}���h(�Ǘ��Ҍn:�A�C�e����ʏ��ǉ�) �����t�@�C��		 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/10/01													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "LibInfoItemType.h"
#include "Command.h"
#include "PacketADMIN_ITEMTYPE_ADD.h"

/* ========================================================================= */
/* �֐���	:CPacketADMIN_ITEMTYPE_ADD::CPacketADMIN_ITEMTYPE_ADD			 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/10/01														 */
/* ========================================================================= */

CPacketADMIN_ITEMTYPE_ADD::CPacketADMIN_ITEMTYPE_ADD()
{
	m_pInfoItem = NULL;
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_ITEMTYPE_ADD::~CPacketADMIN_ITEMTYPE_ADD			 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/10/01														 */
/* ========================================================================= */

CPacketADMIN_ITEMTYPE_ADD::~CPacketADMIN_ITEMTYPE_ADD()
{
	SAFE_DELETE (m_pInfoItem);
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_ITEMTYPE_ADD::Make								 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2007/10/01														 */
/* ========================================================================= */

void CPacketADMIN_ITEMTYPE_ADD::Make(
	CInfoItemTypeBase *pInfo)		/* [in] �A�C�e����ʏ�� */
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
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ADMIN_ITEMTYPE_ADD;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, pInfoDataTmp, dwSizeDataTmp, pDataTmp);	/* �A�C�e����ʏ�� */

	RenewPacket (pData, dwSize);

	SAFE_DELETE_ARRAY (pInfoDataTmp);
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_ITEMTYPE_ADD::Set									 */
/* ���e		:�p�P�b�g��ݒ�													 */
/* ���t		:2007/10/01														 */
/* ========================================================================= */

PBYTE CPacketADMIN_ITEMTYPE_ADD::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;
	CLibInfoItemType LibInfo;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);
	pDataTmp	= LibInfo.GetNewSendData (pDataTmp, m_pInfoItem);

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2007 */
