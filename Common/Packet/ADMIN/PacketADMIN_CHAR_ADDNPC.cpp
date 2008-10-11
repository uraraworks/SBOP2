/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketADMIN_CHAR_ADDNPC.cpp								 */
/* ���e			:�R�}���h(�Ǘ��Ҍn:NPC�̒ǉ�) �����t�@�C��					 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/09/01													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "InfoCharBase.h"
#include "Command.h"
#include "PacketADMIN_CHAR_ADDNPC.h"

/* ========================================================================= */
/* �֐���	:CPacketADMIN_CHAR_ADDNPC::CPacketADMIN_CHAR_ADDNPC				 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/09/01														 */
/* ========================================================================= */

CPacketADMIN_CHAR_ADDNPC::CPacketADMIN_CHAR_ADDNPC()
{
	m_pInfoChar = NULL;
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_CHAR_ADDNPC::~CPacketADMIN_CHAR_ADDNPC			 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/09/01														 */
/* ========================================================================= */

CPacketADMIN_CHAR_ADDNPC::~CPacketADMIN_CHAR_ADDNPC()
{
	SAFE_DELETE (m_pInfoChar);
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_CHAR_ADDNPC::Make									 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2007/09/01														 */
/* ========================================================================= */

void CPacketADMIN_CHAR_ADDNPC::Make(
	CInfoCharBase *pInfo)		/* [in] �L������� */
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

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_ADMIN;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ADMIN_CHAR_ADDNPC;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, pInfoDataTmp, dwSizeDataTmp, pDataTmp);	/* �L������� */

	RenewPacket (pData, dwSize);

	SAFE_DELETE_ARRAY (pInfoDataTmp);
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_CHAR_ADDNPC::Set									 */
/* ���e		:�p�P�b�g��ݒ�													 */
/* ���t		:2007/09/01														 */
/* ========================================================================= */

PBYTE CPacketADMIN_CHAR_ADDNPC::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	SAFE_DELETE (m_pInfoChar);
	m_pInfoChar	= new CInfoCharBase;
	pDataTmp	= m_pInfoChar->SetSendData (pDataTmp);

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2007 */
