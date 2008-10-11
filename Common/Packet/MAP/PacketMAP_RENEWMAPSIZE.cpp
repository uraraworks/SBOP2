/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketMAP_RENEWMAPSIZE.cpp									 */
/* ���e			:�R�}���h(�}�b�v�n:�}�b�v�T�C�Y�X�V) �����t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/05/01													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "InfoMapParts.h"
#include "Command.h"
#include "PacketMAP_RENEWMAPSIZE.h"

/* ========================================================================= */
/* �֐���	:CPacketMAP_RENEWMAPSIZE::CPacketMAP_RENEWMAPSIZE				 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/05/01														 */
/* ========================================================================= */

CPacketMAP_RENEWMAPSIZE::CPacketMAP_RENEWMAPSIZE()
{
	m_dwMapID		= 0;
	m_nDirection	= 0;
	m_nSize			= 0;
}


/* ========================================================================= */
/* �֐���	:CPacketMAP_RENEWMAPSIZE::~CPacketMAP_RENEWMAPSIZE				 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/05/01														 */
/* ========================================================================= */

CPacketMAP_RENEWMAPSIZE::~CPacketMAP_RENEWMAPSIZE()
{
}


/* ========================================================================= */
/* �֐���	:CPacketMAP_RENEWMAPSIZE::Make									 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2007/05/01														 */
/* ========================================================================= */

void CPacketMAP_RENEWMAPSIZE::Make(
	DWORD dwMapID,		/* [in] �}�b�vID */
	int nDirection,		/* [in] ����������� */
	int nSize)			/* [in] �������鋗�� */
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE)	+
			 sizeof (dwMapID)		+
			 sizeof (nDirection)	+
			 sizeof (nSize);

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_MAP;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_MAP_RENEWMAPSIZE;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwMapID,	sizeof (dwMapID),		pDataTmp);	/* �}�b�vID */
	CopyMemoryRenew (pDataTmp, &nDirection,	sizeof (nDirection),	pDataTmp);	/* ����������� */
	CopyMemoryRenew (pDataTmp, &nSize,		sizeof (nSize),			pDataTmp);	/* �������鋗�� */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* �֐���	:CPacketMAP_RENEWMAPSIZE::Set									 */
/* ���e		:�p�P�b�g��ݒ�													 */
/* ���t		:2007/05/01														 */
/* ========================================================================= */

PBYTE CPacketMAP_RENEWMAPSIZE::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwMapID,	pDataTmp, sizeof (m_dwMapID),		pDataTmp);	/* �}�b�vID */
	CopyMemoryRenew (&m_nDirection,	pDataTmp, sizeof (m_nDirection),	pDataTmp);	/* ����������� */
	CopyMemoryRenew (&m_nSize,		pDataTmp, sizeof (m_nSize),			pDataTmp);	/* �������鋗�� */

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2007 */
