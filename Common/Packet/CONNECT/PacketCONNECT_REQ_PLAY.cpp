/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* �t�@�C����	:CPacketCONNECT_REQ_PLAY.cpp								 */
/* ���e			:�R�}���h(�ڑ��n:�Q�[���J�n�v��) �����t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2006/12/31													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketCONNECT_REQ_PLAY.h"

/* ========================================================================= */
/* �֐���	:CPacketCONNECT_REQ_PLAY::CPacketCONNECT_REQ_PLAY				 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2006/12/31														 */
/* ========================================================================= */

CPacketCONNECT_REQ_PLAY::CPacketCONNECT_REQ_PLAY()
{
	m_dwAccountID	= 0;
	m_dwCharID		= 0;
}


/* ========================================================================= */
/* �֐���	:CPacketCONNECT_REQ_PLAY::~CPacketCONNECT_REQ_PLAY				 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2006/12/31														 */
/* ========================================================================= */

CPacketCONNECT_REQ_PLAY::~CPacketCONNECT_REQ_PLAY()
{
}


/* ========================================================================= */
/* �֐���	:CPacketCONNECT_REQ_PLAY::Make									 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2006/12/31														 */
/* ========================================================================= */

void CPacketCONNECT_REQ_PLAY::Make(
	DWORD dwAccountID,		/* [in] �A�J�E���gID */
	DWORD dwCharID)			/* [in] �L����ID */
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE)	+
			 sizeof (dwAccountID)	+
			 sizeof (dwCharID);

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_CONNECT;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_CONNECT_REQ_PLAY;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwAccountID,	sizeof (dwAccountID),	pDataTmp);	/* �A�J�E���gID */
	CopyMemoryRenew (pDataTmp, &dwCharID,		sizeof (dwCharID),		pDataTmp);	/* �L����ID */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* �֐���	:CPacketCONNECT_REQ_PLAY::Set									 */
/* ���e		:�p�P�b�g��ݒ�													 */
/* ���t		:2006/12/31														 */
/* ========================================================================= */

PBYTE CPacketCONNECT_REQ_PLAY::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwAccountID,	pDataTmp, sizeof (m_dwAccountID),	pDataTmp);	/* �A�J�E���gID */
	CopyMemoryRenew (&m_dwCharID,		pDataTmp, sizeof (m_dwCharID),		pDataTmp);	/* �L����ID */

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2006 */
