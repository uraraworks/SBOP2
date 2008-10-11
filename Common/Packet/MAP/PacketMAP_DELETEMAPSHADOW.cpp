/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketMAP_DELETEMAPSHADOW.cpp								 */
/* ���e			:�R�}���h(�}�b�v�n:�}�b�v�e�폜) �����t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/06/08													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketMAP_DELETEMAPSHADOW.h"


/* ========================================================================= */
/* �֐���	:CPacketMAP_DELETEMAPSHADOW::CPacketMAP_DELETEMAPSHADOW			 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/06/08														 */
/* ========================================================================= */

CPacketMAP_DELETEMAPSHADOW::CPacketMAP_DELETEMAPSHADOW()
{
	m_dwShadowID = 0;
}


/* ========================================================================= */
/* �֐���	:CPacketMAP_DELETEMAPSHADOW::~CPacketMAP_DELETEMAPSHADOW		 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/06/08														 */
/* ========================================================================= */

CPacketMAP_DELETEMAPSHADOW::~CPacketMAP_DELETEMAPSHADOW()
{
}


/* ========================================================================= */
/* �֐���	:CPacketMAP_DELETEMAPSHADOW::Make								 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2007/06/08														 */
/* ========================================================================= */

void CPacketMAP_DELETEMAPSHADOW::Make(
	DWORD dwShadowID)		/* [in] �}�b�v�eID */
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE) +
			 sizeof (dwShadowID);

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_MAP;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_MAP_DELETEMAPSHADOW;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwShadowID, sizeof (dwShadowID), pDataTmp);	/* �}�b�v�eID */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* �֐���	:CPacketMAP_DELETEMAPSHADOW::Set								 */
/* ���e		:�p�P�b�g��ݒ�													 */
/* ���t		:2007/06/08														 */
/* ========================================================================= */

PBYTE CPacketMAP_DELETEMAPSHADOW::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwShadowID,	pDataTmp, sizeof (m_dwShadowID), pDataTmp);	/* �}�b�v�eID */

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2007 */
