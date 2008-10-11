/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:PacketADMIN_CHAR_RENEWMOTIONTYPE.cpp						 */
/* ���e			:�R�}���h(�Ǘ��Ҍn:�L�������[�V������ʏ��̍X�V) �����t�@�C��	 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/05/31													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "LibInfoMotionType.h"
#include "Command.h"
#include "PacketADMIN_CHAR_RENEWMOTIONTYPE.h"

/* ========================================================================= */
/* �֐���	:CPacketADMIN_CHAR_RENEWMOTIONTYPE::CPacketADMIN_CHAR_RENEWMOTIONTYPE	 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/05/31														 */
/* ========================================================================= */

CPacketADMIN_CHAR_RENEWMOTIONTYPE::CPacketADMIN_CHAR_RENEWMOTIONTYPE()
{
	m_dwMotionTypeID = 0;
	m_pInfo			 = NULL;
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_CHAR_RENEWMOTIONTYPE::~CPacketADMIN_CHAR_RENEWMOTIONTYPE	 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/05/31														 */
/* ========================================================================= */

CPacketADMIN_CHAR_RENEWMOTIONTYPE::~CPacketADMIN_CHAR_RENEWMOTIONTYPE()
{
	SAFE_DELETE (m_pInfo);
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_CHAR_RENEWMOTIONTYPE::Make						 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2008/05/31														 */
/* ========================================================================= */

void CPacketADMIN_CHAR_RENEWMOTIONTYPE::Make(
	DWORD dwMotionTypeID,			/* [in] ���[�V�������ID */
	CLibInfoMotionType *pSrc)		/* [in] ���[�V������ʏ�� */
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

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_ADMIN;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ADMIN_CHAR_RENEWMOTIONTYPE;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwMotionTypeID, sizeof (dwMotionTypeID), pDataTmp);	/* ���[�V�������ID */
	CopyMemoryRenew (pDataTmp, pInfoDataTmp,	dwSizeDataTmp,			 pDataTmp);	/* ���[�V������ʏ�� */

	RenewPacket (pData, dwSize);

	SAFE_DELETE_ARRAY (pInfoDataTmp);
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_CHAR_RENEWMOTIONTYPE::Set							 */
/* ���e		:�p�P�b�g��ݒ�													 */
/* ���t		:2008/05/31														 */
/* ========================================================================= */

PBYTE CPacketADMIN_CHAR_RENEWMOTIONTYPE::Set(PBYTE pPacket)
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
