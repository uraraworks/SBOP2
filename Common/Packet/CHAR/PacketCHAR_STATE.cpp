/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketCHAR_STATE.cpp										 */
/* ���e			:�R�}���h(�L�����n:�L������Ԓʒm) �����t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/01/24													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "InfoCharBase.h"
#include "PacketCHAR_STATE.h"

/* ========================================================================= */
/* �֐���	:CPacketCHAR_STATE::CPacketCHAR_STATE							 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/01/24														 */
/* ========================================================================= */

CPacketCHAR_STATE::CPacketCHAR_STATE()
{
	m_dwCharID	= 0;
	m_nState	= CHARMOVESTATE_STAND;
}


/* ========================================================================= */
/* �֐���	:CPacketCHAR_STATE::~CPacketCHAR_STATE							 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/01/24														 */
/* ========================================================================= */

CPacketCHAR_STATE::~CPacketCHAR_STATE()
{
}


/* ========================================================================= */
/* �֐���	:CPacketCHAR_STATE::Make										 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2007/01/24														 */
/* ========================================================================= */

void CPacketCHAR_STATE::Make(
	DWORD dwCharID,		/* [in] �L����ID */
	int nState)			/* [in] ��� */
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE)	+
			 sizeof (dwCharID)		+
			 sizeof (nState);

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_CHAR;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_CHAR_STATE;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwCharID,	sizeof (dwCharID),	pDataTmp);	/* �L����ID */
	CopyMemoryRenew (pDataTmp, &nState,		sizeof (nState),	pDataTmp);	/* ��� */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* �֐���	:CPacketCHAR_STATE::Set											 */
/* ���e		:�p�P�b�g��ݒ�													 */
/* ���t		:2007/01/24														 */
/* ========================================================================= */

PBYTE CPacketCHAR_STATE::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwCharID,	pDataTmp, sizeof (m_dwCharID),	pDataTmp);	/* �L����ID */
	CopyMemoryRenew (&m_nState,		pDataTmp, sizeof (m_nState),	pDataTmp);	/* ��� */

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2007 */
