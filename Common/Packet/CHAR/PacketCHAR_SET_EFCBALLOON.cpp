/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketCHAR_SET_EFCBALLOON.cpp								 */
/* ���e			:�R�}���h(�L�����n:���o�����w��) �����t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/12/31													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketCHAR_SET_EFCBALLOON.h"

/* ========================================================================= */
/* �֐���	:CPacketCHAR_SET_EFCBALLOON::CPacketCHAR_SET_EFCBALLOON			 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/12/31														 */
/* ========================================================================= */

CPacketCHAR_SET_EFCBALLOON::CPacketCHAR_SET_EFCBALLOON()
{
	m_dwCharID		= 0;
	m_dwBalloonID	= 0;
}


/* ========================================================================= */
/* �֐���	:CPacketCHAR_SET_EFCBALLOON::~CPacketCHAR_SET_EFCBALLOON		 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/12/31														 */
/* ========================================================================= */

CPacketCHAR_SET_EFCBALLOON::~CPacketCHAR_SET_EFCBALLOON()
{
}


/* ========================================================================= */
/* �֐���	:CPacketCHAR_SET_EFCBALLOON::Make								 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2007/12/31														 */
/* ========================================================================= */

void CPacketCHAR_SET_EFCBALLOON::Make(
	DWORD dwCharID,			/* [in] �L����ID */
	DWORD dwBalloonID)		/* [in] ���o��ID */
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE)	+
			 sizeof (dwCharID)		+
			 sizeof (dwBalloonID);

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_CHAR;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_CHAR_SET_EFCBALLOON;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwCharID,		sizeof (dwCharID),		pDataTmp);	/* �L����ID */
	CopyMemoryRenew (pDataTmp, &dwBalloonID,	sizeof (dwBalloonID),	pDataTmp);	/* ���o��ID */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* �֐���	:CPacketCHAR_SET_EFCBALLOON::Set								 */
/* ���e		:�p�P�b�g��ݒ�													 */
/* ���t		:2007/12/31														 */
/* ========================================================================= */

PBYTE CPacketCHAR_SET_EFCBALLOON::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwCharID,		pDataTmp, sizeof (m_dwCharID),		pDataTmp);	/* �L����ID */
	CopyMemoryRenew (&m_dwBalloonID,	pDataTmp, sizeof (m_dwBalloonID),	pDataTmp);	/* ���o��ID */

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2007 */
