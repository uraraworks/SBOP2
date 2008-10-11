/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* �t�@�C����	:PacketACCOUNT_REQ_MAKECHAR.cpp								 */
/* ���e			:�R�}���h(�A�J�E���g�n:�L�����쐬�v��) �����t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2006/11/14													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "InfoCharBase.h"
#include "PacketACCOUNT_REQ_MAKECHAR.h"

/* ========================================================================= */
/* �֐���	:CPacketACCOUNT_REQ_MAKECHAR::CPacketACCOUNT_REQ_MAKECHAR		 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2006/11/14														 */
/* ========================================================================= */

CPacketACCOUNT_REQ_MAKECHAR::CPacketACCOUNT_REQ_MAKECHAR()
{
	m_pInfoCharBase = NULL;
}


/* ========================================================================= */
/* �֐���	:CPacketACCOUNT_REQ_MAKECHAR::~CPacketACCOUNT_REQ_MAKECHAR		 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2006/11/14														 */
/* ========================================================================= */

CPacketACCOUNT_REQ_MAKECHAR::~CPacketACCOUNT_REQ_MAKECHAR()
{
	SAFE_DELETE (m_pInfoCharBase);
}


/* ========================================================================= */
/* �֐���	:CPacketACCOUNT_REQ_MAKECHAR::Make								 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2006/11/14														 */
/* ========================================================================= */

void CPacketACCOUNT_REQ_MAKECHAR::Make(
	DWORD dwAccountID,					/* [in] �A�J�E���gID */
	CInfoCharBase *pInfoCharBase)		/* [in] �L������� */
{
	PBYTE pData, pDataTmp, pCharData;
	DWORD dwSize, dwCharDataSize;
	PPACKETBASE pPacketBase;

	dwCharDataSize	= pInfoCharBase->GetSendDataSize ();
	pCharData		= pInfoCharBase->GetSendData ();

	dwSize = sizeof (PACKETBASE)	+
			 sizeof (dwAccountID)	+
			 dwCharDataSize;

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_ACCOUNT;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ACCOUNT_REQ_MAKECHAR;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwAccountID, sizeof (dwAccountID), pDataTmp);	/* �A�J�E���gID */
	CopyMemoryRenew (pDataTmp, pCharData, dwCharDataSize, pDataTmp);			/* �L������� */

	RenewPacket (pData, dwSize);

	SAFE_DELETE_ARRAY (pCharData);
}


/* ========================================================================= */
/* �֐���	:CPacketACCOUNT_REQ_MAKECHAR::Set								 */
/* ���e		:�p�P�b�g��ݒ�													 */
/* ���t		:2006/11/14														 */
/* ========================================================================= */

PBYTE CPacketACCOUNT_REQ_MAKECHAR::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	SAFE_DELETE (m_pInfoCharBase);
	m_pInfoCharBase = new CInfoCharBase;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwAccountID, pDataTmp, sizeof (m_dwAccountID), pDataTmp);	/* �A�J�E���gID */
	pDataTmp = m_pInfoCharBase->SetSendData (pDataTmp);

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2006 */
