/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketVERSION_RES_FILELISTCHECK.cpp						 */
/* ���e			:�R�}���h(�o�[�W�����n:�t�@�C�����X�g�`�F�b�N����) �����t�@�C��	 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/08/28													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketVERSION_RES_FILELISTCHECK.h"

/* ========================================================================= */
/* �֐���	:CPacketVERSION_RES_FILELISTCHECK::CPacketVERSION_RES_FILELISTCHECK */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/08/28														 */
/* ========================================================================= */

CPacketVERSION_RES_FILELISTCHECK::CPacketVERSION_RES_FILELISTCHECK()
{
	m_nResult = FILELISTCHECKRES_NONE;
}


/* ========================================================================= */
/* �֐���	:CPacketVERSION_RES_FILELISTCHECK::~CPacketVERSION_RES_FILELISTCHECK */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/08/28														 */
/* ========================================================================= */

CPacketVERSION_RES_FILELISTCHECK::~CPacketVERSION_RES_FILELISTCHECK()
{
}


/* ========================================================================= */
/* �֐���	:CPacketVERSION_RES_FILELISTCHECK::Make							 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2007/08/28														 */
/* ========================================================================= */

void CPacketVERSION_RES_FILELISTCHECK::Make(
	int nResult)		/* [in] ���� */
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE) +
			 sizeof (nResult);

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_VERSION;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_VERSION_RES_FILELISTCHECK;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &nResult, sizeof (nResult), pDataTmp);		/* ���� */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* �֐���	:CPacketVERSION_RES_FILELISTCHECK::Set							 */
/* ���e		:�p�P�b�g��ݒ�													 */
/* ���t		:2007/08/28														 */
/* ========================================================================= */

PBYTE CPacketVERSION_RES_FILELISTCHECK::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_nResult, pDataTmp, sizeof (m_nResult), pDataTmp);

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2007 */
