/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:PacketCHAR_TEXTEFFECT.cpp									 */
/* ���e			:�R�}���h(�L�����n:�����G�t�F�N�g�ʒm) �����t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/07/13													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketCHAR_TEXTEFFECT.h"

/* ========================================================================= */
/* �֐���	:CPacketCHAR_TEXTEFFECT::CPacketCHAR_TEXTEFFECT					 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/07/13														 */
/* ========================================================================= */

CPacketCHAR_TEXTEFFECT::CPacketCHAR_TEXTEFFECT()
{
	m_nColor	= 0;
	m_dwCharID	= 0;
	m_dwData	= -1;
}


/* ========================================================================= */
/* �֐���	:CPacketCHAR_TEXTEFFECT::~CPacketCHAR_TEXTEFFECT				 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/07/13														 */
/* ========================================================================= */

CPacketCHAR_TEXTEFFECT::~CPacketCHAR_TEXTEFFECT()
{
}


/* ========================================================================= */
/* �֐���	:CPacketCHAR_TEXTEFFECT::Make									 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2008/07/13														 */
/* ========================================================================= */

void CPacketCHAR_TEXTEFFECT::Make(
	DWORD dwCharID,			/* [in] �L����ID */
	DWORD dwData,			/* [in] ���l�f�[�^ */
	int nColor,/*0*/		/* [in] �F */
	int nSize)/*0*/			/* [in] �T�C�Y */
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE)	+
			 sizeof (dwCharID)		+
			 sizeof (dwData)		+
			 sizeof (nColor)		+
			 sizeof (nSize)			+
			 1;	/* ������I�[�p */

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_CHAR;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_CHAR_TEXTEFFECT;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwCharID,	sizeof (dwCharID),	pDataTmp);	/* �L����ID */
	CopyMemoryRenew (pDataTmp, &dwData,		sizeof (dwData),	pDataTmp);	/* ���l�f�[�^ */
	CopyMemoryRenew (pDataTmp, &nColor,		sizeof (nColor),	pDataTmp);	/* �F */
	CopyMemoryRenew (pDataTmp, &nSize,		sizeof (nSize),		pDataTmp);	/* �T�C�Y */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* �֐���	:CPacketCHAR_TEXTEFFECT::Make									 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2008/07/13														 */
/* ========================================================================= */

void CPacketCHAR_TEXTEFFECT::Make(
	DWORD dwCharID,			/* [in] �L����ID */
	LPCSTR pszData,			/* [in] ������f�[�^ */
	int nColor,/*0*/		/* [in] �F */
	int nSize)/*0*/			/* [in] �T�C�Y */
{
	PBYTE pData, pDataTmp;
	DWORD dwSize, dwTmp;
	PPACKETBASE pPacketBase;

	dwTmp  = -1;
	dwSize = sizeof (PACKETBASE)	+
			 sizeof (dwCharID)		+
			 sizeof (DWORD)			+	/* ���l�f�[�^�p */
			 sizeof (nColor)		+
			 sizeof (nSize)			+
			 (strlen (pszData) + 1);

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_CHAR;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_CHAR_TEXTEFFECT;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwCharID,	sizeof (dwCharID),	pDataTmp);	/* �L����ID */
	CopyMemoryRenew (pDataTmp, &dwTmp,		sizeof (dwTmp),		pDataTmp);	/* ���l�f�[�^ */
	CopyMemoryRenew (pDataTmp, &nColor,		sizeof (nColor),	pDataTmp);	/* �F */
	CopyMemoryRenew (pDataTmp, &nSize,		sizeof (nSize),		pDataTmp);	/* �T�C�Y */
	strcpyRenew ((LPSTR)pDataTmp, pszData, pDataTmp);						/* ������f�[�^ */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* �֐���	:CPacketCHAR_TEXTEFFECT::Set									 */
/* ���e		:�p�P�b�g��ݒ�													 */
/* ���t		:2008/07/13														 */
/* ========================================================================= */

PBYTE CPacketCHAR_TEXTEFFECT::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwCharID,	pDataTmp, sizeof (m_dwCharID),	pDataTmp);	/* �L����ID */
	CopyMemoryRenew (&m_dwData,		pDataTmp, sizeof (m_dwData),	pDataTmp);	/* ���l�f�[�^ */
	CopyMemoryRenew (&m_nColor,		pDataTmp, sizeof (m_nColor),	pDataTmp);	/* �F */
	CopyMemoryRenew (&m_nSize,		pDataTmp, sizeof (m_nSize),		pDataTmp);	/* �T�C�Y */
	StoreRenew (m_strData, (LPCSTR)pDataTmp, pDataTmp);							/* ������f�[�^ */

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2008 */
