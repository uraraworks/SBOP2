/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* �t�@�C����	:PacketCONNECT_REQ_LOGIN.cpp								 */
/* ���e			:�R�}���h(�ڑ��n:���O�C���v��) �����t�@�C��					 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2006/11/05													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketCONNECT_REQ_LOGIN.h"

/* ========================================================================= */
/* �֐���	:CPacketCONNECT_REQ_LOGIN::CPacketCONNECT_REQ_LOGIN				 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2006/11/05														 */
/* ========================================================================= */

CPacketCONNECT_REQ_LOGIN::CPacketCONNECT_REQ_LOGIN()
{
	ZeroMemory (m_byMacAddr, sizeof (m_byMacAddr));
}


/* ========================================================================= */
/* �֐���	:CPacketCONNECT_REQ_LOGIN::~CPacketCONNECT_REQ_LOGIN			 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2006/11/05														 */
/* ========================================================================= */

CPacketCONNECT_REQ_LOGIN::~CPacketCONNECT_REQ_LOGIN()
{
}


/* ========================================================================= */
/* �֐���	:CPacketCONNECT_REQ_LOGIN::Make									 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2006/11/05														 */
/* ========================================================================= */

void CPacketCONNECT_REQ_LOGIN::Make(
	LPCSTR pszAccount,		/* [in] �A�J�E���g */
	LPCSTR pszPassword,		/* [in] �p�X���[�h */
	PBYTE pbyMacAddr)		/* [in] MAC�A�h���X */
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE)		+
			 (strlen (pszAccount) + 1)	+
			 (strlen (pszPassword) + 1)	+
			 6;

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_CONNECT;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_CONNECT_REQ_LOGIN;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	strcpyRenew ((LPSTR)pDataTmp, pszAccount, pDataTmp);		/* �A�J�E���g */
	strcpyRenew ((LPSTR)pDataTmp, pszPassword, pDataTmp);		/* �p�X���[�h */
	CopyMemoryRenew (pDataTmp, pbyMacAddr, 6, pDataTmp);		/* MAC�A�h���X*/

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* �֐���	:CPacketCONNECT_REQ_LOGIN::Set									 */
/* ���e		:�p�P�b�g��ݒ�													 */
/* ���t		:2006/11/05														 */
/* ========================================================================= */

PBYTE CPacketCONNECT_REQ_LOGIN::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	StoreRenew (m_strAccount,	(LPCSTR)pDataTmp, pDataTmp);		/* �A�J�E���g */
	StoreRenew (m_strPassword,	(LPCSTR)pDataTmp, pDataTmp);		/* �p�X���[�h */
	CopyMemoryRenew (m_byMacAddr, pDataTmp, 6, pDataTmp);			/* MAC�A�h���X*/

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2006 */
