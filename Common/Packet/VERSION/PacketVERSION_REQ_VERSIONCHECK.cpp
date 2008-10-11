/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* �t�@�C����	:PacketVERSION_REQ_VERSIONCHECK.cpp							 */
/* ���e			:�R�}���h(�o�[�W�����n:�o�[�W�����`�F�b�N�v��) �����t�@�C��	 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2006/11/05													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketVERSION_REQ_VERSIONCHECK.h"

/* ========================================================================= */
/* �֐���	:CPacketVERSION_REQ_VERSIONCHECK::CPacketVERSION_REQ_VERSIONCHECK */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2006/11/05														 */
/* ========================================================================= */

CPacketVERSION_REQ_VERSIONCHECK::CPacketVERSION_REQ_VERSIONCHECK()
{
	m_dwVersion = 0;
}


/* ========================================================================= */
/* �֐���	:CPacketVERSION_REQ_VERSIONCHECK::~CPacketVERSION_REQ_VERSIONCHECK */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2006/11/05														 */
/* ========================================================================= */

CPacketVERSION_REQ_VERSIONCHECK::~CPacketVERSION_REQ_VERSIONCHECK()
{
}


/* ========================================================================= */
/* �֐���	:CPacketVERSION_REQ_VERSIONCHECK::Make							 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2006/11/05														 */
/* ========================================================================= */

void CPacketVERSION_REQ_VERSIONCHECK::Make(
	DWORD dwVersion)		/* [in] �o�[�W�����R�[�h */
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE) +
			 sizeof (dwVersion);

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_VERSION;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_VERSION_REQ_VERSIONCHECK;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwVersion, sizeof (dwVersion), pDataTmp);	/* �o�[�W�����R�[�h */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* �֐���	:CPacketVERSION_REQ_VERSIONCHECK::Set							 */
/* ���e		:�p�P�b�g��ݒ�													 */
/* ���t		:2006/11/05														 */
/* ========================================================================= */

PBYTE CPacketVERSION_REQ_VERSIONCHECK::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwVersion, pDataTmp, sizeof (m_dwVersion), pDataTmp);

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2006 */
