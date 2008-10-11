/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketCHAR_REQ_EQUIP.cpp									 */
/* ���e			:�R�}���h(�L�����n:�����v��) �����t�@�C��					 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/10/08													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketCHAR_REQ_EQUIP.h"

/* ========================================================================= */
/* �֐���	:CPacketCHAR_REQ_EQUIP::CPacketCHAR_REQ_EQUIP					 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/10/08														 */
/* ========================================================================= */

CPacketCHAR_REQ_EQUIP::CPacketCHAR_REQ_EQUIP()
{
	m_nUnsetType	= -1;
	m_dwCharID		= 0;
	m_dwItemID		= 0;
}


/* ========================================================================= */
/* �֐���	:CPacketCHAR_REQ_EQUIP::~CPacketCHAR_REQ_EQUIP					 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/10/08														 */
/* ========================================================================= */

CPacketCHAR_REQ_EQUIP::~CPacketCHAR_REQ_EQUIP()
{
}


/* ========================================================================= */
/* �֐���	:CPacketCHAR_REQ_EQUIP::Make									 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2007/10/08														 */
/* ========================================================================= */

void CPacketCHAR_REQ_EQUIP::Make(
	DWORD dwCharID,		/* [in] �L����ID */
	DWORD dwItemID,		/* [in] ��������A�C�e��ID */
	int nUnsetType)		/* [in] �������������� */
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE) +
			 sizeof (dwCharID)	 +
			 sizeof (dwItemID)	 +
			 sizeof (nUnsetType);

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_CHAR;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_CHAR_REQ_EQUIP;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwCharID,	sizeof (dwCharID),		pDataTmp);	/* �L����ID */
	CopyMemoryRenew (pDataTmp, &dwItemID,	sizeof (dwItemID),		pDataTmp);	/* ��������A�C�e��ID */
	CopyMemoryRenew (pDataTmp, &nUnsetType,	sizeof (nUnsetType),	pDataTmp);	/* �������������� */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* �֐���	:CPacketCHAR_REQ_EQUIP::Set										 */
/* ���e		:�p�P�b�g��ݒ�													 */
/* ���t		:2007/10/08														 */
/* ========================================================================= */

PBYTE CPacketCHAR_REQ_EQUIP::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwCharID,	pDataTmp, sizeof (m_dwCharID),		pDataTmp);	/* �L����ID */
	CopyMemoryRenew (&m_dwItemID,	pDataTmp, sizeof (m_dwItemID),		pDataTmp);	/* ��������A�C�e��ID */
	CopyMemoryRenew (&m_nUnsetType,	pDataTmp, sizeof (m_nUnsetType),	pDataTmp);	/* �������������� */

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2007 */
