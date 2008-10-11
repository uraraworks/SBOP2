/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:PacketADMIN_EFC_RENEWEFFECT.cpp							 */
/* ���e			:�R�}���h(�Ǘ��Ҍn:�G�t�F�N�g���̍X�V) �����t�@�C��		 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/07/06													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "InfoEffect.h"
#include "Command.h"
#include "PacketADMIN_EFC_RENEWEFFECT.h"

/* ========================================================================= */
/* �֐���	:CPacketADMIN_EFC_RENEWEFFECT::CPacketADMIN_EFC_RENEWEFFECT		 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/07/06														 */
/* ========================================================================= */

CPacketADMIN_EFC_RENEWEFFECT::CPacketADMIN_EFC_RENEWEFFECT()
{
	m_dwEffectID	= 0;
	m_pInfo			= NULL;
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_EFC_RENEWEFFECT::~CPacketADMIN_EFC_RENEWEFFECT	 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/07/06														 */
/* ========================================================================= */

CPacketADMIN_EFC_RENEWEFFECT::~CPacketADMIN_EFC_RENEWEFFECT()
{
	SAFE_DELETE (m_pInfo);
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_EFC_RENEWEFFECT::Make								 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2008/07/06														 */
/* ========================================================================= */

void CPacketADMIN_EFC_RENEWEFFECT::Make(
	DWORD dwEffectID,		/* [in] �G�t�F�N�gID */
	CInfoEffect *pSrc)		/* [in] �G�t�F�N�g��� */
{
	PBYTE pData, pDataTmp, pInfoDataTmp;
	DWORD dwSize, dwSizeDataTmp;
	PPACKETBASE pPacketBase;

	dwSizeDataTmp	= pSrc->GetSendDataSize ();
	pInfoDataTmp	= pSrc->GetSendData ();

	dwSize = sizeof (PACKETBASE) +
			 sizeof (dwEffectID) +
			 dwSizeDataTmp;

	pData = ZeroNew (dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_ADMIN;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ADMIN_EFC_RENEWEFFECT;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwEffectID,		sizeof (dwEffectID),	pDataTmp);	/* �G�t�F�N�gID */
	CopyMemoryRenew (pDataTmp, pInfoDataTmp,	dwSizeDataTmp,	 		pDataTmp);	/* �G�t�F�N�g��� */

	RenewPacket (pData, dwSize);

	SAFE_DELETE_ARRAY (pInfoDataTmp);
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_EFC_RENEWEFFECT::Set								 */
/* ���e		:�p�P�b�g��ݒ�													 */
/* ���t		:2008/07/06														 */
/* ========================================================================= */

PBYTE CPacketADMIN_EFC_RENEWEFFECT::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwEffectID, pDataTmp, sizeof (m_dwEffectID), pDataTmp);	/* �G�t�F�N�gID */

	SAFE_DELETE (m_pInfo);
	m_pInfo		= new CInfoEffect;
	pDataTmp	= m_pInfo->SetSendData (pDataTmp);

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2008 */
