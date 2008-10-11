/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketCHAR_MOVEPOS.cpp										 */
/* ���e			:�R�}���h(�L�����n:�ړ��ʒm) �����t�@�C��					 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/01/14													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketCHAR_MOVEPOS.h"

/* ========================================================================= */
/* �֐���	:CPacketCHAR_MOVEPOS::CPacketCHAR_MOVEPOS						 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/01/14														 */
/* ========================================================================= */

CPacketCHAR_MOVEPOS::CPacketCHAR_MOVEPOS()
{
	m_bUpdate		= FALSE;
	m_dwMapID		= 0;
	m_dwCharID		= 0;
	m_nDirection	= 0;
	m_pos.x = m_pos.y = 0;
}


/* ========================================================================= */
/* �֐���	:CPacketCHAR_MOVEPOS::~CPacketCHAR_MOVEPOS						 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/01/14														 */
/* ========================================================================= */

CPacketCHAR_MOVEPOS::~CPacketCHAR_MOVEPOS()
{
}


/* ========================================================================= */
/* �֐���	:CPacketCHAR_MOVEPOS::Make										 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2007/01/14														 */
/* ========================================================================= */

void CPacketCHAR_MOVEPOS::Make(
	DWORD dwMapID,		/* [in] �}�b�vID */
	DWORD dwCharID,		/* [in] �L����ID */
	int nDirection,		/* [in] ���� */
	int x,				/* [in] X���W */
	int y,				/* [in] Y���W */
	BOOL bUpdate)		/* [in] ���L�����̍��W���X�V����H */
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE)	+
			 sizeof (dwMapID)		+
			 sizeof (dwCharID)		+
			 sizeof (nDirection)	+
			 sizeof (x)				+
			 sizeof (y)				+
			 sizeof (bUpdate);

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_CHAR;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_CHAR_MOVEPOS;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwMapID,	sizeof (dwMapID),		pDataTmp);	/* �}�b�vID */
	CopyMemoryRenew (pDataTmp, &dwCharID,	sizeof (dwCharID),		pDataTmp);	/* �L����ID */
	CopyMemoryRenew (pDataTmp, &nDirection,	sizeof (nDirection),	pDataTmp);	/* ���� */
	CopyMemoryRenew (pDataTmp, &x,			sizeof (x),				pDataTmp);	/* X���W */
	CopyMemoryRenew (pDataTmp, &y,			sizeof (y),				pDataTmp);	/* Y���W */
	CopyMemoryRenew (pDataTmp, &bUpdate,	sizeof (bUpdate),		pDataTmp);	/* ���L�����̍��W���X�V����H */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* �֐���	:CPacketCHAR_MOVEPOS::Set										 */
/* ���e		:�p�P�b�g��ݒ�													 */
/* ���t		:2007/01/14														 */
/* ========================================================================= */

PBYTE CPacketCHAR_MOVEPOS::Set(PBYTE pPacket)
{
	int x, y;
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwMapID,	pDataTmp, sizeof (m_dwMapID),		pDataTmp);	/* �}�b�vID */
	CopyMemoryRenew (&m_dwCharID,	pDataTmp, sizeof (m_dwCharID),		pDataTmp);	/* �L����ID */
	CopyMemoryRenew (&m_nDirection,	pDataTmp, sizeof (m_nDirection),	pDataTmp);	/* ���� */
	CopyMemoryRenew (&x,			pDataTmp, sizeof (x),				pDataTmp);	/* X���W */
	CopyMemoryRenew (&y,			pDataTmp, sizeof (y),				pDataTmp);	/* Y���W */
	CopyMemoryRenew (&m_bUpdate,	pDataTmp, sizeof (m_bUpdate),		pDataTmp);	/* ���L�����̍��W���X�V����H */

	m_pos.x = x;
	m_pos.y = y;

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2007 */
