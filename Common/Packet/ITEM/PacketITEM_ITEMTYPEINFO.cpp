/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketITEM_ITEMTYPEINFO.cpp								 */
/* ���e			:�R�}���h(�A�C�e���n:�A�C�e����ʏ��ʒm) �����t�@�C��		 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/10/03													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "LibInfoItemType.h"
#include "Command.h"
#include "PacketITEM_ITEMTYPEINFO.h"

/* ========================================================================= */
/* �֐���	:CPacketITEM_ITEMTYPEINFO::CPacketITEM_ITEMTYPEINFO				 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/10/03														 */
/* ========================================================================= */

CPacketITEM_ITEMTYPEINFO::CPacketITEM_ITEMTYPEINFO()
{
	m_pLibInfo = NULL;
}


/* ========================================================================= */
/* �֐���	:CPacketITEM_ITEMTYPEINFO::~CPacketITEM_ITEMTYPEINFO			 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/10/03														 */
/* ========================================================================= */

CPacketITEM_ITEMTYPEINFO::~CPacketITEM_ITEMTYPEINFO()
{
	SAFE_DELETE (m_pLibInfo);
}


/* ========================================================================= */
/* �֐���	:CPacketITEM_ITEMTYPEINFO::Make									 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2007/10/03														 */
/* ========================================================================= */

void CPacketITEM_ITEMTYPEINFO::Make(
	PCLibInfoItemType pInfo)		/* [in] �A�C�e����ʏ�� */
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
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ITEM_ITEMTYPEINFO;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, pInfoDataTmp, dwSizeDataTmp, pDataTmp);	/* �A�C�e����ʏ�� */

	RenewPacket (pData, dwSize);

	SAFE_DELETE_ARRAY (pInfoDataTmp);
}


/* ========================================================================= */
/* �֐���	:CPacketITEM_ITEMTYPEINFO::Set									 */
/* ���e		:�p�P�b�g��ݒ�													 */
/* ���t		:2007/10/03														 */
/* ========================================================================= */

PBYTE CPacketITEM_ITEMTYPEINFO::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	SAFE_DELETE (m_pLibInfo);
	m_pLibInfo	= new CLibInfoItemType;
	m_pLibInfo->Create ();
	pDataTmp	= m_pLibInfo->SetSendData (pDataTmp);

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2007 */
