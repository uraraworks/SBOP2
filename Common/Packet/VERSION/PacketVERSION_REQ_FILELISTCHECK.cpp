/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketVERSION_REQ_FILELISTCHECK.cpp						 */
/* ���e			:�R�}���h(�o�[�W�����n:�t�@�C�����X�g�`�F�b�N�v��) �����t�@�C��	 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/08/28													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketVERSION_REQ_FILELISTCHECK.h"

/* ========================================================================= */
/* �֐���	:CPacketVERSION_REQ_FILELISTCHECK::CPacketVERSION_REQ_FILELISTCHECK */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/08/28														 */
/* ========================================================================= */

CPacketVERSION_REQ_FILELISTCHECK::CPacketVERSION_REQ_FILELISTCHECK()
{
}


/* ========================================================================= */
/* �֐���	:CPacketVERSION_REQ_FILELISTCHECK::~CPacketVERSION_REQ_FILELISTCHECK */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/08/28														 */
/* ========================================================================= */

CPacketVERSION_REQ_FILELISTCHECK::~CPacketVERSION_REQ_FILELISTCHECK()
{
}


/* ========================================================================= */
/* �֐���	:CPacketVERSION_REQ_FILELISTCHECK::Make							 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2007/08/28														 */
/* ========================================================================= */

void CPacketVERSION_REQ_FILELISTCHECK::Make(
	LPCSTR pszFileListHash)		/* [in] �t�@�C�����X�g�n�b�V�� */
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE) +
			 (strlen (pszFileListHash) + 1);

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_VERSION;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_VERSION_REQ_FILELISTCHECK;

	pDataTmp = (PBYTE)(pPacketBase + 1);
	strcpyRenew ((LPSTR)pDataTmp, pszFileListHash, pDataTmp);		/* �t�@�C�����X�g�n�b�V�� */

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* �֐���	:CPacketVERSION_REQ_FILELISTCHECK::Set							 */
/* ���e		:�p�P�b�g��ݒ�													 */
/* ���t		:2007/08/28														 */
/* ========================================================================= */

PBYTE CPacketVERSION_REQ_FILELISTCHECK::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	StoreRenew (m_strFileListHash, (LPCSTR)pDataTmp, pDataTmp);		/* �t�@�C�����X�g�n�b�V�� */

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2007 */
