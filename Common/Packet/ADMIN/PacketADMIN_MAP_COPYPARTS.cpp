/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketADMIN_MAP_COPYPARTS.cpp								 */
/* ���e			:�R�}���h(�Ǘ��Ҍn:�}�b�v�p�[�c�R�s�[) �����t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/06/14													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "InfoMapParts.h"
#include "Command.h"
#include "PacketADMIN_MAP_COPYPARTS.h"

/* ========================================================================= */
/* �֐���	:CPacketADMIN_MAP_COPYPARTS::CPacketADMIN_MAP_COPYPARTS			 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/06/14														 */
/* ========================================================================= */

CPacketADMIN_MAP_COPYPARTS::CPacketADMIN_MAP_COPYPARTS()
{
	m_ptViewPos.x	= 0;
	m_ptViewPos.y	= 0;
	m_dwSrcPartsID	= 0;
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_MAP_COPYPARTS::~CPacketADMIN_MAP_COPYPARTS		 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/06/14														 */
/* ========================================================================= */

CPacketADMIN_MAP_COPYPARTS::~CPacketADMIN_MAP_COPYPARTS()
{
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_MAP_COPYPARTS::Make								 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2007/06/14														 */
/* ========================================================================= */

void CPacketADMIN_MAP_COPYPARTS::Make(
	DWORD dwSrcPartsID,		/* [in] �R�s�[���p�[�cID */
	POINT ptViewPos)		/* [in] �\���ʒu */
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE)	+
			 sizeof (dwSrcPartsID)	+
			 sizeof (ptViewPos);

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_ADMIN;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ADMIN_MAP_COPYPARTS;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwSrcPartsID,	sizeof (dwSrcPartsID),	pDataTmp);	/* �R�s�[���p�[�cID */
	CopyMemoryRenew (pDataTmp, &ptViewPos,		sizeof (ptViewPos),		pDataTmp);	/* �\���ʒu */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_MAP_COPYPARTS::Set								 */
/* ���e		:�p�P�b�g��ݒ�													 */
/* ���t		:2007/06/14														 */
/* ========================================================================= */

PBYTE CPacketADMIN_MAP_COPYPARTS::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwSrcPartsID,	pDataTmp, sizeof (m_dwSrcPartsID),	pDataTmp);	/* �p�[�cID */
	CopyMemoryRenew (&m_ptViewPos,		pDataTmp, sizeof (m_ptViewPos),		pDataTmp);	/* ���W */

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2007 */
