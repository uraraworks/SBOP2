/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:PacketADMIN_CHAR_RENEWSTATUS.cpp							 */
/* ���e			:�R�}���h(�Ǘ��Ҍn:�X�e�[�^�X���X�V) �����t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/07/12													 */
/* ========================================================================= */

#include "StdAfx.h"
#include "InfoCharBase.h"
#include "Command.h"
#include "PacketADMIN_CHAR_RENEWSTATUS.h"

/* ========================================================================= */
/* �֐���	:CPacketADMIN_CHAR_RENEWSTATUS::CPacketADMIN_CHAR_RENEWSTATUS	 */
/* ���e		:�R���X�g���N�^													 */
/* ���t		:2008/07/12														 */
/* ========================================================================= */

CPacketADMIN_CHAR_RENEWSTATUS::CPacketADMIN_CHAR_RENEWSTATUS()
{
	m_byCmdMain	= SBOCOMMANDID_MAIN_ADMIN;
	m_byCmdSub	= SBOCOMMANDID_SUB_ADMIN_CHAR_RENEWSTATUS;
}


/* ========================================================================= */
/* �֐���	:CPacketADMIN_CHAR_RENEWSTATUS::~CPacketADMIN_CHAR_RENEWSTATUS	 */
/* ���e		:�f�X�g���N�^													 */
/* ���t		:2008/07/12														 */
/* ========================================================================= */

CPacketADMIN_CHAR_RENEWSTATUS::~CPacketADMIN_CHAR_RENEWSTATUS()
{
}

/* Copyright(C)URARA-works 2008 */
