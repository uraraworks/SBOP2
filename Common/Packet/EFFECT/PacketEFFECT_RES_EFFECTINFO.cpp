/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketEFFECT_RES_EFFECTINFO.cpp							 */
/* ���e			:�R�}���h(�G�t�F�N�g�n:�G�t�F�N�g��񉞓�) �����t�@�C��		 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/07/26													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "InfoEffect.h"
#include "Command.h"
#include "PacketEFFECT_RES_EFFECTINFO.h"

/* ========================================================================= */
/* �֐���	:CPacketEFFECT_RES_EFFECTINFO::CPacketEFFECT_RES_EFFECTINFO		 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/07/26														 */
/* ========================================================================= */

CPacketEFFECT_RES_EFFECTINFO::CPacketEFFECT_RES_EFFECTINFO()
{
	m_pInfo = NULL;
}


/* ========================================================================= */
/* �֐���	:CPacketEFFECT_RES_EFFECTINFO::~CPacketEFFECT_RES_EFFECTINFO	 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/07/26														 */
/* ========================================================================= */

CPacketEFFECT_RES_EFFECTINFO::~CPacketEFFECT_RES_EFFECTINFO()
{
	SAFE_DELETE (m_pInfo);
}


/* ========================================================================= */
/* �֐���	:CPacketEFFECT_RES_EFFECTINFO::Make								 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2007/07/26														 */
/* ========================================================================= */

void CPacketEFFECT_RES_EFFECTINFO::Make(
	CInfoEffect *pInfo)		/* [in] �G�t�F�N�g��� */
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
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_EFFECT_RES_EFFECTINFO;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, pInfoDataTmp, dwSizeDataTmp, pDataTmp);	/* �G�t�F�N�g��� */

	RenewPacket (pData, dwSize);

	SAFE_DELETE_ARRAY (pInfoDataTmp);
}


/* ========================================================================= */
/* �֐���	:CPacketEFFECT_RES_EFFECTINFO::Set								 */
/* ���e		:�p�P�b�g��ݒ�													 */
/* ���t		:2007/07/26														 */
/* ========================================================================= */

PBYTE CPacketEFFECT_RES_EFFECTINFO::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	SAFE_DELETE (m_pInfo);
	m_pInfo		= new CInfoEffect;
	pDataTmp	= m_pInfo->SetSendData (pDataTmp);

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2007 */
