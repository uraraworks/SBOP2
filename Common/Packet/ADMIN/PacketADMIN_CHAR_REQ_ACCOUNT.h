/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:PacketADMIN_CHAR_REQ_ACCOUNT.h								 */
/* ���e			:�R�}���h(�Ǘ��Ҍn:�A�J�E���g���v��) ��`�t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/06/07													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketADMIN_CHAR_REQ_ACCOUNT : public CPacketBase
{
public:
			CPacketADMIN_CHAR_REQ_ACCOUNT();			/* �R���X�g���N�^ */
	virtual ~CPacketADMIN_CHAR_REQ_ACCOUNT();			/* �f�X�g���N�^ */

	void	Make	(DWORD dwAccountID);						/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);							/* �p�P�b�g��ݒ� */


public:
	DWORD m_dwAccountID;					/* �A�J�E���gID */
} CPacketADMIN_CHAR_REQ_ACCOUNT, *PCPacketADMIN_CHAR_REQ_ACCOUNT;

/* Copyright(C)URARA-works 2008 */
