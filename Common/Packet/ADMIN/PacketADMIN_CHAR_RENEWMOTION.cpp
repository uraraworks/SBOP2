/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketADMIN_CHAR_RENEWMOTION.cpp							 */
/* ���e			:�R�}���h(�Ǘ��Ҍn:�L�������[�V�������̍X�V) �����t�@�C��	 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/11/23													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "LibInfoMotion.h"
#include "Command.h"
#include "PacketADMIN_CHAR_RENEWMOTION.h"

/* ========================================================================= */
/* �֐���	:CPacketADMIN_CHAR_RENEWMOTION::CPacketADMIN_CHAR_RENEWMOTION	 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/11/23														 */
/* ========================================================================= */

CPacketADMIN_CHAR_RENEWMOTION::CPacketADMIN_CHAR_RENEWMOTION()
{
	m_dwMotionTypeID = 0;
	m_dwMotionListID = 0;
	m_pInfo			 = NULL;
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_CHAR_RENEWMOTION::~CPacketADMIN_CHAR_RENEWMOTION	 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/11/23														 */
/* ========================================================================= */

CPacketADMIN_CHAR_RENEWMOTION::~CPacketADMIN_CHAR_RENEWMOTION()
{
	SAFE_DELETE (m_pInfo);
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_CHAR_RENEWMOTION::Make							 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2007/11/23														 */
/* ========================================================================= */

void CPacketADMIN_CHAR_RENEWMOTION::Make(
	DWORD dwMotionTypeID,		/* [in] ���[�V�������ID */
	DWORD dwMotionListID,		/* [in] ���[�V�������X�gID */
	CLibInfoMotion *pSrc)		/* [in] ���[�V������� */
{
	PBYTE pData, pDataTmp, pInfoDataTmp;
	DWORD dwSize, dwSizeDataTmp;
	PPACKETBASE pPacketBase;

	dwSizeDataTmp	= pSrc->GetSendDataSize (dwMotionTypeID, dwMotionListID);
	pInfoDataTmp	= pSrc->GetSendData (dwMotionTypeID, dwMotionListID);

	dwSize = sizeof (PACKETBASE) +
			 sizeof (dwMotionTypeID) +
			 sizeof (dwMotionListID) +
			 dwSizeDataTmp;

	pData = ZeroNew (dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_ADMIN;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ADMIN_CHAR_RENEWMOTION;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwMotionTypeID, sizeof (dwMotionTypeID), pDataTmp);	/* ���[�V�������ID */
	CopyMemoryRenew (pDataTmp, &dwMotionListID, sizeof (dwMotionListID), pDataTmp);	/* ���[�V�������X�gID */
	CopyMemoryRenew (pDataTmp, pInfoDataTmp,	dwSizeDataTmp,			 pDataTmp);	/* ���[�V������� */

	RenewPacket (pData, dwSize);

	SAFE_DELETE_ARRAY (pInfoDataTmp);
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_CHAR_RENEWMOTION::Set								 */
/* ���e		:�p�P�b�g��ݒ�													 */
/* ���t		:2007/11/23														 */
/* ========================================================================= */

PBYTE CPacketADMIN_CHAR_RENEWMOTION::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwMotionTypeID, pDataTmp, sizeof (m_dwMotionTypeID), pDataTmp);	/* ���[�V�������ID */
	CopyMemoryRenew (&m_dwMotionListID, pDataTmp, sizeof (m_dwMotionListID), pDataTmp);	/* ���[�V�������X�gID */

	SAFE_DELETE (m_pInfo);
	m_pInfo		= new CLibInfoMotion;
	m_pInfo->Create ();
	pDataTmp	= m_pInfo->SetSendData (pDataTmp);

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2007 */
