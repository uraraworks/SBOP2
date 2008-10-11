/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketITEM_ITEMINFO.cpp									 */
/* ���e			:�R�}���h(�A�C�e���n:�A�C�e�����ʒm) �����t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/05/05													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketITEM_ITEMINFO.h"

/* ========================================================================= */
/* �֐���	:CPacketITEM_ITEMINFO::CPacketITEM_ITEMINFO						 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/05/05														 */
/* ========================================================================= */

CPacketITEM_ITEMINFO::CPacketITEM_ITEMINFO()
{
	m_pLibInfo = NULL;
}


/* ========================================================================= */
/* �֐���	:CPacketITEM_ITEMINFO::~CPacketITEM_ITEMINFO					 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/05/05														 */
/* ========================================================================= */

CPacketITEM_ITEMINFO::~CPacketITEM_ITEMINFO()
{
	SAFE_DELETE (m_pLibInfo);
}


/* ========================================================================= */
/* �֐���	:CPacketITEM_ITEMINFO::Make										 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2007/05/05														 */
/* ========================================================================= */

void CPacketITEM_ITEMINFO::Make(
	PCLibInfoItem pInfo)		/* [in] �A�C�e����� */
{
	PBYTE pData, pDataTmp, pInfoDataTmp;
	DWORD dwSize, dwSizeDataTmp;
	PPACKETBASE pPacketBase;

	dwSizeDataTmp	= pInfo->GetSendDataSize ();
	pInfoDataTmp	= pInfo->GetSendData ();

	dwSize = sizeof (PACKETBASE) +
			 dwSizeDataTmp;

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_ITEM;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ITEM_ITEMINFO;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, pInfoDataTmp, dwSizeDataTmp, pDataTmp);	/* �A�C�e����� */

	RenewPacket (pData, dwSize);

	SAFE_DELETE_ARRAY (pInfoDataTmp);
}


/* ========================================================================= */
/* �֐���	:CPacketITEM_ITEMINFO::Set										 */
/* ���e		:�p�P�b�g��ݒ�													 */
/* ���t		:2007/05/05														 */
/* ========================================================================= */

PBYTE CPacketITEM_ITEMINFO::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	SAFE_DELETE (m_pLibInfo);
	m_pLibInfo	= new CLibInfoItem;
	m_pLibInfo->Create ();
	pDataTmp	= m_pLibInfo->SetSendData (pDataTmp);

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2007 */
