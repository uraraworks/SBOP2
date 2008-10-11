/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* �t�@�C����	:PacketCHAR_RES_CHARINFO.cpp								 */
/* ���e			:�R�}���h(�L�����n:�L������񉞓�) �����t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2006/12/31													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "InfoCharBase.h"
#include "Command.h"
#include "PacketCHAR_RES_CHARINFO.h"

/* ========================================================================= */
/* �֐���	:CPacketCHAR_RES_CHARINFO::CPacketCHAR_RES_CHARINFO				 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2006/12/31														 */
/* ========================================================================= */

CPacketCHAR_RES_CHARINFO::CPacketCHAR_RES_CHARINFO()
{
	m_bChgScreenPos = FALSE;
	m_pInfo = NULL;
}


/* ========================================================================= */
/* �֐���	:CPacketCHAR_RES_CHARINFO::~CPacketCHAR_RES_CHARINFO			 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2006/12/31														 */
/* ========================================================================= */

CPacketCHAR_RES_CHARINFO::~CPacketCHAR_RES_CHARINFO()
{
	SAFE_DELETE (m_pInfo);
}


/* ========================================================================= */
/* �֐���	:CPacketCHAR_RES_CHARINFO::Make									 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2006/12/31														 */
/* ========================================================================= */

void CPacketCHAR_RES_CHARINFO::Make(
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

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_CHAR;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_CHAR_RES_CHARINFO;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &bChgScreenPos,	sizeof (bChgScreenPos),	pDataTmp);	/* ��ʈʒu�ύX */
	CopyMemoryRenew (pDataTmp, pInfoDataTmp,	dwSizeDataTmp,			pDataTmp);	/* �L������� */

	RenewPacket (pData, dwSize);

	SAFE_DELETE_ARRAY (pInfoDataTmp);
}


/* ========================================================================= */
/* �֐���	:CPacketCHAR_RES_CHARINFO::Set									 */
/* ���e		:�p�P�b�g��ݒ�													 */
/* ���t		:2006/12/31														 */
/* ========================================================================= */

PBYTE CPacketCHAR_RES_CHARINFO::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_bChgScreenPos, pDataTmp, sizeof (m_bChgScreenPos), pDataTmp);	/* ��ʈʒu�ύX */

	SAFE_DELETE (m_pInfo);
	m_pInfo		= new CInfoCharBase;
	pDataTmp	= m_pInfo->SetSendData (pDataTmp);

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2006 */
