/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketMSGCMD_PARA1.cpp										 */
/* ���e			:�R�}���h(���b�Z�[�W�R�}���h�n:�ėp[�p�����[�^1��]) �����t�@�C��	 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/12/31													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketMSGCMD_PARA1.h"

/* ========================================================================= */
/* �֐���	:CPacketMSGCMD_PARA1::CPacketMSGCMD_PARA1						 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/12/31														 */
/* ========================================================================= */

CPacketMSGCMD_PARA1::CPacketMSGCMD_PARA1()
{
	m_dwCharID	= 0;
	m_dwPara	= 0;
}


/* ========================================================================= */
/* �֐���	:CPacketMSGCMD_PARA1::~CPacketMSGCMD_PARA1						 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/12/31														 */
/* ========================================================================= */

CPacketMSGCMD_PARA1::~CPacketMSGCMD_PARA1()
{
}


/* ========================================================================= */
/* �֐���	:CPacketMSGCMD_PARA1::Make										 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2007/12/31														 */
/* ========================================================================= */

void CPacketMSGCMD_PARA1::Make(
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

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_MSGCMD;
	pPacketBase->byCmdSub	= byCmdSub;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwCharID,	sizeof (dwCharID),	pDataTmp);	/* �L����ID */
	CopyMemoryRenew (pDataTmp, &dwPara,		sizeof (dwPara),	pDataTmp);	/* �p�����[�^ */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* �֐���	:CPacketMSGCMD_PARA1::Set										 */
/* ���e		:�p�P�b�g��ݒ�													 */
/* ���t		:2007/12/31														 */
/* ========================================================================= */

PBYTE CPacketMSGCMD_PARA1::Set(PBYTE pPacket)
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
