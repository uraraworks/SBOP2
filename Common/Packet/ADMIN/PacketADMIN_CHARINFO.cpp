/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketADMIN_CHARINFO.cpp									 */
/* ���e			:�R�}���h(�Ǘ��Ҍn:�L�������ʒm) �����t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/03/18													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "InfoCharBase.h"
#include "Command.h"
#include "PacketADMIN_CHARINFO.h"

/* ========================================================================= */
/* �֐���	:CPacketADMIN_CHARINFO::CPacketADMIN_CHARINFO					 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/03/18														 */
/* ========================================================================= */

CPacketADMIN_CHARINFO::CPacketADMIN_CHARINFO()
{
	m_bChgScreenPos = FALSE;
	m_pInfoChar = NULL;
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_CHARINFO::~CPacketADMIN_CHARINFO					 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/03/18														 */
/* ========================================================================= */

CPacketADMIN_CHARINFO::~CPacketADMIN_CHARINFO()
{
	SAFE_DELETE (m_pInfoChar);
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_CHARINFO::Make									 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2007/03/18														 */
/* ========================================================================= */

void CPacketADMIN_CHARINFO::Make(
	CInfoCharBase *pInfo,		/* [in] �L������� */
	BOOL bChgScreenPos)			/* [in] ��ʈʒu�ύX */
{
	PBYTE pData, pDataTmp, pInfoDataTmp;
	DWORD dwSize, dwSizeDataTmp;
	PPACKETBASE pPacketBase;

	dwSizeDataTmp	= pInfo->GetSendDataSize ();
	pInfoDataTmp	= pInfo->GetSendData ();

	dwSize = sizeof (PACKETBASE)	+
			 sizeof (bChgScreenPos)	+
			 dwSizeDataTmp;

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_ADMIN;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ADMIN_CHARINFO;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &bChgScreenPos,	sizeof (bChgScreenPos),	pDataTmp);	/* ��ʈʒu�ύX */
	CopyMemoryRenew (pDataTmp, pInfoDataTmp,	dwSizeDataTmp,			pDataTmp);	/* �L������� */

	RenewPacket (pData, dwSize);

	SAFE_DELETE_ARRAY (pInfoDataTmp);
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_CHARINFO::Set										 */
/* ���e		:�p�P�b�g��ݒ�													 */
/* ���t		:2007/03/18														 */
/* ========================================================================= */

PBYTE CPacketADMIN_CHARINFO::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_bChgScreenPos, pDataTmp, sizeof (m_bChgScreenPos), pDataTmp);	/* ��ʈʒu�ύX */

	SAFE_DELETE (m_pInfoChar);
	m_pInfoChar	= new CInfoCharBase;
	pDataTmp	= m_pInfoChar->SetSendData (pDataTmp);

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2007 */
