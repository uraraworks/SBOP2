/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* �t�@�C����	:PacketACCOUNT_RES_ACCOUNTINFO.cpp							 */
/* ���e			:�R�}���h(�A�J�E���g�n:�A�J�E���g��񉞓�) �����t�@�C��		 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2006/11/07													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "InfoAccount.h"
#include "Command.h"
#include "PacketACCOUNT_RES_ACCOUNTINFO.h"

/* ========================================================================= */
/* �֐���	:CPacketACCOUNT_RES_ACCOUNTINFO::CPacketACCOUNT_RES_ACCOUNTINFO	 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2006/11/07														 */
/* ========================================================================= */

CPacketACCOUNT_RES_ACCOUNTINFO::CPacketACCOUNT_RES_ACCOUNTINFO()
{
	m_pInfo = NULL;
}


/* ========================================================================= */
/* �֐���	:CPacketACCOUNT_RES_ACCOUNTINFO::~CPacketACCOUNT_RES_ACCOUNTINFO */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2006/11/07														 */
/* ========================================================================= */

CPacketACCOUNT_RES_ACCOUNTINFO::~CPacketACCOUNT_RES_ACCOUNTINFO()
{
	SAFE_DELETE (m_pInfo);
}


/* ========================================================================= */
/* �֐���	:CPacketACCOUNT_RES_ACCOUNTINFO::Make							 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2006/11/07														 */
/* ========================================================================= */

void CPacketACCOUNT_RES_ACCOUNTINFO::Make(
	CInfoAccount *pInfo)		/* [in] �A�J�E���g��� */
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

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_ACCOUNT;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ACCOUNT_RES_ACCOUNTINFO;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, pInfoDataTmp, dwSizeDataTmp, pDataTmp);	/* �A�J�E���g��� */

	RenewPacket (pData, dwSize);

	SAFE_DELETE_ARRAY (pInfoDataTmp);
}


/* ========================================================================= */
/* �֐���	:CPacketACCOUNT_RES_ACCOUNTINFO::Set							 */
/* ���e		:�p�P�b�g��ݒ�													 */
/* ���t		:2006/11/07														 */
/* ========================================================================= */

PBYTE CPacketACCOUNT_RES_ACCOUNTINFO::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	SAFE_DELETE (m_pInfo);
	m_pInfo		= new CInfoAccount;
	pDataTmp	= m_pInfo->SetSendData (pDataTmp);

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2006 */
