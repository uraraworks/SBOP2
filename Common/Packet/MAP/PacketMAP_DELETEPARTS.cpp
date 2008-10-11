/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketMAP_DELETEPARTS.cpp									 */
/* ���e			:�R�}���h(�}�b�v�n:�}�b�v�p�[�c�폜) �����t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/05/04													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketMAP_DELETEPARTS.h"


/* ========================================================================= */
/* �֐���	:CPacketMAP_DELETEPARTS::CPacketMAP_DELETEPARTS					 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/05/04														 */
/* ========================================================================= */

CPacketMAP_DELETEPARTS::CPacketMAP_DELETEPARTS()
{
	m_dwPartsID = 0;
}


/* ========================================================================= */
/* �֐���	:CPacketMAP_DELETEPARTS::~CPacketMAP_DELETEPARTS				 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/05/04														 */
/* ========================================================================= */

CPacketMAP_DELETEPARTS::~CPacketMAP_DELETEPARTS()
{
}


/* ========================================================================= */
/* �֐���	:CPacketMAP_DELETEPARTS::Make									 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2007/05/04														 */
/* ========================================================================= */

void CPacketMAP_DELETEPARTS::Make(
	DWORD dwPartsID)		/* [in] �p�[�cID */
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE) +
			 sizeof (dwPartsID);

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_MAP;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_MAP_DELETEPARTS;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwPartsID, sizeof (dwPartsID), pDataTmp);	/* �p�[�cID */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* �֐���	:CPacketMAP_DELETEPARTS::Set									 */
/* ���e		:�p�P�b�g��ݒ�													 */
/* ���t		:2007/05/04														 */
/* ========================================================================= */

PBYTE CPacketMAP_DELETEPARTS::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwPartsID,	pDataTmp, sizeof (m_dwPartsID), pDataTmp);	/* �p�[�cID */

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2007 */
