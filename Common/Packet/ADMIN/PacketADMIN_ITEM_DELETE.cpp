/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketADMIN_ITEM_DELETE.cpp								 */
/* ���e			:�R�}���h(�Ǘ��Ҍn:�A�C�e�����폜) �����t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/08/19													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketADMIN_ITEM_DELETE.h"


/* ========================================================================= */
/* �֐���	:CPacketADMIN_ITEM_DELETE::CPacketADMIN_ITEM_DELETE				 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/08/19														 */
/* ========================================================================= */

CPacketADMIN_ITEM_DELETE::CPacketADMIN_ITEM_DELETE()
{
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_ITEM_DELETE::~CPacketADMIN_ITEM_DELETE			 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/08/19														 */
/* ========================================================================= */

CPacketADMIN_ITEM_DELETE::~CPacketADMIN_ITEM_DELETE()
{
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_ITEM_DELETE::Make									 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2007/08/19														 */
/* ========================================================================= */

void CPacketADMIN_ITEM_DELETE::Make(
	DWORD dwItemID)		/* [in] �A�C�e��ID */
{
	ARRAYDWORD adwItemID;

	adwItemID.Add (dwItemID);
	Make (&adwItemID);
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_ITEM_DELETE::Make									 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2008/11/29														 */
/* ========================================================================= */

void CPacketADMIN_ITEM_DELETE::Make(
	PARRAYDWORD padwItemID)		/* [in] �A�C�e��ID */
{
	int i, nCount;
	PBYTE pData, pDataTmp;
	DWORD dwItemID, dwSize;
	PPACKETBASE pPacketBase;

	nCount = padwItemID->GetSize ();

	dwSize = sizeof (PACKETBASE) +
			 ((nCount + 1) * sizeof (DWORD));

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_ADMIN;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ADMIN_ITEM_DELETE;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	for (i = 0; i < nCount; i ++) {
		dwItemID = padwItemID->GetAt (i);
		CopyMemoryRenew (pDataTmp, &dwItemID, sizeof (dwItemID), pDataTmp);	/* �A�C�e��ID */
	}
	/* �I�[�p */
	dwItemID = 0;
	CopyMemoryRenew (pDataTmp, &dwItemID, sizeof (dwItemID), pDataTmp);	/* �A�C�e��ID */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_ITEM_DELETE::Set									 */
/* ���e		:�p�P�b�g��ݒ�													 */
/* ���t		:2007/08/19														 */
/* ========================================================================= */

PBYTE CPacketADMIN_ITEM_DELETE::Set(PBYTE pPacket)
{
	DWORD dwItemID;
	PBYTE pRet, pDataTmp;

	m_adwItemID.RemoveAll ();

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	while (1) {
		CopyMemoryRenew (&dwItemID, pDataTmp, sizeof (dwItemID), pDataTmp);	/* �A�C�e��ID */
		if (dwItemID == 0) {
			break;
		}
		m_adwItemID.Add (dwItemID);
	}

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2007 */
