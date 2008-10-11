/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketCHAR_SET_EFFECT.h									 */
/* ���e			:�R�}���h(�L�����n:�G�t�F�N�g���w��) ��`�t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/07/27													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketCHAR_SET_EFFECT : public CPacketBase
{
public:
			CPacketCHAR_SET_EFFECT();						/* �R���X�g���N�^ */
	virtual ~CPacketCHAR_SET_EFFECT();						/* �f�X�g���N�^ */

	void	Make	(DWORD dwCharID, DWORD dwEffectID);				/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);								/* �p�P�b�g��ݒ� */


public:
	DWORD	m_dwCharID,						/* �L����ID */
			m_dwEffectID;					/* �G�t�F�N�gID */
} CPacketCHAR_SET_EFFECT, *PCPacketCHAR_SET_EFFECT;

/* Copyright(C)URARA-works 2007 */
