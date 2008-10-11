/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:PacketADMIN_MAP_EVENT.cpp									 */
/* ���e			:�R�}���h(�Ǘ��Ҍn:�}�b�v�C�x���g���ʒm) �����t�@�C��		 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/06/25													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "LibInfoMapEvent.h"
#include "Command.h"
#include "PacketADMIN_MAP_EVENT.h"

/* ========================================================================= */
/* �֐���	:CPacketADMIN_MAP_EVENT::CPacketADMIN_MAP_EVENT					 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/06/25														 */
/* ========================================================================= */

CPacketADMIN_MAP_EVENT::CPacketADMIN_MAP_EVENT()
{
	m_dwMapID	= 0;
	m_pInfo		= NULL;
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_MAP_EVENT::~CPacketADMIN_MAP_EVENT				 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/06/25														 */
/* ========================================================================= */

CPacketADMIN_MAP_EVENT::~CPacketADMIN_MAP_EVENT()
{
	SAFE_DELETE (m_pInfo);
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_MAP_EVENT::Make									 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2008/06/25														 */
/* ========================================================================= */

void CPacketADMIN_MAP_EVENT::Make(
	DWORD dwMapID,					/* [in] �}�b�vID */
	CInfoMapEventBase *pInfo)		/* [in] �C�x���g��� */
{
	PBYTE pData, pDataTmp, pInfoDataTmp;
	DWORD dwSize, dwSizeDataTmp;
	PPACKETBASE pPacketBase;

	dwSizeDataTmp	= pInfo->GetSendDataSize ();
	pInfoDataTmp	= pInfo->GetSendData ();

	dwSize = sizeof (PACKETBASE)	+
			 sizeof (dwMapID)		+
			 dwSizeDataTmp;

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_ADMIN;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ADMIN_MAP_RENEWEVENT;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwMapID,		sizeof (dwMapID),	pDataTmp);	/* �}�b�vID */
	CopyMemoryRenew (pDataTmp, pInfoDataTmp,	dwSizeDataTmp,		pDataTmp);	/* �C�x���g��� */

	RenewPacket (pData, dwSize);

	SAFE_DELETE_ARRAY (pInfoDataTmp);
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_MAP_EVENT::Set									 */
/* ���e		:�p�P�b�g��ݒ�													 */
/* ���t		:2008/06/25														 */
/* ========================================================================= */

PBYTE CPacketADMIN_MAP_EVENT::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp, pDataTmpBack;
	CLibInfoMapEvent LibInfo;
	CInfoMapEventBase InfoTmp;

	pRet		 = pPacket;
	pDataTmp	 = CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwMapID, pDataTmp, sizeof (m_dwMapID), pDataTmp);	/* �}�b�vID */
	pDataTmpBack = pDataTmp;
	InfoTmp.SetSendData (pDataTmp);

	SAFE_DELETE (m_pInfo);
	m_pInfo		= (PCInfoMapEventBase)LibInfo.GetNew (InfoTmp.m_nType);
	pDataTmp	= m_pInfo->SetSendData (pDataTmpBack);

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2008 */
