/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketCHAR_ITEMINFO.cpp									 */
/* ���e			:�R�}���h(�L�����n:�A�C�e�����ʒm) �����t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/07/31													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketCHAR_ITEMINFO.h"

/* ========================================================================= */
/* �֐���	:CPacketCHAR_ITEMINFO::CPacketCHAR_ITEMINFO						 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/07/31														 */
/* ========================================================================= */

CPacketCHAR_ITEMINFO::CPacketCHAR_ITEMINFO()
{
}


/* ========================================================================= */
/* �֐���	:CPacketCHAR_ITEMINFO::~CPacketCHAR_ITEMINFO					 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/07/31														 */
/* ========================================================================= */

CPacketCHAR_ITEMINFO::~CPacketCHAR_ITEMINFO()
{
}


/* ========================================================================= */
/* �֐���	:CPacketCHAR_ITEMINFO::Make										 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2007/07/31														 */
/* ========================================================================= */

void CPacketCHAR_ITEMINFO::Make(
	DWORD dwCharID,			/* [in] �L����ID */
	PARRAYDWORD paSrc)		/* [in] �A�C�e��ID */
{
	PBYTE pData, pDataTmp;
	DWORD i, dwSize, dwCount, dwTmp;
	PPACKETBASE pPacketBase;

	dwCount = paSrc->GetSize ();

	dwSize = sizeof (PACKETBASE)	+
			 sizeof (dwCharID)		+
			 sizeof (DWORD)			+
			 (sizeof (DWORD) * dwCount);

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_CHAR;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_CHAR_ITEMINFO;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwCharID,	sizeof (dwCharID),	pDataTmp);	/* �L����ID */
	CopyMemoryRenew (pDataTmp, &dwCount,	sizeof (dwCount),	pDataTmp);	/* �� */
	for (i = 0; i < dwCount; i ++) {
		dwTmp = paSrc->GetAt (i);
		CopyMemoryRenew (pDataTmp, &dwTmp, sizeof (dwTmp), pDataTmp);		/* �A�C�e��ID */
	}

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* �֐���	:CPacketCHAR_ITEMINFO::Set										 */
/* ���e		:�p�P�b�g��ݒ�													 */
/* ���t		:2007/07/31														 */
/* ========================================================================= */

PBYTE CPacketCHAR_ITEMINFO::Set(PBYTE pPacket)
{
	DWORD i, dwCount, dwTmp;
	PBYTE pRet, pDataTmp;

	m_adwItemID.RemoveAll ();

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwCharID,	pDataTmp, sizeof (m_dwCharID),	pDataTmp);	/* �L����ID */
	CopyMemoryRenew (&dwCount,		pDataTmp, sizeof (dwCount),		pDataTmp);	/* �� */
	for (i = 0; i < dwCount; i ++) {
		CopyMemoryRenew (&dwTmp, pDataTmp, sizeof (dwTmp), pDataTmp);			/* �A�C�e��ID */
		m_adwItemID.Add (dwTmp);
	}

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2007 */
