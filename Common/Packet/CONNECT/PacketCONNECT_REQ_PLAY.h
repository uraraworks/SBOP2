/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* �t�@�C����	:PacketCONNECT_REQ_PLAY.h									 */
/* ���e			:�R�}���h(�ڑ��n:�Q�[���J�n�v��) ��`�t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2006/12/31													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketCONNECT_REQ_PLAY : public CPacketBase
{
public:
			CPacketCONNECT_REQ_PLAY();					/* �R���X�g���N�^ */
	virtual ~CPacketCONNECT_REQ_PLAY();					/* �f�X�g���N�^ */

	void	Make	(DWORD dwAccountID, DWORD dwCharID);		/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);							/* �p�P�b�g��ݒ� */


public:
	DWORD	m_dwAccountID,						/* �A�J�E���gID */
			m_dwCharID;							/* �L����ID */
} CPacketCONNECT_REQ_PLAY, *PCPacketCONNECT_REQ_PLAY;

/* Copyright(C)URARA-works 2006 */
