/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:PacketADMIN_CHAR_RES_ACCOUNT.h								 */
/* ���e			:�R�}���h(�Ǘ��Ҍn:�A�J�E���g��񉞓�) ��`�t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/06/07													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

class CInfoAccount;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketADMIN_CHAR_RES_ACCOUNT : public CPacketBase
{
public:
			CPacketADMIN_CHAR_RES_ACCOUNT();			/* �R���X�g���N�^ */
	virtual ~CPacketADMIN_CHAR_RES_ACCOUNT();			/* �f�X�g���N�^ */

	void	Make	(CInfoAccount *pInfoAccount);				/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);							/* �p�P�b�g��ݒ� */


public:
	DWORD		m_dwAccountID;				/* �A�J�E���gID */
	CmyString	m_strAccount,				/* �A�J�E���g */
				m_strPassword;				/* �p�X���[�h */
} CPacketADMIN_RES_ACCOUNT, *PCPacketADMIN_RES_ACCOUNT;

/* Copyright(C)URARA-works 2008 */
