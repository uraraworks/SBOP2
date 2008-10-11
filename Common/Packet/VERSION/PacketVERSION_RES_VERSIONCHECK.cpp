/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* �t�@�C����	:PacketVERSION_RES_VERSIONCHECK.cpp							 */
/* ���e			:�R�}���h(�o�[�W�����n:�o�[�W�����`�F�b�N����) �����t�@�C��	 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2006/11/05													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketVERSION_RES_VERSIONCHECK.h"

/* ========================================================================= */
/* �֐���	:CPacketVERSION_RES_VERSIONCHECK::CPacketVERSION_RES_VERSIONCHECK */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2006/11/05														 */
/* ========================================================================= */

CPacketVERSION_RES_VERSIONCHECK::CPacketVERSION_RES_VERSIONCHECK()
{
	m_nResult = VERSIONCHECKRES_NONE;
}


/* ========================================================================= */
/* �֐���	:CPacketVERSION_RES_VERSIONCHECK::~CPacketVERSION_RES_VERSIONCHECK */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2006/11/05														 */
/* ========================================================================= */

CPacketVERSION_RES_VERSIONCHECK::~CPacketVERSION_RES_VERSIONCHECK()
{
}


/* ========================================================================= */
/* �֐���	:CPacketVERSION_RES_VERSIONCHECK::Make							 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2006/11/05														 */
/* ========================================================================= */

void CPacketVERSION_RES_VERSIONCHECK::Make(
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
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_VERSION_RES_VERSIONCHECK;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &nResult, sizeof (nResult), pDataTmp);		/* ���� */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* �֐���	:CPacketVERSION_RES_VERSIONCHECK::Set							 */
/* ���e		:�p�P�b�g��ݒ�													 */
/* ���t		:2006/11/05														 */
/* ========================================================================= */

PBYTE CPacketVERSION_RES_VERSIONCHECK::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_nResult, pDataTmp, sizeof (m_nResult), pDataTmp);

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2006 */
