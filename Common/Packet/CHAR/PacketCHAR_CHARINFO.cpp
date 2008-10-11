/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketCHAR_CHARINFO.cpp									 */
/* ���e			:�R�}���h(�L�����n:�L�������ʒm) �����t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/01/27													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketCHAR_CHARINFO.h"

/* ========================================================================= */
/* �֐���	:CPacketCHAR_CHARINFO::CPacketCHAR_CHARINFO						 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/01/27														 */
/* ========================================================================= */

CPacketCHAR_CHARINFO::CPacketCHAR_CHARINFO()
{
	m_pLibInfo = NULL;
}


/* ========================================================================= */
/* �֐���	:CPacketCHAR_CHARINFO::~CPacketCHAR_CHARINFO					 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/01/27														 */
/* ========================================================================= */

CPacketCHAR_CHARINFO::~CPacketCHAR_CHARINFO()
{
	SAFE_DELETE (m_pLibInfo);
}


/* ========================================================================= */
/* �֐���	:CPacketCHAR_CHARINFO::Make										 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2007/01/27														 */
/* ========================================================================= */

void CPacketCHAR_CHARINFO::Make(
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

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_CHAR;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_CHAR_CHARINFO;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, pInfoDataTmp, dwSizeDataTmp, pDataTmp);	/* �L������� */

	RenewPacket (pData, dwSize);

	SAFE_DELETE_ARRAY (pInfoDataTmp);
}


/* ========================================================================= */
/* �֐���	:CPacketCHAR_CHARINFO::Set										 */
/* ���e		:�p�P�b�g��ݒ�													 */
/* ���t		:2007/01/27														 */
/* ========================================================================= */

PBYTE CPacketCHAR_CHARINFO::Set(PBYTE pPacket)
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

/* Copyright(C)URARA-works 2007 */
