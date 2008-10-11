/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* �t�@�C����	:PacketCONNECT_RES_PLAY.cpp									 */
/* ���e			:�R�}���h(�ڑ��n:�Q�[���J�n����) �����t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2006/12/31													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketCONNECT_RES_PLAY.h"

/* ========================================================================= */
/* �֐���	:CPacketCONNECT_RES_PLAY::CPacketCONNECT_RES_PLAY				 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2006/12/31														 */
/* ========================================================================= */

CPacketCONNECT_RES_PLAY::CPacketCONNECT_RES_PLAY()
{
	m_nResult = PLAYRES_NONE;
}


/* ========================================================================= */
/* �֐���	:CPacketCONNECT_RES_PLAY::~CPacketCONNECT_RES_PLAY				 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2006/12/31														 */
/* ========================================================================= */

CPacketCONNECT_RES_PLAY::~CPacketCONNECT_RES_PLAY()
{
}


/* ========================================================================= */
/* �֐���	:CPacketCONNECT_RES_PLAY::Make									 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2006/12/31														 */
/* ========================================================================= */

void CPacketCONNECT_RES_PLAY::Make(
	int nResult)		/* [in] ���� */
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE)	+
			 sizeof (nResult);

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_CONNECT;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_CONNECT_RES_PLAY;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &nResult, sizeof (nResult), pDataTmp);	/* ���� */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* �֐���	:CPacketCONNECT_RES_PLAY::Set									 */
/* ���e		:�p�P�b�g��ݒ�													 */
/* ���t		:2006/12/31														 */
/* ========================================================================= */

PBYTE CPacketCONNECT_RES_PLAY::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_nResult, pDataTmp, sizeof (m_nResult), pDataTmp);	/* ���� */

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2006 */
