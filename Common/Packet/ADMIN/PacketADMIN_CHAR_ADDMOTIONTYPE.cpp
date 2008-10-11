/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:PacketADMIN_CHAR_ADDMOTIONTYPE.cpp							 */
/* ���e			:�R�}���h(�Ǘ��Ҍn:�L�������[�V������ʏ��̒ǉ�) �����t�@�C�� */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/06/09													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "Command.h"
#include "PacketADMIN_CHAR_ADDMOTIONTYPE.h"

/* ========================================================================= */
/* �֐���	:CPacketADMIN_CHAR_ADDMOTIONTYPE::CPacketADMIN_CHAR_ADDMOTIONTYPE */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/06/09														 */
/* ========================================================================= */

CPacketADMIN_CHAR_ADDMOTIONTYPE::CPacketADMIN_CHAR_ADDMOTIONTYPE()
{
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_CHAR_ADDMOTIONTYPE::~CPacketADMIN_CHAR_ADDMOTIONTYPE */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/06/09														 */
/* ========================================================================= */

CPacketADMIN_CHAR_ADDMOTIONTYPE::~CPacketADMIN_CHAR_ADDMOTIONTYPE()
{
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_CHAR_ADDMOTIONTYPE::Make							 */
/* ���e		:�p�P�b�g���쐬													 */
/* ���t		:2008/06/09														 */
/* ========================================================================= */

void CPacketADMIN_CHAR_ADDMOTIONTYPE::Make(void)
{
	PBYTE pData, pDataTmp;
	DWORD dwSize;
	PPACKETBASE pPacketBase;

	dwSize = sizeof (PACKETBASE);

	pData = new BYTE[dwSize];
	ZeroMemory (pData, dwSize);
	pPacketBase = (PPACKETBASE)pData;

	pPacketBase->byCmdMain	= SBOCOMMANDID_MAIN_ADMIN;
	pPacketBase->byCmdSub	= SBOCOMMANDID_SUB_ADMIN_CHAR_ADDMOTIONTYPE;

	pDataTmp = (PBYTE)(pPacketBase + 1);

	RenewPacket (pData, dwSize);
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_CHAR_ADDMOTIONTYPE::Set							 */
/* ���e		:�p�P�b�g��ݒ�													 */
/* ���t		:2008/06/09														 */
/* ========================================================================= */

PBYTE CPacketADMIN_CHAR_ADDMOTIONTYPE::Set(PBYTE pPacket)
{
	PBYTE pRet, pDataTmp;

	pRet		= pPacket;
	pDataTmp	= CPacketBase::Set (pPacket);

	pRet = pDataTmp;
	return pRet;
}

/* Copyright(C)URARA-works 2008 */
