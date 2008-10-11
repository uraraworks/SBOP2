/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketADMIN_MAP_DELETEMAPSHADOW.cpp						 */
/* ���e			:�R�}���h(�Ǘ��Ҍn:�}�b�v�e�폜) �����t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/06/07													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketADMIN_MAP_DELETEMAPSHADOW.h"


/* ========================================================================= */
/* �֐���	:CPacketADMIN_MAP_DELETEMAPSHADOW::CPacketADMIN_MAP_DELETEMAPSHADOW	 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/06/07														 */
/* ========================================================================= */

CPacketADMIN_MAP_DELETEMAPSHADOW::CPacketADMIN_MAP_DELETEMAPSHADOW()
{
	m_dwShadowID = 0;
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_MAP_DELETEMAPSHADOW::~CPacketADMIN_MAP_DELETEMAPSHADOW */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/06/07														 */
/* ========================================================================= */

CPacketADMIN_MAP_DELETEMAPSHADOW::~CPacketADMIN_MAP_DELETEMAPSHADOW()
{
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_MAP_DELETEMAPSHADOW::Make							 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2007/06/07														 */
/* ========================================================================= */

void CPacketADMIN_MAP_DELETEMAPSHADOW::Make(
	DWORD dwShadowID)		/* [in] �eID */
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE) +
			 sizeof (dwShadowID);

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_ADMIN;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ADMIN_MAP_DELETEMAPSHADOW;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwShadowID, sizeof (dwShadowID), pDataTmp);	/* �eID */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_MAP_DELETEMAPSHADOW::Set							 */
/* ���e		:�p�P�b�g��ݒ�													 */
/* ���t		:2007/06/07														 */
/* ========================================================================= */

PBYTE CPacketADMIN_MAP_DELETEMAPSHADOW::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwShadowID,	pDataTmp, sizeof (m_dwShadowID), pDataTmp);	/* �eID */

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2007 */
