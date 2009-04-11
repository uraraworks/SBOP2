/* Copyright(C)URARA-works 2009 */
/* ========================================================================= */
/* �t�@�C����	:PacketADMIN_DISABLE_RENEWINFO.cpp							 */
/* ���e			:�R�}���h(�Ǘ��Ҍn:���ۏ��̍X�V) �����t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2009/04/11													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "LibInfoDisable.h"
#include "PacketADMIN_DISABLE_RENEWINFO.h"

/* ========================================================================= */
/* �֐���	:CPacketADMIN_DISABLE_RENEWINFO::CPacketADMIN_DISABLE_RENEWINFO	 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2009/04/11														 */
/* ========================================================================= */

CPacketADMIN_DISABLE_RENEWINFO::CPacketADMIN_DISABLE_RENEWINFO()
{
	m_pLibInfoDisable = NULL;
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_DISABLE_RENEWINFO::~CPacketADMIN_DISABLE_RENEWINFO */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2009/04/11														 */
/* ========================================================================= */

CPacketADMIN_DISABLE_RENEWINFO::~CPacketADMIN_DISABLE_RENEWINFO()
{
	SAFE_DELETE (m_pLibInfoDisable);
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_DISABLE_RENEWINFO::Make							 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2009/04/11														 */
/* ========================================================================= */

void CPacketADMIN_DISABLE_RENEWINFO::Make(
	CLibInfoDisable *pLibInfoDisable)		/* [in] ���ۏ�� */
{
	PBYTE pData, pDataTmp;
	DWORD dwSize, dwSizeLibInfoDisable;
	PBYTE pDataLibInfoDisable;
	PPACKETBASE pPacketBase;

	dwSizeLibInfoDisable = pLibInfoDisable->GetSendDataSize ();
	pDataLibInfoDisable	 = pLibInfoDisable->GetSendData ();

	dwSize = sizeof (PACKETBASE);
	dwSize += dwSizeLibInfoDisable;

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_ADMIN;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ADMIN_DISABLE_RENEWINFO;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, pDataLibInfoDisable, dwSizeLibInfoDisable, pDataTmp);	/* ���ۏ�� */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_DISABLE_RENEWINFO::Make							 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2009/04/11														 */
/* ========================================================================= */

void CPacketADMIN_DISABLE_RENEWINFO::Make(
	CInfoDisable *pInfoDisable)		/* [in] ���ۏ�� */
{
	PCInfoDisable pInfoDisableTmp;
	CLibInfoDisable LibInfo;

	pInfoDisableTmp = (PCInfoDisable)LibInfo.GetNew ();
	pInfoDisableTmp->Copy (pInfoDisable);
	LibInfo.Create ();
	LibInfo.Add (pInfoDisableTmp);
	Make (&LibInfo);
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_DISABLE_RENEWINFO::Set							 */
/* ���e		:�p�P�b�g��ݒ�													 */
/* ���t		:2009/04/11														 */
/* ========================================================================= */

PBYTE CPacketADMIN_DISABLE_RENEWINFO::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	SAFE_DELETE (m_pLibInfoDisable);
	m_pLibInfoDisable = new CLibInfoDisable;
	m_pLibInfoDisable->Create ();

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);
	pDataTmp	= m_pLibInfoDisable->SetSendData (pDataTmp);

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2009 */
