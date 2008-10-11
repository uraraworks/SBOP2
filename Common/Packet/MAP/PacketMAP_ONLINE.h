/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C����	:PacketMAP_ONLINE.h											 */
/* ���e			:�R�}���h(�}�b�v�n:�I�����C�����ʒm) ��`�t�@�C��			 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2007/02/10													 */
/* ========================================================================= */

#pragma once

#include "PacketBase.h"

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CPacketMAP_ONLINE : public CPacketBase
{
public:
			CPacketMAP_ONLINE();						/* �R���X�g���N�^ */
	virtual ~CPacketMAP_ONLINE();						/* �f�X�g���N�^ */

	void	Make	(int nCount);								/* �p�P�b�g���쐬 */
	PBYTE	Set		(PBYTE pPacket);							/* �p�P�b�g��ݒ� */


public:
	int m_nCount;							/* �I�����C���� */
} CPacketMAP_ONLINE, *PCPacketMAP_ONLINE;

/* Copyright(C)URARA-works 2007 */
