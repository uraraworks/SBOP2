/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketEFFECT_DELETEBALLOONINFO.cpp							 */
/* ���e			:�R�}���h(���o���n:���o�����폜) �����t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/12/24													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketEFFECT_DELETEBALLOONINFO.h"

/* ========================================================================= */
/* �֐���	:CPacketEFFECT_DELETEBALLOONINFO::CPacketEFFECT_DELETEBALLOONINFO */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/12/24														 */
/* ========================================================================= */

CPacketEFFECT_DELETEBALLOONINFO::CPacketEFFECT_DELETEBALLOONINFO()
{
	m_dwBalloonID = 0;
}


/* ========================================================================= */
/* �֐���	:CPacketEFFECT_DELETEBALLOONINFO::~CPacketEFFECT_DELETEBALLOONINFO */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/12/24														 */
/* ========================================================================= */

CPacketEFFECT_DELETEBALLOONINFO::~CPacketEFFECT_DELETEBALLOONINFO()
{
}


/* ========================================================================= */
/* �֐���	:CPacketEFFECT_DELETEBALLOONINFO::Make							 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2007/12/24														 */
/* ========================================================================= */

void CPacketEFFECT_DELETEBALLOONINFO::Make(
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
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_EFFECT_DELETEBALLOONINFO;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwBalloonID, sizeof (dwBalloonID), pDataTmp);	/* ���o��ID */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* �֐���	:CPacketEFFECT_DELETEBALLOONINFO::Set							 */
/* ���e		:�p�P�b�g��ݒ�													 */
/* ���t		:2007/12/24														 */
/* ========================================================================= */

PBYTE CPacketEFFECT_DELETEBALLOONINFO::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwBalloonID, pDataTmp, sizeof (m_dwBalloonID), pDataTmp);	/* ���o��ID */

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2007 */
