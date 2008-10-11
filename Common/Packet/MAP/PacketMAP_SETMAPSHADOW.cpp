/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketMAP_SETMAPSHADOW.cpp									 */
/* ���e			:�R�}���h(�}�b�v�n:�}�b�v�e�z�u) �����t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/06/08													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "InfoMapShadow.h"
#include "Command.h"
#include "PacketMAP_SETMAPSHADOW.h"

/* ========================================================================= */
/* �֐���	:CPacketMAP_SETMAPSHADOW::CPacketMAP_SETMAPSHADOW				 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/06/08														 */
/* ========================================================================= */

CPacketMAP_SETMAPSHADOW::CPacketMAP_SETMAPSHADOW()
{
	m_ptPos.x		= 0;
	m_ptPos.y		= 0;
	m_dwMapID		= 0;
	m_dwShadowID	= 0;
}


/* ========================================================================= */
/* �֐���	:CPacketMAP_SETMAPSHADOW::~CPacketMAP_SETMAPSHADOW				 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/06/08														 */
/* ========================================================================= */

CPacketMAP_SETMAPSHADOW::~CPacketMAP_SETMAPSHADOW()
{
}


/* ========================================================================= */
/* �֐���	:CPacketMAP_SETMAPSHADOW::Make									 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2007/06/08														 */
/* ========================================================================= */

void CPacketMAP_SETMAPSHADOW::Make(
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

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_MAP;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_MAP_SETMAPSHADOW;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwMapID,	sizeof (dwMapID),		pDataTmp);	/* �}�b�vID */
	CopyMemoryRenew (pDataTmp, &ptPos,		sizeof (ptPos),			pDataTmp);	/* ���W */
	CopyMemoryRenew (pDataTmp, &dwShadowID,	sizeof (dwShadowID),	pDataTmp);	/* �}�b�v�eID */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* �֐���	:CPacketMAP_SETMAPSHADOW::Set									 */
/* ���e		:�p�P�b�g��ݒ�													 */
/* ���t		:2007/06/08														 */
/* ========================================================================= */

PBYTE CPacketMAP_SETMAPSHADOW::Set(PBYTE pPacket)
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
