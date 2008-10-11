/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketCHAR_SET_EFFECT.cpp									 */
/* ���e			:�R�}���h(�L�����n:�G�t�F�N�g���w��) �����t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/07/27													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketCHAR_SET_EFFECT.h"

/* ========================================================================= */
/* �֐���	:CPacketCHAR_SET_EFFECT::CPacketCHAR_SET_EFFECT					 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/07/27														 */
/* ========================================================================= */

CPacketCHAR_SET_EFFECT::CPacketCHAR_SET_EFFECT()
{
	m_dwCharID		= 0;
	m_dwEffectID	= 0;
}


/* ========================================================================= */
/* �֐���	:CPacketCHAR_SET_EFFECT::~CPacketCHAR_SET_EFFECT				 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/07/27														 */
/* ========================================================================= */

CPacketCHAR_SET_EFFECT::~CPacketCHAR_SET_EFFECT()
{
}


/* ========================================================================= */
/* �֐���	:CPacketCHAR_SET_EFFECT::Make									 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2007/07/27														 */
/* ========================================================================= */

void CPacketCHAR_SET_EFFECT::Make(
	DWORD dwCharID,			/* [in] �L����ID */
	DWORD dwEffectID)		/* [in] �G�t�F�N�gID */
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE)	+
			 sizeof (dwCharID)		+
			 sizeof (dwEffectID);

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_CHAR;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_CHAR_SET_EFFECT;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwCharID,	sizeof (dwCharID),		pDataTmp);	/* �L����ID */
	CopyMemoryRenew (pDataTmp, &dwEffectID,	sizeof (dwEffectID),	pDataTmp);	/* �G�t�F�N�gID */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* �֐���	:CPacketCHAR_SET_EFFECT::Set									 */
/* ���e		:�p�P�b�g��ݒ�													 */
/* ���t		:2007/07/27														 */
/* ========================================================================= */

PBYTE CPacketCHAR_SET_EFFECT::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwCharID,	pDataTmp, sizeof (m_dwCharID),		pDataTmp);	/* �L����ID */
	CopyMemoryRenew (&m_dwEffectID,	pDataTmp, sizeof (m_dwEffectID),	pDataTmp);	/* �G�t�F�N�gID */

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2007 */
