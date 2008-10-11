/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketCHAR_REQ_PUSH.cpp									 */
/* ���e			:�R�}���h(�L�����n:�����v��) �����t�@�C��					 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/09/02													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketCHAR_REQ_PUSH.h"

/* ========================================================================= */
/* �֐���	:CPacketCHAR_REQ_PUSH::CPacketCHAR_REQ_PUSH						 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/09/02														 */
/* ========================================================================= */

CPacketCHAR_REQ_PUSH::CPacketCHAR_REQ_PUSH()
{
	m_dwCharID		= 0;
	m_nDirection	= 0;
	m_nPushCount	= 1;
}


/* ========================================================================= */
/* �֐���	:CPacketCHAR_REQ_PUSH::~CPacketCHAR_REQ_PUSH					 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/09/02														 */
/* ========================================================================= */

CPacketCHAR_REQ_PUSH::~CPacketCHAR_REQ_PUSH()
{
}


/* ========================================================================= */
/* �֐���	:CPacketCHAR_REQ_PUSH::Make										 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2007/09/02														 */
/* ========================================================================= */

void CPacketCHAR_REQ_PUSH::Make(
	DWORD dwCharID,				/* [in] �L����ID */
	int nDirection,				/* [in] ���� */
	int nPushCount)/*=1*/		/* [in] �����Đi�ސ� */
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE)	+
			 sizeof (dwCharID)		+
			 sizeof (nDirection)	+
			 sizeof (nPushCount);

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_CHAR;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_CHAR_REQ_PUSH;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwCharID,	sizeof (dwCharID),		pDataTmp);	/* �L����ID */
	CopyMemoryRenew (pDataTmp, &nDirection,	sizeof (nDirection),	pDataTmp);	/* ���� */
	CopyMemoryRenew (pDataTmp, &nPushCount,	sizeof (nPushCount),	pDataTmp);	/* �����Đi�ސ� */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* �֐���	:CPacketCHAR_REQ_PUSH::Set										 */
/* ���e		:�p�P�b�g��ݒ�													 */
/* ���t		:2007/09/02														 */
/* ========================================================================= */

PBYTE CPacketCHAR_REQ_PUSH::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwCharID,	pDataTmp, sizeof (m_dwCharID),		pDataTmp);	/* �L����ID */
	CopyMemoryRenew (&m_nDirection,	pDataTmp, sizeof (m_nDirection),	pDataTmp);	/* ���� */
	CopyMemoryRenew (&m_nPushCount,	pDataTmp, sizeof (m_nPushCount),	pDataTmp);	/* �����Đi�ސ� */

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2007 */
