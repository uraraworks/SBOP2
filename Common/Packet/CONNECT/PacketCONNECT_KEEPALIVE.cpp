/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:PacketCONNECT_KEEPALIVE.cpp								 */
/* ���e			:�R�}���h(�ڑ��n:�����m�F�ʒm) �����t�@�C��					 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/06/21													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketCONNECT_KEEPALIVE.h"

/* ========================================================================= */
/* �֐���	:CPacketCONNECT_KEEPALIVE::CPacketCONNECT_KEEPALIVE				 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/06/21														 */
/* ========================================================================= */

CPacketCONNECT_KEEPALIVE::CPacketCONNECT_KEEPALIVE()
{
	m_dwData = 0;
}


/* ========================================================================= */
/* �֐���	:CPacketCONNECT_KEEPALIVE::~CPacketCONNECT_KEEPALIVE			 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/06/21														 */
/* ========================================================================= */

CPacketCONNECT_KEEPALIVE::~CPacketCONNECT_KEEPALIVE()
{
}


/* ========================================================================= */
/* �֐���	:CPacketCONNECT_KEEPALIVE::Make									 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2008/06/21														 */
/* ========================================================================= */

void CPacketCONNECT_KEEPALIVE::Make(DWORD dwData)
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE);
	dwSize += sizeof (dwData);

	pData = ZeroNew (dwSize);
	pPacketBase = (PPACKETBASE)pData;
	pDataTmp = pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_CONNECT;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_CONNECT_KEEPALIVE;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwData, sizeof (dwData), pDataTmp);	/* �ԐM�p�f�[�^ */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* �֐���	:CPacketCONNECT_KEEPALIVE::Set									 */
/* ���e		:�p�P�b�g��ݒ�													 */
/* ���t		:2008/06/21														 */
/* ========================================================================= */

PBYTE CPacketCONNECT_KEEPALIVE::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwData, pDataTmp, sizeof (m_dwData), pDataTmp);	/* �ԐM�p�f�[�^ */

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2008 */
