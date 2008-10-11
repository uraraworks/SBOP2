/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* �t�@�C����	:PacketACCOUNT_RES_MAKECHAR.cpp								 */
/* ���e			:�R�}���h(�A�J�E���g�n:�L�����쐬����) �����t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2006/12/03													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketACCOUNT_RES_MAKECHAR.h"

/* ========================================================================= */
/* �֐���	:CPacketACCOUNT_RES_MAKECHAR::CPacketACCOUNT_RES_MAKECHAR		 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2006/12/03														 */
/* ========================================================================= */

CPacketACCOUNT_RES_MAKECHAR::CPacketACCOUNT_RES_MAKECHAR()
{
	m_nResult	= MAKECHARRES_NONE;
	m_dwCharID	= 0;
}


/* ========================================================================= */
/* �֐���	:CPacketACCOUNT_RES_MAKECHAR::~CPacketACCOUNT_RES_MAKECHAR		 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2006/12/03														 */
/* ========================================================================= */

CPacketACCOUNT_RES_MAKECHAR::~CPacketACCOUNT_RES_MAKECHAR()
{
}


/* ========================================================================= */
/* �֐���	:CPacketACCOUNT_RES_MAKECHAR::Make								 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2006/12/03														 */
/* ========================================================================= */

void CPacketACCOUNT_RES_MAKECHAR::Make(
	int nResult,		/* [in] ���� */
	DWORD dwCharID)		/* [in] �L����ID */
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE)	+
			 sizeof (nResult)		+
			 sizeof (dwCharID);

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_ACCOUNT;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ACCOUNT_RES_MAKECHAR;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &nResult,	sizeof (nResult),	pDataTmp);	/* ���� */
	CopyMemoryRenew (pDataTmp, &dwCharID,	sizeof (dwCharID),	pDataTmp);	/* �L����ID */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* �֐���	:CPacketACCOUNT_RES_MAKECHAR::Set								 */
/* ���e		:�p�P�b�g��ݒ�													 */
/* ���t		:2006/12/03														 */
/* ========================================================================= */

PBYTE CPacketACCOUNT_RES_MAKECHAR::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_nResult,	pDataTmp, sizeof (m_nResult),	pDataTmp);	/* ���� */
	CopyMemoryRenew (&m_dwCharID,	pDataTmp, sizeof (m_dwCharID),	pDataTmp);	/* �L����ID */

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2006 */
