/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketCHAR_RES_PUTGET.cpp									 */
/* ���e			:�R�}���h(�L�����n:�A�C�e�����E���u������) �����t�@�C��		 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/09/08													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketCHAR_RES_PUTGET.h"

/* ========================================================================= */
/* �֐���	:CPacketCHAR_RES_PUTGET::CPacketCHAR_RES_PUTGET					 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/09/08														 */
/* ========================================================================= */

CPacketCHAR_RES_PUTGET::CPacketCHAR_RES_PUTGET()
{
	m_dwItemID	= 0;
	m_nResult	= RESULTID_CHAR_RES_PUTGET_NONE;
}


/* ========================================================================= */
/* �֐���	:CPacketCHAR_RES_PUTGET::~CPacketCHAR_RES_PUTGET				 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/09/08														 */
/* ========================================================================= */

CPacketCHAR_RES_PUTGET::~CPacketCHAR_RES_PUTGET()
{
}


/* ========================================================================= */
/* �֐���	:CPacketCHAR_RES_PUTGET::Make									 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2007/09/08														 */
/* ========================================================================= */

void CPacketCHAR_RES_PUTGET::Make(
	DWORD dwItemID,		/* [in] �A�C�e��ID */
	int nResult)		/* [in] ���� */
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE)	+
			 sizeof (dwItemID)		+
			 sizeof (nResult);

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_CHAR;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_CHAR_RES_PUTGET;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwItemID,	sizeof (dwItemID), pDataTmp);	/* �A�C�e��ID */
	CopyMemoryRenew (pDataTmp, &nResult,	 sizeof (nResult), pDataTmp);	/* ���� */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* �֐���	:CPacketCHAR_RES_PUTGET::Set									 */
/* ���e		:�p�P�b�g��ݒ�													 */
/* ���t		:2007/09/08														 */
/* ========================================================================= */

PBYTE CPacketCHAR_RES_PUTGET::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwItemID,	pDataTmp, sizeof (m_dwItemID),	pDataTmp);	/* �A�C�e��ID */
	CopyMemoryRenew (&m_nResult,	pDataTmp, sizeof (m_nResult),	pDataTmp);	/* ���� */

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2007 */
