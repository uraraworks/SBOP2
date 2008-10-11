/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketADMIN_MAP_DELETEPARTS.cpp							 */
/* ���e			:�R�}���h(�Ǘ��Ҍn:�}�b�v�p�[�c�폜) �����t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/05/04													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketADMIN_MAP_DELETEPARTS.h"


/* ========================================================================= */
/* �֐���	:CPacketADMIN_MAP_DELETEPARTS::CPacketADMIN_MAP_DELETEPARTS		 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/05/04														 */
/* ========================================================================= */

CPacketADMIN_MAP_DELETEPARTS::CPacketADMIN_MAP_DELETEPARTS()
{
	m_dwPartsID = 0;
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_MAP_DELETEPARTS::~CPacketADMIN_MAP_DELETEPARTS	 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/05/04														 */
/* ========================================================================= */

CPacketADMIN_MAP_DELETEPARTS::~CPacketADMIN_MAP_DELETEPARTS()
{
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_MAP_DELETEPARTS::Make								 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2007/05/04														 */
/* ========================================================================= */

void CPacketADMIN_MAP_DELETEPARTS::Make(
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

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_ADMIN;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ADMIN_MAP_DELETEPARTS;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwPartsID, sizeof (dwPartsID), pDataTmp);	/* �p�[�cID */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_MAP_DELETEPARTS::Set								 */
/* ���e		:�p�P�b�g��ݒ�													 */
/* ���t		:2007/05/04														 */
/* ========================================================================= */

PBYTE CPacketADMIN_MAP_DELETEPARTS::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwPartsID,	pDataTmp, sizeof (m_dwPartsID), pDataTmp);	/* �p�[�cID */

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2007 */
