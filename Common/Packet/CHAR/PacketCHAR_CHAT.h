/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketCHAR_CHAT.h											 */
/* ���e			:�R�}���h(�L�����n:�`���b�g�ʒm) ��`�t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/02/04													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketCHAR_CHAT : public CPacketBase
{
public:
			CPacketCHAR_CHAT();								/* �R���X�g���N�^ */
	virtual ~CPacketCHAR_CHAT();							/* �f�X�g���N�^ */

	void	Make	(DWORD dwCharID, LPCSTR pszChat);				/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);								/* �p�P�b�g��ݒ� */


public:
	DWORD		m_dwCharID;					/* �L����ID */
	CmyString	m_strChat;					/* �������e */
} CPacketCHAR_CHAT, *PCPacketCHAR_CHAT;

/* Copyright(C)URARA-works 2007 */
