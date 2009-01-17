/* Copyright(C)URARA-works 2009 */
/* ========================================================================= */
/* �t�@�C����	:PacketADMIN_ACCOUNT_REQ_ADD.h								 */
/* ���e			:�R�}���h(�Ǘ��Ҍn:�A�J�E���g�̒ǉ��v��) ��`�t�@�C��		 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2009/01/14													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketADMIN_ACCOUNT_REQ_ADD : public CPacketBase
{
public:
			CPacketADMIN_ACCOUNT_REQ_ADD();				/* �R���X�g���N�^ */
	virtual ~CPacketADMIN_ACCOUNT_REQ_ADD();			/* �f�X�g���N�^ */

	void	Make	(LPCSTR pszAccount, LPCSTR pszPassword);	/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);							/* �p�P�b�g��ݒ� */


public:
	CmyString	m_strAccount,				/* �A�J�E���g */
				m_strPassword;				/* �p�X���[�h */
} CPacketADMIN_ACCOUNT_REQ_ADD, *PCPacketADMIN_ACCOUNT_REQ_ADD;

/* Copyright(C)URARA-works 2009 */
