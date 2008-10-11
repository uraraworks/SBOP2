/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketADMIN_RENEWADMINLEVEL.cpp							 */
/* ���e			:�R�}���h(�Ǘ��Ҍn:�Ǘ��Ҍ������x���X�V) �����t�@�C��		 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/07/05													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketADMIN_RENEWADMINLEVEL.h"

/* ========================================================================= */
/* �֐���	:CPacketADMIN_RENEWADMINLEVEL::CPacketADMIN_RENEWADMINLEVEL		 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/07/05														 */
/* ========================================================================= */

CPacketADMIN_RENEWADMINLEVEL::CPacketADMIN_RENEWADMINLEVEL()
{
	m_dwAccountID = 0;
	m_nAdminLevel = 0;
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_RENEWADMINLEVEL::~CPacketADMIN_RENEWADMINLEVEL	 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/07/05														 */
/* ========================================================================= */

CPacketADMIN_RENEWADMINLEVEL::~CPacketADMIN_RENEWADMINLEVEL()
{
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_RENEWADMINLEVEL::Make								 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2007/07/05														 */
/* ========================================================================= */

void CPacketADMIN_RENEWADMINLEVEL::Make(
	DWORD dwAccountID,		/* [in] �A�J�E���gID */
	int nLevel)				/* [in] �Ǘ��҃��x�� */
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE)  +
			 sizeof (dwAccountID) +
			 sizeof (nLevel);

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_ADMIN;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ADMIN_RENEWADMINLEVEL;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwAccountID,	sizeof (dwAccountID),	pDataTmp);	/* �A�J�E���gID */
	CopyMemoryRenew (pDataTmp, &nLevel,			sizeof (nLevel),		pDataTmp);	/* �Ǘ��҃��x�� */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_RENEWADMINLEVEL::Set								 */
/* ���e		:�p�P�b�g��ݒ�													 */
/* ���t		:2007/07/05														 */
/* ========================================================================= */

PBYTE CPacketADMIN_RENEWADMINLEVEL::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwAccountID, pDataTmp, sizeof (m_dwAccountID), pDataTmp);	/* �A�J�E���gID */
	CopyMemoryRenew (&m_nAdminLevel, pDataTmp, sizeof (m_nAdminLevel), pDataTmp);	/* �Ǘ��҃��x�� */

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2007 */
