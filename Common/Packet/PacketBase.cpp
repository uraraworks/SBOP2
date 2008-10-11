/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* �t�@�C����	:PacketBase.cpp												 */
/* ���e			:�R�}���h���N���X �����t�@�C��							 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2006/08/01													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "PacketBase.h"

/* ========================================================================= */
/* �֐���	:CPacketBase::CPacketBase										 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2006/08/01														 */
/* ========================================================================= */

CPacketBase::CPacketBase()
{
	m_pPacket		= NULL;
	m_dwPacketSize	= 0;
	m_byCmdMain		= 0;
	m_byCmdSub		= 0;
}


/* ========================================================================= */
/* �֐���	:CPacketBase::~CPacketBase										 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2006/08/01														 */
/* ========================================================================= */

CPacketBase::~CPacketBase()
{
	SAFE_DELETE_ARRAY (m_pPacket);
}


/* ========================================================================= */
/* �֐���	:CPacketBase::GetSize											 */
/* ���e		:�p�P�b�g�T�C�Y���擾											 */
/* ���t		:2006/08/01														 */
/* ========================================================================= */

DWORD CPacketBase::GetSize(void)
{
	return m_dwPacketSize;
}


/* ========================================================================= */
/* �֐���	:CPacketBase::Get												 */
/* ���e		:�p�P�b�g���擾													 */
/* ���t		:2006/08/01														 */
/* ========================================================================= */

PBYTE CPacketBase::Get(void)
{
	return m_pPacket;
}


/* ========================================================================= */
/* �֐���	:CPacketBase::Set												 */
/* ���e		:�p�P�b�g��ݒ�													 */
/* ���t		:2006/11/05														 */
/* ========================================================================= */

PBYTE CPacketBase::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;
	PACKETBASE stPacketBase;

	pRet = pDataTmp = pPacket;

	CopyMemoryRenew ((PBYTE)&stPacketBase, pDataTmp, sizeof (stPacketBase), pDataTmp);

	m_byCmdMain	= stPacketBase.byCmdMain;		/* �R�}���hID(���C��) */
	m_byCmdSub	= stPacketBase.byCmdSub;		/* �R�}���hID(�T�u) */

	pRet = pDataTmp;
	return pRet;
}


/* ========================================================================= */
/* �֐���	:CPacketBase::RenewPacket										 */
/* ���e		:�p�P�b�g���X�V													 */
/* ���t		:2006/08/01														 */
/* ========================================================================= */

void CPacketBase::RenewPacket(PBYTE pPacket, DWORD dwSize)
{
	SAFE_DELETE_ARRAY (m_pPacket);

	m_pPacket		= pPacket;
	m_dwPacketSize	= dwSize;
}

/* Copyright(C)URARA-works 2006 */
