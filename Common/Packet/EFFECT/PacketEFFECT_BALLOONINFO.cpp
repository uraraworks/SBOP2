/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketEFFECT_BALLOONINFO.cpp								 */
/* ���e			:�R�}���h(�G�t�F�N�g�n:���o����񉞓�) �����t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/12/24													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "LibInfoEfcBalloon.h"
#include "Command.h"
#include "PacketEFFECT_BALLOONINFO.h"

/* ========================================================================= */
/* �֐���	:CPacketEFFECT_BALLOONINFO::CPacketEFFECT_BALLOONINFO			 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/12/24														 */
/* ========================================================================= */

CPacketEFFECT_BALLOONINFO::CPacketEFFECT_BALLOONINFO()
{
	m_dwListID	= 0;
	m_pLibInfo	= NULL;
}


/* ========================================================================= */
/* �֐���	:CPacketEFFECT_BALLOONINFO::~CPacketEFFECT_BALLOONINFO			 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/12/24														 */
/* ========================================================================= */

CPacketEFFECT_BALLOONINFO::~CPacketEFFECT_BALLOONINFO()
{
	SAFE_DELETE (m_pLibInfo);
}


/* ========================================================================= */
/* �֐���	:CPacketEFFECT_BALLOONINFO::Make								 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2007/12/24														 */
/* ========================================================================= */

void CPacketEFFECT_BALLOONINFO::Make(
	DWORD dwListID,						/* [in] ���ID */
	CLibInfoEfcBalloon *pLibInfo)		/* [in] ���o����� */
{
	PBYTE pData, pDataTmp, pInfoDataTmp;
	DWORD dwSize, dwSizeDataTmp;
	PPACKETBASE pPacketBase;

	dwSizeDataTmp	= pLibInfo->GetSendDataSize (dwListID);
	pInfoDataTmp	= pLibInfo->GetSendData (dwListID);

	dwSize = sizeof (PACKETBASE) +
			 sizeof (dwListID) +
			 dwSizeDataTmp;

	pData = ZeroNew (dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_EFFECT;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_EFFECT_BALLOONINFO;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwListID,		sizeof (dwListID),	pDataTmp);	/* ���ID */
	CopyMemoryRenew (pDataTmp, pInfoDataTmp,	dwSizeDataTmp,	 	pDataTmp);	/* ���o����� */

	RenewPacket (pData, dwSize);

	SAFE_DELETE_ARRAY (pInfoDataTmp);
}


/* ========================================================================= */
/* �֐���	:CPacketEFFECT_BALLOONINFO::Set									 */
/* ���e		:�p�P�b�g��ݒ�													 */
/* ���t		:2007/12/24														 */
/* ========================================================================= */

PBYTE CPacketEFFECT_BALLOONINFO::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwListID, pDataTmp, sizeof (m_dwListID), pDataTmp);	/* ���ID */

	SAFE_DELETE (m_pLibInfo);
	m_pLibInfo	= new CLibInfoEfcBalloon;
	m_pLibInfo->Create ();
	pDataTmp	= m_pLibInfo->SetSendData (pDataTmp);

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2007 */
