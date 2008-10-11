/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketADMIN_ITEMTYPE_DELETE.cpp							 */
/* ���e			:�R�}���h(�Ǘ��Ҍn:�A�C�e����ʏ��폜) �����t�@�C��		 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/10/01													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketADMIN_ITEMTYPE_DELETE.h"


/* ========================================================================= */
/* �֐���	:CPacketADMIN_ITEMTYPE_DELETE::CPacketADMIN_ITEMTYPE_DELETE		 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/10/01														 */
/* ========================================================================= */

CPacketADMIN_ITEMTYPE_DELETE::CPacketADMIN_ITEMTYPE_DELETE()
{
	m_dwTypeID = 0;
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_ITEMTYPE_DELETE::~CPacketADMIN_ITEMTYPE_DELETE	 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/10/01														 */
/* ========================================================================= */

CPacketADMIN_ITEMTYPE_DELETE::~CPacketADMIN_ITEMTYPE_DELETE()
{
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_ITEMTYPE_DELETE::Make								 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2007/10/01														 */
/* ========================================================================= */

void CPacketADMIN_ITEMTYPE_DELETE::Make(
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

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_ADMIN;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ADMIN_ITEMTYPE_DELETE;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwTypeID, sizeof (dwTypeID), pDataTmp);	/* ���ID */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_ITEMTYPE_DELETE::Set								 */
/* ���e		:�p�P�b�g��ݒ�													 */
/* ���t		:2007/10/01														 */
/* ========================================================================= */

PBYTE CPacketADMIN_ITEMTYPE_DELETE::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwTypeID, pDataTmp, sizeof (m_dwTypeID), pDataTmp);	/* ���ID */

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2007 */
