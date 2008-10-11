/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketADMIN_MAP_SETMAPSHADOW.cpp							 */
/* ���e			:�R�}���h(�Ǘ��Ҍn:�}�b�v�e�z�u) �����t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/06/07													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "InfoMapShadow.h"
#include "Command.h"
#include "PacketADMIN_MAP_SETMAPSHADOW.h"

/* ========================================================================= */
/* �֐���	:CPacketADMIN_MAP_SETMAPSHADOW::CPacketADMIN_MAP_SETMAPSHADOW	 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/06/07														 */
/* ========================================================================= */

CPacketADMIN_MAP_SETMAPSHADOW::CPacketADMIN_MAP_SETMAPSHADOW()
{
	m_ptPos.x		= 0;
	m_ptPos.y		= 0;
	m_dwMapID		= 0;
	m_dwShadowID	= 0;
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_MAP_SETMAPSHADOW::~CPacketADMIN_MAP_SETMAPSHADOW	 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/06/07														 */
/* ========================================================================= */

CPacketADMIN_MAP_SETMAPSHADOW::~CPacketADMIN_MAP_SETMAPSHADOW()
{
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_MAP_SETMAPSHADOW::Make							 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2007/06/07														 */
/* ========================================================================= */

void CPacketADMIN_MAP_SETMAPSHADOW::Make(
	DWORD dwMapID,			/* [in] �}�b�vID */
	int x,					/* [in] X���W */
	int y,					/* [in] Y���W */
	DWORD dwShadowID)		/* [in] �}�b�v�eID */
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
			 sizeof (dwShadowID);

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_ADMIN;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ADMIN_MAP_SETMAPSHADOW;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwMapID,	sizeof (dwMapID),		pDataTmp);	/* �}�b�vID */
	CopyMemoryRenew (pDataTmp, &ptPos,		sizeof (ptPos),			pDataTmp);	/* ���W */
	CopyMemoryRenew (pDataTmp, &dwShadowID,	sizeof (dwShadowID),	pDataTmp);	/* �}�b�v�eID */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_MAP_SETMAPSHADOW::Set								 */
/* ���e		:�p�P�b�g��ݒ�													 */
/* ���t		:2007/06/07														 */
/* ========================================================================= */

PBYTE CPacketADMIN_MAP_SETMAPSHADOW::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwMapID,	pDataTmp, sizeof (m_dwMapID),		pDataTmp);	/* �}�b�vID */
	CopyMemoryRenew (&m_ptPos,		pDataTmp, sizeof (m_ptPos),			pDataTmp);	/* ���W */
	CopyMemoryRenew (&m_dwShadowID,	pDataTmp, sizeof (m_dwShadowID),	pDataTmp);	/* �}�b�v�eID */

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2007 */
