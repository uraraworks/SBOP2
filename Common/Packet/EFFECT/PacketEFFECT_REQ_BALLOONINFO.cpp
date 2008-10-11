/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketEFFECT_REQ_BALLOONINFO.cpp							 */
/* ���e			:�R�}���h(���o���n:���o�����v��) �����t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/12/24													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketEFFECT_REQ_BALLOONINFO.h"

/* ========================================================================= */
/* �֐���	:CPacketEFFECT_REQ_BALLOONINFO::CPacketEFFECT_REQ_BALLOONINFO	 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/12/24														 */
/* ========================================================================= */

CPacketEFFECT_REQ_BALLOONINFO::CPacketEFFECT_REQ_BALLOONINFO()
{
	m_dwBalloonID = 0;
}


/* ========================================================================= */
/* �֐���	:CPacketEFFECT_REQ_BALLOONINFO::~CPacketEFFECT_REQ_BALLOONINFO	 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/12/24														 */
/* ========================================================================= */

CPacketEFFECT_REQ_BALLOONINFO::~CPacketEFFECT_REQ_BALLOONINFO()
{
}


/* ========================================================================= */
/* �֐���	:CPacketEFFECT_REQ_BALLOONINFO::Make							 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2007/12/24														 */
/* ========================================================================= */

void CPacketEFFECT_REQ_BALLOONINFO::Make(
	DWORD dwBalloonID)		/* [in] ���o��ID */
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE) +
			 sizeof (dwBalloonID);

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_EFFECT;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_EFFECT_REQ_BALLOONINFO;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwBalloonID, sizeof (dwBalloonID), pDataTmp);	/* ���o��ID */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* �֐���	:CPacketEFFECT_REQ_BALLOONINFO::Set								 */
/* ���e		:�p�P�b�g��ݒ�													 */
/* ���t		:2007/12/24														 */
/* ========================================================================= */

PBYTE CPacketEFFECT_REQ_BALLOONINFO::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwBalloonID, pDataTmp, sizeof (m_dwBalloonID), pDataTmp);	/* ���o��ID */

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2007 */
