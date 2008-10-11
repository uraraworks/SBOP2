/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketADMIN_CHAR_MODIFYITEM.cpp							 */
/* ���e			:�R�}���h(�Ǘ��Ҍn:�����A�C�e���̕ύX) �����t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/09/24													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "InfoCharBase.h"
#include "Command.h"
#include "PacketADMIN_CHAR_MODIFYITEM.h"

/* ========================================================================= */
/* �֐���	:CPacketADMIN_CHAR_MODIFYITEM::CPacketADMIN_CHAR_MODIFYITEM		 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/09/24														 */
/* ========================================================================= */

CPacketADMIN_CHAR_MODIFYITEM::CPacketADMIN_CHAR_MODIFYITEM()
{
	m_dwCharID	= 0;
	m_dwItemID	= 0;
	m_nType		= CHARMODIFYITEMTYPE_NONE;
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_CHAR_MODIFYITEM::~CPacketADMIN_CHAR_MODIFYITEM	 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/09/24														 */
/* ========================================================================= */

CPacketADMIN_CHAR_MODIFYITEM::~CPacketADMIN_CHAR_MODIFYITEM()
{
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_CHAR_MODIFYITEM::Make								 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2007/09/24														 */
/* ========================================================================= */

void CPacketADMIN_CHAR_MODIFYITEM::Make(
	DWORD dwCharID,		/* [in] �L����ID */
	DWORD dwItemID,		/* [in] �A�C�e��ID */
	int nType)			/* [in] ��� */
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE)	+
			 sizeof (dwCharID)		+
			 sizeof (dwItemID)		+
			 sizeof (nType);

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_ADMIN;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ADMIN_CHAR_MODIFYITEM;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwCharID,	sizeof (dwCharID),	pDataTmp);	/* �L����ID */
	CopyMemoryRenew (pDataTmp, &dwItemID,	sizeof (dwItemID),	pDataTmp);	/* �A�C�e��ID */
	CopyMemoryRenew (pDataTmp, &nType,		sizeof (nType),		pDataTmp);	/* ��� */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_CHAR_MODIFYITEM::Set								 */
/* ���e		:�p�P�b�g��ݒ�													 */
/* ���t		:2007/09/24														 */
/* ========================================================================= */

PBYTE CPacketADMIN_CHAR_MODIFYITEM::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwCharID,	pDataTmp, sizeof (m_dwCharID),	pDataTmp);	/* �L����ID */
	CopyMemoryRenew (&m_dwItemID,	pDataTmp, sizeof (m_dwItemID),	pDataTmp);	/* �A�C�e��ID */
	CopyMemoryRenew (&m_nType,		pDataTmp, sizeof (m_nType),		pDataTmp);	/* ��� */

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2007 */
