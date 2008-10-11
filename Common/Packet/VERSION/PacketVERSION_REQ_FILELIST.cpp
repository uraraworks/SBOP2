/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:PacketVERSION_REQ_FILELIST.cpp								 */
/* ���e			:�R�}���h(�o�[�W�����n:�t�@�C�����X�g�v��) �����t�@�C��		 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/03/08													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketVERSION_REQ_FILELIST.h"

/* ========================================================================= */
/* �֐���	:CPacketVERSION_REQ_FILELIST::CPacketVERSION_REQ_FILELIST		 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/03/08														 */
/* ========================================================================= */

CPacketVERSION_REQ_FILELIST::CPacketVERSION_REQ_FILELIST()
{
}


/* ========================================================================= */
/* �֐���	:CPacketVERSION_REQ_FILELIST::~CPacketVERSION_REQ_FILELIST		 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/03/08														 */
/* ========================================================================= */

CPacketVERSION_REQ_FILELIST::~CPacketVERSION_REQ_FILELIST()
{
}


/* ========================================================================= */
/* �֐���	:CPacketVERSION_REQ_FILELIST::Make								 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2008/03/08														 */
/* ========================================================================= */

void CPacketVERSION_REQ_FILELIST::Make(void)
{
	PBYTE pData;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE);

	pData = ZeroNew (dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_VERSION;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_VERSION_REQ_FILELIST;

	RenewPacket (pData, dwSize);
}

/* Copyright(C)URARA-works 2008 */
