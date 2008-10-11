/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketCHAR_RES_TAIL.cpp									 */
/* ���e			:�R�}���h(�L�����n:�t���čs������) �����t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/09/04													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketCHAR_RES_TAIL.h"

/* ========================================================================= */
/* �֐���	:CPacketCHAR_RES_TAIL::CPacketCHAR_RES_TAIL						 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/09/04														 */
/* ========================================================================= */

CPacketCHAR_RES_TAIL::CPacketCHAR_RES_TAIL()
{
	m_dwCharID			= 0;
	m_dwTargetCharID	= 0;
	m_dwPara			= 0;
	m_nResult			= 0;
}


/* ========================================================================= */
/* �֐���	:CPacketCHAR_RES_TAIL::~CPacketCHAR_RES_TAIL					 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/09/04														 */
/* ========================================================================= */

CPacketCHAR_RES_TAIL::~CPacketCHAR_RES_TAIL()
{
}


/* ========================================================================= */
/* �֐���	:CPacketCHAR_RES_TAIL::Make										 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2007/09/04														 */
/* ========================================================================= */

void CPacketCHAR_RES_TAIL::Make(
	DWORD dwCharID,				/* [in] �L����ID */
	DWORD dwTargetCharID,		/* [in] �t���čs������̃L����ID */
	int nResult,				/* [in] ���� */
	DWORD dwPara/*0*/)			/* [in] �p�����[�^ */
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE)		+
			 sizeof (dwCharID)			+
			 sizeof (dwTargetCharID)	+
			 sizeof (nResult)			+
			 sizeof (dwPara);

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_CHAR;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_CHAR_RES_TAIL;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwCharID,		sizeof (dwCharID),			pDataTmp);	/* �L����ID */
	CopyMemoryRenew (pDataTmp, &dwTargetCharID,	sizeof (dwTargetCharID),	pDataTmp);	/* �t���čs������̃L����ID */
	CopyMemoryRenew (pDataTmp, &nResult,		sizeof (nResult),			pDataTmp);	/* ���� */
	CopyMemoryRenew (pDataTmp, &dwPara,			sizeof (dwPara),			pDataTmp);	/* �p�����[�^ */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* �֐���	:CPacketCHAR_RES_TAIL::Set										 */
/* ���e		:�p�P�b�g��ݒ�													 */
/* ���t		:2007/09/04														 */
/* ========================================================================= */

PBYTE CPacketCHAR_RES_TAIL::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwCharID,		pDataTmp, sizeof (m_dwCharID),			pDataTmp);	/* �L����ID */
	CopyMemoryRenew (&m_dwTargetCharID,	pDataTmp, sizeof (m_dwTargetCharID),	pDataTmp);	/* �t���čs������̃L����ID */
	CopyMemoryRenew (&m_nResult,		pDataTmp, sizeof (m_nResult),			pDataTmp);	/* ���� */
	CopyMemoryRenew (&m_dwPara,			pDataTmp, sizeof (m_dwPara),			pDataTmp);	/* �p�����[�^ */

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2007 */
