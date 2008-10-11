/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketMAP_REQ_MAPINFO.cpp									 */
/* ���e			:�R�}���h(�}�b�v�n:�}�b�v���v��) �����t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/01/04													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketMAP_REQ_MAPINFO.h"

/* ========================================================================= */
/* �֐���	:CPacketMAP_REQ_MAPINFO::CPacketMAP_REQ_MAPINFO					 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/01/04														 */
/* ========================================================================= */

CPacketMAP_REQ_MAPINFO::CPacketMAP_REQ_MAPINFO()
{
	m_dwMapID = 0;
}


/* ========================================================================= */
/* �֐���	:CPacketMAP_REQ_MAPINFO::~CPacketMAP_REQ_MAPINFO				 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/01/04														 */
/* ========================================================================= */

CPacketMAP_REQ_MAPINFO::~CPacketMAP_REQ_MAPINFO()
{
}


/* ========================================================================= */
/* �֐���	:CPacketMAP_REQ_MAPINFO::Make									 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2007/01/04														 */
/* ========================================================================= */

void CPacketMAP_REQ_MAPINFO::Make(
	DWORD dwMapID)		/* [in] �}�b�vID */
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE) +
			 sizeof (dwMapID);

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_MAP;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_MAP_REQ_MAPINFO;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwMapID, sizeof (dwMapID), pDataTmp);	/* �}�b�vID */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* �֐���	:CPacketMAP_REQ_MAPINFO::Set									 */
/* ���e		:�p�P�b�g��ݒ�													 */
/* ���t		:2007/01/04														 */
/* ========================================================================= */

PBYTE CPacketMAP_REQ_MAPINFO::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwMapID, pDataTmp, sizeof (m_dwMapID), pDataTmp);	/* �}�b�vID */

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2007 */
