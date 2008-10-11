/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:PacketCHAR_REQ_USEITEM.cpp									 */
/* ���e			:�R�}���h(�L�����n:�A�C�e�����g���v��) �����t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/07/05													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketCHAR_REQ_USEITEM.h"

/* ========================================================================= */
/* �֐���	:CPacketCHAR_REQ_USEITEM::CPacketCHAR_REQ_USEITEM				 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/07/05														 */
/* ========================================================================= */

CPacketCHAR_REQ_USEITEM::CPacketCHAR_REQ_USEITEM()
{
	m_dwCharID = 0;
	m_dwItemID = 0;
}


/* ========================================================================= */
/* �֐���	:CPacketCHAR_REQ_USEITEM::~CPacketCHAR_REQ_USEITEM				 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/07/05														 */
/* ========================================================================= */

CPacketCHAR_REQ_USEITEM::~CPacketCHAR_REQ_USEITEM()
{
}


/* ========================================================================= */
/* �֐���	:CPacketCHAR_REQ_USEITEM::Make									 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2008/07/05														 */
/* ========================================================================= */

void CPacketCHAR_REQ_USEITEM::Make(
	DWORD dwCharID,		/* [in] �L����ID */
	DWORD dwItemID)		/* [in] �A�C�e��ID */
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE)	+
			 sizeof (dwCharID)		+
			 sizeof (dwItemID);

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_CHAR;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_CHAR_REQ_USEITEM;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwCharID, sizeof (dwCharID), pDataTmp);	/* �L����ID */
	CopyMemoryRenew (pDataTmp, &dwItemID, sizeof (dwItemID), pDataTmp);	/* �A�C�e��ID */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* �֐���	:CPacketCHAR_REQ_USEITEM::Set									 */
/* ���e		:�p�P�b�g��ݒ�													 */
/* ���t		:2008/07/05														 */
/* ========================================================================= */

PBYTE CPacketCHAR_REQ_USEITEM::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwCharID, pDataTmp, sizeof (m_dwCharID), pDataTmp);	/* �L����ID */
	CopyMemoryRenew (&m_dwItemID, pDataTmp, sizeof (m_dwItemID), pDataTmp);	/* �A�C�e��ID */

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2008 */
