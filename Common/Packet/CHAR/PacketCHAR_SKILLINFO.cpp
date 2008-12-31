/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:PacketCHAR_SKILLINFO.cpp									 */
/* ���e			:�R�}���h(�L�����n:�X�L�����ʒm) �����t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/12/31													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketCHAR_SKILLINFO.h"

/* ========================================================================= */
/* �֐���	:CPacketCHAR_SKILLINFO::CPacketCHAR_SKILLINFO					 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/12/31														 */
/* ========================================================================= */

CPacketCHAR_SKILLINFO::CPacketCHAR_SKILLINFO()
{
}


/* ========================================================================= */
/* �֐���	:CPacketCHAR_SKILLINFO::~CPacketCHAR_SKILLINFO					 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/12/31														 */
/* ========================================================================= */

CPacketCHAR_SKILLINFO::~CPacketCHAR_SKILLINFO()
{
}


/* ========================================================================= */
/* �֐���	:CPacketCHAR_SKILLINFO::Make									 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2008/12/31														 */
/* ========================================================================= */

void CPacketCHAR_SKILLINFO::Make(
	DWORD dwCharID,			/* [in] �L����ID */
	PARRAYDWORD paSrc)		/* [in] �X�L��ID */
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
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_CHAR_SKILLINFO;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwCharID,	sizeof (dwCharID),	pDataTmp);	/* �L����ID */
	CopyMemoryRenew (pDataTmp, &dwCount,	sizeof (dwCount),	pDataTmp);	/* �� */
	for (i = 0; i < dwCount; i ++) {
		dwTmp = paSrc->GetAt (i);
		CopyMemoryRenew (pDataTmp, &dwTmp, sizeof (dwTmp), pDataTmp);		/* �X�L��ID */
	}

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* �֐���	:CPacketCHAR_SKILLINFO::Set										 */
/* ���e		:�p�P�b�g��ݒ�													 */
/* ���t		:2008/12/31														 */
/* ========================================================================= */

PBYTE CPacketCHAR_SKILLINFO::Set(PBYTE pPacket)
{
	DWORD i, dwCount, dwTmp;
	PBYTE pRet, pDataTmp;

	m_adwSkillID.RemoveAll ();

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwCharID,	pDataTmp, sizeof (m_dwCharID),	pDataTmp);	/* �L����ID */
	CopyMemoryRenew (&dwCount,		pDataTmp, sizeof (dwCount),		pDataTmp);	/* �� */
	for (i = 0; i < dwCount; i ++) {
		CopyMemoryRenew (&dwTmp, pDataTmp, sizeof (dwTmp), pDataTmp);			/* �X�L��ID */
		m_adwSkillID.Add (dwTmp);
	}

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2008 */
