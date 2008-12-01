/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:PacketADMIN_CHAR_RES_ONLINE.cpp							 */
/* ���e			:�R�}���h(�Ǘ��Ҍn:�L�������ʒm) �����t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/12/01													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketADMIN_CHAR_RES_ONLINE.h"

/* ========================================================================= */
/* �֐���	:CPacketADMIN_CHAR_RES_ONLINE::CPacketADMIN_CHAR_RES_ONLINE		 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/12/01														 */
/* ========================================================================= */

CPacketADMIN_CHAR_RES_ONLINE::CPacketADMIN_CHAR_RES_ONLINE()
{
	m_pLibInfo = NULL;
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_CHAR_RES_ONLINE::~CPacketADMIN_CHAR_RES_ONLINE	 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/12/01														 */
/* ========================================================================= */

CPacketADMIN_CHAR_RES_ONLINE::~CPacketADMIN_CHAR_RES_ONLINE()
{
	SAFE_DELETE (m_pLibInfo);
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_CHAR_RES_ONLINE::Make								 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2008/12/01														 */
/* ========================================================================= */

void CPacketADMIN_CHAR_RES_ONLINE::Make(
	PCLibInfoCharBase pInfo)		/* [in] �L������� */
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
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ADMIN_CHAR_RES_ONLINE;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, pInfoDataTmp, dwSizeDataTmp, pDataTmp);	/* �L������� */

	RenewPacket (pData, dwSize);

	SAFE_DELETE_ARRAY (pInfoDataTmp);
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_CHAR_RES_ONLINE::Set								 */
/* ���e		:�p�P�b�g��ݒ�													 */
/* ���t		:2008/12/01														 */
/* ========================================================================= */

PBYTE CPacketADMIN_CHAR_RES_ONLINE::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	SAFE_DELETE (m_pLibInfo);
	m_pLibInfo	= new CLibInfoCharBase;
	m_pLibInfo->Create ();
	pDataTmp	= m_pLibInfo->SetSendData (pDataTmp);

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2008 */
