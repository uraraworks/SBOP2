/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:PacketADMIN_RENEW_CLIENTVERSION.cpp						 */
/* ���e			:�R�}���h(�Ǘ��Ҍn:�N���C�A���g�o�[�W�����X�V) �����t�@�C��	 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/06/07													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketADMIN_RENEW_CLIENTVERSION.h"

/* ========================================================================= */
/* �֐���	:CPacketADMIN_RENEW_CLIENTVERSION::CPacketADMIN_RENEW_CLIENTVERSION */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/06/07														 */
/* ========================================================================= */

CPacketADMIN_RENEW_CLIENTVERSION::CPacketADMIN_RENEW_CLIENTVERSION()
{
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_RENEW_CLIENTVERSION::~CPacketADMIN_RENEW_CLIENTVERSION */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/06/07														 */
/* ========================================================================= */

CPacketADMIN_RENEW_CLIENTVERSION::~CPacketADMIN_RENEW_CLIENTVERSION()
{
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_RENEW_CLIENTVERSION::Make							 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2008/06/07														 */
/* ========================================================================= */

void CPacketADMIN_RENEW_CLIENTVERSION::Make(
	LPCSTR pszClientVersion)		/* [in] �N���C�A���g�o�[�W���� */
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE) +
			(strlen (pszClientVersion) + 1);

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_ADMIN;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ADMIN_RENEW_CLIENTVERSION;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, (PBYTE)pszClientVersion, strlen (pszClientVersion) + 1, pDataTmp);	/* �N���C�A���g�o�[�W���� */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_RENEW_CLIENTVERSION::Set							 */
/* ���e		:�p�P�b�g��ݒ�													 */
/* ���t		:2008/06/07														 */
/* ========================================================================= */

PBYTE CPacketADMIN_RENEW_CLIENTVERSION::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	StoreRenew (m_strClientVersion, (LPCSTR)pDataTmp, pDataTmp);	/* �N���C�A���g�o�[�W���� */

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2008 */
