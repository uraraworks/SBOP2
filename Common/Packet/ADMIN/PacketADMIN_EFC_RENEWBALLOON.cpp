/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketADMIN_EFC_RENEWBALLOON.cpp							 */
/* ���e			:�R�}���h(�Ǘ��Ҍn:���o�����̍X�V) �����t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/12/30													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "LibInfoEfcBalloon.h"
#include "Command.h"
#include "PacketADMIN_EFC_RENEWBALLOON.h"

/* ========================================================================= */
/* �֐���	:CPacketADMIN_EFC_RENEWBALLOON::CPacketADMIN_EFC_RENEWBALLOON	 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/12/30														 */
/* ========================================================================= */

CPacketADMIN_EFC_RENEWBALLOON::CPacketADMIN_EFC_RENEWBALLOON()
{
	m_dwListID	= 0;
	m_pInfo		= NULL;
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_EFC_RENEWBALLOON::~CPacketADMIN_EFC_RENEWBALLOON	 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/12/30														 */
/* ========================================================================= */

CPacketADMIN_EFC_RENEWBALLOON::~CPacketADMIN_EFC_RENEWBALLOON()
{
	SAFE_DELETE (m_pInfo);
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_EFC_RENEWBALLOON::Make							 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2007/12/30														 */
/* ========================================================================= */

void CPacketADMIN_EFC_RENEWBALLOON::Make(
	DWORD dwListID,					/* [in] ���ID */
	CLibInfoEfcBalloon *pSrc)		/* [in] ���o����� */
{
	PBYTE pData, pDataTmp, pInfoDataTmp;
	DWORD dwSize, dwSizeDataTmp;
	PPACKETBASE pPacketBase;

	dwSizeDataTmp	= pSrc->GetSendDataSize (dwListID);
	pInfoDataTmp	= pSrc->GetSendData (dwListID);

	dwSize = sizeof (PACKETBASE) +
			 sizeof (dwListID) +
			 dwSizeDataTmp;

	pData = ZeroNew (dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_ADMIN;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ADMIN_EFC_RENEWBALLOON;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwListID,		sizeof (dwListID),	pDataTmp);	/* ���ID */
	CopyMemoryRenew (pDataTmp, pInfoDataTmp,	dwSizeDataTmp,	 	pDataTmp);	/* ���o����� */

	RenewPacket (pData, dwSize);

	SAFE_DELETE_ARRAY (pInfoDataTmp);
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_EFC_RENEWBALLOON::Set								 */
/* ���e		:�p�P�b�g��ݒ�													 */
/* ���t		:2007/12/30														 */
/* ========================================================================= */

PBYTE CPacketADMIN_EFC_RENEWBALLOON::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwListID, pDataTmp, sizeof (m_dwListID), pDataTmp);	/* ���ID */

	SAFE_DELETE (m_pInfo);
	m_pInfo		= new CLibInfoEfcBalloon;
	m_pInfo->Create ();
	pDataTmp	= m_pInfo->SetSendData (pDataTmp);

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2007 */
