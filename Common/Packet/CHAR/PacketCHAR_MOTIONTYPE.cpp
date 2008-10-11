/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:PacketCHAR_MOTIONTYPE.cpp									 */
/* ���e			:�R�}���h(�Ǘ��Ҍn:���[�V������ʏ��ʒm) �����t�@�C��		 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/05/31													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "LibInfoMotionType.h"
#include "Command.h"
#include "PacketCHAR_MOTIONTYPE.h"

/* ========================================================================= */
/* �֐���	:CPacketCHAR_MOTIONTYPE::CPacketCHAR_MOTIONTYPE					 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/05/31														 */
/* ========================================================================= */

CPacketCHAR_MOTIONTYPE::CPacketCHAR_MOTIONTYPE()
{
	m_dwMotionTypeID = 0;
	m_pInfo			 = NULL;
}


/* ========================================================================= */
/* �֐���	:CPacketCHAR_MOTIONTYPE::~CPacketCHAR_MOTIONTYPE				 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/05/31														 */
/* ========================================================================= */

CPacketCHAR_MOTIONTYPE::~CPacketCHAR_MOTIONTYPE()
{
	SAFE_DELETE (m_pInfo);
}


/* ========================================================================= */
/* �֐���	:CPacketCHAR_MOTIONTYPE::Make									 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2008/05/31														 */
/* ========================================================================= */

void CPacketCHAR_MOTIONTYPE::Make(
	DWORD dwMotionTypeID,			/* [in] ���[�V�������ID */
	CLibInfoMotionType *pSrc)		/* [in] ���[�V������� */
{
	PBYTE pData, pDataTmp, pInfoDataTmp;
	DWORD dwSize, dwSizeDataTmp;
	PPACKETBASE pPacketBase;

	dwSizeDataTmp	= pSrc->GetSendDataSize (dwMotionTypeID);
	pInfoDataTmp	= pSrc->GetSendData (dwMotionTypeID);

	dwSize = sizeof (PACKETBASE) +
			 sizeof (dwMotionTypeID) +
			 dwSizeDataTmp;

	pData = ZeroNew (dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_CHAR;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_CHAR_MOTIONTYPE;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwMotionTypeID, sizeof (dwMotionTypeID), pDataTmp);	/* ���[�V�������ID */
	CopyMemoryRenew (pDataTmp, pInfoDataTmp,	dwSizeDataTmp,			 pDataTmp);	/* ���[�V������ʏ�� */

	RenewPacket (pData, dwSize);

	SAFE_DELETE_ARRAY (pInfoDataTmp);
}


/* ========================================================================= */
/* �֐���	:CPacketCHAR_MOTIONTYPE::Set									 */
/* ���e		:�p�P�b�g��ݒ�													 */
/* ���t		:2008/05/31														 */
/* ========================================================================= */

PBYTE CPacketCHAR_MOTIONTYPE::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwMotionTypeID, pDataTmp, sizeof (m_dwMotionTypeID), pDataTmp);	/* ���[�V�������ID */

	SAFE_DELETE (m_pInfo);
	m_pInfo		= new CLibInfoMotionType;
	m_pInfo->Create ();
	pDataTmp	= m_pInfo->SetSendData (pDataTmp);

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2008 */
