/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* �t�@�C����	:PacketCONNECT_RES_PLAY.h									 */
/* ���e			:�R�}���h(�ڑ��n:�Q�[���J�n����) ��`�t�@�C��				 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2006/12/31													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* �萔�̒�`																 */
/* ========================================================================= */

enum {
	PLAYRES_NONE = 0,
	PLAYRES_OK,						/* ��薳�� */
	PLAYRES_ADMINLEVEL_ALL,			/* �Ǘ���(�S����) */
};


/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketCONNECT_RES_PLAY : public CPacketBase
{
public:
			CPacketCONNECT_RES_PLAY();					/* �R���X�g���N�^ */
	virtual ~CPacketCONNECT_RES_PLAY();					/* �f�X�g���N�^ */

	void	Make	(int nResult);								/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);							/* �p�P�b�g��ݒ� */


public:
	int		m_nResult;					/* ���� */
} CPacketCONNECT_RES_PLAY, *PCPacketCONNECT_RES_PLAY;

/* Copyright(C)URARA-works 2006 */
