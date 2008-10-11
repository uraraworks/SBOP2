/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:PacketVERSION_REQ_FILE.cpp									 */
/* ���e			:�R�}���h(�o�[�W�����n:�t�@�C���v��) �����t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/01/20													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketVERSION_REQ_FILE.h"

/* ========================================================================= */
/* �֐���	:CPacketVERSION_REQ_FILE::CPacketVERSION_REQ_FILE				 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/01/20														 */
/* ========================================================================= */

CPacketVERSION_REQ_FILE::CPacketVERSION_REQ_FILE()
{
	m_dwOffset	= 0;
	m_dwReqSize	= 0;
}


/* ========================================================================= */
/* �֐���	:CPacketVERSION_REQ_FILE::~CPacketVERSION_REQ_FILE				 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/01/20														 */
/* ========================================================================= */

CPacketVERSION_REQ_FILE::~CPacketVERSION_REQ_FILE()
{
}


/* ========================================================================= */
/* �֐���	:CPacketVERSION_REQ_FILE::Make									 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2008/01/20														 */
/* ========================================================================= */

void CPacketVERSION_REQ_FILE::Make(
	DWORD dwOffset,			/* [in] �J�n�I�t�Z�b�g */
	DWORD dwReqSize,		/* [in] �v���T�C�Y */
	LPCSTR pszFileName)		/* [in] �o�[�W�����R�[�h */
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE)	+
			 sizeof (dwOffset)		+
			 sizeof (dwReqSize)		+
			 (strlen (pszFileName) + 1);

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_VERSION;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_VERSION_REQ_FILE;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwOffset,	sizeof (dwOffset),	pDataTmp);	/* �J�n�I�t�Z�b�g */
	CopyMemoryRenew (pDataTmp, &dwReqSize,	sizeof (dwReqSize),	pDataTmp);	/* �v���T�C�Y */
	strcpyRenew ((LPSTR)pDataTmp, pszFileName, pDataTmp);					/* ���΃p�X�t���t�@�C���� */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* �֐���	:CPacketVERSION_REQ_FILE::Set									 */
/* ���e		:�p�P�b�g��ݒ�													 */
/* ���t		:2008/01/20														 */
/* ========================================================================= */

PBYTE CPacketVERSION_REQ_FILE::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	CopyMemoryRenew (&m_dwOffset,	pDataTmp, sizeof (m_dwOffset),	pDataTmp);	/* �J�n�I�t�Z�b�g */
	CopyMemoryRenew (&m_dwReqSize,	pDataTmp, sizeof (m_dwReqSize),	pDataTmp);	/* �v���T�C�Y */
	StoreRenew (m_strFileName,	(LPCSTR)pDataTmp, pDataTmp);					/* ���΃p�X�t���t�@�C���� */

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2008 */
