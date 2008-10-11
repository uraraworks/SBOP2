/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketCHAR_REQ_CHAT.cpp									 */
/* ���e			:�R�}���h(�L�����n:�`���b�g�v��) �����t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/02/04													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketCHAR_REQ_CHAT.h"

/* ========================================================================= */
/* �֐���	:CPacketCHAR_REQ_CHAT::CPacketCHAR_REQ_CHAT						 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/02/04														 */
/* ========================================================================= */

CPacketCHAR_REQ_CHAT::CPacketCHAR_REQ_CHAT()
{
	m_nType		= CHATTYPE_NORMAL;
	m_dwCharID	= 0;
}


/* ========================================================================= */
/* �֐���	:CPacketCHAR_REQ_CHAT::~CPacketCHAR_REQ_CHAT					 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/02/04														 */
/* ========================================================================= */

CPacketCHAR_REQ_CHAT::~CPacketCHAR_REQ_CHAT()
{
}


/* ========================================================================= */
/* �֐���	:CPacketCHAR_REQ_CHAT::Make										 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2007/02/04														 */
/* ========================================================================= */

void CPacketCHAR_REQ_CHAT::Make(
	int nType,			/* [in] �`���b�g��� */
	DWORD dwCharID,		/* [in] �L����ID */
	LPCSTR pszChat)		/* [in] �������e */
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE)	+
			 sizeof (nType)			+
			 sizeof (dwCharID)		+
			 (strlen (pszChat) + 1);

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_CHAR;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_CHAR_REQ_CHAT;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwCharID,	sizeof (dwCharID),	pDataTmp);	/* �L����ID */
	CopyMemoryRenew (pDataTmp, &nType,		sizeof (nType),		pDataTmp);	/* �`���b�g��� */
	strcpyRenew ((LPSTR)pDataTmp, pszChat, pDataTmp);						/* �������e */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* �֐���	:CPacketCHAR_REQ_CHAT::Set										 */
/* ���e		:�p�P�b�g��ݒ�													 */
/* ���t		:2007/02/04														 */
/* ========================================================================= */

PBYTE CPacketCHAR_REQ_CHAT::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwCharID,	pDataTmp, sizeof (m_dwCharID),	pDataTmp);	/* �L����ID */
	CopyMemoryRenew (&m_nType,		pDataTmp, sizeof (m_nType),		pDataTmp);	/* �`���b�g��� */
	StoreRenew (m_strChat, (LPCSTR)pDataTmp, pDataTmp);							/* �������e */

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2007 */
