/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketMSGCMD_MAKEITEM.cpp									 */
/* ���e			:�R�}���h(���b�Z�[�W�R�}���h�n:�A�C�e���쐬) �����t�@�C��	 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/10/20													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketMSGCMD_MAKEITEM.h"

/* ========================================================================= */
/* �֐���	:CPacketMSGCMD_MAKEITEM::CPacketMSGCMD_MAKEITEM					 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/10/20														 */
/* ========================================================================= */

CPacketMSGCMD_MAKEITEM::CPacketMSGCMD_MAKEITEM()
{
	m_dwCharID		= 0;
	m_dwItemTypeID	= 0;
}


/* ========================================================================= */
/* �֐���	:CPacketMSGCMD_MAKEITEM::~CPacketMSGCMD_MAKEITEM				 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/10/20														 */
/* ========================================================================= */

CPacketMSGCMD_MAKEITEM::~CPacketMSGCMD_MAKEITEM()
{
}


/* ========================================================================= */
/* �֐���	:CPacketMSGCMD_MAKEITEM::Make									 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2007/10/20														 */
/* ========================================================================= */

void CPacketMSGCMD_MAKEITEM::Make(
	DWORD dwCharID,			/* [in] �L����ID */
	DWORD dwItemTypeID)		/* [in] �A�C�e�����ID */
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE)	+
			 sizeof (dwCharID)		+
			 sizeof (dwItemTypeID);

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_MSGCMD;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_MSGCMD_MAKEITEM;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwCharID,		sizeof (dwCharID),		pDataTmp);	/* �L����ID */
	CopyMemoryRenew (pDataTmp, &dwItemTypeID,	sizeof (dwItemTypeID),	pDataTmp);	/* �A�C�e�����ID */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* �֐���	:CPacketMSGCMD_MAKEITEM::Set									 */
/* ���e		:�p�P�b�g��ݒ�													 */
/* ���t		:2007/10/20														 */
/* ========================================================================= */

PBYTE CPacketMSGCMD_MAKEITEM::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwCharID,		pDataTmp, sizeof (m_dwCharID),		pDataTmp);	/* �L����ID */
	CopyMemoryRenew (&m_dwItemTypeID,	pDataTmp, sizeof (m_dwItemTypeID),	pDataTmp);	/* �A�C�e�����ID */

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2007 */
