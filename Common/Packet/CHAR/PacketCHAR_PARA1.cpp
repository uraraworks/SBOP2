/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketCHAR_PARA1.cpp										 */
/* ���e			:�R�}���h(�L�����n:�ėp[�p�����[�^1��]) �����t�@�C��		 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/01/01													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketCHAR_PARA1.h"

/* ========================================================================= */
/* �֐���	:CPacketCHAR_PARA1::CPacketCHAR_PARA1							 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/01/01														 */
/* ========================================================================= */

CPacketCHAR_PARA1::CPacketCHAR_PARA1()
{
	m_dwCharID	= 0;
	m_dwPara	= 0;
}


/* ========================================================================= */
/* �֐���	:CPacketCHAR_PARA1::~CPacketCHAR_PARA1							 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/01/01														 */
/* ========================================================================= */

CPacketCHAR_PARA1::~CPacketCHAR_PARA1()
{
}


/* ========================================================================= */
/* �֐���	:CPacketCHAR_PARA1::Make										 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2008/01/01														 */
/* ========================================================================= */

void CPacketCHAR_PARA1::Make(
	BYTE byCmdSub,		/* [in] �R�}���hID�T�u */
	DWORD dwCharID,		/* [in] �L����ID */
	DWORD dwPara)		/* [in] �p�����[�^ */
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE)	+
			 sizeof (dwCharID)		+
			 sizeof (dwPara);

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_CHAR;
	pPacketBase->byCmdSub	= byCmdSub;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwCharID,	sizeof (dwCharID),	pDataTmp);	/* �L����ID */
	CopyMemoryRenew (pDataTmp, &dwPara,		sizeof (dwPara),	pDataTmp);	/* �p�����[�^ */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* �֐���	:CPacketCHAR_PARA1::Set											 */
/* ���e		:�p�P�b�g��ݒ�													 */
/* ���t		:2008/01/01														 */
/* ========================================================================= */

PBYTE CPacketCHAR_PARA1::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwCharID,	pDataTmp, sizeof (m_dwCharID),	pDataTmp);	/* �L����ID */
	CopyMemoryRenew (&m_dwPara,		pDataTmp, sizeof (m_dwPara),	pDataTmp);	/* �p�����[�^ */

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2007 */
