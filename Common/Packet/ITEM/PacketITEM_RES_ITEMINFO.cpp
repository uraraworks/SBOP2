/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketITEM_RES_ITEMINFO.cpp								 */
/* ���e			:�R�}���h(�A�C�e���n:�A�C�e����񉞓�) �����t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/05/05													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "InfoItem.h"
#include "Command.h"
#include "PacketITEM_RES_ITEMINFO.h"

/* ========================================================================= */
/* �֐���	:CPacketITEM_RES_ITEMINFO::CPacketITEM_RES_ITEMINFO				 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/05/05														 */
/* ========================================================================= */

CPacketITEM_RES_ITEMINFO::CPacketITEM_RES_ITEMINFO()
{
	m_pInfo = NULL;
}


/* ========================================================================= */
/* �֐���	:CPacketITEM_RES_ITEMINFO::~CPacketITEM_RES_ITEMINFO			 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/05/05														 */
/* ========================================================================= */

CPacketITEM_RES_ITEMINFO::~CPacketITEM_RES_ITEMINFO()
{
	SAFE_DELETE (m_pInfo);
}


/* ========================================================================= */
/* �֐���	:CPacketITEM_RES_ITEMINFO::Make									 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2007/05/05														 */
/* ========================================================================= */

void CPacketITEM_RES_ITEMINFO::Make(
	CInfoItem *pInfo)		/* [in] �A�C�e����� */
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
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ITEM_RES_ITEMINFO;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, pInfoDataTmp, dwSizeDataTmp, pDataTmp);	/* �A�C�e����� */

	RenewPacket (pData, dwSize);

	SAFE_DELETE_ARRAY (pInfoDataTmp);
}


/* ========================================================================= */
/* �֐���	:CPacketITEM_RES_ITEMINFO::Set									 */
/* ���e		:�p�P�b�g��ݒ�													 */
/* ���t		:2007/05/05														 */
/* ========================================================================= */

PBYTE CPacketITEM_RES_ITEMINFO::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	SAFE_DELETE (m_pInfo);
	m_pInfo		= new CInfoItem;
	pDataTmp	= m_pInfo->SetSendData (pDataTmp);

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2007 */
