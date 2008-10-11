/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:PacketADMIN_PARA2.cpp										 */
/* ���e			:�R�}���h(�Ǘ��Ҍn:�ėp[�p�����[�^2��]) �����t�@�C��		 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/06/25													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketADMIN_PARA2.h"

/* ========================================================================= */
/* �֐���	:CPacketADMIN_PARA2::CPacketADMIN_PARA2							 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/06/25														 */
/* ========================================================================= */

CPacketADMIN_PARA2::CPacketADMIN_PARA2()
{
	m_dwPara1	= 0;
	m_dwPara2	= 0;
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_PARA2::~CPacketADMIN_PARA2						 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/06/25														 */
/* ========================================================================= */

CPacketADMIN_PARA2::~CPacketADMIN_PARA2()
{
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_PARA2::Make										 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2008/06/25														 */
/* ========================================================================= */

void CPacketADMIN_PARA2::Make(
	BYTE byCmdSub,			/* [in] �R�}���hID�T�u */
	DWORD dwPara1/*0*/,		/* [in] �p�����[�^1 */
	DWORD dwPara2/*0*/)		/* [in] �p�����[�^2 */
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE)	+
			 sizeof (dwPara1)		+
			 sizeof (dwPara2);

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_ADMIN;
	pPacketBase->byCmdSub	= byCmdSub;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwPara1, sizeof (dwPara1), pDataTmp);	/* �p�����[�^1 */
	CopyMemoryRenew (pDataTmp, &dwPara2, sizeof (dwPara2), pDataTmp);	/* �p�����[�^2 */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_PARA2::Set										 */
/* ���e		:�p�P�b�g��ݒ�													 */
/* ���t		:2008/06/25														 */
/* ========================================================================= */

PBYTE CPacketADMIN_PARA2::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwPara1, pDataTmp, sizeof (m_dwPara1), pDataTmp);	/* �p�����[�^1 */
	CopyMemoryRenew (&m_dwPara2, pDataTmp, sizeof (m_dwPara2), pDataTmp);	/* �p�����[�^2 */

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2008 */
