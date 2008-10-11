/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketADMIN_SERVER_SAVEINFO.cpp							 */
/* ���e			:�R�}���h(�Ǘ��Ҍn:�T�[�o�[���ۑ�) �����t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/07/08													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketADMIN_SERVER_SAVEINFO.h"

/* ========================================================================= */
/* �֐���	:CPacketADMIN_SERVER_SAVEINFO::CPacketADMIN_SERVER_SAVEINFO		 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2007/07/08														 */
/* ========================================================================= */

CPacketADMIN_SERVER_SAVEINFO::CPacketADMIN_SERVER_SAVEINFO()
{
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_SERVER_SAVEINFO::~CPacketADMIN_SERVER_SAVEINFO	 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2007/07/08														 */
/* ========================================================================= */

CPacketADMIN_SERVER_SAVEINFO::~CPacketADMIN_SERVER_SAVEINFO()
{
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_SERVER_SAVEINFO::Make								 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2007/07/08														 */
/* ========================================================================= */

void CPacketADMIN_SERVER_SAVEINFO::Make(void)
{
	PBYTE pData;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE);

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_ADMIN;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ADMIN_SERVER_SAVEINFO;

	RenewPacket (pData, dwSize);
}

/* Copyright(C)URARA-works 2007 */
