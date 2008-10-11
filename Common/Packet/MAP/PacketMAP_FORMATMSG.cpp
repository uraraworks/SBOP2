/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:PacketMAP_FORMATMSG.cpp									 */
/* ���e			:�R�}���h(�}�b�v�n:�t�H�[�}�b�g���b�Z�[�W�ʒm) �����t�@�C��	 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/07/03													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketMAP_FORMATMSG.h"

/* ========================================================================= */
/* �֐���	:CPacketMAP_FORMATMSG::CPacketMAP_FORMATMSG						 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/07/03														 */
/* ========================================================================= */

CPacketMAP_FORMATMSG::CPacketMAP_FORMATMSG()
{
	m_nMsgType	= FORMATMSGTYPE_DEFAULT;
	m_bSound	= TRUE;
	m_dwMsgID	= 0;
	m_dwPara1	= 0;
	m_dwPara2	= 0;
	m_clMsg		= 0;
}


/* ========================================================================= */
/* �֐���	:CPacketMAP_FORMATMSG::~CPacketMAP_FORMATMSG					 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/07/03														 */
/* ========================================================================= */

CPacketMAP_FORMATMSG::~CPacketMAP_FORMATMSG()
{
}


/* ========================================================================= */
/* �֐���	:CPacketMAP_FORMATMSG::Make										 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2008/07/03														 */
/* ========================================================================= */

void CPacketMAP_FORMATMSG::Make(
	DWORD dwMsgID,				/* [in] ���b�Z�[�WID */
	DWORD dwPara1,	/*=0*/		/* [in] �p�����[�^1 */
	DWORD dwPara2,	/*=0*/		/* [in] �p�����[�^2 */
	COLORREF clMsg,	/*=0*/		/* [in] �\���F */
	BOOL bSound,	/*=TRUE*/	/* [in] �\�����鎞�ɉ����Ȃ炷������ */
	int nMsgType)	/*=FORMATMSGTYPE_DEFAULT*/	/* [in] ���b�Z�[�W��� */
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE)	+
			 sizeof (dwMsgID)		+
			 sizeof (dwPara1)		+
			 sizeof (dwPara2)		+
			 sizeof (clMsg)			+
			 sizeof (bSound)		+
			 sizeof (m_nMsgType);

	if (clMsg == 0) {
		clMsg = RGB (0, 200, 255);
	}

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_MAP;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_MAP_FORMATMSG;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwMsgID,	sizeof (dwMsgID),	pDataTmp);	/* ���b�Z�[�WID */
	CopyMemoryRenew (pDataTmp, &dwPara1,	sizeof (dwPara1),	pDataTmp);	/* �p�����[�^1 */
	CopyMemoryRenew (pDataTmp, &dwPara2,	sizeof (dwPara2),	pDataTmp);	/* �p�����[�^2 */
	CopyMemoryRenew (pDataTmp, &clMsg,		sizeof (clMsg),		pDataTmp);	/* �\���F */
	CopyMemoryRenew (pDataTmp, &bSound,		sizeof (bSound),	pDataTmp);	/* �\�����鎞�ɉ����Ȃ炷������ */
	CopyMemoryRenew (pDataTmp, &nMsgType,	sizeof (nMsgType),	pDataTmp);	/* ���b�Z�[�W��� */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* �֐���	:CPacketMAP_FORMATMSG::Set										 */
/* ���e		:�p�P�b�g��ݒ�													 */
/* ���t		:2008/07/03														 */
/* ========================================================================= */

PBYTE CPacketMAP_FORMATMSG::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwMsgID,	pDataTmp, sizeof (m_dwMsgID),	pDataTmp);	/* ���b�Z�[�WID */
	CopyMemoryRenew (&m_dwPara1,	pDataTmp, sizeof (m_dwPara1),	pDataTmp);	/* �p�����[�^1 */
	CopyMemoryRenew (&m_dwPara2,	pDataTmp, sizeof (m_dwPara2),	pDataTmp);	/* �p�����[�^2 */
	CopyMemoryRenew (&m_clMsg,		pDataTmp, sizeof (m_clMsg),		pDataTmp);	/* �\���F */
	CopyMemoryRenew (&m_bSound,		pDataTmp, sizeof (m_bSound),	pDataTmp);	/* �\�����鎞�ɉ����Ȃ炷������ */
	CopyMemoryRenew (&m_nMsgType,	pDataTmp, sizeof (m_nMsgType),	pDataTmp);	/* ���b�Z�[�W��� */

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2008 */
