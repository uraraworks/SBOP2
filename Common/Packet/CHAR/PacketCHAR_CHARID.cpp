/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* �t�@�C����	:PacketCHAR_CHARID.cpp										 */
/* ���e			:�R�}���h(�L�����n:�L����ID�ʒm) �����t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/02/12													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketCHAR_CHARID.h"

/* ========================================================================= */
/* �֐���	:CPacketCHAR_CHARID::CPacketCHAR_CHARID							 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/02/12														 */
/* ========================================================================= */

CPacketCHAR_CHARID::CPacketCHAR_CHARID()
{
}


/* ========================================================================= */
/* �֐���	:CPacketCHAR_CHARID::~CPacketCHAR_CHARID						 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/02/12														 */
/* ========================================================================= */

CPacketCHAR_CHARID::~CPacketCHAR_CHARID()
{
}


/* ========================================================================= */
/* �֐���	:CPacketCHAR_CHARID::Make										 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2007/02/12														 */
/* ========================================================================= */

void CPacketCHAR_CHARID::Make(
	PARRAYDWORD paSrc)		/* [in] �L����ID */
{
	PBYTE pData, pDataTmp;
	DWORD i, dwSize, dwCount, dwTmp;
	PPACKETBASE pPacketBase;

	dwCount = paSrc->GetSize ();

	dwSize = sizeof (PACKETBASE)	+
			 sizeof (DWORD)			+
			 (sizeof (DWORD) * dwCount);

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_CHAR;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_CHAR_CHARID;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwCount, sizeof (dwCount), pDataTmp);	/* �� */
	for (i = 0; i < dwCount; i ++) {
		dwTmp = paSrc->GetAt (i);
		CopyMemoryRenew (pDataTmp, &dwTmp, sizeof (dwTmp), pDataTmp);	/* �L����ID */
	}

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* �֐���	:CPacketCHAR_CHARID::Set										 */
/* ���e		:�p�P�b�g��ݒ�													 */
/* ���t		:2007/02/12														 */
/* ========================================================================= */

PBYTE CPacketCHAR_CHARID::Set(PBYTE pPacket)
{
	DWORD i, dwCount, dwTmp;
	PBYTE pRet, pDataTmp;

	m_adwCharID.RemoveAll ();

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&dwCount, pDataTmp, sizeof (dwCount), pDataTmp);	/* �� */
	for (i = 0; i < dwCount; i ++) {
		CopyMemoryRenew (&dwTmp, pDataTmp, sizeof (dwTmp), pDataTmp);	/* �L����ID */
		m_adwCharID.Add (dwTmp);
	}

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2006 */
