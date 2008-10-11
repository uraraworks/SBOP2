/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* �t�@�C����	:PacketCONNECT_RES_LOGIN.cpp								 */
/* ���e			:�R�}���h(�ڑ��n:���O�C������) �����t�@�C��					 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2006/11/05													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketCONNECT_RES_LOGIN.h"

/* ========================================================================= */
/* �֐���	:CPacketCONNECT_RES_LOGIN::CPacketCONNECT_RES_LOGIN				 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2006/11/05														 */
/* ========================================================================= */

CPacketCONNECT_RES_LOGIN::CPacketCONNECT_RES_LOGIN()
{
	m_nResult		= LOGINRES_NG_PASSWORD;
	m_dwAccountID	= 0;
}


/* ========================================================================= */
/* �֐���	:CPacketCONNECT_RES_LOGIN::~CPacketCONNECT_RES_LOGIN			 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2006/11/05														 */
/* ========================================================================= */

CPacketCONNECT_RES_LOGIN::~CPacketCONNECT_RES_LOGIN()
{
}


/* ========================================================================= */
/* �֐���	:CPacketCONNECT_RES_LOGIN::Make									 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2006/11/05														 */
/* ========================================================================= */

void CPacketCONNECT_RES_LOGIN::Make(
	int nResult,			/* [in] ���� */
	DWORD dwAccountID)		/* [in] �A�J�E���gID */
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE)	+
			 sizeof (nResult)		+
			 sizeof (dwAccountID);

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_CONNECT;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_CONNECT_RES_LOGIN;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &nResult,		sizeof (nResult),		pDataTmp);	/* ���� */
	CopyMemoryRenew (pDataTmp, &dwAccountID,	sizeof (dwAccountID),	pDataTmp);	/* �A�J�E���gID */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* �֐���	:CPacketCONNECT_RES_LOGIN::Set									 */
/* ���e		:�p�P�b�g��ݒ�													 */
/* ���t		:2006/11/05														 */
/* ========================================================================= */

PBYTE CPacketCONNECT_RES_LOGIN::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_nResult,		pDataTmp, sizeof (m_nResult),		pDataTmp);	/* ���� */
	CopyMemoryRenew (&m_dwAccountID,	pDataTmp, sizeof (m_dwAccountID),	pDataTmp);	/* �A�J�E���gID */

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2006 */
