/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketADMIN_RENEWMAPSHADOW.cpp								 */
/* ���e			:�R�}���h(�Ǘ��Ҍn:�}�b�v�e�X�V) �����t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/06/06													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "InfoMapShadow.h"
#include "Command.h"
#include "PacketADMIN_RENEWMAPSHADOW.h"

/* ========================================================================= */
/* �֐���	:CPacketADMIN_RENEWMAPSHADOW::CPacketADMIN_RENEWMAPSHADOW		 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/06/06														 */
/* ========================================================================= */

CPacketADMIN_RENEWMAPSHADOW::CPacketADMIN_RENEWMAPSHADOW()
{
	m_pInfoMapShadow = NULL;
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_RENEWMAPSHADOW::~CPacketADMIN_RENEWMAPSHADOW		 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/06/06														 */
/* ========================================================================= */

CPacketADMIN_RENEWMAPSHADOW::~CPacketADMIN_RENEWMAPSHADOW()
{
	SAFE_DELETE (m_pInfoMapShadow);
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_RENEWMAPSHADOW::Make								 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2007/06/06														 */
/* ========================================================================= */

void CPacketADMIN_RENEWMAPSHADOW::Make(
	CInfoMapShadow *pInfo)		/* [in] �}�b�v�e��� */
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

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_ADMIN;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ADMIN_RENEWMAPSHADOW;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, pInfoDataTmp, dwSizeDataTmp, pDataTmp);	/* �}�b�v�e��� */

	RenewPacket (pData, dwSize);

	SAFE_DELETE_ARRAY (pInfoDataTmp);
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_RENEWMAPSHADOW::Set								 */
/* ���e		:�p�P�b�g��ݒ�													 */
/* ���t		:2007/06/06														 */
/* ========================================================================= */

PBYTE CPacketADMIN_RENEWMAPSHADOW::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	SAFE_DELETE (m_pInfoMapShadow);
	m_pInfoMapShadow	= new CInfoMapShadow;
	pDataTmp			= m_pInfoMapShadow->SetSendData (pDataTmp);

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2007 */
