/* Copyright(C)URARA-works 2008 */
/* ========================================================================= */
/* �t�@�C����	:PacketCONNECT_KEEPALIVE.h									 */
/* ���e			:�R�}���h(�ڑ��n:�����m�F�ʒm) ��`�t�@�C��					 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2008/06/21													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketCONNECT_KEEPALIVE : public CPacketBase
{
public:
			CPacketCONNECT_KEEPALIVE();					/* �R���X�g���N�^ */
	virtual ~CPacketCONNECT_KEEPALIVE();				/* �f�X�g���N�^ */

	void	Make	(DWORD dwData);								/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);							/* �p�P�b�g��ݒ� */


public:
	DWORD	m_dwData;				/* �ԐM�p�f�[�^ */
} CPacketCONNECT_KEEPALIVE, *PCPacketCONNECT_KEEPALIVE;

/* Copyright(C)URARA-works 2008 */
