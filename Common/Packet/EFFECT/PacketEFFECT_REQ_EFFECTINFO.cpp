/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketEFFECT_REQ_EFFECTINFO.cpp							 */
/* ���e			:�R�}���h(�G�t�F�N�g�n:�G�t�F�N�g���v��) �����t�@�C��		 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/07/26													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketEFFECT_REQ_EFFECTINFO.h"

/* ========================================================================= */
/* �֐���	:CPacketEFFECT_REQ_EFFECTINFO::CPacketEFFECT_REQ_EFFECTINFO		 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/07/26														 */
/* ========================================================================= */

CPacketEFFECT_REQ_EFFECTINFO::CPacketEFFECT_REQ_EFFECTINFO()
{
	m_dwEffectID = 0;
}


/* ========================================================================= */
/* �֐���	:CPacketEFFECT_REQ_EFFECTINFO::~CPacketEFFECT_REQ_EFFECTINFO	 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/07/26														 */
/* ========================================================================= */

CPacketEFFECT_REQ_EFFECTINFO::~CPacketEFFECT_REQ_EFFECTINFO()
{
}


/* ========================================================================= */
/* �֐���	:CPacketEFFECT_REQ_EFFECTINFO::Make								 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2007/07/26														 */
/* ========================================================================= */

void CPacketEFFECT_REQ_EFFECTINFO::Make(
	DWORD dwEffectID)		/* [in] �G�t�F�N�gID */
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE) +
			 sizeof (dwEffectID);

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_EFFECT;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_EFFECT_REQ_EFFECTINFO;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwEffectID, sizeof (dwEffectID), pDataTmp);	/* �G�t�F�N�gID */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* �֐���	:CPacketEFFECT_REQ_EFFECTINFO::Set								 */
/* ���e		:�p�P�b�g��ݒ�													 */
/* ���t		:2007/07/26														 */
/* ========================================================================= */

PBYTE CPacketEFFECT_REQ_EFFECTINFO::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwEffectID, pDataTmp, sizeof (m_dwEffectID), pDataTmp);	/* �G�t�F�N�gID */

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2007 */
