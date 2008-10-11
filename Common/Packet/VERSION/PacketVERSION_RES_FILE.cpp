/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:PacketVERSION_RES_FILE.cpp									 */
/* ���e			:�R�}���h(�o�[�W�����n:�t�@�C���v��) �����t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/01/20													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketVERSION_RES_FILE.h"

/* ========================================================================= */
/* �֐���	:CPacketVERSION_RES_FILE::CPacketVERSION_RES_FILE				 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/01/20														 */
/* ========================================================================= */

CPacketVERSION_RES_FILE::CPacketVERSION_RES_FILE()
{
	m_dwOffset	= 0;
	m_dwSize	= 0;
	m_pFileData	= NULL;
}


/* ========================================================================= */
/* �֐���	:CPacketVERSION_RES_FILE::~CPacketVERSION_RES_FILE				 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/01/20														 */
/* ========================================================================= */

CPacketVERSION_RES_FILE::~CPacketVERSION_RES_FILE()
{
	SAFE_DELETE_ARRAY (m_pFileData);
}


/* ========================================================================= */
/* �֐���	:CPacketVERSION_RES_FILE::Make									 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2008/01/20														 */
/* ========================================================================= */

void CPacketVERSION_RES_FILE::Make(
	DWORD dwOffset,			/* [in] �J�n�I�t�Z�b�g */
	DWORD dwDataSize,		/* [in] �T�C�Y */
	LPCSTR pszFileName,		/* [in] �o�[�W�����R�[�h */
	PBYTE pFileData)		/* [in] �t�@�C���f�[�^ */
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE)		+
			 sizeof (dwOffset)			+
			 sizeof (dwDataSize)		+
			 (strlen (pszFileName) + 1)	+
			 dwDataSize;

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_VERSION;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_VERSION_RES_FILE;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	CopyMemoryRenew (pDataTmp, &dwOffset,	sizeof (dwOffset),	pDataTmp);	/* �J�n�I�t�Z�b�g */
	CopyMemoryRenew (pDataTmp, &dwDataSize,	sizeof (dwSize),	pDataTmp);	/* �T�C�Y */
	strcpyRenew ((LPSTR)pDataTmp, pszFileName, pDataTmp);					/* ���΃p�X�t���t�@�C���� */
	CopyMemoryRenew (pDataTmp, pFileData,	dwDataSize,			pDataTmp);	/* �t�@�C���f�[�^ */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* �֐���	:CPacketVERSION_RES_FILE::Set									 */
/* ���e		:�p�P�b�g��ݒ�													 */
/* ���t		:2008/01/20														 */
/* ========================================================================= */

PBYTE CPacketVERSION_RES_FILE::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	SAFE_DELETE_ARRAY (m_pFileData);
	CopyMemoryRenew (&m_dwOffset,	pDataTmp, sizeof (m_dwOffset),	pDataTmp);	/* �J�n�I�t�Z�b�g */
	CopyMemoryRenew (&m_dwSize,		pDataTmp, sizeof (m_dwSize),	pDataTmp);	/* �T�C�Y */
	StoreRenew (m_strFileName,	(LPCSTR)pDataTmp, pDataTmp);					/* ���΃p�X�t���t�@�C���� */

	m_pFileData = new BYTE[m_dwSize];
	CopyMemoryRenew (m_pFileData, pDataTmp, m_dwSize, pDataTmp);				/* �t�@�C���f�[�^ */

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2008 */
