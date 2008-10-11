/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketMSGCMD_CHGFACE.cpp									 */
/* ���e			:�R�}���h(���b�Z�[�W�R�}���h�n:�\��ύX) �����t�@�C��		 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/05/02													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketMSGCMD_CHGFACE.h"

/* ========================================================================= */
/* �֐���	:CPacketMSGCMD_CHGFACE::CPacketMSGCMD_CHGFACE					 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/05/02														 */
/* ========================================================================= */

CPacketMSGCMD_CHGFACE::CPacketMSGCMD_CHGFACE()
{
	m_dwCharID	= 0;
	m_nType		= 0;
}


/* ========================================================================= */
/* �֐���	:CPacketMSGCMD_CHGFACE::~CPacketMSGCMD_CHGFACE					 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/05/02														 */
/* ========================================================================= */

CPacketMSGCMD_CHGFACE::~CPacketMSGCMD_CHGFACE()
{
}


/* ========================================================================= */
/* �֐���	:CPacketMSGCMD_CHGFACE::Make									 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2007/05/02														 */
/* ========================================================================= */

void CPacketMSGCMD_CHGFACE::Make(
	DWORD dwCharID,		/* [in] �L����ID */
	int nType)			/* [in] ��� */
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE)	+
			 sizeof (dwCharID)		+
			 sizeof (nType);

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_MSGCMD;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_MSGCMD_CHGFACE;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwCharID,	sizeof (dwCharID),	pDataTmp);	/* �L����ID */
	CopyMemoryRenew (pDataTmp, &nType,		sizeof (nType),		pDataTmp);	/* ��� */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* �֐���	:CPacketMSGCMD_CHGFACE::Set										 */
/* ���e		:�p�P�b�g��ݒ�													 */
/* ���t		:2007/05/02														 */
/* ========================================================================= */

PBYTE CPacketMSGCMD_CHGFACE::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwCharID,	pDataTmp, sizeof (m_dwCharID),	pDataTmp);	/* �L����ID */
	CopyMemoryRenew (&m_nType,		pDataTmp, sizeof (m_nType),		pDataTmp);	/* ��� */

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2007 */
