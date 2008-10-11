/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketBATTLE_REQ_ATACK.cpp									 */
/* ���e			:�R�}���h(�퓬�n:�U���v��) �����t�@�C��						 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/07/26													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketBATTLE_REQ_ATACK.h"

/* ========================================================================= */
/* �֐���	:CPacketBATTLE_REQ_ATACK::CPacketBATTLE_REQ_ATACK				 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/07/26														 */
/* ========================================================================= */

CPacketBATTLE_REQ_ATACK::CPacketBATTLE_REQ_ATACK()
{
	m_dwCharID			= 0;
	m_dwTargetCharID	= 0;
}


/* ========================================================================= */
/* �֐���	:CPacketBATTLE_REQ_ATACK::~CPacketBATTLE_REQ_ATACK				 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/07/26														 */
/* ========================================================================= */

CPacketBATTLE_REQ_ATACK::~CPacketBATTLE_REQ_ATACK()
{
}


/* ========================================================================= */
/* �֐���	:CPacketBATTLE_REQ_ATACK::Make									 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2007/07/26														 */
/* ========================================================================= */

void CPacketBATTLE_REQ_ATACK::Make(
	DWORD dwCharID,				/* [in] �L����ID */
	DWORD dwTargetCharID)		/* [in] �U����L����ID */
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE)	+
			 sizeof (dwCharID)		+
			 sizeof (dwTargetCharID);

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_BATTLE;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_BATTLE_REQ_ATACK;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwCharID,		sizeof (dwCharID),			pDataTmp);	/* �L����ID */
	CopyMemoryRenew (pDataTmp, &dwTargetCharID,	sizeof (dwTargetCharID),	pDataTmp);	/* �U����L����ID */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* �֐���	:CPacketBATTLE_REQ_ATACK::Set									 */
/* ���e		:�p�P�b�g��ݒ�													 */
/* ���t		:2007/07/26														 */
/* ========================================================================= */

PBYTE CPacketBATTLE_REQ_ATACK::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwCharID,		pDataTmp, sizeof (m_dwCharID),			pDataTmp);	/* �L����ID */
	CopyMemoryRenew (&m_dwTargetCharID,	pDataTmp, sizeof (m_dwTargetCharID),	pDataTmp);	/* �U����L����ID */

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2007 */
