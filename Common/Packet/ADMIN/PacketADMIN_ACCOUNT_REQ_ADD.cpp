/* Copyright(C)URARA-works 2009 */
/* ========================================================================= */
/* �t�@�C����	:PacketADMIN_ACCOUNT_REQ_ADD.cpp							 */
/* ���e			:�R�}���h(�Ǘ��Ҍn:�A�J�E���g�̒ǉ��v��) �����t�@�C��		 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2009/01/14													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketADMIN_ACCOUNT_REQ_ADD.h"

/* ========================================================================= */
/* �֐���	:CPacketADMIN_ACCOUNT_REQ_ADD::CPacketADMIN_ACCOUNT_REQ_ADD		 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2009/01/14														 */
/* ========================================================================= */

CPacketADMIN_ACCOUNT_REQ_ADD::CPacketADMIN_ACCOUNT_REQ_ADD()
{
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_ACCOUNT_REQ_ADD::~CPacketADMIN_ACCOUNT_REQ_ADD	 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2009/01/14														 */
/* ========================================================================= */

CPacketADMIN_ACCOUNT_REQ_ADD::~CPacketADMIN_ACCOUNT_REQ_ADD()
{
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_ACCOUNT_REQ_ADD::Make								 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2009/01/14														 */
/* ========================================================================= */

void CPacketADMIN_ACCOUNT_REQ_ADD::Make(
	LPCSTR pszAccount,		/* [in] �A�J�E���g */
	LPCSTR pszPassword)		/* [in] �p�X���[�h */
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;
	CmyString strTmp;

	dwSize = sizeof (PACKETBASE) +
			 		(strlen (pszAccount) + 1) +
			 		(strlen (pszPassword) + 1);

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_ADMIN;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ADMIN_ACCOUNT_REQ_ADD;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	strTmp = pszAccount;
	strcpyRenew ((LPSTR)pDataTmp, strTmp, pDataTmp);	/* �A�J�E���g */
	strTmp = pszPassword;
	strcpyRenew ((LPSTR)pDataTmp, strTmp, pDataTmp);	/* �p�X���[�h */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_ACCOUNT_REQ_ADD::Set								 */
/* ���e		:�p�P�b�g��ݒ�													 */
/* ���t		:2009/01/14														 */
/* ========================================================================= */

PBYTE CPacketADMIN_ACCOUNT_REQ_ADD::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	StoreRenew (m_strAccount,	(LPCSTR)pDataTmp, pDataTmp);	/* �A�J�E���g */
	StoreRenew (m_strPassword,	(LPCSTR)pDataTmp, pDataTmp);	/* �p�X���[�h */

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2009 */
