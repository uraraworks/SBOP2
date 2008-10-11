/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketMAP_ONLINE.cpp										 */
/* ���e			:�R�}���h(�}�b�v�n:�I�����C�����ʒm) �����t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/02/10													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketMAP_ONLINE.h"

/* ========================================================================= */
/* �֐���	:CPacketMAP_ONLINE::CPacketMAP_ONLINE							 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/02/10														 */
/* ========================================================================= */

CPacketMAP_ONLINE::CPacketMAP_ONLINE()
{
	m_nCount = 0;
}


/* ========================================================================= */
/* �֐���	:CPacketMAP_ONLINE::~CPacketMAP_ONLINE							 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/02/10														 */
/* ========================================================================= */

CPacketMAP_ONLINE::~CPacketMAP_ONLINE()
{
}


/* ========================================================================= */
/* �֐���	:CPacketMAP_ONLINE::Make										 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2007/02/10														 */
/* ========================================================================= */

void CPacketMAP_ONLINE::Make(
	int nCount)		/* [in] �I�����C���� */
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE) +
			 sizeof (nCount);

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_MAP;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_MAP_ONLINE;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &nCount, sizeof (nCount), pDataTmp);	/* �I�����C���� */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* �֐���	:CPacketMAP_ONLINE::Set											 */
/* ���e		:�p�P�b�g��ݒ�													 */
/* ���t		:2007/02/10														 */
/* ========================================================================= */

PBYTE CPacketMAP_ONLINE::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_nCount, pDataTmp, sizeof (m_nCount), pDataTmp);	/* �I�����C���� */

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2007 */
