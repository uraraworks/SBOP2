/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:PacketADMIN_SYSTEM_RENEWINFO.cpp							 */
/* ���e			:�R�}���h(�Ǘ��Ҍn:�V�X�e�����̍X�V) �����t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/10/04													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "InfoSystem.h"
#include "PacketADMIN_SYSTEM_RENEWINFO.h"

/* ========================================================================= */
/* �֐���	:CPacketADMIN_SYSTEM_RENEWINFO::CPacketADMIN_SYSTEM_RENEWINFO	 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/10/04														 */
/* ========================================================================= */

CPacketADMIN_SYSTEM_RENEWINFO::CPacketADMIN_SYSTEM_RENEWINFO()
{
	m_pInfoSystem = NULL;
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_SYSTEM_RENEWINFO::~CPacketADMIN_SYSTEM_RENEWINFO	 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/10/04														 */
/* ========================================================================= */

CPacketADMIN_SYSTEM_RENEWINFO::~CPacketADMIN_SYSTEM_RENEWINFO()
{
	SAFE_DELETE (m_pInfoSystem);
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_SYSTEM_RENEWINFO::Make							 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2008/10/04														 */
/* ========================================================================= */

void CPacketADMIN_SYSTEM_RENEWINFO::Make(
	CInfoSystem *pInfoSystem)		/* [in] �V�X�e����� */
{
	PBYTE pData, pDataTmp;
	DWORD dwSize, dwSizeInfoSystem;
	PBYTE pDataInfoSystem;
	PPACKETBASE pPacketBase;

	dwSizeInfoSystem = pInfoSystem->GetSendDataSize ();
	pDataInfoSystem	 = pInfoSystem->GetSendData ();

	dwSize = sizeof (PACKETBASE);
	dwSize += dwSizeInfoSystem;

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_ADMIN;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ADMIN_SYSTEM_RENEWINFO;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, pDataInfoSystem, dwSizeInfoSystem, pDataTmp);	/* �V�X�e����� */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_SYSTEM_RENEWINFO::Set								 */
/* ���e		:�p�P�b�g��ݒ�													 */
/* ���t		:2008/10/04														 */
/* ========================================================================= */

PBYTE CPacketADMIN_SYSTEM_RENEWINFO::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	SAFE_DELETE (m_pInfoSystem);
	m_pInfoSystem = new CInfoSystem;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);
	pDataTmp	= m_pInfoSystem->SetSendData (pDataTmp);

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2008 */
