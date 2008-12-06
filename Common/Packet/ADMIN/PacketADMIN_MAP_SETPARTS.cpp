/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketADMIN_MAP_SETPARTS.cpp								 */
/* ���e			:�R�}���h(�Ǘ��Ҍn:�}�b�v�p�[�c�z�u) �����t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/04/30													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "InfoMapParts.h"
#include "Command.h"
#include "PacketADMIN_MAP_SETPARTS.h"

/* ========================================================================= */
/* �֐���	:CPacketADMIN_MAP_SETPARTS::CPacketADMIN_MAP_SETPARTS			 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/04/30														 */
/* ========================================================================= */

CPacketADMIN_MAP_SETPARTS::CPacketADMIN_MAP_SETPARTS()
{
	m_bPile		= FALSE;
	m_ptPos.x	= 0;
	m_ptPos.y	= 0;
	m_dwMapID	= 0;
	m_dwPartsID	= 0;
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_MAP_SETPARTS::~CPacketADMIN_MAP_SETPARTS			 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/04/30														 */
/* ========================================================================= */

CPacketADMIN_MAP_SETPARTS::~CPacketADMIN_MAP_SETPARTS()
{
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_MAP_SETPARTS::Make								 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2007/04/30														 */
/* ========================================================================= */

void CPacketADMIN_MAP_SETPARTS::Make(
	DWORD dwMapID,			/* [in] �}�b�vID */
	int x,					/* [in] X���W */
	int y,					/* [in] Y���W */
	DWORD dwPartsID,		/* [in] �p�[�cID */
	BOOL bPile)				/* [in] �d�ˍ��킹�p */
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	POINT ptPos;
	PPACKETBASE pPacketBase;

	ptPos.x = x;
	ptPos.y = y;

	dwSize = sizeof (PACKETBASE)	+
			 sizeof (ptPos)			+
			 sizeof (dwMapID)		+
			 sizeof (dwPartsID)		+
			 sizeof (bPile);

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_ADMIN;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ADMIN_MAP_SETPARTS;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwMapID,	sizeof (dwMapID),	pDataTmp);	/* �}�b�vID */
	CopyMemoryRenew (pDataTmp, &ptPos,		sizeof (ptPos),		pDataTmp);	/* ���W */
	CopyMemoryRenew (pDataTmp, &dwPartsID,	sizeof (dwPartsID),	pDataTmp);	/* �p�[�cID */
	CopyMemoryRenew (pDataTmp, &bPile,		sizeof (bPile),		pDataTmp);	/* �d�ˍ��킹�p */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_MAP_SETPARTS::Set									 */
/* ���e		:�p�P�b�g��ݒ�													 */
/* ���t		:2007/04/30														 */
/* ========================================================================= */

PBYTE CPacketADMIN_MAP_SETPARTS::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwMapID,	pDataTmp, sizeof (m_dwMapID),	pDataTmp);	/* �}�b�vID */
	CopyMemoryRenew (&m_ptPos,		pDataTmp, sizeof (m_ptPos),		pDataTmp);	/* ���W */
	CopyMemoryRenew (&m_dwPartsID,	pDataTmp, sizeof (m_dwPartsID),	pDataTmp);	/* �p�[�cID */
	CopyMemoryRenew (&m_bPile,		pDataTmp, sizeof (m_bPile),		pDataTmp);	/* �d�ˍ��킹�p */

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2007 */
