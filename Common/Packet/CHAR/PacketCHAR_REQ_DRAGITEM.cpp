/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:PacketCHAR_REQ_DRAGITEM.cpp								 */
/* ���e			:�R�}���h(�L�����n:�A�C�e���ʒu�ύX�v��) �����t�@�C��		 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/11/22													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketCHAR_REQ_DRAGITEM.h"

/* ========================================================================= */
/* �֐���	:CPacketCHAR_REQ_DRAGITEM::CPacketCHAR_REQ_DRAGITEM				 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/11/22														 */
/* ========================================================================= */

CPacketCHAR_REQ_DRAGITEM::CPacketCHAR_REQ_DRAGITEM()
{
	m_dwCharID = 0;
	m_dwItemID = 0;
	m_ptNewPos.x = m_ptNewPos.y = 0;
}


/* ========================================================================= */
/* �֐���	:CPacketCHAR_REQ_DRAGITEM::~CPacketCHAR_REQ_DRAGITEM			 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/11/22														 */
/* ========================================================================= */

CPacketCHAR_REQ_DRAGITEM::~CPacketCHAR_REQ_DRAGITEM()
{
}


/* ========================================================================= */
/* �֐���	:CPacketCHAR_REQ_DRAGITEM::Make									 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2008/11/22														 */
/* ========================================================================= */

void CPacketCHAR_REQ_DRAGITEM::Make(
	DWORD dwCharID,		/* [in] �L����ID */
	DWORD dwItemID,		/* [in] �A�C�e��ID */
	POINT ptNewPos)		/* [in] �V�����ʒu */
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE)	+
			 sizeof (dwCharID)		+
			 sizeof (dwItemID)		+
			 sizeof (ptNewPos);

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_CHAR;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_CHAR_REQ_DRAGITEM;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwCharID, sizeof (dwCharID), pDataTmp);	/* �L����ID */
	CopyMemoryRenew (pDataTmp, &dwItemID, sizeof (dwItemID), pDataTmp);	/* �A�C�e��ID */
	CopyMemoryRenew (pDataTmp, &ptNewPos, sizeof (ptNewPos), pDataTmp);	/* �V�����ʒu */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* �֐���	:CPacketCHAR_REQ_DRAGITEM::Set									 */
/* ���e		:�p�P�b�g��ݒ�													 */
/* ���t		:2008/11/22														 */
/* ========================================================================= */

PBYTE CPacketCHAR_REQ_DRAGITEM::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwCharID, pDataTmp, sizeof (m_dwCharID), pDataTmp);	/* �L����ID */
	CopyMemoryRenew (&m_dwItemID, pDataTmp, sizeof (m_dwItemID), pDataTmp);	/* �A�C�e��ID */
	CopyMemoryRenew (&m_ptNewPos, pDataTmp, sizeof (m_ptNewPos), pDataTmp);	/* �V�����ʒu */

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2008 */
