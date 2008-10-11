/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* �t�@�C����	:PacketCHAR_REQ_CHARINFO.cpp								 */
/* ���e			:�R�}���h(�L�����n:�L�������v��) �����t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2006/12/31													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketCHAR_REQ_CHARINFO.h"

/* ========================================================================= */
/* �֐���	:CPacketCHAR_REQ_CHARINFO::CPacketCHAR_REQ_CHARINFO				 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2006/12/31														 */
/* ========================================================================= */

CPacketCHAR_REQ_CHARINFO::CPacketCHAR_REQ_CHARINFO()
{
	m_dwCharID = 0;
}


/* ========================================================================= */
/* �֐���	:CPacketCHAR_REQ_CHARINFO::~CPacketCHAR_REQ_CHARINFO			 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2006/12/31														 */
/* ========================================================================= */

CPacketCHAR_REQ_CHARINFO::~CPacketCHAR_REQ_CHARINFO()
{
}


/* ========================================================================= */
/* �֐���	:CPacketCHAR_REQ_CHARINFO::Make									 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2006/12/31														 */
/* ========================================================================= */

void CPacketCHAR_REQ_CHARINFO::Make(
	DWORD dwCharID)		/* [in] �L����ID */
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE) +
			 sizeof (dwCharID);

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_CHAR;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_CHAR_REQ_CHARINFO;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwCharID, sizeof (dwCharID), pDataTmp);	/* �L����ID */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* �֐���	:CPacketCHAR_REQ_CHARINFO::Set									 */
/* ���e		:�p�P�b�g��ݒ�													 */
/* ���t		:2006/12/31														 */
/* ========================================================================= */

PBYTE CPacketCHAR_REQ_CHARINFO::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwCharID, pDataTmp, sizeof (m_dwCharID), pDataTmp);	/* �L����ID */

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2006 */
