/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketMAP_SYSTEMMSG.cpp									 */
/* ���e			:�R�}���h(�}�b�v�n:�V�X�e�����b�Z�[�W�ʒm) �����t�@�C��		 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/02/10													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketMAP_SYSTEMMSG.h"

/* ========================================================================= */
/* �֐���	:CPacketMAP_SYSTEMMSG::CPacketMAP_SYSTEMMSG						 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/02/10														 */
/* ========================================================================= */

CPacketMAP_SYSTEMMSG::CPacketMAP_SYSTEMMSG()
{
	m_nMsgType	= SYSTEMMSGTYPE_DEFAULT;
	m_bSound	= TRUE;
	m_clMsg		= 0;
}


/* ========================================================================= */
/* �֐���	:CPacketMAP_SYSTEMMSG::~CPacketMAP_SYSTEMMSG					 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/02/10														 */
/* ========================================================================= */

CPacketMAP_SYSTEMMSG::~CPacketMAP_SYSTEMMSG()
{
}


/* ========================================================================= */
/* �֐���	:CPacketMAP_SYSTEMMSG::Make										 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2007/02/10														 */
/* ========================================================================= */

void CPacketMAP_SYSTEMMSG::Make(
	LPCSTR pszMsg,								/* [in] �V�X�e�����b�Z�[�W */
	COLORREF clMsg,	/*=0*/						/* [in] �\���F */
	BOOL bSound,	/*=TRUE*/					/* [in] �\�����鎞�ɉ����Ȃ炷������ */
	int nMsgType)	/*=SYSTEMMSGTYPE_DEFAULT*/	/* [in] ���b�Z�[�W��� */
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE)	+
			 sizeof (clMsg)			+
			 sizeof (bSound)		+
			 sizeof (m_nMsgType)	+
			 (strlen (pszMsg) + 1);

	if (clMsg == 0) {
		clMsg = RGB (0, 200, 255);
	}

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_MAP;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_MAP_SYSTEMMSG;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	strcpyRenew ((LPSTR)pDataTmp, pszMsg, pDataTmp);						/* �V�X�e�����b�Z�[�W */
	CopyMemoryRenew (pDataTmp, &clMsg,		sizeof (clMsg),		pDataTmp);	/* �\���F */
	CopyMemoryRenew (pDataTmp, &bSound,		sizeof (bSound),	pDataTmp);	/* �\�����鎞�ɉ����Ȃ炷������ */
	CopyMemoryRenew (pDataTmp, &nMsgType,	sizeof (nMsgType),	pDataTmp);	/* ���b�Z�[�W��� */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* �֐���	:CPacketMAP_SYSTEMMSG::Set										 */
/* ���e		:�p�P�b�g��ݒ�													 */
/* ���t		:2007/02/10														 */
/* ========================================================================= */

PBYTE CPacketMAP_SYSTEMMSG::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	StoreRenew (m_strMsg, (LPCSTR)pDataTmp, pDataTmp);							/* �V�X�e�����b�Z�[�W */
	CopyMemoryRenew (&m_clMsg,		pDataTmp, sizeof (m_clMsg),		pDataTmp);	/* �\���F */
	CopyMemoryRenew (&m_bSound,		pDataTmp, sizeof (m_bSound),	pDataTmp);	/* �\�����鎞�ɉ����Ȃ炷������ */
	CopyMemoryRenew (&m_nMsgType,	pDataTmp, sizeof (m_nMsgType),	pDataTmp);	/* ���b�Z�[�W��� */

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2007 */
