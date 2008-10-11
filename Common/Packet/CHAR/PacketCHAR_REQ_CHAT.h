/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketCHAR_REQ_CHAT.h										 */
/* ���e			:�R�}���h(�L�����n:�`���b�g�v��) ��`�t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/02/04													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* �萔��`																	 */
/* ========================================================================= */

enum {
	CHATTYPE_NORMAL = 0,							/* �ʏ� */
	CHATTYPE_PARTY,									/* �p�[�e�B�`���b�g */
	CHATTYPE_MAP,									/* �}�b�v������ */
	CHATTYPE_ADMIN,									/* ��ĕ��� */
};


/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketCHAR_REQ_CHAT : public CPacketBase
{
public:
			CPacketCHAR_REQ_CHAT();							/* �R���X�g���N�^ */
	virtual ~CPacketCHAR_REQ_CHAT();						/* �f�X�g���N�^ */

	void	Make	(int nType, DWORD dwCharID, LPCSTR pszChat);	/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);								/* �p�P�b�g��ݒ� */


public:
	int			m_nType;					/* �`���b�g��� */
	DWORD		m_dwCharID;					/* �L����ID */
	CmyString	m_strChat;					/* �������e */
} CPacketCHAR_REQ_CHAT, *PCPacketCHAR_REQ_CHAT;

/* Copyright(C)URARA-works 2007 */
