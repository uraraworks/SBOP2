/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketEFFECT_EFFECTINFO.cpp								 */
/* ���e			:�R�}���h(�G�t�F�N�g�n:�G�t�F�N�g���ʒm) �����t�@�C��		 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/07/26													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketEFFECT_EFFECTINFO.h"

/* ========================================================================= */
/* �֐���	:CPacketEFFECT_EFFECTINFO::CPacketEFFECT_EFFECTINFO				 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/07/26														 */
/* ========================================================================= */

CPacketEFFECT_EFFECTINFO::CPacketEFFECT_EFFECTINFO()
{
	m_pLibInfo = NULL;
}


/* ========================================================================= */
/* �֐���	:CPacketEFFECT_EFFECTINFO::~CPacketEFFECT_EFFECTINFO			 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/07/26														 */
/* ========================================================================= */

CPacketEFFECT_EFFECTINFO::~CPacketEFFECT_EFFECTINFO()
{
	SAFE_DELETE (m_pLibInfo);
}


/* ========================================================================= */
/* �֐���	:CPacketEFFECT_EFFECTINFO::Make									 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2007/07/26														 */
/* ========================================================================= */

void CPacketEFFECT_EFFECTINFO::Make(
	PCLibInfoEffect pInfo)		/* [in] �G�t�F�N�g��� */
{
	PBYTE pData, pDataTmp, pInfoDataTmp;
	DWORD dwSize, dwSizeDataTmp;
	PPACKETBASE pPacketBase;

	dwSizeDataTmp	= pInfo->GetSendDataSize ();
	pInfoDataTmp	= pInfo->GetSendData ();

	dwSize = sizeof (PACKETBASE) +
			 dwSizeDataTmp;

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_EFFECT;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_EFFECT_EFFECTINFO;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, pInfoDataTmp, dwSizeDataTmp, pDataTmp);	/* �G�t�F�N�g��� */

	RenewPacket (pData, dwSize);

	SAFE_DELETE_ARRAY (pInfoDataTmp);
}


/* ========================================================================= */
/* �֐���	:CPacketEFFECT_EFFECTINFO::Set									 */
/* ���e		:�p�P�b�g��ݒ�													 */
/* ���t		:2007/07/26														 */
/* ========================================================================= */

PBYTE CPacketEFFECT_EFFECTINFO::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	SAFE_DELETE (m_pLibInfo);
	m_pLibInfo	= new CLibInfoEffect;
	m_pLibInfo->Create ();
	pDataTmp	= m_pLibInfo->SetSendData (pDataTmp);

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2007 */
