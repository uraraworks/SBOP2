/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketCHAR_MODIFY_PARAM.cpp								 */
/* ���e			:�R�}���h(�L�����n:�p�����[�^�ύX�ʒm) �����t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/09/09													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketCHAR_MODIFY_PARAM.h"

/* ========================================================================= */
/* �֐���	:CPacketCHAR_MODIFY_PARAM::CPacketCHAR_MODIFY_PARAM				 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/09/09														 */
/* ========================================================================= */

CPacketCHAR_MODIFY_PARAM::CPacketCHAR_MODIFY_PARAM()
{
	m_dwCharID	= 0;
	m_nType		= PARAMID_CHAR_MODIFY_NONE;
	m_nParam	= 0;
}


/* ========================================================================= */
/* �֐���	:CPacketCHAR_MODIFY_PARAM::~CPacketCHAR_MODIFY_PARAM			 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/09/09														 */
/* ========================================================================= */

CPacketCHAR_MODIFY_PARAM::~CPacketCHAR_MODIFY_PARAM()
{
}


/* ========================================================================= */
/* �֐���	:CPacketCHAR_MODIFY_PARAM::Make									 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2007/09/09														 */
/* ========================================================================= */

void CPacketCHAR_MODIFY_PARAM::Make(
	DWORD dwCharID,		/* [in] �L����ID */
	int nType,			/* [in] ��� */
	int nParam)			/* [in] �p�����[�^ */
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE)	+
			 sizeof (dwCharID)		+
			 sizeof (nType)			+
			 sizeof (nParam);

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_CHAR;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_CHAR_MODIFY_PARAM;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwCharID,	sizeof (dwCharID),	pDataTmp);	/* �L����ID */
	CopyMemoryRenew (pDataTmp, &nType,		sizeof (nType),		pDataTmp);	/* ��� */
	CopyMemoryRenew (pDataTmp, &nParam,		sizeof (nParam),	pDataTmp);	/* �p�����[�^ */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* �֐���	:CPacketCHAR_MODIFY_PARAM::Set									 */
/* ���e		:�p�P�b�g��ݒ�													 */
/* ���t		:2007/09/09														 */
/* ========================================================================= */

PBYTE CPacketCHAR_MODIFY_PARAM::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwCharID,	pDataTmp, sizeof (m_dwCharID),	pDataTmp);	/* �L����ID */
	CopyMemoryRenew (&m_nType,		pDataTmp, sizeof (m_nType),		pDataTmp);	/* ��� */
	CopyMemoryRenew (&m_nParam,		pDataTmp, sizeof (m_nParam),	pDataTmp);	/* �p�����[�^ */

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2007 */
