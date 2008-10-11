/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:PacketSYSTEM_INFO.cpp										 */
/* ���e			:�R�}���h(�V�X�e���n:�V�X�e�����ʒm) �����t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/09/29													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "InfoSystem.h"
#include "PacketSYSTEM_INFO.h"

/* ========================================================================= */
/* �֐���	:CPacketSYSTEM_INFO::CPacketSYSTEM_INFO							 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/09/29														 */
/* ========================================================================= */

CPacketSYSTEM_INFO::CPacketSYSTEM_INFO()
{
	m_pInfoSystem = NULL;
}


/* ========================================================================= */
/* �֐���	:CPacketSYSTEM_INFO::~CPacketSYSTEM_INFO						 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/09/29														 */
/* ========================================================================= */

CPacketSYSTEM_INFO::~CPacketSYSTEM_INFO()
{
	SAFE_DELETE (m_pInfoSystem);
}


/* ========================================================================= */
/* �֐���	:CPacketSYSTEM_INFO::Make										 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2008/09/29														 */
/* ========================================================================= */

void CPacketSYSTEM_INFO::Make(
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

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_SYSTEM;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_SYSTEM_INFO;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, pDataInfoSystem, dwSizeInfoSystem, pDataTmp);	/* �V�X�e����� */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* �֐���	:CPacketSYSTEM_INFO::Set										 */
/* ���e		:�p�P�b�g��ݒ�													 */
/* ���t		:2008/09/29														 */
/* ========================================================================= */

PBYTE CPacketSYSTEM_INFO::Set(PBYTE pPacket)
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
