/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketCHAR_REQ_TAIL.cpp									 */
/* ���e			:�R�}���h(�L�����n:�t���čs���v��) �����t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/09/04													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketCHAR_REQ_TAIL.h"

/* ========================================================================= */
/* �֐���	:CPacketCHAR_REQ_TAIL::CPacketCHAR_REQ_TAIL						 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/09/04														 */
/* ========================================================================= */

CPacketCHAR_REQ_TAIL::CPacketCHAR_REQ_TAIL()
{
	m_bTail				= TRUE;
	m_dwCharID			= 0;
	m_dwTargetCharID	= 0;
}


/* ========================================================================= */
/* �֐���	:CPacketCHAR_REQ_TAIL::~CPacketCHAR_REQ_TAIL					 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/09/04														 */
/* ========================================================================= */

CPacketCHAR_REQ_TAIL::~CPacketCHAR_REQ_TAIL()
{
}


/* ========================================================================= */
/* �֐���	:CPacketCHAR_REQ_TAIL::Make										 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2007/09/04														 */
/* ========================================================================= */

void CPacketCHAR_REQ_TAIL::Make(
	DWORD dwCharID,				/* [in] �L����ID */
	DWORD dwTargetCharID,		/* [in] �t���čs������̃L����ID */
	BOOL bTail)					/* [in] TRUE:���Ă��� FALSE:���� */
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE)	 +
			 sizeof (dwCharID)		 +
			 sizeof (dwTargetCharID) +
			 sizeof (bTail);

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_CHAR;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_CHAR_REQ_TAIL;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwCharID,		sizeof (dwCharID),			pDataTmp);	/* �L����ID */
	CopyMemoryRenew (pDataTmp, &dwTargetCharID,	sizeof (dwTargetCharID),	pDataTmp);	/* �t���čs������̃L����ID */
	CopyMemoryRenew (pDataTmp, &bTail,			sizeof (bTail),				pDataTmp);	/* ���Ă����������� */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* �֐���	:CPacketCHAR_REQ_TAIL::Set										 */
/* ���e		:�p�P�b�g��ݒ�													 */
/* ���t		:2007/09/04														 */
/* ========================================================================= */

PBYTE CPacketCHAR_REQ_TAIL::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwCharID,		pDataTmp, sizeof (m_dwCharID),			pDataTmp);	/* �L����ID */
	CopyMemoryRenew (&m_dwTargetCharID,	pDataTmp, sizeof (m_dwTargetCharID),	pDataTmp);	/* �t���čs������̃L����ID */
	CopyMemoryRenew (&m_bTail,			pDataTmp, sizeof (m_bTail),				pDataTmp);	/* ���Ă����������� */

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2007 */
