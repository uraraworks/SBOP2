/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketITEM_RENEWITEMTYPEINFO.cpp							 */
/* ���e			:�R�}���h(�A�C�e���n:�A�C�e����ʏ��X�V) �����t�@�C��		 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/10/03													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "LibInfoItemType.h"
#include "Command.h"
#include "PacketITEM_RENEWITEMTYPEINFO.h"

/* ========================================================================= */
/* �֐���	:CPacketITEM_RENEWITEMTYPEINFO::CPacketITEM_RENEWITEMTYPEINFO	 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/10/03														 */
/* ========================================================================= */

CPacketITEM_RENEWITEMTYPEINFO::CPacketITEM_RENEWITEMTYPEINFO()
{
	m_pInfo = NULL;
}


/* ========================================================================= */
/* �֐���	:CPacketITEM_RENEWITEMTYPEINFO::~CPacketITEM_RENEWITEMTYPEINFO	 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/10/03														 */
/* ========================================================================= */

CPacketITEM_RENEWITEMTYPEINFO::~CPacketITEM_RENEWITEMTYPEINFO()
{
	SAFE_DELETE (m_pInfo);
}


/* ========================================================================= */
/* �֐���	:CPacketITEM_RENEWITEMTYPEINFO::Make							 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2007/10/03														 */
/* ========================================================================= */

void CPacketITEM_RENEWITEMTYPEINFO::Make(
	CInfoItemTypeBase *pInfo)		/* [in] �A�C�e����ʏ�� */
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
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ITEM_RENEWITEMTYPEINFO;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, pInfoDataTmp, dwSizeDataTmp, pDataTmp);	/* �A�C�e����ʏ�� */

	RenewPacket (pData, dwSize);

	SAFE_DELETE_ARRAY (pInfoDataTmp);
}


/* ========================================================================= */
/* �֐���	:CPacketITEM_RENEWITEMTYPEINFO::Set								 */
/* ���e		:�p�P�b�g��ݒ�													 */
/* ���t		:2007/10/03														 */
/* ========================================================================= */

PBYTE CPacketITEM_RENEWITEMTYPEINFO::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;
	CLibInfoItemType LibInfo;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);
	pDataTmp	= LibInfo.GetNewSendData (pDataTmp, m_pInfo);

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2007 */
